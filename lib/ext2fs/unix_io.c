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

#যদি !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#ifdef __linux__
#অন্তর্ভাব <sys/utsname.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#অন্তর্ভাব <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#অন্তর্ভাব <sys/mount.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_RESOURCE_H
#অন্তর্ভাব <sys/resource.h>
#endif
#যদি HAVE_LINUX_FALLOC_H
#অন্তর্ভাব <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#অন্তর্ভাব <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#অন্তর্ভাব <sys/file.h>
#endif

#যদি defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

স্থির অন্তঃরেখ পূর্ণসংখ্যা find_last_bit_set(পূর্ণসংখ্যা x)
{
	পূর্ণসংখ্যা r = 32;

	যদি (!x)
		ফেরত 0;
	যদি (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	যদি (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	যদি (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	যদি (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	যদি (!(x & 0x80000000u)) {
		r -= 1;
	}
	ফেরত r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
স্থির অন্তঃরেখ পূর্ণসংখ্যা highbit32(uint32_t v)
{
	ফেরত find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)

কাঠামো unix_cache {
	অক্ষর			*buf;
	unsigned long long	block;
	পূর্ণসংখ্যা			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_আকার 8
#define WRITE_DIRECT_আকার 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_আকার 4	/* Should be smaller than CACHE_SIZE */

কাঠামো unix_private_তথ্য {
	পূর্ণসংখ্যা	magic;
	পূর্ণসংখ্যা	dev;
	পূর্ণসংখ্যা	flags;
	পূর্ণসংখ্যা	align;
	পূর্ণসংখ্যা	access_time;
	পূর্ণসংখ্যা	unix_flock_flags;
	ext2_loff_t offset;
	কাঠামো unix_cache *cache;
	unsigned পূর্ণসংখ্যা cache_আকার;
	unsigned পূর্ণসংখ্যা cache_hash_shift;
	শূন্য	*bounce;
	কাঠামো struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

প্রকারলেখ গণনা lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
স্থির অন্তঃরেখ pthread_mutex_t *get_mutex(কাঠামো unix_private_তথ্য *তথ্য,
					 kind_t kind)
{
	যদি (তথ্য->flags & IO_FLAG_THREADS) {
		নির্বাচন (kind) {
		ক্ষেত্র CACHE_MTX:
			ফেরত &তথ্য->cache_mutex;
		ক্ষেত্র BOUNCE_MTX:
			ফেরত &তথ্য->bounce_mutex;
		ক্ষেত্র STATS_MTX:
			ফেরত &তথ্য->stats_mutex;
		}
	}
	ফেরত ফাঁকা;
}
#endif

স্থির অন্তঃরেখ শূন্য mutex_lock(কাঠামো unix_private_তথ্য *তথ্য, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(তথ্য,kind);

	যদি (mtx)
		pthread_mutex_lock(mtx);
#endif
}

স্থির অন্তঃরেখ শূন্য mutex_unlock(কাঠামো unix_private_তথ্য *তথ্য, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(তথ্য,kind);

	যদি (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

স্থির errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	কাঠামো unix_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (stats) {
		mutex_lock(তথ্য, STATS_MTX);
		*stats = &তথ্য->io_stats;
		mutex_unlock(তথ্য, STATS_MTX);
	}

	ফেরত retval;
}

/*
 * Here are the raw I/O functions
 */
স্থির errcode_t raw_read_blk(io_channel channel,
			      কাঠামো unix_private_তথ্য *তথ্য,
			      unsigned long long block,
			      পূর্ণসংখ্যা গুনতি, শূন্য *bufv)
{
	errcode_t	retval;
	ssize_t		আকার_2;
	ext2_loff_t	location;
	পূর্ণসংখ্যা		actual = 0;
	unsigned অক্ষর	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	পূর্ণসংখ্যা		align_আকার, offset;

	আকার_2 = (গুনতি < 0) ? -গুনতি : (ext2_loff_t) গুনতি * channel->block_আকার;
	mutex_lock(তথ্য, STATS_MTX);
	তথ্য->io_stats.bytes_read += আকার_2;
	mutex_unlock(তথ্য, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_আকার) + তথ্য->offset;

	যদি (তথ্য->flags & IO_FLAG_FORCE_BOUNCE)
		যাও bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(আকার_2, channel->align))) {
		actual = pread64(তথ্য->dev, buf, আকার_2, location);
		যদি (actual == আকার_2)
			ফেরত 0;
		actual = 0;
	}
#নতুবা_যদি HAVE_PREAD
	/* Try an aligned pread */
	যদি ((আকার(off_t) >= আকার(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(আকার_2, channel->align)))) {
		actual = pread(তথ্য->dev, buf, আকার_2, location);
		যদি (actual == আকার_2)
			ফেরত 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(আকার_2, channel->align))) {
		mutex_lock(তথ্য, BOUNCE_MTX);
		যদি (ext2fs_llseek(তথ্য->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			যাও error_unlock;
		}
		actual = পঠ(তথ্য->dev, buf, আকার_2);
		যদি (actual != আকার_2) {
		short_read:
			যদি (actual < 0) {
				retval = errno;
				actual = 0;
			} অন্যথা
				retval = EXT2_ET_SHORT_READ;
			যাও error_unlock;
		}
		যাও success_unlock;
	}

#ifdef ALIGN_DEBUG
	printf("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) আকার_2);
#endif

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	যদি (channel->align == 0)
		channel->align = 1;
	যদি ((channel->block_আকার > channel->align) &&
	    (channel->block_আকার % channel->align) == 0)
		align_আকার = channel->block_আকার;
	অন্যথা
		align_আকার = channel->align;
	aligned_blk = location / align_আকার;
	offset = location % align_আকার;

	mutex_lock(তথ্য, BOUNCE_MTX);
	যদি (ext2fs_llseek(তথ্য->dev, aligned_blk * align_আকার, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		যাও error_unlock;
	}
	যখন (আকার_2 > 0) {
		actual = পঠ(তথ্য->dev, তথ্য->bounce, align_আকার);
		যদি (actual != align_আকার) {
			actual = really_read;
			buf -= really_read;
			আকার_2 += really_read;
			যাও short_read;
		}
		যদি ((actual + offset) > align_আকার)
			actual = align_আকার - offset;
		যদি (actual > আকার_2)
			actual = আকার_2;
		memcpy(buf, (অক্ষর *)তথ্য->bounce + offset, actual);

		really_read += actual;
		আকার_2 -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(তথ্য, BOUNCE_MTX);
	ফেরত 0;

error_unlock:
	mutex_unlock(তথ্য, BOUNCE_MTX);
	যদি (actual >= 0 && actual < আকার_2)
		memset((অক্ষর *) buf+actual, 0, আকার_2-actual);
	যদি (channel->read_error)
		retval = (channel->read_error)(channel, block, গুনতি, buf,
					       আকার_2, actual, retval);
	ফেরত retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

স্থির errcode_t raw_write_blk(io_channel channel,
			       কাঠামো unix_private_তথ্য *তথ্য,
			       unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *bufv,
			       পূর্ণসংখ্যা flags)
{
	ssize_t		আকার_2;
	ext2_loff_t	location;
	পূর্ণসংখ্যা		actual = 0;
	errcode_t	retval;
	ধ্রুবক unsigned অক্ষর *buf = bufv;
	unsigned long long aligned_blk;
	পূর্ণসংখ্যা		align_আকার, offset;

	যদি (গুনতি == 1)
		আকার_2 = channel->block_আকার;
	অন্যথা {
		যদি (গুনতি < 0)
			আকার_2 = -গুনতি;
		অন্যথা
			আকার_2 = (ext2_loff_t) গুনতি * channel->block_আকার;
	}
	mutex_lock(তথ্য, STATS_MTX);
	তথ্য->io_stats.bytes_written += আকার_2;
	mutex_unlock(তথ্য, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_আকার) + তথ্য->offset;

	যদি (তথ্য->flags & IO_FLAG_FORCE_BOUNCE)
		যাও bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(আকার_2, channel->align))) {
		actual = pwrite64(তথ্য->dev, buf, আকার_2, location);
		যদি (actual == আকার_2)
			ফেরত 0;
	}
#নতুবা_যদি HAVE_PWRITE
	/* Try an aligned pwrite */
	যদি ((আকার(off_t) >= আকার(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(আকার_2, channel->align)))) {
		actual = pwrite(তথ্য->dev, buf, আকার_2, location);
		যদি (actual == আকার_2)
			ফেরত 0;
	}
#endif /* HAVE_PWRITE */

	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(আকার_2, channel->align))) {
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(তথ্য, BOUNCE_MTX);
		যদি (ext2fs_llseek(তথ্য->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			যাও error_unlock;
		}
		actual = লিখ(তথ্য->dev, buf, আকার_2);
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(তথ্য, BOUNCE_MTX);
		যদি (actual < 0) {
			retval = errno;
			যাও error_out;
		}
		যদি (actual != আকার_2) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			যাও error_out;
		}
		ফেরত 0;
	}

#ifdef ALIGN_DEBUG
	printf("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) আকার_2);
#endif
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	যদি (channel->align == 0)
		channel->align = 1;
	যদি ((channel->block_আকার > channel->align) &&
	    (channel->block_আকার % channel->align) == 0)
		align_আকার = channel->block_আকার;
	অন্যথা
		align_আকার = channel->align;
	aligned_blk = location / align_আকার;
	offset = location % align_আকার;

	যখন (আকার_2 > 0) {
		পূর্ণসংখ্যা actual_w;

		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(তথ্য, BOUNCE_MTX);
		যদি (আকার_2 < align_আকার || offset) {
			যদি (ext2fs_llseek(তথ্য->dev, aligned_blk * align_আকার,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				যাও error_unlock;
			}
			actual = পঠ(তথ্য->dev, তথ্য->bounce,
				      align_আকার);
			যদি (actual != align_আকার) {
				যদি (actual < 0) {
					retval = errno;
					যাও error_unlock;
				}
				memset((অক্ষর *) তথ্য->bounce + actual, 0,
				       align_আকার - actual);
			}
		}
		actual = আকার_2;
		যদি ((actual + offset) > align_আকার)
			actual = align_আকার - offset;
		যদি (actual > আকার_2)
			actual = আকার_2;
		memcpy(((অক্ষর *)তথ্য->bounce) + offset, buf, actual);
		যদি (ext2fs_llseek(তথ্য->dev, aligned_blk * align_আকার, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			যাও error_unlock;
		}
		actual_w = লিখ(তথ্য->dev, তথ্য->bounce, align_আকার);
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(তথ্য, BOUNCE_MTX);
		যদি (actual_w < 0) {
			retval = errno;
			যাও error_out;
		}
		যদি (actual_w != align_আকার)
			যাও short_write;
		আকার_2 -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	ফেরত 0;

error_unlock:
	যদি (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(তথ্য, BOUNCE_MTX);
error_out:
	যদি (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, গুনতি, buf,
						আকার_2, actual, retval);
	ফেরত retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
স্থির errcode_t alloc_cache(io_channel channel,
			     কাঠামো unix_private_তথ্য *তথ্য)
{
	errcode_t		retval;
	কাঠামো unix_cache	*cache;
	unsigned পূর্ণসংখ্যা		i;

	তথ্য->access_time = 0;
	জন্য (i=0, cache = তথ্য->cache; i < তথ্য->cache_আকার; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval)
			ফেরত retval;
	}
	যদি (channel->align || তথ্য->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (তথ্য->bounce)
			ext2fs_free_mem(&তথ্য->bounce);
		retval = io_channel_alloc_buf(channel, 0, &তথ্য->bounce);
	}
	ফেরত retval;
}

/* Free the cache buffers */
স্থির শূন্য free_cache(কাঠামো unix_private_তথ্য *তথ্য)
{
	কাঠামো unix_cache	*cache;
	unsigned পূর্ণসংখ্যা		i;

	তথ্য->access_time = 0;
	জন্য (i=0, cache = তথ্য->cache; i < তথ্য->cache_আকার; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	যদি (তথ্য->bounce)
		ext2fs_free_mem(&তথ্য->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_মৌলিক	0x9e37fffffffc0001UL
#ifndef CACHE_সারি_আকার
/* if the system didn't tell us, guess something reasonable */
#define CACHE_সারি_আকার		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
স্থির unsigned পূর্ণসংখ্যা
cache_hash(কাঠামো unix_private_তথ্য *তথ্য, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	যদি (তথ্য->cache_আকার <= DEFAULT_CACHE_আকার)
		ফেরত 0;

	tmp = hashval ^ (GOLDEN_RATIO_মৌলিক + hashval) / CACHE_সারি_আকার;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_মৌলিক) >> তথ্য->cache_hash_shift);
	ফেরত tmp % তথ্য->cache_আকার;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
স্থির কাঠামো unix_cache *find_cached_block(কাঠামো unix_private_তথ্য *তথ্য,
					    unsigned long long block,
					    কাঠামো unix_cache **eldest)
{
	কাঠামো unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned পূর্ণসংখ্যা		hash = cache_hash(তথ্য, block);
	unsigned পূর্ণসংখ্যা		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	জন্য (i = hash, cache = তথ্য->cache + hash;
	     i < তথ্য->cache_আকার;
	     i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			চালু;
		}
		যদি (cache->block == block) {
			cache->access_time = ++তথ্য->access_time;
			তথ্য->io_stats.cache_hits++;
			ফেরত cache;
		}
		যদি (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	জন্য (i = 0, cache = তথ্য->cache; i < hash; i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			চালু;
		}
		যদি (cache->block == block) {
			cache->access_time = ++তথ্য->access_time;
			তথ্য->io_stats.cache_hits++;
			ফেরত cache;
		}
		যদি (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	যদি (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	তথ্য->io_stats.cache_misses++;
	ফেরত 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
স্থির errcode_t reuse_cache(io_channel channel,
		কাঠামো unix_private_তথ্য *তথ্য, কাঠামো unix_cache *cache,
		unsigned long long block)
{
	যদি (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, তথ্য, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		যদি (retval) {
			cache->write_err = 1;
			ফেরত retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++তথ্য->access_time;
	ফেরত 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
স্থির errcode_t flush_cached_blocks(io_channel channel,
				     কাঠামো unix_private_তথ্য *তথ্য,
				     পূর্ণসংখ্যা flags)
{
	কাঠামো unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned পূর্ণসংখ্যা		i;
	পূর্ণসংখ্যা			errors_found = 0;

	যদি ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(তথ্য, CACHE_MTX);
	জন্য (i=0, cache = তথ্য->cache; i < তথ্য->cache_আকার; i++, cache++) {
		যদি (!cache->in_use)
			চালু;
		যদি (cache->dirty) {
			পূর্ণসংখ্যা raw_flags = RAW_WRITE_NO_HANDLER;

			যদি (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, তথ্য,
					       cache->block, 1, cache->buf,
					       raw_flags);
			যদি (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} অন্যথা {
				cache->dirty = 0;
				cache->write_err = 0;
				যদি (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} অন্যথা যদি (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	যদি ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(তথ্য, CACHE_MTX);
retry:
	যখন (errors_found) {
		যদি ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(তথ্য, CACHE_MTX);
		errors_found = 0;
		জন্য (i=0, cache = তথ্য->cache; i < তথ্য->cache_আকার; i++, cache++) {
			যদি (!cache->in_use || !cache->write_err)
				চালু;
			errors_found = 1;
			যদি (cache->write_err && channel->write_error) {
				অক্ষর *err_buf = ফাঁকা;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				যদি (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ফাঁকা;
				অন্যথা
					memcpy(err_buf, cache->buf,
					       channel->block_আকার);
				mutex_unlock(তথ্য, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_আকার, -1,
					retval2);
				যদি (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(তথ্য, CACHE_MTX);
				যাও retry;
			} অন্যথা
				cache->write_err = 0;
		}
		যদি ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(তথ্য, CACHE_MTX);
	}
	ফেরত retval2;
}

/* Shrink the cache buffers */
স্থির errcode_t shrink_cache(io_channel channel,
			      কাঠামো unix_private_তথ্য *তথ্য,
			      unsigned পূর্ণসংখ্যা new_আকার)
{
	কাঠামো unix_cache	*cache, *new_cache;
	unsigned পূর্ণসংখ্যা		i;
	errcode_t		retval;

	mutex_lock(তথ্য, CACHE_MTX);

	retval = flush_cached_blocks(channel, তথ্য,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	যদি (retval)
		যাও unlock;

	জন্য (i = new_আকার, cache = তথ্য->cache + new_আকার;
	     i < তথ্য->cache_আকার;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(তথ্য->cache, new_আকার * আকার(কাঠামো unix_cache));
	যদি (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		যাও unlock;
	}

	তথ্য->cache = new_cache;
	তথ্য->cache_আকার = new_আকার;
	তথ্য->cache_hash_shift = highbit32(তথ্য->cache_আকার);

unlock:
	mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত retval;
}

/* Grow the cache buffers */
স্থির errcode_t grow_cache(io_channel channel,
			    কাঠামো unix_private_তথ্য *তথ্য,
			    unsigned পূর্ণসংখ্যা new_আকার)
{
	কাঠামো unix_cache	*cache, *new_cache;
	unsigned পূর্ণসংখ্যা		i;
	errcode_t		retval;

	mutex_lock(তথ্য, CACHE_MTX);

	retval = flush_cached_blocks(channel, তথ্য,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	যদি (retval)
		যাও unlock;

	new_cache = realloc(তথ্য->cache, new_আকার * আকার(কাঠামো unix_cache));
	যদি (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		যাও unlock;
	}
	তথ্য->cache = new_cache;

	জন্য (i = তথ্য->cache_আকার, cache = new_cache + তথ্য->cache_আকার;
	     i < new_আকার;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval) {
			যখন (i > তথ্য->cache_আকার) {
				cache--; i--;
				যদি (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			যাও unlock;
		}
	}

	তথ্য->cache_আকার = new_আকার;
	তথ্য->cache_hash_shift = highbit32(তথ্য->cache_আকার);

unlock:
	mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

পূর্ণসংখ্যা ext2fs_open_file(ধ্রুবক অক্ষর *pathname, পূর্ণসংখ্যা flags, mode_t mode)
{
	যদি (mode)
#যদি defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ফেরত open64(pathname, flags, mode);
	অন্যথা
		ফেরত open64(pathname, flags);
#অন্যথা
		ফেরত open(pathname, flags, mode);
	অন্যথা
		ফেরত open(pathname, flags);
#endif
}

পূর্ণসংখ্যা ext2fs_stat(ধ্রুবক অক্ষর *path, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ফেরত stat64(path, buf);
#অন্যথা
	ফেরত stat(path, buf);
#endif
}

পূর্ণসংখ্যা ext2fs_fstat(পূর্ণসংখ্যা fd, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ফেরত fstat64(fd, buf);
#অন্যথা
	ফেরত fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
স্থির errcode_t unix_funlock(io_channel channel)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	পূর্ণসংখ্যা ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (তথ্য->unix_flock_flags) {
		ret = flock(তথ্য->dev, LOCK_UN);
		যদি (ret)
			ফেরত errno;

		তথ্য->unix_flock_flags = 0;
	}

	ফেরত 0;
}

স্থির errcode_t unix_flock(io_channel channel, unsigned পূর্ণসংখ্যা flock_flags)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	পূর্ণসংখ্যা unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	যদি (ret)
		ফেরত ret;

	যদি (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	যদি (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	যদি (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	যদি (!unix_flock_flags)
		ফেরত 0;

	ret = flock(তথ্য->dev, unix_flock_flags);
	যদি (ret < 0)
		ফেরত errno;

	তথ্য->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ফেরত 0;
}
#অন্যথা
#define unix_flock		ফাঁকা

স্থির errcode_t unix_funlock(io_channel channel)
{
	ফেরত 0;
}
#endif /* HAVE_SYS_FILE_H */

স্থির errcode_t unix_open_channel(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা fd,
				   পূর্ণসংখ্যা flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ফাঁকা;
	কাঠামো unix_private_তথ্য *তথ্য = ফাঁকা;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	কাঠামো		utsname ut;
#endif

	যদি (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(শূন্য) fsync(fd);
#endif

	retval = ext2fs_get_mem(আকার(কাঠামো struct_io_channel), &io);
	যদি (retval)
		যাও cleanup;
	memset(io, 0, আকার(কাঠামো struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(আকার(কাঠামো unix_private_তথ্য), &তথ্য);
	যদি (retval)
		যাও cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		যাও cleanup;

	strcpy(io->name, name);
	io->private_তথ্য = তথ্য;
	io->block_আকার = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	যদি (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(তথ্য, 0, আকার(কাঠামো unix_private_তথ্য));
	তথ্য->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	তথ্য->io_stats.অঙ্ক_fields = 4;
	তথ্য->flags = flags;
	তথ্য->dev = fd;

	তথ্য->cache_আকার = DEFAULT_CACHE_আকার;
	তথ্য->cache_hash_shift = highbit32(তথ্য->cache_আকার);
	তথ্য->cache = calloc(DEFAULT_CACHE_আকার, আকার(কাঠামো unix_cache));
	যদি (!তথ্য->cache) {
		retval = EXT2_ET_NO_MEMORY;
		যাও cleanup;
	}

#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(তথ্য->dev);
#নতুবা_যদি defined(F_NOCACHE)
	যদি (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	যদি (ext2fs_fstat(তথ্য->dev, &st) == 0) {
		যদি (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			পূর্ণসংখ্যা zeroes = 0;

			যদি (ioctl(তথ্য->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} অন্যথা {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#যদি defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	যদি (!io->align)
		io->align = 512;
#endif

#যদি defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	যদি (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		পূর্ণসংখ্যা dio_align = ext2fs_get_dio_alignment(fd);

		যদি (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	যদি ((retval = alloc_cache(io, তথ্য)))
		যাও cleanup;

#ifdef BLKROGET
	যদি (flags & IO_FLAG_RW) {
		পূর্ণসংখ্যা error;
		পূর্ণসংখ্যা readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(তথ্য->dev, BLKROGET, &readonly);
		যদি (!error && readonly) {
			retval = EPERM;
			যাও cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#যদি (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#অন্যথা
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	যদি ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(তথ্য->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		কাঠামো rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		যদি (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	যদি (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&তথ্য->cache_mutex, ফাঁকা);
		যদি (retval)
			যাও cleanup;
		retval = pthread_mutex_init(&তথ্য->bounce_mutex, ফাঁকা);
		যদি (retval) {
			pthread_mutex_destroy(&তথ্য->cache_mutex);
			যাও cleanup;
		}
		retval = pthread_mutex_init(&তথ্য->stats_mutex, ফাঁকা);
		যদি (retval) {
			pthread_mutex_destroy(&তথ্য->cache_mutex);
			pthread_mutex_destroy(&তথ্য->bounce_mutex);
			যাও cleanup;
		}
	}
#endif
	*channel = io;
	ফেরত 0;

cleanup:
	যদি (তথ্য) {
		unix_funlock(io);
		যদি (io->manager != unixfd_io_manager && তথ্য->dev >= 0)
			close(তথ্য->dev);
		যদি (তথ্য->cache) {
			free_cache(তথ্য);
			মুক্ত(তথ্য->cache);
		}
		ext2fs_free_mem(&তথ্য);
	}
	যদি (io) {
		যদি (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ফেরত retval;
}

স্থির errcode_t unixfd_open(ধ্রুবক অক্ষর *str_fd, পূর্ণসংখ্যা flags,
			     io_channel *channel)
{
	পূর্ণসংখ্যা fd;
	পূর্ণসংখ্যা fd_flags;

	fd = atoi(str_fd);
#যদি defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	যদি (fd_flags == -1)
		ফেরত EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	যদি (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#যদি defined(O_DIRECT)
	যদি (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	ফেরত unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

স্থির errcode_t unix_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags,
			   io_channel *channel)
{
	পূর্ণসংখ্যা fd = -1;
	পূর্ণসংখ্যা open_flags;

	যদি (name == 0)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	যদি (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	যদি (fd < 0)
		ফেরত errno;
#যদি defined(F_NOCACHE) && !defined(IO_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO) {
		যদি (fcntl(fd, F_NOCACHE, 1) < 0)
			ফেরত errno;
	}
#endif
	ফেরত unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

স্থির errcode_t unix_close(io_channel channel)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ফেরত 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, তথ্য, 0);
#endif

	unix_funlock(channel);

	যদি (channel->manager != unixfd_io_manager && close(তথ্য->dev) < 0)
		retval = errno;
	free_cache(তথ্য);
	মুক্ত(তথ্য->cache);
#ifdef HAVE_PTHREAD
	যদি (তথ্য->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&তথ্য->cache_mutex);
		pthread_mutex_destroy(&তথ্য->bounce_mutex);
		pthread_mutex_destroy(&তথ্য->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_তথ্য);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ফেরত retval;
}

স্থির errcode_t unix_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->block_আকার != blksize) {
		mutex_lock(তথ্য, CACHE_MTX);
		mutex_lock(তথ্য, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		যদি ((retval = flush_cached_blocks(channel, তথ্য, FLUSH_NOLOCK))){
			mutex_unlock(তথ্য, BOUNCE_MTX);
			mutex_unlock(তথ্য, CACHE_MTX);
			ফেরত retval;
		}
#endif

		channel->block_আকার = blksize;
		free_cache(তথ্য);
		retval = alloc_cache(channel, তথ্য);
		mutex_unlock(তথ্য, BOUNCE_MTX);
		mutex_unlock(তথ্য, CACHE_MTX);
	}
	ফেরত retval;
}

স্থির errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	কাঠামো unix_cache *cache;
	errcode_t	retval;
	অক্ষর		*cp;
	পূর্ণসংখ্যা		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ফেরত raw_read_blk(channel, তথ্য, block, গুনতি, buf);
#অন্যথা
	যদি (তথ্য->flags & IO_FLAG_NOCACHE)
		ফেরত raw_read_blk(channel, তথ্য, block, গুনতি, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_আকার) {
		যদি ((retval = flush_cached_blocks(channel, তথ্য, 0)))
			ফেরত retval;
		ফেরত raw_read_blk(channel, তথ্য, block, গুনতি, buf);
	}

	cp = buf;
	mutex_lock(তথ্য, CACHE_MTX);
	যখন (গুনতি > 0) {
		/* If it's in the cache, use it! */
		যদি ((cache = find_cached_block(তথ্য, block, ফাঁকা))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_আকার);
			গুনতি--;
			block++;
			cp += channel->block_আকার;
			চালু;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		জন্য (i=1; i < গুনতি; i++)
			যদি (find_cached_block(তথ্য, block+i, ফাঁকা))
				বিরতি;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(তথ্য, CACHE_MTX);
		যদি ((retval = raw_read_blk(channel, তথ্য, block, i, cp)))
			ফেরত retval;
		mutex_lock(তথ্য, CACHE_MTX);

		/* Save the results in the cache */
		জন্য (j=0; j < i; j++) {
			যদি (!find_cached_block(তথ্য, block, &cache)) {
				retval = reuse_cache(channel, তথ্য,
						     cache, block);
				যদি (retval)
					যাও call_write_handler;
				memcpy(cache->buf, cp, channel->block_আকার);
			}
			গুনতি--;
			block++;
			cp += channel->block_আকার;
		}
	}
	mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত 0;

call_write_handler:
	যদি (cache->write_err && channel->write_error) {
		অক্ষর *err_buf = ফাঁকা;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		যদি (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ফাঁকা;
		অন্যথা
			memcpy(err_buf, cache->buf, channel->block_আকার);
		mutex_unlock(তথ্য, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_আকার, -1,
				       retval);
		যদি (err_buf)
			ext2fs_free_mem(&err_buf);
	} অন্যথা
		mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	ফেরত unix_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	কাঠামো unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ধ্রুবক অক্ষর	*cp;
	পূর্ণসংখ্যা		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ফেরত raw_write_blk(channel, তথ্য, block, গুনতি, buf, 0);
#অন্যথা
	যদি (তথ্য->flags & IO_FLAG_NOCACHE)
		ফেরত raw_write_blk(channel, তথ্য, block, গুনতি, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_আকার) {
		যদি ((retval = flush_cached_blocks(channel, তথ্য,
						  FLUSH_INVALIDATE)))
			ফেরত retval;
		ফেরত raw_write_blk(channel, তথ্য, block, গুনতি, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	যদি (writethrough)
		retval = raw_write_blk(channel, তথ্য, block, গুনতি, buf, 0);

	cp = buf;
	mutex_lock(তথ্য, CACHE_MTX);
	যখন (গুনতি > 0) {
		cache = find_cached_block(তথ্য, block, &reuse);
		যদি (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, তথ্য, cache, block);
			যদি (err)
				যাও call_write_handler;
		}
		যদি (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_আকার);
		cache->dirty = !writethrough;
		গুনতি--;
		block++;
		cp += channel->block_আকার;
	}
	mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত retval;

call_write_handler:
	যদি (cache->write_err && channel->write_error) {
		অক্ষর *err_buf = ফাঁকা;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		যদি (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ফাঁকা;
		অন্যথা
			memcpy(err_buf, cache->buf, channel->block_আকার);
		mutex_unlock(তথ্য, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_আকার, -1,
				       retval);
		যদি (err_buf)
			ext2fs_free_mem(&err_buf);
	} অন্যথা
		mutex_unlock(তথ্য, CACHE_MTX);
	ফেরত retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
#ifdef POSIX_FADV_WILLNEED
	কাঠামো unix_private_তথ্য *তথ্য;

	তথ্য = (কাঠামো unix_private_তথ্য *)channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ফেরত posix_fadvise(তথ্য->dev,
			     (ext2_loff_t)block * channel->block_আকার + তথ্য->offset,
			     (ext2_loff_t)গুনতি * channel->block_আকার,
			     POSIX_FADV_WILLNEED);
#অন্যথা
	ফেরত EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

স্থির errcode_t unix_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ফেরত unix_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণসংখ্যা আকার_2, ধ্রুবক শূন্য *buf)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		ফেরত EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	যদি ((retval = flush_cached_blocks(channel, তথ্য, FLUSH_INVALIDATE)))
		ফেরত retval;
#endif

	যদি (lseek(তথ্য->dev, offset + তথ্য->offset, SEEK_SET) < 0)
		ফেরত errno;

	actual = লিখ(তথ্য->dev, buf, আকার_2);
	যদি (actual < 0)
		ফেরত errno;
	যদি (actual != আকার_2)
		ফেরত EXT2_ET_SHORT_WRITE;

	ফেরত 0;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t unix_flush(io_channel channel)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, তথ্য, 0);
#endif
#ifdef HAVE_FSYNC
	যদি (!retval && fsync(তথ্য->dev) != 0)
		ফেরত errno;
#endif
	ফেরত retval;
}

স্থির errcode_t unix_set_option(io_channel channel, ধ্রুবক অক্ষর *option,
				 ধ্রুবক অক্ষর *arg)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	unsigned long long tmp;
	errcode_t retval;
	অক্ষর *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!strcmp(option, "offset")) {
		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		যদি (*end)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		তথ্য->offset = tmp;
		যদি (তথ্য->offset < 0)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		ফেরত 0;
	}
	যদি (!strcmp(option, "cache")) {
		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		যদি (!strcmp(arg, "on")) {
			তথ্য->flags &= ~IO_FLAG_NOCACHE;
			ফেরত 0;
		}
		যদি (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, তথ্য, 0);
			তথ্য->flags |= IO_FLAG_NOCACHE;
			ফেরত retval;
		}
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	যদি (!strcmp(option, "cache_blocks")) {
		unsigned long long	আকার_2;

		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		আকার_2 = strtoll(arg, ফাঁকা, 0);
		যদি (errno || আকার_2 == 0 || আকার_2 > INT32_MAX)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		যদি (তথ্য->cache_আকার == আকার_2)
			ফেরত 0;
		যদি (তথ্য->cache_আকার > আকার_2)
			ফেরত shrink_cache(channel, তথ্য, আকার_2);
		ফেরত grow_cache(channel, তথ্য, আকার_2);
	}
#endif
	ফেরত EXT2_ET_INVALID_ARGUMENT;
}

#যদি defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

স্থির errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	পূর্ণসংখ্যা		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->flags & CHANNEL_FLAGS_NODISCARD)
		যাও unimplemented;

	যদি (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_আকার + তথ্য->offset;
		range[1] = (__u64)(গুনতি) * channel->block_আকার;

		ret = ioctl(তথ্য->dev, BLKDISCARD, &range);
#অন্যথা
		যাও unimplemented;
#endif
	} অন্যথা {
#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(তথ্য->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_আকার + তথ্য->offset,
				(off_t)(গুনতি) * channel->block_আকার);
#অন্যথা
		যাও unimplemented;
#endif
	}
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			যাও unimplemented;
		}
		ফেরত errno;
	}
	ফেরত 0;
unimplemented:
	ফেরত EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
স্থির পূর্ণসংখ্যা __unix_zeroout(পূর্ণসংখ্যা fd, off_t offset, off_t len)
{
	পূর্ণসংখ্যা ret = -1;

#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	যদি (ret == 0)
		ফেরত 0;
#endif
#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	যদি (ret == 0)
		ফেরত 0;
#endif
	errno = EOPNOTSUPP;
	ফেরত ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
স্থির errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো unix_private_তথ্য *তথ্য;
	পূর্ণসংখ্যা		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো unix_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		কাঠামো stat statbuf;

		যদি (গুনতি == 0)
			ফেরত 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(তথ্য->dev, &statbuf);
		যদি (ret)
			যাও err;
		যদি ((unsigned long long) statbuf.st_আকার <
			(block + গুনতি) * channel->block_আকার + তথ্য->offset) {
			ret = ftruncate(তথ্য->dev,
					(block + গুনতি) * channel->block_আকার + তথ্য->offset);
			যদি (ret)
				যাও err;
		}
	}

	যদি (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		যাও unimplemented;

	ret = __unix_zeroout(তথ্য->dev,
			(off_t)(block) * channel->block_আকার + তথ্য->offset,
			(off_t)(গুনতি) * channel->block_আকার);
err:
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			যাও unimplemented;
		}
		ফেরত errno;
	}
	ফেরত 0;
unimplemented:
	ফেরত EXT2_ET_UNIMPLEMENTED;
}
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বের_করো
#endif

স্থির কাঠামো struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.set_blksize	= unix_set_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.set_option	= unix_set_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unix_io_manager = &struct_unix_manager;

স্থির কাঠামো struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.open		= unixfd_open,
	.close		= unix_close,
	.set_blksize	= unix_set_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.set_option	= unix_set_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
