/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#གལ་ཏེ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་ཏེ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (!opts)
		ཕྱིར 0;

	གལ་ཏེ (!channel->manager->set_option)
		ཕྱིར EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	གལ་ཏེ (!options)
		ཕྱིར EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	སྐབས (ptr && *ptr) {
		next = strchr(ptr, '&');
		གལ་ཏེ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		གལ་ཏེ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		གལ་ཏེ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ཕྱིར retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ཧྲིལ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (channel->manager->write_byte)
		ཕྱིར channel->manager->write_byte(channel, offset,
						    count, data);

	ཕྱིར EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ཧྲིལ count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (channel->manager->read_blk64)
		ཕྱིར (channel->manager->read_blk64)(channel, block,
						      count, data);

	གལ་ཏེ ((block >> 32) != 0)
		ཕྱིར EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ཕྱིར (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ཧྲིལ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (channel->manager->write_blk64)
		ཕྱིར (channel->manager->write_blk64)(channel, block,
						       count, data);

	གལ་ཏེ ((block >> 32) != 0)
		ཕྱིར EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ཕྱིར (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (channel->manager->discard)
		ཕྱིར (channel->manager->discard)(channel, block, count);

	ཕྱིར EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	གལ་ཏེ (channel->manager->zeroout)
		ཕྱིར (channel->manager->zeroout)(channel, block, count);

	ཕྱིར EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ཧྲིལ count, void *ptr)
{
	size_t	size;

	གལ་ཏེ (count == 0)
		size = io->block_size;
	དེ་མིན གལ་ཏེ (count > 0)
		size = io->block_size * count;
	དེ་མིན
		size = -count;

	གལ་ཏེ (io->align > 0) {
		གལ་ཏེ ((unsigned) io->align > size)
			size = io->align;
		ཕྱིར ext2fs_get_memalign(size, io->align, ptr);
	} དེ་མིན
		ཕྱིར ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	གལ་ཏེ (!io->manager->cache_readahead)
		ཕྱིར EXT2_ET_OP_NOT_SUPPORTED;

	ཕྱིར io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ཧྲིལ flock_flags)
{
	གལ་ཏེ (!io->manager->flock)
		ཕྱིར EXT2_ET_OP_NOT_SUPPORTED;

	ཕྱིར io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	གལ་ཏེ (!io->manager->flock)
		ཕྱིར EXT2_ET_OP_NOT_SUPPORTED;

	ཕྱིར io->manager->flock(io, 0);
}
