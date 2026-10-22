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

#जर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#अन्तर्भाव <sys/mount.h>
#endif
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#endif
#जर HAVE_LINUX_FALLOC_H
#अन्तर्भाव <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#अन्तर्भाव <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#अन्तर्भाव <sys/file.h>
#endif

#जर defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर ओळीत पूर्णांक find_last_bit_set(पूर्णांक x)
{
	पूर्णांक r = 32;

	जर (!x)
		परत 0;
	जर (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	जर (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	जर (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	जर (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	जर (!(x & 0x80000000u)) {
		r -= 1;
	}
	परत r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
स्थिर ओळीत पूर्णांक highbit32(uint32_t v)
{
	परत find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जर ((संरचना)->magic != (code)) परत (code)

संरचना unix_cache {
	अक्षर			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णांक			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
	अचिन्हित		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

संरचना unix_private_data {
	पूर्णांक	magic;
	पूर्णांक	dev;
	पूर्णांक	flags;
	पूर्णांक	align;
	पूर्णांक	access_time;
	पूर्णांक	unix_flock_flags;
	ext2_loff_t offset;
	संरचना unix_cache *cache;
	अचिन्हित पूर्णांक cache_size;
	अचिन्हित पूर्णांक cache_hash_shift;
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

प्रकारव्याख्या गणना lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
स्थिर ओळीत pthread_mutex_t *get_mutex(संरचना unix_private_data *data,
					 kind_t kind)
{
	जर (data->flags & IO_FLAG_THREADS) {
		निवड (kind) {
		स्थिती CACHE_MTX:
			परत &data->cache_mutex;
		स्थिती BOUNCE_MTX:
			परत &data->bounce_mutex;
		स्थिती STATS_MTX:
			परत &data->stats_mutex;
		}
	}
	परत रिक्त;
}
#endif

स्थिर ओळीत शून्य mutex_lock(संरचना unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	जर (mtx)
		pthread_mutex_lock(mtx);
#endif
}

स्थिर ओळीत शून्य mutex_unlock(संरचना unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	जर (mtx)
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

	जर (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	परत retval;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना unix_private_data *data,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णांक मोजणी, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	पूर्णांक		actual = 0;
	अचिन्हित अक्षर	*buf = bufv;
	ssize_t		really_read = 0;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णांक		align_size, offset;

	size = (मोजणी < 0) ? -मोजणी : (ext2_loff_t) मोजणी * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जर (data->flags & IO_FLAG_FORCE_BOUNCE)
		जा bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	जर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		जर (actual == size)
			परत 0;
		actual = 0;
	}
#किंवाजर HAVE_PREAD
	/* Try an aligned pread */
	जर ((आकार(off_t) >= आकार(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		जर (actual == size)
			परत 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	जर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		जर (ext2fs_llseek(data->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जा error_unlock;
		}
		actual = पठ(data->dev, buf, size);
		जर (actual != size) {
		short_read:
			जर (actual < 0) {
				retval = त्रुटि_सं;
				actual = 0;
			} अन्यथा
				retval = EXT2_ET_SHORT_READ;
			जा error_unlock;
		}
		जा success_unlock;
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
	जर (channel->align == 0)
		channel->align = 1;
	जर ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	अन्यथा
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	जर (ext2fs_llseek(data->dev, aligned_blk * align_size, शुरू_से) < 0) {
		retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
		जा error_unlock;
	}
	पर्यंत (size > 0) {
		actual = पठ(data->dev, data->bounce, align_size);
		जर (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			जा short_read;
		}
		जर ((actual + offset) > align_size)
			actual = align_size - offset;
		जर (actual > size)
			actual = size;
		स_नकल(buf, (अक्षर *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	परत 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	जर (actual >= 0 && actual < size)
		स_रखो((अक्षर *) buf+actual, 0, size-actual);
	जर (channel->read_error)
		retval = (channel->read_error)(channel, block, मोजणी, buf,
					       size, actual, retval);
	परत retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना unix_private_data *data,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, स्थिरांक शून्य *bufv,
			       पूर्णांक flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	पूर्णांक		actual = 0;
	errcode_t	retval;
	स्थिरांक अचिन्हित अक्षर *buf = bufv;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णांक		align_size, offset;

	जर (मोजणी == 1)
		size = channel->block_size;
	अन्यथा {
		जर (मोजणी < 0)
			size = -मोजणी;
		अन्यथा
			size = (ext2_loff_t) मोजणी * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जर (data->flags & IO_FLAG_FORCE_BOUNCE)
		जा bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	जर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		जर (actual == size)
			परत 0;
	}
#किंवाजर HAVE_PWRITE
	/* Try an aligned pwrite */
	जर ((आकार(off_t) >= आकार(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		जर (actual == size)
			परत 0;
	}
#endif /* HAVE_PWRITE */

	जर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		जर (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		जर (ext2fs_llseek(data->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जा error_unlock;
		}
		actual = लिख(data->dev, buf, size);
		जर (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		जर (actual < 0) {
			retval = त्रुटि_सं;
			जा error_out;
		}
		जर (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			जा error_out;
		}
		परत 0;
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
	जर (channel->align == 0)
		channel->align = 1;
	जर ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	अन्यथा
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	पर्यंत (size > 0) {
		पूर्णांक actual_w;

		जर (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		जर (size < align_size || offset) {
			जर (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  शुरू_से) < 0) {
				retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
				जा error_unlock;
			}
			actual = पठ(data->dev, data->bounce,
				      align_size);
			जर (actual != align_size) {
				जर (actual < 0) {
					retval = त्रुटि_सं;
					जा error_unlock;
				}
				स_रखो((अक्षर *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		जर ((actual + offset) > align_size)
			actual = align_size - offset;
		जर (actual > size)
			actual = size;
		स_नकल(((अक्षर *)data->bounce) + offset, buf, actual);
		जर (ext2fs_llseek(data->dev, aligned_blk * align_size, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जा error_unlock;
		}
		actual_w = लिख(data->dev, data->bounce, align_size);
		जर (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		जर (actual_w < 0) {
			retval = त्रुटि_सं;
			जा error_out;
		}
		जर (actual_w != align_size)
			जा short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	परत 0;

error_unlock:
	जर (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	जर (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, मोजणी, buf,
						size, actual, retval);
	परत retval;
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
	अचिन्हित पूर्णांक		i;

	data->access_time = 0;
	साठी (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		जर (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		जर (retval)
			परत retval;
	}
	जर (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		जर (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	परत retval;
}

/* Free the cache buffers */
स्थिर शून्य free_cache(संरचना unix_private_data *data)
{
	संरचना unix_cache	*cache;
	अचिन्हित पूर्णांक		i;

	data->access_time = 0;
	साठी (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		जर (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	जर (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_मूळसंख्या	0x9e37fffffffc0001UL
#ifndef CACHE_ओळ_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_ओळ_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
स्थिर अचिन्हित पूर्णांक
cache_hash(संरचना unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	जर (data->cache_size <= DEFAULT_CACHE_SIZE)
		परत 0;

	tmp = hashval ^ (GOLDEN_RATIO_मूळसंख्या + hashval) / CACHE_ओळ_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_मूळसंख्या) >> data->cache_hash_shift);
	परत tmp % data->cache_size;
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
	अचिन्हित पूर्णांक		hash = cache_hash(data, block);
	अचिन्हित पूर्णांक		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	साठी (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		जर (!cache->in_use) {
			जर (!unused_cache)
				unused_cache = cache;
			सुरूठेवा;
		}
		जर (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			परत cache;
		}
		जर (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	साठी (i = 0, cache = data->cache; i < hash; i++, cache++) {
		जर (!cache->in_use) {
			जर (!unused_cache)
				unused_cache = cache;
			सुरूठेवा;
		}
		जर (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			परत cache;
		}
		जर (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	जर (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	परत 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर errcode_t reuse_cache(io_channel channel,
		संरचना unix_private_data *data, संरचना unix_cache *cache,
		अचिन्हित दीर्घ दीर्घ block)
{
	जर (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		जर (retval) {
			cache->write_err = 1;
			परत retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	परत 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
स्थिर errcode_t flush_cached_blocks(io_channel channel,
				     संरचना unix_private_data *data,
				     पूर्णांक flags)
{
	संरचना unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	अचिन्हित पूर्णांक		i;
	पूर्णांक			errors_found = 0;

	जर ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	साठी (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		जर (!cache->in_use)
			सुरूठेवा;
		जर (cache->dirty) {
			पूर्णांक raw_flags = RAW_WRITE_NO_HANDLER;

			जर (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			जर (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} अन्यथा {
				cache->dirty = 0;
				cache->write_err = 0;
				जर (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} अन्यथा जर (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	जर ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	पर्यंत (errors_found) {
		जर ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		साठी (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			जर (!cache->in_use || !cache->write_err)
				सुरूठेवा;
			errors_found = 1;
			जर (cache->write_err && channel->write_error) {
				अक्षर *err_buf = रिक्त;
				अचिन्हित दीर्घ दीर्घ err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				जर (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = रिक्त;
				अन्यथा
					स_नकल(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				जर (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				जा retry;
			} अन्यथा
				cache->write_err = 0;
		}
		जर ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	परत retval2;
}

/* Shrink the cache buffers */
स्थिर errcode_t shrink_cache(io_channel channel,
			      संरचना unix_private_data *data,
			      अचिन्हित पूर्णांक new_size)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णांक		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	जर (retval)
		जा unlock;

	साठी (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		जर (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = पुनः_स्मृति(data->cache, new_size * आकार(संरचना unix_cache));
	जर (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		जा unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	परत retval;
}

/* Grow the cache buffers */
स्थिर errcode_t grow_cache(io_channel channel,
			    संरचना unix_private_data *data,
			    अचिन्हित पूर्णांक new_size)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णांक		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	जर (retval)
		जा unlock;

	new_cache = पुनः_स्मृति(data->cache, new_size * आकार(संरचना unix_cache));
	जर (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		जा unlock;
	}
	data->cache = new_cache;

	साठी (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		जर (retval) {
			पर्यंत (i > data->cache_size) {
				cache--; i--;
				जर (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			जा unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	परत retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

पूर्णांक ext2fs_open_file(स्थिरांक अक्षर *pathname, पूर्णांक flags, mode_t mode)
{
	जर (mode)
#जर defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		परत open64(pathname, flags, mode);
	अन्यथा
		परत open64(pathname, flags);
#अन्यथा
		परत खोलो(pathname, flags, mode);
	अन्यथा
		परत खोलो(pathname, flags);
#endif
}

पूर्णांक ext2fs_stat(स्थिरांक अक्षर *path, ext2fs_struct_stat *buf)
{
#जर defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	परत stat64(path, buf);
#अन्यथा
	परत stat(path, buf);
#endif
}

पूर्णांक ext2fs_fstat(पूर्णांक fd, ext2fs_struct_stat *buf)
{
#जर defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	परत fstat64(fd, buf);
#अन्यथा
	परत ख_स्थिति(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
स्थिर errcode_t unix_funlock(io_channel channel)
{
	संरचना unix_private_data *data;
	पूर्णांक ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		जर (ret)
			परत त्रुटि_सं;

		data->unix_flock_flags = 0;
	}

	परत 0;
}

स्थिर errcode_t unix_flock(io_channel channel, अचिन्हित पूर्णांक flock_flags)
{
	संरचना unix_private_data *data;
	पूर्णांक unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	जर (ret)
		परत ret;

	जर (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	जर (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	जर (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	जर (!unix_flock_flags)
		परत 0;

	ret = flock(data->dev, unix_flock_flags);
	जर (ret < 0)
		परत त्रुटि_सं;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	परत 0;
}
#अन्यथा
#define unix_flock		रिक्त

स्थिर errcode_t unix_funlock(io_channel channel)
{
	परत 0;
}
#endif /* HAVE_SYS_FILE_H */

स्थिर errcode_t unix_open_channel(स्थिरांक अक्षर *name, पूर्णांक fd,
				   पूर्णांक flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	संरचना unix_private_data *data = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	संरचना		utsname ut;
#endif

	जर (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(शून्य) fsync(fd);
#endif

	retval = ext2fs_get_mem(आकार(संरचना struct_io_channel), &io);
	जर (retval)
		जा cleanup;
	स_रखो(io, 0, आकार(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(आकार(संरचना unix_private_data), &data);
	जर (retval)
		जा cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	जर (retval)
		जा cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	जर (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	स_रखो(data, 0, आकार(संरचना unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.अंक_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = सुस्मृति(DEFAULT_CACHE_SIZE, आकार(संरचना unix_cache));
	जर (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		जा cleanup;
	}

#जर defined(O_DIRECT)
	जर (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#किंवाजर defined(F_NOCACHE)
	जर (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	जर (ext2fs_fstat(data->dev, &st) == 0) {
		जर (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			पूर्णांक zeroes = 0;

			जर (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} अन्यथा {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#जर defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	जर (!io->align)
		io->align = 512;
#endif

#जर defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	जर (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		पूर्णांक dio_align = ext2fs_get_dio_alignment(fd);

		जर (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	जर ((retval = alloc_cache(io, data)))
		जा cleanup;

#ifdef BLKROGET
	जर (flags & IO_FLAG_RW) {
		पूर्णांक error;
		पूर्णांक readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		जर (!error && readonly) {
			retval = EPERM;
			जा cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#जर (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
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
	जर ((flags & IO_FLAG_RW) &&
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
		जर (((अचिन्हित दीर्घ) rlim.rlim_cur) <
		    ((अचिन्हित दीर्घ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	जर (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, रिक्त);
		जर (retval)
			जा cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, रिक्त);
		जर (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			जा cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, रिक्त);
		जर (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			जा cleanup;
		}
	}
#endif
	*channel = io;
	परत 0;

cleanup:
	जर (data) {
		unix_funlock(io);
		जर (io->manager != unixfd_io_manager && data->dev >= 0)
			बंद(data->dev);
		जर (data->cache) {
			free_cache(data);
			मुक्त(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	जर (io) {
		जर (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	परत retval;
}

स्थिर errcode_t unixfd_open(स्थिरांक अक्षर *str_fd, पूर्णांक flags,
			     io_channel *channel)
{
	पूर्णांक fd;
	पूर्णांक fd_flags;

	fd = म_से_प(str_fd);
#जर defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	जर (fd_flags == -1)
		परत EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	जर (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#जर defined(O_DIRECT)
	जर (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	परत unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

स्थिर errcode_t unix_open(स्थिरांक अक्षर *name, पूर्णांक flags,
			   io_channel *channel)
{
	पूर्णांक fd = -1;
	पूर्णांक open_flags;

	जर (name == 0)
		परत EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	जर (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#जर defined(O_DIRECT)
	जर (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	जर (fd < 0)
		परत त्रुटि_सं;
#जर defined(F_NOCACHE) && !defined(IO_DIRECT)
	जर (flags & IO_FLAG_DIRECT_IO) {
		जर (fcntl(fd, F_NOCACHE, 1) < 0)
			परत त्रुटि_सं;
	}
#endif
	परत unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

स्थिर errcode_t unix_close(io_channel channel)
{
	संरचना unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (--channel->refcount > 0)
		परत 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	जर (channel->manager != unixfd_io_manager && बंद(data->dev) < 0)
		retval = त्रुटि_सं;
	free_cache(data);
	मुक्त(data->cache);
#ifdef HAVE_PTHREAD
	जर (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	जर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	परत retval;
}

स्थिर errcode_t unix_set_blksize(io_channel channel, पूर्णांक blksize)
{
	संरचना unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		जर ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			परत retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	परत retval;
}

स्थिर errcode_t unix_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	संरचना unix_private_data *data;
	संरचना unix_cache *cache;
	errcode_t	retval;
	अक्षर		*cp;
	पूर्णांक		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	परत raw_read_blk(channel, data, block, मोजणी, buf);
#अन्यथा
	जर (data->flags & IO_FLAG_NOCACHE)
		परत raw_read_blk(channel, data, block, मोजणी, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	जर (मोजणी < 0 || मोजणी > WRITE_DIRECT_SIZE) {
		जर ((retval = flush_cached_blocks(channel, data, 0)))
			परत retval;
		परत raw_read_blk(channel, data, block, मोजणी, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	पर्यंत (मोजणी > 0) {
		/* If it's in the cache, use it! */
		जर ((cache = find_cached_block(data, block, रिक्त))) {
#ifdef DEBUG
			म_लिखो("Using cached block %lu\n", block);
#endif
			स_नकल(cp, cache->buf, channel->block_size);
			मोजणी--;
			block++;
			cp += channel->block_size;
			सुरूठेवा;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		साठी (i=1; i < मोजणी; i++)
			जर (find_cached_block(data, block+i, रिक्त))
				थांबा;
#ifdef DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		जर ((retval = raw_read_blk(channel, data, block, i, cp)))
			परत retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		साठी (j=0; j < i; j++) {
			जर (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				जर (retval)
					जा call_write_handler;
				स_नकल(cache->buf, cp, channel->block_size);
			}
			मोजणी--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	परत 0;

call_write_handler:
	जर (cache->write_err && channel->write_error) {
		अक्षर *err_buf = रिक्त;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		जर (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		जर (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(data, CACHE_MTX);
	परत retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t unix_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	परत unix_read_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t unix_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना unix_private_data *data;
	संरचना unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	स्थिरांक अक्षर	*cp;
	पूर्णांक		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	परत raw_write_blk(channel, data, block, मोजणी, buf, 0);
#अन्यथा
	जर (data->flags & IO_FLAG_NOCACHE)
		परत raw_write_blk(channel, data, block, मोजणी, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	जर (मोजणी < 0 || मोजणी > WRITE_DIRECT_SIZE) {
		जर ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			परत retval;
		परत raw_write_blk(channel, data, block, मोजणी, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	जर (writethrough)
		retval = raw_write_blk(channel, data, block, मोजणी, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	पर्यंत (मोजणी > 0) {
		cache = find_cached_block(data, block, &reuse);
		जर (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			जर (err)
				जा call_write_handler;
		}
		जर (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		मोजणी--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	परत retval;

call_write_handler:
	जर (cache->write_err && channel->write_error) {
		अक्षर *err_buf = रिक्त;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		जर (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		जर (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(data, CACHE_MTX);
	परत retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t unix_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ मोजणी)
{
#ifdef POSIX_FADV_WILLNEED
	संरचना unix_private_data *data;

	data = (संरचना unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	परत posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)मोजणी * channel->block_size,
			     POSIX_FADV_WILLNEED);
#अन्यथा
	परत EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

स्थिर errcode_t unix_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	परत unix_write_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t unix_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक size, स्थिरांक शून्य *buf)
{
	संरचना unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (channel->align != 0) {
#ifdef ALIGN_DEBUG
		म_लिखो("unix_write_byte: O_DIRECT fallback\n");
#endif
		परत EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	जर ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		परत retval;
#endif

	जर (lseek(data->dev, offset + data->offset, शुरू_से) < 0)
		परत त्रुटि_सं;

	actual = लिख(data->dev, buf, size);
	जर (actual < 0)
		परत त्रुटि_सं;
	जर (actual != size)
		परत EXT2_ET_SHORT_WRITE;

	परत 0;
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
	जर (!retval && fsync(data->dev) != 0)
		परत त्रुटि_सं;
#endif
	परत retval;
}

स्थिर errcode_t unix_set_option(io_channel channel, स्थिरांक अक्षर *option,
				 स्थिरांक अक्षर *arg)
{
	संरचना unix_private_data *data;
	अचिन्हित दीर्घ दीर्घ tmp;
	errcode_t retval;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (!म_भेद(option, "offset")) {
		जर (!arg)
			परत EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		जर (*end)
			परत EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		जर (data->offset < 0)
			परत EXT2_ET_INVALID_ARGUMENT;
		परत 0;
	}
	जर (!म_भेद(option, "cache")) {
		जर (!arg)
			परत EXT2_ET_INVALID_ARGUMENT;
		जर (!म_भेद(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			परत 0;
		}
		जर (!म_भेद(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			परत retval;
		}
		परत EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	जर (!म_भेद(option, "cache_blocks")) {
		अचिन्हित दीर्घ दीर्घ	size;

		जर (!arg)
			परत EXT2_ET_INVALID_ARGUMENT;

		त्रुटि_सं = 0;
		size = strtoll(arg, रिक्त, 0);
		जर (त्रुटि_सं || size == 0 || size > INT32_MAX)
			परत EXT2_ET_INVALID_ARGUMENT;

		जर (data->cache_size == size)
			परत 0;
		जर (data->cache_size > size)
			परत shrink_cache(channel, data, size);
		परत grow_cache(channel, data, size);
	}
#endif
	परत EXT2_ET_INVALID_ARGUMENT;
}

#जर defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

स्थिर errcode_t unix_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना unix_private_data *data;
	पूर्णांक		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (channel->flags & CHANNEL_FLAGS_NODISCARD)
		जा unimplemented;

	जर (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(मोजणी) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#अन्यथा
		जा unimplemented;
#endif
	} अन्यथा {
#जर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(मोजणी) * channel->block_size);
#अन्यथा
		जा unimplemented;
#endif
	}
	जर (ret < 0) {
		जर (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			जा unimplemented;
		}
		परत त्रुटि_सं;
	}
	परत 0;
unimplemented:
	परत EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
स्थिर पूर्णांक __unix_zeroout(पूर्णांक fd, off_t offset, off_t len)
{
	पूर्णांक ret = -1;

#जर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	जर (ret == 0)
		परत 0;
#endif
#जर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	जर (ret == 0)
		परत 0;
#endif
	त्रुटि_सं = EOPNOTSUPP;
	परत ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#जर __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ढकला
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
स्थिर errcode_t unix_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना unix_private_data *data;
	पूर्णांक		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		संरचना stat statbuf;

		जर (मोजणी == 0)
			परत 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = ख_स्थिति(data->dev, &statbuf);
		जर (ret)
			जा err;
		जर ((अचिन्हित दीर्घ दीर्घ) statbuf.st_size <
			(block + मोजणी) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + मोजणी) * channel->block_size + data->offset);
			जर (ret)
				जा err;
		}
	}

	जर (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		जा unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(मोजणी) * channel->block_size);
err:
	जर (ret < 0) {
		जर (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			जा unimplemented;
		}
		परत त्रुटि_सं;
	}
	परत 0;
unimplemented:
	परत EXT2_ET_UNIMPLEMENTED;
}
#जर __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic काढा
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
