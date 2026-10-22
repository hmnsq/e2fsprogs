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
#تعریف_کرو XOPEN_سروت 600
#تعریف_کرو DARWIN_C_سروت
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
#جے_تعریف_شدہ __linux__
#ضمن <sys/utsname.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#جے_انت
#إذا HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#جے_انت
#جے_تعریف_شدہ HAVE_PTHREAD
#ضمن <pthread.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#جے_انت

#إذا defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#تعریف_کرو BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#جے_انت

#تعریف_رد ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح لبھو_آخری_ثنائی_ہندسہ_سیٹ(صحيح x)
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
	أرجع لبھو_آخری_ثنائی_ہندسہ_سیٹ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

بنية unix_cache {
	حرف			*buf;
	بنانشانی لما لما	بلاک;
	صحيح			access_ویلا;
	بنانشانی		dirty:1;
	بنانشانی		وچ_use:1;
	بنانشانی		write_err:1;
};

#تعریف_کرو DEFAULT_CACHE_ناپ 8
#تعریف_کرو WRITE_سدھا_ناپ 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کرو READ_سدھا_ناپ 4	/* Should be smaller than CACHE_SIZE */

بنية unix_نجی_انکڑے {
	صحيح	magic;
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_ویلا;
	صحيح	unix_flock_flags;
	ext2_loff_t وتھ;
	بنية unix_cache *cache;
	بنانشانی صحيح cache_ناپ;
	بنانشانی صحيح cache_hash_کھسکاؤ;
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
#جے_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#جے_انت
};

#تعریف_کرو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعريفنوع تعداد جندرا_قسم {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} قسم_t;

#جے_تعریف_شدہ HAVE_PTHREAD
ساكن مضمن pthread_mutex_t *get_mutex(بنية unix_نجی_انکڑے *انکڑے,
					 قسم_t قسم)
{
	إذا (انکڑے->flags & IO_FLAG_دھاگے) {
		بدل (قسم) {
		حالة CACHE_MTX:
			أرجع &انکڑے->cache_mutex;
		حالة BOUNCE_MTX:
			أرجع &انکڑے->bounce_mutex;
		حالة STATS_MTX:
			أرجع &انکڑے->stats_mutex;
		}
	}
	أرجع عدم;
}
#جے_انت

ساكن مضمن فراغ mutex_جندرا(بنية unix_نجی_انکڑے *انکڑے, قسم_t قسم)
{
#جے_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(انکڑے,قسم);

	إذا (mtx)
		pthread_mutex_جندرا(mtx);
#جے_انت
}

ساكن مضمن فراغ mutex_unlock(بنية unix_نجی_انکڑے *انکڑے, قسم_t قسم)
{
#جے_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(انکڑے,قسم);

	إذا (mtx)
		pthread_mutex_unlock(mtx);
#جے_انت
}

ساكن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية unix_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (stats) {
		mutex_جندرا(انکڑے, STATS_MTX);
		*stats = &انکڑے->io_stats;
		mutex_unlock(انکڑے, STATS_MTX);
	}

	أرجع retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_نجی_انکڑے *انکڑے,
			      بنانشانی لما لما بلاک,
			      صحيح گنتی, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		ناپ;
	ext2_loff_t	location;
	صحيح		actual = 0;
	بنانشانی حرف	*buf = bufv;
	ssize_t		really_read = 0;
	بنانشانی لما لما aligned_blk;
	صحيح		align_ناپ, وتھ;

	ناپ = (گنتی < 0) ? -گنتی : (ext2_loff_t) گنتی * channel->بلاک_ناپ;
	mutex_جندرا(انکڑے, STATS_MTX);
	انکڑے->io_stats.بائٹاں_read += ناپ;
	mutex_unlock(انکڑے, STATS_MTX);
	location = ((ext2_loff_t) بلاک * channel->بلاک_ناپ) + انکڑے->وتھ;

	إذا (انکڑے->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#جے_تعریف_شدہ HAVE_PREAD64
	/* Try an aligned pread */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ناپ, channel->align))) {
		actual = pread64(انکڑے->dev, buf, ناپ, location);
		إذا (actual == ناپ)
			أرجع 0;
		actual = 0;
	}
#وإلاإذا HAVE_PREAD
	/* Try an aligned pread */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ناپ, channel->align)))) {
		actual = pread(انکڑے->dev, buf, ناپ, location);
		إذا (actual == ناپ)
			أرجع 0;
		actual = 0;
	}
#جے_انت /* HAVE_PREAD */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ناپ, channel->align))) {
		mutex_جندرا(انکڑے, BOUNCE_MTX);
		إذا (ext2fs_llseek(انکڑے->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(انکڑے->dev, buf, ناپ);
		إذا (actual != ناپ) {
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

#جے_تعریف_شدہ ALIGN_DEBUG
	لکھو("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بنانشانی لما) ناپ);
#جے_انت

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاک_ناپ > channel->align) &&
	    (channel->بلاک_ناپ % channel->align) == 0)
		align_ناپ = channel->بلاک_ناپ;
	وإلا
		align_ناپ = channel->align;
	aligned_blk = location / align_ناپ;
	وتھ = location % align_ناپ;

	mutex_جندرا(انکڑے, BOUNCE_MTX);
	إذا (ext2fs_llseek(انکڑے->dev, aligned_blk * align_ناپ, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	طالما (ناپ > 0) {
		actual = اقرأ(انکڑے->dev, انکڑے->bounce, align_ناپ);
		إذا (actual != align_ناپ) {
			actual = really_read;
			buf -= really_read;
			ناپ += really_read;
			اذهب short_read;
		}
		إذا ((actual + وتھ) > align_ناپ)
			actual = align_ناپ - وتھ;
		إذا (actual > ناپ)
			actual = ناپ;
		حافظہ_نقل(buf, (حرف *)انکڑے->bounce + وتھ, actual);

		really_read += actual;
		ناپ -= actual;
		buf += actual;
		وتھ = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(انکڑے, BOUNCE_MTX);
	أرجع 0;

error_unlock:
	mutex_unlock(انکڑے, BOUNCE_MTX);
	إذا (actual >= 0 && actual < ناپ)
		memset((حرف *) buf+actual, 0, ناپ-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, گنتی, buf,
					       ناپ, actual, retval);
	أرجع retval;
}

#تعریف_کرو RAW_WRITE_NO_HANDLER	(1U << 0)
#تعریف_کرو RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_نجی_انکڑے *انکڑے,
			       بنانشانی لما لما بلاک,
			       صحيح گنتی, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		ناپ;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت بنانشانی حرف *buf = bufv;
	بنانشانی لما لما aligned_blk;
	صحيح		align_ناپ, وتھ;

	إذا (گنتی == 1)
		ناپ = channel->بلاک_ناپ;
	وإلا {
		إذا (گنتی < 0)
			ناپ = -گنتی;
		وإلا
			ناپ = (ext2_loff_t) گنتی * channel->بلاک_ناپ;
	}
	mutex_جندرا(انکڑے, STATS_MTX);
	انکڑے->io_stats.بائٹاں_written += ناپ;
	mutex_unlock(انکڑے, STATS_MTX);

	location = ((ext2_loff_t) بلاک * channel->بلاک_ناپ) + انکڑے->وتھ;

	إذا (انکڑے->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#جے_تعریف_شدہ HAVE_PWRITE64
	/* Try an aligned pwrite */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ناپ, channel->align))) {
		actual = pwrite64(انکڑے->dev, buf, ناپ, location);
		إذا (actual == ناپ)
			أرجع 0;
	}
#وإلاإذا HAVE_PWRITE
	/* Try an aligned pwrite */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(ناپ, channel->align)))) {
		actual = pwrite(انکڑے->dev, buf, ناپ, location);
		إذا (actual == ناپ)
			أرجع 0;
	}
#جے_انت /* HAVE_PWRITE */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(ناپ, channel->align))) {
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_جندرا(انکڑے, BOUNCE_MTX);
		إذا (ext2fs_llseek(انکڑے->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(انکڑے->dev, buf, ناپ);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(انکڑے, BOUNCE_MTX);
		إذا (actual < 0) {
			retval = errno;
			اذهب error_باہرلا;
		}
		إذا (actual != ناپ) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_باہرلا;
		}
		أرجع 0;
	}

#جے_تعریف_شدہ ALIGN_DEBUG
	لکھو("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بنانشانی لما) ناپ);
#جے_انت
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاک_ناپ > channel->align) &&
	    (channel->بلاک_ناپ % channel->align) == 0)
		align_ناپ = channel->بلاک_ناپ;
	وإلا
		align_ناپ = channel->align;
	aligned_blk = location / align_ناپ;
	وتھ = location % align_ناپ;

	طالما (ناپ > 0) {
		صحيح actual_w;

		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_جندرا(انکڑے, BOUNCE_MTX);
		إذا (ناپ < align_ناپ || وتھ) {
			إذا (ext2fs_llseek(انکڑے->dev, aligned_blk * align_ناپ,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(انکڑے->dev, انکڑے->bounce,
				      align_ناپ);
			إذا (actual != align_ناپ) {
				إذا (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) انکڑے->bounce + actual, 0,
				       align_ناپ - actual);
			}
		}
		actual = ناپ;
		إذا ((actual + وتھ) > align_ناپ)
			actual = align_ناپ - وتھ;
		إذا (actual > ناپ)
			actual = ناپ;
		حافظہ_نقل(((حرف *)انکڑے->bounce) + وتھ, buf, actual);
		إذا (ext2fs_llseek(انکڑے->dev, aligned_blk * align_ناپ, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(انکڑے->dev, انکڑے->bounce, align_ناپ);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(انکڑے, BOUNCE_MTX);
		إذا (actual_w < 0) {
			retval = errno;
			اذهب error_باہرلا;
		}
		إذا (actual_w != align_ناپ)
			اذهب short_write;
		ناپ -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		وتھ = 0;
	}
	أرجع 0;

error_unlock:
	إذا (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(انکڑے, BOUNCE_MTX);
error_باہرلا:
	إذا (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, بلاک, گنتی, buf,
						ناپ, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية unix_نجی_انکڑے *انکڑے)
{
	errcode_t		retval;
	بنية unix_cache	*cache;
	بنانشانی صحيح		i;

	انکڑے->access_ویلا = 0;
	لكل (i=0, cache = انکڑے->cache; i < انکڑے->cache_ناپ; i++, cache++) {
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
ساكن فراغ free_cache(بنية unix_نجی_انکڑے *انکڑے)
{
	بنية unix_cache	*cache;
	بنانشانی صحيح		i;

	انکڑے->access_ویلا = 0;
	لكل (i=0, cache = انکڑے->cache; i < انکڑے->cache_ناپ; i++, cache++) {
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

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#تعریف_کرو GOLDEN_تناسب_مفرد	0x9e37fffffffc0001UL
#جے_بنا_تعریف CACHE_سطر_ناپ
/* if the system didn't tell us, guess something reasonable */
#تعریف_کرو CACHE_سطر_ناپ		64
#جے_انت

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن بنانشانی صحيح
cache_hash(بنية unix_نجی_انکڑے *انکڑے, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	إذا (انکڑے->cache_ناپ <= DEFAULT_CACHE_ناپ)
		أرجع 0;

	tmp = hashval ^ (GOLDEN_تناسب_مفرد + hashval) / CACHE_سطر_ناپ;
	tmp = tmp ^ ((tmp ^ GOLDEN_تناسب_مفرد) >> انکڑے->cache_hash_کھسکاؤ);
	أرجع tmp % انکڑے->cache_ناپ;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية unix_cache *لبھو_cached_بلاک(بنية unix_نجی_انکڑے *انکڑے,
					    بنانشانی لما لما بلاک,
					    بنية unix_cache **eldest)
{
	بنية unix_cache	*cache, *بن_ورتیا_cache, *oldest_cache;
	بنانشانی صحيح		hash = cache_hash(انکڑے, بلاک);
	بنانشانی صحيح		i;

	بن_ورتیا_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	لكل (i = hash, cache = انکڑے->cache + hash;
	     i < انکڑے->cache_ناپ;
	     i++, cache++) {
		إذا (!cache->وچ_use) {
			إذا (!بن_ورتیا_cache)
				بن_ورتیا_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_ویلا = ++انکڑے->access_ویلا;
			انکڑے->io_stats.cache_کامیابیاں++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_ویلا < oldest_cache->access_ویلا))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	لكل (i = 0, cache = انکڑے->cache; i < hash; i++, cache++) {
		إذا (!cache->وچ_use) {
			إذا (!بن_ورتیا_cache)
				بن_ورتیا_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_ویلا = ++انکڑے->access_ویلا;
			انکڑے->io_stats.cache_کامیابیاں++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_ویلا < oldest_cache->access_ویلا))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (بن_ورتیا_cache) ? بن_ورتیا_cache : oldest_cache;
	انکڑے->io_stats.cache_کھنجاں++;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_نجی_انکڑے *انکڑے, بنية unix_cache *cache,
		بنانشانی لما لما بلاک)
{
	إذا (cache->dirty && cache->وچ_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, انکڑے, cache->بلاک, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		إذا (retval) {
			cache->write_err = 1;
			أرجع retval;
		}
	}

	cache->وچ_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->بلاک = بلاک;
	cache->access_ویلا = ++انکڑے->access_ویلا;
	أرجع 0;
}

#تعریف_کرو FLUSH_INVALIDATE	0x01
#تعریف_کرو FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاکاں(io_channel channel,
				     بنية unix_نجی_انکڑے *انکڑے,
				     صحيح flags)
{
	بنية unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	بنانشانی صحيح		i;
	صحيح			errors_لبھیا = 0;

	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_جندرا(انکڑے, CACHE_MTX);
	لكل (i=0, cache = انکڑے->cache; i < انکڑے->cache_ناپ; i++, cache++) {
		إذا (!cache->وچ_use)
			استمر;
		إذا (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			إذا (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, انکڑے,
					       cache->بلاک, 1, cache->buf,
					       raw_flags);
			إذا (retval) {
				cache->write_err = 1;
				errors_لبھیا = 1;
				retval2 = retval;
			} وإلا {
				cache->dirty = 0;
				cache->write_err = 0;
				إذا (flags & FLUSH_INVALIDATE)
					cache->وچ_use = 0;
			}
		} وإلا إذا (flags & FLUSH_INVALIDATE) {
			cache->وچ_use = 0;
		}
	}
	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(انکڑے, CACHE_MTX);
retry:
	طالما (errors_لبھیا) {
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_جندرا(انکڑے, CACHE_MTX);
		errors_لبھیا = 0;
		لكل (i=0, cache = انکڑے->cache; i < انکڑے->cache_ناپ; i++, cache++) {
			إذا (!cache->وچ_use || !cache->write_err)
				استمر;
			errors_لبھیا = 1;
			إذا (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				بنانشانی لما لما err_بلاک = cache->بلاک;

				cache->dirty = 0;
				cache->وچ_use = 0;
				cache->write_err = 0;
				إذا (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				وإلا
					حافظہ_نقل(err_buf, cache->buf,
					       channel->بلاک_ناپ);
				mutex_unlock(انکڑے, CACHE_MTX);
				(channel->write_error)(channel, err_بلاک,
					1, err_buf, channel->بلاک_ناپ, -1,
					retval2);
				إذا (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_جندرا(انکڑے, CACHE_MTX);
				اذهب retry;
			} وإلا
				cache->write_err = 0;
		}
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(انکڑے, CACHE_MTX);
	}
	أرجع retval2;
}

/* Shrink the cache buffers */
ساكن errcode_t shrink_cache(io_channel channel,
			      بنية unix_نجی_انکڑے *انکڑے,
			      بنانشانی صحيح new_ناپ)
{
	بنية unix_cache	*cache, *new_cache;
	بنانشانی صحيح		i;
	errcode_t		retval;

	mutex_جندرا(انکڑے, CACHE_MTX);

	retval = flush_cached_بلاکاں(channel, انکڑے,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	لكل (i = new_ناپ, cache = انکڑے->cache + new_ناپ;
	     i < انکڑے->cache_ناپ;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_ویلا = 0;
		cache->dirty = 0;
		cache->وچ_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(انکڑے->cache, new_ناپ * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظہ;
		اذهب unlock;
	}

	انکڑے->cache = new_cache;
	انکڑے->cache_ناپ = new_ناپ;
	انکڑے->cache_hash_کھسکاؤ = highbit32(انکڑے->cache_ناپ);

unlock:
	mutex_unlock(انکڑے, CACHE_MTX);
	أرجع retval;
}

/* Grow the cache buffers */
ساكن errcode_t grow_cache(io_channel channel,
			    بنية unix_نجی_انکڑے *انکڑے,
			    بنانشانی صحيح new_ناپ)
{
	بنية unix_cache	*cache, *new_cache;
	بنانشانی صحيح		i;
	errcode_t		retval;

	mutex_جندرا(انکڑے, CACHE_MTX);

	retval = flush_cached_بلاکاں(channel, انکڑے,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	new_cache = realloc(انکڑے->cache, new_ناپ * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظہ;
		اذهب unlock;
	}
	انکڑے->cache = new_cache;

	لكل (i = انکڑے->cache_ناپ, cache = new_cache + انکڑے->cache_ناپ;
	     i < new_ناپ;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_ویلا = 0;
		cache->dirty = 0;
		cache->وچ_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval) {
			طالما (i > انکڑے->cache_ناپ) {
				cache--; i--;
				إذا (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	انکڑے->cache_ناپ = new_ناپ;
	انکڑے->cache_hash_کھسکاؤ = highbit32(انکڑے->cache_ناپ);

unlock:
	mutex_unlock(انکڑے, CACHE_MTX);
	أرجع retval;
}
#جے_انت /* NO_IO_CACHE */

#جے_تعریف_شدہ __linux__
#جے_بنا_تعریف BLKDISCARDZEROES
#تعریف_کرو BLKDISCARDZEROES _IO(0x12,124)
#جے_انت
#جے_انت

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t منوال)
{
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

#جے_تعریف_شدہ HAVE_SYS_FILE_H
ساكن errcode_t unix_funlock(io_channel channel)
{
	بنية unix_نجی_انکڑے *انکڑے;
	صحيح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (انکڑے->unix_flock_flags) {
		ret = flock(انکڑے->dev, LOCK_UN);
		إذا (ret)
			أرجع errno;

		انکڑے->unix_flock_flags = 0;
	}

	أرجع 0;
}

ساكن errcode_t unix_flock(io_channel channel, بنانشانی صحيح flock_flags)
{
	بنية unix_نجی_انکڑے *انکڑے;
	صحيح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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

	ret = flock(انکڑے->dev, unix_flock_flags);
	إذا (ret < 0)
		أرجع errno;

	انکڑے->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	أرجع 0;
}
#وإلا
#تعریف_کرو unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	أرجع 0;
}
#جے_انت /* HAVE_SYS_FILE_H */

ساكن errcode_t unix_open_channel(ثابت حرف *name, صحيح fd,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	بنية unix_نجی_انکڑے *انکڑے = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;
#جے_تعریف_شدہ __linux__
	بنية		utsname ut;
#جے_انت

	إذا (ext2fs_محفوظ_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#جے_تعریف_شدہ __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#جے_انت

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_نجی_انکڑے), &انکڑے);
	إذا (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->نجی_انکڑے = انکڑے;
	io->بلاک_ناپ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	إذا (ext2fs_محفوظ_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(انکڑے, 0, حجم(بنية unix_نجی_انکڑے));
	انکڑے->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	انکڑے->io_stats.عدد_fields = 4;
	انکڑے->flags = flags;
	انکڑے->dev = fd;

	انکڑے->cache_ناپ = DEFAULT_CACHE_ناپ;
	انکڑے->cache_hash_کھسکاؤ = highbit32(انکڑے->cache_ناپ);
	انکڑے->cache = صفر_حافظہ_دیو(DEFAULT_CACHE_ناپ, حجم(بنية unix_cache));
	إذا (!انکڑے->cache) {
		retval = EXT2_ET_NO_حافظہ;
		اذهب cleanup;
	}

#إذا defined(O_سدھا)
	إذا (flags & IO_FLAG_سدھا_IO)
		io->align = ext2fs_get_dio_alignment(انکڑے->dev);
#وإلاإذا defined(F_NOCACHE)
	إذا (flags & IO_FLAG_سدھا_IO)
		io->align = 4096;
#جے_انت

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(انکڑے->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_منوال)) {
#جے_تعریف_شدہ BLKDISCARDZEROES
			صحيح zeroes = 0;

			إذا (ioctl(انکڑے->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#جے_انت
			io->flags |= CHANNEL_FLAGS_بلاک_DEVICE;
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
#جے_انت

#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	إذا (io->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		إذا (io->align < dio_align)
			io->align = dio_align;
	}
#جے_انت

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

#جے_تعریف_شدہ __linux__
#تعریف_رد RLIM_INFINITY
#إذا (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#تعریف_کرو RLIM_INFINITY	((بنانشانی لما)(~0UL>>1))
#وإلا
#تعریف_کرو RLIM_INFINITY  (~0UL)
#جے_انت
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
	    (ext2fs_fstat(انکڑے->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_منوال))) {
		بنية rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (بنانشانی لما) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		إذا (((بنانشانی لما) rlim.rlim_cur) <
		    ((بنانشانی لما) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#جے_انت
#جے_تعریف_شدہ HAVE_PTHREAD
	إذا (flags & IO_FLAG_دھاگے) {
		io->flags |= CHANNEL_FLAGS_دھاگے;
		retval = pthread_mutex_init(&انکڑے->cache_mutex, عدم);
		إذا (retval)
			اذهب cleanup;
		retval = pthread_mutex_init(&انکڑے->bounce_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&انکڑے->cache_mutex);
			اذهب cleanup;
		}
		retval = pthread_mutex_init(&انکڑے->stats_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&انکڑے->cache_mutex);
			pthread_mutex_destroy(&انکڑے->bounce_mutex);
			اذهب cleanup;
		}
	}
#جے_انت
	*channel = io;
	أرجع 0;

cleanup:
	إذا (انکڑے) {
		unix_funlock(io);
		إذا (io->manager != unixfd_io_manager && انکڑے->dev >= 0)
			close(انکڑے->dev);
		إذا (انکڑے->cache) {
			free_cache(انکڑے);
			حرر(انکڑے->cache);
		}
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
#إذا defined(O_سدھا)
	إذا (fd_flags & O_سدھا)
		flags |= IO_FLAG_سدھا_IO;
#جے_انت
#جے_انت  /* HAVE_FCNTL */

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
#إذا defined(O_سدھا)
	إذا (flags & IO_FLAG_سدھا_IO)
		open_flags |= O_سدھا;
#جے_انت
	fd = ext2fs_open_file(name, open_flags, 0);
	إذا (fd < 0)
		أرجع errno;
#إذا defined(F_NOCACHE) && !defined(IO_سدھا)
	إذا (flags & IO_FLAG_سدھا_IO) {
		إذا (fcntl(fd, F_NOCACHE, 1) < 0)
			أرجع errno;
	}
#جے_انت
	أرجع unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#جے_بنا_تعریف NO_IO_CACHE
	retval = flush_cached_بلاکاں(channel, انکڑے, 0);
#جے_انت

	unix_funlock(channel);

	إذا (channel->manager != unixfd_io_manager && close(انکڑے->dev) < 0)
		retval = errno;
	free_cache(انکڑے);
	حرر(انکڑے->cache);
#جے_تعریف_شدہ HAVE_PTHREAD
	إذا (انکڑے->flags & IO_FLAG_دھاگے) {
		pthread_mutex_destroy(&انکڑے->cache_mutex);
		pthread_mutex_destroy(&انکڑے->bounce_mutex);
		pthread_mutex_destroy(&انکڑے->stats_mutex);
	}
#جے_انت

	ext2fs_free_mem(&channel->نجی_انکڑے);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t unix_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_نجی_انکڑے *انکڑے;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->بلاک_ناپ != blksize) {
		mutex_جندرا(انکڑے, CACHE_MTX);
		mutex_جندرا(انکڑے, BOUNCE_MTX);
#جے_بنا_تعریف NO_IO_CACHE
		إذا ((retval = flush_cached_بلاکاں(channel, انکڑے, FLUSH_NOLOCK))){
			mutex_unlock(انکڑے, BOUNCE_MTX);
			mutex_unlock(انکڑے, CACHE_MTX);
			أرجع retval;
		}
#جے_انت

		channel->بلاک_ناپ = blksize;
		free_cache(انکڑے);
		retval = alloc_cache(channel, انکڑے);
		mutex_unlock(انکڑے, BOUNCE_MTX);
		mutex_unlock(انکڑے, CACHE_MTX);
	}
	أرجع retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, بنانشانی لما لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	بنية unix_نجی_انکڑے *انکڑے;
	بنية unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جے_تعریف_شدہ NO_IO_CACHE
	أرجع raw_read_blk(channel, انکڑے, بلاک, گنتی, buf);
#وإلا
	إذا (انکڑے->flags & IO_FLAG_NOCACHE)
		أرجع raw_read_blk(channel, انکڑے, بلاک, گنتی, buf);
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
	mutex_جندرا(انکڑے, CACHE_MTX);
	طالما (گنتی > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = لبھو_cached_بلاک(انکڑے, بلاک, عدم))) {
#جے_تعریف_شدہ DEBUG
			لکھو("Using cached block %lu\n", بلاک);
#جے_انت
			حافظہ_نقل(cp, cache->buf, channel->بلاک_ناپ);
			گنتی--;
			بلاک++;
			cp += channel->بلاک_ناپ;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < گنتی; i++)
			إذا (لبھو_cached_بلاک(انکڑے, بلاک+i, عدم))
				اكسر;
#جے_تعریف_شدہ DEBUG
		لکھو("Reading %d blocks starting at %lu\n", i, بلاک);
#جے_انت
		mutex_unlock(انکڑے, CACHE_MTX);
		إذا ((retval = raw_read_blk(channel, انکڑے, بلاک, i, cp)))
			أرجع retval;
		mutex_جندرا(انکڑے, CACHE_MTX);

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			إذا (!لبھو_cached_بلاک(انکڑے, بلاک, &cache)) {
				retval = reuse_cache(channel, انکڑے,
						     cache, بلاک);
				إذا (retval)
					اذهب call_write_handler;
				حافظہ_نقل(cache->buf, cp, channel->بلاک_ناپ);
			}
			گنتی--;
			بلاک++;
			cp += channel->بلاک_ناپ;
		}
	}
	mutex_unlock(انکڑے, CACHE_MTX);
	أرجع 0;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بنانشانی لما لما err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->وچ_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			حافظہ_نقل(err_buf, cache->buf, channel->بلاک_ناپ);
		mutex_unlock(انکڑے, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_ناپ, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(انکڑے, CACHE_MTX);
	أرجع retval;
#جے_انت /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	أرجع unix_read_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, بنانشانی لما لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	بنية unix_نجی_انکڑے *انکڑے;
	بنية unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جے_تعریف_شدہ NO_IO_CACHE
	أرجع raw_write_blk(channel, انکڑے, بلاک, گنتی, buf, 0);
#وإلا
	إذا (انکڑے->flags & IO_FLAG_NOCACHE)
		أرجع raw_write_blk(channel, انکڑے, بلاک, گنتی, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (گنتی < 0 || گنتی > WRITE_سدھا_ناپ) {
		إذا ((retval = flush_cached_بلاکاں(channel, انکڑے,
						  FLUSH_INVALIDATE)))
			أرجع retval;
		أرجع raw_write_blk(channel, انکڑے, بلاک, گنتی, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, انکڑے, بلاک, گنتی, buf, 0);

	cp = buf;
	mutex_جندرا(انکڑے, CACHE_MTX);
	طالما (گنتی > 0) {
		cache = لبھو_cached_بلاک(انکڑے, بلاک, &reuse);
		إذا (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, انکڑے, cache, بلاک);
			إذا (err)
				اذهب call_write_handler;
		}
		إذا (cache->buf != cp)
			حافظہ_نقل(cache->buf, cp, channel->بلاک_ناپ);
		cache->dirty = !writethrough;
		گنتی--;
		بلاک++;
		cp += channel->بلاک_ناپ;
	}
	mutex_unlock(انکڑے, CACHE_MTX);
	أرجع retval;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بنانشانی لما لما err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->وچ_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			حافظہ_نقل(err_buf, cache->buf, channel->بلاک_ناپ);
		mutex_unlock(انکڑے, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_ناپ, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(انکڑے, CACHE_MTX);
	أرجع retval;
#جے_انت /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      بنانشانی لما لما بلاک,
				      بنانشانی لما لما گنتی)
{
#جے_تعریف_شدہ POSIX_FADV_WILLNEED
	بنية unix_نجی_انکڑے *انکڑے;

	انکڑے = (بنية unix_نجی_انکڑے *)channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	أرجع posix_fadvise(انکڑے->dev,
			     (ext2_loff_t)بلاک * channel->بلاک_ناپ + انکڑے->وتھ,
			     (ext2_loff_t)گنتی * channel->بلاک_ناپ,
			     POSIX_FADV_WILLNEED);
#وإلا
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
#جے_انت
}

ساكن errcode_t unix_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	أرجع unix_write_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t unix_write_byte(io_channel channel, بنانشانی لما وتھ,
				 صحيح ناپ, ثابت فراغ *buf)
{
	بنية unix_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->align != 0) {
#جے_تعریف_شدہ ALIGN_DEBUG
		لکھو("unix_write_byte: O_DIRECT fallback\n");
#جے_انت
		أرجع EXT2_ET_UNIMPLEMENTED;
	}

#جے_بنا_تعریف NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	إذا ((retval = flush_cached_بلاکاں(channel, انکڑے, FLUSH_INVALIDATE)))
		أرجع retval;
#جے_انت

	إذا (lseek(انکڑے->dev, وتھ + انکڑے->وتھ, SEEK_SET) < 0)
		أرجع errno;

	actual = اكتب(انکڑے->dev, buf, ناپ);
	إذا (actual < 0)
		أرجع errno;
	إذا (actual != ناپ)
		أرجع EXT2_ET_SHORT_WRITE;

	أرجع 0;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t unix_flush(io_channel channel)
{
	بنية unix_نجی_انکڑے *انکڑے;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#جے_بنا_تعریف NO_IO_CACHE
	retval = flush_cached_بلاکاں(channel, انکڑے, 0);
#جے_انت
#جے_تعریف_شدہ HAVE_FSYNC
	إذا (!retval && fsync(انکڑے->dev) != 0)
		أرجع errno;
#جے_انت
	أرجع retval;
}

ساكن errcode_t unix_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية unix_نجی_انکڑے *انکڑے;
	بنانشانی لما لما tmp;
	errcode_t retval;
	حرف *سرا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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
	إذا (!strcmp(option, "cache")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (!strcmp(arg, "on")) {
			انکڑے->flags &= ~IO_FLAG_NOCACHE;
			أرجع 0;
		}
		إذا (!strcmp(arg, "off")) {
			retval = flush_cached_بلاکاں(channel, انکڑے, 0);
			انکڑے->flags |= IO_FLAG_NOCACHE;
			أرجع retval;
		}
		أرجع EXT2_ET_INVALID_دلیل;
	}
#جے_بنا_تعریف NO_IO_CACHE
	إذا (!strcmp(option, "cache_blocks")) {
		بنانشانی لما لما	ناپ;

		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		errno = 0;
		ناپ = strtoll(arg, عدم, 0);
		إذا (errno || ناپ == 0 || ناپ > INT32_MAX)
			أرجع EXT2_ET_INVALID_دلیل;

		إذا (انکڑے->cache_ناپ == ناپ)
			أرجع 0;
		إذا (انکڑے->cache_ناپ > ناپ)
			أرجع shrink_cache(channel, انکڑے, ناپ);
		أرجع grow_cache(channel, انکڑے, ناپ);
	}
#جے_انت
	أرجع EXT2_ET_INVALID_دلیل;
}

#إذا defined(__linux__) && !defined(BLKDISCARD)
#تعریف_کرو BLKDISCARD		_IO(0x12,119)
#جے_انت

ساكن errcode_t unix_discard(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية unix_نجی_انکڑے *انکڑے;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	إذا (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
#جے_تعریف_شدہ BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(بلاک) * channel->بلاک_ناپ + انکڑے->وتھ;
		range[1] = (__u64)(گنتی) * channel->بلاک_ناپ;

		ret = ioctl(انکڑے->dev, BLKDISCARD, &range);
#وإلا
		اذهب unimplemented;
#جے_انت
	} وإلا {
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(انکڑے->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(بلاک) * channel->بلاک_ناپ + انکڑے->وتھ,
				(off_t)(گنتی) * channel->بلاک_ناپ);
#وإلا
		اذهب unimplemented;
#جے_انت
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
ساكن صحيح __unix_zeroout(صحيح fd, off_t وتھ, off_t len)
{
	صحيح ret = -1;

#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, وتھ, len);
	إذا (ret == 0)
		أرجع 0;
#جے_انت
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			وتھ,  len);
	إذا (ret == 0)
		أرجع 0;
#جے_انت
	errno = EOPNOTSUPP;
	أرجع ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wunused-parameter"
#جے_انت
ساكن errcode_t unix_zeroout(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية unix_نجی_انکڑے *انکڑے;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية unix_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!(channel->flags & CHANNEL_FLAGS_بلاک_DEVICE)) {
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
	}

	إذا (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(انکڑے->dev,
			(off_t)(بلاک) * channel->بلاک_ناپ + انکڑے->وتھ,
			(off_t)(گنتی) * channel->بلاک_ناپ);
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
#ہدایت GCC diagnostic نکالو
#جے_انت

ساكن بنية struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.سیٹ_blksize	= unix_سیٹ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سیٹ_option	= unix_سیٹ_option,
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
	.سیٹ_blksize	= unix_سیٹ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سیٹ_option	= unix_سیٹ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
