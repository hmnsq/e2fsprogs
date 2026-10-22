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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ifdef __linux__
#ضمن <sys/utsname.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#endif
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#endif
#إذا HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#ضمن <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#endif

#إذا defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح find_last_bit_set(صحيح x)
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
	أرجع find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  إذا ((بنية)->magic != (code)) أرجع (code)

بنية unix_cache {
	حرف			*buf;
	unsigned long long	block;
	صحيح			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

بنية unix_private_data {
	صحيح	magic;
	صحيح	dev;
	صحيح	flags;
	صحيح	align;
	صحيح	access_time;
	صحيح	unix_flock_flags;
	ext2_loff_t offset;
	بنية unix_cache *cache;
	unsigned صحيح cache_size;
	unsigned صحيح cache_hash_shift;
	فراغ	*bounce;
	بنية struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعريفنوع تعداد lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
ساكن مضمن pthread_mutex_t *get_mutex(بنية unix_private_data *data,
					 kind_t kind)
{
	إذا (data->flags & IO_FLAG_THREADS) {
		بدل (kind) {
		حالة CACHE_MTX:
			أرجع &data->cache_mutex;
		حالة BOUNCE_MTX:
			أرجع &data->bounce_mutex;
		حالة STATS_MTX:
			أرجع &data->stats_mutex;
		}
	}
	أرجع عدم;
}
#endif

ساكن مضمن فراغ mutex_lock(بنية unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	إذا (mtx)
		pthread_mutex_lock(mtx);
#endif
}

ساكن مضمن فراغ mutex_unlock(بنية unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	إذا (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

ساكن errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	بنية unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	أرجع retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_private_data *data,
			      unsigned long long block,
			      صحيح گنتی, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	صحيح		actual = 0;
	unsigned حرف	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	صحيح		align_size, offset;

	size = (گنتی < 0) ? -گنتی : (ext2_loff_t) گنتی * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	إذا (data->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		إذا (actual == size)
			أرجع 0;
		actual = 0;
	}
#وإلاإذا HAVE_PREAD
	/* Try an aligned pread */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		إذا (actual == size)
			أرجع 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		إذا (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(data->dev, buf, size);
		إذا (actual != size) {
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

#ifdef ALIGN_DEBUG
	printf("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) size);
#endif

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	وإلا
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	إذا (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	طالما (size > 0) {
		actual = اقرأ(data->dev, data->bounce, align_size);
		إذا (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			اذهب short_read;
		}
		إذا ((actual + offset) > align_size)
			actual = align_size - offset;
		إذا (actual > size)
			actual = size;
		memcpy(buf, (حرف *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	أرجع 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	إذا (actual >= 0 && actual < size)
		memset((حرف *) buf+actual, 0, size-actual);
	إذا (channel->read_error)
		retval = (channel->read_error)(channel, block, گنتی, buf,
					       size, actual, retval);
	أرجع retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_private_data *data,
			       unsigned long long block,
			       صحيح گنتی, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت unsigned حرف *buf = bufv;
	unsigned long long aligned_blk;
	صحيح		align_size, offset;

	إذا (گنتی == 1)
		size = channel->block_size;
	وإلا {
		إذا (گنتی < 0)
			size = -گنتی;
		وإلا
			size = (ext2_loff_t) گنتی * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	إذا (data->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		إذا (actual == size)
			أرجع 0;
	}
#وإلاإذا HAVE_PWRITE
	/* Try an aligned pwrite */
	إذا ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		إذا (actual == size)
			أرجع 0;
	}
#endif /* HAVE_PWRITE */

	إذا ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		إذا (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(data->dev, buf, size);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		إذا (actual < 0) {
			retval = errno;
			اذهب error_out;
		}
		إذا (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_out;
		}
		أرجع 0;
	}

#ifdef ALIGN_DEBUG
	printf("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) size);
#endif
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	إذا (channel->align == 0)
		channel->align = 1;
	إذا ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	وإلا
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	طالما (size > 0) {
		صحيح actual_w;

		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		إذا (size < align_size || offset) {
			إذا (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(data->dev, data->bounce,
				      align_size);
			إذا (actual != align_size) {
				إذا (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		إذا ((actual + offset) > align_size)
			actual = align_size - offset;
		إذا (actual > size)
			actual = size;
		memcpy(((حرف *)data->bounce) + offset, buf, actual);
		إذا (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(data->dev, data->bounce, align_size);
		إذا (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		إذا (actual_w < 0) {
			retval = errno;
			اذهب error_out;
		}
		إذا (actual_w != align_size)
			اذهب short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	أرجع 0;

error_unlock:
	إذا (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	إذا (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, گنتی, buf,
						size, actual, retval);
	أرجع retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ساكن errcode_t alloc_cache(io_channel channel,
			     بنية unix_private_data *data)
{
	errcode_t		retval;
	بنية unix_cache	*cache;
	unsigned صحيح		i;

	data->access_time = 0;
	لكل (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval)
			أرجع retval;
	}
	إذا (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		إذا (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	أرجع retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية unix_private_data *data)
{
	بنية unix_cache	*cache;
	unsigned صحيح		i;

	data->access_time = 0;
	لكل (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	إذا (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_مفرد	0x9e37fffffffc0001UL
#ifndef CACHE_سطر_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_سطر_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن unsigned صحيح
cache_hash(بنية unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	إذا (data->cache_size <= DEFAULT_CACHE_SIZE)
		أرجع 0;

	tmp = hashval ^ (GOLDEN_RATIO_مفرد + hashval) / CACHE_سطر_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_مفرد) >> data->cache_hash_shift);
	أرجع tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ساكن بنية unix_cache *find_cached_block(بنية unix_private_data *data,
					    unsigned long long block,
					    بنية unix_cache **eldest)
{
	بنية unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned صحيح		hash = cache_hash(data, block);
	unsigned صحيح		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	لكل (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		إذا (!cache->in_use) {
			إذا (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		إذا (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	لكل (i = 0, cache = data->cache; i < hash; i++, cache++) {
		إذا (!cache->in_use) {
			إذا (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		إذا (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			أرجع cache;
		}
		إذا (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	إذا (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	أرجع 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_private_data *data, بنية unix_cache *cache,
		unsigned long long block)
{
	إذا (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		إذا (retval) {
			cache->write_err = 1;
			أرجع retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	أرجع 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ساكن errcode_t flush_cached_blocks(io_channel channel,
				     بنية unix_private_data *data,
				     صحيح flags)
{
	بنية unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned صحيح		i;
	صحيح			errors_found = 0;

	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	لكل (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		إذا (!cache->in_use)
			استمر;
		إذا (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			إذا (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			إذا (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} وإلا {
				cache->dirty = 0;
				cache->write_err = 0;
				إذا (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} وإلا إذا (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	إذا ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	طالما (errors_found) {
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		لكل (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			إذا (!cache->in_use || !cache->write_err)
				استمر;
			errors_found = 1;
			إذا (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				إذا (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				وإلا
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				إذا (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				اذهب retry;
			} وإلا
				cache->write_err = 0;
		}
		إذا ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	أرجع retval2;
}

/* Shrink the cache buffers */
ساكن errcode_t shrink_cache(io_channel channel,
			      بنية unix_private_data *data,
			      unsigned صحيح new_size)
{
	بنية unix_cache	*cache, *new_cache;
	unsigned صحيح		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	لكل (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		إذا (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	أرجع retval;
}

/* Grow the cache buffers */
ساكن errcode_t grow_cache(io_channel channel,
			    بنية unix_private_data *data,
			    unsigned صحيح new_size)
{
	بنية unix_cache	*cache, *new_cache;
	unsigned صحيح		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	إذا (retval)
		اذهب unlock;

	new_cache = realloc(data->cache, new_size * حجم(بنية unix_cache));
	إذا (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب unlock;
	}
	data->cache = new_cache;

	لكل (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		إذا (retval) {
			طالما (i > data->cache_size) {
				cache--; i--;
				إذا (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	أرجع retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t mode)
{
	إذا (mode)
#إذا defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		أرجع open64(pathname, flags, mode);
	وإلا
		أرجع open64(pathname, flags);
#وإلا
		أرجع open(pathname, flags, mode);
	وإلا
		أرجع open(pathname, flags);
#endif
}

صحيح ext2fs_stat(ثابت حرف *path, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع stat64(path, buf);
#وإلا
	أرجع stat(path, buf);
#endif
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	أرجع fstat64(fd, buf);
#وإلا
	أرجع fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
ساكن errcode_t unix_funlock(io_channel channel)
{
	بنية unix_private_data *data;
	صحيح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		إذا (ret)
			أرجع errno;

		data->unix_flock_flags = 0;
	}

	أرجع 0;
}

ساكن errcode_t unix_flock(io_channel channel, unsigned صحيح flock_flags)
{
	بنية unix_private_data *data;
	صحيح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

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

	ret = flock(data->dev, unix_flock_flags);
	إذا (ret < 0)
		أرجع errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	أرجع 0;
}
#وإلا
#define unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	أرجع 0;
}
#endif /* HAVE_SYS_FILE_H */

ساكن errcode_t unix_open_channel(ثابت حرف *name, صحيح fd,
				   صحيح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = عدم;
	بنية unix_private_data *data = عدم;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	بنية		utsname ut;
#endif

	إذا (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#endif

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_private_data), &data);
	إذا (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	إذا (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, حجم(بنية unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.عدد_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, حجم(بنية unix_cache));
	إذا (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب cleanup;
	}

#إذا defined(O_DIRECT)
	إذا (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#وإلاإذا defined(F_NOCACHE)
	إذا (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	إذا (ext2fs_fstat(data->dev, &st) == 0) {
		إذا (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			صحيح zeroes = 0;

			إذا (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
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
#endif

#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	إذا (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		إذا (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	إذا ((retval = alloc_cache(io, data)))
		اذهب cleanup;

#ifdef BLKROGET
	إذا (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		إذا (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#إذا (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#وإلا
#define RLIM_INFINITY  (~0UL)
#endif
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
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		بنية rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		إذا (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	إذا (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, عدم);
		إذا (retval)
			اذهب cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			اذهب cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, عدم);
		إذا (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			اذهب cleanup;
		}
	}
#endif
	*channel = io;
	أرجع 0;

cleanup:
	إذا (data) {
		unix_funlock(io);
		إذا (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		إذا (data->cache) {
			free_cache(data);
			حرر(data->cache);
		}
		ext2fs_free_mem(&data);
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
#إذا defined(O_DIRECT)
	إذا (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

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
#إذا defined(O_DIRECT)
	إذا (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	إذا (fd < 0)
		أرجع errno;
#إذا defined(F_NOCACHE) && !defined(IO_DIRECT)
	إذا (flags & IO_FLAG_DIRECT_IO) {
		إذا (fcntl(fd, F_NOCACHE, 1) < 0)
			أرجع errno;
	}
#endif
	أرجع unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	إذا (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	حرر(data->cache);
#ifdef HAVE_PTHREAD
	إذا (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t unix_set_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		إذا ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			أرجع retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	أرجع retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       صحيح گنتی, فراغ *buf)
{
	بنية unix_private_data *data;
	بنية unix_cache *cache;
	errcode_t	retval;
	حرف		*cp;
	صحيح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	أرجع raw_read_blk(channel, data, block, گنتی, buf);
#وإلا
	إذا (data->flags & IO_FLAG_NOCACHE)
		أرجع raw_read_blk(channel, data, block, گنتی, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	إذا (گنتی < 0 || گنتی > WRITE_DIRECT_SIZE) {
		إذا ((retval = flush_cached_blocks(channel, data, 0)))
			أرجع retval;
		أرجع raw_read_blk(channel, data, block, گنتی, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	طالما (گنتی > 0) {
		/* If it's in the cache, use it! */
		إذا ((cache = find_cached_block(data, block, عدم))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			گنتی--;
			block++;
			cp += channel->block_size;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		لكل (i=1; i < گنتی; i++)
			إذا (find_cached_block(data, block+i, عدم))
				اكسر;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		إذا ((retval = raw_read_blk(channel, data, block, i, cp)))
			أرجع retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		لكل (j=0; j < i; j++) {
			إذا (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				إذا (retval)
					اذهب call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			گنتی--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	أرجع 0;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(data, CACHE_MTX);
	أرجع retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       صحيح گنتی, فراغ *buf)
{
	أرجع unix_read_blk64(channel, block, گنتی, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				صحيح گنتی, ثابت فراغ *buf)
{
	بنية unix_private_data *data;
	بنية unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت حرف	*cp;
	صحيح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	أرجع raw_write_blk(channel, data, block, گنتی, buf, 0);
#وإلا
	إذا (data->flags & IO_FLAG_NOCACHE)
		أرجع raw_write_blk(channel, data, block, گنتی, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	إذا (گنتی < 0 || گنتی > WRITE_DIRECT_SIZE) {
		إذا ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			أرجع retval;
		أرجع raw_write_blk(channel, data, block, گنتی, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	إذا (writethrough)
		retval = raw_write_blk(channel, data, block, گنتی, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	طالما (گنتی > 0) {
		cache = find_cached_block(data, block, &reuse);
		إذا (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			إذا (err)
				اذهب call_write_handler;
		}
		إذا (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		گنتی--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	أرجع retval;

call_write_handler:
	إذا (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		إذا (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		وإلا
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		إذا (err_buf)
			ext2fs_free_mem(&err_buf);
	} وإلا
		mutex_unlock(data, CACHE_MTX);
	أرجع retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long گنتی)
{
#ifdef POSIX_FADV_WILLNEED
	بنية unix_private_data *data;

	data = (بنية unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	أرجع posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)گنتی * channel->block_size,
			     POSIX_FADV_WILLNEED);
#وإلا
	أرجع EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

ساكن errcode_t unix_write_blk(io_channel channel, unsigned long block,
				صحيح گنتی, ثابت فراغ *buf)
{
	أرجع unix_write_blk64(channel, block, گنتی, buf);
}

ساكن errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 صحيح size, ثابت فراغ *buf)
{
	بنية unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		أرجع EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	إذا ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		أرجع retval;
#endif

	إذا (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		أرجع errno;

	actual = اكتب(data->dev, buf, size);
	إذا (actual < 0)
		أرجع errno;
	إذا (actual != size)
		أرجع EXT2_ET_SHORT_WRITE;

	أرجع 0;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t unix_flush(io_channel channel)
{
	بنية unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	إذا (!retval && fsync(data->dev) != 0)
		أرجع errno;
#endif
	أرجع retval;
}

ساكن errcode_t unix_set_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	حرف *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		إذا (*end)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		إذا (data->offset < 0)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		أرجع 0;
	}
	إذا (!strcmp(option, "cache")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		إذا (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			أرجع 0;
		}
		إذا (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			أرجع retval;
		}
		أرجع EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	إذا (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		إذا (!arg)
			أرجع EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, عدم, 0);
		إذا (errno || size == 0 || size > INT32_MAX)
			أرجع EXT2_ET_INVALID_ARGUMENT;

		إذا (data->cache_size == size)
			أرجع 0;
		إذا (data->cache_size > size)
			أرجع shrink_cache(channel, data, size);
		أرجع grow_cache(channel, data, size);
	}
#endif
	أرجع EXT2_ET_INVALID_ARGUMENT;
}

#إذا defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

ساكن errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long گنتی)
{
	بنية unix_private_data *data;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	إذا (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(گنتی) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#وإلا
		اذهب unimplemented;
#endif
	} وإلا {
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(گنتی) * channel->block_size);
#وإلا
		اذهب unimplemented;
#endif
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
ساكن صحيح __unix_zeroout(صحيح fd, off_t offset, off_t len)
{
	صحيح ret = -1;

#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	إذا (ret == 0)
		أرجع 0;
#endif
#إذا defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	إذا (ret == 0)
		أرجع 0;
#endif
	errno = EOPNOTSUPP;
	أرجع ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#إذا __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ڈالو
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ساكن errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long گنتی)
{
	بنية unix_private_data *data;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		إذا (گنتی == 0)
			أرجع 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		إذا (ret)
			اذهب err;
		إذا ((unsigned long long) statbuf.st_size <
			(block + گنتی) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + گنتی) * channel->block_size + data->offset);
			إذا (ret)
				اذهب err;
		}
	}

	إذا (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(گنتی) * channel->block_size);
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
#pragma GCC diagnostic نکالو
#endif

ساكن بنية struct_io_manager struct_unix_manager = {
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

ساكن بنية struct_io_manager struct_unixfd_manager = {
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
