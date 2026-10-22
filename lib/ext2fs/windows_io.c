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

#නම් !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#නම් HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#නම් HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    var_na __log[256];\
		    snprintf(__log, parimaa_na(__log), __VA_ARGS__);\
		    __log[parimaa_na(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#නැතහොත්
#define TRACE(...) do { } අතරතුර (0);
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  නම් ((sa_nracanaa)->magic != (code)) ආපසු (code)
#define EXT2_CHECK_MAGIC_RETURN(sa_nracanaa, code, ret) \
	  නම් ((sa_nracanaa)->magic != (code)) ආපසු (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

sa_nracanaa windows_cache {
	var_na			*buf;
	unsigned long long	block;
	පූර්ණ_සංඛ්‍යා			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

sa_nracanaa windows_private_data {
	පූර්ණ_සංඛ්‍යා	magic;
	var_na	name[MAX_PATH];
	HANDLE	handle;
	var_na	dos_device[MAX_PATH];
	var_na	cf_device[MAX_PATH];
	පූර්ණ_සංඛ්‍යා	dev;
	පූර්ණ_සංඛ්‍යා	flags;
	පූර්ණ_සංඛ්‍යා	align;
	පූර්ණ_සංඛ්‍යා	access_time;
	ext2_loff_t offset;
	sa_nracanaa windows_cache cache[CACHE_SIZE];
	void	*bounce;
	sa_nracanaa struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

static පූර්ණ_සංඛ්‍යා fake_dos_name_for_device(sa_nracanaa windows_private_data *data)
{
	නම් (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ආපසු 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	නම් (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ආපසු 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ආපසු 0;
}

static void remove_fake_dos_name(sa_nracanaa windows_private_data *data)
{
	නම් (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

static errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	sa_nracanaa windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	නම් (stats)
		*stats = &data->io_stats;

	ආපසු retval;
}

static LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	ආපසු li;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      sa_nracanaa windows_private_data *data,
			      unsigned long long block,
			      පූර්ණ_සංඛ්‍යා count, void *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned var_na	*buf = bufv;
	ssize_t		really_read = 0;

	size = (count < 0) ? -count : count * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	නම් (data->flags & IO_FLAG_FORCE_BOUNCE) {
		නම් (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		goto bounce_read;
	}

	නම් (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
		retval = GetLastError();
		goto error_out;
	}
	නම් ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		නම් (!ReadFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		නම් (actual != size) {
	short_read:
		නම් (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} නැතහොත්
			retval = EXT2_ET_SHORT_READ;
		goto error_out;
		}
	ආපසු 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	අතරතුර (size > 0) {
		නම් (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		නම් (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		actual = size;
		නම් (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ආපසු 0;

error_out:
	නම් (actual >= 0 && actual < size)
		memset((var_na *) buf+actual, 0, size-actual);
	නම් (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	ආපසු retval;
}

static errcode_t raw_write_blk(io_channel channel,
			       sa_nracanaa windows_private_data *data,
			       unsigned long long block,
			       පූර්ණ_සංඛ්‍යා count, const void *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	const unsigned var_na *buf = bufv;

	නම් (count == 1)
		size = channel->block_size;
	නැතහොත් {
		නම් (count < 0)
			size = -count;
		නැතහොත්
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	නම් (data->flags & IO_FLAG_FORCE_BOUNCE) {
		නම් (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
	goto bounce_write;
	}

		නම් (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
		goto error_out;
	}

	SetLastError(0);

	නම් ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		නම් (!WriteFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		නම් (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		ආපසු 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	අතරතුර (size > 0) {
		නම් (size < channel->block_size) {
			නම් (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
				retval = GetLastError();
				goto error_out;
			}
			නම් (actual != channel->block_size) {
				නම් (actual < 0) {
					retval = GetLastError();
					goto error_out;
				}
				memset((var_na *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		නම් (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		නම් (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		නම් (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		නම් (actual != channel->block_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ආපසු 0;

error_out:
	නම් (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	ආපසු retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
static errcode_t alloc_cache(io_channel channel,
			     sa_nracanaa windows_private_data *data)
{
	errcode_t		retval;
	sa_nracanaa windows_cache	*cache;
	පූර්ණ_සංඛ්‍යා			i;

	data->access_time = 0;
	සඳහා (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		නම් (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		නම් (retval)
			ආපසු retval;
	}
	නම් (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		නම් (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ආපසු retval;
}

/* Free the cache buffers */
static void free_cache(sa_nracanaa windows_private_data *data)
{
	sa_nracanaa windows_cache    *cache;
	පූර්ණ_සංඛ්‍යා			i;

	data->access_time = 0;
	සඳහා (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		නම් (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	නම් (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
static sa_nracanaa windows_cache *find_cached_block(sa_nracanaa windows_private_data *data,
					    unsigned long long block,
					    sa_nracanaa windows_cache **eldest)
{
	sa_nracanaa windows_cache	*cache, *unused_cache, *oldest_cache;
	පූර්ණ_සංඛ්‍යා			i;

	unused_cache = oldest_cache = 0;
	සඳහා (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		නම් (!cache->in_use) {
			නම් (!unused_cache)
				unused_cache = cache;
			continue;
		}
		නම් (cache->block == block) {
			cache->access_time = ++data->access_time;
			ආපසු cache;
		}
		නම් (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	නම් (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ආපසු 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static void reuse_cache(io_channel channel, sa_nracanaa windows_private_data *data,
		 sa_nracanaa windows_cache *cache, unsigned long long block)
{
	නම් (cache->dirty && cache->in_use)
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
				     sa_nracanaa windows_private_data *data,
				     පූර්ණ_සංඛ්‍යා invalidate)
{
	sa_nracanaa windows_cache	*cache;
	errcode_t	retval, retval2;
	පූර්ණ_සංඛ්‍යා			i;

	retval2 = 0;
	සඳහා (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		නම් (!cache->in_use)
			continue;

		නම් (invalidate)
			cache->in_use = 0;

		නම් (!cache->dirty)
			continue;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		නම් (retval)
			retval2 = retval;
		නැතහොත්
			cache->dirty = 0;
	}
	ආපසු retval2;
}
#endif /* NO_IO_CACHE */

static errcode_t windows_open_channel(sa_nracanaa windows_private_data *data,
				   පූර්ණ_සංඛ්‍යා flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	නම් (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	නම් (retval)
		goto cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#නම් defined(O_DIRECT)
	නම් (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	නම් (ext2fs_fstat(data->dev, &st) == 0) {
		නම් (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		නැතහොත්
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	නම් ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	නම් (flags & IO_FLAG_RW) {
		පූර්ණ_සංඛ්‍යා error;
		පූර්ණ_සංඛ්‍යා readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		නම් (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

	*channel = io;
	ආපසු 0;

cleanup:
	නම් (data) {
		නම් (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	නම් (io) {
		නම් (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ආපසු retval;
}

static DWORD windows_open_device(sa_nracanaa windows_private_data *data, පූර්ණ_සංඛ්‍යා open_flags)
{
	DWORD ret = 0;

	නම් (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	නැතහොත් නම් (fake_dos_name_for_device(data))
		ආපසු -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#නම් defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#නැතහොත්
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, NULL, OPEN_EXISTING,
				  flags_and_attributes, NULL);

	නම් (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		goto invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	නම් (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		goto osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ආපසු 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ආපසු ret;
}

static sa_nracanaa windows_private_data *init_private_data(const var_na *name, පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa windows_private_data *data = NULL;

	නම් (ext2fs_get_mem(parimaa_na(sa_nracanaa windows_private_data), &data))
		ආපසු NULL;

	memset(data, 0, parimaa_na(sa_nracanaa windows_private_data));
	strncpy(data->name, name, parimaa_na(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	ආපසු data;
}

static errcode_t windows_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel)
{
	පූර්ණ_සංඛ්‍යා open_flags;
	sa_nracanaa windows_private_data *data;

	නම් (name == 0)
		ආපසු EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	නම් (!data)
		ආපසු EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	නම් (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#නම් defined(O_DIRECT)
	නම් (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	නම් (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ආපසු EXT2_ET_BAD_DEVICE_NAME;
	}

	ආපසු windows_open_channel(data, flags, channel, windows_io_manager);
}

static errcode_t windows_close(io_channel channel)
{
	sa_nracanaa windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	නම් (--channel->refcount > 0)
		ආපසු 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	නම් (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	නම් (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ආපසු retval;
}

static errcode_t windows_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize)
{
	sa_nracanaa windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	නම් (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		නම් ((retval = flush_cached_blocks(channel, data, 0)))
			ආපසු retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		නම් ((retval = alloc_cache(channel, data)))
			ආපසු retval;
	}
	ආපසු 0;
}

static errcode_t windows_read_blk64(io_channel channel, unsigned long long block, පූර්ණ_සංඛ්‍යා count, void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	var_na		*cp;
	පූර්ණ_සංඛ්‍යා		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ආපසු raw_read_blk(channel, data, block, count, buf);
#නැතහොත්
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	නම් (count < 0 || count > WRITE_DIRECT_SIZE) {
		නම් ((retval = flush_cached_blocks(channel, data, 0)))
			ආපසු retval;
		ආපසු raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	අතරතුර (count > 0) {
		/* If it's in the cache, use it! */
		නම් ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}
		නම් (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			නම් ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ආපසු retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ආපසු 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		සඳහා (i=1; i < count; i++)
			නම් (find_cached_block(data, block+i, &reuse[i]))
				bha_nga;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		නම් ((retval = raw_read_blk(channel, data, block, i, cp)))
			ආපසු retval;

		/* Save the results in the cache */
		සඳහා (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ආපසු 0;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *buf)
{
	ආපසු windows_read_blk64(channel, block, count, buf);
}

static errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	const var_na	*cp;
	පූර්ණ_සංඛ්‍යා		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ආපසු raw_write_blk(channel, data, block, count, buf);
#නැතහොත්
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	නම් (count < 0 || count > WRITE_DIRECT_SIZE) {
		නම් ((retval = flush_cached_blocks(channel, data, 1)))
			ආපසු retval;
		ආපසු raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	නම් (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	අතරතුර (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		නම් (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		නම් (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	ආපසු retval;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	ආපසු EXT2_ET_OP_NOT_SUPPORTED;
}

static errcode_t windows_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	ආපසු windows_write_blk64(channel, block, count, buf);
}

static errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 පූර්ණ_සංඛ්‍යා size, const void *buf)
{
	ආපසු EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
static errcode_t windows_flush(io_channel channel)
{
	sa_nracanaa windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	ආපසු retval;
}

static errcode_t windows_set_option(io_channel channel, const var_na *option,
				 const var_na *arg)
{
	sa_nracanaa windows_private_data *data;
	unsigned long long tmp;
	var_na *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	නම් (!strcmp(option, "offset")) {
		නම් (!arg)
			ආපසු EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		නම් (*end)
			ආපසු EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		නම් (data->offset < 0)
			ආපසු EXT2_ET_INVALID_ARGUMENT;
		ආපසු 0;
	}
	ආපසු EXT2_ET_INVALID_ARGUMENT;
}

static errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ආපසු EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#නම් __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa windows_private_data *data;
	පූර්ණ_සංඛ්‍යා		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	නම් (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		goto unimplemented;
	} නැතහොත් {
		/* Regular file, try to use truncate/punch/zero. */
		sa_nracanaa stat statbuf;

		නම් (count == 0)
			ආපසු 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		නම් (ret)
			goto err;
		නම් ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			නම් (ret)
				goto err;
		}
		goto unimplemented;
	}
err:
	නම් (ret < 0) {
		නම් (errno == EOPNOTSUPP)
			goto unimplemented;
		ආපසු errno;
	}
	ආපසු 0;
unimplemented:
	ආපසු EXT2_ET_UNIMPLEMENTED;
}

පූර්ණ_සංඛ්‍යා ext2fs_open_file(const var_na *pathname, පූර්ණ_සංඛ්‍යා flags, mode_t mode)
{
	flags |= O_BINARY;

	නම් (mode)
#නම් defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ආපසු open64(pathname, flags, mode);
	නැතහොත්
		ආපසු open64(pathname, flags);
#නැතහොත්
		ආපසු open(pathname, flags, mode);
	නැතහොත්
		ආපසු open(pathname, flags);
#endif
}

පූර්ණ_සංඛ්‍යා ext2fs_stat(const var_na *path, ext2fs_struct_stat *buf)
{
#නම් defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ආපසු stat64(path, buf);
#නැතහොත්
	ආපසු stat(path, buf);
#endif
}

පූර්ණ_සංඛ්‍යා ext2fs_fstat(පූර්ණ_සංඛ්‍යා fd, ext2fs_struct_stat *buf)
{
#නම් defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ආපසු fstat64(fd, buf);
#නැතහොත්
	ආපසු fstat(fd, buf);
#endif
}

#නම් __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

static sa_nracanaa struct_io_manager struct_windows_manager = {
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
