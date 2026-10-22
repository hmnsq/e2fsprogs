/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#если HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#если HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#если HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (!opts)
		вернуть 0;

	если (!channel->manager->set_option)
		вернуть EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	если (!options)
		вернуть EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	пока (ptr && *ptr) {
		next = strchr(ptr, '&');
		если (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		если (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		если (retval)
			break;
		ptr = next;
	}
	free(options);
	вернуть retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				целое count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (channel->manager->write_byte)
		вернуть channel->manager->write_byte(channel, offset,
						    count, data);

	вернуть EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 целое count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (channel->manager->read_blk64)
		вернуть (channel->manager->read_blk64)(channel, block,
						      count, data);

	если ((block >> 32) != 0)
		вернуть EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	вернуть (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 целое count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (channel->manager->write_blk64)
		вернуть (channel->manager->write_blk64)(channel, block,
						       count, data);

	если ((block >> 32) != 0)
		вернуть EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	вернуть (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (channel->manager->discard)
		вернуть (channel->manager->discard)(channel, block, count);

	вернуть EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	если (channel->manager->zeroout)
		вернуть (channel->manager->zeroout)(channel, block, count);

	вернуть EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, целое count, void *ptr)
{
	size_t	size;

	если (count == 0)
		size = io->block_size;
	иначе если (count > 0)
		size = io->block_size * count;
	иначе
		size = -count;

	если (io->align > 0) {
		если ((unsigned) io->align > size)
			size = io->align;
		вернуть ext2fs_get_memalign(size, io->align, ptr);
	} иначе
		вернуть ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	если (!io->manager->cache_readahead)
		вернуть EXT2_ET_OP_NOT_SUPPORTED;

	вернуть io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned целое flock_flags)
{
	если (!io->manager->flock)
		вернуть EXT2_ET_OP_NOT_SUPPORTED;

	вернуть io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	если (!io->manager->flock)
		вернуть EXT2_ET_OP_NOT_SUPPORTED;

	вернуть io->manager->flock(io, 0);
}
