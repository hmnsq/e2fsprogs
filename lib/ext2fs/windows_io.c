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

#അന്തര്ഭാവ <windows.h>
#അന്തര്ഭാവ <winioctl.h>
#അന്തര്ഭാവ <io.h>

#എങ്കിൽ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#define PR_GET_DUMPABLE 3
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#അന്തര്ഭാവ <fcntl.h>

#undef ALIGN_DEBUG

//#define DEBUG
#ifdef DEBUG
#define TRACE(...) {\
		    അക്ഷരം __log[256];\
		    snprintf(__log, വലുപ്പം(__log), __VA_ARGS__);\
		    __log[വലുപ്പം(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#അല്ലെങ്കിൽ
#define TRACE(...) ചെയ്യുക { } ആകുമ്പോൾ (0);
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)
#define EXT2_CHECK_MAGIC_RETURN(ഘടന, code, ret) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (ret)

#define EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

ഘടന windows_cache {
	അക്ഷരം			*buf;
	unsigned long long	block;
	പൂർണ്ണസംഖ്യ			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

ഘടന windows_private_data {
	പൂർണ്ണസംഖ്യ	magic;
	അക്ഷരം	name[MAX_PATH];
	HANDLE	handle;
	അക്ഷരം	dos_device[MAX_PATH];
	അക്ഷരം	cf_device[MAX_PATH];
	പൂർണ്ണസംഖ്യ	dev;
	പൂർണ്ണസംഖ്യ	flags;
	പൂർണ്ണസംഖ്യ	align;
	പൂർണ്ണസംഖ്യ	access_time;
	ext2_loff_t offset;
	ഘടന windows_cache cache[CACHE_SIZE];
	ശൂന്യം	*bounce;
	ഘടന struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

സ്ഥിതം പൂർണ്ണസംഖ്യ fake_dos_name_for_device(ഘടന windows_private_data *data)
{
	എങ്കിൽ (strncmp(data->name, "\\\\", 2) == 0) {
		data->dos_device[0] = 0;
		strcpy(data->cf_device, data->name);
		തിരികെ 0;
	}

	_snprintf(data->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	എങ്കിൽ (!DefineDosDevice(DDD_RAW_TARGET_PATH, data->dos_device, data->name))
		തിരികെ 1;

	_snprintf(data->cf_device, MAX_PATH, "\\\\.\\%s", data->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", data->dos_device);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം remove_fake_dos_name(ഘടന windows_private_data *data)
{
	എങ്കിൽ (*data->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", data->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, data->dos_device, data->name);
	}
}

സ്ഥിതം errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	ഘടന windows_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	എങ്കിൽ (stats)
		*stats = &data->io_stats;

	തിരികെ retval;
}

സ്ഥിതം LARGE_INTEGER make_large_integer(LONGLONG മൂല്യം)
{
	LARGE_INTEGER	li;

	li.QuadPart = മൂല്യം;
	തിരികെ li;
}

/*
 * Here are the raw I/O functions
 */
സ്ഥിതം errcode_t raw_read_blk(io_channel channel,
			      ഘടന windows_private_data *data,
			      unsigned long long block,
			      പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *bufv)
{
	errcode_t	retval;
	ssize_t	size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	unsigned അക്ഷരം	*buf = bufv;
	ssize_t		really_read = 0;

	size = (എണ്ണം < 0) ? -എണ്ണം : എണ്ണം * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	എങ്കിൽ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		എങ്കിൽ (!SetFilePointerEx(data->handle, make_large_integer(location), ശൂന്യ, FILE_BEGIN)) {
			retval = GetLastError();
			പോകുക error_out;
		}
		പോകുക bounce_read;
	}

	എങ്കിൽ (!SetFilePointerEx(data->handle, make_large_integer(location), ശൂന്യ, FILE_BEGIN)) {
		retval = GetLastError();
		പോകുക error_out;
	}
	എങ്കിൽ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		എങ്കിൽ (!ReadFile(data->handle, buf, size, &actual, ശൂന്യ)) {
			retval = GetLastError();
			പോകുക error_out;
		}
		എങ്കിൽ (actual != size) {
	short_read:
		എങ്കിൽ (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} അല്ലെങ്കിൽ
			retval = EXT2_ET_SHORT_READ;
		പോകുക error_out;
		}
	തിരികെ 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	ആകുമ്പോൾ (size > 0) {
		എങ്കിൽ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ശൂന്യ)) {
			retval = GetLastError();
			പോകുക error_out;
		}
		എങ്കിൽ (actual != channel->block_size) {
			actual = really_read;
			buf -= really_read;
			size += really_read;
			പോകുക short_read;
		}
		actual = size;
		എങ്കിൽ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		really_read += actual;
		size -= actual;
		buf += actual;
	}
	തിരികെ 0;

error_out:
	എങ്കിൽ (actual >= 0 && actual < size)
		memset((അക്ഷരം *) buf+actual, 0, size-actual);
	എങ്കിൽ (channel->read_error)
		retval = (channel->read_error)(channel, block, എണ്ണം, buf,
					       size, actual, retval);
	തിരികെ retval;
}

സ്ഥിതം errcode_t raw_write_blk(io_channel channel,
			       ഘടന windows_private_data *data,
			       unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	സ്ഥിരം unsigned അക്ഷരം *buf = bufv;

	എങ്കിൽ (എണ്ണം == 1)
		size = channel->block_size;
	അല്ലെങ്കിൽ {
		എങ്കിൽ (എണ്ണം < 0)
			size = -എണ്ണം;
		അല്ലെങ്കിൽ
			size = എണ്ണം * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

	എങ്കിൽ (data->flags & IO_FLAG_FORCE_BOUNCE) {
		എങ്കിൽ (!SetFilePointerEx(data->handle, make_large_integer(location), ശൂന്യ, FILE_BEGIN)) {
			retval = GetLastError();
			പോകുക error_out;
		}
	പോകുക bounce_write;
	}

		എങ്കിൽ (!SetFilePointerEx(data->handle, make_large_integer(location), ശൂന്യ, FILE_BEGIN)) {
			retval = GetLastError();
		പോകുക error_out;
	}

	SetLastError(0);

	എങ്കിൽ ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(size, channel->align))) {
		എങ്കിൽ (!WriteFile(data->handle, buf, size, &actual, ശൂന്യ)) {
			retval = GetLastError();
			പോകുക error_out;
		}

		എങ്കിൽ (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			പോകുക error_out;
		}
		തിരികെ 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	ആകുമ്പോൾ (size > 0) {
		എങ്കിൽ (size < channel->block_size) {
			എങ്കിൽ (!ReadFile(data->handle, data->bounce, channel->block_size, &actual, ശൂന്യ)) {
				retval = GetLastError();
				പോകുക error_out;
			}
			എങ്കിൽ (actual != channel->block_size) {
				എങ്കിൽ (actual < 0) {
					retval = GetLastError();
					പോകുക error_out;
				}
				memset((അക്ഷരം *) data->bounce + actual, 0,
				       channel->block_size - actual);
			}
		}
		actual = size;
		എങ്കിൽ (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		എങ്കിൽ (!SetFilePointerEx(data->handle, make_large_integer(location), ശൂന്യ, FILE_BEGIN)) {
			retval = GetLastError();
			പോകുക error_out;
		}
		എങ്കിൽ (!WriteFile(data->handle, data->bounce, channel->block_size, &actual, ശൂന്യ)) {
			retval = GetLastError();
			പോകുക error_out;
		}

		എങ്കിൽ (actual != channel->block_size)
			പോകുക short_write;
		size -= actual;
		buf += actual;
		location += actual;
	}
	തിരികെ 0;

error_out:
	എങ്കിൽ (channel->write_error)
		retval = (channel->write_error)(channel, block, എണ്ണം, buf,
						size, actual, retval);
	തിരികെ retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
സ്ഥിതം errcode_t alloc_cache(io_channel channel,
			     ഘടന windows_private_data *data)
{
	errcode_t		retval;
	ഘടന windows_cache	*cache;
	പൂർണ്ണസംഖ്യ			i;

	data->access_time = 0;
	വേണ്ടി (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		എങ്കിൽ (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	എങ്കിൽ (channel->align || data->flags & IO_FLAG_FORCE_BOUNCE) {
		എങ്കിൽ (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	തിരികെ retval;
}

/* Free the cache buffers */
സ്ഥിതം ശൂന്യം free_cache(ഘടന windows_private_data *data)
{
	ഘടന windows_cache    *cache;
	പൂർണ്ണസംഖ്യ			i;

	data->access_time = 0;
	വേണ്ടി (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		എങ്കിൽ (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	എങ്കിൽ (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
സ്ഥിതം ഘടന windows_cache *find_cached_block(ഘടന windows_private_data *data,
					    unsigned long long block,
					    ഘടന windows_cache **eldest)
{
	ഘടന windows_cache	*cache, *unused_cache, *oldest_cache;
	പൂർണ്ണസംഖ്യ			i;

	unused_cache = oldest_cache = 0;
	വേണ്ടി (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		എങ്കിൽ (!cache->in_use) {
			എങ്കിൽ (!unused_cache)
				unused_cache = cache;
			തുടരുക;
		}
		എങ്കിൽ (cache->block == block) {
			cache->access_time = ++data->access_time;
			തിരികെ cache;
		}
		എങ്കിൽ (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	എങ്കിൽ (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	തിരികെ 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
സ്ഥിതം ശൂന്യം reuse_cache(io_channel channel, ഘടന windows_private_data *data,
		 ഘടന windows_cache *cache, unsigned long long block)
{
	എങ്കിൽ (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
സ്ഥിതം errcode_t flush_cached_blocks(io_channel channel,
				     ഘടന windows_private_data *data,
				     പൂർണ്ണസംഖ്യ invalidate)
{
	ഘടന windows_cache	*cache;
	errcode_t	retval, retval2;
	പൂർണ്ണസംഖ്യ			i;

	retval2 = 0;
	വേണ്ടി (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		എങ്കിൽ (!cache->in_use)
			തുടരുക;

		എങ്കിൽ (invalidate)
			cache->in_use = 0;

		എങ്കിൽ (!cache->dirty)
			തുടരുക;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		എങ്കിൽ (retval)
			retval2 = retval;
		അല്ലെങ്കിൽ
			cache->dirty = 0;
	}
	തിരികെ retval2;
}
#endif /* NO_IO_CACHE */

സ്ഥിതം errcode_t windows_open_channel(ഘടന windows_private_data *data,
				   പൂർണ്ണസംഖ്യ flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = ശൂന്യ;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_io_channel), &io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(io, 0, വലുപ്പം(ഘടന struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(strlen(data->name)+1, &io->name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(io->name, data->name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#എങ്കിൽ defined(O_DIRECT)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(data->dev);
#endif

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	എങ്കിൽ (ext2fs_fstat(data->dev, &st) == 0) {
		എങ്കിൽ (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		അല്ലെങ്കിൽ
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

	എങ്കിൽ ((retval = alloc_cache(io, data)))
		പോകുക cleanup;

#ifdef BLKROGET
	എങ്കിൽ (flags & IO_FLAG_RW) {
		പൂർണ്ണസംഖ്യ error;
		പൂർണ്ണസംഖ്യ readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		എങ്കിൽ (!error && readonly) {
			retval = EPERM;
			പോകുക cleanup;
		}
	}
#endif

	*channel = io;
	തിരികെ 0;

cleanup:
	എങ്കിൽ (data) {
		എങ്കിൽ (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	എങ്കിൽ (io) {
		എങ്കിൽ (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	തിരികെ retval;
}

സ്ഥിതം DWORD windows_open_device(ഘടന windows_private_data *data, പൂർണ്ണസംഖ്യ open_flags)
{
	DWORD ret = 0;

	എങ്കിൽ (*data->name != '\\')
		strcpy(data->cf_device, data->name);
	അല്ലെങ്കിൽ എങ്കിൽ (fake_dos_name_for_device(data))
		തിരികെ -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#എങ്കിൽ defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#അല്ലെങ്കിൽ
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#endif

	data->handle = CreateFile(data->cf_device, desired_access, share_mode, ശൂന്യ, OPEN_EXISTING,
				  flags_and_attributes, ശൂന്യ);

	എങ്കിൽ (data->handle == INVALID_HANDLE_മൂല്യം) {
		ret = GetLastError();
		പോകുക invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", data->cf_device, data->handle);

	data->dev = _open_osfhandle((intptr_t)data->handle, 0);
	എങ്കിൽ (data->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		പോകുക osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", data->handle, data->dev);

	തിരികെ 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", data->handle);
	CloseHandle(data->handle);
invalid_handle:
	remove_fake_dos_name(data);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, errno);
	തിരികെ ret;
}

സ്ഥിതം ഘടന windows_private_data *init_private_data(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന windows_private_data *data = ശൂന്യ;

	എങ്കിൽ (ext2fs_get_mem(വലുപ്പം(ഘടന windows_private_data), &data))
		തിരികെ ശൂന്യ;

	memset(data, 0, വലുപ്പം(ഘടന windows_private_data));
	strncpy(data->name, name, വലുപ്പം(data->name) - 1);
	data->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	data->io_stats.അക്കം_fields = 2;
	data->flags = flags;
	data->handle = INVALID_HANDLE_മൂല്യം;

	തിരികെ data;
}

സ്ഥിതം errcode_t windows_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	പൂർണ്ണസംഖ്യ open_flags;
	ഘടന windows_private_data *data;

	എങ്കിൽ (name == 0)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;

	data = init_private_data(name, flags);
	എങ്കിൽ (!data)
		തിരികെ EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	എങ്കിൽ (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#എങ്കിൽ defined(O_DIRECT)
	എങ്കിൽ (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#endif

	എങ്കിൽ (windows_open_device(data, open_flags)) {
		ext2fs_free_mem(&data);
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;
	}

	തിരികെ windows_open_channel(data, flags, channel, windows_io_manager);
}

സ്ഥിതം errcode_t windows_close(io_channel channel)
{
	ഘടന windows_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
		തിരികെ 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	remove_fake_dos_name(data);

	എങ്കിൽ (_close(data->dev) != 0)
		retval = errno;

	TRACE("e2fsprogs::windows_close::_close(%d)", data->dev);

	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	എങ്കിൽ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t windows_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	ഘടന windows_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	എങ്കിൽ (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data, 0)))
			തിരികെ retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		എങ്കിൽ ((retval = alloc_cache(channel, data)))
			തിരികെ retval;
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t windows_read_blk64(io_channel channel, unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	ഘടന windows_private_data *data;
	ഘടന windows_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	അക്ഷരം		*cp;
	പൂർണ്ണസംഖ്യ		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	തിരികെ raw_read_blk(channel, data, block, എണ്ണം, buf);
#അല്ലെങ്കിൽ
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	എങ്കിൽ (എണ്ണം < 0 || എണ്ണം > WRITE_DIRECT_SIZE) {
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data, 0)))
			തിരികെ retval;
		തിരികെ raw_read_blk(channel, data, block, എണ്ണം, buf);
	}

	cp = buf;
	ആകുമ്പോൾ (എണ്ണം > 0) {
		/* If it's in the cache, use it! */
		എങ്കിൽ ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			എണ്ണം--;
			block++;
			cp += channel->block_size;
			തുടരുക;
		}
		എങ്കിൽ (എണ്ണം == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			എങ്കിൽ ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				തിരികെ retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			തിരികെ 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		വേണ്ടി (i=1; i < എണ്ണം; i++)
			എങ്കിൽ (find_cached_block(data, block+i, &reuse[i]))
				നിർത്തുക;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		എങ്കിൽ ((retval = raw_read_blk(channel, data, block, i, cp)))
			തിരികെ retval;

		/* Save the results in the cache */
		വേണ്ടി (j=0; j < i; j++) {
			എണ്ണം--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	തിരികെ 0;
#endif /* NO_IO_CACHE */
}

സ്ഥിതം errcode_t windows_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	തിരികെ windows_read_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t windows_write_blk64(io_channel channel,
				unsigned long long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന windows_private_data *data;
	ഘടന windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	സ്ഥിരം അക്ഷരം	*cp;
	പൂർണ്ണസംഖ്യ		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifdef NO_IO_CACHE
	തിരികെ raw_write_blk(channel, data, block, എണ്ണം, buf);
#അല്ലെങ്കിൽ
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	എങ്കിൽ (എണ്ണം < 0 || എണ്ണം > WRITE_DIRECT_SIZE) {
		എങ്കിൽ ((retval = flush_cached_blocks(channel, data, 1)))
			തിരികെ retval;
		തിരികെ raw_write_blk(channel, data, block, എണ്ണം, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	എങ്കിൽ (writethrough)
		retval = raw_write_blk(channel, data, block, എണ്ണം, buf);

	cp = buf;
	ആകുമ്പോൾ (എണ്ണം > 0) {
		cache = find_cached_block(data, block, &reuse);
		എങ്കിൽ (!cache) {
			cache = reuse;
			reuse_cache(channel, data, cache, block);
		}
		എങ്കിൽ (cache->buf != cp)
			memcpy(cache->buf, cp, channel->block_size);
		cache->dirty = !writethrough;
		എണ്ണം--;
		block++;
		cp += channel->block_size;
	}
	തിരികെ retval;
#endif /* NO_IO_CACHE */
}

സ്ഥിതം errcode_t windows_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long എണ്ണം)
{
	തിരികെ EXT2_ET_OP_NOT_SUPPORTED;
}

സ്ഥിതം errcode_t windows_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ windows_write_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t windows_write_byte(io_channel channel, unsigned long offset,
				 പൂർണ്ണസംഖ്യ size, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
സ്ഥിതം errcode_t windows_flush(io_channel channel)
{
	ഘടന windows_private_data *data;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	തിരികെ retval;
}

സ്ഥിതം errcode_t windows_set_option(io_channel channel, സ്ഥിരം അക്ഷരം *option,
				 സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന windows_private_data *data;
	unsigned long long tmp;
	അക്ഷരം *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	എങ്കിൽ (!strcmp(option, "offset")) {
		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoull(arg, &end, 0);
		എങ്കിൽ (*end)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
		എങ്കിൽ (data->offset < 0)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		തിരികെ 0;
	}
	തിരികെ EXT2_ET_INVALID_ARGUMENT;
}

സ്ഥിതം errcode_t windows_discard(io_channel channel, unsigned long long block,
				 unsigned long long എണ്ണം)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic തള്ളുക
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
സ്ഥിതം errcode_t windows_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന windows_private_data *data;
	പൂർണ്ണസംഖ്യ		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന windows_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	എങ്കിൽ (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		പോകുക unimplemented;
	} അല്ലെങ്കിൽ {
		/* Regular file, try to use truncate/punch/zero. */
		ഘടന stat statbuf;

		എങ്കിൽ (എണ്ണം == 0)
			തിരികെ 0;
		/*
		 * If we're trying to zero a range past the end of the file,
		 * extend the file size, then truncate everything.
		 */
		ret = fstat(data->dev, &statbuf);
		എങ്കിൽ (ret)
			പോകുക err;
		എങ്കിൽ ((unsigned long long) statbuf.st_size <
			(block + എണ്ണം) * channel->block_size + data->offset) {
			ret = ftruncate(data->dev,
					(block + എണ്ണം) * channel->block_size + data->offset);
			എങ്കിൽ (ret)
				പോകുക err;
		}
		പോകുക unimplemented;
	}
err:
	എങ്കിൽ (ret < 0) {
		എങ്കിൽ (errno == EOPNOTSUPP)
			പോകുക unimplemented;
		തിരികെ errno;
	}
	തിരികെ 0;
unimplemented:
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

പൂർണ്ണസംഖ്യ ext2fs_open_file(സ്ഥിരം അക്ഷരം *pathname, പൂർണ്ണസംഖ്യ flags, mode_t mode)
{
	flags |= O_ദ്വിമാന;

	എങ്കിൽ (mode)
#എങ്കിൽ defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		തിരികെ open64(pathname, flags, mode);
	അല്ലെങ്കിൽ
		തിരികെ open64(pathname, flags);
#അല്ലെങ്കിൽ
		തിരികെ open(pathname, flags, mode);
	അല്ലെങ്കിൽ
		തിരികെ open(pathname, flags);
#endif
}

പൂർണ്ണസംഖ്യ ext2fs_stat(സ്ഥിരം അക്ഷരം *path, ext2fs_struct_stat *buf)
{
#എങ്കിൽ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	തിരികെ stat64(path, buf);
#അല്ലെങ്കിൽ
	തിരികെ stat(path, buf);
#endif
}

പൂർണ്ണസംഖ്യ ext2fs_fstat(പൂർണ്ണസംഖ്യ fd, ext2fs_struct_stat *buf)
{
#എങ്കിൽ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	തിരികെ fstat64(fd, buf);
#അല്ലെങ്കിൽ
	തിരികെ fstat(fd, buf);
#endif
}

#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic എടുക്കുക
#endif

സ്ഥിതം ഘടന struct_io_manager struct_windows_manager = {
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
