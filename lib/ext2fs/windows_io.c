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

#जर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    अक्षर __log[256];\
		    snprintf(__log, आकार(__log), __VA_ARGS__);\
		    __log[आकार(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#अन्यथा
#define TRACE(...) करा { } पर्यंत (0);
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जर ((संरचना)->magic != (code)) परत (code)
#define EXT2_CHECK_MAGIC_RETURN(संरचना, code, ret) \
	  जर ((संरचना)->magic != (code)) परत (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

संरचना windows_cache {
	अक्षर			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णांक			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

संरचना windows_private_data {
	पूर्णांक	magic;
	अक्षर	name[MAX_PATH];
	HANDLE	handle;
	अक्षर	dos_device[MAX_PATH];
	अक्षर	cf_device[MAX_PATH];
	पूर्णांक	dev;
	पूर्णांक	flags;
	पूर्णांक	align;
	पूर्णांक	access_time;
	ext2_loff_t offset;
	संरचना windows_cache cache[CACHE_SIZE];
	शून्य	*bounce;
	संरचना struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

स्थिर पूर्णांक fake_dos_name_for_device(संरचना windows_private_data *data)
{
	जर (म_भेदन(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		म_नकल(data->cf_device, data->name);
		परत 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	जर (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		परत 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	परत 0;
}

स्थिर शून्य remove_fake_dos_name(संरचना windows_private_data *data)
{
	जर (*data->dos_device) {
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

	जर (stats)
		*stats = &data->io_stats;

	परत retval;
}

स्थिर LARGE_INTEGER make_large_integer(LONGLONG मूल्य)
{
	LARGE_INTEGER	li;

	li.QuadPart = मूल्य;
	परत li;
}

/*
 * Here are the raw I/O functions
 */
स्थिर errcode_t raw_read_blk(io_channel channel,
			      संरचना windows_private_data *data,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णांक मोजणी, शून्य *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	अचिन्हित अक्षर	*buf = bufv;
	ssize_t		really_read = 0;

	size = (मोजणी < 0) ? -मोजणी : मोजणी * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जर (data->flags & IO_FLAG_FORCE_BOUNCE) {
		जर (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			जा error_out;
		}
		जा bounce_read;
	}

	जर (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
		retval = GetLastError();
		जा error_out;
	}
	जर ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		जर (!ReadFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			जा error_out;
		}
		जर (actual != size) {
	short_read:
		जर (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} अन्यथा
			retval = EXT2_ET_SHORT_READ;
		जा error_out;
		}
	परत 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	पर्यंत (size > 0) {
		जर (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			जा error_out;
		}
		जर (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			जा short_read;
		}
		actual = size;
		जर (size > channel->block_size)
			actual = channel->block_size;
		स_नकल(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	परत 0;

error_out:
	जर (actual >= 0 && actual < size)
		स_रखो((अक्षर *) buf+actual, 0, size-actual);
	जर (channel->read_error)
		retval = (channel->read_error)(channel, block, मोजणी, buf,
					       size, actual, retval);
	परत retval;
}

स्थिर errcode_t raw_write_blk(io_channel channel,
			       संरचना windows_private_data *data,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, स्थिरांक शून्य *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	स्थिरांक अचिन्हित अक्षर *buf = bufv;

	जर (मोजणी == 1)
		size = channel->block_size;
	अन्यथा {
		जर (मोजणी < 0)
			size = -मोजणी;
		अन्यथा
			size = मोजणी * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	जर (data->flags & IO_FLAG_FORCE_BOUNCE) {
		जर (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			जा error_out;
		}
	जा bounce_write;
	}

		जर (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
		जा error_out;
	}

	SetLastError(0);

	जर ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		जर (!WriteFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			जा error_out;
		}

		जर (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			जा error_out;
		}
		परत 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	पर्यंत (size > 0) {
		जर (size < channel->block_size) {
			जर (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
				retval = GetLastError();
				जा error_out;
			}
			जर (actual != channel->block_size) {
				जर (actual < 0) {
					retval = GetLastError();
					जा error_out;
				}
				स_रखो((अक्षर *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		जर (size > channel->block_size)
			actual = channel->block_size;
		स_नकल(data->bounce, buf, actual);
		जर (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			जा error_out;
		}
		जर (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			जा error_out;
		}

		जर (actual != channel->block_size)
			जा short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	परत 0;

error_out:
	जर (channel->write_error)
		retval = (channel->write_error)(channel, block, मोजणी, buf,
						size, actual, retval);
	परत retval;
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
	पूर्णांक			i;

	data->access_time = 0;
	साठी (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
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
स्थिर शून्य free_cache(संरचना windows_private_data *data)
{
	संरचना windows_cache    *cache;
	पूर्णांक			i;

	data->access_time = 0;
	साठी (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
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
	पूर्णांक			i;

	unused_cache = oldest_cache = 0;
	साठी (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		जर (!cache->in_use) {
			जर (!unused_cache)
				unused_cache = cache;
			सुरूठेवा;
		}
		जर (cache->block == block) {
			cache->access_time = ++data->access_time;
			परत cache;
		}
		जर (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	जर (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	परत 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर शून्य reuse_cache(io_channel channel, संरचना windows_private_data *data,
		 संरचना windows_cache *cache, अचिन्हित दीर्घ दीर्घ block)
{
	जर (cache->dirty && cache->in_use)
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
				     पूर्णांक invalidate)
{
	संरचना windows_cache	*cache;
	errcode_t	retval, retval2;
	पूर्णांक			i;

	retval2 = 0;
	साठी (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		जर (!cache->in_use)
			सुरूठेवा;

		जर (invalidate)
			cache->in_use = 0;

		जर (!cache->dirty)
			सुरूठेवा;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		जर (retval)
			retval2 = retval;
		अन्यथा
			cache->dirty = 0;
	}
	परत retval2;
}
#endif /* NO_IO_CACHE */

स्थिर errcode_t windows_open_channel(संरचना windows_private_data *data,
				   पूर्णांक flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = रिक्त;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(आकार(संरचना struct_io_channel), &io);
	जर (retval)
		जा cleanup;
	स_रखो(io, 0, आकार(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(data->name)+1, &io->name);
	जर (retval)
		जा cleanup;

	म_नकल(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#जर defined(O_DIRECT)
	जर (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	जर (ext2fs_fstat(data->dev, &st) == 0) {
		जर (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		अन्यथा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

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

	*channel = io;
	परत 0;

cleanup:
	जर (data) {
		जर (data->dev >= 0)
			बंद(data->dev);
		free_cache(data);
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

स्थिर DWORD windows_open_device(संरचना windows_private_data *data, पूर्णांक open_flags)
{
	DWORD ret = 0;

	जर (*data->name != '\\')
		म_नकल(data->cf_device, data->name);
	अन्यथा जर (fake_dos_name_for_device(data))
		परत -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#जर defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#अन्यथा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, रिक्त, OPEN_EXISTING,
				  flags_and_attributes, रिक्त);

	जर (data->handle == INVALID_HANDLE_मूल्य) {
		ret = GetLastError();
		जा invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	जर (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		जा osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	परत 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, त्रुटि_सं);
	परत ret;
}

स्थिर संरचना windows_private_data *init_private_data(स्थिरांक अक्षर *name, पूर्णांक flags)
{
	संरचना windows_private_data *data = रिक्त;

	जर (ext2fs_get_mem(आकार(संरचना windows_private_data), &data))
		परत रिक्त;

	स_रखो(data, 0, आकार(संरचना windows_private_data));
	म_नकलन(data->name, name, आकार(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.अंक_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_मूल्य;

	परत data;
}

स्थिर errcode_t windows_open(स्थिरांक अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	पूर्णांक open_flags;
	संरचना windows_private_data *data;

	जर (name == 0)
		परत EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	जर (!data)
		परत EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	जर (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#जर defined(O_DIRECT)
	जर (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	जर (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		परत EXT2_ET_BAD_DEVICE_NAME;
	}

	परत windows_open_channel(data, flags, channel, windows_io_manager);
}

स्थिर errcode_t windows_close(io_channel channel)
{
	संरचना windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जर (--channel->refcount > 0)
		परत 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	जर (_close(data->dev) != 0)
		retval = त्रुटि_सं;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	जर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	परत retval;
}

स्थिर errcode_t windows_set_blksize(io_channel channel, पूर्णांक blksize)
{
	संरचना windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जर (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		जर ((retval = flush_cached_blocks(channel, data, 0)))
			परत retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		जर ((retval = alloc_cache(channel, data)))
			परत retval;
	}
	परत 0;
}

स्थिर errcode_t windows_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी, शून्य *buf)
{
	संरचना windows_private_data *data;
	संरचना windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	अक्षर		*cp;
	पूर्णांक		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	परत raw_read_blk(channel, data, block, मोजणी, buf);
#अन्यथा
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
	पर्यंत (मोजणी > 0) {
		/* If it's in the cache, use it! */
		जर ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			म_लिखो("Using cached block %lu\n", block);
#endif
			स_नकल(cp, cache->buf, channel->block_size);
			मोजणी--;
			block++;
			cp += channel->block_size;
			सुरूठेवा;
		}
		जर (मोजणी == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			जर ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				परत retval;
			}
			स_नकल(cp, cache->buf, channel->block_size);
			परत 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		साठी (i=1; i < मोजणी; i++)
			जर (find_cached_block(data, block+i, &reuse[i]))
				थांबा;
#ifdef DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#endif
		जर ((retval = raw_read_blk(channel, data, block, i, cp)))
			परत retval;

		/* Save the results in the cache */
		साठी (j=0; j < i; j++) {
			मोजणी--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			स_नकल(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	परत 0;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	परत windows_read_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t windows_write_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना windows_private_data *data;
	संरचना windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	स्थिरांक अक्षर	*cp;
	पूर्णांक		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	परत raw_write_blk(channel, data, block, मोजणी, buf);
#अन्यथा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	जर (मोजणी < 0 || मोजणी > WRITE_DIRECT_SIZE) {
		जर ((retval = flush_cached_blocks(channel, data, 1)))
			परत retval;
		परत raw_write_blk(channel, data, block, मोजणी, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	जर (writethrough)
		retval = raw_write_blk(channel, data, block, मोजणी, buf);

	cp = buf;
	पर्यंत (मोजणी > 0) {
		cache = find_cached_block(data, block, &reuse);
		जर (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		जर (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		मोजणी--;
		block++;
		cp += channel->block_size;
	}
	परत retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	परत EXT2_ET_OP_NOT_SUPPORTED;
}

स्थिर errcode_t windows_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	परत windows_write_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t windows_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक size, स्थिरांक शून्य *buf)
{
	परत EXT2_ET_UNIMPLEMENTED;
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

	परत retval;
}

स्थिर errcode_t windows_set_option(io_channel channel, स्थिरांक अक्षर *option,
				 स्थिरांक अक्षर *arg)
{
	संरचना windows_private_data *data;
	अचिन्हित दीर्घ दीर्घ tmp;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

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
	परत EXT2_ET_INVALID_ARGUMENT;
}

स्थिर errcode_t windows_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 अचिन्हित दीर्घ दीर्घ मोजणी)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	परत EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#जर __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ढकला
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
स्थिर errcode_t windows_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना windows_private_data *data;
	पूर्णांक		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	जर (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		जा unimplemented;
	} अन्यथा {
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
		जा unimplemented;
	}
err:
	जर (ret < 0) {
		जर (त्रुटि_सं == EOPNOTSUPP)
			जा unimplemented;
		परत त्रुटि_सं;
	}
	परत 0;
unimplemented:
	परत EXT2_ET_UNIMPLEMENTED;
}

पूर्णांक ext2fs_open_file(स्थिरांक अक्षर *pathname, पूर्णांक flags, mode_t mode)
{
	flags |= O_द्विमान;

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

#जर __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic काढा
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
