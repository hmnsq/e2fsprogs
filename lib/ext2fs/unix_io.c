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

#ఒకవేళ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#నిర్వచించు XOPEN_ఆధారం 600
#నిర్వచించు DARWIN_C_ఆధారం
#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే GNU_ఆధారం
#నిర్వచించు GNU_ఆధారం
#అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#నిర్వచితమైతే __linux__
#అన్తర్భావ <sys/utsname.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_IOCTL_H
#అన్తర్భావ <sys/ioctl.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_MOUNT_H
#అన్తర్భావ <sys/mount.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_RESOURCE_H
#అన్తర్భావ <sys/resource.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_LINUX_FALLOC_H
#అన్తర్భావ <linux/falloc.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_PTHREAD
#అన్తర్భావ <pthread.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_FILE_H
#అన్తర్భావ <sys/file.h>
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#నిర్వచించు BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#అయితే_ముగింపు

#నిర్వచనం_రద్దు ALIGN_DEBUG

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

స్థిర అంతఃవరుస పూర్ణాంకం కనుగొను_చివరి_బిట్టు_సమితి(పూర్ణాంకం x)
{
	పూర్ణాంకం r = 32;

	ఒకవేళ (!x)
		తిరిగి 0;
	ఒకవేళ (!(x & 0xffff0000u)) {
		x = (x & 0xffffu) << 16;
		r -= 16;
	}
	ఒకవేళ (!(x & 0xff000000u)) {
		x = (x & 0xffffffu) << 8;
		r -= 8;
	}
	ఒకవేళ (!(x & 0xf0000000u)) {
		x = (x & 0xfffffffu) << 4;
		r -= 4;
	}
	ఒకవేళ (!(x & 0xc0000000u)) {
		x = (x & 0x3fffffffu) << 2;
		r -= 2;
	}
	ఒకవేళ (!(x & 0x80000000u)) {
		r -= 1;
	}
	తిరిగి r;
}

/* Get high bit set out of 32-bit argument, -1 if none set */
స్థిర అంతఃవరుస పూర్ణాంకం highbit32(uint32_t v)
{
	తిరిగి కనుగొను_చివరి_బిట్టు_సమితి(v) - 1;
}

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

నిర్మాణం unix_cache {
	అక్షరం			*buf;
	గుర్తులేని దీర్ఘ దీర్ఘ	ఖండం;
	పూర్ణాంకం			access_సమయం;
	గుర్తులేని		dirty:1;
	గుర్తులేని		లోపల_use:1;
	గుర్తులేని		write_err:1;
};

#నిర్వచించు DEFAULT_CACHE_కొలత 8
#నిర్వచించు WRITE_ప్రత్యక్ష_కొలత 4	/* Must be smaller than CACHE_SIZE */
#నిర్వచించు READ_ప్రత్యక్ష_కొలత 4	/* Should be smaller than CACHE_SIZE */

నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం	magic;
	పూర్ణాంకం	dev;
	పూర్ణాంకం	flags;
	పూర్ణాంకం	align;
	పూర్ణాంకం	access_సమయం;
	పూర్ణాంకం	unix_flock_flags;
	ext2_loff_t స్థానభ్రంశం;
	నిర్మాణం unix_cache *cache;
	గుర్తులేని పూర్ణాంకం cache_కొలత;
	గుర్తులేని పూర్ణాంకం cache_hash_జరుపు;
	శూన్య	*bounce;
	నిర్మాణం struct_io_stats io_stats;
#నిర్వచితమైతే HAVE_PTHREAD
	pthread_mutex_t cache_mutex;
	pthread_mutex_t bounce_mutex;
	pthread_mutex_t stats_mutex;
#అయితే_ముగింపు
};

#నిర్వచించు IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

రకనిర్వచనం గణన తాళం_రకం {
	CACHE_MTX, BOUNCE_MTX, STATS_MTX
} రకం_t;

#నిర్వచితమైతే HAVE_PTHREAD
స్థిర అంతఃవరుస pthread_mutex_t *get_mutex(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
					 రకం_t రకం)
{
	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_దారాలు) {
		ఎంపిక (రకం) {
		స్థితి CACHE_MTX:
			తిరిగి &దత్తాంశం->cache_mutex;
		స్థితి BOUNCE_MTX:
			తిరిగి &దత్తాంశం->bounce_mutex;
		స్థితి STATS_MTX:
			తిరిగి &దత్తాంశం->stats_mutex;
		}
	}
	తిరిగి ఖాళీ;
}
#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య mutex_తాళం(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, రకం_t రకం)
{
#నిర్వచితమైతే HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(దత్తాంశం,రకం);

	ఒకవేళ (mtx)
		pthread_mutex_తాళం(mtx);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య mutex_unlock(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, రకం_t రకం)
{
#నిర్వచితమైతే HAVE_PTHREAD
	pthread_mutex_t *mtx = get_mutex(దత్తాంశం,రకం);

	ఒకవేళ (mtx)
		pthread_mutex_unlock(mtx);
#అయితే_ముగింపు
}

స్థిర errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (stats) {
		mutex_తాళం(దత్తాంశం, STATS_MTX);
		*stats = &దత్తాంశం->io_stats;
		mutex_unlock(దత్తాంశం, STATS_MTX);
	}

	తిరిగి retval;
}

/*
 * Here are the raw I/O functions
 */
స్థిర errcode_t raw_read_blk(io_channel channel,
			      నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      పూర్ణాంకం లెక్క, శూన్య *bufv)
{
	errcode_t	retval;
	ssize_t		కొలత;
	ext2_loff_t	location;
	పూర్ణాంకం		actual = 0;
	గుర్తులేని అక్షరం	*buf = bufv;
	ssize_t		really_read = 0;
	గుర్తులేని దీర్ఘ దీర్ఘ aligned_blk;
	పూర్ణాంకం		align_కొలత, స్థానభ్రంశం;

	కొలత = (లెక్క < 0) ? -లెక్క : (ext2_loff_t) లెక్క * channel->ఖండం_కొలత;
	mutex_తాళం(దత్తాంశం, STATS_MTX);
	దత్తాంశం->io_stats.బైట్లు_read += కొలత;
	mutex_unlock(దత్తాంశం, STATS_MTX);
	location = ((ext2_loff_t) ఖండం * channel->ఖండం_కొలత) + దత్తాంశం->స్థానభ్రంశం;

	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_FORCE_BOUNCE)
		వెళ్ళు bounce_read;

#నిర్వచితమైతే HAVE_PREAD64
	/* Try an aligned pread */
	ఒకవేళ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(కొలత, channel->align))) {
		actual = pread64(దత్తాంశం->dev, buf, కొలత, location);
		ఒకవేళ (actual == కొలత)
			తిరిగి 0;
		actual = 0;
	}
#లేదావేళ HAVE_PREAD
	/* Try an aligned pread */
	ఒకవేళ ((పరిమాణం(off_t) >= పరిమాణం(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(కొలత, channel->align)))) {
		actual = pread(దత్తాంశం->dev, buf, కొలత, location);
		ఒకవేళ (actual == కొలత)
			తిరిగి 0;
		actual = 0;
	}
#అయితే_ముగింపు /* HAVE_PREAD */

	ఒకవేళ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(కొలత, channel->align))) {
		mutex_తాళం(దత్తాంశం, BOUNCE_MTX);
		ఒకవేళ (ext2fs_llseek(దత్తాంశం->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			వెళ్ళు error_unlock;
		}
		actual = పఠ(దత్తాంశం->dev, buf, కొలత);
		ఒకవేళ (actual != కొలత) {
		short_read:
			ఒకవేళ (actual < 0) {
				retval = errno;
				actual = 0;
			} లేకపోతే
				retval = EXT2_ET_SHORT_READ;
			వెళ్ళు error_unlock;
		}
		వెళ్ళు success_unlock;
	}

#నిర్వచితమైతే ALIGN_DEBUG
	రాయి("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (గుర్తులేని దీర్ఘ) కొలత);
#అయితే_ముగింపు

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ఒకవేళ (channel->align == 0)
		channel->align = 1;
	ఒకవేళ ((channel->ఖండం_కొలత > channel->align) &&
	    (channel->ఖండం_కొలత % channel->align) == 0)
		align_కొలత = channel->ఖండం_కొలత;
	లేకపోతే
		align_కొలత = channel->align;
	aligned_blk = location / align_కొలత;
	స్థానభ్రంశం = location % align_కొలత;

	mutex_తాళం(దత్తాంశం, BOUNCE_MTX);
	ఒకవేళ (ext2fs_llseek(దత్తాంశం->dev, aligned_blk * align_కొలత, SEEK_SET) < 0) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		వెళ్ళు error_unlock;
	}
	అయితే (కొలత > 0) {
		actual = పఠ(దత్తాంశం->dev, దత్తాంశం->bounce, align_కొలత);
		ఒకవేళ (actual != align_కొలత) {
			actual = really_read;
			buf -= really_read;
			కొలత += really_read;
			వెళ్ళు short_read;
		}
		ఒకవేళ ((actual + స్థానభ్రంశం) > align_కొలత)
			actual = align_కొలత - స్థానభ్రంశం;
		ఒకవేళ (actual > కొలత)
			actual = కొలత;
		జ్ఞాపకం_నకలు(buf, (అక్షరం *)దత్తాంశం->bounce + స్థానభ్రంశం, actual);

		really_read += actual;
		కొలత -= actual;
		buf += actual;
		స్థానభ్రంశం = 0;
		aligned_blk++;
	}
success_unlock:
	mutex_unlock(దత్తాంశం, BOUNCE_MTX);
	తిరిగి 0;

error_unlock:
	mutex_unlock(దత్తాంశం, BOUNCE_MTX);
	ఒకవేళ (actual >= 0 && actual < కొలత)
		memset((అక్షరం *) buf+actual, 0, కొలత-actual);
	ఒకవేళ (channel->read_error)
		retval = (channel->read_error)(channel, ఖండం, లెక్క, buf,
					       కొలత, actual, retval);
	తిరిగి retval;
}

#నిర్వచించు RAW_WRITE_NO_HANDLER	(1U << 0)
#నిర్వచించు RAW_WRITE_NOLOCK	(1U << 1)

స్థిర errcode_t raw_write_blk(io_channel channel,
			       నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			       గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *bufv,
			       పూర్ణాంకం flags)
{
	ssize_t		కొలత;
	ext2_loff_t	location;
	పూర్ణాంకం		actual = 0;
	errcode_t	retval;
	స్థిరరాశి గుర్తులేని అక్షరం *buf = bufv;
	గుర్తులేని దీర్ఘ దీర్ఘ aligned_blk;
	పూర్ణాంకం		align_కొలత, స్థానభ్రంశం;

	ఒకవేళ (లెక్క == 1)
		కొలత = channel->ఖండం_కొలత;
	లేకపోతే {
		ఒకవేళ (లెక్క < 0)
			కొలత = -లెక్క;
		లేకపోతే
			కొలత = (ext2_loff_t) లెక్క * channel->ఖండం_కొలత;
	}
	mutex_తాళం(దత్తాంశం, STATS_MTX);
	దత్తాంశం->io_stats.బైట్లు_written += కొలత;
	mutex_unlock(దత్తాంశం, STATS_MTX);

	location = ((ext2_loff_t) ఖండం * channel->ఖండం_కొలత) + దత్తాంశం->స్థానభ్రంశం;

	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_FORCE_BOUNCE)
		వెళ్ళు bounce_write;

#నిర్వచితమైతే HAVE_PWRITE64
	/* Try an aligned pwrite */
	ఒకవేళ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(కొలత, channel->align))) {
		actual = pwrite64(దత్తాంశం->dev, buf, కొలత, location);
		ఒకవేళ (actual == కొలత)
			తిరిగి 0;
	}
#లేదావేళ HAVE_PWRITE
	/* Try an aligned pwrite */
	ఒకవేళ ((పరిమాణం(off_t) >= పరిమాణం(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(location, channel->align) &&
	      IS_ALIGNED(కొలత, channel->align)))) {
		actual = pwrite(దత్తాంశం->dev, buf, కొలత, location);
		ఒకవేళ (actual == కొలత)
			తిరిగి 0;
	}
#అయితే_ముగింపు /* HAVE_PWRITE */

	ఒకవేళ ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(location, channel->align) &&
	     IS_ALIGNED(కొలత, channel->align))) {
		ఒకవేళ (!(flags & RAW_WRITE_NOLOCK))
			mutex_తాళం(దత్తాంశం, BOUNCE_MTX);
		ఒకవేళ (ext2fs_llseek(దత్తాంశం->dev, location, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			వెళ్ళు error_unlock;
		}
		actual = లిఖ(దత్తాంశం->dev, buf, కొలత);
		ఒకవేళ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(దత్తాంశం, BOUNCE_MTX);
		ఒకవేళ (actual < 0) {
			retval = errno;
			వెళ్ళు error_బహిర్గతం;
		}
		ఒకవేళ (actual != కొలత) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			వెళ్ళు error_బహిర్గతం;
		}
		తిరిగి 0;
	}

#నిర్వచితమైతే ALIGN_DEBUG
	రాయి("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (గుర్తులేని దీర్ఘ) కొలత);
#అయితే_ముగింపు
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ఒకవేళ (channel->align == 0)
		channel->align = 1;
	ఒకవేళ ((channel->ఖండం_కొలత > channel->align) &&
	    (channel->ఖండం_కొలత % channel->align) == 0)
		align_కొలత = channel->ఖండం_కొలత;
	లేకపోతే
		align_కొలత = channel->align;
	aligned_blk = location / align_కొలత;
	స్థానభ్రంశం = location % align_కొలత;

	అయితే (కొలత > 0) {
		పూర్ణాంకం actual_w;

		ఒకవేళ (!(flags & RAW_WRITE_NOLOCK))
			mutex_తాళం(దత్తాంశం, BOUNCE_MTX);
		ఒకవేళ (కొలత < align_కొలత || స్థానభ్రంశం) {
			ఒకవేళ (ext2fs_llseek(దత్తాంశం->dev, aligned_blk * align_కొలత,
					  SEEK_SET) < 0) {
				retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
				వెళ్ళు error_unlock;
			}
			actual = పఠ(దత్తాంశం->dev, దత్తాంశం->bounce,
				      align_కొలత);
			ఒకవేళ (actual != align_కొలత) {
				ఒకవేళ (actual < 0) {
					retval = errno;
					వెళ్ళు error_unlock;
				}
				memset((అక్షరం *) దత్తాంశం->bounce + actual, 0,
				       align_కొలత - actual);
			}
		}
		actual = కొలత;
		ఒకవేళ ((actual + స్థానభ్రంశం) > align_కొలత)
			actual = align_కొలత - స్థానభ్రంశం;
		ఒకవేళ (actual > కొలత)
			actual = కొలత;
		జ్ఞాపకం_నకలు(((అక్షరం *)దత్తాంశం->bounce) + స్థానభ్రంశం, buf, actual);
		ఒకవేళ (ext2fs_llseek(దత్తాంశం->dev, aligned_blk * align_కొలత, SEEK_SET) < 0) {
			retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
			వెళ్ళు error_unlock;
		}
		actual_w = లిఖ(దత్తాంశం->dev, దత్తాంశం->bounce, align_కొలత);
		ఒకవేళ (!(flags & RAW_WRITE_NOLOCK))
			mutex_unlock(దత్తాంశం, BOUNCE_MTX);
		ఒకవేళ (actual_w < 0) {
			retval = errno;
			వెళ్ళు error_బహిర్గతం;
		}
		ఒకవేళ (actual_w != align_కొలత)
			వెళ్ళు short_write;
		కొలత -= actual;
		buf += actual;
		location += actual;
		aligned_blk++;
		స్థానభ్రంశం = 0;
	}
	తిరిగి 0;

error_unlock:
	ఒకవేళ (!(flags & RAW_WRITE_NOLOCK))
		mutex_unlock(దత్తాంశం, BOUNCE_MTX);
error_బహిర్గతం:
	ఒకవేళ (((flags & RAW_WRITE_NO_HANDLER) == 0) && channel->write_error)
		retval = (channel->write_error)(channel, ఖండం, లెక్క, buf,
						కొలత, actual, retval);
	తిరిగి retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
స్థిర errcode_t alloc_cache(io_channel channel,
			     నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	errcode_t		retval;
	నిర్మాణం unix_cache	*cache;
	గుర్తులేని పూర్ణాంకం		i;

	దత్తాంశం->access_సమయం = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < దత్తాంశం->cache_కొలత; i++, cache++) {
		cache->ఖండం = 0;
		cache->access_సమయం = 0;
		cache->dirty = 0;
		cache->లోపల_use = 0;
		ఒకవేళ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ఒకవేళ (channel->align || దత్తాంశం->flags & IO_FLAG_FORCE_BOUNCE) {
		ఒకవేళ (దత్తాంశం->bounce)
			ext2fs_free_mem(&దత్తాంశం->bounce);
		retval = io_channel_alloc_buf(channel, 0, &దత్తాంశం->bounce);
	}
	తిరిగి retval;
}

/* Free the cache buffers */
స్థిర శూన్య free_cache(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం unix_cache	*cache;
	గుర్తులేని పూర్ణాంకం		i;

	దత్తాంశం->access_సమయం = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < దత్తాంశం->cache_కొలత; i++, cache++) {
		cache->ఖండం = 0;
		cache->access_సమయం = 0;
		cache->dirty = 0;
		cache->లోపల_use = 0;
		ఒకవేళ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	ఒకవేళ (దత్తాంశం->bounce)
		ext2fs_free_mem(&దత్తాంశం->bounce);
}

#నిర్వచితం_కాకపోతే NO_IO_CACHE

/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#నిర్వచించు GOLDEN_నిష్పత్తి_ప్రధానసంఖ్య	0x9e37fffffffc0001UL
#నిర్వచితం_కాకపోతే CACHE_పంక్తి_కొలత
/* if the system didn't tell us, guess something reasonable */
#నిర్వచించు CACHE_పంక్తి_కొలత		64
#అయితే_ముగింపు

/* buffer cache hashing function, crudely stolen from xfsprogs */
స్థిర గుర్తులేని పూర్ణాంకం
cache_hash(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, blk64_t blkno)
{
	uint64_t	hashval = blkno;
	uint64_t	tmp;

	/* the default cache size is small, just do a linear scan */
	ఒకవేళ (దత్తాంశం->cache_కొలత <= DEFAULT_CACHE_కొలత)
		తిరిగి 0;

	tmp = hashval ^ (GOLDEN_నిష్పత్తి_ప్రధానసంఖ్య + hashval) / CACHE_పంక్తి_కొలత;
	tmp = tmp ^ ((tmp ^ GOLDEN_నిష్పత్తి_ప్రధానసంఖ్య) >> దత్తాంశం->cache_hash_జరుపు);
	తిరిగి tmp % దత్తాంశం->cache_కొలత;
}

/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
స్థిర నిర్మాణం unix_cache *కనుగొను_cached_ఖండం(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
					    గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					    నిర్మాణం unix_cache **eldest)
{
	నిర్మాణం unix_cache	*cache, *వాడని_cache, *oldest_cache;
	గుర్తులేని పూర్ణాంకం		hash = cache_hash(దత్తాంశం, ఖండం);
	గుర్తులేని పూర్ణాంకం		i;

	వాడని_cache = oldest_cache = 0;
	/* walk [hash..] cache elements */
	కొరకు (i = hash, cache = దత్తాంశం->cache + hash;
	     i < దత్తాంశం->cache_కొలత;
	     i++, cache++) {
		ఒకవేళ (!cache->లోపల_use) {
			ఒకవేళ (!వాడని_cache)
				వాడని_cache = cache;
			కొనసాగు;
		}
		ఒకవేళ (cache->ఖండం == ఖండం) {
			cache->access_సమయం = ++దత్తాంశం->access_సమయం;
			దత్తాంశం->io_stats.cache_తగిలినవి++;
			తిరిగి cache;
		}
		ఒకవేళ (!oldest_cache ||
		    (cache->access_సమయం < oldest_cache->access_సమయం))
			oldest_cache = cache;
	}
	/* walk [..hash] since we didnt find a good slot yet */
	కొరకు (i = 0, cache = దత్తాంశం->cache; i < hash; i++, cache++) {
		ఒకవేళ (!cache->లోపల_use) {
			ఒకవేళ (!వాడని_cache)
				వాడని_cache = cache;
			కొనసాగు;
		}
		ఒకవేళ (cache->ఖండం == ఖండం) {
			cache->access_సమయం = ++దత్తాంశం->access_సమయం;
			దత్తాంశం->io_stats.cache_తగిలినవి++;
			తిరిగి cache;
		}
		ఒకవేళ (!oldest_cache ||
		    (cache->access_సమయం < oldest_cache->access_సమయం))
			oldest_cache = cache;
	}
	ఒకవేళ (eldest)
		*eldest = (వాడని_cache) ? వాడని_cache : oldest_cache;
	దత్తాంశం->io_stats.cache_తప్పినవి++;
	తిరిగి 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
స్థిర errcode_t reuse_cache(io_channel channel,
		నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, నిర్మాణం unix_cache *cache,
		గుర్తులేని దీర్ఘ దీర్ఘ ఖండం)
{
	ఒకవేళ (cache->dirty && cache->లోపల_use) {
		errcode_t retval;

		retval = raw_write_blk(channel, దత్తాంశం, cache->ఖండం, 1,
				       cache->buf, RAW_WRITE_NO_HANDLER);
		ఒకవేళ (retval) {
			cache->write_err = 1;
			తిరిగి retval;
		}
	}

	cache->లోపల_use = 1;
	cache->dirty = 0;
	cache->write_err = 0;
	cache->ఖండం = ఖండం;
	cache->access_సమయం = ++దత్తాంశం->access_సమయం;
	తిరిగి 0;
}

#నిర్వచించు FLUSH_INVALIDATE	0x01
#నిర్వచించు FLUSH_NOLOCK		0x02

/*
 * Flush all of the blocks in the cache
 */
స్థిర errcode_t flush_cached_ఖండాలు(io_channel channel,
				     నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
				     పూర్ణాంకం flags)
{
	నిర్మాణం unix_cache	*cache;
	errcode_t		retval, retval2 = 0;
	గుర్తులేని పూర్ణాంకం		i;
	పూర్ణాంకం			errors_దొరికింది = 0;

	ఒకవేళ ((flags & FLUSH_NOLOCK) == 0)
		mutex_తాళం(దత్తాంశం, CACHE_MTX);
	కొరకు (i=0, cache = దత్తాంశం->cache; i < దత్తాంశం->cache_కొలత; i++, cache++) {
		ఒకవేళ (!cache->లోపల_use)
			కొనసాగు;
		ఒకవేళ (cache->dirty) {
			పూర్ణాంకం raw_flags = RAW_WRITE_NO_HANDLER;

			ఒకవేళ (flags & FLUSH_NOLOCK)
				raw_flags |= RAW_WRITE_NOLOCK;

			retval = raw_write_blk(channel, దత్తాంశం,
					       cache->ఖండం, 1, cache->buf,
					       raw_flags);
			ఒకవేళ (retval) {
				cache->write_err = 1;
				errors_దొరికింది = 1;
				retval2 = retval;
			} లేకపోతే {
				cache->dirty = 0;
				cache->write_err = 0;
				ఒకవేళ (flags & FLUSH_INVALIDATE)
					cache->లోపల_use = 0;
			}
		} లేకపోతే ఒకవేళ (flags & FLUSH_INVALIDATE) {
			cache->లోపల_use = 0;
		}
	}
	ఒకవేళ ((flags & FLUSH_NOLOCK) == 0)
		mutex_unlock(దత్తాంశం, CACHE_MTX);
retry:
	అయితే (errors_దొరికింది) {
		ఒకవేళ ((flags & FLUSH_NOLOCK) == 0)
			mutex_తాళం(దత్తాంశం, CACHE_MTX);
		errors_దొరికింది = 0;
		కొరకు (i=0, cache = దత్తాంశం->cache; i < దత్తాంశం->cache_కొలత; i++, cache++) {
			ఒకవేళ (!cache->లోపల_use || !cache->write_err)
				కొనసాగు;
			errors_దొరికింది = 1;
			ఒకవేళ (cache->write_err && channel->write_error) {
				అక్షరం *err_buf = ఖాళీ;
				గుర్తులేని దీర్ఘ దీర్ఘ err_ఖండం = cache->ఖండం;

				cache->dirty = 0;
				cache->లోపల_use = 0;
				cache->write_err = 0;
				ఒకవేళ (io_channel_alloc_buf(channel, 0,
							 &err_buf))
					err_buf = ఖాళీ;
				లేకపోతే
					జ్ఞాపకం_నకలు(err_buf, cache->buf,
					       channel->ఖండం_కొలత);
				mutex_unlock(దత్తాంశం, CACHE_MTX);
				(channel->write_error)(channel, err_ఖండం,
					1, err_buf, channel->ఖండం_కొలత, -1,
					retval2);
				ఒకవేళ (err_buf)
					ext2fs_free_mem(&err_buf);
				mutex_తాళం(దత్తాంశం, CACHE_MTX);
				వెళ్ళు retry;
			} లేకపోతే
				cache->write_err = 0;
		}
		ఒకవేళ ((flags & FLUSH_NOLOCK) == 0)
			mutex_unlock(దత్తాంశం, CACHE_MTX);
	}
	తిరిగి retval2;
}

/* Shrink the cache buffers */
స్థిర errcode_t shrink_cache(io_channel channel,
			      నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			      గుర్తులేని పూర్ణాంకం new_కొలత)
{
	నిర్మాణం unix_cache	*cache, *new_cache;
	గుర్తులేని పూర్ణాంకం		i;
	errcode_t		retval;

	mutex_తాళం(దత్తాంశం, CACHE_MTX);

	retval = flush_cached_ఖండాలు(channel, దత్తాంశం,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ఒకవేళ (retval)
		వెళ్ళు unlock;

	కొరకు (i = new_కొలత, cache = దత్తాంశం->cache + new_కొలత;
	     i < దత్తాంశం->cache_కొలత;
	     i++, cache++) {
		cache->ఖండం = 0;
		cache->access_సమయం = 0;
		cache->dirty = 0;
		cache->లోపల_use = 0;
		ఒకవేళ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}

	new_cache = realloc(దత్తాంశం->cache, new_కొలత * పరిమాణం(నిర్మాణం unix_cache));
	ఒకవేళ (!new_cache) {
		retval = EXT2_ET_NO_జ్ఞాపకం;
		వెళ్ళు unlock;
	}

	దత్తాంశం->cache = new_cache;
	దత్తాంశం->cache_కొలత = new_కొలత;
	దత్తాంశం->cache_hash_జరుపు = highbit32(దత్తాంశం->cache_కొలత);

unlock:
	mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి retval;
}

/* Grow the cache buffers */
స్థిర errcode_t grow_cache(io_channel channel,
			    నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			    గుర్తులేని పూర్ణాంకం new_కొలత)
{
	నిర్మాణం unix_cache	*cache, *new_cache;
	గుర్తులేని పూర్ణాంకం		i;
	errcode_t		retval;

	mutex_తాళం(దత్తాంశం, CACHE_MTX);

	retval = flush_cached_ఖండాలు(channel, దత్తాంశం,
			FLUSH_INVALIDATE | FLUSH_NOLOCK);
	ఒకవేళ (retval)
		వెళ్ళు unlock;

	new_cache = realloc(దత్తాంశం->cache, new_కొలత * పరిమాణం(నిర్మాణం unix_cache));
	ఒకవేళ (!new_cache) {
		retval = EXT2_ET_NO_జ్ఞాపకం;
		వెళ్ళు unlock;
	}
	దత్తాంశం->cache = new_cache;

	కొరకు (i = దత్తాంశం->cache_కొలత, cache = new_cache + దత్తాంశం->cache_కొలత;
	     i < new_కొలత;
	     i++, cache++) {
		cache->ఖండం = 0;
		cache->access_సమయం = 0;
		cache->dirty = 0;
		cache->లోపల_use = 0;
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		ఒకవేళ (retval) {
			అయితే (i > దత్తాంశం->cache_కొలత) {
				cache--; i--;
				ఒకవేళ (cache->buf)
					ext2fs_free_mem(&cache->buf);
			}
			వెళ్ళు unlock;
		}
	}

	దత్తాంశం->cache_కొలత = new_కొలత;
	దత్తాంశం->cache_hash_జరుపు = highbit32(దత్తాంశం->cache_కొలత);

unlock:
	mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి retval;
}
#అయితే_ముగింపు /* NO_IO_CACHE */

#నిర్వచితమైతే __linux__
#నిర్వచితం_కాకపోతే BLKDISCARDZEROES
#నిర్వచించు BLKDISCARDZEROES _IO(0x12,124)
#అయితే_ముగింపు
#అయితే_ముగింపు

పూర్ణాంకం ext2fs_open_file(స్థిరరాశి అక్షరం *pathname, పూర్ణాంకం flags, mode_t బహుళకం)
{
	ఒకవేళ (బహుళకం)
#ఒకవేళ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		తిరిగి open64(pathname, flags, బహుళకం);
	లేకపోతే
		తిరిగి open64(pathname, flags);
#లేకపోతే
		తిరిగి open(pathname, flags, బహుళకం);
	లేకపోతే
		తిరిగి open(pathname, flags);
#అయితే_ముగింపు
}

పూర్ణాంకం ext2fs_stat(స్థిరరాశి అక్షరం *మార్గం, ext2fs_struct_stat *buf)
{
#ఒకవేళ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	తిరిగి stat64(మార్గం, buf);
#లేకపోతే
	తిరిగి stat(మార్గం, buf);
#అయితే_ముగింపు
}

పూర్ణాంకం ext2fs_fstat(పూర్ణాంకం fd, ext2fs_struct_stat *buf)
{
#ఒకవేళ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	తిరిగి fstat64(fd, buf);
#లేకపోతే
	తిరిగి fstat(fd, buf);
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_SYS_FILE_H
స్థిర errcode_t unix_funlock(io_channel channel)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	పూర్ణాంకం ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->unix_flock_flags) {
		ret = flock(దత్తాంశం->dev, LOCK_UN);
		ఒకవేళ (ret)
			తిరిగి errno;

		దత్తాంశం->unix_flock_flags = 0;
	}

	తిరిగి 0;
}

స్థిర errcode_t unix_flock(io_channel channel, గుర్తులేని పూర్ణాంకం flock_flags)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	పూర్ణాంకం unix_flock_flags = 0;
	errcode_t ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ret = unix_funlock(channel);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (flock_flags & IO_CHANNEL_FLOCK_EXCLUSIVE)
		unix_flock_flags |= LOCK_EX;

	ఒకవేళ (flock_flags & IO_CHANNEL_FLOCK_SHARED)
		unix_flock_flags |= LOCK_SH;

	ఒకవేళ (flock_flags & IO_CHANNEL_FLOCK_TRYLOCK)
		unix_flock_flags |= LOCK_NB;

	ఒకవేళ (!unix_flock_flags)
		తిరిగి 0;

	ret = flock(దత్తాంశం->dev, unix_flock_flags);
	ఒకవేళ (ret < 0)
		తిరిగి errno;

	దత్తాంశం->unix_flock_flags = unix_flock_flags & ~LOCK_NB;
	తిరిగి 0;
}
#లేకపోతే
#నిర్వచించు unix_flock		ఖాళీ

స్థిర errcode_t unix_funlock(io_channel channel)
{
	తిరిగి 0;
}
#అయితే_ముగింపు /* HAVE_SYS_FILE_H */

స్థిర errcode_t unix_open_channel(స్థిరరాశి అక్షరం *name, పూర్ణాంకం fd,
				   పూర్ణాంకం flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ఖాళీ;
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం = ఖాళీ;
	errcode_t	retval;
	ext2fs_struct_stat st;
#నిర్వచితమైతే __linux__
	నిర్మాణం		utsname ut;
#అయితే_ముగింపు

	ఒకవేళ (ext2fs_సురక్షిత_getenv("UNIX_IO_FORCE_BOUNCE"))
		flags |= IO_FLAG_FORCE_BOUNCE;

#నిర్వచితమైతే __linux__
	/*
	 * We need to make sure any previous errors in the block
	 * device are thrown away, sigh.
	 */
	(శూన్య) fsync(fd);
#అయితే_ముగింపు

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_io_channel), &io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(io, 0, పరిమాణం(నిర్మాణం struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం), &దత్తాంశం);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &io->name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(io->name, name);
	io->వ్యక్తిగత_దత్తాంశం = దత్తాంశం;
	io->ఖండం_కొలత = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	ఒకవేళ (ext2fs_సురక్షిత_getenv("UNIX_IO_NOZEROOUT"))
		io->flags |= CHANNEL_FLAGS_NOZEROOUT;

	memset(దత్తాంశం, 0, పరిమాణం(నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం));
	దత్తాంశం->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	దత్తాంశం->io_stats.అంకె_fields = 4;
	దత్తాంశం->flags = flags;
	దత్తాంశం->dev = fd;

	దత్తాంశం->cache_కొలత = DEFAULT_CACHE_కొలత;
	దత్తాంశం->cache_hash_జరుపు = highbit32(దత్తాంశం->cache_కొలత);
	దత్తాంశం->cache = సున్నా_జ్ఞాపకం_ఇవ్వు(DEFAULT_CACHE_కొలత, పరిమాణం(నిర్మాణం unix_cache));
	ఒకవేళ (!దత్తాంశం->cache) {
		retval = EXT2_ET_NO_జ్ఞాపకం;
		వెళ్ళు cleanup;
	}

#ఒకవేళ defined(O_ప్రత్యక్ష)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO)
		io->align = ext2fs_get_dio_alignment(దత్తాంశం->dev);
#లేదావేళ defined(F_NOCACHE)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO)
		io->align = 4096;
#అయితే_ముగింపు

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ఒకవేళ (ext2fs_fstat(దత్తాంశం->dev, &st) == 0) {
		ఒకవేళ (ext2fsP_is_disk_device(st.st_బహుళకం)) {
#నిర్వచితమైతే BLKDISCARDZEROES
			పూర్ణాంకం zeroes = 0;

			ఒకవేళ (ioctl(దత్తాంశం->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
			    zeroes)
				io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
#అయితే_ముగింపు
			io->flags |= CHANNEL_FLAGS_ఖండం_DEVICE;
		} లేకపోతే {
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
		}
	}

#ఒకవేళ defined(__CYGWIN__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	ఒకవేళ (!io->align)
		io->align = 512;
#అయితే_ముగింపు

#ఒకవేళ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ఒకవేళ (io->flags & CHANNEL_FLAGS_ఖండం_DEVICE) {
		పూర్ణాంకం dio_align = ext2fs_get_dio_alignment(fd);

		ఒకవేళ (io->align < dio_align)
			io->align = dio_align;
	}
#అయితే_ముగింపు

	ఒకవేళ ((retval = alloc_cache(io, దత్తాంశం)))
		వెళ్ళు cleanup;

#నిర్వచితమైతే BLKROGET
	ఒకవేళ (flags & IO_FLAG_RW) {
		పూర్ణాంకం error;
		పూర్ణాంకం readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(దత్తాంశం->dev, BLKROGET, &readonly);
		ఒకవేళ (!error && readonly) {
			retval = EPERM;
			వెళ్ళు cleanup;
		}
	}
#అయితే_ముగింపు

#నిర్వచితమైతే __linux__
#నిర్వచనం_రద్దు RLIM_INFINITY
#ఒకవేళ (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#నిర్వచించు RLIM_INFINITY	((గుర్తులేని దీర్ఘ)(~0UL>>1))
#లేకపోతే
#నిర్వచించు RLIM_INFINITY  (~0UL)
#అయితే_ముగింపు
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	ఒకవేళ ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_fstat(దత్తాంశం->dev, &st) == 0) &&
	    (ext2fsP_is_disk_device(st.st_బహుళకం))) {
		నిర్మాణం rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (గుర్తులేని దీర్ఘ) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		ఒకవేళ (((గుర్తులేని దీర్ఘ) rlim.rlim_cur) <
		    ((గుర్తులేని దీర్ఘ) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_PTHREAD
	ఒకవేళ (flags & IO_FLAG_దారాలు) {
		io->flags |= CHANNEL_FLAGS_దారాలు;
		retval = pthread_mutex_init(&దత్తాంశం->cache_mutex, ఖాళీ);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		retval = pthread_mutex_init(&దత్తాంశం->bounce_mutex, ఖాళీ);
		ఒకవేళ (retval) {
			pthread_mutex_destroy(&దత్తాంశం->cache_mutex);
			వెళ్ళు cleanup;
		}
		retval = pthread_mutex_init(&దత్తాంశం->stats_mutex, ఖాళీ);
		ఒకవేళ (retval) {
			pthread_mutex_destroy(&దత్తాంశం->cache_mutex);
			pthread_mutex_destroy(&దత్తాంశం->bounce_mutex);
			వెళ్ళు cleanup;
		}
	}
#అయితే_ముగింపు
	*channel = io;
	తిరిగి 0;

cleanup:
	ఒకవేళ (దత్తాంశం) {
		unix_funlock(io);
		ఒకవేళ (io->manager != unixfd_io_manager && దత్తాంశం->dev >= 0)
			close(దత్తాంశం->dev);
		ఒకవేళ (దత్తాంశం->cache) {
			free_cache(దత్తాంశం);
			విడుదల(దత్తాంశం->cache);
		}
		ext2fs_free_mem(&దత్తాంశం);
	}
	ఒకవేళ (io) {
		ఒకవేళ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	తిరిగి retval;
}

స్థిర errcode_t unixfd_open(స్థిరరాశి అక్షరం *str_fd, పూర్ణాంకం flags,
			     io_channel *channel)
{
	పూర్ణాంకం fd;
	పూర్ణాంకం fd_flags;

	fd = atoi(str_fd);
#ఒకవేళ defined(HAVE_FCNTL)
	fd_flags = fcntl(fd, F_GETFL);
	ఒకవేళ (fd_flags == -1)
		తిరిగి EBADF;

	/* O_EXCL is cleared by Linux at open and not returned by F_GETFL */
	flags &= IO_FLAG_EXCLUSIVE;
	ఒకవేళ (fd_flags & O_RDWR)
		flags |= IO_FLAG_RW;
#ఒకవేళ defined(O_ప్రత్యక్ష)
	ఒకవేళ (fd_flags & O_ప్రత్యక్ష)
		flags |= IO_FLAG_ప్రత్యక్ష_IO;
#అయితే_ముగింపు
#అయితే_ముగింపు  /* HAVE_FCNTL */

	తిరిగి unix_open_channel(str_fd, fd, flags, channel, unixfd_io_manager);
}

స్థిర errcode_t unix_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags,
			   io_channel *channel)
{
	పూర్ణాంకం fd = -1;
	పూర్ణాంకం open_flags;

	ఒకవేళ (name == 0)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ఒకవేళ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ఒకవేళ defined(O_ప్రత్యక్ష)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO)
		open_flags |= O_ప్రత్యక్ష;
#అయితే_ముగింపు
	fd = ext2fs_open_file(name, open_flags, 0);
	ఒకవేళ (fd < 0)
		తిరిగి errno;
#ఒకవేళ defined(F_NOCACHE) && !defined(IO_ప్రత్యక్ష)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO) {
		ఒకవేళ (fcntl(fd, F_NOCACHE, 1) < 0)
			తిరిగి errno;
	}
#అయితే_ముగింపు
	తిరిగి unix_open_channel(name, fd, flags, channel, unix_io_manager);
}

స్థిర errcode_t unix_close(io_channel channel)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
		తిరిగి 0;

#నిర్వచితం_కాకపోతే NO_IO_CACHE
	retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0);
#అయితే_ముగింపు

	unix_funlock(channel);

	ఒకవేళ (channel->manager != unixfd_io_manager && close(దత్తాంశం->dev) < 0)
		retval = errno;
	free_cache(దత్తాంశం);
	విడుదల(దత్తాంశం->cache);
#నిర్వచితమైతే HAVE_PTHREAD
	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_దారాలు) {
		pthread_mutex_destroy(&దత్తాంశం->cache_mutex);
		pthread_mutex_destroy(&దత్తాంశం->bounce_mutex);
		pthread_mutex_destroy(&దత్తాంశం->stats_mutex);
	}
#అయితే_ముగింపు

	ext2fs_free_mem(&channel->వ్యక్తిగత_దత్తాంశం);
	ఒకవేళ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	తిరిగి retval;
}

స్థిర errcode_t unix_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (channel->ఖండం_కొలత != blksize) {
		mutex_తాళం(దత్తాంశం, CACHE_MTX);
		mutex_తాళం(దత్తాంశం, BOUNCE_MTX);
#నిర్వచితం_కాకపోతే NO_IO_CACHE
		ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం, FLUSH_NOLOCK))){
			mutex_unlock(దత్తాంశం, BOUNCE_MTX);
			mutex_unlock(దత్తాంశం, CACHE_MTX);
			తిరిగి retval;
		}
#అయితే_ముగింపు

		channel->ఖండం_కొలత = blksize;
		free_cache(దత్తాంశం);
		retval = alloc_cache(channel, దత్తాంశం);
		mutex_unlock(దత్తాంశం, BOUNCE_MTX);
		mutex_unlock(దత్తాంశం, CACHE_MTX);
	}
	తిరిగి retval;
}

స్థిర errcode_t unix_read_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	నిర్మాణం unix_cache *cache;
	errcode_t	retval;
	అక్షరం		*cp;
	పూర్ణాంకం		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#నిర్వచితమైతే NO_IO_CACHE
	తిరిగి raw_read_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
#లేకపోతే
	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_NOCACHE)
		తిరిగి raw_read_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	ఒకవేళ (లెక్క < 0 || లెక్క > WRITE_ప్రత్యక్ష_కొలత) {
		ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0)))
			తిరిగి retval;
		తిరిగి raw_read_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
	}

	cp = buf;
	mutex_తాళం(దత్తాంశం, CACHE_MTX);
	అయితే (లెక్క > 0) {
		/* If it's in the cache, use it! */
		ఒకవేళ ((cache = కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం, ఖాళీ))) {
#నిర్వచితమైతే DEBUG
			రాయి("Using cached block %lu\n", ఖండం);
#అయితే_ముగింపు
			జ్ఞాపకం_నకలు(cp, cache->buf, channel->ఖండం_కొలత);
			లెక్క--;
			ఖండం++;
			cp += channel->ఖండం_కొలత;
			కొనసాగు;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		కొరకు (i=1; i < లెక్క; i++)
			ఒకవేళ (కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం+i, ఖాళీ))
				ఆపు;
#నిర్వచితమైతే DEBUG
		రాయి("Reading %d blocks starting at %lu\n", i, ఖండం);
#అయితే_ముగింపు
		mutex_unlock(దత్తాంశం, CACHE_MTX);
		ఒకవేళ ((retval = raw_read_blk(channel, దత్తాంశం, ఖండం, i, cp)))
			తిరిగి retval;
		mutex_తాళం(దత్తాంశం, CACHE_MTX);

		/* Save the results in the cache */
		కొరకు (j=0; j < i; j++) {
			ఒకవేళ (!కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం, &cache)) {
				retval = reuse_cache(channel, దత్తాంశం,
						     cache, ఖండం);
				ఒకవేళ (retval)
					వెళ్ళు call_write_handler;
				జ్ఞాపకం_నకలు(cache->buf, cp, channel->ఖండం_కొలత);
			}
			లెక్క--;
			ఖండం++;
			cp += channel->ఖండం_కొలత;
		}
	}
	mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి 0;

call_write_handler:
	ఒకవేళ (cache->write_err && channel->write_error) {
		అక్షరం *err_buf = ఖాళీ;
		గుర్తులేని దీర్ఘ దీర్ఘ err_ఖండం = cache->ఖండం;

		cache->dirty = 0;
		cache->లోపల_use = 0;
		cache->write_err = 0;
		ఒకవేళ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ఖాళీ;
		లేకపోతే
			జ్ఞాపకం_నకలు(err_buf, cache->buf, channel->ఖండం_కొలత);
		mutex_unlock(దత్తాంశం, CACHE_MTX);
		(channel->write_error)(channel, err_ఖండం, 1, err_buf,
				       channel->ఖండం_కొలత, -1,
				       retval);
		ఒకవేళ (err_buf)
			ext2fs_free_mem(&err_buf);
	} లేకపోతే
		mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి retval;
#అయితే_ముగింపు /* NO_IO_CACHE */
}

స్థిర errcode_t unix_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	తిరిగి unix_read_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t unix_write_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	నిర్మాణం unix_cache *cache, *reuse;
	errcode_t	retval = 0;
	స్థిరరాశి అక్షరం	*cp;
	పూర్ణాంకం		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#నిర్వచితమైతే NO_IO_CACHE
	తిరిగి raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf, 0);
#లేకపోతే
	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_NOCACHE)
		తిరిగి raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf, 0);
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ఒకవేళ (లెక్క < 0 || లెక్క > WRITE_ప్రత్యక్ష_కొలత) {
		ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం,
						  FLUSH_INVALIDATE)))
			తిరిగి retval;
		తిరిగి raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf, 0);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ఒకవేళ (writethrough)
		retval = raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf, 0);

	cp = buf;
	mutex_తాళం(దత్తాంశం, CACHE_MTX);
	అయితే (లెక్క > 0) {
		cache = కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం, &reuse);
		ఒకవేళ (!cache) {
			errcode_t err;

			cache = reuse;
			err = reuse_cache(channel, దత్తాంశం, cache, ఖండం);
			ఒకవేళ (err)
				వెళ్ళు call_write_handler;
		}
		ఒకవేళ (cache->buf != cp)
			జ్ఞాపకం_నకలు(cache->buf, cp, channel->ఖండం_కొలత);
		cache->dirty = !writethrough;
		లెక్క--;
		ఖండం++;
		cp += channel->ఖండం_కొలత;
	}
	mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి retval;

call_write_handler:
	ఒకవేళ (cache->write_err && channel->write_error) {
		అక్షరం *err_buf = ఖాళీ;
		గుర్తులేని దీర్ఘ దీర్ఘ err_ఖండం = cache->ఖండం;

		cache->dirty = 0;
		cache->లోపల_use = 0;
		cache->write_err = 0;
		ఒకవేళ (io_channel_alloc_buf(channel, 0, &err_buf))
			err_buf = ఖాళీ;
		లేకపోతే
			జ్ఞాపకం_నకలు(err_buf, cache->buf, channel->ఖండం_కొలత);
		mutex_unlock(దత్తాంశం, CACHE_MTX);
		(channel->write_error)(channel, err_ఖండం, 1, err_buf,
				       channel->ఖండం_కొలత, -1,
				       retval);
		ఒకవేళ (err_buf)
			ext2fs_free_mem(&err_buf);
	} లేకపోతే
		mutex_unlock(దత్తాంశం, CACHE_MTX);
	తిరిగి retval;
#అయితే_ముగింపు /* NO_IO_CACHE */
}

స్థిర errcode_t unix_cache_readahead(io_channel channel,
				      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
#నిర్వచితమైతే POSIX_FADV_WILLNEED
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *)channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	తిరిగి posix_fadvise(దత్తాంశం->dev,
			     (ext2_loff_t)ఖండం * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం,
			     (ext2_loff_t)లెక్క * channel->ఖండం_కొలత,
			     POSIX_FADV_WILLNEED);
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

స్థిర errcode_t unix_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	తిరిగి unix_write_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t unix_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				 పూర్ణాంకం కొలత, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;
	ssize_t		actual;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (channel->align != 0) {
#నిర్వచితమైతే ALIGN_DEBUG
		రాయి("unix_write_byte: O_DIRECT fallback\n");
#అయితే_ముగింపు
		తిరిగి EXT2_ET_UNIMPLEMENTED;
	}

#నిర్వచితం_కాకపోతే NO_IO_CACHE
	/*
	 * Flush out the cache completely
	 */
	ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం, FLUSH_INVALIDATE)))
		తిరిగి retval;
#అయితే_ముగింపు

	ఒకవేళ (lseek(దత్తాంశం->dev, స్థానభ్రంశం + దత్తాంశం->స్థానభ్రంశం, SEEK_SET) < 0)
		తిరిగి errno;

	actual = లిఖ(దత్తాంశం->dev, buf, కొలత);
	ఒకవేళ (actual < 0)
		తిరిగి errno;
	ఒకవేళ (actual != కొలత)
		తిరిగి EXT2_ET_SHORT_WRITE;

	తిరిగి 0;
}

/*
 * Flush data buffers to disk.
 */
స్థిర errcode_t unix_flush(io_channel channel)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#నిర్వచితం_కాకపోతే NO_IO_CACHE
	retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0);
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_FSYNC
	ఒకవేళ (!retval && fsync(దత్తాంశం->dev) != 0)
		తిరిగి errno;
#అయితే_ముగింపు
	తిరిగి retval;
}

స్థిర errcode_t unix_సమితి_option(io_channel channel, స్థిరరాశి అక్షరం *option,
				 స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	గుర్తులేని దీర్ఘ దీర్ఘ tmp;
	errcode_t retval;
	అక్షరం *చివర;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (!strcmp(option, "offset")) {
		ఒకవేళ (!arg)
			తిరిగి EXT2_ET_INVALID_వాదం;

		tmp = strtoull(arg, &చివర, 0);
		ఒకవేళ (*చివర)
			తిరిగి EXT2_ET_INVALID_వాదం;
		దత్తాంశం->స్థానభ్రంశం = tmp;
		ఒకవేళ (దత్తాంశం->స్థానభ్రంశం < 0)
			తిరిగి EXT2_ET_INVALID_వాదం;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(option, "cache")) {
		ఒకవేళ (!arg)
			తిరిగి EXT2_ET_INVALID_వాదం;
		ఒకవేళ (!strcmp(arg, "on")) {
			దత్తాంశం->flags &= ~IO_FLAG_NOCACHE;
			తిరిగి 0;
		}
		ఒకవేళ (!strcmp(arg, "off")) {
			retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0);
			దత్తాంశం->flags |= IO_FLAG_NOCACHE;
			తిరిగి retval;
		}
		తిరిగి EXT2_ET_INVALID_వాదం;
	}
#నిర్వచితం_కాకపోతే NO_IO_CACHE
	ఒకవేళ (!strcmp(option, "cache_blocks")) {
		గుర్తులేని దీర్ఘ దీర్ఘ	కొలత;

		ఒకవేళ (!arg)
			తిరిగి EXT2_ET_INVALID_వాదం;

		errno = 0;
		కొలత = strtoll(arg, ఖాళీ, 0);
		ఒకవేళ (errno || కొలత == 0 || కొలత > INT32_MAX)
			తిరిగి EXT2_ET_INVALID_వాదం;

		ఒకవేళ (దత్తాంశం->cache_కొలత == కొలత)
			తిరిగి 0;
		ఒకవేళ (దత్తాంశం->cache_కొలత > కొలత)
			తిరిగి shrink_cache(channel, దత్తాంశం, కొలత);
		తిరిగి grow_cache(channel, దత్తాంశం, కొలత);
	}
#అయితే_ముగింపు
	తిరిగి EXT2_ET_INVALID_వాదం;
}

#ఒకవేళ defined(__linux__) && !defined(BLKDISCARD)
#నిర్వచించు BLKDISCARD		_IO(0x12,119)
#అయితే_ముగింపు

స్థిర errcode_t unix_discard(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	పూర్ణాంకం		ret = EOPNOTSUPP;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (channel->flags & CHANNEL_FLAGS_NODISCARD)
		వెళ్ళు unimplemented;

	ఒకవేళ (channel->flags & CHANNEL_FLAGS_ఖండం_DEVICE) {
#నిర్వచితమైతే BLKDISCARD
		__u64 range[2];

		range[0] = (__u64)(ఖండం) * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం;
		range[1] = (__u64)(లెక్క) * channel->ఖండం_కొలత;

		ret = ioctl(దత్తాంశం->dev, BLKDISCARD, &range);
#లేకపోతే
		వెళ్ళు unimplemented;
#అయితే_ముగింపు
	} లేకపోతే {
#ఒకవేళ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE)
		/*
		 * If we are not on block device, try to use punch hole
		 * to reclaim free space.
		 */
		ret = fallocate(దత్తాంశం->dev,
				FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
				(off_t)(ఖండం) * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం,
				(off_t)(లెక్క) * channel->ఖండం_కొలత);
#లేకపోతే
		వెళ్ళు unimplemented;
#అయితే_ముగింపు
	}
	ఒకవేళ (ret < 0) {
		ఒకవేళ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NODISCARD;
			వెళ్ళు unimplemented;
		}
		తిరిగి errno;
	}
	తిరిగి 0;
unimplemented:
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

/*
 * If we know about ZERO_RANGE, try that before we try PUNCH_HOLE because
 * ZERO_RANGE doesn't unmap preallocated blocks.  We prefer fallocate because
 * it always invalidates page cache, and libext2fs requires that reads after
 * ZERO_RANGE return zeroes.
 */
స్థిర పూర్ణాంకం __unix_zeroout(పూర్ణాంకం fd, off_t స్థానభ్రంశం, off_t len)
{
	పూర్ణాంకం ret = -1;

#ఒకవేళ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_ZERO_RANGE)
	ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, స్థానభ్రంశం, len);
	ఒకవేళ (ret == 0)
		తిరిగి 0;
#అయితే_ముగింపు
#ఒకవేళ defined(HAVE_FALLOCATE) && defined(FALLOC_FL_PUNCH_HOLE) && defined(FALLOC_FL_KEEP_SIZE)
	ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
			స్థానభ్రంశం,  len);
	ఒకవేళ (ret == 0)
		తిరిగి 0;
#అయితే_ముగింపు
	errno = EOPNOTSUPP;
	తిరిగి ret;
}

/* parameters might not be used if OS doesn't support zeroout */
#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wunused-parameter"
#అయితే_ముగింపు
స్థిర errcode_t unix_zeroout(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	పూర్ణాంకం		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం unix_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (!(channel->flags & CHANNEL_FLAGS_ఖండం_DEVICE)) {
		/* Regular file, try to use truncate/punch/zero. */
		నిర్మాణం stat statbuf;

		ఒకవేళ (లెక్క == 0)
			తిరిగి 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(దత్తాంశం->dev, &statbuf);
		ఒకవేళ (ret)
			వెళ్ళు err;
		ఒకవేళ ((గుర్తులేని దీర్ఘ దీర్ఘ) statbuf.st_కొలత <
			(ఖండం + లెక్క) * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం) {
			ret = ftruncate(దత్తాంశం->dev,
					(ఖండం + లెక్క) * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం);
			ఒకవేళ (ret)
				వెళ్ళు err;
		}
	}

	ఒకవేళ (channel->flags & CHANNEL_FLAGS_NOZEROOUT)
		వెళ్ళు unimplemented;

	ret = __unix_zeroout(దత్తాంశం->dev,
			(off_t)(ఖండం) * channel->ఖండం_కొలత + దత్తాంశం->స్థానభ్రంశం,
			(off_t)(లెక్క) * channel->ఖండం_కొలత);
err:
	ఒకవేళ (ret < 0) {
		ఒకవేళ (errno == EOPNOTSUPP) {
			channel->flags |= CHANNEL_FLAGS_NOZEROOUT;
			వెళ్ళు unimplemented;
		}
		తిరిగి errno;
	}
	తిరిగి 0;
unimplemented:
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}
#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు

స్థిర నిర్మాణం struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.సమితి_blksize	= unix_సమితి_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.సమితి_option	= unix_సమితి_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unix_io_manager = &struct_unix_manager;

స్థిర నిర్మాణం struct_io_manager struct_unixfd_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix fd I/O Manager",
	.open		= unixfd_open,
	.close		= unix_close,
	.సమితి_blksize	= unix_సమితి_blksize,
	.read_blk	= unix_read_blk,
	.write_blk	= unix_write_blk,
	.flush		= unix_flush,
	.write_byte	= unix_write_byte,
	.సమితి_option	= unix_సమితి_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	.write_blk64	= unix_write_blk64,
	.discard	= unix_discard,
	.cache_readahead	= unix_cache_readahead,
	.zeroout	= unix_zeroout,
	.flock		= unix_flock,
};

io_manager unixfd_io_manager = &struct_unixfd_manager;
