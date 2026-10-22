/*
 * windows_io.c --- This is the Windows implementation of the I/O manager.
 *
 * Implements a one-block write-through cache.
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *	2002 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ <windows.h>
#అన్తర్భావ <winioctl.h>
#అన్తర్భావ <io.h>

#ఒకవేళ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#నిర్వచించు XOPEN_ఆధారం 600
#నిర్వచించు DARWIN_C_ఆధారం
#నిర్వచించు FILE_స్థానభ్రంశం_బిట్లు 64
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
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచించు PR_GET_DUMPABLE 3
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>

#నిర్వచనం_రద్దు ALIGN_DEBUG

//#define DEBUG
#నిర్వచితమైతే DEBUG
#నిర్వచించు TRACE(...) {\
		    అక్షరం __log[256];\
		    snprintf(__log, పరిమాణం(__log), __VA_ARGS__);\
		    __log[పరిమాణం(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#లేకపోతే
#నిర్వచించు TRACE(...) చేయి { } అయితే (0);
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)
#నిర్వచించు EXT2_CHECK_MAGIC_RETURN(నిర్మాణం, సంకేతం_2, ret) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (ret)

#నిర్వచించు EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

నిర్మాణం windows_cache {
	అక్షరం			*buf;
	గుర్తులేని దీర్ఘ దీర్ఘ	ఖండం;
	పూర్ణాంకం			access_సమయం;
	గుర్తులేని		dirty:1;
	గుర్తులేని		లోపల_use:1;
};

#నిర్వచించు CACHE_కొలత 8
#నిర్వచించు WRITE_ప్రత్యక్ష_కొలత 4	/* Must be smaller than CACHE_SIZE */
#నిర్వచించు READ_ప్రత్యక్ష_కొలత 4	/* Should be smaller than CACHE_SIZE */

నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం	magic;
	అక్షరం	name[MAX_మార్గం];
	HANDLE	handle;
	అక్షరం	dos_device[MAX_మార్గం];
	అక్షరం	cf_device[MAX_మార్గం];
	పూర్ణాంకం	dev;
	పూర్ణాంకం	flags;
	పూర్ణాంకం	align;
	పూర్ణాంకం	access_సమయం;
	ext2_loff_t స్థానభ్రంశం;
	నిర్మాణం windows_cache cache[CACHE_కొలత];
	శూన్య	*bounce;
	నిర్మాణం struct_io_stats io_stats;
};

#నిర్వచించు IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

స్థిర పూర్ణాంకం fake_dos_name_for_device(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	ఒకవేళ (అక్షరశ్రేణి_పోలిక(దత్తాంశం->name, "\\\\", 2) == 0) {
		దత్తాంశం->dos_device[0] = 0;
		strcpy(దత్తాంశం->cf_device, దత్తాంశం->name);
		తిరిగి 0;
	}

	_snprintf(దత్తాంశం->dos_device, MAX_మార్గం, "fakedevice%lu", GetCurrentProcessId());

	ఒకవేళ (!DefineDosDevice(DDD_RAW_లక్ష్యం_మార్గం, దత్తాంశం->dos_device, దత్తాంశం->name))
		తిరిగి 1;

	_snprintf(దత్తాంశం->cf_device, MAX_మార్గం, "\\\\.\\%s", దత్తాంశం->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", దత్తాంశం->dos_device);

	తిరిగి 0;
}

స్థిర శూన్య remove_fake_dos_name(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	ఒకవేళ (*దత్తాంశం->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", దత్తాంశం->dos_device);
		DefineDosDevice(DDD_RAW_లక్ష్యం_మార్గం | DDD_EXACT_సరిపోలిక_ON_REMOVE | DDD_REMOVE_DEFINITION, దత్తాంశం->dos_device, దత్తాంశం->name);
	}
}

స్థిర errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ఒకవేళ (stats)
		*stats = &దత్తాంశం->io_stats;

	తిరిగి retval;
}

స్థిర LARGE_INTEGER make_large_integer(LONGLONG విలువ)
{
	LARGE_INTEGER	li;

	li.QuadPart = విలువ;
	తిరిగి li;
}

/*
 * Here are the raw I/O functions
 */
స్థిర errcode_t raw_read_blk(io_channel channel,
			      నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      పూర్ణాంకం లెక్క, శూన్య *bufv)
{
	errcode_t	retval;
	ssize_t	కొలత;
	ext2_loff_t	location;
	DWORD		actual = 0;
	గుర్తులేని అక్షరం	*buf = bufv;
	ssize_t		really_read = 0;

	కొలత = (లెక్క < 0) ? -లెక్క : లెక్క * channel->ఖండం_కొలత;
	దత్తాంశం->io_stats.బైట్లు_read += కొలత;
	location = ((ext2_loff_t) ఖండం * channel->ఖండం_కొలత) + దత్తాంశం->స్థానభ్రంశం;

	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_FORCE_BOUNCE) {
		ఒకవేళ (!సమితి_File_సూచిక_Ex(దత్తాంశం->handle, make_large_integer(location), ఖాళీ, FILE_BEGIN)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}
		వెళ్ళు bounce_read;
	}

	ఒకవేళ (!సమితి_File_సూచిక_Ex(దత్తాంశం->handle, make_large_integer(location), ఖాళీ, FILE_BEGIN)) {
		retval = Get_చివరి_Error();
		వెళ్ళు error_బహిర్గతం;
	}
	ఒకవేళ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(కొలత, channel->align))) {
		ఒకవేళ (!ReadFile(దత్తాంశం->handle, buf, కొలత, &actual, ఖాళీ)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}
		ఒకవేళ (actual != కొలత) {
	short_read:
		ఒకవేళ (actual < 0) {
			retval = Get_చివరి_Error();
			actual = 0;
		} లేకపోతే
			retval = EXT2_ET_SHORT_READ;
		వెళ్ళు error_బహిర్గతం;
		}
	తిరిగి 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	అయితే (కొలత > 0) {
		ఒకవేళ (!ReadFile(దత్తాంశం->handle, దత్తాంశం->bounce, channel->ఖండం_కొలత, &actual, ఖాళీ)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}
		ఒకవేళ (actual != channel->ఖండం_కొలత) {
			actual = really_read;
			buf -= really_read;
			కొలత += really_read;
			వెళ్ళు short_read;
		}
		actual = కొలత;
		ఒకవేళ (కొలత > channel->ఖండం_కొలత)
			actual = channel->ఖండం_కొలత;
		జ్ఞాపకం_నకలు(buf, దత్తాంశం->bounce, actual);
		really_read += actual;
		కొలత -= actual;
		buf += actual;
	}
	తిరిగి 0;

error_బహిర్గతం:
	ఒకవేళ (actual >= 0 && actual < కొలత)
		memset((అక్షరం *) buf+actual, 0, కొలత-actual);
	ఒకవేళ (channel->read_error)
		retval = (channel->read_error)(channel, ఖండం, లెక్క, buf,
					       కొలత, actual, retval);
	తిరిగి retval;
}

స్థిర errcode_t raw_write_blk(io_channel channel,
			       నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			       గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *bufv)
{
	ssize_t		కొలత;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	స్థిరరాశి గుర్తులేని అక్షరం *buf = bufv;

	ఒకవేళ (లెక్క == 1)
		కొలత = channel->ఖండం_కొలత;
	లేకపోతే {
		ఒకవేళ (లెక్క < 0)
			కొలత = -లెక్క;
		లేకపోతే
			కొలత = లెక్క * channel->ఖండం_కొలత;
	}
	దత్తాంశం->io_stats.బైట్లు_written += కొలత;

	location = ((ext2_loff_t) ఖండం * channel->ఖండం_కొలత) + దత్తాంశం->స్థానభ్రంశం;

	ఒకవేళ (దత్తాంశం->flags & IO_FLAG_FORCE_BOUNCE) {
		ఒకవేళ (!సమితి_File_సూచిక_Ex(దత్తాంశం->handle, make_large_integer(location), ఖాళీ, FILE_BEGIN)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}
	వెళ్ళు bounce_write;
	}

		ఒకవేళ (!సమితి_File_సూచిక_Ex(దత్తాంశం->handle, make_large_integer(location), ఖాళీ, FILE_BEGIN)) {
			retval = Get_చివరి_Error();
		వెళ్ళు error_బహిర్గతం;
	}

	సమితి_చివరి_Error(0);

	ఒకవేళ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(కొలత, channel->align))) {
		ఒకవేళ (!WriteFile(దత్తాంశం->handle, buf, కొలత, &actual, ఖాళీ)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}

		ఒకవేళ (actual != కొలత) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			వెళ్ళు error_బహిర్గతం;
		}
		తిరిగి 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	అయితే (కొలత > 0) {
		ఒకవేళ (కొలత < channel->ఖండం_కొలత) {
			ఒకవేళ (!ReadFile(దత్తాంశం->handle, దత్తాంశం->bounce, channel->ఖండం_కొలత, &actual, ఖాళీ)) {
				retval = Get_చివరి_Error();
				వెళ్ళు error_బహిర్గతం;
			}
			ఒకవేళ (actual != channel->ఖండం_కొలత) {
				ఒకవేళ (actual < 0) {
					retval = Get_చివరి_Error();
					వెళ్ళు error_బహిర్గతం;
				}
				memset((అక్షరం *) దత్తాంశం->bounce + actual, 0,
				       channel->ఖండం_కొలత - actual);
			}
		}
		actual = కొలత;
		ఒకవేళ (కొలత > channel->ఖండం_కొలత)
			actual = channel->ఖండం_కొలత;
		జ్ఞాపకం_నకలు(దత్తాంశం->bounce, buf, actual);
		ఒకవేళ (!సమితి_File_సూచిక_Ex(దత్తాంశం->handle, make_large_integer(location), ఖాళీ, FILE_BEGIN)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}
		ఒకవేళ (!WriteFile(దత్తాంశం->handle, దత్తాంశం->bounce, channel->ఖండం_కొలత, &actual, ఖాళీ)) {
			retval = Get_చివరి_Error();
			వెళ్ళు error_బహిర్గతం;
		}

		ఒకవేళ (actual != channel->ఖండం_కొలత)
			వెళ్ళు short_write;
		కొలత -= actual;
		buf += actual;
		location += actual;
	}
	తిరిగి 0;

error_బహిర్గతం:
	ఒకవేళ (channel->write_error)
		retval = (channel->write_error)(channel, ఖండం, లెక్క, buf,
						కొలత, actual, retval);
	తిరిగి retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
స్థిర errcode_t alloc_cache(io_channel channel,
			     నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	errcode_t		retval;
	నిర్మాణం windows_cache	*cache;
	పూర్ణాంకం			i;

	దత్తాంశం->access_సమయం = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < CACHE_కొలత; i++, cache++) {
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
స్థిర శూన్య free_cache(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం windows_cache    *cache;
	పూర్ణాంకం			i;

	దత్తాంశం->access_సమయం = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < CACHE_కొలత; i++, cache++) {
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
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
స్థిర నిర్మాణం windows_cache *కనుగొను_cached_ఖండం(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
					    గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					    నిర్మాణం windows_cache **eldest)
{
	నిర్మాణం windows_cache	*cache, *వాడని_cache, *oldest_cache;
	పూర్ణాంకం			i;

	వాడని_cache = oldest_cache = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < CACHE_కొలత; i++, cache++) {
		ఒకవేళ (!cache->లోపల_use) {
			ఒకవేళ (!వాడని_cache)
				వాడని_cache = cache;
			కొనసాగు;
		}
		ఒకవేళ (cache->ఖండం == ఖండం) {
			cache->access_సమయం = ++దత్తాంశం->access_సమయం;
			తిరిగి cache;
		}
		ఒకవేళ (!oldest_cache ||
		   (cache->access_సమయం < oldest_cache->access_సమయం))
			oldest_cache = cache;
	}
	ఒకవేళ (eldest)
		*eldest = (వాడని_cache) ? వాడని_cache : oldest_cache;
	తిరిగి 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
స్థిర శూన్య reuse_cache(io_channel channel, నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
		 నిర్మాణం windows_cache *cache, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం)
{
	ఒకవేళ (cache->dirty && cache->లోపల_use)
		raw_write_blk(channel, దత్తాంశం, cache->ఖండం, 1, cache->buf);

	cache->లోపల_use = 1;
	cache->dirty = 0;
	cache->ఖండం = ఖండం;
	cache->access_సమయం = ++దత్తాంశం->access_సమయం;
}

/*
 * Flush all of the blocks in the cache
 */
స్థిర errcode_t flush_cached_ఖండాలు(io_channel channel,
				     నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
				     పూర్ణాంకం invalidate)
{
	నిర్మాణం windows_cache	*cache;
	errcode_t	retval, retval2;
	పూర్ణాంకం			i;

	retval2 = 0;
	కొరకు (i=0, cache = దత్తాంశం->cache; i < CACHE_కొలత; i++, cache++) {
		ఒకవేళ (!cache->లోపల_use)
			కొనసాగు;

		ఒకవేళ (invalidate)
			cache->లోపల_use = 0;

		ఒకవేళ (!cache->dirty)
			కొనసాగు;

		retval = raw_write_blk(channel, దత్తాంశం,
				       cache->ఖండం, 1, cache->buf);
		ఒకవేళ (retval)
			retval2 = retval;
		లేకపోతే
			cache->dirty = 0;
	}
	తిరిగి retval2;
}
#అయితే_ముగింపు /* NO_IO_CACHE */

స్థిర errcode_t windows_open_channel(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
				   పూర్ణాంకం flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ఖాళీ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_io_channel), &io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(io, 0, పరిమాణం(నిర్మాణం struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(దత్తాంశం->name)+1, &io->name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(io->name, దత్తాంశం->name);
	io->వ్యక్తిగత_దత్తాంశం = దత్తాంశం;
	io->ఖండం_కొలత = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#ఒకవేళ defined(O_ప్రత్యక్ష)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO)
		io->align = ext2fs_get_dio_alignment(దత్తాంశం->dev);
#అయితే_ముగింపు

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	ఒకవేళ (ext2fs_fstat(దత్తాంశం->dev, &st) == 0) {
		ఒకవేళ (ext2fsP_is_disk_device(st.st_బహుళకం))
			io->flags |= CHANNEL_FLAGS_ఖండం_DEVICE;
		లేకపోతే
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

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

	*channel = io;
	తిరిగి 0;

cleanup:
	ఒకవేళ (దత్తాంశం) {
		ఒకవేళ (దత్తాంశం->dev >= 0)
			close(దత్తాంశం->dev);
		free_cache(దత్తాంశం);
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

స్థిర DWORD windows_open_device(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, పూర్ణాంకం open_flags)
{
	DWORD ret = 0;

	ఒకవేళ (*దత్తాంశం->name != '\\')
		strcpy(దత్తాంశం->cf_device, దత్తాంశం->name);
	లేకపోతే ఒకవేళ (fake_dos_name_for_device(దత్తాంశం))
		తిరిగి -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_బహుళకం = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_లక్షణాలు =
#ఒకవేళ defined(O_ప్రత్యక్ష)
	(open_flags & O_ప్రత్యక్ష) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_సాధారణ;
#లేకపోతే
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#అయితే_ముగింపు

	దత్తాంశం->handle = CreateFile(దత్తాంశం->cf_device, desired_access, share_బహుళకం, ఖాళీ, OPEN_EXISTING,
				  flags_and_లక్షణాలు, ఖాళీ);

	ఒకవేళ (దత్తాంశం->handle == INVALID_HANDLE_విలువ) {
		ret = Get_చివరి_Error();
		వెళ్ళు invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", దత్తాంశం->cf_device, దత్తాంశం->handle);

	దత్తాంశం->dev = _open_osfhandle((intptr_t)దత్తాంశం->handle, 0);
	ఒకవేళ (దత్తాంశం->dev < 0) {
		ret = Get_చివరి_Error() ? Get_చివరి_Error() : 9999;
		వెళ్ళు osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", దత్తాంశం->handle, దత్తాంశం->dev);

	తిరిగి 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", దత్తాంశం->handle);
	CloseHandle(దత్తాంశం->handle);
invalid_handle:
	remove_fake_dos_name(దత్తాంశం);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	తిరిగి ret;
}

స్థిర నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *init_వ్యక్తిగత_దత్తాంశం(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం = ఖాళీ;

	ఒకవేళ (ext2fs_get_mem(పరిమాణం(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం), &దత్తాంశం))
		తిరిగి ఖాళీ;

	memset(దత్తాంశం, 0, పరిమాణం(నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం));
	strncpy(దత్తాంశం->name, name, పరిమాణం(దత్తాంశం->name) - 1);
	దత్తాంశం->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	దత్తాంశం->io_stats.అంకె_fields = 2;
	దత్తాంశం->flags = flags;
	దత్తాంశం->handle = INVALID_HANDLE_విలువ;

	తిరిగి దత్తాంశం;
}

స్థిర errcode_t windows_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	పూర్ణాంకం open_flags;
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	ఒకవేళ (name == 0)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;

	దత్తాంశం = init_వ్యక్తిగత_దత్తాంశం(name, flags);
	ఒకవేళ (!దత్తాంశం)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	ఒకవేళ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#ఒకవేళ defined(O_ప్రత్యక్ష)
	ఒకవేళ (flags & IO_FLAG_ప్రత్యక్ష_IO)
		open_flags |= O_ప్రత్యక్ష;
#అయితే_ముగింపు

	ఒకవేళ (windows_open_device(దత్తాంశం, open_flags)) {
		ext2fs_free_mem(&దత్తాంశం);
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;
	}

	తిరిగి windows_open_channel(దత్తాంశం, flags, channel, windows_io_manager);
}

స్థిర errcode_t windows_close(io_channel channel)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
		తిరిగి 0;

#నిర్వచితం_కాకపోతే NO_IO_CACHE
	retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0);
#అయితే_ముగింపు

	remove_fake_dos_name(దత్తాంశం);

	ఒకవేళ (_close(దత్తాంశం->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", దత్తాంశం->dev);

	free_cache(దత్తాంశం);

	ext2fs_free_mem(&channel->వ్యక్తిగత_దత్తాంశం);
	ఒకవేళ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	తిరిగి retval;
}

స్థిర errcode_t windows_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ఒకవేళ (channel->ఖండం_కొలత != blksize) {
#నిర్వచితం_కాకపోతే NO_IO_CACHE
		ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0)))
			తిరిగి retval;
#అయితే_ముగింపు

		channel->ఖండం_కొలత = blksize;
		free_cache(దత్తాంశం);
		ఒకవేళ ((retval = alloc_cache(channel, దత్తాంశం)))
			తిరిగి retval;
	}
	తిరిగి 0;
}

స్థిర errcode_t windows_read_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, శూన్య *buf)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	నిర్మాణం windows_cache *cache, *reuse[READ_ప్రత్యక్ష_కొలత];
	errcode_t	retval;
	అక్షరం		*cp;
	పూర్ణాంకం		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#నిర్వచితమైతే NO_IO_CACHE
	తిరిగి raw_read_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
#లేకపోతే
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
	అయితే (లెక్క > 0) {
		/* If it's in the cache, use it! */
		ఒకవేళ ((cache = కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం, &reuse[0]))) {
#నిర్వచితమైతే DEBUG
			రాయి("Using cached block %lu\n", ఖండం);
#అయితే_ముగింపు
			జ్ఞాపకం_నకలు(cp, cache->buf, channel->ఖండం_కొలత);
			లెక్క--;
			ఖండం++;
			cp += channel->ఖండం_కొలత;
			కొనసాగు;
		}
		ఒకవేళ (లెక్క == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, దత్తాంశం, cache, ఖండం);
			ఒకవేళ ((retval = raw_read_blk(channel, దత్తాంశం, ఖండం, 1,
						   cache->buf))) {
				cache->లోపల_use = 0;
				తిరిగి retval;
			}
			జ్ఞాపకం_నకలు(cp, cache->buf, channel->ఖండం_కొలత);
			తిరిగి 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		కొరకు (i=1; i < లెక్క; i++)
			ఒకవేళ (కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం+i, &reuse[i]))
				ఆపు;
#నిర్వచితమైతే DEBUG
		రాయి("Reading %d blocks starting at %lu\n", i, ఖండం);
#అయితే_ముగింపు
		ఒకవేళ ((retval = raw_read_blk(channel, దత్తాంశం, ఖండం, i, cp)))
			తిరిగి retval;

		/* Save the results in the cache */
		కొరకు (j=0; j < i; j++) {
			లెక్క--;
			cache = reuse[j];
			reuse_cache(channel, దత్తాంశం, cache, ఖండం++);
			జ్ఞాపకం_నకలు(cache->buf, cp, channel->ఖండం_కొలత);
			cp += channel->ఖండం_కొలత;
		}
	}
	తిరిగి 0;
#అయితే_ముగింపు /* NO_IO_CACHE */
}

స్థిర errcode_t windows_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	తిరిగి windows_read_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t windows_write_blk64(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	నిర్మాణం windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	స్థిరరాశి అక్షరం	*cp;
	పూర్ణాంకం		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#నిర్వచితమైతే NO_IO_CACHE
	తిరిగి raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
#లేకపోతే
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	ఒకవేళ (లెక్క < 0 || లెక్క > WRITE_ప్రత్యక్ష_కొలత) {
		ఒకవేళ ((retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 1)))
			తిరిగి retval;
		తిరిగి raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	ఒకవేళ (writethrough)
		retval = raw_write_blk(channel, దత్తాంశం, ఖండం, లెక్క, buf);

	cp = buf;
	అయితే (లెక్క > 0) {
		cache = కనుగొను_cached_ఖండం(దత్తాంశం, ఖండం, &reuse);
		ఒకవేళ (!cache) {
			cache = reuse;
			reuse_cache(channel, దత్తాంశం, cache, ఖండం);
		}
		ఒకవేళ (cache->buf != cp)
			జ్ఞాపకం_నకలు(cache->buf, cp, channel->ఖండం_కొలత);
		cache->dirty = !writethrough;
		లెక్క--;
		ఖండం++;
		cp += channel->ఖండం_కొలత;
	}
	తిరిగి retval;
#అయితే_ముగింపు /* NO_IO_CACHE */
}

స్థిర errcode_t windows_cache_readahead(io_channel channel,
				      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
}

స్థిర errcode_t windows_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	తిరిగి windows_write_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t windows_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				 పూర్ణాంకం కొలత, స్థిరరాశి శూన్య *buf)
{
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
స్థిర errcode_t windows_flush(io_channel channel)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#నిర్వచితం_కాకపోతే NO_IO_CACHE
	retval = flush_cached_ఖండాలు(channel, దత్తాంశం, 0);
#అయితే_ముగింపు

	తిరిగి retval;
}

స్థిర errcode_t windows_సమితి_option(io_channel channel, స్థిరరాశి అక్షరం *option,
				 స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	గుర్తులేని దీర్ఘ దీర్ఘ tmp;
	అక్షరం *చివర;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

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
	తిరిగి EXT2_ET_INVALID_వాదం;
}

స్థిర errcode_t windows_discard(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				 గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wunused-parameter"
#అయితే_ముగింపు
స్థిర errcode_t windows_zeroout(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	పూర్ణాంకం		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం windows_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	ఒకవేళ (channel->flags & CHANNEL_FLAGS_ఖండం_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		వెళ్ళు unimplemented;
	} లేకపోతే {
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
		వెళ్ళు unimplemented;
	}
err:
	ఒకవేళ (ret < 0) {
		ఒకవేళ (errno == EOPNOTSUPP)
			వెళ్ళు unimplemented;
		తిరిగి errno;
	}
	తిరిగి 0;
unimplemented:
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

పూర్ణాంకం ext2fs_open_file(స్థిరరాశి అక్షరం *pathname, పూర్ణాంకం flags, mode_t బహుళకం)
{
	flags |= O_ద్విమాన;

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

#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు

స్థిర నిర్మాణం struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.సమితి_blksize	= windows_సమితి_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.సమితి_option	= windows_సమితి_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
