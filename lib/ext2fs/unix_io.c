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
#परिभाषय _XOPEN_SOURCE 600
#परिभाषय _DARWIN_C_SOURCE
#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _GNU_SOURCE
#परिभाषय _GNU_SOURCE
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि_परिभाषितम् __linux__
#अन्तर्भाव <sys/utsname.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_MOUNT_H
#अन्तर्भाव <sys/mount.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#यदि_अन्तः
#यदि HAVE_LINUX_FALLOC_H
#अन्तर्भाव <linux/falloc.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_PTHREAD
#अन्तर्भाव <pthread.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_FILE_H
#अन्तर्भाव <sys/file.h>
#यदि_अन्तः

#यदि defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#परिभाषय BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#यदि_अन्तः

#अपरिभाषय ALIGN_DEBUG

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर अन्तःस्थ पूर्णाङ्क find_चरमः_bit_समुच्चयः(पूर्णाङ्क x)
{
	पूर्णाङ्क r = 32;

	यदि (!x)
		प्रत्यागम 0;
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
	प्रत्यागम r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
स्थिर अन्तःस्थ पूर्णाङ्क highbit32(uint32_t v)
{
	प्रत्यागम find_चरमः_bit_समुच्चयः(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)

संरचना unix_cache {
	वर्ण			*buf;
	अचिह्नित दीर्घ दीर्घ	खण्डम्;
	पूर्णाङ्क			access_time;
	अचिह्नित		dirty:1;
	अचिह्नित		अन्तर्_use:1;
	अचिह्नित		write_err:1;
};

#परिभाषय DEFAULT_CACHE_आकार 8
#परिभाषय WRITE_प्रत्यक्षम्_आकार 4	/* Must be smaller than CACHE_SIZE */
#परिभाषय READ_प्रत्यक्षम्_आकार 4	/* Should be smaller than CACHE_SIZE */

संरचना unix_वैयक्तिकम्_दत्त {
	पूर्णाङ्क	magic;
	पूर्णाङ्क	dev;
	पूर्णाङ्क	flags;
	पूर्णाङ्क	align;
	पूर्णाङ्क	access_time;
	पूर्णाङ्क	unix_flock_flags;
	ext2_loff_t विस्थापनम्;
	संरचना unix_cache *cache;
	अचिह्नित पूर्णाङ्क cache_आकार;
	अचिह्नित पूर्णाङ्क cache_hash_सरणम्;
	शून्य	*bounce;
	संरचना struct_io_stats io_stats;
#यदि_परिभाषितम् HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#यदि_अन्तः
};

#परिभाषय IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

प्रकारनिर्देश गणना तालकम्_विधा {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} विधा_t;

#यदि_परिभाषितम् HAVE_PTHREAD
स्थिर अन्तःस्थ pthread_mutex_t *get_mutex(संरचना unix_वैयक्तिकम्_दत्त *दत्त,
					 विधा_t विधा)
{
	यदि (दत्त->flags & IO_FLAG_तन्तवः) {
		विकल्प (विधा) {
		पक्ष CACHE_MTX:
			प्रत्यागम &दत्त->cache_mutex;
		पक्ष BOUNCE_MTX:
			प्रत्यागम &दत्त->bounce_mutex;
		पक्ष STATS_MTX:
			प्रत्यागम &दत्त->stats_mutex;
		}
	}
	प्रत्यागम रिक्त;
}
#यदि_अन्तः

स्थिर अन्तःस्थ शून्य mutex_तालकम्(संरचना unix_वैयक्तिकम्_दत्त *दत्त, विधा_t विधा)
{
#यदि_परिभाषितम् HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(दत्त,विधा);

	यदि (mtx)
		pthread_mutex_तालकम्(mtx);
#यदि_अन्तः
}

स्थिर अन्तःस्थ शून्य mutex_unlock(संरचना unix_वैयक्तिकम्_दत्त *दत्त, विधा_t विधा)
{
#यदि_परिभाषितम् HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(दत्त,विधा);

	यदि (mtx)
		pthread_mutex_unlock(mtx);
#यदि_अन्तः
}

स्थिर errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	संरचना unix_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (stats) {
		mutex_तालकम्(दत्त, STATS_MTX);
		*stats = &दत्त->io_stats;
		mutex_unlock(दत्त, STATS_MTX);
	}

	प्रत्यागम retval;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना unix_वैयक्तिकम्_दत्त *दत्त,
			      अचिह्नित दीर्घ दीर्घ खण्डम्,
			      पूर्णाङ्क गणनम्, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t		आकार;
	ext2_loff_t	location;
	पूर्णाङ्क		actual = 0;
	अचिह्नित वर्ण	*buf = bufv;
	ssize_t		really_read = 0;
	अचिह्नित दीर्घ दीर्घ aligned_blk;
	पूर्णाङ्क		align_आकार, विस्थापनम्;

	आकार = (गणनम् < 0) ? -गणनम् : (ext2_loff_t) गणनम् * channel->खण्डम्_आकार;
	mutex_तालकम्(दत्त, STATS_MTX);
	दत्त->io_stats.अष्टकानि_read += आकार;
	mutex_unlock(दत्त, STATS_MTX);
	location = ((ext2_loff_t) खण्डम् * channel->खण्डम्_आकार) + दत्त->विस्थापनम्;

	यदि (दत्त->flags & IO_FLAG_FORCE_BOUNCE)
		गच्छ bounce_read;

#यदि_परिभाषितम् HAVE_PREAD64
	/* Try an aligned pread */
	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		actual = pread64(दत्त->dev, buf, आकार, location);
		यदि (actual == आकार)
			प्रत्यागम 0;
		actual = 0;
	}
#नोचेद्यदि HAVE_PREAD
	/* Try an aligned pread */
	यदि ((परिमाण(off_t) >= परिमाण(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(आकार, channel->align)))) {
		actual = pread(दत्त->dev, buf, आकार, location);
		यदि (actual == आकार)
			प्रत्यागम 0;
		actual = 0;
	}
#यदि_अन्तः /* HAVE_PREAD */

	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		mutex_तालकम्(दत्त, BOUNCE_MTX);
		यदि (ext2fs_llseek(दत्त->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual = पठ(दत्त->dev, buf, आकार);
		यदि (actual != आकार) {
		short_read:
			यदि (actual < 0) {
				retval = errno;
				actual = 0;
			} अन्यथा
				retval = EXT2_ET_SHORT_READ;
			गच्छ error_unlock;
		}
		गच्छ success_unlock;
	}

#यदि_परिभाषितम् ALIGN_DEBUG
	प्रारूप_लिख("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिह्नित दीर्घ) आकार);
#यदि_अन्तः

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	यदि (channel->align == 0)
		channel->align = 1;
	यदि ((channel->खण्डम्_आकार > channel->align) &&
	    (channel->खण्डम्_आकार % channel->align) == 0)
		align_आकार = channel->खण्डम्_आकार;
	अन्यथा
		align_आकार = channel->align;
	aligned_blk = location / align_आकार;
	विस्थापनम् = location % align_आकार;

	mutex_तालकम्(दत्त, BOUNCE_MTX);
	यदि (ext2fs_llseek(दत्त->dev, aligned_blk * align_आकार, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		गच्छ error_unlock;
	}
	यावत् (आकार > 0) {
		actual = पठ(दत्त->dev, दत्त->bounce, align_आकार);
		यदि (actual != align_आकार) {
			actual = really_read;
			buf -= really_read;
			आकार += really_read;
			गच्छ short_read;
		}
		यदि ((actual + विस्थापनम्) > align_आकार)
			actual = align_आकार - विस्थापनम्;
		यदि (actual > आकार)
			actual = आकार;
		स्मृतिं_प्रतिलिख(buf, (वर्ण *)दत्त->bounce + विस्थापनम्, actual);

		really_read += actual;
		आकार -= actual;
		buf += actual;
		विस्थापनम् = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(दत्त, BOUNCE_MTX);
	प्रत्यागम 0;

error_unlock:
	mutex_unlock(दत्त, BOUNCE_MTX);
	यदि (actual >= 0 && actual < आकार)
		स्मृतिं_पूरय((वर्ण *) buf+actual, 0, आकार-actual);
	यदि (channel->read_error)
		retval = (channel->read_error)(channel, खण्डम्, गणनम्, buf,
					       आकार, actual, retval);
	प्रत्यागम retval;
}

#परिभाषय RAW_WRITE_NO_HANDLER	(1U << 0)
#परिभाषय RAW_WRITE_NOLOCK	(1U << 1)

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना unix_वैयक्तिकम्_दत्त *दत्त,
			       अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *bufv,
			       पूर्णाङ्क flags)
{
	ssize_t		आकार;
	ext2_loff_t	location;
	पूर्णाङ्क		actual = 0;
	errcode_t	retval;
	ध्रुव अचिह्नित वर्ण *buf = bufv;
	अचिह्नित दीर्घ दीर्घ aligned_blk;
	पूर्णाङ्क		align_आकार, विस्थापनम्;

	यदि (गणनम् == 1)
		आकार = channel->खण्डम्_आकार;
	अन्यथा {
		यदि (गणनम् < 0)
			आकार = -गणनम्;
		अन्यथा
			आकार = (ext2_loff_t) गणनम् * channel->खण्डम्_आकार;
	}
	mutex_तालकम्(दत्त, STATS_MTX);
	दत्त->io_stats.अष्टकानि_written += आकार;
	mutex_unlock(दत्त, STATS_MTX);

	location = ((ext2_loff_t) खण्डम् * channel->खण्डम्_आकार) + दत्त->विस्थापनम्;

	यदि (दत्त->flags & IO_FLAG_FORCE_BOUNCE)
		गच्छ bounce_write;

#यदि_परिभाषितम् HAVE_PWRITE64
	/* Try an aligned pwrite */
	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		actual = pwrite64(दत्त->dev, buf, आकार, location);
		यदि (actual == आकार)
			प्रत्यागम 0;
	}
#नोचेद्यदि HAVE_PWRITE
	/* Try an aligned pwrite */
	यदि ((परिमाण(off_t) >= परिमाण(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(आकार, channel->align)))) {
		actual = pwrite(दत्त->dev, buf, आकार, location);
		यदि (actual == आकार)
			प्रत्यागम 0;
	}
#यदि_अन्तः /* HAVE_PWRITE */

	यदि ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_तालकम्(दत्त, BOUNCE_MTX);
		यदि (ext2fs_llseek(दत्त->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual = लिख(दत्त->dev, buf, आकार);
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(दत्त, BOUNCE_MTX);
		यदि (actual < 0) {
			retval = errno;
			गच्छ error_बहिर्गमनम्;
		}
		यदि (actual != आकार) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_बहिर्गमनम्;
		}
		प्रत्यागम 0;
	}

#यदि_परिभाषितम् ALIGN_DEBUG
	प्रारूप_लिख("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिह्नित दीर्घ) आकार);
#यदि_अन्तः
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	यदि (channel->align == 0)
		channel->align = 1;
	यदि ((channel->खण्डम्_आकार > channel->align) &&
	    (channel->खण्डम्_आकार % channel->align) == 0)
		align_आकार = channel->खण्डम्_आकार;
	अन्यथा
		align_आकार = channel->align;
	aligned_blk = location / align_आकार;
	विस्थापनम् = location % align_आकार;

	यावत् (आकार > 0) {
		पूर्णाङ्क actual_w;

		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_तालकम्(दत्त, BOUNCE_MTX);
		यदि (आकार < align_आकार || विस्थापनम्) {
			यदि (ext2fs_llseek(दत्त->dev, aligned_blk * align_आकार,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				गच्छ error_unlock;
			}
			actual = पठ(दत्त->dev, दत्त->bounce,
				      align_आकार);
			यदि (actual != align_आकार) {
				यदि (actual < 0) {
					retval = errno;
					गच्छ error_unlock;
				}
				स्मृतिं_पूरय((वर्ण *) दत्त->bounce + actual, 0,
				       align_आकार - actual);
			}
		}
		actual = आकार;
		यदि ((actual + विस्थापनम्) > align_आकार)
			actual = align_आकार - विस्थापनम्;
		यदि (actual > आकार)
			actual = आकार;
		स्मृतिं_प्रतिलिख(((वर्ण *)दत्त->bounce) + विस्थापनम्, buf, actual);
		यदि (ext2fs_llseek(दत्त->dev, aligned_blk * align_आकार, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			गच्छ error_unlock;
		}
		actual_w = लिख(दत्त->dev, दत्त->bounce, align_आकार);
		यदि (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(दत्त, BOUNCE_MTX);
		यदि (actual_w < 0) {
			retval = errno;
			गच्छ error_बहिर्गमनम्;
		}
		यदि (actual_w != align_आकार)
			गच्छ short_write;
		आकार -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		विस्थापनम् = 0;
	}
	प्रत्यागम 0;

error_unlock:
	यदि (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(दत्त, BOUNCE_MTX);
error_बहिर्गमनम्:
	यदि (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, खण्डम्, गणनम्, buf,
						आकार, actual, retval);
	प्रत्यागम retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
स्थिर errcode_t alloc_cache(io_channel channel,
			     संरचना unix_वैयक्तिकम्_दत्त *दत्त)
{
	errcode_t		retval;
	संरचना unix_cache	*cache;
	अचिह्नित पूर्णाङ्क		i;

	दत्त->access_time = 0;
	प्रति (i=0, cache = दत्त->cache; i < दत्त->cache_आकार; i++, cache++) {
		cache->खण्डम् = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		यदि (retval)
			प्रत्यागम retval;
	}
	यदि (channel->align || दत्त->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (दत्त->bounce)
			ext2fs_free_mem(&दत्त->bounce);
		retval = io_channel_alloc_buf(channel, 0, &दत्त->bounce);
	}
	प्रत्यागम retval;
}

/* Free the cache buffers */
स्थिर शून्य free_cache(संरचना unix_वैयक्तिकम्_दत्त *दत्त)
{
	संरचना unix_cache	*cache;
	अचिह्नित पूर्णाङ्क		i;

	दत्त->access_time = 0;
	प्रति (i=0, cache = दत्त->cache; i < दत्त->cache_आकार; i++, cache++) {
		cache->खण्डम् = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	यदि (दत्त->bounce)
		ext2fs_free_mem(&दत्त->bounce);
}

#यदि_अपरिभाषितम् NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#परिभाषय GOLDEN_अनुपातः_अविभाज्यः	0x9e37fffffffc0001UL
#यदि_अपरिभाषितम् CACHE_पङ्क्तिः_आकार
/* if the system didn't tell us, guess something reasonable */
#परिभाषय CACHE_पङ्क्तिः_आकार		64
#यदि_अन्तः

/* buffer cache hashing function, crudely stolen from xfsprogs */
स्थिर अचिह्नित पूर्णाङ्क
cache_hash(संरचना unix_वैयक्तिकम्_दत्त *दत्त, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	यदि (दत्त->cache_आकार <= DEFAULT_CACHE_आकार)
		प्रत्यागम 0;

	tmp = hashval ^ (GOLDEN_अनुपातः_अविभाज्यः + hashval) / CACHE_पङ्क्तिः_आकार;
	tmp = tmp ^ ((tmp ^ GOLDEN_अनुपातः_अविभाज्यः) >> दत्त->cache_hash_सरणम्);
	प्रत्यागम tmp % दत्त->cache_आकार;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
स्थिर संरचना unix_cache *find_cached_खण्डम्(संरचना unix_वैयक्तिकम्_दत्त *दत्त,
					    अचिह्नित दीर्घ दीर्घ खण्डम्,
					    संरचना unix_cache **eldest)
{
	संरचना unix_cache	*cache, *अप्रयुक्तम्_cache, *oldest_cache;
	अचिह्नित पूर्णाङ्क		hash = cache_hash(दत्त, खण्डम्);
	अचिह्नित पूर्णाङ्क		i;

	अप्रयुक्तम्_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	प्रति (i = hash, cache = दत्त->cache + hash;
	     i < दत्त->cache_आकार;
	     i++, cache++) {
		यदि (!cache->अन्तर्_use) {
			यदि (!अप्रयुक्तम्_cache)
				अप्रयुक्तम्_cache = cache;
			अनुवर्त;
		}
		यदि (cache->खण्डम् == खण्डम्) {
			cache->access_time = ++दत्त->access_time;
			दत्त->io_stats.cache_लब्धानि++;
			प्रत्यागम cache;
		}
		यदि (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	प्रति (i = 0, cache = दत्त->cache; i < hash; i++, cache++) {
		यदि (!cache->अन्तर्_use) {
			यदि (!अप्रयुक्तम्_cache)
				अप्रयुक्तम्_cache = cache;
			अनुवर्त;
		}
		यदि (cache->खण्डम् == खण्डम्) {
			cache->access_time = ++दत्त->access_time;
			दत्त->io_stats.cache_लब्धानि++;
			प्रत्यागम cache;
		}
		यदि (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	यदि (eldest)
		*eldest = (अप्रयुक्तम्_cache) ? अप्रयुक्तम्_cache : oldest_cache;
	दत्त->io_stats.cache_अलब्धानि++;
	प्रत्यागम 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर errcode_t reuse_cache(io_channel channel,
		संरचना unix_वैयक्तिकम्_दत्त *दत्त, संरचना unix_cache *cache,
		अचिह्नित दीर्घ दीर्घ खण्डम्)
{
	यदि (cache->dirty && cache->अन्तर्_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, दत्त, cache->खण्डम्, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		यदि (retval) {
			cache->write_err = 1;
			प्रत्यागम retval;
		}
	}

	cache->अन्तर्_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->खण्डम् = खण्डम्;
	cache->access_time = ++दत्त->access_time;
	प्रत्यागम 0;
}

#परिभाषय FLUSH_INVALIDATE	0x01
#परिभाषय FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
स्थिर errcode_t flush_cached_खण्डानि(io_channel channel,
				     संरचना unix_वैयक्तिकम्_दत्त *दत्त,
				     पूर्णाङ्क flags)
{
	संरचना unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	अचिह्नित पूर्णाङ्क		i;
	पूर्णाङ्क			errors_प्राप्तम् = 0;

	यदि ((flags & FLUSH_NOLOCK) == 0)
		mutex_तालकम्(दत्त, CACHE_MTX);
	प्रति (i=0, cache = दत्त->cache; i < दत्त->cache_आकार; i++, cache++) {
		यदि (!cache->अन्तर्_use)
			अनुवर्त;
		यदि (cache->dirty) {
			पूर्णाङ्क raw_flags = RAW_WRITE_NO_HANDLER;

			यदि (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, दत्त,
					       cache->खण्डम्, 1, cache->buf,
					       raw_flags);
			यदि (retval) {
				cache->write_err = 1;
				errors_प्राप्तम् = 1;
				retval2 = retval;
			} अन्यथा {
				cache->dirty = 0;
				cache->write_err = 0;
				यदि (flags & FLUSH_INVALIDATE)
					cache->अन्तर्_use = 0;
			}
		} अन्यथा यदि (flags & FLUSH_INVALIDATE) {
			cache->अन्तर्_use = 0;
		}
	}
	यदि ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(दत्त, CACHE_MTX);
retry:
	यावत् (errors_प्राप्तम्) {
		यदि ((flags & FLUSH_NOLOCK) == 0)
			mutex_तालकम्(दत्त, CACHE_MTX);
		errors_प्राप्तम् = 0;
		प्रति (i=0, cache = दत्त->cache; i < दत्त->cache_आकार; i++, cache++) {
			यदि (!cache->अन्तर्_use || !cache->write_err)
				अनुवर्त;
			errors_प्राप्तम् = 1;
			यदि (cache->write_err && channel->write_error) {
				वर्ण *err_buf = रिक्त;
				अचिह्नित दीर्घ दीर्घ err_खण्डम् = cache->खण्डम्;

				cache->dirty = 0;
				cache->अन्तर्_use = 0;
				cache->write_err = 0;
				यदि (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = रिक्त;
				अन्यथा
					स्मृतिं_प्रतिलिख(err_buf, cache->buf,
					       channel->खण्डम्_आकार);
				mutex_unlock(दत्त, CACHE_MTX);
				(channel->write_error)(channel, err_खण्डम्,
					1, err_buf, channel->खण्डम्_आकार, -1,
					retval2);
				यदि (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_तालकम्(दत्त, CACHE_MTX);
				गच्छ retry;
			} अन्यथा
				cache->write_err = 0;
		}
		यदि ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(दत्त, CACHE_MTX);
	}
	प्रत्यागम retval2;
}

/* Shrink the cache buffers */
स्थिर errcode_t shrink_cache(io_channel channel,
			      संरचना unix_वैयक्तिकम्_दत्त *दत्त,
			      अचिह्नित पूर्णाङ्क new_आकार)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिह्नित पूर्णाङ्क		i;
	errcode_t		retval;

	mutex_तालकम्(दत्त, CACHE_MTX);

	retval = flush_cached_खण्डानि(channel, दत्त,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	यदि (retval)
		गच्छ unlock;

	प्रति (i = new_आकार, cache = दत्त->cache + new_आकार;
	     i < दत्त->cache_आकार;
	     i++, cache++) {
		cache->खण्डम् = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		यदि (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(दत्त->cache, new_आकार * परिमाण(संरचना unix_cache));
	यदि (!new_cache) {
		retval = EXT2_ET_NO_स्मृतिः;
		गच्छ unlock;
	}

	दत्त->cache = new_cache;
	दत्त->cache_आकार = new_आकार;
	दत्त->cache_hash_सरणम् = highbit32(दत्त->cache_आकार);

unlock:
	mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम retval;
}

/* Grow the cache buffers */
स्थिर errcode_t grow_cache(io_channel channel,
			    संरचना unix_वैयक्तिकम्_दत्त *दत्त,
			    अचिह्नित पूर्णाङ्क new_आकार)
{
	संरचना unix_cache	*cache, *new_cache;
	अचिह्नित पूर्णाङ्क		i;
	errcode_t		retval;

	mutex_तालकम्(दत्त, CACHE_MTX);

	retval = flush_cached_खण्डानि(channel, दत्त,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	यदि (retval)
		गच्छ unlock;

	new_cache = realloc(दत्त->cache, new_आकार * परिमाण(संरचना unix_cache));
	यदि (!new_cache) {
		retval = EXT2_ET_NO_स्मृतिः;
		गच्छ unlock;
	}
	दत्त->cache = new_cache;

	प्रति (i = दत्त->cache_आकार, cache = new_cache + दत्त->cache_आकार;
	     i < new_आकार;
	     i++, cache++) {
		cache->खण्डम् = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		यदि (retval) {
			यावत् (i > दत्त->cache_आकार) {
				cache--; i--;
				यदि (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			गच्छ unlock;
		}
	}

	दत्त->cache_आकार = new_आकार;
	दत्त->cache_hash_सरणम् = highbit32(दत्त->cache_आकार);

unlock:
	mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम retval;
}
#यदि_अन्तः /* NO_IO_CACHE */

#यदि_परिभाषितम् __linux__
#यदि_अपरिभाषितम् BLKDISCARDZEROES
#परिभाषय BLKDISCARDZEROES _IO(0x12,124)
#यदि_अन्तः
#यदि_अन्तः

पूर्णाङ्क ext2fs_open_file(ध्रुव वर्ण *pathname, पूर्णाङ्क flags, mode_t बहुलकम्)
{
	यदि (बहुलकम्)
#यदि defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		प्रत्यागम open64(pathname, flags, बहुलकम्);
	अन्यथा
		प्रत्यागम open64(pathname, flags);
#अन्यथा
		प्रत्यागम उद्घाटय(pathname, flags, बहुलकम्);
	अन्यथा
		प्रत्यागम उद्घाटय(pathname, flags);
#यदि_अन्तः
}

पूर्णाङ्क ext2fs_stat(ध्रुव वर्ण *मार्गः, ext2fs_struct_stat *buf)
{
#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	प्रत्यागम stat64(मार्गः, buf);
#अन्यथा
	प्रत्यागम stat(मार्गः, buf);
#यदि_अन्तः
}

पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf)
{
#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	प्रत्यागम fstat64(fd, buf);
#अन्यथा
	प्रत्यागम fstat(fd, buf);
#यदि_अन्तः
}

#यदि_परिभाषितम् HAVE_SYS_FILE_H
स्थिर errcode_t unix_funlock(io_channel channel)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	पूर्णाङ्क ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (दत्त->unix_flock_flags) {
		ret = flock(दत्त->dev, LOCK_UN);
		यदि (ret)
			प्रत्यागम errno;

		दत्त->unix_flock_flags = 0;
	}

	प्रत्यागम 0;
}

स्थिर errcode_t unix_flock(io_channel channel, अचिह्नित पूर्णाङ्क flock_flags)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	पूर्णाङ्क unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	यदि (ret)
		प्रत्यागम ret;

	यदि (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	यदि (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	यदि (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	यदि (!unix_flock_flags)
		प्रत्यागम 0;

	ret = flock(दत्त->dev, unix_flock_flags);
	यदि (ret < 0)
		प्रत्यागम errno;

	दत्त->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	प्रत्यागम 0;
}
#अन्यथा
#परिभाषय unix_flock		रिक्त

स्थिर errcode_t unix_funlock(io_channel channel)
{
	प्रत्यागम 0;
}
#यदि_अन्तः /* HAVE_SYS_FILE_H */

स्थिर errcode_t unix_open_channel(ध्रुव वर्ण *name, पूर्णाङ्क fd,
				   पूर्णाङ्क flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	संरचना unix_वैयक्तिकम्_दत्त *दत्त = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;
#यदि_परिभाषितम् __linux__
	संरचना		utsname ut;
#यदि_अन्तः

	यदि (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#यदि_परिभाषितम् __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(शून्य) fsync(fd);
#यदि_अन्तः

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना unix_वैयक्तिकम्_दत्त), &दत्त);
	यदि (retval)
		गच्छ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(io->name, name);
	io->वैयक्तिकम्_दत्त = दत्त;
	io->खण्डम्_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	यदि (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	स्मृतिं_पूरय(दत्त, 0, परिमाण(संरचना unix_वैयक्तिकम्_दत्त));
	दत्त->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	दत्त->io_stats.अङ्कः_fields = 4;
	दत्त->flags = flags;
	दत्त->dev = fd;

	दत्त->cache_आकार = DEFAULT_CACHE_आकार;
	दत्त->cache_hash_सरणम् = highbit32(दत्त->cache_आकार);
	दत्त->cache = शून्यस्मृतिं_देहि(DEFAULT_CACHE_आकार, परिमाण(संरचना unix_cache));
	यदि (!दत्त->cache) {
		retval = EXT2_ET_NO_स्मृतिः;
		गच्छ cleanup;
	}

#यदि defined(O_प्रत्यक्षम्)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO)
		io->align = ext2fs_get_dio_alignment(दत्त->dev);
#नोचेद्यदि defined(F_NOCACHE)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO)
		io->align = 4096;
#यदि_अन्तः

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	यदि (ext2fs_fstat(दत्त->dev, &st) == 0) {
		यदि (ext2fsP_is_disk_device(st.st_बहुलकम्)) {
#यदि_परिभाषितम् BLKDISCARDZEROES
			पूर्णाङ्क zeroes = 0;

			यदि (ioctl(दत्त->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#यदि_अन्तः
			io->flags |= CHANNEL_FLAGS_खण्डम्_DEVICE;
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
#यदि_अन्तः

#यदि defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	यदि (io->flags & CHANNEL_FLAGS_खण्डम्_DEVICE) {
		पूर्णाङ्क dio_align = ext2fs_get_dio_alignment(fd);

		यदि (io->align < dio_align)
			io->align = dio_align;
	}
#यदि_अन्तः

	यदि ((retval = alloc_cache(io, दत्त)))
		गच्छ cleanup;

#यदि_परिभाषितम् BLKROGET
	यदि (flags & IO_FLAG_RW) {
		पूर्णाङ्क error;
		पूर्णाङ्क readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(दत्त->dev, BLKROGET, &readonly);
		यदि (!error && readonly) {
			retval = EPERM;
			गच्छ cleanup;
		}
	}
#यदि_अन्तः

#यदि_परिभाषितम् __linux__
#अपरिभाषय RLIM_INFINITY
#यदि (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#परिभाषय RLIM_INFINITY	((अचिह्नित दीर्घ)(~0UL>>1))
#अन्यथा
#परिभाषय RLIM_INFINITY  (~0UL)
#यदि_अन्तः
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
	    (ext2fs_fstat(दत्त->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_बहुलकम्))) {
		संरचना rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (अचिह्नित दीर्घ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		यदि (((अचिह्नित दीर्घ) rlim.rlim_cur) <
		    ((अचिह्नित दीर्घ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_PTHREAD
	यदि (flags & IO_FLAG_तन्तवः) {
		io->flags |= CHANNEL_FLAGS_तन्तवः;
		retval = pthread_mutex_init(&दत्त->cache_mutex, रिक्त);
		यदि (retval)
			गच्छ cleanup;
		retval = pthread_mutex_init(&दत्त->bounce_mutex, रिक्त);
		यदि (retval) {
			pthread_mutex_destroy(&दत्त->cache_mutex);
			गच्छ cleanup;
		}
		retval = pthread_mutex_init(&दत्त->stats_mutex, रिक्त);
		यदि (retval) {
			pthread_mutex_destroy(&दत्त->cache_mutex);
			pthread_mutex_destroy(&दत्त->bounce_mutex);
			गच्छ cleanup;
		}
	}
#यदि_अन्तः
	*channel = io;
	प्रत्यागम 0;

cleanup:
	यदि (दत्त) {
		unix_funlock(io);
		यदि (io->manager != unixfd_io_manager && दत्त->dev >= 0)
			close(दत्त->dev);
		यदि (दत्त->cache) {
			free_cache(दत्त);
			मुक्त(दत्त->cache);
		}
		ext2fs_free_mem(&दत्त);
	}
	यदि (io) {
		यदि (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t unixfd_open(ध्रुव वर्ण *str_fd, पूर्णाङ्क flags,
			     io_channel *channel)
{
	पूर्णाङ्क fd;
	पूर्णाङ्क fd_flags;

	fd = atoi(str_fd);
#यदि defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	यदि (fd_flags == -1)
		प्रत्यागम EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	यदि (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#यदि defined(O_प्रत्यक्षम्)
	यदि (fd_flags & O_प्रत्यक्षम्)
		flags |= IO_FLAG_प्रत्यक्षम्_IO;
#यदि_अन्तः
#यदि_अन्तः  /* HAVE_FCNTL */

	प्रत्यागम unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

स्थिर errcode_t unix_open(ध्रुव वर्ण *name, पूर्णाङ्क flags,
			   io_channel *channel)
{
	पूर्णाङ्क fd = -1;
	पूर्णाङ्क open_flags;

	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	यदि (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#यदि defined(O_प्रत्यक्षम्)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO)
		open_flags |= O_प्रत्यक्षम्;
#यदि_अन्तः
	fd = ext2fs_open_file(name, open_flags, 0);
	यदि (fd < 0)
		प्रत्यागम errno;
#यदि defined(F_NOCACHE) && !defined(IO_प्रत्यक्षम्)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO) {
		यदि (fcntl(fd, F_NOCACHE, 1) < 0)
			प्रत्यागम errno;
	}
#यदि_अन्तः
	प्रत्यागम unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

स्थिर errcode_t unix_close(io_channel channel)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;

#यदि_अपरिभाषितम् NO_IO_CACHE
	retval = flush_cached_खण्डानि(channel, दत्त, 0);
#यदि_अन्तः

	unix_funlock(channel);

	यदि (channel->manager != unixfd_io_manager && close(दत्त->dev) < 0)
		retval = errno;
	free_cache(दत्त);
	मुक्त(दत्त->cache);
#यदि_परिभाषितम् HAVE_PTHREAD
	यदि (दत्त->flags & IO_FLAG_तन्तवः) {
		pthread_mutex_destroy(&दत्त->cache_mutex);
		pthread_mutex_destroy(&दत्त->bounce_mutex);
		pthread_mutex_destroy(&दत्त->stats_mutex);
	}
#यदि_अन्तः

	ext2fs_free_mem(&channel->वैयक्तिकम्_दत्त);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t unix_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->खण्डम्_आकार != blksize) {
		mutex_तालकम्(दत्त, CACHE_MTX);
		mutex_तालकम्(दत्त, BOUNCE_MTX);
#यदि_अपरिभाषितम् NO_IO_CACHE
		यदि ((retval = flush_cached_खण्डानि(channel, दत्त, FLUSH_NOLOCK))){
			mutex_unlock(दत्त, BOUNCE_MTX);
			mutex_unlock(दत्त, CACHE_MTX);
			प्रत्यागम retval;
		}
#यदि_अन्तः

		channel->खण्डम्_आकार = blksize;
		free_cache(दत्त);
		retval = alloc_cache(channel, दत्त);
		mutex_unlock(दत्त, BOUNCE_MTX);
		mutex_unlock(दत्त, CACHE_MTX);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t unix_read_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	संरचना unix_cache *cache;
	errcode_t	retval;
	वर्ण		*cp;
	पूर्णाङ्क		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#यदि_परिभाषितम् NO_IO_CACHE
	प्रत्यागम raw_read_blk(channel, दत्त, खण्डम्, गणनम्, buf);
#अन्यथा
	यदि (दत्त->flags & IO_FLAG_NOCACHE)
		प्रत्यागम raw_read_blk(channel, दत्त, खण्डम्, गणनम्, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	यदि (गणनम् < 0 || गणनम् > WRITE_प्रत्यक्षम्_आकार) {
		यदि ((retval = flush_cached_खण्डानि(channel, दत्त, 0)))
			प्रत्यागम retval;
		प्रत्यागम raw_read_blk(channel, दत्त, खण्डम्, गणनम्, buf);
	}

	cp = buf;
	mutex_तालकम्(दत्त, CACHE_MTX);
	यावत् (गणनम् > 0) {
		/* If it's in the cache, use it! */
		यदि ((cache = find_cached_खण्डम्(दत्त, खण्डम्, रिक्त))) {
#यदि_परिभाषितम् DEBUG
			प्रारूप_लिख("Using cached block %lu\n", खण्डम्);
#यदि_अन्तः
			स्मृतिं_प्रतिलिख(cp, cache->buf, channel->खण्डम्_आकार);
			गणनम्--;
			खण्डम्++;
			cp += channel->खण्डम्_आकार;
			अनुवर्त;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		प्रति (i=1; i < गणनम्; i++)
			यदि (find_cached_खण्डम्(दत्त, खण्डम्+i, रिक्त))
				भङ्ग;
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("Reading %d blocks starting at %lu\n", i, खण्डम्);
#यदि_अन्तः
		mutex_unlock(दत्त, CACHE_MTX);
		यदि ((retval = raw_read_blk(channel, दत्त, खण्डम्, i, cp)))
			प्रत्यागम retval;
		mutex_तालकम्(दत्त, CACHE_MTX);

		/* Save the results in the cache */
		प्रति (j=0; j < i; j++) {
			यदि (!find_cached_खण्डम्(दत्त, खण्डम्, &cache)) {
				retval = reuse_cache(channel, दत्त,
						     cache, खण्डम्);
				यदि (retval)
					गच्छ call_write_handler;
				स्मृतिं_प्रतिलिख(cache->buf, cp, channel->खण्डम्_आकार);
			}
			गणनम्--;
			खण्डम्++;
			cp += channel->खण्डम्_आकार;
		}
	}
	mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम 0;

call_write_handler:
	यदि (cache->write_err && channel->write_error) {
		वर्ण *err_buf = रिक्त;
		अचिह्नित दीर्घ दीर्घ err_खण्डम् = cache->खण्डम्;

		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		cache->write_err = 0;
		यदि (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स्मृतिं_प्रतिलिख(err_buf, cache->buf, channel->खण्डम्_आकार);
		mutex_unlock(दत्त, CACHE_MTX);
		(channel->write_error)(channel, err_खण्डम्, 1, err_buf,
				       channel->खण्डम्_आकार, -1,
				       retval);
		यदि (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम retval;
#यदि_अन्तः /* NO_IO_CACHE */
}

स्थिर errcode_t unix_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम unix_read_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t unix_write_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	संरचना unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	ध्रुव वर्ण	*cp;
	पूर्णाङ्क		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#यदि_परिभाषितम् NO_IO_CACHE
	प्रत्यागम raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf, 0);
#अन्यथा
	यदि (दत्त->flags & IO_FLAG_NOCACHE)
		प्रत्यागम raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	यदि (गणनम् < 0 || गणनम् > WRITE_प्रत्यक्षम्_आकार) {
		यदि ((retval = flush_cached_खण्डानि(channel, दत्त,
						  FLUSH_INVALIDATE)))
			प्रत्यागम retval;
		प्रत्यागम raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	यदि (writethrough)
		retval = raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf, 0);

	cp = buf;
	mutex_तालकम्(दत्त, CACHE_MTX);
	यावत् (गणनम् > 0) {
		cache = find_cached_खण्डम्(दत्त, खण्डम्, &reuse);
		यदि (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, दत्त, cache, खण्डम्);
			यदि (err)
				गच्छ call_write_handler;
		}
		यदि (cache->buf != cp)
			स्मृतिं_प्रतिलिख(cache->buf, cp, channel->खण्डम्_आकार);
		cache->dirty = !writethrough;
		गणनम्--;
		खण्डम्++;
		cp += channel->खण्डम्_आकार;
	}
	mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम retval;

call_write_handler:
	यदि (cache->write_err && channel->write_error) {
		वर्ण *err_buf = रिक्त;
		अचिह्नित दीर्घ दीर्घ err_खण्डम् = cache->खण्डम्;

		cache->dirty = 0;
		cache->अन्तर्_use = 0;
		cache->write_err = 0;
		यदि (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = रिक्त;
		अन्यथा
			स्मृतिं_प्रतिलिख(err_buf, cache->buf, channel->खण्डम्_आकार);
		mutex_unlock(दत्त, CACHE_MTX);
		(channel->write_error)(channel, err_खण्डम्, 1, err_buf,
				       channel->खण्डम्_आकार, -1,
				       retval);
		यदि (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(दत्त, CACHE_MTX);
	प्रत्यागम retval;
#यदि_अन्तः /* NO_IO_CACHE */
}

स्थिर errcode_t unix_cache_readahead(io_channel channel,
				      अचिह्नित दीर्घ दीर्घ खण्डम्,
				      अचिह्नित दीर्घ दीर्घ गणनम्)
{
#यदि_परिभाषितम् POSIX_FADV_WILLNEED
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;

	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *)channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	प्रत्यागम posix_fadvise(दत्त->dev,
			     (ext2_loff_t)खण्डम् * channel->खण्डम्_आकार + दत्त->विस्थापनम्,
			     (ext2_loff_t)गणनम् * channel->खण्डम्_आकार,
			     POSIX_FADV_WILLNEED);
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#यदि_अन्तः
}

स्थिर errcode_t unix_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम unix_write_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t unix_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				 पूर्णाङ्क आकार, ध्रुव शून्य *buf)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->align != 0) {
#यदि_परिभाषितम् ALIGN_DEBUG
		प्रारूप_लिख("unix_write_byte: O_DIRECT fallback\n");
#यदि_अन्तः
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	}

#यदि_अपरिभाषितम् NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	यदि ((retval = flush_cached_खण्डानि(channel, दत्त, FLUSH_INVALIDATE)))
		प्रत्यागम retval;
#यदि_अन्तः

	यदि (lseek(दत्त->dev, विस्थापनम् + दत्त->विस्थापनम्, SEEK_SET) < 0)
		प्रत्यागम errno;

	actual = लिख(दत्त->dev, buf, आकार);
	यदि (actual < 0)
		प्रत्यागम errno;
	यदि (actual != आकार)
		प्रत्यागम EXT2_ET_SHORT_WRITE;

	प्रत्यागम 0;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t unix_flush(io_channel channel)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#यदि_अपरिभाषितम् NO_IO_CACHE
	retval = flush_cached_खण्डानि(channel, दत्त, 0);
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_FSYNC
	यदि (!retval && fsync(दत्त->dev) != 0)
		प्रत्यागम errno;
#यदि_अन्तः
	प्रत्यागम retval;
}

स्थिर errcode_t unix_समुच्चयः_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	अचिह्नित दीर्घ दीर्घ tmp;
	errcode_t retval;
	वर्ण *अन्तः;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!वाक्यं_तुलय(option, "offset")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		tmp = strtoull(arg, &अन्तः, 0);
		यदि (*अन्तः)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		दत्त->विस्थापनम् = tmp;
		यदि (दत्त->विस्थापनम् < 0)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(option, "cache")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		यदि (!वाक्यं_तुलय(arg, "on")) {
			दत्त->flags &= ~IO_FLAG_NOCACHE;
			प्रत्यागम 0;
		}
		यदि (!वाक्यं_तुलय(arg, "off")) {
			retval = flush_cached_खण्डानि(channel, दत्त, 0);
			दत्त->flags |= IO_FLAG_NOCACHE;
			प्रत्यागम retval;
		}
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	}
#यदि_अपरिभाषितम् NO_IO_CACHE
	यदि (!वाक्यं_तुलय(option, "cache_blocks")) {
		अचिह्नित दीर्घ दीर्घ	आकार;

		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		errno = 0;
		आकार = strtoll(arg, रिक्त, 0);
		यदि (errno || आकार == 0 || आकार > INT32_MAX)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		यदि (दत्त->cache_आकार == आकार)
			प्रत्यागम 0;
		यदि (दत्त->cache_आकार > आकार)
			प्रत्यागम shrink_cache(channel, दत्त, आकार);
		प्रत्यागम grow_cache(channel, दत्त, आकार);
	}
#यदि_अन्तः
	प्रत्यागम EXT2_ET_INVALID_तर्कः;
}

#यदि defined(__linux__) && !defined(BLKDISCARD)
#परिभाषय BLKDISCARD		_IO(0x12,119)
#यदि_अन्तः

स्थिर errcode_t unix_discard(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	पूर्णाङ्क		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (channel->flags & CHANNEL_FLAGS_NODISCARD)
		गच्छ unimplemented;

	यदि (channel->flags & CHANNEL_FLAGS_खण्डम्_DEVICE) {
#यदि_परिभाषितम् BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(खण्डम्) * channel->खण्डम्_आकार + दत्त->विस्थापनम्;
		range[1] = (__u64)(गणनम्) * channel->खण्डम्_आकार;

		ret = ioctl(दत्त->dev, BLKDISCARD, &range);
#अन्यथा
		गच्छ unimplemented;
#यदि_अन्तः
	} अन्यथा {
#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(दत्त->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(खण्डम्) * channel->खण्डम्_आकार + दत्त->विस्थापनम्,
				(off_t)(गणनम्) * channel->खण्डम्_आकार);
#अन्यथा
		गच्छ unimplemented;
#यदि_अन्तः
	}
	यदि (ret < 0) {
		यदि (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			गच्छ unimplemented;
		}
		प्रत्यागम errno;
	}
	प्रत्यागम 0;
unimplemented:
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
स्थिर पूर्णाङ्क __unix_zeroout(पूर्णाङ्क fd, off_t विस्थापनम्, off_t len)
{
	पूर्णाङ्क ret = -1;

#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, विस्थापनम्, len);
	यदि (ret == 0)
		प्रत्यागम 0;
#यदि_अन्तः
#यदि defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			विस्थापनम्,  len);
	यदि (ret == 0)
		प्रत्यागम 0;
#यदि_अन्तः
	errno = EOPNOTSUPP;
	प्रत्यागम ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wunused-parameter"
#यदि_अन्तः
स्थिर errcode_t unix_zeroout(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना unix_वैयक्तिकम्_दत्त *दत्त;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना unix_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!(channel->flags & CHANNEL_FLAGS_खण्डम्_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		संरचना stat statbuf;

		यदि (गणनम् == 0)
			प्रत्यागम 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(दत्त->dev, &statbuf);
		यदि (ret)
			गच्छ err;
		यदि ((अचिह्नित दीर्घ दीर्घ) statbuf.st_आकार <
			(खण्डम् + गणनम्) * channel->खण्डम्_आकार + दत्त->विस्थापनम्) {
			ret = ftruncate(दत्त->dev,
					(खण्डम् + गणनम्) * channel->खण्डम्_आकार + दत्त->विस्थापनम्);
			यदि (ret)
				गच्छ err;
		}
	}

	यदि (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		गच्छ unimplemented;

	ret = __unix_zeroout(दत्त->dev,
			(off_t)(खण्डम्) * channel->खण्डम्_आकार + दत्त->विस्थापनम्,
			(off_t)(गणनम्) * channel->खण्डम्_आकार);
err:
	यदि (ret < 0) {
		यदि (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			गच्छ unimplemented;
		}
		प्रत्यागम errno;
	}
	प्रत्यागम 0;
unimplemented:
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}
#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः

स्थिर संरचना struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.उद्घाटय		= unix_open,
	.close		= unix_close,
	.समुच्चयः_blksize	= unix_समुच्चयः_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.समुच्चयः_option	= unix_समुच्चयः_option,
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
	.उद्घाटय		= unixfd_open,
	.close		= unix_close,
	.समुच्चयः_blksize	= unix_समुच्चयः_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.समुच्चयः_option	= unix_समुच्चयः_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
