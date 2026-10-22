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

#ᰀᰧ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#ᰀᰧ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ᰀᰧ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ifdef __linux__
#include <sys/utsname.h>
#endif
#ᰀᰧ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#ᰀᰧ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᰀᰧ HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ᰀᰧ HAVE_LINUX_FALLOC_H
#include <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#ᰀᰧ defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

static anta_hstha puur_naa_nka find_last_bit_set(puur_naa_nka x)
{
	puur_naa_nka r = 32;

	ᰀᰧ (!x)
		ᰀᰧᰌ 0;
	ᰀᰧ (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	ᰀᰧ (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	ᰀᰧ (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	ᰀᰧ (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	ᰀᰧ (!(x & 0x80000000u)) {
		r -= 1;
	}
	ᰀᰧᰌ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
static anta_hstha puur_naa_nka highbit32(uint32_t v)
{
	ᰀᰧᰌ find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  ᰀᰧ ((sa_nracanaa)->magic != (code)) ᰀᰧᰌ (code)

sa_nracanaa unix_cache {
	var_na			*buf;
	unsigned long long	block;
	puur_naa_nka			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

sa_nracanaa unix_private_data {
	puur_naa_nka	magic;
	puur_naa_nka	dev;
	puur_naa_nka	flags;
	puur_naa_nka	align;
	puur_naa_nka	access_time;
	puur_naa_nka	unix_flock_flags;
	ext2_loff_t offset;
	sa_nracanaa unix_cache *cache;
	unsigned puur_naa_nka cache_size;
	unsigned puur_naa_nka cache_hash_shift;
	void	*bounce;
	sa_nracanaa struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

typedef ga_nanaa lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
static anta_hstha pthread_mutex_t *get_mutex(sa_nracanaa unix_private_data *data,
					 kind_t kind)
{
	ᰀᰧ (data->flags & IO_FLAG_THREADS) {
		switch (kind) {
		pak_sa CACHE_MTX:
			ᰀᰧᰌ &data->cache_mutex;
		pak_sa BOUNCE_MTX:
			ᰀᰧᰌ &data->bounce_mutex;
		pak_sa STATS_MTX:
			ᰀᰧᰌ &data->stats_mutex;
		}
	}
	ᰀᰧᰌ NULL;
}
#endif

static anta_hstha void mutex_lock(sa_nracanaa unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	ᰀᰧ (mtx)
		pthread_mutex_lock(mtx);
#endif
}

static anta_hstha void mutex_unlock(sa_nracanaa unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	ᰀᰧ (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

static errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	sa_nracanaa unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	ᰀᰧᰌ retval;
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      sa_nracanaa unix_private_data *data,
			      unsigned long long block,
			      puur_naa_nka count, void *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	puur_naa_nka		actual = 0;
	unsigned var_na	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	puur_naa_nka		align_size, offset;

	size = (count < 0) ? -count : (ext2_loff_t) count * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ᰀᰧ (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	ᰀᰧ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		ᰀᰧ (actual == size)
			ᰀᰧᰌ 0;
		actual = 0;
	}
#elif HAVE_PREAD
	/* Try an aligned pread */
	ᰀᰧ ((parimaa_na(off_t) >= parimaa_na(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		ᰀᰧ (actual == size)
			ᰀᰧᰌ 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	ᰀᰧ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		ᰀᰧ (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = read(data->dev, buf, size);
		ᰀᰧ (actual != size) {
		short_read:
			ᰀᰧ (actual < 0) {
				retval = errno;
				actual = 0;
			} ᰀᰧᰀ
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
	ᰀᰧ (channel->align == 0)
		channel->align = 1;
	ᰀᰧ ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	ᰀᰧᰀ
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	ᰀᰧ (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		goto error_unlock;
	}
	ᰀᰬ (size > 0) {
		actual = read(data->dev, data->bounce, align_size);
		ᰀᰧ (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			goto short_read;
		}
		ᰀᰧ ((actual + offset) > align_size)
			actual = align_size - offset;
		ᰀᰧ (actual > size)
			actual = size;
		memcpy(buf, (var_na *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	ᰀᰧᰌ 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	ᰀᰧ (actual >= 0 && actual < size)
		memset((var_na *) buf+actual, 0, size-actual);
	ᰀᰧ (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	ᰀᰧᰌ retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

static errcode_t raw_write_blk(io_channel channel,
			       sa_nracanaa unix_private_data *data,
			       unsigned long long block,
			       puur_naa_nka count, const void *bufv,
			       puur_naa_nka flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	puur_naa_nka		actual = 0;
	errcode_t	retval;
	const unsigned var_na *buf = bufv;
	unsigned long long aligned_blk;
	puur_naa_nka		align_size, offset;

	ᰀᰧ (count == 1)
		size = channel->block_size;
	ᰀᰧᰀ {
		ᰀᰧ (count < 0)
			size = -count;
		ᰀᰧᰀ
			size = (ext2_loff_t) count * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ᰀᰧ (data->flags & IO_FLAG_FORCE_BOUNCE)
		goto bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	ᰀᰧ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		ᰀᰧ (actual == size)
			ᰀᰧᰌ 0;
	}
#elif HAVE_PWRITE
	/* Try an aligned pwrite */
	ᰀᰧ ((parimaa_na(off_t) >= parimaa_na(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		ᰀᰧ (actual == size)
			ᰀᰧᰌ 0;
	}
#endif /* HAVE_PWRITE */

	ᰀᰧ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		ᰀᰧ (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		ᰀᰧ (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual = write(data->dev, buf, size);
		ᰀᰧ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		ᰀᰧ (actual < 0) {
			retval = errno;
			goto error_out;
		}
		ᰀᰧ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		ᰀᰧᰌ 0;
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
	ᰀᰧ (channel->align == 0)
		channel->align = 1;
	ᰀᰧ ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	ᰀᰧᰀ
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	ᰀᰬ (size > 0) {
		puur_naa_nka actual_w;

		ᰀᰧ (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		ᰀᰧ (size < align_size || offset) {
			ᰀᰧ (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				goto error_unlock;
			}
			actual = read(data->dev, data->bounce,
				      align_size);
			ᰀᰧ (actual != align_size) {
				ᰀᰧ (actual < 0) {
					retval = errno;
					goto error_unlock;
				}
				memset((var_na *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		ᰀᰧ ((actual + offset) > align_size)
			actual = align_size - offset;
		ᰀᰧ (actual > size)
			actual = size;
		memcpy(((var_na *)data->bounce) + offset, buf, actual);
		ᰀᰧ (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			goto error_unlock;
		}
		actual_w = write(data->dev, data->bounce, align_size);
		ᰀᰧ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		ᰀᰧ (actual_w < 0) {
			retval = errno;
			goto error_out;
		}
		ᰀᰧ (actual_w != align_size)
			goto short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	ᰀᰧᰌ 0;

error_unlock:
	ᰀᰧ (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	ᰀᰧ (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	ᰀᰧᰌ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
static errcode_t alloc_cache(io_channel channel,
			     sa_nracanaa unix_private_data *data)
{
	errcode_t		retval;
	sa_nracanaa unix_cache	*cache;
	unsigned puur_naa_nka		i;

	data->access_time = 0;
	for (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ᰀᰧ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
	}
	ᰀᰧ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ᰀᰧ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ᰀᰧᰌ retval;
}

/* Free the cache buffers */
static void free_cache(sa_nracanaa unix_private_data *data)
{
	sa_nracanaa unix_cache	*cache;
	unsigned puur_naa_nka		i;

	data->access_time = 0;
	for (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ᰀᰧ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ᰀᰧ (data->bounce)
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
static unsigned puur_naa_nka
cache_hash(sa_nracanaa unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	ᰀᰧ (data->cache_size <= DEFAULT_CACHE_SIZE)
		ᰀᰧᰌ 0;

	tmp = hashval ^ (GOLDEN_RATIO_PRIME + hashval) / CACHE_LINE_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_PRIME) >> data->cache_hash_shift);
	ᰀᰧᰌ tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
static sa_nracanaa unix_cache *find_cached_block(sa_nracanaa unix_private_data *data,
					    unsigned long long block,
					    sa_nracanaa unix_cache **eldest)
{
	sa_nracanaa unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned puur_naa_nka		hash = cache_hash(data, block);
	unsigned puur_naa_nka		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	for (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		ᰀᰧ (!cache->in_use) {
			ᰀᰧ (!unused_cache)
				unused_cache = cache;
			continue;
		}
		ᰀᰧ (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ᰀᰧᰌ cache;
		}
		ᰀᰧ (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	for (i = 0, cache = data->cache; i < hash; i++, cache++) {
		ᰀᰧ (!cache->in_use) {
			ᰀᰧ (!unused_cache)
				unused_cache = cache;
			continue;
		}
		ᰀᰧ (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ᰀᰧᰌ cache;
		}
		ᰀᰧ (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ᰀᰧ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	ᰀᰧᰌ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static errcode_t reuse_cache(io_channel channel,
		sa_nracanaa unix_private_data *data, sa_nracanaa unix_cache *cache,
		unsigned long long block)
{
	ᰀᰧ (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		ᰀᰧ (retval) {
			cache->write_err = 1;
			ᰀᰧᰌ retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	ᰀᰧᰌ 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
static errcode_t flush_cached_blocks(io_channel channel,
				     sa_nracanaa unix_private_data *data,
				     puur_naa_nka flags)
{
	sa_nracanaa unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned puur_naa_nka		i;
	puur_naa_nka			errors_found = 0;

	ᰀᰧ ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	for (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		ᰀᰧ (!cache->in_use)
			continue;
		ᰀᰧ (cache->dirty) {
			puur_naa_nka raw_flags = RAW_WRITE_NO_HANDLER;

			ᰀᰧ (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			ᰀᰧ (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} ᰀᰧᰀ {
				cache->dirty = 0;
				cache->write_err = 0;
				ᰀᰧ (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} ᰀᰧᰀ ᰀᰧ (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	ᰀᰧ ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	ᰀᰬ (errors_found) {
		ᰀᰧ ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		for (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			ᰀᰧ (!cache->in_use || !cache->write_err)
				continue;
			errors_found = 1;
			ᰀᰧ (cache->write_err && channel->write_error) {
				var_na *err_buf = NULL;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				ᰀᰧ (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = NULL;
				ᰀᰧᰀ
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				ᰀᰧ (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				goto retry;
			} ᰀᰧᰀ
				cache->write_err = 0;
		}
		ᰀᰧ ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	ᰀᰧᰌ retval2;
}

/* Shrink the cache buffers */
static errcode_t shrink_cache(io_channel channel,
			      sa_nracanaa unix_private_data *data,
			      unsigned puur_naa_nka new_size)
{
	sa_nracanaa unix_cache	*cache, *new_cache;
	unsigned puur_naa_nka		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ᰀᰧ (retval)
		goto unlock;

	for (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ᰀᰧ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * parimaa_na(sa_nracanaa unix_cache));
	ᰀᰧ (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ retval;
}

/* Grow the cache buffers */
static errcode_t grow_cache(io_channel channel,
			    sa_nracanaa unix_private_data *data,
			    unsigned puur_naa_nka new_size)
{
	sa_nracanaa unix_cache	*cache, *new_cache;
	unsigned puur_naa_nka		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ᰀᰧ (retval)
		goto unlock;

	new_cache = realloc(data->cache, new_size * parimaa_na(sa_nracanaa unix_cache));
	ᰀᰧ (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto unlock;
	}
	data->cache = new_cache;

	for (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ᰀᰧ (retval) {
			ᰀᰬ (i > data->cache_size) {
				cache--; i--;
				ᰀᰧ (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			goto unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

puur_naa_nka ext2fs_open_file(const var_na *pathname, puur_naa_nka flags, mode_t mode)
{
	ᰀᰧ (mode)
#ᰀᰧ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ᰀᰧᰌ open64(pathname, flags, mode);
	ᰀᰧᰀ
		ᰀᰧᰌ open64(pathname, flags);
#ᰀᰧᰀ
		ᰀᰧᰌ open(pathname, flags, mode);
	ᰀᰧᰀ
		ᰀᰧᰌ open(pathname, flags);
#endif
}

puur_naa_nka ext2fs_stat(const var_na *path, ext2fs_struct_stat *buf)
{
#ᰀᰧ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ᰀᰧᰌ stat64(path, buf);
#ᰀᰧᰀ
	ᰀᰧᰌ stat(path, buf);
#endif
}

puur_naa_nka ext2fs_fstat(puur_naa_nka fd, ext2fs_struct_stat *buf)
{
#ᰀᰧ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ᰀᰧᰌ fstat64(fd, buf);
#ᰀᰧᰀ
	ᰀᰧᰌ fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
static errcode_t unix_funlock(io_channel channel)
{
	sa_nracanaa unix_private_data *data;
	puur_naa_nka ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		ᰀᰧ (ret)
			ᰀᰧᰌ errno;

		data->unix_flock_flags = 0;
	}

	ᰀᰧᰌ 0;
}

static errcode_t unix_flock(io_channel channel, unsigned puur_naa_nka flock_flags)
{
	sa_nracanaa unix_private_data *data;
	puur_naa_nka unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	ᰀᰧ (ret)
		ᰀᰧᰌ ret;

	ᰀᰧ (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	ᰀᰧ (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	ᰀᰧ (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	ᰀᰧ (!unix_flock_flags)
		ᰀᰧᰌ 0;

	ret = flock(data->dev, unix_flock_flags);
	ᰀᰧ (ret < 0)
		ᰀᰧᰌ errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ᰀᰧᰌ 0;
}
#ᰀᰧᰀ
#define unix_flock		NULL

static errcode_t unix_funlock(io_channel channel)
{
	ᰀᰧᰌ 0;
}
#endif /* HAVE_SYS_FILE_H */

static errcode_t unix_open_channel(const var_na *name, puur_naa_nka fd,
				   puur_naa_nka flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = NULL;
	sa_nracanaa unix_private_data *data = NULL;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	sa_nracanaa		utsname ut;
#endif

	ᰀᰧ (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(void) fsync(fd);
#endif

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	ᰀᰧ (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa unix_private_data), &data);
	ᰀᰧ (retval)
		goto cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ᰀᰧ (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	ᰀᰧ (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, parimaa_na(sa_nracanaa unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.num_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, parimaa_na(sa_nracanaa unix_cache));
	ᰀᰧ (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		goto cleanup;
	}

#ᰀᰧ defined(O_DIRECT)
	ᰀᰧ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#elif defined(F_NOCACHE)
	ᰀᰧ (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ᰀᰧ (ext2fs_fstat(data->dev, &st) == 0) {
		ᰀᰧ (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			puur_naa_nka zeroes = 0;

			ᰀᰧ (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} ᰀᰧᰀ {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#ᰀᰧ defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	ᰀᰧ (!io->align)
		io->align = 512;
#endif

#ᰀᰧ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ᰀᰧ (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		puur_naa_nka dio_align = ext2fs_get_dio_alignment(fd);

		ᰀᰧ (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	ᰀᰧ ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	ᰀᰧ (flags & IO_FLAG_RW) {
		puur_naa_nka error;
		puur_naa_nka readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ᰀᰧ (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#ᰀᰧ (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#ᰀᰧᰀ
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	ᰀᰧ ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		sa_nracanaa rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		ᰀᰧ (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	ᰀᰧ (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, NULL);
		ᰀᰧ (retval)
			goto cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, NULL);
		ᰀᰧ (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			goto cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, NULL);
		ᰀᰧ (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			goto cleanup;
		}
	}
#endif
	*channel = io;
	ᰀᰧᰌ 0;

cleanup:
	ᰀᰧ (data) {
		unix_funlock(io);
		ᰀᰧ (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		ᰀᰧ (data->cache) {
			free_cache(data);
			free(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	ᰀᰧ (io) {
		ᰀᰧ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ᰀᰧᰌ retval;
}

static errcode_t unixfd_open(const var_na *str_fd, puur_naa_nka flags,
			     io_channel *channel)
{
	puur_naa_nka fd;
	puur_naa_nka fd_flags;

	fd = atoi(str_fd);
#ᰀᰧ defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	ᰀᰧ (fd_flags == -1)
		ᰀᰧᰌ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	ᰀᰧ (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#ᰀᰧ defined(O_DIRECT)
	ᰀᰧ (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	ᰀᰧᰌ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

static errcode_t unix_open(const var_na *name, puur_naa_nka flags,
			   io_channel *channel)
{
	puur_naa_nka fd = -1;
	puur_naa_nka open_flags;

	ᰀᰧ (name == 0)
		ᰀᰧᰌ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ᰀᰧ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ᰀᰧ defined(O_DIRECT)
	ᰀᰧ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	ᰀᰧ (fd < 0)
		ᰀᰧᰌ errno;
#ᰀᰧ defined(F_NOCACHE) && !defined(IO_DIRECT)
	ᰀᰧ (flags & IO_FLAG_DIRECT_IO) {
		ᰀᰧ (fcntl(fd, F_NOCACHE, 1) < 0)
			ᰀᰧᰌ errno;
	}
#endif
	ᰀᰧᰌ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

static errcode_t unix_close(io_channel channel)
{
	sa_nracanaa unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (--channel->refcount > 0)
		ᰀᰧᰌ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	ᰀᰧ (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	free(data->cache);
#ifdef HAVE_PTHREAD
	ᰀᰧ (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	ᰀᰧ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ᰀᰧᰌ retval;
}

static errcode_t unix_set_blksize(io_channel channel, puur_naa_nka blksize)
{
	sa_nracanaa unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		ᰀᰧ ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			ᰀᰧᰌ retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	ᰀᰧᰌ retval;
}

static errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       puur_naa_nka count, void *buf)
{
	sa_nracanaa unix_private_data *data;
	sa_nracanaa unix_cache *cache;
	errcode_t	retval;
	var_na		*cp;
	puur_naa_nka		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ᰀᰧᰌ raw_read_blk(channel, data, block, count, buf);
#ᰀᰧᰀ
	ᰀᰧ (data->flags & IO_FLAG_NOCACHE)
		ᰀᰧᰌ raw_read_blk(channel, data, block, count, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ᰀᰧ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ᰀᰧ ((retval = flush_cached_blocks(channel, data, 0)))
			ᰀᰧᰌ retval;
		ᰀᰧᰌ raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ᰀᰬ (count > 0) {
		/* If it's in the cache, use it! */
		ᰀᰧ ((cache = find_cached_block(data, block, NULL))) {
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
		for (i=1; i < count; i++)
			ᰀᰧ (find_cached_block(data, block+i, NULL))
				bha_nga;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		ᰀᰧ ((retval = raw_read_blk(channel, data, block, i, cp)))
			ᰀᰧᰌ retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		for (j=0; j < i; j++) {
			ᰀᰧ (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				ᰀᰧ (retval)
					goto call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			count--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ 0;

call_write_handler:
	ᰀᰧ (cache->write_err && channel->write_error) {
		var_na *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		ᰀᰧ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		ᰀᰧᰀ
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		ᰀᰧ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ᰀᰧᰀ
		mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ retval;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       puur_naa_nka count, void *buf)
{
	ᰀᰧᰌ unix_read_blk64(channel, block, count, buf);
}

static errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				puur_naa_nka count, const void *buf)
{
	sa_nracanaa unix_private_data *data;
	sa_nracanaa unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	const var_na	*cp;
	puur_naa_nka		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ᰀᰧᰌ raw_write_blk(channel, data, block, count, buf, 0);
#ᰀᰧᰀ
	ᰀᰧ (data->flags & IO_FLAG_NOCACHE)
		ᰀᰧᰌ raw_write_blk(channel, data, block, count, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ᰀᰧ (count < 0 || count > WRITE_DIRECT_SIZE) {
		ᰀᰧ ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			ᰀᰧᰌ retval;
		ᰀᰧᰌ raw_write_blk(channel, data, block, count, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ᰀᰧ (writethrough)
		retval = raw_write_blk(channel, data, block, count, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ᰀᰬ (count > 0) {
		cache = find_cached_block(data, block, &reuse);
		ᰀᰧ (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			ᰀᰧ (err)
				goto call_write_handler;
		}
		ᰀᰧ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		count--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ retval;

call_write_handler:
	ᰀᰧ (cache->write_err && channel->write_error) {
		var_na *err_buf = NULL;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		ᰀᰧ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = NULL;
		ᰀᰧᰀ
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		ᰀᰧ (err_buf)
			ext2fs_free_mem(&err_buf);
	} ᰀᰧᰀ
		mutex_unlock(data, CACHE_MTX);
	ᰀᰧᰌ retval;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
#ifdef POSIX_FADV_WILLNEED
	sa_nracanaa unix_private_data *data;

	data = (sa_nracanaa unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ᰀᰧᰌ posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)count * channel->block_size,
			     POSIX_FADV_WILLNEED);
#ᰀᰧᰀ
	ᰀᰧᰌ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

static errcode_t unix_write_blk(io_channel channel, unsigned long block,
				puur_naa_nka count, const void *buf)
{
	ᰀᰧᰌ unix_write_blk64(channel, block, count, buf);
}

static errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 puur_naa_nka size, const void *buf)
{
	sa_nracanaa unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	ᰀᰧ ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		ᰀᰧᰌ retval;
#endif

	ᰀᰧ (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		ᰀᰧᰌ errno;

	actual = write(data->dev, buf, size);
	ᰀᰧ (actual < 0)
		ᰀᰧᰌ errno;
	ᰀᰧ (actual != size)
		ᰀᰧᰌ EXT2_ET_SHORT_WRITE;

	ᰀᰧᰌ 0;
}

/*
 * Flush data buffers to disk.
 */
static errcode_t unix_flush(io_channel channel)
{
	sa_nracanaa unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	ᰀᰧ (!retval && fsync(data->dev) != 0)
		ᰀᰧᰌ errno;
#endif
	ᰀᰧᰌ retval;
}

static errcode_t unix_set_option(io_channel channel, const var_na *option,
				 const var_na *arg)
{
	sa_nracanaa unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	var_na *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (!strcmp(option, "offset")) {
		ᰀᰧ (!arg)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ᰀᰧ (*end)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ᰀᰧ (data->offset < 0)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;
		ᰀᰧᰌ 0;
	}
	ᰀᰧ (!strcmp(option, "cache")) {
		ᰀᰧ (!arg)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;
		ᰀᰧ (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			ᰀᰧᰌ 0;
		}
		ᰀᰧ (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			ᰀᰧᰌ retval;
		}
		ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	ᰀᰧ (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		ᰀᰧ (!arg)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, NULL, 0);
		ᰀᰧ (errno || size == 0 || size > INT32_MAX)
			ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

		ᰀᰧ (data->cache_size == size)
			ᰀᰧᰌ 0;
		ᰀᰧ (data->cache_size > size)
			ᰀᰧᰌ shrink_cache(channel, data, size);
		ᰀᰧᰌ grow_cache(channel, data, size);
	}
#endif
	ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;
}

#ᰀᰧ defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

static errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa unix_private_data *data;
	puur_naa_nka		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (channel->flags & CHANNEL_FLAGS_NODISCARD)
		goto unimplemented;

	ᰀᰧ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(count) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#ᰀᰧᰀ
		goto unimplemented;
#endif
	} ᰀᰧᰀ {
#ᰀᰧ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(count) * channel->block_size);
#ᰀᰧᰀ
		goto unimplemented;
#endif
	}
	ᰀᰧ (ret < 0) {
		ᰀᰧ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			goto unimplemented;
		}
		ᰀᰧᰌ errno;
	}
	ᰀᰧᰌ 0;
unimplemented:
	ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
static puur_naa_nka __unix_zeroout(puur_naa_nka fd, off_t offset, off_t len)
{
	puur_naa_nka ret = -1;

#ᰀᰧ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	ᰀᰧ (ret == 0)
		ᰀᰧᰌ 0;
#endif
#ᰀᰧ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	ᰀᰧ (ret == 0)
		ᰀᰧᰌ 0;
#endif
	errno = EOPNOTSUPP;
	ᰀᰧᰌ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#ᰀᰧ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
static errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa unix_private_data *data;
	puur_naa_nka		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ᰀᰧ (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		sa_nracanaa stat statbuf;

		ᰀᰧ (count == 0)
			ᰀᰧᰌ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ᰀᰧ (ret)
			goto err;
		ᰀᰧ ((unsigned long long) statbuf.st_size <
			(block + count) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + count) * channel->block_size + data->offset);
			ᰀᰧ (ret)
				goto err;
		}
	}

	ᰀᰧ (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		goto unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(count) * channel->block_size);
err:
	ᰀᰧ (ret < 0) {
		ᰀᰧ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			goto unimplemented;
		}
		ᰀᰧᰌ errno;
	}
	ᰀᰧᰌ 0;
unimplemented:
	ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
}
#ᰀᰧ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

static sa_nracanaa struct_io_manager struct_unix_manager = {
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

static sa_nracanaa struct_io_manager struct_unixfd_manager = {
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
