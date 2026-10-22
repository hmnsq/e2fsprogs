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
#تعریف_کن XOPEN_منبع 600
#تعریف_کن DARWIN_C_منبع
#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر
#اگر_تعریف_نشده GNU_منبع
#تعریف_کن GNU_منبع
#پایان_اگر
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#اگر_تعریف_شده __linux__
#ضمن <sys/utsname.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#پایان_اگر
#إذا HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_PTHREAD
#ضمن <pthread.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#پایان_اگر

#إذا defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#تعریف_کن BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#پایان_اگر

#لغو_تعریف ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح پیدا_کن_آخرین_بیت_مجموعه(صحيح x)
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
	أرجع پیدا_کن_آخرین_بیت_مجموعه(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)

بنية unix_cache {
	حرف			*buf;
	بیعلامت دراز دراز	بلوک;
	صحيح			access_زمان;
	بیعلامت		dirty:1;
	بیعلامت		درون_use:1;
	بیعلامت		write_err:1;
};

#تعریف_کن DEFAULT_CACHE_اندازه 8
#تعریف_کن WRITE_مستقیم_اندازه 4	/* Must be smaller than CACHE_SIZE */
#تعریف_کن READ_مستقیم_اندازه 4	/* Should be smaller than CACHE_SIZE */

بنية unix_خصوصی_داده {
	صحيح	magic;
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_زمان;
	صحيح	unix_flock_flags;
	ext2_loff_t جابه_جایی;
	بنية unix_cache *cache;
	بیعلامت صحيح cache_اندازه;
	بیعلامت صحيح cache_hash_لغزش;
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
#اگر_تعریف_شده HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#پایان_اگر
};

#تعریف_کن IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعريفنوع تعداد قفل_نوع {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} نوع_t;

#اگر_تعریف_شده HAVE_PTHREAD
ساكن مضمن pthread_mutex_t *get_mutex(بنية unix_خصوصی_داده *داده,
					 نوع_t نوع)
{
	إذا (داده->flags & IO_FLAG_نخها) {
		بدل (نوع) {
		حالة CACHE_MTX:
			أرجع &داده->cache_mutex;
		حالة BOUNCE_MTX:
			أرجع &داده->bounce_mutex;
		حالة STATS_MTX:
			أرجع &داده->stats_mutex;
		}
	}
	أرجع عدم;
}
#پایان_اگر

ساكن مضمن فراغ mutex_قفل(بنية unix_خصوصی_داده *داده, نوع_t نوع)
{
#اگر_تعریف_شده HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(داده,نوع);

	إذا (mtx)
		pthread_mutex_قفل(mtx);
#پایان_اگر
}

ساكن مضمن فراغ mutex_unlock(بنية unix_خصوصی_داده *داده, نوع_t نوع)
{
#اگر_تعریف_شده HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(داده,نوع);

	إذا (mtx)
		pthread_mutex_unlock(mtx);
#پایان_اگر
}

ساكن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية unix_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (stats) {
		mutex_قفل(داده, STATS_MTX);
		*stats = &داده->io_stats;
		mutex_unlock(داده, STATS_MTX);
	}

	أرجع retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_خصوصی_داده *داده,
			      بیعلامت دراز دراز بلوک,
			      صحيح شمار, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		اندازه;
	ext2_loff_t	location;
	صحيح		actual = 0;
	بیعلامت حرف	*buf = bufv;
	ssize_t		really_read = 0;
	بیعلامت دراز دراز aligned_blk;
	صحيح		align_اندازه, جابه_جایی;

	اندازه = (شمار < 0) ? -شمار : (ext2_loff_t) شمار * channel->بلوک_اندازه;
	mutex_قفل(داده, STATS_MTX);
	داده->io_stats.بایتها_read += اندازه;
	mutex_unlock(داده, STATS_MTX);
	location = ((ext2_loff_t) بلوک * channel->بلوک_اندازه) + داده->جابه_جایی;

	إذا (داده->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#اگر_تعریف_شده HAVE_PREAD64
	/* Try an aligned pread */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		actual = pread64(داده->dev, buf, اندازه, location);
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
		actual = pread(داده->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
		actual = 0;
	}
#پایان_اگر /* HAVE_PREAD */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		mutex_قفل(داده, BOUNCE_MTX);
		إذا (ext2fs_llseek(داده->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(داده->dev, buf, اندازه);
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

#اگر_تعریف_شده ALIGN_DEBUG
	چاپ_کن("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بیعلامت دراز) اندازه);
#پایان_اگر

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلوک_اندازه > channel->align) &&
	    (channel->بلوک_اندازه % channel->align) == 0)
		align_اندازه = channel->بلوک_اندازه;
	وإلا
		align_اندازه = channel->align;
	aligned_blk = location / align_اندازه;
	جابه_جایی = location % align_اندازه;

	mutex_قفل(داده, BOUNCE_MTX);
	إذا (ext2fs_llseek(داده->dev, aligned_blk * align_اندازه, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	طالما (اندازه > 0) {
		actual = اقرأ(داده->dev, داده->bounce, align_اندازه);
		إذا (actual != align_اندازه) {
			actual = really_read;
			buf -= really_read;
			اندازه += really_read;
			اذهب short_read;
		}
		إذا ((actual + جابه_جایی) > align_اندازه)
			actual = align_اندازه - جابه_جایی;
		إذا (actual > اندازه)
			actual = اندازه;
		کپی_حافظه(buf, (حرف *)داده->bounce + جابه_جایی, actual);

		really_read += actual;
		اندازه -= actual;
		buf += actual;
		جابه_جایی = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(داده, BOUNCE_MTX);
	أرجع 0;

error_unlock:
	mutex_unlock(داده, BOUNCE_MTX);
	إذا (actual >= 0 && actual < اندازه)
		memset((حرف *) buf+actual, 0, اندازه-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, بلوک, شمار, buf,
					       اندازه, actual, retval);
	أرجع retval;
}

#تعریف_کن RAW_WRITE_NO_HANDLER	(1U << 0)
#تعریف_کن RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_خصوصی_داده *داده,
			       بیعلامت دراز دراز بلوک,
			       صحيح شمار, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		اندازه;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت بیعلامت حرف *buf = bufv;
	بیعلامت دراز دراز aligned_blk;
	صحيح		align_اندازه, جابه_جایی;

	إذا (شمار == 1)
		اندازه = channel->بلوک_اندازه;
	وإلا {
		إذا (شمار < 0)
			اندازه = -شمار;
		وإلا
			اندازه = (ext2_loff_t) شمار * channel->بلوک_اندازه;
	}
	mutex_قفل(داده, STATS_MTX);
	داده->io_stats.بایتها_written += اندازه;
	mutex_unlock(داده, STATS_MTX);

	location = ((ext2_loff_t) بلوک * channel->بلوک_اندازه) + داده->جابه_جایی;

	إذا (داده->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#اگر_تعریف_شده HAVE_PWRITE64
	/* Try an aligned pwrite */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		actual = pwrite64(داده->dev, buf, اندازه, location);
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
		actual = pwrite(داده->dev, buf, اندازه, location);
		إذا (actual == اندازه)
			أرجع 0;
	}
#پایان_اگر /* HAVE_PWRITE */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(اندازه, channel->align))) {
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_قفل(داده, BOUNCE_MTX);
		إذا (ext2fs_llseek(داده->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(داده->dev, buf, اندازه);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(داده, BOUNCE_MTX);
		إذا (actual < 0) {
			retval = errno;
			اذهب error_بیرون;
		}
		إذا (actual != اندازه) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_بیرون;
		}
		أرجع 0;
	}

#اگر_تعریف_شده ALIGN_DEBUG
	چاپ_کن("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (بیعلامت دراز) اندازه);
#پایان_اگر
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->بلوک_اندازه > channel->align) &&
	    (channel->بلوک_اندازه % channel->align) == 0)
		align_اندازه = channel->بلوک_اندازه;
	وإلا
		align_اندازه = channel->align;
	aligned_blk = location / align_اندازه;
	جابه_جایی = location % align_اندازه;

	طالما (اندازه > 0) {
		صحيح actual_w;

		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_قفل(داده, BOUNCE_MTX);
		إذا (اندازه < align_اندازه || جابه_جایی) {
			إذا (ext2fs_llseek(داده->dev, aligned_blk * align_اندازه,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(داده->dev, داده->bounce,
				      align_اندازه);
			إذا (actual != align_اندازه) {
				إذا (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) داده->bounce + actual, 0,
				       align_اندازه - actual);
			}
		}
		actual = اندازه;
		إذا ((actual + جابه_جایی) > align_اندازه)
			actual = align_اندازه - جابه_جایی;
		إذا (actual > اندازه)
			actual = اندازه;
		کپی_حافظه(((حرف *)داده->bounce) + جابه_جایی, buf, actual);
		إذا (ext2fs_llseek(داده->dev, aligned_blk * align_اندازه, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(داده->dev, داده->bounce, align_اندازه);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(داده, BOUNCE_MTX);
		إذا (actual_w < 0) {
			retval = errno;
			اذهب error_بیرون;
		}
		إذا (actual_w != align_اندازه)
			اذهب short_write;
		اندازه -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		جابه_جایی = 0;
	}
	أرجع 0;

error_unlock:
	إذا (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(داده, BOUNCE_MTX);
error_بیرون:
	إذا (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, بلوک, شمار, buf,
						اندازه, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية unix_خصوصی_داده *داده)
{
	errcode_t		retval;
	بنية unix_cache	*cache;
	بیعلامت صحيح		i;

	داده->access_زمان = 0;
	لكل (i=0, cache = داده->cache; i < داده->cache_اندازه; i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval)
			أرجع retval;
	}
	إذا (channel->align || داده->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (داده->bounce)
			ext2fs_free_mem(&داده->bounce);
		retval = io_channel_alloc_buf(channel, 0, &داده->bounce);
	}
	أرجع retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية unix_خصوصی_داده *داده)
{
	بنية unix_cache	*cache;
	بیعلامت صحيح		i;

	داده->access_زمان = 0;
	لكل (i=0, cache = داده->cache; i < داده->cache_اندازه; i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (داده->bounce)
		ext2fs_free_mem(&داده->bounce);
}

#اگر_تعریف_نشده NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#تعریف_کن GOLDEN_نسبت_اول	0x9e37fffffffc0001UL
#اگر_تعریف_نشده CACHE_سطر_اندازه
/* if the system didn't tell us, guess something reasonable */
#تعریف_کن CACHE_سطر_اندازه		64
#پایان_اگر

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن بیعلامت صحيح
cache_hash(بنية unix_خصوصی_داده *داده, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	إذا (داده->cache_اندازه <= DEFAULT_CACHE_اندازه)
		أرجع 0;

	tmp = hashval ^ (GOLDEN_نسبت_اول + hashval) / CACHE_سطر_اندازه;
	tmp = tmp ^ ((tmp ^ GOLDEN_نسبت_اول) >> داده->cache_hash_لغزش);
	أرجع tmp % داده->cache_اندازه;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية unix_cache *پیدا_کن_cached_بلوک(بنية unix_خصوصی_داده *داده,
					    بیعلامت دراز دراز بلوک,
					    بنية unix_cache **eldest)
{
	بنية unix_cache	*cache, *استفادهنشده_cache, *oldest_cache;
	بیعلامت صحيح		hash = cache_hash(داده, بلوک);
	بیعلامت صحيح		i;

	استفادهنشده_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	لكل (i = hash, cache = داده->cache + hash;
	     i < داده->cache_اندازه;
	     i++, cache++) {
		إذا (!cache->درون_use) {
			إذا (!استفادهنشده_cache)
				استفادهنشده_cache = cache;
			استمر;
		}
		إذا (cache->بلوک == بلوک) {
			cache->access_زمان = ++داده->access_زمان;
			داده->io_stats.cache_اصابتها++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_زمان < oldest_cache->access_زمان))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	لكل (i = 0, cache = داده->cache; i < hash; i++, cache++) {
		إذا (!cache->درون_use) {
			إذا (!استفادهنشده_cache)
				استفادهنشده_cache = cache;
			استمر;
		}
		إذا (cache->بلوک == بلوک) {
			cache->access_زمان = ++داده->access_زمان;
			داده->io_stats.cache_اصابتها++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_زمان < oldest_cache->access_زمان))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (استفادهنشده_cache) ? استفادهنشده_cache : oldest_cache;
	داده->io_stats.cache_ناکامیها++;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_خصوصی_داده *داده, بنية unix_cache *cache,
		بیعلامت دراز دراز بلوک)
{
	إذا (cache->dirty && cache->درون_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, داده, cache->بلوک, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		إذا (retval) {
			cache->write_err = 1;
			أرجع retval;
		}
	}

	cache->درون_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->بلوک = بلوک;
	cache->access_زمان = ++داده->access_زمان;
	أرجع 0;
}

#تعریف_کن FLUSH_INVALIDATE	0x01
#تعریف_کن FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_بلوکها(io_channel channel,
				     بنية unix_خصوصی_داده *داده,
				     صحيح flags)
{
	بنية unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	بیعلامت صحيح		i;
	صحيح			errors_یافت = 0;

	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_قفل(داده, CACHE_MTX);
	لكل (i=0, cache = داده->cache; i < داده->cache_اندازه; i++, cache++) {
		إذا (!cache->درون_use)
			استمر;
		إذا (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			إذا (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, داده,
					       cache->بلوک, 1, cache->buf,
					       raw_flags);
			إذا (retval) {
				cache->write_err = 1;
				errors_یافت = 1;
				retval2 = retval;
			} وإلا {
				cache->dirty = 0;
				cache->write_err = 0;
				إذا (flags & FLUSH_INVALIDATE)
					cache->درون_use = 0;
			}
		} وإلا إذا (flags & FLUSH_INVALIDATE) {
			cache->درون_use = 0;
		}
	}
	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(داده, CACHE_MTX);
retry:
	طالما (errors_یافت) {
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_قفل(داده, CACHE_MTX);
		errors_یافت = 0;
		لكل (i=0, cache = داده->cache; i < داده->cache_اندازه; i++, cache++) {
			إذا (!cache->درون_use || !cache->write_err)
				استمر;
			errors_یافت = 1;
			إذا (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				بیعلامت دراز دراز err_بلوک = cache->بلوک;

				cache->dirty = 0;
				cache->درون_use = 0;
				cache->write_err = 0;
				إذا (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				وإلا
					کپی_حافظه(err_buf, cache->buf,
					       channel->بلوک_اندازه);
				mutex_unlock(داده, CACHE_MTX);
				(channel->write_error)(channel, err_بلوک,
					1, err_buf, channel->بلوک_اندازه, -1,
					retval2);
				إذا (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_قفل(داده, CACHE_MTX);
				اذهب retry;
			} وإلا
				cache->write_err = 0;
		}
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(داده, CACHE_MTX);
	}
	أرجع retval2;
}

/* Shrink the cache buffers */
ساكن errcode_t shrink_cache(io_channel channel,
			      بنية unix_خصوصی_داده *داده,
			      بیعلامت صحيح new_اندازه)
{
	بنية unix_cache	*cache, *new_cache;
	بیعلامت صحيح		i;
	errcode_t		retval;

	mutex_قفل(داده, CACHE_MTX);

	retval = flush_cached_بلوکها(channel, داده,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	لكل (i = new_اندازه, cache = داده->cache + new_اندازه;
	     i < داده->cache_اندازه;
	     i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(داده->cache, new_اندازه * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب unlock;
	}

	داده->cache = new_cache;
	داده->cache_اندازه = new_اندازه;
	داده->cache_hash_لغزش = highbit32(داده->cache_اندازه);

unlock:
	mutex_unlock(داده, CACHE_MTX);
	أرجع retval;
}

/* Grow the cache buffers */
ساكن errcode_t grow_cache(io_channel channel,
			    بنية unix_خصوصی_داده *داده,
			    بیعلامت صحيح new_اندازه)
{
	بنية unix_cache	*cache, *new_cache;
	بیعلامت صحيح		i;
	errcode_t		retval;

	mutex_قفل(داده, CACHE_MTX);

	retval = flush_cached_بلوکها(channel, داده,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	new_cache = realloc(داده->cache, new_اندازه * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب unlock;
	}
	داده->cache = new_cache;

	لكل (i = داده->cache_اندازه, cache = new_cache + داده->cache_اندازه;
	     i < new_اندازه;
	     i++, cache++) {
		cache->بلوک = 0;
		cache->access_زمان = 0;
		cache->dirty = 0;
		cache->درون_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval) {
			طالما (i > داده->cache_اندازه) {
				cache--; i--;
				إذا (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	داده->cache_اندازه = new_اندازه;
	داده->cache_hash_لغزش = highbit32(داده->cache_اندازه);

unlock:
	mutex_unlock(داده, CACHE_MTX);
	أرجع retval;
}
#پایان_اگر /* NO_IO_CACHE */

#اگر_تعریف_شده __linux__
#اگر_تعریف_نشده BLKDISCARDZEROES
#تعریف_کن BLKDISCARDZEROES _IO(0x12,124)
#پایان_اگر
#پایان_اگر

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t نما)
{
	إذا (نما)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, نما);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, نما);
	وإلا
		أرجع open(pathname, flags);
#پایان_اگر
}

صحيح ext2fs_stat(ثابت حرف *مسیر, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(مسیر, buf);
#وإلا
	أرجع stat(مسیر, buf);
#پایان_اگر
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#پایان_اگر
}

#اگر_تعریف_شده HAVE_SYS_FILE_H
ساكن errcode_t unix_funlock(io_channel channel)
{
	بنية unix_خصوصی_داده *داده;
	صحيح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (داده->unix_flock_flags) {
		ret = flock(داده->dev, LOCK_UN);
		إذا (ret)
			أرجع errno;

		داده->unix_flock_flags = 0;
	}

	أرجع 0;
}

ساكن errcode_t unix_flock(io_channel channel, بیعلامت صحيح flock_flags)
{
	بنية unix_خصوصی_داده *داده;
	صحيح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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

	ret = flock(داده->dev, unix_flock_flags);
	إذا (ret < 0)
		أرجع errno;

	داده->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	أرجع 0;
}
#وإلا
#تعریف_کن unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	أرجع 0;
}
#پایان_اگر /* HAVE_SYS_FILE_H */

ساكن errcode_t unix_open_channel(ثابت حرف *name, صحيح fd,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	بنية unix_خصوصی_داده *داده = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;
#اگر_تعریف_شده __linux__
	بنية		utsname ut;
#پایان_اگر

	إذا (ext2fs_امن_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#اگر_تعریف_شده __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#پایان_اگر

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_خصوصی_داده), &داده);
	إذا (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(طول_رشته(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->خصوصی_داده = داده;
	io->بلوک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	إذا (ext2fs_امن_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(داده, 0, حجم(بنية unix_خصوصی_داده));
	داده->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	داده->io_stats.رقم_fields = 4;
	داده->flags = flags;
	داده->dev = fd;

	داده->cache_اندازه = DEFAULT_CACHE_اندازه;
	داده->cache_hash_لغزش = highbit32(داده->cache_اندازه);
	داده->cache = حافظه_صفر_بده(DEFAULT_CACHE_اندازه, حجم(بنية unix_cache));
	إذا (!داده->cache) {
		retval = EXT2_ET_NO_حافظه;
		اذهب cleanup;
	}

#إذا defined(O_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = ext2fs_get_dio_alignment(داده->dev);
#وإلاإذا defined(F_NOCACHE)
	إذا (flags & IO_FLAG_مستقیم_IO)
		io->align = 4096;
#پایان_اگر

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(داده->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_نما)) {
#اگر_تعریف_شده BLKDISCARDZEROES
			صحيح zeroes = 0;

			إذا (ioctl(داده->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#پایان_اگر
			io->flags |= CHANNEL_FLAGS_بلوک_DEVICE;
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
#پایان_اگر

#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	إذا (io->flags & CHANNEL_FLAGS_بلوک_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		إذا (io->align < dio_align)
			io->align = dio_align;
	}
#پایان_اگر

	إذا ((retval = alloc_cache(io, داده)))
		اذهب cleanup;

#اگر_تعریف_شده BLKROGET
	إذا (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(داده->dev, BLKROGET, &readonly);
		إذا (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#پایان_اگر

#اگر_تعریف_شده __linux__
#لغو_تعریف RLIM_INFINITY
#إذا (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#تعریف_کن RLIM_INFINITY	((بیعلامت دراز)(~0UL>>1))
#وإلا
#تعریف_کن RLIM_INFINITY  (~0UL)
#پایان_اگر
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
	    (ext2fs_fstat(داده->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_نما))) {
		بنية rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (بیعلامت دراز) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		إذا (((بیعلامت دراز) rlim.rlim_cur) <
		    ((بیعلامت دراز) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#پایان_اگر
#اگر_تعریف_شده HAVE_PTHREAD
	إذا (flags & IO_FLAG_نخها) {
		io->flags |= CHANNEL_FLAGS_نخها;
		retval = pthread_mutex_init(&داده->cache_mutex, عدم);
		إذا (retval)
			اذهب cleanup;
		retval = pthread_mutex_init(&داده->bounce_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&داده->cache_mutex);
			اذهب cleanup;
		}
		retval = pthread_mutex_init(&داده->stats_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&داده->cache_mutex);
			pthread_mutex_destroy(&داده->bounce_mutex);
			اذهب cleanup;
		}
	}
#پایان_اگر
	*channel = io;
	أرجع 0;

cleanup:
	إذا (داده) {
		unix_funlock(io);
		إذا (io->manager != unixfd_io_manager && داده->dev >= 0)
			close(داده->dev);
		إذا (داده->cache) {
			free_cache(داده);
			حرر(داده->cache);
		}
		ext2fs_free_mem(&داده);
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
#پایان_اگر
#پایان_اگر  /* HAVE_FCNTL */

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
#پایان_اگر
	fd = ext2fs_open_file(name, open_flags, 0);
	إذا (fd < 0)
		أرجع errno;
#إذا defined(F_NOCACHE) && !defined(IO_مستقیم)
	إذا (flags & IO_FLAG_مستقیم_IO) {
		إذا (fcntl(fd, F_NOCACHE, 1) < 0)
			أرجع errno;
	}
#پایان_اگر
	أرجع unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#اگر_تعریف_نشده NO_IO_CACHE
	retval = flush_cached_بلوکها(channel, داده, 0);
#پایان_اگر

	unix_funlock(channel);

	إذا (channel->manager != unixfd_io_manager && close(داده->dev) < 0)
		retval = errno;
	free_cache(داده);
	حرر(داده->cache);
#اگر_تعریف_شده HAVE_PTHREAD
	إذا (داده->flags & IO_FLAG_نخها) {
		pthread_mutex_destroy(&داده->cache_mutex);
		pthread_mutex_destroy(&داده->bounce_mutex);
		pthread_mutex_destroy(&داده->stats_mutex);
	}
#پایان_اگر

	ext2fs_free_mem(&channel->خصوصی_داده);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t unix_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_خصوصی_داده *داده;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->بلوک_اندازه != blksize) {
		mutex_قفل(داده, CACHE_MTX);
		mutex_قفل(داده, BOUNCE_MTX);
#اگر_تعریف_نشده NO_IO_CACHE
		إذا ((retval = flush_cached_بلوکها(channel, داده, FLUSH_NOLOCK))){
			mutex_unlock(داده, BOUNCE_MTX);
			mutex_unlock(داده, CACHE_MTX);
			أرجع retval;
		}
#پایان_اگر

		channel->بلوک_اندازه = blksize;
		free_cache(داده);
		retval = alloc_cache(channel, داده);
		mutex_unlock(داده, BOUNCE_MTX);
		mutex_unlock(داده, CACHE_MTX);
	}
	أرجع retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	بنية unix_خصوصی_داده *داده;
	بنية unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_تعریف_شده NO_IO_CACHE
	أرجع raw_read_blk(channel, داده, بلوک, شمار, buf);
#وإلا
	إذا (داده->flags & IO_FLAG_NOCACHE)
		أرجع raw_read_blk(channel, داده, بلوک, شمار, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (شمار < 0 || شمار > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلوکها(channel, داده, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, داده, بلوک, شمار, buf);
	}

	cp = buf;
	mutex_قفل(داده, CACHE_MTX);
	طالما (شمار > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = پیدا_کن_cached_بلوک(داده, بلوک, عدم))) {
#اگر_تعریف_شده DEBUG
			چاپ_کن("Using cached block %lu\n", بلوک);
#پایان_اگر
			کپی_حافظه(cp, cache->buf, channel->بلوک_اندازه);
			شمار--;
			بلوک++;
			cp += channel->بلوک_اندازه;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < شمار; i++)
			إذا (پیدا_کن_cached_بلوک(داده, بلوک+i, عدم))
				اكسر;
#اگر_تعریف_شده DEBUG
		چاپ_کن("Reading %d blocks starting at %lu\n", i, بلوک);
#پایان_اگر
		mutex_unlock(داده, CACHE_MTX);
		إذا ((retval = raw_read_blk(channel, داده, بلوک, i, cp)))
			أرجع retval;
		mutex_قفل(داده, CACHE_MTX);

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			إذا (!پیدا_کن_cached_بلوک(داده, بلوک, &cache)) {
				retval = reuse_cache(channel, داده,
						     cache, بلوک);
				إذا (retval)
					اذهب call_write_handler;
				کپی_حافظه(cache->buf, cp, channel->بلوک_اندازه);
			}
			شمار--;
			بلوک++;
			cp += channel->بلوک_اندازه;
		}
	}
	mutex_unlock(داده, CACHE_MTX);
	أرجع 0;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بیعلامت دراز دراز err_بلوک = cache->بلوک;

		cache->dirty = 0;
		cache->درون_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			کپی_حافظه(err_buf, cache->buf, channel->بلوک_اندازه);
		mutex_unlock(داده, CACHE_MTX);
		(channel->write_error)(channel, err_بلوک, 1, err_buf,
				       channel->بلوک_اندازه, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(داده, CACHE_MTX);
	أرجع retval;
#پایان_اگر /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	أرجع unix_read_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	بنية unix_خصوصی_داده *داده;
	بنية unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_تعریف_شده NO_IO_CACHE
	أرجع raw_write_blk(channel, داده, بلوک, شمار, buf, 0);
#وإلا
	إذا (داده->flags & IO_FLAG_NOCACHE)
		أرجع raw_write_blk(channel, داده, بلوک, شمار, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (شمار < 0 || شمار > WRITE_مستقیم_اندازه) {
		إذا ((retval = flush_cached_بلوکها(channel, داده,
						  FLUSH_INVALIDATE)))
			أرجع retval;
		أرجع raw_write_blk(channel, داده, بلوک, شمار, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, داده, بلوک, شمار, buf, 0);

	cp = buf;
	mutex_قفل(داده, CACHE_MTX);
	طالما (شمار > 0) {
		cache = پیدا_کن_cached_بلوک(داده, بلوک, &reuse);
		إذا (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, داده, cache, بلوک);
			إذا (err)
				اذهب call_write_handler;
		}
		إذا (cache->buf != cp)
			کپی_حافظه(cache->buf, cp, channel->بلوک_اندازه);
		cache->dirty = !writethrough;
		شمار--;
		بلوک++;
		cp += channel->بلوک_اندازه;
	}
	mutex_unlock(داده, CACHE_MTX);
	أرجع retval;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		بیعلامت دراز دراز err_بلوک = cache->بلوک;

		cache->dirty = 0;
		cache->درون_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			کپی_حافظه(err_buf, cache->buf, channel->بلوک_اندازه);
		mutex_unlock(داده, CACHE_MTX);
		(channel->write_error)(channel, err_بلوک, 1, err_buf,
				       channel->بلوک_اندازه, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(داده, CACHE_MTX);
	أرجع retval;
#پایان_اگر /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      بیعلامت دراز دراز بلوک,
				      بیعلامت دراز دراز شمار)
{
#اگر_تعریف_شده POSIX_FADV_WILLNEED
	بنية unix_خصوصی_داده *داده;

	داده = (بنية unix_خصوصی_داده *)channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	أرجع posix_fadvise(داده->dev,
			     (ext2_loff_t)بلوک * channel->بلوک_اندازه + داده->جابه_جایی,
			     (ext2_loff_t)شمار * channel->بلوک_اندازه,
			     POSIX_FADV_WILLNEED);
#وإلا
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
#پایان_اگر
}

ساكن errcode_t unix_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	أرجع unix_write_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t unix_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				 صحيح اندازه, ثابت فراغ *buf)
{
	بنية unix_خصوصی_داده *داده;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->align != 0) {
#اگر_تعریف_شده ALIGN_DEBUG
		چاپ_کن("unix_write_byte: O_DIRECT fallback\n");
#پایان_اگر
		أرجع EXT2_ET_UNIMPLEMENTED;
	}

#اگر_تعریف_نشده NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	إذا ((retval = flush_cached_بلوکها(channel, داده, FLUSH_INVALIDATE)))
		أرجع retval;
#پایان_اگر

	إذا (lseek(داده->dev, جابه_جایی + داده->جابه_جایی, SEEK_SET) < 0)
		أرجع errno;

	actual = اكتب(داده->dev, buf, اندازه);
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
	بنية unix_خصوصی_داده *داده;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#اگر_تعریف_نشده NO_IO_CACHE
	retval = flush_cached_بلوکها(channel, داده, 0);
#پایان_اگر
#اگر_تعریف_شده HAVE_FSYNC
	إذا (!retval && fsync(داده->dev) != 0)
		أرجع errno;
#پایان_اگر
	أرجع retval;
}

ساكن errcode_t unix_مجموعه_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية unix_خصوصی_داده *داده;
	بیعلامت دراز دراز tmp;
	errcode_t retval;
	حرف *انتها;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;

		tmp = strtoull(arg, &انتها, 0);
		إذا (*انتها)
			أرجع EXT2_ET_INVALID_آرگومان;
		داده->جابه_جایی = tmp;
		إذا (داده->جابه_جایی < 0)
			أرجع EXT2_ET_INVALID_آرگومان;
		أرجع 0;
	}
	إذا (!strcmp(option, "cache")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;
		إذا (!strcmp(arg, "on")) {
			داده->flags &= ~IO_FLAG_NOCACHE;
			أرجع 0;
		}
		إذا (!strcmp(arg, "off")) {
			retval = flush_cached_بلوکها(channel, داده, 0);
			داده->flags |= IO_FLAG_NOCACHE;
			أرجع retval;
		}
		أرجع EXT2_ET_INVALID_آرگومان;
	}
#اگر_تعریف_نشده NO_IO_CACHE
	إذا (!strcmp(option, "cache_blocks")) {
		بیعلامت دراز دراز	اندازه;

		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;

		errno = 0;
		اندازه = strtoll(arg, عدم, 0);
		إذا (errno || اندازه == 0 || اندازه > INT32_MAX)
			أرجع EXT2_ET_INVALID_آرگومان;

		إذا (داده->cache_اندازه == اندازه)
			أرجع 0;
		إذا (داده->cache_اندازه > اندازه)
			أرجع shrink_cache(channel, داده, اندازه);
		أرجع grow_cache(channel, داده, اندازه);
	}
#پایان_اگر
	أرجع EXT2_ET_INVALID_آرگومان;
}

#إذا defined(__linux__) && !defined(BLKDISCARD)
#تعریف_کن BLKDISCARD		_IO(0x12,119)
#پایان_اگر

ساكن errcode_t unix_discard(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية unix_خصوصی_داده *داده;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	إذا (channel->flags & CHANNEL_FLAGS_بلوک_DEVICE) {
#اگر_تعریف_شده BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(بلوک) * channel->بلوک_اندازه + داده->جابه_جایی;
		range[1] = (__u64)(شمار) * channel->بلوک_اندازه;

		ret = ioctl(داده->dev, BLKDISCARD, &range);
#وإلا
		اذهب unimplemented;
#پایان_اگر
	} وإلا {
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(داده->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(بلوک) * channel->بلوک_اندازه + داده->جابه_جایی,
				(off_t)(شمار) * channel->بلوک_اندازه);
#وإلا
		اذهب unimplemented;
#پایان_اگر
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
ساكن صحيح __unix_zeroout(صحيح fd, off_t جابه_جایی, off_t len)
{
	صحيح ret = -1;

#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, جابه_جایی, len);
	إذا (ret == 0)
		أرجع 0;
#پایان_اگر
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			جابه_جایی,  len);
	إذا (ret == 0)
		أرجع 0;
#پایان_اگر
	errno = EOPNOTSUPP;
	أرجع ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#رهنمود GCC diagnostic افزودن
#رهنمود GCC diagnostic ignored "-Wunused-parameter"
#پایان_اگر
ساكن errcode_t unix_zeroout(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية unix_خصوصی_داده *داده;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية unix_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!(channel->flags & CHANNEL_FLAGS_بلوک_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (شمار == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(داده->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((بیعلامت دراز دراز) statbuf.st_اندازه <
			(بلوک + شمار) * channel->بلوک_اندازه + داده->جابه_جایی) {
			ret = ftruncate(داده->dev,
					(بلوک + شمار) * channel->بلوک_اندازه + داده->جابه_جایی);
			إذا (ret)
				اذهب err;
		}
	}

	إذا (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(داده->dev,
			(off_t)(بلوک) * channel->بلوک_اندازه + داده->جابه_جایی,
			(off_t)(شمار) * channel->بلوک_اندازه);
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
#رهنمود GCC diagnostic برداشت
#پایان_اگر

ساكن بنية struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.مجموعه_blksize	= unix_مجموعه_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.مجموعه_option	= unix_مجموعه_option,
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
	.مجموعه_blksize	= unix_مجموعه_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.مجموعه_option	= unix_مجموعه_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
