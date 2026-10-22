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
#وصف_ڪريو XOPEN_ذريعو 600
#وصف_ڪريو DARWIN_C_ذريعو
#وصف_ڪريو FILE_وٿي_بٽ 64
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل GNU_ذريعو
#وصف_ڪريو GNU_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#وصف_ڪريو PR_GET_DUMPABLE 3
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>

#وصف_رد ALIGN_DEBUG

//#define DEBUG
#جيڪڏهن_وصف_ٿيل DEBUG
#وصف_ڪريو TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#وإلا
#وصف_ڪريو TRACE(...) افعل { } طالما (0);
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)
#وصف_ڪريو EXT2_CHECK_MAGIC_RETURN(بنية, ڪوڊ, ret) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ret)

#وصف_ڪريو EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

بنية windows_cache {
	حرف			*buf;
	بنانشان ڊگهو ڊگهو	بلاڪ;
	صحيح			access_وقت;
	بنانشان		dirty:1;
	بنانشان		اندريون_use:1;
};

#وصف_ڪريو CACHE_سائيز 8
#وصف_ڪريو WRITE_سڌو_سائيز 4	/* Must be smaller than CACHE_SIZE */
#وصف_ڪريو READ_سڌو_سائيز 4	/* Should be smaller than CACHE_SIZE */

بنية windows_ذاتي_معلومات {
	صحيح	magic;
	حرف	name[MAX_رستو];
	HANDLE	handle;
	حرف	dos_device[MAX_رستو];
	حرف	cf_device[MAX_رستو];
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_وقت;
	ext2_loff_t وٿي;
	بنية windows_cache cache[CACHE_سائيز];
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
};

#وصف_ڪريو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساكن صحيح fake_dos_name_for_device(بنية windows_ذاتي_معلومات *معلومات)
{
	إذا (لڙي_ڀيٽ(معلومات->name, "\\\\", 2) == 0) {
		معلومات->dos_device[0] = 0;
		strcpy(معلومات->cf_device, معلومات->name);
		أرجع 0;
	}

	_snprintf(معلومات->dos_device, MAX_رستو, "fakedevice%lu", GetCurrentProcessId());

	إذا (!DefineDosDevice(DDD_RAW_هدف_رستو, معلومات->dos_device, معلومات->name))
		أرجع 1;

	_snprintf(معلومات->cf_device, MAX_رستو, "\\\\.\\%s", معلومات->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", معلومات->dos_device);

	أرجع 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_ذاتي_معلومات *معلومات)
{
	إذا (*معلومات->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", معلومات->dos_device);
		DefineDosDevice(DDD_RAW_هدف_رستو | DDD_EXACT_ميلاپ_ON_REMOVE | DDD_REMOVE_DEFINITION, معلومات->dos_device, معلومات->name);
	}
}

ساكن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية windows_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (stats)
		*stats = &معلومات->io_stats;

	أرجع retval;
}

ساكن LARGE_INTEGER make_large_integer(LONGLONG قدر)
{
	LARGE_INTEGER	li;

	li.QuadPart = قدر;
	أرجع li;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية windows_ذاتي_معلومات *معلومات,
			      بنانشان ڊگهو ڊگهو بلاڪ,
			      صحيح ڳڻپ, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t	سائيز;
	ext2_loff_t	location;
	DWORD		actual = 0;
	بنانشان حرف	*buf = bufv;
	ssize_t		really_read = 0;

	سائيز = (ڳڻپ < 0) ? -ڳڻپ : ڳڻپ * channel->بلاڪ_سائيز;
	معلومات->io_stats.بائيٽون_read += سائيز;
	location = ((ext2_loff_t) بلاڪ * channel->بلاڪ_سائيز) + معلومات->وٿي;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سيٽ_File_اشارو_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}
		اذهب bounce_read;
	}

	إذا (!سيٽ_File_اشارو_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = Get_آخري_Error();
		اذهب error_ٻاهريون;
	}
	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(سائيز, channel->align))) {
		إذا (!ReadFile(معلومات->handle, buf, سائيز, &actual, عدم)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}
		إذا (actual != سائيز) {
	short_read:
		إذا (actual < 0) {
			retval = Get_آخري_Error();
			actual = 0;
		} وإلا
			retval = EXT2_ET_SHORT_READ;
		اذهب error_ٻاهريون;
		}
	أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	طالما (سائيز > 0) {
		إذا (!ReadFile(معلومات->handle, معلومات->bounce, channel->بلاڪ_سائيز, &actual, عدم)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}
		إذا (actual != channel->بلاڪ_سائيز) {
			actual = really_read;
			buf -= really_read;
			سائيز += really_read;
			اذهب short_read;
		}
		actual = سائيز;
		إذا (سائيز > channel->بلاڪ_سائيز)
			actual = channel->بلاڪ_سائيز;
		يادگيري_نقل(buf, معلومات->bounce, actual);
		really_read += actual;
		سائيز -= actual;
		buf += actual;
	}
	أرجع 0;

error_ٻاهريون:
	إذا (actual >= 0 && actual < سائيز)
		memset((حرف *) buf+actual, 0, سائيز-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاڪ, ڳڻپ, buf,
					       سائيز, actual, retval);
	أرجع retval;
}

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية windows_ذاتي_معلومات *معلومات,
			       بنانشان ڊگهو ڊگهو بلاڪ,
			       صحيح ڳڻپ, ثابت فراغ *bufv)
{
	ssize_t		سائيز;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت بنانشان حرف *buf = bufv;

	إذا (ڳڻپ == 1)
		سائيز = channel->بلاڪ_سائيز;
	وإلا {
		إذا (ڳڻپ < 0)
			سائيز = -ڳڻپ;
		وإلا
			سائيز = ڳڻپ * channel->بلاڪ_سائيز;
	}
	معلومات->io_stats.بائيٽون_written += سائيز;

	location = ((ext2_loff_t) بلاڪ * channel->بلاڪ_سائيز) + معلومات->وٿي;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سيٽ_File_اشارو_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}
	اذهب bounce_write;
	}

		إذا (!سيٽ_File_اشارو_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخري_Error();
		اذهب error_ٻاهريون;
	}

	سيٽ_آخري_Error(0);

	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(سائيز, channel->align))) {
		إذا (!WriteFile(معلومات->handle, buf, سائيز, &actual, عدم)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}

		إذا (actual != سائيز) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_ٻاهريون;
		}
		أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	طالما (سائيز > 0) {
		إذا (سائيز < channel->بلاڪ_سائيز) {
			إذا (!ReadFile(معلومات->handle, معلومات->bounce, channel->بلاڪ_سائيز, &actual, عدم)) {
				retval = Get_آخري_Error();
				اذهب error_ٻاهريون;
			}
			إذا (actual != channel->بلاڪ_سائيز) {
				إذا (actual < 0) {
					retval = Get_آخري_Error();
					اذهب error_ٻاهريون;
				}
				memset((حرف *) معلومات->bounce + actual, 0,
				       channel->بلاڪ_سائيز - actual);
			}
		}
		actual = سائيز;
		إذا (سائيز > channel->بلاڪ_سائيز)
			actual = channel->بلاڪ_سائيز;
		يادگيري_نقل(معلومات->bounce, buf, actual);
		إذا (!سيٽ_File_اشارو_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}
		إذا (!WriteFile(معلومات->handle, معلومات->bounce, channel->بلاڪ_سائيز, &actual, عدم)) {
			retval = Get_آخري_Error();
			اذهب error_ٻاهريون;
		}

		إذا (actual != channel->بلاڪ_سائيز)
			اذهب short_write;
		سائيز -= actual;
		buf += actual;
		location += actual;
	}
	أرجع 0;

error_ٻاهريون:
	إذا (channel->write_error)
		retval = (channel->write_error)(channel, بلاڪ, ڳڻپ, buf,
						سائيز, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية windows_ذاتي_معلومات *معلومات)
{
	errcode_t		retval;
	بنية windows_cache	*cache;
	صحيح			i;

	معلومات->access_وقت = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_سائيز; i++, cache++) {
		cache->بلاڪ = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندريون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval)
			أرجع retval;
	}
	إذا (channel->align || معلومات->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (معلومات->bounce)
			ext2fs_free_mem(&معلومات->bounce);
		retval = io_channel_alloc_buf(channel, 0, &معلومات->bounce);
	}
	أرجع retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية windows_ذاتي_معلومات *معلومات)
{
	بنية windows_cache    *cache;
	صحيح			i;

	معلومات->access_وقت = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_سائيز; i++, cache++) {
		cache->بلاڪ = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندريون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (معلومات->bounce)
		ext2fs_free_mem(&معلومات->bounce);
}

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية windows_cache *ڳوليو_cached_بلاڪ(بنية windows_ذاتي_معلومات *معلومات,
					    بنانشان ڊگهو ڊگهو بلاڪ,
					    بنية windows_cache **eldest)
{
	بنية windows_cache	*cache, *اڻ_استعمال_ٿيل_cache, *oldest_cache;
	صحيح			i;

	اڻ_استعمال_ٿيل_cache = oldest_cache = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_سائيز; i++, cache++) {
		إذا (!cache->اندريون_use) {
			إذا (!اڻ_استعمال_ٿيل_cache)
				اڻ_استعمال_ٿيل_cache = cache;
			استمر;
		}
		إذا (cache->بلاڪ == بلاڪ) {
			cache->access_وقت = ++معلومات->access_وقت;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		   (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (اڻ_استعمال_ٿيل_cache) ? اڻ_استعمال_ٿيل_cache : oldest_cache;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_ذاتي_معلومات *معلومات,
		 بنية windows_cache *cache, بنانشان ڊگهو ڊگهو بلاڪ)
{
	إذا (cache->dirty && cache->اندريون_use)
		raw_write_blk(channel, معلومات, cache->بلاڪ, 1, cache->buf);

	cache->اندريون_use = 1;
	cache->dirty = 0;
	cache->بلاڪ = بلاڪ;
	cache->access_وقت = ++معلومات->access_وقت;
}

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاڪون(io_channel channel,
				     بنية windows_ذاتي_معلومات *معلومات,
				     صحيح invalidate)
{
	بنية windows_cache	*cache;
	errcode_t	retval, retval2;
	صحيح			i;

	retval2 = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_سائيز; i++, cache++) {
		إذا (!cache->اندريون_use)
			استمر;

		إذا (invalidate)
			cache->اندريون_use = 0;

		إذا (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, معلومات,
				       cache->بلاڪ, 1, cache->buf);
		إذا (retval)
			retval2 = retval;
		وإلا
			cache->dirty = 0;
	}
	أرجع retval2;
}
#جيڪڏهن_پورو /* NO_IO_CACHE */

ساكن errcode_t windows_open_channel(بنية windows_ذاتي_معلومات *معلومات,
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
	retval = ext2fs_get_mem(لڙي_ڊيگهه(معلومات->name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, معلومات->name);
	io->ذاتي_معلومات = معلومات;
	io->بلاڪ_سائيز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#إذا defined(O_سڌو)
	إذا (flags & IO_FLAG_سڌو_IO)
		io->align = ext2fs_get_dio_alignment(معلومات->dev);
#جيڪڏهن_پورو

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(معلومات->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_گهڻائي))
			io->flags |= CHANNEL_FLAGS_بلاڪ_DEVICE;
		وإلا
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	إذا ((retval = alloc_cache(io, معلومات)))
		اذهب cleanup;

#جيڪڏهن_وصف_ٿيل BLKROGET
	إذا (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(معلومات->dev, BLKROGET, &readonly);
		إذا (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#جيڪڏهن_پورو

	*channel = io;
	أرجع 0;

cleanup:
	إذا (معلومات) {
		إذا (معلومات->dev >= 0)
			close(معلومات->dev);
		free_cache(معلومات);
		ext2fs_free_mem(&معلومات);
	}
	إذا (io) {
		إذا (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	أرجع retval;
}

ساكن DWORD windows_open_device(بنية windows_ذاتي_معلومات *معلومات, صحيح open_flags)
{
	DWORD ret = 0;

	إذا (*معلومات->name != '\\')
		strcpy(معلومات->cf_device, معلومات->name);
	وإلا إذا (fake_dos_name_for_device(معلومات))
		أرجع -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_گهڻائي = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_خاصيتون =
#إذا defined(O_سڌو)
	(open_flags & O_سڌو) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_عام;
#وإلا
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#جيڪڏهن_پورو

	معلومات->handle = CreateFile(معلومات->cf_device, desired_access, share_گهڻائي, عدم, OPEN_EXISTING,
				  flags_and_خاصيتون, عدم);

	إذا (معلومات->handle == INVALID_HANDLE_قدر) {
		ret = Get_آخري_Error();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", معلومات->cf_device, معلومات->handle);

	معلومات->dev = _open_osfhandle((intptr_t)معلومات->handle, 0);
	إذا (معلومات->dev < 0) {
		ret = Get_آخري_Error() ? Get_آخري_Error() : 9999;
		اذهب osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", معلومات->handle, معلومات->dev);

	أرجع 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", معلومات->handle);
	CloseHandle(معلومات->handle);
invalid_handle:
	remove_fake_dos_name(معلومات);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	أرجع ret;
}

ساكن بنية windows_ذاتي_معلومات *init_ذاتي_معلومات(ثابت حرف *name, صحيح flags)
{
	بنية windows_ذاتي_معلومات *معلومات = عدم;

	إذا (ext2fs_get_mem(حجم(بنية windows_ذاتي_معلومات), &معلومات))
		أرجع عدم;

	memset(معلومات, 0, حجم(بنية windows_ذاتي_معلومات));
	strncpy(معلومات->name, name, حجم(معلومات->name) - 1);
	معلومات->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	معلومات->io_stats.انگ_fields = 2;
	معلومات->flags = flags;
	معلومات->handle = INVALID_HANDLE_قدر;

	أرجع معلومات;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_ذاتي_معلومات *معلومات;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	معلومات = init_ذاتي_معلومات(name, flags);
	إذا (!معلومات)
		أرجع EXT2_ET_NO_يادگيري;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	إذا (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#إذا defined(O_سڌو)
	إذا (flags & IO_FLAG_سڌو_IO)
		open_flags |= O_سڌو;
#جيڪڏهن_پورو

	إذا (windows_open_device(معلومات, open_flags)) {
		ext2fs_free_mem(&معلومات);
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	أرجع windows_open_channel(معلومات, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	retval = flush_cached_بلاڪون(channel, معلومات, 0);
#جيڪڏهن_پورو

	remove_fake_dos_name(معلومات);

	إذا (_close(معلومات->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", معلومات->dev);

	free_cache(معلومات);

	ext2fs_free_mem(&channel->ذاتي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t windows_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_ذاتي_معلومات *معلومات;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->بلاڪ_سائيز != blksize) {
#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
		إذا ((retval = flush_cached_بلاڪون(channel, معلومات, 0)))
			أرجع retval;
#جيڪڏهن_پورو

		channel->بلاڪ_سائيز = blksize;
		free_cache(معلومات);
		إذا ((retval = alloc_cache(channel, معلومات)))
			أرجع retval;
	}
	أرجع 0;
}

ساكن errcode_t windows_read_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, فراغ *buf)
{
	بنية windows_ذاتي_معلومات *معلومات;
	بنية windows_cache *cache, *reuse[READ_سڌو_سائيز];
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جيڪڏهن_وصف_ٿيل NO_IO_CACHE
	أرجع raw_read_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
#وإلا
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (ڳڻپ < 0 || ڳڻپ > WRITE_سڌو_سائيز) {
		إذا ((retval = flush_cached_بلاڪون(channel, معلومات, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
	}

	cp = buf;
	طالما (ڳڻپ > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = ڳوليو_cached_بلاڪ(معلومات, بلاڪ, &reuse[0]))) {
#جيڪڏهن_وصف_ٿيل DEBUG
			لکو("Using cached block %lu\n", بلاڪ);
#جيڪڏهن_پورو
			يادگيري_نقل(cp, cache->buf, channel->بلاڪ_سائيز);
			ڳڻپ--;
			بلاڪ++;
			cp += channel->بلاڪ_سائيز;
			استمر;
		}
		إذا (ڳڻپ == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, معلومات, cache, بلاڪ);
			إذا ((retval = raw_read_blk(channel, معلومات, بلاڪ, 1,
						   cache->buf))) {
				cache->اندريون_use = 0;
				أرجع retval;
			}
			يادگيري_نقل(cp, cache->buf, channel->بلاڪ_سائيز);
			أرجع 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < ڳڻپ; i++)
			إذا (ڳوليو_cached_بلاڪ(معلومات, بلاڪ+i, &reuse[i]))
				اكسر;
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("Reading %d blocks starting at %lu\n", i, بلاڪ);
#جيڪڏهن_پورو
		إذا ((retval = raw_read_blk(channel, معلومات, بلاڪ, i, cp)))
			أرجع retval;

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			ڳڻپ--;
			cache = reuse[j];
			reuse_cache(channel, معلومات, cache, بلاڪ++);
			يادگيري_نقل(cache->buf, cp, channel->بلاڪ_سائيز);
			cp += channel->بلاڪ_سائيز;
		}
	}
	أرجع 0;
#جيڪڏهن_پورو /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	أرجع windows_read_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t windows_write_blk64(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	بنية windows_ذاتي_معلومات *معلومات;
	بنية windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جيڪڏهن_وصف_ٿيل NO_IO_CACHE
	أرجع raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
#وإلا
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (ڳڻپ < 0 || ڳڻپ > WRITE_سڌو_سائيز) {
		إذا ((retval = flush_cached_بلاڪون(channel, معلومات, 1)))
			أرجع retval;
		أرجع raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);

	cp = buf;
	طالما (ڳڻپ > 0) {
		cache = ڳوليو_cached_بلاڪ(معلومات, بلاڪ, &reuse);
		إذا (!cache) {
			cache = reuse;
			reuse_cache(channel, معلومات, cache, بلاڪ);
		}
		إذا (cache->buf != cp)
			يادگيري_نقل(cache->buf, cp, channel->بلاڪ_سائيز);
		cache->dirty = !writethrough;
		ڳڻپ--;
		بلاڪ++;
		cp += channel->بلاڪ_سائيز;
	}
	أرجع retval;
#جيڪڏهن_پورو /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      بنانشان ڊگهو ڊگهو بلاڪ,
				      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	أرجع windows_write_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				 صحيح سائيز, ثابت فراغ *buf)
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t windows_flush(io_channel channel)
{
	بنية windows_ذاتي_معلومات *معلومات;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	retval = flush_cached_بلاڪون(channel, معلومات, 0);
#جيڪڏهن_پورو

	أرجع retval;
}

ساكن errcode_t windows_سيٽ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية windows_ذاتي_معلومات *معلومات;
	بنانشان ڊگهو ڊگهو tmp;
	حرف *پڇاڙي;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دليل;

		tmp = strtoull(arg, &پڇاڙي, 0);
		إذا (*پڇاڙي)
			أرجع EXT2_ET_INVALID_دليل;
		معلومات->وٿي = tmp;
		إذا (معلومات->وٿي < 0)
			أرجع EXT2_ET_INVALID_دليل;
		أرجع 0;
	}
	أرجع EXT2_ET_INVALID_دليل;
}

ساكن errcode_t windows_discard(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
				 بنانشان ڊگهو ڊگهو ڳڻپ)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڌڪو
#هدايت GCC diagnostic ignored "-Wunused-parameter"
#جيڪڏهن_پورو
ساكن errcode_t windows_zeroout(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية windows_ذاتي_معلومات *معلومات;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_بلاڪ_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} وإلا {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (ڳڻپ == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(معلومات->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((بنانشان ڊگهو ڊگهو) statbuf.st_سائيز <
			(بلاڪ + ڳڻپ) * channel->بلاڪ_سائيز + معلومات->وٿي) {
			ret = ftruncate(معلومات->dev,
					(بلاڪ + ڳڻپ) * channel->بلاڪ_سائيز + معلومات->وٿي);
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

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t گهڻائي)
{
	flags |= O_ٻٽو;

	إذا (گهڻائي)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, گهڻائي);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, گهڻائي);
	وإلا
		أرجع open(pathname, flags);
#جيڪڏهن_پورو
}

صحيح ext2fs_stat(ثابت حرف *رستو, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(رستو, buf);
#وإلا
	أرجع stat(رستو, buf);
#جيڪڏهن_پورو
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#جيڪڏهن_پورو
}

#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو

ساكن بنية struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.سيٽ_blksize	= windows_سيٽ_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.سيٽ_option	= windows_سيٽ_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
