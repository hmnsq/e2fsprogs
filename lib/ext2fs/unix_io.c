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

#എങ്കിൽ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#ifdef __linux__
#അന്തര്ഭാവ <sys/utsname.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#അന്തര്ഭാവ <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#അന്തര്ഭാവ <sys/mount.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_RESOURCE_H
#അന്തര്ഭാവ <sys/resource.h>
#endif
#എങ്കിൽ HAVE_LINUX_FALLOC_H
#അന്തര്ഭാവ <linux/falloc.h>
#endif
#ifdef HAVE_PTHREAD
#അന്തര്ഭാവ <pthread.h>
#endif
#ifdef HAVE_SYS_FILE_H
#അന്തര്ഭാവ <sys/file.h>
#endif

#എങ്കിൽ defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ find_last_bit_set(പൂർണ്ണസംഖ്യ x)
{
	പൂർണ്ണസംഖ്യ r = 32;

	എങ്കിൽ (!x)
		തിരികെ 0;
	എങ്കിൽ (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	എങ്കിൽ (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	എങ്കിൽ (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	എങ്കിൽ (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	എങ്കിൽ (!(x & 0x80000000u)) {
		r -= 1;
	}
	തിരികെ r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ highbit32(uint32_t v)
{
	തിരികെ find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

ഘടന unix_cache {
	അക്ഷരം			*buf;
	unsigned long long	block;
	പൂർണ്ണസംഖ്യ			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
	unsigned		write_err:1;
};

#define DEFAULT_CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ഘടന unix_private_data {
	പൂർണ്ണസംഖ്യ	magic;
	പൂർണ്ണസംഖ്യ	dev;
	പൂർണ്ണസംഖ്യ	flags;
	പൂർണ്ണസംഖ്യ	align;
	പൂർണ്ണസംഖ്യ	access_time;
	പൂർണ്ണസംഖ്യ	unix_flock_flags;
	ext2_loff_t offset;
	ഘടന unix_cache *cache;
	unsigned പൂർണ്ണസംഖ്യ cache_size;
	unsigned പൂർണ്ണസംഖ്യ cache_hash_shift;
	ശൂന്യം	*bounce;
	ഘടന struct_io_stats io_stats;
#ifdef HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#endif
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

തരനിർവചനം എണ്ണൽ lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#ifdef HAVE_PTHREAD
സ്ഥിതം വരിയിൽ pthread_mutex_t *get_mutex(ഘടന unix_private_data *data,
					 kind_t kind)
{
	എങ്കിൽ (data->flags & IO_FLAG_THREADS) {
		തിരഞ്ഞെടുപ്പ് (kind) {
		അവസ്ഥ CACHE_MTX:
			തിരികെ &data->cache_mutex;
		അവസ്ഥ BOUNCE_MTX:
			തിരികെ &data->bounce_mutex;
		അവസ്ഥ STATS_MTX:
			തിരികെ &data->stats_mutex;
		}
	}
	തിരികെ ശൂന്യ;
}
#endif

സ്ഥിതം വരിയിൽ ശൂന്യം mutex_lock(ഘടന unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	എങ്കിൽ (mtx)
		pthread_mutex_lock(mtx);
#endif
}

സ്ഥിതം വരിയിൽ ശൂന്യം mutex_unlock(ഘടന unix_private_data *data, kind_t kind)
{
#ifdef HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(data,kind);

	എങ്കിൽ (mtx)
		pthread_mutex_unlock(mtx);
#endif
}

സ്ഥിതം errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ഘടന unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (stats) {
		mutex_lock(data, STATS_MTX);
		*stats = &data->io_stats;
		mutex_unlock(data, STATS_MTX);
	}

	തിരികെ retval;
}

/*
 * Here are the raw I/O functions
 */
സ്ഥിതം errcode_t raw_read_blk(io_channel channel,
			      ഘടന unix_private_data *data,
			      unsigned long long block,
			      പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	പൂർണ്ണസംഖ്യ		actual = 0;
	unsigned അക്ഷരം	*buf = bufv;
	ssize_t		really_read = 0;
	unsigned long long aligned_blk;
	പൂർണ്ണസംഖ്യ		align_size, offset;

	size = (എണ്ണം < 0) ? -എണ്ണം : (ext2_loff_t) എണ്ണം * channel->block_size;
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_read += size;
	mutex_unlock(data, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	എങ്കിൽ (data->flags & IO_FLAG_FORCE_BOUNCE)
		പോകുക bounce_read;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	എങ്കിൽ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		എങ്കിൽ (actual == size)
			തിരികെ 0;
		actual = 0;
	}
#അല്ലെങ്കിലെങ്കിൽ HAVE_PREAD
	/* Try an aligned pread */
	എങ്കിൽ ((വലുപ്പം(off_t) >= വലുപ്പം(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		എങ്കിൽ (actual == size)
			തിരികെ 0;
		actual = 0;
	}
#endif /* HAVE_PREAD */

	എങ്കിൽ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		mutex_lock(data, BOUNCE_MTX);
		എങ്കിൽ (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			പോകുക error_unlock;
		}
		actual = പഠ(data->dev, buf, size);
		എങ്കിൽ (actual != size) {
		short_read:
			എങ്കിൽ (actual < 0) {
				retval = errno;
				actual = 0;
			} അല്ലെങ്കിൽ
				retval = EXT2_ET_SHORT_READ;
			പോകുക error_unlock;
		}
		പോകുക success_unlock;
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
	എങ്കിൽ (channel->align == 0)
		channel->align = 1;
	എങ്കിൽ ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	അല്ലെങ്കിൽ
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	mutex_lock(data, BOUNCE_MTX);
	എങ്കിൽ (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		പോകുക error_unlock;
	}
	ആകുമ്പോൾ (size > 0) {
		actual = പഠ(data->dev, data->bounce, align_size);
		എങ്കിൽ (actual != align_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			പോകുക short_read;
		}
		എങ്കിൽ ((actual + offset) > align_size)
			actual = align_size - offset;
		എങ്കിൽ (actual > size)
			actual = size;
		memcpy(buf, (അക്ഷരം *)data->bounce + offset, actual);

		really_read += actual;
		size -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	തിരികെ 0;

error_unlock:
	mutex_unlock(data, BOUNCE_MTX);
	എങ്കിൽ (actual >= 0 && actual < size)
		memset((അക്ഷരം *) buf+actual, 0, size-actual);
	എങ്കിൽ (channel->read_error)
		retval = (channel->read_error)(channel, block, എണ്ണം, buf,
					       size, actual, retval);
	തിരികെ retval;
}

#define RAW_WRITE_NO_HANDLER	(1U << 0)
#define RAW_WRITE_NOLOCK	(1U << 1)

സ്ഥിതം errcode_t raw_write_blk(io_channel channel,
			       ഘടന unix_private_data *data,
			       unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *bufv,
			       പൂർണ്ണസംഖ്യ flags)
{
	ssize_t		size;
	ext2_loff_t	location;
	പൂർണ്ണസംഖ്യ		actual = 0;
	errcode_t	retval;
	സ്ഥിരം unsigned അക്ഷരം *buf = bufv;
	unsigned long long aligned_blk;
	പൂർണ്ണസംഖ്യ		align_size, offset;

	എങ്കിൽ (എണ്ണം == 1)
		size = channel->block_size;
	അല്ലെങ്കിൽ {
		എങ്കിൽ (എണ്ണം < 0)
			size = -എണ്ണം;
		അല്ലെങ്കിൽ
			size = (ext2_loff_t) എണ്ണം * channel->block_size;
	}
	mutex_lock(data, STATS_MTX);
	data->io_stats.bytes_written += size;
	mutex_unlock(data, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	എങ്കിൽ (data->flags & IO_FLAG_FORCE_BOUNCE)
		പോകുക bounce_write;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	എങ്കിൽ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		എങ്കിൽ (actual == size)
			തിരികെ 0;
	}
#അല്ലെങ്കിലെങ്കിൽ HAVE_PWRITE
	/* Try an aligned pwrite */
	എങ്കിൽ ((വലുപ്പം(off_t) >= വലുപ്പം(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		എങ്കിൽ (actual == size)
			തിരികെ 0;
	}
#endif /* HAVE_PWRITE */

	എങ്കിൽ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		എങ്കിൽ (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		എങ്കിൽ (ext2fs_llseek(data->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			പോകുക error_unlock;
		}
		actual = ലിഖ(data->dev, buf, size);
		എങ്കിൽ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		എങ്കിൽ (actual < 0) {
			retval = errno;
			പോകുക error_out;
		}
		എങ്കിൽ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			പോകുക error_out;
		}
		തിരികെ 0;
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
	എങ്കിൽ (channel->align == 0)
		channel->align = 1;
	എങ്കിൽ ((channel->block_size > channel->align) &&
	    (channel->block_size % channel->align) == 0)
		align_size = channel->block_size;
	അല്ലെങ്കിൽ
		align_size = channel->align;
	aligned_blk = location / align_size;
	offset = location % align_size;

	ആകുമ്പോൾ (size > 0) {
		പൂർണ്ണസംഖ്യ actual_w;

		എങ്കിൽ (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(data, BOUNCE_MTX);
		എങ്കിൽ (size < align_size || offset) {
			എങ്കിൽ (ext2fs_llseek(data->dev, aligned_blk * align_size,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				പോകുക error_unlock;
			}
			actual = പഠ(data->dev, data->bounce,
				      align_size);
			എങ്കിൽ (actual != align_size) {
				എങ്കിൽ (actual < 0) {
					retval = errno;
					പോകുക error_unlock;
				}
				memset((അക്ഷരം *) data->bounce + actual, 0,
				       align_size - actual);
			}
		}
		actual = size;
		എങ്കിൽ ((actual + offset) > align_size)
			actual = align_size - offset;
		എങ്കിൽ (actual > size)
			actual = size;
		memcpy(((അക്ഷരം *)data->bounce) + offset, buf, actual);
		എങ്കിൽ (ext2fs_llseek(data->dev, aligned_blk * align_size, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			പോകുക error_unlock;
		}
		actual_w = ലിഖ(data->dev, data->bounce, align_size);
		എങ്കിൽ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(data, BOUNCE_MTX);
		എങ്കിൽ (actual_w < 0) {
			retval = errno;
			പോകുക error_out;
		}
		എങ്കിൽ (actual_w != align_size)
			പോകുക short_write;
		size -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	തിരികെ 0;

error_unlock:
	എങ്കിൽ (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(data, BOUNCE_MTX);
error_out:
	എങ്കിൽ (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, എണ്ണം, buf,
						size, actual, retval);
	തിരികെ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
സ്ഥിതം errcode_t alloc_cache(io_channel channel,
			     ഘടന unix_private_data *data)
{
	errcode_t		retval;
	ഘടന unix_cache	*cache;
	unsigned പൂർണ്ണസംഖ്യ		i;

	data->access_time = 0;
	വേണ്ടി (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		എങ്കിൽ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	എങ്കിൽ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		എങ്കിൽ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	തിരികെ retval;
}

/* Free the cache buffers */
സ്ഥിതം ശൂന്യം free_cache(ഘടന unix_private_data *data)
{
	ഘടന unix_cache	*cache;
	unsigned പൂർണ്ണസംഖ്യ		i;

	data->access_time = 0;
	വേണ്ടി (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		എങ്കിൽ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	എങ്കിൽ (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_അഭാജ്യം	0x9e37fffffffc0001UL
#ifndef CACHE_വരി_SIZE
/* if the system didn't tell us, guess something reasonable */
#define CACHE_വരി_SIZE		64
#endif

/* buffer cache hashing function, crudely stolen from xfsprogs */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
cache_hash(ഘടന unix_private_data *data, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	എങ്കിൽ (data->cache_size <= DEFAULT_CACHE_SIZE)
		തിരികെ 0;

	tmp = hashval ^ (GOLDEN_RATIO_അഭാജ്യം + hashval) / CACHE_വരി_SIZE;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_അഭാജ്യം) >> data->cache_hash_shift);
	തിരികെ tmp % data->cache_size;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
സ്ഥിതം ഘടന unix_cache *find_cached_block(ഘടന unix_private_data *data,
					    unsigned long long block,
					    ഘടന unix_cache **eldest)
{
	ഘടന unix_cache	*cache, *unused_cache, *oldest_cache;
	unsigned പൂർണ്ണസംഖ്യ		hash = cache_hash(data, block);
	unsigned പൂർണ്ണസംഖ്യ		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	വേണ്ടി (i = hash, cache = data->cache + hash;
	     i < data->cache_size;
	     i++, cache++) {
		എങ്കിൽ (!cache->in_use) {
			എങ്കിൽ (!unused_cache)
				unused_cache = cache;
			തുടരുക;
		}
		എങ്കിൽ (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			തിരികെ cache;
		}
		എങ്കിൽ (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	വേണ്ടി (i = 0, cache = data->cache; i < hash; i++, cache++) {
		എങ്കിൽ (!cache->in_use) {
			എങ്കിൽ (!unused_cache)
				unused_cache = cache;
			തുടരുക;
		}
		എങ്കിൽ (cache->block == block) {
			cache->access_time = ++data->access_time;
			data->io_stats.cache_hits++;
			തിരികെ cache;
		}
		എങ്കിൽ (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	എങ്കിൽ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	data->io_stats.cache_misses++;
	തിരികെ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
സ്ഥിതം errcode_t reuse_cache(io_channel channel,
		ഘടന unix_private_data *data, ഘടന unix_cache *cache,
		unsigned long long block)
{
	എങ്കിൽ (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, data, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		എങ്കിൽ (retval) {
			cache->write_err = 1;
			തിരികെ retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
	തിരികെ 0;
}

#define FLUSH_INVALIDATE	0x01
#define FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
സ്ഥിതം errcode_t flush_cached_blocks(io_channel channel,
				     ഘടന unix_private_data *data,
				     പൂർണ്ണസംഖ്യ flags)
{
	ഘടന unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	unsigned പൂർണ്ണസംഖ്യ		i;
	പൂർണ്ണസംഖ്യ			errors_found = 0;

	എങ്കിൽ ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(data, CACHE_MTX);
	വേണ്ടി (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
		എങ്കിൽ (!cache->in_use)
			തുടരുക;
		എങ്കിൽ (cache->dirty) {
			പൂർണ്ണസംഖ്യ raw_flags = RAW_WRITE_NO_HANDLER;

			എങ്കിൽ (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, data,
					       cache->block, 1, cache->buf,
					       raw_flags);
			എങ്കിൽ (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} അല്ലെങ്കിൽ {
				cache->dirty = 0;
				cache->write_err = 0;
				എങ്കിൽ (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	എങ്കിൽ ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(data, CACHE_MTX);
retry:
	ആകുമ്പോൾ (errors_found) {
		എങ്കിൽ ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(data, CACHE_MTX);
		errors_found = 0;
		വേണ്ടി (i=0, cache = data->cache; i < data->cache_size; i++, cache++) {
			എങ്കിൽ (!cache->in_use || !cache->write_err)
				തുടരുക;
			errors_found = 1;
			എങ്കിൽ (cache->write_err && channel->write_error) {
				അക്ഷരം *err_buf = ശൂന്യ;
				unsigned long long err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				എങ്കിൽ (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ശൂന്യ;
				അല്ലെങ്കിൽ
					memcpy(err_buf, cache->buf,
					       channel->block_size);
				mutex_unlock(data, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_size, -1,
					retval2);
				എങ്കിൽ (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(data, CACHE_MTX);
				പോകുക retry;
			} അല്ലെങ്കിൽ
				cache->write_err = 0;
		}
		എങ്കിൽ ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(data, CACHE_MTX);
	}
	തിരികെ retval2;
}

/* Shrink the cache buffers */
സ്ഥിതം errcode_t shrink_cache(io_channel channel,
			      ഘടന unix_private_data *data,
			      unsigned പൂർണ്ണസംഖ്യ new_size)
{
	ഘടന unix_cache	*cache, *new_cache;
	unsigned പൂർണ്ണസംഖ്യ		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	എങ്കിൽ (retval)
		പോകുക unlock;

	വേണ്ടി (i = new_size, cache = data->cache + new_size;
	     i < data->cache_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		എങ്കിൽ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(data->cache, new_size * വലുപ്പം(ഘടന unix_cache));
	എങ്കിൽ (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		പോകുക unlock;
	}

	data->cache = new_cache;
	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	തിരികെ retval;
}

/* Grow the cache buffers */
സ്ഥിതം errcode_t grow_cache(io_channel channel,
			    ഘടന unix_private_data *data,
			    unsigned പൂർണ്ണസംഖ്യ new_size)
{
	ഘടന unix_cache	*cache, *new_cache;
	unsigned പൂർണ്ണസംഖ്യ		i;
	errcode_t		retval;

	mutex_lock(data, CACHE_MTX);

	retval = flush_cached_blocks(channel, data,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	എങ്കിൽ (retval)
		പോകുക unlock;

	new_cache = realloc(data->cache, new_size * വലുപ്പം(ഘടന unix_cache));
	എങ്കിൽ (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		പോകുക unlock;
	}
	data->cache = new_cache;

	വേണ്ടി (i = data->cache_size, cache = new_cache + data->cache_size;
	     i < new_size;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		എങ്കിൽ (retval) {
			ആകുമ്പോൾ (i > data->cache_size) {
				cache--; i--;
				എങ്കിൽ (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			പോകുക unlock;
		}
	}

	data->cache_size = new_size;
	data->cache_hash_shift = highbit32(data->cache_size);

unlock:
	mutex_unlock(data, CACHE_MTX);
	തിരികെ retval;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

പൂർണ്ണസംഖ്യ ext2fs_open_file(സ്ഥിരം അക്ഷരം *pathname, പൂർണ്ണസംഖ്യ flags, mode_t mode)
{
	എങ്കിൽ (mode)
#എങ്കിൽ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		തിരികെ open64(pathname, flags, mode);
	അല്ലെങ്കിൽ
		തിരികെ open64(pathname, flags);
#അല്ലെങ്കിൽ
		തിരികെ open(pathname, flags, mode);
	അല്ലെങ്കിൽ
		തിരികെ open(pathname, flags);
#endif
}

പൂർണ്ണസംഖ്യ ext2fs_stat(സ്ഥിരം അക്ഷരം *path, ext2fs_struct_stat *buf)
{
#എങ്കിൽ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	തിരികെ stat64(path, buf);
#അല്ലെങ്കിൽ
	തിരികെ stat(path, buf);
#endif
}

പൂർണ്ണസംഖ്യ ext2fs_fstat(പൂർണ്ണസംഖ്യ fd, ext2fs_struct_stat *buf)
{
#എങ്കിൽ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	തിരികെ fstat64(fd, buf);
#അല്ലെങ്കിൽ
	തിരികെ fstat(fd, buf);
#endif
}

#ifdef HAVE_SYS_FILE_H
സ്ഥിതം errcode_t unix_funlock(io_channel channel)
{
	ഘടന unix_private_data *data;
	പൂർണ്ണസംഖ്യ ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (data->unix_flock_flags) {
		ret = flock(data->dev, LOCK_UN);
		എങ്കിൽ (ret)
			തിരികെ errno;

		data->unix_flock_flags = 0;
	}

	തിരികെ 0;
}

സ്ഥിതം errcode_t unix_flock(io_channel channel, unsigned പൂർണ്ണസംഖ്യ flock_flags)
{
	ഘടന unix_private_data *data;
	പൂർണ്ണസംഖ്യ unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	എങ്കിൽ (ret)
		തിരികെ ret;

	എങ്കിൽ (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	എങ്കിൽ (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	എങ്കിൽ (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	എങ്കിൽ (!unix_flock_flags)
		തിരികെ 0;

	ret = flock(data->dev, unix_flock_flags);
	എങ്കിൽ (ret < 0)
		തിരികെ errno;

	data->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	തിരികെ 0;
}
#അല്ലെങ്കിൽ
#define unix_flock		ശൂന്യ

സ്ഥിതം errcode_t unix_funlock(io_channel channel)
{
	തിരികെ 0;
}
#endif /* HAVE_SYS_FILE_H */

സ്ഥിതം errcode_t unix_open_channel(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ fd,
				   പൂർണ്ണസംഖ്യ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ശൂന്യ;
	ഘടന unix_private_data *data = ശൂന്യ;
	errcode_t	retval;
	ext2fs_struct_stat st;
#ifdef __linux__
	ഘടന		utsname ut;
#endif

	എങ്കിൽ (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#ifdef __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(ശൂന്യം) fsync(fd);
#endif

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_io_channel), &io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(io, 0, വലുപ്പം(ഘടന struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന unix_private_data), &data);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	എങ്കിൽ (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(data, 0, വലുപ്പം(ഘടന unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.അക്കം_fields = 4;
	data->flags = flags;
	data->dev = fd;

	data->cache_size = DEFAULT_CACHE_SIZE;
	data->cache_hash_shift = highbit32(data->cache_size);
	data->cache = calloc(DEFAULT_CACHE_SIZE, വലുപ്പം(ഘടന unix_cache));
	എങ്കിൽ (!data->cache) {
		retval = EXT2_ET_NO_MEMORY;
		പോകുക cleanup;
	}

#എങ്കിൽ defined(O_DIRECT)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#അല്ലെങ്കിലെങ്കിൽ defined(F_NOCACHE)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	എങ്കിൽ (ext2fs_fstat(data->dev, &st) == 0) {
		എങ്കിൽ (ext2fsP_is_disk_device(st.st_mode)) {
#ifdef BLKDISCARDZEROES
			പൂർണ്ണസംഖ്യ zeroes = 0;

			എങ്കിൽ (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#endif
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} അല്ലെങ്കിൽ {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#എങ്കിൽ defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	എങ്കിൽ (!io->align)
		io->align = 512;
#endif

#എങ്കിൽ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	എങ്കിൽ (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		പൂർണ്ണസംഖ്യ dio_align = ext2fs_get_dio_alignment(fd);

		എങ്കിൽ (io->align < dio_align)
			io->align = dio_align;
	}
#endif

	എങ്കിൽ ((retval = alloc_cache(io, data)))
		പോകുക cleanup;

#ifdef BLKROGET
	എങ്കിൽ (flags & IO_FLAG_RW) {
		പൂർണ്ണസംഖ്യ error;
		പൂർണ്ണസംഖ്യ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		എങ്കിൽ (!error && readonly) {
			retval = EPERM;
			പോകുക cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#എങ്കിൽ (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#അല്ലെങ്കിൽ
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	എങ്കിൽ ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(data->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		ഘടന rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		എങ്കിൽ (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
#ifdef HAVE_PTHREAD
	എങ്കിൽ (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&data->cache_mutex, ശൂന്യ);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		retval = pthread_mutex_init(&data->bounce_mutex, ശൂന്യ);
		എങ്കിൽ (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			പോകുക cleanup;
		}
		retval = pthread_mutex_init(&data->stats_mutex, ശൂന്യ);
		എങ്കിൽ (retval) {
			pthread_mutex_destroy(&data->cache_mutex);
			pthread_mutex_destroy(&data->bounce_mutex);
			പോകുക cleanup;
		}
	}
#endif
	*channel = io;
	തിരികെ 0;

cleanup:
	എങ്കിൽ (data) {
		unix_funlock(io);
		എങ്കിൽ (io->manager != unixfd_io_manager && data->dev >= 0)
			close(data->dev);
		എങ്കിൽ (data->cache) {
			free_cache(data);
			സ്വതന്ത്രം(data->cache);
		}
		ext2fs_free_mem(&data);
	}
	എങ്കിൽ (io) {
		എങ്കിൽ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t unixfd_open(സ്ഥിരം അക്ഷരം *str_fd, പൂർണ്ണസംഖ്യ flags,
			     io_channel *channel)
{
	പൂർണ്ണസംഖ്യ fd;
	പൂർണ്ണസംഖ്യ fd_flags;

	fd = atoi(str_fd);
#എങ്കിൽ defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	എങ്കിൽ (fd_flags == -1)
		തിരികെ EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	എങ്കിൽ (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#എങ്കിൽ defined(O_DIRECT)
	എങ്കിൽ (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#endif
#endif  /* HAVE_FCNTL */

	തിരികെ unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

സ്ഥിതം errcode_t unix_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags,
			   io_channel *channel)
{
	പൂർണ്ണസംഖ്യ fd = -1;
	പൂർണ്ണസംഖ്യ open_flags;

	എങ്കിൽ (name == 0)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	എങ്കിൽ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#എങ്കിൽ defined(O_DIRECT)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif
	fd = ext2fs_open_file(name, open_flags, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ errno;
#എങ്കിൽ defined(F_NOCACHE) && !defined(IO_DIRECT)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO) {
		എങ്കിൽ (fcntl(fd, F_NOCACHE, 1) < 0)
			തിരികെ errno;
	}
#endif
	തിരികെ unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

സ്ഥിതം errcode_t unix_close(io_channel channel)
{
	ഘടന unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
		തിരികെ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	unix_funlock(channel);

	എങ്കിൽ (channel->manager != unixfd_io_manager && close(data->dev) < 0)
		retval = errno;
	free_cache(data);
	സ്വതന്ത്രം(data->cache);
#ifdef HAVE_PTHREAD
	എങ്കിൽ (data->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&data->cache_mutex);
		pthread_mutex_destroy(&data->bounce_mutex);
		pthread_mutex_destroy(&data->stats_mutex);
	}
#endif

	ext2fs_free_mem(&channel->private_data);
	എങ്കിൽ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t unix_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	ഘടന unix_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (channel->block_size != blksize) {
		mutex_lock(data, CACHE_MTX);
		mutex_lock(data, BOUNCE_MTX);
#ifndef NO_IO_CACHE
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data, FLUSH_NOLOCK))){
			mutex_unlock(data, BOUNCE_MTX);
			mutex_unlock(data, CACHE_MTX);
			തിരികെ retval;
		}
#endif

		channel->block_size = blksize;
		free_cache(data);
		retval = alloc_cache(channel, data);
		mutex_unlock(data, BOUNCE_MTX);
		mutex_unlock(data, CACHE_MTX);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	ഘടന unix_private_data *data;
	ഘടന unix_cache *cache;
	errcode_t	retval;
	അക്ഷരം		*cp;
	പൂർണ്ണസംഖ്യ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	തിരികെ raw_read_blk(channel, data, block, എണ്ണം, buf);
#അല്ലെങ്കിൽ
	എങ്കിൽ (data->flags & IO_FLAG_NOCACHE)
		തിരികെ raw_read_blk(channel, data, block, എണ്ണം, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	എങ്കിൽ (എണ്ണം < 0 || എണ്ണം > WRITE_DIRECT_SIZE) {
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data, 0)))
			തിരികെ retval;
		തിരികെ raw_read_blk(channel, data, block, എണ്ണം, buf);
	}

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ആകുമ്പോൾ (എണ്ണം > 0) {
		/* If it's in the cache, use it! */
		എങ്കിൽ ((cache = find_cached_block(data, block, ശൂന്യ))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			എണ്ണം--;
			block++;
			cp += channel->block_size;
			തുടരുക;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		വേണ്ടി (i=1; i < എണ്ണം; i++)
			എങ്കിൽ (find_cached_block(data, block+i, ശൂന്യ))
				നിർത്തുക;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		mutex_unlock(data, CACHE_MTX);
		എങ്കിൽ ((retval = raw_read_blk(channel, data, block, i, cp)))
			തിരികെ retval;
		mutex_lock(data, CACHE_MTX);

		/* Save the results in the cache */
		വേണ്ടി (j=0; j < i; j++) {
			എങ്കിൽ (!find_cached_block(data, block, &cache)) {
				retval = reuse_cache(channel, data,
						     cache, block);
				എങ്കിൽ (retval)
					പോകുക call_write_handler;
				memcpy(cache->buf, cp, channel->block_size);
			}
			എണ്ണം--;
			block++;
			cp += channel->block_size;
		}
	}
	mutex_unlock(data, CACHE_MTX);
	തിരികെ 0;

call_write_handler:
	എങ്കിൽ (cache->write_err && channel->write_error) {
		അക്ഷരം *err_buf = ശൂന്യ;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		എങ്കിൽ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ശൂന്യ;
		അല്ലെങ്കിൽ
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		എങ്കിൽ (err_buf)
			ext2fs_free_mem(&err_buf);
	} അല്ലെങ്കിൽ
		mutex_unlock(data, CACHE_MTX);
	തിരികെ retval;
#endif /* NO_IO_CACHE */
}

സ്ഥിതം errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	തിരികെ unix_read_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t unix_write_blk64(io_channel channel, unsigned long long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന unix_private_data *data;
	ഘടന unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	സ്ഥിരം അക്ഷരം	*cp;
	പൂർണ്ണസംഖ്യ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	തിരികെ raw_write_blk(channel, data, block, എണ്ണം, buf, 0);
#അല്ലെങ്കിൽ
	എങ്കിൽ (data->flags & IO_FLAG_NOCACHE)
		തിരികെ raw_write_blk(channel, data, block, എണ്ണം, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	എങ്കിൽ (എണ്ണം < 0 || എണ്ണം > WRITE_DIRECT_SIZE) {
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data,
						  FLUSH_INVALIDATE)))
			തിരികെ retval;
		തിരികെ raw_write_blk(channel, data, block, എണ്ണം, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	എങ്കിൽ (writethrough)
		retval = raw_write_blk(channel, data, block, എണ്ണം, buf, 0);

	cp = buf;
	mutex_lock(data, CACHE_MTX);
	ആകുമ്പോൾ (എണ്ണം > 0) {
		cache = find_cached_block(data, block, &reuse);
		എങ്കിൽ (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, data, cache, block);
			എങ്കിൽ (err)
				പോകുക call_write_handler;
		}
		എങ്കിൽ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		എണ്ണം--;
		block++;
		cp += channel->block_size;
	}
	mutex_unlock(data, CACHE_MTX);
	തിരികെ retval;

call_write_handler:
	എങ്കിൽ (cache->write_err && channel->write_error) {
		അക്ഷരം *err_buf = ശൂന്യ;
		unsigned long long err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		എങ്കിൽ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ശൂന്യ;
		അല്ലെങ്കിൽ
			memcpy(err_buf, cache->buf, channel->block_size);
		mutex_unlock(data, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_size, -1,
				       retval);
		എങ്കിൽ (err_buf)
			ext2fs_free_mem(&err_buf);
	} അല്ലെങ്കിൽ
		mutex_unlock(data, CACHE_MTX);
	തിരികെ retval;
#endif /* NO_IO_CACHE */
}

സ്ഥിതം errcode_t unix_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long എണ്ണം)
{
#ifdef POSIX_FADV_WILLNEED
	ഘടന unix_private_data *data;

	data = (ഘടന unix_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	തിരികെ posix_fadvise(data->dev,
			     (ext2_loff_t)block * channel->block_size + data->offset,
			     (ext2_loff_t)എണ്ണം * channel->block_size,
			     POSIX_FADV_WILLNEED);
#അല്ലെങ്കിൽ
	തിരികെ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

സ്ഥിതം errcode_t unix_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ unix_write_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t unix_write_byte(io_channel channel, unsigned long offset,
				 പൂർണ്ണസംഖ്യ size, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന unix_private_data *data;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (channel->align != 0) {
#ifdef ALIGN_DEBUG
		printf("unix_write_byte: O_DIRECT fallback\n");
#endif
		തിരികെ EXT2_ET_UNIMPLEMENTED;
	}

#ifndef NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	എങ്കിൽ ((retval = flush_cached_blocks(channel, data, FLUSH_INVALIDATE)))
		തിരികെ retval;
#endif

	എങ്കിൽ (lseek(data->dev, offset + data->offset, SEEK_SET) < 0)
		തിരികെ errno;

	actual = ലിഖ(data->dev, buf, size);
	എങ്കിൽ (actual < 0)
		തിരികെ errno;
	എങ്കിൽ (actual != size)
		തിരികെ EXT2_ET_SHORT_WRITE;

	തിരികെ 0;
}

/*
 * Flush data buffers to disk.
 */
സ്ഥിതം errcode_t unix_flush(io_channel channel)
{
	ഘടന unix_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif
#ifdef HAVE_FSYNC
	എങ്കിൽ (!retval && fsync(data->dev) != 0)
		തിരികെ errno;
#endif
	തിരികെ retval;
}

സ്ഥിതം errcode_t unix_set_option(io_channel channel, സ്ഥിരം അക്ഷരം *option,
				 സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന unix_private_data *data;
	unsigned long long tmp;
	errcode_t retval;
	അക്ഷരം *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (!strcmp(option, "offset")) {
		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		എങ്കിൽ (*end)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		എങ്കിൽ (data->offset < 0)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(option, "cache")) {
		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		എങ്കിൽ (!strcmp(arg, "on")) {
			data->flags &= ~IO_FLAG_NOCACHE;
			തിരികെ 0;
		}
		എങ്കിൽ (!strcmp(arg, "off")) {
			retval = flush_cached_blocks(channel, data, 0);
			data->flags |= IO_FLAG_NOCACHE;
			തിരികെ retval;
		}
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	}
#ifndef NO_IO_CACHE
	എങ്കിൽ (!strcmp(option, "cache_blocks")) {
		unsigned long long	size;

		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		errno = 0;
		size = strtoll(arg, ശൂന്യ, 0);
		എങ്കിൽ (errno || size == 0 || size > INT32_MAX)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		എങ്കിൽ (data->cache_size == size)
			തിരികെ 0;
		എങ്കിൽ (data->cache_size > size)
			തിരികെ shrink_cache(channel, data, size);
		തിരികെ grow_cache(channel, data, size);
	}
#endif
	തിരികെ EXT2_ET_INVALID_ARGUMENT;
}

#എങ്കിൽ defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

സ്ഥിതം errcode_t unix_discard(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന unix_private_data *data;
	പൂർണ്ണസംഖ്യ		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (channel->flags & CHANNEL_FLAGS_NODISCARD)
		പോകുക unimplemented;

	എങ്കിൽ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#ifdef BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_size + data->offset;
		range[1] = (__u64)(എണ്ണം) * channel->block_size;

		ret = ioctl(data->dev, BLKDISCARD, &range);
#അല്ലെങ്കിൽ
		പോകുക unimplemented;
#endif
	} അല്ലെങ്കിൽ {
#എങ്കിൽ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(data->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_size + data->offset,
				(off_t)(എണ്ണം) * channel->block_size);
#അല്ലെങ്കിൽ
		പോകുക unimplemented;
#endif
	}
	എങ്കിൽ (ret < 0) {
		എങ്കിൽ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			പോകുക unimplemented;
		}
		തിരികെ errno;
	}
	തിരികെ 0;
unimplemented:
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ __unix_zeroout(പൂർണ്ണസംഖ്യ fd, off_t offset, off_t len)
{
	പൂർണ്ണസംഖ്യ ret = -1;

#എങ്കിൽ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	എങ്കിൽ (ret == 0)
		തിരികെ 0;
#endif
#എങ്കിൽ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	എങ്കിൽ (ret == 0)
		തിരികെ 0;
#endif
	errno = EOPNOTSUPP;
	തിരികെ ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic തള്ളുക
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
സ്ഥിതം errcode_t unix_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന unix_private_data *data;
	പൂർണ്ണസംഖ്യ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		ഘടന stat statbuf;

		എങ്കിൽ (എണ്ണം == 0)
			തിരികെ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		എങ്കിൽ (ret)
			പോകുക err;
		എങ്കിൽ ((unsigned long long) statbuf.st_size <
			(block + എണ്ണം) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + എണ്ണം) * channel->block_size + data->offset);
			എങ്കിൽ (ret)
				പോകുക err;
		}
	}

	എങ്കിൽ (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		പോകുക unimplemented;

	ret = __unix_zeroout(data->dev,
			(off_t)(block) * channel->block_size + data->offset,
			(off_t)(എണ്ണം) * channel->block_size);
err:
	എങ്കിൽ (ret < 0) {
		എങ്കിൽ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			പോകുക unimplemented;
		}
		തിരികെ errno;
	}
	തിരികെ 0;
unimplemented:
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}
#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic എടുക്കുക
#endif

സ്ഥിതം ഘടന struct_io_manager struct_unix_manager = {
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

സ്ഥിതം ഘടന struct_io_manager struct_unixfd_manager = {
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
