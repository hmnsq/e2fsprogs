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

#ଯଦି !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ifdef __linux__
#ଅନ୍ତର୍ଭାଵ <sys/utsname.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_RESOURCE_H
#ଅନ୍ତର୍ଭାଵ <sys/resource.h>
#endif
#ଯଦି HAVE_LINUX_FALLOC_H
#ଅନ୍ତର୍ଭାଵ <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#ଅନ୍ତର୍ଭାଵ <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#ଅନ୍ତର୍ଭାଵ <sys/file.h>
#endif

#ଯଦି defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_last_bit_set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା x)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 32;

	ଯଦି (!x)
		ଫେରନ୍ତୁ 0;
	ଯଦି (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	ଯଦି (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	ଯଦି (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	ଯଦି (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	ଯଦି (!(x & 0x80000000u)) {
		r -= 1;
	}
	ଫେରନ୍ତୁ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା highbit32(uint32_t v)
{
	ଫେରନ୍ତୁ find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)

ସଂରଚନା unix_cache {
	ଅକ୍ଷର			*buf;
	unsigned long long	block;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ସଂରଚନା unix_private_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	magic;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	dev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	align;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	access_time;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	unix_flock_flags;
	ext2_loff_t offset;
	ସଂରଚନା unix_cache *cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cache_size;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cache_hash_shift;
	ଶୂନ୍ୟ	*bounce;
	ସଂରଚନା struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
ସ୍ଥିର ଧାଡ଼ିଭିତରେ pthread_mutex_t *get_mutex(ସଂରଚନା unix_private_data *data,
					 kind_t kind)
{
	ଯଦି (data->flags & IO_FLAG_THREADS) {
		ବାଛ (kind) {
		ସ୍ଥିତି CACHE_MTX:
			ଫେରନ୍ତୁ &data->cache_mutex;
		ସ୍ଥିତି BOUNCE_MTX:
			ଫେରନ୍ତୁ &data->bounce_mutex;
		ସ୍ଥିତି STATS_MTX:
			ଫେରନ୍ତୁ &data->stats_mutex;
		}
	}
	ଫେରନ୍ତୁ ଖାଲି;
}
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mutex_lock(ସଂରଚନା unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	ଯଦି (mtx)
		pthread_mutex_lock(mtx);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mutex_unlock(ସଂରଚନା unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	ଯଦି (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

ସ୍ଥିର errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ସଂରଚନା unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	ଫେରନ୍ତୁ retval;
}

/*
 * Here are the raw I/O functions
 */
ସ୍ଥିର errcode_t raw_read_blk(io_channel channel,
			      ସଂରଚନା unix_private_data *data,
			      unsigned long long block,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		actual = 0;
	unsigned ଅକ୍ଷର	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		align_size, offset;

	size = (ଗଣନା_2 < 0) ? -ଗଣନା_2 : (ext2_loff_t) ଗଣନା_2 * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ଯଦି (data->flags & IO_FLAG_FORCE_BOUNCE)
		ଯାଅ bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	ଯଦି ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		ଯଦି (actual == size)
			ଫେରନ୍ତୁ 0;
		actual = 0;
	}
#ନଚେତଯଦି HAVE_PREAD
	/* Try an aligned pread */
	ଯଦି ((ଆକାର(off_t) >= ଆକାର(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		ଯଦି (actual == size)
			ଫେରନ୍ତୁ 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	ଯଦି ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		ଯଦି (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ଯାଅ error_unlock;
		}
		actual = ପଠ(data->dev, buf, size);
		ଯଦି (actual != size) {
		short_read:
			ଯଦି (actual < 0) {
				retval = errno;
				actual = 0;
			} ଅନ୍ୟଥା
				retval = EXT2_ET_SHORT_READ;
			ଯାଅ error_unlock;
		}
		ଯାଅ success_unlock;
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
	ଯଦି (channel->align == 0)
		channel->align = 1;
	ଯଦି ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	ଅନ୍ୟଥା
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	ଯଦି (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		ଯାଅ error_unlock;
	}
	ଯେତେବେଳେ (size > 0) {
		actual = ପଠ(data->dev, data->bounce, align_size);
		ଯଦି (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			ଯାଅ short_read;
		}
		ଯଦି ((actual + offset) > align_size)
			actual = align_size - offset;
		ଯଦି (actual > size)
			actual = size;
		memcpy(buf, (ଅକ୍ଷର *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	ଫେରନ୍ତୁ 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	ଯଦି (actual >= 0 && actual < size)
		memset((ଅକ୍ଷର *) buf+actual, 0, size-actual);
	ଯଦି (channel->read_error)
		retval = (channel->read_error)(channel, block, ଗଣନା_2, buf,
					       size, actual, retval);
	ଫେରନ୍ତୁ retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

ସ୍ଥିର errcode_t raw_write_blk(io_channel channel,
			       ସଂରଚନା unix_private_data *data,
			       unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *bufv,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		actual = 0;
	errcode_t	retval;
	ଅଚଳ unsigned ଅକ୍ଷର *buf = bufv;
	unsigned long long aligned_blk;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		align_size, offset;

	ଯଦି (ଗଣନା_2 == 1)
		size = channel->block_size;
	ଅନ୍ୟଥା {
		ଯଦି (ଗଣନା_2 < 0)
			size = -ଗଣନା_2;
		ଅନ୍ୟଥା
			size = (ext2_loff_t) ଗଣନା_2 * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	ଯଦି (data->flags & IO_FLAG_FORCE_BOUNCE)
		ଯାଅ bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	ଯଦି ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		ଯଦି (actual == size)
			ଫେରନ୍ତୁ 0;
	}
#ନଚେତଯଦି HAVE_PWRITE
	/* Try an aligned pwrite */
	ଯଦି ((ଆକାର(off_t) >= ଆକାର(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		ଯଦି (actual == size)
			ଫେରନ୍ତୁ 0;
	}
#endif /* HAVE_PWRITE */

	ଯଦି ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		ଯଦି (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		ଯଦି (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ଯାଅ error_unlock;
		}
		actual = ଲିଖ(data->dev, buf, size);
		ଯଦି (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		ଯଦି (actual < 0) {
			retval = errno;
			ଯାଅ error_out;
		}
		ଯଦି (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			ଯାଅ error_out;
		}
		ଫେରନ୍ତୁ 0;
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
	ଯଦି (channel->align == 0)
		channel->align = 1;
	ଯଦି ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	ଅନ୍ୟଥା
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	ଯେତେବେଳେ (size > 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା actual_w;

		ଯଦି (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		ଯଦି (size < align_size || offset) {
			ଯଦି (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				ଯାଅ error_unlock;
			}
			actual = ପଠ(data->dev, data->bounce,
				      align_size);
			ଯଦି (actual != align_size) {
				ଯଦି (actual < 0) {
					retval = errno;
					ଯାଅ error_unlock;
				}
				memset((ଅକ୍ଷର *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		ଯଦି ((actual + offset) > align_size)
			actual = align_size - offset;
		ଯଦି (actual > size)
			actual = size;
		memcpy(((ଅକ୍ଷର *)data->bounce) + offset, buf, actual);
		ଯଦି (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			ଯାଅ error_unlock;
		}
		actual_w = ଲିଖ(data->dev, data->bounce, align_size);
		ଯଦି (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		ଯଦି (actual_w < 0) {
			retval = errno;
			ଯାଅ error_out;
		}
		ଯଦି (actual_w != align_size)
			ଯାଅ short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	ଫେରନ୍ତୁ 0;

error_unlock:
	ଯଦି (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	ଯଦି (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, ଗଣନା_2, buf,
						size, actual, retval);
	ଫେରନ୍ତୁ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
ସ୍ଥିର errcode_t alloc_cache(io_channel channel,
			     ସଂରଚନା unix_private_data *data)
{
	errcode_t		retval;
	ସଂରଚନା unix_cache	*cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;

	data->access_time = 0;
	ପାଇଁ (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ଯଦି (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ଯଦି (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		ଯଦି (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	ଫେରନ୍ତୁ retval;
}

/* Free the cache buffers */
ସ୍ଥିର ଶୂନ୍ୟ free_cache(ସଂରଚନା unix_private_data *data)
{
	ସଂରଚନା unix_cache	*cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;

	data->access_time = 0;
	ପାଇଁ (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ଯଦି (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ଯଦି (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_ମୌଳିକ	0x9e37fffffffc0001UL
#ifndef CACHE_ଧାଡ଼ି_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_ଧାଡ଼ି_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cache_hash(ସଂରଚନା unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	ଯଦି (data->cache_size <= DEFAULT_CACHE_SIZE)
		ଫେରନ୍ତୁ 0;

	tmp = hashval ^ (GOLDEN_RATIO_ମୌଳିକ + hashval) / CACHE_ଧାଡ଼ି_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_ମୌଳିକ) >> data->cache_hash_shift);
	ଫେରନ୍ତୁ tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
ସ୍ଥିର ସଂରଚନା unix_cache *find_cached_block(ସଂରଚନା unix_private_data *data,
					    unsigned long long block,
					    ସଂରଚନା unix_cache **eldest)
{
	ସଂରଚନା unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		hash = cache_hash(data, block);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	ପାଇଁ (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		ଯଦି (!cache->in_use) {
			ଯଦି (!unused_cache)
				unused_cache = cache;
			ଜାରି;
		}
		ଯଦି (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ଫେରନ୍ତୁ cache;
		}
		ଯଦି (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	ପାଇଁ (i = 0, cache = data->cache; i < hash; i++, cache++) {
		ଯଦି (!cache->in_use) {
			ଯଦି (!unused_cache)
				unused_cache = cache;
			ଜାରି;
		}
		ଯଦି (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			ଫେରନ୍ତୁ cache;
		}
		ଯଦି (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	ଯଦି (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	ଫେରନ୍ତୁ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
ସ୍ଥିର errcode_t reuse_cache(io_channel channel,
		ସଂରଚନା unix_private_data *data, ସଂରଚନା unix_cache *cache,
		unsigned long long block)
{
	ଯଦି (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		ଯଦି (retval) {
			cache->write_err = 1;
			ଫେରନ୍ତୁ retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	ଫେରନ୍ତୁ 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
ସ୍ଥିର errcode_t flush_cached_blocks(io_channel channel,
				     ସଂରଚନା unix_private_data *data,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			errors_found = 0;

	ଯଦି ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	ପାଇଁ (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		ଯଦି (!cache->in_use)
			ଜାରି;
		ଯଦି (cache->dirty) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw_flags = RAW_WRITE_NO_HANDLER;

			ଯଦି (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			ଯଦି (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} ଅନ୍ୟଥା {
				cache->dirty = 0;
				cache->write_err = 0;
				ଯଦି (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} ଅନ୍ୟଥା ଯଦି (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	ଯଦି ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	ଯେତେବେଳେ (errors_found) {
		ଯଦି ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		ପାଇଁ (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			ଯଦି (!cache->in_use || !cache->write_err)
				ଜାରି;
			errors_found = 1;
			ଯଦି (cache->write_err && channel->write_error) {
				ଅକ୍ଷର *err_buf = ଖାଲି;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				ଯଦି (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ଖାଲି;
				ଅନ୍ୟଥା
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				ଯଦି (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				ଯାଅ retry;
			} ଅନ୍ୟଥା
				cache->write_err = 0;
		}
		ଯଦି ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	ଫେରନ୍ତୁ retval2;
}

/* Shrink the cache buffers */
ସ୍ଥିର errcode_t shrink_cache(io_channel channel,
			      ସଂରଚନା unix_private_data *data,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_size)
{
	ସଂରଚନା unix_cache	*cache, *new_cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ଯଦି (retval)
		ଯାଅ unlock;

	ପାଇଁ (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		ଯଦି (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * ଆକାର(ସଂରଚନା unix_cache));
	ଯଦି (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		ଯାଅ unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ retval;
}

/* Grow the cache buffers */
ସ୍ଥିର errcode_t grow_cache(io_channel channel,
			    ସଂରଚନା unix_private_data *data,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_size)
{
	ସଂରଚନା unix_cache	*cache, *new_cache;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ଯଦି (retval)
		ଯାଅ unlock;

	new_cache = realloc(data->cache, new_size * ଆକାର(ସଂରଚନା unix_cache));
	ଯଦି (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		ଯାଅ unlock;
	}
	data->cache = new_cache;

	ପାଇଁ (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ଯଦି (retval) {
			ଯେତେବେଳେ (i > data->cache_size) {
				cache--; i--;
				ଯଦି (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			ଯାଅ unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_open_file(ଅଚଳ ଅକ୍ଷର *pathname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, mode_t mode)
{
	ଯଦି (mode)
#ଯଦି defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		ଫେରନ୍ତୁ open64(pathname, flags, mode);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open64(pathname, flags);
#ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open(pathname, flags, mode);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ open(pathname, flags);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_stat(ଅଚଳ ଅକ୍ଷର *path, ext2fs_struct_stat *buf)
{
#ଯଦି defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ଫେରନ୍ତୁ stat64(path, buf);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ stat(path, buf);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_fstat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2fs_struct_stat *buf)
{
#ଯଦି defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	ଫେରନ୍ତୁ fstat64(fd, buf);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
ସ୍ଥିର errcode_t unix_funlock(io_channel channel)
{
	ସଂରଚନା unix_private_data *data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		ଯଦି (ret)
			ଫେରନ୍ତୁ errno;

		data->unix_flock_flags = 0;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t unix_flock(io_channel channel, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flock_flags)
{
	ସଂରଚନା unix_private_data *data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ଯଦି (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	ଯଦି (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	ଯଦି (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	ଯଦି (!unix_flock_flags)
		ଫେରନ୍ତୁ 0;

	ret = flock(data->dev, unix_flock_flags);
	ଯଦି (ret < 0)
		ଫେରନ୍ତୁ errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	ଫେରନ୍ତୁ 0;
}
#ଅନ୍ୟଥା
#define unix_flock		ଖାଲି

ସ୍ଥିର errcode_t unix_funlock(io_channel channel)
{
	ଫେରନ୍ତୁ 0;
}
#endif /* HAVE_SYS_FILE_H */

ସ୍ଥିର errcode_t unix_open_channel(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ଖାଲି;
	ସଂରଚନା unix_private_data *data = ଖାଲି;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	ସଂରଚନା		utsname ut;
#endif

	ଯଦି (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(ଶୂନ୍ୟ) fsync(fd);
#endif

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_io_channel), &io);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(io, 0, ଆକାର(ସଂରଚନା struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା unix_private_data), &data);
	ଯଦି (retval)
		ଯାଅ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ଯଦି (retval)
		ଯାଅ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	ଯଦି (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, ଆକାର(ସଂରଚନା unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.ଅଙ୍କ_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, ଆକାର(ସଂରଚନା unix_cache));
	ଯଦି (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		ଯାଅ cleanup;
	}

#ଯଦି defined(O_DIRECT)
	ଯଦି (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#ନଚେତଯଦି defined(F_NOCACHE)
	ଯଦି (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ଯଦି (ext2fs_fstat(data->dev, &st) == 0) {
		ଯଦି (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା zeroes = 0;

			ଯଦି (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} ଅନ୍ୟଥା {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#ଯଦି defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	ଯଦି (!io->align)
		io->align = 512;
#endif

#ଯଦି defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ଯଦି (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା dio_align = ext2fs_get_dio_alignment(fd);

		ଯଦି (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	ଯଦି ((retval = alloc_cache(io, data)))
		ଯାଅ cleanup;

#ifdef BLKROGET
	ଯଦି (flags & IO_FLAG_RW) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		ଯଦି (!error && readonly) {
			retval = EPERM;
			ଯାଅ cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#ଯଦି (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#ଅନ୍ୟଥା
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	ଯଦି ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		ସଂରଚନା rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		ଯଦି (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	ଯଦି (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, ଖାଲି);
		ଯଦି (retval)
			ଯାଅ cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, ଖାଲି);
		ଯଦି (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			ଯାଅ cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, ଖାଲି);
		ଯଦି (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			ଯାଅ cleanup;
		}
	}
#endif
	*channel = io;
	ଫେରନ୍ତୁ 0;

cleanup:
	ଯଦି (data) {
		unix_funlock(io);
		ଯଦି (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		ଯଦି (data->cache) {
			free_cache(data);
			ମୁକ୍ତ(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	ଯଦି (io) {
		ଯଦି (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t unixfd_open(ଅଚଳ ଅକ୍ଷର *str_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			     io_channel *channel)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_flags;

	fd = atoi(str_fd);
#ଯଦି defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	ଯଦି (fd_flags == -1)
		ଫେରନ୍ତୁ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	ଯଦି (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#ଯଦି defined(O_DIRECT)
	ଯଦି (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	ଫେରନ୍ତୁ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

ସ୍ଥିର errcode_t unix_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			   io_channel *channel)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags;

	ଯଦି (name == 0)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ଯଦି (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ଯଦି defined(O_DIRECT)
	ଯଦି (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ errno;
#ଯଦି defined(F_NOCACHE) && !defined(IO_DIRECT)
	ଯଦି (flags & IO_FLAG_DIRECT_IO) {
		ଯଦି (fcntl(fd, F_NOCACHE, 1) < 0)
			ଫେରନ୍ତୁ errno;
	}
#endif
	ଫେରନ୍ତୁ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

ସ୍ଥିର errcode_t unix_close(io_channel channel)
{
	ସଂରଚନା unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
		ଫେରନ୍ତୁ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	ଯଦି (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	ମୁକ୍ତ(data->cache);
#ifdef HAVE_PTHREAD
	ଯଦି (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	ଯଦି (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t unix_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	ସଂରଚନା unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		ଯଦି ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			ଫେରନ୍ତୁ retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା unix_private_data *data;
	ସଂରଚନା unix_cache *cache;
	errcode_t	retval;
	ଅକ୍ଷର		*cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ଫେରନ୍ତୁ raw_read_blk(channel, data, block, ଗଣନା_2, buf);
#ଅନ୍ୟଥା
	ଯଦି (data->flags & IO_FLAG_NOCACHE)
		ଫେରନ୍ତୁ raw_read_blk(channel, data, block, ଗଣନା_2, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ଯଦି (ଗଣନା_2 < 0 || ଗଣନା_2 > WRITE_DIRECT_SIZE) {
		ଯଦି ((retval = flush_cached_blocks(channel, data, 0)))
			ଫେରନ୍ତୁ retval;
		ଫେରନ୍ତୁ raw_read_blk(channel, data, block, ଗଣନା_2, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ଯେତେବେଳେ (ଗଣନା_2 > 0) {
		/* If it's in the cache, use it! */
		ଯଦି ((cache = find_cached_block(data, block, ଖାଲି))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			ଗଣନା_2--;
			block++;
			cp += channel->block_size;
			ଜାରି;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		ପାଇଁ (i=1; i < ଗଣନା_2; i++)
			ଯଦି (find_cached_block(data, block+i, ଖାଲି))
				ଭାଙ୍ଗ;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		ଯଦି ((retval = raw_read_blk(channel, data, block, i, cp)))
			ଫେରନ୍ତୁ retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		ପାଇଁ (j=0; j < i; j++) {
			ଯଦି (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				ଯଦି (retval)
					ଯାଅ call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			ଗଣନା_2--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ 0;

call_write_handler:
	ଯଦି (cache->write_err && channel->write_error) {
		ଅକ୍ଷର *err_buf = ଖାଲି;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		ଯଦି (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ଖାଲି;
		ଅନ୍ୟଥା
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		ଯଦି (err_buf)
			ext2fs_free_mem(&err_buf);
	} ଅନ୍ୟଥା
		mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ retval;
#endif /* NO_IO_CACHE */
}

ସ୍ଥିର errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ unix_read_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା unix_private_data *data;
	ସଂରଚନା unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ଅଚଳ ଅକ୍ଷର	*cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	ଫେରନ୍ତୁ raw_write_blk(channel, data, block, ଗଣନା_2, buf, 0);
#ଅନ୍ୟଥା
	ଯଦି (data->flags & IO_FLAG_NOCACHE)
		ଫେରନ୍ତୁ raw_write_blk(channel, data, block, ଗଣନା_2, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ଯଦି (ଗଣନା_2 < 0 || ଗଣନା_2 > WRITE_DIRECT_SIZE) {
		ଯଦି ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			ଫେରନ୍ତୁ retval;
		ଫେରନ୍ତୁ raw_write_blk(channel, data, block, ଗଣନା_2, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ଯଦି (writethrough)
		retval = raw_write_blk(channel, data, block, ଗଣନା_2, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ଯେତେବେଳେ (ଗଣନା_2 > 0) {
		cache = find_cached_block(data, block, &reuse);
		ଯଦି (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			ଯଦି (err)
				ଯାଅ call_write_handler;
		}
		ଯଦି (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		ଗଣନା_2--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ retval;

call_write_handler:
	ଯଦି (cache->write_err && channel->write_error) {
		ଅକ୍ଷର *err_buf = ଖାଲି;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		ଯଦି (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ଖାଲି;
		ଅନ୍ୟଥା
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		ଯଦି (err_buf)
			ext2fs_free_mem(&err_buf);
	} ଅନ୍ୟଥା
		mutex_unlock(data, CACHE_MTX);
	ଫେରନ୍ତୁ retval;
#endif /* NO_IO_CACHE */
}

ସ୍ଥିର errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ଗଣନା_2)
{
#ifdef POSIX_FADV_WILLNEED
	ସଂରଚନା unix_private_data *data;

	data = (ସଂରଚନା unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	ଫେରନ୍ତୁ posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)ଗଣନା_2 * channel->block_size,
			     POSIX_FADV_WILLNEED);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

ସ୍ଥିର errcode_t unix_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ unix_write_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	ଯଦି ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		ଫେରନ୍ତୁ retval;
#endif

	ଯଦି (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		ଫେରନ୍ତୁ errno;

	actual = ଲିଖ(data->dev, buf, size);
	ଯଦି (actual < 0)
		ଫେରନ୍ତୁ errno;
	ଯଦି (actual != size)
		ଫେରନ୍ତୁ EXT2_ET_SHORT_WRITE;

	ଫେରନ୍ତୁ 0;
}

/*
 * Flush data buffers to disk.
 */
ସ୍ଥିର errcode_t unix_flush(io_channel channel)
{
	ସଂରଚନା unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	ଯଦି (!retval && fsync(data->dev) != 0)
		ଫେରନ୍ତୁ errno;
#endif
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t unix_set_option(io_channel channel, ଅଚଳ ଅକ୍ଷର *option,
				 ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	ଅକ୍ଷର *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (!strcmp(option, "offset")) {
		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		ଯଦି (*end)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		ଯଦି (data->offset < 0)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(option, "cache")) {
		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		ଯଦି (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			ଫେରନ୍ତୁ retval;
		}
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	ଯଦି (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, ଖାଲି, 0);
		ଯଦି (errno || size == 0 || size > INT32_MAX)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		ଯଦି (data->cache_size == size)
			ଫେରନ୍ତୁ 0;
		ଯଦି (data->cache_size > size)
			ଫେରନ୍ତୁ shrink_cache(channel, data, size);
		ଫେରନ୍ତୁ grow_cache(channel, data, size);
	}
#endif
	ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
}

#ଯଦି defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

ସ୍ଥିର errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା unix_private_data *data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (channel->flags & CHANNEL_FLAGS_NODISCARD)
		ଯାଅ unimplemented;

	ଯଦି (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(ଗଣନା_2) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#ଅନ୍ୟଥା
		ଯାଅ unimplemented;
#endif
	} ଅନ୍ୟଥା {
#ଯଦି defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(ଗଣନା_2) * channel->block_size);
#ଅନ୍ୟଥା
		ଯାଅ unimplemented;
#endif
	}
	ଯଦି (ret < 0) {
		ଯଦି (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			ଯାଅ unimplemented;
		}
		ଫେରନ୍ତୁ errno;
	}
	ଫେରନ୍ତୁ 0;
unimplemented:
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __unix_zeroout(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t offset, off_t len)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

#ଯଦି defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	ଯଦି (ret == 0)
		ଫେରନ୍ତୁ 0;
#endif
#ଯଦି defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	ଯଦି (ret == 0)
		ଫେରନ୍ତୁ 0;
#endif
	errno = EOPNOTSUPP;
	ଫେରନ୍ତୁ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ଠେଲ
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
ସ୍ଥିର errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା unix_private_data *data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		ସଂରଚନା stat statbuf;

		ଯଦି (ଗଣନା_2 == 0)
			ଫେରନ୍ତୁ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		ଯଦି (ret)
			ଯାଅ err;
		ଯଦି ((unsigned long long) statbuf.st_size <
			(block + ଗଣନା_2) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + ଗଣନା_2) * channel->block_size + data->offset);
			ଯଦି (ret)
				ଯାଅ err;
		}
	}

	ଯଦି (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		ଯାଅ unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(ଗଣନା_2) * channel->block_size);
err:
	ଯଦି (ret < 0) {
		ଯଦି (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			ଯାଅ unimplemented;
		}
		ଫେରନ୍ତୁ errno;
	}
	ଫେରନ୍ତୁ 0;
unimplemented:
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}
#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic କାଢ଼
#endif

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_unix_manager = {
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

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_unixfd_manager = {
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
