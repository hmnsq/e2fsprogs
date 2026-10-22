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

#אם !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#אם HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ifdef __linux__
#ضمن <sys/utsname.h>
#endif
#אם HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#ضمن <sys/mount.h>
#endif
#אם HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#אם HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#endif
#אם HAVE_LINUX_FALLOC_H
#ضمن <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#ضمن <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#ضمن <sys/file.h>
#endif

#אם defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن مضمن صحيح find_last_bit_set(صحيح x)
{
	صحيح r = 32;

	אם (!x)
		החזר 0;
	אם (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	אם (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	אם (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	אם (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	אם (!(x & 0x80000000u)) {
		r -= 1;
	}
	החזר r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ساكن مضمن صحيح highbit32(uint32_t v)
{
	החזר find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  אם ((بنية)->magic != (code)) החזר (code)

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
	אם (data->flags & IO_FLAG_THREADS) {
		بدل (kind) {
		حالة CACHE_MTX:
			החזר &data->cache_mutex;
		حالة BOUNCE_MTX:
			החזר &data->bounce_mutex;
		حالة STATS_MTX:
			החזר &data->stats_mutex;
		}
	}
	החזר عدم;
}
#endif

ساكن مضمن فراغ mutex_lock(بنية unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	אם (mtx)
		pthread_mutex_lock(mtx);
#endif
}

ساكن مضمن فراغ mutex_unlock(بنية unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	אם (mtx)
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

	אם (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	החזר retval;
}

/*
 * Here are the raw I/O functions
 */
ساكن errcode_t raw_read_blk(io_channel channel,
			      بنية unix_private_data *data,
			      unsigned long long block,
			      صحيح count, فراغ *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	صحيح		actual = 0;
	unsigned حرف	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	صحيح		align_size, offset;

	size = (count < 0) ? -count : (ext2_loff_t) count * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	אם (data->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	אם ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		אם (actual == size)
			החזר 0;
		actual = 0;
	}
#وإلاإذا HAVE_PREAD
	/* Try an aligned pread */
	אם ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		אם (actual == size)
			החזר 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	אם ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		אם (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اقرأ(data->dev, buf, size);
		אם (actual != size) {
		short_read:
			אם (actual < 0) {
				retval = errno;
				actual = 0;
			} אחרת
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
	אם (channel->align == 0)
		channel->align = 1;
	אם ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	אחרת
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	אם (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		اذهب error_unlock;
	}
	כלעוד (size > 0) {
		actual = اقرأ(data->dev, data->bounce, align_size);
		אם (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			اذهب short_read;
		}
		אם ((actual + offset) > align_size)
			actual = align_size - offset;
		אם (actual > size)
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
	החזר 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	אם (actual >= 0 && actual < size)
		memset((حرف *) buf+actual, 0, size-actual);
	אם (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	החזר retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

ساكن errcode_t raw_write_blk(io_channel channel,
			       بنية unix_private_data *data,
			       unsigned long long block,
			       صحيح count, ثابت فراغ *bufv,
			       صحيح flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	صحيح		actual = 0;
	errcode_t	retval;
	ثابت unsigned حرف *buf = bufv;
	unsigned long long aligned_blk;
	صحيح		align_size, offset;

	אם (count == 1)
		size = channel->block_size;
	אחרת {
		אם (count < 0)
			size = -count;
		אחרת
			size = (ext2_loff_t) count * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	אם (data->flags & IO_FLAG_FORCE_BOUNCE)
		اذهب bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	אם ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		אם (actual == size)
			החזר 0;
	}
#وإلاإذا HAVE_PWRITE
	/* Try an aligned pwrite */
	אם ((حجم(off_t) >= حجم(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		אם (actual == size)
			החזר 0;
	}
#endif /* HAVE_PWRITE */

	אם ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		אם (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		אם (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual = اكتب(data->dev, buf, size);
		אם (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		אם (actual < 0) {
			retval = errno;
			اذهب error_out;
		}
		אם (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			اذهب error_out;
		}
		החזר 0;
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
	אם (channel->align == 0)
		channel->align = 1;
	אם ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	אחרת
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	כלעוד (size > 0) {
		صحيح actual_w;

		אם (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		אם (size < align_size || offset) {
			אם (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				اذهب error_unlock;
			}
			actual = اقرأ(data->dev, data->bounce,
				      align_size);
			אם (actual != align_size) {
				אם (actual < 0) {
					retval = errno;
					اذهب error_unlock;
				}
				memset((حرف *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		אם ((actual + offset) > align_size)
			actual = align_size - offset;
		אם (actual > size)
			actual = size;
		memcpy(((حرف *)data->bounce) + offset, buf, actual);
		אם (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			اذهب error_unlock;
		}
		actual_w = اكتب(data->dev, data->bounce, align_size);
		אם (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		אם (actual_w < 0) {
			retval = errno;
			اذهب error_out;
		}
		אם (actual_w != align_size)
			اذهب short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	החזר 0;

error_unlock:
	אם (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	אם (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	החזר retval;
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
	עבור (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		אם (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		אם (retval)
			החזר retval;
	}
	אם (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		אם (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	החזר retval;
}

/* Free the cache buffers */
ساكن فراغ free_cache(بنية unix_private_data *data)
{
	بنية unix_cache	*cache;
	unsigned صحيح		i;

	data->access_time = 0;
	עבור (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		אם (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	אם (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_PRIME	0x9e37fffffffc0001UL
#ifndef CACHE_LINE_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_LINE_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
ساكن unsigned صحيح
cache_hash(بنية unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	אם (data->cache_size <= DEFAULT_CACHE_SIZE)
		החזר 0;

	tmp = hashval ^ (GOLDEN_RATIO_PRIME + hashval) / CACHE_LINE_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_PRIME) >> data->cache_hash_shift);
	החזר tmp % data->cache_size;
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
	עבור (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		אם (!cache->in_use) {
			אם (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		אם (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			החזר cache;
		}
		אם (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	עבור (i = 0, cache = data->cache; i < hash; i++, cache++) {
		אם (!cache->in_use) {
			אם (!unused_cache)
				unused_cache = cache;
			استمر;
		}
		אם (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			החזר cache;
		}
		אם (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	אם (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	החזר 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ساكن errcode_t reuse_cache(io_channel channel,
		بنية unix_private_data *data, بنية unix_cache *cache,
		unsigned long long block)
{
	אם (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		אם (retval) {
			cache->write_err = 1;
			החזר retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	החזר 0;
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

	אם ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	עבור (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		אם (!cache->in_use)
			استمر;
		אם (cache->dirty) {
			صحيح raw_flags = RAW_WRITE_NO_HANDLER;

			אם (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			אם (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} אחרת {
				cache->dirty = 0;
				cache->write_err = 0;
				אם (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} אחרת אם (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	אם ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	כלעוד (errors_found) {
		אם ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		עבור (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			אם (!cache->in_use || !cache->write_err)
				استمر;
			errors_found = 1;
			אם (cache->write_err && channel->write_error) {
				حرف *err_buf = عدم;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				אם (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = عدم;
				אחרת
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				אם (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				اذهب retry;
			} אחרת
				cache->write_err = 0;
		}
		אם ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	החזר retval2;
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
	אם (retval)
		اذهب unlock;

	עבור (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		אם (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * حجم(بنية unix_cache));
	אם (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	החזר retval;
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
	אם (retval)
		اذهب unlock;

	new_cache = realloc(data->cache, new_size * حجم(بنية unix_cache));
	אם (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب unlock;
	}
	data->cache = new_cache;

	עבור (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		אם (retval) {
			כלעוד (i > data->cache_size) {
				cache--; i--;
				אם (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			اذهب unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	החזר retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t mode)
{
	אם (mode)
#אם defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		החזר open64(pathname, flags, mode);
	אחרת
		החזר open64(pathname, flags);
#אחרת
		החזר open(pathname, flags, mode);
	אחרת
		החזר open(pathname, flags);
#endif
}

صحيح ext2fs_stat(ثابت حرف *path, ext2fs_struct_stat *buf)
{
#אם defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	החזר stat64(path, buf);
#אחרת
	החזר stat(path, buf);
#endif
}

صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf)
{
#אם defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	החזר fstat64(fd, buf);
#אחרת
	החזר fstat(fd, buf);
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

	אם (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		אם (ret)
			החזר errno;

		data->unix_flock_flags = 0;
	}

	החזר 0;
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
	אם (ret)
		החזר ret;

	אם (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	אם (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	אם (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	אם (!unix_flock_flags)
		החזר 0;

	ret = flock(data->dev, unix_flock_flags);
	אם (ret < 0)
		החזר errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	החזר 0;
}
#אחרת
#define unix_flock		عدم

ساكن errcode_t unix_funlock(io_channel channel)
{
	החזר 0;
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

	אם (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(فراغ) fsync(fd);
#endif

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	אם (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية unix_private_data), &data);
	אם (retval)
		اذهب cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	אם (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	אם (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, حجم(بنية unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.num_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, حجم(بنية unix_cache));
	אם (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		اذهب cleanup;
	}

#אם defined(O_DIRECT)
	אם (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#وإلاإذا defined(F_NOCACHE)
	אם (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	אם (ext2fs_fstat(data->dev, &st) == 0) {
		אם (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			صحيح zeroes = 0;

			אם (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} אחרת {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#אם defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	אם (!io->align)
		io->align = 512;
#endif

#אם defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	אם (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		صحيح dio_align = ext2fs_get_dio_alignment(fd);

		אם (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	אם ((retval = alloc_cache(io, data)))
		اذهب cleanup;

#ifdef BLKROGET
	אם (flags & IO_FLAG_RW) {
		صحيح error;
		صحيح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		אם (!error && readonly) {
			retval = EPERM;
			اذهب cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#אם (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#אחרת
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	אם ((flags & IO_FLAG_RW) &&
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
		אם (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	אם (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, عدم);
		אם (retval)
			اذهب cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, عدم);
		אם (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			اذهب cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, عدم);
		אם (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			اذهب cleanup;
		}
	}
#endif
	*channel = io;
	החזר 0;

cleanup:
	אם (data) {
		unix_funlock(io);
		אם (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		אם (data->cache) {
			free_cache(data);
			حرر(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	אם (io) {
		אם (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	החזר retval;
}

ساكن errcode_t unixfd_open(ثابت حرف *str_fd, صحيح flags,
			     io_channel *channel)
{
	صحيح fd;
	صحيح fd_flags;

	fd = atoi(str_fd);
#אם defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	אם (fd_flags == -1)
		החזר EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	אם (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#אם defined(O_DIRECT)
	אם (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	החזר unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ساكن errcode_t unix_open(ثابت حرف *name, صحيح flags,
			   io_channel *channel)
{
	صحيح fd = -1;
	صحيح open_flags;

	אם (name == 0)
		החזר EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	אם (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#אם defined(O_DIRECT)
	אם (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	אם (fd < 0)
		החזר errno;
#אם defined(F_NOCACHE) && !defined(IO_DIRECT)
	אם (flags & IO_FLAG_DIRECT_IO) {
		אם (fcntl(fd, F_NOCACHE, 1) < 0)
			החזר errno;
	}
#endif
	החזר unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ساكن errcode_t unix_close(io_channel channel)
{
	بنية unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	אם (--channel->refcount > 0)
		החזר 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	אם (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	حرر(data->cache);
#ifdef HAVE_PTHREAD
	אם (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	אם (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	החזר retval;
}

ساكن errcode_t unix_set_blksize(io_channel channel, صحيح blksize)
{
	بنية unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	אם (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		אם ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			החזר retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	החזר retval;
}

ساكن errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       صحيح count, فراغ *buf)
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
	החזר raw_read_blk(channel, data, block, count, buf);
#אחרת
	אם (data->flags & IO_FLAG_NOCACHE)
		החזר raw_read_blk(channel, data, block, count, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	אם (count < 0 || count > WRITE_DIRECT_SIZE) {
		אם ((retval = flush_cached_blocks(channel, data, 0)))
			החזר retval;
		החזר raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	כלעוד (count > 0) {
		/* If it's in the cache, use it! */
		אם ((cache = find_cached_block(data, block, عدم))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			استمر;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		עבור (i=1; i < count; i++)
			אם (find_cached_block(data, block+i, عدم))
				اكسر;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		אם ((retval = raw_read_blk(channel, data, block, i, cp)))
			החזר retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		עבור (j=0; j < i; j++) {
			אם (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				אם (retval)
					اذهب call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			count--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	החזר 0;

call_write_handler:
	אם (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		אם (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		אחרת
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		אם (err_buf)
			ext2fs_free_mem(&err_buf);
	} אחרת
		mutex_unlock(data, CACHE_MTX);
	החזר retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *buf)
{
	החזר unix_read_blk64(channel, block, count, buf);
}

ساكن errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				صحيح count, ثابت فراغ *buf)
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
	החזר raw_write_blk(channel, data, block, count, buf, 0);
#אחרת
	אם (data->flags & IO_FLAG_NOCACHE)
		החזר raw_write_blk(channel, data, block, count, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	אם (count < 0 || count > WRITE_DIRECT_SIZE) {
		אם ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			החזר retval;
		החזר raw_write_blk(channel, data, block, count, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	אם (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	כלעוד (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		אם (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			אם (err)
				اذهب call_write_handler;
		}
		אם (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	החזר retval;

call_write_handler:
	אם (cache->write_err && channel->write_error) {
		حرف *err_buf = عدم;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		אם (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = عدم;
		אחרת
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		אם (err_buf)
			ext2fs_free_mem(&err_buf);
	} אחרת
		mutex_unlock(data, CACHE_MTX);
	החזר retval;
#endif /* NO_IO_CACHE */
}

ساكن errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
#ifdef POSIX_FADV_WILLNEED
	بنية unix_private_data *data;

	data = (بنية unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	החזר posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)count * channel->block_size,
			     POSIX_FADV_WILLNEED);
#אחרת
	החזר EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

ساكن errcode_t unix_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *buf)
{
	החזר unix_write_blk64(channel, block, count, buf);
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

	אם (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		החזר EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	אם ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		החזר retval;
#endif

	אם (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		החזר errno;

	actual = اكتب(data->dev, buf, size);
	אם (actual < 0)
		החזר errno;
	אם (actual != size)
		החזר EXT2_ET_SHORT_WRITE;

	החזר 0;
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
	אם (!retval && fsync(data->dev) != 0)
		החזר errno;
#endif
	החזר retval;
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

	אם (!strcmp(option, "offset")) {
		אם (!arg)
			החזר EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		אם (*end)
			החזר EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		אם (data->offset < 0)
			החזר EXT2_ET_INVALID_ARGUMENT;
		החזר 0;
	}
	אם (!strcmp(option, "cache")) {
		אם (!arg)
			החזר EXT2_ET_INVALID_ARGUMENT;
		אם (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			החזר 0;
		}
		אם (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			החזר retval;
		}
		החזר EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	אם (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		אם (!arg)
			החזר EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, عدم, 0);
		אם (errno || size == 0 || size > INT32_MAX)
			החזר EXT2_ET_INVALID_ARGUMENT;

		אם (data->cache_size == size)
			החזר 0;
		אם (data->cache_size > size)
			החזר shrink_cache(channel, data, size);
		החזר grow_cache(channel, data, size);
	}
#endif
	החזר EXT2_ET_INVALID_ARGUMENT;
}

#אם defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

ساكن errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	بنية unix_private_data *data;
	صحيح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	אם (channel->flags & CHANNEL_FLAGS_NODISCARD)
		اذهب unimplemented;

	אם (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(count) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#אחרת
		اذهب unimplemented;
#endif
	} אחרת {
#אם defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(count) * channel->block_size);
#אחרת
		اذهب unimplemented;
#endif
	}
	אם (ret < 0) {
		אם (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			اذهب unimplemented;
		}
		החזר errno;
	}
	החזר 0;
unimplemented:
	החזר EXT2_ET_UNIMPLEMENTED;
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

#אם defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	אם (ret == 0)
		החזר 0;
#endif
#אם defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	אם (ret == 0)
		החזר 0;
#endif
	errno = EOPNOTSUPP;
	החזר ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#אם __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ساكن errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	بنية unix_private_data *data;
	صحيح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	אם (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		بنية stat statbuf;

		אם (count == 0)
			החזר 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		אם (ret)
			اذهب err;
		אם ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			אם (ret)
				اذهب err;
		}
	}

	אם (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		اذهب unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(count) * channel->block_size);
err:
	אם (ret < 0) {
		אם (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			اذهب unimplemented;
		}
		החזר errno;
	}
	החזר 0;
unimplemented:
	החזר EXT2_ET_UNIMPLEMENTED;
}
#אם __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
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
