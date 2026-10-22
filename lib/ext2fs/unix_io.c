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

#إذا !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#وصف_ڪريو XOPEN_ذريعو 600
#وصف_ڪريو DARWIN_C_ذريعو
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
#جيڪڏهن_وصف_ٿيل __linux__
#ضمن <sys/utsname.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#جيڪڏهن_پورو
#إذا HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
#ضمن <pthread.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#جيڪڏهن_پورو

#إذا defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#وصف_ڪريو BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#جيڪڏهن_پورو

#وصف_رد ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح ڳوليو_آخري_ٻٽو_انگ_سيٽ(صحيح x)
{
	صحيح r = 32;

	إذا (!x)
		أرجع 0;
	إذا (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	إذا (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	إذا (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	إذا (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	إذا (!(x & 0x80000000u)) {
		r -= 1;
	}
	أرجع r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ساكن مضمن صحيح highbit32(uint32_t v)
{
	أرجع ڳوليو_آخري_ٻٽو_انگ_سيٽ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)

بنية unix_cache {
	حرف			*buf;
	بنانشان ڊگهو ڊگهو	بلاڪ;
	صحيح			access_وقت;
	بنانشان		dirty:1;
	بنانشان		اندريون_use:1;
	بنانشان		write_err:1;
};

#وصف_ڪريو DEFAULT_CACHE_سائيز 8
#وصف_ڪريو WRITE_سڌو_سائيز 4	/* Must be smaller than CACHE_SIZE */
#وصف_ڪريو READ_سڌو_سائيز 4	/* Should be smaller than CACHE_SIZE */

بنية unix_ذاتي_معلومات {
	صحيح	magic;
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_وقت;
	صحيح	unix_flock_flags;
	ext2_loff_t وٿي;
	بنية unix_cache *cache;
	بنانشان صحيح cache_سائيز;
	بنانشان صحيح cache_hash_سرڪايو;
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#جيڪڏهن_پورو
};

#وصف_ڪريو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعريفنوع تعداد تالو_قسم {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} قسم_t;

#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
ساكن مضمن pthread_mutex_t *get_mutex(بنية unix_ذاتي_معلومات *معلومات,
					 قسم_t قسم)
{
	إذا (معلومات->flags & IO_FLAG_ڌاڳا) {
		بدل (قسم) {
		حالة CACHE_MTX:
			أرجع &معلومات->cache_mutex;
		حالة BOUNCE_MTX:
			أرجع &معلومات->bounce_mutex;
		حالة STATS_MTX:
			أرجع &معلومات->stats_mutex;
		}
	}
	أرجع عدم;
}
#جيڪڏهن_پورو

ساكن مضمن فراغ mutex_تالو(بنية unix_ذاتي_معلومات *معلومات, قسم_t قسم)
{
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(معلومات,قسم);

	إذا (mtx)
		pthread_mutex_تالو(mtx);
#جيڪڏهن_پورو
}

ساكن مضمن فراغ mutex_unlock(بنية unix_ذاتي_معلومات *معلومات, قسم_t قسم)
{
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(معلومات,قسم);

	إذا (mtx)
		pthread_mutex_unlock(mtx);
#جيڪڏهن_پورو
}

ساكن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية unix_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (stats) {
		mutex_تالو(معلومات, STATS_MTX);
		*stats = &معلومات->io_stats;
		mutex_unlock(معلومات, STATS_MTX);
	}

	أرجع retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_ذاتي_معلومات *معلومات,
			      بنانشان ڊگهو ڊگهو بلاڪ,
			      صحيح ڳڻپ, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		سائيز;
	ext2_loff_t	location;
	صحيح		actual = 0;
	بنانشان حرف	*buf = bufv;
	ssize_t		really_read = 0;
	بنانشان ڊگهو ڊگهو aligned_blk;
	صحيح		align_سائيز, وٿي;

	سائيز = (ڳڻپ < 0) ? -ڳڻپ : (ext2_loff_t) ڳڻپ * channel->بلاڪ_سائيز;
	mutex_تالو(معلومات, STATS_MTX);
	معلومات->io_stats.بائيٽون_read += سائيز;
	mutex_unlock(معلومات, STATS_MTX);
	location = ((ext2_loff_t) بلاڪ * channel->بلاڪ_سائيز) + معلومات->وٿي;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#جيڪڏهن_وصف_ٿيل HAVE_PREAD64
	/* Try an aligned pread */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائيز, channel->align))) {
		actual = pread64(معلومات->dev, buf, سائيز, location);
		إذا (actual == سائيز)
			أرجع 0;
		actual = 0;
	}
#وإلاإذا HAVE_PREAD
	/* Try an aligned pread */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(سائيز, channel->align)))) {
		actual = pread(معلومات->dev, buf, سائيز, location);
		إذا (actual == سائيز)
			أرجع 0;
		actual = 0;
	}
#جيڪڏهن_پورو /* HAVE_PREAD */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائيز, channel->align))) {
		mutex_تالو(معلومات, BOUNCE_MTX);
		إذا (ext2fs_llseek(معلومات->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(معلومات->dev, buf, سائيز);
		إذا (actual != سائيز) {
		short_read:
			إذا (actual < 0) {
				retval = errno;
				actual = 0;
			} وإلا
				retval = EXT2_ET_SHORT_READ;
			اذهب error_unlock;
		}
		اذهب success_unlock;
	}

#جيڪڏهن_وصف_ٿيل ALIGN_DEBUG
	لکو("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بنانشان ڊگهو) سائيز);
#جيڪڏهن_پورو

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاڪ_سائيز > channel->align) &&
	    (channel->بلاڪ_سائيز % channel->align) == 0)
		align_سائيز = channel->بلاڪ_سائيز;
	وإلا
		align_سائيز = channel->align;
	aligned_blk = location / align_سائيز;
	وٿي = location % align_سائيز;

	mutex_تالو(معلومات, BOUNCE_MTX);
	إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_سائيز, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	طالما (سائيز > 0) {
		actual = اقرأ(معلومات->dev, معلومات->bounce, align_سائيز);
		إذا (actual != align_سائيز) {
			actual = really_read;
			buf -= really_read;
			سائيز += really_read;
			اذهب short_read;
		}
		إذا ((actual + وٿي) > align_سائيز)
			actual = align_سائيز - وٿي;
		إذا (actual > سائيز)
			actual = سائيز;
		يادگيري_نقل(buf, (حرف *)معلومات->bounce + وٿي, actual);

		really_read += actual;
		سائيز -= actual;
		buf += actual;
		وٿي = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(معلومات, BOUNCE_MTX);
	أرجع 0;

error_unlock:
	mutex_unlock(معلومات, BOUNCE_MTX);
	إذا (actual >= 0 && actual < سائيز)
		memset((حرف *) buf+actual, 0, سائيز-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاڪ, ڳڻپ, buf,
					       سائيز, actual, retval);
	أرجع retval;
}

#وصف_ڪريو RAW_WRITE_NO_HANDLER	(1U << 0)
#وصف_ڪريو RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_ذاتي_معلومات *معلومات,
			       بنانشان ڊگهو ڊگهو بلاڪ,
			       صحيح ڳڻپ, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		سائيز;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت بنانشان حرف *buf = bufv;
	بنانشان ڊگهو ڊگهو aligned_blk;
	صحيح		align_سائيز, وٿي;

	إذا (ڳڻپ == 1)
		سائيز = channel->بلاڪ_سائيز;
	وإلا {
		إذا (ڳڻپ < 0)
			سائيز = -ڳڻپ;
		وإلا
			سائيز = (ext2_loff_t) ڳڻپ * channel->بلاڪ_سائيز;
	}
	mutex_تالو(معلومات, STATS_MTX);
	معلومات->io_stats.بائيٽون_written += سائيز;
	mutex_unlock(معلومات, STATS_MTX);

	location = ((ext2_loff_t) بلاڪ * channel->بلاڪ_سائيز) + معلومات->وٿي;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#جيڪڏهن_وصف_ٿيل HAVE_PWRITE64
	/* Try an aligned pwrite */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائيز, channel->align))) {
		actual = pwrite64(معلومات->dev, buf, سائيز, location);
		إذا (actual == سائيز)
			أرجع 0;
	}
#وإلاإذا HAVE_PWRITE
	/* Try an aligned pwrite */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(سائيز, channel->align)))) {
		actual = pwrite(معلومات->dev, buf, سائيز, location);
		إذا (actual == سائيز)
			أرجع 0;
	}
#جيڪڏهن_پورو /* HAVE_PWRITE */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائيز, channel->align))) {
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_تالو(معلومات, BOUNCE_MTX);
		إذا (ext2fs_llseek(معلومات->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(معلومات->dev, buf, سائيز);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(معلومات, BOUNCE_MTX);
		إذا (actual < 0) {
			retval = errno;
			اذهب error_ٻاهريون;
		}
		إذا (actual != سائيز) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_ٻاهريون;
		}
		أرجع 0;
	}

#جيڪڏهن_وصف_ٿيل ALIGN_DEBUG
	لکو("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بنانشان ڊگهو) سائيز);
#جيڪڏهن_پورو
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاڪ_سائيز > channel->align) &&
	    (channel->بلاڪ_سائيز % channel->align) == 0)
		align_سائيز = channel->بلاڪ_سائيز;
	وإلا
		align_سائيز = channel->align;
	aligned_blk = location / align_سائيز;
	وٿي = location % align_سائيز;

	طالما (سائيز > 0) {
		صحيح actual_w;

		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_تالو(معلومات, BOUNCE_MTX);
		إذا (سائيز < align_سائيز || وٿي) {
			إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_سائيز,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(معلومات->dev, معلومات->bounce,
				      align_سائيز);
			إذا (actual != align_سائيز) {
				إذا (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) معلومات->bounce + actual, 0,
				       align_سائيز - actual);
			}
		}
		actual = سائيز;
		إذا ((actual + وٿي) > align_سائيز)
			actual = align_سائيز - وٿي;
		إذا (actual > سائيز)
			actual = سائيز;
		يادگيري_نقل(((حرف *)معلومات->bounce) + وٿي, buf, actual);
		إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_سائيز, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(معلومات->dev, معلومات->bounce, align_سائيز);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(معلومات, BOUNCE_MTX);
		إذا (actual_w < 0) {
			retval = errno;
			اذهب error_ٻاهريون;
		}
		إذا (actual_w != align_سائيز)
			اذهب short_write;
		سائيز -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		وٿي = 0;
	}
	أرجع 0;

error_unlock:
	إذا (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(معلومات, BOUNCE_MTX);
error_ٻاهريون:
	إذا (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, بلاڪ, ڳڻپ, buf,
						سائيز, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية unix_ذاتي_معلومات *معلومات)
{
	errcode_t		retval;
	بنية unix_cache	*cache;
	بنانشان صحيح		i;

	معلومات->access_وقت = 0;
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_سائيز; i++, cache++) {
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
ساكن فراغ free_cache(بنية unix_ذاتي_معلومات *معلومات)
{
	بنية unix_cache	*cache;
	بنانشان صحيح		i;

	معلومات->access_وقت = 0;
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_سائيز; i++, cache++) {
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

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#وصف_ڪريو GOLDEN_تناسب_مفرد	0x9e37fffffffc0001UL
#جيڪڏهن_وصف_نه_ٿيل CACHE_سٽ_سائيز
/* if the system didn't tell us, guess something reasonable */
#وصف_ڪريو CACHE_سٽ_سائيز		64
#جيڪڏهن_پورو

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن بنانشان صحيح
cache_hash(بنية unix_ذاتي_معلومات *معلومات, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	إذا (معلومات->cache_سائيز <= DEFAULT_CACHE_سائيز)
		أرجع 0;

	tmp = hashval ^ (GOLDEN_تناسب_مفرد + hashval) / CACHE_سٽ_سائيز;
	tmp = tmp ^ ((tmp ^ GOLDEN_تناسب_مفرد) >> معلومات->cache_hash_سرڪايو);
	أرجع tmp % معلومات->cache_سائيز;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية unix_cache *ڳوليو_cached_بلاڪ(بنية unix_ذاتي_معلومات *معلومات,
					    بنانشان ڊگهو ڊگهو بلاڪ,
					    بنية unix_cache **eldest)
{
	بنية unix_cache	*cache, *اڻ_استعمال_ٿيل_cache, *oldest_cache;
	بنانشان صحيح		hash = cache_hash(معلومات, بلاڪ);
	بنانشان صحيح		i;

	اڻ_استعمال_ٿيل_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	لكل (i = hash, cache = معلومات->cache + hash;
	     i < معلومات->cache_سائيز;
	     i++, cache++) {
		إذا (!cache->اندريون_use) {
			إذا (!اڻ_استعمال_ٿيل_cache)
				اڻ_استعمال_ٿيل_cache = cache;
			استمر;
		}
		إذا (cache->بلاڪ == بلاڪ) {
			cache->access_وقت = ++معلومات->access_وقت;
			معلومات->io_stats.cache_ڪاميابيون++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	لكل (i = 0, cache = معلومات->cache; i < hash; i++, cache++) {
		إذا (!cache->اندريون_use) {
			إذا (!اڻ_استعمال_ٿيل_cache)
				اڻ_استعمال_ٿيل_cache = cache;
			استمر;
		}
		إذا (cache->بلاڪ == بلاڪ) {
			cache->access_وقت = ++معلومات->access_وقت;
			معلومات->io_stats.cache_ڪاميابيون++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (اڻ_استعمال_ٿيل_cache) ? اڻ_استعمال_ٿيل_cache : oldest_cache;
	معلومات->io_stats.cache_ناڪاميون++;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_ذاتي_معلومات *معلومات, بنية unix_cache *cache,
		بنانشان ڊگهو ڊگهو بلاڪ)
{
	إذا (cache->dirty && cache->اندريون_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, معلومات, cache->بلاڪ, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		إذا (retval) {
			cache->write_err = 1;
			أرجع retval;
		}
	}

	cache->اندريون_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->بلاڪ = بلاڪ;
	cache->access_وقت = ++معلومات->access_وقت;
	أرجع 0;
}

#وصف_ڪريو FLUSH_INVALIDATE	0x01
#وصف_ڪريو FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاڪون(io_channel channel,
				     بنية unix_ذاتي_معلومات *معلومات,
				     صحيح flags)
{
	بنية unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	بنانشان صحيح		i;
	صحيح			errors_مليو = 0;

	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_تالو(معلومات, CACHE_MTX);
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_سائيز; i++, cache++) {
		إذا (!cache->اندريون_use)
			استمر;
		إذا (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			إذا (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, معلومات,
					       cache->بلاڪ, 1, cache->buf,
					       raw_flags);
			إذا (retval) {
				cache->write_err = 1;
				errors_مليو = 1;
				retval2 = retval;
			} وإلا {
				cache->dirty = 0;
				cache->write_err = 0;
				إذا (flags & FLUSH_INVALIDATE)
					cache->اندريون_use = 0;
			}
		} وإلا إذا (flags & FLUSH_INVALIDATE) {
			cache->اندريون_use = 0;
		}
	}
	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(معلومات, CACHE_MTX);
retry:
	طالما (errors_مليو) {
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_تالو(معلومات, CACHE_MTX);
		errors_مليو = 0;
		لكل (i=0, cache = معلومات->cache; i < معلومات->cache_سائيز; i++, cache++) {
			إذا (!cache->اندريون_use || !cache->write_err)
				استمر;
			errors_مليو = 1;
			إذا (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				بنانشان ڊگهو ڊگهو err_بلاڪ = cache->بلاڪ;

				cache->dirty = 0;
				cache->اندريون_use = 0;
				cache->write_err = 0;
				إذا (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				وإلا
					يادگيري_نقل(err_buf, cache->buf,
					       channel->بلاڪ_سائيز);
				mutex_unlock(معلومات, CACHE_MTX);
				(channel->write_error)(channel, err_بلاڪ,
					1, err_buf, channel->بلاڪ_سائيز, -1,
					retval2);
				إذا (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_تالو(معلومات, CACHE_MTX);
				اذهب retry;
			} وإلا
				cache->write_err = 0;
		}
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(معلومات, CACHE_MTX);
	}
	أرجع retval2;
}

/* Shrink the cache buffers */
ساكن errcode_t shrink_cache(io_channel channel,
			      بنية unix_ذاتي_معلومات *معلومات,
			      بنانشان صحيح new_سائيز)
{
	بنية unix_cache	*cache, *new_cache;
	بنانشان صحيح		i;
	errcode_t		retval;

	mutex_تالو(معلومات, CACHE_MTX);

	retval = flush_cached_بلاڪون(channel, معلومات,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	لكل (i = new_سائيز, cache = معلومات->cache + new_سائيز;
	     i < معلومات->cache_سائيز;
	     i++, cache++) {
		cache->بلاڪ = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندريون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(معلومات->cache, new_سائيز * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_يادگيري;
		اذهب unlock;
	}

	معلومات->cache = new_cache;
	معلومات->cache_سائيز = new_سائيز;
	معلومات->cache_hash_سرڪايو = highbit32(معلومات->cache_سائيز);

unlock:
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
}

/* Grow the cache buffers */
ساكن errcode_t grow_cache(io_channel channel,
			    بنية unix_ذاتي_معلومات *معلومات,
			    بنانشان صحيح new_سائيز)
{
	بنية unix_cache	*cache, *new_cache;
	بنانشان صحيح		i;
	errcode_t		retval;

	mutex_تالو(معلومات, CACHE_MTX);

	retval = flush_cached_بلاڪون(channel, معلومات,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	new_cache = realloc(معلومات->cache, new_سائيز * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_يادگيري;
		اذهب unlock;
	}
	معلومات->cache = new_cache;

	لكل (i = معلومات->cache_سائيز, cache = new_cache + معلومات->cache_سائيز;
	     i < new_سائيز;
	     i++, cache++) {
		cache->بلاڪ = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندريون_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval) {
			طالما (i > معلومات->cache_سائيز) {
				cache--; i--;
				إذا (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	معلومات->cache_سائيز = new_سائيز;
	معلومات->cache_hash_سرڪايو = highbit32(معلومات->cache_سائيز);

unlock:
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
}
#جيڪڏهن_پورو /* NO_IO_CACHE */

#جيڪڏهن_وصف_ٿيل __linux__
#جيڪڏهن_وصف_نه_ٿيل BLKDISCARDZEROES
#وصف_ڪريو BLKDISCARDZEROES _IO(0x12,124)
#جيڪڏهن_پورو
#جيڪڏهن_پورو

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t گهڻائي)
{
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

#جيڪڏهن_وصف_ٿيل HAVE_SYS_FILE_H
ساكن errcode_t unix_funlock(io_channel channel)
{
	بنية unix_ذاتي_معلومات *معلومات;
	صحيح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->unix_flock_flags) {
		ret = flock(معلومات->dev, LOCK_UN);
		إذا (ret)
			أرجع errno;

		معلومات->unix_flock_flags = 0;
	}

	أرجع 0;
}

ساكن errcode_t unix_flock(io_channel channel, بنانشان صحيح flock_flags)
{
	بنية unix_ذاتي_معلومات *معلومات;
	صحيح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	إذا (ret)
		أرجع ret;

	إذا (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	إذا (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	إذا (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	إذا (!unix_flock_flags)
		أرجع 0;

	ret = flock(معلومات->dev, unix_flock_flags);
	إذا (ret < 0)
		أرجع errno;

	معلومات->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	أرجع 0;
}
#وإلا
#وصف_ڪريو unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	أرجع 0;
}
#جيڪڏهن_پورو /* HAVE_SYS_FILE_H */

ساكن errcode_t unix_open_channel(ثابت حرف *name, صحيح fd,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	بنية unix_ذاتي_معلومات *معلومات = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;
#جيڪڏهن_وصف_ٿيل __linux__
	بنية		utsname ut;
#جيڪڏهن_پورو

	إذا (ext2fs_محفوظ_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#جيڪڏهن_وصف_ٿيل __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#جيڪڏهن_پورو

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_ذاتي_معلومات), &معلومات);
	إذا (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(لڙي_ڊيگهه(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->ذاتي_معلومات = معلومات;
	io->بلاڪ_سائيز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	إذا (ext2fs_محفوظ_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(معلومات, 0, حجم(بنية unix_ذاتي_معلومات));
	معلومات->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	معلومات->io_stats.انگ_fields = 4;
	معلومات->flags = flags;
	معلومات->dev = fd;

	معلومات->cache_سائيز = DEFAULT_CACHE_سائيز;
	معلومات->cache_hash_سرڪايو = highbit32(معلومات->cache_سائيز);
	معلومات->cache = صفر_يادگيري_ڏيو(DEFAULT_CACHE_سائيز, حجم(بنية unix_cache));
	إذا (!معلومات->cache) {
		retval = EXT2_ET_NO_يادگيري;
		اذهب cleanup;
	}

#إذا defined(O_سڌو)
	إذا (flags & IO_FLAG_سڌو_IO)
		io->align = ext2fs_get_dio_alignment(معلومات->dev);
#وإلاإذا defined(F_NOCACHE)
	إذا (flags & IO_FLAG_سڌو_IO)
		io->align = 4096;
#جيڪڏهن_پورو

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(معلومات->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_گهڻائي)) {
#جيڪڏهن_وصف_ٿيل BLKDISCARDZEROES
			صحيح zeroes = 0;

			إذا (ioctl(معلومات->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#جيڪڏهن_پورو
			io->flags |= CHANNEL_FLAGS_بلاڪ_DEVICE;
		} وإلا {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#إذا defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	إذا (!io->align)
		io->align = 512;
#جيڪڏهن_پورو

#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	إذا (io->flags & CHANNEL_FLAGS_بلاڪ_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		إذا (io->align < dio_align)
			io->align = dio_align;
	}
#جيڪڏهن_پورو

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

#جيڪڏهن_وصف_ٿيل __linux__
#وصف_رد RLIM_INFINITY
#إذا (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#وصف_ڪريو RLIM_INFINITY	((بنانشان ڊگهو)(~0UL>>1))
#وإلا
#وصف_ڪريو RLIM_INFINITY  (~0UL)
#جيڪڏهن_پورو
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	إذا ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(معلومات->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_گهڻائي))) {
		بنية rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (بنانشان ڊگهو) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		إذا (((بنانشان ڊگهو) rlim.rlim_cur) <
		    ((بنانشان ڊگهو) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	إذا (flags & IO_FLAG_ڌاڳا) {
		io->flags |= CHANNEL_FLAGS_ڌاڳا;
		retval = pthread_mutex_init(&معلومات->cache_mutex, عدم);
		إذا (retval)
			اذهب cleanup;
		retval = pthread_mutex_init(&معلومات->bounce_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&معلومات->cache_mutex);
			اذهب cleanup;
		}
		retval = pthread_mutex_init(&معلومات->stats_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&معلومات->cache_mutex);
			pthread_mutex_destroy(&معلومات->bounce_mutex);
			اذهب cleanup;
		}
	}
#جيڪڏهن_پورو
	*channel = io;
	أرجع 0;

cleanup:
	إذا (معلومات) {
		unix_funlock(io);
		إذا (io->manager != unixfd_io_manager && معلومات->dev >= 0)
			close(معلومات->dev);
		إذا (معلومات->cache) {
			free_cache(معلومات);
			حرر(معلومات->cache);
		}
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

ساكن errcode_t unixfd_open(ثابت حرف *str_fd, صحيح flags,
			     io_channel *channel)
{
	صحيح fd;
	صحيح fd_flags;

	fd = atoi(str_fd);
#إذا defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	إذا (fd_flags == -1)
		أرجع EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	إذا (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#إذا defined(O_سڌو)
	إذا (fd_flags & O_سڌو)
		flags |= IO_FLAG_سڌو_IO;
#جيڪڏهن_پورو
#جيڪڏهن_پورو  /* HAVE_FCNTL */

	أرجع unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ساكن errcode_t unix_open(ثابت حرف *name, صحيح flags,
			   io_channel *channel)
{
	صحيح fd = -1;
	صحيح open_flags;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	إذا (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#إذا defined(O_سڌو)
	إذا (flags & IO_FLAG_سڌو_IO)
		open_flags |= O_سڌو;
#جيڪڏهن_پورو
	fd = ext2fs_open_file(name, open_flags, 0);
	إذا (fd < 0)
		أرجع errno;
#إذا defined(F_NOCACHE) && !defined(IO_سڌو)
	إذا (flags & IO_FLAG_سڌو_IO) {
		إذا (fcntl(fd, F_NOCACHE, 1) < 0)
			أرجع errno;
	}
#جيڪڏهن_پورو
	أرجع unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	retval = flush_cached_بلاڪون(channel, معلومات, 0);
#جيڪڏهن_پورو

	unix_funlock(channel);

	إذا (channel->manager != unixfd_io_manager && close(معلومات->dev) < 0)
		retval = errno;
	free_cache(معلومات);
	حرر(معلومات->cache);
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	إذا (معلومات->flags & IO_FLAG_ڌاڳا) {
		pthread_mutex_destroy(&معلومات->cache_mutex);
		pthread_mutex_destroy(&معلومات->bounce_mutex);
		pthread_mutex_destroy(&معلومات->stats_mutex);
	}
#جيڪڏهن_پورو

	ext2fs_free_mem(&channel->ذاتي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t unix_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_ذاتي_معلومات *معلومات;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->بلاڪ_سائيز != blksize) {
		mutex_تالو(معلومات, CACHE_MTX);
		mutex_تالو(معلومات, BOUNCE_MTX);
#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
		إذا ((retval = flush_cached_بلاڪون(channel, معلومات, FLUSH_NOLOCK))){
			mutex_unlock(معلومات, BOUNCE_MTX);
			mutex_unlock(معلومات, CACHE_MTX);
			أرجع retval;
		}
#جيڪڏهن_پورو

		channel->بلاڪ_سائيز = blksize;
		free_cache(معلومات);
		retval = alloc_cache(channel, معلومات);
		mutex_unlock(معلومات, BOUNCE_MTX);
		mutex_unlock(معلومات, CACHE_MTX);
	}
	أرجع retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	بنية unix_ذاتي_معلومات *معلومات;
	بنية unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جيڪڏهن_وصف_ٿيل NO_IO_CACHE
	أرجع raw_read_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
#وإلا
	إذا (معلومات->flags & IO_FLAG_NOCACHE)
		أرجع raw_read_blk(channel, معلومات, بلاڪ, ڳڻپ, buf);
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
	mutex_تالو(معلومات, CACHE_MTX);
	طالما (ڳڻپ > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = ڳوليو_cached_بلاڪ(معلومات, بلاڪ, عدم))) {
#جيڪڏهن_وصف_ٿيل DEBUG
			لکو("Using cached block %lu\n", بلاڪ);
#جيڪڏهن_پورو
			يادگيري_نقل(cp, cache->buf, channel->بلاڪ_سائيز);
			ڳڻپ--;
			بلاڪ++;
			cp += channel->بلاڪ_سائيز;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < ڳڻپ; i++)
			إذا (ڳوليو_cached_بلاڪ(معلومات, بلاڪ+i, عدم))
				اكسر;
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("Reading %d blocks starting at %lu\n", i, بلاڪ);
#جيڪڏهن_پورو
		mutex_unlock(معلومات, CACHE_MTX);
		إذا ((retval = raw_read_blk(channel, معلومات, بلاڪ, i, cp)))
			أرجع retval;
		mutex_تالو(معلومات, CACHE_MTX);

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			إذا (!ڳوليو_cached_بلاڪ(معلومات, بلاڪ, &cache)) {
				retval = reuse_cache(channel, معلومات,
						     cache, بلاڪ);
				إذا (retval)
					اذهب call_write_handler;
				يادگيري_نقل(cache->buf, cp, channel->بلاڪ_سائيز);
			}
			ڳڻپ--;
			بلاڪ++;
			cp += channel->بلاڪ_سائيز;
		}
	}
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع 0;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بنانشان ڊگهو ڊگهو err_بلاڪ = cache->بلاڪ;

		cache->dirty = 0;
		cache->اندريون_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			يادگيري_نقل(err_buf, cache->buf, channel->بلاڪ_سائيز);
		mutex_unlock(معلومات, CACHE_MTX);
		(channel->write_error)(channel, err_بلاڪ, 1, err_buf,
				       channel->بلاڪ_سائيز, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
#جيڪڏهن_پورو /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	أرجع unix_read_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	بنية unix_ذاتي_معلومات *معلومات;
	بنية unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جيڪڏهن_وصف_ٿيل NO_IO_CACHE
	أرجع raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf, 0);
#وإلا
	إذا (معلومات->flags & IO_FLAG_NOCACHE)
		أرجع raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (ڳڻپ < 0 || ڳڻپ > WRITE_سڌو_سائيز) {
		إذا ((retval = flush_cached_بلاڪون(channel, معلومات,
						  FLUSH_INVALIDATE)))
			أرجع retval;
		أرجع raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, معلومات, بلاڪ, ڳڻپ, buf, 0);

	cp = buf;
	mutex_تالو(معلومات, CACHE_MTX);
	طالما (ڳڻپ > 0) {
		cache = ڳوليو_cached_بلاڪ(معلومات, بلاڪ, &reuse);
		إذا (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, معلومات, cache, بلاڪ);
			إذا (err)
				اذهب call_write_handler;
		}
		إذا (cache->buf != cp)
			يادگيري_نقل(cache->buf, cp, channel->بلاڪ_سائيز);
		cache->dirty = !writethrough;
		ڳڻپ--;
		بلاڪ++;
		cp += channel->بلاڪ_سائيز;
	}
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بنانشان ڊگهو ڊگهو err_بلاڪ = cache->بلاڪ;

		cache->dirty = 0;
		cache->اندريون_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			يادگيري_نقل(err_buf, cache->buf, channel->بلاڪ_سائيز);
		mutex_unlock(معلومات, CACHE_MTX);
		(channel->write_error)(channel, err_بلاڪ, 1, err_buf,
				       channel->بلاڪ_سائيز, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
#جيڪڏهن_پورو /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      بنانشان ڊگهو ڊگهو بلاڪ,
				      بنانشان ڊگهو ڊگهو ڳڻپ)
{
#جيڪڏهن_وصف_ٿيل POSIX_FADV_WILLNEED
	بنية unix_ذاتي_معلومات *معلومات;

	معلومات = (بنية unix_ذاتي_معلومات *)channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	أرجع posix_fadvise(معلومات->dev,
			     (ext2_loff_t)بلاڪ * channel->بلاڪ_سائيز + معلومات->وٿي,
			     (ext2_loff_t)ڳڻپ * channel->بلاڪ_سائيز,
			     POSIX_FADV_WILLNEED);
#وإلا
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
#جيڪڏهن_پورو
}

ساكن errcode_t unix_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	أرجع unix_write_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t unix_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				 صحيح سائيز, ثابت فراغ *buf)
{
	بنية unix_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->align != 0) {
#جيڪڏهن_وصف_ٿيل ALIGN_DEBUG
		لکو("unix_write_byte: O_DIRECT fallback\n");
#جيڪڏهن_پورو
		أرجع EXT2_ET_UNIMPLEMENTED;
	}

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	إذا ((retval = flush_cached_بلاڪون(channel, معلومات, FLUSH_INVALIDATE)))
		أرجع retval;
#جيڪڏهن_پورو

	إذا (lseek(معلومات->dev, وٿي + معلومات->وٿي, SEEK_SET) < 0)
		أرجع errno;

	actual = اكتب(معلومات->dev, buf, سائيز);
	إذا (actual < 0)
		أرجع errno;
	إذا (actual != سائيز)
		أرجع EXT2_ET_SHORT_WRITE;

	أرجع 0;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t unix_flush(io_channel channel)
{
	بنية unix_ذاتي_معلومات *معلومات;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	retval = flush_cached_بلاڪون(channel, معلومات, 0);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_FSYNC
	إذا (!retval && fsync(معلومات->dev) != 0)
		أرجع errno;
#جيڪڏهن_پورو
	أرجع retval;
}

ساكن errcode_t unix_سيٽ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية unix_ذاتي_معلومات *معلومات;
	بنانشان ڊگهو ڊگهو tmp;
	errcode_t retval;
	حرف *پڇاڙي;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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
	إذا (!strcmp(option, "cache")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دليل;
		إذا (!strcmp(arg, "on")) {
			معلومات->flags &= ~IO_FLAG_NOCACHE;
			أرجع 0;
		}
		إذا (!strcmp(arg, "off")) {
			retval = flush_cached_بلاڪون(channel, معلومات, 0);
			معلومات->flags |= IO_FLAG_NOCACHE;
			أرجع retval;
		}
		أرجع EXT2_ET_INVALID_دليل;
	}
#جيڪڏهن_وصف_نه_ٿيل NO_IO_CACHE
	إذا (!strcmp(option, "cache_blocks")) {
		بنانشان ڊگهو ڊگهو	سائيز;

		إذا (!arg)
			أرجع EXT2_ET_INVALID_دليل;

		errno = 0;
		سائيز = strtoll(arg, عدم, 0);
		إذا (errno || سائيز == 0 || سائيز > INT32_MAX)
			أرجع EXT2_ET_INVALID_دليل;

		إذا (معلومات->cache_سائيز == سائيز)
			أرجع 0;
		إذا (معلومات->cache_سائيز > سائيز)
			أرجع shrink_cache(channel, معلومات, سائيز);
		أرجع grow_cache(channel, معلومات, سائيز);
	}
#جيڪڏهن_پورو
	أرجع EXT2_ET_INVALID_دليل;
}

#إذا defined(__linux__) && !defined(BLKDISCARD)
#وصف_ڪريو BLKDISCARD		_IO(0x12,119)
#جيڪڏهن_پورو

ساكن errcode_t unix_discard(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية unix_ذاتي_معلومات *معلومات;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	إذا (channel->flags & CHANNEL_FLAGS_بلاڪ_DEVICE) {
#جيڪڏهن_وصف_ٿيل BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(بلاڪ) * channel->بلاڪ_سائيز + معلومات->وٿي;
		range[1] = (__u64)(ڳڻپ) * channel->بلاڪ_سائيز;

		ret = ioctl(معلومات->dev, BLKDISCARD, &range);
#وإلا
		اذهب unimplemented;
#جيڪڏهن_پورو
	} وإلا {
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(معلومات->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(بلاڪ) * channel->بلاڪ_سائيز + معلومات->وٿي,
				(off_t)(ڳڻپ) * channel->بلاڪ_سائيز);
#وإلا
		اذهب unimplemented;
#جيڪڏهن_پورو
	}
	إذا (ret < 0) {
		إذا (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			اذهب unimplemented;
		}
		أرجع errno;
	}
	أرجع 0;
unimplemented:
	أرجع EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ساكن صحيح __unix_zeroout(صحيح fd, off_t وٿي, off_t len)
{
	صحيح ret = -1;

#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, وٿي, len);
	إذا (ret == 0)
		أرجع 0;
#جيڪڏهن_پورو
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			وٿي,  len);
	إذا (ret == 0)
		أرجع 0;
#جيڪڏهن_پورو
	errno = EOPNOTSUPP;
	أرجع ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڌڪو
#هدايت GCC diagnostic ignored "-Wunused-parameter"
#جيڪڏهن_پورو
ساكن errcode_t unix_zeroout(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية unix_ذاتي_معلومات *معلومات;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!(channel->flags & CHANNEL_FLAGS_بلاڪ_DEVICE)) {
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
	}

	إذا (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(معلومات->dev,
			(off_t)(بلاڪ) * channel->بلاڪ_سائيز + معلومات->وٿي,
			(off_t)(ڳڻپ) * channel->بلاڪ_سائيز);
err:
	إذا (ret < 0) {
		إذا (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			اذهب unimplemented;
		}
		أرجع errno;
	}
	أرجع 0;
unimplemented:
	أرجع EXT2_ET_UNIMPLEMENTED;
}
#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو

ساكن بنية struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.سيٽ_blksize	= unix_سيٽ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سيٽ_option	= unix_سيٽ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unix_io_manager = &struct_unix_manager;

ساكن بنية struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.open		= unixfd_open,
	.close		= unix_close,
	.سيٽ_blksize	= unix_سيٽ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سيٽ_option	= unix_سيٽ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
