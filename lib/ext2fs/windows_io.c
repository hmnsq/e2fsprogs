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

#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <winioctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <io.h>

#ਜੇ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ 600
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DARWIN_C_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FILE_ਵਿੱਥ_ਬਿੱਟ 64
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GNU_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PR_GET_DUMPABLE 3
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALIGN_DEBUG

//#define DEBUG
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE(...) {\
		    ਅੱਖਰ __log[256];\
		    snprintf(__log, ਆਕਾਰ(__log), __VA_ARGS__);\
		    __log[ਆਕਾਰ(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE(...) ਕਰੋ { } ਜਦੋਂ (0);
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC_RETURN(ਬਣਤਰ, ਕੋਡ, ret) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ret)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ਬਣਤਰ windows_cache {
	ਅੱਖਰ			*buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਖੰਡ;
	ਪੂਰਨ			access_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ		dirty:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ		ਵਿੱਚ_use:1;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CACHE_ਮਾਪ 8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_ਸਿੱਧਾ_ਮਾਪ 4	/* Must be smaller than CACHE_SIZE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_ਸਿੱਧਾ_ਮਾਪ 4	/* Should be smaller than CACHE_SIZE */

ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ	magic;
	ਅੱਖਰ	name[MAX_ਰਸਤਾ];
	HANDLE	handle;
	ਅੱਖਰ	dos_device[MAX_ਰਸਤਾ];
	ਅੱਖਰ	cf_device[MAX_ਰਸਤਾ];
	ਪੂਰਨ	dev;
	ਪੂਰਨ	flags;
	ਪੂਰਨ	align;
	ਪੂਰਨ	access_ਸਮਾਂ;
	ext2_loff_t ਵਿੱਥ;
	ਬਣਤਰ windows_cache cache[CACHE_ਮਾਪ];
	ਖਾਲੀ	*bounce;
	ਬਣਤਰ struct_io_stats io_stats;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ਸਥਿਰ ਪੂਰਨ fake_dos_name_for_device(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ਅੰਕੜੇ->name, "\\\\", 2) == 0) {
		ਅੰਕੜੇ->dos_device[0] = 0;
		strcpy(ਅੰਕੜੇ->cf_device, ਅੰਕੜੇ->name);
		ਵਾਪਸ 0;
	}

	_snprintf(ਅੰਕੜੇ->dos_device, MAX_ਰਸਤਾ, "fakedevice%lu", GetCurrentProcessId());

	ਜੇ (!DefineDosDevice(DDD_RAW_ਨਿਸ਼ਾਨਾ_ਰਸਤਾ, ਅੰਕੜੇ->dos_device, ਅੰਕੜੇ->name))
		ਵਾਪਸ 1;

	_snprintf(ਅੰਕੜੇ->cf_device, MAX_ਰਸਤਾ, "\\\\.\\%s", ਅੰਕੜੇ->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", ਅੰਕੜੇ->dos_device);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ remove_fake_dos_name(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਜੇ (*ਅੰਕੜੇ->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", ਅੰਕੜੇ->dos_device);
		DefineDosDevice(DDD_RAW_ਨਿਸ਼ਾਨਾ_ਰਸਤਾ | DDD_EXACT_ਮੇਲ_ON_REMOVE | DDD_REMOVE_DEFINITION, ਅੰਕੜੇ->dos_device, ਅੰਕੜੇ->name);
	}
}

ਸਥਿਰ errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ਜੇ (stats)
		*stats = &ਅੰਕੜੇ->io_stats;

	ਵਾਪਸ retval;
}

ਸਥਿਰ LARGE_INTEGER make_large_integer(LONGLONG ਮੁੱਲ)
{
	LARGE_INTEGER	li;

	li.QuadPart = ਮੁੱਲ;
	ਵਾਪਸ li;
}

/*
 * Here are the raw I/O functions
 */
ਸਥਿਰ errcode_t raw_read_blk(io_channel channel,
			      ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *bufv)
{
	errcode_t	retval;
	ssize_t	ਮਾਪ;
	ext2_loff_t	location;
	DWORD		actual = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*buf = bufv;
	ssize_t		really_read = 0;

	ਮਾਪ = (ਗਿਣਤੀ_2 < 0) ? -ਗਿਣਤੀ_2 : ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ;
	ਅੰਕੜੇ->io_stats.ਬਾਈਟਾਂ_read += ਮਾਪ;
	location = ((ext2_loff_t) ਖੰਡ * channel->ਖੰਡ_ਮਾਪ) + ਅੰਕੜੇ->ਵਿੱਥ;

	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_FORCE_BOUNCE) {
		ਜੇ (!ਸੈੱਟ_File_ਸੂਚਕ_Ex(ਅੰਕੜੇ->handle, make_large_integer(location), ਨਿਰਾ, FILE_BEGIN)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜਾਓ bounce_read;
	}

	ਜੇ (!ਸੈੱਟ_File_ਸੂਚਕ_Ex(ਅੰਕੜੇ->handle, make_large_integer(location), ਨਿਰਾ, FILE_BEGIN)) {
		retval = Get_ਆਖਰੀ_Error();
		ਜਾਓ error_ਬਾਹਰਲਾ;
	}
	ਜੇ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ਮਾਪ, channel->align))) {
		ਜੇ (!ReadFile(ਅੰਕੜੇ->handle, buf, ਮਾਪ, &actual, ਨਿਰਾ)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜੇ (actual != ਮਾਪ) {
	short_read:
		ਜੇ (actual < 0) {
			retval = Get_ਆਖਰੀ_Error();
			actual = 0;
		} ਨਹੀਂਤਾਂ
			retval = EXT2_ET_SHORT_READ;
		ਜਾਓ error_ਬਾਹਰਲਾ;
		}
	ਵਾਪਸ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ਜਦੋਂ (ਮਾਪ > 0) {
		ਜੇ (!ReadFile(ਅੰਕੜੇ->handle, ਅੰਕੜੇ->bounce, channel->ਖੰਡ_ਮਾਪ, &actual, ਨਿਰਾ)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜੇ (actual != channel->ਖੰਡ_ਮਾਪ) {
			actual = really_read;
			buf -= really_read;
			ਮਾਪ += really_read;
			ਜਾਓ short_read;
		}
		actual = ਮਾਪ;
		ਜੇ (ਮਾਪ > channel->ਖੰਡ_ਮਾਪ)
			actual = channel->ਖੰਡ_ਮਾਪ;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, ਅੰਕੜੇ->bounce, actual);
		really_read += actual;
		ਮਾਪ -= actual;
		buf += actual;
	}
	ਵਾਪਸ 0;

error_ਬਾਹਰਲਾ:
	ਜੇ (actual >= 0 && actual < ਮਾਪ)
		memset((ਅੱਖਰ *) buf+actual, 0, ਮਾਪ-actual);
	ਜੇ (channel->read_error)
		retval = (channel->read_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
					       ਮਾਪ, actual, retval);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t raw_write_blk(io_channel channel,
			       ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *bufv)
{
	ssize_t		ਮਾਪ;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf = bufv;

	ਜੇ (ਗਿਣਤੀ_2 == 1)
		ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ {
		ਜੇ (ਗਿਣਤੀ_2 < 0)
			ਮਾਪ = -ਗਿਣਤੀ_2;
		ਨਹੀਂਤਾਂ
			ਮਾਪ = ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ;
	}
	ਅੰਕੜੇ->io_stats.ਬਾਈਟਾਂ_written += ਮਾਪ;

	location = ((ext2_loff_t) ਖੰਡ * channel->ਖੰਡ_ਮਾਪ) + ਅੰਕੜੇ->ਵਿੱਥ;

	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_FORCE_BOUNCE) {
		ਜੇ (!ਸੈੱਟ_File_ਸੂਚਕ_Ex(ਅੰਕੜੇ->handle, make_large_integer(location), ਨਿਰਾ, FILE_BEGIN)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
	ਜਾਓ bounce_write;
	}

		ਜੇ (!ਸੈੱਟ_File_ਸੂਚਕ_Ex(ਅੰਕੜੇ->handle, make_large_integer(location), ਨਿਰਾ, FILE_BEGIN)) {
			retval = Get_ਆਖਰੀ_Error();
		ਜਾਓ error_ਬਾਹਰਲਾ;
	}

	ਸੈੱਟ_ਆਖਰੀ_Error(0);

	ਜੇ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ਮਾਪ, channel->align))) {
		ਜੇ (!WriteFile(ਅੰਕੜੇ->handle, buf, ਮਾਪ, &actual, ਨਿਰਾ)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}

		ਜੇ (actual != ਮਾਪ) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਵਾਪਸ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ਜਦੋਂ (ਮਾਪ > 0) {
		ਜੇ (ਮਾਪ < channel->ਖੰਡ_ਮਾਪ) {
			ਜੇ (!ReadFile(ਅੰਕੜੇ->handle, ਅੰਕੜੇ->bounce, channel->ਖੰਡ_ਮਾਪ, &actual, ਨਿਰਾ)) {
				retval = Get_ਆਖਰੀ_Error();
				ਜਾਓ error_ਬਾਹਰਲਾ;
			}
			ਜੇ (actual != channel->ਖੰਡ_ਮਾਪ) {
				ਜੇ (actual < 0) {
					retval = Get_ਆਖਰੀ_Error();
					ਜਾਓ error_ਬਾਹਰਲਾ;
				}
				memset((ਅੱਖਰ *) ਅੰਕੜੇ->bounce + actual, 0,
				       channel->ਖੰਡ_ਮਾਪ - actual);
			}
		}
		actual = ਮਾਪ;
		ਜੇ (ਮਾਪ > channel->ਖੰਡ_ਮਾਪ)
			actual = channel->ਖੰਡ_ਮਾਪ;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ->bounce, buf, actual);
		ਜੇ (!ਸੈੱਟ_File_ਸੂਚਕ_Ex(ਅੰਕੜੇ->handle, make_large_integer(location), ਨਿਰਾ, FILE_BEGIN)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜੇ (!WriteFile(ਅੰਕੜੇ->handle, ਅੰਕੜੇ->bounce, channel->ਖੰਡ_ਮਾਪ, &actual, ਨਿਰਾ)) {
			retval = Get_ਆਖਰੀ_Error();
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}

		ਜੇ (actual != channel->ਖੰਡ_ਮਾਪ)
			ਜਾਓ short_write;
		ਮਾਪ -= actual;
		buf += actual;
		location += actual;
	}
	ਵਾਪਸ 0;

error_ਬਾਹਰਲਾ:
	ਜੇ (channel->write_error)
		retval = (channel->write_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
						ਮਾਪ, actual, retval);
	ਵਾਪਸ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ਸਥਿਰ errcode_t alloc_cache(io_channel channel,
			     ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	errcode_t		retval;
	ਬਣਤਰ windows_cache	*cache;
	ਪੂਰਨ			i;

	ਅੰਕੜੇ->access_ਸਮਾਂ = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < CACHE_ਮਾਪ; i++, cache++) {
		cache->ਖੰਡ = 0;
		cache->access_ਸਮਾਂ = 0;
		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		ਜੇ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਜੇ (channel->align || ਅੰਕੜੇ->flags & IO_FLAG_FORCE_BOUNCE) {
		ਜੇ (ਅੰਕੜੇ->bounce)
			ext2fs_free_mem(&ਅੰਕੜੇ->bounce);
		retval = io_channel_alloc_buf(channel, 0, &ਅੰਕੜੇ->bounce);
	}
	ਵਾਪਸ retval;
}

/* Free the cache buffers */
ਸਥਿਰ ਖਾਲੀ free_cache(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ windows_cache    *cache;
	ਪੂਰਨ			i;

	ਅੰਕੜੇ->access_ਸਮਾਂ = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < CACHE_ਮਾਪ; i++, cache++) {
		cache->ਖੰਡ = 0;
		cache->access_ਸਮਾਂ = 0;
		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		ਜੇ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ਜੇ (ਅੰਕੜੇ->bounce)
		ext2fs_free_mem(&ਅੰਕੜੇ->bounce);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ਸਥਿਰ ਬਣਤਰ windows_cache *ਲੱਭੋ_cached_ਖੰਡ(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					    ਬਣਤਰ windows_cache **eldest)
{
	ਬਣਤਰ windows_cache	*cache, *ਅਣਵਰਤਿਆ_cache, *oldest_cache;
	ਪੂਰਨ			i;

	ਅਣਵਰਤਿਆ_cache = oldest_cache = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < CACHE_ਮਾਪ; i++, cache++) {
		ਜੇ (!cache->ਵਿੱਚ_use) {
			ਜੇ (!ਅਣਵਰਤਿਆ_cache)
				ਅਣਵਰਤਿਆ_cache = cache;
			ਜਾਰੀ;
		}
		ਜੇ (cache->ਖੰਡ == ਖੰਡ) {
			cache->access_ਸਮਾਂ = ++ਅੰਕੜੇ->access_ਸਮਾਂ;
			ਵਾਪਸ cache;
		}
		ਜੇ (!oldest_cache ||
		   (cache->access_ਸਮਾਂ < oldest_cache->access_ਸਮਾਂ))
			oldest_cache = cache;
	}
	ਜੇ (eldest)
		*eldest = (ਅਣਵਰਤਿਆ_cache) ? ਅਣਵਰਤਿਆ_cache : oldest_cache;
	ਵਾਪਸ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ਸਥਿਰ ਖਾਲੀ reuse_cache(io_channel channel, ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
		 ਬਣਤਰ windows_cache *cache, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ)
{
	ਜੇ (cache->dirty && cache->ਵਿੱਚ_use)
		raw_write_blk(channel, ਅੰਕੜੇ, cache->ਖੰਡ, 1, cache->buf);

	cache->ਵਿੱਚ_use = 1;
	cache->dirty = 0;
	cache->ਖੰਡ = ਖੰਡ;
	cache->access_ਸਮਾਂ = ++ਅੰਕੜੇ->access_ਸਮਾਂ;
}

/*
 * Flush all of the blocks in the cache
 */
ਸਥਿਰ errcode_t flush_cached_ਖੰਡਾਂ(io_channel channel,
				     ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
				     ਪੂਰਨ invalidate)
{
	ਬਣਤਰ windows_cache	*cache;
	errcode_t	retval, retval2;
	ਪੂਰਨ			i;

	retval2 = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < CACHE_ਮਾਪ; i++, cache++) {
		ਜੇ (!cache->ਵਿੱਚ_use)
			ਜਾਰੀ;

		ਜੇ (invalidate)
			cache->ਵਿੱਚ_use = 0;

		ਜੇ (!cache->dirty)
			ਜਾਰੀ;

		retval = raw_write_blk(channel, ਅੰਕੜੇ,
				       cache->ਖੰਡ, 1, cache->buf);
		ਜੇ (retval)
			retval2 = retval;
		ਨਹੀਂਤਾਂ
			cache->dirty = 0;
	}
	ਵਾਪਸ retval2;
}
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */

ਸਥਿਰ errcode_t windows_open_channel(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
				   ਪੂਰਨ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ਨਿਰਾ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_io_channel), &io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(io, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(ਅੰਕੜੇ->name)+1, &io->name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(io->name, ਅੰਕੜੇ->name);
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	io->ਖੰਡ_ਮਾਪ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ਜੇ defined(O_ਸਿੱਧਾ)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO)
		io->align = ext2fs_get_dio_alignment(ਅੰਕੜੇ->dev);
#ਜੇ_ਅੰਤ

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ਜੇ (ext2fs_fstat(ਅੰਕੜੇ->dev, &st) == 0) {
		ਜੇ (ext2fsP_is_disk_device(st.st_ਬਹੁਲਕ))
			io->flags |= CHANNEL_FLAGS_ਖੰਡ_DEVICE;
		ਨਹੀਂਤਾਂ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ਜੇ ((retval = alloc_cache(io, ਅੰਕੜੇ)))
		ਜਾਓ cleanup;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKROGET
	ਜੇ (flags & IO_FLAG_RW) {
		ਪੂਰਨ error;
		ਪੂਰਨ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(ਅੰਕੜੇ->dev, BLKROGET, &readonly);
		ਜੇ (!error && readonly) {
			retval = EPERM;
			ਜਾਓ cleanup;
		}
	}
#ਜੇ_ਅੰਤ

	*channel = io;
	ਵਾਪਸ 0;

cleanup:
	ਜੇ (ਅੰਕੜੇ) {
		ਜੇ (ਅੰਕੜੇ->dev >= 0)
			close(ਅੰਕੜੇ->dev);
		free_cache(ਅੰਕੜੇ);
		ext2fs_free_mem(&ਅੰਕੜੇ);
	}
	ਜੇ (io) {
		ਜੇ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ DWORD windows_open_device(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, ਪੂਰਨ open_flags)
{
	DWORD ret = 0;

	ਜੇ (*ਅੰਕੜੇ->name != '\\')
		strcpy(ਅੰਕੜੇ->cf_device, ਅੰਕੜੇ->name);
	ਨਹੀਂਤਾਂ ਜੇ (fake_dos_name_for_device(ਅੰਕੜੇ))
		ਵਾਪਸ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_ਬਹੁਲਕ = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_ਗੁਣ =
#ਜੇ defined(O_ਸਿੱਧਾ)
	(open_flags & O_ਸਿੱਧਾ) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_ਸਾਧਾਰਨ;
#ਨਹੀਂਤਾਂ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#ਜੇ_ਅੰਤ

	ਅੰਕੜੇ->handle = CreateFile(ਅੰਕੜੇ->cf_device, desired_access, share_ਬਹੁਲਕ, ਨਿਰਾ, OPEN_EXISTING,
				  flags_and_ਗੁਣ, ਨਿਰਾ);

	ਜੇ (ਅੰਕੜੇ->handle == INVALID_HANDLE_ਮੁੱਲ) {
		ret = Get_ਆਖਰੀ_Error();
		ਜਾਓ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", ਅੰਕੜੇ->cf_device, ਅੰਕੜੇ->handle);

	ਅੰਕੜੇ->dev = _open_osfhandle((intptr_t)ਅੰਕੜੇ->handle, 0);
	ਜੇ (ਅੰਕੜੇ->dev < 0) {
		ret = Get_ਆਖਰੀ_Error() ? Get_ਆਖਰੀ_Error() : 9999;
		ਜਾਓ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", ਅੰਕੜੇ->handle, ਅੰਕੜੇ->dev);

	ਵਾਪਸ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", ਅੰਕੜੇ->handle);
	CloseHandle(ਅੰਕੜੇ->handle);
invalid_handle:
	remove_fake_dos_name(ਅੰਕੜੇ);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *init_ਨਿੱਜੀ_ਅੰਕੜੇ(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ = ਨਿਰਾ;

	ਜੇ (ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ), &ਅੰਕੜੇ))
		ਵਾਪਸ ਨਿਰਾ;

	memset(ਅੰਕੜੇ, 0, ਆਕਾਰ(ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ));
	strncpy(ਅੰਕੜੇ->name, name, ਆਕਾਰ(ਅੰਕੜੇ->name) - 1);
	ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	ਅੰਕੜੇ->io_stats.ਅੰਕ_fields = 2;
	ਅੰਕੜੇ->flags = flags;
	ਅੰਕੜੇ->handle = INVALID_HANDLE_ਮੁੱਲ;

	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ errcode_t windows_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	ਪੂਰਨ open_flags;
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	ਜੇ (name == 0)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;

	ਅੰਕੜੇ = init_ਨਿੱਜੀ_ਅੰਕੜੇ(name, flags);
	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ਜੇ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ਜੇ defined(O_ਸਿੱਧਾ)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO)
		open_flags |= O_ਸਿੱਧਾ;
#ਜੇ_ਅੰਤ

	ਜੇ (windows_open_device(ਅੰਕੜੇ, open_flags)) {
		ext2fs_free_mem(&ਅੰਕੜੇ);
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
	}

	ਵਾਪਸ windows_open_channel(ਅੰਕੜੇ, flags, channel, windows_io_manager);
}

ਸਥਿਰ errcode_t windows_close(io_channel channel)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
		ਵਾਪਸ 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0);
#ਜੇ_ਅੰਤ

	remove_fake_dos_name(ਅੰਕੜੇ);

	ਜੇ (_close(ਅੰਕੜੇ->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", ਅੰਕੜੇ->dev);

	free_cache(ਅੰਕੜੇ);

	ext2fs_free_mem(&channel->ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਜੇ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t windows_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ਜੇ (channel->ਖੰਡ_ਮਾਪ != blksize) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
		ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0)))
			ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ

		channel->ਖੰਡ_ਮਾਪ = blksize;
		free_cache(ਅੰਕੜੇ);
		ਜੇ ((retval = alloc_cache(channel, ਅੰਕੜੇ)))
			ਵਾਪਸ retval;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t windows_read_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਣਤਰ windows_cache *cache, *reuse[READ_ਸਿੱਧਾ_ਮਾਪ];
	errcode_t	retval;
	ਅੱਖਰ		*cp;
	ਪੂਰਨ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	ਵਾਪਸ raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
#ਨਹੀਂਤਾਂ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ਜੇ (ਗਿਣਤੀ_2 < 0 || ਗਿਣਤੀ_2 > WRITE_ਸਿੱਧਾ_ਮਾਪ) {
		ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0)))
			ਵਾਪਸ retval;
		ਵਾਪਸ raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	}

	cp = buf;
	ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
		/* If it's in the cache, use it! */
		ਜੇ ((cache = ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ, &reuse[0]))) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
			ਲਿਖੋ("Using cached block %lu\n", ਖੰਡ);
#ਜੇ_ਅੰਤ
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cp, cache->buf, channel->ਖੰਡ_ਮਾਪ);
			ਗਿਣਤੀ_2--;
			ਖੰਡ++;
			cp += channel->ਖੰਡ_ਮਾਪ;
			ਜਾਰੀ;
		}
		ਜੇ (ਗਿਣਤੀ_2 == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, ਅੰਕੜੇ, cache, ਖੰਡ);
			ਜੇ ((retval = raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, 1,
						   cache->buf))) {
				cache->ਵਿੱਚ_use = 0;
				ਵਾਪਸ retval;
			}
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cp, cache->buf, channel->ਖੰਡ_ਮਾਪ);
			ਵਾਪਸ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ਲਈ (i=1; i < ਗਿਣਤੀ_2; i++)
			ਜੇ (ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ+i, &reuse[i]))
				ਰੋਕੋ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("Reading %d blocks starting at %lu\n", i, ਖੰਡ);
#ਜੇ_ਅੰਤ
		ਜੇ ((retval = raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, i, cp)))
			ਵਾਪਸ retval;

		/* Save the results in the cache */
		ਲਈ (j=0; j < i; j++) {
			ਗਿਣਤੀ_2--;
			cache = reuse[j];
			reuse_cache(channel, ਅੰਕੜੇ, cache, ਖੰਡ++);
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cache->buf, cp, channel->ਖੰਡ_ਮਾਪ);
			cp += channel->ਖੰਡ_ਮਾਪ;
		}
	}
	ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */
}

ਸਥਿਰ errcode_t windows_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਵਾਪਸ windows_read_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t windows_write_blk64(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਣਤਰ windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ਅਚਲ ਅੱਖਰ	*cp;
	ਪੂਰਨ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	ਵਾਪਸ raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
#ਨਹੀਂਤਾਂ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ਜੇ (ਗਿਣਤੀ_2 < 0 || ਗਿਣਤੀ_2 > WRITE_ਸਿੱਧਾ_ਮਾਪ) {
		ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 1)))
			ਵਾਪਸ retval;
		ਵਾਪਸ raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ਜੇ (writethrough)
		retval = raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);

	cp = buf;
	ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
		cache = ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ, &reuse);
		ਜੇ (!cache) {
			cache = reuse;
			reuse_cache(channel, ਅੰਕੜੇ, cache, ਖੰਡ);
		}
		ਜੇ (cache->buf != cp)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cache->buf, cp, channel->ਖੰਡ_ਮਾਪ);
		cache->dirty = !writethrough;
		ਗਿਣਤੀ_2--;
		ਖੰਡ++;
		cp += channel->ਖੰਡ_ਮਾਪ;
	}
	ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */
}

ਸਥਿਰ errcode_t windows_cache_readahead(io_channel channel,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
}

ਸਥਿਰ errcode_t windows_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ windows_write_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t windows_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				 ਪੂਰਨ ਮਾਪ, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ਸਥਿਰ errcode_t windows_flush(io_channel channel)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0);
#ਜੇ_ਅੰਤ

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t windows_ਸੈੱਟ_option(io_channel channel, ਅਚਲ ਅੱਖਰ *option,
				 ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ tmp;
	ਅੱਖਰ *ਸਿਰਾ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ਜੇ (!strcmp(option, "offset")) {
		ਜੇ (!arg)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		tmp = strtoull(arg, &ਸਿਰਾ, 0);
		ਜੇ (*ਸਿਰਾ)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਅੰਕੜੇ->ਵਿੱਥ = tmp;
		ਜੇ (ਅੰਕੜੇ->ਵਿੱਥ < 0)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
}

ਸਥਿਰ errcode_t windows_discard(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"
#ਜੇ_ਅੰਤ
ਸਥਿਰ errcode_t windows_zeroout(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਪੂਰਨ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ windows_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ਜੇ (channel->flags & CHANNEL_FLAGS_ਖੰਡ_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		ਜਾਓ unimplemented;
	} ਨਹੀਂਤਾਂ {
		/* Regular file, try to use truncate/punch/zero. */
		ਬਣਤਰ stat statbuf;

		ਜੇ (ਗਿਣਤੀ_2 == 0)
			ਵਾਪਸ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(ਅੰਕੜੇ->dev, &statbuf);
		ਜੇ (ret)
			ਜਾਓ err;
		ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) statbuf.st_ਮਾਪ <
			(ਖੰਡ + ਗਿਣਤੀ_2) * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ) {
			ret = ftruncate(ਅੰਕੜੇ->dev,
					(ਖੰਡ + ਗਿਣਤੀ_2) * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ);
			ਜੇ (ret)
				ਜਾਓ err;
		}
		ਜਾਓ unimplemented;
	}
err:
	ਜੇ (ret < 0) {
		ਜੇ (errno == EOPNOTSUPP)
			ਜਾਓ unimplemented;
		ਵਾਪਸ errno;
	}
	ਵਾਪਸ 0;
unimplemented:
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

ਪੂਰਨ ext2fs_open_file(ਅਚਲ ਅੱਖਰ *pathname, ਪੂਰਨ flags, mode_t ਬਹੁਲਕ)
{
	flags |= O_ਦੁਆਧਾਰੀ;

	ਜੇ (ਬਹੁਲਕ)
#ਜੇ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ਵਾਪਸ open64(pathname, flags, ਬਹੁਲਕ);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ open64(pathname, flags);
#ਨਹੀਂਤਾਂ
		ਵਾਪਸ open(pathname, flags, ਬਹੁਲਕ);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ open(pathname, flags);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ ext2fs_stat(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ext2fs_struct_stat *buf)
{
#ਜੇ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ਵਾਪਸ stat64(ਰਸਤਾ, buf);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ stat(ਰਸਤਾ, buf);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ ext2fs_fstat(ਪੂਰਨ fd, ext2fs_struct_stat *buf)
{
#ਜੇ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ਵਾਪਸ fstat64(fd, buf);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ fstat(fd, buf);
#ਜੇ_ਅੰਤ
}

#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.ਸੈੱਟ_blksize	= windows_ਸੈੱਟ_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.ਸੈੱਟ_option	= windows_ਸੈੱਟ_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
