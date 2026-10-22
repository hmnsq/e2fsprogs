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

#यदि !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#अन्तर्भाव <sys/mount.h>
#endif
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#endif
#यदि HAVE_LINUX_FALLOC_H
#अन्तर्भाव <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#अन्तर्भाव <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#अन्तर्भाव <sys/file.h>
#endif

#यदि defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर अन्तःस्थ पूर्णाङ्क find_last_bit_set(पूर्णाङ्क x)
{
	पूर्णाङ्क r = 32;

	यदि (!x)
		फर्काऊ 0;
	यदि (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	यदि (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	यदि (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	यदि (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	यदि (!(x & 0x80000000u)) {
		r -= 1;
	}
	फर्काऊ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
स्थिर अन्तःस्थ पूर्णाङ्क highbit32(uint32_t v)
{
	फर्काऊ find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) फर्काऊ (code)

संरचना unix_cache {
	वर्ण			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णाङ्क			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
	अचिन्हित		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

संरचना unix_private_data {
	पूर्णाङ्क	magic;
	पूर्णाङ्क	dev;
	पूर्णाङ्क	flags;
	पूर्णाङ्क	align;
	पूर्णाङ्क	access_time;
	पूर्णाङ्क	unix_flock_flags;
	ext2_loff_t offset;
	संरचना unix_cache *cache;
	अचिन्हित पूर्णाङ्क cache_size;
	अचिन्हित पूर्णाङ्क cache_hash_shift;
	शून्य	*bounce;
	संरचना struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

प्रकारनिर्देश गणना lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
स्थिर अन्तःस्थ pthread_mutex_t *get_mutex(संरचना unix_private_data *data,
					 kind_t kind)
{
	यदि (data->flags & IO_FLAG_THREADS) {
		विकल्प (kind) {
		पक्ष CACHE_MTX:
			फर्काऊ &data->cache_mutex;
		पक्ष BOUNCE_MTX:
			फर्काऊ &data->bounce_mutex;
		पक्ष STATS_MTX:
			फर्काऊ &data->stats_mutex;
		}
	}
	फर्काऊ रिक्त;
}
#endif

स्थिर अन्तःस्थ शून्य mutex_lock(संरचना unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	यदि (mtx)
		pthread_mutex_lock(mtx);
#endif
}

स्थिर अन्तःस्थ शून्य mutex_unlock(संरचना unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	यदि (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

स्थिर errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	संरचना unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	फर्काऊ retval;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना unix_private_data *data,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णाङ्क गन्ती, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	पूर्णाङ्क		actual = 0;
	अचिन्हित वर्ण	*buf = bufv;
	ssize_t		really_read = 0;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णाङ्क		align_size, offset;

	size = (गन्ती < 0) ? -गन्ती : (ext2_loff_t) गन्ती * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	यदि (data->flags & IO_FLAG_FORCE_BOUNCE)
		गच्छ bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		यदि (actual == size)
			फर्काऊ 0;
		actual = 0;
	}
#अथवायदि HAVE_PREAD
	/* Try an aligned pread */
	यदि ((परिमाण(off_t) >= परिमाण(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		यदि (actual == size)
			फर्काऊ 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		यदि (ext2fs_llseek(data->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual = पठ(data->dev, buf, size);
		यदि (actual != size) {
		short_read:
			यदि (actual < 0) {
				retval = त्रुटि_सं;
				actual = 0;
			} अन्यथा
				retval = EXT2_ET_SHORT_READ;
			गच्छ error_unlock;
		}
		गच्छ success_unlock;
	}

#ifdef ALIGN_DEBUG
	म_लिखो("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिन्हित दीर्घ) size);
#endif

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	यदि (channel->align == 0)
		channel->align = 1;
	यदि ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	अन्यथा
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	यदि (ext2fs_llseek(data->dev, aligned_blk * align_size, शुरू_से) < 0) {
		retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
		गच्छ error_unlock;
	}
	जबसम्म (size > 0) {
		actual = पठ(data->dev, data->bounce, align_size);
		यदि (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			गच्छ short_read;
		}
		यदि ((actual + offset) > align_size)
			actual = align_size - offset;
		यदि (actual > size)
			actual = size;
		स_नकल(buf, (वर्ण *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	फर्काऊ 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	यदि (actual >= 0 && actual < size)
		स_रखो((वर्ण *) buf+actual, 0, size-actual);
	यदि (channel->read_error)
		retval = (channel->read_error)(channel, block, गन्ती, buf,
					       size, actual, retval);
	फर्काऊ retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना unix_private_data *data,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गन्ती, अचर शून्य *bufv,
			       पूर्णाङ्क flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	पूर्णाङ्क		actual = 0;
	errcode_t	retval;
	अचर अचिन्हित वर्ण *buf = bufv;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णाङ्क		align_size, offset;

	यदि (गन्ती == 1)
		size = channel->block_size;
	अन्यथा {
		यदि (गन्ती < 0)
			size = -गन्ती;
		अन्यथा
			size = (ext2_loff_t) गन्ती * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	यदि (data->flags & IO_FLAG_FORCE_BOUNCE)
		गच्छ bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		यदि (actual == size)
			फर्काऊ 0;
	}
#अथवायदि HAVE_PWRITE
	/* Try an aligned pwrite */
	यदि ((परिमाण(off_t) >= परिमाण(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		यदि (actual == size)
			फर्काऊ 0;
	}
#endif /* HAVE_PWRITE */

	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		यदि (ext2fs_llseek(data->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual = लिख(data->dev, buf, size);
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		यदि (actual < 0) {
			retval = त्रुटि_सं;
			गच्छ error_out;
		}
		यदि (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_out;
		}
		फर्काऊ 0;
	}

#ifdef ALIGN_DEBUG
	म_लिखो("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिन्हित दीर्घ) size);
#endif
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	यदि (channel->align == 0)
		channel->align = 1;
	यदि ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	अन्यथा
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	जबसम्म (size > 0) {
		पूर्णाङ्क actual_w;

		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		यदि (size < align_size || offset) {
			यदि (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  शुरू_से) < 0) {
				retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
				गच्छ error_unlock;
			}
			actual = पठ(data->dev, data->bounce,
				      align_size);
			यदि (actual != align_size) {
				यदि (actual < 0) {
					retval = त्रुटि_सं;
					गच्छ error_unlock;
				}
				स_रखो((वर्ण *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		यदि ((actual + offset) > align_size)
			actual = align_size - offset;
		यदि (actual > size)
			actual = size;
		स_नकल(((वर्ण *)data->bounce) + offset, buf, actual);
		यदि (ext2fs_llseek(data->dev, aligned_blk * align_size, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual_w = लिख(data->dev, data->bounce, align_size);
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		यदि (actual_w < 0) {
			retval = त्रुटि_सं;
			गच्छ error_out;
		}
		यदि (actual_w != align_size)
			गच्छ short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	फर्काऊ 0;

error_unlock:
	यदि (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	यदि (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, गन्ती, buf,
						size, actual, retval);
	फर्काऊ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
स्थिर errcode_t alloc_cache(io_channel channel,
			     संरचना unix_private_data *data)
{
	errcode_t		retval;
	संरचना unix_cache	*cache;
	अचिन्हित पूर्णाङ्क		i;

	data->access_time = 0;
	लागि (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		यदि (retval)
			फर्काऊ retval;
	}
	यदि (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	फर्काऊ retval;
}

/* Free the cache buffers */
स्थिर शून्य free_cache(संरचना unix_private_data *data)
{
	संरचना unix_cache	*cache;
	अचिन्हित पूर्णाङ्क		i;

	data->access_time = 0;
	लागि (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	यदि (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_अविभाज्य	0x9e37fffffffc0001UL
#ifndef CACHE_पङ्क्ति_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_पङ्क्ति_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
स्थिर अचिन्हित पूर्णाङ्क
cache_hash(संरचना unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	यदि (data->cache_size <= DEFAULT_CACHE_SIZE)
		फर्काऊ 0;

	tmp = hashval ^ (GOLDEN_RATIO_अविभाज्य + hashval) / CACHE_पङ्क्ति_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_अविभाज्य) >> data->cache_hash_shift);
	फर्काऊ tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
स्थिर संरचना unix_cache *find_cached_block(संरचना unix_private_data *data,
					    अचिन्हित दीर्घ दीर्घ block,
					    संरचना unix_cache **eldest)
{
	संरचना unix_cache	*cache, *unused_cache, *oldest_cache;
	अचिन्हित पूर्णाङ्क		hash = cache_hash(data, block);
	अचिन्हित पूर्णाङ्क		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	लागि (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		यदि (!cache->in_use) {
			यदि (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		यदि (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			फर्काऊ cache;
		}
		यदि (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	लागि (i = 0, cache = data->cache; i < hash; i++, cache++) {
		यदि (!cache->in_use) {
			यदि (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		यदि (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			फर्काऊ cache;
		}
		यदि (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	यदि (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	फर्काऊ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर errcode_t reuse_cache(io_channel channel,
		संरचना unix_private_data *data, संरचना unix_cache *cache,
		अचिन्हित दीर्घ दीर्घ block)
{
	यदि (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		यदि (retval) {
			cache->write_err = 1;
			फर्काऊ retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	फर्काऊ 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
स्थिर errcode_t flush_cached_blocks(io_channel channel,
				     संरचना unix_private_data *data,
				     पूर्णाङ्क flags)
{
	संरचना unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	अचिन्हित पूर्णाङ्क		i;
	पूर्णाङ्क			errors_found = 0;

	यदि ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	लागि (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		यदि (!cache->in_use)
			जारी;
		यदि (cache->dirty) {
			पूर्णाङ्क raw_flags = RAW_WRITE_NO_HANDLER;

			यदि (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			यदि (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} अन्यथा {
				cache->dirty = 0;
				cache->write_err = 0;
				यदि (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} अन्यथा यदि (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	यदि ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	जबसम्म (errors_found) {
		यदि ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		लागि (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			यदि (!cache->in_use || !cache->write_err)
				जारी;
			errors_found = 1;
			यदि (cache->write_err && channel->write_error) {
				वर्ण *err_buf = रिक्त;
				अचिन्हित दीर्घ दीर्घ err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				यदि (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = रिक्त;
				अन्यथा
					स_नकल(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				यदि (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				गच्छ retry;
			} अन्यथा
				cache->write_err = 0;
		}
		यदि ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	फर्काऊ retval2;
}

/* Shrink the cache buffers */
स्थिर errcode_t shrink_cache(io_channel channel,
			      संरचना unix_private_data *data,
			      अचिन्हित पूर्णाङ्क new_size)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णाङ्क		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	यदि (retval)
		गच्छ unlock;

	लागि (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = पुनः_स्मृति(data->cache, new_size * परिमाण(संरचना unix_cache));
	यदि (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		गच्छ unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	फर्काऊ retval;
}

/* Grow the cache buffers */
स्थिर errcode_t grow_cache(io_channel channel,
			    संरचना unix_private_data *data,
			    अचिन्हित पूर्णाङ्क new_size)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णाङ्क		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	यदि (retval)
		गच्छ unlock;

	new_cache = पुनः_स्मृति(data->cache, new_size * परिमाण(संरचना unix_cache));
	यदि (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		गच्छ unlock;
	}
	data->cache = new_cache;

	लागि (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		यदि (retval) {
			जबसम्म (i > data->cache_size) {
				cache--; i--;
				यदि (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			गच्छ unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	फर्काऊ retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

पूर्णाङ्क ext2fs_open_file(अचर वर्ण *pathname, पूर्णाङ्क flags, mode_t mode)
{
	यदि (mode)
#यदि defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		फर्काऊ open64(pathname, flags, mode);
	अन्यथा
		फर्काऊ open64(pathname, flags);
#अन्यथा
		फर्काऊ खोलो(pathname, flags, mode);
	अन्यथा
		फर्काऊ खोलो(pathname, flags);
#endif
}

पूर्णाङ्क ext2fs_stat(अचर वर्ण *path, ext2fs_struct_stat *buf)
{
#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	फर्काऊ stat64(path, buf);
#अन्यथा
	फर्काऊ stat(path, buf);
#endif
}

पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf)
{
#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	फर्काऊ fstat64(fd, buf);
#अन्यथा
	फर्काऊ ख_स्थिति(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
स्थिर errcode_t unix_funlock(io_channel channel)
{
	संरचना unix_private_data *data;
	पूर्णाङ्क ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		यदि (ret)
			फर्काऊ त्रुटि_सं;

		data->unix_flock_flags = 0;
	}

	फर्काऊ 0;
}

स्थिर errcode_t unix_flock(io_channel channel, अचिन्हित पूर्णाङ्क flock_flags)
{
	संरचना unix_private_data *data;
	पूर्णाङ्क unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	यदि (ret)
		फर्काऊ ret;

	यदि (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	यदि (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	यदि (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	यदि (!unix_flock_flags)
		फर्काऊ 0;

	ret = flock(data->dev, unix_flock_flags);
	यदि (ret < 0)
		फर्काऊ त्रुटि_सं;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	फर्काऊ 0;
}
#अन्यथा
#define unix_flock		रिक्त

स्थिर errcode_t unix_funlock(io_channel channel)
{
	फर्काऊ 0;
}
#endif /* HAVE_SYS_FILE_H */

स्थिर errcode_t unix_open_channel(अचर वर्ण *name, पूर्णाङ्क fd,
				   पूर्णाङ्क flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	संरचना unix_private_data *data = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	संरचना		utsname ut;
#endif

	यदि (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(शून्य) fsync(fd);
#endif

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स_रखो(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना unix_private_data), &data);
	यदि (retval)
		गच्छ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	यदि (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	स_रखो(data, 0, परिमाण(संरचना unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.अङ्क_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = सुस्मृति(DEFAULT_CACHE_SIZE, परिमाण(संरचना unix_cache));
	यदि (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		गच्छ cleanup;
	}

#यदि defined(O_DIRECT)
	यदि (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#अथवायदि defined(F_NOCACHE)
	यदि (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	यदि (ext2fs_fstat(data->dev, &st) == 0) {
		यदि (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			पूर्णाङ्क zeroes = 0;

			यदि (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} अन्यथा {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#यदि defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	यदि (!io->align)
		io->align = 512;
#endif

#यदि defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	यदि (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		पूर्णाङ्क dio_align = ext2fs_get_dio_alignment(fd);

		यदि (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	यदि ((retval = alloc_cache(io, data)))
		गच्छ cleanup;

#ifdef BLKROGET
	यदि (flags & IO_FLAG_RW) {
		पूर्णाङ्क error;
		पूर्णाङ्क readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		यदि (!error && readonly) {
			retval = EPERM;
			गच्छ cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#यदि (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((अचिन्हित दीर्घ)(~0UL>>1))
#अन्यथा
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	यदि ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		संरचना rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (अचिन्हित दीर्घ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		यदि (((अचिन्हित दीर्घ) rlim.rlim_cur) <
		    ((अचिन्हित दीर्घ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	यदि (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, रिक्त);
		यदि (retval)
			गच्छ cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, रिक्त);
		यदि (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			गच्छ cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, रिक्त);
		यदि (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			गच्छ cleanup;
		}
	}
#endif
	*channel = io;
	फर्काऊ 0;

cleanup:
	यदि (data) {
		unix_funlock(io);
		यदि (io->manager != unixfd_io_manager && data->dev >= 0)
			बंद(data->dev);
		यदि (data->cache) {
			free_cache(data);
			मुक्त(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	यदि (io) {
		यदि (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	फर्काऊ retval;
}

स्थिर errcode_t unixfd_open(अचर वर्ण *str_fd, पूर्णाङ्क flags,
			     io_channel *channel)
{
	पूर्णाङ्क fd;
	पूर्णाङ्क fd_flags;

	fd = म_से_प(str_fd);
#यदि defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	यदि (fd_flags == -1)
		फर्काऊ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	यदि (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#यदि defined(O_DIRECT)
	यदि (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	फर्काऊ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

स्थिर errcode_t unix_open(अचर वर्ण *name, पूर्णाङ्क flags,
			   io_channel *channel)
{
	पूर्णाङ्क fd = -1;
	पूर्णाङ्क open_flags;

	यदि (name == 0)
		फर्काऊ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	यदि (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#यदि defined(O_DIRECT)
	यदि (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	यदि (fd < 0)
		फर्काऊ त्रुटि_सं;
#यदि defined(F_NOCACHE) && !defined(IO_DIRECT)
	यदि (flags & IO_FLAG_DIRECT_IO) {
		यदि (fcntl(fd, F_NOCACHE, 1) < 0)
			फर्काऊ त्रुटि_सं;
	}
#endif
	फर्काऊ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

स्थिर errcode_t unix_close(io_channel channel)
{
	संरचना unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		फर्काऊ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	यदि (channel->manager != unixfd_io_manager && बंद(data->dev) < 0)
		retval = त्रुटि_सं;
	free_cache(data);
	मुक्त(data->cache);
#ifdef HAVE_PTHREAD
	यदि (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	फर्काऊ retval;
}

स्थिर errcode_t unix_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		यदि ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			फर्काऊ retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	फर्काऊ retval;
}

स्थिर errcode_t unix_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गन्ती, शून्य *buf)
{
	संरचना unix_private_data *data;
	संरचना unix_cache *cache;
	errcode_t	retval;
	वर्ण		*cp;
	पूर्णाङ्क		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	फर्काऊ raw_read_blk(channel, data, block, गन्ती, buf);
#अन्यथा
	यदि (data->flags & IO_FLAG_NOCACHE)
		फर्काऊ raw_read_blk(channel, data, block, गन्ती, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	यदि (गन्ती < 0 || गन्ती > WRITE_DIRECT_SIZE) {
		यदि ((retval = flush_cached_blocks(channel, data, 0)))
			फर्काऊ retval;
		फर्काऊ raw_read_blk(channel, data, block, गन्ती, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	जबसम्म (गन्ती > 0) {
		/* If it's in the cache, use it! */
		यदि ((cache = find_cached_block(data, block, रिक्त))) {
#ifdef DEBUG
			म_लिखो("Using cached block %lu\n", block);
#endif
			स_नकल(cp, cache->buf, channel->block_size);
			गन्ती--;
			block++;
			cp += channel->block_size;
			जारी;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		लागि (i=1; i < गन्ती; i++)
			यदि (find_cached_block(data, block+i, रिक्त))
				रोक;
#ifdef DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		यदि ((retval = raw_read_blk(channel, data, block, i, cp)))
			फर्काऊ retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		लागि (j=0; j < i; j++) {
			यदि (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				यदि (retval)
					गच्छ call_write_handler;
				स_नकल(cache->buf, cp, channel->block_size);
			}
			गन्ती--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	फर्काऊ 0;

call_write_handler:
	यदि (cache->write_err && channel->write_error) {
		वर्ण *err_buf = रिक्त;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		यदि (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		यदि (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(data, CACHE_MTX);
	फर्काऊ retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t unix_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गन्ती, शून्य *buf)
{
	फर्काऊ unix_read_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t unix_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	संरचना unix_private_data *data;
	संरचना unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	अचर वर्ण	*cp;
	पूर्णाङ्क		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	फर्काऊ raw_write_blk(channel, data, block, गन्ती, buf, 0);
#अन्यथा
	यदि (data->flags & IO_FLAG_NOCACHE)
		फर्काऊ raw_write_blk(channel, data, block, गन्ती, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	यदि (गन्ती < 0 || गन्ती > WRITE_DIRECT_SIZE) {
		यदि ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			फर्काऊ retval;
		फर्काऊ raw_write_blk(channel, data, block, गन्ती, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	यदि (writethrough)
		retval = raw_write_blk(channel, data, block, गन्ती, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	जबसम्म (गन्ती > 0) {
		cache = find_cached_block(data, block, &reuse);
		यदि (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			यदि (err)
				गच्छ call_write_handler;
		}
		यदि (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		गन्ती--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	फर्काऊ retval;

call_write_handler:
	यदि (cache->write_err && channel->write_error) {
		वर्ण *err_buf = रिक्त;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		यदि (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		यदि (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(data, CACHE_MTX);
	फर्काऊ retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t unix_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गन्ती)
{
#ifdef POSIX_FADV_WILLNEED
	संरचना unix_private_data *data;

	data = (संरचना unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	फर्काऊ posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)गन्ती * channel->block_size,
			     POSIX_FADV_WILLNEED);
#अन्यथा
	फर्काऊ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

स्थिर errcode_t unix_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	फर्काऊ unix_write_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t unix_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णाङ्क size, अचर शून्य *buf)
{
	संरचना unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->align != 0) {
#ifdef ALIGN_DEBUG
		म_लिखो("unix_write_byte: O_DIRECT fallback\n");
#endif
		फर्काऊ EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	यदि ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		फर्काऊ retval;
#endif

	यदि (lseek(data->dev, offset + data->offset, शुरू_से) < 0)
		फर्काऊ त्रुटि_सं;

	actual = लिख(data->dev, buf, size);
	यदि (actual < 0)
		फर्काऊ त्रुटि_सं;
	यदि (actual != size)
		फर्काऊ EXT2_ET_SHORT_WRITE;

	फर्काऊ 0;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t unix_flush(io_channel channel)
{
	संरचना unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	यदि (!retval && fsync(data->dev) != 0)
		फर्काऊ त्रुटि_सं;
#endif
	फर्काऊ retval;
}

स्थिर errcode_t unix_set_option(io_channel channel, अचर वर्ण *option,
				 अचर वर्ण *arg)
{
	संरचना unix_private_data *data;
	अचिन्हित दीर्घ दीर्घ tmp;
	errcode_t retval;
	वर्ण *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!म_भेद(option, "offset")) {
		यदि (!arg)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		यदि (*end)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		यदि (data->offset < 0)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;
		फर्काऊ 0;
	}
	यदि (!म_भेद(option, "cache")) {
		यदि (!arg)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;
		यदि (!म_भेद(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			फर्काऊ 0;
		}
		यदि (!म_भेद(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			फर्काऊ retval;
		}
		फर्काऊ EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	यदि (!म_भेद(option, "cache_blocks")) {
		अचिन्हित दीर्घ दीर्घ	size;

		यदि (!arg)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;

		त्रुटि_सं = 0;
		size = strtoll(arg, रिक्त, 0);
		यदि (त्रुटि_सं || size == 0 || size > INT32_MAX)
			फर्काऊ EXT2_ET_INVALID_ARGUMENT;

		यदि (data->cache_size == size)
			फर्काऊ 0;
		यदि (data->cache_size > size)
			फर्काऊ shrink_cache(channel, data, size);
		फर्काऊ grow_cache(channel, data, size);
	}
#endif
	फर्काऊ EXT2_ET_INVALID_ARGUMENT;
}

#यदि defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

स्थिर errcode_t unix_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गन्ती)
{
	संरचना unix_private_data *data;
	पूर्णाङ्क		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->flags & CHANNEL_FLAGS_NODISCARD)
		गच्छ unimplemented;

	यदि (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(गन्ती) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#अन्यथा
		गच्छ unimplemented;
#endif
	} अन्यथा {
#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(गन्ती) * channel->block_size);
#अन्यथा
		गच्छ unimplemented;
#endif
	}
	यदि (ret < 0) {
		यदि (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			गच्छ unimplemented;
		}
		फर्काऊ त्रुटि_सं;
	}
	फर्काऊ 0;
unimplemented:
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
स्थिर पूर्णाङ्क __unix_zeroout(पूर्णाङ्क fd, off_t offset, off_t len)
{
	पूर्णाङ्क ret = -1;

#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	यदि (ret == 0)
		फर्काऊ 0;
#endif
#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	यदि (ret == 0)
		फर्काऊ 0;
#endif
	त्रुटि_सं = EOPNOTSUPP;
	फर्काऊ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic थप
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
स्थिर errcode_t unix_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गन्ती)
{
	संरचना unix_private_data *data;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		संरचना stat statbuf;

		यदि (गन्ती == 0)
			फर्काऊ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = ख_स्थिति(data->dev, &statbuf);
		यदि (ret)
			गच्छ err;
		यदि ((अचिन्हित दीर्घ दीर्घ) statbuf.st_size <
			(block + गन्ती) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + गन्ती) * channel->block_size + data->offset);
			यदि (ret)
				गच्छ err;
		}
	}

	यदि (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		गच्छ unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(गन्ती) * channel->block_size);
err:
	यदि (ret < 0) {
		यदि (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			गच्छ unimplemented;
		}
		फर्काऊ त्रुटि_सं;
	}
	फर्काऊ 0;
unimplemented:
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}
#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic झिक
#endif

स्थिर संरचना struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.खोलो		= unix_open,
	.बंद		= unix_close,
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

स्थिर संरचना struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.खोलो		= unixfd_open,
	.बंद		= unix_close,
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
