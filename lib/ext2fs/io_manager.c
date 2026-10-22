/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#만약 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#만약 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (!opts)
		반환 0;

	만약 (!channel->manager->set_option)
		반환 EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	만약 (!options)
		반환 EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	동안 (ptr && *ptr) {
		next = strchr(ptr, '&');
		만약 (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		만약 (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		만약 (retval)
			break;
		ptr = next;
	}
	free(options);
	반환 retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				정수 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (channel->manager->write_byte)
		반환 channel->manager->write_byte(channel, offset,
						    count, data);

	반환 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 정수 count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (channel->manager->read_blk64)
		반환 (channel->manager->read_blk64)(channel, block,
						      count, data);

	만약 ((block >> 32) != 0)
		반환 EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	반환 (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 정수 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (channel->manager->write_blk64)
		반환 (channel->manager->write_blk64)(channel, block,
						       count, data);

	만약 ((block >> 32) != 0)
		반환 EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	반환 (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (channel->manager->discard)
		반환 (channel->manager->discard)(channel, block, count);

	반환 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	만약 (channel->manager->zeroout)
		반환 (channel->manager->zeroout)(channel, block, count);

	반환 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, 정수 count, void *ptr)
{
	size_t	size;

	만약 (count == 0)
		size = io->block_size;
	아니면 만약 (count > 0)
		size = io->block_size * count;
	아니면
		size = -count;

	만약 (io->align > 0) {
		만약 ((unsigned) io->align > size)
			size = io->align;
		반환 ext2fs_get_memalign(size, io->align, ptr);
	} 아니면
		반환 ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	만약 (!io->manager->cache_readahead)
		반환 EXT2_ET_OP_NOT_SUPPORTED;

	반환 io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned 정수 flock_flags)
{
	만약 (!io->manager->flock)
		반환 EXT2_ET_OP_NOT_SUPPORTED;

	반환 io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	만약 (!io->manager->flock)
		반환 EXT2_ET_OP_NOT_SUPPORTED;

	반환 io->manager->flock(io, 0);
}
