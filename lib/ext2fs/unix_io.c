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
#تعریف_کړه XOPEN_سرچینه 600
#تعریف_کړه DARWIN_C_سرچینه
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
#که_تعریف_شوی __linux__
#ضمن <sys/utsname.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#د_که_پای
#إذا HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#د_که_پای
#که_تعریف_شوی HAVE_PTHREAD
#ضمن <pthread.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#د_که_پای

#إذا defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#تعریف_کړه BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#د_که_پای

#تعریف_لغوه ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح پیدا_کړه_وروستی_بټ_سېټ(صحيح x)
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
	أرجع پیدا_کړه_وروستی_بټ_سېټ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)

بنية unix_cache {
	حرف			*buf;
	بینښه اوږد اوږد	بلاک;
	صحيح			access_وخت;
	بینښه		dirty:1;
	بینښه		دننه_use:1;
	بینښه		write_err:1;
};

#تعریف_کړه DEFAULT_CACHE_اندازه 8
#تعریف_کړه WRITE_مستقیم_اندازه 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کړه READ_مستقیم_اندازه 4	/* Should be smaller than CACHE_SIZE */

بنية unix_شخصي_معلومات {
	صحيح	magic;
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_وخت;
	صحيح	unix_flock_flags;
	ext2_loff_t دننه_واټن;
	بنية unix_cache *cache;
	بینښه صحيح cache_اندازه;
	بینښه صحيح cache_hash_ښويدنه;
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
#که_تعریف_شوی HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#د_که_پای
};

#تعریف_کړه IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعريفنوع تعداد قلف_ډول {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} ډول_t;

#که_تعریف_شوی HAVE_PTHREAD
ساكن مضمن pthread_mutex_t *get_mutex(بنية unix_شخصي_معلومات *معلومات,
					 ډول_t ډول)
{
	إذا (معلومات->flags & IO_FLAG_تارونه) {
		بدل (ډول) {
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
#د_که_پای

ساكن مضمن فراغ mutex_قلف(بنية unix_شخصي_معلومات *معلومات, ډول_t ډول)
{
#که_تعریف_شوی HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(معلومات,ډول);

	إذا (mtx)
		pthread_mutex_قلف(mtx);
#د_که_پای
}

ساكن مضمن فراغ mutex_unlock(بنية unix_شخصي_معلومات *معلومات, ډول_t ډول)
{
#که_تعریف_شوی HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(معلومات,ډول);

	إذا (mtx)
		pthread_mutex_unlock(mtx);
#د_که_پای
}

ساكن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية unix_شخصي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (stats) {
		mutex_قلف(معلومات, STATS_MTX);
		*stats = &معلومات->io_stats;
		mutex_unlock(معلومات, STATS_MTX);
	}

	أرجع retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_شخصي_معلومات *معلومات,
			      بینښه اوږد اوږد بلاک,
			      صحيح شمېر, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		اندازه;
	ext2_loff_t	location;
	صحيح		actual = 0;
	بینښه حرف	*buf = bufv;
	ssize_t		really_read = 0;
	بینښه اوږد اوږد aligned_blk;
	صحيح		align_اندازه, دننه_واټن;

	اندازه = (شمېر < 0) ? -شمېر : (ext2_loff_t) شمېر * channel->بلاک_اندازه;
	mutex_قلف(معلومات, STATS_MTX);
	معلومات->io_stats.بایټونه_read += اندازه;
	mutex_unlock(معلومات, STATS_MTX);
	location = ((ext2_loff_t) بلاک * channel->بلاک_اندازه) + معلومات->دننه_واټن;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#که_تعریف_شوی HAVE_PREAD64
	/* Try an aligned pread */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		actual = pread64(معلومات->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
		actual = 0;
	}
#وإلاإذا HAVE_PREAD
	/* Try an aligned pread */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(اندازه, channel->align)))) {
		actual = pread(معلومات->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
		actual = 0;
	}
#د_که_پای /* HAVE_PREAD */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		mutex_قلف(معلومات, BOUNCE_MTX);
		إذا (ext2fs_llseek(معلومات->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(معلومات->dev, buf, اندازه);
		إذا (actual != اندازه) {
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

#که_تعریف_شوی ALIGN_DEBUG
	چاپ_کړه("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بینښه اوږد) اندازه);
#د_که_پای

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاک_اندازه > channel->align) &&
	    (channel->بلاک_اندازه % channel->align) == 0)
		align_اندازه = channel->بلاک_اندازه;
	وإلا
		align_اندازه = channel->align;
	aligned_blk = location / align_اندازه;
	دننه_واټن = location % align_اندازه;

	mutex_قلف(معلومات, BOUNCE_MTX);
	إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_اندازه, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	طالما (اندازه > 0) {
		actual = اقرأ(معلومات->dev, معلومات->bounce, align_اندازه);
		إذا (actual != align_اندازه) {
			actual = really_read;
			buf -= really_read;
			اندازه += really_read;
			اذهب short_read;
		}
		إذا ((actual + دننه_واټن) > align_اندازه)
			actual = align_اندازه - دننه_واټن;
		إذا (actual > اندازه)
			actual = اندازه;
		حافظه_نقل(buf, (حرف *)معلومات->bounce + دننه_واټن, actual);

		really_read += actual;
		اندازه -= actual;
		buf += actual;
		دننه_واټن = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(معلومات, BOUNCE_MTX);
	أرجع 0;

error_unlock:
	mutex_unlock(معلومات, BOUNCE_MTX);
	إذا (actual >= 0 && actual < اندازه)
		memset((حرف *) buf+actual, 0, اندازه-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, شمېر, buf,
					       اندازه, actual, retval);
	أرجع retval;
}

#تعریف_کړه RAW_WRITE_NO_HANDLER	(1U << 0)
#تعریف_کړه RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_شخصي_معلومات *معلومات,
			       بینښه اوږد اوږد بلاک,
			       صحيح شمېر, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		اندازه;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت بینښه حرف *buf = bufv;
	بینښه اوږد اوږد aligned_blk;
	صحيح		align_اندازه, دننه_واټن;

	إذا (شمېر == 1)
		اندازه = channel->بلاک_اندازه;
	وإلا {
		إذا (شمېر < 0)
			اندازه = -شمېر;
		وإلا
			اندازه = (ext2_loff_t) شمېر * channel->بلاک_اندازه;
	}
	mutex_قلف(معلومات, STATS_MTX);
	معلومات->io_stats.بایټونه_written += اندازه;
	mutex_unlock(معلومات, STATS_MTX);

	location = ((ext2_loff_t) بلاک * channel->بلاک_اندازه) + معلومات->دننه_واټن;

	إذا (معلومات->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#که_تعریف_شوی HAVE_PWRITE64
	/* Try an aligned pwrite */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		actual = pwrite64(معلومات->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
	}
#وإلاإذا HAVE_PWRITE
	/* Try an aligned pwrite */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(اندازه, channel->align)))) {
		actual = pwrite(معلومات->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
	}
#د_که_پای /* HAVE_PWRITE */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_قلف(معلومات, BOUNCE_MTX);
		إذا (ext2fs_llseek(معلومات->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(معلومات->dev, buf, اندازه);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(معلومات, BOUNCE_MTX);
		إذا (actual < 0) {
			retval = errno;
			اذهب error_بهر;
		}
		إذا (actual != اندازه) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_بهر;
		}
		أرجع 0;
	}

#که_تعریف_شوی ALIGN_DEBUG
	چاپ_کړه("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بینښه اوږد) اندازه);
#د_که_پای
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلاک_اندازه > channel->align) &&
	    (channel->بلاک_اندازه % channel->align) == 0)
		align_اندازه = channel->بلاک_اندازه;
	وإلا
		align_اندازه = channel->align;
	aligned_blk = location / align_اندازه;
	دننه_واټن = location % align_اندازه;

	طالما (اندازه > 0) {
		صحيح actual_w;

		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_قلف(معلومات, BOUNCE_MTX);
		إذا (اندازه < align_اندازه || دننه_واټن) {
			إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_اندازه,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(معلومات->dev, معلومات->bounce,
				      align_اندازه);
			إذا (actual != align_اندازه) {
				إذا (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) معلومات->bounce + actual, 0,
				       align_اندازه - actual);
			}
		}
		actual = اندازه;
		إذا ((actual + دننه_واټن) > align_اندازه)
			actual = align_اندازه - دننه_واټن;
		إذا (actual > اندازه)
			actual = اندازه;
		حافظه_نقل(((حرف *)معلومات->bounce) + دننه_واټن, buf, actual);
		إذا (ext2fs_llseek(معلومات->dev, aligned_blk * align_اندازه, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(معلومات->dev, معلومات->bounce, align_اندازه);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(معلومات, BOUNCE_MTX);
		إذا (actual_w < 0) {
			retval = errno;
			اذهب error_بهر;
		}
		إذا (actual_w != align_اندازه)
			اذهب short_write;
		اندازه -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		دننه_واټن = 0;
	}
	أرجع 0;

error_unlock:
	إذا (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(معلومات, BOUNCE_MTX);
error_بهر:
	إذا (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, بلاک, شمېر, buf,
						اندازه, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية unix_شخصي_معلومات *معلومات)
{
	errcode_t		retval;
	بنية unix_cache	*cache;
	بینښه صحيح		i;

	معلومات->access_وخت = 0;
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_اندازه; i++, cache++) {
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
ساكن فراغ free_cache(بنية unix_شخصي_معلومات *معلومات)
{
	بنية unix_cache	*cache;
	بینښه صحيح		i;

	معلومات->access_وخت = 0;
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_اندازه; i++, cache++) {
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

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#تعریف_کړه GOLDEN_نسبت_اولي	0x9e37fffffffc0001UL
#که_نه_تعریف_شوی CACHE_کرښه_اندازه
/* if the system didn't tell us, guess something reasonable */
#تعریف_کړه CACHE_کرښه_اندازه		64
#د_که_پای

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن بینښه صحيح
cache_hash(بنية unix_شخصي_معلومات *معلومات, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	إذا (معلومات->cache_اندازه <= DEFAULT_CACHE_اندازه)
		أرجع 0;

	tmp = hashval ^ (GOLDEN_نسبت_اولي + hashval) / CACHE_کرښه_اندازه;
	tmp = tmp ^ ((tmp ^ GOLDEN_نسبت_اولي) >> معلومات->cache_hash_ښويدنه);
	أرجع tmp % معلومات->cache_اندازه;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية unix_cache *پیدا_کړه_cached_بلاک(بنية unix_شخصي_معلومات *معلومات,
					    بینښه اوږد اوږد بلاک,
					    بنية unix_cache **eldest)
{
	بنية unix_cache	*cache, *ناکارول_شوی_cache, *oldest_cache;
	بینښه صحيح		hash = cache_hash(معلومات, بلاک);
	بینښه صحيح		i;

	ناکارول_شوی_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	لكل (i = hash, cache = معلومات->cache + hash;
	     i < معلومات->cache_اندازه;
	     i++, cache++) {
		إذا (!cache->دننه_use) {
			إذا (!ناکارول_شوی_cache)
				ناکارول_شوی_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_وخت = ++معلومات->access_وخت;
			معلومات->io_stats.cache_موندنې++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_وخت < oldest_cache->access_وخت))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	لكل (i = 0, cache = معلومات->cache; i < hash; i++, cache++) {
		إذا (!cache->دننه_use) {
			إذا (!ناکارول_شوی_cache)
				ناکارول_شوی_cache = cache;
			استمر;
		}
		إذا (cache->بلاک == بلاک) {
			cache->access_وخت = ++معلومات->access_وخت;
			معلومات->io_stats.cache_موندنې++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_وخت < oldest_cache->access_وخت))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (ناکارول_شوی_cache) ? ناکارول_شوی_cache : oldest_cache;
	معلومات->io_stats.cache_ناکامۍ++;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_شخصي_معلومات *معلومات, بنية unix_cache *cache,
		بینښه اوږد اوږد بلاک)
{
	إذا (cache->dirty && cache->دننه_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, معلومات, cache->بلاک, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		إذا (retval) {
			cache->write_err = 1;
			أرجع retval;
		}
	}

	cache->دننه_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->بلاک = بلاک;
	cache->access_وخت = ++معلومات->access_وخت;
	أرجع 0;
}

#تعریف_کړه FLUSH_INVALIDATE	0x01
#تعریف_کړه FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلاکونه(io_channel channel,
				     بنية unix_شخصي_معلومات *معلومات,
				     صحيح flags)
{
	بنية unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	بینښه صحيح		i;
	صحيح			errors_وموندل = 0;

	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_قلف(معلومات, CACHE_MTX);
	لكل (i=0, cache = معلومات->cache; i < معلومات->cache_اندازه; i++, cache++) {
		إذا (!cache->دننه_use)
			استمر;
		إذا (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			إذا (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, معلومات,
					       cache->بلاک, 1, cache->buf,
					       raw_flags);
			إذا (retval) {
				cache->write_err = 1;
				errors_وموندل = 1;
				retval2 = retval;
			} وإلا {
				cache->dirty = 0;
				cache->write_err = 0;
				إذا (flags & FLUSH_INVALIDATE)
					cache->دننه_use = 0;
			}
		} وإلا إذا (flags & FLUSH_INVALIDATE) {
			cache->دننه_use = 0;
		}
	}
	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(معلومات, CACHE_MTX);
retry:
	طالما (errors_وموندل) {
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_قلف(معلومات, CACHE_MTX);
		errors_وموندل = 0;
		لكل (i=0, cache = معلومات->cache; i < معلومات->cache_اندازه; i++, cache++) {
			إذا (!cache->دننه_use || !cache->write_err)
				استمر;
			errors_وموندل = 1;
			إذا (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				بینښه اوږد اوږد err_بلاک = cache->بلاک;

				cache->dirty = 0;
				cache->دننه_use = 0;
				cache->write_err = 0;
				إذا (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				وإلا
					حافظه_نقل(err_buf, cache->buf,
					       channel->بلاک_اندازه);
				mutex_unlock(معلومات, CACHE_MTX);
				(channel->write_error)(channel, err_بلاک,
					1, err_buf, channel->بلاک_اندازه, -1,
					retval2);
				إذا (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_قلف(معلومات, CACHE_MTX);
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
			      بنية unix_شخصي_معلومات *معلومات,
			      بینښه صحيح new_اندازه)
{
	بنية unix_cache	*cache, *new_cache;
	بینښه صحيح		i;
	errcode_t		retval;

	mutex_قلف(معلومات, CACHE_MTX);

	retval = flush_cached_بلاکونه(channel, معلومات,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	لكل (i = new_اندازه, cache = معلومات->cache + new_اندازه;
	     i < معلومات->cache_اندازه;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_وخت = 0;
		cache->dirty = 0;
		cache->دننه_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(معلومات->cache, new_اندازه * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب unlock;
	}

	معلومات->cache = new_cache;
	معلومات->cache_اندازه = new_اندازه;
	معلومات->cache_hash_ښويدنه = highbit32(معلومات->cache_اندازه);

unlock:
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
}

/* Grow the cache buffers */
ساكن errcode_t grow_cache(io_channel channel,
			    بنية unix_شخصي_معلومات *معلومات,
			    بینښه صحيح new_اندازه)
{
	بنية unix_cache	*cache, *new_cache;
	بینښه صحيح		i;
	errcode_t		retval;

	mutex_قلف(معلومات, CACHE_MTX);

	retval = flush_cached_بلاکونه(channel, معلومات,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	new_cache = realloc(معلومات->cache, new_اندازه * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب unlock;
	}
	معلومات->cache = new_cache;

	لكل (i = معلومات->cache_اندازه, cache = new_cache + معلومات->cache_اندازه;
	     i < new_اندازه;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_وخت = 0;
		cache->dirty = 0;
		cache->دننه_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval) {
			طالما (i > معلومات->cache_اندازه) {
				cache--; i--;
				إذا (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	معلومات->cache_اندازه = new_اندازه;
	معلومات->cache_hash_ښويدنه = highbit32(معلومات->cache_اندازه);

unlock:
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
}
#د_که_پای /* NO_IO_CACHE */

#که_تعریف_شوی __linux__
#که_نه_تعریف_شوی BLKDISCARDZEROES
#تعریف_کړه BLKDISCARDZEROES _IO(0x12,124)
#د_که_پای
#د_که_پای

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t ډېرپېښ)
{
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

#که_تعریف_شوی HAVE_SYS_FILE_H
ساكن errcode_t unix_funlock(io_channel channel)
{
	بنية unix_شخصي_معلومات *معلومات;
	صحيح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->unix_flock_flags) {
		ret = flock(معلومات->dev, LOCK_UN);
		إذا (ret)
			أرجع errno;

		معلومات->unix_flock_flags = 0;
	}

	أرجع 0;
}

ساكن errcode_t unix_flock(io_channel channel, بینښه صحيح flock_flags)
{
	بنية unix_شخصي_معلومات *معلومات;
	صحيح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
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
#تعریف_کړه unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	أرجع 0;
}
#د_که_پای /* HAVE_SYS_FILE_H */

ساكن errcode_t unix_open_channel(ثابت حرف *name, صحيح fd,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	بنية unix_شخصي_معلومات *معلومات = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;
#که_تعریف_شوی __linux__
	بنية		utsname ut;
#د_که_پای

	إذا (ext2fs_خوندي_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#که_تعریف_شوی __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#د_که_پای

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_شخصي_معلومات), &معلومات);
	إذا (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(مزي_اوږدوالی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->شخصي_معلومات = معلومات;
	io->بلاک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	إذا (ext2fs_خوندي_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(معلومات, 0, حجم(بنية unix_شخصي_معلومات));
	معلومات->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	معلومات->io_stats.عدد_fields = 4;
	معلومات->flags = flags;
	معلومات->dev = fd;

	معلومات->cache_اندازه = DEFAULT_CACHE_اندازه;
	معلومات->cache_hash_ښويدنه = highbit32(معلومات->cache_اندازه);
	معلومات->cache = حافظه_صفر_ورکړه(DEFAULT_CACHE_اندازه, حجم(بنية unix_cache));
	إذا (!معلومات->cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب cleanup;
	}

#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = ext2fs_get_dio_alignment(معلومات->dev);
#وإلاإذا defined(F_NOCACHE)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = 4096;
#د_که_پای

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(معلومات->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_ډېرپېښ)) {
#که_تعریف_شوی BLKDISCARDZEROES
			صحيح zeroes = 0;

			إذا (ioctl(معلومات->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#د_که_پای
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
#د_که_پای

#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	إذا (io->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		إذا (io->align < dio_align)
			io->align = dio_align;
	}
#د_که_پای

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

#که_تعریف_شوی __linux__
#تعریف_لغوه RLIM_INFINITY
#إذا (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#تعریف_کړه RLIM_INFINITY	((بینښه اوږد)(~0UL>>1))
#وإلا
#تعریف_کړه RLIM_INFINITY  (~0UL)
#د_که_پای
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
	    (ext2fsP_is_disk_device(st.st_ډېرپېښ))) {
		بنية rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (بینښه اوږد) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		إذا (((بینښه اوږد) rlim.rlim_cur) <
		    ((بینښه اوږد) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#د_که_پای
#که_تعریف_شوی HAVE_PTHREAD
	إذا (flags & IO_FLAG_تارونه) {
		io->flags |= CHANNEL_FLAGS_تارونه;
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
#د_که_پای
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
#إذا defined(O_مستقیم)
	إذا (fd_flags & O_مستقیم)
		flags |= IO_FLAG_مستقیم_IO;
#د_که_پای
#د_که_پای  /* HAVE_FCNTL */

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
#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		open_flags |= O_مستقیم;
#د_که_پای
	fd = ext2fs_open_file(name, open_flags, 0);
	إذا (fd < 0)
		أرجع errno;
#إذا defined(F_NOCACHE) && !defined(IO_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO) {
		إذا (fcntl(fd, F_NOCACHE, 1) < 0)
			أرجع errno;
	}
#د_که_پای
	أرجع unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#که_نه_تعریف_شوی NO_IO_CACHE
	retval = flush_cached_بلاکونه(channel, معلومات, 0);
#د_که_پای

	unix_funlock(channel);

	إذا (channel->manager != unixfd_io_manager && close(معلومات->dev) < 0)
		retval = errno;
	free_cache(معلومات);
	حرر(معلومات->cache);
#که_تعریف_شوی HAVE_PTHREAD
	إذا (معلومات->flags & IO_FLAG_تارونه) {
		pthread_mutex_destroy(&معلومات->cache_mutex);
		pthread_mutex_destroy(&معلومات->bounce_mutex);
		pthread_mutex_destroy(&معلومات->stats_mutex);
	}
#د_که_پای

	ext2fs_free_mem(&channel->شخصي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t unix_سېټ_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_شخصي_معلومات *معلومات;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->بلاک_اندازه != blksize) {
		mutex_قلف(معلومات, CACHE_MTX);
		mutex_قلف(معلومات, BOUNCE_MTX);
#که_نه_تعریف_شوی NO_IO_CACHE
		إذا ((retval = flush_cached_بلاکونه(channel, معلومات, FLUSH_NOLOCK))){
			mutex_unlock(معلومات, BOUNCE_MTX);
			mutex_unlock(معلومات, CACHE_MTX);
			أرجع retval;
		}
#د_که_پای

		channel->بلاک_اندازه = blksize;
		free_cache(معلومات);
		retval = alloc_cache(channel, معلومات);
		mutex_unlock(معلومات, BOUNCE_MTX);
		mutex_unlock(معلومات, CACHE_MTX);
	}
	أرجع retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	بنية unix_شخصي_معلومات *معلومات;
	بنية unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#که_تعریف_شوی NO_IO_CACHE
	أرجع raw_read_blk(channel, معلومات, بلاک, شمېر, buf);
#وإلا
	إذا (معلومات->flags & IO_FLAG_NOCACHE)
		أرجع raw_read_blk(channel, معلومات, بلاک, شمېر, buf);
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
	mutex_قلف(معلومات, CACHE_MTX);
	طالما (شمېر > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = پیدا_کړه_cached_بلاک(معلومات, بلاک, عدم))) {
#که_تعریف_شوی DEBUG
			چاپ_کړه("Using cached block %lu\n", بلاک);
#د_که_پای
			حافظه_نقل(cp, cache->buf, channel->بلاک_اندازه);
			شمېر--;
			بلاک++;
			cp += channel->بلاک_اندازه;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < شمېر; i++)
			إذا (پیدا_کړه_cached_بلاک(معلومات, بلاک+i, عدم))
				اكسر;
#که_تعریف_شوی DEBUG
		چاپ_کړه("Reading %d blocks starting at %lu\n", i, بلاک);
#د_که_پای
		mutex_unlock(معلومات, CACHE_MTX);
		إذا ((retval = raw_read_blk(channel, معلومات, بلاک, i, cp)))
			أرجع retval;
		mutex_قلف(معلومات, CACHE_MTX);

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			إذا (!پیدا_کړه_cached_بلاک(معلومات, بلاک, &cache)) {
				retval = reuse_cache(channel, معلومات,
						     cache, بلاک);
				إذا (retval)
					اذهب call_write_handler;
				حافظه_نقل(cache->buf, cp, channel->بلاک_اندازه);
			}
			شمېر--;
			بلاک++;
			cp += channel->بلاک_اندازه;
		}
	}
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع 0;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بینښه اوږد اوږد err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->دننه_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			حافظه_نقل(err_buf, cache->buf, channel->بلاک_اندازه);
		mutex_unlock(معلومات, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_اندازه, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
#د_که_پای /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	أرجع unix_read_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	بنية unix_شخصي_معلومات *معلومات;
	بنية unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#که_تعریف_شوی NO_IO_CACHE
	أرجع raw_write_blk(channel, معلومات, بلاک, شمېر, buf, 0);
#وإلا
	إذا (معلومات->flags & IO_FLAG_NOCACHE)
		أرجع raw_write_blk(channel, معلومات, بلاک, شمېر, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (شمېر < 0 || شمېر > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلاکونه(channel, معلومات,
						  FLUSH_INVALIDATE)))
			أرجع retval;
		أرجع raw_write_blk(channel, معلومات, بلاک, شمېر, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, معلومات, بلاک, شمېر, buf, 0);

	cp = buf;
	mutex_قلف(معلومات, CACHE_MTX);
	طالما (شمېر > 0) {
		cache = پیدا_کړه_cached_بلاک(معلومات, بلاک, &reuse);
		إذا (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, معلومات, cache, بلاک);
			إذا (err)
				اذهب call_write_handler;
		}
		إذا (cache->buf != cp)
			حافظه_نقل(cache->buf, cp, channel->بلاک_اندازه);
		cache->dirty = !writethrough;
		شمېر--;
		بلاک++;
		cp += channel->بلاک_اندازه;
	}
	mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بینښه اوږد اوږد err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->دننه_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			حافظه_نقل(err_buf, cache->buf, channel->بلاک_اندازه);
		mutex_unlock(معلومات, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_اندازه, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(معلومات, CACHE_MTX);
	أرجع retval;
#د_که_پای /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      بینښه اوږد اوږد بلاک,
				      بینښه اوږد اوږد شمېر)
{
#که_تعریف_شوی POSIX_FADV_WILLNEED
	بنية unix_شخصي_معلومات *معلومات;

	معلومات = (بنية unix_شخصي_معلومات *)channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	أرجع posix_fadvise(معلومات->dev,
			     (ext2_loff_t)بلاک * channel->بلاک_اندازه + معلومات->دننه_واټن,
			     (ext2_loff_t)شمېر * channel->بلاک_اندازه,
			     POSIX_FADV_WILLNEED);
#وإلا
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
#د_که_پای
}

ساكن errcode_t unix_write_blk(io_channel channel, بینښه اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	أرجع unix_write_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t unix_write_byte(io_channel channel, بینښه اوږد دننه_واټن,
				 صحيح اندازه, ثابت فراغ *buf)
{
	بنية unix_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->align != 0) {
#که_تعریف_شوی ALIGN_DEBUG
		چاپ_کړه("unix_write_byte: O_DIRECT fallback\n");
#د_که_پای
		أرجع EXT2_ET_UNIMPLEMENTED;
	}

#که_نه_تعریف_شوی NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	إذا ((retval = flush_cached_بلاکونه(channel, معلومات, FLUSH_INVALIDATE)))
		أرجع retval;
#د_که_پای

	إذا (lseek(معلومات->dev, دننه_واټن + معلومات->دننه_واټن, SEEK_SET) < 0)
		أرجع errno;

	actual = اكتب(معلومات->dev, buf, اندازه);
	إذا (actual < 0)
		أرجع errno;
	إذا (actual != اندازه)
		أرجع EXT2_ET_SHORT_WRITE;

	أرجع 0;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t unix_flush(io_channel channel)
{
	بنية unix_شخصي_معلومات *معلومات;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#که_نه_تعریف_شوی NO_IO_CACHE
	retval = flush_cached_بلاکونه(channel, معلومات, 0);
#د_که_پای
#که_تعریف_شوی HAVE_FSYNC
	إذا (!retval && fsync(معلومات->dev) != 0)
		أرجع errno;
#د_که_پای
	أرجع retval;
}

ساكن errcode_t unix_سېټ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية unix_شخصي_معلومات *معلومات;
	بینښه اوږد اوږد tmp;
	errcode_t retval;
	حرف *پای_ټکی;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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
	إذا (!strcmp(option, "cache")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (!strcmp(arg, "on")) {
			معلومات->flags &= ~IO_FLAG_NOCACHE;
			أرجع 0;
		}
		إذا (!strcmp(arg, "off")) {
			retval = flush_cached_بلاکونه(channel, معلومات, 0);
			معلومات->flags |= IO_FLAG_NOCACHE;
			أرجع retval;
		}
		أرجع EXT2_ET_INVALID_دلیل;
	}
#که_نه_تعریف_شوی NO_IO_CACHE
	إذا (!strcmp(option, "cache_blocks")) {
		بینښه اوږد اوږد	اندازه;

		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		errno = 0;
		اندازه = strtoll(arg, عدم, 0);
		إذا (errno || اندازه == 0 || اندازه > INT32_MAX)
			أرجع EXT2_ET_INVALID_دلیل;

		إذا (معلومات->cache_اندازه == اندازه)
			أرجع 0;
		إذا (معلومات->cache_اندازه > اندازه)
			أرجع shrink_cache(channel, معلومات, اندازه);
		أرجع grow_cache(channel, معلومات, اندازه);
	}
#د_که_پای
	أرجع EXT2_ET_INVALID_دلیل;
}

#إذا defined(__linux__) && !defined(BLKDISCARD)
#تعریف_کړه BLKDISCARD		_IO(0x12,119)
#د_که_پای

ساكن errcode_t unix_discard(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية unix_شخصي_معلومات *معلومات;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	إذا (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
#که_تعریف_شوی BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(بلاک) * channel->بلاک_اندازه + معلومات->دننه_واټن;
		range[1] = (__u64)(شمېر) * channel->بلاک_اندازه;

		ret = ioctl(معلومات->dev, BLKDISCARD, &range);
#وإلا
		اذهب unimplemented;
#د_که_پای
	} وإلا {
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(معلومات->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(بلاک) * channel->بلاک_اندازه + معلومات->دننه_واټن,
				(off_t)(شمېر) * channel->بلاک_اندازه);
#وإلا
		اذهب unimplemented;
#د_که_پای
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
ساكن صحيح __unix_zeroout(صحيح fd, off_t دننه_واټن, off_t len)
{
	صحيح ret = -1;

#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, دننه_واټن, len);
	إذا (ret == 0)
		أرجع 0;
#د_که_پای
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			دننه_واټن,  len);
	إذا (ret == 0)
		أرجع 0;
#د_که_پای
	errno = EOPNOTSUPP;
	أرجع ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wunused-parameter"
#د_که_پای
ساكن errcode_t unix_zeroout(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية unix_شخصي_معلومات *معلومات;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية unix_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!(channel->flags & CHANNEL_FLAGS_بلاک_DEVICE)) {
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
	}

	إذا (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(معلومات->dev,
			(off_t)(بلاک) * channel->بلاک_اندازه + معلومات->دننه_واټن,
			(off_t)(شمېر) * channel->بلاک_اندازه);
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
#لارښوونه GCC diagnostic راوباسه
#د_که_پای

ساكن بنية struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.سېټ_blksize	= unix_سېټ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سېټ_option	= unix_سېټ_option,
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
	.سېټ_blksize	= unix_سېټ_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.سېټ_option	= unix_سېټ_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
