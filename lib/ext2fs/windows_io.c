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

#অন্তর্ভাব <windows.h>
#অন্তর্ভাব <winioctl.h>
#অন্তর্ভাব <io.h>

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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#অন্তর্ভাব <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    অক্ষর __log[256];\
		    snprintf(__log, আকার(__log), __VA_ARGS__);\
		    __log[আকার(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#অন্যথা
#define TRACE(...) করো { } যখন (0);
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)
#define EXT2_CHECK_MAGIC_RETURN(কাঠামো, code, ret) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

কাঠামো windows_cache {
	অক্ষর			*buf;
	unsigned long long	block;
	পূর্ণসংখ্যা			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_আকার 8
#define WRITE_DIRECT_আকার 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_আকার 4	/* Should be smaller than CACHE_SIZE */

কাঠামো windows_private_তথ্য {
	পূর্ণসংখ্যা	magic;
	অক্ষর	name[MAX_PATH];
	HANDLE	handle;
	অক্ষর	dos_device[MAX_PATH];
	অক্ষর	cf_device[MAX_PATH];
	পূর্ণসংখ্যা	dev;
	পূর্ণসংখ্যা	flags;
	পূর্ণসংখ্যা	align;
	পূর্ণসংখ্যা	access_time;
	ext2_loff_t offset;
	কাঠামো windows_cache cache[CACHE_আকার];
	শূন্য	*bounce;
	কাঠামো struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

স্থির পূর্ণসংখ্যা fake_dos_name_for_device(কাঠামো windows_private_তথ্য *তথ্য)
{
	যদি (strncmp(তথ্য->name, "\\\\", 2) == 0) {
		তথ্য->dos_device[0] = 0;
		strcpy(তথ্য->cf_device, তথ্য->name);
		ফেরত 0;
	}

	_snprintf(তথ্য->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	যদি (!DefineDosDevice(DDD_RAW_TARGET_PATH, তথ্য->dos_device, তথ্য->name))
		ফেরত 1;

	_snprintf(তথ্য->cf_device, MAX_PATH, "\\\\.\\%s", তথ্য->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", তথ্য->dos_device);

	ফেরত 0;
}

স্থির শূন্য remove_fake_dos_name(কাঠামো windows_private_তথ্য *তথ্য)
{
	যদি (*তথ্য->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", তথ্য->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, তথ্য->dos_device, তথ্য->name);
	}
}

স্থির errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	কাঠামো windows_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (stats)
		*stats = &তথ্য->io_stats;

	ফেরত retval;
}

স্থির LARGE_INTEGER make_large_integer(LONGLONG মান)
{
	LARGE_INTEGER	li;

	li.QuadPart = মান;
	ফেরত li;
}

/*
 * Here are the raw I/O functions
 */
স্থির errcode_t raw_read_blk(io_channel channel,
			      কাঠামো windows_private_তথ্য *তথ্য,
			      unsigned long long block,
			      পূর্ণসংখ্যা গুনতি, শূন্য *bufv)
{
	errcode_t	retval;
	ssize_t	আকার_2;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned অক্ষর	*buf = bufv;
	ssize_t		really_read = 0;

	আকার_2 = (গুনতি < 0) ? -গুনতি : গুনতি * channel->block_আকার;
	তথ্য->io_stats.bytes_read += আকার_2;
	location = ((ext2_loff_t) block * channel->block_আকার) + তথ্য->offset;

	যদি (তথ্য->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (!SetFilePointerEx(তথ্য->handle, make_large_integer(location), ফাঁকা, FILE_BEGIN)) {
			retval = GetLastError();
			যাও error_out;
		}
		যাও bounce_read;
	}

	যদি (!SetFilePointerEx(তথ্য->handle, make_large_integer(location), ফাঁকা, FILE_BEGIN)) {
		retval = GetLastError();
		যাও error_out;
	}
	যদি ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(আকার_2, channel->align))) {
		যদি (!ReadFile(তথ্য->handle, buf, আকার_2, &actual, ফাঁকা)) {
			retval = GetLastError();
			যাও error_out;
		}
		যদি (actual != আকার_2) {
	short_read:
		যদি (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} অন্যথা
			retval = EXT2_ET_SHORT_READ;
		যাও error_out;
		}
	ফেরত 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	যখন (আকার_2 > 0) {
		যদি (!ReadFile(তথ্য->handle, তথ্য->bounce, channel->block_আকার, &actual, ফাঁকা)) {
			retval = GetLastError();
			যাও error_out;
		}
		যদি (actual != channel->block_আকার) {
			actual = really_read;
			buf -= really_read;
			আকার_2 += really_read;
			যাও short_read;
		}
		actual = আকার_2;
		যদি (আকার_2 > channel->block_আকার)
			actual = channel->block_আকার;
		memcpy(buf, তথ্য->bounce, actual);
		really_read += actual;
		আকার_2 -= actual;
		buf += actual;
	}
	ফেরত 0;

error_out:
	যদি (actual >= 0 && actual < আকার_2)
		memset((অক্ষর *) buf+actual, 0, আকার_2-actual);
	যদি (channel->read_error)
		retval = (channel->read_error)(channel, block, গুনতি, buf,
					       আকার_2, actual, retval);
	ফেরত retval;
}

স্থির errcode_t raw_write_blk(io_channel channel,
			       কাঠামো windows_private_তথ্য *তথ্য,
			       unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *bufv)
{
	ssize_t		আকার_2;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ধ্রুবক unsigned অক্ষর *buf = bufv;

	যদি (গুনতি == 1)
		আকার_2 = channel->block_আকার;
	অন্যথা {
		যদি (গুনতি < 0)
			আকার_2 = -গুনতি;
		অন্যথা
			আকার_2 = গুনতি * channel->block_আকার;
	}
	তথ্য->io_stats.bytes_written += আকার_2;

	location = ((ext2_loff_t) block * channel->block_আকার) + তথ্য->offset;

	যদি (তথ্য->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (!SetFilePointerEx(তথ্য->handle, make_large_integer(location), ফাঁকা, FILE_BEGIN)) {
			retval = GetLastError();
			যাও error_out;
		}
	যাও bounce_write;
	}

		যদি (!SetFilePointerEx(তথ্য->handle, make_large_integer(location), ফাঁকা, FILE_BEGIN)) {
			retval = GetLastError();
		যাও error_out;
	}

	SetLastError(0);

	যদি ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(আকার_2, channel->align))) {
		যদি (!WriteFile(তথ্য->handle, buf, আকার_2, &actual, ফাঁকা)) {
			retval = GetLastError();
			যাও error_out;
		}

		যদি (actual != আকার_2) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			যাও error_out;
		}
		ফেরত 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	যখন (আকার_2 > 0) {
		যদি (আকার_2 < channel->block_আকার) {
			যদি (!ReadFile(তথ্য->handle, তথ্য->bounce, channel->block_আকার, &actual, ফাঁকা)) {
				retval = GetLastError();
				যাও error_out;
			}
			যদি (actual != channel->block_আকার) {
				যদি (actual < 0) {
					retval = GetLastError();
					যাও error_out;
				}
				memset((অক্ষর *) তথ্য->bounce + actual, 0,
				       channel->block_আকার - actual);
			}
		}
		actual = আকার_2;
		যদি (আকার_2 > channel->block_আকার)
			actual = channel->block_আকার;
		memcpy(তথ্য->bounce, buf, actual);
		যদি (!SetFilePointerEx(তথ্য->handle, make_large_integer(location), ফাঁকা, FILE_BEGIN)) {
			retval = GetLastError();
			যাও error_out;
		}
		যদি (!WriteFile(তথ্য->handle, তথ্য->bounce, channel->block_আকার, &actual, ফাঁকা)) {
			retval = GetLastError();
			যাও error_out;
		}

		যদি (actual != channel->block_আকার)
			যাও short_write;
		আকার_2 -= actual;
		buf += actual;
		location += actual;
	}
	ফেরত 0;

error_out:
	যদি (channel->write_error)
		retval = (channel->write_error)(channel, block, গুনতি, buf,
						আকার_2, actual, retval);
	ফেরত retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
স্থির errcode_t alloc_cache(io_channel channel,
			     কাঠামো windows_private_তথ্য *তথ্য)
{
	errcode_t		retval;
	কাঠামো windows_cache	*cache;
	পূর্ণসংখ্যা			i;

	তথ্য->access_time = 0;
	জন্য (i=0, cache = তথ্য->cache; i < CACHE_আকার; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval)
			ফেরত retval;
	}
	যদি (channel->align || তথ্য->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (তথ্য->bounce)
			ext2fs_free_mem(&তথ্য->bounce);
		retval = io_channel_alloc_buf(channel, 0, &তথ্য->bounce);
	}
	ফেরত retval;
}

/* Free the cache buffers */
স্থির শূন্য free_cache(কাঠামো windows_private_তথ্য *তথ্য)
{
	কাঠামো windows_cache    *cache;
	পূর্ণসংখ্যা			i;

	তথ্য->access_time = 0;
	জন্য (i=0, cache = তথ্য->cache; i < CACHE_আকার; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	যদি (তথ্য->bounce)
		ext2fs_free_mem(&তথ্য->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
স্থির কাঠামো windows_cache *find_cached_block(কাঠামো windows_private_তথ্য *তথ্য,
					    unsigned long long block,
					    কাঠামো windows_cache **eldest)
{
	কাঠামো windows_cache	*cache, *unused_cache, *oldest_cache;
	পূর্ণসংখ্যা			i;

	unused_cache = oldest_cache = 0;
	জন্য (i=0, cache = তথ্য->cache; i < CACHE_আকার; i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			চালু;
		}
		যদি (cache->block == block) {
			cache->access_time = ++তথ্য->access_time;
			ফেরত cache;
		}
		যদি (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	যদি (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	ফেরত 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
স্থির শূন্য reuse_cache(io_channel channel, কাঠামো windows_private_তথ্য *তথ্য,
		 কাঠামো windows_cache *cache, unsigned long long block)
{
	যদি (cache->dirty && cache->in_use)
		raw_write_blk(channel, তথ্য, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++তথ্য->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
স্থির errcode_t flush_cached_blocks(io_channel channel,
				     কাঠামো windows_private_তথ্য *তথ্য,
				     পূর্ণসংখ্যা invalidate)
{
	কাঠামো windows_cache	*cache;
	errcode_t	retval, retval2;
	পূর্ণসংখ্যা			i;

	retval2 = 0;
	জন্য (i=0, cache = তথ্য->cache; i < CACHE_আকার; i++, cache++) {
		যদি (!cache->in_use)
			চালু;

		যদি (invalidate)
			cache->in_use = 0;

		যদি (!cache->dirty)
			চালু;

		retval = raw_write_blk(channel, তথ্য,
				       cache->block, 1, cache->buf);
		যদি (retval)
			retval2 = retval;
		অন্যথা
			cache->dirty = 0;
	}
	ফেরত retval2;
}
#endif /* NO_IO_CACHE */

স্থির errcode_t windows_open_channel(কাঠামো windows_private_তথ্য *তথ্য,
				   পূর্ণসংখ্যা flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ফাঁকা;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(আকার(কাঠামো struct_io_channel), &io);
	যদি (retval)
		যাও cleanup;
	memset(io, 0, আকার(কাঠামো struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(তথ্য->name)+1, &io->name);
	যদি (retval)
		যাও cleanup;

	strcpy(io->name, তথ্য->name);
	io->private_তথ্য = তথ্য;
	io->block_আকার = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(তথ্য->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	যদি (ext2fs_fstat(তথ্য->dev, &st) == 0) {
		যদি (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		অন্যথা
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	যদি ((retval = alloc_cache(io, তথ্য)))
		যাও cleanup;

#ifdef BLKROGET
	যদি (flags & IO_FLAG_RW) {
		পূর্ণসংখ্যা error;
		পূর্ণসংখ্যা readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(তথ্য->dev, BLKROGET, &readonly);
		যদি (!error && readonly) {
			retval = EPERM;
			যাও cleanup;
		}
	}
#endif

	*channel = io;
	ফেরত 0;

cleanup:
	যদি (তথ্য) {
		যদি (তথ্য->dev >= 0)
			close(তথ্য->dev);
		free_cache(তথ্য);
		ext2fs_free_mem(&তথ্য);
	}
	যদি (io) {
		যদি (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ফেরত retval;
}

স্থির DWORD windows_open_device(কাঠামো windows_private_তথ্য *তথ্য, পূর্ণসংখ্যা open_flags)
{
	DWORD ret = 0;

	যদি (*তথ্য->name != '\\')
		strcpy(তথ্য->cf_device, তথ্য->name);
	অন্যথা যদি (fake_dos_name_for_device(তথ্য))
		ফেরত -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#যদি defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#অন্যথা
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	তথ্য->handle = CreateFile(তথ্য->cf_device, desired_access, share_mode, ফাঁকা, OPEN_EXISTING,
				  flags_and_attributes, ফাঁকা);

	যদি (তথ্য->handle == INVALID_HANDLE_মান) {
		ret = GetLastError();
		যাও invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", তথ্য->cf_device, তথ্য->handle);

	তথ্য->dev = _open_osfhandle((intptr_t)তথ্য->handle, 0);
	যদি (তথ্য->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		যাও osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", তথ্য->handle, তথ্য->dev);

	ফেরত 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", তথ্য->handle);
	CloseHandle(তথ্য->handle);
invalid_handle:
	remove_fake_dos_name(তথ্য);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ফেরত ret;
}

স্থির কাঠামো windows_private_তথ্য *init_private_তথ্য(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags)
{
	কাঠামো windows_private_তথ্য *তথ্য = ফাঁকা;

	যদি (ext2fs_get_mem(আকার(কাঠামো windows_private_তথ্য), &তথ্য))
		ফেরত ফাঁকা;

	memset(তথ্য, 0, আকার(কাঠামো windows_private_তথ্য));
	strncpy(তথ্য->name, name, আকার(তথ্য->name) - 1);
	তথ্য->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	তথ্য->io_stats.অঙ্ক_fields = 2;
	তথ্য->flags = flags;
	তথ্য->handle = INVALID_HANDLE_মান;

	ফেরত তথ্য;
}

স্থির errcode_t windows_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	পূর্ণসংখ্যা open_flags;
	কাঠামো windows_private_তথ্য *তথ্য;

	যদি (name == 0)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;

	তথ্য = init_private_তথ্য(name, flags);
	যদি (!তথ্য)
		ফেরত EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	যদি (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	যদি (windows_open_device(তথ্য, open_flags)) {
		ext2fs_free_mem(&তথ্য);
		ফেরত EXT2_ET_BAD_DEVICE_NAME;
	}

	ফেরত windows_open_channel(তথ্য, flags, channel, windows_io_manager);
}

স্থির errcode_t windows_close(io_channel channel)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ফেরত 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, তথ্য, 0);
#endif

	remove_fake_dos_name(তথ্য);

	যদি (_close(তথ্য->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", তথ্য->dev);

	free_cache(তথ্য);

	ext2fs_free_mem(&channel->private_তথ্য);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ফেরত retval;
}

স্থির errcode_t windows_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (channel->block_আকার != blksize) {
#ifndef NO_IO_CACHE
		যদি ((retval = flush_cached_blocks(channel, তথ্য, 0)))
			ফেরত retval;
#endif

		channel->block_আকার = blksize;
		free_cache(তথ্য);
		যদি ((retval = alloc_cache(channel, তথ্য)))
			ফেরত retval;
	}
	ফেরত 0;
}

স্থির errcode_t windows_read_blk64(io_channel channel, unsigned long long block, পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	কাঠামো windows_cache *cache, *reuse[READ_DIRECT_আকার];
	errcode_t	retval;
	অক্ষর		*cp;
	পূর্ণসংখ্যা		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ফেরত raw_read_blk(channel, তথ্য, block, গুনতি, buf);
#অন্যথা
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_আকার) {
		যদি ((retval = flush_cached_blocks(channel, তথ্য, 0)))
			ফেরত retval;
		ফেরত raw_read_blk(channel, তথ্য, block, গুনতি, buf);
	}

	cp = buf;
	যখন (গুনতি > 0) {
		/* If it's in the cache, use it! */
		যদি ((cache = find_cached_block(তথ্য, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_আকার);
			গুনতি--;
			block++;
			cp += channel->block_আকার;
			চালু;
		}
		যদি (গুনতি == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, তথ্য, cache, block);
			যদি ((retval = raw_read_blk(channel, তথ্য, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				ফেরত retval;
			}
			memcpy(cp, cache->buf, channel->block_আকার);
			ফেরত 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		জন্য (i=1; i < গুনতি; i++)
			যদি (find_cached_block(তথ্য, block+i, &reuse[i]))
				বিরতি;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		যদি ((retval = raw_read_blk(channel, তথ্য, block, i, cp)))
			ফেরত retval;

		/* Save the results in the cache */
		জন্য (j=0; j < i; j++) {
			গুনতি--;
			cache = reuse[j];
			reuse_cache(channel, তথ্য, cache, block++);
			memcpy(cache->buf, cp, channel->block_আকার);
			cp += channel->block_আকার;
		}
	}
	ফেরত 0;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	ফেরত windows_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	কাঠামো windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ধ্রুবক অক্ষর	*cp;
	পূর্ণসংখ্যা		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ফেরত raw_write_blk(channel, তথ্য, block, গুনতি, buf);
#অন্যথা
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_আকার) {
		যদি ((retval = flush_cached_blocks(channel, তথ্য, 1)))
			ফেরত retval;
		ফেরত raw_write_blk(channel, তথ্য, block, গুনতি, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	যদি (writethrough)
		retval = raw_write_blk(channel, তথ্য, block, গুনতি, buf);

	cp = buf;
	যখন (গুনতি > 0) {
		cache = find_cached_block(তথ্য, block, &reuse);
		যদি (!cache) {
			cache = reuse;
			reuse_cache(channel, তথ্য, cache, block);
		}
		যদি (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_আকার);
		cache->dirty = !writethrough;
		গুনতি--;
		block++;
		cp += channel->block_আকার;
	}
	ফেরত retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	ফেরত EXT2_ET_OP_NOT_SUPPORTED;
}

স্থির errcode_t windows_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ফেরত windows_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণসংখ্যা আকার_2, ধ্রুবক শূন্য *buf)
{
	ফেরত EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t windows_flush(io_channel channel)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, তথ্য, 0);
#endif

	ফেরত retval;
}

স্থির errcode_t windows_set_option(io_channel channel, ধ্রুবক অক্ষর *option,
				 ধ্রুবক অক্ষর *arg)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	unsigned long long tmp;
	অক্ষর *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (!strcmp(option, "offset")) {
		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		যদি (*end)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		তথ্য->offset = tmp;
		যদি (তথ্য->offset < 0)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		ফেরত 0;
	}
	ফেরত EXT2_ET_INVALID_ARGUMENT;
}

স্থির errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long গুনতি)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ফেরত EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
স্থির errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো windows_private_তথ্য *তথ্য;
	পূর্ণসংখ্যা		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো windows_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	যদি (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		যাও unimplemented;
	} অন্যথা {
		/* Regular file, try to use truncate/punch/zero. */
		কাঠামো stat statbuf;

		যদি (গুনতি == 0)
			ফেরত 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(তথ্য->dev, &statbuf);
		যদি (ret)
			যাও err;
		যদি ((unsigned long long) statbuf.st_আকার <
			(block + গুনতি) * channel->block_আকার + তথ্য->offset) {
			ret = ftruncate(তথ্য->dev,
					(block + গুনতি) * channel->block_আকার + তথ্য->offset);
			যদি (ret)
				যাও err;
		}
		যাও unimplemented;
	}
err:
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP)
			যাও unimplemented;
		ফেরত errno;
	}
	ফেরত 0;
unimplemented:
	ফেরত EXT2_ET_UNIMPLEMENTED;
}

পূর্ণসংখ্যা ext2fs_open_file(ধ্রুবক অক্ষর *pathname, পূর্ণসংখ্যা flags, mode_t mode)
{
	flags |= O_দ্বিমিক;

	যদি (mode)
#যদি defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ফেরত open64(pathname, flags, mode);
	অন্যথা
		ফেরত open64(pathname, flags);
#অন্যথা
		ফেরত open(pathname, flags, mode);
	অন্যথা
		ফেরত open(pathname, flags);
#endif
}

পূর্ণসংখ্যা ext2fs_stat(ধ্রুবক অক্ষর *path, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ফেরত stat64(path, buf);
#অন্যথা
	ফেরত stat(path, buf);
#endif
}

পূর্ণসংখ্যা ext2fs_fstat(পূর্ণসংখ্যা fd, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ফেরত fstat64(fd, buf);
#অন্যথা
	ফেরত fstat(fd, buf);
#endif
}

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বের_করো
#endif

স্থির কাঠামো struct_io_manager struct_windows_manager = {
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
