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

#सचे !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#सचे HAVE_SYS_STAT_H
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
#अञ्ञथा
#define TRACE(...) करोहि { } याव (0);
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  सचे ((संरचना)->magic != (code)) पटिनिवत्त (code)
#define EXT2_CHECK_MAGIC_RETURN(संरचना, code, ret) \
	  सचे ((संरचना)->magic != (code)) पटिनिवत्त (ret)

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
	सचे (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		पटिनिवत्त 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	सचे (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		पटिनिवत्त 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	पटिनिवत्त 0;
}

स्थिर शून्य remove_fake_dos_name(संरचना windows_private_data *data)
{
	सचे (*data->dos_device) {
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

	सचे (stats)
		*stats = &data->io_stats;

	पटिनिवत्त retval;
}

स्थिर LARGE_INTEGER make_large_integer(LONGLONG मानम्)
{
	LARGE_INTEGER	li;

	li.QuadPart = मानम्;
	पटिनिवत्त li;
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

	सचे (data->flags & IO_FLAG_FORCE_BOUNCE) {
		सचे (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		गच्छ bounce_read;
	}

	सचे (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
		retval = GetLastError();
		गच्छ error_out;
	}
	सचे ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		सचे (!ReadFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		सचे (actual != size) {
	short_read:
		सचे (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} अञ्ञथा
			retval = EXT2_ET_SHORT_READ;
		गच्छ error_out;
		}
	पटिनिवत्त 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	याव (size > 0) {
		सचे (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		सचे (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			गच्छ short_read;
		}
		actual = size;
		सचे (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	पटिनिवत्त 0;

error_out:
	सचे (actual >= 0 && actual < size)
		memset((वर्ण *) buf+actual, 0, size-actual);
	सचे (channel->read_error)
		retval = (channel->read_error)(channel, block, गणनम्, buf,
					       size, actual, retval);
	पटिनिवत्त retval;
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

	सचे (गणनम् == 1)
		size = channel->block_size;
	अञ्ञथा {
		सचे (गणनम् < 0)
			size = -गणनम्;
		अञ्ञथा
			size = गणनम् * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	सचे (data->flags & IO_FLAG_FORCE_BOUNCE) {
		सचे (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
	गच्छ bounce_write;
	}

		सचे (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
		गच्छ error_out;
	}

	SetLastError(0);

	सचे ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		सचे (!WriteFile(data->handle, buf, size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		सचे (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			गच्छ error_out;
		}
		पटिनिवत्त 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	याव (size > 0) {
		सचे (size < channel->block_size) {
			सचे (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
				retval = GetLastError();
				गच्छ error_out;
			}
			सचे (actual != channel->block_size) {
				सचे (actual < 0) {
					retval = GetLastError();
					गच्छ error_out;
				}
				memset((वर्ण *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		सचे (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		सचे (!SetFilePointerEx(data->handle, make_large_integer(location), रिक्त, FILE_BEGIN)) {
			retval = GetLastError();
			गच्छ error_out;
		}
		सचे (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, रिक्त)) {
			retval = GetLastError();
			गच्छ error_out;
		}

		सचे (actual != channel->block_size)
			गच्छ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	पटिनिवत्त 0;

error_out:
	सचे (channel->write_error)
		retval = (channel->write_error)(channel, block, गणनम्, buf,
						size, actual, retval);
	पटिनिवत्त retval;
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
	अत्थाय (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		सचे (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		सचे (retval)
			पटिनिवत्त retval;
	}
	सचे (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		सचे (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	पटिनिवत्त retval;
}

/* Free the cache buffers */
स्थिर शून्य free_cache(संरचना windows_private_data *data)
{
	संरचना windows_cache    *cache;
	पूर्णाङ्क			i;

	data->access_time = 0;
	अत्थाय (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		सचे (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	सचे (data->bounce)
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
	अत्थाय (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		सचे (!cache->in_use) {
			सचे (!unused_cache)
				unused_cache = cache;
			अनुवर्त;
		}
		सचे (cache->block == block) {
			cache->access_time = ++data->access_time;
			पटिनिवत्त cache;
		}
		सचे (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	सचे (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	पटिनिवत्त 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
स्थिर शून्य reuse_cache(io_channel channel, संरचना windows_private_data *data,
		 संरचना windows_cache *cache, unsigned long long block)
{
	सचे (cache->dirty && cache->in_use)
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
	अत्थाय (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		सचे (!cache->in_use)
			अनुवर्त;

		सचे (invalidate)
			cache->in_use = 0;

		सचे (!cache->dirty)
			अनुवर्त;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		सचे (retval)
			retval2 = retval;
		अञ्ञथा
			cache->dirty = 0;
	}
	पटिनिवत्त retval2;
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
	सचे (retval)
		गच्छ cleanup;
	memset(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	सचे (retval)
		गच्छ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#सचे defined(O_DIRECT)
	सचे (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	सचे (ext2fs_fstat(data->dev, &st) == 0) {
		सचे (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		अञ्ञथा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	सचे ((retval = alloc_cache(io, data)))
		गच्छ cleanup;

#ifdef BLKROGET
	सचे (flags & IO_FLAG_RW) {
		पूर्णाङ्क error;
		पूर्णाङ्क readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		सचे (!error && readonly) {
			retval = EPERM;
			गच्छ cleanup;
		}
	}
#endif

	*channel = io;
	पटिनिवत्त 0;

cleanup:
	सचे (data) {
		सचे (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	सचे (io) {
		सचे (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	पटिनिवत्त retval;
}

स्थिर DWORD windows_open_device(संरचना windows_private_data *data, पूर्णाङ्क open_flags)
{
	DWORD ret = 0;

	सचे (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	अञ्ञथा सचे (fake_dos_name_for_device(data))
		पटिनिवत्त -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#सचे defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#अञ्ञथा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, रिक्त, OPEN_EXISTING,
				  flags_and_attributes, रिक्त);

	सचे (data->handle == INVALID_HANDLE_मानम्) {
		ret = GetLastError();
		गच्छ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	सचे (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		गच्छ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	पटिनिवत्त 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	पटिनिवत्त ret;
}

स्थिर संरचना windows_private_data *init_private_data(ध्रुव वर्ण *name, पूर्णाङ्क flags)
{
	संरचना windows_private_data *data = रिक्त;

	सचे (ext2fs_get_mem(परिमाण(संरचना windows_private_data), &data))
		पटिनिवत्त रिक्त;

	memset(data, 0, परिमाण(संरचना windows_private_data));
	strncpy(data->name, name, परिमाण(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.अङ्कः_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_मानम्;

	पटिनिवत्त data;
}

स्थिर errcode_t windows_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	पूर्णाङ्क open_flags;
	संरचना windows_private_data *data;

	सचे (name == 0)
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	सचे (!data)
		पटिनिवत्त EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	सचे (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#सचे defined(O_DIRECT)
	सचे (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	सचे (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;
	}

	पटिनिवत्त windows_open_channel(data, flags, channel, windows_io_manager);
}

स्थिर errcode_t windows_close(io_channel channel)
{
	संरचना windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	सचे (--channel->refcount > 0)
		पटिनिवत्त 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	सचे (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	सचे (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	पटिनिवत्त retval;
}

स्थिर errcode_t windows_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	सचे (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		सचे ((retval = flush_cached_blocks(channel, data, 0)))
			पटिनिवत्त retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		सचे ((retval = alloc_cache(channel, data)))
			पटिनिवत्त retval;
	}
	पटिनिवत्त 0;
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
	पटिनिवत्त raw_read_blk(channel, data, block, गणनम्, buf);
#अञ्ञथा
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	सचे (गणनम् < 0 || गणनम् > WRITE_DIRECT_SIZE) {
		सचे ((retval = flush_cached_blocks(channel, data, 0)))
			पटिनिवत्त retval;
		पटिनिवत्त raw_read_blk(channel, data, block, गणनम्, buf);
	}

	cp = buf;
	याव (गणनम् > 0) {
		/* If it's in the cache, use it! */
		सचे ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			गणनम्--;
			block++;
			cp += channel->block_size;
			अनुवर्त;
		}
		सचे (गणनम् == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			सचे ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				पटिनिवत्त retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			पटिनिवत्त 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		अत्थाय (i=1; i < गणनम्; i++)
			सचे (find_cached_block(data, block+i, &reuse[i]))
				भङ्ग;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		सचे ((retval = raw_read_blk(channel, data, block, i, cp)))
			पटिनिवत्त retval;

		/* Save the results in the cache */
		अत्थाय (j=0; j < i; j++) {
			गणनम्--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	पटिनिवत्त 0;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	पटिनिवत्त windows_read_blk64(channel, block, गणनम्, buf);
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
	पटिनिवत्त raw_write_blk(channel, data, block, गणनम्, buf);
#अञ्ञथा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	सचे (गणनम् < 0 || गणनम् > WRITE_DIRECT_SIZE) {
		सचे ((retval = flush_cached_blocks(channel, data, 1)))
			पटिनिवत्त retval;
		पटिनिवत्त raw_write_blk(channel, data, block, गणनम्, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	सचे (writethrough)
		retval = raw_write_blk(channel, data, block, गणनम्, buf);

	cp = buf;
	याव (गणनम् > 0) {
		cache = find_cached_block(data, block, &reuse);
		सचे (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		सचे (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		गणनम्--;
		block++;
		cp += channel->block_size;
	}
	पटिनिवत्त retval;
#endif /* NO_IO_CACHE */
}

स्थिर errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long गणनम्)
{
	पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;
}

स्थिर errcode_t windows_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	पटिनिवत्त windows_write_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 पूर्णाङ्क size, ध्रुव शून्य *buf)
{
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
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

	पटिनिवत्त retval;
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

	सचे (!strcmp(option, "offset")) {
		सचे (!arg)
			पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		सचे (*end)
			पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		सचे (data->offset < 0)
			पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
		पटिनिवत्त 0;
	}
	पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
}

स्थिर errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long गणनम्)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#सचे __GNUC_PREREQ (4, 6)
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

	सचे (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		गच्छ unimplemented;
	} अञ्ञथा {
		/* Regular file, try to use truncate/punch/zero. */
		संरचना stat statbuf;

		सचे (गणनम् == 0)
			पटिनिवत्त 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		सचे (ret)
			गच्छ err;
		सचे ((unsigned long long) statbuf.st_size <
			(block + गणनम्) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + गणनम्) * channel->block_size + data->offset);
			सचे (ret)
				गच्छ err;
		}
		गच्छ unimplemented;
	}
err:
	सचे (ret < 0) {
		सचे (errno == EOPNOTSUPP)
			गच्छ unimplemented;
		पटिनिवत्त errno;
	}
	पटिनिवत्त 0;
unimplemented:
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}

पूर्णाङ्क ext2fs_open_file(ध्रुव वर्ण *pathname, पूर्णाङ्क flags, mode_t mode)
{
	flags |= O_द्विमान;

	सचे (mode)
#सचे defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		पटिनिवत्त open64(pathname, flags, mode);
	अञ्ञथा
		पटिनिवत्त open64(pathname, flags);
#अञ्ञथा
		पटिनिवत्त open(pathname, flags, mode);
	अञ्ञथा
		पटिनिवत्त open(pathname, flags);
#endif
}

पूर्णाङ्क ext2fs_stat(ध्रुव वर्ण *path, ext2fs_struct_stat *buf)
{
#सचे defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	पटिनिवत्त stat64(path, buf);
#अञ्ञथा
	पटिनिवत्त stat(path, buf);
#endif
}

पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf)
{
#सचे defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	पटिनिवत्त fstat64(fd, buf);
#अञ्ञथा
	पटिनिवत्त fstat(fd, buf);
#endif
}

#सचे __GNUC_PREREQ (4, 6)
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
