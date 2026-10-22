/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#如果 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#如果 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (!opts)
		返回 0;

	如果 (!channel->manager->set_option)
		返回 EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	如果 (!options)
		返回 EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	当 (ptr && *ptr) {
		next = strchr(ptr, '&');
		如果 (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		如果 (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		如果 (retval)
			break;
		ptr = next;
	}
	free(options);
	返回 retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				整数 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (channel->manager->write_byte)
		返回 channel->manager->write_byte(channel, offset,
						    count, data);

	返回 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 整数 count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (channel->manager->read_blk64)
		返回 (channel->manager->read_blk64)(channel, block,
						      count, data);

	如果 ((block >> 32) != 0)
		返回 EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	返回 (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 整数 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (channel->manager->write_blk64)
		返回 (channel->manager->write_blk64)(channel, block,
						       count, data);

	如果 ((block >> 32) != 0)
		返回 EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	返回 (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (channel->manager->discard)
		返回 (channel->manager->discard)(channel, block, count);

	返回 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	如果 (channel->manager->zeroout)
		返回 (channel->manager->zeroout)(channel, block, count);

	返回 EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, 整数 count, void *ptr)
{
	size_t	size;

	如果 (count == 0)
		size = io->block_size;
	否则 如果 (count > 0)
		size = io->block_size * count;
	否则
		size = -count;

	如果 (io->align > 0) {
		如果 ((unsigned) io->align > size)
			size = io->align;
		返回 ext2fs_get_memalign(size, io->align, ptr);
	} 否则
		返回 ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	如果 (!io->manager->cache_readahead)
		返回 EXT2_ET_OP_NOT_SUPPORTED;

	返回 io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned 整数 flock_flags)
{
	如果 (!io->manager->flock)
		返回 EXT2_ET_OP_NOT_SUPPORTED;

	返回 io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	如果 (!io->manager->flock)
		返回 EXT2_ET_OP_NOT_SUPPORTED;

	返回 io->manager->flock(io, 0);
}
