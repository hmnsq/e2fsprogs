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

#nếu !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ifdef __linux__
#include <sys/utsname.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#nếu HAVE_LINUX_FALLOC_H
#include <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#nếu defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

static inline số_nguyên find_last_bit_set(số_nguyên x)
{
	số_nguyên r = 32;

	nếu (!x)
		trả_về 0;
	nếu (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	nếu (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	nếu (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	nếu (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	nếu (!(x & 0x80000000u)) {
		r -= 1;
	}
	trả_về r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
static inline số_nguyên highbit32(uint32_t v)
{
	trả_về find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  nếu ((struct)->magic != (code)) trả_về (code)

struct unix_cache {
	char			*buf;
	unsigned long long	block;
	số_nguyên			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

struct unix_private_data {
	số_nguyên	magic;
	số_nguyên	dev;
	số_nguyên	flags;
	số_nguyên	align;
	số_nguyên	access_time;
	số_nguyên	unix_flock_flags;
	ext2_loff_t offset;
	struct unix_cache *cache;
	unsigned số_nguyên cache_size;
	unsigned số_nguyên cache_hash_shift;
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
	nếu (data->flags & IO_FLAG_THREADS) {
		switch (kind) {
		case CACHE_MTX:
			trả_về &data->cache_mutex;
		case BOUNCE_MTX:
			trả_về &data->bounce_mutex;
		case STATS_MTX:
			trả_về &data->stats_mutex;
		}
	}
	trả_về NULL;
}
#endif

static inline void mutex_lock(struct unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	nếu (mtx)
		pthread_mutex_lock(mtx);
#endif
}

static inline void mutex_unlock(struct unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	nếu (mtx)
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

	nếu (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	trả_về retval;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      struct unix_private_data *data,
			      unsigned long long block,
			      số_nguyên count, void *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	số_nguyên		actual = 0;
	unsigned char	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	số_nguyên		align_size, offset;

	size = (count < 0) ? -count : (ext2_loff_t) count * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	nếu (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	nếu ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		nếu (actual == size)
			trả_về 0;
		actual = 0;
	}
#elif HAVE_PREAD
	/* Try an aligned pread */
	nếu ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		nếu (actual == size)
			trả_về 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	nếu ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		nếu (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = read(data->dev, buf, size);
		nếu (actual != size) {
		short_read:
			nếu (actual < 0) {
				retval = errno;
				actual = 0;
			} không_thì
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
	nếu (channel->align == 0)
		channel->align = 1;
	nếu ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	không_thì
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	nếu (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		goto error_unlock;
	}
	trong_khi (size > 0) {
		actual = read(data->dev, data->bounce, align_size);
		nếu (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		nếu ((actual + offset) > align_size)
			actual = align_size - offset;
		nếu (actual > size)
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
	trả_về 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	nếu (actual >= 0 && actual < size)
		memset((char *) buf+actual, 0, size-actual);
	nếu (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	trả_về retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

static errcode_t raw_write_blk(io_channel channel,
			       struct unix_private_data *data,
			       unsigned long long block,
			       số_nguyên count, const void *bufv,
			       số_nguyên flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	số_nguyên		actual = 0;
	errcode_t	retval;
	const unsigned char *buf = bufv;
	unsigned long long aligned_blk;
	số_nguyên		align_size, offset;

	nếu (count == 1)
		size = channel->block_size;
	không_thì {
		nếu (count < 0)
			size = -count;
		không_thì
			size = (ext2_loff_t) count * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	nếu (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	nếu ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		nếu (actual == size)
			trả_về 0;
	}
#elif HAVE_PWRITE
	/* Try an aligned pwrite */
	nếu ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		nếu (actual == size)
			trả_về 0;
	}
#endif /* HAVE_PWRITE */

	nếu ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		nếu (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		nếu (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = write(data->dev, buf, size);
		nếu (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		nếu (actual < 0) {
			retval = errno;
			goto error_out;
		}
		nếu (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		trả_về 0;
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
	nếu (channel->align == 0)
		channel->align = 1;
	nếu ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	không_thì
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	trong_khi (size > 0) {
		số_nguyên actual_w;

		nếu (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		nếu (size < align_size || offset) {
			nếu (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				goto error_unlock;
			}
			actual = read(data->dev, data->bounce,
				      align_size);
			nếu (actual != align_size) {
				nếu (actual < 0) {
					retval = errno;
					goto error_unlock;
				}
				memset((char *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		nếu ((actual + offset) > align_size)
			actual = align_size - offset;
		nếu (actual > size)
			actual = size;
		memcpy(((char *)data->bounce) + offset, buf, actual);
		nếu (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual_w = write(data->dev, data->bounce, align_size);
		nếu (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		nếu (actual_w < 0) {
			retval = errno;
			goto error_out;
		}
		nếu (actual_w != align_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	trả_về 0;

error_unlock:
	nếu (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	nếu (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	trả_về retval;
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
	unsigned số_nguyên		i;

	data->access_time = 0;
	cho (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		nếu (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		nếu (retval)
			trả_về retval;
	}
	nếu (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		nếu (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	trả_về retval;
}

/* Free the cache buffers */
static void free_cache(struct unix_private_data *data)
{
	struct unix_cache	*cache;
	unsigned số_nguyên		i;

	data->access_time = 0;
	cho (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		nếu (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	nếu (data->bounce)
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
static unsigned số_nguyên
cache_hash(struct unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	nếu (data->cache_size <= DEFAULT_CACHE_SIZE)
		trả_về 0;

	tmp = hashval ^ (GOLDEN_RATIO_PRIME + hashval) / CACHE_LINE_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_PRIME) >> data->cache_hash_shift);
	trả_về tmp % data->cache_size;
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
	unsigned số_nguyên		hash = cache_hash(data, block);
	unsigned số_nguyên		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	cho (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		nếu (!cache->in_use) {
			nếu (!unused_cache)
				unused_cache = cache;
			continue;
		}
		nếu (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			trả_về cache;
		}
		nếu (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	cho (i = 0, cache = data->cache; i < hash; i++, cache++) {
		nếu (!cache->in_use) {
			nếu (!unused_cache)
				unused_cache = cache;
			continue;
		}
		nếu (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			trả_về cache;
		}
		nếu (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	nếu (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	trả_về 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static errcode_t reuse_cache(io_channel channel,
		struct unix_private_data *data, struct unix_cache *cache,
		unsigned long long block)
{
	nếu (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		nếu (retval) {
			cache->write_err = 1;
			trả_về retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	trả_về 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
static errcode_t flush_cached_blocks(io_channel channel,
				     struct unix_private_data *data,
				     số_nguyên flags)
{
	struct unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned số_nguyên		i;
	số_nguyên			errors_found = 0;

	nếu ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	cho (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		nếu (!cache->in_use)
			continue;
		nếu (cache->dirty) {
			số_nguyên raw_flags = RAW_WRITE_NO_HANDLER;

			nếu (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			nếu (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} không_thì {
				cache->dirty = 0;
				cache->write_err = 0;
				nếu (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} không_thì nếu (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	nếu ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	trong_khi (errors_found) {
		nếu ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		cho (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			nếu (!cache->in_use || !cache->write_err)
				continue;
			errors_found = 1;
			nếu (cache->write_err && channel->write_error) {
				char *err_buf = NULL;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				nếu (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = NULL;
				không_thì
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				nếu (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				goto retry;
			} không_thì
				cache->write_err = 0;
		}
		nếu ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	trả_về retval2;
}

/* Shrink the cache buffers */
static errcode_t shrink_cache(io_channel channel,
			      struct unix_private_data *data,
			      unsigned số_nguyên new_size)
{
	struct unix_cache	*cache, *new_cache;
	unsigned số_nguyên		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	nếu (retval)
		goto unlock;

	cho (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		nếu (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * sizeof(struct unix_cache));
	nếu (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	trả_về retval;
}

/* Grow the cache buffers */
static errcode_t grow_cache(io_channel channel,
			    struct unix_private_data *data,
			    unsigned số_nguyên new_size)
{
	struct unix_cache	*cache, *new_cache;
	unsigned số_nguyên		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	nếu (retval)
		goto unlock;

	new_cache = realloc(data->cache, new_size * sizeof(struct unix_cache));
	nếu (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}
	data->cache = new_cache;

	cho (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		nếu (retval) {
			trong_khi (i > data->cache_size) {
				cache--; i--;
				nếu (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			goto unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	trả_về retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

số_nguyên ext2fs_open_file(const char *pathname, số_nguyên flags, mode_t mode)
{
	nếu (mode)
#nếu defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		trả_về open64(pathname, flags, mode);
	không_thì
		trả_về open64(pathname, flags);
#không_thì
		trả_về open(pathname, flags, mode);
	không_thì
		trả_về open(pathname, flags);
#endif
}

số_nguyên ext2fs_stat(const char *path, ext2fs_struct_stat *buf)
{
#nếu defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	trả_về stat64(path, buf);
#không_thì
	trả_về stat(path, buf);
#endif
}

số_nguyên ext2fs_fstat(số_nguyên fd, ext2fs_struct_stat *buf)
{
#nếu defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	trả_về fstat64(fd, buf);
#không_thì
	trả_về fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
static errcode_t unix_funlock(io_channel channel)
{
	struct unix_private_data *data;
	số_nguyên ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		nếu (ret)
			trả_về errno;

		data->unix_flock_flags = 0;
	}

	trả_về 0;
}

static errcode_t unix_flock(io_channel channel, unsigned số_nguyên flock_flags)
{
	struct unix_private_data *data;
	số_nguyên unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	nếu (ret)
		trả_về ret;

	nếu (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	nếu (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	nếu (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	nếu (!unix_flock_flags)
		trả_về 0;

	ret = flock(data->dev, unix_flock_flags);
	nếu (ret < 0)
		trả_về errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	trả_về 0;
}
#không_thì
#define unix_flock		NULL

static errcode_t unix_funlock(io_channel channel)
{
	trả_về 0;
}
#endif /* HAVE_SYS_FILE_H */

static errcode_t unix_open_channel(const char *name, số_nguyên fd,
				   số_nguyên flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	struct unix_private_data *data = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	struct		utsname ut;
#endif

	nếu (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(void) fsync(fd);
#endif

	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	nếu (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(sizeof(struct unix_private_data), &data);
	nếu (retval)
		goto cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	nếu (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	nếu (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, sizeof(struct unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.num_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, sizeof(struct unix_cache));
	nếu (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto cleanup;
	}

#nếu defined(O_DIRECT)
	nếu (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#elif defined(F_NOCACHE)
	nếu (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	nếu (ext2fs_fstat(data->dev, &st) == 0) {
		nếu (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			số_nguyên zeroes = 0;

			nếu (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} không_thì {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#nếu defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	nếu (!io->align)
		io->align = 512;
#endif

#nếu defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	nếu (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		số_nguyên dio_align = ext2fs_get_dio_alignment(fd);

		nếu (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	nếu ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	nếu (flags & IO_FLAG_RW) {
		số_nguyên error;
		số_nguyên readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		nếu (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#nếu (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#không_thì
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	nếu ((flags & IO_FLAG_RW) &&
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
		nếu (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	nếu (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, NULL);
		nếu (retval)
			goto cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, NULL);
		nếu (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			goto cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, NULL);
		nếu (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			goto cleanup;
		}
	}
#endif
	*channel = io;
	trả_về 0;

cleanup:
	nếu (data) {
		unix_funlock(io);
		nếu (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		nếu (data->cache) {
			free_cache(data);
			free(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	nếu (io) {
		nếu (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	trả_về retval;
}

static errcode_t unixfd_open(const char *str_fd, số_nguyên flags,
			     io_channel *channel)
{
	số_nguyên fd;
	số_nguyên fd_flags;

	fd = atoi(str_fd);
#nếu defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	nếu (fd_flags == -1)
		trả_về EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	nếu (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#nếu defined(O_DIRECT)
	nếu (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	trả_về unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

static errcode_t unix_open(const char *name, số_nguyên flags,
			   io_channel *channel)
{
	số_nguyên fd = -1;
	số_nguyên open_flags;

	nếu (name == 0)
		trả_về EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	nếu (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#nếu defined(O_DIRECT)
	nếu (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	nếu (fd < 0)
		trả_về errno;
#nếu defined(F_NOCACHE) && !defined(IO_DIRECT)
	nếu (flags & IO_FLAG_DIRECT_IO) {
		nếu (fcntl(fd, F_NOCACHE, 1) < 0)
			trả_về errno;
	}
#endif
	trả_về unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

static errcode_t unix_close(io_channel channel)
{
	struct unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (--channel->refcount > 0)
		trả_về 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	nếu (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	free(data->cache);
#ifdef HAVE_PTHREAD
	nếu (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	nếu (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	trả_về retval;
}

static errcode_t unix_set_blksize(io_channel channel, số_nguyên blksize)
{
	struct unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		nếu ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			trả_về retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	trả_về retval;
}

static errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       số_nguyên count, void *buf)
{
	struct unix_private_data *data;
	struct unix_cache *cache;
	errcode_t	retval;
	char		*cp;
	số_nguyên		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	trả_về raw_read_blk(channel, data, block, count, buf);
#không_thì
	nếu (data->flags & IO_FLAG_NOCACHE)
		trả_về raw_read_blk(channel, data, block, count, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	nếu (count < 0 || count > WRITE_DIRECT_SIZE) {
		nếu ((retval = flush_cached_blocks(channel, data, 0)))
			trả_về retval;
		trả_về raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	trong_khi (count > 0) {
		/* If it's in the cache, use it! */
		nếu ((cache = find_cached_block(data, block, NULL))) {
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
		cho (i=1; i < count; i++)
			nếu (find_cached_block(data, block+i, NULL))
				break;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		nếu ((retval = raw_read_blk(channel, data, block, i, cp)))
			trả_về retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		cho (j=0; j < i; j++) {
			nếu (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				nếu (retval)
					goto call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			count--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	trả_về 0;

call_write_handler:
	nếu (cache->write_err && channel->write_error) {
		char *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		nếu (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		không_thì
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		nếu (err_buf)
			ext2fs_free_mem(&err_buf);
	} không_thì
		mutex_unlock(data, CACHE_MTX);
	trả_về retval;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       số_nguyên count, void *buf)
{
	trả_về unix_read_blk64(channel, block, count, buf);
}

static errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				số_nguyên count, const void *buf)
{
	struct unix_private_data *data;
	struct unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	const char	*cp;
	số_nguyên		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	trả_về raw_write_blk(channel, data, block, count, buf, 0);
#không_thì
	nếu (data->flags & IO_FLAG_NOCACHE)
		trả_về raw_write_blk(channel, data, block, count, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	nếu (count < 0 || count > WRITE_DIRECT_SIZE) {
		nếu ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			trả_về retval;
		trả_về raw_write_blk(channel, data, block, count, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	nếu (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	trong_khi (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		nếu (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			nếu (err)
				goto call_write_handler;
		}
		nếu (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	trả_về retval;

call_write_handler:
	nếu (cache->write_err && channel->write_error) {
		char *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		nếu (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		không_thì
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		nếu (err_buf)
			ext2fs_free_mem(&err_buf);
	} không_thì
		mutex_unlock(data, CACHE_MTX);
	trả_về retval;
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
	trả_về posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)count * channel->block_size,
			     POSIX_FADV_WILLNEED);
#không_thì
	trả_về EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

static errcode_t unix_write_blk(io_channel channel, unsigned long block,
				số_nguyên count, const void *buf)
{
	trả_về unix_write_blk64(channel, block, count, buf);
}

static errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 số_nguyên size, const void *buf)
{
	struct unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		trả_về EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	nếu ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		trả_về retval;
#endif

	nếu (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		trả_về errno;

	actual = write(data->dev, buf, size);
	nếu (actual < 0)
		trả_về errno;
	nếu (actual != size)
		trả_về EXT2_ET_SHORT_WRITE;

	trả_về 0;
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
	nếu (!retval && fsync(data->dev) != 0)
		trả_về errno;
#endif
	trả_về retval;
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

	nếu (!strcmp(option, "offset")) {
		nếu (!arg)
			trả_về EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		nếu (*end)
			trả_về EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		nếu (data->offset < 0)
			trả_về EXT2_ET_INVALID_ARGUMENT;
		trả_về 0;
	}
	nếu (!strcmp(option, "cache")) {
		nếu (!arg)
			trả_về EXT2_ET_INVALID_ARGUMENT;
		nếu (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			trả_về 0;
		}
		nếu (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			trả_về retval;
		}
		trả_về EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	nếu (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		nếu (!arg)
			trả_về EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, NULL, 0);
		nếu (errno || size == 0 || size > INT32_MAX)
			trả_về EXT2_ET_INVALID_ARGUMENT;

		nếu (data->cache_size == size)
			trả_về 0;
		nếu (data->cache_size > size)
			trả_về shrink_cache(channel, data, size);
		trả_về grow_cache(channel, data, size);
	}
#endif
	trả_về EXT2_ET_INVALID_ARGUMENT;
}

#nếu defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

static errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct unix_private_data *data;
	số_nguyên		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (channel->flags & CHANNEL_FLAGS_NODISCARD)
		goto unimplemented;

	nếu (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(count) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#không_thì
		goto unimplemented;
#endif
	} không_thì {
#nếu defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(count) * channel->block_size);
#không_thì
		goto unimplemented;
#endif
	}
	nếu (ret < 0) {
		nếu (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			goto unimplemented;
		}
		trả_về errno;
	}
	trả_về 0;
unimplemented:
	trả_về EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
static số_nguyên __unix_zeroout(số_nguyên fd, off_t offset, off_t len)
{
	số_nguyên ret = -1;

#nếu defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	nếu (ret == 0)
		trả_về 0;
#endif
#nếu defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	nếu (ret == 0)
		trả_về 0;
#endif
	errno = EOPNOTSUPP;
	trả_về ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#nếu __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct unix_private_data *data;
	số_nguyên		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	nếu (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		struct stat statbuf;

		nếu (count == 0)
			trả_về 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		nếu (ret)
			goto err;
		nếu ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			nếu (ret)
				goto err;
		}
	}

	nếu (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		goto unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(count) * channel->block_size);
err:
	nếu (ret < 0) {
		nếu (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			goto unimplemented;
		}
		trả_về errno;
	}
	trả_về 0;
unimplemented:
	trả_về EXT2_ET_UNIMPLEMENTED;
}
#nếu __GNUC_PREREQ (4, 6)
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
