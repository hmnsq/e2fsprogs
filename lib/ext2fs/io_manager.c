/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#གལ་སྲིད HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་སྲིད HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (!opts)
		ལོག 0;

	གལ་སྲིད (!channel->manager->set_option)
		ལོག EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	གལ་སྲིད (!options)
		ལོག EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	སྐབས (ptr && *ptr) {
		next = strchr(ptr, '&');
		གལ་སྲིད (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		གལ་སྲིད (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		གལ་སྲིད (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ལོག retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ཧྲིལ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (channel->manager->write_byte)
		ལོག channel->manager->write_byte(channel, offset,
						    count, data);

	ལོག EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ཧྲིལ count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (channel->manager->read_blk64)
		ལོག (channel->manager->read_blk64)(channel, block,
						      count, data);

	གལ་སྲིད ((block >> 32) != 0)
		ལོག EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ལོག (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ཧྲིལ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (channel->manager->write_blk64)
		ལོག (channel->manager->write_blk64)(channel, block,
						       count, data);

	གལ་སྲིད ((block >> 32) != 0)
		ལོག EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ལོག (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (channel->manager->discard)
		ལོག (channel->manager->discard)(channel, block, count);

	ལོག EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་སྲིད (channel->manager->zeroout)
		ལོག (channel->manager->zeroout)(channel, block, count);

	ལོག EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ཧྲིལ count, void *ptr)
{
	size_t	size;

	གལ་སྲིད (count == 0)
		size = io->block_size;
	དེ་མིན གལ་སྲིད (count > 0)
		size = io->block_size * count;
	དེ་མིན
		size = -count;

	གལ་སྲིད (io->align > 0) {
		གལ་སྲིད ((unsigned) io->align > size)
			size = io->align;
		ལོག ext2fs_get_memalign(size, io->align, ptr);
	} དེ་མིན
		ལོག ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	གལ་སྲིད (!io->manager->cache_readahead)
		ལོག EXT2_ET_OP_NOT_SUPPORTED;

	ལོག io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ཧྲིལ flock_flags)
{
	གལ་སྲིད (!io->manager->flock)
		ལོག EXT2_ET_OP_NOT_SUPPORTED;

	ལོག io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	གལ་སྲིད (!io->manager->flock)
		ལོག EXT2_ET_OP_NOT_SUPPORTED;

	ལོག io->manager->flock(io, 0);
}
