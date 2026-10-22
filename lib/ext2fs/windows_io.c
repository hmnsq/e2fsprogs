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

#אם !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#אם HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#אם HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#אם HAVE_SYS_STAT_H
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
#אחרת
#define TRACE(...) עשה { } כלעוד (0);
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  אם ((بنية)->magic != (code)) החזר (code)
#define EXT2_CHECK_MAGIC_RETURN(بنية, code, ret) \
	  אם ((بنية)->magic != (code)) החזר (ret)

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
	אם (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		החזר 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	אם (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		החזר 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	החזר 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_private_data *data)
{
	אם (*data->dos_device) {
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

	אם (stats)
		*stats = &data->io_stats;

	החזר retval;
}

ساكن LARGE_INTEGER make_large_integer(LONGLONG value)
{
	LARGE_INTEGER	li;

	li.QuadPart = value;
	החזר li;
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

	אם (data->flags & IO_FLAG_FORCE_BOUNCE) {
		אם (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
		اذهب bounce_read;
	}

	אם (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = GetLastError();
		اذهب error_out;
	}
	אם ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		אם (!ReadFile(data->handle, buf, size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}
		אם (actual != size) {
	short_read:
		אם (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} אחרת
			retval = EXT2_ET_SHORT_READ;
		اذهب error_out;
		}
	החזר 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	כלעוד (size > 0) {
		אם (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}
		אם (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			اذهب short_read;
		}
		actual = size;
		אם (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	החזר 0;

error_out:
	אם (actual >= 0 && actual < size)
		memset((حرف *) buf+actual, 0, size-actual);
	אם (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	החזר retval;
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

	אם (count == 1)
		size = channel->block_size;
	אחרת {
		אם (count < 0)
			size = -count;
		אחרת
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	אם (data->flags & IO_FLAG_FORCE_BOUNCE) {
		אם (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
	اذهب bounce_write;
	}

		אם (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
		اذهب error_out;
	}

	SetLastError(0);

	אם ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		אם (!WriteFile(data->handle, buf, size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}

		אם (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_out;
		}
		החזר 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	כלעוד (size > 0) {
		אם (size < channel->block_size) {
			אם (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
				retval = GetLastError();
				اذهب error_out;
			}
			אם (actual != channel->block_size) {
				אם (actual < 0) {
					retval = GetLastError();
					اذهب error_out;
				}
				memset((حرف *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		אם (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		אם (!SetFilePointerEx(data->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = GetLastError();
			اذهب error_out;
		}
		אם (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, عدم)) {
			retval = GetLastError();
			اذهب error_out;
		}

		אם (actual != channel->block_size)
			اذهب short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	החזר 0;

error_out:
	אם (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	החזר retval;
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
	עבור (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		אם (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		אם (retval)
			החזר retval;
	}
	אם (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		אם (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	החזר retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية windows_private_data *data)
{
	بنية windows_cache    *cache;
	صحيح			i;

	data->access_time = 0;
	עבור (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		אם (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	אם (data->bounce)
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
	עבור (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		אם (!cache->in_use) {
			אם (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		אם (cache->block == block) {
			cache->access_time = ++data->access_time;
			החזר cache;
		}
		אם (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	אם (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	החזר 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_private_data *data,
		 بنية windows_cache *cache, unsigned long long block)
{
	אם (cache->dirty && cache->in_use)
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
	עבור (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		אם (!cache->in_use)
			استمر;

		אם (invalidate)
			cache->in_use = 0;

		אם (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		אם (retval)
			retval2 = retval;
		אחרת
			cache->dirty = 0;
	}
	החזר retval2;
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
	אם (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	אם (retval)
		اذهب cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#אם defined(O_DIRECT)
	אם (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	אם (ext2fs_fstat(data->dev, &st) == 0) {
		אם (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		אחרת
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	אם ((retval = alloc_cache(io, data)))
		اذهب cleanup;

#ifdef BLKROGET
	אם (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		אם (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#endif

	*channel = io;
	החזר 0;

cleanup:
	אם (data) {
		אם (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	אם (io) {
		אם (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	החזר retval;
}

ساكن DWORD windows_open_device(بنية windows_private_data *data, صحيح open_flags)
{
	DWORD ret = 0;

	אם (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	אחרת אם (fake_dos_name_for_device(data))
		החזר -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#אם defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#אחרת
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, عدم, OPEN_EXISTING,
				  flags_and_attributes, عدم);

	אם (data->handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	אם (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		اذهب osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	החזר 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	החזר ret;
}

ساكن بنية windows_private_data *init_private_data(ثابت حرف *name, صحيح flags)
{
	بنية windows_private_data *data = عدم;

	אם (ext2fs_get_mem(حجم(بنية windows_private_data), &data))
		החזר عدم;

	memset(data, 0, حجم(بنية windows_private_data));
	strncpy(data->name, name, حجم(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_VALUE;

	החזר data;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_private_data *data;

	אם (name == 0)
		החזר EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	אם (!data)
		החזר EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	אם (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#אם defined(O_DIRECT)
	אם (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	אם (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		החזר EXT2_ET_BAD_DEVICE_NAME;
	}

	החזר windows_open_channel(data, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	אם (--channel->refcount > 0)
		החזר 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	אם (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	אם (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	החזר retval;
}

ساكن errcode_t windows_set_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	אם (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		אם ((retval = flush_cached_blocks(channel, data, 0)))
			החזר retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		אם ((retval = alloc_cache(channel, data)))
			החזר retval;
	}
	החזר 0;
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
	החזר raw_read_blk(channel, data, block, count, buf);
#אחרת
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	אם (count < 0 || count > WRITE_DIRECT_SIZE) {
		אם ((retval = flush_cached_blocks(channel, data, 0)))
			החזר retval;
		החזר raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	כלעוד (count > 0) {
		/* If it's in the cache, use it! */
		אם ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			استمر;
		}
		אם (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			אם ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				החזר retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			החזר 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		עבור (i=1; i < count; i++)
			אם (find_cached_block(data, block+i, &reuse[i]))
				اكسر;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		אם ((retval = raw_read_blk(channel, data, block, i, cp)))
			החזר retval;

		/* Save the results in the cache */
		עבור (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	החזר 0;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *buf)
{
	החזר windows_read_blk64(channel, block, count, buf);
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
	החזר raw_write_blk(channel, data, block, count, buf);
#אחרת
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	אם (count < 0 || count > WRITE_DIRECT_SIZE) {
		אם ((retval = flush_cached_blocks(channel, data, 1)))
			החזר retval;
		החזר raw_write_blk(channel, data, block, count, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	אם (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf);

	cp = buf;
	כלעוד (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		אם (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		אם (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	החזר retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	החזר EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *buf)
{
	החזר windows_write_blk64(channel, block, count, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 صحيح size, ثابت فراغ *buf)
{
	החזר EXT2_ET_UNIMPLEMENTED;
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

	החזר retval;
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

	אם (!strcmp(option, "offset")) {
		אם (!arg)
			החזר EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		אם (*end)
			החזר EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		אם (data->offset < 0)
			החזר EXT2_ET_INVALID_ARGUMENT;
		החזר 0;
	}
	החזר EXT2_ET_INVALID_ARGUMENT;
}

ساكن errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long count)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	החזר EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#אם __GNUC_PREREQ (4, 6)
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

	אם (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} אחרת {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		אם (count == 0)
			החזר 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		אם (ret)
			اذهب err;
		אם ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			אם (ret)
				اذهب err;
		}
		اذهب unimplemented;
	}
err:
	אם (ret < 0) {
		אם (errno == EOPNOTSUPP)
			اذهب unimplemented;
		החזר errno;
	}
	החזר 0;
unimplemented:
	החזר EXT2_ET_UNIMPLEMENTED;
}

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t mode)
{
	flags |= O_BINARY;

	אם (mode)
#אם defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		החזר open64(pathname, flags, mode);
	אחרת
		החזר open64(pathname, flags);
#אחרת
		החזר open(pathname, flags, mode);
	אחרת
		החזר open(pathname, flags);
#endif
}

صحيح ext2fs_stat(ثابت حرف *path, ext2fs_struct_stat *buf)
{
#אם defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	החזר stat64(path, buf);
#אחרת
	החזר stat(path, buf);
#endif
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#אם defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	החזר fstat64(fd, buf);
#אחרת
	החזר fstat(fd, buf);
#endif
}

#אם __GNUC_PREREQ (4, 6)
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
