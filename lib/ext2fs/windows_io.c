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

#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <winioctl.h>
#ಅನ್ತರ್ಭಾವ <io.h>

#ಯದಿ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    ವರ್ಣ __log[256];\
		    snprintf(__log, ಪರಿಮಾಣ(__log), __VA_ARGS__);\
		    __log[ಪರಿಮಾಣ(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ಅನ್ಯಥಾ
#define TRACE(...) ಕುರು { } ಯಾವತ್ (0);
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)
#define EXT2_CHECK_MAGIC_RETURN(ಸಂರಚನಾ, code, ret) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ಸಂರಚನಾ windows_cache {
	ವರ್ಣ			*buf;
	unsigned long long	block;
	ಪೂರ್ಣಾಙ್ಕ			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ಸಂರಚನಾ windows_private_data {
	ಪೂರ್ಣಾಙ್ಕ	magic;
	ವರ್ಣ	name[MAX_PATH];
	HANDLE	handle;
	ವರ್ಣ	dos_device[MAX_PATH];
	ವರ್ಣ	cf_device[MAX_PATH];
	ಪೂರ್ಣಾಙ್ಕ	dev;
	ಪೂರ್ಣಾಙ್ಕ	flags;
	ಪೂರ್ಣಾಙ್ಕ	align;
	ಪೂರ್ಣಾಙ್ಕ	access_time;
	ext2_loff_t offset;
	ಸಂರಚನಾ windows_cache cache[CACHE_SIZE];
	ಶೂನ್ಯ	*bounce;
	ಸಂರಚನಾ struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fake_dos_name_for_device(ಸಂರಚನಾ windows_private_data *data)
{
	ಯದಿ (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	ಯದಿ (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ಪ್ರತ್ಯಾಗಮ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_fake_dos_name(ಸಂರಚನಾ windows_private_data *data)
{
	ಯದಿ (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

ಸ್ಥಿರ errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ಸಂರಚನಾ windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಯದಿ (stats)
		*stats = &data->io_stats;

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ LARGE_INTEGER make_large_integer(LONGLONG ಮೌಲ್ಯ)
{
	LARGE_INTEGER	li;

	li.QuadPart = ಮೌಲ್ಯ;
	ಪ್ರತ್ಯಾಗಮ li;
}

/*
 * Here are the raw I/O functions
 */
ಸ್ಥಿರ errcode_t raw_read_blk(io_channel channel,
			      ಸಂರಚನಾ windows_private_data *data,
			      unsigned long long block,
			      ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned ವರ್ಣ	*buf = bufv;
	ssize_t		really_read = 0;

	size = (ಎಣಿಕೆ < 0) ? -ಎಣಿಕೆ : ಎಣಿಕೆ * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ಯದಿ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ಯದಿ (!SetFilePointerEx(data->handle, make_large_integer(location), ರಿಕ್ತ, FILE_BEGIN)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}
		ಗಚ್ಛ bounce_read;
	}

	ಯದಿ (!SetFilePointerEx(data->handle, make_large_integer(location), ರಿಕ್ತ, FILE_BEGIN)) {
		retval = GetLastError();
		ಗಚ್ಛ error_out;
	}
	ಯದಿ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ಯದಿ (!ReadFile(data->handle, buf, size, &actual, ರಿಕ್ತ)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}
		ಯದಿ (actual != size) {
	short_read:
		ಯದಿ (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} ಅನ್ಯಥಾ
			retval = EXT2_ET_SHORT_READ;
		ಗಚ್ಛ error_out;
		}
	ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ಯಾವತ್ (size > 0) {
		ಯದಿ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ರಿಕ್ತ)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}
		ಯದಿ (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			ಗಚ್ಛ short_read;
		}
		actual = size;
		ಯದಿ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ಪ್ರತ್ಯಾಗಮ 0;

error_out:
	ಯದಿ (actual >= 0 && actual < size)
		memset((ವರ್ಣ *) buf+actual, 0, size-actual);
	ಯದಿ (channel->read_error)
		retval = (channel->read_error)(channel, block, ಎಣಿಕೆ, buf,
					       size, actual, retval);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t raw_write_blk(io_channel channel,
			       ಸಂರಚನಾ windows_private_data *data,
			       unsigned long long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ಧ್ರುವ unsigned ವರ್ಣ *buf = bufv;

	ಯದಿ (ಎಣಿಕೆ == 1)
		size = channel->block_size;
	ಅನ್ಯಥಾ {
		ಯದಿ (ಎಣಿಕೆ < 0)
			size = -ಎಣಿಕೆ;
		ಅನ್ಯಥಾ
			size = ಎಣಿಕೆ * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ಯದಿ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ಯದಿ (!SetFilePointerEx(data->handle, make_large_integer(location), ರಿಕ್ತ, FILE_BEGIN)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}
	ಗಚ್ಛ bounce_write;
	}

		ಯದಿ (!SetFilePointerEx(data->handle, make_large_integer(location), ರಿಕ್ತ, FILE_BEGIN)) {
			retval = GetLastError();
		ಗಚ್ಛ error_out;
	}

	SetLastError(0);

	ಯದಿ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ಯದಿ (!WriteFile(data->handle, buf, size, &actual, ರಿಕ್ತ)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}

		ಯದಿ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ಗಚ್ಛ error_out;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ಯಾವತ್ (size > 0) {
		ಯದಿ (size < channel->block_size) {
			ಯದಿ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ರಿಕ್ತ)) {
				retval = GetLastError();
				ಗಚ್ಛ error_out;
			}
			ಯದಿ (actual != channel->block_size) {
				ಯದಿ (actual < 0) {
					retval = GetLastError();
					ಗಚ್ಛ error_out;
				}
				memset((ವರ್ಣ *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		ಯದಿ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		ಯದಿ (!SetFilePointerEx(data->handle, make_large_integer(location), ರಿಕ್ತ, FILE_BEGIN)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}
		ಯದಿ (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, ರಿಕ್ತ)) {
			retval = GetLastError();
			ಗಚ್ಛ error_out;
		}

		ಯದಿ (actual != channel->block_size)
			ಗಚ್ಛ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ಪ್ರತ್ಯಾಗಮ 0;

error_out:
	ಯದಿ (channel->write_error)
		retval = (channel->write_error)(channel, block, ಎಣಿಕೆ, buf,
						size, actual, retval);
	ಪ್ರತ್ಯಾಗಮ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ಸ್ಥಿರ errcode_t alloc_cache(io_channel channel,
			     ಸಂರಚನಾ windows_private_data *data)
{
	errcode_t		retval;
	ಸಂರಚನಾ windows_cache	*cache;
	ಪೂರ್ಣಾಙ್ಕ			i;

	data->access_time = 0;
	ಪ್ರತಿ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ಯದಿ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಯದಿ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ಯದಿ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

/* Free the cache buffers */
ಸ್ಥಿರ ಶೂನ್ಯ free_cache(ಸಂರಚನಾ windows_private_data *data)
{
	ಸಂರಚನಾ windows_cache    *cache;
	ಪೂರ್ಣಾಙ್ಕ			i;

	data->access_time = 0;
	ಪ್ರತಿ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ಯದಿ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ಯದಿ (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ಸ್ಥಿರ ಸಂರಚನಾ windows_cache *find_cached_block(ಸಂರಚನಾ windows_private_data *data,
					    unsigned long long block,
					    ಸಂರಚನಾ windows_cache **eldest)
{
	ಸಂರಚನಾ windows_cache	*cache, *unused_cache, *oldest_cache;
	ಪೂರ್ಣಾಙ್ಕ			i;

	unused_cache = oldest_cache = 0;
	ಪ್ರತಿ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ಯದಿ (!cache->in_use) {
			ಯದಿ (!unused_cache)
				unused_cache = cache;
			ಅನುವರ್ತ;
		}
		ಯದಿ (cache->block == block) {
			cache->access_time = ++data->access_time;
			ಪ್ರತ್ಯಾಗಮ cache;
		}
		ಯದಿ (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ಯದಿ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ಸ್ಥಿರ ಶೂನ್ಯ reuse_cache(io_channel channel, ಸಂರಚನಾ windows_private_data *data,
		 ಸಂರಚನಾ windows_cache *cache, unsigned long long block)
{
	ಯದಿ (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
ಸ್ಥಿರ errcode_t flush_cached_blocks(io_channel channel,
				     ಸಂರಚನಾ windows_private_data *data,
				     ಪೂರ್ಣಾಙ್ಕ invalidate)
{
	ಸಂರಚನಾ windows_cache	*cache;
	errcode_t	retval, retval2;
	ಪೂರ್ಣಾಙ್ಕ			i;

	retval2 = 0;
	ಪ್ರತಿ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ಯದಿ (!cache->in_use)
			ಅನುವರ್ತ;

		ಯದಿ (invalidate)
			cache->in_use = 0;

		ಯದಿ (!cache->dirty)
			ಅನುವರ್ತ;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		ಯದಿ (retval)
			retval2 = retval;
		ಅನ್ಯಥಾ
			cache->dirty = 0;
	}
	ಪ್ರತ್ಯಾಗಮ retval2;
}
#endif /* NO_IO_CACHE */

ಸ್ಥಿರ errcode_t windows_open_channel(ಸಂರಚನಾ windows_private_data *data,
				   ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ರಿಕ್ತ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel), &io);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(io, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ಯದಿ defined(O_DIRECT)
	ಯದಿ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ಯದಿ (ext2fs_fstat(data->dev, &st) == 0) {
		ಯದಿ (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		ಅನ್ಯಥಾ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ಯದಿ ((retval = alloc_cache(io, data)))
		ಗಚ್ಛ cleanup;

#ifdef BLKROGET
	ಯದಿ (flags & IO_FLAG_RW) {
		ಪೂರ್ಣಾಙ್ಕ error;
		ಪೂರ್ಣಾಙ್ಕ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ಯದಿ (!error && readonly) {
			retval = EPERM;
			ಗಚ್ಛ cleanup;
		}
	}
#endif

	*channel = io;
	ಪ್ರತ್ಯಾಗಮ 0;

cleanup:
	ಯದಿ (data) {
		ಯದಿ (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	ಯದಿ (io) {
		ಯದಿ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ DWORD windows_open_device(ಸಂರಚನಾ windows_private_data *data, ಪೂರ್ಣಾಙ್ಕ open_flags)
{
	DWORD ret = 0;

	ಯದಿ (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	ಅನ್ಯಥಾ ಯದಿ (fake_dos_name_for_device(data))
		ಪ್ರತ್ಯಾಗಮ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#ಯದಿ defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#ಅನ್ಯಥಾ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, ರಿಕ್ತ, OPEN_EXISTING,
				  flags_and_attributes, ರಿಕ್ತ);

	ಯದಿ (data->handle == INVALID_HANDLE_ಮೌಲ್ಯ) {
		ret = GetLastError();
		ಗಚ್ಛ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	ಯದಿ (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		ಗಚ್ಛ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ಪ್ರತ್ಯಾಗಮ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಸಂರಚನಾ windows_private_data *init_private_data(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ windows_private_data *data = ರಿಕ್ತ;

	ಯದಿ (ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ windows_private_data), &data))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	memset(data, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ windows_private_data));
	strncpy(data->name, name, ಪರಿಮಾಣ(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.ಅಂಕೆ_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_ಮೌಲ್ಯ;

	ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ errcode_t windows_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	ಪೂರ್ಣಾಙ್ಕ open_flags;
	ಸಂರಚನಾ windows_private_data *data;

	ಯದಿ (name == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	ಯದಿ (!data)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ಯದಿ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ಯದಿ defined(O_DIRECT)
	ಯದಿ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	ಯದಿ (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
	}

	ಪ್ರತ್ಯಾಗಮ windows_open_channel(data, flags, channel, windows_io_manager);
}

ಸ್ಥಿರ errcode_t windows_close(io_channel channel)
{
	ಸಂರಚನಾ windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಯದಿ (--channel->refcount > 0)
		ಪ್ರತ್ಯಾಗಮ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	ಯದಿ (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	ಯದಿ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t windows_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	ಸಂರಚನಾ windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಯದಿ (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		ಯದಿ ((retval = flush_cached_blocks(channel, data, 0)))
			ಪ್ರತ್ಯಾಗಮ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		ಯದಿ ((retval = alloc_cache(channel, data)))
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t windows_read_blk64(io_channel channel, unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ windows_private_data *data;
	ಸಂರಚನಾ windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	ವರ್ಣ		*cp;
	ಪೂರ್ಣಾಙ್ಕ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ಪ್ರತ್ಯಾಗಮ raw_read_blk(channel, data, block, ಎಣಿಕೆ, buf);
#ಅನ್ಯಥಾ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ಯದಿ (ಎಣಿಕೆ < 0 || ಎಣಿಕೆ > WRITE_DIRECT_SIZE) {
		ಯದಿ ((retval = flush_cached_blocks(channel, data, 0)))
			ಪ್ರತ್ಯಾಗಮ retval;
		ಪ್ರತ್ಯಾಗಮ raw_read_blk(channel, data, block, ಎಣಿಕೆ, buf);
	}

	cp = buf;
	ಯಾವತ್ (ಎಣಿಕೆ > 0) {
		/* If it's in the cache, use it! */
		ಯದಿ ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			ಎಣಿಕೆ--;
			block++;
			cp += channel->block_size;
			ಅನುವರ್ತ;
		}
		ಯದಿ (ಎಣಿಕೆ == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			ಯದಿ ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ಪ್ರತ್ಯಾಗಮ retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ಪ್ರತ್ಯಾಗಮ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ಪ್ರತಿ (i=1; i < ಎಣಿಕೆ; i++)
			ಯದಿ (find_cached_block(data, block+i, &reuse[i]))
				ಭಙ್ಗ;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		ಯದಿ ((retval = raw_read_blk(channel, data, block, i, cp)))
			ಪ್ರತ್ಯಾಗಮ retval;

		/* Save the results in the cache */
		ಪ್ರತಿ (j=0; j < i; j++) {
			ಎಣಿಕೆ--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
#endif /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ windows_read_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ windows_private_data *data;
	ಸಂರಚನಾ windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ಧ್ರುವ ವರ್ಣ	*cp;
	ಪೂರ್ಣಾಙ್ಕ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ಪ್ರತ್ಯಾಗಮ raw_write_blk(channel, data, block, ಎಣಿಕೆ, buf);
#ಅನ್ಯಥಾ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ಯದಿ (ಎಣಿಕೆ < 0 || ಎಣಿಕೆ > WRITE_DIRECT_SIZE) {
		ಯದಿ ((retval = flush_cached_blocks(channel, data, 1)))
			ಪ್ರತ್ಯಾಗಮ retval;
		ಪ್ರತ್ಯಾಗಮ raw_write_blk(channel, data, block, ಎಣಿಕೆ, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ಯದಿ (writethrough)
		retval = raw_write_blk(channel, data, block, ಎಣಿಕೆ, buf);

	cp = buf;
	ಯಾವತ್ (ಎಣಿಕೆ > 0) {
		cache = find_cached_block(data, block, &reuse);
		ಯದಿ (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		ಯದಿ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		ಎಣಿಕೆ--;
		block++;
		cp += channel->block_size;
	}
	ಪ್ರತ್ಯಾಗಮ retval;
#endif /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ಎಣಿಕೆ)
{
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_OP_NOT_SUPPORTED;
}

ಸ್ಥಿರ errcode_t windows_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ windows_write_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 ಪೂರ್ಣಾಙ್ಕ size, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t windows_flush(io_channel channel)
{
	ಸಂರಚನಾ windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t windows_set_option(io_channel channel, ಧ್ರುವ ವರ್ಣ *option,
				 ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸಂರಚನಾ windows_private_data *data;
	unsigned long long tmp;
	ವರ್ಣ *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಯದಿ (!strcmp(option, "offset")) {
		ಯದಿ (!arg)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ಯದಿ (*end)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ಯದಿ (data->offset < 0)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
}

ಸ್ಥಿರ errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long ಎಣಿಕೆ)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ಯದಿ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ತಳ್ಳು
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ಸ್ಥಿರ errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ windows_private_data *data;
	ಪೂರ್ಣಾಙ್ಕ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಯದಿ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		ಗಚ್ಛ unimplemented;
	} ಅನ್ಯಥಾ {
		/* Regular file, try to use truncate/punch/zero. */
		ಸಂರಚನಾ stat statbuf;

		ಯದಿ (ಎಣಿಕೆ == 0)
			ಪ್ರತ್ಯಾಗಮ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ಯದಿ (ret)
			ಗಚ್ಛ err;
		ಯದಿ ((unsigned long long) statbuf.st_size <
			(block + ಎಣಿಕೆ) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + ಎಣಿಕೆ) * channel->block_size + data->offset);
			ಯದಿ (ret)
				ಗಚ್ಛ err;
		}
		ಗಚ್ಛ unimplemented;
	}
err:
	ಯದಿ (ret < 0) {
		ಯದಿ (errno == EOPNOTSUPP)
			ಗಚ್ಛ unimplemented;
		ಪ್ರತ್ಯಾಗಮ errno;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
unimplemented:
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_open_file(ಧ್ರುವ ವರ್ಣ *pathname, ಪೂರ್ಣಾಙ್ಕ flags, mode_t mode)
{
	flags |= O_ದ್ವಿಮಾನ;

	ಯದಿ (mode)
#ಯದಿ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ಪ್ರತ್ಯಾಗಮ open64(pathname, flags, mode);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ open64(pathname, flags);
#ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ open(pathname, flags, mode);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ open(pathname, flags);
#endif
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_stat(ಧ್ರುವ ವರ್ಣ *path, ext2fs_struct_stat *buf)
{
#ಯದಿ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ಪ್ರತ್ಯಾಗಮ stat64(path, buf);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ stat(path, buf);
#endif
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_fstat(ಪೂರ್ಣಾಙ್ಕ fd, ext2fs_struct_stat *buf)
{
#ಯದಿ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ಪ್ರತ್ಯಾಗಮ fstat64(fd, buf);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ fstat(fd, buf);
#endif
}

#ಯದಿ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ತೆಗೆ
#endif

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_windows_manager = {
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
