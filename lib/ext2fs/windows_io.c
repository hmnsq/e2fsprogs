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
#تعریف_کرو XOPEN_ماخذ 600
#تعریف_کرو DARWIN_C_ماخذ
#تعریف_کرو FILE_انحراف_بٹ 64
#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ GNU_ماخذ
#تعریف_کرو GNU_ماخذ
#اگر_ختم
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#تعریف_کرو PR_GET_DUMPABLE 3
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#شامل <fcntl.h>

#تعریف_منسوخ ALIGN_DEBUG

//#define DEBUG
#اگر_تعریف_شدہ DEBUG
#تعریف_کرو TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ورنہ
#تعریف_کرو TRACE(...) کرو { } جب (0);
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)
#تعریف_کرو EXT2_CHECK_MAGIC_RETURN(ساخت, رمز, ret) \
	  اگر ((ساخت)->magic != (رمز)) واپس (ret)

#تعریف_کرو EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ساخت windows_cache {
	حرف			*buf;
	غیرعلامتی طویل طویل	بلاک;
	صحیح			access_وقت;
	غیرعلامتی		dirty:1;
	غیرعلامتی		اندر_use:1;
};

#تعریف_کرو CACHE_سائز 8
#تعریف_کرو WRITE_براہ_راست_سائز 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کرو READ_براہ_راست_سائز 4	/* Should be smaller than CACHE_SIZE */

ساخت windows_نجی_ڈیٹا {
	صحیح	magic;
	حرف	name[MAX_راستہ];
	HANDLE	handle;
	حرف	dos_device[MAX_راستہ];
	حرف	cf_device[MAX_راستہ];
	صحیح	dev;
	صحیح	flags;
	صحیح	align;
	صحیح	access_وقت;
	ext2_loff_t انحراف;
	ساخت windows_cache cache[CACHE_سائز];
	خالی	*bounce;
	ساخت struct_io_stats io_stats;
};

#تعریف_کرو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساکن صحیح fake_dos_name_for_device(ساخت windows_نجی_ڈیٹا *ڈیٹا)
{
	اگر (لڑی_موازنہ(ڈیٹا->name, "\\\\", 2) == 0) {
		ڈیٹا->dos_device[0] = 0;
		strcpy(ڈیٹا->cf_device, ڈیٹا->name);
		واپس 0;
	}

	_snprintf(ڈیٹا->dos_device, MAX_راستہ, "fakedevice%lu", GetCurrentProcessId());

	اگر (!DefineDosDevice(DDD_RAW_ہدف_راستہ, ڈیٹا->dos_device, ڈیٹا->name))
		واپس 1;

	_snprintf(ڈیٹا->cf_device, MAX_راستہ, "\\\\.\\%s", ڈیٹا->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", ڈیٹا->dos_device);

	واپس 0;
}

ساکن خالی remove_fake_dos_name(ساخت windows_نجی_ڈیٹا *ڈیٹا)
{
	اگر (*ڈیٹا->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", ڈیٹا->dos_device);
		DefineDosDevice(DDD_RAW_ہدف_راستہ | DDD_EXACT_مطابقت_ON_REMOVE | DDD_REMOVE_DEFINITION, ڈیٹا->dos_device, ڈیٹا->name);
	}
}

ساکن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ساخت windows_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (stats)
		*stats = &ڈیٹا->io_stats;

	واپس retval;
}

ساکن LARGE_INTEGER make_large_integer(LONGLONG قدر)
{
	LARGE_INTEGER	li;

	li.QuadPart = قدر;
	واپس li;
}

/*
 * Here are the raw I/O functions
 */
ساکن errcode_t raw_read_blk(io_channel channel,
			      ساخت windows_نجی_ڈیٹا *ڈیٹا,
			      غیرعلامتی طویل طویل بلاک,
			      صحیح گنتی, خالی *bufv)
{
	errcode_t	retval;
	ssize_t	سائز;
	ext2_loff_t	location;
	DWORD		actual = 0;
	غیرعلامتی حرف	*buf = bufv;
	ssize_t		really_read = 0;

	سائز = (گنتی < 0) ? -گنتی : گنتی * channel->بلاک_سائز;
	ڈیٹا->io_stats.بائٹیں_read += سائز;
	location = ((ext2_loff_t) بلاک * channel->بلاک_سائز) + ڈیٹا->انحراف;

	اگر (ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (!سیٹ_File_اشارہ_گر_Ex(ڈیٹا->handle, make_large_integer(location), تہی, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}
		جاؤ bounce_read;
	}

	اگر (!سیٹ_File_اشارہ_گر_Ex(ڈیٹا->handle, make_large_integer(location), تہی, FILE_BEGIN)) {
		retval = Get_آخری_Error();
		جاؤ error_برآمد;
	}
	اگر ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(سائز, channel->align))) {
		اگر (!ReadFile(ڈیٹا->handle, buf, سائز, &actual, تہی)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}
		اگر (actual != سائز) {
	short_read:
		اگر (actual < 0) {
			retval = Get_آخری_Error();
			actual = 0;
		} ورنہ
			retval = EXT2_ET_SHORT_READ;
		جاؤ error_برآمد;
		}
	واپس 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	جب (سائز > 0) {
		اگر (!ReadFile(ڈیٹا->handle, ڈیٹا->bounce, channel->بلاک_سائز, &actual, تہی)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}
		اگر (actual != channel->بلاک_سائز) {
			actual = really_read;
			buf -= really_read;
			سائز += really_read;
			جاؤ short_read;
		}
		actual = سائز;
		اگر (سائز > channel->بلاک_سائز)
			actual = channel->بلاک_سائز;
		حافظہ_نقل(buf, ڈیٹا->bounce, actual);
		really_read += actual;
		سائز -= actual;
		buf += actual;
	}
	واپس 0;

error_برآمد:
	اگر (actual >= 0 && actual < سائز)
		memset((حرف *) buf+actual, 0, سائز-actual);
	اگر (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, گنتی, buf,
					       سائز, actual, retval);
	واپس retval;
}

ساکن errcode_t raw_write_blk(io_channel channel,
			       ساخت windows_نجی_ڈیٹا *ڈیٹا,
			       غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, ثابت خالی *bufv)
{
	ssize_t		سائز;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت غیرعلامتی حرف *buf = bufv;

	اگر (گنتی == 1)
		سائز = channel->بلاک_سائز;
	ورنہ {
		اگر (گنتی < 0)
			سائز = -گنتی;
		ورنہ
			سائز = گنتی * channel->بلاک_سائز;
	}
	ڈیٹا->io_stats.بائٹیں_written += سائز;

	location = ((ext2_loff_t) بلاک * channel->بلاک_سائز) + ڈیٹا->انحراف;

	اگر (ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (!سیٹ_File_اشارہ_گر_Ex(ڈیٹا->handle, make_large_integer(location), تہی, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}
	جاؤ bounce_write;
	}

		اگر (!سیٹ_File_اشارہ_گر_Ex(ڈیٹا->handle, make_large_integer(location), تہی, FILE_BEGIN)) {
			retval = Get_آخری_Error();
		جاؤ error_برآمد;
	}

	سیٹ_آخری_Error(0);

	اگر ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(سائز, channel->align))) {
		اگر (!WriteFile(ڈیٹا->handle, buf, سائز, &actual, تہی)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}

		اگر (actual != سائز) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			جاؤ error_برآمد;
		}
		واپس 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	جب (سائز > 0) {
		اگر (سائز < channel->بلاک_سائز) {
			اگر (!ReadFile(ڈیٹا->handle, ڈیٹا->bounce, channel->بلاک_سائز, &actual, تہی)) {
				retval = Get_آخری_Error();
				جاؤ error_برآمد;
			}
			اگر (actual != channel->بلاک_سائز) {
				اگر (actual < 0) {
					retval = Get_آخری_Error();
					جاؤ error_برآمد;
				}
				memset((حرف *) ڈیٹا->bounce + actual, 0,
				       channel->بلاک_سائز - actual);
			}
		}
		actual = سائز;
		اگر (سائز > channel->بلاک_سائز)
			actual = channel->بلاک_سائز;
		حافظہ_نقل(ڈیٹا->bounce, buf, actual);
		اگر (!سیٹ_File_اشارہ_گر_Ex(ڈیٹا->handle, make_large_integer(location), تہی, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}
		اگر (!WriteFile(ڈیٹا->handle, ڈیٹا->bounce, channel->بلاک_سائز, &actual, تہی)) {
			retval = Get_آخری_Error();
			جاؤ error_برآمد;
		}

		اگر (actual != channel->بلاک_سائز)
			جاؤ short_write;
		سائز -= actual;
		buf += actual;
		location += actual;
	}
	واپس 0;

error_برآمد:
	اگر (channel->write_error)
		retval = (channel->write_error)(channel, بلاک, گنتی, buf,
						سائز, actual, retval);
	واپس retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساکن errcode_t alloc_cache(io_channel channel,
			     ساخت windows_نجی_ڈیٹا *ڈیٹا)
{
	errcode_t		retval;
	ساخت windows_cache	*cache;
	صحیح			i;

	ڈیٹا->access_وقت = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < CACHE_سائز; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval)
			واپس retval;
	}
	اگر (channel->align || ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (ڈیٹا->bounce)
			ext2fs_free_mem(&ڈیٹا->bounce);
		retval = io_channel_alloc_buf(channel, 0, &ڈیٹا->bounce);
	}
	واپس retval;
}

/* Free the cache buffers */
ساکن خالی free_cache(ساخت windows_نجی_ڈیٹا *ڈیٹا)
{
	ساخت windows_cache    *cache;
	صحیح			i;

	ڈیٹا->access_وقت = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < CACHE_سائز; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	اگر (ڈیٹا->bounce)
		ext2fs_free_mem(&ڈیٹا->bounce);
}

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساکن ساخت windows_cache *ڈھونڈو_cached_بلاک(ساخت windows_نجی_ڈیٹا *ڈیٹا,
					    غیرعلامتی طویل طویل بلاک,
					    ساخت windows_cache **eldest)
{
	ساخت windows_cache	*cache, *غیر_مستعمل_cache, *oldest_cache;
	صحیح			i;

	غیر_مستعمل_cache = oldest_cache = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < CACHE_سائز; i++, cache++) {
		اگر (!cache->اندر_use) {
			اگر (!غیر_مستعمل_cache)
				غیر_مستعمل_cache = cache;
			جاری;
		}
		اگر (cache->بلاک == بلاک) {
			cache->access_وقت = ++ڈیٹا->access_وقت;
			واپس cache;
		}
		اگر (!oldest_cache ||
		   (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	اگر (eldest)
		*eldest = (غیر_مستعمل_cache) ? غیر_مستعمل_cache : oldest_cache;
	واپس 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساکن خالی reuse_cache(io_channel channel, ساخت windows_نجی_ڈیٹا *ڈیٹا,
		 ساخت windows_cache *cache, غیرعلامتی طویل طویل بلاک)
{
	اگر (cache->dirty && cache->اندر_use)
		raw_write_blk(channel, ڈیٹا, cache->بلاک, 1, cache->buf);

	cache->اندر_use = 1;
	cache->dirty = 0;
	cache->بلاک = بلاک;
	cache->access_وقت = ++ڈیٹا->access_وقت;
}

/*
 * Flush all of the blocks in the cache
 */
ساکن errcode_t flush_cached_بلاکیں(io_channel channel,
				     ساخت windows_نجی_ڈیٹا *ڈیٹا,
				     صحیح invalidate)
{
	ساخت windows_cache	*cache;
	errcode_t	retval, retval2;
	صحیح			i;

	retval2 = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < CACHE_سائز; i++, cache++) {
		اگر (!cache->اندر_use)
			جاری;

		اگر (invalidate)
			cache->اندر_use = 0;

		اگر (!cache->dirty)
			جاری;

		retval = raw_write_blk(channel, ڈیٹا,
				       cache->بلاک, 1, cache->buf);
		اگر (retval)
			retval2 = retval;
		ورنہ
			cache->dirty = 0;
	}
	واپس retval2;
}
#اگر_ختم /* NO_IO_CACHE */

ساکن errcode_t windows_open_channel(ساخت windows_نجی_ڈیٹا *ڈیٹا,
				   صحیح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = تہی;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(حجم(ساخت struct_io_channel), &io);
	اگر (retval)
		جاؤ cleanup;
	memset(io, 0, حجم(ساخت struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(لڑی_لمبائی(ڈیٹا->name)+1, &io->name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(io->name, ڈیٹا->name);
	io->نجی_ڈیٹا = ڈیٹا;
	io->بلاک_سائز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#اگر defined(O_براہ_راست)
	اگر (flags & IO_FLAG_براہ_راست_IO)
		io->align = ext2fs_get_dio_alignment(ڈیٹا->dev);
#اگر_ختم

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	اگر (ext2fs_fstat(ڈیٹا->dev, &st) == 0) {
		اگر (ext2fsP_is_disk_device(st.st_منوال))
			io->flags |= CHANNEL_FLAGS_بلاک_DEVICE;
		ورنہ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	اگر ((retval = alloc_cache(io, ڈیٹا)))
		جاؤ cleanup;

#اگر_تعریف_شدہ BLKROGET
	اگر (flags & IO_FLAG_RW) {
		صحیح error;
		صحیح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(ڈیٹا->dev, BLKROGET, &readonly);
		اگر (!error && readonly) {
			retval = EPERM;
			جاؤ cleanup;
		}
	}
#اگر_ختم

	*channel = io;
	واپس 0;

cleanup:
	اگر (ڈیٹا) {
		اگر (ڈیٹا->dev >= 0)
			close(ڈیٹا->dev);
		free_cache(ڈیٹا);
		ext2fs_free_mem(&ڈیٹا);
	}
	اگر (io) {
		اگر (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	واپس retval;
}

ساکن DWORD windows_open_device(ساخت windows_نجی_ڈیٹا *ڈیٹا, صحیح open_flags)
{
	DWORD ret = 0;

	اگر (*ڈیٹا->name != '\\')
		strcpy(ڈیٹا->cf_device, ڈیٹا->name);
	ورنہ اگر (fake_dos_name_for_device(ڈیٹا))
		واپس -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_منوال = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_اوصاف =
#اگر defined(O_براہ_راست)
	(open_flags & O_براہ_راست) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_معمول;
#ورنہ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#اگر_ختم

	ڈیٹا->handle = CreateFile(ڈیٹا->cf_device, desired_access, share_منوال, تہی, OPEN_EXISTING,
				  flags_and_اوصاف, تہی);

	اگر (ڈیٹا->handle == INVALID_HANDLE_قدر) {
		ret = Get_آخری_Error();
		جاؤ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", ڈیٹا->cf_device, ڈیٹا->handle);

	ڈیٹا->dev = _open_osfhandle((intptr_t)ڈیٹا->handle, 0);
	اگر (ڈیٹا->dev < 0) {
		ret = Get_آخری_Error() ? Get_آخری_Error() : 9999;
		جاؤ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", ڈیٹا->handle, ڈیٹا->dev);

	واپس 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", ڈیٹا->handle);
	CloseHandle(ڈیٹا->handle);
invalid_handle:
	remove_fake_dos_name(ڈیٹا);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	واپس ret;
}

ساکن ساخت windows_نجی_ڈیٹا *init_نجی_ڈیٹا(ثابت حرف *name, صحیح flags)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا = تہی;

	اگر (ext2fs_get_mem(حجم(ساخت windows_نجی_ڈیٹا), &ڈیٹا))
		واپس تہی;

	memset(ڈیٹا, 0, حجم(ساخت windows_نجی_ڈیٹا));
	strncpy(ڈیٹا->name, name, حجم(ڈیٹا->name) - 1);
	ڈیٹا->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	ڈیٹا->io_stats.عدد_fields = 2;
	ڈیٹا->flags = flags;
	ڈیٹا->handle = INVALID_HANDLE_قدر;

	واپس ڈیٹا;
}

ساکن errcode_t windows_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	صحیح open_flags;
	ساخت windows_نجی_ڈیٹا *ڈیٹا;

	اگر (name == 0)
		واپس EXT2_ET_BAD_DEVICE_NAME;

	ڈیٹا = init_نجی_ڈیٹا(name, flags);
	اگر (!ڈیٹا)
		واپس EXT2_ET_NO_حافظہ;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	اگر (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#اگر defined(O_براہ_راست)
	اگر (flags & IO_FLAG_براہ_راست_IO)
		open_flags |= O_براہ_راست;
#اگر_ختم

	اگر (windows_open_device(ڈیٹا, open_flags)) {
		ext2fs_free_mem(&ڈیٹا);
		واپس EXT2_ET_BAD_DEVICE_NAME;
	}

	واپس windows_open_channel(ڈیٹا, flags, channel, windows_io_manager);
}

ساکن errcode_t windows_close(io_channel channel)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		واپس 0;

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	retval = flush_cached_بلاکیں(channel, ڈیٹا, 0);
#اگر_ختم

	remove_fake_dos_name(ڈیٹا);

	اگر (_close(ڈیٹا->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", ڈیٹا->dev);

	free_cache(ڈیٹا);

	ext2fs_free_mem(&channel->نجی_ڈیٹا);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	واپس retval;
}

ساکن errcode_t windows_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (channel->بلاک_سائز != blksize) {
#اگر_غیر_تعریف_شدہ NO_IO_CACHE
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, 0)))
			واپس retval;
#اگر_ختم

		channel->بلاک_سائز = blksize;
		free_cache(ڈیٹا);
		اگر ((retval = alloc_cache(channel, ڈیٹا)))
			واپس retval;
	}
	واپس 0;
}

ساکن errcode_t windows_read_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک, صحیح گنتی, خالی *buf)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	ساخت windows_cache *cache, *reuse[READ_براہ_راست_سائز];
	errcode_t	retval;
	حرف		*cp;
	صحیح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_تعریف_شدہ NO_IO_CACHE
	واپس raw_read_blk(channel, ڈیٹا, بلاک, گنتی, buf);
#ورنہ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	اگر (گنتی < 0 || گنتی > WRITE_براہ_راست_سائز) {
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, 0)))
			واپس retval;
		واپس raw_read_blk(channel, ڈیٹا, بلاک, گنتی, buf);
	}

	cp = buf;
	جب (گنتی > 0) {
		/* If it's in the cache, use it! */
		اگر ((cache = ڈھونڈو_cached_بلاک(ڈیٹا, بلاک, &reuse[0]))) {
#اگر_تعریف_شدہ DEBUG
			لکھو("Using cached block %lu\n", بلاک);
#اگر_ختم
			حافظہ_نقل(cp, cache->buf, channel->بلاک_سائز);
			گنتی--;
			بلاک++;
			cp += channel->بلاک_سائز;
			جاری;
		}
		اگر (گنتی == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, ڈیٹا, cache, بلاک);
			اگر ((retval = raw_read_blk(channel, ڈیٹا, بلاک, 1,
						   cache->buf))) {
				cache->اندر_use = 0;
				واپس retval;
			}
			حافظہ_نقل(cp, cache->buf, channel->بلاک_سائز);
			واپس 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		برائے (i=1; i < گنتی; i++)
			اگر (ڈھونڈو_cached_بلاک(ڈیٹا, بلاک+i, &reuse[i]))
				توڑو;
#اگر_تعریف_شدہ DEBUG
		لکھو("Reading %d blocks starting at %lu\n", i, بلاک);
#اگر_ختم
		اگر ((retval = raw_read_blk(channel, ڈیٹا, بلاک, i, cp)))
			واپس retval;

		/* Save the results in the cache */
		برائے (j=0; j < i; j++) {
			گنتی--;
			cache = reuse[j];
			reuse_cache(channel, ڈیٹا, cache, بلاک++);
			حافظہ_نقل(cache->buf, cp, channel->بلاک_سائز);
			cp += channel->بلاک_سائز;
		}
	}
	واپس 0;
#اگر_ختم /* NO_IO_CACHE */
}

ساکن errcode_t windows_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	واپس windows_read_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t windows_write_blk64(io_channel channel,
				غیرعلامتی طویل طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	ساخت windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحیح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_تعریف_شدہ NO_IO_CACHE
	واپس raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf);
#ورنہ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	اگر (گنتی < 0 || گنتی > WRITE_براہ_راست_سائز) {
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, 1)))
			واپس retval;
		واپس raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	اگر (writethrough)
		retval = raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf);

	cp = buf;
	جب (گنتی > 0) {
		cache = ڈھونڈو_cached_بلاک(ڈیٹا, بلاک, &reuse);
		اگر (!cache) {
			cache = reuse;
			reuse_cache(channel, ڈیٹا, cache, بلاک);
		}
		اگر (cache->buf != cp)
			حافظہ_نقل(cache->buf, cp, channel->بلاک_سائز);
		cache->dirty = !writethrough;
		گنتی--;
		بلاک++;
		cp += channel->بلاک_سائز;
	}
	واپس retval;
#اگر_ختم /* NO_IO_CACHE */
}

ساکن errcode_t windows_cache_readahead(io_channel channel,
				      غیرعلامتی طویل طویل بلاک,
				      غیرعلامتی طویل طویل گنتی)
{
	واپس EXT2_ET_OP_NOT_SUPPORTED;
}

ساکن errcode_t windows_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	واپس windows_write_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t windows_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				 صحیح سائز, ثابت خالی *buf)
{
	واپس EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساکن errcode_t windows_flush(io_channel channel)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	retval = flush_cached_بلاکیں(channel, ڈیٹا, 0);
#اگر_ختم

	واپس retval;
}

ساکن errcode_t windows_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	غیرعلامتی طویل طویل tmp;
	حرف *انتہا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (!strcmp(option, "offset")) {
		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;

		tmp = strtoull(arg, &انتہا, 0);
		اگر (*انتہا)
			واپس EXT2_ET_INVALID_دلیل;
		ڈیٹا->انحراف = tmp;
		اگر (ڈیٹا->انحراف < 0)
			واپس EXT2_ET_INVALID_دلیل;
		واپس 0;
	}
	واپس EXT2_ET_INVALID_دلیل;
}

ساکن errcode_t windows_discard(io_channel channel, غیرعلامتی طویل طویل بلاک,
				 غیرعلامتی طویل طویل گنتی)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	واپس EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wunused-parameter"
#اگر_ختم
ساکن errcode_t windows_zeroout(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت windows_نجی_ڈیٹا *ڈیٹا;
	صحیح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت windows_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	اگر (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		جاؤ unimplemented;
	} ورنہ {
		/* Regular file, try to use truncate/punch/zero. */
		ساخت stat statbuf;

		اگر (گنتی == 0)
			واپس 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(ڈیٹا->dev, &statbuf);
		اگر (ret)
			جاؤ err;
		اگر ((غیرعلامتی طویل طویل) statbuf.st_سائز <
			(بلاک + گنتی) * channel->بلاک_سائز + ڈیٹا->انحراف) {
			ret = ftruncate(ڈیٹا->dev,
					(بلاک + گنتی) * channel->بلاک_سائز + ڈیٹا->انحراف);
			اگر (ret)
				جاؤ err;
		}
		جاؤ unimplemented;
	}
err:
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP)
			جاؤ unimplemented;
		واپس errno;
	}
	واپس 0;
unimplemented:
	واپس EXT2_ET_UNIMPLEMENTED;
}

صحیح ext2fs_open_file(ثابت حرف *pathname, صحیح flags, mode_t منوال)
{
	flags |= O_ثنائی;

	اگر (منوال)
#اگر defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		واپس open64(pathname, flags, منوال);
	ورنہ
		واپس open64(pathname, flags);
#ورنہ
		واپس open(pathname, flags, منوال);
	ورنہ
		واپس open(pathname, flags);
#اگر_ختم
}

صحیح ext2fs_stat(ثابت حرف *راستہ, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	واپس stat64(راستہ, buf);
#ورنہ
	واپس stat(راستہ, buf);
#اگر_ختم
}

صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	واپس fstat64(fd, buf);
#ورنہ
	واپس fstat(fd, buf);
#اگر_ختم
}

#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic نکالو
#اگر_ختم

ساکن ساخت struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.سیٹ_blksize	= windows_سیٹ_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.سیٹ_option	= windows_سیٹ_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
