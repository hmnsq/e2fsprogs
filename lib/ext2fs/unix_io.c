/*
 * unix_io.c --- This is the Unix (well, really POSIX) implementation
 *	of the I/O manager.
 *
 * Implements a one-block write-through cache.
 *
 * Includes support for Windows NT support under Cygwin.
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *	2002 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಆದರೆ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ 600
#ವ್ಯಾಖ್ಯಾನಿಸು DARWIN_C_ಮೂಲ
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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_MOUNT_H
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_RESOURCE_H
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_LINUX_FALLOC_H
#ಅನ್ತರ್ಭಾವ <linux/falloc.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_FILE_H
#ಅನ್ತರ್ಭಾವ <sys/file.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ALIGN_DEBUG

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ಕೊನೆಯ_ಬಿಟ್_ಗಣ(ಪೂರ್ಣಾಂಕ x)
{
	ಪೂರ್ಣಾಂಕ r = 32;

	ಆದರೆ (!x)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	ಆದರೆ (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	ಆದರೆ (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	ಆದರೆ (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	ಆದರೆ (!(x & 0x80000000u)) {
		r -= 1;
	}
	ಹಿಂತಿರುಗಿ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ highbit32(uint32_t v)
{
	ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_ಕೊನೆಯ_ಬಿಟ್_ಗಣ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

ರಚನೆ unix_cache {
	ಅಕ್ಷರ			*buf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಖಂಡ;
	ಪೂರ್ಣಾಂಕ			access_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ		dirty:1;
	ಚಿಹ್ನೆರಹಿತ		ಒಳ_use:1;
	ಚಿಹ್ನೆರಹಿತ		write_err:1;
};

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_CACHE_ಗಾತ್ರ 8
#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_ನೇರ_ಗಾತ್ರ 4	/* Must be smaller than CACHE_SIZE */
#ವ್ಯಾಖ್ಯಾನಿಸು READ_ನೇರ_ಗಾತ್ರ 4	/* Should be smaller than CACHE_SIZE */

ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ	magic;
	ಪೂರ್ಣಾಂಕ	dev;
	ಪೂರ್ಣಾಂಕ	flags;
	ಪೂರ್ಣಾಂಕ	align;
	ಪೂರ್ಣಾಂಕ	access_ಸಮಯ;
	ಪೂರ್ಣಾಂಕ	unix_flock_flags;
	ext2_loff_t ಸ್ಥಾನಾಂತರ;
	ರಚನೆ unix_cache *cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cache_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cache_hash_ಸರಿಸು;
	ಶೂನ್ಯ	*bounce;
	ರಚನೆ struct_io_stats io_stats;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#ಆದರೆ_ಅಂತ್ಯ
};

#ವ್ಯಾಖ್ಯಾನಿಸು IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ ಬೀಗ_ಬಗೆ {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} ಬಗೆ_t;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
ಸ್ಥಿರ ಸಾಲೊಳಗೆ pthread_mutex_t *get_mutex(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
					 ಬಗೆ_t ಬಗೆ)
{
	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_ಎಳೆಗಳು) {
		ಆಯ್ಕೆ (ಬಗೆ) {
		ಸ್ಥಿತಿ CACHE_MTX:
			ಹಿಂತಿರುಗಿ &ದತ್ತಾಂಶ->cache_mutex;
		ಸ್ಥಿತಿ BOUNCE_MTX:
			ಹಿಂತಿರುಗಿ &ದತ್ತಾಂಶ->bounce_mutex;
		ಸ್ಥಿತಿ STATS_MTX:
			ಹಿಂತಿರುಗಿ &ದತ್ತಾಂಶ->stats_mutex;
		}
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mutex_ಬೀಗ(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ಬಗೆ_t ಬಗೆ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ದತ್ತಾಂಶ,ಬಗೆ);

	ಆದರೆ (mtx)
		pthread_mutex_ಬೀಗ(mtx);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mutex_unlock(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ಬಗೆ_t ಬಗೆ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ದತ್ತಾಂಶ,ಬಗೆ);

	ಆದರೆ (mtx)
		pthread_mutex_unlock(mtx);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (stats) {
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, STATS_MTX);
		*stats = &ದತ್ತಾಂಶ->io_stats;
		mutex_unlock(ದತ್ತಾಂಶ, STATS_MTX);
	}

	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Here are the raw I/O functions
 */
ಸ್ಥಿರ errcode_t raw_read_blk(io_channel channel,
			      ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *bufv)
{
	errcode_t	retval;
	ssize_t		ಗಾತ್ರ_2;
	ext2_loff_t	location;
	ಪೂರ್ಣಾಂಕ		actual = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*buf = bufv;
	ssize_t		really_read = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ aligned_blk;
	ಪೂರ್ಣಾಂಕ		align_ಗಾತ್ರ, ಸ್ಥಾನಾಂತರ;

	ಗಾತ್ರ_2 = (ಎಣಿಕೆ_2 < 0) ? -ಎಣಿಕೆ_2 : (ext2_loff_t) ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ;
	mutex_ಬೀಗ(ದತ್ತಾಂಶ, STATS_MTX);
	ದತ್ತಾಂಶ->io_stats.ಬೈಟುಗಳು_read += ಗಾತ್ರ_2;
	mutex_unlock(ದತ್ತಾಂಶ, STATS_MTX);
	location = ((ext2_loff_t) ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ) + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_FORCE_BOUNCE)
		ಹೋಗು bounce_read;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PREAD64
	/* Try an aligned pread */
	ಆದರೆ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		actual = pread64(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2, location);
		ಆದರೆ (actual == ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ 0;
		actual = 0;
	}
#ಅಥವಾಆದರೆ HAVE_PREAD
	/* Try an aligned pread */
	ಆದರೆ ((ಗಾತ್ರ(off_t) >= ಗಾತ್ರ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ಗಾತ್ರ_2, channel->align)))) {
		actual = pread(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2, location);
		ಆದರೆ (actual == ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ 0;
		actual = 0;
	}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_PREAD */

	ಆದರೆ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, BOUNCE_MTX);
		ಆದರೆ (ext2fs_llseek(ದತ್ತಾಂಶ->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ಹೋಗು error_unlock;
		}
		actual = ಪಠ(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2);
		ಆದರೆ (actual != ಗಾತ್ರ_2) {
		short_read:
			ಆದರೆ (actual < 0) {
				retval = errno;
				actual = 0;
			} ಇಲ್ಲದಿದ್ದರೆ
				retval = EXT2_ET_SHORT_READ;
			ಹೋಗು error_unlock;
		}
		ಹೋಗು success_unlock;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ALIGN_DEBUG
	ಬರೆ("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ಗಾತ್ರ_2);
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ಆದರೆ (channel->align == 0)
		channel->align = 1;
	ಆದರೆ ((channel->ಖಂಡ_ಗಾತ್ರ > channel->align) &&
	    (channel->ಖಂಡ_ಗಾತ್ರ % channel->align) == 0)
		align_ಗಾತ್ರ = channel->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ
		align_ಗಾತ್ರ = channel->align;
	aligned_blk = location / align_ಗಾತ್ರ;
	ಸ್ಥಾನಾಂತರ = location % align_ಗಾತ್ರ;

	mutex_ಬೀಗ(ದತ್ತಾಂಶ, BOUNCE_MTX);
	ಆದರೆ (ext2fs_llseek(ದತ್ತಾಂಶ->dev, aligned_blk * align_ಗಾತ್ರ, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		ಹೋಗು error_unlock;
	}
	ಆಗ (ಗಾತ್ರ_2 > 0) {
		actual = ಪಠ(ದತ್ತಾಂಶ->dev, ದತ್ತಾಂಶ->bounce, align_ಗಾತ್ರ);
		ಆದರೆ (actual != align_ಗಾತ್ರ) {
			actual = really_read;
			buf -= really_read;
			ಗಾತ್ರ_2 += really_read;
			ಹೋಗು short_read;
		}
		ಆದರೆ ((actual + ಸ್ಥಾನಾಂತರ) > align_ಗಾತ್ರ)
			actual = align_ಗಾತ್ರ - ಸ್ಥಾನಾಂತರ;
		ಆದರೆ (actual > ಗಾತ್ರ_2)
			actual = ಗಾತ್ರ_2;
		ಸ್ಮರಣೆ_ನಕಲು(buf, (ಅಕ್ಷರ *)ದತ್ತಾಂಶ->bounce + ಸ್ಥಾನಾಂತರ, actual);

		really_read += actual;
		ಗಾತ್ರ_2 -= actual;
		buf += actual;
		ಸ್ಥಾನಾಂತರ = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
	ಹಿಂತಿರುಗಿ 0;

error_unlock:
	mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
	ಆದರೆ (actual >= 0 && actual < ಗಾತ್ರ_2)
		memset((ಅಕ್ಷರ *) buf+actual, 0, ಗಾತ್ರ_2-actual);
	ಆದರೆ (channel->read_error)
		retval = (channel->read_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
					       ಗಾತ್ರ_2, actual, retval);
	ಹಿಂತಿರುಗಿ retval;
}

#ವ್ಯಾಖ್ಯಾನಿಸು RAW_WRITE_NO_HANDLER	(1U << 0)
#ವ್ಯಾಖ್ಯಾನಿಸು RAW_WRITE_NOLOCK	(1U << 1)

ಸ್ಥಿರ errcode_t raw_write_blk(io_channel channel,
			       ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *bufv,
			       ಪೂರ್ಣಾಂಕ flags)
{
	ssize_t		ಗಾತ್ರ_2;
	ext2_loff_t	location;
	ಪೂರ್ಣಾಂಕ		actual = 0;
	errcode_t	retval;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf = bufv;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ aligned_blk;
	ಪೂರ್ಣಾಂಕ		align_ಗಾತ್ರ, ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ಎಣಿಕೆ_2 == 1)
		ಗಾತ್ರ_2 = channel->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಎಣಿಕೆ_2 < 0)
			ಗಾತ್ರ_2 = -ಎಣಿಕೆ_2;
		ಇಲ್ಲದಿದ್ದರೆ
			ಗಾತ್ರ_2 = (ext2_loff_t) ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ;
	}
	mutex_ಬೀಗ(ದತ್ತಾಂಶ, STATS_MTX);
	ದತ್ತಾಂಶ->io_stats.ಬೈಟುಗಳು_written += ಗಾತ್ರ_2;
	mutex_unlock(ದತ್ತಾಂಶ, STATS_MTX);

	location = ((ext2_loff_t) ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ) + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_FORCE_BOUNCE)
		ಹೋಗು bounce_write;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PWRITE64
	/* Try an aligned pwrite */
	ಆದರೆ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		actual = pwrite64(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2, location);
		ಆದರೆ (actual == ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ 0;
	}
#ಅಥವಾಆದರೆ HAVE_PWRITE
	/* Try an aligned pwrite */
	ಆದರೆ ((ಗಾತ್ರ(off_t) >= ಗಾತ್ರ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ಗಾತ್ರ_2, channel->align)))) {
		actual = pwrite(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2, location);
		ಆದರೆ (actual == ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_PWRITE */

	ಆದರೆ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ಗಾತ್ರ_2, channel->align))) {
		ಆದರೆ (!(flags & RAW_WRITE_NOLOCK))
			mutex_ಬೀಗ(ದತ್ತಾಂಶ, BOUNCE_MTX);
		ಆದರೆ (ext2fs_llseek(ದತ್ತಾಂಶ->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ಹೋಗು error_unlock;
		}
		actual = ಲಿಖ(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2);
		ಆದರೆ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
		ಆದರೆ (actual < 0) {
			retval = errno;
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಆದರೆ (actual != ಗಾತ್ರ_2) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ALIGN_DEBUG
	ಬರೆ("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ಗಾತ್ರ_2);
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ಆದರೆ (channel->align == 0)
		channel->align = 1;
	ಆದರೆ ((channel->ಖಂಡ_ಗಾತ್ರ > channel->align) &&
	    (channel->ಖಂಡ_ಗಾತ್ರ % channel->align) == 0)
		align_ಗಾತ್ರ = channel->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ
		align_ಗಾತ್ರ = channel->align;
	aligned_blk = location / align_ಗಾತ್ರ;
	ಸ್ಥಾನಾಂತರ = location % align_ಗಾತ್ರ;

	ಆಗ (ಗಾತ್ರ_2 > 0) {
		ಪೂರ್ಣಾಂಕ actual_w;

		ಆದರೆ (!(flags & RAW_WRITE_NOLOCK))
			mutex_ಬೀಗ(ದತ್ತಾಂಶ, BOUNCE_MTX);
		ಆದರೆ (ಗಾತ್ರ_2 < align_ಗಾತ್ರ || ಸ್ಥಾನಾಂತರ) {
			ಆದರೆ (ext2fs_llseek(ದತ್ತಾಂಶ->dev, aligned_blk * align_ಗಾತ್ರ,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				ಹೋಗು error_unlock;
			}
			actual = ಪಠ(ದತ್ತಾಂಶ->dev, ದತ್ತಾಂಶ->bounce,
				      align_ಗಾತ್ರ);
			ಆದರೆ (actual != align_ಗಾತ್ರ) {
				ಆದರೆ (actual < 0) {
					retval = errno;
					ಹೋಗು error_unlock;
				}
				memset((ಅಕ್ಷರ *) ದತ್ತಾಂಶ->bounce + actual, 0,
				       align_ಗಾತ್ರ - actual);
			}
		}
		actual = ಗಾತ್ರ_2;
		ಆದರೆ ((actual + ಸ್ಥಾನಾಂತರ) > align_ಗಾತ್ರ)
			actual = align_ಗಾತ್ರ - ಸ್ಥಾನಾಂತರ;
		ಆದರೆ (actual > ಗಾತ್ರ_2)
			actual = ಗಾತ್ರ_2;
		ಸ್ಮರಣೆ_ನಕಲು(((ಅಕ್ಷರ *)ದತ್ತಾಂಶ->bounce) + ಸ್ಥಾನಾಂತರ, buf, actual);
		ಆದರೆ (ext2fs_llseek(ದತ್ತಾಂಶ->dev, aligned_blk * align_ಗಾತ್ರ, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ಹೋಗು error_unlock;
		}
		actual_w = ಲಿಖ(ದತ್ತಾಂಶ->dev, ದತ್ತಾಂಶ->bounce, align_ಗಾತ್ರ);
		ಆದರೆ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
		ಆದರೆ (actual_w < 0) {
			retval = errno;
			ಹೋಗು error_ಹೊರಹೋಗು;
		}
		ಆದರೆ (actual_w != align_ಗಾತ್ರ)
			ಹೋಗು short_write;
		ಗಾತ್ರ_2 -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		ಸ್ಥಾನಾಂತರ = 0;
	}
	ಹಿಂತಿರುಗಿ 0;

error_unlock:
	ಆದರೆ (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
error_ಹೊರಹೋಗು:
	ಆದರೆ (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
						ಗಾತ್ರ_2, actual, retval);
	ಹಿಂತಿರುಗಿ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ಸ್ಥಿರ errcode_t alloc_cache(io_channel channel,
			     ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	errcode_t		retval;
	ರಚನೆ unix_cache	*cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;

	ದತ್ತಾಂಶ->access_ಸಮಯ = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ; i++, cache++) {
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
ಸ್ಥಿರ ಶೂನ್ಯ free_cache(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ unix_cache	*cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;

	ದತ್ತಾಂಶ->access_ಸಮಯ = 0;
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ; i++, cache++) {
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

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#ವ್ಯಾಖ್ಯಾನಿಸು GOLDEN_ಅನುಪಾತ_ಅವಿಭಾಜ್ಯ	0x9e37fffffffc0001UL
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CACHE_ಸಾಲು_ಗಾತ್ರ
/* if the system didn't tell us, guess something reasonable */
#ವ್ಯಾಖ್ಯಾನಿಸು CACHE_ಸಾಲು_ಗಾತ್ರ		64
#ಆದರೆ_ಅಂತ್ಯ

/* buffer cache hashing function, crudely stolen from xfsprogs */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
cache_hash(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	ಆದರೆ (ದತ್ತಾಂಶ->cache_ಗಾತ್ರ <= DEFAULT_CACHE_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;

	tmp = hashval ^ (GOLDEN_ಅನುಪಾತ_ಅವಿಭಾಜ್ಯ + hashval) / CACHE_ಸಾಲು_ಗಾತ್ರ;
	tmp = tmp ^ ((tmp ^ GOLDEN_ಅನುಪಾತ_ಅವಿಭಾಜ್ಯ) >> ದತ್ತಾಂಶ->cache_hash_ಸರಿಸು);
	ಹಿಂತಿರುಗಿ tmp % ದತ್ತಾಂಶ->cache_ಗಾತ್ರ;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ಸ್ಥಿರ ರಚನೆ unix_cache *ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
					    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					    ರಚನೆ unix_cache **eldest)
{
	ರಚನೆ unix_cache	*cache, *ಬಳಸದ_cache, *oldest_cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		hash = cache_hash(ದತ್ತಾಂಶ, ಖಂಡ);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;

	ಬಳಸದ_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	ಗಾಗಿ (i = hash, cache = ದತ್ತಾಂಶ->cache + hash;
	     i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ;
	     i++, cache++) {
		ಆದರೆ (!cache->ಒಳ_use) {
			ಆದರೆ (!ಬಳಸದ_cache)
				ಬಳಸದ_cache = cache;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (cache->ಖಂಡ == ಖಂಡ) {
			cache->access_ಸಮಯ = ++ದತ್ತಾಂಶ->access_ಸಮಯ;
			ದತ್ತಾಂಶ->io_stats.cache_ದೊರಕಿದವು++;
			ಹಿಂತಿರುಗಿ cache;
		}
		ಆದರೆ (!oldest_cache ||
		    (cache->access_ಸಮಯ < oldest_cache->access_ಸಮಯ))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	ಗಾಗಿ (i = 0, cache = ದತ್ತಾಂಶ->cache; i < hash; i++, cache++) {
		ಆದರೆ (!cache->ಒಳ_use) {
			ಆದರೆ (!ಬಳಸದ_cache)
				ಬಳಸದ_cache = cache;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (cache->ಖಂಡ == ಖಂಡ) {
			cache->access_ಸಮಯ = ++ದತ್ತಾಂಶ->access_ಸಮಯ;
			ದತ್ತಾಂಶ->io_stats.cache_ದೊರಕಿದವು++;
			ಹಿಂತಿರುಗಿ cache;
		}
		ಆದರೆ (!oldest_cache ||
		    (cache->access_ಸಮಯ < oldest_cache->access_ಸಮಯ))
			oldest_cache = cache;
	}
	ಆದರೆ (eldest)
		*eldest = (ಬಳಸದ_cache) ? ಬಳಸದ_cache : oldest_cache;
	ದತ್ತಾಂಶ->io_stats.cache_ತಪ್ಪಿದವು++;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ಸ್ಥಿರ errcode_t reuse_cache(io_channel channel,
		ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ರಚನೆ unix_cache *cache,
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ)
{
	ಆದರೆ (cache->dirty && cache->ಒಳ_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, ದತ್ತಾಂಶ, cache->ಖಂಡ, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		ಆದರೆ (retval) {
			cache->write_err = 1;
			ಹಿಂತಿರುಗಿ retval;
		}
	}

	cache->ಒಳ_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->ಖಂಡ = ಖಂಡ;
	cache->access_ಸಮಯ = ++ದತ್ತಾಂಶ->access_ಸಮಯ;
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು FLUSH_INVALIDATE	0x01
#ವ್ಯಾಖ್ಯಾನಿಸು FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ಸ್ಥಿರ errcode_t flush_cached_ಖಂಡಗಳು(io_channel channel,
				     ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				     ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;
	ಪೂರ್ಣಾಂಕ			errors_ಸಿಕ್ಕಿತು = 0;

	ಆದರೆ ((flags & FLUSH_NOLOCK) == 0)
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
	ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ; i++, cache++) {
		ಆದರೆ (!cache->ಒಳ_use)
			ಮುಂದುವರಿ;
		ಆದರೆ (cache->dirty) {
			ಪೂರ್ಣಾಂಕ raw_flags = RAW_WRITE_NO_HANDLER;

			ಆದರೆ (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, ದತ್ತಾಂಶ,
					       cache->ಖಂಡ, 1, cache->buf,
					       raw_flags);
			ಆದರೆ (retval) {
				cache->write_err = 1;
				errors_ಸಿಕ್ಕಿತು = 1;
				retval2 = retval;
			} ಇಲ್ಲದಿದ್ದರೆ {
				cache->dirty = 0;
				cache->write_err = 0;
				ಆದರೆ (flags & FLUSH_INVALIDATE)
					cache->ಒಳ_use = 0;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & FLUSH_INVALIDATE) {
			cache->ಒಳ_use = 0;
		}
	}
	ಆದರೆ ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
retry:
	ಆಗ (errors_ಸಿಕ್ಕಿತು) {
		ಆದರೆ ((flags & FLUSH_NOLOCK) == 0)
			mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
		errors_ಸಿಕ್ಕಿತು = 0;
		ಗಾಗಿ (i=0, cache = ದತ್ತಾಂಶ->cache; i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ; i++, cache++) {
			ಆದರೆ (!cache->ಒಳ_use || !cache->write_err)
				ಮುಂದುವರಿ;
			errors_ಸಿಕ್ಕಿತು = 1;
			ಆದರೆ (cache->write_err && channel->write_error) {
				ಅಕ್ಷರ *err_buf = ಖಾಲಿ;
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ err_ಖಂಡ = cache->ಖಂಡ;

				cache->dirty = 0;
				cache->ಒಳ_use = 0;
				cache->write_err = 0;
				ಆದರೆ (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ಖಾಲಿ;
				ಇಲ್ಲದಿದ್ದರೆ
					ಸ್ಮರಣೆ_ನಕಲು(err_buf, cache->buf,
					       channel->ಖಂಡ_ಗಾತ್ರ);
				mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
				(channel->write_error)(channel, err_ಖಂಡ,
					1, err_buf, channel->ಖಂಡ_ಗಾತ್ರ, -1,
					retval2);
				ಆದರೆ (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
				ಹೋಗು retry;
			} ಇಲ್ಲದಿದ್ದರೆ
				cache->write_err = 0;
		}
		ಆದರೆ ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	}
	ಹಿಂತಿರುಗಿ retval2;
}

/* Shrink the cache buffers */
ಸ್ಥಿರ errcode_t shrink_cache(io_channel channel,
			      ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ new_ಗಾತ್ರ)
{
	ರಚನೆ unix_cache	*cache, *new_cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;
	errcode_t		retval;

	mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);

	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ಆದರೆ (retval)
		ಹೋಗು unlock;

	ಗಾಗಿ (i = new_ಗಾತ್ರ, cache = ದತ್ತಾಂಶ->cache + new_ಗಾತ್ರ;
	     i < ದತ್ತಾಂಶ->cache_ಗಾತ್ರ;
	     i++, cache++) {
		cache->ಖಂಡ = 0;
		cache->access_ಸಮಯ = 0;
		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		ಆದರೆ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(ದತ್ತಾಂಶ->cache, new_ಗಾತ್ರ * ಗಾತ್ರ(ರಚನೆ unix_cache));
	ಆದರೆ (!new_cache) {
		retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
		ಹೋಗು unlock;
	}

	ದತ್ತಾಂಶ->cache = new_cache;
	ದತ್ತಾಂಶ->cache_ಗಾತ್ರ = new_ಗಾತ್ರ;
	ದತ್ತಾಂಶ->cache_hash_ಸರಿಸು = highbit32(ದತ್ತಾಂಶ->cache_ಗಾತ್ರ);

unlock:
	mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ retval;
}

/* Grow the cache buffers */
ಸ್ಥಿರ errcode_t grow_cache(io_channel channel,
			    ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ new_ಗಾತ್ರ)
{
	ರಚನೆ unix_cache	*cache, *new_cache;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		i;
	errcode_t		retval;

	mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);

	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ಆದರೆ (retval)
		ಹೋಗು unlock;

	new_cache = realloc(ದತ್ತಾಂಶ->cache, new_ಗಾತ್ರ * ಗಾತ್ರ(ರಚನೆ unix_cache));
	ಆದರೆ (!new_cache) {
		retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
		ಹೋಗು unlock;
	}
	ದತ್ತಾಂಶ->cache = new_cache;

	ಗಾಗಿ (i = ದತ್ತಾಂಶ->cache_ಗಾತ್ರ, cache = new_cache + ದತ್ತಾಂಶ->cache_ಗಾತ್ರ;
	     i < new_ಗಾತ್ರ;
	     i++, cache++) {
		cache->ಖಂಡ = 0;
		cache->access_ಸಮಯ = 0;
		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ಆದರೆ (retval) {
			ಆಗ (i > ದತ್ತಾಂಶ->cache_ಗಾತ್ರ) {
				cache--; i--;
				ಆದರೆ (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			ಹೋಗು unlock;
		}
	}

	ದತ್ತಾಂಶ->cache_ಗಾತ್ರ = new_ಗಾತ್ರ;
	ದತ್ತಾಂಶ->cache_hash_ಸರಿಸು = highbit32(ದತ್ತಾಂಶ->cache_ಗಾತ್ರ);

unlock:
	mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ retval;
}
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BLKDISCARDZEROES
#ವ್ಯಾಖ್ಯಾನಿಸು BLKDISCARDZEROES _IO(0x12,124)
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ ext2fs_open_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಪೂರ್ಣಾಂಕ flags, mode_t ಬಹುಲಕ)
{
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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_FILE_H
ಸ್ಥಿರ errcode_t unix_funlock(io_channel channel)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->unix_flock_flags) {
		ret = flock(ದತ್ತಾಂಶ->dev, LOCK_UN);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ errno;

		ದತ್ತಾಂಶ->unix_flock_flags = 0;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t unix_flock(io_channel channel, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flock_flags)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	ಆದರೆ (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	ಆದರೆ (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	ಆದರೆ (!unix_flock_flags)
		ಹಿಂತಿರುಗಿ 0;

	ret = flock(ದತ್ತಾಂಶ->dev, unix_flock_flags);
	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ errno;

	ದತ್ತಾಂಶ->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ಹಿಂತಿರುಗಿ 0;
}
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು unix_flock		ಖಾಲಿ

ಸ್ಥಿರ errcode_t unix_funlock(io_channel channel)
{
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_SYS_FILE_H */

ಸ್ಥಿರ errcode_t unix_open_channel(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ fd,
				   ಪೂರ್ಣಾಂಕ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ಖಾಲಿ;
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = ಖಾಲಿ;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
	ರಚನೆ		utsname ut;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(ಶೂನ್ಯ) fsync(fd);
#ಆದರೆ_ಅಂತ್ಯ

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_io_channel), &io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(io, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ), &ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &io->name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(io->name, name);
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	io->ಖಂಡ_ಗಾತ್ರ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ));
	ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ದತ್ತಾಂಶ->io_stats.ಅಂಕೆ_fields = 4;
	ದತ್ತಾಂಶ->flags = flags;
	ದತ್ತಾಂಶ->dev = fd;

	ದತ್ತಾಂಶ->cache_ಗಾತ್ರ = DEFAULT_CACHE_ಗಾತ್ರ;
	ದತ್ತಾಂಶ->cache_hash_ಸರಿಸು = highbit32(ದತ್ತಾಂಶ->cache_ಗಾತ್ರ);
	ದತ್ತಾಂಶ->cache = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(DEFAULT_CACHE_ಗಾತ್ರ, ಗಾತ್ರ(ರಚನೆ unix_cache));
	ಆದರೆ (!ದತ್ತಾಂಶ->cache) {
		retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
		ಹೋಗು cleanup;
	}

#ಆದರೆ defined(O_ನೇರ)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO)
		io->align = ext2fs_get_dio_alignment(ದತ್ತಾಂಶ->dev);
#ಅಥವಾಆದರೆ defined(F_NOCACHE)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO)
		io->align = 4096;
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ಆದರೆ (ext2fs_fstat(ದತ್ತಾಂಶ->dev, &st) == 0) {
		ಆದರೆ (ext2fsP_is_disk_device(st.st_ಬಹುಲಕ)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKDISCARDZEROES
			ಪೂರ್ಣಾಂಕ zeroes = 0;

			ಆದರೆ (ioctl(ದತ್ತಾಂಶ->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#ಆದರೆ_ಅಂತ್ಯ
			io->flags |= CHANNEL_FLAGS_ಖಂಡ_DEVICE;
		} ಇಲ್ಲದಿದ್ದರೆ {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#ಆದರೆ defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	ಆದರೆ (!io->align)
		io->align = 512;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ಆದರೆ (io->flags & CHANNEL_FLAGS_ಖಂಡ_DEVICE) {
		ಪೂರ್ಣಾಂಕ dio_align = ext2fs_get_dio_alignment(fd);

		ಆದರೆ (io->align < dio_align)
			io->align = dio_align;
	}
#ಆದರೆ_ಅಂತ್ಯ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು RLIM_INFINITY
#ಆದರೆ (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#ವ್ಯಾಖ್ಯಾನಿಸು RLIM_INFINITY	((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(~0UL>>1))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು RLIM_INFINITY  (~0UL)
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	ಆದರೆ ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(ದತ್ತಾಂಶ->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_ಬಹುಲಕ))) {
		ರಚನೆ rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		ಆದರೆ (((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) rlim.rlim_cur) <
		    ((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	ಆದರೆ (flags & IO_FLAG_ಎಳೆಗಳು) {
		io->flags |= CHANNEL_FLAGS_ಎಳೆಗಳು;
		retval = pthread_mutex_init(&ದತ್ತಾಂಶ->cache_mutex, ಖಾಲಿ);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		retval = pthread_mutex_init(&ದತ್ತಾಂಶ->bounce_mutex, ಖಾಲಿ);
		ಆದರೆ (retval) {
			pthread_mutex_destroy(&ದತ್ತಾಂಶ->cache_mutex);
			ಹೋಗು cleanup;
		}
		retval = pthread_mutex_init(&ದತ್ತಾಂಶ->stats_mutex, ಖಾಲಿ);
		ಆದರೆ (retval) {
			pthread_mutex_destroy(&ದತ್ತಾಂಶ->cache_mutex);
			pthread_mutex_destroy(&ದತ್ತಾಂಶ->bounce_mutex);
			ಹೋಗು cleanup;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ
	*channel = io;
	ಹಿಂತಿರುಗಿ 0;

cleanup:
	ಆದರೆ (ದತ್ತಾಂಶ) {
		unix_funlock(io);
		ಆದರೆ (io->manager != unixfd_io_manager && ದತ್ತಾಂಶ->dev >= 0)
			close(ದತ್ತಾಂಶ->dev);
		ಆದರೆ (ದತ್ತಾಂಶ->cache) {
			free_cache(ದತ್ತಾಂಶ);
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ->cache);
		}
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

ಸ್ಥಿರ errcode_t unixfd_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str_fd, ಪೂರ್ಣಾಂಕ flags,
			     io_channel *channel)
{
	ಪೂರ್ಣಾಂಕ fd;
	ಪೂರ್ಣಾಂಕ fd_flags;

	fd = atoi(str_fd);
#ಆದರೆ defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	ಆದರೆ (fd_flags == -1)
		ಹಿಂತಿರುಗಿ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	ಆದರೆ (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#ಆದರೆ defined(O_ನೇರ)
	ಆದರೆ (fd_flags & O_ನೇರ)
		flags |= IO_FLAG_ನೇರ_IO;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ  /* HAVE_FCNTL */

	ಹಿಂತಿರುಗಿ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ಸ್ಥಿರ errcode_t unix_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags,
			   io_channel *channel)
{
	ಪೂರ್ಣಾಂಕ fd = -1;
	ಪೂರ್ಣಾಂಕ open_flags;

	ಆದರೆ (name == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ಆದರೆ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ಆದರೆ defined(O_ನೇರ)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO)
		open_flags |= O_ನೇರ;
#ಆದರೆ_ಅಂತ್ಯ
	fd = ext2fs_open_file(name, open_flags, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ errno;
#ಆದರೆ defined(F_NOCACHE) && !defined(IO_ನೇರ)
	ಆದರೆ (flags & IO_FLAG_ನೇರ_IO) {
		ಆದರೆ (fcntl(fd, F_NOCACHE, 1) < 0)
			ಹಿಂತಿರುಗಿ errno;
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ಸ್ಥಿರ errcode_t unix_close(io_channel channel)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
		ಹಿಂತಿರುಗಿ 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0);
#ಆದರೆ_ಅಂತ್ಯ

	unix_funlock(channel);

	ಆದರೆ (channel->manager != unixfd_io_manager && close(ದತ್ತಾಂಶ->dev) < 0)
		retval = errno;
	free_cache(ದತ್ತಾಂಶ);
	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ->cache);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_ಎಳೆಗಳು) {
		pthread_mutex_destroy(&ದತ್ತಾಂಶ->cache_mutex);
		pthread_mutex_destroy(&ದತ್ತಾಂಶ->bounce_mutex);
		pthread_mutex_destroy(&ದತ್ತಾಂಶ->stats_mutex);
	}
#ಆದರೆ_ಅಂತ್ಯ

	ext2fs_free_mem(&channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಆದರೆ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t unix_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (channel->ಖಂಡ_ಗಾತ್ರ != blksize) {
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, BOUNCE_MTX);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
		ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, FLUSH_NOLOCK))){
			mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
			mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
			ಹಿಂತಿರುಗಿ retval;
		}
#ಆದರೆ_ಅಂತ್ಯ

		channel->ಖಂಡ_ಗಾತ್ರ = blksize;
		free_cache(ದತ್ತಾಂಶ);
		retval = alloc_cache(channel, ದತ್ತಾಂಶ);
		mutex_unlock(ದತ್ತಾಂಶ, BOUNCE_MTX);
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t unix_read_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ರಚನೆ unix_cache *cache;
	errcode_t	retval;
	ಅಕ್ಷರ		*cp;
	ಪೂರ್ಣಾಂಕ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_IO_CACHE
	ಹಿಂತಿರುಗಿ raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_NOCACHE)
		ಹಿಂತಿರುಗಿ raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf);
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
	mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
	ಆಗ (ಎಣಿಕೆ_2 > 0) {
		/* If it's in the cache, use it! */
		ಆದರೆ ((cache = ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ, ಖಾಲಿ))) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
			ಬರೆ("Using cached block %lu\n", ಖಂಡ);
#ಆದರೆ_ಅಂತ್ಯ
			ಸ್ಮರಣೆ_ನಕಲು(cp, cache->buf, channel->ಖಂಡ_ಗಾತ್ರ);
			ಎಣಿಕೆ_2--;
			ಖಂಡ++;
			cp += channel->ಖಂಡ_ಗಾತ್ರ;
			ಮುಂದುವರಿ;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ಗಾಗಿ (i=1; i < ಎಣಿಕೆ_2; i++)
			ಆದರೆ (ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ+i, ಖಾಲಿ))
				ನಿಲ್ಲಿಸು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("Reading %d blocks starting at %lu\n", i, ಖಂಡ);
#ಆದರೆ_ಅಂತ್ಯ
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
		ಆದರೆ ((retval = raw_read_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, i, cp)))
			ಹಿಂತಿರುಗಿ retval;
		mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);

		/* Save the results in the cache */
		ಗಾಗಿ (j=0; j < i; j++) {
			ಆದರೆ (!ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ, &cache)) {
				retval = reuse_cache(channel, ದತ್ತಾಂಶ,
						     cache, ಖಂಡ);
				ಆದರೆ (retval)
					ಹೋಗು call_write_handler;
				ಸ್ಮರಣೆ_ನಕಲು(cache->buf, cp, channel->ಖಂಡ_ಗಾತ್ರ);
			}
			ಎಣಿಕೆ_2--;
			ಖಂಡ++;
			cp += channel->ಖಂಡ_ಗಾತ್ರ;
		}
	}
	mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ 0;

call_write_handler:
	ಆದರೆ (cache->write_err && channel->write_error) {
		ಅಕ್ಷರ *err_buf = ಖಾಲಿ;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ err_ಖಂಡ = cache->ಖಂಡ;

		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		cache->write_err = 0;
		ಆದರೆ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ಖಾಲಿ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಸ್ಮರಣೆ_ನಕಲು(err_buf, cache->buf, channel->ಖಂಡ_ಗಾತ್ರ);
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
		(channel->write_error)(channel, err_ಖಂಡ, 1, err_buf,
				       channel->ಖಂಡ_ಗಾತ್ರ, -1,
				       retval);
		ಆದರೆ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t unix_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ unix_read_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t unix_write_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ರಚನೆ unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*cp;
	ಪೂರ್ಣಾಂಕ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_IO_CACHE
	ಹಿಂತಿರುಗಿ raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf, 0);
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ (ದತ್ತಾಂಶ->flags & IO_FLAG_NOCACHE)
		ಹಿಂತಿರುಗಿ raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ಆದರೆ (ಎಣಿಕೆ_2 < 0 || ಎಣಿಕೆ_2 > WRITE_ನೇರ_ಗಾತ್ರ) {
		ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ,
						  FLUSH_INVALIDATE)))
			ಹಿಂತಿರುಗಿ retval;
		ಹಿಂತಿರುಗಿ raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ಆದರೆ (writethrough)
		retval = raw_write_blk(channel, ದತ್ತಾಂಶ, ಖಂಡ, ಎಣಿಕೆ_2, buf, 0);

	cp = buf;
	mutex_ಬೀಗ(ದತ್ತಾಂಶ, CACHE_MTX);
	ಆಗ (ಎಣಿಕೆ_2 > 0) {
		cache = ಕಂಡುಹಿಡಿ_cached_ಖಂಡ(ದತ್ತಾಂಶ, ಖಂಡ, &reuse);
		ಆದರೆ (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, ದತ್ತಾಂಶ, cache, ಖಂಡ);
			ಆದರೆ (err)
				ಹೋಗು call_write_handler;
		}
		ಆದರೆ (cache->buf != cp)
			ಸ್ಮರಣೆ_ನಕಲು(cache->buf, cp, channel->ಖಂಡ_ಗಾತ್ರ);
		cache->dirty = !writethrough;
		ಎಣಿಕೆ_2--;
		ಖಂಡ++;
		cp += channel->ಖಂಡ_ಗಾತ್ರ;
	}
	mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ retval;

call_write_handler:
	ಆದರೆ (cache->write_err && channel->write_error) {
		ಅಕ್ಷರ *err_buf = ಖಾಲಿ;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ err_ಖಂಡ = cache->ಖಂಡ;

		cache->dirty = 0;
		cache->ಒಳ_use = 0;
		cache->write_err = 0;
		ಆದರೆ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ಖಾಲಿ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಸ್ಮರಣೆ_ನಕಲು(err_buf, cache->buf, channel->ಖಂಡ_ಗಾತ್ರ);
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
		(channel->write_error)(channel, err_ಖಂಡ, 1, err_buf,
				       channel->ಖಂಡ_ಗಾತ್ರ, -1,
				       retval);
		ಆದರೆ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		mutex_unlock(ದತ್ತಾಂಶ, CACHE_MTX);
	ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ /* NO_IO_CACHE */
}

ಸ್ಥಿರ errcode_t unix_cache_readahead(io_channel channel,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ POSIX_FADV_WILLNEED
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *)channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ಹಿಂತಿರುಗಿ posix_fadvise(ದತ್ತಾಂಶ->dev,
			     (ext2_loff_t)ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ,
			     (ext2_loff_t)ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ,
			     POSIX_FADV_WILLNEED);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ errcode_t unix_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ unix_write_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t unix_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				 ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (channel->align != 0) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ALIGN_DEBUG
		ಬರೆ("unix_write_byte: O_DIRECT fallback\n");
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	ಆದರೆ ((retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, FLUSH_INVALIDATE)))
		ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (lseek(ದತ್ತಾಂಶ->dev, ಸ್ಥಾನಾಂತರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ, SEEK_SET) < 0)
		ಹಿಂತಿರುಗಿ errno;

	actual = ಲಿಖ(ದತ್ತಾಂಶ->dev, buf, ಗಾತ್ರ_2);
	ಆದರೆ (actual < 0)
		ಹಿಂತಿರುಗಿ errno;
	ಆದರೆ (actual != ಗಾತ್ರ_2)
		ಹಿಂತಿರುಗಿ EXT2_ET_SHORT_WRITE;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t unix_flush(io_channel channel)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FSYNC
	ಆದರೆ (!retval && fsync(ದತ್ತಾಂಶ->dev) != 0)
		ಹಿಂತಿರುಗಿ errno;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t unix_ಗಣ_option(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ tmp;
	errcode_t retval;
	ಅಕ್ಷರ *ಕೊನೆ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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
	ಆದರೆ (!strcmp(option, "cache")) {
		ಆದರೆ (!arg)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ಆದರೆ (!strcmp(arg, "on")) {
			ದತ್ತಾಂಶ->flags &= ~IO_FLAG_NOCACHE;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (!strcmp(arg, "off")) {
			retval = flush_cached_ಖಂಡಗಳು(channel, ದತ್ತಾಂಶ, 0);
			ದತ್ತಾಂಶ->flags |= IO_FLAG_NOCACHE;
			ಹಿಂತಿರುಗಿ retval;
		}
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NO_IO_CACHE
	ಆದರೆ (!strcmp(option, "cache_blocks")) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಗಾತ್ರ_2;

		ಆದರೆ (!arg)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		errno = 0;
		ಗಾತ್ರ_2 = strtoll(arg, ಖಾಲಿ, 0);
		ಆದರೆ (errno || ಗಾತ್ರ_2 == 0 || ಗಾತ್ರ_2 > INT32_MAX)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		ಆದರೆ (ದತ್ತಾಂಶ->cache_ಗಾತ್ರ == ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (ದತ್ತಾಂಶ->cache_ಗಾತ್ರ > ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ shrink_cache(channel, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
		ಹಿಂತಿರುಗಿ grow_cache(channel, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
}

#ಆದರೆ defined(__linux__) && !defined(BLKDISCARD)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKDISCARD		_IO(0x12,119)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ errcode_t unix_discard(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (channel->flags & CHANNEL_FLAGS_NODISCARD)
		ಹೋಗು unimplemented;

	ಆದರೆ (channel->flags & CHANNEL_FLAGS_ಖಂಡ_DEVICE) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(ಖಂಡ) * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;
		range[1] = (__u64)(ಎಣಿಕೆ_2) * channel->ಖಂಡ_ಗಾತ್ರ;

		ret = ioctl(ದತ್ತಾಂಶ->dev, BLKDISCARD, &range);
#ಇಲ್ಲದಿದ್ದರೆ
		ಹೋಗು unimplemented;
#ಆದರೆ_ಅಂತ್ಯ
	} ಇಲ್ಲದಿದ್ದರೆ {
#ಆದರೆ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(ದತ್ತಾಂಶ->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(ಖಂಡ) * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ,
				(off_t)(ಎಣಿಕೆ_2) * channel->ಖಂಡ_ಗಾತ್ರ);
#ಇಲ್ಲದಿದ್ದರೆ
		ಹೋಗು unimplemented;
#ಆದರೆ_ಅಂತ್ಯ
	}
	ಆದರೆ (ret < 0) {
		ಆದರೆ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			ಹೋಗು unimplemented;
		}
		ಹಿಂತಿರುಗಿ errno;
	}
	ಹಿಂತಿರುಗಿ 0;
unimplemented:
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __unix_zeroout(ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ, off_t len)
{
	ಪೂರ್ಣಾಂಕ ret = -1;

#ಆದರೆ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, ಸ್ಥಾನಾಂತರ, len);
	ಆದರೆ (ret == 0)
		ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			ಸ್ಥಾನಾಂತರ,  len);
	ಆದರೆ (ret == 0)
		ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
	errno = EOPNOTSUPP;
	ಹಿಂತಿರುಗಿ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-parameter"
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ errcode_t unix_zeroout(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ unix_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (!(channel->flags & CHANNEL_FLAGS_ಖಂಡ_DEVICE)) {
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
	}

	ಆದರೆ (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		ಹೋಗು unimplemented;

	ret = __unix_zeroout(ದತ್ತಾಂಶ->dev,
			(off_t)(ಖಂಡ) * channel->ಖಂಡ_ಗಾತ್ರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ,
			(off_t)(ಎಣಿಕೆ_2) * channel->ಖಂಡ_ಗಾತ್ರ);
err:
	ಆದರೆ (ret < 0) {
		ಆದರೆ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			ಹೋಗು unimplemented;
		}
		ಹಿಂತಿರುಗಿ errno;
	}
	ಹಿಂತಿರುಗಿ 0;
unimplemented:
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}
#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.ಗಣ_blksize	= unix_ಗಣ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.ಗಣ_option	= unix_ಗಣ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unix_io_manager = &struct_unix_manager;

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.open		= unixfd_open,
	.close		= unix_close,
	.ಗಣ_blksize	= unix_ಗಣ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.ಗಣ_option	= unix_ಗಣ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
