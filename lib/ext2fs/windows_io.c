/*
 * windows_io.c --- This is the Windows implementation of the I/O manager.
 *
 * Implements a one-block write-through cache.
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *	2002 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include <windows.h>
#include <winioctl.h>
#include <io.h>

#nếu !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#define _FILE_OFFSET_BITS 64
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    char __log[256];\
		    snprintf(__log, sizeof(__log), __VA_ARGS__);\
		    __log[sizeof(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#không_thì
#define TRACE(...) do { } trong_khi (0);
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  nếu ((struct)->magic != (code)) trả_về (code)
#define EXT2_CHECK_MAGIC_RETURN(struct, code, ret) \
	  nếu ((struct)->magic != (code)) trả_về (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

struct windows_cache {
	char			*buf;
	unsigned long long	block;
	số_nguyên			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

struct windows_private_data {
	số_nguyên	magic;
	char	name[MAX_PATH];
	HANDLE	handle;
	char	dos_device[MAX_PATH];
	char	cf_device[MAX_PATH];
	số_nguyên	dev;
	số_nguyên	flags;
	số_nguyên	align;
	số_nguyên	access_time;
	ext2_loff_t offset;
	struct windows_cache cache[CACHE_SIZE];
	void	*bounce;
	struct struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

static số_nguyên fake_dos_name_for_device(struct windows_private_data *data)
{
	nếu (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		trả_về 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	nếu (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		trả_về 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	trả_về 0;
}

static void remove_fake_dos_name(struct windows_private_data *data)
{
	nếu (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

static errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	struct windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	nếu (stats)
		*stats = &data->io_stats;

	trả_về retval;
}

static LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	trả_về li;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      struct windows_private_data *data,
			      unsigned long long block,
			      số_nguyên count, void *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned char	*buf = bufv;
	ssize_t		really_read = 0;

	size = (count < 0) ? -count : count * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	nếu (data->flags & IO_FLAG_FORCE_BOUNCE) {
		nếu (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		goto bounce_read;
	}

	nếu (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
		retval = GetLastError();
		goto error_out;
	}
	nếu ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		nếu (!ReadFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		nếu (actual != size) {
	short_read:
		nếu (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} không_thì
			retval = EXT2_ET_SHORT_READ;
		goto error_out;
		}
	trả_về 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	trong_khi (size > 0) {
		nếu (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		nếu (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		actual = size;
		nếu (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	trả_về 0;

error_out:
	nếu (actual >= 0 && actual < size)
		memset((char *) buf+actual, 0, size-actual);
	nếu (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	trả_về retval;
}

static errcode_t raw_write_blk(io_channel channel,
			       struct windows_private_data *data,
			       unsigned long long block,
			       số_nguyên count, const void *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	const unsigned char *buf = bufv;

	nếu (count == 1)
		size = channel->block_size;
	không_thì {
		nếu (count < 0)
			size = -count;
		không_thì
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	nếu (data->flags & IO_FLAG_FORCE_BOUNCE) {
		nếu (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
	goto bounce_write;
	}

		nếu (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
		goto error_out;
	}

	SetLastError(0);

	nếu ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		nếu (!WriteFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		nếu (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		trả_về 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	trong_khi (size > 0) {
		nếu (size < channel->block_size) {
			nếu (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
				retval = GetLastError();
				goto error_out;
			}
			nếu (actual != channel->block_size) {
				nếu (actual < 0) {
					retval = GetLastError();
					goto error_out;
				}
				memset((char *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		nếu (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		nếu (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		nếu (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		nếu (actual != channel->block_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	trả_về 0;

error_out:
	nếu (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	trả_về retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
static errcode_t alloc_cache(io_channel channel,
			     struct windows_private_data *data)
{
	errcode_t		retval;
	struct windows_cache	*cache;
	số_nguyên			i;

	data->access_time = 0;
	cho (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		nếu (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		nếu (retval)
			trả_về retval;
	}
	nếu (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		nếu (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	trả_về retval;
}

/* Free the cache buffers */
static void free_cache(struct windows_private_data *data)
{
	struct windows_cache    *cache;
	số_nguyên			i;

	data->access_time = 0;
	cho (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		nếu (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	nếu (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
static struct windows_cache *find_cached_block(struct windows_private_data *data,
					    unsigned long long block,
					    struct windows_cache **eldest)
{
	struct windows_cache	*cache, *unused_cache, *oldest_cache;
	số_nguyên			i;

	unused_cache = oldest_cache = 0;
	cho (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		nếu (!cache->in_use) {
			nếu (!unused_cache)
				unused_cache = cache;
			continue;
		}
		nếu (cache->block == block) {
			cache->access_time = ++data->access_time;
			trả_về cache;
		}
		nếu (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	nếu (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	trả_về 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static void reuse_cache(io_channel channel, struct windows_private_data *data,
		 struct windows_cache *cache, unsigned long long block)
{
	nếu (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
static errcode_t flush_cached_blocks(io_channel channel,
				     struct windows_private_data *data,
				     số_nguyên invalidate)
{
	struct windows_cache	*cache;
	errcode_t	retval, retval2;
	số_nguyên			i;

	retval2 = 0;
	cho (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		nếu (!cache->in_use)
			continue;

		nếu (invalidate)
			cache->in_use = 0;

		nếu (!cache->dirty)
			continue;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		nếu (retval)
			retval2 = retval;
		không_thì
			cache->dirty = 0;
	}
	trả_về retval2;
}
#endif /* NO_IO_CACHE */

static errcode_t windows_open_channel(struct windows_private_data *data,
				   số_nguyên flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	nếu (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	nếu (retval)
		goto cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#nếu defined(O_DIRECT)
	nếu (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	nếu (ext2fs_fstat(data->dev, &st) == 0) {
		nếu (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		không_thì
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	nếu ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	nếu (flags & IO_FLAG_RW) {
		số_nguyên error;
		số_nguyên readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		nếu (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

	*channel = io;
	trả_về 0;

cleanup:
	nếu (data) {
		nếu (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	nếu (io) {
		nếu (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	trả_về retval;
}

static DWORD windows_open_device(struct windows_private_data *data, số_nguyên open_flags)
{
	DWORD ret = 0;

	nếu (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	không_thì nếu (fake_dos_name_for_device(data))
		trả_về -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#nếu defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#không_thì
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, NULL, OPEN_EXISTING,
				  flags_and_attributes, NULL);

	nếu (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		goto invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	nếu (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		goto osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	trả_về 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	trả_về ret;
}

static struct windows_private_data *init_private_data(const char *name, số_nguyên flags)
{
	struct windows_private_data *data = NULL;

	nếu (ext2fs_get_mem(sizeof(struct windows_private_data), &data))
		trả_về NULL;

	memset(data, 0, sizeof(struct windows_private_data));
	strncpy(data->name, name, sizeof(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	trả_về data;
}

static errcode_t windows_open(const char *name, số_nguyên flags, io_channel *channel)
{
	số_nguyên open_flags;
	struct windows_private_data *data;

	nếu (name == 0)
		trả_về EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	nếu (!data)
		trả_về EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	nếu (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#nếu defined(O_DIRECT)
	nếu (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	nếu (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		trả_về EXT2_ET_BAD_DEVICE_NAME;
	}

	trả_về windows_open_channel(data, flags, channel, windows_io_manager);
}

static errcode_t windows_close(io_channel channel)
{
	struct windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	nếu (--channel->refcount > 0)
		trả_về 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	nếu (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	nếu (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	trả_về retval;
}

static errcode_t windows_set_blksize(io_channel channel, số_nguyên blksize)
{
	struct windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	nếu (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		nếu ((retval = flush_cached_blocks(channel, data, 0)))
			trả_về retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		nếu ((retval = alloc_cache(channel, data)))
			trả_về retval;
	}
	trả_về 0;
}

static errcode_t windows_read_blk64(io_channel channel, unsigned long long block, số_nguyên count, void *buf)
{
	struct windows_private_data *data;
	struct windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	char		*cp;
	số_nguyên		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	trả_về raw_read_blk(channel, data, block, count, buf);
#không_thì
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	nếu (count < 0 || count > WRITE_DIRECT_SIZE) {
		nếu ((retval = flush_cached_blocks(channel, data, 0)))
			trả_về retval;
		trả_về raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	trong_khi (count > 0) {
		/* If it's in the cache, use it! */
		nếu ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}
		nếu (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			nếu ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				trả_về retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			trả_về 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		cho (i=1; i < count; i++)
			nếu (find_cached_block(data, block+i, &reuse[i]))
				break;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		nếu ((retval = raw_read_blk(channel, data, block, i, cp)))
			trả_về retval;

		/* Save the results in the cache */
		cho (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	trả_về 0;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       số_nguyên count, void *buf)
{
	trả_về windows_read_blk64(channel, block, count, buf);
}

static errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				số_nguyên count, const void *buf)
{
	struct windows_private_data *data;
	struct windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	const char	*cp;
	số_nguyên		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	trả_về raw_write_blk(channel, data, block, count, buf);
#không_thì
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	nếu (count < 0 || count > WRITE_DIRECT_SIZE) {
		nếu ((retval = flush_cached_blocks(channel, data, 1)))
			trả_về retval;
		trả_về raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	nếu (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	trong_khi (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		nếu (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		nếu (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	trả_về retval;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	trả_về EXT2_ET_OP_NOT_SUPPORTED;
}

static errcode_t windows_write_blk(io_channel channel, unsigned long block,
				số_nguyên count, const void *buf)
{
	trả_về windows_write_blk64(channel, block, count, buf);
}

static errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 số_nguyên size, const void *buf)
{
	trả_về EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
static errcode_t windows_flush(io_channel channel)
{
	struct windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	trả_về retval;
}

static errcode_t windows_set_option(io_channel channel, const char *option,
				 const char *arg)
{
	struct windows_private_data *data;
	unsigned long long tmp;
	char *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	nếu (!strcmp(option, "offset")) {
		nếu (!arg)
			trả_về EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		nếu (*end)
			trả_về EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		nếu (data->offset < 0)
			trả_về EXT2_ET_INVALID_ARGUMENT;
		trả_về 0;
	}
	trả_về EXT2_ET_INVALID_ARGUMENT;
}

static errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	trả_về EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#nếu __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct windows_private_data *data;
	số_nguyên		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	nếu (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		goto unimplemented;
	} không_thì {
		/* Regular file, try to use truncate/punch/zero. */
		struct stat statbuf;

		nếu (count == 0)
			trả_về 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		nếu (ret)
			goto err;
		nếu ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			nếu (ret)
				goto err;
		}
		goto unimplemented;
	}
err:
	nếu (ret < 0) {
		nếu (errno == EOPNOTSUPP)
			goto unimplemented;
		trả_về errno;
	}
	trả_về 0;
unimplemented:
	trả_về EXT2_ET_UNIMPLEMENTED;
}

số_nguyên ext2fs_open_file(const char *pathname, số_nguyên flags, mode_t mode)
{
	flags |= O_BINARY;

	nếu (mode)
#nếu defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		trả_về open64(pathname, flags, mode);
	không_thì
		trả_về open64(pathname, flags);
#không_thì
		trả_về open(pathname, flags, mode);
	không_thì
		trả_về open(pathname, flags);
#endif
}

số_nguyên ext2fs_stat(const char *path, ext2fs_struct_stat *buf)
{
#nếu defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	trả_về stat64(path, buf);
#không_thì
	trả_về stat(path, buf);
#endif
}

số_nguyên ext2fs_fstat(số_nguyên fd, ext2fs_struct_stat *buf)
{
#nếu defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	trả_về fstat64(fd, buf);
#không_thì
	trả_về fstat(fd, buf);
#endif
}

#nếu __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

static struct struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.set_blksize	= windows_set_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.set_option	= windows_set_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
