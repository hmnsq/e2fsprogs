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

#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <winioctl.h>
#ಅನ್ತರ್ಭಾವ <io.h>

#ಆದರೆ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ 600
#ವ್ಯಾಖ್ಯಾನಿಸು DARWIN_C_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು FILE_ಸ್ಥಾನಾಂತರ_ಬಿಟ್ಟುಗಳು 64
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GNU_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು PR_GET_DUMPABLE 3
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ALIGN_DEBUG

//#define DEBUG
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು TRACE(...) {\
		    ಅಕ್ಷರ __log[256];\
		    snprintf(__log, ಗಾತ್ರ(__log), __VA_ARGS__);\
		    __log[ಗಾತ್ರ(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು TRACE(...) ಮಾಡು { } ಆಗ (0);
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC_RETURN(ರಚನೆ, ಸಂಕೇತ_2, ret) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ret)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ರಚನೆ windows_cache {
	ಅಕ್ಷರ			*buf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಖಂಡ;
	ಪೂರ್ಣಾಂಕ			access_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ		dirty:1;
	ಚಿಹ್ನೆರಹಿತ		ಒಳ_use:1;
};

#ವ್ಯಾಖ್ಯಾನಿಸು CACHE_ಗಾತ್ರ 8
#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_ನೇರ_ಗಾತ್ರ 4	/* Must be smaller than CACHE_SIZE */
#ವ್ಯಾಖ್ಯಾನಿಸು READ_ನೇರ_ಗಾತ್ರ 4	/* Should be smaller than CACHE_SIZE */

ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ	magic;
	ಅಕ್ಷರ	name[MAX_ಮಾರ್ಗ];
	HANDLE	handle;
	ಅಕ್ಷರ	dos_device[MAX_ಮಾರ್ಗ];
	ಅಕ್ಷರ	cf_device[MAX_ಮಾರ್ಗ];
	ಪೂರ್ಣಾಂಕ	dev;
	ಪೂರ್ಣಾಂಕ	flags;
	ಪೂರ್ಣಾಂಕ	align;
	ಪೂರ್ಣಾಂಕ	access_ಸಮಯ;
	ext2_loff_t ಸ್ಥಾನಾಂತರ;
	ರಚನೆ windows_cache cache[CACHE_ಗಾತ್ರ];
	ಶೂನ್ಯ	*bounce;
	ರಚನೆ struct_io_stats io_stats;
};

#ವ್ಯಾಖ್ಯಾನಿಸು IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fake_dos_name_for_device(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ದತ್ತಾಂಶ->name, "\\\\", 2) == 0) {
		ದತ್ತಾಂಶ->dos_device[0] = 0;
		strcpy(ದತ್ತಾಂಶ->cf_device, ದತ್ತಾಂಶ->name);
		ಹಿಂತಿರುಗಿ 0;
	}

	_snprintf(ದತ್ತಾಂಶ->dos_device, MAX_ಮಾರ್ಗ, "fakedevice%lu", GetCurrentProcessId());

	ಆದರೆ (!DefineDosDevice(DDD_RAW_ಗುರಿ_ಮಾರ್ಗ, ದತ್ತಾಂಶ->dos_device, ದತ್ತಾಂಶ->name))
		ಹಿಂತಿರುಗಿ 1;

	_snprintf(ದತ್ತಾಂಶ->cf_device, MAX_ಮಾರ್ಗ, "\\\\.\\%s", ದತ್ತಾಂಶ->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", ದತ್ತಾಂಶ->dos_device);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_fake_dos_name(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಆದರೆ (*ದತ್ತಾಂಶ->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", ದತ್ತಾಂಶ->dos_device);
		DefineDosDevice(DDD_RAW_ಗುರಿ_ಮಾರ್ಗ | DDD_EXACT_ಹೊಂದಿಕೆ_ON_REMOVE | DDD_REMOVE_DEFINITION, ದತ್ತಾಂಶ->dos_device, ದತ್ತಾಂಶ->name);
	}
}

ಸ್ಥಿರ errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಆದರೆ (stats)
		*stats = &ದತ್ತಾಂಶ->io_stats;

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ LARGE_INTEGER make_large_integer(LONGLONG ಮೌಲ್ಯ)
{
	LARGE_INTEGER	li;

	li.QuadPart = ಮೌಲ್ಯ;
	ಹಿಂತಿರುಗಿ li;
}

/*
 * Here are the raw I/O functions
 */
ಸ್ಥಿರ errcode_t raw_read_blk(io_channel channel,
			      ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *bufv)
{
	errcode_t	retval;
	ssize_t	ಗಾತ್ರ_2;
	ext2_loff_t	location;
	DWORD		actual = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*buf = bufv;
	ssize_t		really_read = 0;

	ಗಾತ್ರ_2 = (ಎಣಿಕೆ_2 < 0) ? -ಎಣಿಕೆ_2 : ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ;
	ದತ್ತಾಂಶ->io_stats.ಬೈಟುಗಳು_read += ಗಾತ್ರ_2;
	location = ((ext2_loff_t) ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ) + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_FORCE_BOUNCE) {
		ಆದರೆ (!ಗಣ_File_ಸೂಚಕ_Ex(ದತ್ತಾಂಶ->handle, make_large_integer(location), ಖಾಲಿ, FILE_BEGIN)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಹೋಗು bounce_read;
	}

	ಆದರೆ (!ಗಣ_File_ಸೂಚಕ_Ex(ದತ್ತಾಂಶ->handle, make_large_integer(location), ಖಾಲಿ, FILE_BEGIN)) {
		retval = Get_ಕೊನೆಯ_Error();
		ಹೋಗು error_ಹೊರಹೋಗು;
	}
	ಆದರೆ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		ಆದರೆ (!ReadFile(ದತ್ತಾಂಶ->handle, buf, ಗಾತ್ರ_2, &actual, ಖಾಲಿ)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಆದರೆ (actual != ಗಾತ್ರ_2) {
	short_read:
		ಆದರೆ (actual < 0) {
			retval = Get_ಕೊನೆಯ_Error();
			actual = 0;
		} ಇಲ್ಲದಿದ್ದರೆ
			retval = EXT2_ET_SHORT_READ;
		ಹೋಗು error_ಹೊರಹೋಗು;
		}
	ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ಆಗ (ಗಾತ್ರ_2 > 0) {
		ಆದರೆ (!ReadFile(ದತ್ತಾಂಶ->handle, ದತ್ತಾಂಶ->bounce, channel->ಖಂಡ_ಗಾತ್ರ, &actual, ಖಾಲಿ)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಆದರೆ (actual != channel->ಖಂಡ_ಗಾತ್ರ) {
			actual = really_read;
			buf -= really_read;
			ಗಾತ್ರ_2 += really_read;
			ಹೋಗು short_read;
		}
		actual = ಗಾತ್ರ_2;
		ಆದರೆ (ಗಾತ್ರ_2 > channel->ಖಂಡ_ಗಾತ್ರ)
			actual = channel->ಖಂಡ_ಗಾತ್ರ;
		ಸ್ಮರಣೆ_ನಕಲು(buf, ದತ್ತಾಂಶ->bounce, actual);
		really_read += actual;
		ಗಾತ್ರ_2 -= actual;
		buf += actual;
	}
	ಹಿಂತಿರುಗಿ 0;

error_ಹೊರಹೋಗು:
	ಆದರೆ (actual >= 0 && actual < ಗಾತ್ರ_2)
		memset((ಅಕ್ಷರ *) buf+actual, 0, ಗಾತ್ರ_2-actual);
	ಆದರೆ (channel->read_error)
		retval = (channel->read_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
					       ಗಾತ್ರ_2, actual, retval);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t raw_write_blk(io_channel channel,
			       ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *bufv)
{
	ssize_t		ಗಾತ್ರ_2;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf = bufv;

	ಆದರೆ (ಎಣಿಕೆ_2 == 1)
		ಗಾತ್ರ_2 = channel->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಎಣಿಕೆ_2 < 0)
			ಗಾತ್ರ_2 = -ಎಣಿಕೆ_2;
		ಇಲ್ಲದಿದ್ದರೆ
			ಗಾತ್ರ_2 = ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ;
	}
	ದತ್ತಾಂಶ->io_stats.ಬೈಟುಗಳು_written += ಗಾತ್ರ_2;

	location = ((ext2_loff_t) ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ) + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_FORCE_BOUNCE) {
		ಆದರೆ (!ಗಣ_File_ಸೂಚಕ_Ex(ದತ್ತಾಂಶ->handle, make_large_integer(location), ಖಾಲಿ, FILE_BEGIN)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
	ಹೋಗು bounce_write;
	}

		ಆದರೆ (!ಗಣ_File_ಸೂಚಕ_Ex(ದತ್ತಾಂಶ->handle, make_large_integer(location), ಖಾಲಿ, FILE_BEGIN)) {
			retval = Get_ಕೊನೆಯ_Error();
		ಹೋಗು error_ಹೊರಹೋಗು;
	}

	ಗಣ_ಕೊನೆಯ_Error(0);

	ಆದರೆ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		ಆದರೆ (!WriteFile(ದತ್ತಾಂಶ->handle, buf, ಗಾತ್ರ_2, &actual, ಖಾಲಿ)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}

		ಆದರೆ (actual != ಗಾತ್ರ_2) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ಆಗ (ಗಾತ್ರ_2 > 0) {
		ಆದರೆ (ಗಾತ್ರ_2 < channel->ಖಂಡ_ಗಾತ್ರ) {
			ಆದರೆ (!ReadFile(ದತ್ತಾಂಶ->handle, ದತ್ತಾಂಶ->bounce, channel->ಖಂಡ_ಗಾತ್ರ, &actual, ಖಾಲಿ)) {
				retval = Get_ಕೊನೆಯ_Error();
				ಹೋಗು error_ಹೊರಹೋಗು;
			}
			ಆದರೆ (actual != channel->ಖಂಡ_ಗಾತ್ರ) {
				ಆದರೆ (actual < 0) {
					retval = Get_ಕೊನೆಯ_Error();
					ಹೋಗು error_ಹೊರಹೋಗು;
				}
				memset((ಅಕ್ಷರ *) ದತ್ತಾಂಶ->bounce + actual, 0,
				       channel->ಖಂಡ_ಗಾತ್ರ - actual);
			}
		}
		actual = ಗಾತ್ರ_2;
		ಆದರೆ (ಗಾತ್ರ_2 > channel->ಖಂಡ_ಗಾತ್ರ)
			actual = channel->ಖಂಡ_ಗಾತ್ರ;
		ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ->bounce, buf, actual);
		ಆದರೆ (!ಗಣ_File_ಸೂಚಕ_Ex(ದತ್ತಾಂಶ->handle, make_large_integer(location), ಖಾಲಿ, FILE_BEGIN)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಆದರೆ (!WriteFile(ದತ್ತಾಂಶ->handle, ದತ್ತಾಂಶ->bounce, channel->ಖಂಡ_ಗಾತ್ರ, &actual, ಖಾಲಿ)) {
			retval = Get_ಕೊನೆಯ_Error();
			ಹೋಗು error_ಹೊರಹೋಗು;
		}

		ಆದರೆ (actual != channel->ಖಂಡ_ಗಾತ್ರ)
			ಹೋಗು short_write;
		ಗಾತ್ರ_2 -= actual;
		buf += actual;
		location += actual;
	}
	ಹಿಂತಿರುಗಿ 0;

error_ಹೊರಹೋಗು:
	ಆದರೆ (channel->write_error)
		retval = (channel->write_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
						ಗಾತ್ರ_2, actual, retval);
	ಹಿಂತಿರುಗಿ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ಸ್ಥಿರ errcode_t alloc_cache(io_channel channel,
			     ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	errcode_t		retval;
	ರಚನೆ windows_cache	*cache;
	ಪೂರ್ಣಾಂಕ			i;

	ದತ್ತಾಂಶ->access_ಸಮಯ = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < CACHE_ಗಾತ್ರ; i++, cache++) {
		cache->ಖಂಡ = 0;
		cache->access_ಸಮಯ = 0;
		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		ಆದರೆ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (channel->align || ದತ್ತಾಂಶ->flags & IO_FLAG_FORCE_BOUNCE) {
		ಆದರೆ (ದತ್ತಾಂಶ->bounce)
			ext2fs_free_mem(&ದತ್ತಾಂಶ->bounce);
		retval = io_channel_alloc_buf(channel, 0, &ದತ್ತಾಂಶ->bounce);
	}
	ಹಿಂತಿರುಗಿ retval;
}

/* Free the cache buffers */
ಸ್ಥಿರ ಶೂನ್ಯ free_cache(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ windows_cache    *cache;
	ಪೂರ್ಣಾಂಕ			i;

	ದತ್ತಾಂಶ->access_ಸಮಯ = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < CACHE_ಗಾತ್ರ; i++, cache++) {
		cache->ಖಂಡ = 0;
		cache->access_ಸಮಯ = 0;
		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		ಆದರೆ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ಆದರೆ (ದತ್ತಾಂಶ->bounce)
		ext2fs_free_mem(&ದತ್ತಾಂಶ->bounce);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ಸ್ಥಿರ ರಚನೆ windows_cache *ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
					    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					    ರಚನೆ windows_cache **eldest)
{
	ರಚನೆ windows_cache	*cache, *ಬಳಸದ_cache, *oldest_cache;
	ಪೂರ್ಣಾಂಕ			i;

	ಬಳಸದ_cache = oldest_cache = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < CACHE_ಗಾತ್ರ; i++, cache++) {
		ಆದರೆ (!cache->ಒಳ_use) {
			ಆದರೆ (!ಬಳಸದ_cache)
				ಬಳಸದ_cache = cache;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (cache->ಖಂಡ == ಖಂಡ) {
			cache->access_ಸಮಯ = ++ದತ್ತಾಂಶ->access_ಸಮಯ;
			ಹಿಂತಿರುಗಿ cache;
		}
		ಆದರೆ (!oldest_cache ||
		   (cache->access_ಸಮಯ < oldest_cache->access_ಸಮಯ))
			oldest_cache = cache;
	}
	ಆದರೆ (eldest)
		*eldest = (ಬಳಸದ_cache) ? ಬಳಸದ_cache : oldest_cache;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ಸ್ಥಿರ ಶೂನ್ಯ reuse_cache(io_channel channel, ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
		 ರಚನೆ windows_cache *cache, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ)
{
	ಆದರೆ (cache->dirty && cache->ಒಳ_use)
		raw_write_blk(channel, ದತ್ತಾಂಶ, cache->ಖಂಡ, 1, cache->buf);

	cache->ಒಳ_use = 1;
	cache->dirty = 0;
	cache->ಖಂಡ = ಖಂಡ;
	cache->access_ಸಮಯ = ++ದತ್ತಾಂಶ->access_ಸಮಯ;
}

/*
 * Flush all of the blocks in the cache
 */
ಸ್ಥಿರ errcode_t flush_cached_ಖಂಡಗಳು(io_channel channel,
				     ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				     ಪೂರ್ಣಾಂಕ invalidate)
{
	ರಚನೆ windows_cache	*cache;
	errcode_t	retval, retval2;
	ಪೂರ್ಣಾಂಕ			i;

	retval2 = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < CACHE_ಗಾತ್ರ; i++, cache++) {
		ಆದರೆ (!cache->ಒಳ_use)
			ಮುಂದುವರಿ;

		ಆದರೆ (invalidate)
			cache->ಒಳ_use = 0;

		ಆದರೆ (!cache->dirty)
			ಮುಂದುವರಿ;

		retval = raw_write_blk(channel, ದತ್ತಾಂಶ,
				       cache->ಖಂಡ, 1, cache->buf);
		ಆದರೆ (retval)
			retval2 = retval;
		ಇಲ್ಲದಿದ್ದರೆ
			cache->dirty = 0;
	}
	ಹಿಂತಿರುಗಿ retval2;
}
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */

ಸ್ಥಿರ errcode_t windows_open_channel(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				   ಪೂರ್ಣಾಂಕ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ಖಾಲಿ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_io_channel), &io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(io, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ದತ್ತಾಂಶ->name)+1, &io->name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(io->name, ದತ್ತಾಂಶ->name);
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	io->ಖಂಡ_ಗಾತ್ರ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ಆದರೆ defined(O_ನೇರ)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO)
		io->align = ext2fs_get_dio_alignment(ದತ್ತಾಂಶ->dev);
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ಆದರೆ (ext2fs_fstat(ದತ್ತಾಂಶ->dev, &st) == 0) {
		ಆದರೆ (ext2fsP_is_disk_device(st.st_ಬಹುಲಕ))
			io->flags |= CHANNEL_FLAGS_ಖಂಡ_DEVICE;
		ಇಲ್ಲದಿದ್ದರೆ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	ಆದರೆ ((retval = alloc_cache(io, ದತ್ತಾಂಶ)))
		ಹೋಗು cleanup;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKROGET
	ಆದರೆ (flags & IO_FLAG_RW) {
		ಪೂರ್ಣಾಂಕ error;
		ಪೂರ್ಣಾಂಕ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(ದತ್ತಾಂಶ->dev, BLKROGET, &readonly);
		ಆದರೆ (!error && readonly) {
			retval = EPERM;
			ಹೋಗು cleanup;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ

	*channel = io;
	ಹಿಂತಿರುಗಿ 0;

cleanup:
	ಆದರೆ (ದತ್ತಾಂಶ) {
		ಆದರೆ (ದತ್ತಾಂಶ->dev >= 0)
			close(ದತ್ತಾಂಶ->dev);
		free_cache(ದತ್ತಾಂಶ);
		ext2fs_free_mem(&ದತ್ತಾಂಶ);
	}
	ಆದರೆ (io) {
		ಆದರೆ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ DWORD windows_open_device(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ open_flags)
{
	DWORD ret = 0;

	ಆದರೆ (*ದತ್ತಾಂಶ->name != '\\')
		strcpy(ದತ್ತಾಂಶ->cf_device, ದತ್ತಾಂಶ->name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fake_dos_name_for_device(ದತ್ತಾಂಶ))
		ಹಿಂತಿರುಗಿ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_ಬಹುಲಕ = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_ಗುಣಲಕ್ಷಣಗಳು =
#ಆದರೆ defined(O_ನೇರ)
	(open_flags & O_ನೇರ) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_ಸಾಮಾನ್ಯ;
#ಇಲ್ಲದಿದ್ದರೆ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#ಆದರೆ_ಅಂತ್ಯ

	ದತ್ತಾಂಶ->handle = CreateFile(ದತ್ತಾಂಶ->cf_device, desired_access, share_ಬಹುಲಕ, ಖಾಲಿ, OPEN_EXISTING,
				  flags_and_ಗುಣಲಕ್ಷಣಗಳು, ಖಾಲಿ);

	ಆದರೆ (ದತ್ತಾಂಶ->handle == INVALID_HANDLE_ಮೌಲ್ಯ) {
		ret = Get_ಕೊನೆಯ_Error();
		ಹೋಗು invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", ದತ್ತಾಂಶ->cf_device, ದತ್ತಾಂಶ->handle);

	ದತ್ತಾಂಶ->dev = _open_osfhandle((intptr_t)ದತ್ತಾಂಶ->handle, 0);
	ಆದರೆ (ದತ್ತಾಂಶ->dev < 0) {
		ret = Get_ಕೊನೆಯ_Error() ? Get_ಕೊನೆಯ_Error() : 9999;
		ಹೋಗು osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", ದತ್ತಾಂಶ->handle, ದತ್ತಾಂಶ->dev);

	ಹಿಂತಿರುಗಿ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", ದತ್ತಾಂಶ->handle);
	CloseHandle(ದತ್ತಾಂಶ->handle);
invalid_handle:
	remove_fake_dos_name(ದತ್ತಾಂಶ);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *init_ಖಾಸಗಿ_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = ಖಾಲಿ;

	ಆದರೆ (ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ), &ದತ್ತಾಂಶ))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	memset(ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ));
	strncpy(ದತ್ತಾಂಶ->name, name, ಗಾತ್ರ(ದತ್ತಾಂಶ->name) - 1);
	ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	ದತ್ತಾಂಶ->io_stats.ಅಂಕೆ_fields = 2;
	ದತ್ತಾಂಶ->flags = flags;
	ದತ್ತಾಂಶ->handle = INVALID_HANDLE_ಮೌಲ್ಯ;

	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ errcode_t windows_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	ಪೂರ್ಣಾಂಕ open_flags;
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	ಆದರೆ (name == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;

	ದತ್ತಾಂಶ = init_ಖಾಸಗಿ_ದತ್ತಾಂಶ(name, flags);
	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ಆದರೆ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ಆದರೆ defined(O_ನೇರ)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO)
		open_flags |= O_ನೇರ;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (windows_open_device(ದತ್ತಾಂಶ, open_flags)) {
		ext2fs_free_mem(&ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
	}

	ಹಿಂತಿರುಗಿ windows_open_channel(ದತ್ತಾಂಶ, flags, channel, windows_io_manager);
}

ಸ್ಥಿರ errcode_t windows_close(io_channel channel)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
		ಹಿಂತಿರುಗಿ 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0);
#ಆದರೆ_ಅಂತ್ಯ

	remove_fake_dos_name(ದತ್ತಾಂಶ);

	ಆದರೆ (_close(ದತ್ತಾಂಶ->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", ದತ್ತಾಂಶ->dev);

	free_cache(ದತ್ತಾಂಶ);

	ext2fs_free_mem(&channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಆದರೆ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t windows_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಆದರೆ (channel->ಖಂಡ_ಗಾತ್ರ != blksize) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
		ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0)))
			ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ

		channel->ಖಂಡ_ಗಾತ್ರ = blksize;
		free_cache(ದತ್ತಾಂಶ);
		ಆದರೆ ((retval = alloc_cache(channel, ದತ್ತಾಂಶ)))
			ಹಿಂತಿರುಗಿ retval;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t windows_read_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ರಚನೆ windows_cache *cache, *reuse[READ_ನೇರ_ಗಾತ್ರ];
	errcode_t	retval;
	ಅಕ್ಷರ		*cp;
	ಪೂರ್ಣಾಂಕ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_IO_CACHE
	ಹಿಂತಿರುಗಿ raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
#ಇಲ್ಲದಿದ್ದರೆ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ಆದರೆ (ಎಣಿಕೆ_2 < 0 || ಎಣಿಕೆ_2 > WRITE_ನೇರ_ಗಾತ್ರ) {
		ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0)))
			ಹಿಂತಿರುಗಿ retval;
		ಹಿಂತಿರುಗಿ raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	}

	cp = buf;
	ಆಗ (ಎಣಿಕೆ_2 > 0) {
		/* If it's in the cache, use it! */
		ಆದರೆ ((cache = ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ, &reuse[0]))) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
			ಬರೆ("Using cached block %lu\n", ಖಂಡ);
#ಆದರೆ_ಅಂತ್ಯ
			ಸ್ಮರಣೆ_ನಕಲು(cp, cache->buf, channel->ಖಂಡ_ಗಾತ್ರ);
			ಎಣಿಕೆ_2--;
			ಖಂಡ++;
			cp += channel->ಖಂಡ_ಗಾತ್ರ;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (ಎಣಿಕೆ_2 == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, ದತ್ತಾಂಶ, cache, ಖಂಡ);
			ಆದರೆ ((retval = raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, 1,
						   cache->buf))) {
				cache->ಒಳ_use = 0;
				ಹಿಂತಿರುಗಿ retval;
			}
			ಸ್ಮರಣೆ_ನಕಲು(cp, cache->buf, channel->ಖಂಡ_ಗಾತ್ರ);
			ಹಿಂತಿರುಗಿ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ಗಾಗಿ (i=1; i < ಎಣಿಕೆ_2; i++)
			ಆದರೆ (ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ+i, &reuse[i]))
				ನಿಲ್ಲಿಸು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("Reading %d blocks starting at %lu\n", i, ಖಂಡ);
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ ((retval = raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, i, cp)))
			ಹಿಂತಿರುಗಿ retval;

		/* Save the results in the cache */
		ಗಾಗಿ (j=0; j < i; j++) {
			ಎಣಿಕೆ_2--;
			cache = reuse[j];
			reuse_cache(channel, ದತ್ತಾಂಶ, cache, ಖಂಡ++);
			ಸ್ಮರಣೆ_ನಕಲು(cache->buf, cp, channel->ಖಂಡ_ಗಾತ್ರ);
			cp += channel->ಖಂಡ_ಗಾತ್ರ;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t windows_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ windows_read_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t windows_write_blk64(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ರಚನೆ windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*cp;
	ಪೂರ್ಣಾಂಕ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_IO_CACHE
	ಹಿಂತಿರುಗಿ raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
#ಇಲ್ಲದಿದ್ದರೆ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ಆದರೆ (ಎಣಿಕೆ_2 < 0 || ಎಣಿಕೆ_2 > WRITE_ನೇರ_ಗಾತ್ರ) {
		ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 1)))
			ಹಿಂತಿರುಗಿ retval;
		ಹಿಂತಿರುಗಿ raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ಆದರೆ (writethrough)
		retval = raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);

	cp = buf;
	ಆಗ (ಎಣಿಕೆ_2 > 0) {
		cache = ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ, &reuse);
		ಆದರೆ (!cache) {
			cache = reuse;
			reuse_cache(channel, ದತ್ತಾಂಶ, cache, ಖಂಡ);
		}
		ಆದರೆ (cache->buf != cp)
			ಸ್ಮರಣೆ_ನಕಲು(cache->buf, cp, channel->ಖಂಡ_ಗಾತ್ರ);
		cache->dirty = !writethrough;
		ಎಣಿಕೆ_2--;
		ಖಂಡ++;
		cp += channel->ಖಂಡ_ಗಾತ್ರ;
	}
	ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t windows_cache_readahead(io_channel channel,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
}

ಸ್ಥಿರ errcode_t windows_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ windows_write_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t windows_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				 ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t windows_flush(io_channel channel)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0);
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t windows_ಗಣ_option(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ tmp;
	ಅಕ್ಷರ *ಕೊನೆ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಆದರೆ (!strcmp(option, "offset")) {
		ಆದರೆ (!arg)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		tmp = strtoull(arg, &ಕೊನೆ, 0);
		ಆದರೆ (*ಕೊನೆ)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ = tmp;
		ಆದರೆ (ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ < 0)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
}

ಸ್ಥಿರ errcode_t windows_discard(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-parameter"
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ errcode_t windows_zeroout(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ windows_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ಆದರೆ (channel->flags & CHANNEL_FLAGS_ಖಂಡ_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		ಹೋಗು unimplemented;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Regular file, try to use truncate/punch/zero. */
		ರಚನೆ stat statbuf;

		ಆದರೆ (ಎಣಿಕೆ_2 == 0)
			ಹಿಂತಿರುಗಿ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(ದತ್ತಾಂಶ->dev, &statbuf);
		ಆದರೆ (ret)
			ಹೋಗು err;
		ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) statbuf.st_ಗಾತ್ರ <
			(ಖಂಡ + ಎಣಿಕೆ_2) * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ) {
			ret = ftruncate(ದತ್ತಾಂಶ->dev,
					(ಖಂಡ + ಎಣಿಕೆ_2) * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ);
			ಆದರೆ (ret)
				ಹೋಗು err;
		}
		ಹೋಗು unimplemented;
	}
err:
	ಆದರೆ (ret < 0) {
		ಆದರೆ (errno == EOPNOTSUPP)
			ಹೋಗು unimplemented;
		ಹಿಂತಿರುಗಿ errno;
	}
	ಹಿಂತಿರುಗಿ 0;
unimplemented:
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

ಪೂರ್ಣಾಂಕ ext2fs_open_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಪೂರ್ಣಾಂಕ flags, mode_t ಬಹುಲಕ)
{
	flags |= O_ದ್ವಿಮಾನ;

	ಆದರೆ (ಬಹುಲಕ)
#ಆದರೆ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ಹಿಂತಿರುಗಿ open64(pathname, flags, ಬಹುಲಕ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ open64(pathname, flags);
#ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ open(pathname, flags, ಬಹುಲಕ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ open(pathname, flags);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ ext2fs_stat(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ext2fs_struct_stat *buf)
{
#ಆದರೆ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ಹಿಂತಿರುಗಿ stat64(ಮಾರ್ಗ, buf);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ stat(ಮಾರ್ಗ, buf);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ ext2fs_fstat(ಪೂರ್ಣಾಂಕ fd, ext2fs_struct_stat *buf)
{
#ಆದರೆ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ಹಿಂತಿರುಗಿ fstat64(fd, buf);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ fstat(fd, buf);
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.ಗಣ_blksize	= windows_ಗಣ_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.ಗಣ_option	= windows_ಗಣ_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
