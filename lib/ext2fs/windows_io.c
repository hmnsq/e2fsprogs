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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#यदि HAVE_SYS_STAT_H
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
#अन्यथा
#define TRACE(...) गर { } जबसम्म (0);
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) फर्काऊ (code)
#define EXT2_CHECK_MAGIC_RETURN(संरचना, code, ret) \
	  यदि ((संरचना)->magic != (code)) फर्काऊ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

संरचना windows_cache {
	वर्ण			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णाङ्क			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
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
	यदि (म_भेदन(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		म_नकल(data->cf_device, data->name);
		फर्काऊ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	यदि (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		फर्काऊ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	फर्काऊ 0;
}

स्थिर शून्य remove_fake_dos_name(संरचना windows_private_data *data)
{
	यदि (*data->dos_device) {
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

	यदि (stats)
		*stats = &data->io_stats;

	फर्काऊ retval;
}

स्थिर LARGE_INTEGER make_large_integer(LONGLONG मान)
{
	LARGE_INTEGER	li;

	li.QuadPart = मान;
	फर्काऊ li;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना windows_private_data *data,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णाङ्क गन्ती, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	अचिन्हित वर्ण	*buf = bufv;
	ssize_t		really_read = 0;

	size = (गन्ती < 0) ? -गन्ती : गन्ती * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	यदि (data->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		गच्छ bounce_read;
	}

	यदि (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
		retval = GetLastError();
		गच्छ error_out;
	}
	यदि ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		यदि (!ReadFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		यदि (actual != size) {
	short_read:
		यदि (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} अन्यथा
			retval = EXT2_ET_SHORT_READ;
		गच्छ error_out;
		}
	फर्काऊ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	जबसम्म (size > 0) {
		यदि (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		यदि (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			गच्छ short_read;
		}
		actual = size;
		यदि (size > channel->block_size)
			actual = channel->block_size;
		स_नकल(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	फर्काऊ 0;

error_out:
	यदि (actual >= 0 && actual < size)
		स_रखो((वर्ण *) buf+actual, 0, size-actual);
	यदि (channel->read_error)
		retval = (channel->read_error)(channel, block, गन्ती, buf,
					       size, actual, retval);
	फर्काऊ retval;
}

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना windows_private_data *data,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गन्ती, अचर शून्य *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	अचर अचिन्हित वर्ण *buf = bufv;

	यदि (गन्ती == 1)
		size = channel->block_size;
	अन्यथा {
		यदि (गन्ती < 0)
			size = -गन्ती;
		अन्यथा
			size = गन्ती * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	यदि (data->flags & IO_FLAG_FORCE_BOUNCE) {
		यदि (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
	गच्छ bounce_write;
	}

		यदि (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
		गच्छ error_out;
	}

	SetLastError(0);

	यदि ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		यदि (!WriteFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		यदि (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_out;
		}
		फर्काऊ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	जबसम्म (size > 0) {
		यदि (size < channel->block_size) {
			यदि (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
				retval = GetLastError();
				गच्छ error_out;
			}
			यदि (actual != channel->block_size) {
				यदि (actual < 0) {
					retval = GetLastError();
					गच्छ error_out;
				}
				स_रखो((वर्ण *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		यदि (size > channel->block_size)
			actual = channel->block_size;
		स_नकल(data->bounce, buf, actual);
		यदि (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		यदि (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		यदि (actual != channel->block_size)
			गच्छ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	फर्काऊ 0;

error_out:
	यदि (channel->write_error)
		retval = (channel->write_error)(channel, block, गन्ती, buf,
						size, actual, retval);
	फर्काऊ retval;
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
	लागि (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
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
स्थिर शून्य free_cache(संरचना windows_private_data *data)
{
	संरचना windows_cache    *cache;
	पूर्णाङ्क			i;

	data->access_time = 0;
	लागि (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
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
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
स्थिर संरचना windows_cache *find_cached_block(संरचना windows_private_data *data,
					    अचिन्हित दीर्घ दीर्घ block,
					    संरचना windows_cache **eldest)
{
	संरचना windows_cache	*cache, *unused_cache, *oldest_cache;
	पूर्णाङ्क			i;

	unused_cache = oldest_cache = 0;
	लागि (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		यदि (!cache->in_use) {
			यदि (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		यदि (cache->block == block) {
			cache->access_time = ++data->access_time;
			फर्काऊ cache;
		}
		यदि (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	यदि (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	फर्काऊ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर शून्य reuse_cache(io_channel channel, संरचना windows_private_data *data,
		 संरचना windows_cache *cache, अचिन्हित दीर्घ दीर्घ block)
{
	यदि (cache->dirty && cache->in_use)
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
	लागि (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		यदि (!cache->in_use)
			जारी;

		यदि (invalidate)
			cache->in_use = 0;

		यदि (!cache->dirty)
			जारी;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		यदि (retval)
			retval2 = retval;
		अन्यथा
			cache->dirty = 0;
	}
	फर्काऊ retval2;
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
	यदि (retval)
		गच्छ cleanup;
	स_रखो(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(data->name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	म_नकल(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#यदि defined(O_DIRECT)
	यदि (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	यदि (ext2fs_fstat(data->dev, &st) == 0) {
		यदि (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		अन्यथा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

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

	*channel = io;
	फर्काऊ 0;

cleanup:
	यदि (data) {
		यदि (data->dev >= 0)
			बंद(data->dev);
		free_cache(data);
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

स्थिर DWORD windows_open_device(संरचना windows_private_data *data, पूर्णाङ्क open_flags)
{
	DWORD ret = 0;

	यदि (*data->name != '\\')
		म_नकल(data->cf_device, data->name);
	अन्यथा यदि (fake_dos_name_for_device(data))
		फर्काऊ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#यदि defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#अन्यथा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, रिक्त, OPEN_EXISTING,
				  flags_and_attributes, रिक्त);

	यदि (data->handle == INVALID_HANDLE_मान) {
		ret = GetLastError();
		गच्छ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	यदि (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		गच्छ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	फर्काऊ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, त्रुटि_सं);
	फर्काऊ ret;
}

स्थिर संरचना windows_private_data *init_private_data(अचर वर्ण *name, पूर्णाङ्क flags)
{
	संरचना windows_private_data *data = रिक्त;

	यदि (ext2fs_get_mem(परिमाण(संरचना windows_private_data), &data))
		फर्काऊ रिक्त;

	स_रखो(data, 0, परिमाण(संरचना windows_private_data));
	म_नकलन(data->name, name, परिमाण(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.अङ्क_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_मान;

	फर्काऊ data;
}

स्थिर errcode_t windows_open(अचर वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	पूर्णाङ्क open_flags;
	संरचना windows_private_data *data;

	यदि (name == 0)
		फर्काऊ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	यदि (!data)
		फर्काऊ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	यदि (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#यदि defined(O_DIRECT)
	यदि (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	यदि (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		फर्काऊ EXT2_ET_BAD_DEVICE_NAME;
	}

	फर्काऊ windows_open_channel(data, flags, channel, windows_io_manager);
}

स्थिर errcode_t windows_close(io_channel channel)
{
	संरचना windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		फर्काऊ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	यदि (_close(data->dev) != 0)
		retval = त्रुटि_सं;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	फर्काऊ retval;
}

स्थिर errcode_t windows_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		यदि ((retval = flush_cached_blocks(channel, data, 0)))
			फर्काऊ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		यदि ((retval = alloc_cache(channel, data)))
			फर्काऊ retval;
	}
	फर्काऊ 0;
}

स्थिर errcode_t windows_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गन्ती, शून्य *buf)
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
	फर्काऊ raw_read_blk(channel, data, block, गन्ती, buf);
#अन्यथा
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
	जबसम्म (गन्ती > 0) {
		/* If it's in the cache, use it! */
		यदि ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			म_लिखो("Using cached block %lu\n", block);
#endif
			स_नकल(cp, cache->buf, channel->block_size);
			गन्ती--;
			block++;
			cp += channel->block_size;
			जारी;
		}
		यदि (गन्ती == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			यदि ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				फर्काऊ retval;
			}
			स_नकल(cp, cache->buf, channel->block_size);
			फर्काऊ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		लागि (i=1; i < गन्ती; i++)
			यदि (find_cached_block(data, block+i, &reuse[i]))
				रोक;
#ifdef DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#endif
		यदि ((retval = raw_read_blk(channel, data, block, i, cp)))
			फर्काऊ retval;

		/* Save the results in the cache */
		लागि (j=0; j < i; j++) {
			गन्ती--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			स_नकल(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	फर्काऊ 0;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गन्ती, शून्य *buf)
{
	फर्काऊ windows_read_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t windows_write_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block,
				पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	संरचना windows_private_data *data;
	संरचना windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	अचर वर्ण	*cp;
	पूर्णाङ्क		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	फर्काऊ raw_write_blk(channel, data, block, गन्ती, buf);
#अन्यथा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	यदि (गन्ती < 0 || गन्ती > WRITE_DIRECT_SIZE) {
		यदि ((retval = flush_cached_blocks(channel, data, 1)))
			फर्काऊ retval;
		फर्काऊ raw_write_blk(channel, data, block, गन्ती, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	यदि (writethrough)
		retval = raw_write_blk(channel, data, block, गन्ती, buf);

	cp = buf;
	जबसम्म (गन्ती > 0) {
		cache = find_cached_block(data, block, &reuse);
		यदि (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		यदि (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		गन्ती--;
		block++;
		cp += channel->block_size;
	}
	फर्काऊ retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गन्ती)
{
	फर्काऊ EXT2_ET_OP_NOT_SUPPORTED;
}

स्थिर errcode_t windows_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	फर्काऊ windows_write_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t windows_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णाङ्क size, अचर शून्य *buf)
{
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
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

	फर्काऊ retval;
}

स्थिर errcode_t windows_set_option(io_channel channel, अचर वर्ण *option,
				 अचर वर्ण *arg)
{
	संरचना windows_private_data *data;
	अचिन्हित दीर्घ दीर्घ tmp;
	वर्ण *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

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
	फर्काऊ EXT2_ET_INVALID_ARGUMENT;
}

स्थिर errcode_t windows_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 अचिन्हित दीर्घ दीर्घ गन्ती)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic थप
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
स्थिर errcode_t windows_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गन्ती)
{
	संरचना windows_private_data *data;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	यदि (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		गच्छ unimplemented;
	} अन्यथा {
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
		गच्छ unimplemented;
	}
err:
	यदि (ret < 0) {
		यदि (त्रुटि_सं == EOPNOTSUPP)
			गच्छ unimplemented;
		फर्काऊ त्रुटि_सं;
	}
	फर्काऊ 0;
unimplemented:
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

पूर्णाङ्क ext2fs_open_file(अचर वर्ण *pathname, पूर्णाङ्क flags, mode_t mode)
{
	flags |= O_द्विआधारी;

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

#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic झिक
#endif

स्थिर संरचना struct_io_manager struct_windows_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Windows I/O Manager",
	.खोलो		= windows_open,
	.बंद		= windows_close,
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
