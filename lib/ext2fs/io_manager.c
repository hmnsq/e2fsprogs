/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ако HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ако HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (!opts)
		врати 0;

	ако (!channel->manager->set_option)
		врати EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ако (!options)
		врати EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	док (ptr && *ptr) {
		next = strchr(ptr, '&');
		ако (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ако (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ако (retval)
			break;
		ptr = next;
	}
	free(options);
	врати retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				цео count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (channel->manager->write_byte)
		врати channel->manager->write_byte(channel, offset,
						    count, data);

	врати EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 цео count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (channel->manager->read_blk64)
		врати (channel->manager->read_blk64)(channel, block,
						      count, data);

	ако ((block >> 32) != 0)
		врати EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	врати (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 цео count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (channel->manager->write_blk64)
		врати (channel->manager->write_blk64)(channel, block,
						       count, data);

	ако ((block >> 32) != 0)
		врати EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	врати (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (channel->manager->discard)
		врати (channel->manager->discard)(channel, block, count);

	врати EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ако (channel->manager->zeroout)
		врати (channel->manager->zeroout)(channel, block, count);

	врати EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, цео count, void *ptr)
{
	size_t	size;

	ако (count == 0)
		size = io->block_size;
	инако ако (count > 0)
		size = io->block_size * count;
	инако
		size = -count;

	ако (io->align > 0) {
		ако ((unsigned) io->align > size)
			size = io->align;
		врати ext2fs_get_memalign(size, io->align, ptr);
	} инако
		врати ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ако (!io->manager->cache_readahead)
		врати EXT2_ET_OP_NOT_SUPPORTED;

	врати io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned цео flock_flags)
{
	ако (!io->manager->flock)
		врати EXT2_ET_OP_NOT_SUPPORTED;

	врати io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ако (!io->manager->flock)
		врати EXT2_ET_OP_NOT_SUPPORTED;

	врати io->manager->flock(io, 0);
}
