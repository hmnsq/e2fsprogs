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

#समावेश <windows.h>
#समावेश <winioctl.h>
#समावेश <io.h>

#अगर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#घोषणा _XOPEN_SOURCE 600
#घोषणा _DARWIN_C_SOURCE
#घोषणा _FILE_OFFSET_BITS 64
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
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#घोषणा PR_GET_DUMPABLE 3
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#समावेश <fcntl.h>

#अघोषित ALIGN_DEBUG

//#define DEBUG
#अगर_घोषित DEBUG
#घोषणा TRACE(...) {\
		    अक्षर __log[256];\
		    snprintf(__log, माप(__log), __VA_ARGS__);\
		    __log[माप(__log)-1] = 0;\
		    OutputDebugString(__log);\
		}
#अन्यथा
#घोषणा TRACE(...) करो { } जबतक (0);
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)
#घोषणा EXT2_CHECK_MAGIC_RETURN(काष्ठा, code, ret) \
	  अगर ((काष्ठा)->magic != (code)) वापस (ret)

#घोषणा EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL  0x10ed

काष्ठा windows_cache {
	अक्षर			*buf;
	अचिन्हित दीर्घ दीर्घ	block;
	पूर्णांक			access_time;
	अचिन्हित		dirty:1;
	अचिन्हित		in_use:1;
};

#घोषणा CACHE_आकार 8
#घोषणा WRITE_DIRECT_आकार 4	/* Must be smaller than CACHE_SIZE */
#घोषणा READ_DIRECT_आकार 4	/* Should be smaller than CACHE_SIZE */

काष्ठा windows_private_तथ्य {
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
	काष्ठा windows_cache cache[CACHE_आकार];
	व्योम	*bounce;
	काष्ठा struct_io_stats io_stats;
};

#घोषणा IS_ALIGNED(n, align) ((((uintptr_t) n) & \
			       ((uintptr_t) ((align)-1))) == 0)

अटल पूर्णांक fake_dos_name_for_device(काष्ठा windows_private_तथ्य *तथ्य)
{
	अगर (म_भेदन(तथ्य->name, "\\\\", 2) == 0) {
		तथ्य->dos_device[0] = 0;
		म_नकल(तथ्य->cf_device, तथ्य->name);
		वापस 0;
	}

	_snprintf(तथ्य->dos_device, MAX_PATH, "fakedevice%lu", GetCurrentProcessId());

	अगर (!DefineDosDevice(DDD_RAW_TARGET_PATH, तथ्य->dos_device, तथ्य->name))
		वापस 1;

	_snprintf(तथ्य->cf_device, MAX_PATH, "\\\\.\\%s", तथ्य->dos_device);
	TRACE("e2fsprogs::fake_dos_name_for_device::DefineDosDevice(\"%s\")", तथ्य->dos_device);

	वापस 0;
}

अटल व्योम remove_fake_dos_name(काष्ठा windows_private_तथ्य *तथ्य)
{
	अगर (*तथ्य->dos_device) {
		TRACE("e2fsprogs::remove_fake_dos_name::DefineDosDevice(\"%s\")", तथ्य->dos_device);
		DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_EXACT_MATCH_ON_REMOVE | DDD_REMOVE_DEFINITION, तथ्य->dos_device, तथ्य->name);
	}
}

अटल errcode_t windows_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	काष्ठा windows_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	अगर (stats)
		*stats = &तथ्य->io_stats;

	वापस retval;
}

अटल LARGE_INTEGER make_large_integer(LONGLONG मूल्य)
{
	LARGE_INTEGER	li;

	li.QuadPart = मूल्य;
	वापस li;
}

/*
 * Here are the raw I/O functions
 */
अटल errcode_t raw_read_blk(io_channel channel,
			      काष्ठा windows_private_तथ्य *तथ्य,
			      अचिन्हित दीर्घ दीर्घ block,
			      पूर्णांक गिनती, व्योम *bufv)
{
	errcode_t	retval;
	ssize_t	आकार;
	ext2_loff_t	location;
	DWORD		actual = 0;
	अचिन्हित अक्षर	*buf = bufv;
	ssize_t		really_read = 0;

	आकार = (गिनती < 0) ? -गिनती : गिनती * channel->block_आकार;
	तथ्य->io_stats.bytes_read += आकार;
	location = ((ext2_loff_t) block * channel->block_आकार) + तथ्य->offset;

	अगर (तथ्य->flags & IO_FLAG_FORCE_BOUNCE) {
		अगर (!SetFilePointerEx(तथ्य->handle, make_large_integer(location), शून्य, FILE_BEGIN)) {
			retval = GetLastError();
			जाओ error_out;
		}
		जाओ bounce_read;
	}

	अगर (!SetFilePointerEx(तथ्य->handle, make_large_integer(location), शून्य, FILE_BEGIN)) {
		retval = GetLastError();
		जाओ error_out;
	}
	अगर ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(आकार, channel->align))) {
		अगर (!ReadFile(तथ्य->handle, buf, आकार, &actual, शून्य)) {
			retval = GetLastError();
			जाओ error_out;
		}
		अगर (actual != आकार) {
	short_read:
		अगर (actual < 0) {
			retval = GetLastError();
			actual = 0;
		} अन्यथा
			retval = EXT2_ET_SHORT_READ;
		जाओ error_out;
		}
	वापस 0;
	}

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_read:
	जबतक (आकार > 0) {
		अगर (!ReadFile(तथ्य->handle, तथ्य->bounce, channel->block_आकार, &actual, शून्य)) {
			retval = GetLastError();
			जाओ error_out;
		}
		अगर (actual != channel->block_आकार) {
			actual = really_read;
			buf -= really_read;
			आकार += really_read;
			जाओ short_read;
		}
		actual = आकार;
		अगर (आकार > channel->block_आकार)
			actual = channel->block_आकार;
		स_नकल(buf, तथ्य->bounce, actual);
		really_read += actual;
		आकार -= actual;
		buf += actual;
	}
	वापस 0;

error_out:
	अगर (actual >= 0 && actual < आकार)
		स_रखो((अक्षर *) buf+actual, 0, आकार-actual);
	अगर (channel->read_error)
		retval = (channel->read_error)(channel, block, गिनती, buf,
					       आकार, actual, retval);
	वापस retval;
}

अटल errcode_t raw_write_blk(io_channel channel,
			       काष्ठा windows_private_तथ्य *तथ्य,
			       अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, स्थिर व्योम *bufv)
{
	ssize_t		आकार;
	ext2_loff_t	location;
	DWORD		actual = 0;
	errcode_t	retval;
	स्थिर अचिन्हित अक्षर *buf = bufv;

	अगर (गिनती == 1)
		आकार = channel->block_आकार;
	अन्यथा {
		अगर (गिनती < 0)
			आकार = -गिनती;
		अन्यथा
			आकार = गिनती * channel->block_आकार;
	}
	तथ्य->io_stats.bytes_written += आकार;

	location = ((ext2_loff_t) block * channel->block_आकार) + तथ्य->offset;

	अगर (तथ्य->flags & IO_FLAG_FORCE_BOUNCE) {
		अगर (!SetFilePointerEx(तथ्य->handle, make_large_integer(location), शून्य, FILE_BEGIN)) {
			retval = GetLastError();
			जाओ error_out;
		}
	जाओ bounce_write;
	}

		अगर (!SetFilePointerEx(तथ्य->handle, make_large_integer(location), शून्य, FILE_BEGIN)) {
			retval = GetLastError();
		जाओ error_out;
	}

	SetLastError(0);

	अगर ((channel->align == 0) || (IS_ALIGNED(buf, channel->align) && IS_ALIGNED(आकार, channel->align))) {
		अगर (!WriteFile(तथ्य->handle, buf, आकार, &actual, शून्य)) {
			retval = GetLastError();
			जाओ error_out;
		}

		अगर (actual != आकार) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			जाओ error_out;
		}
		वापस 0;
	}

	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
bounce_write:
	जबतक (आकार > 0) {
		अगर (आकार < channel->block_आकार) {
			अगर (!ReadFile(तथ्य->handle, तथ्य->bounce, channel->block_आकार, &actual, शून्य)) {
				retval = GetLastError();
				जाओ error_out;
			}
			अगर (actual != channel->block_आकार) {
				अगर (actual < 0) {
					retval = GetLastError();
					जाओ error_out;
				}
				स_रखो((अक्षर *) तथ्य->bounce + actual, 0,
				       channel->block_आकार - actual);
			}
		}
		actual = आकार;
		अगर (आकार > channel->block_आकार)
			actual = channel->block_आकार;
		स_नकल(तथ्य->bounce, buf, actual);
		अगर (!SetFilePointerEx(तथ्य->handle, make_large_integer(location), शून्य, FILE_BEGIN)) {
			retval = GetLastError();
			जाओ error_out;
		}
		अगर (!WriteFile(तथ्य->handle, तथ्य->bounce, channel->block_आकार, &actual, शून्य)) {
			retval = GetLastError();
			जाओ error_out;
		}

		अगर (actual != channel->block_आकार)
			जाओ short_write;
		आकार -= actual;
		buf += actual;
		location += actual;
	}
	वापस 0;

error_out:
	अगर (channel->write_error)
		retval = (channel->write_error)(channel, block, गिनती, buf,
						आकार, actual, retval);
	वापस retval;
}


/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
अटल errcode_t alloc_cache(io_channel channel,
			     काष्ठा windows_private_तथ्य *तथ्य)
{
	errcode_t		retval;
	काष्ठा windows_cache	*cache;
	पूर्णांक			i;

	तथ्य->access_time = 0;
	क्रम (i=0, cache = तथ्य->cache; i < CACHE_आकार; i++, cache++) {
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
अटल व्योम free_cache(काष्ठा windows_private_तथ्य *तथ्य)
{
	काष्ठा windows_cache    *cache;
	पूर्णांक			i;

	तथ्य->access_time = 0;
	क्रम (i=0, cache = तथ्य->cache; i < CACHE_आकार; i++, cache++) {
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
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
अटल काष्ठा windows_cache *find_cached_block(काष्ठा windows_private_तथ्य *तथ्य,
					    अचिन्हित दीर्घ दीर्घ block,
					    काष्ठा windows_cache **eldest)
{
	काष्ठा windows_cache	*cache, *unused_cache, *oldest_cache;
	पूर्णांक			i;

	unused_cache = oldest_cache = 0;
	क्रम (i=0, cache = तथ्य->cache; i < CACHE_आकार; i++, cache++) {
		अगर (!cache->in_use) {
			अगर (!unused_cache)
				unused_cache = cache;
			जारी;
		}
		अगर (cache->block == block) {
			cache->access_time = ++तथ्य->access_time;
			वापस cache;
		}
		अगर (!oldest_cache ||
		   (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	अगर (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	वापस 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
अटल व्योम reuse_cache(io_channel channel, काष्ठा windows_private_तथ्य *तथ्य,
		 काष्ठा windows_cache *cache, अचिन्हित दीर्घ दीर्घ block)
{
	अगर (cache->dirty && cache->in_use)
		raw_write_blk(channel, तथ्य, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++तथ्य->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
अटल errcode_t flush_cached_blocks(io_channel channel,
				     काष्ठा windows_private_तथ्य *तथ्य,
				     पूर्णांक invalidate)
{
	काष्ठा windows_cache	*cache;
	errcode_t	retval, retval2;
	पूर्णांक			i;

	retval2 = 0;
	क्रम (i=0, cache = तथ्य->cache; i < CACHE_आकार; i++, cache++) {
		अगर (!cache->in_use)
			जारी;

		अगर (invalidate)
			cache->in_use = 0;

		अगर (!cache->dirty)
			जारी;

		retval = raw_write_blk(channel, तथ्य,
				       cache->block, 1, cache->buf);
		अगर (retval)
			retval2 = retval;
		अन्यथा
			cache->dirty = 0;
	}
	वापस retval2;
}
#पूर्ण_अगर /* NO_IO_CACHE */

अटल errcode_t windows_open_channel(काष्ठा windows_private_तथ्य *तथ्य,
				   पूर्णांक flags, io_channel *channel,
				   io_manager io_mgr)
{
	io_channel	io = शून्य;
	errcode_t	retval;
	ext2fs_struct_stat st;

	retval = ext2fs_get_mem(माप(काष्ठा struct_io_channel), &io);
	अगर (retval)
		जाओ cleanup;
	स_रखो(io, 0, माप(काष्ठा struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

	io->manager = io_mgr;
	retval = ext2fs_get_mem(म_माप(तथ्य->name)+1, &io->name);
	अगर (retval)
		जाओ cleanup;

	म_नकल(io->name, तथ्य->name);
	io->private_तथ्य = तथ्य;
	io->block_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

#अगर defined(O_DIRECT)
	अगर (flags & IO_FLAG_DIRECT_IO)
		io->align = ext2fs_get_dio_alignment(तथ्य->dev);
#पूर्ण_अगर

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	अगर (ext2fs_fstat(तथ्य->dev, &st) == 0) {
		अगर (ext2fsP_is_disk_device(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		अन्यथा
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

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

	*channel = io;
	वापस 0;

cleanup:
	अगर (तथ्य) {
		अगर (तथ्य->dev >= 0)
			बंद(तथ्य->dev);
		free_cache(तथ्य);
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

अटल DWORD windows_open_device(काष्ठा windows_private_तथ्य *तथ्य, पूर्णांक open_flags)
{
	DWORD ret = 0;

	अगर (*तथ्य->name != '\\')
		म_नकल(तथ्य->cf_device, तथ्य->name);
	अन्यथा अगर (fake_dos_name_for_device(तथ्य))
		वापस -1;

	DWORD desired_access = GENERIC_READ | ((open_flags & O_RDWR) ? GENERIC_WRITE : 0);
	DWORD share_mode = (open_flags & O_EXCL) ? 0 : FILE_SHARE_READ | ((open_flags & O_RDWR) ? FILE_SHARE_WRITE : 0);
	DWORD flags_and_attributes =
#अगर defined(O_DIRECT)
	(open_flags & O_DIRECT) ? (FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH) : FILE_ATTRIBUTE_NORMAL;
#अन्यथा
	FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
#पूर्ण_अगर

	तथ्य->handle = CreateFile(तथ्य->cf_device, desired_access, share_mode, शून्य, OPEN_EXISTING,
				  flags_and_attributes, शून्य);

	अगर (तथ्य->handle == INVALID_HANDLE_मूल्य) {
		ret = GetLastError();
		जाओ invalid_handle;
	}

	TRACE("e2fsprogs::windows_open_device::CreateFile(\"%s\") = %p", तथ्य->cf_device, तथ्य->handle);

	तथ्य->dev = _open_osfhandle((intptr_t)तथ्य->handle, 0);
	अगर (तथ्य->dev < 0) {
		ret = GetLastError() ? GetLastError() : 9999;
		जाओ osfhandle_error;
	}

	TRACE("e2fsprogs::windows_open_device::_open_osfhandle(%p) = %d", तथ्य->handle, तथ्य->dev);

	वापस 0;

osfhandle_error:
	TRACE("e2fsprogs::windows_open_device::CloseHandle(%p)", तथ्य->handle);
	CloseHandle(तथ्य->handle);
invalid_handle:
	remove_fake_dos_name(तथ्य);
	TRACE("e2fsprogs::windows_open_device() = %lu, errno = %d", ret, त्रुटि_सं);
	वापस ret;
}

अटल काष्ठा windows_private_तथ्य *init_private_तथ्य(स्थिर अक्षर *name, पूर्णांक flags)
{
	काष्ठा windows_private_तथ्य *तथ्य = शून्य;

	अगर (ext2fs_get_mem(माप(काष्ठा windows_private_तथ्य), &तथ्य))
		वापस शून्य;

	स_रखो(तथ्य, 0, माप(काष्ठा windows_private_तथ्य));
	म_नकलन(तथ्य->name, name, माप(तथ्य->name) - 1);
	तथ्य->magic = EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL;
	तथ्य->io_stats.अंक_fields = 2;
	तथ्य->flags = flags;
	तथ्य->handle = INVALID_HANDLE_मूल्य;

	वापस तथ्य;
}

अटल errcode_t windows_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	पूर्णांक open_flags;
	काष्ठा windows_private_तथ्य *तथ्य;

	अगर (name == 0)
		वापस EXT2_ET_BAD_DEVICE_NAME;

	तथ्य = init_private_तथ्य(name, flags);
	अगर (!तथ्य)
		वापस EXT2_ET_NO_MEMORY;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	अगर (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#अगर defined(O_DIRECT)
	अगर (flags & IO_FLAG_DIRECT_IO)
		open_flags |= O_DIRECT;
#पूर्ण_अगर

	अगर (windows_open_device(तथ्य, open_flags)) {
		ext2fs_free_mem(&तथ्य);
		वापस EXT2_ET_BAD_DEVICE_NAME;
	}

	वापस windows_open_channel(तथ्य, flags, channel, windows_io_manager);
}

अटल errcode_t windows_close(io_channel channel)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	अगर (--channel->refcount > 0)
		वापस 0;

#अगर_अघोषित NO_IO_CACHE
	retval = flush_cached_blocks(channel, तथ्य, 0);
#पूर्ण_अगर

	remove_fake_dos_name(तथ्य);

	अगर (_close(तथ्य->dev) != 0)
		retval = त्रुटि_सं;

	TRACE("e2fsprogs::windows_close::_close(%d)", तथ्य->dev);

	free_cache(तथ्य);

	ext2fs_free_mem(&channel->private_तथ्य);
	अगर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	वापस retval;
}

अटल errcode_t windows_set_blksize(io_channel channel, पूर्णांक blksize)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	अगर (channel->block_आकार != blksize) {
#अगर_अघोषित NO_IO_CACHE
		अगर ((retval = flush_cached_blocks(channel, तथ्य, 0)))
			वापस retval;
#पूर्ण_अगर

		channel->block_आकार = blksize;
		free_cache(तथ्य);
		अगर ((retval = alloc_cache(channel, तथ्य)))
			वापस retval;
	}
	वापस 0;
}

अटल errcode_t windows_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, व्योम *buf)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	काष्ठा windows_cache *cache, *reuse[READ_DIRECT_आकार];
	errcode_t	retval;
	अक्षर		*cp;
	पूर्णांक		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#अगर_घोषित NO_IO_CACHE
	वापस raw_read_blk(channel, तथ्य, block, गिनती, buf);
#अन्यथा
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
	जबतक (गिनती > 0) {
		/* If it's in the cache, use it! */
		अगर ((cache = find_cached_block(तथ्य, block, &reuse[0]))) {
#अगर_घोषित DEBUG
			म_लिखो("Using cached block %lu\n", block);
#पूर्ण_अगर
			स_नकल(cp, cache->buf, channel->block_आकार);
			गिनती--;
			block++;
			cp += channel->block_आकार;
			जारी;
		}
		अगर (गिनती == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, तथ्य, cache, block);
			अगर ((retval = raw_read_blk(channel, तथ्य, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				वापस retval;
			}
			स_नकल(cp, cache->buf, channel->block_आकार);
			वापस 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		क्रम (i=1; i < गिनती; i++)
			अगर (find_cached_block(तथ्य, block+i, &reuse[i]))
				अवरोध;
#अगर_घोषित DEBUG
		म_लिखो("Reading %d blocks starting at %lu\n", i, block);
#पूर्ण_अगर
		अगर ((retval = raw_read_blk(channel, तथ्य, block, i, cp)))
			वापस retval;

		/* Save the results in the cache */
		क्रम (j=0; j < i; j++) {
			गिनती--;
			cache = reuse[j];
			reuse_cache(channel, तथ्य, cache, block++);
			स_नकल(cache->buf, cp, channel->block_आकार);
			cp += channel->block_आकार;
		}
	}
	वापस 0;
#पूर्ण_अगर /* NO_IO_CACHE */
}

अटल errcode_t windows_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	वापस windows_read_blk64(channel, block, गिनती, buf);
}

अटल errcode_t windows_write_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	काष्ठा windows_cache *cache, *reuse;
	errcode_t	retval = 0;
	स्थिर अक्षर	*cp;
	पूर्णांक		writethrough;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#अगर_घोषित NO_IO_CACHE
	वापस raw_write_blk(channel, तथ्य, block, गिनती, buf);
#अन्यथा
	/*
	 * If we're doing an odd-sized write or a very large write,
	 * flush out the cache completely and then do a direct write.
	 */
	अगर (गिनती < 0 || गिनती > WRITE_DIRECT_आकार) {
		अगर ((retval = flush_cached_blocks(channel, तथ्य, 1)))
			वापस retval;
		वापस raw_write_blk(channel, तथ्य, block, गिनती, buf);
	}

	/*
	 * For a moderate-sized multi-block write, first force a write
	 * if we're in write-through cache mode, and then fill the
	 * cache with the blocks.
	 */
	writethrough = channel->flags & CHANNEL_FLAGS_WRITETHROUGH;
	अगर (writethrough)
		retval = raw_write_blk(channel, तथ्य, block, गिनती, buf);

	cp = buf;
	जबतक (गिनती > 0) {
		cache = find_cached_block(तथ्य, block, &reuse);
		अगर (!cache) {
			cache = reuse;
			reuse_cache(channel, तथ्य, cache, block);
		}
		अगर (cache->buf != cp)
			स_नकल(cache->buf, cp, channel->block_आकार);
		cache->dirty = !writethrough;
		गिनती--;
		block++;
		cp += channel->block_आकार;
	}
	वापस retval;
#पूर्ण_अगर /* NO_IO_CACHE */
}

अटल errcode_t windows_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
	वापस EXT2_ET_OP_NOT_SUPPORTED;
}

अटल errcode_t windows_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	वापस windows_write_blk64(channel, block, गिनती, buf);
}

अटल errcode_t windows_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक आकार, स्थिर व्योम *buf)
{
	वापस EXT2_ET_UNIMPLEMENTED;
}

/*
 * Flush data buffers to disk.
 */
अटल errcode_t windows_flush(io_channel channel)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	errcode_t retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

#अगर_अघोषित NO_IO_CACHE
	retval = flush_cached_blocks(channel, तथ्य, 0);
#पूर्ण_अगर

	वापस retval;
}

अटल errcode_t windows_set_option(io_channel channel, स्थिर अक्षर *option,
				 स्थिर अक्षर *arg)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	अचिन्हित दीर्घ दीर्घ tmp;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

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
	वापस EXT2_ET_INVALID_ARGUMENT;
}

अटल errcode_t windows_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 अचिन्हित दीर्घ दीर्घ गिनती)
{
	TRACE("e2fsprogs::windows_discard::EXT2_ET_UNIMPLEMENTED");
	वापस EXT2_ET_UNIMPLEMENTED;
}

/* parameters might not be used if OS doesn't support zeroout */
#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wunused-parameter"
#पूर्ण_अगर
अटल errcode_t windows_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा windows_private_तथ्य *तथ्य;
	पूर्णांक		ret;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा windows_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_WINDOWS_IO_CHANNEL);

	अगर (channel->flags & CHANNEL_FLAGS_BLOCK_DEVICE) {
		/* Not implemented until the BLKZEROOUT mess is fixed */
		जाओ unimplemented;
	} अन्यथा {
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
		जाओ unimplemented;
	}
err:
	अगर (ret < 0) {
		अगर (त्रुटि_सं == EOPNOTSUPP)
			जाओ unimplemented;
		वापस त्रुटि_सं;
	}
	वापस 0;
unimplemented:
	वापस EXT2_ET_UNIMPLEMENTED;
}

पूर्णांक ext2fs_open_file(स्थिर अक्षर *pathname, पूर्णांक flags, mode_t mode)
{
	flags |= O_द्विआधारी;

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

#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर

अटल काष्ठा struct_io_manager struct_windows_manager = {
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
