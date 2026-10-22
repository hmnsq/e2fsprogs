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

#اگر !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#تعریف_کرو XOPEN_ماخذ 600
#تعریف_کرو DARWIN_C_ماخذ
#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ GNU_ماخذ
#تعریف_کرو GNU_ماخذ
#اگر_ختم
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر_تعریف_شدہ __linux__
#شامل <sys/utsname.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_MOUNT_H
#شامل <sys/mount.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_RESOURCE_H
#شامل <sys/resource.h>
#اگر_ختم
#اگر HAVE_LINUX_FALLOC_H
#شامل <linux/falloc.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_PTHREAD
#شامل <pthread.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_FILE_H
#شامل <sys/file.h>
#اگر_ختم

#اگر defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#تعریف_کرو BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#اگر_ختم

#تعریف_منسوخ ALIGN_DEBUG

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساکن اندرونی صحیح ڈھونڈو_آخری_ثنائی_ہندسہ_سیٹ(صحیح x)
{
	صحیح r = 32;

	اگر (!x)
		واپس 0;
	اگر (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	اگر (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	اگر (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	اگر (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	اگر (!(x & 0x80000000u)) {
		r -= 1;
	}
	واپس r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ساکن اندرونی صحیح highbit32(uint32_t v)
{
	واپس ڈھونڈو_آخری_ثنائی_ہندسہ_سیٹ(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)

ساخت unix_cache {
	حرف			*buf;
	غیرعلامتی طویل طویل	بلاک;
	صحیح			access_وقت;
	غیرعلامتی		dirty:1;
	غیرعلامتی		اندر_use:1;
	غیرعلامتی		write_err:1;
};

#تعریف_کرو DEFAULT_CACHE_سائز 8
#تعریف_کرو WRITE_براہ_راست_سائز 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کرو READ_براہ_راست_سائز 4	/* Should be smaller than CACHE_SIZE */

ساخت unix_نجی_ڈیٹا {
	صحیح	magic;
	صحیح	dev;
	صحیح	flags;
	صحیح	align;
	صحیح	access_وقت;
	صحیح	unix_flock_flags;
	ext2_loff_t انحراف;
	ساخت unix_cache *cache;
	غیرعلامتی صحیح cache_سائز;
	غیرعلامتی صحیح cache_hash_سرکاؤ;
	خالی	*bounce;
	ساخت struct_io_stats io_stats;
#اگر_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#اگر_ختم
};

#تعریف_کرو IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

قسمتعریف شمار تالا_قسم {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} قسم_t;

#اگر_تعریف_شدہ HAVE_PTHREAD
ساکن اندرونی pthread_mutex_t *get_mutex(ساخت unix_نجی_ڈیٹا *ڈیٹا,
					 قسم_t قسم)
{
	اگر (ڈیٹا->flags & IO_FLAG_دھاگے) {
		انتخاب (قسم) {
		حالت CACHE_MTX:
			واپس &ڈیٹا->cache_mutex;
		حالت BOUNCE_MTX:
			واپس &ڈیٹا->bounce_mutex;
		حالت STATS_MTX:
			واپس &ڈیٹا->stats_mutex;
		}
	}
	واپس تہی;
}
#اگر_ختم

ساکن اندرونی خالی mutex_تالا(ساخت unix_نجی_ڈیٹا *ڈیٹا, قسم_t قسم)
{
#اگر_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ڈیٹا,قسم);

	اگر (mtx)
		pthread_mutex_تالا(mtx);
#اگر_ختم
}

ساکن اندرونی خالی mutex_unlock(ساخت unix_نجی_ڈیٹا *ڈیٹا, قسم_t قسم)
{
#اگر_تعریف_شدہ HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(ڈیٹا,قسم);

	اگر (mtx)
		pthread_mutex_unlock(mtx);
#اگر_ختم
}

ساکن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ساخت unix_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (stats) {
		mutex_تالا(ڈیٹا, STATS_MTX);
		*stats = &ڈیٹا->io_stats;
		mutex_unlock(ڈیٹا, STATS_MTX);
	}

	واپس retval;
}

/*
 * Here are the raw I/O functions
 */
ساکن errcode_t raw_read_blk(io_channel channel,
			      ساخت unix_نجی_ڈیٹا *ڈیٹا,
			      غیرعلامتی طویل طویل بلاک,
			      صحیح گنتی, خالی *bufv)
{
	errcode_t	retval;
	ssize_t		سائز;
	ext2_loff_t	location;
	صحیح		actual = 0;
	غیرعلامتی حرف	*buf = bufv;
	ssize_t		really_read = 0;
	غیرعلامتی طویل طویل aligned_blk;
	صحیح		align_سائز, انحراف;

	سائز = (گنتی < 0) ? -گنتی : (ext2_loff_t) گنتی * channel->بلاک_سائز;
	mutex_تالا(ڈیٹا, STATS_MTX);
	ڈیٹا->io_stats.بائٹیں_read += سائز;
	mutex_unlock(ڈیٹا, STATS_MTX);
	location = ((ext2_loff_t) بلاک * channel->بلاک_سائز) + ڈیٹا->انحراف;

	اگر (ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE)
		جاؤ bounce_read;

#اگر_تعریف_شدہ HAVE_PREAD64
	/* Try an aligned pread */
	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائز, channel->align))) {
		actual = pread64(ڈیٹا->dev, buf, سائز, location);
		اگر (actual == سائز)
			واپس 0;
		actual = 0;
	}
#وگرنہ_اگر HAVE_PREAD
	/* Try an aligned pread */
	اگر ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(سائز, channel->align)))) {
		actual = pread(ڈیٹا->dev, buf, سائز, location);
		اگر (actual == سائز)
			واپس 0;
		actual = 0;
	}
#اگر_ختم /* HAVE_PREAD */

	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائز, channel->align))) {
		mutex_تالا(ڈیٹا, BOUNCE_MTX);
		اگر (ext2fs_llseek(ڈیٹا->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			جاؤ error_unlock;
		}
		actual = اقرأ(ڈیٹا->dev, buf, سائز);
		اگر (actual != سائز) {
		short_read:
			اگر (actual < 0) {
				retval = errno;
				actual = 0;
			} ورنہ
				retval = EXT2_ET_SHORT_READ;
			جاؤ error_unlock;
		}
		جاؤ success_unlock;
	}

#اگر_تعریف_شدہ ALIGN_DEBUG
	لکھو("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (غیرعلامتی طویل) سائز);
#اگر_ختم

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	اگر (channel->align == 0)
		channel->align = 1;
	اگر ((channel->بلاک_سائز > channel->align) &&
	    (channel->بلاک_سائز % channel->align) == 0)
		align_سائز = channel->بلاک_سائز;
	ورنہ
		align_سائز = channel->align;
	aligned_blk = location / align_سائز;
	انحراف = location % align_سائز;

	mutex_تالا(ڈیٹا, BOUNCE_MTX);
	اگر (ext2fs_llseek(ڈیٹا->dev, aligned_blk * align_سائز, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		جاؤ error_unlock;
	}
	جب (سائز > 0) {
		actual = اقرأ(ڈیٹا->dev, ڈیٹا->bounce, align_سائز);
		اگر (actual != align_سائز) {
			actual = really_read;
			buf -= really_read;
			سائز += really_read;
			جاؤ short_read;
		}
		اگر ((actual + انحراف) > align_سائز)
			actual = align_سائز - انحراف;
		اگر (actual > سائز)
			actual = سائز;
		حافظہ_نقل(buf, (حرف *)ڈیٹا->bounce + انحراف, actual);

		really_read += actual;
		سائز -= actual;
		buf += actual;
		انحراف = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(ڈیٹا, BOUNCE_MTX);
	واپس 0;

error_unlock:
	mutex_unlock(ڈیٹا, BOUNCE_MTX);
	اگر (actual >= 0 && actual < سائز)
		memset((حرف *) buf+actual, 0, سائز-actual);
	اگر (channel->read_error)
		retval = (channel->read_error)(channel, بلاک, گنتی, buf,
					       سائز, actual, retval);
	واپس retval;
}

#تعریف_کرو RAW_WRITE_NO_HANDLER	(1U << 0)
#تعریف_کرو RAW_WRITE_NOLOCK	(1U << 1)

ساکن errcode_t raw_write_blk(io_channel channel,
			       ساخت unix_نجی_ڈیٹا *ڈیٹا,
			       غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, ثابت خالی *bufv,
			       صحیح flags)
{
	ssize_t		سائز;
	ext2_loff_t	location;
	صحیح		actual = 0;
	errcode_t	retval;
	ثابت غیرعلامتی حرف *buf = bufv;
	غیرعلامتی طویل طویل aligned_blk;
	صحیح		align_سائز, انحراف;

	اگر (گنتی == 1)
		سائز = channel->بلاک_سائز;
	ورنہ {
		اگر (گنتی < 0)
			سائز = -گنتی;
		ورنہ
			سائز = (ext2_loff_t) گنتی * channel->بلاک_سائز;
	}
	mutex_تالا(ڈیٹا, STATS_MTX);
	ڈیٹا->io_stats.بائٹیں_written += سائز;
	mutex_unlock(ڈیٹا, STATS_MTX);

	location = ((ext2_loff_t) بلاک * channel->بلاک_سائز) + ڈیٹا->انحراف;

	اگر (ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE)
		جاؤ bounce_write;

#اگر_تعریف_شدہ HAVE_PWRITE64
	/* Try an aligned pwrite */
	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائز, channel->align))) {
		actual = pwrite64(ڈیٹا->dev, buf, سائز, location);
		اگر (actual == سائز)
			واپس 0;
	}
#وگرنہ_اگر HAVE_PWRITE
	/* Try an aligned pwrite */
	اگر ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(سائز, channel->align)))) {
		actual = pwrite(ڈیٹا->dev, buf, سائز, location);
		اگر (actual == سائز)
			واپس 0;
	}
#اگر_ختم /* HAVE_PWRITE */

	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(سائز, channel->align))) {
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_تالا(ڈیٹا, BOUNCE_MTX);
		اگر (ext2fs_llseek(ڈیٹا->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			جاؤ error_unlock;
		}
		actual = اكتب(ڈیٹا->dev, buf, سائز);
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ڈیٹا, BOUNCE_MTX);
		اگر (actual < 0) {
			retval = errno;
			جاؤ error_برآمد;
		}
		اگر (actual != سائز) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			جاؤ error_برآمد;
		}
		واپس 0;
	}

#اگر_تعریف_شدہ ALIGN_DEBUG
	لکھو("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (غیرعلامتی طویل) سائز);
#اگر_ختم
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	اگر (channel->align == 0)
		channel->align = 1;
	اگر ((channel->بلاک_سائز > channel->align) &&
	    (channel->بلاک_سائز % channel->align) == 0)
		align_سائز = channel->بلاک_سائز;
	ورنہ
		align_سائز = channel->align;
	aligned_blk = location / align_سائز;
	انحراف = location % align_سائز;

	جب (سائز > 0) {
		صحیح actual_w;

		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_تالا(ڈیٹا, BOUNCE_MTX);
		اگر (سائز < align_سائز || انحراف) {
			اگر (ext2fs_llseek(ڈیٹا->dev, aligned_blk * align_سائز,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				جاؤ error_unlock;
			}
			actual = اقرأ(ڈیٹا->dev, ڈیٹا->bounce,
				      align_سائز);
			اگر (actual != align_سائز) {
				اگر (actual < 0) {
					retval = errno;
					جاؤ error_unlock;
				}
				memset((حرف *) ڈیٹا->bounce + actual, 0,
				       align_سائز - actual);
			}
		}
		actual = سائز;
		اگر ((actual + انحراف) > align_سائز)
			actual = align_سائز - انحراف;
		اگر (actual > سائز)
			actual = سائز;
		حافظہ_نقل(((حرف *)ڈیٹا->bounce) + انحراف, buf, actual);
		اگر (ext2fs_llseek(ڈیٹا->dev, aligned_blk * align_سائز, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			جاؤ error_unlock;
		}
		actual_w = اكتب(ڈیٹا->dev, ڈیٹا->bounce, align_سائز);
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(ڈیٹا, BOUNCE_MTX);
		اگر (actual_w < 0) {
			retval = errno;
			جاؤ error_برآمد;
		}
		اگر (actual_w != align_سائز)
			جاؤ short_write;
		سائز -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		انحراف = 0;
	}
	واپس 0;

error_unlock:
	اگر (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(ڈیٹا, BOUNCE_MTX);
error_برآمد:
	اگر (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, بلاک, گنتی, buf,
						سائز, actual, retval);
	واپس retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساکن errcode_t alloc_cache(io_channel channel,
			     ساخت unix_نجی_ڈیٹا *ڈیٹا)
{
	errcode_t		retval;
	ساخت unix_cache	*cache;
	غیرعلامتی صحیح		i;

	ڈیٹا->access_وقت = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < ڈیٹا->cache_سائز; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval)
			واپس retval;
	}
	اگر (channel->align || ڈیٹا->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (ڈیٹا->bounce)
			ext2fs_free_mem(&ڈیٹا->bounce);
		retval = io_channel_alloc_buf(channel, 0, &ڈیٹا->bounce);
	}
	واپس retval;
}

/* Free the cache buffers */
ساکن خالی free_cache(ساخت unix_نجی_ڈیٹا *ڈیٹا)
{
	ساخت unix_cache	*cache;
	غیرعلامتی صحیح		i;

	ڈیٹا->access_وقت = 0;
	برائے (i=0, cache = ڈیٹا->cache; i < ڈیٹا->cache_سائز; i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	اگر (ڈیٹا->bounce)
		ext2fs_free_mem(&ڈیٹا->bounce);
}

#اگر_غیر_تعریف_شدہ NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#تعریف_کرو GOLDEN_تناسب_مفرد	0x9e37fffffffc0001UL
#اگر_غیر_تعریف_شدہ CACHE_سطر_سائز
/* if the system didn't tell us, guess something reasonable */
#تعریف_کرو CACHE_سطر_سائز		64
#اگر_ختم

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساکن غیرعلامتی صحیح
cache_hash(ساخت unix_نجی_ڈیٹا *ڈیٹا, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	اگر (ڈیٹا->cache_سائز <= DEFAULT_CACHE_سائز)
		واپس 0;

	tmp = hashval ^ (GOLDEN_تناسب_مفرد + hashval) / CACHE_سطر_سائز;
	tmp = tmp ^ ((tmp ^ GOLDEN_تناسب_مفرد) >> ڈیٹا->cache_hash_سرکاؤ);
	واپس tmp % ڈیٹا->cache_سائز;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساکن ساخت unix_cache *ڈھونڈو_cached_بلاک(ساخت unix_نجی_ڈیٹا *ڈیٹا,
					    غیرعلامتی طویل طویل بلاک,
					    ساخت unix_cache **eldest)
{
	ساخت unix_cache	*cache, *غیر_مستعمل_cache, *oldest_cache;
	غیرعلامتی صحیح		hash = cache_hash(ڈیٹا, بلاک);
	غیرعلامتی صحیح		i;

	غیر_مستعمل_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	برائے (i = hash, cache = ڈیٹا->cache + hash;
	     i < ڈیٹا->cache_سائز;
	     i++, cache++) {
		اگر (!cache->اندر_use) {
			اگر (!غیر_مستعمل_cache)
				غیر_مستعمل_cache = cache;
			جاری;
		}
		اگر (cache->بلاک == بلاک) {
			cache->access_وقت = ++ڈیٹا->access_وقت;
			ڈیٹا->io_stats.cache_کامیابیاں++;
			واپس cache;
		}
		اگر (!oldest_cache ||
		    (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	برائے (i = 0, cache = ڈیٹا->cache; i < hash; i++, cache++) {
		اگر (!cache->اندر_use) {
			اگر (!غیر_مستعمل_cache)
				غیر_مستعمل_cache = cache;
			جاری;
		}
		اگر (cache->بلاک == بلاک) {
			cache->access_وقت = ++ڈیٹا->access_وقت;
			ڈیٹا->io_stats.cache_کامیابیاں++;
			واپس cache;
		}
		اگر (!oldest_cache ||
		    (cache->access_وقت < oldest_cache->access_وقت))
			oldest_cache = cache;
	}
	اگر (eldest)
		*eldest = (غیر_مستعمل_cache) ? غیر_مستعمل_cache : oldest_cache;
	ڈیٹا->io_stats.cache_ناکامیاں++;
	واپس 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساکن errcode_t reuse_cache(io_channel channel,
		ساخت unix_نجی_ڈیٹا *ڈیٹا, ساخت unix_cache *cache,
		غیرعلامتی طویل طویل بلاک)
{
	اگر (cache->dirty && cache->اندر_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, ڈیٹا, cache->بلاک, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		اگر (retval) {
			cache->write_err = 1;
			واپس retval;
		}
	}

	cache->اندر_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->بلاک = بلاک;
	cache->access_وقت = ++ڈیٹا->access_وقت;
	واپس 0;
}

#تعریف_کرو FLUSH_INVALIDATE	0x01
#تعریف_کرو FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساکن errcode_t flush_cached_بلاکیں(io_channel channel,
				     ساخت unix_نجی_ڈیٹا *ڈیٹا,
				     صحیح flags)
{
	ساخت unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	غیرعلامتی صحیح		i;
	صحیح			errors_ملا = 0;

	اگر ((flags & FLUSH_NOLOCK) == 0)
		mutex_تالا(ڈیٹا, CACHE_MTX);
	برائے (i=0, cache = ڈیٹا->cache; i < ڈیٹا->cache_سائز; i++, cache++) {
		اگر (!cache->اندر_use)
			جاری;
		اگر (cache->dirty) {
			صحیح raw_flags = RAW_WRITE_NO_HANDLER;

			اگر (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, ڈیٹا,
					       cache->بلاک, 1, cache->buf,
					       raw_flags);
			اگر (retval) {
				cache->write_err = 1;
				errors_ملا = 1;
				retval2 = retval;
			} ورنہ {
				cache->dirty = 0;
				cache->write_err = 0;
				اگر (flags & FLUSH_INVALIDATE)
					cache->اندر_use = 0;
			}
		} ورنہ اگر (flags & FLUSH_INVALIDATE) {
			cache->اندر_use = 0;
		}
	}
	اگر ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(ڈیٹا, CACHE_MTX);
retry:
	جب (errors_ملا) {
		اگر ((flags & FLUSH_NOLOCK) == 0)
			mutex_تالا(ڈیٹا, CACHE_MTX);
		errors_ملا = 0;
		برائے (i=0, cache = ڈیٹا->cache; i < ڈیٹا->cache_سائز; i++, cache++) {
			اگر (!cache->اندر_use || !cache->write_err)
				جاری;
			errors_ملا = 1;
			اگر (cache->write_err && channel->write_error) {
				حرف *err_buf = تہی;
				غیرعلامتی طویل طویل err_بلاک = cache->بلاک;

				cache->dirty = 0;
				cache->اندر_use = 0;
				cache->write_err = 0;
				اگر (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = تہی;
				ورنہ
					حافظہ_نقل(err_buf, cache->buf,
					       channel->بلاک_سائز);
				mutex_unlock(ڈیٹا, CACHE_MTX);
				(channel->write_error)(channel, err_بلاک,
					1, err_buf, channel->بلاک_سائز, -1,
					retval2);
				اگر (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_تالا(ڈیٹا, CACHE_MTX);
				جاؤ retry;
			} ورنہ
				cache->write_err = 0;
		}
		اگر ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(ڈیٹا, CACHE_MTX);
	}
	واپس retval2;
}

/* Shrink the cache buffers */
ساکن errcode_t shrink_cache(io_channel channel,
			      ساخت unix_نجی_ڈیٹا *ڈیٹا,
			      غیرعلامتی صحیح new_سائز)
{
	ساخت unix_cache	*cache, *new_cache;
	غیرعلامتی صحیح		i;
	errcode_t		retval;

	mutex_تالا(ڈیٹا, CACHE_MTX);

	retval = flush_cached_بلاکیں(channel, ڈیٹا,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	اگر (retval)
		جاؤ unlock;

	برائے (i = new_سائز, cache = ڈیٹا->cache + new_سائز;
	     i < ڈیٹا->cache_سائز;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(ڈیٹا->cache, new_سائز * حجم(ساخت unix_cache));
	اگر (!new_cache) {
		retval = EXT2_ET_NO_حافظہ;
		جاؤ unlock;
	}

	ڈیٹا->cache = new_cache;
	ڈیٹا->cache_سائز = new_سائز;
	ڈیٹا->cache_hash_سرکاؤ = highbit32(ڈیٹا->cache_سائز);

unlock:
	mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس retval;
}

/* Grow the cache buffers */
ساکن errcode_t grow_cache(io_channel channel,
			    ساخت unix_نجی_ڈیٹا *ڈیٹا,
			    غیرعلامتی صحیح new_سائز)
{
	ساخت unix_cache	*cache, *new_cache;
	غیرعلامتی صحیح		i;
	errcode_t		retval;

	mutex_تالا(ڈیٹا, CACHE_MTX);

	retval = flush_cached_بلاکیں(channel, ڈیٹا,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	اگر (retval)
		جاؤ unlock;

	new_cache = realloc(ڈیٹا->cache, new_سائز * حجم(ساخت unix_cache));
	اگر (!new_cache) {
		retval = EXT2_ET_NO_حافظہ;
		جاؤ unlock;
	}
	ڈیٹا->cache = new_cache;

	برائے (i = ڈیٹا->cache_سائز, cache = new_cache + ڈیٹا->cache_سائز;
	     i < new_سائز;
	     i++, cache++) {
		cache->بلاک = 0;
		cache->access_وقت = 0;
		cache->dirty = 0;
		cache->اندر_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval) {
			جب (i > ڈیٹا->cache_سائز) {
				cache--; i--;
				اگر (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			جاؤ unlock;
		}
	}

	ڈیٹا->cache_سائز = new_سائز;
	ڈیٹا->cache_hash_سرکاؤ = highbit32(ڈیٹا->cache_سائز);

unlock:
	mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس retval;
}
#اگر_ختم /* NO_IO_CACHE */

#اگر_تعریف_شدہ __linux__
#اگر_غیر_تعریف_شدہ BLKDISCARDZEROES
#تعریف_کرو BLKDISCARDZEROES _IO(0x12,124)
#اگر_ختم
#اگر_ختم

صحیح ext2fs_open_file(ثابت حرف *pathname, صحیح flags, mode_t منوال)
{
	اگر (منوال)
#اگر defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		واپس open64(pathname, flags, منوال);
	ورنہ
		واپس open64(pathname, flags);
#ورنہ
		واپس open(pathname, flags, منوال);
	ورنہ
		واپس open(pathname, flags);
#اگر_ختم
}

صحیح ext2fs_stat(ثابت حرف *راستہ, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	واپس stat64(راستہ, buf);
#ورنہ
	واپس stat(راستہ, buf);
#اگر_ختم
}

صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	واپس fstat64(fd, buf);
#ورنہ
	واپس fstat(fd, buf);
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_SYS_FILE_H
ساکن errcode_t unix_funlock(io_channel channel)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	صحیح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (ڈیٹا->unix_flock_flags) {
		ret = flock(ڈیٹا->dev, LOCK_UN);
		اگر (ret)
			واپس errno;

		ڈیٹا->unix_flock_flags = 0;
	}

	واپس 0;
}

ساکن errcode_t unix_flock(io_channel channel, غیرعلامتی صحیح flock_flags)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	صحیح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	اگر (ret)
		واپس ret;

	اگر (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	اگر (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	اگر (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	اگر (!unix_flock_flags)
		واپس 0;

	ret = flock(ڈیٹا->dev, unix_flock_flags);
	اگر (ret < 0)
		واپس errno;

	ڈیٹا->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	واپس 0;
}
#ورنہ
#تعریف_کرو unix_flock		تہی

ساکن errcode_t unix_funlock(io_channel channel)
{
	واپس 0;
}
#اگر_ختم /* HAVE_SYS_FILE_H */

ساکن errcode_t unix_open_channel(ثابت حرف *name, صحیح fd,
				   صحیح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = تہی;
	ساخت unix_نجی_ڈیٹا *ڈیٹا = تہی;
	errcode_t	retval;
	ext2fs_struct_stat st;
#اگر_تعریف_شدہ __linux__
	ساخت		utsname ut;
#اگر_ختم

	اگر (ext2fs_محفوظ_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#اگر_تعریف_شدہ __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(خالی) fsync(fd);
#اگر_ختم

	retval = ext2fs_get_mem(حجم(ساخت struct_io_channel), &io);
	اگر (retval)
		جاؤ cleanup;
	memset(io, 0, حجم(ساخت struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(ساخت unix_نجی_ڈیٹا), &ڈیٹا);
	اگر (retval)
		جاؤ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(io->name, name);
	io->نجی_ڈیٹا = ڈیٹا;
	io->بلاک_سائز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	اگر (ext2fs_محفوظ_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(ڈیٹا, 0, حجم(ساخت unix_نجی_ڈیٹا));
	ڈیٹا->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ڈیٹا->io_stats.عدد_fields = 4;
	ڈیٹا->flags = flags;
	ڈیٹا->dev = fd;

	ڈیٹا->cache_سائز = DEFAULT_CACHE_سائز;
	ڈیٹا->cache_hash_سرکاؤ = highbit32(ڈیٹا->cache_سائز);
	ڈیٹا->cache = صفر_حافظہ_دو(DEFAULT_CACHE_سائز, حجم(ساخت unix_cache));
	اگر (!ڈیٹا->cache) {
		retval = EXT2_ET_NO_حافظہ;
		جاؤ cleanup;
	}

#اگر defined(O_براہ_راست)
	اگر (flags & IO_FLAG_براہ_راست_IO)
		io->align = ext2fs_get_dio_alignment(ڈیٹا->dev);
#وگرنہ_اگر defined(F_NOCACHE)
	اگر (flags & IO_FLAG_براہ_راست_IO)
		io->align = 4096;
#اگر_ختم

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	اگر (ext2fs_fstat(ڈیٹا->dev, &st) == 0) {
		اگر (ext2fsP_is_disk_device(st.st_منوال)) {
#اگر_تعریف_شدہ BLKDISCARDZEROES
			صحیح zeroes = 0;

			اگر (ioctl(ڈیٹا->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#اگر_ختم
			io->flags |= CHANNEL_FLAGS_بلاک_DEVICE;
		} ورنہ {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#اگر defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	اگر (!io->align)
		io->align = 512;
#اگر_ختم

#اگر defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	اگر (io->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
		صحیح dio_align = ext2fs_get_dio_alignment(fd);

		اگر (io->align < dio_align)
			io->align = dio_align;
	}
#اگر_ختم

	اگر ((retval = alloc_cache(io, ڈیٹا)))
		جاؤ cleanup;

#اگر_تعریف_شدہ BLKROGET
	اگر (flags & IO_FLAG_RW) {
		صحیح error;
		صحیح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(ڈیٹا->dev, BLKROGET, &readonly);
		اگر (!error && readonly) {
			retval = EPERM;
			جاؤ cleanup;
		}
	}
#اگر_ختم

#اگر_تعریف_شدہ __linux__
#تعریف_منسوخ RLIM_INFINITY
#اگر (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#تعریف_کرو RLIM_INFINITY	((غیرعلامتی طویل)(~0UL>>1))
#ورنہ
#تعریف_کرو RLIM_INFINITY  (~0UL)
#اگر_ختم
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	اگر ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(ڈیٹا->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_منوال))) {
		ساخت rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (غیرعلامتی طویل) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		اگر (((غیرعلامتی طویل) rlim.rlim_cur) <
		    ((غیرعلامتی طویل) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#اگر_ختم
#اگر_تعریف_شدہ HAVE_PTHREAD
	اگر (flags & IO_FLAG_دھاگے) {
		io->flags |= CHANNEL_FLAGS_دھاگے;
		retval = pthread_mutex_init(&ڈیٹا->cache_mutex, تہی);
		اگر (retval)
			جاؤ cleanup;
		retval = pthread_mutex_init(&ڈیٹا->bounce_mutex, تہی);
		اگر (retval) {
			pthread_mutex_destroy(&ڈیٹا->cache_mutex);
			جاؤ cleanup;
		}
		retval = pthread_mutex_init(&ڈیٹا->stats_mutex, تہی);
		اگر (retval) {
			pthread_mutex_destroy(&ڈیٹا->cache_mutex);
			pthread_mutex_destroy(&ڈیٹا->bounce_mutex);
			جاؤ cleanup;
		}
	}
#اگر_ختم
	*channel = io;
	واپس 0;

cleanup:
	اگر (ڈیٹا) {
		unix_funlock(io);
		اگر (io->manager != unixfd_io_manager && ڈیٹا->dev >= 0)
			close(ڈیٹا->dev);
		اگر (ڈیٹا->cache) {
			free_cache(ڈیٹا);
			آزاد(ڈیٹا->cache);
		}
		ext2fs_free_mem(&ڈیٹا);
	}
	اگر (io) {
		اگر (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	واپس retval;
}

ساکن errcode_t unixfd_open(ثابت حرف *str_fd, صحیح flags,
			     io_channel *channel)
{
	صحیح fd;
	صحیح fd_flags;

	fd = atoi(str_fd);
#اگر defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	اگر (fd_flags == -1)
		واپس EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	اگر (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#اگر defined(O_براہ_راست)
	اگر (fd_flags & O_براہ_راست)
		flags |= IO_FLAG_براہ_راست_IO;
#اگر_ختم
#اگر_ختم  /* HAVE_FCNTL */

	واپس unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ساکن errcode_t unix_open(ثابت حرف *name, صحیح flags,
			   io_channel *channel)
{
	صحیح fd = -1;
	صحیح open_flags;

	اگر (name == 0)
		واپس EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	اگر (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#اگر defined(O_براہ_راست)
	اگر (flags & IO_FLAG_براہ_راست_IO)
		open_flags |= O_براہ_راست;
#اگر_ختم
	fd = ext2fs_open_file(name, open_flags, 0);
	اگر (fd < 0)
		واپس errno;
#اگر defined(F_NOCACHE) && !defined(IO_براہ_راست)
	اگر (flags & IO_FLAG_براہ_راست_IO) {
		اگر (fcntl(fd, F_NOCACHE, 1) < 0)
			واپس errno;
	}
#اگر_ختم
	واپس unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساکن errcode_t unix_close(io_channel channel)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		واپس 0;

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	retval = flush_cached_بلاکیں(channel, ڈیٹا, 0);
#اگر_ختم

	unix_funlock(channel);

	اگر (channel->manager != unixfd_io_manager && close(ڈیٹا->dev) < 0)
		retval = errno;
	free_cache(ڈیٹا);
	آزاد(ڈیٹا->cache);
#اگر_تعریف_شدہ HAVE_PTHREAD
	اگر (ڈیٹا->flags & IO_FLAG_دھاگے) {
		pthread_mutex_destroy(&ڈیٹا->cache_mutex);
		pthread_mutex_destroy(&ڈیٹا->bounce_mutex);
		pthread_mutex_destroy(&ڈیٹا->stats_mutex);
	}
#اگر_ختم

	ext2fs_free_mem(&channel->نجی_ڈیٹا);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	واپس retval;
}

ساکن errcode_t unix_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->بلاک_سائز != blksize) {
		mutex_تالا(ڈیٹا, CACHE_MTX);
		mutex_تالا(ڈیٹا, BOUNCE_MTX);
#اگر_غیر_تعریف_شدہ NO_IO_CACHE
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, FLUSH_NOLOCK))){
			mutex_unlock(ڈیٹا, BOUNCE_MTX);
			mutex_unlock(ڈیٹا, CACHE_MTX);
			واپس retval;
		}
#اگر_ختم

		channel->بلاک_سائز = blksize;
		free_cache(ڈیٹا);
		retval = alloc_cache(channel, ڈیٹا);
		mutex_unlock(ڈیٹا, BOUNCE_MTX);
		mutex_unlock(ڈیٹا, CACHE_MTX);
	}
	واپس retval;
}

ساکن errcode_t unix_read_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	ساخت unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحیح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_تعریف_شدہ NO_IO_CACHE
	واپس raw_read_blk(channel, ڈیٹا, بلاک, گنتی, buf);
#ورنہ
	اگر (ڈیٹا->flags & IO_FLAG_NOCACHE)
		واپس raw_read_blk(channel, ڈیٹا, بلاک, گنتی, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	اگر (گنتی < 0 || گنتی > WRITE_براہ_راست_سائز) {
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, 0)))
			واپس retval;
		واپس raw_read_blk(channel, ڈیٹا, بلاک, گنتی, buf);
	}

	cp = buf;
	mutex_تالا(ڈیٹا, CACHE_MTX);
	جب (گنتی > 0) {
		/* If it's in the cache, use it! */
		اگر ((cache = ڈھونڈو_cached_بلاک(ڈیٹا, بلاک, تہی))) {
#اگر_تعریف_شدہ DEBUG
			لکھو("Using cached block %lu\n", بلاک);
#اگر_ختم
			حافظہ_نقل(cp, cache->buf, channel->بلاک_سائز);
			گنتی--;
			بلاک++;
			cp += channel->بلاک_سائز;
			جاری;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		برائے (i=1; i < گنتی; i++)
			اگر (ڈھونڈو_cached_بلاک(ڈیٹا, بلاک+i, تہی))
				توڑو;
#اگر_تعریف_شدہ DEBUG
		لکھو("Reading %d blocks starting at %lu\n", i, بلاک);
#اگر_ختم
		mutex_unlock(ڈیٹا, CACHE_MTX);
		اگر ((retval = raw_read_blk(channel, ڈیٹا, بلاک, i, cp)))
			واپس retval;
		mutex_تالا(ڈیٹا, CACHE_MTX);

		/* Save the results in the cache */
		برائے (j=0; j < i; j++) {
			اگر (!ڈھونڈو_cached_بلاک(ڈیٹا, بلاک, &cache)) {
				retval = reuse_cache(channel, ڈیٹا,
						     cache, بلاک);
				اگر (retval)
					جاؤ call_write_handler;
				حافظہ_نقل(cache->buf, cp, channel->بلاک_سائز);
			}
			گنتی--;
			بلاک++;
			cp += channel->بلاک_سائز;
		}
	}
	mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس 0;

call_write_handler:
	اگر (cache->write_err && channel->write_error) {
		حرف *err_buf = تہی;
		غیرعلامتی طویل طویل err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->اندر_use = 0;
		cache->write_err = 0;
		اگر (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = تہی;
		ورنہ
			حافظہ_نقل(err_buf, cache->buf, channel->بلاک_سائز);
		mutex_unlock(ڈیٹا, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_سائز, -1,
				       retval);
		اگر (err_buf)
			ext2fs_free_mem(&err_buf);
	} ورنہ
		mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس retval;
#اگر_ختم /* NO_IO_CACHE */
}

ساکن errcode_t unix_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	واپس unix_read_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t unix_write_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	ساخت unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحیح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_تعریف_شدہ NO_IO_CACHE
	واپس raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf, 0);
#ورنہ
	اگر (ڈیٹا->flags & IO_FLAG_NOCACHE)
		واپس raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	اگر (گنتی < 0 || گنتی > WRITE_براہ_راست_سائز) {
		اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا,
						  FLUSH_INVALIDATE)))
			واپس retval;
		واپس raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	اگر (writethrough)
		retval = raw_write_blk(channel, ڈیٹا, بلاک, گنتی, buf, 0);

	cp = buf;
	mutex_تالا(ڈیٹا, CACHE_MTX);
	جب (گنتی > 0) {
		cache = ڈھونڈو_cached_بلاک(ڈیٹا, بلاک, &reuse);
		اگر (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, ڈیٹا, cache, بلاک);
			اگر (err)
				جاؤ call_write_handler;
		}
		اگر (cache->buf != cp)
			حافظہ_نقل(cache->buf, cp, channel->بلاک_سائز);
		cache->dirty = !writethrough;
		گنتی--;
		بلاک++;
		cp += channel->بلاک_سائز;
	}
	mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس retval;

call_write_handler:
	اگر (cache->write_err && channel->write_error) {
		حرف *err_buf = تہی;
		غیرعلامتی طویل طویل err_بلاک = cache->بلاک;

		cache->dirty = 0;
		cache->اندر_use = 0;
		cache->write_err = 0;
		اگر (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = تہی;
		ورنہ
			حافظہ_نقل(err_buf, cache->buf, channel->بلاک_سائز);
		mutex_unlock(ڈیٹا, CACHE_MTX);
		(channel->write_error)(channel, err_بلاک, 1, err_buf,
				       channel->بلاک_سائز, -1,
				       retval);
		اگر (err_buf)
			ext2fs_free_mem(&err_buf);
	} ورنہ
		mutex_unlock(ڈیٹا, CACHE_MTX);
	واپس retval;
#اگر_ختم /* NO_IO_CACHE */
}

ساکن errcode_t unix_cache_readahead(io_channel channel,
				      غیرعلامتی طویل طویل بلاک,
				      غیرعلامتی طویل طویل گنتی)
{
#اگر_تعریف_شدہ POSIX_FADV_WILLNEED
	ساخت unix_نجی_ڈیٹا *ڈیٹا;

	ڈیٹا = (ساخت unix_نجی_ڈیٹا *)channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	واپس posix_fadvise(ڈیٹا->dev,
			     (ext2_loff_t)بلاک * channel->بلاک_سائز + ڈیٹا->انحراف,
			     (ext2_loff_t)گنتی * channel->بلاک_سائز,
			     POSIX_FADV_WILLNEED);
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

ساکن errcode_t unix_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	واپس unix_write_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t unix_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				 صحیح سائز, ثابت خالی *buf)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->align != 0) {
#اگر_تعریف_شدہ ALIGN_DEBUG
		لکھو("unix_write_byte: O_DIRECT fallback\n");
#اگر_ختم
		واپس EXT2_ET_UNIMPLEMENTED;
	}

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	اگر ((retval = flush_cached_بلاکیں(channel, ڈیٹا, FLUSH_INVALIDATE)))
		واپس retval;
#اگر_ختم

	اگر (lseek(ڈیٹا->dev, انحراف + ڈیٹا->انحراف, SEEK_SET) < 0)
		واپس errno;

	actual = اكتب(ڈیٹا->dev, buf, سائز);
	اگر (actual < 0)
		واپس errno;
	اگر (actual != سائز)
		واپس EXT2_ET_SHORT_WRITE;

	واپس 0;
}

/*
 * Flush data buffers to disk.
 */
ساکن errcode_t unix_flush(io_channel channel)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	retval = flush_cached_بلاکیں(channel, ڈیٹا, 0);
#اگر_ختم
#اگر_تعریف_شدہ HAVE_FSYNC
	اگر (!retval && fsync(ڈیٹا->dev) != 0)
		واپس errno;
#اگر_ختم
	واپس retval;
}

ساکن errcode_t unix_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	غیرعلامتی طویل طویل tmp;
	errcode_t retval;
	حرف *انتہا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!strcmp(option, "offset")) {
		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;

		tmp = strtoull(arg, &انتہا, 0);
		اگر (*انتہا)
			واپس EXT2_ET_INVALID_دلیل;
		ڈیٹا->انحراف = tmp;
		اگر (ڈیٹا->انحراف < 0)
			واپس EXT2_ET_INVALID_دلیل;
		واپس 0;
	}
	اگر (!strcmp(option, "cache")) {
		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;
		اگر (!strcmp(arg, "on")) {
			ڈیٹا->flags &= ~IO_FLAG_NOCACHE;
			واپس 0;
		}
		اگر (!strcmp(arg, "off")) {
			retval = flush_cached_بلاکیں(channel, ڈیٹا, 0);
			ڈیٹا->flags |= IO_FLAG_NOCACHE;
			واپس retval;
		}
		واپس EXT2_ET_INVALID_دلیل;
	}
#اگر_غیر_تعریف_شدہ NO_IO_CACHE
	اگر (!strcmp(option, "cache_blocks")) {
		غیرعلامتی طویل طویل	سائز;

		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;

		errno = 0;
		سائز = strtoll(arg, تہی, 0);
		اگر (errno || سائز == 0 || سائز > INT32_MAX)
			واپس EXT2_ET_INVALID_دلیل;

		اگر (ڈیٹا->cache_سائز == سائز)
			واپس 0;
		اگر (ڈیٹا->cache_سائز > سائز)
			واپس shrink_cache(channel, ڈیٹا, سائز);
		واپس grow_cache(channel, ڈیٹا, سائز);
	}
#اگر_ختم
	واپس EXT2_ET_INVALID_دلیل;
}

#اگر defined(__linux__) && !defined(BLKDISCARD)
#تعریف_کرو BLKDISCARD		_IO(0x12,119)
#اگر_ختم

ساکن errcode_t unix_discard(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	صحیح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->flags & CHANNEL_FLAGS_NODISCARD)
		جاؤ unimplemented;

	اگر (channel->flags & CHANNEL_FLAGS_بلاک_DEVICE) {
#اگر_تعریف_شدہ BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(بلاک) * channel->بلاک_سائز + ڈیٹا->انحراف;
		range[1] = (__u64)(گنتی) * channel->بلاک_سائز;

		ret = ioctl(ڈیٹا->dev, BLKDISCARD, &range);
#ورنہ
		جاؤ unimplemented;
#اگر_ختم
	} ورنہ {
#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(ڈیٹا->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(بلاک) * channel->بلاک_سائز + ڈیٹا->انحراف,
				(off_t)(گنتی) * channel->بلاک_سائز);
#ورنہ
		جاؤ unimplemented;
#اگر_ختم
	}
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			جاؤ unimplemented;
		}
		واپس errno;
	}
	واپس 0;
unimplemented:
	واپس EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ساکن صحیح __unix_zeroout(صحیح fd, off_t انحراف, off_t len)
{
	صحیح ret = -1;

#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, انحراف, len);
	اگر (ret == 0)
		واپس 0;
#اگر_ختم
#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			انحراف,  len);
	اگر (ret == 0)
		واپس 0;
#اگر_ختم
	errno = EOPNOTSUPP;
	واپس ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wunused-parameter"
#اگر_ختم
ساکن errcode_t unix_zeroout(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت unix_نجی_ڈیٹا *ڈیٹا;
	صحیح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت unix_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!(channel->flags & CHANNEL_FLAGS_بلاک_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		ساخت stat statbuf;

		اگر (گنتی == 0)
			واپس 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(ڈیٹا->dev, &statbuf);
		اگر (ret)
			جاؤ err;
		اگر ((غیرعلامتی طویل طویل) statbuf.st_سائز <
			(بلاک + گنتی) * channel->بلاک_سائز + ڈیٹا->انحراف) {
			ret = ftruncate(ڈیٹا->dev,
					(بلاک + گنتی) * channel->بلاک_سائز + ڈیٹا->انحراف);
			اگر (ret)
				جاؤ err;
		}
	}

	اگر (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		جاؤ unimplemented;

	ret = __unix_zeroout(ڈیٹا->dev,
			(off_t)(بلاک) * channel->بلاک_سائز + ڈیٹا->انحراف,
			(off_t)(گنتی) * channel->بلاک_سائز);
err:
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			جاؤ unimplemented;
		}
		واپس errno;
	}
	واپس 0;
unimplemented:
	واپس EXT2_ET_UNIMPLEMENTED;
}
#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic نکالو
#اگر_ختم

ساکن ساخت struct_io_manager struct_unix_manager = {
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

ساکن ساخت struct_io_manager struct_unixfd_manager = {
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
