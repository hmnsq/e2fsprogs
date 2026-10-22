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

#அந்தர்பாவ <windows.h>
#அந்தர்பாவ <winioctl.h>
#அந்தர்பாவ <io.h>

#எனில் !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#அந்தர்பாவ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    எழுத்து __log[256];\
		    snprintf(__log, அளவு(__log), __VA_ARGS__);\
		    __log[அளவு(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#இல்லையெனில்
#define TRACE(...) செய் { } போது (0);
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)
#define EXT2_CHECK_MAGIC_RETURN(கட்டமைப்பு, code, ret) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

கட்டமைப்பு windows_cache {
	எழுத்து			*buf;
	unsigned long long	block;
	முழு			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

கட்டமைப்பு windows_private_data {
	முழு	magic;
	எழுத்து	name[MAX_PATH];
	HANDLE	handle;
	எழுத்து	dos_device[MAX_PATH];
	எழுத்து	cf_device[MAX_PATH];
	முழு	dev;
	முழு	flags;
	முழு	align;
	முழு	access_time;
	ext2_loff_t offset;
	கட்டமைப்பு windows_cache cache[CACHE_SIZE];
	வெறுமை	*bounce;
	கட்டமைப்பு struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

நிலையான முழு fake_dos_name_for_device(கட்டமைப்பு windows_private_data *data)
{
	எனில் (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		திரும்பு 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	எனில் (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		திரும்பு 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	திரும்பு 0;
}

நிலையான வெறுமை remove_fake_dos_name(கட்டமைப்பு windows_private_data *data)
{
	எனில் (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

நிலையான errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	கட்டமைப்பு windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	எனில் (stats)
		*stats = &data->io_stats;

	திரும்பு retval;
}

நிலையான LARGE_INTEGER make_large_integer(LONGLONG மதிப்பு)
{
	LARGE_INTEGER	li;

	li.QuadPart = மதிப்பு;
	திரும்பு li;
}

/*
 * Here are the raw I/O functions
 */
நிலையான errcode_t raw_read_blk(io_channel channel,
			      கட்டமைப்பு windows_private_data *data,
			      unsigned long long block,
			      முழு மொத்தம், வெறுமை *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned எழுத்து	*buf = bufv;
	ssize_t		really_read = 0;

	size = (மொத்தம் < 0) ? -மொத்தம் : மொத்தம் * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	எனில் (data->flags & IO_FLAG_FORCE_BOUNCE) {
		எனில் (!SetFilePointerEx(data->handle, make_large_integer(location), வெற்று, FILE_BEGIN)) {
			retval = GetLastError();
			செல் error_out;
		}
		செல் bounce_read;
	}

	எனில் (!SetFilePointerEx(data->handle, make_large_integer(location), வெற்று, FILE_BEGIN)) {
		retval = GetLastError();
		செல் error_out;
	}
	எனில் ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		எனில் (!ReadFile(data->handle, buf, size, &actual, வெற்று)) {
			retval = GetLastError();
			செல் error_out;
		}
		எனில் (actual != size) {
	short_read:
		எனில் (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} இல்லையெனில்
			retval = EXT2_ET_SHORT_READ;
		செல் error_out;
		}
	திரும்பு 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	போது (size > 0) {
		எனில் (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, வெற்று)) {
			retval = GetLastError();
			செல் error_out;
		}
		எனில் (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			செல் short_read;
		}
		actual = size;
		எனில் (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	திரும்பு 0;

error_out:
	எனில் (actual >= 0 && actual < size)
		memset((எழுத்து *) buf+actual, 0, size-actual);
	எனில் (channel->read_error)
		retval = (channel->read_error)(channel, block, மொத்தம், buf,
					       size, actual, retval);
	திரும்பு retval;
}

நிலையான errcode_t raw_write_blk(io_channel channel,
			       கட்டமைப்பு windows_private_data *data,
			       unsigned long long block,
			       முழு மொத்தம், மாறிலி வெறுமை *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	மாறிலி unsigned எழுத்து *buf = bufv;

	எனில் (மொத்தம் == 1)
		size = channel->block_size;
	இல்லையெனில் {
		எனில் (மொத்தம் < 0)
			size = -மொத்தம்;
		இல்லையெனில்
			size = மொத்தம் * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	எனில் (data->flags & IO_FLAG_FORCE_BOUNCE) {
		எனில் (!SetFilePointerEx(data->handle, make_large_integer(location), வெற்று, FILE_BEGIN)) {
			retval = GetLastError();
			செல் error_out;
		}
	செல் bounce_write;
	}

		எனில் (!SetFilePointerEx(data->handle, make_large_integer(location), வெற்று, FILE_BEGIN)) {
			retval = GetLastError();
		செல் error_out;
	}

	SetLastError(0);

	எனில் ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		எனில் (!WriteFile(data->handle, buf, size, &actual, வெற்று)) {
			retval = GetLastError();
			செல் error_out;
		}

		எனில் (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			செல் error_out;
		}
		திரும்பு 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	போது (size > 0) {
		எனில் (size < channel->block_size) {
			எனில் (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, வெற்று)) {
				retval = GetLastError();
				செல் error_out;
			}
			எனில் (actual != channel->block_size) {
				எனில் (actual < 0) {
					retval = GetLastError();
					செல் error_out;
				}
				memset((எழுத்து *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		எனில் (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		எனில் (!SetFilePointerEx(data->handle, make_large_integer(location), வெற்று, FILE_BEGIN)) {
			retval = GetLastError();
			செல் error_out;
		}
		எனில் (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, வெற்று)) {
			retval = GetLastError();
			செல் error_out;
		}

		எனில் (actual != channel->block_size)
			செல் short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	திரும்பு 0;

error_out:
	எனில் (channel->write_error)
		retval = (channel->write_error)(channel, block, மொத்தம், buf,
						size, actual, retval);
	திரும்பு retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
நிலையான errcode_t alloc_cache(io_channel channel,
			     கட்டமைப்பு windows_private_data *data)
{
	errcode_t		retval;
	கட்டமைப்பு windows_cache	*cache;
	முழு			i;

	data->access_time = 0;
	க்கு (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		எனில் (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		எனில் (retval)
			திரும்பு retval;
	}
	எனில் (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		எனில் (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	திரும்பு retval;
}

/* Free the cache buffers */
நிலையான வெறுமை free_cache(கட்டமைப்பு windows_private_data *data)
{
	கட்டமைப்பு windows_cache    *cache;
	முழு			i;

	data->access_time = 0;
	க்கு (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		எனில் (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	எனில் (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
நிலையான கட்டமைப்பு windows_cache *find_cached_block(கட்டமைப்பு windows_private_data *data,
					    unsigned long long block,
					    கட்டமைப்பு windows_cache **eldest)
{
	கட்டமைப்பு windows_cache	*cache, *unused_cache, *oldest_cache;
	முழு			i;

	unused_cache = oldest_cache = 0;
	க்கு (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		எனில் (!cache->in_use) {
			எனில் (!unused_cache)
				unused_cache = cache;
			தொடர்;
		}
		எனில் (cache->block == block) {
			cache->access_time = ++data->access_time;
			திரும்பு cache;
		}
		எனில் (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	எனில் (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	திரும்பு 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
நிலையான வெறுமை reuse_cache(io_channel channel, கட்டமைப்பு windows_private_data *data,
		 கட்டமைப்பு windows_cache *cache, unsigned long long block)
{
	எனில் (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
நிலையான errcode_t flush_cached_blocks(io_channel channel,
				     கட்டமைப்பு windows_private_data *data,
				     முழு invalidate)
{
	கட்டமைப்பு windows_cache	*cache;
	errcode_t	retval, retval2;
	முழு			i;

	retval2 = 0;
	க்கு (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		எனில் (!cache->in_use)
			தொடர்;

		எனில் (invalidate)
			cache->in_use = 0;

		எனில் (!cache->dirty)
			தொடர்;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		எனில் (retval)
			retval2 = retval;
		இல்லையெனில்
			cache->dirty = 0;
	}
	திரும்பு retval2;
}
#endif /* NO_IO_CACHE */

நிலையான errcode_t windows_open_channel(கட்டமைப்பு windows_private_data *data,
				   முழு flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = வெற்று;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_io_channel), &io);
	எனில் (retval)
		செல் cleanup;
	memset(io, 0, அளவு(கட்டமைப்பு struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	எனில் (retval)
		செல் cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#எனில் defined(O_DIRECT)
	எனில் (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	எனில் (ext2fs_fstat(data->dev, &st) == 0) {
		எனில் (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		இல்லையெனில்
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	எனில் ((retval = alloc_cache(io, data)))
		செல் cleanup;

#ifdef BLKROGET
	எனில் (flags & IO_FLAG_RW) {
		முழு error;
		முழு readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		எனில் (!error && readonly) {
			retval = EPERM;
			செல் cleanup;
		}
	}
#endif

	*channel = io;
	திரும்பு 0;

cleanup:
	எனில் (data) {
		எனில் (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	எனில் (io) {
		எனில் (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	திரும்பு retval;
}

நிலையான DWORD windows_open_device(கட்டமைப்பு windows_private_data *data, முழு open_flags)
{
	DWORD ret = 0;

	எனில் (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	இல்லையெனில் எனில் (fake_dos_name_for_device(data))
		திரும்பு -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#எனில் defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#இல்லையெனில்
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, வெற்று, OPEN_EXISTING,
				  flags_and_attributes, வெற்று);

	எனில் (data->handle == INVALID_HANDLE_மதிப்பு) {
		ret = GetLastError();
		செல் invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	எனில் (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		செல் osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	திரும்பு 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	திரும்பு ret;
}

நிலையான கட்டமைப்பு windows_private_data *init_private_data(மாறிலி எழுத்து *name, முழு flags)
{
	கட்டமைப்பு windows_private_data *data = வெற்று;

	எனில் (ext2fs_get_mem(அளவு(கட்டமைப்பு windows_private_data), &data))
		திரும்பு வெற்று;

	memset(data, 0, அளவு(கட்டமைப்பு windows_private_data));
	strncpy(data->name, name, அளவு(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.இலக்கம்_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_மதிப்பு;

	திரும்பு data;
}

நிலையான errcode_t windows_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	முழு open_flags;
	கட்டமைப்பு windows_private_data *data;

	எனில் (name == 0)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	எனில் (!data)
		திரும்பு EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	எனில் (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#எனில் defined(O_DIRECT)
	எனில் (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	எனில் (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;
	}

	திரும்பு windows_open_channel(data, flags, channel, windows_io_manager);
}

நிலையான errcode_t windows_close(io_channel channel)
{
	கட்டமைப்பு windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
		திரும்பு 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	எனில் (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	எனில் (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	திரும்பு retval;
}

நிலையான errcode_t windows_set_blksize(io_channel channel, முழு blksize)
{
	கட்டமைப்பு windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	எனில் (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		எனில் ((retval = flush_cached_blocks(channel, data, 0)))
			திரும்பு retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		எனில் ((retval = alloc_cache(channel, data)))
			திரும்பு retval;
	}
	திரும்பு 0;
}

நிலையான errcode_t windows_read_blk64(io_channel channel, unsigned long long block, முழு மொத்தம், வெறுமை *buf)
{
	கட்டமைப்பு windows_private_data *data;
	கட்டமைப்பு windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	எழுத்து		*cp;
	முழு		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	திரும்பு raw_read_blk(channel, data, block, மொத்தம், buf);
#இல்லையெனில்
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	எனில் (மொத்தம் < 0 || மொத்தம் > WRITE_DIRECT_SIZE) {
		எனில் ((retval = flush_cached_blocks(channel, data, 0)))
			திரும்பு retval;
		திரும்பு raw_read_blk(channel, data, block, மொத்தம், buf);
	}

	cp = buf;
	போது (மொத்தம் > 0) {
		/* If it's in the cache, use it! */
		எனில் ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			மொத்தம்--;
			block++;
			cp += channel->block_size;
			தொடர்;
		}
		எனில் (மொத்தம் == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			எனில் ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				திரும்பு retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			திரும்பு 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		க்கு (i=1; i < மொத்தம்; i++)
			எனில் (find_cached_block(data, block+i, &reuse[i]))
				நிறுத்து;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		எனில் ((retval = raw_read_blk(channel, data, block, i, cp)))
			திரும்பு retval;

		/* Save the results in the cache */
		க்கு (j=0; j < i; j++) {
			மொத்தம்--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	திரும்பு 0;
#endif /* NO_IO_CACHE */
}

நிலையான errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	திரும்பு windows_read_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு windows_private_data *data;
	கட்டமைப்பு windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	மாறிலி எழுத்து	*cp;
	முழு		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	திரும்பு raw_write_blk(channel, data, block, மொத்தம், buf);
#இல்லையெனில்
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	எனில் (மொத்தம் < 0 || மொத்தம் > WRITE_DIRECT_SIZE) {
		எனில் ((retval = flush_cached_blocks(channel, data, 1)))
			திரும்பு retval;
		திரும்பு raw_write_blk(channel, data, block, மொத்தம், buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	எனில் (writethrough)
		retval = raw_write_blk(channel, data, block, மொத்தம், buf);

	cp = buf;
	போது (மொத்தம் > 0) {
		cache = find_cached_block(data, block, &reuse);
		எனில் (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		எனில் (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		மொத்தம்--;
		block++;
		cp += channel->block_size;
	}
	திரும்பு retval;
#endif /* NO_IO_CACHE */
}

நிலையான errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long மொத்தம்)
{
	திரும்பு EXT2_ET_OP_NOT_SUPPORTED;
}

நிலையான errcode_t windows_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	திரும்பு windows_write_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 முழு size, மாறிலி வெறுமை *buf)
{
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
நிலையான errcode_t windows_flush(io_channel channel)
{
	கட்டமைப்பு windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	திரும்பு retval;
}

நிலையான errcode_t windows_set_option(io_channel channel, மாறிலி எழுத்து *option,
				 மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு windows_private_data *data;
	unsigned long long tmp;
	எழுத்து *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	எனில் (!strcmp(option, "offset")) {
		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		எனில் (*end)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		எனில் (data->offset < 0)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		திரும்பு 0;
	}
	திரும்பு EXT2_ET_INVALID_ARGUMENT;
}

நிலையான errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long மொத்தம்)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic தள்ளு
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
நிலையான errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு windows_private_data *data;
	முழு		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	எனில் (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		செல் unimplemented;
	} இல்லையெனில் {
		/* Regular file, try to use truncate/punch/zero. */
		கட்டமைப்பு stat statbuf;

		எனில் (மொத்தம் == 0)
			திரும்பு 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		எனில் (ret)
			செல் err;
		எனில் ((unsigned long long) statbuf.st_size <
			(block + மொத்தம்) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + மொத்தம்) * channel->block_size + data->offset);
			எனில் (ret)
				செல் err;
		}
		செல் unimplemented;
	}
err:
	எனில் (ret < 0) {
		எனில் (errno == EOPNOTSUPP)
			செல் unimplemented;
		திரும்பு errno;
	}
	திரும்பு 0;
unimplemented:
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

முழு ext2fs_open_file(மாறிலி எழுத்து *pathname, முழு flags, mode_t mode)
{
	flags |= O_இருமம்;

	எனில் (mode)
#எனில் defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		திரும்பு open64(pathname, flags, mode);
	இல்லையெனில்
		திரும்பு open64(pathname, flags);
#இல்லையெனில்
		திரும்பு open(pathname, flags, mode);
	இல்லையெனில்
		திரும்பு open(pathname, flags);
#endif
}

முழு ext2fs_stat(மாறிலி எழுத்து *path, ext2fs_struct_stat *buf)
{
#எனில் defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	திரும்பு stat64(path, buf);
#இல்லையெனில்
	திரும்பு stat(path, buf);
#endif
}

முழு ext2fs_fstat(முழு fd, ext2fs_struct_stat *buf)
{
#எனில் defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	திரும்பு fstat64(fd, buf);
#இல்லையெனில்
	திரும்பு fstat(fd, buf);
#endif
}

#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic எடு
#endif

நிலையான கட்டமைப்பு struct_io_manager struct_windows_manager = {
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
