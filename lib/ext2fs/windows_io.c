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

#གལ་སྲིད !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#གལ་སྲིད HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#གལ་སྲིད HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#གལ་སྲིད HAVE_SYS_STAT_H
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
#དེ་མིན
#define TRACE(...) do { } སྐབས (0);
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  གལ་སྲིད ((sa_nracanaa)->magic != (code)) ལོག (code)
#define EXT2_CHECK_MAGIC_RETURN(sa_nracanaa, code, ret) \
	  གལ་སྲིད ((sa_nracanaa)->magic != (code)) ལོག (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

sa_nracanaa windows_cache {
	var_na			*buf;
	unsigned long long	block;
	ཧྲིལ			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

sa_nracanaa windows_private_data {
	ཧྲིལ	magic;
	var_na	name[MAX_PATH];
	HANDLE	handle;
	var_na	dos_device[MAX_PATH];
	var_na	cf_device[MAX_PATH];
	ཧྲིལ	dev;
	ཧྲིལ	flags;
	ཧྲིལ	align;
	ཧྲིལ	access_time;
	ext2_loff_t offset;
	sa_nracanaa windows_cache cache[CACHE_SIZE];
	void	*bounce;
	sa_nracanaa struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

static ཧྲིལ fake_dos_name_for_device(sa_nracanaa windows_private_data *data)
{
	གལ་སྲིད (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ལོག 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	གལ་སྲིད (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ལོག 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ལོག 0;
}

static void remove_fake_dos_name(sa_nracanaa windows_private_data *data)
{
	གལ་སྲིད (*data->dos_device) {
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

	གལ་སྲིད (stats)
		*stats = &data->io_stats;

	ལོག retval;
}

static LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	ལོག li;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      sa_nracanaa windows_private_data *data,
			      unsigned long long block,
			      ཧྲིལ count, void *bufv)
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

	གལ་སྲིད (data->flags & IO_FLAG_FORCE_BOUNCE) {
		གལ་སྲིད (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		goto bounce_read;
	}

	གལ་སྲིད (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
		retval = GetLastError();
		goto error_out;
	}
	གལ་སྲིད ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		གལ་སྲིད (!ReadFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		གལ་སྲིད (actual != size) {
	short_read:
		གལ་སྲིད (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} དེ་མིན
			retval = EXT2_ET_SHORT_READ;
		goto error_out;
		}
	ལོག 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	སྐབས (size > 0) {
		གལ་སྲིད (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		གལ་སྲིད (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		actual = size;
		གལ་སྲིད (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ལོག 0;

error_out:
	གལ་སྲིད (actual >= 0 && actual < size)
		memset((var_na *) buf+actual, 0, size-actual);
	གལ་སྲིད (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	ལོག retval;
}

static errcode_t raw_write_blk(io_channel channel,
			       sa_nracanaa windows_private_data *data,
			       unsigned long long block,
			       ཧྲིལ count, const void *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	const unsigned var_na *buf = bufv;

	གལ་སྲིད (count == 1)
		size = channel->block_size;
	དེ་མིན {
		གལ་སྲིད (count < 0)
			size = -count;
		དེ་མིན
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	གལ་སྲིད (data->flags & IO_FLAG_FORCE_BOUNCE) {
		གལ་སྲིད (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
	goto bounce_write;
	}

		གལ་སྲིད (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
		goto error_out;
	}

	SetLastError(0);

	གལ་སྲིད ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		གལ་སྲིད (!WriteFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		གལ་སྲིད (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		ལོག 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	སྐབས (size > 0) {
		གལ་སྲིད (size < channel->block_size) {
			གལ་སྲིད (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
				retval = GetLastError();
				goto error_out;
			}
			གལ་སྲིད (actual != channel->block_size) {
				གལ་སྲིད (actual < 0) {
					retval = GetLastError();
					goto error_out;
				}
				memset((var_na *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		གལ་སྲིད (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		གལ་སྲིད (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		གལ་སྲིད (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		གལ་སྲིད (actual != channel->block_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ལོག 0;

error_out:
	གལ་སྲིད (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	ལོག retval;
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
	ཧྲིལ			i;

	data->access_time = 0;
	དོན་ལུ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		གལ་སྲིད (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		གལ་སྲིད (retval)
			ལོག retval;
	}
	གལ་སྲིད (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		གལ་སྲིད (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ལོག retval;
}

/* Free the cache buffers */
static void free_cache(sa_nracanaa windows_private_data *data)
{
	sa_nracanaa windows_cache    *cache;
	ཧྲིལ			i;

	data->access_time = 0;
	དོན་ལུ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		གལ་སྲིད (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	གལ་སྲིད (data->bounce)
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
	ཧྲིལ			i;

	unused_cache = oldest_cache = 0;
	དོན་ལུ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		གལ་སྲིད (!cache->in_use) {
			གལ་སྲིད (!unused_cache)
				unused_cache = cache;
			continue;
		}
		གལ་སྲིད (cache->block == block) {
			cache->access_time = ++data->access_time;
			ལོག cache;
		}
		གལ་སྲིད (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	གལ་སྲིད (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ལོག 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static void reuse_cache(io_channel channel, sa_nracanaa windows_private_data *data,
		 sa_nracanaa windows_cache *cache, unsigned long long block)
{
	གལ་སྲིད (cache->dirty && cache->in_use)
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
				     ཧྲིལ invalidate)
{
	sa_nracanaa windows_cache	*cache;
	errcode_t	retval, retval2;
	ཧྲིལ			i;

	retval2 = 0;
	དོན་ལུ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		གལ་སྲིད (!cache->in_use)
			continue;

		གལ་སྲིད (invalidate)
			cache->in_use = 0;

		གལ་སྲིད (!cache->dirty)
			continue;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		གལ་སྲིད (retval)
			retval2 = retval;
		དེ་མིན
			cache->dirty = 0;
	}
	ལོག retval2;
}
#endif /* NO_IO_CACHE */

static errcode_t windows_open_channel(sa_nracanaa windows_private_data *data,
				   ཧྲིལ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	གལ་སྲིད (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	གལ་སྲིད (retval)
		goto cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#གལ་སྲིད defined(O_DIRECT)
	གལ་སྲིད (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	གལ་སྲིད (ext2fs_fstat(data->dev, &st) == 0) {
		གལ་སྲིད (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		དེ་མིན
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	གལ་སྲིད ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	གལ་སྲིད (flags & IO_FLAG_RW) {
		ཧྲིལ error;
		ཧྲིལ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		གལ་སྲིད (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

	*channel = io;
	ལོག 0;

cleanup:
	གལ་སྲིད (data) {
		གལ་སྲིད (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	གལ་སྲིད (io) {
		གལ་སྲིད (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ལོག retval;
}

static DWORD windows_open_device(sa_nracanaa windows_private_data *data, ཧྲིལ open_flags)
{
	DWORD ret = 0;

	གལ་སྲིད (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	དེ་མིན གལ་སྲིད (fake_dos_name_for_device(data))
		ལོག -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#གལ་སྲིད defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#དེ་མིན
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, NULL, OPEN_EXISTING,
				  flags_and_attributes, NULL);

	གལ་སྲིད (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		goto invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	གལ་སྲིད (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		goto osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ལོག 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ལོག ret;
}

static sa_nracanaa windows_private_data *init_private_data(const var_na *name, ཧྲིལ flags)
{
	sa_nracanaa windows_private_data *data = NULL;

	གལ་སྲིད (ext2fs_get_mem(parimaa_na(sa_nracanaa windows_private_data), &data))
		ལོག NULL;

	memset(data, 0, parimaa_na(sa_nracanaa windows_private_data));
	strncpy(data->name, name, parimaa_na(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	ལོག data;
}

static errcode_t windows_open(const var_na *name, ཧྲིལ flags, io_channel *channel)
{
	ཧྲིལ open_flags;
	sa_nracanaa windows_private_data *data;

	གལ་སྲིད (name == 0)
		ལོག EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	གལ་སྲིད (!data)
		ལོག EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	གལ་སྲིད (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#གལ་སྲིད defined(O_DIRECT)
	གལ་སྲིད (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	གལ་སྲིད (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ལོག EXT2_ET_BAD_DEVICE_NAME;
	}

	ལོག windows_open_channel(data, flags, channel, windows_io_manager);
}

static errcode_t windows_close(io_channel channel)
{
	sa_nracanaa windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	གལ་སྲིད (--channel->refcount > 0)
		ལོག 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	གལ་སྲིད (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	གལ་སྲིད (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ལོག retval;
}

static errcode_t windows_set_blksize(io_channel channel, ཧྲིལ blksize)
{
	sa_nracanaa windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	གལ་སྲིད (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		གལ་སྲིད ((retval = flush_cached_blocks(channel, data, 0)))
			ལོག retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		གལ་སྲིད ((retval = alloc_cache(channel, data)))
			ལོག retval;
	}
	ལོག 0;
}

static errcode_t windows_read_blk64(io_channel channel, unsigned long long block, ཧྲིལ count, void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	var_na		*cp;
	ཧྲིལ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ལོག raw_read_blk(channel, data, block, count, buf);
#དེ་མིན
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	གལ་སྲིད (count < 0 || count > WRITE_DIRECT_SIZE) {
		གལ་སྲིད ((retval = flush_cached_blocks(channel, data, 0)))
			ལོག retval;
		ལོག raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	སྐབས (count > 0) {
		/* If it's in the cache, use it! */
		གལ་སྲིད ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}
		གལ་སྲིད (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			གལ་སྲིད ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ལོག retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ལོག 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		དོན་ལུ (i=1; i < count; i++)
			གལ་སྲིད (find_cached_block(data, block+i, &reuse[i]))
				bha_nga;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		གལ་སྲིད ((retval = raw_read_blk(channel, data, block, i, cp)))
			ལོག retval;

		/* Save the results in the cache */
		དོན་ལུ (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ལོག 0;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       ཧྲིལ count, void *buf)
{
	ལོག windows_read_blk64(channel, block, count, buf);
}

static errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				ཧྲིལ count, const void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	const var_na	*cp;
	ཧྲིལ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ལོག raw_write_blk(channel, data, block, count, buf);
#དེ་མིན
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	གལ་སྲིད (count < 0 || count > WRITE_DIRECT_SIZE) {
		གལ་སྲིད ((retval = flush_cached_blocks(channel, data, 1)))
			ལོག retval;
		ལོག raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	གལ་སྲིད (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	སྐབས (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		གལ་སྲིད (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		གལ་སྲིད (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	ལོག retval;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	ལོག EXT2_ET_OP_NOT_SUPPORTED;
}

static errcode_t windows_write_blk(io_channel channel, unsigned long block,
				ཧྲིལ count, const void *buf)
{
	ལོག windows_write_blk64(channel, block, count, buf);
}

static errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 ཧྲིལ size, const void *buf)
{
	ལོག EXT2_ET_UNIMPLEMENTED;
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

	ལོག retval;
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

	གལ་སྲིད (!strcmp(option, "offset")) {
		གལ་སྲིད (!arg)
			ལོག EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		གལ་སྲིད (*end)
			ལོག EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		གལ་སྲིད (data->offset < 0)
			ལོག EXT2_ET_INVALID_ARGUMENT;
		ལོག 0;
	}
	ལོག EXT2_ET_INVALID_ARGUMENT;
}

static errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ལོག EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#གལ་སྲིད __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa windows_private_data *data;
	ཧྲིལ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	གལ་སྲིད (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		goto unimplemented;
	} དེ་མིན {
		/* Regular file, try to use truncate/punch/zero. */
		sa_nracanaa stat statbuf;

		གལ་སྲིད (count == 0)
			ལོག 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		གལ་སྲིད (ret)
			goto err;
		གལ་སྲིད ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			གལ་སྲིད (ret)
				goto err;
		}
		goto unimplemented;
	}
err:
	གལ་སྲིད (ret < 0) {
		གལ་སྲིད (errno == EOPNOTSUPP)
			goto unimplemented;
		ལོག errno;
	}
	ལོག 0;
unimplemented:
	ལོག EXT2_ET_UNIMPLEMENTED;
}

ཧྲིལ ext2fs_open_file(const var_na *pathname, ཧྲིལ flags, mode_t mode)
{
	flags |= O_BINARY;

	གལ་སྲིད (mode)
#གལ་སྲིད defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ལོག open64(pathname, flags, mode);
	དེ་མིན
		ལོག open64(pathname, flags);
#དེ་མིན
		ལོག open(pathname, flags, mode);
	དེ་མིན
		ལོག open(pathname, flags);
#endif
}

ཧྲིལ ext2fs_stat(const var_na *path, ext2fs_struct_stat *buf)
{
#གལ་སྲིད defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ལོག stat64(path, buf);
#དེ་མིན
	ལོག stat(path, buf);
#endif
}

ཧྲིལ ext2fs_fstat(ཧྲིལ fd, ext2fs_struct_stat *buf)
{
#གལ་སྲིད defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ལོག fstat64(fd, buf);
#དེ་མིན
	ལོག fstat(fd, buf);
#endif
}

#གལ་སྲིད __GNUC_PREREQ (4, 6)
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
