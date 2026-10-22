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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#ifdef __linux__
#অন্তর্ভাৱ <sys/utsname.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#অন্তর্ভাৱ <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#অন্তর্ভাৱ <sys/mount.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_RESOURCE_H
#অন্তর্ভাৱ <sys/resource.h>
#endif
#যদি HAVE_LINUX_FALLOC_H
#অন্তর্ভাৱ <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#অন্তর্ভাৱ <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#অন্তর্ভাৱ <sys/file.h>
#endif

#যদি defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

স্থির অন্তঃস্থ পূর্ণাঙ্ক find_last_bit_set(পূর্ণাঙ্ক x)
{
	পূর্ণাঙ্ক r = 32;

	যদি (!x)
		ঘূৰাই 0;
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
	ঘূৰাই r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
স্থির অন্তঃস্থ পূর্ণাঙ্ক highbit32(uint32_t v)
{
	ঘূৰাই find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) ঘূৰাই (code)

সংরচনা unix_cache {
	ৱর্ণ			*buf;
	unsigned long long	block;
	পূর্ণাঙ্ক			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

সংরচনা unix_private_data {
	পূর্ণাঙ্ক	magic;
	পূর্ণাঙ্ক	dev;
	পূর্ণাঙ্ক	flags;
	পূর্ণাঙ্ক	align;
	পূর্ণাঙ্ক	access_time;
	পূর্ণাঙ্ক	unix_flock_flags;
	ext2_loff_t offset;
	সংরচনা unix_cache *cache;
	unsigned পূর্ণাঙ্ক cache_size;
	unsigned পূর্ণাঙ্ক cache_hash_shift;
	শূন্য	*bounce;
	সংরচনা struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

প্রকারনির্দেশ গণনা lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
স্থির অন্তঃস্থ pthread_mutex_t *get_mutex(সংরচনা unix_private_data *data,
					 kind_t kind)
{
	যদি (data->flags & IO_FLAG_THREADS) {
		ৱিকল্প (kind) {
		পক্ষ CACHE_MTX:
			ঘূৰাই &data->cache_mutex;
		পক্ষ BOUNCE_MTX:
			ঘূৰাই &data->bounce_mutex;
		পক্ষ STATS_MTX:
			ঘূৰাই &data->stats_mutex;
		}
	}
	ঘূৰাই রিক্ত;
}
#endif

স্থির অন্তঃস্থ শূন্য mutex_lock(সংরচনা unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	যদি (mtx)
		pthread_mutex_lock(mtx);
#endif
}

স্থির অন্তঃস্থ শূন্য mutex_unlock(সংরচনা unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	যদি (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

স্থির errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	সংরচনা unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	ঘূৰাই retval;
}

/*
 * Here are the raw I/O functions
 */
স্থির errcode_t raw_read_blk(io_channel channel,
			      সংরচনা unix_private_data *data,
			      unsigned long long block,
			      পূর্ণাঙ্ক গুনতি, শূন্য *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	পূর্ণাঙ্ক		actual = 0;
	unsigned ৱর্ণ	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	পূর্ণাঙ্ক		align_size, offset;

	size = (গুনতি < 0) ? -গুনতি : (ext2_loff_t) গুনতি * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	যদি (data->flags & IO_FLAG_FORCE_BOUNCE)
		গচ্ছ bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		যদি (actual == size)
			ঘূৰাই 0;
		actual = 0;
	}
#নোচেদ্যদি HAVE_PREAD
	/* Try an aligned pread */
	যদি ((পরিমাণ(off_t) >= পরিমাণ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		যদি (actual == size)
			ঘূৰাই 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		যদি (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			গচ্ছ error_unlock;
		}
		actual = পঠ(data->dev, buf, size);
		যদি (actual != size) {
		short_read:
			যদি (actual < 0) {
				retval = errno;
				actual = 0;
			} অন্যথা
				retval = EXT2_ET_SHORT_READ;
			গচ্ছ error_unlock;
		}
		গচ্ছ success_unlock;
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
	যদি (channel->align == 0)
		channel->align = 1;
	যদি ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	অন্যথা
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	যদি (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		গচ্ছ error_unlock;
	}
	যেতিয়ালৈ (size > 0) {
		actual = পঠ(data->dev, data->bounce, align_size);
		যদি (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			গচ্ছ short_read;
		}
		যদি ((actual + offset) > align_size)
			actual = align_size - offset;
		যদি (actual > size)
			actual = size;
		memcpy(buf, (ৱর্ণ *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	ঘূৰাই 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	যদি (actual >= 0 && actual < size)
		memset((ৱর্ণ *) buf+actual, 0, size-actual);
	যদি (channel->read_error)
		retval = (channel->read_error)(channel, block, গুনতি, buf,
					       size, actual, retval);
	ঘূৰাই retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

স্থির errcode_t raw_write_blk(io_channel channel,
			       সংরচনা unix_private_data *data,
			       unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *bufv,
			       পূর্ণাঙ্ক flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	পূর্ণাঙ্ক		actual = 0;
	errcode_t	retval;
	ধ্রুৱ unsigned ৱর্ণ *buf = bufv;
	unsigned long long aligned_blk;
	পূর্ণাঙ্ক		align_size, offset;

	যদি (গুনতি == 1)
		size = channel->block_size;
	অন্যথা {
		যদি (গুনতি < 0)
			size = -গুনতি;
		অন্যথা
			size = (ext2_loff_t) গুনতি * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	যদি (data->flags & IO_FLAG_FORCE_BOUNCE)
		গচ্ছ bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		যদি (actual == size)
			ঘূৰাই 0;
	}
#নোচেদ্যদি HAVE_PWRITE
	/* Try an aligned pwrite */
	যদি ((পরিমাণ(off_t) >= পরিমাণ(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		যদি (actual == size)
			ঘূৰাই 0;
	}
#endif /* HAVE_PWRITE */

	যদি ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		যদি (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			গচ্ছ error_unlock;
		}
		actual = লিখ(data->dev, buf, size);
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		যদি (actual < 0) {
			retval = errno;
			গচ্ছ error_out;
		}
		যদি (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			গচ্ছ error_out;
		}
		ঘূৰাই 0;
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
	যদি (channel->align == 0)
		channel->align = 1;
	যদি ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	অন্যথা
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	যেতিয়ালৈ (size > 0) {
		পূর্ণাঙ্ক actual_w;

		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		যদি (size < align_size || offset) {
			যদি (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				গচ্ছ error_unlock;
			}
			actual = পঠ(data->dev, data->bounce,
				      align_size);
			যদি (actual != align_size) {
				যদি (actual < 0) {
					retval = errno;
					গচ্ছ error_unlock;
				}
				memset((ৱর্ণ *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		যদি ((actual + offset) > align_size)
			actual = align_size - offset;
		যদি (actual > size)
			actual = size;
		memcpy(((ৱর্ণ *)data->bounce) + offset, buf, actual);
		যদি (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			গচ্ছ error_unlock;
		}
		actual_w = লিখ(data->dev, data->bounce, align_size);
		যদি (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		যদি (actual_w < 0) {
			retval = errno;
			গচ্ছ error_out;
		}
		যদি (actual_w != align_size)
			গচ্ছ short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	ঘূৰাই 0;

error_unlock:
	যদি (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	যদি (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, গুনতি, buf,
						size, actual, retval);
	ঘূৰাই retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
স্থির errcode_t alloc_cache(io_channel channel,
			     সংরচনা unix_private_data *data)
{
	errcode_t		retval;
	সংরচনা unix_cache	*cache;
	unsigned পূর্ণাঙ্ক		i;

	data->access_time = 0;
	বাবে (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval)
			ঘূৰাই retval;
	}
	যদি (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		যদি (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ঘূৰাই retval;
}

/* Free the cache buffers */
স্থির শূন্য free_cache(সংরচনা unix_private_data *data)
{
	সংরচনা unix_cache	*cache;
	unsigned পূর্ণাঙ্ক		i;

	data->access_time = 0;
	বাবে (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	যদি (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_মৌলিক	0x9e37fffffffc0001UL
#ifndef CACHE_সাৰি_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_সাৰি_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
স্থির unsigned পূর্ণাঙ্ক
cache_hash(সংরচনা unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	যদি (data->cache_size <= DEFAULT_CACHE_SIZE)
		ঘূৰাই 0;

	tmp = hashval ^ (GOLDEN_RATIO_মৌলিক + hashval) / CACHE_সাৰি_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_মৌলিক) >> data->cache_hash_shift);
	ঘূৰাই tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
স্থির সংরচনা unix_cache *find_cached_block(সংরচনা unix_private_data *data,
					    unsigned long long block,
					    সংরচনা unix_cache **eldest)
{
	সংরচনা unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned পূর্ণাঙ্ক		hash = cache_hash(data, block);
	unsigned পূর্ণাঙ্ক		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	বাবে (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			অনুৱর্ত;
		}
		যদি (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ঘূৰাই cache;
		}
		যদি (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	বাবে (i = 0, cache = data->cache; i < hash; i++, cache++) {
		যদি (!cache->in_use) {
			যদি (!unused_cache)
				unused_cache = cache;
			অনুৱর্ত;
		}
		যদি (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ঘূৰাই cache;
		}
		যদি (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	যদি (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	ঘূৰাই 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
স্থির errcode_t reuse_cache(io_channel channel,
		সংরচনা unix_private_data *data, সংরচনা unix_cache *cache,
		unsigned long long block)
{
	যদি (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		যদি (retval) {
			cache->write_err = 1;
			ঘূৰাই retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	ঘূৰাই 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
স্থির errcode_t flush_cached_blocks(io_channel channel,
				     সংরচনা unix_private_data *data,
				     পূর্ণাঙ্ক flags)
{
	সংরচনা unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned পূর্ণাঙ্ক		i;
	পূর্ণাঙ্ক			errors_found = 0;

	যদি ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	বাবে (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		যদি (!cache->in_use)
			অনুৱর্ত;
		যদি (cache->dirty) {
			পূর্ণাঙ্ক raw_flags = RAW_WRITE_NO_HANDLER;

			যদি (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
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
		mutex_unlock(data, CACHE_MTX);
retry:
	যেতিয়ালৈ (errors_found) {
		যদি ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		বাবে (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			যদি (!cache->in_use || !cache->write_err)
				অনুৱর্ত;
			errors_found = 1;
			যদি (cache->write_err && channel->write_error) {
				ৱর্ণ *err_buf = রিক্ত;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				যদি (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = রিক্ত;
				অন্যথা
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				যদি (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				গচ্ছ retry;
			} অন্যথা
				cache->write_err = 0;
		}
		যদি ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	ঘূৰাই retval2;
}

/* Shrink the cache buffers */
স্থির errcode_t shrink_cache(io_channel channel,
			      সংরচনা unix_private_data *data,
			      unsigned পূর্ণাঙ্ক new_size)
{
	সংরচনা unix_cache	*cache, *new_cache;
	unsigned পূর্ণাঙ্ক		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	যদি (retval)
		গচ্ছ unlock;

	বাবে (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		যদি (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * পরিমাণ(সংরচনা unix_cache));
	যদি (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		গচ্ছ unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ঘূৰাই retval;
}

/* Grow the cache buffers */
স্থির errcode_t grow_cache(io_channel channel,
			    সংরচনা unix_private_data *data,
			    unsigned পূর্ণাঙ্ক new_size)
{
	সংরচনা unix_cache	*cache, *new_cache;
	unsigned পূর্ণাঙ্ক		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	যদি (retval)
		গচ্ছ unlock;

	new_cache = realloc(data->cache, new_size * পরিমাণ(সংরচনা unix_cache));
	যদি (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		গচ্ছ unlock;
	}
	data->cache = new_cache;

	বাবে (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		যদি (retval) {
			যেতিয়ালৈ (i > data->cache_size) {
				cache--; i--;
				যদি (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			গচ্ছ unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ঘূৰাই retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

পূর্ণাঙ্ক ext2fs_open_file(ধ্রুৱ ৱর্ণ *pathname, পূর্ণাঙ্ক flags, mode_t mode)
{
	যদি (mode)
#যদি defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ঘূৰাই open64(pathname, flags, mode);
	অন্যথা
		ঘূৰাই open64(pathname, flags);
#অন্যথা
		ঘূৰাই open(pathname, flags, mode);
	অন্যথা
		ঘূৰাই open(pathname, flags);
#endif
}

পূর্ণাঙ্ক ext2fs_stat(ধ্রুৱ ৱর্ণ *path, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ঘূৰাই stat64(path, buf);
#অন্যথা
	ঘূৰাই stat(path, buf);
#endif
}

পূর্ণাঙ্ক ext2fs_fstat(পূর্ণাঙ্ক fd, ext2fs_struct_stat *buf)
{
#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ঘূৰাই fstat64(fd, buf);
#অন্যথা
	ঘূৰাই fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
স্থির errcode_t unix_funlock(io_channel channel)
{
	সংরচনা unix_private_data *data;
	পূর্ণাঙ্ক ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		যদি (ret)
			ঘূৰাই errno;

		data->unix_flock_flags = 0;
	}

	ঘূৰাই 0;
}

স্থির errcode_t unix_flock(io_channel channel, unsigned পূর্ণাঙ্ক flock_flags)
{
	সংরচনা unix_private_data *data;
	পূর্ণাঙ্ক unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	যদি (ret)
		ঘূৰাই ret;

	যদি (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	যদি (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	যদি (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	যদি (!unix_flock_flags)
		ঘূৰাই 0;

	ret = flock(data->dev, unix_flock_flags);
	যদি (ret < 0)
		ঘূৰাই errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ঘূৰাই 0;
}
#অন্যথা
#define unix_flock		রিক্ত

স্থির errcode_t unix_funlock(io_channel channel)
{
	ঘূৰাই 0;
}
#endif /* HAVE_SYS_FILE_H */

স্থির errcode_t unix_open_channel(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক fd,
				   পূর্ণাঙ্ক flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = রিক্ত;
	সংরচনা unix_private_data *data = রিক্ত;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	সংরচনা		utsname ut;
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

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা struct_io_channel), &io);
	যদি (retval)
		গচ্ছ cleanup;
	memset(io, 0, পরিমাণ(সংরচনা struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা unix_private_data), &data);
	যদি (retval)
		গচ্ছ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		গচ্ছ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	যদি (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, পরিমাণ(সংরচনা unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.অঙ্ক_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, পরিমাণ(সংরচনা unix_cache));
	যদি (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		গচ্ছ cleanup;
	}

#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#নোচেদ্যদি defined(F_NOCACHE)
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
	যদি (ext2fs_fstat(data->dev, &st) == 0) {
		যদি (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			পূর্ণাঙ্ক zeroes = 0;

			যদি (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
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
		পূর্ণাঙ্ক dio_align = ext2fs_get_dio_alignment(fd);

		যদি (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	যদি ((retval = alloc_cache(io, data)))
		গচ্ছ cleanup;

#ifdef BLKROGET
	যদি (flags & IO_FLAG_RW) {
		পূর্ণাঙ্ক error;
		পূর্ণাঙ্ক readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		যদি (!error && readonly) {
			retval = EPERM;
			গচ্ছ cleanup;
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
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		সংরচনা rlimit	rlim;

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
		retval = pthread_mutex_init(&data->cache_mutex, রিক্ত);
		যদি (retval)
			গচ্ছ cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, রিক্ত);
		যদি (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			গচ্ছ cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, রিক্ত);
		যদি (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			গচ্ছ cleanup;
		}
	}
#endif
	*channel = io;
	ঘূৰাই 0;

cleanup:
	যদি (data) {
		unix_funlock(io);
		যদি (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		যদি (data->cache) {
			free_cache(data);
			মুক্ত(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	যদি (io) {
		যদি (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ঘূৰাই retval;
}

স্থির errcode_t unixfd_open(ধ্রুৱ ৱর্ণ *str_fd, পূর্ণাঙ্ক flags,
			     io_channel *channel)
{
	পূর্ণাঙ্ক fd;
	পূর্ণাঙ্ক fd_flags;

	fd = atoi(str_fd);
#যদি defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	যদি (fd_flags == -1)
		ঘূৰাই EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	যদি (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#যদি defined(O_DIRECT)
	যদি (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	ঘূৰাই unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

স্থির errcode_t unix_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags,
			   io_channel *channel)
{
	পূর্ণাঙ্ক fd = -1;
	পূর্ণাঙ্ক open_flags;

	যদি (name == 0)
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	যদি (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#যদি defined(O_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	যদি (fd < 0)
		ঘূৰাই errno;
#যদি defined(F_NOCACHE) && !defined(IO_DIRECT)
	যদি (flags & IO_FLAG_DIRECT_IO) {
		যদি (fcntl(fd, F_NOCACHE, 1) < 0)
			ঘূৰাই errno;
	}
#endif
	ঘূৰাই unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

স্থির errcode_t unix_close(io_channel channel)
{
	সংরচনা unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ঘূৰাই 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	যদি (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	মুক্ত(data->cache);
#ifdef HAVE_PTHREAD
	যদি (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ঘূৰাই retval;
}

স্থির errcode_t unix_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	সংরচনা unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		যদি ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			ঘূৰাই retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	ঘূৰাই retval;
}

স্থির errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	সংরচনা unix_private_data *data;
	সংরচনা unix_cache *cache;
	errcode_t	retval;
	ৱর্ণ		*cp;
	পূর্ণাঙ্ক		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ঘূৰাই raw_read_blk(channel, data, block, গুনতি, buf);
#অন্যথা
	যদি (data->flags & IO_FLAG_NOCACHE)
		ঘূৰাই raw_read_blk(channel, data, block, গুনতি, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_SIZE) {
		যদি ((retval = flush_cached_blocks(channel, data, 0)))
			ঘূৰাই retval;
		ঘূৰাই raw_read_blk(channel, data, block, গুনতি, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	যেতিয়ালৈ (গুনতি > 0) {
		/* If it's in the cache, use it! */
		যদি ((cache = find_cached_block(data, block, রিক্ত))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			গুনতি--;
			block++;
			cp += channel->block_size;
			অনুৱর্ত;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		বাবে (i=1; i < গুনতি; i++)
			যদি (find_cached_block(data, block+i, রিক্ত))
				ভঙ্গ;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		যদি ((retval = raw_read_blk(channel, data, block, i, cp)))
			ঘূৰাই retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		বাবে (j=0; j < i; j++) {
			যদি (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				যদি (retval)
					গচ্ছ call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			গুনতি--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	ঘূৰাই 0;

call_write_handler:
	যদি (cache->write_err && channel->write_error) {
		ৱর্ণ *err_buf = রিক্ত;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		যদি (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = রিক্ত;
		অন্যথা
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		যদি (err_buf)
			ext2fs_free_mem(&err_buf);
	} অন্যথা
		mutex_unlock(data, CACHE_MTX);
	ঘূৰাই retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	ঘূৰাই unix_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	সংরচনা unix_private_data *data;
	সংরচনা unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ধ্রুৱ ৱর্ণ	*cp;
	পূর্ণাঙ্ক		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ঘূৰাই raw_write_blk(channel, data, block, গুনতি, buf, 0);
#অন্যথা
	যদি (data->flags & IO_FLAG_NOCACHE)
		ঘূৰাই raw_write_blk(channel, data, block, গুনতি, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	যদি (গুনতি < 0 || গুনতি > WRITE_DIRECT_SIZE) {
		যদি ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			ঘূৰাই retval;
		ঘূৰাই raw_write_blk(channel, data, block, গুনতি, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	যদি (writethrough)
		retval = raw_write_blk(channel, data, block, গুনতি, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	যেতিয়ালৈ (গুনতি > 0) {
		cache = find_cached_block(data, block, &reuse);
		যদি (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			যদি (err)
				গচ্ছ call_write_handler;
		}
		যদি (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		গুনতি--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	ঘূৰাই retval;

call_write_handler:
	যদি (cache->write_err && channel->write_error) {
		ৱর্ণ *err_buf = রিক্ত;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		যদি (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = রিক্ত;
		অন্যথা
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		যদি (err_buf)
			ext2fs_free_mem(&err_buf);
	} অন্যথা
		mutex_unlock(data, CACHE_MTX);
	ঘূৰাই retval;
#endif /* NO_IO_CACHE */
}

স্থির errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
#ifdef POSIX_FADV_WILLNEED
	সংরচনা unix_private_data *data;

	data = (সংরচনা unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ঘূৰাই posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)গুনতি * channel->block_size,
			     POSIX_FADV_WILLNEED);
#অন্যথা
	ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

স্থির errcode_t unix_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	ঘূৰাই unix_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণাঙ্ক size, ধ্রুৱ শূন্য *buf)
{
	সংরচনা unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		ঘূৰাই EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	যদি ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		ঘূৰাই retval;
#endif

	যদি (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		ঘূৰাই errno;

	actual = লিখ(data->dev, buf, size);
	যদি (actual < 0)
		ঘূৰাই errno;
	যদি (actual != size)
		ঘূৰাই EXT2_ET_SHORT_WRITE;

	ঘূৰাই 0;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t unix_flush(io_channel channel)
{
	সংরচনা unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	যদি (!retval && fsync(data->dev) != 0)
		ঘূৰাই errno;
#endif
	ঘূৰাই retval;
}

স্থির errcode_t unix_set_option(io_channel channel, ধ্রুৱ ৱর্ণ *option,
				 ধ্রুৱ ৱর্ণ *arg)
{
	সংরচনা unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	ৱর্ণ *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!strcmp(option, "offset")) {
		যদি (!arg)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		যদি (*end)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		যদি (data->offset < 0)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
		ঘূৰাই 0;
	}
	যদি (!strcmp(option, "cache")) {
		যদি (!arg)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
		যদি (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			ঘূৰাই 0;
		}
		যদি (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			ঘূৰাই retval;
		}
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	যদি (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		যদি (!arg)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, রিক্ত, 0);
		যদি (errno || size == 0 || size > INT32_MAX)
			ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

		যদি (data->cache_size == size)
			ঘূৰাই 0;
		যদি (data->cache_size > size)
			ঘূৰাই shrink_cache(channel, data, size);
		ঘূৰাই grow_cache(channel, data, size);
	}
#endif
	ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
}

#যদি defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

স্থির errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা unix_private_data *data;
	পূর্ণাঙ্ক		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (channel->flags & CHANNEL_FLAGS_NODISCARD)
		গচ্ছ unimplemented;

	যদি (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(গুনতি) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#অন্যথা
		গচ্ছ unimplemented;
#endif
	} অন্যথা {
#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(গুনতি) * channel->block_size);
#অন্যথা
		গচ্ছ unimplemented;
#endif
	}
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			গচ্ছ unimplemented;
		}
		ঘূৰাই errno;
	}
	ঘূৰাই 0;
unimplemented:
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
স্থির পূর্ণাঙ্ক __unix_zeroout(পূর্ণাঙ্ক fd, off_t offset, off_t len)
{
	পূর্ণাঙ্ক ret = -1;

#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	যদি (ret == 0)
		ঘূৰাই 0;
#endif
#যদি defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	যদি (ret == 0)
		ঘূৰাই 0;
#endif
	errno = EOPNOTSUPP;
	ঘূৰাই ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
স্থির errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা unix_private_data *data;
	পূর্ণাঙ্ক		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		সংরচনা stat statbuf;

		যদি (গুনতি == 0)
			ঘূৰাই 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		যদি (ret)
			গচ্ছ err;
		যদি ((unsigned long long) statbuf.st_size <
			(block + গুনতি) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + গুনতি) * channel->block_size + data->offset);
			যদি (ret)
				গচ্ছ err;
		}
	}

	যদি (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		গচ্ছ unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(গুনতি) * channel->block_size);
err:
	যদি (ret < 0) {
		যদি (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			গচ্ছ unimplemented;
		}
		ঘূৰাই errno;
	}
	ঘূৰাই 0;
unimplemented:
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}
#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বেৰ_কৰো
#endif

স্থির সংরচনা struct_io_manager struct_unix_manager = {
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

স্থির সংরচনা struct_io_manager struct_unixfd_manager = {
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
