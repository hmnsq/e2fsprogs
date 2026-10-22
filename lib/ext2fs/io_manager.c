/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᨀᨘ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᨀᨘ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᨀᨘ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (!opts)
		ᨀᨘᨒ 0;

	ᨀᨘ (!channel->manager->set_option)
		ᨀᨘᨒ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᨀᨘ (!options)
		ᨀᨘᨒ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᨀᨛ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᨀᨘ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᨀᨘ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᨀᨘ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᨀᨘᨒ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (channel->manager->write_byte)
		ᨀᨘᨒ channel->manager->write_byte(channel, offset,
						    count, data);

	ᨀᨘᨒ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (channel->manager->read_blk64)
		ᨀᨘᨒ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᨀᨘ ((block >> 32) != 0)
		ᨀᨘᨒ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᨀᨘᨒ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (channel->manager->write_blk64)
		ᨀᨘᨒ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᨀᨘ ((block >> 32) != 0)
		ᨀᨘᨒ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᨀᨘᨒ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (channel->manager->discard)
		ᨀᨘᨒ (channel->manager->discard)(channel, block, count);

	ᨀᨘᨒ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᨀᨘ (channel->manager->zeroout)
		ᨀᨘᨒ (channel->manager->zeroout)(channel, block, count);

	ᨀᨘᨒ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᨀᨘ (count == 0)
		size = io->block_size;
	ᨀᨘᨁ ᨀᨘ (count > 0)
		size = io->block_size * count;
	ᨀᨘᨁ
		size = -count;

	ᨀᨘ (io->align > 0) {
		ᨀᨘ ((unsigned) io->align > size)
			size = io->align;
		ᨀᨘᨒ ext2fs_get_memalign(size, io->align, ptr);
	} ᨀᨘᨁ
		ᨀᨘᨒ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᨀᨘ (!io->manager->cache_readahead)
		ᨀᨘᨒ EXT2_ET_OP_NOT_SUPPORTED;

	ᨀᨘᨒ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᨀᨘ (!io->manager->flock)
		ᨀᨘᨒ EXT2_ET_OP_NOT_SUPPORTED;

	ᨀᨘᨒ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᨀᨘ (!io->manager->flock)
		ᨀᨘᨒ EXT2_ET_OP_NOT_SUPPORTED;

	ᨀᨘᨒ io->manager->flock(io, 0);
}
