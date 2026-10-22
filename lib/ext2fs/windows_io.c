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

#અન્તર્ભાવ <windows.h>
#અન્તર્ભાવ <winioctl.h>
#અન્તર્ભાવ <io.h>

#જો !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#અન્તર્ભાવ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    અક્ષર __log[256];\
		    snprintf(__log, કદ(__log), __VA_ARGS__);\
		    __log[કદ(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#નહીંતર
#define TRACE(...) કરો { } જ્યારે (0);
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)
#define EXT2_CHECK_MAGIC_RETURN(સંરચના, code, ret) \
	  જો ((સંરચના)->magic != (code)) પાછા (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

સંરચના windows_cache {
	અક્ષર			*buf;
	unsigned long long	block;
	પૂર્ણાંક			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

સંરચના windows_private_data {
	પૂર્ણાંક	magic;
	અક્ષર	name[MAX_PATH];
	HANDLE	handle;
	અક્ષર	dos_device[MAX_PATH];
	અક્ષર	cf_device[MAX_PATH];
	પૂર્ણાંક	dev;
	પૂર્ણાંક	flags;
	પૂર્ણાંક	align;
	પૂર્ણાંક	access_time;
	ext2_loff_t offset;
	સંરચના windows_cache cache[CACHE_SIZE];
	શૂન્ય	*bounce;
	સંરચના struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

સ્થિર પૂર્ણાંક fake_dos_name_for_device(સંરચના windows_private_data *data)
{
	જો (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		પાછા 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	જો (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		પાછા 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	પાછા 0;
}

સ્થિર શૂન્ય remove_fake_dos_name(સંરચના windows_private_data *data)
{
	જો (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

સ્થિર errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	સંરચના windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	જો (stats)
		*stats = &data->io_stats;

	પાછા retval;
}

સ્થિર LARGE_INTEGER make_large_integer(LONGLONG કિંમત)
{
	LARGE_INTEGER	li;

	li.QuadPart = કિંમત;
	પાછા li;
}

/*
 * Here are the raw I/O functions
 */
સ્થિર errcode_t raw_read_blk(io_channel channel,
			      સંરચના windows_private_data *data,
			      unsigned long long block,
			      પૂર્ણાંક ગણતરી, શૂન્ય *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned અક્ષર	*buf = bufv;
	ssize_t		really_read = 0;

	size = (ગણતરી < 0) ? -ગણતરી : ગણતરી * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	જો (data->flags & IO_FLAG_FORCE_BOUNCE) {
		જો (!SetFilePointerEx(data->handle, make_large_integer(location), ખાલી, FILE_BEGIN)) {
			retval = GetLastError();
			જાઓ error_out;
		}
		જાઓ bounce_read;
	}

	જો (!SetFilePointerEx(data->handle, make_large_integer(location), ખાલી, FILE_BEGIN)) {
		retval = GetLastError();
		જાઓ error_out;
	}
	જો ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		જો (!ReadFile(data->handle, buf, size, &actual, ખાલી)) {
			retval = GetLastError();
			જાઓ error_out;
		}
		જો (actual != size) {
	short_read:
		જો (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} નહીંતર
			retval = EXT2_ET_SHORT_READ;
		જાઓ error_out;
		}
	પાછા 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	જ્યારે (size > 0) {
		જો (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ખાલી)) {
			retval = GetLastError();
			જાઓ error_out;
		}
		જો (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			જાઓ short_read;
		}
		actual = size;
		જો (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	પાછા 0;

error_out:
	જો (actual >= 0 && actual < size)
		memset((અક્ષર *) buf+actual, 0, size-actual);
	જો (channel->read_error)
		retval = (channel->read_error)(channel, block, ગણતરી, buf,
					       size, actual, retval);
	પાછા retval;
}

સ્થિર errcode_t raw_write_blk(io_channel channel,
			       સંરચના windows_private_data *data,
			       unsigned long long block,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	અચલ unsigned અક્ષર *buf = bufv;

	જો (ગણતરી == 1)
		size = channel->block_size;
	નહીંતર {
		જો (ગણતરી < 0)
			size = -ગણતરી;
		નહીંતર
			size = ગણતરી * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	જો (data->flags & IO_FLAG_FORCE_BOUNCE) {
		જો (!SetFilePointerEx(data->handle, make_large_integer(location), ખાલી, FILE_BEGIN)) {
			retval = GetLastError();
			જાઓ error_out;
		}
	જાઓ bounce_write;
	}

		જો (!SetFilePointerEx(data->handle, make_large_integer(location), ખાલી, FILE_BEGIN)) {
			retval = GetLastError();
		જાઓ error_out;
	}

	SetLastError(0);

	જો ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		જો (!WriteFile(data->handle, buf, size, &actual, ખાલી)) {
			retval = GetLastError();
			જાઓ error_out;
		}

		જો (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			જાઓ error_out;
		}
		પાછા 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	જ્યારે (size > 0) {
		જો (size < channel->block_size) {
			જો (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ખાલી)) {
				retval = GetLastError();
				જાઓ error_out;
			}
			જો (actual != channel->block_size) {
				જો (actual < 0) {
					retval = GetLastError();
					જાઓ error_out;
				}
				memset((અક્ષર *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		જો (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		જો (!SetFilePointerEx(data->handle, make_large_integer(location), ખાલી, FILE_BEGIN)) {
			retval = GetLastError();
			જાઓ error_out;
		}
		જો (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, ખાલી)) {
			retval = GetLastError();
			જાઓ error_out;
		}

		જો (actual != channel->block_size)
			જાઓ short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	પાછા 0;

error_out:
	જો (channel->write_error)
		retval = (channel->write_error)(channel, block, ગણતરી, buf,
						size, actual, retval);
	પાછા retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
સ્થિર errcode_t alloc_cache(io_channel channel,
			     સંરચના windows_private_data *data)
{
	errcode_t		retval;
	સંરચના windows_cache	*cache;
	પૂર્ણાંક			i;

	data->access_time = 0;
	માટે (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		જો (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		જો (retval)
			પાછા retval;
	}
	જો (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		જો (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	પાછા retval;
}

/* Free the cache buffers */
સ્થિર શૂન્ય free_cache(સંરચના windows_private_data *data)
{
	સંરચના windows_cache    *cache;
	પૂર્ણાંક			i;

	data->access_time = 0;
	માટે (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		જો (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	જો (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
સ્થિર સંરચના windows_cache *find_cached_block(સંરચના windows_private_data *data,
					    unsigned long long block,
					    સંરચના windows_cache **eldest)
{
	સંરચના windows_cache	*cache, *unused_cache, *oldest_cache;
	પૂર્ણાંક			i;

	unused_cache = oldest_cache = 0;
	માટે (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		જો (!cache->in_use) {
			જો (!unused_cache)
				unused_cache = cache;
			ચાલુ;
		}
		જો (cache->block == block) {
			cache->access_time = ++data->access_time;
			પાછા cache;
		}
		જો (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	જો (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	પાછા 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
સ્થિર શૂન્ય reuse_cache(io_channel channel, સંરચના windows_private_data *data,
		 સંરચના windows_cache *cache, unsigned long long block)
{
	જો (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
સ્થિર errcode_t flush_cached_blocks(io_channel channel,
				     સંરચના windows_private_data *data,
				     પૂર્ણાંક invalidate)
{
	સંરચના windows_cache	*cache;
	errcode_t	retval, retval2;
	પૂર્ણાંક			i;

	retval2 = 0;
	માટે (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		જો (!cache->in_use)
			ચાલુ;

		જો (invalidate)
			cache->in_use = 0;

		જો (!cache->dirty)
			ચાલુ;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		જો (retval)
			retval2 = retval;
		નહીંતર
			cache->dirty = 0;
	}
	પાછા retval2;
}
#endif /* NO_IO_CACHE */

સ્થિર errcode_t windows_open_channel(સંરચના windows_private_data *data,
				   પૂર્ણાંક flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ખાલી;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(કદ(સંરચના struct_io_channel), &io);
	જો (retval)
		જાઓ cleanup;
	memset(io, 0, કદ(સંરચના struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	જો (retval)
		જાઓ cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#જો defined(O_DIRECT)
	જો (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	જો (ext2fs_fstat(data->dev, &st) == 0) {
		જો (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		નહીંતર
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	જો ((retval = alloc_cache(io, data)))
		જાઓ cleanup;

#ifdef BLKROGET
	જો (flags & IO_FLAG_RW) {
		પૂર્ણાંક error;
		પૂર્ણાંક readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		જો (!error && readonly) {
			retval = EPERM;
			જાઓ cleanup;
		}
	}
#endif

	*channel = io;
	પાછા 0;

cleanup:
	જો (data) {
		જો (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	જો (io) {
		જો (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	પાછા retval;
}

સ્થિર DWORD windows_open_device(સંરચના windows_private_data *data, પૂર્ણાંક open_flags)
{
	DWORD ret = 0;

	જો (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	નહીંતર જો (fake_dos_name_for_device(data))
		પાછા -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#જો defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#નહીંતર
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, ખાલી, OPEN_EXISTING,
				  flags_and_attributes, ખાલી);

	જો (data->handle == INVALID_HANDLE_કિંમત) {
		ret = GetLastError();
		જાઓ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	જો (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		જાઓ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	પાછા 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	પાછા ret;
}

સ્થિર સંરચના windows_private_data *init_private_data(અચલ અક્ષર *name, પૂર્ણાંક flags)
{
	સંરચના windows_private_data *data = ખાલી;

	જો (ext2fs_get_mem(કદ(સંરચના windows_private_data), &data))
		પાછા ખાલી;

	memset(data, 0, કદ(સંરચના windows_private_data));
	strncpy(data->name, name, કદ(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.અંક_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_કિંમત;

	પાછા data;
}

સ્થિર errcode_t windows_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	પૂર્ણાંક open_flags;
	સંરચના windows_private_data *data;

	જો (name == 0)
		પાછા EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	જો (!data)
		પાછા EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	જો (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#જો defined(O_DIRECT)
	જો (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	જો (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		પાછા EXT2_ET_BAD_DEVICE_NAME;
	}

	પાછા windows_open_channel(data, flags, channel, windows_io_manager);
}

સ્થિર errcode_t windows_close(io_channel channel)
{
	સંરચના windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	જો (--channel->refcount > 0)
		પાછા 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	જો (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	જો (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	પાછા retval;
}

સ્થિર errcode_t windows_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	સંરચના windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	જો (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		જો ((retval = flush_cached_blocks(channel, data, 0)))
			પાછા retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		જો ((retval = alloc_cache(channel, data)))
			પાછા retval;
	}
	પાછા 0;
}

સ્થિર errcode_t windows_read_blk64(io_channel channel, unsigned long long block, પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	સંરચના windows_private_data *data;
	સંરચના windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	અક્ષર		*cp;
	પૂર્ણાંક		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	પાછા raw_read_blk(channel, data, block, ગણતરી, buf);
#નહીંતર
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	જો (ગણતરી < 0 || ગણતરી > WRITE_DIRECT_SIZE) {
		જો ((retval = flush_cached_blocks(channel, data, 0)))
			પાછા retval;
		પાછા raw_read_blk(channel, data, block, ગણતરી, buf);
	}

	cp = buf;
	જ્યારે (ગણતરી > 0) {
		/* If it's in the cache, use it! */
		જો ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			ગણતરી--;
			block++;
			cp += channel->block_size;
			ચાલુ;
		}
		જો (ગણતરી == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			જો ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				પાછા retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			પાછા 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		માટે (i=1; i < ગણતરી; i++)
			જો (find_cached_block(data, block+i, &reuse[i]))
				તોડો;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		જો ((retval = raw_read_blk(channel, data, block, i, cp)))
			પાછા retval;

		/* Save the results in the cache */
		માટે (j=0; j < i; j++) {
			ગણતરી--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	પાછા 0;
#endif /* NO_IO_CACHE */
}

સ્થિર errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પાછા windows_read_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના windows_private_data *data;
	સંરચના windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	અચલ અક્ષર	*cp;
	પૂર્ણાંક		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	પાછા raw_write_blk(channel, data, block, ગણતરી, buf);
#નહીંતર
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	જો (ગણતરી < 0 || ગણતરી > WRITE_DIRECT_SIZE) {
		જો ((retval = flush_cached_blocks(channel, data, 1)))
			પાછા retval;
		પાછા raw_write_blk(channel, data, block, ગણતરી, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	જો (writethrough)
		retval = raw_write_blk(channel, data, block, ગણતરી, buf);

	cp = buf;
	જ્યારે (ગણતરી > 0) {
		cache = find_cached_block(data, block, &reuse);
		જો (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		જો (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		ગણતરી--;
		block++;
		cp += channel->block_size;
	}
	પાછા retval;
#endif /* NO_IO_CACHE */
}

સ્થિર errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ગણતરી)
{
	પાછા EXT2_ET_OP_NOT_SUPPORTED;
}

સ્થિર errcode_t windows_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પાછા windows_write_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 પૂર્ણાંક size, અચલ શૂન્ય *buf)
{
	પાછા EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
સ્થિર errcode_t windows_flush(io_channel channel)
{
	સંરચના windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	પાછા retval;
}

સ્થિર errcode_t windows_set_option(io_channel channel, અચલ અક્ષર *option,
				 અચલ અક્ષર *arg)
{
	સંરચના windows_private_data *data;
	unsigned long long tmp;
	અક્ષર *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	જો (!strcmp(option, "offset")) {
		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		જો (*end)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		જો (data->offset < 0)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		પાછા 0;
	}
	પાછા EXT2_ET_INVALID_ARGUMENT;
}

સ્થિર errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long ગણતરી)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	પાછા EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ધકેલો
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
સ્થિર errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના windows_private_data *data;
	પૂર્ણાંક		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	જો (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		જાઓ unimplemented;
	} નહીંતર {
		/* Regular file, try to use truncate/punch/zero. */
		સંરચના stat statbuf;

		જો (ગણતરી == 0)
			પાછા 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		જો (ret)
			જાઓ err;
		જો ((unsigned long long) statbuf.st_size <
			(block + ગણતરી) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + ગણતરી) * channel->block_size + data->offset);
			જો (ret)
				જાઓ err;
		}
		જાઓ unimplemented;
	}
err:
	જો (ret < 0) {
		જો (errno == EOPNOTSUPP)
			જાઓ unimplemented;
		પાછા errno;
	}
	પાછા 0;
unimplemented:
	પાછા EXT2_ET_UNIMPLEMENTED;
}

પૂર્ણાંક ext2fs_open_file(અચલ અક્ષર *pathname, પૂર્ણાંક flags, mode_t mode)
{
	flags |= O_દ્વિઅંકી;

	જો (mode)
#જો defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		પાછા open64(pathname, flags, mode);
	નહીંતર
		પાછા open64(pathname, flags);
#નહીંતર
		પાછા open(pathname, flags, mode);
	નહીંતર
		પાછા open(pathname, flags);
#endif
}

પૂર્ણાંક ext2fs_stat(અચલ અક્ષર *path, ext2fs_struct_stat *buf)
{
#જો defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	પાછા stat64(path, buf);
#નહીંતર
	પાછા stat(path, buf);
#endif
}

પૂર્ણાંક ext2fs_fstat(પૂર્ણાંક fd, ext2fs_struct_stat *buf)
{
#જો defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	પાછા fstat64(fd, buf);
#નહીંતર
	પાછા fstat(fd, buf);
#endif
}

#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic કાઢો
#endif

સ્થિર સંરચના struct_io_manager struct_windows_manager = {
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
