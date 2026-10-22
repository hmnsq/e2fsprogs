/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#もし HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#もし HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#もし HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (!opts)
		戻る 0;

	もし (!channel->manager->set_option)
		戻る EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	もし (!options)
		戻る EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ながら (ptr && *ptr) {
		next = strchr(ptr, '&');
		もし (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		もし (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		もし (retval)
			break;
		ptr = next;
	}
	free(options);
	戻る retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				整数 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (channel->manager->write_byte)
		戻る channel->manager->write_byte(channel, offset,
						    count, data);

	戻る EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 整数 count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (channel->manager->read_blk64)
		戻る (channel->manager->read_blk64)(channel, block,
						      count, data);

	もし ((block >> 32) != 0)
		戻る EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	戻る (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 整数 count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (channel->manager->write_blk64)
		戻る (channel->manager->write_blk64)(channel, block,
						       count, data);

	もし ((block >> 32) != 0)
		戻る EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	戻る (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (channel->manager->discard)
		戻る (channel->manager->discard)(channel, block, count);

	戻る EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	もし (channel->manager->zeroout)
		戻る (channel->manager->zeroout)(channel, block, count);

	戻る EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, 整数 count, void *ptr)
{
	size_t	size;

	もし (count == 0)
		size = io->block_size;
	否则 もし (count > 0)
		size = io->block_size * count;
	否则
		size = -count;

	もし (io->align > 0) {
		もし ((unsigned) io->align > size)
			size = io->align;
		戻る ext2fs_get_memalign(size, io->align, ptr);
	} 否则
		戻る ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	もし (!io->manager->cache_readahead)
		戻る EXT2_ET_OP_NOT_SUPPORTED;

	戻る io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned 整数 flock_flags)
{
	もし (!io->manager->flock)
		戻る EXT2_ET_OP_NOT_SUPPORTED;

	戻る io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	もし (!io->manager->flock)
		戻る EXT2_ET_OP_NOT_SUPPORTED;

	戻る io->manager->flock(io, 0);
}
