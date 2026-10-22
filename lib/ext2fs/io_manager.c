/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᰀᰧ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᰀᰧ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᰀᰧ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (!opts)
		ᰀᰧᰌ 0;

	ᰀᰧ (!channel->manager->set_option)
		ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᰀᰧ (!options)
		ᰀᰧᰌ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᰀᰬ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᰀᰧ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᰀᰧ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᰀᰧ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᰀᰧᰌ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (channel->manager->write_byte)
		ᰀᰧᰌ channel->manager->write_byte(channel, offset,
						    count, data);

	ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (channel->manager->read_blk64)
		ᰀᰧᰌ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᰀᰧ ((block >> 32) != 0)
		ᰀᰧᰌ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᰀᰧᰌ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (channel->manager->write_blk64)
		ᰀᰧᰌ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᰀᰧ ((block >> 32) != 0)
		ᰀᰧᰌ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᰀᰧᰌ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (channel->manager->discard)
		ᰀᰧᰌ (channel->manager->discard)(channel, block, count);

	ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᰀᰧ (channel->manager->zeroout)
		ᰀᰧᰌ (channel->manager->zeroout)(channel, block, count);

	ᰀᰧᰌ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᰀᰧ (count == 0)
		size = io->block_size;
	ᰀᰧᰀ ᰀᰧ (count > 0)
		size = io->block_size * count;
	ᰀᰧᰀ
		size = -count;

	ᰀᰧ (io->align > 0) {
		ᰀᰧ ((unsigned) io->align > size)
			size = io->align;
		ᰀᰧᰌ ext2fs_get_memalign(size, io->align, ptr);
	} ᰀᰧᰀ
		ᰀᰧᰌ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᰀᰧ (!io->manager->cache_readahead)
		ᰀᰧᰌ EXT2_ET_OP_NOT_SUPPORTED;

	ᰀᰧᰌ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᰀᰧ (!io->manager->flock)
		ᰀᰧᰌ EXT2_ET_OP_NOT_SUPPORTED;

	ᰀᰧᰌ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᰀᰧ (!io->manager->flock)
		ᰀᰧᰌ EXT2_ET_OP_NOT_SUPPORTED;

	ᰀᰧᰌ io->manager->flock(io, 0);
}
