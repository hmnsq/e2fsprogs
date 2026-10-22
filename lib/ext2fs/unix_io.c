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

#如果 !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#include "config.h"
#include <stdio.h>
#include <string.h>
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#如果 HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ifdef __linux__
#include <sys/utsname.h>
#endif
#如果 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#如果 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#如果 HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#如果 HAVE_LINUX_FALLOC_H
#include <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#如果 defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

static inline 整数 find_last_bit_set(整数 x)
{
	整数 r = 32;

	如果 (!x)
		返回 0;
	如果 (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	如果 (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	如果 (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	如果 (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	如果 (!(x & 0x80000000u)) {
		r -= 1;
	}
	返回 r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
static inline 整数 highbit32(uint32_t v)
{
	返回 find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  如果 ((struct)->magic != (code)) 返回 (code)

struct unix_cache {
	char			*buf;
	unsigned long long	block;
	整数			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

struct unix_private_data {
	整数	magic;
	整数	dev;
	整数	flags;
	整数	align;
	整数	access_time;
	整数	unix_flock_flags;
	ext2_loff_t offset;
	struct unix_cache *cache;
	unsigned 整数 cache_size;
	unsigned 整数 cache_hash_shift;
	void	*bounce;
	struct struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

typedef enum lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
static inline pthread_mutex_t *get_mutex(struct unix_private_data *data,
					 kind_t kind)
{
	如果 (data->flags & IO_FLAG_THREADS) {
		switch (kind) {
		case CACHE_MTX:
			返回 &data->cache_mutex;
		case BOUNCE_MTX:
			返回 &data->bounce_mutex;
		case STATS_MTX:
			返回 &data->stats_mutex;
		}
	}
	返回 NULL;
}
#endif

static inline void mutex_lock(struct unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	如果 (mtx)
		pthread_mutex_lock(mtx);
#endif
}

static inline void mutex_unlock(struct unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	如果 (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

static errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	struct unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	返回 retval;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      struct unix_private_data *data,
			      unsigned long long block,
			      整数 count, void *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	整数		actual = 0;
	unsigned char	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	整数		align_size, offset;

	size = (count < 0) ? -count : (ext2_loff_t) count * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	如果 (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	如果 ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		如果 (actual == size)
			返回 0;
		actual = 0;
	}
#elif HAVE_PREAD
	/* Try an aligned pread */
	如果 ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		如果 (actual == size)
			返回 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	如果 ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		如果 (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = read(data->dev, buf, size);
		如果 (actual != size) {
		short_read:
			如果 (actual < 0) {
				retval = errno;
				actual = 0;
			} 否则
				retval = EXT2_ET_SHORT_READ;
			goto error_unlock;
		}
		goto success_unlock;
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
	如果 (channel->align == 0)
		channel->align = 1;
	如果 ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	否则
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	如果 (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		goto error_unlock;
	}
	当 (size > 0) {
		actual = read(data->dev, data->bounce, align_size);
		如果 (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		如果 ((actual + offset) > align_size)
			actual = align_size - offset;
		如果 (actual > size)
			actual = size;
		memcpy(buf, (char *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	返回 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	如果 (actual >= 0 && actual < size)
		memset((char *) buf+actual, 0, size-actual);
	如果 (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	返回 retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

static errcode_t raw_write_blk(io_channel channel,
			       struct unix_private_data *data,
			       unsigned long long block,
			       整数 count, const void *bufv,
			       整数 flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	整数		actual = 0;
	errcode_t	retval;
	const unsigned char *buf = bufv;
	unsigned long long aligned_blk;
	整数		align_size, offset;

	如果 (count == 1)
		size = channel->block_size;
	否则 {
		如果 (count < 0)
			size = -count;
		否则
			size = (ext2_loff_t) count * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	如果 (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	如果 ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		如果 (actual == size)
			返回 0;
	}
#elif HAVE_PWRITE
	/* Try an aligned pwrite */
	如果 ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		如果 (actual == size)
			返回 0;
	}
#endif /* HAVE_PWRITE */

	如果 ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		如果 (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		如果 (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = write(data->dev, buf, size);
		如果 (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		如果 (actual < 0) {
			retval = errno;
			goto error_out;
		}
		如果 (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		返回 0;
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
	如果 (channel->align == 0)
		channel->align = 1;
	如果 ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	否则
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	当 (size > 0) {
		整数 actual_w;

		如果 (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		如果 (size < align_size || offset) {
			如果 (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				goto error_unlock;
			}
			actual = read(data->dev, data->bounce,
				      align_size);
			如果 (actual != align_size) {
				如果 (actual < 0) {
					retval = errno;
					goto error_unlock;
				}
				memset((char *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		如果 ((actual + offset) > align_size)
			actual = align_size - offset;
		如果 (actual > size)
			actual = size;
		memcpy(((char *)data->bounce) + offset, buf, actual);
		如果 (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual_w = write(data->dev, data->bounce, align_size);
		如果 (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		如果 (actual_w < 0) {
			retval = errno;
			goto error_out;
		}
		如果 (actual_w != align_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	返回 0;

error_unlock:
	如果 (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	如果 (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	返回 retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
static errcode_t alloc_cache(io_channel channel,
			     struct unix_private_data *data)
{
	errcode_t		retval;
	struct unix_cache	*cache;
	unsigned 整数		i;

	data->access_time = 0;
	为 (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		如果 (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		如果 (retval)
			返回 retval;
	}
	如果 (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		如果 (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	返回 retval;
}

/* Free the cache buffers */
static void free_cache(struct unix_private_data *data)
{
	struct unix_cache	*cache;
	unsigned 整数		i;

	data->access_time = 0;
	为 (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		如果 (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	如果 (data->bounce)
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
static unsigned 整数
cache_hash(struct unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	如果 (data->cache_size <= DEFAULT_CACHE_SIZE)
		返回 0;

	tmp = hashval ^ (GOLDEN_RATIO_PRIME + hashval) / CACHE_LINE_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_PRIME) >> data->cache_hash_shift);
	返回 tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
static struct unix_cache *find_cached_block(struct unix_private_data *data,
					    unsigned long long block,
					    struct unix_cache **eldest)
{
	struct unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned 整数		hash = cache_hash(data, block);
	unsigned 整数		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	为 (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		如果 (!cache->in_use) {
			如果 (!unused_cache)
				unused_cache = cache;
			continue;
		}
		如果 (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			返回 cache;
		}
		如果 (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	为 (i = 0, cache = data->cache; i < hash; i++, cache++) {
		如果 (!cache->in_use) {
			如果 (!unused_cache)
				unused_cache = cache;
			continue;
		}
		如果 (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			返回 cache;
		}
		如果 (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	如果 (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	返回 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static errcode_t reuse_cache(io_channel channel,
		struct unix_private_data *data, struct unix_cache *cache,
		unsigned long long block)
{
	如果 (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		如果 (retval) {
			cache->write_err = 1;
			返回 retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	返回 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
static errcode_t flush_cached_blocks(io_channel channel,
				     struct unix_private_data *data,
				     整数 flags)
{
	struct unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned 整数		i;
	整数			errors_found = 0;

	如果 ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	为 (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		如果 (!cache->in_use)
			continue;
		如果 (cache->dirty) {
			整数 raw_flags = RAW_WRITE_NO_HANDLER;

			如果 (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			如果 (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} 否则 {
				cache->dirty = 0;
				cache->write_err = 0;
				如果 (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} 否则 如果 (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	如果 ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	当 (errors_found) {
		如果 ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		为 (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			如果 (!cache->in_use || !cache->write_err)
				continue;
			errors_found = 1;
			如果 (cache->write_err && channel->write_error) {
				char *err_buf = NULL;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				如果 (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = NULL;
				否则
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				如果 (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				goto retry;
			} 否则
				cache->write_err = 0;
		}
		如果 ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	返回 retval2;
}

/* Shrink the cache buffers */
static errcode_t shrink_cache(io_channel channel,
			      struct unix_private_data *data,
			      unsigned 整数 new_size)
{
	struct unix_cache	*cache, *new_cache;
	unsigned 整数		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	如果 (retval)
		goto unlock;

	为 (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		如果 (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * sizeof(struct unix_cache));
	如果 (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	返回 retval;
}

/* Grow the cache buffers */
static errcode_t grow_cache(io_channel channel,
			    struct unix_private_data *data,
			    unsigned 整数 new_size)
{
	struct unix_cache	*cache, *new_cache;
	unsigned 整数		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	如果 (retval)
		goto unlock;

	new_cache = realloc(data->cache, new_size * sizeof(struct unix_cache));
	如果 (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}
	data->cache = new_cache;

	为 (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		如果 (retval) {
			当 (i > data->cache_size) {
				cache--; i--;
				如果 (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			goto unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	返回 retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

整数 ext2fs_open_file(const char *pathname, 整数 flags, mode_t mode)
{
	如果 (mode)
#如果 defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		返回 open64(pathname, flags, mode);
	否则
		返回 open64(pathname, flags);
#否则
		返回 open(pathname, flags, mode);
	否则
		返回 open(pathname, flags);
#endif
}

整数 ext2fs_stat(const char *path, ext2fs_struct_stat *buf)
{
#如果 defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	返回 stat64(path, buf);
#否则
	返回 stat(path, buf);
#endif
}

整数 ext2fs_fstat(整数 fd, ext2fs_struct_stat *buf)
{
#如果 defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	返回 fstat64(fd, buf);
#否则
	返回 fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
static errcode_t unix_funlock(io_channel channel)
{
	struct unix_private_data *data;
	整数 ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		如果 (ret)
			返回 errno;

		data->unix_flock_flags = 0;
	}

	返回 0;
}

static errcode_t unix_flock(io_channel channel, unsigned 整数 flock_flags)
{
	struct unix_private_data *data;
	整数 unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	如果 (ret)
		返回 ret;

	如果 (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	如果 (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	如果 (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	如果 (!unix_flock_flags)
		返回 0;

	ret = flock(data->dev, unix_flock_flags);
	如果 (ret < 0)
		返回 errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	返回 0;
}
#否则
#define unix_flock		NULL

static errcode_t unix_funlock(io_channel channel)
{
	返回 0;
}
#endif /* HAVE_SYS_FILE_H */

static errcode_t unix_open_channel(const char *name, 整数 fd,
				   整数 flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	struct unix_private_data *data = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	struct		utsname ut;
#endif

	如果 (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(void) fsync(fd);
#endif

	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	如果 (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(sizeof(struct unix_private_data), &data);
	如果 (retval)
		goto cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	如果 (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	如果 (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, sizeof(struct unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.num_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, sizeof(struct unix_cache));
	如果 (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto cleanup;
	}

#如果 defined(O_DIRECT)
	如果 (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#elif defined(F_NOCACHE)
	如果 (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	如果 (ext2fs_fstat(data->dev, &st) == 0) {
		如果 (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			整数 zeroes = 0;

			如果 (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} 否则 {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#如果 defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	如果 (!io->align)
		io->align = 512;
#endif

#如果 defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	如果 (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		整数 dio_align = ext2fs_get_dio_alignment(fd);

		如果 (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	如果 ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	如果 (flags & IO_FLAG_RW) {
		整数 error;
		整数 readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		如果 (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#如果 (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#否则
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	如果 ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		struct rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		如果 (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	如果 (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, NULL);
		如果 (retval)
			goto cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, NULL);
		如果 (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			goto cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, NULL);
		如果 (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			goto cleanup;
		}
	}
#endif
	*channel = io;
	返回 0;

cleanup:
	如果 (data) {
		unix_funlock(io);
		如果 (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		如果 (data->cache) {
			free_cache(data);
			free(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	如果 (io) {
		如果 (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	返回 retval;
}

static errcode_t unixfd_open(const char *str_fd, 整数 flags,
			     io_channel *channel)
{
	整数 fd;
	整数 fd_flags;

	fd = atoi(str_fd);
#如果 defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	如果 (fd_flags == -1)
		返回 EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	如果 (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#如果 defined(O_DIRECT)
	如果 (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	返回 unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

static errcode_t unix_open(const char *name, 整数 flags,
			   io_channel *channel)
{
	整数 fd = -1;
	整数 open_flags;

	如果 (name == 0)
		返回 EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	如果 (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#如果 defined(O_DIRECT)
	如果 (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	如果 (fd < 0)
		返回 errno;
#如果 defined(F_NOCACHE) && !defined(IO_DIRECT)
	如果 (flags & IO_FLAG_DIRECT_IO) {
		如果 (fcntl(fd, F_NOCACHE, 1) < 0)
			返回 errno;
	}
#endif
	返回 unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

static errcode_t unix_close(io_channel channel)
{
	struct unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (--channel->refcount > 0)
		返回 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	如果 (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	free(data->cache);
#ifdef HAVE_PTHREAD
	如果 (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	如果 (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	返回 retval;
}

static errcode_t unix_set_blksize(io_channel channel, 整数 blksize)
{
	struct unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		如果 ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			返回 retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	返回 retval;
}

static errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       整数 count, void *buf)
{
	struct unix_private_data *data;
	struct unix_cache *cache;
	errcode_t	retval;
	char		*cp;
	整数		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	返回 raw_read_blk(channel, data, block, count, buf);
#否则
	如果 (data->flags & IO_FLAG_NOCACHE)
		返回 raw_read_blk(channel, data, block, count, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	如果 (count < 0 || count > WRITE_DIRECT_SIZE) {
		如果 ((retval = flush_cached_blocks(channel, data, 0)))
			返回 retval;
		返回 raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	当 (count > 0) {
		/* If it's in the cache, use it! */
		如果 ((cache = find_cached_block(data, block, NULL))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		为 (i=1; i < count; i++)
			如果 (find_cached_block(data, block+i, NULL))
				break;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		如果 ((retval = raw_read_blk(channel, data, block, i, cp)))
			返回 retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		为 (j=0; j < i; j++) {
			如果 (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				如果 (retval)
					goto call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			count--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	返回 0;

call_write_handler:
	如果 (cache->write_err && channel->write_error) {
		char *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		如果 (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		否则
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		如果 (err_buf)
			ext2fs_free_mem(&err_buf);
	} 否则
		mutex_unlock(data, CACHE_MTX);
	返回 retval;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       整数 count, void *buf)
{
	返回 unix_read_blk64(channel, block, count, buf);
}

static errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				整数 count, const void *buf)
{
	struct unix_private_data *data;
	struct unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	const char	*cp;
	整数		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	返回 raw_write_blk(channel, data, block, count, buf, 0);
#否则
	如果 (data->flags & IO_FLAG_NOCACHE)
		返回 raw_write_blk(channel, data, block, count, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	如果 (count < 0 || count > WRITE_DIRECT_SIZE) {
		如果 ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			返回 retval;
		返回 raw_write_blk(channel, data, block, count, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	如果 (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	当 (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		如果 (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			如果 (err)
				goto call_write_handler;
		}
		如果 (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	返回 retval;

call_write_handler:
	如果 (cache->write_err && channel->write_error) {
		char *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		如果 (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		否则
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		如果 (err_buf)
			ext2fs_free_mem(&err_buf);
	} 否则
		mutex_unlock(data, CACHE_MTX);
	返回 retval;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
#ifdef POSIX_FADV_WILLNEED
	struct unix_private_data *data;

	data = (struct unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	返回 posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)count * channel->block_size,
			     POSIX_FADV_WILLNEED);
#否则
	返回 EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

static errcode_t unix_write_blk(io_channel channel, unsigned long block,
				整数 count, const void *buf)
{
	返回 unix_write_blk64(channel, block, count, buf);
}

static errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 整数 size, const void *buf)
{
	struct unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		返回 EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	如果 ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		返回 retval;
#endif

	如果 (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		返回 errno;

	actual = write(data->dev, buf, size);
	如果 (actual < 0)
		返回 errno;
	如果 (actual != size)
		返回 EXT2_ET_SHORT_WRITE;

	返回 0;
}

/*
 * Flush data buffers to disk.
 */
static errcode_t unix_flush(io_channel channel)
{
	struct unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	如果 (!retval && fsync(data->dev) != 0)
		返回 errno;
#endif
	返回 retval;
}

static errcode_t unix_set_option(io_channel channel, const char *option,
				 const char *arg)
{
	struct unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	char *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (!strcmp(option, "offset")) {
		如果 (!arg)
			返回 EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		如果 (*end)
			返回 EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		如果 (data->offset < 0)
			返回 EXT2_ET_INVALID_ARGUMENT;
		返回 0;
	}
	如果 (!strcmp(option, "cache")) {
		如果 (!arg)
			返回 EXT2_ET_INVALID_ARGUMENT;
		如果 (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			返回 0;
		}
		如果 (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			返回 retval;
		}
		返回 EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	如果 (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		如果 (!arg)
			返回 EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, NULL, 0);
		如果 (errno || size == 0 || size > INT32_MAX)
			返回 EXT2_ET_INVALID_ARGUMENT;

		如果 (data->cache_size == size)
			返回 0;
		如果 (data->cache_size > size)
			返回 shrink_cache(channel, data, size);
		返回 grow_cache(channel, data, size);
	}
#endif
	返回 EXT2_ET_INVALID_ARGUMENT;
}

#如果 defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

static errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct unix_private_data *data;
	整数		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (channel->flags & CHANNEL_FLAGS_NODISCARD)
		goto unimplemented;

	如果 (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(count) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#否则
		goto unimplemented;
#endif
	} 否则 {
#如果 defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(count) * channel->block_size);
#否则
		goto unimplemented;
#endif
	}
	如果 (ret < 0) {
		如果 (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			goto unimplemented;
		}
		返回 errno;
	}
	返回 0;
unimplemented:
	返回 EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
static 整数 __unix_zeroout(整数 fd, off_t offset, off_t len)
{
	整数 ret = -1;

#如果 defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	如果 (ret == 0)
		返回 0;
#endif
#如果 defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	如果 (ret == 0)
		返回 0;
#endif
	errno = EOPNOTSUPP;
	返回 ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#如果 __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct unix_private_data *data;
	整数		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	如果 (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		struct stat statbuf;

		如果 (count == 0)
			返回 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		如果 (ret)
			goto err;
		如果 ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			如果 (ret)
				goto err;
		}
	}

	如果 (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		goto unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(count) * channel->block_size);
err:
	如果 (ret < 0) {
		如果 (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			goto unimplemented;
		}
		返回 errno;
	}
	返回 0;
unimplemented:
	返回 EXT2_ET_UNIMPLEMENTED;
}
#如果 __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

static struct struct_io_manager struct_unix_manager = {
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

static struct struct_io_manager struct_unixfd_manager = {
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
