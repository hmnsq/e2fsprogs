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

#ଅନ୍ତର୍ଭାଵ <windows.h>
#ଅନ୍ତର୍ଭାଵ <winioctl.h>
#ଅନ୍ତର୍ଭାଵ <io.h>

#ଯଦି !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    ଅକ୍ଷର __log[256];\
		    snprintf(__log, ଆକାର(__log), __VA_ARGS__);\
		    __log[ଆକାର(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ଅନ୍ୟଥା
#define TRACE(...) କର { } ଯେତେବେଳେ (0);
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)
#define EXT2_CHECK_MAGIC_RETURN(ସଂରଚନା, code, ret) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ସଂରଚନା windows_cache {
	ଅକ୍ଷର			*buf;
	unsigned long long	block;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ସଂରଚନା windows_private_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	magic;
	ଅକ୍ଷର	name[MAX_PATH];
	HANDLE	handle;
	ଅକ୍ଷର	dos_device[MAX_PATH];
	ଅକ୍ଷର	cf_device[MAX_PATH];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	dev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	align;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	access_time;
	ext2_loff_t offset;
	ସଂରଚନା windows_cache cache[CACHE_SIZE];
	ଶୂନ୍ୟ	*bounce;
	ସଂରଚନା struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fake_dos_name_for_device(ସଂରଚନା windows_private_data *data)
{
	ଯଦି (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ଫେରନ୍ତୁ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	ଯଦି (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ଫେରନ୍ତୁ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ remove_fake_dos_name(ସଂରଚନା windows_private_data *data)
{
	ଯଦି (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

ସ୍ଥିର errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ସଂରଚନା windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ଯଦି (stats)
		*stats = &data->io_stats;

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର LARGE_INTEGER make_large_integer(LONGLONG ମୂଲ୍ୟ)
{
	LARGE_INTEGER	li;

	li.QuadPart = ମୂଲ୍ୟ;
	ଫେରନ୍ତୁ li;
}

/*
 * Here are the raw I/O functions
 */
ସ୍ଥିର errcode_t raw_read_blk(io_channel channel,
			      ସଂରଚନା windows_private_data *data,
			      unsigned long long block,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned ଅକ୍ଷର	*buf = bufv;
	ssize_t		really_read = 0;

	size = (ଗଣନା_2 < 0) ? -ଗଣନା_2 : ଗଣନା_2 * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ଯଦି (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ଯଦି (!SetFilePointerEx(data->handle, make_large_integer(location), ଖାଲି, FILE_BEGIN)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}
		ଯାଅ bounce_read;
	}

	ଯଦି (!SetFilePointerEx(data->handle, make_large_integer(location), ଖାଲି, FILE_BEGIN)) {
		retval = GetLastError();
		ଯାଅ error_out;
	}
	ଯଦି ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ଯଦି (!ReadFile(data->handle, buf, size, &actual, ଖାଲି)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}
		ଯଦି (actual != size) {
	short_read:
		ଯଦି (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} ଅନ୍ୟଥା
			retval = EXT2_ET_SHORT_READ;
		ଯାଅ error_out;
		}
	ଫେରନ୍ତୁ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ଯେତେବେଳେ (size > 0) {
		ଯଦି (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ଖାଲି)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}
		ଯଦି (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			ଯାଅ short_read;
		}
		actual = size;
		ଯଦି (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ଫେରନ୍ତୁ 0;

error_out:
	ଯଦି (actual >= 0 && actual < size)
		memset((ଅକ୍ଷର *) buf+actual, 0, size-actual);
	ଯଦି (channel->read_error)
		retval = (channel->read_error)(channel, block, ଗଣନା_2, buf,
					       size, actual, retval);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t raw_write_blk(io_channel channel,
			       ସଂରଚନା windows_private_data *data,
			       unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ଅଚଳ unsigned ଅକ୍ଷର *buf = bufv;

	ଯଦି (ଗଣନା_2 == 1)
		size = channel->block_size;
	ଅନ୍ୟଥା {
		ଯଦି (ଗଣନା_2 < 0)
			size = -ଗଣନା_2;
		ଅନ୍ୟଥା
			size = ଗଣନା_2 * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ଯଦି (data->flags & IO_FLAG_FORCE_BOUNCE) {
		ଯଦି (!SetFilePointerEx(data->handle, make_large_integer(location), ଖାଲି, FILE_BEGIN)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}
	ଯାଅ bounce_write;
	}

		ଯଦି (!SetFilePointerEx(data->handle, make_large_integer(location), ଖାଲି, FILE_BEGIN)) {
			retval = GetLastError();
		ଯାଅ error_out;
	}

	SetLastError(0);

	ଯଦି ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		ଯଦି (!WriteFile(data->handle, buf, size, &actual, ଖାଲି)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}

		ଯଦି (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ଯାଅ error_out;
		}
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ଯେତେବେଳେ (size > 0) {
		ଯଦି (size < channel->block_size) {
			ଯଦି (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ଖାଲି)) {
				retval = GetLastError();
				ଯାଅ error_out;
			}
			ଯଦି (actual != channel->block_size) {
				ଯଦି (actual < 0) {
					retval = GetLastError();
					ଯାଅ error_out;
				}
				memset((ଅକ୍ଷର *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		ଯଦି (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		ଯଦି (!SetFilePointerEx(data->handle, make_large_integer(location), ଖାଲି, FILE_BEGIN)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}
		ଯଦି (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, ଖାଲି)) {
			retval = GetLastError();
			ଯାଅ error_out;
		}

		ଯଦି (actual != channel->block_size)
			ଯାଅ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ଫେରନ୍ତୁ 0;

error_out:
	ଯଦି (channel->write_error)
		retval = (channel->write_error)(channel, block, ଗଣନା_2, buf,
						size, actual, retval);
	ଫେରନ୍ତୁ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ସ୍ଥିର errcode_t alloc_cache(io_channel channel,
			     ସଂରଚନା windows_private_data *data)
{
	errcode_t		retval;
	ସଂରଚନା windows_cache	*cache;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;

	data->access_time = 0;
	ପାଇଁ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ଯଦି (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ଯଦି (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ଯଦି (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ଫେରନ୍ତୁ retval;
}

/* Free the cache buffers */
ସ୍ଥିର ଶୂନ୍ୟ free_cache(ସଂରଚନା windows_private_data *data)
{
	ସଂରଚନା windows_cache    *cache;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;

	data->access_time = 0;
	ପାଇଁ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ଯଦି (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ଯଦି (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ସ୍ଥିର ସଂରଚନା windows_cache *find_cached_block(ସଂରଚନା windows_private_data *data,
					    unsigned long long block,
					    ସଂରଚନା windows_cache **eldest)
{
	ସଂରଚନା windows_cache	*cache, *unused_cache, *oldest_cache;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;

	unused_cache = oldest_cache = 0;
	ପାଇଁ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ଯଦି (!cache->in_use) {
			ଯଦି (!unused_cache)
				unused_cache = cache;
			ଜାରି;
		}
		ଯଦି (cache->block == block) {
			cache->access_time = ++data->access_time;
			ଫେରନ୍ତୁ cache;
		}
		ଯଦି (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ଯଦି (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ଫେରନ୍ତୁ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ସ୍ଥିର ଶୂନ୍ୟ reuse_cache(io_channel channel, ସଂରଚନା windows_private_data *data,
		 ସଂରଚନା windows_cache *cache, unsigned long long block)
{
	ଯଦି (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
ସ୍ଥିର errcode_t flush_cached_blocks(io_channel channel,
				     ସଂରଚନା windows_private_data *data,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା invalidate)
{
	ସଂରଚନା windows_cache	*cache;
	errcode_t	retval, retval2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;

	retval2 = 0;
	ପାଇଁ (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		ଯଦି (!cache->in_use)
			ଜାରି;

		ଯଦି (invalidate)
			cache->in_use = 0;

		ଯଦି (!cache->dirty)
			ଜାରି;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		ଯଦି (retval)
			retval2 = retval;
		ଅନ୍ୟଥା
			cache->dirty = 0;
	}
	ଫେରନ୍ତୁ retval2;
}
#endif /* NO_IO_CACHE */

ସ୍ଥିର errcode_t windows_open_channel(ସଂରଚନା windows_private_data *data,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ଖାଲି;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_io_channel), &io);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(io, 0, ଆକାର(ସଂରଚନା struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	ଯଦି (retval)
		ଯାଅ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ଯଦି defined(O_DIRECT)
	ଯଦି (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ଯଦି (ext2fs_fstat(data->dev, &st) == 0) {
		ଯଦି (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		ଅନ୍ୟଥା
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ଯଦି ((retval = alloc_cache(io, data)))
		ଯାଅ cleanup;

#ifdef BLKROGET
	ଯଦି (flags & IO_FLAG_RW) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ଯଦି (!error && readonly) {
			retval = EPERM;
			ଯାଅ cleanup;
		}
	}
#endif

	*channel = io;
	ଫେରନ୍ତୁ 0;

cleanup:
	ଯଦି (data) {
		ଯଦି (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	ଯଦି (io) {
		ଯଦି (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର DWORD windows_open_device(ସଂରଚନା windows_private_data *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags)
{
	DWORD ret = 0;

	ଯଦି (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	ଅନ୍ୟଥା ଯଦି (fake_dos_name_for_device(data))
		ଫେରନ୍ତୁ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#ଯଦି defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#ଅନ୍ୟଥା
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, ଖାଲି, OPEN_EXISTING,
				  flags_and_attributes, ଖାଲି);

	ଯଦି (data->handle == INVALID_HANDLE_ମୂଲ୍ୟ) {
		ret = GetLastError();
		ଯାଅ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	ଯଦି (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		ଯାଅ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ଫେରନ୍ତୁ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ସଂରଚନା windows_private_data *init_private_data(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା windows_private_data *data = ଖାଲି;

	ଯଦି (ext2fs_get_mem(ଆକାର(ସଂରଚନା windows_private_data), &data))
		ଫେରନ୍ତୁ ଖାଲି;

	memset(data, 0, ଆକାର(ସଂରଚନା windows_private_data));
	strncpy(data->name, name, ଆକାର(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.ଅଙ୍କ_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_ମୂଲ୍ୟ;

	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର errcode_t windows_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags;
	ସଂରଚନା windows_private_data *data;

	ଯଦି (name == 0)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	ଯଦି (!data)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ଯଦି (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ଯଦି defined(O_DIRECT)
	ଯଦି (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	ଯଦି (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
	}

	ଫେରନ୍ତୁ windows_open_channel(data, flags, channel, windows_io_manager);
}

ସ୍ଥିର errcode_t windows_close(io_channel channel)
{
	ସଂରଚନା windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
		ଫେରନ୍ତୁ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	ଯଦି (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	ଯଦି (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t windows_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	ସଂରଚନା windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ଯଦି (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		ଯଦି ((retval = flush_cached_blocks(channel, data, 0)))
			ଫେରନ୍ତୁ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		ଯଦି ((retval = alloc_cache(channel, data)))
			ଫେରନ୍ତୁ retval;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t windows_read_blk64(io_channel channel, unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା windows_private_data *data;
	ସଂରଚନା windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	ଅକ୍ଷର		*cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ଫେରନ୍ତୁ raw_read_blk(channel, data, block, ଗଣନା_2, buf);
#ଅନ୍ୟଥା
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ଯଦି (ଗଣନା_2 < 0 || ଗଣନା_2 > WRITE_DIRECT_SIZE) {
		ଯଦି ((retval = flush_cached_blocks(channel, data, 0)))
			ଫେରନ୍ତୁ retval;
		ଫେରନ୍ତୁ raw_read_blk(channel, data, block, ଗଣନା_2, buf);
	}

	cp = buf;
	ଯେତେବେଳେ (ଗଣନା_2 > 0) {
		/* If it's in the cache, use it! */
		ଯଦି ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			ଗଣନା_2--;
			block++;
			cp += channel->block_size;
			ଜାରି;
		}
		ଯଦି (ଗଣନା_2 == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			ଯଦି ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ଫେରନ୍ତୁ retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ଫେରନ୍ତୁ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ପାଇଁ (i=1; i < ଗଣନା_2; i++)
			ଯଦି (find_cached_block(data, block+i, &reuse[i]))
				ଭାଙ୍ଗ;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		ଯଦି ((retval = raw_read_blk(channel, data, block, i, cp)))
			ଫେରନ୍ତୁ retval;

		/* Save the results in the cache */
		ପାଇଁ (j=0; j < i; j++) {
			ଗଣନା_2--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ଫେରନ୍ତୁ 0;
#endif /* NO_IO_CACHE */
}

ସ୍ଥିର errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ windows_read_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା windows_private_data *data;
	ସଂରଚନା windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ଅଚଳ ଅକ୍ଷର	*cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ଫେରନ୍ତୁ raw_write_blk(channel, data, block, ଗଣନା_2, buf);
#ଅନ୍ୟଥା
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ଯଦି (ଗଣନା_2 < 0 || ଗଣନା_2 > WRITE_DIRECT_SIZE) {
		ଯଦି ((retval = flush_cached_blocks(channel, data, 1)))
			ଫେରନ୍ତୁ retval;
		ଫେରନ୍ତୁ raw_write_blk(channel, data, block, ଗଣନା_2, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ଯଦି (writethrough)
		retval = raw_write_blk(channel, data, block, ଗଣନା_2, buf);

	cp = buf;
	ଯେତେବେଳେ (ଗଣନା_2 > 0) {
		cache = find_cached_block(data, block, &reuse);
		ଯଦି (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		ଯଦି (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		ଗଣନା_2--;
		block++;
		cp += channel->block_size;
	}
	ଫେରନ୍ତୁ retval;
#endif /* NO_IO_CACHE */
}

ସ୍ଥିର errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ଗଣନା_2)
{
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
}

ସ୍ଥିର errcode_t windows_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ windows_write_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ସ୍ଥିର errcode_t windows_flush(io_channel channel)
{
	ସଂରଚନା windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t windows_set_option(io_channel channel, ଅଚଳ ଅକ୍ଷର *option,
				 ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା windows_private_data *data;
	unsigned long long tmp;
	ଅକ୍ଷର *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ଯଦି (!strcmp(option, "offset")) {
		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ଯଦି (*end)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ଯଦି (data->offset < 0)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
}

ସ୍ଥିର errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long ଗଣନା_2)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ଠେଲ
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ସ୍ଥିର errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା windows_private_data *data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ଯଦି (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		ଯାଅ unimplemented;
	} ଅନ୍ୟଥା {
		/* Regular file, try to use truncate/punch/zero. */
		ସଂରଚନା stat statbuf;

		ଯଦି (ଗଣନା_2 == 0)
			ଫେରନ୍ତୁ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ଯଦି (ret)
			ଯାଅ err;
		ଯଦି ((unsigned long long) statbuf.st_size <
			(block + ଗଣନା_2) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + ଗଣନା_2) * channel->block_size + data->offset);
			ଯଦି (ret)
				ଯାଅ err;
		}
		ଯାଅ unimplemented;
	}
err:
	ଯଦି (ret < 0) {
		ଯଦି (errno == EOPNOTSUPP)
			ଯାଅ unimplemented;
		ଫେରନ୍ତୁ errno;
	}
	ଫେରନ୍ତୁ 0;
unimplemented:
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_open_file(ଅଚଳ ଅକ୍ଷର *pathname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, mode_t mode)
{
	flags |= O_ଦ୍ୱିମିକ;

	ଯଦି (mode)
#ଯଦି defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ଫେରନ୍ତୁ open64(pathname, flags, mode);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open64(pathname, flags);
#ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open(pathname, flags, mode);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open(pathname, flags);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_stat(ଅଚଳ ଅକ୍ଷର *path, ext2fs_struct_stat *buf)
{
#ଯଦି defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ଫେରନ୍ତୁ stat64(path, buf);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ stat(path, buf);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_fstat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2fs_struct_stat *buf)
{
#ଯଦି defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ଫେରନ୍ତୁ fstat64(fd, buf);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ fstat(fd, buf);
#endif
}

#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic କାଢ଼
#endif

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_windows_manager = {
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
