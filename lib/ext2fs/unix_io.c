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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#ifdef __linux__
#شامل <sys/utsname.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#شامل <sys/mount.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_RESOURCE_H
#شامل <sys/resource.h>
#endif
#اگر HAVE_LINUX_FALLOC_H
#شامل <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#شامل <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#شامل <sys/file.h>
#endif

#اگر defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ایستا درونخطی صحیح find_last_bit_set(صحیح x)
{
	صحیح r = 32;

	اگر (!x)
		بازگشت 0;
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
	بازگشت r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ایستا درونخطی صحیح highbit32(uint32_t v)
{
	بازگشت find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)

ساختار unix_cache {
	نویسه			*buf;
	unsigned long long	block;
	صحیح			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ساختار unix_private_data {
	صحیح	magic;
	صحیح	dev;
	صحیح	flags;
	صحیح	align;
	صحیح	access_time;
	صحیح	unix_flock_flags;
	ext2_loff_t offset;
	ساختار unix_cache *cache;
	unsigned صحیح cache_size;
	unsigned صحیح cache_hash_shift;
	تهی	*bounce;
	ساختار struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

تعریفنوع شمارش lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
ایستا درونخطی pthread_mutex_t *get_mutex(ساختار unix_private_data *data,
					 kind_t kind)
{
	اگر (data->flags & IO_FLAG_THREADS) {
		انتخاب (kind) {
		حالت CACHE_MTX:
			بازگشت &data->cache_mutex;
		حالت BOUNCE_MTX:
			بازگشت &data->bounce_mutex;
		حالت STATS_MTX:
			بازگشت &data->stats_mutex;
		}
	}
	بازگشت پوچ;
}
#endif

ایستا درونخطی تهی mutex_lock(ساختار unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	اگر (mtx)
		pthread_mutex_lock(mtx);
#endif
}

ایستا درونخطی تهی mutex_unlock(ساختار unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	اگر (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

ایستا errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ساختار unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	بازگشت retval;
}

/*
 * Here are the raw I/O functions
 */
ایستا errcode_t raw_read_blk(io_channel channel,
			      ساختار unix_private_data *data,
			      unsigned long long block,
			      صحیح شمار, تهی *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	صحیح		actual = 0;
	unsigned نویسه	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	صحیح		align_size, offset;

	size = (شمار < 0) ? -شمار : (ext2_loff_t) شمار * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	اگر (data->flags & IO_FLAG_FORCE_BOUNCE)
		برو bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		اگر (actual == size)
			بازگشت 0;
		actual = 0;
	}
#وگرنهاگر HAVE_PREAD
	/* Try an aligned pread */
	اگر ((اندازه(off_t) >= اندازه(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		اگر (actual == size)
			بازگشت 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		اگر (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			برو error_unlock;
		}
		actual = اقرأ(data->dev, buf, size);
		اگر (actual != size) {
		short_read:
			اگر (actual < 0) {
				retval = errno;
				actual = 0;
			} وگرنه
				retval = EXT2_ET_SHORT_READ;
			برو error_unlock;
		}
		برو success_unlock;
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
	اگر (channel->align == 0)
		channel->align = 1;
	اگر ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	وگرنه
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	اگر (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		برو error_unlock;
	}
	وقتی (size > 0) {
		actual = اقرأ(data->dev, data->bounce, align_size);
		اگر (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			برو short_read;
		}
		اگر ((actual + offset) > align_size)
			actual = align_size - offset;
		اگر (actual > size)
			actual = size;
		memcpy(buf, (نویسه *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	بازگشت 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	اگر (actual >= 0 && actual < size)
		memset((نویسه *) buf+actual, 0, size-actual);
	اگر (channel->read_error)
		retval = (channel->read_error)(channel, block, شمار, buf,
					       size, actual, retval);
	بازگشت retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

ایستا errcode_t raw_write_blk(io_channel channel,
			       ساختار unix_private_data *data,
			       unsigned long long block,
			       صحیح شمار, ثابت تهی *bufv,
			       صحیح flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	صحیح		actual = 0;
	errcode_t	retval;
	ثابت unsigned نویسه *buf = bufv;
	unsigned long long aligned_blk;
	صحیح		align_size, offset;

	اگر (شمار == 1)
		size = channel->block_size;
	وگرنه {
		اگر (شمار < 0)
			size = -شمار;
		وگرنه
			size = (ext2_loff_t) شمار * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	اگر (data->flags & IO_FLAG_FORCE_BOUNCE)
		برو bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		اگر (actual == size)
			بازگشت 0;
	}
#وگرنهاگر HAVE_PWRITE
	/* Try an aligned pwrite */
	اگر ((اندازه(off_t) >= اندازه(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		اگر (actual == size)
			بازگشت 0;
	}
#endif /* HAVE_PWRITE */

	اگر ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		اگر (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			برو error_unlock;
		}
		actual = اكتب(data->dev, buf, size);
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		اگر (actual < 0) {
			retval = errno;
			برو error_out;
		}
		اگر (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			برو error_out;
		}
		بازگشت 0;
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
	اگر (channel->align == 0)
		channel->align = 1;
	اگر ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	وگرنه
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	وقتی (size > 0) {
		صحیح actual_w;

		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		اگر (size < align_size || offset) {
			اگر (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				برو error_unlock;
			}
			actual = اقرأ(data->dev, data->bounce,
				      align_size);
			اگر (actual != align_size) {
				اگر (actual < 0) {
					retval = errno;
					برو error_unlock;
				}
				memset((نویسه *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		اگر ((actual + offset) > align_size)
			actual = align_size - offset;
		اگر (actual > size)
			actual = size;
		memcpy(((نویسه *)data->bounce) + offset, buf, actual);
		اگر (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			برو error_unlock;
		}
		actual_w = اكتب(data->dev, data->bounce, align_size);
		اگر (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		اگر (actual_w < 0) {
			retval = errno;
			برو error_out;
		}
		اگر (actual_w != align_size)
			برو short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	بازگشت 0;

error_unlock:
	اگر (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	اگر (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, شمار, buf,
						size, actual, retval);
	بازگشت retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ایستا errcode_t alloc_cache(io_channel channel,
			     ساختار unix_private_data *data)
{
	errcode_t		retval;
	ساختار unix_cache	*cache;
	unsigned صحیح		i;

	data->access_time = 0;
	برای (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval)
			بازگشت retval;
	}
	اگر (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		اگر (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	بازگشت retval;
}

/* Free the cache buffers */
ایستا تهی free_cache(ساختار unix_private_data *data)
{
	ساختار unix_cache	*cache;
	unsigned صحیح		i;

	data->access_time = 0;
	برای (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	اگر (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_اول	0x9e37fffffffc0001UL
#ifndef CACHE_سطر_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_سطر_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
ایستا unsigned صحیح
cache_hash(ساختار unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	اگر (data->cache_size <= DEFAULT_CACHE_SIZE)
		بازگشت 0;

	tmp = hashval ^ (GOLDEN_RATIO_اول + hashval) / CACHE_سطر_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_اول) >> data->cache_hash_shift);
	بازگشت tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ایستا ساختار unix_cache *find_cached_block(ساختار unix_private_data *data,
					    unsigned long long block,
					    ساختار unix_cache **eldest)
{
	ساختار unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned صحیح		hash = cache_hash(data, block);
	unsigned صحیح		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	برای (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		اگر (!cache->in_use) {
			اگر (!unused_cache)
				unused_cache = cache;
			ادامه;
		}
		اگر (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			بازگشت cache;
		}
		اگر (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	برای (i = 0, cache = data->cache; i < hash; i++, cache++) {
		اگر (!cache->in_use) {
			اگر (!unused_cache)
				unused_cache = cache;
			ادامه;
		}
		اگر (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			بازگشت cache;
		}
		اگر (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	اگر (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	بازگشت 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ایستا errcode_t reuse_cache(io_channel channel,
		ساختار unix_private_data *data, ساختار unix_cache *cache,
		unsigned long long block)
{
	اگر (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		اگر (retval) {
			cache->write_err = 1;
			بازگشت retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	بازگشت 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ایستا errcode_t flush_cached_blocks(io_channel channel,
				     ساختار unix_private_data *data,
				     صحیح flags)
{
	ساختار unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned صحیح		i;
	صحیح			errors_found = 0;

	اگر ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	برای (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		اگر (!cache->in_use)
			ادامه;
		اگر (cache->dirty) {
			صحیح raw_flags = RAW_WRITE_NO_HANDLER;

			اگر (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			اگر (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} وگرنه {
				cache->dirty = 0;
				cache->write_err = 0;
				اگر (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} وگرنه اگر (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	اگر ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	وقتی (errors_found) {
		اگر ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		برای (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			اگر (!cache->in_use || !cache->write_err)
				ادامه;
			errors_found = 1;
			اگر (cache->write_err && channel->write_error) {
				نویسه *err_buf = پوچ;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				اگر (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = پوچ;
				وگرنه
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				اگر (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				برو retry;
			} وگرنه
				cache->write_err = 0;
		}
		اگر ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	بازگشت retval2;
}

/* Shrink the cache buffers */
ایستا errcode_t shrink_cache(io_channel channel,
			      ساختار unix_private_data *data,
			      unsigned صحیح new_size)
{
	ساختار unix_cache	*cache, *new_cache;
	unsigned صحیح		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	اگر (retval)
		برو unlock;

	برای (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		اگر (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * اندازه(ساختار unix_cache));
	اگر (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		برو unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	بازگشت retval;
}

/* Grow the cache buffers */
ایستا errcode_t grow_cache(io_channel channel,
			    ساختار unix_private_data *data,
			    unsigned صحیح new_size)
{
	ساختار unix_cache	*cache, *new_cache;
	unsigned صحیح		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	اگر (retval)
		برو unlock;

	new_cache = realloc(data->cache, new_size * اندازه(ساختار unix_cache));
	اگر (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		برو unlock;
	}
	data->cache = new_cache;

	برای (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		اگر (retval) {
			وقتی (i > data->cache_size) {
				cache--; i--;
				اگر (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			برو unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	بازگشت retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

صحیح ext2fs_open_file(ثابت نویسه *pathname, صحیح flags, mode_t mode)
{
	اگر (mode)
#اگر defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		بازگشت open64(pathname, flags, mode);
	وگرنه
		بازگشت open64(pathname, flags);
#وگرنه
		بازگشت open(pathname, flags, mode);
	وگرنه
		بازگشت open(pathname, flags);
#endif
}

صحیح ext2fs_stat(ثابت نویسه *path, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	بازگشت stat64(path, buf);
#وگرنه
	بازگشت stat(path, buf);
#endif
}

صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf)
{
#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	بازگشت fstat64(fd, buf);
#وگرنه
	بازگشت fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
ایستا errcode_t unix_funlock(io_channel channel)
{
	ساختار unix_private_data *data;
	صحیح ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		اگر (ret)
			بازگشت errno;

		data->unix_flock_flags = 0;
	}

	بازگشت 0;
}

ایستا errcode_t unix_flock(io_channel channel, unsigned صحیح flock_flags)
{
	ساختار unix_private_data *data;
	صحیح unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	اگر (ret)
		بازگشت ret;

	اگر (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	اگر (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	اگر (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	اگر (!unix_flock_flags)
		بازگشت 0;

	ret = flock(data->dev, unix_flock_flags);
	اگر (ret < 0)
		بازگشت errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	بازگشت 0;
}
#وگرنه
#define unix_flock		پوچ

ایستا errcode_t unix_funlock(io_channel channel)
{
	بازگشت 0;
}
#endif /* HAVE_SYS_FILE_H */

ایستا errcode_t unix_open_channel(ثابت نویسه *name, صحیح fd,
				   صحیح flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = پوچ;
	ساختار unix_private_data *data = پوچ;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	ساختار		utsname ut;
#endif

	اگر (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(تهی) fsync(fd);
#endif

	retval = ext2fs_get_mem(اندازه(ساختار struct_io_channel), &io);
	اگر (retval)
		برو cleanup;
	memset(io, 0, اندازه(ساختار struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(اندازه(ساختار unix_private_data), &data);
	اگر (retval)
		برو cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	اگر (retval)
		برو cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	اگر (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, اندازه(ساختار unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.رقم_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, اندازه(ساختار unix_cache));
	اگر (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		برو cleanup;
	}

#اگر defined(O_DIRECT)
	اگر (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#وگرنهاگر defined(F_NOCACHE)
	اگر (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	اگر (ext2fs_fstat(data->dev, &st) == 0) {
		اگر (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			صحیح zeroes = 0;

			اگر (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} وگرنه {
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
#endif

#اگر defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	اگر (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		صحیح dio_align = ext2fs_get_dio_alignment(fd);

		اگر (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	اگر ((retval = alloc_cache(io, data)))
		برو cleanup;

#ifdef BLKROGET
	اگر (flags & IO_FLAG_RW) {
		صحیح error;
		صحیح readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		اگر (!error && readonly) {
			retval = EPERM;
			برو cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#اگر (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#وگرنه
#define RLIM_INFINITY  (~0UL)
#endif
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
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		ساختار rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		اگر (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	اگر (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, پوچ);
		اگر (retval)
			برو cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, پوچ);
		اگر (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			برو cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, پوچ);
		اگر (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			برو cleanup;
		}
	}
#endif
	*channel = io;
	بازگشت 0;

cleanup:
	اگر (data) {
		unix_funlock(io);
		اگر (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		اگر (data->cache) {
			free_cache(data);
			آزاد(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	اگر (io) {
		اگر (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	بازگشت retval;
}

ایستا errcode_t unixfd_open(ثابت نویسه *str_fd, صحیح flags,
			     io_channel *channel)
{
	صحیح fd;
	صحیح fd_flags;

	fd = atoi(str_fd);
#اگر defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	اگر (fd_flags == -1)
		بازگشت EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	اگر (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#اگر defined(O_DIRECT)
	اگر (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	بازگشت unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ایستا errcode_t unix_open(ثابت نویسه *name, صحیح flags,
			   io_channel *channel)
{
	صحیح fd = -1;
	صحیح open_flags;

	اگر (name == 0)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	اگر (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#اگر defined(O_DIRECT)
	اگر (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	اگر (fd < 0)
		بازگشت errno;
#اگر defined(F_NOCACHE) && !defined(IO_DIRECT)
	اگر (flags & IO_FLAG_DIRECT_IO) {
		اگر (fcntl(fd, F_NOCACHE, 1) < 0)
			بازگشت errno;
	}
#endif
	بازگشت unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ایستا errcode_t unix_close(io_channel channel)
{
	ساختار unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		بازگشت 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	اگر (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	آزاد(data->cache);
#ifdef HAVE_PTHREAD
	اگر (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	بازگشت retval;
}

ایستا errcode_t unix_set_blksize(io_channel channel, صحیح blksize)
{
	ساختار unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		اگر ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			بازگشت retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	بازگشت retval;
}

ایستا errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       صحیح شمار, تهی *buf)
{
	ساختار unix_private_data *data;
	ساختار unix_cache *cache;
	errcode_t	retval;
	نویسه		*cp;
	صحیح		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	بازگشت raw_read_blk(channel, data, block, شمار, buf);
#وگرنه
	اگر (data->flags & IO_FLAG_NOCACHE)
		بازگشت raw_read_blk(channel, data, block, شمار, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	اگر (شمار < 0 || شمار > WRITE_DIRECT_SIZE) {
		اگر ((retval = flush_cached_blocks(channel, data, 0)))
			بازگشت retval;
		بازگشت raw_read_blk(channel, data, block, شمار, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	وقتی (شمار > 0) {
		/* If it's in the cache, use it! */
		اگر ((cache = find_cached_block(data, block, پوچ))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			شمار--;
			block++;
			cp += channel->block_size;
			ادامه;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		برای (i=1; i < شمار; i++)
			اگر (find_cached_block(data, block+i, پوچ))
				بشکن;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		اگر ((retval = raw_read_blk(channel, data, block, i, cp)))
			بازگشت retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		برای (j=0; j < i; j++) {
			اگر (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				اگر (retval)
					برو call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			شمار--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	بازگشت 0;

call_write_handler:
	اگر (cache->write_err && channel->write_error) {
		نویسه *err_buf = پوچ;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		اگر (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = پوچ;
		وگرنه
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		اگر (err_buf)
			ext2fs_free_mem(&err_buf);
	} وگرنه
		mutex_unlock(data, CACHE_MTX);
	بازگشت retval;
#endif /* NO_IO_CACHE */
}

ایستا errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	بازگشت unix_read_blk64(channel, block, شمار, buf);
}

ایستا errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				صحیح شمار, ثابت تهی *buf)
{
	ساختار unix_private_data *data;
	ساختار unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ثابت نویسه	*cp;
	صحیح		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	بازگشت raw_write_blk(channel, data, block, شمار, buf, 0);
#وگرنه
	اگر (data->flags & IO_FLAG_NOCACHE)
		بازگشت raw_write_blk(channel, data, block, شمار, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	اگر (شمار < 0 || شمار > WRITE_DIRECT_SIZE) {
		اگر ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			بازگشت retval;
		بازگشت raw_write_blk(channel, data, block, شمار, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	اگر (writethrough)
		retval = raw_write_blk(channel, data, block, شمار, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	وقتی (شمار > 0) {
		cache = find_cached_block(data, block, &reuse);
		اگر (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			اگر (err)
				برو call_write_handler;
		}
		اگر (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		شمار--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	بازگشت retval;

call_write_handler:
	اگر (cache->write_err && channel->write_error) {
		نویسه *err_buf = پوچ;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		اگر (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = پوچ;
		وگرنه
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		اگر (err_buf)
			ext2fs_free_mem(&err_buf);
	} وگرنه
		mutex_unlock(data, CACHE_MTX);
	بازگشت retval;
#endif /* NO_IO_CACHE */
}

ایستا errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long شمار)
{
#ifdef POSIX_FADV_WILLNEED
	ساختار unix_private_data *data;

	data = (ساختار unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	بازگشت posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)شمار * channel->block_size,
			     POSIX_FADV_WILLNEED);
#وگرنه
	بازگشت EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

ایستا errcode_t unix_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *buf)
{
	بازگشت unix_write_blk64(channel, block, شمار, buf);
}

ایستا errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 صحیح size, ثابت تهی *buf)
{
	ساختار unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		بازگشت EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	اگر ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		بازگشت retval;
#endif

	اگر (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		بازگشت errno;

	actual = اكتب(data->dev, buf, size);
	اگر (actual < 0)
		بازگشت errno;
	اگر (actual != size)
		بازگشت EXT2_ET_SHORT_WRITE;

	بازگشت 0;
}

/*
 * Flush data buffers to disk.
 */
ایستا errcode_t unix_flush(io_channel channel)
{
	ساختار unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	اگر (!retval && fsync(data->dev) != 0)
		بازگشت errno;
#endif
	بازگشت retval;
}

ایستا errcode_t unix_set_option(io_channel channel, ثابت نویسه *option,
				 ثابت نویسه *arg)
{
	ساختار unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	نویسه *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!strcmp(option, "offset")) {
		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		اگر (*end)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		اگر (data->offset < 0)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		بازگشت 0;
	}
	اگر (!strcmp(option, "cache")) {
		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		اگر (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			بازگشت 0;
		}
		اگر (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			بازگشت retval;
		}
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	اگر (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, پوچ, 0);
		اگر (errno || size == 0 || size > INT32_MAX)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		اگر (data->cache_size == size)
			بازگشت 0;
		اگر (data->cache_size > size)
			بازگشت shrink_cache(channel, data, size);
		بازگشت grow_cache(channel, data, size);
	}
#endif
	بازگشت EXT2_ET_INVALID_ARGUMENT;
}

#اگر defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

ایستا errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار unix_private_data *data;
	صحیح		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (channel->flags & CHANNEL_FLAGS_NODISCARD)
		برو unimplemented;

	اگر (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(شمار) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#وگرنه
		برو unimplemented;
#endif
	} وگرنه {
#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(شمار) * channel->block_size);
#وگرنه
		برو unimplemented;
#endif
	}
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			برو unimplemented;
		}
		بازگشت errno;
	}
	بازگشت 0;
unimplemented:
	بازگشت EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ایستا صحیح __unix_zeroout(صحیح fd, off_t offset, off_t len)
{
	صحیح ret = -1;

#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	اگر (ret == 0)
		بازگشت 0;
#endif
#اگر defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	اگر (ret == 0)
		بازگشت 0;
#endif
	errno = EOPNOTSUPP;
	بازگشت ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic افزودن
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ایستا errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار unix_private_data *data;
	صحیح		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		ساختار stat statbuf;

		اگر (شمار == 0)
			بازگشت 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		اگر (ret)
			برو err;
		اگر ((unsigned long long) statbuf.st_size <
			(block + شمار) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + شمار) * channel->block_size + data->offset);
			اگر (ret)
				برو err;
		}
	}

	اگر (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		برو unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(شمار) * channel->block_size);
err:
	اگر (ret < 0) {
		اگر (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			برو unimplemented;
		}
		بازگشت errno;
	}
	بازگشت 0;
unimplemented:
	بازگشت EXT2_ET_UNIMPLEMENTED;
}
#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic برداشت
#endif

ایستا ساختار struct_io_manager struct_unix_manager = {
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

ایستا ساختار struct_io_manager struct_unixfd_manager = {
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
