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
#تعریف_کړه XOPEN_سرچینه 600
#تعریف_کړه DARWIN_C_سرچینه
#تعریف_کړه FILE_دننه_واټن_بټونه 64
#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای
#که_نه_تعریف_شوی GNU_سرچینه
#تعریف_کړه GNU_سرچینه
#د_که_پای
#د_که_پای

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#تعریف_کړه PR_GET_DUMPABLE 3
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#ضمن <fcntl.h>

#تعریف_لغوه ALIGN_DEBUG

//#define DEBUG
#که_تعریف_شوی DEBUG
#تعریف_کړه TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#وإلا
#تعریف_کړه TRACE(...) افعل { } طالما (0);
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)
#تعریف_کړه EXT2_CHECK_MAGIC_RETURN(بنية, کوډ, ret) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (ret)

#تعریف_کړه EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

بنية windows_cache {
	حرف			*buf;
	بینښه اوږد اوږد	بلاک;
	صحيح			access_وخت;
	بینښه		dirty:1;
	بینښه		دننه_use:1;
};

#تعریف_کړه CACHE_اندازه 8
#تعریف_کړه WRITE_مستقیم_اندازه 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کړه READ_مستقیم_اندازه 4	/* Should be smaller than CACHE_SIZE */

بنية windows_شخصي_معلومات {
	صحيح	magic;
	حرف	name[MAX_لار];
	HANDLE	handle;
	حرف	dos_device[MAX_لار];
	حرف	cf_device[MAX_لار];
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_وخت;
	ext2_loff_t دننه_واټن;
	بنية windows_cache cache[CACHE_اندازه];
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
};

#تعریف_کړه IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساكن صحيح fake_dos_name_for_device(بنية windows_شخصي_معلومات *معلومات)
{
	إذا (مزي_پرتله(معلومات->name, "\\\\", 2) == 0) {
		معلومات->dos_device[0] = 0;
		strcpy(معلومات->cf_device, معلومات->name);
		أرجع 0;
	}

	_snprintf(معلومات->dos_device, MAX_لار, "fakedevice%lu", GetCurrentProcessId());

	إذا (!DefineDosDevice(DDD_RAW_موخه_لار, معلومات->dos_device, معلومات->name))
		أرجع 1;

	_snprintf(معلومات->cf_device, MAX_لار, "\\\\.\\%s", معلومات->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", معلومات->dos_device);

	أرجع 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_شخصي_معلومات *معلومات)
{
	إذا (*معلومات->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", معلومات->dos_device);
		DefineDosDevice(DDD_RAW_موخه_لار | DDD_EXACT_سمون_ON_REMOVE | DDD_REMOVE_DEFINITION, معلومات->dos_device, معلومات->name);
	}
}

ساكن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية windows_شخصي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (stats)
		*stats = &معلومات->io_stats;

	أرجع retval;
}

ساكن LARGE_INTEGER make_large_integer(LONGLONG ارزښت)
{
	LARGE_INTEGER	li;

	li.QuadPart = ارزښت;
	أرجع li;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية windows_شخصي_معلومات *معلومات,
			      بینښه اوږد اوږد بلاک,
			      صحيح شمېر, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t	اندازه;
	ext2_loff_t	location;
	DWORD		actual = 0;
	بینښه حرف	*buf = bufv;
	ssize_t		really_read = 0;

	اندازه = (شمېر < 0) ? -شمېر : شمېر * channel->بلاک_اندازه;
	معلومات->io_stats.بایټونه_read += اندازه;
	location = ((ext2_loff_t) بلاک * channel->بلاک_اندازه) + معلومات->دننه_واټن;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سېټ_File_اشاره_ګر_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}
		اذهب bounce_read;
	}

	إذا (!سېټ_File_اشاره_ګر_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = Get_وروستی_Error();
		اذهب error_بهر;
	}
	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(اندازه, channel->align))) {
		إذا (!ReadFile(معلومات->handle, buf, اندازه, &actual, عدم)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}
		إذا (actual != اندازه) {
	short_read:
		إذا (actual < 0) {
			retval = Get_وروستی_Error();
			actual = 0;
		} وإلا
			retval = EXT2_ET_SHORT_READ;
		اذهب error_بهر;
		}
	أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	طالما (اندازه > 0) {
		إذا (!ReadFile(معلومات->handle, معلومات->bounce, channel->بلاک_اندازه, &actual, عدم)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}
		إذا (actual != channel->بلاک_اندازه) {
			actual = really_read;
			buf -= really_read;
			اندازه += really_read;
			اذهب short_read;
		}
		actual = اندازه;
		إذا (اندازه > channel->بلاک_اندازه)
			actual = channel->بلاک_اندازه;
		حافظه_نقل(buf, معلومات->bounce, actual);
		really_read += actual;
		اندازه -= actual;
		buf += actual;
	}
	أرجع 0;

error_بهر:
	إذا (actual >= 0 && actual < اندازه)
		memset((حرف *) buf+actual, 0, اندازه-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, شمېر, buf,
					       اندازه, actual, retval);
	أرجع retval;
}

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية windows_شخصي_معلومات *معلومات,
			       بینښه اوږد اوږد بلاک,
			       صحيح شمېر, ثابت فراغ *bufv)
{
	ssize_t		اندازه;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت بینښه حرف *buf = bufv;

	إذا (شمېر == 1)
		اندازه = channel->بلاک_اندازه;
	وإلا {
		إذا (شمېر < 0)
			اندازه = -شمېر;
		وإلا
			اندازه = شمېر * channel->بلاک_اندازه;
	}
	معلومات->io_stats.بایټونه_written += اندازه;

	location = ((ext2_loff_t) بلاک * channel->بلاک_اندازه) + معلومات->دننه_واټن;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سېټ_File_اشاره_ګر_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}
	اذهب bounce_write;
	}

		إذا (!سېټ_File_اشاره_ګر_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_وروستی_Error();
		اذهب error_بهر;
	}

	سېټ_وروستی_Error(0);

	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(اندازه, channel->align))) {
		إذا (!WriteFile(معلومات->handle, buf, اندازه, &actual, عدم)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}

		إذا (actual != اندازه) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_بهر;
		}
		أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	طالما (اندازه > 0) {
		إذا (اندازه < channel->بلاک_اندازه) {
			إذا (!ReadFile(معلومات->handle, معلومات->bounce, channel->بلاک_اندازه, &actual, عدم)) {
				retval = Get_وروستی_Error();
				اذهب error_بهر;
			}
			إذا (actual != channel->بلاک_اندازه) {
				إذا (actual < 0) {
					retval = Get_وروستی_Error();
					اذهب error_بهر;
				}
				memset((حرف *) معلومات->bounce + actual, 0,
				       channel->بلاک_اندازه - actual);
			}
		}
		actual = اندازه;
		إذا (اندازه > channel->بلاک_اندازه)
			actual = channel->بلاک_اندازه;
		حافظه_نقل(معلومات->bounce, buf, actual);
		إذا (!سېټ_File_اشاره_ګر_Ex(معلومات->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}
		إذا (!WriteFile(معلومات->handle, معلومات->bounce, channel->بلاک_اندازه, &actual, عدم)) {
			retval = Get_وروستی_Error();
			اذهب error_بهر;
		}

		إذا (actual != channel->بلاک_اندازه)
			اذهب short_write;
		اندازه -= actual;
		buf += actual;
		location += actual;
	}
	أرجع 0;

error_بهر:
	إذا (channel->write_error)
		retval = (channel->write_error)(channel, بلاک, شمېر, buf,
						اندازه, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية windows_شخصي_معلومات *معلومات)
{
	errcode_t		retval;
	بنية windows_cache	*cache;
	صحيح			i;

	معلومات->access_وخت = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_اندازه; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وخت = 0;
		cache->dirty = 0;
		cache->دننه_use = 0;
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
ساكن فراغ free_cache(بنية windows_شخصي_معلومات *معلومات)
{
	بنية windows_cache    *cache;
	صحيح			i;

	معلومات->access_وخت = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_اندازه; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وخت = 0;
		cache->dirty = 0;
		cache->دننه_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (معلومات->bounce)
		ext2fs_free_mem(&معلومات->bounce);
}

#که_نه_تعریف_شوی NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية windows_cache *پیدا_کړه_cached_بلاک(بنية windows_شخصي_معلومات *معلومات,
					    بینښه اوږد اوږد بلاک,
					    بنية windows_cache **eldest)
{
	بنية windows_cache	*cache, *ناکارول_شوی_cache, *oldest_cache;
	صحيح			i;

	ناکارول_شوی_cache = oldest_cache = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_اندازه; i++, cache++) {
		إذا (!cache->دننه_use) {
			إذا (!ناکارول_شوی_cache)
				ناکارول_شوی_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_وخت = ++معلومات->access_وخت;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		   (cache->access_وخت < oldest_cache->access_وخت))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (ناکارول_شوی_cache) ? ناکارول_شوی_cache : oldest_cache;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_شخصي_معلومات *معلومات,
		 بنية windows_cache *cache, بینښه اوږد اوږد بلاک)
{
	إذا (cache->dirty && cache->دننه_use)
		raw_write_blk(channel, معلومات, cache->بلاک, 1, cache->buf);

	cache->دننه_use = 1;
	cache->dirty = 0;
	cache->بلاک = بلاک;
	cache->access_وخت = ++معلومات->access_وخت;
}

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاکونه(io_channel channel,
				     بنية windows_شخصي_معلومات *معلومات,
				     صحيح invalidate)
{
	بنية windows_cache	*cache;
	errcode_t	retval, retval2;
	صحيح			i;

	retval2 = 0;
	لكل (i=0, cache = معلومات->cache; i < CACHE_اندازه; i++, cache++) {
		إذا (!cache->دننه_use)
			استمر;

		إذا (invalidate)
			cache->دننه_use = 0;

		إذا (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, معلومات,
				       cache->بلاک, 1, cache->buf);
		إذا (retval)
			retval2 = retval;
		وإلا
			cache->dirty = 0;
	}
	أرجع retval2;
}
#د_که_پای /* NO_IO_CACHE */

ساكن errcode_t windows_open_channel(بنية windows_شخصي_معلومات *معلومات,
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
	retval = ext2fs_get_mem(مزي_اوږدوالی(معلومات->name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, معلومات->name);
	io->شخصي_معلومات = معلومات;
	io->بلاک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = ext2fs_get_dio_alignment(معلومات->dev);
#د_که_پای

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(معلومات->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_ډېرپېښ))
			io->flags |= CHANNEL_FLAGS_بلاک_DEVICE;
		وإلا
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	إذا ((retval = alloc_cache(io, معلومات)))
		اذهب cleanup;

#که_تعریف_شوی BLKROGET
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
#د_که_پای

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

ساكن DWORD windows_open_device(بنية windows_شخصي_معلومات *معلومات, صحيح open_flags)
{
	DWORD ret = 0;

	إذا (*معلومات->name != '\\')
		strcpy(معلومات->cf_device, معلومات->name);
	وإلا إذا (fake_dos_name_for_device(معلومات))
		أرجع -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_ډېرپېښ = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_ځانګړتیاوې =
#إذا defined(O_مستقیم)
	(open_flags & O_مستقیم) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_نورمال;
#وإلا
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#د_که_پای

	معلومات->handle = CreateFile(معلومات->cf_device, desired_access, share_ډېرپېښ, عدم, OPEN_EXISTING,
				  flags_and_ځانګړتیاوې, عدم);

	إذا (معلومات->handle == INVALID_HANDLE_ارزښت) {
		ret = Get_وروستی_Error();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", معلومات->cf_device, معلومات->handle);

	معلومات->dev = _open_osfhandle((intptr_t)معلومات->handle, 0);
	إذا (معلومات->dev < 0) {
		ret = Get_وروستی_Error() ? Get_وروستی_Error() : 9999;
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

ساكن بنية windows_شخصي_معلومات *init_شخصي_معلومات(ثابت حرف *name, صحيح flags)
{
	بنية windows_شخصي_معلومات *معلومات = عدم;

	إذا (ext2fs_get_mem(حجم(بنية windows_شخصي_معلومات), &معلومات))
		أرجع عدم;

	memset(معلومات, 0, حجم(بنية windows_شخصي_معلومات));
	strncpy(معلومات->name, name, حجم(معلومات->name) - 1);
	معلومات->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	معلومات->io_stats.عدد_fields = 2;
	معلومات->flags = flags;
	معلومات->handle = INVALID_HANDLE_ارزښت;

	أرجع معلومات;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_شخصي_معلومات *معلومات;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	معلومات = init_شخصي_معلومات(name, flags);
	إذا (!معلومات)
		أرجع EXT2_ET_NO_حافظه;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	إذا (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		open_flags |= O_مستقیم;
#د_که_پای

	إذا (windows_open_device(معلومات, open_flags)) {
		ext2fs_free_mem(&معلومات);
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	أرجع windows_open_channel(معلومات, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#که_نه_تعریف_شوی NO_IO_CACHE
	retval = flush_cached_بلاکونه(channel, معلومات, 0);
#د_که_پای

	remove_fake_dos_name(معلومات);

	إذا (_close(معلومات->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", معلومات->dev);

	free_cache(معلومات);

	ext2fs_free_mem(&channel->شخصي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t windows_سېټ_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_شخصي_معلومات *معلومات;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->بلاک_اندازه != blksize) {
#که_نه_تعریف_شوی NO_IO_CACHE
		إذا ((retval = flush_cached_بلاکونه(channel, معلومات, 0)))
			أرجع retval;
#د_که_پای

		channel->بلاک_اندازه = blksize;
		free_cache(معلومات);
		إذا ((retval = alloc_cache(channel, معلومات)))
			أرجع retval;
	}
	أرجع 0;
}

ساكن errcode_t windows_read_blk64(io_channel channel, بینښه اوږد اوږد بلاک, صحيح شمېر, فراغ *buf)
{
	بنية windows_شخصي_معلومات *معلومات;
	بنية windows_cache *cache, *reuse[READ_مستقیم_اندازه];
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#که_تعریف_شوی NO_IO_CACHE
	أرجع raw_read_blk(channel, معلومات, بلاک, شمېر, buf);
#وإلا
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (شمېر < 0 || شمېر > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلاکونه(channel, معلومات, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, معلومات, بلاک, شمېر, buf);
	}

	cp = buf;
	طالما (شمېر > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = پیدا_کړه_cached_بلاک(معلومات, بلاک, &reuse[0]))) {
#که_تعریف_شوی DEBUG
			چاپ_کړه("Using cached block %lu\n", بلاک);
#د_که_پای
			حافظه_نقل(cp, cache->buf, channel->بلاک_اندازه);
			شمېر--;
			بلاک++;
			cp += channel->بلاک_اندازه;
			استمر;
		}
		إذا (شمېر == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, معلومات, cache, بلاک);
			إذا ((retval = raw_read_blk(channel, معلومات, بلاک, 1,
						   cache->buf))) {
				cache->دننه_use = 0;
				أرجع retval;
			}
			حافظه_نقل(cp, cache->buf, channel->بلاک_اندازه);
			أرجع 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < شمېر; i++)
			إذا (پیدا_کړه_cached_بلاک(معلومات, بلاک+i, &reuse[i]))
				اكسر;
#که_تعریف_شوی DEBUG
		چاپ_کړه("Reading %d blocks starting at %lu\n", i, بلاک);
#د_که_پای
		إذا ((retval = raw_read_blk(channel, معلومات, بلاک, i, cp)))
			أرجع retval;

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			شمېر--;
			cache = reuse[j];
			reuse_cache(channel, معلومات, cache, بلاک++);
			حافظه_نقل(cache->buf, cp, channel->بلاک_اندازه);
			cp += channel->بلاک_اندازه;
		}
	}
	أرجع 0;
#د_که_پای /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	أرجع windows_read_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t windows_write_blk64(io_channel channel,
				بینښه اوږد اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	بنية windows_شخصي_معلومات *معلومات;
	بنية windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#که_تعریف_شوی NO_IO_CACHE
	أرجع raw_write_blk(channel, معلومات, بلاک, شمېر, buf);
#وإلا
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (شمېر < 0 || شمېر > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلاکونه(channel, معلومات, 1)))
			أرجع retval;
		أرجع raw_write_blk(channel, معلومات, بلاک, شمېر, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, معلومات, بلاک, شمېر, buf);

	cp = buf;
	طالما (شمېر > 0) {
		cache = پیدا_کړه_cached_بلاک(معلومات, بلاک, &reuse);
		إذا (!cache) {
			cache = reuse;
			reuse_cache(channel, معلومات, cache, بلاک);
		}
		إذا (cache->buf != cp)
			حافظه_نقل(cache->buf, cp, channel->بلاک_اندازه);
		cache->dirty = !writethrough;
		شمېر--;
		بلاک++;
		cp += channel->بلاک_اندازه;
	}
	أرجع retval;
#د_که_پای /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      بینښه اوږد اوږد بلاک,
				      بینښه اوږد اوږد شمېر)
{
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, بینښه اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	أرجع windows_write_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, بینښه اوږد دننه_واټن,
				 صحيح اندازه, ثابت فراغ *buf)
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t windows_flush(io_channel channel)
{
	بنية windows_شخصي_معلومات *معلومات;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#که_نه_تعریف_شوی NO_IO_CACHE
	retval = flush_cached_بلاکونه(channel, معلومات, 0);
#د_که_پای

	أرجع retval;
}

ساكن errcode_t windows_سېټ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية windows_شخصي_معلومات *معلومات;
	بینښه اوږد اوږد tmp;
	حرف *پای_ټکی;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoull(arg, &پای_ټکی, 0);
		إذا (*پای_ټکی)
			أرجع EXT2_ET_INVALID_دلیل;
		معلومات->دننه_واټن = tmp;
		إذا (معلومات->دننه_واټن < 0)
			أرجع EXT2_ET_INVALID_دلیل;
		أرجع 0;
	}
	أرجع EXT2_ET_INVALID_دلیل;
}

ساكن errcode_t windows_discard(io_channel channel, بینښه اوږد اوږد بلاک,
				 بینښه اوږد اوږد شمېر)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wunused-parameter"
#د_که_پای
ساكن errcode_t windows_zeroout(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية windows_شخصي_معلومات *معلومات;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية windows_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} وإلا {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (شمېر == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(معلومات->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((بینښه اوږد اوږد) statbuf.st_اندازه <
			(بلاک + شمېر) * channel->بلاک_اندازه + معلومات->دننه_واټن) {
			ret = ftruncate(معلومات->dev,
					(بلاک + شمېر) * channel->بلاک_اندازه + معلومات->دننه_واټن);
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

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t ډېرپېښ)
{
	flags |= O_دوګونی;

	إذا (ډېرپېښ)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, ډېرپېښ);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, ډېرپېښ);
	وإلا
		أرجع open(pathname, flags);
#د_که_پای
}

صحيح ext2fs_stat(ثابت حرف *لار, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(لار, buf);
#وإلا
	أرجع stat(لار, buf);
#د_که_پای
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#د_که_پای
}

#إذا __GNUC_PREREQ (4, 6)
#لارښوونه GCC diagnostic راوباسه
#د_که_پای

ساكن بنية struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.سېټ_blksize	= windows_سېټ_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.سېټ_option	= windows_سېټ_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
