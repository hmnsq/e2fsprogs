/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᤀᤡ᤺ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᤀᤡ᤺ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (!opts)
		ᤀᤡᤴ 0;

	ᤀᤡ᤺ (!channel->manager->set_option)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᤀᤡ᤺ (!options)
		ᤀᤡᤴ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᤀᤠ᤺ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᤀᤡ᤺ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᤀᤡ᤺ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᤀᤡ᤺ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᤀᤡᤴ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (channel->manager->write_byte)
		ᤀᤡᤴ channel->manager->write_byte(channel, offset,
						    count, data);

	ᤀᤡᤴ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (channel->manager->read_blk64)
		ᤀᤡᤴ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᤀᤡ᤺ ((block >> 32) != 0)
		ᤀᤡᤴ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᤀᤡᤴ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (channel->manager->write_blk64)
		ᤀᤡᤴ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᤀᤡ᤺ ((block >> 32) != 0)
		ᤀᤡᤴ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᤀᤡᤴ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (channel->manager->discard)
		ᤀᤡᤴ (channel->manager->discard)(channel, block, count);

	ᤀᤡᤴ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᤀᤡ᤺ (channel->manager->zeroout)
		ᤀᤡᤴ (channel->manager->zeroout)(channel, block, count);

	ᤀᤡᤴ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᤀᤡ᤺ (count == 0)
		size = io->block_size;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (count > 0)
		size = io->block_size * count;
	ᤀᤡ᤺ᤆᤡ
		size = -count;

	ᤀᤡ᤺ (io->align > 0) {
		ᤀᤡ᤺ ((unsigned) io->align > size)
			size = io->align;
		ᤀᤡᤴ ext2fs_get_memalign(size, io->align, ptr);
	} ᤀᤡ᤺ᤆᤡ
		ᤀᤡᤴ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᤀᤡ᤺ (!io->manager->cache_readahead)
		ᤀᤡᤴ EXT2_ET_OP_NOT_SUPPORTED;

	ᤀᤡᤴ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᤀᤡ᤺ (!io->manager->flock)
		ᤀᤡᤴ EXT2_ET_OP_NOT_SUPPORTED;

	ᤀᤡᤴ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᤀᤡ᤺ (!io->manager->flock)
		ᤀᤡᤴ EXT2_ET_OP_NOT_SUPPORTED;

	ᤀᤡᤴ io->manager->flock(io, 0);
}
