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

#अगर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#घोषणा _XOPEN_SOURCE 600
#घोषणा _DARWIN_C_SOURCE
#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _GNU_SOURCE
#घोषणा _GNU_SOURCE
#पूर्ण_अगर
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर_घोषित __linux__
#समावेश <sys/utsname.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_IOCTL_H
#समावेश <sys/ioctl.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_MOUNT_H
#समावेश <sys/mount.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_RESOURCE_H
#समावेश <sys/resource.h>
#पूर्ण_अगर
#अगर HAVE_LINUX_FALLOC_H
#समावेश <linux/falloc.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_PTHREAD
#समावेश <pthread.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_FILE_H
#समावेश <sys/file.h>
#पूर्ण_अगर

#अगर defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#घोषणा BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#पूर्ण_अगर

#अघोषित ALIGN_DEBUG

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

अटल अंतरभूत पूर्णांक find_last_bit_set(पूर्णांक x)
{
	पूर्णांक r = 32;

	अगर (!x)
		वापस 0;
	अगर (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	अगर (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	अगर (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	अगर (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	अगर (!(x & 0x80000000u)) {
		r -= 1;
	}
	वापस r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
अटल अंतरभूत पूर्णांक highbit32(uint32_t v)
{
	वापस find_last_bit_set(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)

काष्ठा unix_cache {
	अक्षर			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णांक			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
	अचिन्हित		write_err:1;
};

#घोषणा DEFAULT_CACHE_आकार 8
#घोषणा WRITE_DIRECT_आकार 4	/* Must be smaller than CACHE_SIZE */
#घोषणा READ_DIRECT_आकार 4	/* Should be smaller than CACHE_SIZE */

काष्ठा unix_private_तथ्य {
	पूर्णांक	magic;
	पूर्णांक	dev;
	पूर्णांक	flags;
	पूर्णांक	align;
	पूर्णांक	access_time;
	पूर्णांक	unix_flock_flags;
	ext2_loff_t offset;
	काष्ठा unix_cache *cache;
	अचिन्हित पूर्णांक cache_आकार;
	अचिन्हित पूर्णांक cache_hash_shift;
	व्योम	*bounce;
	काष्ठा struct_io_stats io_stats;
#अगर_घोषित HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#पूर्ण_अगर
};

#घोषणा IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

प्रकार क्रमागत lock_kind {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} kind_t;

#अगर_घोषित HAVE_PTHREAD
अटल अंतरभूत pthread_mutex_t *get_mutex(काष्ठा unix_private_तथ्य *तथ्य,
					 kind_t kind)
{
	अगर (तथ्य->flags & IO_FLAG_THREADS) {
		चयन (kind) {
		हाल CACHE_MTX:
			वापस &तथ्य->cache_mutex;
		हाल BOUNCE_MTX:
			वापस &तथ्य->bounce_mutex;
		हाल STATS_MTX:
			वापस &तथ्य->stats_mutex;
		}
	}
	वापस शून्य;
}
#पूर्ण_अगर

अटल अंतरभूत व्योम mutex_lock(काष्ठा unix_private_तथ्य *तथ्य, kind_t kind)
{
#अगर_घोषित HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(तथ्य,kind);

	अगर (mtx)
		pthread_mutex_lock(mtx);
#पूर्ण_अगर
}

अटल अंतरभूत व्योम mutex_unlock(काष्ठा unix_private_तथ्य *तथ्य, kind_t kind)
{
#अगर_घोषित HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(तथ्य,kind);

	अगर (mtx)
		pthread_mutex_unlock(mtx);
#पूर्ण_अगर
}

अटल errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	काष्ठा unix_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (stats) {
		mutex_lock(तथ्य, STATS_MTX);
		*stats = &तथ्य->io_stats;
		mutex_unlock(तथ्य, STATS_MTX);
	}

	वापस retval;
}

/*
 * Here are the raw I/O functions
 */
अटल errcode_t raw_read_blk(io_channel channel,
			      काष्ठा unix_private_तथ्य *तथ्य,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णांक गिनती, व्योम *bufv)
{
	errcode_t	retval;
	ssize_t		आकार;
	ext2_loff_t	location;
	पूर्णांक		actual = 0;
	अचिन्हित अक्षर	*buf = bufv;
	ssize_t		really_read = 0;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णांक		align_आकार, offset;

	आकार = (गिनती < 0) ? -गिनती : (ext2_loff_t) गिनती * channel->block_आकार;
	mutex_lock(तथ्य, STATS_MTX);
	तथ्य->io_stats.bytes_read += आकार;
	mutex_unlock(तथ्य, STATS_MTX);
	location = ((ext2_loff_t) block * channel->block_आकार) + तथ्य->offset;

	अगर (तथ्य->flags & IO_FLAG_FORCE_BOUNCE)
		जाओ bounce_read;

#अगर_घोषित HAVE_PREAD64
	/* Try an aligned pread */
	अगर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		actual = pread64(तथ्य->dev, buf, आकार, location);
		अगर (actual == आकार)
			वापस 0;
		actual = 0;
	}
#या_अगर HAVE_PREAD
	/* Try an aligned pread */
	अगर ((माप(off_t) >= माप(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(आकार, channel->align)))) {
		actual = pread(तथ्य->dev, buf, आकार, location);
		अगर (actual == आकार)
			वापस 0;
		actual = 0;
	}
#पूर्ण_अगर /* HAVE_PREAD */

	अगर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		mutex_lock(तथ्य, BOUNCE_MTX);
		अगर (ext2fs_llseek(तथ्य->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जाओ error_unlock;
		}
		actual = पढ़ो(तथ्य->dev, buf, आकार);
		अगर (actual != आकार) {
		short_read:
			अगर (actual < 0) {
				retval = त्रुटि_सं;
				actual = 0;
			} अन्यथा
				retval = EXT2_ET_SHORT_READ;
			जाओ error_unlock;
		}
		जाओ success_unlock;
	}

#अगर_घोषित ALIGN_DEBUG
	म_लिखो("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिन्हित दीर्घ) आकार);
#पूर्ण_अगर

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	अगर (channel->align == 0)
		channel->align = 1;
	अगर ((channel->block_आकार > channel->align) &&
	    (channel->block_आकार % channel->align) == 0)
		align_आकार = channel->block_आकार;
	अन्यथा
		align_आकार = channel->align;
	aligned_blk = location / align_आकार;
	offset = location % align_आकार;

	mutex_lock(तथ्य, BOUNCE_MTX);
	अगर (ext2fs_llseek(तथ्य->dev, aligned_blk * align_आकार, शुरू_से) < 0) {
		retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
		जाओ error_unlock;
	}
	जबतक (आकार > 0) {
		actual = पढ़ो(तथ्य->dev, तथ्य->bounce, align_आकार);
		अगर (actual != align_आकार) {
			actual = really_read;
			buf -= really_read;
			आकार += really_read;
			जाओ short_read;
		}
		अगर ((actual + offset) > align_आकार)
			actual = align_आकार - offset;
		अगर (actual > आकार)
			actual = आकार;
		स_नकल(buf, (अक्षर *)तथ्य->bounce + offset, actual);

		really_read += actual;
		आकार -= actual;
		buf += actual;
		offset = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(तथ्य, BOUNCE_MTX);
	वापस 0;

error_unlock:
	mutex_unlock(तथ्य, BOUNCE_MTX);
	अगर (actual >= 0 && actual < आकार)
		स_रखो((अक्षर *) buf+actual, 0, आकार-actual);
	अगर (channel->read_error)
		retval = (channel->read_error)(channel, block, गिनती, buf,
					       आकार, actual, retval);
	वापस retval;
}

#घोषणा RAW_WRITE_NO_HANDLER	(1U << 0)
#घोषणा RAW_WRITE_NOLOCK	(1U << 1)

अटल errcode_t raw_write_blk(io_channel channel,
			       काष्ठा unix_private_तथ्य *तथ्य,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, स्थिर व्योम *bufv,
			       पूर्णांक flags)
{
	ssize_t		आकार;
	ext2_loff_t	location;
	पूर्णांक		actual = 0;
	errcode_t	retval;
	स्थिर अचिन्हित अक्षर *buf = bufv;
	अचिन्हित दीर्घ दीर्घ aligned_blk;
	पूर्णांक		align_आकार, offset;

	अगर (गिनती == 1)
		आकार = channel->block_आकार;
	अन्यथा {
		अगर (गिनती < 0)
			आकार = -गिनती;
		अन्यथा
			आकार = (ext2_loff_t) गिनती * channel->block_आकार;
	}
	mutex_lock(तथ्य, STATS_MTX);
	तथ्य->io_stats.bytes_written += आकार;
	mutex_unlock(तथ्य, STATS_MTX);

	location = ((ext2_loff_t) block * channel->block_आकार) + तथ्य->offset;

	अगर (तथ्य->flags & IO_FLAG_FORCE_BOUNCE)
		जाओ bounce_write;

#अगर_घोषित HAVE_PWRITE64
	/* Try an aligned pwrite */
	अगर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		actual = pwrite64(तथ्य->dev, buf, आकार, location);
		अगर (actual == आकार)
			वापस 0;
	}
#या_अगर HAVE_PWRITE
	/* Try an aligned pwrite */
	अगर ((माप(off_t) >= माप(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(आकार, channel->align)))) {
		actual = pwrite(तथ्य->dev, buf, आकार, location);
		अगर (actual == आकार)
			वापस 0;
	}
#पूर्ण_अगर /* HAVE_PWRITE */

	अगर ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(आकार, channel->align))) {
		अगर (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(तथ्य, BOUNCE_MTX);
		अगर (ext2fs_llseek(तथ्य->dev, location, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जाओ error_unlock;
		}
		actual = ग_लिखो(तथ्य->dev, buf, आकार);
		अगर (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(तथ्य, BOUNCE_MTX);
		अगर (actual < 0) {
			retval = त्रुटि_सं;
			जाओ error_out;
		}
		अगर (actual != आकार) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			जाओ error_out;
		}
		वापस 0;
	}

#अगर_घोषित ALIGN_DEBUG
	म_लिखो("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (अचिन्हित दीर्घ) आकार);
#पूर्ण_अगर
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	अगर (channel->align == 0)
		channel->align = 1;
	अगर ((channel->block_आकार > channel->align) &&
	    (channel->block_आकार % channel->align) == 0)
		align_आकार = channel->block_आकार;
	अन्यथा
		align_आकार = channel->align;
	aligned_blk = location / align_आकार;
	offset = location % align_आकार;

	जबतक (आकार > 0) {
		पूर्णांक actual_w;

		अगर (!(flags & RAW_WRITE_NOLOCK))
			mutex_lock(तथ्य, BOUNCE_MTX);
		अगर (आकार < align_आकार || offset) {
			अगर (ext2fs_llseek(तथ्य->dev, aligned_blk * align_आकार,
					  शुरू_से) < 0) {
				retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
				जाओ error_unlock;
			}
			actual = पढ़ो(तथ्य->dev, तथ्य->bounce,
				      align_आकार);
			अगर (actual != align_आकार) {
				अगर (actual < 0) {
					retval = त्रुटि_सं;
					जाओ error_unlock;
				}
				स_रखो((अक्षर *) तथ्य->bounce + actual, 0,
				       align_आकार - actual);
			}
		}
		actual = आकार;
		अगर ((actual + offset) > align_आकार)
			actual = align_आकार - offset;
		अगर (actual > आकार)
			actual = आकार;
		स_नकल(((अक्षर *)तथ्य->bounce) + offset, buf, actual);
		अगर (ext2fs_llseek(तथ्य->dev, aligned_blk * align_आकार, शुरू_से) < 0) {
			retval = त्रुटि_सं ? त्रुटि_सं : EXT2_ET_LLSEEK_FAILED;
			जाओ error_unlock;
		}
		actual_w = ग_लिखो(तथ्य->dev, तथ्य->bounce, align_आकार);
		अगर (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(तथ्य, BOUNCE_MTX);
		अगर (actual_w < 0) {
			retval = त्रुटि_सं;
			जाओ error_out;
		}
		अगर (actual_w != align_आकार)
			जाओ short_write;
		आकार -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		offset = 0;
	}
	वापस 0;

error_unlock:
	अगर (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(तथ्य, BOUNCE_MTX);
error_out:
	अगर (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, block, गिनती, buf,
						आकार, actual, retval);
	वापस retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
अटल errcode_t alloc_cache(io_channel channel,
			     काष्ठा unix_private_तथ्य *तथ्य)
{
	errcode_t		retval;
	काष्ठा unix_cache	*cache;
	अचिन्हित पूर्णांक		i;

	तथ्य->access_time = 0;
	क्रम (i=0, cache = तथ्य->cache; i < तथ्य->cache_आकार; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		अगर (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		अगर (retval)
			वापस retval;
	}
	अगर (channel->align || तथ्य->flags & IO_FLAG_FORCE_BOUNCE) {
		अगर (तथ्य->bounce)
			ext2fs_free_mem(&तथ्य->bounce);
		retval = io_channel_alloc_buf(channel, 0, &तथ्य->bounce);
	}
	वापस retval;
}

/* Free the cache buffers */
अटल व्योम free_cache(काष्ठा unix_private_तथ्य *तथ्य)
{
	काष्ठा unix_cache	*cache;
	अचिन्हित पूर्णांक		i;

	तथ्य->access_time = 0;
	क्रम (i=0, cache = तथ्य->cache; i < तथ्य->cache_आकार; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		अगर (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	अगर (तथ्य->bounce)
		ext2fs_free_mem(&तथ्य->bounce);
}

#अगर_अघोषित NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#घोषणा GOLDEN_RATIO_अभाज्य	0x9e37fffffffc0001UL
#अगर_अघोषित CACHE_पंक्ति_आकार
/* if the system didn't tell us, guess something reasonable */
#घोषणा CACHE_पंक्ति_आकार		64
#पूर्ण_अगर

/* buffer cache hashing function, crudely stolen from xfsprogs */
अटल अचिन्हित पूर्णांक
cache_hash(काष्ठा unix_private_तथ्य *तथ्य, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	अगर (तथ्य->cache_आकार <= DEFAULT_CACHE_आकार)
		वापस 0;

	tmp = hashval ^ (GOLDEN_RATIO_अभाज्य + hashval) / CACHE_पंक्ति_आकार;
	tmp = tmp ^ ((tmp ^ GOLDEN_RATIO_अभाज्य) >> तथ्य->cache_hash_shift);
	वापस tmp % तथ्य->cache_आकार;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
अटल काष्ठा unix_cache *find_cached_block(काष्ठा unix_private_तथ्य *तथ्य,
					    अचिन्हित दीर्घ दीर्घ block,
					    काष्ठा unix_cache **eldest)
{
	काष्ठा unix_cache	*cache, *unused_cache, *oldest_cache;
	अचिन्हित पूर्णांक		hash = cache_hash(तथ्य, block);
	अचिन्हित पूर्णांक		i;

	unused_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	क्रम (i = hash, cache = तथ्य->cache + hash;
	     i < तथ्य->cache_आकार;
	     i++, cache++) {
		अगर (!cache->in_use) {
			अगर (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		अगर (cache->block == block) {
			cache->access_time = ++तथ्य->access_time;
			तथ्य->io_stats.cache_hits++;
			वापस cache;
		}
		अगर (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	क्रम (i = 0, cache = तथ्य->cache; i < hash; i++, cache++) {
		अगर (!cache->in_use) {
			अगर (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		अगर (cache->block == block) {
			cache->access_time = ++तथ्य->access_time;
			तथ्य->io_stats.cache_hits++;
			वापस cache;
		}
		अगर (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	अगर (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	तथ्य->io_stats.cache_misses++;
	वापस 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
अटल errcode_t reuse_cache(io_channel channel,
		काष्ठा unix_private_तथ्य *तथ्य, काष्ठा unix_cache *cache,
		अचिन्हित दीर्घ दीर्घ block)
{
	अगर (cache->dirty && cache->in_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, तथ्य, cache->block, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		अगर (retval) {
			cache->write_err = 1;
			वापस retval;
		}
	}

	cache->in_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->block = block;
	cache->access_time = ++तथ्य->access_time;
	वापस 0;
}

#घोषणा FLUSH_INVALIDATE	0x01
#घोषणा FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
अटल errcode_t flush_cached_blocks(io_channel channel,
				     काष्ठा unix_private_तथ्य *तथ्य,
				     पूर्णांक flags)
{
	काष्ठा unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	अचिन्हित पूर्णांक		i;
	पूर्णांक			errors_found = 0;

	अगर ((flags & FLUSH_NOLOCK) == 0)
		mutex_lock(तथ्य, CACHE_MTX);
	क्रम (i=0, cache = तथ्य->cache; i < तथ्य->cache_आकार; i++, cache++) {
		अगर (!cache->in_use)
			जारी;
		अगर (cache->dirty) {
			पूर्णांक raw_flags = RAW_WRITE_NO_HANDLER;

			अगर (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, तथ्य,
					       cache->block, 1, cache->buf,
					       raw_flags);
			अगर (retval) {
				cache->write_err = 1;
				errors_found = 1;
				retval2 = retval;
			} अन्यथा {
				cache->dirty = 0;
				cache->write_err = 0;
				अगर (flags & FLUSH_INVALIDATE)
					cache->in_use = 0;
			}
		} अन्यथा अगर (flags & FLUSH_INVALIDATE) {
			cache->in_use = 0;
		}
	}
	अगर ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(तथ्य, CACHE_MTX);
retry:
	जबतक (errors_found) {
		अगर ((flags & FLUSH_NOLOCK) == 0)
			mutex_lock(तथ्य, CACHE_MTX);
		errors_found = 0;
		क्रम (i=0, cache = तथ्य->cache; i < तथ्य->cache_आकार; i++, cache++) {
			अगर (!cache->in_use || !cache->write_err)
				जारी;
			errors_found = 1;
			अगर (cache->write_err && channel->write_error) {
				अक्षर *err_buf = शून्य;
				अचिन्हित दीर्घ दीर्घ err_block = cache->block;

				cache->dirty = 0;
				cache->in_use = 0;
				cache->write_err = 0;
				अगर (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = शून्य;
				अन्यथा
					स_नकल(err_buf, cache->buf,
					       channel->block_आकार);
				mutex_unlock(तथ्य, CACHE_MTX);
				(channel->write_error)(channel, err_block,
					1, err_buf, channel->block_आकार, -1,
					retval2);
				अगर (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_lock(तथ्य, CACHE_MTX);
				जाओ retry;
			} अन्यथा
				cache->write_err = 0;
		}
		अगर ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(तथ्य, CACHE_MTX);
	}
	वापस retval2;
}

/* Shrink the cache buffers */
अटल errcode_t shrink_cache(io_channel channel,
			      काष्ठा unix_private_तथ्य *तथ्य,
			      अचिन्हित पूर्णांक new_आकार)
{
	काष्ठा unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णांक		i;
	errcode_t		retval;

	mutex_lock(तथ्य, CACHE_MTX);

	retval = flush_cached_blocks(channel, तथ्य,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	अगर (retval)
		जाओ unlock;

	क्रम (i = new_आकार, cache = तथ्य->cache + new_आकार;
	     i < तथ्य->cache_आकार;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		अगर (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = पुनः_स्मृति(तथ्य->cache, new_आकार * माप(काष्ठा unix_cache));
	अगर (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		जाओ unlock;
	}

	तथ्य->cache = new_cache;
	तथ्य->cache_आकार = new_आकार;
	तथ्य->cache_hash_shift = highbit32(तथ्य->cache_आकार);

unlock:
	mutex_unlock(तथ्य, CACHE_MTX);
	वापस retval;
}

/* Grow the cache buffers */
अटल errcode_t grow_cache(io_channel channel,
			    काष्ठा unix_private_तथ्य *तथ्य,
			    अचिन्हित पूर्णांक new_आकार)
{
	काष्ठा unix_cache	*cache, *new_cache;
	अचिन्हित पूर्णांक		i;
	errcode_t		retval;

	mutex_lock(तथ्य, CACHE_MTX);

	retval = flush_cached_blocks(channel, तथ्य,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	अगर (retval)
		जाओ unlock;

	new_cache = पुनः_स्मृति(तथ्य->cache, new_आकार * माप(काष्ठा unix_cache));
	अगर (!new_cache) {
		retval = EXT2_ET_NO_MEMORY;
		जाओ unlock;
	}
	तथ्य->cache = new_cache;

	क्रम (i = तथ्य->cache_आकार, cache = new_cache + तथ्य->cache_आकार;
	     i < new_आकार;
	     i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		अगर (retval) {
			जबतक (i > तथ्य->cache_आकार) {
				cache--; i--;
				अगर (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			जाओ unlock;
		}
	}

	तथ्य->cache_आकार = new_आकार;
	तथ्य->cache_hash_shift = highbit32(तथ्य->cache_आकार);

unlock:
	mutex_unlock(तथ्य, CACHE_MTX);
	वापस retval;
}
#पूर्ण_अगर /* NO_IO_CACHE */

#अगर_घोषित __linux__
#अगर_अघोषित BLKDISCARDZEROES
#घोषणा BLKDISCARDZEROES _IO(0x12,124)
#पूर्ण_अगर
#पूर्ण_अगर

पूर्णांक ext2fs_open_file(स्थिर अक्षर *pathname, पूर्णांक flags, mode_t mode)
{
	अगर (mode)
#अगर defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		वापस open64(pathname, flags, mode);
	अन्यथा
		वापस open64(pathname, flags);
#अन्यथा
		वापस खोलो(pathname, flags, mode);
	अन्यथा
		वापस खोलो(pathname, flags);
#पूर्ण_अगर
}

पूर्णांक ext2fs_stat(स्थिर अक्षर *path, ext2fs_struct_stat *buf)
{
#अगर defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	वापस stat64(path, buf);
#अन्यथा
	वापस stat(path, buf);
#पूर्ण_अगर
}

पूर्णांक ext2fs_fstat(पूर्णांक fd, ext2fs_struct_stat *buf)
{
#अगर defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	वापस fstat64(fd, buf);
#अन्यथा
	वापस ख_स्थिति(fd, buf);
#पूर्ण_अगर
}

#अगर_घोषित HAVE_SYS_FILE_H
अटल errcode_t unix_funlock(io_channel channel)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	पूर्णांक ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (तथ्य->unix_flock_flags) {
		ret = flock(तथ्य->dev, LOCK_UN);
		अगर (ret)
			वापस त्रुटि_सं;

		तथ्य->unix_flock_flags = 0;
	}

	वापस 0;
}

अटल errcode_t unix_flock(io_channel channel, अचिन्हित पूर्णांक flock_flags)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	पूर्णांक unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	अगर (ret)
		वापस ret;

	अगर (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	अगर (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	अगर (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	अगर (!unix_flock_flags)
		वापस 0;

	ret = flock(तथ्य->dev, unix_flock_flags);
	अगर (ret < 0)
		वापस त्रुटि_सं;

	तथ्य->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	वापस 0;
}
#अन्यथा
#घोषणा unix_flock		शून्य

अटल errcode_t unix_funlock(io_channel channel)
{
	वापस 0;
}
#पूर्ण_अगर /* HAVE_SYS_FILE_H */

अटल errcode_t unix_open_channel(स्थिर अक्षर *name, पूर्णांक fd,
				   पूर्णांक flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = शून्य;
	काष्ठा unix_private_तथ्य *तथ्य = शून्य;
	errcode_t	retval;
	ext2fs_struct_stat st;
#अगर_घोषित __linux__
	काष्ठा		utsname ut;
#पूर्ण_अगर

	अगर (ext2fs_safe_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#अगर_घोषित __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(व्योम) fsync(fd);
#पूर्ण_अगर

	retval = ext2fs_get_mem(माप(काष्ठा struct_io_channel), &io);
	अगर (retval)
		जाओ cleanup;
	स_रखो(io, 0, माप(काष्ठा struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(माप(काष्ठा unix_private_तथ्य), &तथ्य);
	अगर (retval)
		जाओ cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	अगर (retval)
		जाओ cleanup;

	म_नकल(io->name, name);
	io->private_तथ्य = तथ्य;
	io->block_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	अगर (ext2fs_safe_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	स_रखो(तथ्य, 0, माप(काष्ठा unix_private_तथ्य));
	तथ्य->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	तथ्य->io_stats.अंक_fields = 4;
	तथ्य->flags = flags;
	तथ्य->dev = fd;

	तथ्य->cache_आकार = DEFAULT_CACHE_आकार;
	तथ्य->cache_hash_shift = highbit32(तथ्य->cache_आकार);
	तथ्य->cache = सुस्मृति(DEFAULT_CACHE_आकार, माप(काष्ठा unix_cache));
	अगर (!तथ्य->cache) {
		retval = EXT2_ET_NO_MEMORY;
		जाओ cleanup;
	}

#अगर defined(O_DIRECT)
	अगर (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(तथ्य->dev);
#या_अगर defined(F_NOCACHE)
	अगर (flags & IO_FLAG_DIRECT_IO)
		io->align = 4096;
#पूर्ण_अगर

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	अगर (ext2fs_fstat(तथ्य->dev, &st) == 0) {
		अगर (ext2fsP_is_disk_device(st.st_mode)) {
#अगर_घोषित BLKDISCARDZEROES
			पूर्णांक zeroes = 0;

			अगर (ioctl(तथ्य->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#पूर्ण_अगर
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		} अन्यथा {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#अगर defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	अगर (!io->align)
		io->align = 512;
#पूर्ण_अगर

#अगर defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	अगर (io->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		पूर्णांक dio_align = ext2fs_get_dio_alignment(fd);

		अगर (io->align < dio_align)
			io->align = dio_align;
	}
#पूर्ण_अगर

	अगर ((retval = alloc_cache(io, तथ्य)))
		जाओ cleanup;

#अगर_घोषित BLKROGET
	अगर (flags & IO_FLAG_RW) {
		पूर्णांक error;
		पूर्णांक readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(तथ्य->dev, BLKROGET, &readonly);
		अगर (!error && readonly) {
			retval = EPERM;
			जाओ cleanup;
		}
	}
#पूर्ण_अगर

#अगर_घोषित __linux__
#अघोषित RLIM_INFINITY
#अगर (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#घोषणा RLIM_INFINITY	((अचिन्हित दीर्घ)(~0UL>>1))
#अन्यथा
#घोषणा RLIM_INFINITY  (~0UL)
#पूर्ण_अगर
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	अगर ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(तथ्य->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_mode))) {
		काष्ठा rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (अचिन्हित दीर्घ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		अगर (((अचिन्हित दीर्घ) rlim.rlim_cur) <
		    ((अचिन्हित दीर्घ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#पूर्ण_अगर
#अगर_घोषित HAVE_PTHREAD
	अगर (flags & IO_FLAG_THREADS) {
		io->flags |= CHANNEL_FLAGS_THREADS;
		retval = pthread_mutex_init(&तथ्य->cache_mutex, शून्य);
		अगर (retval)
			जाओ cleanup;
		retval = pthread_mutex_init(&तथ्य->bounce_mutex, शून्य);
		अगर (retval) {
			pthread_mutex_destroy(&तथ्य->cache_mutex);
			जाओ cleanup;
		}
		retval = pthread_mutex_init(&तथ्य->stats_mutex, शून्य);
		अगर (retval) {
			pthread_mutex_destroy(&तथ्य->cache_mutex);
			pthread_mutex_destroy(&तथ्य->bounce_mutex);
			जाओ cleanup;
		}
	}
#पूर्ण_अगर
	*channel = io;
	वापस 0;

cleanup:
	अगर (तथ्य) {
		unix_funlock(io);
		अगर (io->manager != unixfd_io_manager && तथ्य->dev >= 0)
			बंद(तथ्य->dev);
		अगर (तथ्य->cache) {
			free_cache(तथ्य);
			मुक्त(तथ्य->cache);
		}
		ext2fs_free_mem(&तथ्य);
	}
	अगर (io) {
		अगर (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	वापस retval;
}

अटल errcode_t unixfd_open(स्थिर अक्षर *str_fd, पूर्णांक flags,
			     io_channel *channel)
{
	पूर्णांक fd;
	पूर्णांक fd_flags;

	fd = म_से_प(str_fd);
#अगर defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	अगर (fd_flags == -1)
		वापस EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	अगर (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#अगर defined(O_DIRECT)
	अगर (fd_flags & O_DIRECT)
		flags |= IO_FLAG_DIRECT_IO;
#पूर्ण_अगर
#पूर्ण_अगर  /* HAVE_FCNTL */

	वापस unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

अटल errcode_t unix_open(स्थिर अक्षर *name, पूर्णांक flags,
			   io_channel *channel)
{
	पूर्णांक fd = -1;
	पूर्णांक open_flags;

	अगर (name == 0)
		वापस EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	अगर (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#अगर defined(O_DIRECT)
	अगर (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#पूर्ण_अगर
	fd = ext2fs_open_file(name, open_flags, 0);
	अगर (fd < 0)
		वापस त्रुटि_सं;
#अगर defined(F_NOCACHE) && !defined(IO_DIRECT)
	अगर (flags & IO_FLAG_DIRECT_IO) {
		अगर (fcntl(fd, F_NOCACHE, 1) < 0)
			वापस त्रुटि_सं;
	}
#पूर्ण_अगर
	वापस unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

अटल errcode_t unix_close(io_channel channel)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (--channel->refcount > 0)
		वापस 0;

#अगर_अघोषित NO_IO_CACHE
	retval = flush_cached_blocks(channel, तथ्य, 0);
#पूर्ण_अगर

	unix_funlock(channel);

	अगर (channel->manager != unixfd_io_manager && बंद(तथ्य->dev) < 0)
		retval = त्रुटि_सं;
	free_cache(तथ्य);
	मुक्त(तथ्य->cache);
#अगर_घोषित HAVE_PTHREAD
	अगर (तथ्य->flags & IO_FLAG_THREADS) {
		pthread_mutex_destroy(&तथ्य->cache_mutex);
		pthread_mutex_destroy(&तथ्य->bounce_mutex);
		pthread_mutex_destroy(&तथ्य->stats_mutex);
	}
#पूर्ण_अगर

	ext2fs_free_mem(&channel->private_तथ्य);
	अगर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	वापस retval;
}

अटल errcode_t unix_set_blksize(io_channel channel, पूर्णांक blksize)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (channel->block_आकार != blksize) {
		mutex_lock(तथ्य, CACHE_MTX);
		mutex_lock(तथ्य, BOUNCE_MTX);
#अगर_अघोषित NO_IO_CACHE
		अगर ((retval = flush_cached_blocks(channel, तथ्य, FLUSH_NOLOCK))){
			mutex_unlock(तथ्य, BOUNCE_MTX);
			mutex_unlock(तथ्य, CACHE_MTX);
			वापस retval;
		}
#पूर्ण_अगर

		channel->block_आकार = blksize;
		free_cache(तथ्य);
		retval = alloc_cache(channel, तथ्य);
		mutex_unlock(तथ्य, BOUNCE_MTX);
		mutex_unlock(तथ्य, CACHE_MTX);
	}
	वापस retval;
}

अटल errcode_t unix_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	काष्ठा unix_cache *cache;
	errcode_t	retval;
	अक्षर		*cp;
	पूर्णांक		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#अगर_घोषित NO_IO_CACHE
	वापस raw_read_blk(channel, तथ्य, block, गिनती, buf);
#अन्यथा
	अगर (तथ्य->flags & IO_FLAG_NOCACHE)
		वापस raw_read_blk(channel, तथ्य, block, गिनती, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	अगर (गिनती < 0 || गिनती > WRITE_DIRECT_आकार) {
		अगर ((retval = flush_cached_blocks(channel, तथ्य, 0)))
			वापस retval;
		वापस raw_read_blk(channel, तथ्य, block, गिनती, buf);
	}

	cp = buf;
	mutex_lock(तथ्य, CACHE_MTX);
	जबतक (गिनती > 0) {
		/* If it's in the cache, use it! */
		अगर ((cache = find_cached_block(तथ्य, block, शून्य))) {
#अगर_घोषित DEBUG
			म_लिखो("Using cached block %lu\n", block);
#पूर्ण_अगर
			स_नकल(cp, cache->buf, channel->block_आकार);
			गिनती--;
			block++;
			cp += channel->block_आकार;
			जारी;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		क्रम (i=1; i < गिनती; i++)
			अगर (find_cached_block(तथ्य, block+i, शून्य))
				अवरोध;
#अगर_घोषित DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#पूर्ण_अगर
		mutex_unlock(तथ्य, CACHE_MTX);
		अगर ((retval = raw_read_blk(channel, तथ्य, block, i, cp)))
			वापस retval;
		mutex_lock(तथ्य, CACHE_MTX);

		/* Save the results in the cache */
		क्रम (j=0; j < i; j++) {
			अगर (!find_cached_block(तथ्य, block, &cache)) {
				retval = reuse_cache(channel, तथ्य,
						     cache, block);
				अगर (retval)
					जाओ call_write_handler;
				स_नकल(cache->buf, cp, channel->block_आकार);
			}
			गिनती--;
			block++;
			cp += channel->block_आकार;
		}
	}
	mutex_unlock(तथ्य, CACHE_MTX);
	वापस 0;

call_write_handler:
	अगर (cache->write_err && channel->write_error) {
		अक्षर *err_buf = शून्य;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		अगर (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = शून्य;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_आकार);
		mutex_unlock(तथ्य, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_आकार, -1,
				       retval);
		अगर (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(तथ्य, CACHE_MTX);
	वापस retval;
#पूर्ण_अगर /* NO_IO_CACHE */
}

अटल errcode_t unix_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	वापस unix_read_blk64(channel, block, गिनती, buf);
}

अटल errcode_t unix_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	काष्ठा unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	स्थिर अक्षर	*cp;
	पूर्णांक		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#अगर_घोषित NO_IO_CACHE
	वापस raw_write_blk(channel, तथ्य, block, गिनती, buf, 0);
#अन्यथा
	अगर (तथ्य->flags & IO_FLAG_NOCACHE)
		वापस raw_write_blk(channel, तथ्य, block, गिनती, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	अगर (गिनती < 0 || गिनती > WRITE_DIRECT_आकार) {
		अगर ((retval = flush_cached_blocks(channel, तथ्य,
						  FLUSH_INVALIDATE)))
			वापस retval;
		वापस raw_write_blk(channel, तथ्य, block, गिनती, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	अगर (writethrough)
		retval = raw_write_blk(channel, तथ्य, block, गिनती, buf, 0);

	cp = buf;
	mutex_lock(तथ्य, CACHE_MTX);
	जबतक (गिनती > 0) {
		cache = find_cached_block(तथ्य, block, &reuse);
		अगर (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, तथ्य, cache, block);
			अगर (err)
				जाओ call_write_handler;
		}
		अगर (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_आकार);
		cache->dirty = !writethrough;
		गिनती--;
		block++;
		cp += channel->block_आकार;
	}
	mutex_unlock(तथ्य, CACHE_MTX);
	वापस retval;

call_write_handler:
	अगर (cache->write_err && channel->write_error) {
		अक्षर *err_buf = शून्य;
		अचिन्हित दीर्घ दीर्घ err_block = cache->block;

		cache->dirty = 0;
		cache->in_use = 0;
		cache->write_err = 0;
		अगर (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = शून्य;
		अन्यथा
			स_नकल(err_buf, cache->buf, channel->block_आकार);
		mutex_unlock(तथ्य, CACHE_MTX);
		(channel->write_error)(channel, err_block, 1, err_buf,
				       channel->block_आकार, -1,
				       retval);
		अगर (err_buf)
			ext2fs_free_mem(&err_buf);
	} अन्यथा
		mutex_unlock(तथ्य, CACHE_MTX);
	वापस retval;
#पूर्ण_अगर /* NO_IO_CACHE */
}

अटल errcode_t unix_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
#अगर_घोषित POSIX_FADV_WILLNEED
	काष्ठा unix_private_तथ्य *तथ्य;

	तथ्य = (काष्ठा unix_private_तथ्य *)channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	वापस posix_fadvise(तथ्य->dev,
			     (ext2_loff_t)block * channel->block_आकार + तथ्य->offset,
			     (ext2_loff_t)गिनती * channel->block_आकार,
			     POSIX_FADV_WILLNEED);
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

अटल errcode_t unix_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	वापस unix_write_blk64(channel, block, गिनती, buf);
}

अटल errcode_t unix_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक आकार, स्थिर व्योम *buf)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (channel->align != 0) {
#अगर_घोषित ALIGN_DEBUG
		म_लिखो("unix_write_byte: O_DIRECT fallback\n");
#पूर्ण_अगर
		वापस EXT2_ET_UNIMPLEMENTED;
	}

#अगर_अघोषित NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	अगर ((retval = flush_cached_blocks(channel, तथ्य, FLUSH_INVALIDATE)))
		वापस retval;
#पूर्ण_अगर

	अगर (lseek(तथ्य->dev, offset + तथ्य->offset, शुरू_से) < 0)
		वापस त्रुटि_सं;

	actual = ग_लिखो(तथ्य->dev, buf, आकार);
	अगर (actual < 0)
		वापस त्रुटि_सं;
	अगर (actual != आकार)
		वापस EXT2_ET_SHORT_WRITE;

	वापस 0;
}

/*
 * Flush data buffers to disk.
 */
अटल errcode_t unix_flush(io_channel channel)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#अगर_अघोषित NO_IO_CACHE
	retval = flush_cached_blocks(channel, तथ्य, 0);
#पूर्ण_अगर
#अगर_घोषित HAVE_FSYNC
	अगर (!retval && fsync(तथ्य->dev) != 0)
		वापस त्रुटि_सं;
#पूर्ण_अगर
	वापस retval;
}

अटल errcode_t unix_set_option(io_channel channel, स्थिर अक्षर *option,
				 स्थिर अक्षर *arg)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	अचिन्हित दीर्घ दीर्घ tmp;
	errcode_t retval;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (!म_भेद(option, "offset")) {
		अगर (!arg)
			वापस EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		अगर (*end)
			वापस EXT2_ET_INVALID_ARGUMENT;
		तथ्य->offset = tmp;
		अगर (तथ्य->offset < 0)
			वापस EXT2_ET_INVALID_ARGUMENT;
		वापस 0;
	}
	अगर (!म_भेद(option, "cache")) {
		अगर (!arg)
			वापस EXT2_ET_INVALID_ARGUMENT;
		अगर (!म_भेद(arg, "on")) {
			तथ्य->flags &= ~IO_FLAG_NOCACHE;
			वापस 0;
		}
		अगर (!म_भेद(arg, "off")) {
			retval = flush_cached_blocks(channel, तथ्य, 0);
			तथ्य->flags |= IO_FLAG_NOCACHE;
			वापस retval;
		}
		वापस EXT2_ET_INVALID_ARGUMENT;
	}
#अगर_अघोषित NO_IO_CACHE
	अगर (!म_भेद(option, "cache_blocks")) {
		अचिन्हित दीर्घ दीर्घ	आकार;

		अगर (!arg)
			वापस EXT2_ET_INVALID_ARGUMENT;

		त्रुटि_सं = 0;
		आकार = strtoll(arg, शून्य, 0);
		अगर (त्रुटि_सं || आकार == 0 || आकार > INT32_MAX)
			वापस EXT2_ET_INVALID_ARGUMENT;

		अगर (तथ्य->cache_आकार == आकार)
			वापस 0;
		अगर (तथ्य->cache_आकार > आकार)
			वापस shrink_cache(channel, तथ्य, आकार);
		वापस grow_cache(channel, तथ्य, आकार);
	}
#पूर्ण_अगर
	वापस EXT2_ET_INVALID_ARGUMENT;
}

#अगर defined(__linux__) && !defined(BLKDISCARD)
#घोषणा BLKDISCARD		_IO(0x12,119)
#पूर्ण_अगर

अटल errcode_t unix_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	पूर्णांक		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (channel->flags & CHANNEL_FLAGS_NODISCARD)
		जाओ unimplemented;

	अगर (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
#अगर_घोषित BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(block) * channel->block_आकार + तथ्य->offset;
		range[1] = (__u64)(गिनती) * channel->block_आकार;

		ret = ioctl(तथ्य->dev, BLKDISCARD, &range);
#अन्यथा
		जाओ unimplemented;
#पूर्ण_अगर
	} अन्यथा {
#अगर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(तथ्य->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(block) * channel->block_आकार + तथ्य->offset,
				(off_t)(गिनती) * channel->block_आकार);
#अन्यथा
		जाओ unimplemented;
#पूर्ण_अगर
	}
	अगर (ret < 0) {
		अगर (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			जाओ unimplemented;
		}
		वापस त्रुटि_सं;
	}
	वापस 0;
unimplemented:
	वापस EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
अटल पूर्णांक __unix_zeroout(पूर्णांक fd, off_t offset, off_t len)
{
	पूर्णांक ret = -1;

#अगर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, offset, len);
	अगर (ret == 0)
		वापस 0;
#पूर्ण_अगर
#अगर defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			offset,  len);
	अगर (ret == 0)
		वापस 0;
#पूर्ण_अगर
	त्रुटि_सं = EOPNOTSUPP;
	वापस ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wunused-parameter"
#पूर्ण_अगर
अटल errcode_t unix_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा unix_private_तथ्य *तथ्य;
	पूर्णांक		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा unix_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (!(channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		काष्ठा stat statbuf;

		अगर (गिनती == 0)
			वापस 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = ख_स्थिति(तथ्य->dev, &statbuf);
		अगर (ret)
			जाओ err;
		अगर ((अचिन्हित दीर्घ दीर्घ) statbuf.st_आकार <
			(block + गिनती) * channel->block_आकार + तथ्य->offset) {
			ret = ftruncate(तथ्य->dev,
					(block + गिनती) * channel->block_आकार + तथ्य->offset);
			अगर (ret)
				जाओ err;
		}
	}

	अगर (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		जाओ unimplemented;

	ret = __unix_zeroout(तथ्य->dev,
			(off_t)(block) * channel->block_आकार + तथ्य->offset,
			(off_t)(गिनती) * channel->block_आकार);
err:
	अगर (ret < 0) {
		अगर (त्रुटि_सं == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			जाओ unimplemented;
		}
		वापस त्रुटि_सं;
	}
	वापस 0;
unimplemented:
	वापस EXT2_ET_UNIMPLEMENTED;
}
#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर

अटल काष्ठा struct_io_manager struct_unix_manager = {
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

अटल काष्ठा struct_io_manager struct_unixfd_manager = {
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
