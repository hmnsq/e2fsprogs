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

#જો !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#ifdef __linux__
#અન્તર્ભાવ <sys/utsname.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#અન્તર્ભાવ <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#અન્તર્ભાવ <sys/mount.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_RESOURCE_H
#અન્તર્ભાવ <sys/resource.h>
#endif
#જો HAVE_LINUX_FALLOC_H
#અન્તર્ભાવ <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#અન્તર્ભાવ <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#અન્તર્ભાવ <sys/file.h>
#endif

#જો defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સ્થિર અંતઃરેખ પૂર્ણાંક find_last_bit_set(પૂર્ણાંક x)
{
	પૂર્ણાંક r = 32;

	જો (!x)
		પાછા 0;
	જો (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	જો (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	જો (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	જો (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	જો (!(x & 0x80000000u)) {
		r -= 1;
	}
	પાછા r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
સ્થિર અંતઃરેખ પૂર્ણાંક highbit32(uint32_t v)
{
	પાછા find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)

સંરચના unix_cache {
	અક્ષર			*buf;
	unsigned long long	block;
	પૂર્ણાંક			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

સંરચના unix_private_data {
	પૂર્ણાંક	magic;
	પૂર્ણાંક	dev;
	પૂર્ણાંક	flags;
	પૂર્ણાંક	align;
	પૂર્ણાંક	access_time;
	પૂર્ણાંક	unix_flock_flags;
	ext2_loff_t offset;
	સંરચના unix_cache *cache;
	unsigned પૂર્ણાંક cache_size;
	unsigned પૂર્ણાંક cache_hash_shift;
	શૂન્ય	*bounce;
	સંરચના struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

પ્રકારવ્યાખ્યા ગણના lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
સ્થિર અંતઃરેખ pthread_mutex_t *get_mutex(સંરચના unix_private_data *data,
					 kind_t kind)
{
	જો (data->flags & IO_FLAG_THREADS) {
		પસંદગી (kind) {
		સ્થિતિ CACHE_MTX:
			પાછા &data->cache_mutex;
		સ્થિતિ BOUNCE_MTX:
			પાછા &data->bounce_mutex;
		સ્થિતિ STATS_MTX:
			પાછા &data->stats_mutex;
		}
	}
	પાછા ખાલી;
}
#endif

સ્થિર અંતઃરેખ શૂન્ય mutex_lock(સંરચના unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	જો (mtx)
		pthread_mutex_lock(mtx);
#endif
}

સ્થિર અંતઃરેખ શૂન્ય mutex_unlock(સંરચના unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	જો (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

સ્થિર errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	સંરચના unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	પાછા retval;
}

/*
 * Here are the raw I/O functions
 */
સ્થિર errcode_t raw_read_blk(io_channel channel,
			      સંરચના unix_private_data *data,
			      unsigned long long block,
			      પૂર્ણાંક ગણતરી, શૂન્ય *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	પૂર્ણાંક		actual = 0;
	unsigned અક્ષર	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	પૂર્ણાંક		align_size, offset;

	size = (ગણતરી < 0) ? -ગણતરી : (ext2_loff_t) ગણતરી * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	જો (data->flags & IO_FLAG_FORCE_BOUNCE)
		જાઓ bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	જો ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		જો (actual == size)
			પાછા 0;
		actual = 0;
	}
#અથવાજો HAVE_PREAD
	/* Try an aligned pread */
	જો ((કદ(off_t) >= કદ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		જો (actual == size)
			પાછા 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	જો ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		જો (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			જાઓ error_unlock;
		}
		actual = પઠ(data->dev, buf, size);
		જો (actual != size) {
		short_read:
			જો (actual < 0) {
				retval = errno;
				actual = 0;
			} નહીંતર
				retval = EXT2_ET_SHORT_READ;
			જાઓ error_unlock;
		}
		જાઓ success_unlock;
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
	જો (channel->align == 0)
		channel->align = 1;
	જો ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	નહીંતર
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	જો (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		જાઓ error_unlock;
	}
	જ્યારે (size > 0) {
		actual = પઠ(data->dev, data->bounce, align_size);
		જો (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			જાઓ short_read;
		}
		જો ((actual + offset) > align_size)
			actual = align_size - offset;
		જો (actual > size)
			actual = size;
		memcpy(buf, (અક્ષર *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	પાછા 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	જો (actual >= 0 && actual < size)
		memset((અક્ષર *) buf+actual, 0, size-actual);
	જો (channel->read_error)
		retval = (channel->read_error)(channel, block, ગણતરી, buf,
					       size, actual, retval);
	પાછા retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

સ્થિર errcode_t raw_write_blk(io_channel channel,
			       સંરચના unix_private_data *data,
			       unsigned long long block,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *bufv,
			       પૂર્ણાંક flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	પૂર્ણાંક		actual = 0;
	errcode_t	retval;
	અચલ unsigned અક્ષર *buf = bufv;
	unsigned long long aligned_blk;
	પૂર્ણાંક		align_size, offset;

	જો (ગણતરી == 1)
		size = channel->block_size;
	નહીંતર {
		જો (ગણતરી < 0)
			size = -ગણતરી;
		નહીંતર
			size = (ext2_loff_t) ગણતરી * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	જો (data->flags & IO_FLAG_FORCE_BOUNCE)
		જાઓ bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	જો ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		જો (actual == size)
			પાછા 0;
	}
#અથવાજો HAVE_PWRITE
	/* Try an aligned pwrite */
	જો ((કદ(off_t) >= કદ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		જો (actual == size)
			પાછા 0;
	}
#endif /* HAVE_PWRITE */

	જો ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		જો (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		જો (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			જાઓ error_unlock;
		}
		actual = લિખ(data->dev, buf, size);
		જો (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		જો (actual < 0) {
			retval = errno;
			જાઓ error_out;
		}
		જો (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			જાઓ error_out;
		}
		પાછા 0;
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
	જો (channel->align == 0)
		channel->align = 1;
	જો ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	નહીંતર
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	જ્યારે (size > 0) {
		પૂર્ણાંક actual_w;

		જો (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		જો (size < align_size || offset) {
			જો (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				જાઓ error_unlock;
			}
			actual = પઠ(data->dev, data->bounce,
				      align_size);
			જો (actual != align_size) {
				જો (actual < 0) {
					retval = errno;
					જાઓ error_unlock;
				}
				memset((અક્ષર *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		જો ((actual + offset) > align_size)
			actual = align_size - offset;
		જો (actual > size)
			actual = size;
		memcpy(((અક્ષર *)data->bounce) + offset, buf, actual);
		જો (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			જાઓ error_unlock;
		}
		actual_w = લિખ(data->dev, data->bounce, align_size);
		જો (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		જો (actual_w < 0) {
			retval = errno;
			જાઓ error_out;
		}
		જો (actual_w != align_size)
			જાઓ short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	પાછા 0;

error_unlock:
	જો (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	જો (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, ગણતરી, buf,
						size, actual, retval);
	પાછા retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
સ્થિર errcode_t alloc_cache(io_channel channel,
			     સંરચના unix_private_data *data)
{
	errcode_t		retval;
	સંરચના unix_cache	*cache;
	unsigned પૂર્ણાંક		i;

	data->access_time = 0;
	માટે (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		જો (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		જો (retval)
			પાછા retval;
	}
	જો (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		જો (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	પાછા retval;
}

/* Free the cache buffers */
સ્થિર શૂન્ય free_cache(સંરચના unix_private_data *data)
{
	સંરચના unix_cache	*cache;
	unsigned પૂર્ણાંક		i;

	data->access_time = 0;
	માટે (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		જો (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	જો (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_અવિભાજ્ય	0x9e37fffffffc0001UL
#ifndef CACHE_લીટી_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_લીટી_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
સ્થિર unsigned પૂર્ણાંક
cache_hash(સંરચના unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	જો (data->cache_size <= DEFAULT_CACHE_SIZE)
		પાછા 0;

	tmp = hashval ^ (GOLDEN_RATIO_અવિભાજ્ય + hashval) / CACHE_લીટી_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_અવિભાજ્ય) >> data->cache_hash_shift);
	પાછા tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
સ્થિર સંરચના unix_cache *find_cached_block(સંરચના unix_private_data *data,
					    unsigned long long block,
					    સંરચના unix_cache **eldest)
{
	સંરચના unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned પૂર્ણાંક		hash = cache_hash(data, block);
	unsigned પૂર્ણાંક		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	માટે (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		જો (!cache->in_use) {
			જો (!unused_cache)
				unused_cache = cache;
			ચાલુ;
		}
		જો (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			પાછા cache;
		}
		જો (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	માટે (i = 0, cache = data->cache; i < hash; i++, cache++) {
		જો (!cache->in_use) {
			જો (!unused_cache)
				unused_cache = cache;
			ચાલુ;
		}
		જો (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			પાછા cache;
		}
		જો (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	જો (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	પાછા 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
સ્થિર errcode_t reuse_cache(io_channel channel,
		સંરચના unix_private_data *data, સંરચના unix_cache *cache,
		unsigned long long block)
{
	જો (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		જો (retval) {
			cache->write_err = 1;
			પાછા retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	પાછા 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
સ્થિર errcode_t flush_cached_blocks(io_channel channel,
				     સંરચના unix_private_data *data,
				     પૂર્ણાંક flags)
{
	સંરચના unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned પૂર્ણાંક		i;
	પૂર્ણાંક			errors_found = 0;

	જો ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	માટે (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		જો (!cache->in_use)
			ચાલુ;
		જો (cache->dirty) {
			પૂર્ણાંક raw_flags = RAW_WRITE_NO_HANDLER;

			જો (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			જો (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} નહીંતર {
				cache->dirty = 0;
				cache->write_err = 0;
				જો (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} નહીંતર જો (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	જો ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	જ્યારે (errors_found) {
		જો ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		માટે (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			જો (!cache->in_use || !cache->write_err)
				ચાલુ;
			errors_found = 1;
			જો (cache->write_err && channel->write_error) {
				અક્ષર *err_buf = ખાલી;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				જો (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ખાલી;
				નહીંતર
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				જો (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				જાઓ retry;
			} નહીંતર
				cache->write_err = 0;
		}
		જો ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	પાછા retval2;
}

/* Shrink the cache buffers */
સ્થિર errcode_t shrink_cache(io_channel channel,
			      સંરચના unix_private_data *data,
			      unsigned પૂર્ણાંક new_size)
{
	સંરચના unix_cache	*cache, *new_cache;
	unsigned પૂર્ણાંક		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	જો (retval)
		જાઓ unlock;

	માટે (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		જો (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * કદ(સંરચના unix_cache));
	જો (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		જાઓ unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	પાછા retval;
}

/* Grow the cache buffers */
સ્થિર errcode_t grow_cache(io_channel channel,
			    સંરચના unix_private_data *data,
			    unsigned પૂર્ણાંક new_size)
{
	સંરચના unix_cache	*cache, *new_cache;
	unsigned પૂર્ણાંક		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	જો (retval)
		જાઓ unlock;

	new_cache = realloc(data->cache, new_size * કદ(સંરચના unix_cache));
	જો (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		જાઓ unlock;
	}
	data->cache = new_cache;

	માટે (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		જો (retval) {
			જ્યારે (i > data->cache_size) {
				cache--; i--;
				જો (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			જાઓ unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	પાછા retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

પૂર્ણાંક ext2fs_open_file(અચલ અક્ષર *pathname, પૂર્ણાંક flags, mode_t mode)
{
	જો (mode)
#જો defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		પાછા open64(pathname, flags, mode);
	નહીંતર
		પાછા open64(pathname, flags);
#નહીંતર
		પાછા open(pathname, flags, mode);
	નહીંતર
		પાછા open(pathname, flags);
#endif
}

પૂર્ણાંક ext2fs_stat(અચલ અક્ષર *path, ext2fs_struct_stat *buf)
{
#જો defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	પાછા stat64(path, buf);
#નહીંતર
	પાછા stat(path, buf);
#endif
}

પૂર્ણાંક ext2fs_fstat(પૂર્ણાંક fd, ext2fs_struct_stat *buf)
{
#જો defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	પાછા fstat64(fd, buf);
#નહીંતર
	પાછા fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
સ્થિર errcode_t unix_funlock(io_channel channel)
{
	સંરચના unix_private_data *data;
	પૂર્ણાંક ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		જો (ret)
			પાછા errno;

		data->unix_flock_flags = 0;
	}

	પાછા 0;
}

સ્થિર errcode_t unix_flock(io_channel channel, unsigned પૂર્ણાંક flock_flags)
{
	સંરચના unix_private_data *data;
	પૂર્ણાંક unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	જો (ret)
		પાછા ret;

	જો (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	જો (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	જો (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	જો (!unix_flock_flags)
		પાછા 0;

	ret = flock(data->dev, unix_flock_flags);
	જો (ret < 0)
		પાછા errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	પાછા 0;
}
#નહીંતર
#define unix_flock		ખાલી

સ્થિર errcode_t unix_funlock(io_channel channel)
{
	પાછા 0;
}
#endif /* HAVE_SYS_FILE_H */

સ્થિર errcode_t unix_open_channel(અચલ અક્ષર *name, પૂર્ણાંક fd,
				   પૂર્ણાંક flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ખાલી;
	સંરચના unix_private_data *data = ખાલી;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	સંરચના		utsname ut;
#endif

	જો (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(શૂન્ય) fsync(fd);
#endif

	retval = ext2fs_get_mem(કદ(સંરચના struct_io_channel), &io);
	જો (retval)
		જાઓ cleanup;
	memset(io, 0, કદ(સંરચના struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(કદ(સંરચના unix_private_data), &data);
	જો (retval)
		જાઓ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	જો (retval)
		જાઓ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	જો (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, કદ(સંરચના unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.અંક_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, કદ(સંરચના unix_cache));
	જો (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		જાઓ cleanup;
	}

#જો defined(O_DIRECT)
	જો (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#અથવાજો defined(F_NOCACHE)
	જો (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	જો (ext2fs_fstat(data->dev, &st) == 0) {
		જો (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			પૂર્ણાંક zeroes = 0;

			જો (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} નહીંતર {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#જો defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	જો (!io->align)
		io->align = 512;
#endif

#જો defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	જો (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		પૂર્ણાંક dio_align = ext2fs_get_dio_alignment(fd);

		જો (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	જો ((retval = alloc_cache(io, data)))
		જાઓ cleanup;

#ifdef BLKROGET
	જો (flags & IO_FLAG_RW) {
		પૂર્ણાંક error;
		પૂર્ણાંક readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		જો (!error && readonly) {
			retval = EPERM;
			જાઓ cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#જો (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#નહીંતર
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	જો ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		સંરચના rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		જો (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	જો (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, ખાલી);
		જો (retval)
			જાઓ cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, ખાલી);
		જો (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			જાઓ cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, ખાલી);
		જો (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			જાઓ cleanup;
		}
	}
#endif
	*channel = io;
	પાછા 0;

cleanup:
	જો (data) {
		unix_funlock(io);
		જો (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		જો (data->cache) {
			free_cache(data);
			મુક્ત(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	જો (io) {
		જો (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	પાછા retval;
}

સ્થિર errcode_t unixfd_open(અચલ અક્ષર *str_fd, પૂર્ણાંક flags,
			     io_channel *channel)
{
	પૂર્ણાંક fd;
	પૂર્ણાંક fd_flags;

	fd = atoi(str_fd);
#જો defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	જો (fd_flags == -1)
		પાછા EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	જો (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#જો defined(O_DIRECT)
	જો (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	પાછા unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

સ્થિર errcode_t unix_open(અચલ અક્ષર *name, પૂર્ણાંક flags,
			   io_channel *channel)
{
	પૂર્ણાંક fd = -1;
	પૂર્ણાંક open_flags;

	જો (name == 0)
		પાછા EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	જો (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#જો defined(O_DIRECT)
	જો (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	જો (fd < 0)
		પાછા errno;
#જો defined(F_NOCACHE) && !defined(IO_DIRECT)
	જો (flags & IO_FLAG_DIRECT_IO) {
		જો (fcntl(fd, F_NOCACHE, 1) < 0)
			પાછા errno;
	}
#endif
	પાછા unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

સ્થિર errcode_t unix_close(io_channel channel)
{
	સંરચના unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (--channel->refcount > 0)
		પાછા 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	જો (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	મુક્ત(data->cache);
#ifdef HAVE_PTHREAD
	જો (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	જો (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	પાછા retval;
}

સ્થિર errcode_t unix_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	સંરચના unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		જો ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			પાછા retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	પાછા retval;
}

સ્થિર errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	સંરચના unix_private_data *data;
	સંરચના unix_cache *cache;
	errcode_t	retval;
	અક્ષર		*cp;
	પૂર્ણાંક		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	પાછા raw_read_blk(channel, data, block, ગણતરી, buf);
#નહીંતર
	જો (data->flags & IO_FLAG_NOCACHE)
		પાછા raw_read_blk(channel, data, block, ગણતરી, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	જો (ગણતરી < 0 || ગણતરી > WRITE_DIRECT_SIZE) {
		જો ((retval = flush_cached_blocks(channel, data, 0)))
			પાછા retval;
		પાછા raw_read_blk(channel, data, block, ગણતરી, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	જ્યારે (ગણતરી > 0) {
		/* If it's in the cache, use it! */
		જો ((cache = find_cached_block(data, block, ખાલી))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			ગણતરી--;
			block++;
			cp += channel->block_size;
			ચાલુ;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		માટે (i=1; i < ગણતરી; i++)
			જો (find_cached_block(data, block+i, ખાલી))
				તોડો;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		જો ((retval = raw_read_blk(channel, data, block, i, cp)))
			પાછા retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		માટે (j=0; j < i; j++) {
			જો (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				જો (retval)
					જાઓ call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			ગણતરી--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	પાછા 0;

call_write_handler:
	જો (cache->write_err && channel->write_error) {
		અક્ષર *err_buf = ખાલી;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		જો (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ખાલી;
		નહીંતર
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		જો (err_buf)
			ext2fs_free_mem(&err_buf);
	} નહીંતર
		mutex_unlock(data, CACHE_MTX);
	પાછા retval;
#endif /* NO_IO_CACHE */
}

સ્થિર errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પાછા unix_read_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના unix_private_data *data;
	સંરચના unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	અચલ અક્ષર	*cp;
	પૂર્ણાંક		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	પાછા raw_write_blk(channel, data, block, ગણતરી, buf, 0);
#નહીંતર
	જો (data->flags & IO_FLAG_NOCACHE)
		પાછા raw_write_blk(channel, data, block, ગણતરી, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	જો (ગણતરી < 0 || ગણતરી > WRITE_DIRECT_SIZE) {
		જો ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			પાછા retval;
		પાછા raw_write_blk(channel, data, block, ગણતરી, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	જો (writethrough)
		retval = raw_write_blk(channel, data, block, ગણતરી, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	જ્યારે (ગણતરી > 0) {
		cache = find_cached_block(data, block, &reuse);
		જો (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			જો (err)
				જાઓ call_write_handler;
		}
		જો (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		ગણતરી--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	પાછા retval;

call_write_handler:
	જો (cache->write_err && channel->write_error) {
		અક્ષર *err_buf = ખાલી;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		જો (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ખાલી;
		નહીંતર
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		જો (err_buf)
			ext2fs_free_mem(&err_buf);
	} નહીંતર
		mutex_unlock(data, CACHE_MTX);
	પાછા retval;
#endif /* NO_IO_CACHE */
}

સ્થિર errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ગણતરી)
{
#ifdef POSIX_FADV_WILLNEED
	સંરચના unix_private_data *data;

	data = (સંરચના unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	પાછા posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)ગણતરી * channel->block_size,
			     POSIX_FADV_WILLNEED);
#નહીંતર
	પાછા EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

સ્થિર errcode_t unix_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પાછા unix_write_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 પૂર્ણાંક size, અચલ શૂન્ય *buf)
{
	સંરચના unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		પાછા EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	જો ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		પાછા retval;
#endif

	જો (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		પાછા errno;

	actual = લિખ(data->dev, buf, size);
	જો (actual < 0)
		પાછા errno;
	જો (actual != size)
		પાછા EXT2_ET_SHORT_WRITE;

	પાછા 0;
}

/*
 * Flush data buffers to disk.
 */
સ્થિર errcode_t unix_flush(io_channel channel)
{
	સંરચના unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	જો (!retval && fsync(data->dev) != 0)
		પાછા errno;
#endif
	પાછા retval;
}

સ્થિર errcode_t unix_set_option(io_channel channel, અચલ અક્ષર *option,
				 અચલ અક્ષર *arg)
{
	સંરચના unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	અક્ષર *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (!strcmp(option, "offset")) {
		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		જો (*end)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		જો (data->offset < 0)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		પાછા 0;
	}
	જો (!strcmp(option, "cache")) {
		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		જો (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			પાછા 0;
		}
		જો (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			પાછા retval;
		}
		પાછા EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	જો (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, ખાલી, 0);
		જો (errno || size == 0 || size > INT32_MAX)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		જો (data->cache_size == size)
			પાછા 0;
		જો (data->cache_size > size)
			પાછા shrink_cache(channel, data, size);
		પાછા grow_cache(channel, data, size);
	}
#endif
	પાછા EXT2_ET_INVALID_ARGUMENT;
}

#જો defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

સ્થિર errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના unix_private_data *data;
	પૂર્ણાંક		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (channel->flags & CHANNEL_FLAGS_NODISCARD)
		જાઓ unimplemented;

	જો (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(ગણતરી) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#નહીંતર
		જાઓ unimplemented;
#endif
	} નહીંતર {
#જો defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(ગણતરી) * channel->block_size);
#નહીંતર
		જાઓ unimplemented;
#endif
	}
	જો (ret < 0) {
		જો (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			જાઓ unimplemented;
		}
		પાછા errno;
	}
	પાછા 0;
unimplemented:
	પાછા EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
સ્થિર પૂર્ણાંક __unix_zeroout(પૂર્ણાંક fd, off_t offset, off_t len)
{
	પૂર્ણાંક ret = -1;

#જો defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	જો (ret == 0)
		પાછા 0;
#endif
#જો defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	જો (ret == 0)
		પાછા 0;
#endif
	errno = EOPNOTSUPP;
	પાછા ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ધકેલો
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
સ્થિર errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના unix_private_data *data;
	પૂર્ણાંક		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		સંરચના stat statbuf;

		જો (ગણતરી == 0)
			પાછા 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		જો (ret)
			જાઓ err;
		જો ((unsigned long long) statbuf.st_size <
			(block + ગણતરી) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + ગણતરી) * channel->block_size + data->offset);
			જો (ret)
				જાઓ err;
		}
	}

	જો (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		જાઓ unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(ગણતરી) * channel->block_size);
err:
	જો (ret < 0) {
		જો (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			જાઓ unimplemented;
		}
		પાછા errno;
	}
	પાછા 0;
unimplemented:
	પાછા EXT2_ET_UNIMPLEMENTED;
}
#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic કાઢો
#endif

સ્થિર સંરચના struct_io_manager struct_unix_manager = {
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

સ્થિર સંરચના struct_io_manager struct_unixfd_manager = {
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
