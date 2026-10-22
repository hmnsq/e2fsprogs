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

#ਜੇ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ 600
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DARWIN_C_ਸਰੋਤ
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/utsname.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_IOCTL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_MOUNT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_RESOURCE_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_LINUX_FALLOC_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/falloc.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_FILE_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/file.h>
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALIGN_DEBUG

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਲੱਭੋ_ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(ਪੂਰਨ x)
{
	ਪੂਰਨ r = 32;

	ਜੇ (!x)
		ਵਾਪਸ 0;
	ਜੇ (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	ਜੇ (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	ਜੇ (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	ਜੇ (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	ਜੇ (!(x & 0x80000000u)) {
		r -= 1;
	}
	ਵਾਪਸ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ highbit32(uint32_t v)
{
	ਵਾਪਸ ਲੱਭੋ_ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

ਬਣਤਰ unix_cache {
	ਅੱਖਰ			*buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਖੰਡ;
	ਪੂਰਨ			access_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ		dirty:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ		ਵਿੱਚ_use:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ		write_err:1;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_CACHE_ਮਾਪ 8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_ਸਿੱਧਾ_ਮਾਪ 4	/* Must be smaller than CACHE_SIZE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_ਸਿੱਧਾ_ਮਾਪ 4	/* Should be smaller than CACHE_SIZE */

ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ	magic;
	ਪੂਰਨ	dev;
	ਪੂਰਨ	flags;
	ਪੂਰਨ	align;
	ਪੂਰਨ	access_ਸਮਾਂ;
	ਪੂਰਨ	unix_flock_flags;
	ext2_loff_t ਵਿੱਥ;
	ਬਣਤਰ unix_cache *cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cache_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cache_hash_ਖਿਸਕਾਓ;
	ਖਾਲੀ	*bounce;
	ਬਣਤਰ struct_io_stats io_stats;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#ਜੇ_ਅੰਤ
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ ਤਾਲਾ_ਕਿਸਮ {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} ਕਿਸਮ_t;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
ਸਥਿਰ ਸਤਰਅੰਦਰ pthread_mutex_t *get_mutex(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
					 ਕਿਸਮ_t ਕਿਸਮ)
{
	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_ਧਾਗੇ) {
		ਚੋਣ (ਕਿਸਮ) {
		ਹਾਲਤ CACHE_MTX:
			ਵਾਪਸ &ਅੰਕੜੇ->cache_mutex;
		ਹਾਲਤ BOUNCE_MTX:
			ਵਾਪਸ &ਅੰਕੜੇ->bounce_mutex;
		ਹਾਲਤ STATS_MTX:
			ਵਾਪਸ &ਅੰਕੜੇ->stats_mutex;
		}
	}
	ਵਾਪਸ ਨਿਰਾ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mutex_ਤਾਲਾ(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, ਕਿਸਮ_t ਕਿਸਮ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ਅੰਕੜੇ,ਕਿਸਮ);

	ਜੇ (mtx)
		pthread_mutex_ਤਾਲਾ(mtx);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mutex_unlock(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, ਕਿਸਮ_t ਕਿਸਮ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ਅੰਕੜੇ,ਕਿਸਮ);

	ਜੇ (mtx)
		pthread_mutex_unlock(mtx);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (stats) {
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, STATS_MTX);
		*stats = &ਅੰਕੜੇ->io_stats;
		mutex_unlock(ਅੰਕੜੇ, STATS_MTX);
	}

	ਵਾਪਸ retval;
}

/*
 * Here are the raw I/O functions
 */
ਸਥਿਰ errcode_t raw_read_blk(io_channel channel,
			      ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *bufv)
{
	errcode_t	retval;
	ssize_t		ਮਾਪ;
	ext2_loff_t	location;
	ਪੂਰਨ		actual = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*buf = bufv;
	ssize_t		really_read = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ aligned_blk;
	ਪੂਰਨ		align_ਮਾਪ, ਵਿੱਥ;

	ਮਾਪ = (ਗਿਣਤੀ_2 < 0) ? -ਗਿਣਤੀ_2 : (ext2_loff_t) ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ;
	mutex_ਤਾਲਾ(ਅੰਕੜੇ, STATS_MTX);
	ਅੰਕੜੇ->io_stats.ਬਾਈਟਾਂ_read += ਮਾਪ;
	mutex_unlock(ਅੰਕੜੇ, STATS_MTX);
	location = ((ext2_loff_t) ਖੰਡ * channel->ਖੰਡ_ਮਾਪ) + ਅੰਕੜੇ->ਵਿੱਥ;

	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_FORCE_BOUNCE)
		ਜਾਓ bounce_read;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PREAD64
	/* Try an aligned pread */
	ਜੇ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ਮਾਪ, channel->align))) {
		actual = pread64(ਅੰਕੜੇ->dev, buf, ਮਾਪ, location);
		ਜੇ (actual == ਮਾਪ)
			ਵਾਪਸ 0;
		actual = 0;
	}
#ਜਾਂਜੇ HAVE_PREAD
	/* Try an aligned pread */
	ਜੇ ((ਆਕਾਰ(off_t) >= ਆਕਾਰ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ਮਾਪ, channel->align)))) {
		actual = pread(ਅੰਕੜੇ->dev, buf, ਮਾਪ, location);
		ਜੇ (actual == ਮਾਪ)
			ਵਾਪਸ 0;
		actual = 0;
	}
#ਜੇ_ਅੰਤ /* HAVE_PREAD */

	ਜੇ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ਮਾਪ, channel->align))) {
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, BOUNCE_MTX);
		ਜੇ (ext2fs_llseek(ਅੰਕੜੇ->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ਜਾਓ error_unlock;
		}
		actual = ਪਠ(ਅੰਕੜੇ->dev, buf, ਮਾਪ);
		ਜੇ (actual != ਮਾਪ) {
		short_read:
			ਜੇ (actual < 0) {
				retval = errno;
				actual = 0;
			} ਨਹੀਂਤਾਂ
				retval = EXT2_ET_SHORT_READ;
			ਜਾਓ error_unlock;
		}
		ਜਾਓ success_unlock;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ALIGN_DEBUG
	ਲਿਖੋ("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਮਾਪ);
#ਜੇ_ਅੰਤ

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ਜੇ (channel->align == 0)
		channel->align = 1;
	ਜੇ ((channel->ਖੰਡ_ਮਾਪ > channel->align) &&
	    (channel->ਖੰਡ_ਮਾਪ % channel->align) == 0)
		align_ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ
		align_ਮਾਪ = channel->align;
	aligned_blk = location / align_ਮਾਪ;
	ਵਿੱਥ = location % align_ਮਾਪ;

	mutex_ਤਾਲਾ(ਅੰਕੜੇ, BOUNCE_MTX);
	ਜੇ (ext2fs_llseek(ਅੰਕੜੇ->dev, aligned_blk * align_ਮਾਪ, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		ਜਾਓ error_unlock;
	}
	ਜਦੋਂ (ਮਾਪ > 0) {
		actual = ਪਠ(ਅੰਕੜੇ->dev, ਅੰਕੜੇ->bounce, align_ਮਾਪ);
		ਜੇ (actual != align_ਮਾਪ) {
			actual = really_read;
			buf -= really_read;
			ਮਾਪ += really_read;
			ਜਾਓ short_read;
		}
		ਜੇ ((actual + ਵਿੱਥ) > align_ਮਾਪ)
			actual = align_ਮਾਪ - ਵਿੱਥ;
		ਜੇ (actual > ਮਾਪ)
			actual = ਮਾਪ;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, (ਅੱਖਰ *)ਅੰਕੜੇ->bounce + ਵਿੱਥ, actual);

		really_read += actual;
		ਮਾਪ -= actual;
		buf += actual;
		ਵਿੱਥ = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
	ਵਾਪਸ 0;

error_unlock:
	mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
	ਜੇ (actual >= 0 && actual < ਮਾਪ)
		memset((ਅੱਖਰ *) buf+actual, 0, ਮਾਪ-actual);
	ਜੇ (channel->read_error)
		retval = (channel->read_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
					       ਮਾਪ, actual, retval);
	ਵਾਪਸ retval;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RAW_WRITE_NO_HANDLER	(1U << 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RAW_WRITE_NOLOCK	(1U << 1)

ਸਥਿਰ errcode_t raw_write_blk(io_channel channel,
			       ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *bufv,
			       ਪੂਰਨ flags)
{
	ssize_t		ਮਾਪ;
	ext2_loff_t	location;
	ਪੂਰਨ		actual = 0;
	errcode_t	retval;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf = bufv;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ aligned_blk;
	ਪੂਰਨ		align_ਮਾਪ, ਵਿੱਥ;

	ਜੇ (ਗਿਣਤੀ_2 == 1)
		ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ {
		ਜੇ (ਗਿਣਤੀ_2 < 0)
			ਮਾਪ = -ਗਿਣਤੀ_2;
		ਨਹੀਂਤਾਂ
			ਮਾਪ = (ext2_loff_t) ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ;
	}
	mutex_ਤਾਲਾ(ਅੰਕੜੇ, STATS_MTX);
	ਅੰਕੜੇ->io_stats.ਬਾਈਟਾਂ_written += ਮਾਪ;
	mutex_unlock(ਅੰਕੜੇ, STATS_MTX);

	location = ((ext2_loff_t) ਖੰਡ * channel->ਖੰਡ_ਮਾਪ) + ਅੰਕੜੇ->ਵਿੱਥ;

	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_FORCE_BOUNCE)
		ਜਾਓ bounce_write;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PWRITE64
	/* Try an aligned pwrite */
	ਜੇ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ਮਾਪ, channel->align))) {
		actual = pwrite64(ਅੰਕੜੇ->dev, buf, ਮਾਪ, location);
		ਜੇ (actual == ਮਾਪ)
			ਵਾਪਸ 0;
	}
#ਜਾਂਜੇ HAVE_PWRITE
	/* Try an aligned pwrite */
	ਜੇ ((ਆਕਾਰ(off_t) >= ਆਕਾਰ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ਮਾਪ, channel->align)))) {
		actual = pwrite(ਅੰਕੜੇ->dev, buf, ਮਾਪ, location);
		ਜੇ (actual == ਮਾਪ)
			ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ /* HAVE_PWRITE */

	ਜੇ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ਮਾਪ, channel->align))) {
		ਜੇ (!(flags & RAW_WRITE_NOLOCK))
			mutex_ਤਾਲਾ(ਅੰਕੜੇ, BOUNCE_MTX);
		ਜੇ (ext2fs_llseek(ਅੰਕੜੇ->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ਜਾਓ error_unlock;
		}
		actual = ਲਿਖ(ਅੰਕੜੇ->dev, buf, ਮਾਪ);
		ਜੇ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
		ਜੇ (actual < 0) {
			retval = errno;
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜੇ (actual != ਮਾਪ) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਵਾਪਸ 0;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ALIGN_DEBUG
	ਲਿਖੋ("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਮਾਪ);
#ਜੇ_ਅੰਤ
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ਜੇ (channel->align == 0)
		channel->align = 1;
	ਜੇ ((channel->ਖੰਡ_ਮਾਪ > channel->align) &&
	    (channel->ਖੰਡ_ਮਾਪ % channel->align) == 0)
		align_ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ
		align_ਮਾਪ = channel->align;
	aligned_blk = location / align_ਮਾਪ;
	ਵਿੱਥ = location % align_ਮਾਪ;

	ਜਦੋਂ (ਮਾਪ > 0) {
		ਪੂਰਨ actual_w;

		ਜੇ (!(flags & RAW_WRITE_NOLOCK))
			mutex_ਤਾਲਾ(ਅੰਕੜੇ, BOUNCE_MTX);
		ਜੇ (ਮਾਪ < align_ਮਾਪ || ਵਿੱਥ) {
			ਜੇ (ext2fs_llseek(ਅੰਕੜੇ->dev, aligned_blk * align_ਮਾਪ,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				ਜਾਓ error_unlock;
			}
			actual = ਪਠ(ਅੰਕੜੇ->dev, ਅੰਕੜੇ->bounce,
				      align_ਮਾਪ);
			ਜੇ (actual != align_ਮਾਪ) {
				ਜੇ (actual < 0) {
					retval = errno;
					ਜਾਓ error_unlock;
				}
				memset((ਅੱਖਰ *) ਅੰਕੜੇ->bounce + actual, 0,
				       align_ਮਾਪ - actual);
			}
		}
		actual = ਮਾਪ;
		ਜੇ ((actual + ਵਿੱਥ) > align_ਮਾਪ)
			actual = align_ਮਾਪ - ਵਿੱਥ;
		ਜੇ (actual > ਮਾਪ)
			actual = ਮਾਪ;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(((ਅੱਖਰ *)ਅੰਕੜੇ->bounce) + ਵਿੱਥ, buf, actual);
		ਜੇ (ext2fs_llseek(ਅੰਕੜੇ->dev, aligned_blk * align_ਮਾਪ, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ਜਾਓ error_unlock;
		}
		actual_w = ਲਿਖ(ਅੰਕੜੇ->dev, ਅੰਕੜੇ->bounce, align_ਮਾਪ);
		ਜੇ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
		ਜੇ (actual_w < 0) {
			retval = errno;
			ਜਾਓ error_ਬਾਹਰਲਾ;
		}
		ਜੇ (actual_w != align_ਮਾਪ)
			ਜਾਓ short_write;
		ਮਾਪ -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		ਵਿੱਥ = 0;
	}
	ਵਾਪਸ 0;

error_unlock:
	ਜੇ (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
error_ਬਾਹਰਲਾ:
	ਜੇ (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
						ਮਾਪ, actual, retval);
	ਵਾਪਸ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ਸਥਿਰ errcode_t alloc_cache(io_channel channel,
			     ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	errcode_t		retval;
	ਬਣਤਰ unix_cache	*cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;

	ਅੰਕੜੇ->access_ਸਮਾਂ = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < ਅੰਕੜੇ->cache_ਮਾਪ; i++, cache++) {
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
ਸਥਿਰ ਖਾਲੀ free_cache(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ unix_cache	*cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;

	ਅੰਕੜੇ->access_ਸਮਾਂ = 0;
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < ਅੰਕੜੇ->cache_ਮਾਪ; i++, cache++) {
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

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GOLDEN_ਅਨੁਪਾਤ_ਅਭਾਜ	0x9e37fffffffc0001UL
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CACHE_ਸਤਰ_ਮਾਪ
/* if the system didn't tell us, guess something reasonable */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CACHE_ਸਤਰ_ਮਾਪ		64
#ਜੇ_ਅੰਤ

/* buffer cache hashing function, crudely stolen from xfsprogs */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
cache_hash(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	ਜੇ (ਅੰਕੜੇ->cache_ਮਾਪ <= DEFAULT_CACHE_ਮਾਪ)
		ਵਾਪਸ 0;

	tmp = hashval ^ (GOLDEN_ਅਨੁਪਾਤ_ਅਭਾਜ + hashval) / CACHE_ਸਤਰ_ਮਾਪ;
	tmp = tmp ^ ((tmp ^ GOLDEN_ਅਨੁਪਾਤ_ਅਭਾਜ) >> ਅੰਕੜੇ->cache_hash_ਖਿਸਕਾਓ);
	ਵਾਪਸ tmp % ਅੰਕੜੇ->cache_ਮਾਪ;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ਸਥਿਰ ਬਣਤਰ unix_cache *ਲੱਭੋ_cached_ਖੰਡ(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					    ਬਣਤਰ unix_cache **eldest)
{
	ਬਣਤਰ unix_cache	*cache, *ਅਣਵਰਤਿਆ_cache, *oldest_cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		hash = cache_hash(ਅੰਕੜੇ, ਖੰਡ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;

	ਅਣਵਰਤਿਆ_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	ਲਈ (i = hash, cache = ਅੰਕੜੇ->cache + hash;
	     i < ਅੰਕੜੇ->cache_ਮਾਪ;
	     i++, cache++) {
		ਜੇ (!cache->ਵਿੱਚ_use) {
			ਜੇ (!ਅਣਵਰਤਿਆ_cache)
				ਅਣਵਰਤਿਆ_cache = cache;
			ਜਾਰੀ;
		}
		ਜੇ (cache->ਖੰਡ == ਖੰਡ) {
			cache->access_ਸਮਾਂ = ++ਅੰਕੜੇ->access_ਸਮਾਂ;
			ਅੰਕੜੇ->io_stats.cache_ਪ੍ਰਾਪਤੀਆਂ++;
			ਵਾਪਸ cache;
		}
		ਜੇ (!oldest_cache ||
		    (cache->access_ਸਮਾਂ < oldest_cache->access_ਸਮਾਂ))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	ਲਈ (i = 0, cache = ਅੰਕੜੇ->cache; i < hash; i++, cache++) {
		ਜੇ (!cache->ਵਿੱਚ_use) {
			ਜੇ (!ਅਣਵਰਤਿਆ_cache)
				ਅਣਵਰਤਿਆ_cache = cache;
			ਜਾਰੀ;
		}
		ਜੇ (cache->ਖੰਡ == ਖੰਡ) {
			cache->access_ਸਮਾਂ = ++ਅੰਕੜੇ->access_ਸਮਾਂ;
			ਅੰਕੜੇ->io_stats.cache_ਪ੍ਰਾਪਤੀਆਂ++;
			ਵਾਪਸ cache;
		}
		ਜੇ (!oldest_cache ||
		    (cache->access_ਸਮਾਂ < oldest_cache->access_ਸਮਾਂ))
			oldest_cache = cache;
	}
	ਜੇ (eldest)
		*eldest = (ਅਣਵਰਤਿਆ_cache) ? ਅਣਵਰਤਿਆ_cache : oldest_cache;
	ਅੰਕੜੇ->io_stats.cache_ਖੁੰਝਾਂ++;
	ਵਾਪਸ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ਸਥਿਰ errcode_t reuse_cache(io_channel channel,
		ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, ਬਣਤਰ unix_cache *cache,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ)
{
	ਜੇ (cache->dirty && cache->ਵਿੱਚ_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, ਅੰਕੜੇ, cache->ਖੰਡ, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		ਜੇ (retval) {
			cache->write_err = 1;
			ਵਾਪਸ retval;
		}
	}

	cache->ਵਿੱਚ_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->ਖੰਡ = ਖੰਡ;
	cache->access_ਸਮਾਂ = ++ਅੰਕੜੇ->access_ਸਮਾਂ;
	ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLUSH_INVALIDATE	0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ਸਥਿਰ errcode_t flush_cached_ਖੰਡਾਂ(io_channel channel,
				     ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
				     ਪੂਰਨ flags)
{
	ਬਣਤਰ unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;
	ਪੂਰਨ			errors_ਮਿਲਿਆ = 0;

	ਜੇ ((flags & FLUSH_NOLOCK) == 0)
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
	ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < ਅੰਕੜੇ->cache_ਮਾਪ; i++, cache++) {
		ਜੇ (!cache->ਵਿੱਚ_use)
			ਜਾਰੀ;
		ਜੇ (cache->dirty) {
			ਪੂਰਨ raw_flags = RAW_WRITE_NO_HANDLER;

			ਜੇ (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, ਅੰਕੜੇ,
					       cache->ਖੰਡ, 1, cache->buf,
					       raw_flags);
			ਜੇ (retval) {
				cache->write_err = 1;
				errors_ਮਿਲਿਆ = 1;
				retval2 = retval;
			} ਨਹੀਂਤਾਂ {
				cache->dirty = 0;
				cache->write_err = 0;
				ਜੇ (flags & FLUSH_INVALIDATE)
					cache->ਵਿੱਚ_use = 0;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (flags & FLUSH_INVALIDATE) {
			cache->ਵਿੱਚ_use = 0;
		}
	}
	ਜੇ ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
retry:
	ਜਦੋਂ (errors_ਮਿਲਿਆ) {
		ਜੇ ((flags & FLUSH_NOLOCK) == 0)
			mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
		errors_ਮਿਲਿਆ = 0;
		ਲਈ (i=0, cache = ਅੰਕੜੇ->cache; i < ਅੰਕੜੇ->cache_ਮਾਪ; i++, cache++) {
			ਜੇ (!cache->ਵਿੱਚ_use || !cache->write_err)
				ਜਾਰੀ;
			errors_ਮਿਲਿਆ = 1;
			ਜੇ (cache->write_err && channel->write_error) {
				ਅੱਖਰ *err_buf = ਨਿਰਾ;
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ err_ਖੰਡ = cache->ਖੰਡ;

				cache->dirty = 0;
				cache->ਵਿੱਚ_use = 0;
				cache->write_err = 0;
				ਜੇ (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ਨਿਰਾ;
				ਨਹੀਂਤਾਂ
					ਸਮ੍ਰਿਤੀ_ਨਕਲ(err_buf, cache->buf,
					       channel->ਖੰਡ_ਮਾਪ);
				mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
				(channel->write_error)(channel, err_ਖੰਡ,
					1, err_buf, channel->ਖੰਡ_ਮਾਪ, -1,
					retval2);
				ਜੇ (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
				ਜਾਓ retry;
			} ਨਹੀਂਤਾਂ
				cache->write_err = 0;
		}
		ਜੇ ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	}
	ਵਾਪਸ retval2;
}

/* Shrink the cache buffers */
ਸਥਿਰ errcode_t shrink_cache(io_channel channel,
			      ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਮਾਪ)
{
	ਬਣਤਰ unix_cache	*cache, *new_cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;
	errcode_t		retval;

	mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);

	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ਜੇ (retval)
		ਜਾਓ unlock;

	ਲਈ (i = new_ਮਾਪ, cache = ਅੰਕੜੇ->cache + new_ਮਾਪ;
	     i < ਅੰਕੜੇ->cache_ਮਾਪ;
	     i++, cache++) {
		cache->ਖੰਡ = 0;
		cache->access_ਸਮਾਂ = 0;
		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		ਜੇ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(ਅੰਕੜੇ->cache, new_ਮਾਪ * ਆਕਾਰ(ਬਣਤਰ unix_cache));
	ਜੇ (!new_cache) {
		retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		ਜਾਓ unlock;
	}

	ਅੰਕੜੇ->cache = new_cache;
	ਅੰਕੜੇ->cache_ਮਾਪ = new_ਮਾਪ;
	ਅੰਕੜੇ->cache_hash_ਖਿਸਕਾਓ = highbit32(ਅੰਕੜੇ->cache_ਮਾਪ);

unlock:
	mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ retval;
}

/* Grow the cache buffers */
ਸਥਿਰ errcode_t grow_cache(io_channel channel,
			    ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਮਾਪ)
{
	ਬਣਤਰ unix_cache	*cache, *new_cache;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		i;
	errcode_t		retval;

	mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);

	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ਜੇ (retval)
		ਜਾਓ unlock;

	new_cache = realloc(ਅੰਕੜੇ->cache, new_ਮਾਪ * ਆਕਾਰ(ਬਣਤਰ unix_cache));
	ਜੇ (!new_cache) {
		retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		ਜਾਓ unlock;
	}
	ਅੰਕੜੇ->cache = new_cache;

	ਲਈ (i = ਅੰਕੜੇ->cache_ਮਾਪ, cache = new_cache + ਅੰਕੜੇ->cache_ਮਾਪ;
	     i < new_ਮਾਪ;
	     i++, cache++) {
		cache->ਖੰਡ = 0;
		cache->access_ਸਮਾਂ = 0;
		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ਜੇ (retval) {
			ਜਦੋਂ (i > ਅੰਕੜੇ->cache_ਮਾਪ) {
				cache--; i--;
				ਜੇ (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			ਜਾਓ unlock;
		}
	}

	ਅੰਕੜੇ->cache_ਮਾਪ = new_ਮਾਪ;
	ਅੰਕੜੇ->cache_hash_ਖਿਸਕਾਓ = highbit32(ਅੰਕੜੇ->cache_ਮਾਪ);

unlock:
	mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ retval;
}
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLKDISCARDZEROES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKDISCARDZEROES _IO(0x12,124)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਪੂਰਨ ext2fs_open_file(ਅਚਲ ਅੱਖਰ *pathname, ਪੂਰਨ flags, mode_t ਬਹੁਲਕ)
{
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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_FILE_H
ਸਥਿਰ errcode_t unix_funlock(io_channel channel)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਪੂਰਨ ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->unix_flock_flags) {
		ret = flock(ਅੰਕੜੇ->dev, LOCK_UN);
		ਜੇ (ret)
			ਵਾਪਸ errno;

		ਅੰਕੜੇ->unix_flock_flags = 0;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t unix_flock(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flock_flags)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਪੂਰਨ unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	ਜੇ (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	ਜੇ (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	ਜੇ (!unix_flock_flags)
		ਵਾਪਸ 0;

	ret = flock(ਅੰਕੜੇ->dev, unix_flock_flags);
	ਜੇ (ret < 0)
		ਵਾਪਸ errno;

	ਅੰਕੜੇ->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ਵਾਪਸ 0;
}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unix_flock		ਨਿਰਾ

ਸਥਿਰ errcode_t unix_funlock(io_channel channel)
{
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* HAVE_SYS_FILE_H */

ਸਥਿਰ errcode_t unix_open_channel(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ fd,
				   ਪੂਰਨ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ਨਿਰਾ;
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ = ਨਿਰਾ;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
	ਬਣਤਰ		utsname ut;
#ਜੇ_ਅੰਤ

	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(ਖਾਲੀ) fsync(fd);
#ਜੇ_ਅੰਤ

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_io_channel), &io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(io, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ), &ਅੰਕੜੇ);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &io->name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(io->name, name);
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	io->ਖੰਡ_ਮਾਪ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(ਅੰਕੜੇ, 0, ਆਕਾਰ(ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ));
	ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ਅੰਕੜੇ->io_stats.ਅੰਕ_fields = 4;
	ਅੰਕੜੇ->flags = flags;
	ਅੰਕੜੇ->dev = fd;

	ਅੰਕੜੇ->cache_ਮਾਪ = DEFAULT_CACHE_ਮਾਪ;
	ਅੰਕੜੇ->cache_hash_ਖਿਸਕਾਓ = highbit32(ਅੰਕੜੇ->cache_ਮਾਪ);
	ਅੰਕੜੇ->cache = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(DEFAULT_CACHE_ਮਾਪ, ਆਕਾਰ(ਬਣਤਰ unix_cache));
	ਜੇ (!ਅੰਕੜੇ->cache) {
		retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		ਜਾਓ cleanup;
	}

#ਜੇ defined(O_ਸਿੱਧਾ)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO)
		io->align = ext2fs_get_dio_alignment(ਅੰਕੜੇ->dev);
#ਜਾਂਜੇ defined(F_NOCACHE)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO)
		io->align = 4096;
#ਜੇ_ਅੰਤ

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ਜੇ (ext2fs_fstat(ਅੰਕੜੇ->dev, &st) == 0) {
		ਜੇ (ext2fsP_is_disk_device(st.st_ਬਹੁਲਕ)) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKDISCARDZEROES
			ਪੂਰਨ zeroes = 0;

			ਜੇ (ioctl(ਅੰਕੜੇ->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#ਜੇ_ਅੰਤ
			io->flags |= CHANNEL_FLAGS_ਖੰਡ_DEVICE;
		} ਨਹੀਂਤਾਂ {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#ਜੇ defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	ਜੇ (!io->align)
		io->align = 512;
#ਜੇ_ਅੰਤ

#ਜੇ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ਜੇ (io->flags & CHANNEL_FLAGS_ਖੰਡ_DEVICE) {
		ਪੂਰਨ dio_align = ext2fs_get_dio_alignment(fd);

		ਜੇ (io->align < dio_align)
			io->align = dio_align;
	}
#ਜੇ_ਅੰਤ

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ RLIM_INFINITY
#ਜੇ (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RLIM_INFINITY	((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(~0UL>>1))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RLIM_INFINITY  (~0UL)
#ਜੇ_ਅੰਤ
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	ਜੇ ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(ਅੰਕੜੇ->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_ਬਹੁਲਕ))) {
		ਬਣਤਰ rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		ਜੇ (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) rlim.rlim_cur) <
		    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	ਜੇ (flags & IO_FLAG_ਧਾਗੇ) {
		io->flags |= CHANNEL_FLAGS_ਧਾਗੇ;
		retval = pthread_mutex_init(&ਅੰਕੜੇ->cache_mutex, ਨਿਰਾ);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		retval = pthread_mutex_init(&ਅੰਕੜੇ->bounce_mutex, ਨਿਰਾ);
		ਜੇ (retval) {
			pthread_mutex_destroy(&ਅੰਕੜੇ->cache_mutex);
			ਜਾਓ cleanup;
		}
		retval = pthread_mutex_init(&ਅੰਕੜੇ->stats_mutex, ਨਿਰਾ);
		ਜੇ (retval) {
			pthread_mutex_destroy(&ਅੰਕੜੇ->cache_mutex);
			pthread_mutex_destroy(&ਅੰਕੜੇ->bounce_mutex);
			ਜਾਓ cleanup;
		}
	}
#ਜੇ_ਅੰਤ
	*channel = io;
	ਵਾਪਸ 0;

cleanup:
	ਜੇ (ਅੰਕੜੇ) {
		unix_funlock(io);
		ਜੇ (io->manager != unixfd_io_manager && ਅੰਕੜੇ->dev >= 0)
			close(ਅੰਕੜੇ->dev);
		ਜੇ (ਅੰਕੜੇ->cache) {
			free_cache(ਅੰਕੜੇ);
			ਮੁਕਤ(ਅੰਕੜੇ->cache);
		}
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

ਸਥਿਰ errcode_t unixfd_open(ਅਚਲ ਅੱਖਰ *str_fd, ਪੂਰਨ flags,
			     io_channel *channel)
{
	ਪੂਰਨ fd;
	ਪੂਰਨ fd_flags;

	fd = atoi(str_fd);
#ਜੇ defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	ਜੇ (fd_flags == -1)
		ਵਾਪਸ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	ਜੇ (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#ਜੇ defined(O_ਸਿੱਧਾ)
	ਜੇ (fd_flags & O_ਸਿੱਧਾ)
		flags |= IO_FLAG_ਸਿੱਧਾ_IO;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  /* HAVE_FCNTL */

	ਵਾਪਸ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ਸਥਿਰ errcode_t unix_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags,
			   io_channel *channel)
{
	ਪੂਰਨ fd = -1;
	ਪੂਰਨ open_flags;

	ਜੇ (name == 0)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ਜੇ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ਜੇ defined(O_ਸਿੱਧਾ)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO)
		open_flags |= O_ਸਿੱਧਾ;
#ਜੇ_ਅੰਤ
	fd = ext2fs_open_file(name, open_flags, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ errno;
#ਜੇ defined(F_NOCACHE) && !defined(IO_ਸਿੱਧਾ)
	ਜੇ (flags & IO_FLAG_ਸਿੱਧਾ_IO) {
		ਜੇ (fcntl(fd, F_NOCACHE, 1) < 0)
			ਵਾਪਸ errno;
	}
#ਜੇ_ਅੰਤ
	ਵਾਪਸ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ਸਥਿਰ errcode_t unix_close(io_channel channel)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
		ਵਾਪਸ 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0);
#ਜੇ_ਅੰਤ

	unix_funlock(channel);

	ਜੇ (channel->manager != unixfd_io_manager && close(ਅੰਕੜੇ->dev) < 0)
		retval = errno;
	free_cache(ਅੰਕੜੇ);
	ਮੁਕਤ(ਅੰਕੜੇ->cache);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_ਧਾਗੇ) {
		pthread_mutex_destroy(&ਅੰਕੜੇ->cache_mutex);
		pthread_mutex_destroy(&ਅੰਕੜੇ->bounce_mutex);
		pthread_mutex_destroy(&ਅੰਕੜੇ->stats_mutex);
	}
#ਜੇ_ਅੰਤ

	ext2fs_free_mem(&channel->ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਜੇ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t unix_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (channel->ਖੰਡ_ਮਾਪ != blksize) {
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, BOUNCE_MTX);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
		ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, FLUSH_NOLOCK))){
			mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
			mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
			ਵਾਪਸ retval;
		}
#ਜੇ_ਅੰਤ

		channel->ਖੰਡ_ਮਾਪ = blksize;
		free_cache(ਅੰਕੜੇ);
		retval = alloc_cache(channel, ਅੰਕੜੇ);
		mutex_unlock(ਅੰਕੜੇ, BOUNCE_MTX);
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t unix_read_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਣਤਰ unix_cache *cache;
	errcode_t	retval;
	ਅੱਖਰ		*cp;
	ਪੂਰਨ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	ਵਾਪਸ raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
#ਨਹੀਂਤਾਂ
	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_NOCACHE)
		ਵਾਪਸ raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf);
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
	mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
	ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
		/* If it's in the cache, use it! */
		ਜੇ ((cache = ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ, ਨਿਰਾ))) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
			ਲਿਖੋ("Using cached block %lu\n", ਖੰਡ);
#ਜੇ_ਅੰਤ
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cp, cache->buf, channel->ਖੰਡ_ਮਾਪ);
			ਗਿਣਤੀ_2--;
			ਖੰਡ++;
			cp += channel->ਖੰਡ_ਮਾਪ;
			ਜਾਰੀ;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ਲਈ (i=1; i < ਗਿਣਤੀ_2; i++)
			ਜੇ (ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ+i, ਨਿਰਾ))
				ਰੋਕੋ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("Reading %d blocks starting at %lu\n", i, ਖੰਡ);
#ਜੇ_ਅੰਤ
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
		ਜੇ ((retval = raw_read_blk(channel, ਅੰਕੜੇ, ਖੰਡ, i, cp)))
			ਵਾਪਸ retval;
		mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);

		/* Save the results in the cache */
		ਲਈ (j=0; j < i; j++) {
			ਜੇ (!ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ, &cache)) {
				retval = reuse_cache(channel, ਅੰਕੜੇ,
						     cache, ਖੰਡ);
				ਜੇ (retval)
					ਜਾਓ call_write_handler;
				ਸਮ੍ਰਿਤੀ_ਨਕਲ(cache->buf, cp, channel->ਖੰਡ_ਮਾਪ);
			}
			ਗਿਣਤੀ_2--;
			ਖੰਡ++;
			cp += channel->ਖੰਡ_ਮਾਪ;
		}
	}
	mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ 0;

call_write_handler:
	ਜੇ (cache->write_err && channel->write_error) {
		ਅੱਖਰ *err_buf = ਨਿਰਾ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ err_ਖੰਡ = cache->ਖੰਡ;

		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		cache->write_err = 0;
		ਜੇ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ਨਿਰਾ;
		ਨਹੀਂਤਾਂ
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(err_buf, cache->buf, channel->ਖੰਡ_ਮਾਪ);
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
		(channel->write_error)(channel, err_ਖੰਡ, 1, err_buf,
				       channel->ਖੰਡ_ਮਾਪ, -1,
				       retval);
		ਜੇ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ਨਹੀਂਤਾਂ
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */
}

ਸਥਿਰ errcode_t unix_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਵਾਪਸ unix_read_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t unix_write_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਣਤਰ unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ਅਚਲ ਅੱਖਰ	*cp;
	ਪੂਰਨ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	ਵਾਪਸ raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf, 0);
#ਨਹੀਂਤਾਂ
	ਜੇ (ਅੰਕੜੇ->flags & IO_FLAG_NOCACHE)
		ਵਾਪਸ raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ਜੇ (ਗਿਣਤੀ_2 < 0 || ਗਿਣਤੀ_2 > WRITE_ਸਿੱਧਾ_ਮਾਪ) {
		ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ,
						  FLUSH_INVALIDATE)))
			ਵਾਪਸ retval;
		ਵਾਪਸ raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ਜੇ (writethrough)
		retval = raw_write_blk(channel, ਅੰਕੜੇ, ਖੰਡ, ਗਿਣਤੀ_2, buf, 0);

	cp = buf;
	mutex_ਤਾਲਾ(ਅੰਕੜੇ, CACHE_MTX);
	ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
		cache = ਲੱਭੋ_cached_ਖੰਡ(ਅੰਕੜੇ, ਖੰਡ, &reuse);
		ਜੇ (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, ਅੰਕੜੇ, cache, ਖੰਡ);
			ਜੇ (err)
				ਜਾਓ call_write_handler;
		}
		ਜੇ (cache->buf != cp)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(cache->buf, cp, channel->ਖੰਡ_ਮਾਪ);
		cache->dirty = !writethrough;
		ਗਿਣਤੀ_2--;
		ਖੰਡ++;
		cp += channel->ਖੰਡ_ਮਾਪ;
	}
	mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ retval;

call_write_handler:
	ਜੇ (cache->write_err && channel->write_error) {
		ਅੱਖਰ *err_buf = ਨਿਰਾ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ err_ਖੰਡ = cache->ਖੰਡ;

		cache->dirty = 0;
		cache->ਵਿੱਚ_use = 0;
		cache->write_err = 0;
		ਜੇ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ਨਿਰਾ;
		ਨਹੀਂਤਾਂ
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(err_buf, cache->buf, channel->ਖੰਡ_ਮਾਪ);
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
		(channel->write_error)(channel, err_ਖੰਡ, 1, err_buf,
				       channel->ਖੰਡ_ਮਾਪ, -1,
				       retval);
		ਜੇ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ਨਹੀਂਤਾਂ
		mutex_unlock(ਅੰਕੜੇ, CACHE_MTX);
	ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ /* NO_IO_CACHE */
}

ਸਥਿਰ errcode_t unix_cache_readahead(io_channel channel,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ POSIX_FADV_WILLNEED
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *)channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ਵਾਪਸ posix_fadvise(ਅੰਕੜੇ->dev,
			     (ext2_loff_t)ਖੰਡ * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ,
			     (ext2_loff_t)ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ,
			     POSIX_FADV_WILLNEED);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ errcode_t unix_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ unix_write_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t unix_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				 ਪੂਰਨ ਮਾਪ, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (channel->align != 0) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ALIGN_DEBUG
		ਲਿਖੋ("unix_write_byte: O_DIRECT fallback\n");
#ਜੇ_ਅੰਤ
		ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
	}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	ਜੇ ((retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, FLUSH_INVALIDATE)))
		ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ

	ਜੇ (lseek(ਅੰਕੜੇ->dev, ਵਿੱਥ + ਅੰਕੜੇ->ਵਿੱਥ, SEEK_SET) < 0)
		ਵਾਪਸ errno;

	actual = ਲਿਖ(ਅੰਕੜੇ->dev, buf, ਮਾਪ);
	ਜੇ (actual < 0)
		ਵਾਪਸ errno;
	ਜੇ (actual != ਮਾਪ)
		ਵਾਪਸ EXT2_ET_SHORT_WRITE;

	ਵਾਪਸ 0;
}

/*
 * Flush data buffers to disk.
 */
ਸਥਿਰ errcode_t unix_flush(io_channel channel)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FSYNC
	ਜੇ (!retval && fsync(ਅੰਕੜੇ->dev) != 0)
		ਵਾਪਸ errno;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t unix_ਸੈੱਟ_option(io_channel channel, ਅਚਲ ਅੱਖਰ *option,
				 ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ tmp;
	errcode_t retval;
	ਅੱਖਰ *ਸਿਰਾ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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
	ਜੇ (!strcmp(option, "cache")) {
		ਜੇ (!arg)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਜੇ (!strcmp(arg, "on")) {
			ਅੰਕੜੇ->flags &= ~IO_FLAG_NOCACHE;
			ਵਾਪਸ 0;
		}
		ਜੇ (!strcmp(arg, "off")) {
			retval = flush_cached_ਖੰਡਾਂ(channel, ਅੰਕੜੇ, 0);
			ਅੰਕੜੇ->flags |= IO_FLAG_NOCACHE;
			ਵਾਪਸ retval;
		}
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_IO_CACHE
	ਜੇ (!strcmp(option, "cache_blocks")) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਮਾਪ;

		ਜੇ (!arg)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		errno = 0;
		ਮਾਪ = strtoll(arg, ਨਿਰਾ, 0);
		ਜੇ (errno || ਮਾਪ == 0 || ਮਾਪ > INT32_MAX)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		ਜੇ (ਅੰਕੜੇ->cache_ਮਾਪ == ਮਾਪ)
			ਵਾਪਸ 0;
		ਜੇ (ਅੰਕੜੇ->cache_ਮਾਪ > ਮਾਪ)
			ਵਾਪਸ shrink_cache(channel, ਅੰਕੜੇ, ਮਾਪ);
		ਵਾਪਸ grow_cache(channel, ਅੰਕੜੇ, ਮਾਪ);
	}
#ਜੇ_ਅੰਤ
	ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
}

#ਜੇ defined(__linux__) && !defined(BLKDISCARD)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKDISCARD		_IO(0x12,119)
#ਜੇ_ਅੰਤ

ਸਥਿਰ errcode_t unix_discard(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਪੂਰਨ		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (channel->flags & CHANNEL_FLAGS_NODISCARD)
		ਜਾਓ unimplemented;

	ਜੇ (channel->flags & CHANNEL_FLAGS_ਖੰਡ_DEVICE) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(ਖੰਡ) * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ;
		range[1] = (__u64)(ਗਿਣਤੀ_2) * channel->ਖੰਡ_ਮਾਪ;

		ret = ioctl(ਅੰਕੜੇ->dev, BLKDISCARD, &range);
#ਨਹੀਂਤਾਂ
		ਜਾਓ unimplemented;
#ਜੇ_ਅੰਤ
	} ਨਹੀਂਤਾਂ {
#ਜੇ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(ਅੰਕੜੇ->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(ਖੰਡ) * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ,
				(off_t)(ਗਿਣਤੀ_2) * channel->ਖੰਡ_ਮਾਪ);
#ਨਹੀਂਤਾਂ
		ਜਾਓ unimplemented;
#ਜੇ_ਅੰਤ
	}
	ਜੇ (ret < 0) {
		ਜੇ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			ਜਾਓ unimplemented;
		}
		ਵਾਪਸ errno;
	}
	ਵਾਪਸ 0;
unimplemented:
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ਸਥਿਰ ਪੂਰਨ __unix_zeroout(ਪੂਰਨ fd, off_t ਵਿੱਥ, off_t len)
{
	ਪੂਰਨ ret = -1;

#ਜੇ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, ਵਿੱਥ, len);
	ਜੇ (ret == 0)
		ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
#ਜੇ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			ਵਿੱਥ,  len);
	ਜੇ (ret == 0)
		ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
	errno = EOPNOTSUPP;
	ਵਾਪਸ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"
#ਜੇ_ਅੰਤ
ਸਥਿਰ errcode_t unix_zeroout(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਪੂਰਨ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ unix_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (!(channel->flags & CHANNEL_FLAGS_ਖੰਡ_DEVICE)) {
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
	}

	ਜੇ (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		ਜਾਓ unimplemented;

	ret = __unix_zeroout(ਅੰਕੜੇ->dev,
			(off_t)(ਖੰਡ) * channel->ਖੰਡ_ਮਾਪ + ਅੰਕੜੇ->ਵਿੱਥ,
			(off_t)(ਗਿਣਤੀ_2) * channel->ਖੰਡ_ਮਾਪ);
err:
	ਜੇ (ret < 0) {
		ਜੇ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			ਜਾਓ unimplemented;
		}
		ਵਾਪਸ errno;
	}
	ਵਾਪਸ 0;
unimplemented:
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}
#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.ਸੈੱਟ_blksize	= unix_ਸੈੱਟ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.ਸੈੱਟ_option	= unix_ਸੈੱਟ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unix_io_manager = &struct_unix_manager;

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.open		= unixfd_open,
	.close		= unix_close,
	.ਸੈੱਟ_blksize	= unix_ਸੈੱਟ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.ਸੈੱਟ_option	= unix_ਸੈੱਟ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
