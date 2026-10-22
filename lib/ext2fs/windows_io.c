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

#شامل <windows.h>
#شامل <winioctl.h>
#شامل <io.h>

#اگر !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#شامل <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    نویسه __log[256];\
		    snprintf(__log, اندازه(__log), __VA_ARGS__);\
		    __log[اندازه(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#وگرنه
#define TRACE(...) انجامبده { } وقتی (0);
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)
#define EXT2_CHECK_MAGIC_RETURN(ساختار, code, ret) \
	  اگر ((ساختار)->magic != (code)) بازگشت (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ساختار windows_cache {
	نویسه			*buf;
	unsigned long long	block;
	صحیح			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ساختار windows_private_data {
	صحیح	magic;
	نویسه	name[MAX_PATH];
	HANDLE	handle;
	نویسه	dos_device[MAX_PATH];
	نویسه	cf_device[MAX_PATH];
	صحیح	dev;
	صحیح	flags;
	صحیح	align;
	صحیح	access_time;
	ext2_loff_t offset;
	ساختار windows_cache cache[CACHE_SIZE];
	تهی	*bounce;
	ساختار struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ایستا صحیح fake_dos_name_for_device(ساختار windows_private_data *data)
{
	اگر (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		بازگشت 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	اگر (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		بازگشت 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	بازگشت 0;
}

ایستا تهی remove_fake_dos_name(ساختار windows_private_data *data)
{
	اگر (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

ایستا errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ساختار windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (stats)
		*stats = &data->io_stats;

	بازگشت retval;
}

ایستا LARGE_INTEGER make_large_integer(LONGLONG مقدار)
{
	LARGE_INTEGER	li;

	li.QuadPart = مقدار;
	بازگشت li;
}

/*
 * Here are the raw I/O functions
 */
ایستا errcode_t raw_read_blk(io_channel channel,
			      ساختار windows_private_data *data,
			      unsigned long long block,
			      صحیح شمار, تهی *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned نویسه	*buf = bufv;
	ssize_t		really_read = 0;

	size = (شمار < 0) ? -شمار : شمار * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	اگر (data->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (!SetFilePointerEx(data->handle, make_large_integer(location), پوچ, FILE_BEGIN)) {
			retval = GetLastError();
			برو error_out;
		}
		برو bounce_read;
	}

	اگر (!SetFilePointerEx(data->handle, make_large_integer(location), پوچ, FILE_BEGIN)) {
		retval = GetLastError();
		برو error_out;
	}
	اگر ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		اگر (!ReadFile(data->handle, buf, size, &actual, پوچ)) {
			retval = GetLastError();
			برو error_out;
		}
		اگر (actual != size) {
	short_read:
		اگر (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} وگرنه
			retval = EXT2_ET_SHORT_READ;
		برو error_out;
		}
	بازگشت 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	وقتی (size > 0) {
		اگر (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, پوچ)) {
			retval = GetLastError();
			برو error_out;
		}
		اگر (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			برو short_read;
		}
		actual = size;
		اگر (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	بازگشت 0;

error_out:
	اگر (actual >= 0 && actual < size)
		memset((نویسه *) buf+actual, 0, size-actual);
	اگر (channel->read_error)
		retval = (channel->read_error)(channel, block, شمار, buf,
					       size, actual, retval);
	بازگشت retval;
}

ایستا errcode_t raw_write_blk(io_channel channel,
			       ساختار windows_private_data *data,
			       unsigned long long block,
			       صحیح شمار, ثابت تهی *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت unsigned نویسه *buf = bufv;

	اگر (شمار == 1)
		size = channel->block_size;
	وگرنه {
		اگر (شمار < 0)
			size = -شمار;
		وگرنه
			size = شمار * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	اگر (data->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (!SetFilePointerEx(data->handle, make_large_integer(location), پوچ, FILE_BEGIN)) {
			retval = GetLastError();
			برو error_out;
		}
	برو bounce_write;
	}

		اگر (!SetFilePointerEx(data->handle, make_large_integer(location), پوچ, FILE_BEGIN)) {
			retval = GetLastError();
		برو error_out;
	}

	SetLastError(0);

	اگر ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		اگر (!WriteFile(data->handle, buf, size, &actual, پوچ)) {
			retval = GetLastError();
			برو error_out;
		}

		اگر (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			برو error_out;
		}
		بازگشت 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	وقتی (size > 0) {
		اگر (size < channel->block_size) {
			اگر (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, پوچ)) {
				retval = GetLastError();
				برو error_out;
			}
			اگر (actual != channel->block_size) {
				اگر (actual < 0) {
					retval = GetLastError();
					برو error_out;
				}
				memset((نویسه *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		اگر (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		اگر (!SetFilePointerEx(data->handle, make_large_integer(location), پوچ, FILE_BEGIN)) {
			retval = GetLastError();
			برو error_out;
		}
		اگر (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, پوچ)) {
			retval = GetLastError();
			برو error_out;
		}

		اگر (actual != channel->block_size)
			برو short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	بازگشت 0;

error_out:
	اگر (channel->write_error)
		retval = (channel->write_error)(channel, block, شمار, buf,
						size, actual, retval);
	بازگشت retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ایستا errcode_t alloc_cache(io_channel channel,
			     ساختار windows_private_data *data)
{
	errcode_t		retval;
	ساختار windows_cache	*cache;
	صحیح			i;

	data->access_time = 0;
	برای (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval)
			بازگشت retval;
	}
	اگر (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	بازگشت retval;
}

/* Free the cache buffers */
ایستا تهی free_cache(ساختار windows_private_data *data)
{
	ساختار windows_cache    *cache;
	صحیح			i;

	data->access_time = 0;
	برای (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	اگر (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ایستا ساختار windows_cache *find_cached_block(ساختار windows_private_data *data,
					    unsigned long long block,
					    ساختار windows_cache **eldest)
{
	ساختار windows_cache	*cache, *unused_cache, *oldest_cache;
	صحیح			i;

	unused_cache = oldest_cache = 0;
	برای (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		اگر (!cache->in_use) {
			اگر (!unused_cache)
				unused_cache = cache;
			ادامه;
		}
		اگر (cache->block == block) {
			cache->access_time = ++data->access_time;
			بازگشت cache;
		}
		اگر (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	اگر (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	بازگشت 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ایستا تهی reuse_cache(io_channel channel, ساختار windows_private_data *data,
		 ساختار windows_cache *cache, unsigned long long block)
{
	اگر (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
ایستا errcode_t flush_cached_blocks(io_channel channel,
				     ساختار windows_private_data *data,
				     صحیح invalidate)
{
	ساختار windows_cache	*cache;
	errcode_t	retval, retval2;
	صحیح			i;

	retval2 = 0;
	برای (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		اگر (!cache->in_use)
			ادامه;

		اگر (invalidate)
			cache->in_use = 0;

		اگر (!cache->dirty)
			ادامه;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		اگر (retval)
			retval2 = retval;
		وگرنه
			cache->dirty = 0;
	}
	بازگشت retval2;
}
#endif /* NO_IO_CACHE */

ایستا errcode_t windows_open_channel(ساختار windows_private_data *data,
				   صحیح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = پوچ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(اندازه(ساختار struct_io_channel), &io);
	اگر (retval)
		برو cleanup;
	memset(io, 0, اندازه(ساختار struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	اگر (retval)
		برو cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#اگر defined(O_DIRECT)
	اگر (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	اگر (ext2fs_fstat(data->dev, &st) == 0) {
		اگر (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		وگرنه
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	اگر ((retval = alloc_cache(io, data)))
		برو cleanup;

#ifdef BLKROGET
	اگر (flags & IO_FLAG_RW) {
		صحیح error;
		صحیح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		اگر (!error && readonly) {
			retval = EPERM;
			برو cleanup;
		}
	}
#endif

	*channel = io;
	بازگشت 0;

cleanup:
	اگر (data) {
		اگر (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	اگر (io) {
		اگر (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	بازگشت retval;
}

ایستا DWORD windows_open_device(ساختار windows_private_data *data, صحیح open_flags)
{
	DWORD ret = 0;

	اگر (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	وگرنه اگر (fake_dos_name_for_device(data))
		بازگشت -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#اگر defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#وگرنه
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, پوچ, OPEN_EXISTING,
				  flags_and_attributes, پوچ);

	اگر (data->handle == INVALID_HANDLE_مقدار) {
		ret = GetLastError();
		برو invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	اگر (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		برو osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	بازگشت 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	بازگشت ret;
}

ایستا ساختار windows_private_data *init_private_data(ثابت نویسه *name, صحیح flags)
{
	ساختار windows_private_data *data = پوچ;

	اگر (ext2fs_get_mem(اندازه(ساختار windows_private_data), &data))
		بازگشت پوچ;

	memset(data, 0, اندازه(ساختار windows_private_data));
	strncpy(data->name, name, اندازه(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.رقم_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_مقدار;

	بازگشت data;
}

ایستا errcode_t windows_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	صحیح open_flags;
	ساختار windows_private_data *data;

	اگر (name == 0)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	اگر (!data)
		بازگشت EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	اگر (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#اگر defined(O_DIRECT)
	اگر (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	اگر (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		بازگشت EXT2_ET_BAD_DEVICE_NAME;
	}

	بازگشت windows_open_channel(data, flags, channel, windows_io_manager);
}

ایستا errcode_t windows_close(io_channel channel)
{
	ساختار windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		بازگشت 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	اگر (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	بازگشت retval;
}

ایستا errcode_t windows_set_blksize(io_channel channel, صحیح blksize)
{
	ساختار windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		اگر ((retval = flush_cached_blocks(channel, data, 0)))
			بازگشت retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		اگر ((retval = alloc_cache(channel, data)))
			بازگشت retval;
	}
	بازگشت 0;
}

ایستا errcode_t windows_read_blk64(io_channel channel, unsigned long long block, صحیح شمار, تهی *buf)
{
	ساختار windows_private_data *data;
	ساختار windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	نویسه		*cp;
	صحیح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	بازگشت raw_read_blk(channel, data, block, شمار, buf);
#وگرنه
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	اگر (شمار < 0 || شمار > WRITE_DIRECT_SIZE) {
		اگر ((retval = flush_cached_blocks(channel, data, 0)))
			بازگشت retval;
		بازگشت raw_read_blk(channel, data, block, شمار, buf);
	}

	cp = buf;
	وقتی (شمار > 0) {
		/* If it's in the cache, use it! */
		اگر ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			شمار--;
			block++;
			cp += channel->block_size;
			ادامه;
		}
		اگر (شمار == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			اگر ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				بازگشت retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			بازگشت 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		برای (i=1; i < شمار; i++)
			اگر (find_cached_block(data, block+i, &reuse[i]))
				بشکن;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		اگر ((retval = raw_read_blk(channel, data, block, i, cp)))
			بازگشت retval;

		/* Save the results in the cache */
		برای (j=0; j < i; j++) {
			شمار--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	بازگشت 0;
#endif /* NO_IO_CACHE */
}

ایستا errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	بازگشت windows_read_blk64(channel, block, شمار, buf);
}

ایستا errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				صحیح شمار, ثابت تهی *buf)
{
	ساختار windows_private_data *data;
	ساختار windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت نویسه	*cp;
	صحیح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	بازگشت raw_write_blk(channel, data, block, شمار, buf);
#وگرنه
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	اگر (شمار < 0 || شمار > WRITE_DIRECT_SIZE) {
		اگر ((retval = flush_cached_blocks(channel, data, 1)))
			بازگشت retval;
		بازگشت raw_write_blk(channel, data, block, شمار, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	اگر (writethrough)
		retval = raw_write_blk(channel, data, block, شمار, buf);

	cp = buf;
	وقتی (شمار > 0) {
		cache = find_cached_block(data, block, &reuse);
		اگر (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		اگر (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		شمار--;
		block++;
		cp += channel->block_size;
	}
	بازگشت retval;
#endif /* NO_IO_CACHE */
}

ایستا errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long شمار)
{
	بازگشت EXT2_ET_OP_NOT_SUPPORTED;
}

ایستا errcode_t windows_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *buf)
{
	بازگشت windows_write_blk64(channel, block, شمار, buf);
}

ایستا errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 صحیح size, ثابت تهی *buf)
{
	بازگشت EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ایستا errcode_t windows_flush(io_channel channel)
{
	ساختار windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	بازگشت retval;
}

ایستا errcode_t windows_set_option(io_channel channel, ثابت نویسه *option,
				 ثابت نویسه *arg)
{
	ساختار windows_private_data *data;
	unsigned long long tmp;
	نویسه *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (!strcmp(option, "offset")) {
		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		اگر (*end)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		اگر (data->offset < 0)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		بازگشت 0;
	}
	بازگشت EXT2_ET_INVALID_ARGUMENT;
}

ایستا errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long شمار)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	بازگشت EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic افزودن
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ایستا errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار windows_private_data *data;
	صحیح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		برو unimplemented;
	} وگرنه {
		/* Regular file, try to use truncate/punch/zero. */
		ساختار stat statbuf;

		اگر (شمار == 0)
			بازگشت 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		اگر (ret)
			برو err;
		اگر ((unsigned long long) statbuf.st_size <
			(block + شمار) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + شمار) * channel->block_size + data->offset);
			اگر (ret)
				برو err;
		}
		برو unimplemented;
	}
err:
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP)
			برو unimplemented;
		بازگشت errno;
	}
	بازگشت 0;
unimplemented:
	بازگشت EXT2_ET_UNIMPLEMENTED;
}

صحیح ext2fs_open_file(ثابت نویسه *pathname, صحیح flags, mode_t mode)
{
	flags |= O_دودویی;

	اگر (mode)
#اگر defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		بازگشت open64(pathname, flags, mode);
	وگرنه
		بازگشت open64(pathname, flags);
#وگرنه
		بازگشت open(pathname, flags, mode);
	وگرنه
		بازگشت open(pathname, flags);
#endif
}

صحیح ext2fs_stat(ثابت نویسه *path, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	بازگشت stat64(path, buf);
#وگرنه
	بازگشت stat(path, buf);
#endif
}

صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	بازگشت fstat64(fd, buf);
#وگرنه
	بازگشت fstat(fd, buf);
#endif
}

#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic برداشت
#endif

ایستا ساختار struct_io_manager struct_windows_manager = {
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
