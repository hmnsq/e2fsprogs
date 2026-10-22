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

#إذا !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#تعریف_کن XOPEN_منبع 600
#تعریف_کن DARWIN_C_منبع
#تعریف_کن FILE_جابه_جایی_بیتها 64
#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر
#اگر_تعریف_نشده GNU_منبع
#تعریف_کن GNU_منبع
#پایان_اگر
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#تعریف_کن PR_GET_DUMPABLE 3
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#ضمن <fcntl.h>

#لغو_تعریف ALIGN_DEBUG

//#define DEBUG
#اگر_تعریف_شده DEBUG
#تعریف_کن TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#وإلا
#تعریف_کن TRACE(...) افعل { } طالما (0);
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)
#تعریف_کن EXT2_CHECK_MAGIC_RETURN(بنية, کد, ret) \
	  إذا ((بنية)->magic != (کد)) أرجع (ret)

#تعریف_کن EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

بنية windows_cache {
	حرف			*buf;
	بیعلامت دراز دراز	بلوک;
	صحيح			access_زمان;
	بیعلامت		dirty:1;
	بیعلامت		درون_use:1;
};

#تعریف_کن CACHE_اندازه 8
#تعریف_کن WRITE_مستقیم_اندازه 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کن READ_مستقیم_اندازه 4	/* Should be smaller than CACHE_SIZE */

بنية windows_خصوصی_داده {
	صحيح	magic;
	حرف	name[MAX_مسیر];
	HANDLE	handle;
	حرف	dos_device[MAX_مسیر];
	حرف	cf_device[MAX_مسیر];
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_زمان;
	ext2_loff_t جابه_جایی;
	بنية windows_cache cache[CACHE_اندازه];
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
};

#تعریف_کن IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساكن صحيح fake_dos_name_for_device(بنية windows_خصوصی_داده *داده)
{
	إذا (مقایسه_رشته(داده->name, "\\\\", 2) == 0) {
		داده->dos_device[0] = 0;
		strcpy(داده->cf_device, داده->name);
		أرجع 0;
	}

	_snprintf(داده->dos_device, MAX_مسیر, "fakedevice%lu", GetCurrentProcessId());

	إذا (!DefineDosDevice(DDD_RAW_هدف_مسیر, داده->dos_device, داده->name))
		أرجع 1;

	_snprintf(داده->cf_device, MAX_مسیر, "\\\\.\\%s", داده->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", داده->dos_device);

	أرجع 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_خصوصی_داده *داده)
{
	إذا (*داده->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", داده->dos_device);
		DefineDosDevice(DDD_RAW_هدف_مسیر | DDD_EXACT_تطابق_ON_REMOVE | DDD_REMOVE_DEFINITION, داده->dos_device, داده->name);
	}
}

ساكن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية windows_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (stats)
		*stats = &داده->io_stats;

	أرجع retval;
}

ساكن LARGE_INTEGER make_large_integer(LONGLONG مقدار)
{
	LARGE_INTEGER	li;

	li.QuadPart = مقدار;
	أرجع li;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية windows_خصوصی_داده *داده,
			      بیعلامت دراز دراز بلوک,
			      صحيح شمار, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t	اندازه;
	ext2_loff_t	location;
	DWORD		actual = 0;
	بیعلامت حرف	*buf = bufv;
	ssize_t		really_read = 0;

	اندازه = (شمار < 0) ? -شمار : شمار * channel->بلوک_اندازه;
	داده->io_stats.بایتها_read += اندازه;
	location = ((ext2_loff_t) بلوک * channel->بلوک_اندازه) + داده->جابه_جایی;

	إذا (داده->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!مجموعه_File_اشاره_گر_Ex(داده->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}
		اذهب bounce_read;
	}

	إذا (!مجموعه_File_اشاره_گر_Ex(داده->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = Get_آخرین_Error();
		اذهب error_بیرون;
	}
	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(اندازه, channel->align))) {
		إذا (!ReadFile(داده->handle, buf, اندازه, &actual, عدم)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}
		إذا (actual != اندازه) {
	short_read:
		إذا (actual < 0) {
			retval = Get_آخرین_Error();
			actual = 0;
		} وإلا
			retval = EXT2_ET_SHORT_READ;
		اذهب error_بیرون;
		}
	أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	طالما (اندازه > 0) {
		إذا (!ReadFile(داده->handle, داده->bounce, channel->بلوک_اندازه, &actual, عدم)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}
		إذا (actual != channel->بلوک_اندازه) {
			actual = really_read;
			buf -= really_read;
			اندازه += really_read;
			اذهب short_read;
		}
		actual = اندازه;
		إذا (اندازه > channel->بلوک_اندازه)
			actual = channel->بلوک_اندازه;
		کپی_حافظه(buf, داده->bounce, actual);
		really_read += actual;
		اندازه -= actual;
		buf += actual;
	}
	أرجع 0;

error_بیرون:
	إذا (actual >= 0 && actual < اندازه)
		memset((حرف *) buf+actual, 0, اندازه-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلوک, شمار, buf,
					       اندازه, actual, retval);
	أرجع retval;
}

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية windows_خصوصی_داده *داده,
			       بیعلامت دراز دراز بلوک,
			       صحيح شمار, ثابت فراغ *bufv)
{
	ssize_t		اندازه;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت بیعلامت حرف *buf = bufv;

	إذا (شمار == 1)
		اندازه = channel->بلوک_اندازه;
	وإلا {
		إذا (شمار < 0)
			اندازه = -شمار;
		وإلا
			اندازه = شمار * channel->بلوک_اندازه;
	}
	داده->io_stats.بایتها_written += اندازه;

	location = ((ext2_loff_t) بلوک * channel->بلوک_اندازه) + داده->جابه_جایی;

	إذا (داده->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!مجموعه_File_اشاره_گر_Ex(داده->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}
	اذهب bounce_write;
	}

		إذا (!مجموعه_File_اشاره_گر_Ex(داده->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخرین_Error();
		اذهب error_بیرون;
	}

	مجموعه_آخرین_Error(0);

	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(اندازه, channel->align))) {
		إذا (!WriteFile(داده->handle, buf, اندازه, &actual, عدم)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}

		إذا (actual != اندازه) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_بیرون;
		}
		أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	طالما (اندازه > 0) {
		إذا (اندازه < channel->بلوک_اندازه) {
			إذا (!ReadFile(داده->handle, داده->bounce, channel->بلوک_اندازه, &actual, عدم)) {
				retval = Get_آخرین_Error();
				اذهب error_بیرون;
			}
			إذا (actual != channel->بلوک_اندازه) {
				إذا (actual < 0) {
					retval = Get_آخرین_Error();
					اذهب error_بیرون;
				}
				memset((حرف *) داده->bounce + actual, 0,
				       channel->بلوک_اندازه - actual);
			}
		}
		actual = اندازه;
		إذا (اندازه > channel->بلوک_اندازه)
			actual = channel->بلوک_اندازه;
		کپی_حافظه(داده->bounce, buf, actual);
		إذا (!مجموعه_File_اشاره_گر_Ex(داده->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}
		إذا (!WriteFile(داده->handle, داده->bounce, channel->بلوک_اندازه, &actual, عدم)) {
			retval = Get_آخرین_Error();
			اذهب error_بیرون;
		}

		إذا (actual != channel->بلوک_اندازه)
			اذهب short_write;
		اندازه -= actual;
		buf += actual;
		location += actual;
	}
	أرجع 0;

error_بیرون:
	إذا (channel->write_error)
		retval = (channel->write_error)(channel, بلوک, شمار, buf,
						اندازه, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية windows_خصوصی_داده *داده)
{
	errcode_t		retval;
	بنية windows_cache	*cache;
	صحيح			i;

	داده->access_زمان = 0;
	لكل (i=0, cache = داده->cache; i < CACHE_اندازه; i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval)
			أرجع retval;
	}
	إذا (channel->align || داده->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (داده->bounce)
			ext2fs_free_mem(&داده->bounce);
		retval = io_channel_alloc_buf(channel, 0, &داده->bounce);
	}
	أرجع retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية windows_خصوصی_داده *داده)
{
	بنية windows_cache    *cache;
	صحيح			i;

	داده->access_زمان = 0;
	لكل (i=0, cache = داده->cache; i < CACHE_اندازه; i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (داده->bounce)
		ext2fs_free_mem(&داده->bounce);
}

#اگر_تعریف_نشده NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية windows_cache *پیدا_کن_cached_بلوک(بنية windows_خصوصی_داده *داده,
					    بیعلامت دراز دراز بلوک,
					    بنية windows_cache **eldest)
{
	بنية windows_cache	*cache, *استفادهنشده_cache, *oldest_cache;
	صحيح			i;

	استفادهنشده_cache = oldest_cache = 0;
	لكل (i=0, cache = داده->cache; i < CACHE_اندازه; i++, cache++) {
		إذا (!cache->درون_use) {
			إذا (!استفادهنشده_cache)
				استفادهنشده_cache = cache;
			استمر;
		}
		إذا (cache->بلوک == بلوک) {
			cache->access_زمان = ++داده->access_زمان;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		   (cache->access_زمان < oldest_cache->access_زمان))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (استفادهنشده_cache) ? استفادهنشده_cache : oldest_cache;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_خصوصی_داده *داده,
		 بنية windows_cache *cache, بیعلامت دراز دراز بلوک)
{
	إذا (cache->dirty && cache->درون_use)
		raw_write_blk(channel, داده, cache->بلوک, 1, cache->buf);

	cache->درون_use = 1;
	cache->dirty = 0;
	cache->بلوک = بلوک;
	cache->access_زمان = ++داده->access_زمان;
}

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلوکها(io_channel channel,
				     بنية windows_خصوصی_داده *داده,
				     صحيح invalidate)
{
	بنية windows_cache	*cache;
	errcode_t	retval, retval2;
	صحيح			i;

	retval2 = 0;
	لكل (i=0, cache = داده->cache; i < CACHE_اندازه; i++, cache++) {
		إذا (!cache->درون_use)
			استمر;

		إذا (invalidate)
			cache->درون_use = 0;

		إذا (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, داده,
				       cache->بلوک, 1, cache->buf);
		إذا (retval)
			retval2 = retval;
		وإلا
			cache->dirty = 0;
	}
	أرجع retval2;
}
#پایان_اگر /* NO_IO_CACHE */

ساكن errcode_t windows_open_channel(بنية windows_خصوصی_داده *داده,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(طول_رشته(داده->name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, داده->name);
	io->خصوصی_داده = داده;
	io->بلوک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = ext2fs_get_dio_alignment(داده->dev);
#پایان_اگر

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(داده->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_نما))
			io->flags |= CHANNEL_FLAGS_بلوک_DEVICE;
		وإلا
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	إذا ((retval = alloc_cache(io, داده)))
		اذهب cleanup;

#اگر_تعریف_شده BLKROGET
	إذا (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(داده->dev, BLKROGET, &readonly);
		إذا (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#پایان_اگر

	*channel = io;
	أرجع 0;

cleanup:
	إذا (داده) {
		إذا (داده->dev >= 0)
			close(داده->dev);
		free_cache(داده);
		ext2fs_free_mem(&داده);
	}
	إذا (io) {
		إذا (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	أرجع retval;
}

ساكن DWORD windows_open_device(بنية windows_خصوصی_داده *داده, صحيح open_flags)
{
	DWORD ret = 0;

	إذا (*داده->name != '\\')
		strcpy(داده->cf_device, داده->name);
	وإلا إذا (fake_dos_name_for_device(داده))
		أرجع -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_نما = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_صفات =
#إذا defined(O_مستقیم)
	(open_flags & O_مستقیم) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_نرمال;
#وإلا
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#پایان_اگر

	داده->handle = CreateFile(داده->cf_device, desired_access, share_نما, عدم, OPEN_EXISTING,
				  flags_and_صفات, عدم);

	إذا (داده->handle == INVALID_HANDLE_مقدار) {
		ret = Get_آخرین_Error();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", داده->cf_device, داده->handle);

	داده->dev = _open_osfhandle((intptr_t)داده->handle, 0);
	إذا (داده->dev < 0) {
		ret = Get_آخرین_Error() ? Get_آخرین_Error() : 9999;
		اذهب osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", داده->handle, داده->dev);

	أرجع 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", داده->handle);
	CloseHandle(داده->handle);
invalid_handle:
	remove_fake_dos_name(داده);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	أرجع ret;
}

ساكن بنية windows_خصوصی_داده *init_خصوصی_داده(ثابت حرف *name, صحيح flags)
{
	بنية windows_خصوصی_داده *داده = عدم;

	إذا (ext2fs_get_mem(حجم(بنية windows_خصوصی_داده), &داده))
		أرجع عدم;

	memset(داده, 0, حجم(بنية windows_خصوصی_داده));
	strncpy(داده->name, name, حجم(داده->name) - 1);
	داده->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	داده->io_stats.رقم_fields = 2;
	داده->flags = flags;
	داده->handle = INVALID_HANDLE_مقدار;

	أرجع داده;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_خصوصی_داده *داده;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	داده = init_خصوصی_داده(name, flags);
	إذا (!داده)
		أرجع EXT2_ET_NO_حافظه;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	إذا (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		open_flags |= O_مستقیم;
#پایان_اگر

	إذا (windows_open_device(داده, open_flags)) {
		ext2fs_free_mem(&داده);
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	أرجع windows_open_channel(داده, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#اگر_تعریف_نشده NO_IO_CACHE
	retval = flush_cached_بلوکها(channel, داده, 0);
#پایان_اگر

	remove_fake_dos_name(داده);

	إذا (_close(داده->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", داده->dev);

	free_cache(داده);

	ext2fs_free_mem(&channel->خصوصی_داده);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t windows_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_خصوصی_داده *داده;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->بلوک_اندازه != blksize) {
#اگر_تعریف_نشده NO_IO_CACHE
		إذا ((retval = flush_cached_بلوکها(channel, داده, 0)))
			أرجع retval;
#پایان_اگر

		channel->بلوک_اندازه = blksize;
		free_cache(داده);
		إذا ((retval = alloc_cache(channel, داده)))
			أرجع retval;
	}
	أرجع 0;
}

ساكن errcode_t windows_read_blk64(io_channel channel, بیعلامت دراز دراز بلوک, صحيح شمار, فراغ *buf)
{
	بنية windows_خصوصی_داده *داده;
	بنية windows_cache *cache, *reuse[READ_مستقیم_اندازه];
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_تعریف_شده NO_IO_CACHE
	أرجع raw_read_blk(channel, داده, بلوک, شمار, buf);
#وإلا
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (شمار < 0 || شمار > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلوکها(channel, داده, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, داده, بلوک, شمار, buf);
	}

	cp = buf;
	طالما (شمار > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = پیدا_کن_cached_بلوک(داده, بلوک, &reuse[0]))) {
#اگر_تعریف_شده DEBUG
			چاپ_کن("Using cached block %lu\n", بلوک);
#پایان_اگر
			کپی_حافظه(cp, cache->buf, channel->بلوک_اندازه);
			شمار--;
			بلوک++;
			cp += channel->بلوک_اندازه;
			استمر;
		}
		إذا (شمار == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, داده, cache, بلوک);
			إذا ((retval = raw_read_blk(channel, داده, بلوک, 1,
						   cache->buf))) {
				cache->درون_use = 0;
				أرجع retval;
			}
			کپی_حافظه(cp, cache->buf, channel->بلوک_اندازه);
			أرجع 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < شمار; i++)
			إذا (پیدا_کن_cached_بلوک(داده, بلوک+i, &reuse[i]))
				اكسر;
#اگر_تعریف_شده DEBUG
		چاپ_کن("Reading %d blocks starting at %lu\n", i, بلوک);
#پایان_اگر
		إذا ((retval = raw_read_blk(channel, داده, بلوک, i, cp)))
			أرجع retval;

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			شمار--;
			cache = reuse[j];
			reuse_cache(channel, داده, cache, بلوک++);
			کپی_حافظه(cache->buf, cp, channel->بلوک_اندازه);
			cp += channel->بلوک_اندازه;
		}
	}
	أرجع 0;
#پایان_اگر /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	أرجع windows_read_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t windows_write_blk64(io_channel channel,
				بیعلامت دراز دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	بنية windows_خصوصی_داده *داده;
	بنية windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_تعریف_شده NO_IO_CACHE
	أرجع raw_write_blk(channel, داده, بلوک, شمار, buf);
#وإلا
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (شمار < 0 || شمار > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلوکها(channel, داده, 1)))
			أرجع retval;
		أرجع raw_write_blk(channel, داده, بلوک, شمار, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, داده, بلوک, شمار, buf);

	cp = buf;
	طالما (شمار > 0) {
		cache = پیدا_کن_cached_بلوک(داده, بلوک, &reuse);
		إذا (!cache) {
			cache = reuse;
			reuse_cache(channel, داده, cache, بلوک);
		}
		إذا (cache->buf != cp)
			کپی_حافظه(cache->buf, cp, channel->بلوک_اندازه);
		cache->dirty = !writethrough;
		شمار--;
		بلوک++;
		cp += channel->بلوک_اندازه;
	}
	أرجع retval;
#پایان_اگر /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      بیعلامت دراز دراز بلوک,
				      بیعلامت دراز دراز شمار)
{
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	أرجع windows_write_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				 صحيح اندازه, ثابت فراغ *buf)
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t windows_flush(io_channel channel)
{
	بنية windows_خصوصی_داده *داده;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_تعریف_نشده NO_IO_CACHE
	retval = flush_cached_بلوکها(channel, داده, 0);
#پایان_اگر

	أرجع retval;
}

ساكن errcode_t windows_مجموعه_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية windows_خصوصی_داده *داده;
	بیعلامت دراز دراز tmp;
	حرف *انتها;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;

		tmp = strtoull(arg, &انتها, 0);
		إذا (*انتها)
			أرجع EXT2_ET_INVALID_آرگومان;
		داده->جابه_جایی = tmp;
		إذا (داده->جابه_جایی < 0)
			أرجع EXT2_ET_INVALID_آرگومان;
		أرجع 0;
	}
	أرجع EXT2_ET_INVALID_آرگومان;
}

ساكن errcode_t windows_discard(io_channel channel, بیعلامت دراز دراز بلوک,
				 بیعلامت دراز دراز شمار)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#رهنمود GCC diagnostic افزودن
#رهنمود GCC diagnostic ignored "-Wunused-parameter"
#پایان_اگر
ساكن errcode_t windows_zeroout(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية windows_خصوصی_داده *داده;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية windows_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_بلوک_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} وإلا {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (شمار == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(داده->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((بیعلامت دراز دراز) statbuf.st_اندازه <
			(بلوک + شمار) * channel->بلوک_اندازه + داده->جابه_جایی) {
			ret = ftruncate(داده->dev,
					(بلوک + شمار) * channel->بلوک_اندازه + داده->جابه_جایی);
			إذا (ret)
				اذهب err;
		}
		اذهب unimplemented;
	}
err:
	إذا (ret < 0) {
		إذا (errno == EOPNOTSUPP)
			اذهب unimplemented;
		أرجع errno;
	}
	أرجع 0;
unimplemented:
	أرجع EXT2_ET_UNIMPLEMENTED;
}

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t نما)
{
	flags |= O_دودویی;

	إذا (نما)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, نما);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, نما);
	وإلا
		أرجع open(pathname, flags);
#پایان_اگر
}

صحيح ext2fs_stat(ثابت حرف *مسیر, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(مسیر, buf);
#وإلا
	أرجع stat(مسیر, buf);
#پایان_اگر
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#پایان_اگر
}

#إذا __GNUC_PREREQ (4, 6)
#رهنمود GCC diagnostic برداشت
#پایان_اگر

ساكن بنية struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.مجموعه_blksize	= windows_مجموعه_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.مجموعه_option	= windows_مجموعه_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
