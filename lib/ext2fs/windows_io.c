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

#ຖ້າ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#ຖ້າ HAVE_SYS_STAT_H
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
#ບໍ່ດັ່ງນັ້ນ
#define TRACE(...) do { } ໃນຂະນະທີ່ (0);
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  ຖ້າ ((sa_nracanaa)->magic != (code)) ສົ່ງຄືນ (code)
#define EXT2_CHECK_MAGIC_RETURN(sa_nracanaa, code, ret) \
	  ຖ້າ ((sa_nracanaa)->magic != (code)) ສົ່ງຄືນ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

sa_nracanaa windows_cache {
	var_na			*buf;
	unsigned long long	block;
	ຈຳນວນເຕັມ			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

sa_nracanaa windows_private_data {
	ຈຳນວນເຕັມ	magic;
	var_na	name[MAX_PATH];
	HANDLE	handle;
	var_na	dos_device[MAX_PATH];
	var_na	cf_device[MAX_PATH];
	ຈຳນວນເຕັມ	dev;
	ຈຳນວນເຕັມ	flags;
	ຈຳນວນເຕັມ	align;
	ຈຳນວນເຕັມ	access_time;
	ext2_loff_t offset;
	sa_nracanaa windows_cache cache[CACHE_SIZE];
	void	*bounce;
	sa_nracanaa struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

static ຈຳນວນເຕັມ fake_dos_name_for_device(sa_nracanaa windows_private_data *data)
{
	ຖ້າ (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ສົ່ງຄືນ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	ຖ້າ (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ສົ່ງຄືນ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ສົ່ງຄືນ 0;
}

static void remove_fake_dos_name(sa_nracanaa windows_private_data *data)
{
	ຖ້າ (*data->dos_device) {
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

	ຖ້າ (stats)
		*stats = &data->io_stats;

	ສົ່ງຄືນ retval;
}

static LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	ສົ່ງຄືນ li;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      sa_nracanaa windows_private_data *data,
			      unsigned long long block,
			      ຈຳນວນເຕັມ count, void *bufv)
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

	ຖ້າ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ຖ້າ (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		goto bounce_read;
	}

	ຖ້າ (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
		retval = GetLastError();
		goto error_out;
	}
	ຖ້າ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ຖ້າ (!ReadFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		ຖ້າ (actual != size) {
	short_read:
		ຖ້າ (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} ບໍ່ດັ່ງນັ້ນ
			retval = EXT2_ET_SHORT_READ;
		goto error_out;
		}
	ສົ່ງຄືນ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ໃນຂະນະທີ່ (size > 0) {
		ຖ້າ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}
		ຖ້າ (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		actual = size;
		ຖ້າ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ສົ່ງຄືນ 0;

error_out:
	ຖ້າ (actual >= 0 && actual < size)
		memset((var_na *) buf+actual, 0, size-actual);
	ຖ້າ (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	ສົ່ງຄືນ retval;
}

static errcode_t raw_write_blk(io_channel channel,
			       sa_nracanaa windows_private_data *data,
			       unsigned long long block,
			       ຈຳນວນເຕັມ count, const void *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	const unsigned var_na *buf = bufv;

	ຖ້າ (count == 1)
		size = channel->block_size;
	ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (count < 0)
			size = -count;
		ບໍ່ດັ່ງນັ້ນ
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ຖ້າ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ຖ້າ (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
	goto bounce_write;
	}

		ຖ້າ (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
		goto error_out;
	}

	SetLastError(0);

	ຖ້າ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ຖ້າ (!WriteFile(data->handle, buf, size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		ຖ້າ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		ສົ່ງຄືນ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ໃນຂະນະທີ່ (size > 0) {
		ຖ້າ (size < channel->block_size) {
			ຖ້າ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
				retval = GetLastError();
				goto error_out;
			}
			ຖ້າ (actual != channel->block_size) {
				ຖ້າ (actual < 0) {
					retval = GetLastError();
					goto error_out;
				}
				memset((var_na *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		ຖ້າ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		ຖ້າ (!SetFilePointerEx(data->handle, make_large_integer(location), NULL, FILE_BEGIN)) {
			retval = GetLastError();
			goto error_out;
		}
		ຖ້າ (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, NULL)) {
			retval = GetLastError();
			goto error_out;
		}

		ຖ້າ (actual != channel->block_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ສົ່ງຄືນ 0;

error_out:
	ຖ້າ (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	ສົ່ງຄືນ retval;
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
	ຈຳນວນເຕັມ			i;

	data->access_time = 0;
	ສຳລັບ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ຖ້າ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}
	ຖ້າ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ຖ້າ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ສົ່ງຄືນ retval;
}

/* Free the cache buffers */
static void free_cache(sa_nracanaa windows_private_data *data)
{
	sa_nracanaa windows_cache    *cache;
	ຈຳນວນເຕັມ			i;

	data->access_time = 0;
	ສຳລັບ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ຖ້າ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ຖ້າ (data->bounce)
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
	ຈຳນວນເຕັມ			i;

	unused_cache = oldest_cache = 0;
	ສຳລັບ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ຖ້າ (!cache->in_use) {
			ຖ້າ (!unused_cache)
				unused_cache = cache;
			continue;
		}
		ຖ້າ (cache->block == block) {
			cache->access_time = ++data->access_time;
			ສົ່ງຄືນ cache;
		}
		ຖ້າ (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ຖ້າ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ສົ່ງຄືນ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static void reuse_cache(io_channel channel, sa_nracanaa windows_private_data *data,
		 sa_nracanaa windows_cache *cache, unsigned long long block)
{
	ຖ້າ (cache->dirty && cache->in_use)
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
				     ຈຳນວນເຕັມ invalidate)
{
	sa_nracanaa windows_cache	*cache;
	errcode_t	retval, retval2;
	ຈຳນວນເຕັມ			i;

	retval2 = 0;
	ສຳລັບ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ຖ້າ (!cache->in_use)
			continue;

		ຖ້າ (invalidate)
			cache->in_use = 0;

		ຖ້າ (!cache->dirty)
			continue;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		ຖ້າ (retval)
			retval2 = retval;
		ບໍ່ດັ່ງນັ້ນ
			cache->dirty = 0;
	}
	ສົ່ງຄືນ retval2;
}
#endif /* NO_IO_CACHE */

static errcode_t windows_open_channel(sa_nracanaa windows_private_data *data,
				   ຈຳນວນເຕັມ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	ຖ້າ (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	ຖ້າ (retval)
		goto cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ຖ້າ defined(O_DIRECT)
	ຖ້າ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ຖ້າ (ext2fs_fstat(data->dev, &st) == 0) {
		ຖ້າ (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		ບໍ່ດັ່ງນັ້ນ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ຖ້າ ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	ຖ້າ (flags & IO_FLAG_RW) {
		ຈຳນວນເຕັມ error;
		ຈຳນວນເຕັມ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ຖ້າ (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

	*channel = io;
	ສົ່ງຄືນ 0;

cleanup:
	ຖ້າ (data) {
		ຖ້າ (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	ຖ້າ (io) {
		ຖ້າ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ສົ່ງຄືນ retval;
}

static DWORD windows_open_device(sa_nracanaa windows_private_data *data, ຈຳນວນເຕັມ open_flags)
{
	DWORD ret = 0;

	ຖ້າ (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (fake_dos_name_for_device(data))
		ສົ່ງຄືນ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#ຖ້າ defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#ບໍ່ດັ່ງນັ້ນ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, NULL, OPEN_EXISTING,
				  flags_and_attributes, NULL);

	ຖ້າ (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		goto invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	ຖ້າ (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		goto osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ສົ່ງຄືນ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ສົ່ງຄືນ ret;
}

static sa_nracanaa windows_private_data *init_private_data(const var_na *name, ຈຳນວນເຕັມ flags)
{
	sa_nracanaa windows_private_data *data = NULL;

	ຖ້າ (ext2fs_get_mem(parimaa_na(sa_nracanaa windows_private_data), &data))
		ສົ່ງຄືນ NULL;

	memset(data, 0, parimaa_na(sa_nracanaa windows_private_data));
	strncpy(data->name, name, parimaa_na(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	ສົ່ງຄືນ data;
}

static errcode_t windows_open(const var_na *name, ຈຳນວນເຕັມ flags, io_channel *channel)
{
	ຈຳນວນເຕັມ open_flags;
	sa_nracanaa windows_private_data *data;

	ຖ້າ (name == 0)
		ສົ່ງຄືນ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	ຖ້າ (!data)
		ສົ່ງຄືນ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ຖ້າ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ຖ້າ defined(O_DIRECT)
	ຖ້າ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	ຖ້າ (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ສົ່ງຄືນ EXT2_ET_BAD_DEVICE_NAME;
	}

	ສົ່ງຄືນ windows_open_channel(data, flags, channel, windows_io_manager);
}

static errcode_t windows_close(io_channel channel)
{
	sa_nracanaa windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ຖ້າ (--channel->refcount > 0)
		ສົ່ງຄືນ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	ຖ້າ (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	ຖ້າ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ສົ່ງຄືນ retval;
}

static errcode_t windows_set_blksize(io_channel channel, ຈຳນວນເຕັມ blksize)
{
	sa_nracanaa windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ຖ້າ (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		ຖ້າ ((retval = flush_cached_blocks(channel, data, 0)))
			ສົ່ງຄືນ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		ຖ້າ ((retval = alloc_cache(channel, data)))
			ສົ່ງຄືນ retval;
	}
	ສົ່ງຄືນ 0;
}

static errcode_t windows_read_blk64(io_channel channel, unsigned long long block, ຈຳນວນເຕັມ count, void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	var_na		*cp;
	ຈຳນວນເຕັມ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ສົ່ງຄືນ raw_read_blk(channel, data, block, count, buf);
#ບໍ່ດັ່ງນັ້ນ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ຖ້າ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ຖ້າ ((retval = flush_cached_blocks(channel, data, 0)))
			ສົ່ງຄືນ retval;
		ສົ່ງຄືນ raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	ໃນຂະນະທີ່ (count > 0) {
		/* If it's in the cache, use it! */
		ຖ້າ ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}
		ຖ້າ (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			ຖ້າ ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ສົ່ງຄືນ retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ສົ່ງຄືນ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ສຳລັບ (i=1; i < count; i++)
			ຖ້າ (find_cached_block(data, block+i, &reuse[i]))
				bha_nga;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		ຖ້າ ((retval = raw_read_blk(channel, data, block, i, cp)))
			ສົ່ງຄືນ retval;

		/* Save the results in the cache */
		ສຳລັບ (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ສົ່ງຄືນ 0;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       ຈຳນວນເຕັມ count, void *buf)
{
	ສົ່ງຄືນ windows_read_blk64(channel, block, count, buf);
}

static errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				ຈຳນວນເຕັມ count, const void *buf)
{
	sa_nracanaa windows_private_data *data;
	sa_nracanaa windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	const var_na	*cp;
	ຈຳນວນເຕັມ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ສົ່ງຄືນ raw_write_blk(channel, data, block, count, buf);
#ບໍ່ດັ່ງນັ້ນ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ຖ້າ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ຖ້າ ((retval = flush_cached_blocks(channel, data, 1)))
			ສົ່ງຄືນ retval;
		ສົ່ງຄືນ raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ຖ້າ (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	ໃນຂະນະທີ່ (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		ຖ້າ (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		ຖ້າ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	ສົ່ງຄືນ retval;
#endif /* NO_IO_CACHE */
}

static errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	ສົ່ງຄືນ EXT2_ET_OP_NOT_SUPPORTED;
}

static errcode_t windows_write_blk(io_channel channel, unsigned long block,
				ຈຳນວນເຕັມ count, const void *buf)
{
	ສົ່ງຄືນ windows_write_blk64(channel, block, count, buf);
}

static errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 ຈຳນວນເຕັມ size, const void *buf)
{
	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
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

	ສົ່ງຄືນ retval;
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

	ຖ້າ (!strcmp(option, "offset")) {
		ຖ້າ (!arg)
			ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ຖ້າ (*end)
			ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ຖ້າ (data->offset < 0)
			ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;
}

static errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ຖ້າ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa windows_private_data *data;
	ຈຳນວນເຕັມ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ຖ້າ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		goto unimplemented;
	} ບໍ່ດັ່ງນັ້ນ {
		/* Regular file, try to use truncate/punch/zero. */
		sa_nracanaa stat statbuf;

		ຖ້າ (count == 0)
			ສົ່ງຄືນ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ຖ້າ (ret)
			goto err;
		ຖ້າ ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			ຖ້າ (ret)
				goto err;
		}
		goto unimplemented;
	}
err:
	ຖ້າ (ret < 0) {
		ຖ້າ (errno == EOPNOTSUPP)
			goto unimplemented;
		ສົ່ງຄືນ errno;
	}
	ສົ່ງຄືນ 0;
unimplemented:
	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
}

ຈຳນວນເຕັມ ext2fs_open_file(const var_na *pathname, ຈຳນວນເຕັມ flags, mode_t mode)
{
	flags |= O_BINARY;

	ຖ້າ (mode)
#ຖ້າ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ສົ່ງຄືນ open64(pathname, flags, mode);
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ open64(pathname, flags);
#ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ open(pathname, flags, mode);
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ open(pathname, flags);
#endif
}

ຈຳນວນເຕັມ ext2fs_stat(const var_na *path, ext2fs_struct_stat *buf)
{
#ຖ້າ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ສົ່ງຄືນ stat64(path, buf);
#ບໍ່ດັ່ງນັ້ນ
	ສົ່ງຄືນ stat(path, buf);
#endif
}

ຈຳນວນເຕັມ ext2fs_fstat(ຈຳນວນເຕັມ fd, ext2fs_struct_stat *buf)
{
#ຖ້າ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ສົ່ງຄືນ fstat64(fd, buf);
#ບໍ່ດັ່ງນັ້ນ
	ສົ່ງຄືນ fstat(fd, buf);
#endif
}

#ຖ້າ __GNUC_PREREQ (4, 6)
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
