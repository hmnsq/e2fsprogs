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

#अन्तर्भाव <windows.h>
#अन्तर्भाव <winioctl.h>
#अन्तर्भाव <io.h>

#यदि !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#परिभाषय _XOPEN_SOURCE 600
#परिभाषय _DARWIN_C_SOURCE
#परिभाषय FILE_विस्थापनम्_द्वयङ्काः 64
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
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#परिभाषय PR_GET_DUMPABLE 3
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>

#अपरिभाषय ALIGN_DEBUG

//#define DEBUG
#यदि_परिभाषितम् DEBUG
#परिभाषय TRACE(...) {\
		    वर्ण __log[256];\
		    snprintf(__log, परिमाण(__log), __VA_ARGS__);\
		    __log[परिमाण(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#अन्यथा
#परिभाषय TRACE(...) कुरु { } यावत् (0);
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)
#परिभाषय EXT2_CHECK_MAGIC_RETURN(संरचना, संहिता, ret) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (ret)

#परिभाषय EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

संरचना windows_cache {
	वर्ण			*buf;
	अचिह्नित दीर्घ दीर्घ	खण्डम्;
	पूर्णाङ्क			access_time;
	अचिह्नित		dirty:1;
	अचिह्नित		अन्तर्_use:1;
};

#परिभाषय CACHE_आकार 8
#परिभाषय WRITE_प्रत्यक्षम्_आकार 4	/* Must be smaller than CACHE_SIZE */
#परिभाषय READ_प्रत्यक्षम्_आकार 4	/* Should be smaller than CACHE_SIZE */

संरचना windows_वैयक्तिकम्_दत्त {
	पूर्णाङ्क	magic;
	वर्ण	name[MAX_मार्गः];
	HANDLE	handle;
	वर्ण	dos_device[MAX_मार्गः];
	वर्ण	cf_device[MAX_मार्गः];
	पूर्णाङ्क	dev;
	पूर्णाङ्क	flags;
	पूर्णाङ्क	align;
	पूर्णाङ्क	access_time;
	ext2_loff_t विस्थापनम्;
	संरचना windows_cache cache[CACHE_आकार];
	शून्य	*bounce;
	संरचना struct_io_stats io_stats;
};

#परिभाषय IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

स्थिर पूर्णाङ्क fake_dos_name_for_device(संरचना windows_वैयक्तिकम्_दत्त *दत्त)
{
	यदि (वाक्ये_तुलय(दत्त->name, "\\\\", 2) == 0) {
		दत्त->dos_device[0] = 0;
		वाक्यं_प्रतिलिख(दत्त->cf_device, दत्त->name);
		प्रत्यागम 0;
	}

	_snprintf(दत्त->dos_device, MAX_मार्गः, "fakedevice%lu", GetCurrentProcessId());

	यदि (!DefineDosDevice(DDD_RAW_TARGET_मार्गः, दत्त->dos_device, दत्त->name))
		प्रत्यागम 1;

	_snprintf(दत्त->cf_device, MAX_मार्गः, "\\\\.\\%s", दत्त->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", दत्त->dos_device);

	प्रत्यागम 0;
}

स्थिर शून्य remove_fake_dos_name(संरचना windows_वैयक्तिकम्_दत्त *दत्त)
{
	यदि (*दत्त->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", दत्त->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_मार्गः | DDD_EXACT_मेलनम्_ON_REMOVE | DDD_REMOVE_DEFINITION, दत्त->dos_device, दत्त->name);
	}
}

स्थिर errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	संरचना windows_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (stats)
		*stats = &दत्त->io_stats;

	प्रत्यागम retval;
}

स्थिर LARGE_INTEGER make_large_integer(LONGLONG मानम्)
{
	LARGE_INTEGER	li;

	li.QuadPart = मानम्;
	प्रत्यागम li;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना windows_वैयक्तिकम्_दत्त *दत्त,
			      अचिह्नित दीर्घ दीर्घ खण्डम्,
			      पूर्णाङ्क गणनम्, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t	आकार;
	ext2_loff_t	location;
	DWORD		actual = 0;
	अचिह्नित वर्ण	*buf = bufv;
	ssize_t		really_read = 0;

	आकार = (गणनम् < 0) ? -गणनम् : गणनम् * channel->खण्डम्_आकार;
	दत्त->io_stats.अष्टकानि_read += आकार;
	location = ((ext2_loff_t) खण्डम् * channel->खण्डम्_आकार) + दत्त->विस्थापनम्;

	यदि (दत्त->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (!समुच्चयः_File_Pointer_Ex(दत्त->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}
		गच्छ bounce_read;
	}

	यदि (!समुच्चयः_File_Pointer_Ex(दत्त->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
		retval = Get_चरमः_Error();
		गच्छ error_बहिर्गमनम्;
	}
	यदि ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(आकार, channel->align))) {
		यदि (!ReadFile(दत्त->handle, buf, आकार, &actual, रिक्त)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}
		यदि (actual != आकार) {
	short_read:
		यदि (actual < 0) {
			retval = Get_चरमः_Error();
			actual = 0;
		} अन्यथा
			retval = EXT2_ET_SHORT_READ;
		गच्छ error_बहिर्गमनम्;
		}
	प्रत्यागम 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	यावत् (आकार > 0) {
		यदि (!ReadFile(दत्त->handle, दत्त->bounce, channel->खण्डम्_आकार, &actual, रिक्त)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}
		यदि (actual != channel->खण्डम्_आकार) {
			actual = really_read;
			buf -= really_read;
			आकार += really_read;
			गच्छ short_read;
		}
		actual = आकार;
		यदि (आकार > channel->खण्डम्_आकार)
			actual = channel->खण्डम्_आकार;
		स्मृतिं_प्रतिलिख(buf, दत्त->bounce, actual);
		really_read += actual;
		आकार -= actual;
		buf += actual;
	}
	प्रत्यागम 0;

error_बहिर्गमनम्:
	यदि (actual >= 0 && actual < आकार)
		स्मृतिं_पूरय((वर्ण *) buf+actual, 0, आकार-actual);
	यदि (channel->read_error)
		retval = (channel->read_error)(channel, खण्डम्, गणनम्, buf,
					       आकार, actual, retval);
	प्रत्यागम retval;
}

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना windows_वैयक्तिकम्_दत्त *दत्त,
			       अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *bufv)
{
	ssize_t		आकार;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ध्रुव अचिह्नित वर्ण *buf = bufv;

	यदि (गणनम् == 1)
		आकार = channel->खण्डम्_आकार;
	अन्यथा {
		यदि (गणनम् < 0)
			आकार = -गणनम्;
		अन्यथा
			आकार = गणनम् * channel->खण्डम्_आकार;
	}
	दत्त->io_stats.अष्टकानि_written += आकार;

	location = ((ext2_loff_t) खण्डम् * channel->खण्डम्_आकार) + दत्त->विस्थापनम्;

	यदि (दत्त->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (!समुच्चयः_File_Pointer_Ex(दत्त->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}
	गच्छ bounce_write;
	}

		यदि (!समुच्चयः_File_Pointer_Ex(दत्त->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = Get_चरमः_Error();
		गच्छ error_बहिर्गमनम्;
	}

	समुच्चयः_चरमः_Error(0);

	यदि ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(आकार, channel->align))) {
		यदि (!WriteFile(दत्त->handle, buf, आकार, &actual, रिक्त)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}

		यदि (actual != आकार) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_बहिर्गमनम्;
		}
		प्रत्यागम 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	यावत् (आकार > 0) {
		यदि (आकार < channel->खण्डम्_आकार) {
			यदि (!ReadFile(दत्त->handle, दत्त->bounce, channel->खण्डम्_आकार, &actual, रिक्त)) {
				retval = Get_चरमः_Error();
				गच्छ error_बहिर्गमनम्;
			}
			यदि (actual != channel->खण्डम्_आकार) {
				यदि (actual < 0) {
					retval = Get_चरमः_Error();
					गच्छ error_बहिर्गमनम्;
				}
				स्मृतिं_पूरय((वर्ण *) दत्त->bounce + actual, 0,
				       channel->खण्डम्_आकार - actual);
			}
		}
		actual = आकार;
		यदि (आकार > channel->खण्डम्_आकार)
			actual = channel->खण्डम्_आकार;
		स्मृतिं_प्रतिलिख(दत्त->bounce, buf, actual);
		यदि (!समुच्चयः_File_Pointer_Ex(दत्त->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}
		यदि (!WriteFile(दत्त->handle, दत्त->bounce, channel->खण्डम्_आकार, &actual, रिक्त)) {
			retval = Get_चरमः_Error();
			गच्छ error_बहिर्गमनम्;
		}

		यदि (actual != channel->खण्डम्_आकार)
			गच्छ short_write;
		आकार -= actual;
		buf += actual;
		location += actual;
	}
	प्रत्यागम 0;

error_बहिर्गमनम्:
	यदि (channel->write_error)
		retval = (channel->write_error)(channel, खण्डम्, गणनम्, buf,
						आकार, actual, retval);
	प्रत्यागम retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
स्थिर errcode_t alloc_cache(io_channel channel,
			     संरचना windows_वैयक्तिकम्_दत्त *दत्त)
{
	errcode_t		retval;
	संरचना windows_cache	*cache;
	पूर्णाङ्क			i;

	दत्त->access_time = 0;
	प्रति (i=0, cache = दत्त->cache; i < CACHE_आकार; i++, cache++) {
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
स्थिर शून्य free_cache(संरचना windows_वैयक्तिकम्_दत्त *दत्त)
{
	संरचना windows_cache    *cache;
	पूर्णाङ्क			i;

	दत्त->access_time = 0;
	प्रति (i=0, cache = दत्त->cache; i < CACHE_आकार; i++, cache++) {
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
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
स्थिर संरचना windows_cache *find_cached_खण्डम्(संरचना windows_वैयक्तिकम्_दत्त *दत्त,
					    अचिह्नित दीर्घ दीर्घ खण्डम्,
					    संरचना windows_cache **eldest)
{
	संरचना windows_cache	*cache, *अप्रयुक्तम्_cache, *oldest_cache;
	पूर्णाङ्क			i;

	अप्रयुक्तम्_cache = oldest_cache = 0;
	प्रति (i=0, cache = दत्त->cache; i < CACHE_आकार; i++, cache++) {
		यदि (!cache->अन्तर्_use) {
			यदि (!अप्रयुक्तम्_cache)
				अप्रयुक्तम्_cache = cache;
			अनुवर्त;
		}
		यदि (cache->खण्डम् == खण्डम्) {
			cache->access_time = ++दत्त->access_time;
			प्रत्यागम cache;
		}
		यदि (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	यदि (eldest)
		*eldest = (अप्रयुक्तम्_cache) ? अप्रयुक्तम्_cache : oldest_cache;
	प्रत्यागम 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर शून्य reuse_cache(io_channel channel, संरचना windows_वैयक्तिकम्_दत्त *दत्त,
		 संरचना windows_cache *cache, अचिह्नित दीर्घ दीर्घ खण्डम्)
{
	यदि (cache->dirty && cache->अन्तर्_use)
		raw_write_blk(channel, दत्त, cache->खण्डम्, 1, cache->buf);

	cache->अन्तर्_use = 1;
	cache->dirty = 0;
	cache->खण्डम् = खण्डम्;
	cache->access_time = ++दत्त->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
स्थिर errcode_t flush_cached_खण्डानि(io_channel channel,
				     संरचना windows_वैयक्तिकम्_दत्त *दत्त,
				     पूर्णाङ्क invalidate)
{
	संरचना windows_cache	*cache;
	errcode_t	retval, retval2;
	पूर्णाङ्क			i;

	retval2 = 0;
	प्रति (i=0, cache = दत्त->cache; i < CACHE_आकार; i++, cache++) {
		यदि (!cache->अन्तर्_use)
			अनुवर्त;

		यदि (invalidate)
			cache->अन्तर्_use = 0;

		यदि (!cache->dirty)
			अनुवर्त;

		retval = raw_write_blk(channel, दत्त,
				       cache->खण्डम्, 1, cache->buf);
		यदि (retval)
			retval2 = retval;
		अन्यथा
			cache->dirty = 0;
	}
	प्रत्यागम retval2;
}
#यदि_अन्तः /* NO_IO_CACHE */

स्थिर errcode_t windows_open_channel(संरचना windows_वैयक्तिकम्_दत्त *दत्त,
				   पूर्णाङ्क flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(दत्त->name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(io->name, दत्त->name);
	io->वैयक्तिकम्_दत्त = दत्त;
	io->खण्डम्_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#यदि defined(O_प्रत्यक्षम्)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO)
		io->align = ext2fs_get_dio_alignment(दत्त->dev);
#यदि_अन्तः

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	यदि (ext2fs_fstat(दत्त->dev, &st) == 0) {
		यदि (ext2fsP_is_disk_device(st.st_बहुलकम्))
			io->flags |= CHANNEL_FLAGS_खण्डम्_DEVICE;
		अन्यथा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

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

	*channel = io;
	प्रत्यागम 0;

cleanup:
	यदि (दत्त) {
		यदि (दत्त->dev >= 0)
			close(दत्त->dev);
		free_cache(दत्त);
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

स्थिर DWORD windows_open_device(संरचना windows_वैयक्तिकम्_दत्त *दत्त, पूर्णाङ्क open_flags)
{
	DWORD ret = 0;

	यदि (*दत्त->name != '\\')
		वाक्यं_प्रतिलिख(दत्त->cf_device, दत्त->name);
	अन्यथा यदि (fake_dos_name_for_device(दत्त))
		प्रत्यागम -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_बहुलकम् = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_गुणाः =
#यदि defined(O_प्रत्यक्षम्)
	(open_flags & O_प्रत्यक्षम्) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_प्रसामान्यम्;
#अन्यथा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#यदि_अन्तः

	दत्त->handle = CreateFile(दत्त->cf_device, desired_access, share_बहुलकम्, रिक्त, OPEN_EXISTING,
				  flags_and_गुणाः, रिक्त);

	यदि (दत्त->handle == INVALID_HANDLE_मानम्) {
		ret = Get_चरमः_Error();
		गच्छ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", दत्त->cf_device, दत्त->handle);

	दत्त->dev = _open_osfhandle((intptr_t)दत्त->handle, 0);
	यदि (दत्त->dev < 0) {
		ret = Get_चरमः_Error() ? Get_चरमः_Error() : 9999;
		गच्छ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", दत्त->handle, दत्त->dev);

	प्रत्यागम 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", दत्त->handle);
	CloseHandle(दत्त->handle);
invalid_handle:
	remove_fake_dos_name(दत्त);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	प्रत्यागम ret;
}

स्थिर संरचना windows_वैयक्तिकम्_दत्त *init_वैयक्तिकम्_दत्त(ध्रुव वर्ण *name, पूर्णाङ्क flags)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त = रिक्त;

	यदि (ext2fs_get_mem(परिमाण(संरचना windows_वैयक्तिकम्_दत्त), &दत्त))
		प्रत्यागम रिक्त;

	स्मृतिं_पूरय(दत्त, 0, परिमाण(संरचना windows_वैयक्तिकम्_दत्त));
	strncpy(दत्त->name, name, परिमाण(दत्त->name) - 1);
	दत्त->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	दत्त->io_stats.अङ्कः_fields = 2;
	दत्त->flags = flags;
	दत्त->handle = INVALID_HANDLE_मानम्;

	प्रत्यागम दत्त;
}

स्थिर errcode_t windows_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	पूर्णाङ्क open_flags;
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;

	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	दत्त = init_वैयक्तिकम्_दत्त(name, flags);
	यदि (!दत्त)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	यदि (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#यदि defined(O_प्रत्यक्षम्)
	यदि (flags & IO_FLAG_प्रत्यक्षम्_IO)
		open_flags |= O_प्रत्यक्षम्;
#यदि_अन्तः

	यदि (windows_open_device(दत्त, open_flags)) {
		ext2fs_free_mem(&दत्त);
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	}

	प्रत्यागम windows_open_channel(दत्त, flags, channel, windows_io_manager);
}

स्थिर errcode_t windows_close(io_channel channel)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;

#यदि_अपरिभाषितम् NO_IO_CACHE
	retval = flush_cached_खण्डानि(channel, दत्त, 0);
#यदि_अन्तः

	remove_fake_dos_name(दत्त);

	यदि (_close(दत्त->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", दत्त->dev);

	free_cache(दत्त);

	ext2fs_free_mem(&channel->वैयक्तिकम्_दत्त);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t windows_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (channel->खण्डम्_आकार != blksize) {
#यदि_अपरिभाषितम् NO_IO_CACHE
		यदि ((retval = flush_cached_खण्डानि(channel, दत्त, 0)))
			प्रत्यागम retval;
#यदि_अन्तः

		channel->खण्डम्_आकार = blksize;
		free_cache(दत्त);
		यदि ((retval = alloc_cache(channel, दत्त)))
			प्रत्यागम retval;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t windows_read_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	संरचना windows_cache *cache, *reuse[READ_प्रत्यक्षम्_आकार];
	errcode_t	retval;
	वर्ण		*cp;
	पूर्णाङ्क		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#यदि_परिभाषितम् NO_IO_CACHE
	प्रत्यागम raw_read_blk(channel, दत्त, खण्डम्, गणनम्, buf);
#अन्यथा
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
	यावत् (गणनम् > 0) {
		/* If it's in the cache, use it! */
		यदि ((cache = find_cached_खण्डम्(दत्त, खण्डम्, &reuse[0]))) {
#यदि_परिभाषितम् DEBUG
			प्रारूप_लिख("Using cached block %lu\n", खण्डम्);
#यदि_अन्तः
			स्मृतिं_प्रतिलिख(cp, cache->buf, channel->खण्डम्_आकार);
			गणनम्--;
			खण्डम्++;
			cp += channel->खण्डम्_आकार;
			अनुवर्त;
		}
		यदि (गणनम् == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, दत्त, cache, खण्डम्);
			यदि ((retval = raw_read_blk(channel, दत्त, खण्डम्, 1,
						   cache->buf))) {
				cache->अन्तर्_use = 0;
				प्रत्यागम retval;
			}
			स्मृतिं_प्रतिलिख(cp, cache->buf, channel->खण्डम्_आकार);
			प्रत्यागम 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		प्रति (i=1; i < गणनम्; i++)
			यदि (find_cached_खण्डम्(दत्त, खण्डम्+i, &reuse[i]))
				भङ्ग;
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("Reading %d blocks starting at %lu\n", i, खण्डम्);
#यदि_अन्तः
		यदि ((retval = raw_read_blk(channel, दत्त, खण्डम्, i, cp)))
			प्रत्यागम retval;

		/* Save the results in the cache */
		प्रति (j=0; j < i; j++) {
			गणनम्--;
			cache = reuse[j];
			reuse_cache(channel, दत्त, cache, खण्डम्++);
			स्मृतिं_प्रतिलिख(cache->buf, cp, channel->खण्डम्_आकार);
			cp += channel->खण्डम्_आकार;
		}
	}
	प्रत्यागम 0;
#यदि_अन्तः /* NO_IO_CACHE */
}

स्थिर errcode_t windows_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम windows_read_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t windows_write_blk64(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	संरचना windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ध्रुव वर्ण	*cp;
	पूर्णाङ्क		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#यदि_परिभाषितम् NO_IO_CACHE
	प्रत्यागम raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf);
#अन्यथा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	यदि (गणनम् < 0 || गणनम् > WRITE_प्रत्यक्षम्_आकार) {
		यदि ((retval = flush_cached_खण्डानि(channel, दत्त, 1)))
			प्रत्यागम retval;
		प्रत्यागम raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	यदि (writethrough)
		retval = raw_write_blk(channel, दत्त, खण्डम्, गणनम्, buf);

	cp = buf;
	यावत् (गणनम् > 0) {
		cache = find_cached_खण्डम्(दत्त, खण्डम्, &reuse);
		यदि (!cache) {
			cache = reuse;
			reuse_cache(channel, दत्त, cache, खण्डम्);
		}
		यदि (cache->buf != cp)
			स्मृतिं_प्रतिलिख(cache->buf, cp, channel->खण्डम्_आकार);
		cache->dirty = !writethrough;
		गणनम्--;
		खण्डम्++;
		cp += channel->खण्डम्_आकार;
	}
	प्रत्यागम retval;
#यदि_अन्तः /* NO_IO_CACHE */
}

स्थिर errcode_t windows_cache_readahead(io_channel channel,
				      अचिह्नित दीर्घ दीर्घ खण्डम्,
				      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
}

स्थिर errcode_t windows_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम windows_write_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t windows_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				 पूर्णाङ्क आकार, ध्रुव शून्य *buf)
{
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t windows_flush(io_channel channel)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#यदि_अपरिभाषितम् NO_IO_CACHE
	retval = flush_cached_खण्डानि(channel, दत्त, 0);
#यदि_अन्तः

	प्रत्यागम retval;
}

स्थिर errcode_t windows_समुच्चयः_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	अचिह्नित दीर्घ दीर्घ tmp;
	वर्ण *अन्तः;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

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
	प्रत्यागम EXT2_ET_INVALID_तर्कः;
}

स्थिर errcode_t windows_discard(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
				 अचिह्नित दीर्घ दीर्घ गणनम्)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wunused-parameter"
#यदि_अन्तः
स्थिर errcode_t windows_zeroout(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना windows_वैयक्तिकम्_दत्त *दत्त;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना windows_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (channel->flags & CHANNEL_FLAGS_खण्डम्_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		गच्छ unimplemented;
	} अन्यथा {
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
		गच्छ unimplemented;
	}
err:
	यदि (ret < 0) {
		यदि (errno == EOPNOTSUPP)
			गच्छ unimplemented;
		प्रत्यागम errno;
	}
	प्रत्यागम 0;
unimplemented:
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

पूर्णाङ्क ext2fs_open_file(ध्रुव वर्ण *pathname, पूर्णाङ्क flags, mode_t बहुलकम्)
{
	flags |= O_द्विमान;

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

#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः

स्थिर संरचना struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.उद्घाटय		= windows_open,
	.close		= windows_close,
	.समुच्चयः_blksize	= windows_समुच्चयः_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.समुच्चयः_option	= windows_समुच्चयः_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
