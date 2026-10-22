/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (!opts)
		повернути 0;

	якщо (!channel->manager->set_option)
		повернути EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	якщо (!options)
		повернути EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	поки (ptr && *ptr) {
		next = strchr(ptr, '&');
		якщо (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		якщо (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		якщо (retval)
			break;
		ptr = next;
	}
	free(options);
	повернути retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ціле count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (channel->manager->write_byte)
		повернути channel->manager->write_byte(channel, offset,
						    count, data);

	повернути EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ціле count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (channel->manager->read_blk64)
		повернути (channel->manager->read_blk64)(channel, block,
						      count, data);

	якщо ((block >> 32) != 0)
		повернути EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	повернути (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ціле count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (channel->manager->write_blk64)
		повернути (channel->manager->write_blk64)(channel, block,
						       count, data);

	якщо ((block >> 32) != 0)
		повернути EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	повернути (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (channel->manager->discard)
		повернути (channel->manager->discard)(channel, block, count);

	повернути EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	якщо (channel->manager->zeroout)
		повернути (channel->manager->zeroout)(channel, block, count);

	повернути EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ціле count, void *ptr)
{
	size_t	size;

	якщо (count == 0)
		size = io->block_size;
	інакше якщо (count > 0)
		size = io->block_size * count;
	інакше
		size = -count;

	якщо (io->align > 0) {
		якщо ((unsigned) io->align > size)
			size = io->align;
		повернути ext2fs_get_memalign(size, io->align, ptr);
	} інакше
		повернути ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	якщо (!io->manager->cache_readahead)
		повернути EXT2_ET_OP_NOT_SUPPORTED;

	повернути io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ціле flock_flags)
{
	якщо (!io->manager->flock)
		повернути EXT2_ET_OP_NOT_SUPPORTED;

	повернути io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	якщо (!io->manager->flock)
		повернути EXT2_ET_OP_NOT_SUPPORTED;

	повернути io->manager->flock(io, 0);
}
