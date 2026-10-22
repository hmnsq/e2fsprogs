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

#எனில் !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#ifdef __linux__
#அந்தர்பாவ <sys/utsname.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#அந்தர்பாவ <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#அந்தர்பாவ <sys/mount.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_RESOURCE_H
#அந்தர்பாவ <sys/resource.h>
#endif
#எனில் HAVE_LINUX_FALLOC_H
#அந்தர்பாவ <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#அந்தர்பாவ <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#அந்தர்பாவ <sys/file.h>
#endif

#எனில் defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

நிலையான உள்வரி முழு find_last_bit_set(முழு x)
{
	முழு r = 32;

	எனில் (!x)
		திரும்பு 0;
	எனில் (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	எனில் (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	எனில் (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	எனில் (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	எனில் (!(x & 0x80000000u)) {
		r -= 1;
	}
	திரும்பு r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
நிலையான உள்வரி முழு highbit32(uint32_t v)
{
	திரும்பு find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)

கட்டமைப்பு unix_cache {
	எழுத்து			*buf;
	unsigned long long	block;
	முழு			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

கட்டமைப்பு unix_private_data {
	முழு	magic;
	முழு	dev;
	முழு	flags;
	முழு	align;
	முழு	access_time;
	முழு	unix_flock_flags;
	ext2_loff_t offset;
	கட்டமைப்பு unix_cache *cache;
	unsigned முழு cache_size;
	unsigned முழு cache_hash_shift;
	வெறுமை	*bounce;
	கட்டமைப்பு struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

வகைவரை எண்ணிக்கை lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
நிலையான உள்வரி pthread_mutex_t *get_mutex(கட்டமைப்பு unix_private_data *data,
					 kind_t kind)
{
	எனில் (data->flags & IO_FLAG_THREADS) {
		தேர்வு (kind) {
		நிலை CACHE_MTX:
			திரும்பு &data->cache_mutex;
		நிலை BOUNCE_MTX:
			திரும்பு &data->bounce_mutex;
		நிலை STATS_MTX:
			திரும்பு &data->stats_mutex;
		}
	}
	திரும்பு வெற்று;
}
#endif

நிலையான உள்வரி வெறுமை mutex_lock(கட்டமைப்பு unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	எனில் (mtx)
		pthread_mutex_lock(mtx);
#endif
}

நிலையான உள்வரி வெறுமை mutex_unlock(கட்டமைப்பு unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	எனில் (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

நிலையான errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	கட்டமைப்பு unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	திரும்பு retval;
}

/*
 * Here are the raw I/O functions
 */
நிலையான errcode_t raw_read_blk(io_channel channel,
			      கட்டமைப்பு unix_private_data *data,
			      unsigned long long block,
			      முழு மொத்தம், வெறுமை *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	முழு		actual = 0;
	unsigned எழுத்து	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	முழு		align_size, offset;

	size = (மொத்தம் < 0) ? -மொத்தம் : (ext2_loff_t) மொத்தம் * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	எனில் (data->flags & IO_FLAG_FORCE_BOUNCE)
		செல் bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	எனில் ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		எனில் (actual == size)
			திரும்பு 0;
		actual = 0;
	}
#அல்லதெனில் HAVE_PREAD
	/* Try an aligned pread */
	எனில் ((அளவு(off_t) >= அளவு(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		எனில் (actual == size)
			திரும்பு 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	எனில் ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		எனில் (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			செல் error_unlock;
		}
		actual = பட(data->dev, buf, size);
		எனில் (actual != size) {
		short_read:
			எனில் (actual < 0) {
				retval = errno;
				actual = 0;
			} இல்லையெனில்
				retval = EXT2_ET_SHORT_READ;
			செல் error_unlock;
		}
		செல் success_unlock;
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
	எனில் (channel->align == 0)
		channel->align = 1;
	எனில் ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	இல்லையெனில்
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	எனில் (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		செல் error_unlock;
	}
	போது (size > 0) {
		actual = பட(data->dev, data->bounce, align_size);
		எனில் (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			செல் short_read;
		}
		எனில் ((actual + offset) > align_size)
			actual = align_size - offset;
		எனில் (actual > size)
			actual = size;
		memcpy(buf, (எழுத்து *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	திரும்பு 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	எனில் (actual >= 0 && actual < size)
		memset((எழுத்து *) buf+actual, 0, size-actual);
	எனில் (channel->read_error)
		retval = (channel->read_error)(channel, block, மொத்தம், buf,
					       size, actual, retval);
	திரும்பு retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

நிலையான errcode_t raw_write_blk(io_channel channel,
			       கட்டமைப்பு unix_private_data *data,
			       unsigned long long block,
			       முழு மொத்தம், மாறிலி வெறுமை *bufv,
			       முழு flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	முழு		actual = 0;
	errcode_t	retval;
	மாறிலி unsigned எழுத்து *buf = bufv;
	unsigned long long aligned_blk;
	முழு		align_size, offset;

	எனில் (மொத்தம் == 1)
		size = channel->block_size;
	இல்லையெனில் {
		எனில் (மொத்தம் < 0)
			size = -மொத்தம்;
		இல்லையெனில்
			size = (ext2_loff_t) மொத்தம் * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	எனில் (data->flags & IO_FLAG_FORCE_BOUNCE)
		செல் bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	எனில் ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		எனில் (actual == size)
			திரும்பு 0;
	}
#அல்லதெனில் HAVE_PWRITE
	/* Try an aligned pwrite */
	எனில் ((அளவு(off_t) >= அளவு(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		எனில் (actual == size)
			திரும்பு 0;
	}
#endif /* HAVE_PWRITE */

	எனில் ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		எனில் (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		எனில் (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			செல் error_unlock;
		}
		actual = லிக(data->dev, buf, size);
		எனில் (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		எனில் (actual < 0) {
			retval = errno;
			செல் error_out;
		}
		எனில் (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			செல் error_out;
		}
		திரும்பு 0;
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
	எனில் (channel->align == 0)
		channel->align = 1;
	எனில் ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	இல்லையெனில்
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	போது (size > 0) {
		முழு actual_w;

		எனில் (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		எனில் (size < align_size || offset) {
			எனில் (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				செல் error_unlock;
			}
			actual = பட(data->dev, data->bounce,
				      align_size);
			எனில் (actual != align_size) {
				எனில் (actual < 0) {
					retval = errno;
					செல் error_unlock;
				}
				memset((எழுத்து *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		எனில் ((actual + offset) > align_size)
			actual = align_size - offset;
		எனில் (actual > size)
			actual = size;
		memcpy(((எழுத்து *)data->bounce) + offset, buf, actual);
		எனில் (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			செல் error_unlock;
		}
		actual_w = லிக(data->dev, data->bounce, align_size);
		எனில் (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		எனில் (actual_w < 0) {
			retval = errno;
			செல் error_out;
		}
		எனில் (actual_w != align_size)
			செல் short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	திரும்பு 0;

error_unlock:
	எனில் (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	எனில் (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, மொத்தம், buf,
						size, actual, retval);
	திரும்பு retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
நிலையான errcode_t alloc_cache(io_channel channel,
			     கட்டமைப்பு unix_private_data *data)
{
	errcode_t		retval;
	கட்டமைப்பு unix_cache	*cache;
	unsigned முழு		i;

	data->access_time = 0;
	க்கு (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		எனில் (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		எனில் (retval)
			திரும்பு retval;
	}
	எனில் (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		எனில் (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	திரும்பு retval;
}

/* Free the cache buffers */
நிலையான வெறுமை free_cache(கட்டமைப்பு unix_private_data *data)
{
	கட்டமைப்பு unix_cache	*cache;
	unsigned முழு		i;

	data->access_time = 0;
	க்கு (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		எனில் (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	எனில் (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_பகாஎண்	0x9e37fffffffc0001UL
#ifndef CACHE_வரி_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_வரி_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
நிலையான unsigned முழு
cache_hash(கட்டமைப்பு unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	எனில் (data->cache_size <= DEFAULT_CACHE_SIZE)
		திரும்பு 0;

	tmp = hashval ^ (GOLDEN_RATIO_பகாஎண் + hashval) / CACHE_வரி_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_பகாஎண்) >> data->cache_hash_shift);
	திரும்பு tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
நிலையான கட்டமைப்பு unix_cache *find_cached_block(கட்டமைப்பு unix_private_data *data,
					    unsigned long long block,
					    கட்டமைப்பு unix_cache **eldest)
{
	கட்டமைப்பு unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned முழு		hash = cache_hash(data, block);
	unsigned முழு		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	க்கு (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		எனில் (!cache->in_use) {
			எனில் (!unused_cache)
				unused_cache = cache;
			தொடர்;
		}
		எனில் (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			திரும்பு cache;
		}
		எனில் (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	க்கு (i = 0, cache = data->cache; i < hash; i++, cache++) {
		எனில் (!cache->in_use) {
			எனில் (!unused_cache)
				unused_cache = cache;
			தொடர்;
		}
		எனில் (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			திரும்பு cache;
		}
		எனில் (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	எனில் (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	திரும்பு 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
நிலையான errcode_t reuse_cache(io_channel channel,
		கட்டமைப்பு unix_private_data *data, கட்டமைப்பு unix_cache *cache,
		unsigned long long block)
{
	எனில் (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		எனில் (retval) {
			cache->write_err = 1;
			திரும்பு retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	திரும்பு 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
நிலையான errcode_t flush_cached_blocks(io_channel channel,
				     கட்டமைப்பு unix_private_data *data,
				     முழு flags)
{
	கட்டமைப்பு unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned முழு		i;
	முழு			errors_found = 0;

	எனில் ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	க்கு (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		எனில் (!cache->in_use)
			தொடர்;
		எனில் (cache->dirty) {
			முழு raw_flags = RAW_WRITE_NO_HANDLER;

			எனில் (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			எனில் (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} இல்லையெனில் {
				cache->dirty = 0;
				cache->write_err = 0;
				எனில் (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} இல்லையெனில் எனில் (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	எனில் ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	போது (errors_found) {
		எனில் ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		க்கு (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			எனில் (!cache->in_use || !cache->write_err)
				தொடர்;
			errors_found = 1;
			எனில் (cache->write_err && channel->write_error) {
				எழுத்து *err_buf = வெற்று;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				எனில் (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = வெற்று;
				இல்லையெனில்
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				எனில் (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				செல் retry;
			} இல்லையெனில்
				cache->write_err = 0;
		}
		எனில் ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	திரும்பு retval2;
}

/* Shrink the cache buffers */
நிலையான errcode_t shrink_cache(io_channel channel,
			      கட்டமைப்பு unix_private_data *data,
			      unsigned முழு new_size)
{
	கட்டமைப்பு unix_cache	*cache, *new_cache;
	unsigned முழு		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	எனில் (retval)
		செல் unlock;

	க்கு (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		எனில் (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * அளவு(கட்டமைப்பு unix_cache));
	எனில் (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		செல் unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	திரும்பு retval;
}

/* Grow the cache buffers */
நிலையான errcode_t grow_cache(io_channel channel,
			    கட்டமைப்பு unix_private_data *data,
			    unsigned முழு new_size)
{
	கட்டமைப்பு unix_cache	*cache, *new_cache;
	unsigned முழு		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	எனில் (retval)
		செல் unlock;

	new_cache = realloc(data->cache, new_size * அளவு(கட்டமைப்பு unix_cache));
	எனில் (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		செல் unlock;
	}
	data->cache = new_cache;

	க்கு (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		எனில் (retval) {
			போது (i > data->cache_size) {
				cache--; i--;
				எனில் (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			செல் unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	திரும்பு retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

முழு ext2fs_open_file(மாறிலி எழுத்து *pathname, முழு flags, mode_t mode)
{
	எனில் (mode)
#எனில் defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		திரும்பு open64(pathname, flags, mode);
	இல்லையெனில்
		திரும்பு open64(pathname, flags);
#இல்லையெனில்
		திரும்பு open(pathname, flags, mode);
	இல்லையெனில்
		திரும்பு open(pathname, flags);
#endif
}

முழு ext2fs_stat(மாறிலி எழுத்து *path, ext2fs_struct_stat *buf)
{
#எனில் defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	திரும்பு stat64(path, buf);
#இல்லையெனில்
	திரும்பு stat(path, buf);
#endif
}

முழு ext2fs_fstat(முழு fd, ext2fs_struct_stat *buf)
{
#எனில் defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	திரும்பு fstat64(fd, buf);
#இல்லையெனில்
	திரும்பு fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
நிலையான errcode_t unix_funlock(io_channel channel)
{
	கட்டமைப்பு unix_private_data *data;
	முழு ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		எனில் (ret)
			திரும்பு errno;

		data->unix_flock_flags = 0;
	}

	திரும்பு 0;
}

நிலையான errcode_t unix_flock(io_channel channel, unsigned முழு flock_flags)
{
	கட்டமைப்பு unix_private_data *data;
	முழு unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	எனில் (ret)
		திரும்பு ret;

	எனில் (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	எனில் (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	எனில் (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	எனில் (!unix_flock_flags)
		திரும்பு 0;

	ret = flock(data->dev, unix_flock_flags);
	எனில் (ret < 0)
		திரும்பு errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	திரும்பு 0;
}
#இல்லையெனில்
#define unix_flock		வெற்று

நிலையான errcode_t unix_funlock(io_channel channel)
{
	திரும்பு 0;
}
#endif /* HAVE_SYS_FILE_H */

நிலையான errcode_t unix_open_channel(மாறிலி எழுத்து *name, முழு fd,
				   முழு flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = வெற்று;
	கட்டமைப்பு unix_private_data *data = வெற்று;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	கட்டமைப்பு		utsname ut;
#endif

	எனில் (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(வெறுமை) fsync(fd);
#endif

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_io_channel), &io);
	எனில் (retval)
		செல் cleanup;
	memset(io, 0, அளவு(கட்டமைப்பு struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு unix_private_data), &data);
	எனில் (retval)
		செல் cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	எனில் (retval)
		செல் cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	எனில் (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, அளவு(கட்டமைப்பு unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.இலக்கம்_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, அளவு(கட்டமைப்பு unix_cache));
	எனில் (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		செல் cleanup;
	}

#எனில் defined(O_DIRECT)
	எனில் (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#அல்லதெனில் defined(F_NOCACHE)
	எனில் (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	எனில் (ext2fs_fstat(data->dev, &st) == 0) {
		எனில் (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			முழு zeroes = 0;

			எனில் (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} இல்லையெனில் {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#எனில் defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	எனில் (!io->align)
		io->align = 512;
#endif

#எனில் defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	எனில் (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		முழு dio_align = ext2fs_get_dio_alignment(fd);

		எனில் (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	எனில் ((retval = alloc_cache(io, data)))
		செல் cleanup;

#ifdef BLKROGET
	எனில் (flags & IO_FLAG_RW) {
		முழு error;
		முழு readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		எனில் (!error && readonly) {
			retval = EPERM;
			செல் cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#எனில் (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#இல்லையெனில்
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	எனில் ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		கட்டமைப்பு rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		எனில் (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	எனில் (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, வெற்று);
		எனில் (retval)
			செல் cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, வெற்று);
		எனில் (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			செல் cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, வெற்று);
		எனில் (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			செல் cleanup;
		}
	}
#endif
	*channel = io;
	திரும்பு 0;

cleanup:
	எனில் (data) {
		unix_funlock(io);
		எனில் (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		எனில் (data->cache) {
			free_cache(data);
			விடுவி(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	எனில் (io) {
		எனில் (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	திரும்பு retval;
}

நிலையான errcode_t unixfd_open(மாறிலி எழுத்து *str_fd, முழு flags,
			     io_channel *channel)
{
	முழு fd;
	முழு fd_flags;

	fd = atoi(str_fd);
#எனில் defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	எனில் (fd_flags == -1)
		திரும்பு EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	எனில் (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#எனில் defined(O_DIRECT)
	எனில் (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	திரும்பு unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

நிலையான errcode_t unix_open(மாறிலி எழுத்து *name, முழு flags,
			   io_channel *channel)
{
	முழு fd = -1;
	முழு open_flags;

	எனில் (name == 0)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	எனில் (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#எனில் defined(O_DIRECT)
	எனில் (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	எனில் (fd < 0)
		திரும்பு errno;
#எனில் defined(F_NOCACHE) && !defined(IO_DIRECT)
	எனில் (flags & IO_FLAG_DIRECT_IO) {
		எனில் (fcntl(fd, F_NOCACHE, 1) < 0)
			திரும்பு errno;
	}
#endif
	திரும்பு unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

நிலையான errcode_t unix_close(io_channel channel)
{
	கட்டமைப்பு unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
		திரும்பு 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	எனில் (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	விடுவி(data->cache);
#ifdef HAVE_PTHREAD
	எனில் (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	எனில் (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	திரும்பு retval;
}

நிலையான errcode_t unix_set_blksize(io_channel channel, முழு blksize)
{
	கட்டமைப்பு unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		எனில் ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			திரும்பு retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	திரும்பு retval;
}

நிலையான errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	கட்டமைப்பு unix_private_data *data;
	கட்டமைப்பு unix_cache *cache;
	errcode_t	retval;
	எழுத்து		*cp;
	முழு		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	திரும்பு raw_read_blk(channel, data, block, மொத்தம், buf);
#இல்லையெனில்
	எனில் (data->flags & IO_FLAG_NOCACHE)
		திரும்பு raw_read_blk(channel, data, block, மொத்தம், buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	எனில் (மொத்தம் < 0 || மொத்தம் > WRITE_DIRECT_SIZE) {
		எனில் ((retval = flush_cached_blocks(channel, data, 0)))
			திரும்பு retval;
		திரும்பு raw_read_blk(channel, data, block, மொத்தம், buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	போது (மொத்தம் > 0) {
		/* If it's in the cache, use it! */
		எனில் ((cache = find_cached_block(data, block, வெற்று))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			மொத்தம்--;
			block++;
			cp += channel->block_size;
			தொடர்;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		க்கு (i=1; i < மொத்தம்; i++)
			எனில் (find_cached_block(data, block+i, வெற்று))
				நிறுத்து;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		எனில் ((retval = raw_read_blk(channel, data, block, i, cp)))
			திரும்பு retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		க்கு (j=0; j < i; j++) {
			எனில் (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				எனில் (retval)
					செல் call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			மொத்தம்--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	திரும்பு 0;

call_write_handler:
	எனில் (cache->write_err && channel->write_error) {
		எழுத்து *err_buf = வெற்று;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		எனில் (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = வெற்று;
		இல்லையெனில்
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		எனில் (err_buf)
			ext2fs_free_mem(&err_buf);
	} இல்லையெனில்
		mutex_unlock(data, CACHE_MTX);
	திரும்பு retval;
#endif /* NO_IO_CACHE */
}

நிலையான errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	திரும்பு unix_read_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு unix_private_data *data;
	கட்டமைப்பு unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	மாறிலி எழுத்து	*cp;
	முழு		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	திரும்பு raw_write_blk(channel, data, block, மொத்தம், buf, 0);
#இல்லையெனில்
	எனில் (data->flags & IO_FLAG_NOCACHE)
		திரும்பு raw_write_blk(channel, data, block, மொத்தம், buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	எனில் (மொத்தம் < 0 || மொத்தம் > WRITE_DIRECT_SIZE) {
		எனில் ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			திரும்பு retval;
		திரும்பு raw_write_blk(channel, data, block, மொத்தம், buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	எனில் (writethrough)
		retval = raw_write_blk(channel, data, block, மொத்தம், buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	போது (மொத்தம் > 0) {
		cache = find_cached_block(data, block, &reuse);
		எனில் (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			எனில் (err)
				செல் call_write_handler;
		}
		எனில் (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		மொத்தம்--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	திரும்பு retval;

call_write_handler:
	எனில் (cache->write_err && channel->write_error) {
		எழுத்து *err_buf = வெற்று;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		எனில் (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = வெற்று;
		இல்லையெனில்
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		எனில் (err_buf)
			ext2fs_free_mem(&err_buf);
	} இல்லையெனில்
		mutex_unlock(data, CACHE_MTX);
	திரும்பு retval;
#endif /* NO_IO_CACHE */
}

நிலையான errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long மொத்தம்)
{
#ifdef POSIX_FADV_WILLNEED
	கட்டமைப்பு unix_private_data *data;

	data = (கட்டமைப்பு unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	திரும்பு posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)மொத்தம் * channel->block_size,
			     POSIX_FADV_WILLNEED);
#இல்லையெனில்
	திரும்பு EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

நிலையான errcode_t unix_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	திரும்பு unix_write_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 முழு size, மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		திரும்பு EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	எனில் ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		திரும்பு retval;
#endif

	எனில் (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		திரும்பு errno;

	actual = லிக(data->dev, buf, size);
	எனில் (actual < 0)
		திரும்பு errno;
	எனில் (actual != size)
		திரும்பு EXT2_ET_SHORT_WRITE;

	திரும்பு 0;
}

/*
 * Flush data buffers to disk.
 */
நிலையான errcode_t unix_flush(io_channel channel)
{
	கட்டமைப்பு unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	எனில் (!retval && fsync(data->dev) != 0)
		திரும்பு errno;
#endif
	திரும்பு retval;
}

நிலையான errcode_t unix_set_option(io_channel channel, மாறிலி எழுத்து *option,
				 மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	எழுத்து *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (!strcmp(option, "offset")) {
		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		எனில் (*end)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		எனில் (data->offset < 0)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		திரும்பு 0;
	}
	எனில் (!strcmp(option, "cache")) {
		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		எனில் (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			திரும்பு 0;
		}
		எனில் (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			திரும்பு retval;
		}
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	எனில் (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, வெற்று, 0);
		எனில் (errno || size == 0 || size > INT32_MAX)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		எனில் (data->cache_size == size)
			திரும்பு 0;
		எனில் (data->cache_size > size)
			திரும்பு shrink_cache(channel, data, size);
		திரும்பு grow_cache(channel, data, size);
	}
#endif
	திரும்பு EXT2_ET_INVALID_ARGUMENT;
}

#எனில் defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

நிலையான errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு unix_private_data *data;
	முழு		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (channel->flags & CHANNEL_FLAGS_NODISCARD)
		செல் unimplemented;

	எனில் (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(மொத்தம்) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#இல்லையெனில்
		செல் unimplemented;
#endif
	} இல்லையெனில் {
#எனில் defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(மொத்தம்) * channel->block_size);
#இல்லையெனில்
		செல் unimplemented;
#endif
	}
	எனில் (ret < 0) {
		எனில் (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			செல் unimplemented;
		}
		திரும்பு errno;
	}
	திரும்பு 0;
unimplemented:
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
நிலையான முழு __unix_zeroout(முழு fd, off_t offset, off_t len)
{
	முழு ret = -1;

#எனில் defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	எனில் (ret == 0)
		திரும்பு 0;
#endif
#எனில் defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	எனில் (ret == 0)
		திரும்பு 0;
#endif
	errno = EOPNOTSUPP;
	திரும்பு ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic தள்ளு
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
நிலையான errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு unix_private_data *data;
	முழு		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		கட்டமைப்பு stat statbuf;

		எனில் (மொத்தம் == 0)
			திரும்பு 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		எனில் (ret)
			செல் err;
		எனில் ((unsigned long long) statbuf.st_size <
			(block + மொத்தம்) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + மொத்தம்) * channel->block_size + data->offset);
			எனில் (ret)
				செல் err;
		}
	}

	எனில் (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		செல் unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(மொத்தம்) * channel->block_size);
err:
	எனில் (ret < 0) {
		எனில் (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			செல் unimplemented;
		}
		திரும்பு errno;
	}
	திரும்பு 0;
unimplemented:
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}
#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic எடு
#endif

நிலையான கட்டமைப்பு struct_io_manager struct_unix_manager = {
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

நிலையான கட்டமைப்பு struct_io_manager struct_unixfd_manager = {
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
