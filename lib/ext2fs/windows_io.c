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

#जइ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#define _FILE_OFFSET_BITS 64
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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जइ HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    वर्ण __log[256];\
		    snprintf(__log, परिमाण(__log), __VA_ARGS__);\
		    __log[परिमाण(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#अण्णहा
#define TRACE(...) करेहि { } जाव (0);
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जइ ((संरचना)->magic != (code)) प्रत्यागम (code)
#define EXT2_CHECK_MAGIC_RETURN(संरचना, code, ret) \
	  जइ ((संरचना)->magic != (code)) प्रत्यागम (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

संरचना windows_cache {
	वर्ण			*buf;
	unsigned long long	block;
	पूर्णाङ्क			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

संरचना windows_private_data {
	पूर्णाङ्क	magic;
	वर्ण	name[MAX_PATH];
	HANDLE	handle;
	वर्ण	dos_device[MAX_PATH];
	वर्ण	cf_device[MAX_PATH];
	पूर्णाङ्क	dev;
	पूर्णाङ्क	flags;
	पूर्णाङ्क	align;
	पूर्णाङ्क	access_time;
	ext2_loff_t offset;
	संरचना windows_cache cache[CACHE_SIZE];
	शून्य	*bounce;
	संरचना struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

स्थिर पूर्णाङ्क fake_dos_name_for_device(संरचना windows_private_data *data)
{
	जइ (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		प्रत्यागम 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	जइ (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		प्रत्यागम 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	प्रत्यागम 0;
}

स्थिर शून्य remove_fake_dos_name(संरचना windows_private_data *data)
{
	जइ (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

स्थिर errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	संरचना windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जइ (stats)
		*stats = &data->io_stats;

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
			      संरचना windows_private_data *data,
			      unsigned long long block,
			      पूर्णाङ्क गणनम्, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned वर्ण	*buf = bufv;
	ssize_t		really_read = 0;

	size = (गणनम् < 0) ? -गणनम् : गणनम् * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जइ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		जइ (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		गच्छ bounce_read;
	}

	जइ (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
		retval = GetLastError();
		गच्छ error_out;
	}
	जइ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		जइ (!ReadFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		जइ (actual != size) {
	short_read:
		जइ (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} अण्णहा
			retval = EXT2_ET_SHORT_READ;
		गच्छ error_out;
		}
	प्रत्यागम 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	जाव (size > 0) {
		जइ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		जइ (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			गच्छ short_read;
		}
		actual = size;
		जइ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	प्रत्यागम 0;

error_out:
	जइ (actual >= 0 && actual < size)
		memset((वर्ण *) buf+actual, 0, size-actual);
	जइ (channel->read_error)
		retval = (channel->read_error)(channel, block, गणनम्, buf,
					       size, actual, retval);
	प्रत्यागम retval;
}

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना windows_private_data *data,
			       unsigned long long block,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	ध्रुव unsigned वर्ण *buf = bufv;

	जइ (गणनम् == 1)
		size = channel->block_size;
	अण्णहा {
		जइ (गणनम् < 0)
			size = -गणनम्;
		अण्णहा
			size = गणनम् * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जइ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		जइ (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
	गच्छ bounce_write;
	}

		जइ (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
		गच्छ error_out;
	}

	SetLastError(0);

	जइ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		जइ (!WriteFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		जइ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_out;
		}
		प्रत्यागम 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	जाव (size > 0) {
		जइ (size < channel->block_size) {
			जइ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
				retval = GetLastError();
				गच्छ error_out;
			}
			जइ (actual != channel->block_size) {
				जइ (actual < 0) {
					retval = GetLastError();
					गच्छ error_out;
				}
				memset((वर्ण *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		जइ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		जइ (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		जइ (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		जइ (actual != channel->block_size)
			गच्छ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	प्रत्यागम 0;

error_out:
	जइ (channel->write_error)
		retval = (channel->write_error)(channel, block, गणनम्, buf,
						size, actual, retval);
	प्रत्यागम retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
स्थिर errcode_t alloc_cache(io_channel channel,
			     संरचना windows_private_data *data)
{
	errcode_t		retval;
	संरचना windows_cache	*cache;
	पूर्णाङ्क			i;

	data->access_time = 0;
	प्रति (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		जइ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		जइ (retval)
			प्रत्यागम retval;
	}
	जइ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		जइ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	प्रत्यागम retval;
}

/* Free the cache buffers */
स्थिर शून्य free_cache(संरचना windows_private_data *data)
{
	संरचना windows_cache    *cache;
	पूर्णाङ्क			i;

	data->access_time = 0;
	प्रति (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		जइ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	जइ (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
स्थिर संरचना windows_cache *find_cached_block(संरचना windows_private_data *data,
					    unsigned long long block,
					    संरचना windows_cache **eldest)
{
	संरचना windows_cache	*cache, *unused_cache, *oldest_cache;
	पूर्णाङ्क			i;

	unused_cache = oldest_cache = 0;
	प्रति (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		जइ (!cache->in_use) {
			जइ (!unused_cache)
				unused_cache = cache;
			अनुवर्त;
		}
		जइ (cache->block == block) {
			cache->access_time = ++data->access_time;
			प्रत्यागम cache;
		}
		जइ (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	जइ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	प्रत्यागम 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर शून्य reuse_cache(io_channel channel, संरचना windows_private_data *data,
		 संरचना windows_cache *cache, unsigned long long block)
{
	जइ (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
स्थिर errcode_t flush_cached_blocks(io_channel channel,
				     संरचना windows_private_data *data,
				     पूर्णाङ्क invalidate)
{
	संरचना windows_cache	*cache;
	errcode_t	retval, retval2;
	पूर्णाङ्क			i;

	retval2 = 0;
	प्रति (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		जइ (!cache->in_use)
			अनुवर्त;

		जइ (invalidate)
			cache->in_use = 0;

		जइ (!cache->dirty)
			अनुवर्त;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		जइ (retval)
			retval2 = retval;
		अण्णहा
			cache->dirty = 0;
	}
	प्रत्यागम retval2;
}
#endif /* NO_IO_CACHE */

स्थिर errcode_t windows_open_channel(संरचना windows_private_data *data,
				   पूर्णाङ्क flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	जइ (retval)
		गच्छ cleanup;
	memset(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	जइ (retval)
		गच्छ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#जइ defined(O_DIRECT)
	जइ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	जइ (ext2fs_fstat(data->dev, &st) == 0) {
		जइ (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		अण्णहा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	जइ ((retval = alloc_cache(io, data)))
		गच्छ cleanup;

#ifdef BLKROGET
	जइ (flags & IO_FLAG_RW) {
		पूर्णाङ्क error;
		पूर्णाङ्क readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		जइ (!error && readonly) {
			retval = EPERM;
			गच्छ cleanup;
		}
	}
#endif

	*channel = io;
	प्रत्यागम 0;

cleanup:
	जइ (data) {
		जइ (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	जइ (io) {
		जइ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	प्रत्यागम retval;
}

स्थिर DWORD windows_open_device(संरचना windows_private_data *data, पूर्णाङ्क open_flags)
{
	DWORD ret = 0;

	जइ (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	अण्णहा जइ (fake_dos_name_for_device(data))
		प्रत्यागम -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#जइ defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#अण्णहा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, रिक्त, OPEN_EXISTING,
				  flags_and_attributes, रिक्त);

	जइ (data->handle == INVALID_HANDLE_मानम्) {
		ret = GetLastError();
		गच्छ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	जइ (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		गच्छ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	प्रत्यागम 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	प्रत्यागम ret;
}

स्थिर संरचना windows_private_data *init_private_data(ध्रुव वर्ण *name, पूर्णाङ्क flags)
{
	संरचना windows_private_data *data = रिक्त;

	जइ (ext2fs_get_mem(परिमाण(संरचना windows_private_data), &data))
		प्रत्यागम रिक्त;

	memset(data, 0, परिमाण(संरचना windows_private_data));
	strncpy(data->name, name, परिमाण(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.अङ्कः_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_मानम्;

	प्रत्यागम data;
}

स्थिर errcode_t windows_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	पूर्णाङ्क open_flags;
	संरचना windows_private_data *data;

	जइ (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	जइ (!data)
		प्रत्यागम EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	जइ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#जइ defined(O_DIRECT)
	जइ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	जइ (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	}

	प्रत्यागम windows_open_channel(data, flags, channel, windows_io_manager);
}

स्थिर errcode_t windows_close(io_channel channel)
{
	संरचना windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जइ (--channel->refcount > 0)
		प्रत्यागम 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	जइ (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	जइ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t windows_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जइ (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		जइ ((retval = flush_cached_blocks(channel, data, 0)))
			प्रत्यागम retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		जइ ((retval = alloc_cache(channel, data)))
			प्रत्यागम retval;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t windows_read_blk64(io_channel channel, unsigned long long block, पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना windows_private_data *data;
	संरचना windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	वर्ण		*cp;
	पूर्णाङ्क		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	प्रत्यागम raw_read_blk(channel, data, block, गणनम्, buf);
#अण्णहा
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	जइ (गणनम् < 0 || गणनम् > WRITE_DIRECT_SIZE) {
		जइ ((retval = flush_cached_blocks(channel, data, 0)))
			प्रत्यागम retval;
		प्रत्यागम raw_read_blk(channel, data, block, गणनम्, buf);
	}

	cp = buf;
	जाव (गणनम् > 0) {
		/* If it's in the cache, use it! */
		जइ ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			गणनम्--;
			block++;
			cp += channel->block_size;
			अनुवर्त;
		}
		जइ (गणनम् == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			जइ ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				प्रत्यागम retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			प्रत्यागम 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		प्रति (i=1; i < गणनम्; i++)
			जइ (find_cached_block(data, block+i, &reuse[i]))
				भङ्ग;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		जइ ((retval = raw_read_blk(channel, data, block, i, cp)))
			प्रत्यागम retval;

		/* Save the results in the cache */
		प्रति (j=0; j < i; j++) {
			गणनम्--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	प्रत्यागम 0;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम windows_read_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना windows_private_data *data;
	संरचना windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	ध्रुव वर्ण	*cp;
	पूर्णाङ्क		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	प्रत्यागम raw_write_blk(channel, data, block, गणनम्, buf);
#अण्णहा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	जइ (गणनम् < 0 || गणनम् > WRITE_DIRECT_SIZE) {
		जइ ((retval = flush_cached_blocks(channel, data, 1)))
			प्रत्यागम retval;
		प्रत्यागम raw_write_blk(channel, data, block, गणनम्, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	जइ (writethrough)
		retval = raw_write_blk(channel, data, block, गणनम्, buf);

	cp = buf;
	जाव (गणनम् > 0) {
		cache = find_cached_block(data, block, &reuse);
		जइ (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		जइ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		गणनम्--;
		block++;
		cp += channel->block_size;
	}
	प्रत्यागम retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long गणनम्)
{
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
}

स्थिर errcode_t windows_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम windows_write_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 पूर्णाङ्क size, ध्रुव शून्य *buf)
{
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t windows_flush(io_channel channel)
{
	संरचना windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	प्रत्यागम retval;
}

स्थिर errcode_t windows_set_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना windows_private_data *data;
	unsigned long long tmp;
	वर्ण *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जइ (!strcmp(option, "offset")) {
		जइ (!arg)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		जइ (*end)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		जइ (data->offset < 0)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		प्रत्यागम 0;
	}
	प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
}

स्थिर errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long गणनम्)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#जइ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic प्रक्षिप
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
स्थिर errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long गणनम्)
{
	संरचना windows_private_data *data;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जइ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		गच्छ unimplemented;
	} अण्णहा {
		/* Regular file, try to use truncate/punch/zero. */
		संरचना stat statbuf;

		जइ (गणनम् == 0)
			प्रत्यागम 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		जइ (ret)
			गच्छ err;
		जइ ((unsigned long long) statbuf.st_size <
			(block + गणनम्) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + गणनम्) * channel->block_size + data->offset);
			जइ (ret)
				गच्छ err;
		}
		गच्छ unimplemented;
	}
err:
	जइ (ret < 0) {
		जइ (errno == EOPNOTSUPP)
			गच्छ unimplemented;
		प्रत्यागम errno;
	}
	प्रत्यागम 0;
unimplemented:
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

पूर्णाङ्क ext2fs_open_file(ध्रुव वर्ण *pathname, पूर्णाङ्क flags, mode_t mode)
{
	flags |= O_द्विमान;

	जइ (mode)
#जइ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		प्रत्यागम open64(pathname, flags, mode);
	अण्णहा
		प्रत्यागम open64(pathname, flags);
#अण्णहा
		प्रत्यागम open(pathname, flags, mode);
	अण्णहा
		प्रत्यागम open(pathname, flags);
#endif
}

पूर्णाङ्क ext2fs_stat(ध्रुव वर्ण *path, ext2fs_struct_stat *buf)
{
#जइ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	प्रत्यागम stat64(path, buf);
#अण्णहा
	प्रत्यागम stat(path, buf);
#endif
}

पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf)
{
#जइ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	प्रत्यागम fstat64(fd, buf);
#अण्णहा
	प्रत्यागम fstat(fd, buf);
#endif
}

#जइ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic निष्कासय
#endif

स्थिर संरचना struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.open		= windows_open,
	.close		= windows_close,
	.set_blksize	= windows_set_blksize,
	.read_blk	= windows_read_blk,
	.write_blk	= windows_write_blk,
	.flush		= windows_flush,
	.write_byte	= windows_write_byte,
	.set_option	= windows_set_option,
	.get_stats	= windows_get_stats,
	.read_blk64	= windows_read_blk64,
	.write_blk64	= windows_write_blk64,
	.discard	= windows_discard,
	.cache_readahead	= windows_cache_readahead,
	.zeroout	= windows_zeroout,
};

io_manager windows_io_manager = &struct_windows_manager;
