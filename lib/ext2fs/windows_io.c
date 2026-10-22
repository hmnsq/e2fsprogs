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

#ضمن <windows.h>
#ضمن <winioctl.h>
#ضمن <io.h>

#ܐܢ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ضمن <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ܐܚܪܢܐ
#define TRACE(...) ܥܒܕ { } ܟܡܐ (0);
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  ܐܢ ((بنية)->magic != (code)) ܦܢܐ (code)
#define EXT2_CHECK_MAGIC_RETURN(بنية, code, ret) \
	  ܐܢ ((بنية)->magic != (code)) ܦܢܐ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

بنية windows_cache {
	حرف			*buf;
	unsigned long long	block;
	صحيح			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

بنية windows_private_data {
	صحيح	magic;
	حرف	name[MAX_PATH];
	HANDLE	handle;
	حرف	dos_device[MAX_PATH];
	حرف	cf_device[MAX_PATH];
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_time;
	ext2_loff_t offset;
	بنية windows_cache cache[CACHE_SIZE];
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساكن صحيح fake_dos_name_for_device(بنية windows_private_data *data)
{
	ܐܢ (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ܦܢܐ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	ܐܢ (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ܦܢܐ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ܦܢܐ 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_private_data *data)
{
	ܐܢ (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

ساكن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ܐܢ (stats)
		*stats = &data->io_stats;

	ܦܢܐ retval;
}

ساكن LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	ܦܢܐ li;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية windows_private_data *data,
			      unsigned long long block,
			      صحيح count, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned حرف	*buf = bufv;
	ssize_t		really_read = 0;

	size = (count < 0) ? -count : count * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ܐܢ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ܐܢ (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
		اذهب bounce_read;
	}

	ܐܢ (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = GetLastError();
		اذهب error_out;
	}
	ܐܢ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ܐܢ (!ReadFile(data->handle, buf, size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}
		ܐܢ (actual != size) {
	short_read:
		ܐܢ (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} ܐܚܪܢܐ
			retval = EXT2_ET_SHORT_READ;
		اذهب error_out;
		}
	ܦܢܐ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ܟܡܐ (size > 0) {
		ܐܢ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}
		ܐܢ (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			اذهب short_read;
		}
		actual = size;
		ܐܢ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ܦܢܐ 0;

error_out:
	ܐܢ (actual >= 0 && actual < size)
		memset((حرف *) buf+actual, 0, size-actual);
	ܐܢ (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	ܦܢܐ retval;
}

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية windows_private_data *data,
			       unsigned long long block,
			       صحيح count, ثابت فراغ *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت unsigned حرف *buf = bufv;

	ܐܢ (count == 1)
		size = channel->block_size;
	ܐܚܪܢܐ {
		ܐܢ (count < 0)
			size = -count;
		ܐܚܪܢܐ
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ܐܢ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ܐܢ (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
	اذهب bounce_write;
	}

		ܐܢ (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
		اذهب error_out;
	}

	SetLastError(0);

	ܐܢ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ܐܢ (!WriteFile(data->handle, buf, size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}

		ܐܢ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_out;
		}
		ܦܢܐ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ܟܡܐ (size > 0) {
		ܐܢ (size < channel->block_size) {
			ܐܢ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
				retval = GetLastError();
				اذهب error_out;
			}
			ܐܢ (actual != channel->block_size) {
				ܐܢ (actual < 0) {
					retval = GetLastError();
					اذهب error_out;
				}
				memset((حرف *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		ܐܢ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		ܐܢ (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
		ܐܢ (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}

		ܐܢ (actual != channel->block_size)
			اذهب short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ܦܢܐ 0;

error_out:
	ܐܢ (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	ܦܢܐ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية windows_private_data *data)
{
	errcode_t		retval;
	بنية windows_cache	*cache;
	صحيح			i;

	data->access_time = 0;
	لكل (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ܐܢ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ܐܢ (retval)
			ܦܢܐ retval;
	}
	ܐܢ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ܐܢ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ܦܢܐ retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية windows_private_data *data)
{
	بنية windows_cache    *cache;
	صحيح			i;

	data->access_time = 0;
	لكل (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ܐܢ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ܐܢ (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية windows_cache *find_cached_block(بنية windows_private_data *data,
					    unsigned long long block,
					    بنية windows_cache **eldest)
{
	بنية windows_cache	*cache, *unused_cache, *oldest_cache;
	صحيح			i;

	unused_cache = oldest_cache = 0;
	لكل (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ܐܢ (!cache->in_use) {
			ܐܢ (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		ܐܢ (cache->block == block) {
			cache->access_time = ++data->access_time;
			ܦܢܐ cache;
		}
		ܐܢ (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ܐܢ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ܦܢܐ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_private_data *data,
		 بنية windows_cache *cache, unsigned long long block)
{
	ܐܢ (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_blocks(io_channel channel,
				     بنية windows_private_data *data,
				     صحيح invalidate)
{
	بنية windows_cache	*cache;
	errcode_t	retval, retval2;
	صحيح			i;

	retval2 = 0;
	لكل (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ܐܢ (!cache->in_use)
			استمر;

		ܐܢ (invalidate)
			cache->in_use = 0;

		ܐܢ (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		ܐܢ (retval)
			retval2 = retval;
		ܐܚܪܢܐ
			cache->dirty = 0;
	}
	ܦܢܐ retval2;
}
#endif /* NO_IO_CACHE */

ساكن errcode_t windows_open_channel(بنية windows_private_data *data,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	ܐܢ (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	ܐܢ (retval)
		اذهب cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ܐܢ defined(O_DIRECT)
	ܐܢ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ܐܢ (ext2fs_fstat(data->dev, &st) == 0) {
		ܐܢ (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		ܐܚܪܢܐ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ܐܢ ((retval = alloc_cache(io, data)))
		اذهب cleanup;

#ifdef BLKROGET
	ܐܢ (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ܐܢ (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#endif

	*channel = io;
	ܦܢܐ 0;

cleanup:
	ܐܢ (data) {
		ܐܢ (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	ܐܢ (io) {
		ܐܢ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ܦܢܐ retval;
}

ساكن DWORD windows_open_device(بنية windows_private_data *data, صحيح open_flags)
{
	DWORD ret = 0;

	ܐܢ (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	ܐܚܪܢܐ ܐܢ (fake_dos_name_for_device(data))
		ܦܢܐ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#ܐܢ defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#ܐܚܪܢܐ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, عدم, OPEN_EXISTING,
				  flags_and_attributes, عدم);

	ܐܢ (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	ܐܢ (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		اذهب osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ܦܢܐ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ܦܢܐ ret;
}

ساكن بنية windows_private_data *init_private_data(ثابت حرف *name, صحيح flags)
{
	بنية windows_private_data *data = عدم;

	ܐܢ (ext2fs_get_mem(حجم(بنية windows_private_data), &data))
		ܦܢܐ عدم;

	memset(data, 0, حجم(بنية windows_private_data));
	strncpy(data->name, name, حجم(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	ܦܢܐ data;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_private_data *data;

	ܐܢ (name == 0)
		ܦܢܐ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	ܐܢ (!data)
		ܦܢܐ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ܐܢ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ܐܢ defined(O_DIRECT)
	ܐܢ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	ܐܢ (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ܦܢܐ EXT2_ET_BAD_DEVICE_NAME;
	}

	ܦܢܐ windows_open_channel(data, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ܐܢ (--channel->refcount > 0)
		ܦܢܐ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	ܐܢ (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	ܐܢ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ܦܢܐ retval;
}

ساكن errcode_t windows_set_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ܐܢ (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		ܐܢ ((retval = flush_cached_blocks(channel, data, 0)))
			ܦܢܐ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		ܐܢ ((retval = alloc_cache(channel, data)))
			ܦܢܐ retval;
	}
	ܦܢܐ 0;
}

ساكن errcode_t windows_read_blk64(io_channel channel, unsigned long long block, صحيح count, فراغ *buf)
{
	بنية windows_private_data *data;
	بنية windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ܦܢܐ raw_read_blk(channel, data, block, count, buf);
#ܐܚܪܢܐ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ܐܢ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ܐܢ ((retval = flush_cached_blocks(channel, data, 0)))
			ܦܢܐ retval;
		ܦܢܐ raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	ܟܡܐ (count > 0) {
		/* If it's in the cache, use it! */
		ܐܢ ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			استمر;
		}
		ܐܢ (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			ܐܢ ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ܦܢܐ retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ܦܢܐ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < count; i++)
			ܐܢ (find_cached_block(data, block+i, &reuse[i]))
				اكسر;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		ܐܢ ((retval = raw_read_blk(channel, data, block, i, cp)))
			ܦܢܐ retval;

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ܦܢܐ 0;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *buf)
{
	ܦܢܐ windows_read_blk64(channel, block, count, buf);
}

ساكن errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				صحيح count, ثابت فراغ *buf)
{
	بنية windows_private_data *data;
	بنية windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ܦܢܐ raw_write_blk(channel, data, block, count, buf);
#ܐܚܪܢܐ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ܐܢ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ܐܢ ((retval = flush_cached_blocks(channel, data, 1)))
			ܦܢܐ retval;
		ܦܢܐ raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ܐܢ (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	ܟܡܐ (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		ܐܢ (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		ܐܢ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	ܦܢܐ retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *buf)
{
	ܦܢܐ windows_write_blk64(channel, block, count, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 صحيح size, ثابت فراغ *buf)
{
	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t windows_flush(io_channel channel)
{
	بنية windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	ܦܢܐ retval;
}

ساكن errcode_t windows_set_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية windows_private_data *data;
	unsigned long long tmp;
	حرف *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ܐܢ (!strcmp(option, "offset")) {
		ܐܢ (!arg)
			ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ܐܢ (*end)
			ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ܐܢ (data->offset < 0)
			ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
		ܦܢܐ 0;
	}
	ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
}

ساكن errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ܐܢ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ساكن errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	بنية windows_private_data *data;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ܐܢ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} ܐܚܪܢܐ {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		ܐܢ (count == 0)
			ܦܢܐ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ܐܢ (ret)
			اذهب err;
		ܐܢ ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			ܐܢ (ret)
				اذهب err;
		}
		اذهب unimplemented;
	}
err:
	ܐܢ (ret < 0) {
		ܐܢ (errno == EOPNOTSUPP)
			اذهب unimplemented;
		ܦܢܐ errno;
	}
	ܦܢܐ 0;
unimplemented:
	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t mode)
{
	flags |= O_BINARY;

	ܐܢ (mode)
#ܐܢ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ܦܢܐ open64(pathname, flags, mode);
	ܐܚܪܢܐ
		ܦܢܐ open64(pathname, flags);
#ܐܚܪܢܐ
		ܦܢܐ open(pathname, flags, mode);
	ܐܚܪܢܐ
		ܦܢܐ open(pathname, flags);
#endif
}

صحيح ext2fs_stat(ثابت حرف *path, ext2fs_struct_stat *buf)
{
#ܐܢ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ܦܢܐ stat64(path, buf);
#ܐܚܪܢܐ
	ܦܢܐ stat(path, buf);
#endif
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#ܐܢ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ܦܢܐ fstat64(fd, buf);
#ܐܚܪܢܐ
	ܦܢܐ fstat(fd, buf);
#endif
}

#ܐܢ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

ساكن بنية struct_io_manager struct_windows_manager = {
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
