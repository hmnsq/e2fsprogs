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
#تعریف_کرو XOPEN_سروت 600
#تعریف_کرو DARWIN_C_سروت
#تعریف_کرو FILE_وتھ_بٹ 64
#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت
#جے_بنا_تعریف GNU_سروت
#تعریف_کرو GNU_سروت
#جے_انت
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#تعریف_کرو PR_GET_DUMPABLE 3
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#ضمن <fcntl.h>

#تعریف_رد ALIGN_DEBUG

//#define DEBUG
#جے_تعریف_شدہ DEBUG
#تعریف_کرو TRACE(...) {\
		    حرف __log[256];\
		    snprintf(__log, حجم(__log), __VA_ARGS__);\
		    __log[حجم(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#وإلا
#تعریف_کرو TRACE(...) افعل { } طالما (0);
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)
#تعریف_کرو EXT2_CHECK_MAGIC_RETURN(بنية, کوڈ, ret) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (ret)

#تعریف_کرو EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

بنية windows_cache {
	حرف			*buf;
	بنانشانی لما لما	بلاک;
	صحيح			access_ویلا;
	بنانشانی		dirty:1;
	بنانشانی		وچ_use:1;
};

#تعریف_کرو CACHE_ناپ 8
#تعریف_کرو WRITE_سدھا_ناپ 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کرو READ_سدھا_ناپ 4	/* Should be smaller than CACHE_SIZE */

بنية windows_نجی_انکڑے {
	صحيح	magic;
	حرف	name[MAX_رستہ];
	HANDLE	handle;
	حرف	dos_device[MAX_رستہ];
	حرف	cf_device[MAX_رستہ];
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_ویلا;
	ext2_loff_t وتھ;
	بنية windows_cache cache[CACHE_ناپ];
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
};

#تعریف_کرو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ساكن صحيح fake_dos_name_for_device(بنية windows_نجی_انکڑے *انکڑے)
{
	إذا (لڑی_مقابلہ(انکڑے->name, "\\\\", 2) == 0) {
		انکڑے->dos_device[0] = 0;
		strcpy(انکڑے->cf_device, انکڑے->name);
		أرجع 0;
	}

	_snprintf(انکڑے->dos_device, MAX_رستہ, "fakedevice%lu", GetCurrentProcessId());

	إذا (!DefineDosDevice(DDD_RAW_نشانہ_رستہ, انکڑے->dos_device, انکڑے->name))
		أرجع 1;

	_snprintf(انکڑے->cf_device, MAX_رستہ, "\\\\.\\%s", انکڑے->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", انکڑے->dos_device);

	أرجع 0;
}

ساكن فراغ remove_fake_dos_name(بنية windows_نجی_انکڑے *انکڑے)
{
	إذا (*انکڑے->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", انکڑے->dos_device);
		DefineDosDevice(DDD_RAW_نشانہ_رستہ | DDD_EXACT_میل_ON_REMOVE | DDD_REMOVE_DEFINITION, انکڑے->dos_device, انکڑے->name);
	}
}

ساكن errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية windows_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (stats)
		*stats = &انکڑے->io_stats;

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
			      بنية windows_نجی_انکڑے *انکڑے,
			      بنانشانی لما لما بلاک,
			      صحيح گنتی, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t	ناپ;
	ext2_loff_t	location;
	DWORD		actual = 0;
	بنانشانی حرف	*buf = bufv;
	ssize_t		really_read = 0;

	ناپ = (گنتی < 0) ? -گنتی : گنتی * channel->بلاک_ناپ;
	انکڑے->io_stats.بائٹاں_read += ناپ;
	location = ((ext2_loff_t) بلاک * channel->بلاک_ناپ) + انکڑے->وتھ;

	إذا (انکڑے->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سیٹ_File_اشارہ_گر_Ex(انکڑے->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}
		اذهب bounce_read;
	}

	إذا (!سیٹ_File_اشارہ_گر_Ex(انکڑے->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
		retval = Get_آخری_Error();
		اذهب error_باہرلا;
	}
	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ناپ, channel->align))) {
		إذا (!ReadFile(انکڑے->handle, buf, ناپ, &actual, عدم)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}
		إذا (actual != ناپ) {
	short_read:
		إذا (actual < 0) {
			retval = Get_آخری_Error();
			actual = 0;
		} وإلا
			retval = EXT2_ET_SHORT_READ;
		اذهب error_باہرلا;
		}
	أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	طالما (ناپ > 0) {
		إذا (!ReadFile(انکڑے->handle, انکڑے->bounce, channel->بلاک_ناپ, &actual, عدم)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}
		إذا (actual != channel->بلاک_ناپ) {
			actual = really_read;
			buf -= really_read;
			ناپ += really_read;
			اذهب short_read;
		}
		actual = ناپ;
		إذا (ناپ > channel->بلاک_ناپ)
			actual = channel->بلاک_ناپ;
		حافظہ_نقل(buf, انکڑے->bounce, actual);
		really_read += actual;
		ناپ -= actual;
		buf += actual;
	}
	أرجع 0;

error_باہرلا:
	إذا (actual >= 0 && actual < ناپ)
		memset((حرف *) buf+actual, 0, ناپ-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, گنتی, buf,
					       ناپ, actual, retval);
	أرجع retval;
}

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية windows_نجی_انکڑے *انکڑے,
			       بنانشانی لما لما بلاک,
			       صحيح گنتی, ثابت فراغ *bufv)
{
	ssize_t		ناپ;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ثابت بنانشانی حرف *buf = bufv;

	إذا (گنتی == 1)
		ناپ = channel->بلاک_ناپ;
	وإلا {
		إذا (گنتی < 0)
			ناپ = -گنتی;
		وإلا
			ناپ = گنتی * channel->بلاک_ناپ;
	}
	انکڑے->io_stats.بائٹاں_written += ناپ;

	location = ((ext2_loff_t) بلاک * channel->بلاک_ناپ) + انکڑے->وتھ;

	إذا (انکڑے->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (!سیٹ_File_اشارہ_گر_Ex(انکڑے->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}
	اذهب bounce_write;
	}

		إذا (!سیٹ_File_اشارہ_گر_Ex(انکڑے->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخری_Error();
		اذهب error_باہرلا;
	}

	سیٹ_آخری_Error(0);

	إذا ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ناپ, channel->align))) {
		إذا (!WriteFile(انکڑے->handle, buf, ناپ, &actual, عدم)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}

		إذا (actual != ناپ) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_باہرلا;
		}
		أرجع 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	طالما (ناپ > 0) {
		إذا (ناپ < channel->بلاک_ناپ) {
			إذا (!ReadFile(انکڑے->handle, انکڑے->bounce, channel->بلاک_ناپ, &actual, عدم)) {
				retval = Get_آخری_Error();
				اذهب error_باہرلا;
			}
			إذا (actual != channel->بلاک_ناپ) {
				إذا (actual < 0) {
					retval = Get_آخری_Error();
					اذهب error_باہرلا;
				}
				memset((حرف *) انکڑے->bounce + actual, 0,
				       channel->بلاک_ناپ - actual);
			}
		}
		actual = ناپ;
		إذا (ناپ > channel->بلاک_ناپ)
			actual = channel->بلاک_ناپ;
		حافظہ_نقل(انکڑے->bounce, buf, actual);
		إذا (!سیٹ_File_اشارہ_گر_Ex(انکڑے->handle, make_large_integer(location), عدم, FILE_BEGIN)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}
		إذا (!WriteFile(انکڑے->handle, انکڑے->bounce, channel->بلاک_ناپ, &actual, عدم)) {
			retval = Get_آخری_Error();
			اذهب error_باہرلا;
		}

		إذا (actual != channel->بلاک_ناپ)
			اذهب short_write;
		ناپ -= actual;
		buf += actual;
		location += actual;
	}
	أرجع 0;

error_باہرلا:
	إذا (channel->write_error)
		retval = (channel->write_error)(channel, بلاک, گنتی, buf,
						ناپ, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية windows_نجی_انکڑے *انکڑے)
{
	errcode_t		retval;
	بنية windows_cache	*cache;
	صحيح			i;

	انکڑے->access_ویلا = 0;
	لكل (i=0, cache = انکڑے->cache; i < CACHE_ناپ; i++, cache++) {
		cache->بلاک = 0;
		cache->access_ویلا = 0;
		cache->dirty = 0;
		cache->وچ_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval)
			أرجع retval;
	}
	إذا (channel->align || انکڑے->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (انکڑے->bounce)
			ext2fs_free_mem(&انکڑے->bounce);
		retval = io_channel_alloc_buf(channel, 0, &انکڑے->bounce);
	}
	أرجع retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية windows_نجی_انکڑے *انکڑے)
{
	بنية windows_cache    *cache;
	صحيح			i;

	انکڑے->access_ویلا = 0;
	لكل (i=0, cache = انکڑے->cache; i < CACHE_ناپ; i++, cache++) {
		cache->بلاک = 0;
		cache->access_ویلا = 0;
		cache->dirty = 0;
		cache->وچ_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (انکڑے->bounce)
		ext2fs_free_mem(&انکڑے->bounce);
}

#جے_بنا_تعریف NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية windows_cache *لبھو_cached_بلاک(بنية windows_نجی_انکڑے *انکڑے,
					    بنانشانی لما لما بلاک,
					    بنية windows_cache **eldest)
{
	بنية windows_cache	*cache, *بن_ورتیا_cache, *oldest_cache;
	صحيح			i;

	بن_ورتیا_cache = oldest_cache = 0;
	لكل (i=0, cache = انکڑے->cache; i < CACHE_ناپ; i++, cache++) {
		إذا (!cache->وچ_use) {
			إذا (!بن_ورتیا_cache)
				بن_ورتیا_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_ویلا = ++انکڑے->access_ویلا;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		   (cache->access_ویلا < oldest_cache->access_ویلا))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (بن_ورتیا_cache) ? بن_ورتیا_cache : oldest_cache;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن فراغ reuse_cache(io_channel channel, بنية windows_نجی_انکڑے *انکڑے,
		 بنية windows_cache *cache, بنانشانی لما لما بلاک)
{
	إذا (cache->dirty && cache->وچ_use)
		raw_write_blk(channel, انکڑے, cache->بلاک, 1, cache->buf);

	cache->وچ_use = 1;
	cache->dirty = 0;
	cache->بلاک = بلاک;
	cache->access_ویلا = ++انکڑے->access_ویلا;
}

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاکاں(io_channel channel,
				     بنية windows_نجی_انکڑے *انکڑے,
				     صحيح invalidate)
{
	بنية windows_cache	*cache;
	errcode_t	retval, retval2;
	صحيح			i;

	retval2 = 0;
	لكل (i=0, cache = انکڑے->cache; i < CACHE_ناپ; i++, cache++) {
		إذا (!cache->وچ_use)
			استمر;

		إذا (invalidate)
			cache->وچ_use = 0;

		إذا (!cache->dirty)
			استمر;

		retval = raw_write_blk(channel, انکڑے,
				       cache->بلاک, 1, cache->buf);
		إذا (retval)
			retval2 = retval;
		وإلا
			cache->dirty = 0;
	}
	أرجع retval2;
}
#جے_انت /* NO_IO_CACHE */

ساكن errcode_t windows_open_channel(بنية windows_نجی_انکڑے *انکڑے,
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
	retval = ext2fs_get_mem(لڑی_لمبائی(انکڑے->name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, انکڑے->name);
	io->نجی_انکڑے = انکڑے;
	io->بلاک_ناپ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#إذا defined(O_سدھا)
	إذا (flags & IO_FLAG_سدھا_IO)
		io->align = ext2fs_get_dio_alignment(انکڑے->dev);
#جے_انت

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(انکڑے->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_منوال))
			io->flags |= CHANNEL_FLAGS_بلاک_DEVICE;
		وإلا
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	إذا ((retval = alloc_cache(io, انکڑے)))
		اذهب cleanup;

#جے_تعریف_شدہ BLKROGET
	إذا (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(انکڑے->dev, BLKROGET, &readonly);
		إذا (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#جے_انت

	*channel = io;
	أرجع 0;

cleanup:
	إذا (انکڑے) {
		إذا (انکڑے->dev >= 0)
			close(انکڑے->dev);
		free_cache(انکڑے);
		ext2fs_free_mem(&انکڑے);
	}
	إذا (io) {
		إذا (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	أرجع retval;
}

ساكن DWORD windows_open_device(بنية windows_نجی_انکڑے *انکڑے, صحيح open_flags)
{
	DWORD ret = 0;

	إذا (*انکڑے->name != '\\')
		strcpy(انکڑے->cf_device, انکڑے->name);
	وإلا إذا (fake_dos_name_for_device(انکڑے))
		أرجع -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_منوال = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_گن =
#إذا defined(O_سدھا)
	(open_flags & O_سدھا) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_عام;
#وإلا
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#جے_انت

	انکڑے->handle = CreateFile(انکڑے->cf_device, desired_access, share_منوال, عدم, OPEN_EXISTING,
				  flags_and_گن, عدم);

	إذا (انکڑے->handle == INVALID_HANDLE_قدر) {
		ret = Get_آخری_Error();
		اذهب invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", انکڑے->cf_device, انکڑے->handle);

	انکڑے->dev = _open_osfhandle((intptr_t)انکڑے->handle, 0);
	إذا (انکڑے->dev < 0) {
		ret = Get_آخری_Error() ? Get_آخری_Error() : 9999;
		اذهب osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", انکڑے->handle, انکڑے->dev);

	أرجع 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", انکڑے->handle);
	CloseHandle(انکڑے->handle);
invalid_handle:
	remove_fake_dos_name(انکڑے);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	أرجع ret;
}

ساكن بنية windows_نجی_انکڑے *init_نجی_انکڑے(ثابت حرف *name, صحيح flags)
{
	بنية windows_نجی_انکڑے *انکڑے = عدم;

	إذا (ext2fs_get_mem(حجم(بنية windows_نجی_انکڑے), &انکڑے))
		أرجع عدم;

	memset(انکڑے, 0, حجم(بنية windows_نجی_انکڑے));
	strncpy(انکڑے->name, name, حجم(انکڑے->name) - 1);
	انکڑے->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	انکڑے->io_stats.عدد_fields = 2;
	انکڑے->flags = flags;
	انکڑے->handle = INVALID_HANDLE_قدر;

	أرجع انکڑے;
}

ساكن errcode_t windows_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	صحيح open_flags;
	بنية windows_نجی_انکڑے *انکڑے;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	انکڑے = init_نجی_انکڑے(name, flags);
	إذا (!انکڑے)
		أرجع EXT2_ET_NO_حافظہ;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	إذا (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#إذا defined(O_سدھا)
	إذا (flags & IO_FLAG_سدھا_IO)
		open_flags |= O_سدھا;
#جے_انت

	إذا (windows_open_device(انکڑے, open_flags)) {
		ext2fs_free_mem(&انکڑے);
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	أرجع windows_open_channel(انکڑے, flags, channel, windows_io_manager);
}

ساكن errcode_t windows_close(io_channel channel)
{
	بنية windows_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#جے_بنا_تعریف NO_IO_CACHE
	retval = flush_cached_بلاکاں(channel, انکڑے, 0);
#جے_انت

	remove_fake_dos_name(انکڑے);

	إذا (_close(انکڑے->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", انکڑے->dev);

	free_cache(انکڑے);

	ext2fs_free_mem(&channel->نجی_انکڑے);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t windows_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	بنية windows_نجی_انکڑے *انکڑے;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->بلاک_ناپ != blksize) {
#جے_بنا_تعریف NO_IO_CACHE
		إذا ((retval = flush_cached_بلاکاں(channel, انکڑے, 0)))
			أرجع retval;
#جے_انت

		channel->بلاک_ناپ = blksize;
		free_cache(انکڑے);
		إذا ((retval = alloc_cache(channel, انکڑے)))
			أرجع retval;
	}
	أرجع 0;
}

ساكن errcode_t windows_read_blk64(io_channel channel, بنانشانی لما لما بلاک, صحيح گنتی, فراغ *buf)
{
	بنية windows_نجی_انکڑے *انکڑے;
	بنية windows_cache *cache, *reuse[READ_سدھا_ناپ];
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جے_تعریف_شدہ NO_IO_CACHE
	أرجع raw_read_blk(channel, انکڑے, بلاک, گنتی, buf);
#وإلا
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (گنتی < 0 || گنتی > WRITE_سدھا_ناپ) {
		إذا ((retval = flush_cached_بلاکاں(channel, انکڑے, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, انکڑے, بلاک, گنتی, buf);
	}

	cp = buf;
	طالما (گنتی > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = لبھو_cached_بلاک(انکڑے, بلاک, &reuse[0]))) {
#جے_تعریف_شدہ DEBUG
			لکھو("Using cached block %lu\n", بلاک);
#جے_انت
			حافظہ_نقل(cp, cache->buf, channel->بلاک_ناپ);
			گنتی--;
			بلاک++;
			cp += channel->بلاک_ناپ;
			استمر;
		}
		إذا (گنتی == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, انکڑے, cache, بلاک);
			إذا ((retval = raw_read_blk(channel, انکڑے, بلاک, 1,
						   cache->buf))) {
				cache->وچ_use = 0;
				أرجع retval;
			}
			حافظہ_نقل(cp, cache->buf, channel->بلاک_ناپ);
			أرجع 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < گنتی; i++)
			إذا (لبھو_cached_بلاک(انکڑے, بلاک+i, &reuse[i]))
				اكسر;
#جے_تعریف_شدہ DEBUG
		لکھو("Reading %d blocks starting at %lu\n", i, بلاک);
#جے_انت
		إذا ((retval = raw_read_blk(channel, انکڑے, بلاک, i, cp)))
			أرجع retval;

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			گنتی--;
			cache = reuse[j];
			reuse_cache(channel, انکڑے, cache, بلاک++);
			حافظہ_نقل(cache->buf, cp, channel->بلاک_ناپ);
			cp += channel->بلاک_ناپ;
		}
	}
	أرجع 0;
#جے_انت /* NO_IO_CACHE */
}

ساكن errcode_t windows_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	أرجع windows_read_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t windows_write_blk64(io_channel channel,
				بنانشانی لما لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	بنية windows_نجی_انکڑے *انکڑے;
	بنية windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جے_تعریف_شدہ NO_IO_CACHE
	أرجع raw_write_blk(channel, انکڑے, بلاک, گنتی, buf);
#وإلا
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (گنتی < 0 || گنتی > WRITE_سدھا_ناپ) {
		إذا ((retval = flush_cached_بلاکاں(channel, انکڑے, 1)))
			أرجع retval;
		أرجع raw_write_blk(channel, انکڑے, بلاک, گنتی, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, انکڑے, بلاک, گنتی, buf);

	cp = buf;
	طالما (گنتی > 0) {
		cache = لبھو_cached_بلاک(انکڑے, بلاک, &reuse);
		إذا (!cache) {
			cache = reuse;
			reuse_cache(channel, انکڑے, cache, بلاک);
		}
		إذا (cache->buf != cp)
			حافظہ_نقل(cache->buf, cp, channel->بلاک_ناپ);
		cache->dirty = !writethrough;
		گنتی--;
		بلاک++;
		cp += channel->بلاک_ناپ;
	}
	أرجع retval;
#جے_انت /* NO_IO_CACHE */
}

ساكن errcode_t windows_cache_readahead(io_channel channel,
				      بنانشانی لما لما بلاک,
				      بنانشانی لما لما گنتی)
{
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
}

ساكن errcode_t windows_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	أرجع windows_write_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t windows_write_byte(io_channel channel, بنانشانی لما وتھ,
				 صحيح ناپ, ثابت فراغ *buf)
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t windows_flush(io_channel channel)
{
	بنية windows_نجی_انکڑے *انکڑے;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#جے_بنا_تعریف NO_IO_CACHE
	retval = flush_cached_بلاکاں(channel, انکڑے, 0);
#جے_انت

	أرجع retval;
}

ساكن errcode_t windows_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية windows_نجی_انکڑے *انکڑے;
	بنانشانی لما لما tmp;
	حرف *سرا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoull(arg, &سرا, 0);
		إذا (*سرا)
			أرجع EXT2_ET_INVALID_دلیل;
		انکڑے->وتھ = tmp;
		إذا (انکڑے->وتھ < 0)
			أرجع EXT2_ET_INVALID_دلیل;
		أرجع 0;
	}
	أرجع EXT2_ET_INVALID_دلیل;
}

ساكن errcode_t windows_discard(io_channel channel, بنانشانی لما لما بلاک,
				 بنانشانی لما لما گنتی)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wunused-parameter"
#جے_انت
ساكن errcode_t windows_zeroout(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية windows_نجی_انکڑے *انکڑے;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية windows_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		اذهب unimplemented;
	} وإلا {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (گنتی == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(انکڑے->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((بنانشانی لما لما) statbuf.st_ناپ <
			(بلاک + گنتی) * channel->بلاک_ناپ + انکڑے->وتھ) {
			ret = ftruncate(انکڑے->dev,
					(بلاک + گنتی) * channel->بلاک_ناپ + انکڑے->وتھ);
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

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t منوال)
{
	flags |= O_ثنائی;

	إذا (منوال)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, منوال);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, منوال);
	وإلا
		أرجع open(pathname, flags);
#جے_انت
}

صحيح ext2fs_stat(ثابت حرف *رستہ, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(رستہ, buf);
#وإلا
	أرجع stat(رستہ, buf);
#جے_انت
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#جے_انت
}

#إذا __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic نکالو
#جے_انت

ساكن بنية struct_io_manager struct_windows_manager = {
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
