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

#অন্তর্ভাৱ <windows.h>
#অন্তর্ভাৱ <winioctl.h>
#অন্তর্ভাৱ <io.h>

#যদি !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#অন্তর্ভাৱ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    ৱর্ণ __log[256];\
		    snprintf(__log, পরিমাণ(__log), __VA_ARGS__);\
		    __log[পরিমাণ(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#অন্যথা
#define TRACE(...) কৰা { } যেতিয়ালৈ (0);
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) ঘূৰাই (code)
#define EXT2_CHECK_MAGIC_RETURN(সংরচনা, code, ret) \
	  যদি ((সংরচনা)->magic != (code)) ঘূৰাই (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

সংরচনা windows_cache {
	ৱর্ণ			*buf;
	unsigned long long	block;
	পূর্ণাঙ্ক			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

সংরচনা windows_private_data {
	পূর্ণাঙ্ক	magic;
	ৱর্ণ	name[MAX_PATH];
	HANDLE	handle;
	ৱর্ণ	dos_device[MAX_PATH];
	ৱর্ণ	cf_device[MAX_PATH];
	পূর্ণাঙ্ক	dev;
	পূর্ণাঙ্ক	flags;
	পূর্ণাঙ্ক	align;
	পূর্ণাঙ্ক	access_time;
	ext2_loff_t offset;
	সংরচনা windows_cache cache[CACHE_SIZE];
	শূন্য	*bounce;
	সংরচনা struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

স্থির পূর্ণাঙ্ক fake_dos_name_for_device(সংরচনা windows_private_data *data)
{
	যদি (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		ঘূৰাই 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	যদি (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		ঘূৰাই 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	ঘূৰাই 0;
}

স্থির শূন্য remove_fake_dos_name(সংরচনা windows_private_data *data)
{
	যদি (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

স্থির errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	সংরচনা windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (stats)
		*stats = &data->io_stats;

	ঘূৰাই retval;
}

স্থির LARGE_INTEGER make_large_integer(LONGLONG মান)
{
	LARGE_INTEGER	li;

	li.QuadPart = মান;
	ঘূৰাই li;
}

/*
 * Here are the raw I/O functions
 */
স্থির errcode_t raw_read_blk(io_channel channel,
			      সংরচনা windows_private_data *data,
			      unsigned long long block,
			      পূর্ণাঙ্ক গুনতি, শূন্য *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned ৱর্ণ	*buf = bufv;
	ssize_t		really_read = 0;

	size = (গুনতি < 0) ? -গুনতি : গুনতি * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	যদি (data->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (!SetFilePointerEx(data->handle, make_large_integer(location), রিক্ত, FILE_BEGIN)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}
		গচ্ছ bounce_read;
	}

	যদি (!SetFilePointerEx(data->handle, make_large_integer(location), রিক্ত, FILE_BEGIN)) {
		retval = GetLastError();
		গচ্ছ error_out;
	}
	যদি ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		যদি (!ReadFile(data->handle, buf, size, &actual, রিক্ত)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}
		যদি (actual != size) {
	short_read:
		যদি (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} অন্যথা
			retval = EXT2_ET_SHORT_READ;
		গচ্ছ error_out;
		}
	ঘূৰাই 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	যেতিয়ালৈ (size > 0) {
		যদি (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, রিক্ত)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}
		যদি (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			গচ্ছ short_read;
		}
		actual = size;
		যদি (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	ঘূৰাই 0;

error_out:
	যদি (actual >= 0 && actual < size)
		memset((ৱর্ণ *) buf+actual, 0, size-actual);
	যদি (channel->read_error)
		retval = (channel->read_error)(channel, block, গুনতি, buf,
					       size, actual, retval);
	ঘূৰাই retval;
}

স্থির errcode_t raw_write_blk(io_channel channel,
			       সংরচনা windows_private_data *data,
			       unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ধ্রুৱ unsigned ৱর্ণ *buf = bufv;

	যদি (গুনতি == 1)
		size = channel->block_size;
	অন্যথা {
		যদি (গুনতি < 0)
			size = -গুনতি;
		অন্যথা
			size = গুনতি * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	যদি (data->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (!SetFilePointerEx(data->handle, make_large_integer(location), রিক্ত, FILE_BEGIN)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}
	গচ্ছ bounce_write;
	}

		যদি (!SetFilePointerEx(data->handle, make_large_integer(location), রিক্ত, FILE_BEGIN)) {
			retval = GetLastError();
		গচ্ছ error_out;
	}

	SetLastError(0);

	যদি ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		যদি (!WriteFile(data->handle, buf, size, &actual, রিক্ত)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}

		যদি (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			গচ্ছ error_out;
		}
		ঘূৰাই 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	যেতিয়ালৈ (size > 0) {
		যদি (size < channel->block_size) {
			যদি (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, রিক্ত)) {
				retval = GetLastError();
				গচ্ছ error_out;
			}
			যদি (actual != channel->block_size) {
				যদি (actual < 0) {
					retval = GetLastError();
					গচ্ছ error_out;
				}
				memset((ৱর্ণ *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		যদি (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		যদি (!SetFilePointerEx(data->handle, make_large_integer(location), রিক্ত, FILE_BEGIN)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}
		যদি (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, রিক্ত)) {
			retval = GetLastError();
			গচ্ছ error_out;
		}

		যদি (actual != channel->block_size)
			গচ্ছ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	ঘূৰাই 0;

error_out:
	যদি (channel->write_error)
		retval = (channel->write_error)(channel, block, গুনতি, buf,
						size, actual, retval);
	ঘূৰাই retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
স্থির errcode_t alloc_cache(io_channel channel,
			     সংরচনা windows_private_data *data)
{
	errcode_t		retval;
	সংরচনা windows_cache	*cache;
	পূর্ণাঙ্ক			i;

	data->access_time = 0;
	বাবে (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval)
			ঘূৰাই retval;
	}
	যদি (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ঘূৰাই retval;
}

/* Free the cache buffers */
স্থির শূন্য free_cache(সংরচনা windows_private_data *data)
{
	সংরচনা windows_cache    *cache;
	পূর্ণাঙ্ক			i;

	data->access_time = 0;
	বাবে (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	যদি (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
স্থির সংরচনা windows_cache *find_cached_block(সংরচনা windows_private_data *data,
					    unsigned long long block,
					    সংরচনা windows_cache **eldest)
{
	সংরচনা windows_cache	*cache, *unused_cache, *oldest_cache;
	পূর্ণাঙ্ক			i;

	unused_cache = oldest_cache = 0;
	বাবে (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			অনুৱর্ত;
		}
		যদি (cache->block == block) {
			cache->access_time = ++data->access_time;
			ঘূৰাই cache;
		}
		যদি (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	যদি (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ঘূৰাই 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
স্থির শূন্য reuse_cache(io_channel channel, সংরচনা windows_private_data *data,
		 সংরচনা windows_cache *cache, unsigned long long block)
{
	যদি (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
স্থির errcode_t flush_cached_blocks(io_channel channel,
				     সংরচনা windows_private_data *data,
				     পূর্ণাঙ্ক invalidate)
{
	সংরচনা windows_cache	*cache;
	errcode_t	retval, retval2;
	পূর্ণাঙ্ক			i;

	retval2 = 0;
	বাবে (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		যদি (!cache->in_use)
			অনুৱর্ত;

		যদি (invalidate)
			cache->in_use = 0;

		যদি (!cache->dirty)
			অনুৱর্ত;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		যদি (retval)
			retval2 = retval;
		অন্যথা
			cache->dirty = 0;
	}
	ঘূৰাই retval2;
}
#endif /* NO_IO_CACHE */

স্থির errcode_t windows_open_channel(সংরচনা windows_private_data *data,
				   পূর্ণাঙ্ক flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = রিক্ত;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা struct_io_channel), &io);
	যদি (retval)
		গচ্ছ cleanup;
	memset(io, 0, পরিমাণ(সংরচনা struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	যদি (retval)
		গচ্ছ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	যদি (ext2fs_fstat(data->dev, &st) == 0) {
		যদি (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		অন্যথা
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	যদি ((retval = alloc_cache(io, data)))
		গচ্ছ cleanup;

#ifdef BLKROGET
	যদি (flags & IO_FLAG_RW) {
		পূর্ণাঙ্ক error;
		পূর্ণাঙ্ক readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		যদি (!error && readonly) {
			retval = EPERM;
			গচ্ছ cleanup;
		}
	}
#endif

	*channel = io;
	ঘূৰাই 0;

cleanup:
	যদি (data) {
		যদি (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	যদি (io) {
		যদি (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ঘূৰাই retval;
}

স্থির DWORD windows_open_device(সংরচনা windows_private_data *data, পূর্ণাঙ্ক open_flags)
{
	DWORD ret = 0;

	যদি (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	অন্যথা যদি (fake_dos_name_for_device(data))
		ঘূৰাই -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#যদি defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#অন্যথা
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, রিক্ত, OPEN_EXISTING,
				  flags_and_attributes, রিক্ত);

	যদি (data->handle == INVALID_HANDLE_মান) {
		ret = GetLastError();
		গচ্ছ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	যদি (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		গচ্ছ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	ঘূৰাই 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ঘূৰাই ret;
}

স্থির সংরচনা windows_private_data *init_private_data(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags)
{
	সংরচনা windows_private_data *data = রিক্ত;

	যদি (ext2fs_get_mem(পরিমাণ(সংরচনা windows_private_data), &data))
		ঘূৰাই রিক্ত;

	memset(data, 0, পরিমাণ(সংরচনা windows_private_data));
	strncpy(data->name, name, পরিমাণ(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.অঙ্ক_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_মান;

	ঘূৰাই data;
}

স্থির errcode_t windows_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	পূর্ণাঙ্ক open_flags;
	সংরচনা windows_private_data *data;

	যদি (name == 0)
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	যদি (!data)
		ঘূৰাই EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	যদি (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	যদি (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;
	}

	ঘূৰাই windows_open_channel(data, flags, channel, windows_io_manager);
}

স্থির errcode_t windows_close(io_channel channel)
{
	সংরচনা windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ঘূৰাই 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	যদি (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ঘূৰাই retval;
}

স্থির errcode_t windows_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	সংরচনা windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		যদি ((retval = flush_cached_blocks(channel, data, 0)))
			ঘূৰাই retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		যদি ((retval = alloc_cache(channel, data)))
			ঘূৰাই retval;
	}
	ঘূৰাই 0;
}

স্থির errcode_t windows_read_blk64(io_channel channel, unsigned long long block, পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	সংরচনা windows_private_data *data;
	সংরচনা windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	ৱর্ণ		*cp;
	পূর্ণাঙ্ক		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ঘূৰাই raw_read_blk(channel, data, block, গুনতি, buf);
#অন্যথা
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_SIZE) {
		যদি ((retval = flush_cached_blocks(channel, data, 0)))
			ঘূৰাই retval;
		ঘূৰাই raw_read_blk(channel, data, block, গুনতি, buf);
	}

	cp = buf;
	যেতিয়ালৈ (গুনতি > 0) {
		/* If it's in the cache, use it! */
		যদি ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			গুনতি--;
			block++;
			cp += channel->block_size;
			অনুৱর্ত;
		}
		যদি (গুনতি == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			যদি ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ঘূৰাই retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			ঘূৰাই 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		বাবে (i=1; i < গুনতি; i++)
			যদি (find_cached_block(data, block+i, &reuse[i]))
				ভঙ্গ;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		যদি ((retval = raw_read_blk(channel, data, block, i, cp)))
			ঘূৰাই retval;

		/* Save the results in the cache */
		বাবে (j=0; j < i; j++) {
			গুনতি--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	ঘূৰাই 0;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	ঘূৰাই windows_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	সংরচনা windows_private_data *data;
	সংরচনা windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ধ্রুৱ ৱর্ণ	*cp;
	পূর্ণাঙ্ক		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ঘূৰাই raw_write_blk(channel, data, block, গুনতি, buf);
#অন্যথা
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_SIZE) {
		যদি ((retval = flush_cached_blocks(channel, data, 1)))
			ঘূৰাই retval;
		ঘূৰাই raw_write_blk(channel, data, block, গুনতি, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	যদি (writethrough)
		retval = raw_write_blk(channel, data, block, গুনতি, buf);

	cp = buf;
	যেতিয়ালৈ (গুনতি > 0) {
		cache = find_cached_block(data, block, &reuse);
		যদি (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		যদি (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		গুনতি--;
		block++;
		cp += channel->block_size;
	}
	ঘূৰাই retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;
}

স্থির errcode_t windows_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	ঘূৰাই windows_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণাঙ্ক size, ধ্রুৱ শূন্য *buf)
{
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t windows_flush(io_channel channel)
{
	সংরচনা windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	ঘূৰাই retval;
}

স্থির errcode_t windows_set_option(io_channel channel, ধ্রুৱ ৱর্ণ *option,
				 ধ্রুৱ ৱর্ণ *arg)
{
	সংরচনা windows_private_data *data;
	unsigned long long tmp;
	ৱর্ণ *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (!strcmp(option, "offset")) {
		যদি (!arg)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		যদি (*end)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		যদি (data->offset < 0)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
		ঘূৰাই 0;
	}
	ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
}

স্থির errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long গুনতি)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
স্থির errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা windows_private_data *data;
	পূর্ণাঙ্ক		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		গচ্ছ unimplemented;
	} অন্যথা {
		/* Regular file, try to use truncate/punch/zero. */
		সংরচনা stat statbuf;

		যদি (গুনতি == 0)
			ঘূৰাই 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		যদি (ret)
			গচ্ছ err;
		যদি ((unsigned long long) statbuf.st_size <
			(block + গুনতি) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + গুনতি) * channel->block_size + data->offset);
			যদি (ret)
				গচ্ছ err;
		}
		গচ্ছ unimplemented;
	}
err:
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP)
			গচ্ছ unimplemented;
		ঘূৰাই errno;
	}
	ঘূৰাই 0;
unimplemented:
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

পূর্ণাঙ্ক ext2fs_open_file(ধ্রুৱ ৱর্ণ *pathname, পূর্ণাঙ্ক flags, mode_t mode)
{
	flags |= O_দ্বিমিক;

	যদি (mode)
#যদি defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ঘূৰাই open64(pathname, flags, mode);
	অন্যথা
		ঘূৰাই open64(pathname, flags);
#অন্যথা
		ঘূৰাই open(pathname, flags, mode);
	অন্যথা
		ঘূৰাই open(pathname, flags);
#endif
}

পূর্ণাঙ্ক ext2fs_stat(ধ্রুৱ ৱর্ণ *path, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ঘূৰাই stat64(path, buf);
#অন্যথা
	ঘূৰাই stat(path, buf);
#endif
}

পূর্ণাঙ্ক ext2fs_fstat(পূর্ণাঙ্ক fd, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ঘূৰাই fstat64(fd, buf);
#অন্যথা
	ঘূৰাই fstat(fd, buf);
#endif
}

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বেৰ_কৰো
#endif

স্থির সংরচনা struct_io_manager struct_windows_manager = {
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
