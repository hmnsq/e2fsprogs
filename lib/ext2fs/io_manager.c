/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ꦪꦼꦤ꧀ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ꦪꦼꦤ꧀ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (!opts)
		ꦮꦭꦶ 0;

	ꦪꦼꦤ꧀ (!channel->manager->set_option)
		ꦮꦭꦶ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ꦪꦼꦤ꧀ (!options)
		ꦮꦭꦶ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ꦱꦼꦢꦼꦭ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ꦪꦼꦤ꧀ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ꦪꦼꦤ꧀ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ꦪꦼꦤ꧀ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ꦮꦭꦶ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (channel->manager->write_byte)
		ꦮꦭꦶ channel->manager->write_byte(channel, offset,
						    count, data);

	ꦮꦭꦶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (channel->manager->read_blk64)
		ꦮꦭꦶ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ꦪꦼꦤ꧀ ((block >> 32) != 0)
		ꦮꦭꦶ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ꦮꦭꦶ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (channel->manager->write_blk64)
		ꦮꦭꦶ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ꦪꦼꦤ꧀ ((block >> 32) != 0)
		ꦮꦭꦶ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ꦮꦭꦶ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (channel->manager->discard)
		ꦮꦭꦶ (channel->manager->discard)(channel, block, count);

	ꦮꦭꦶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ꦪꦼꦤ꧀ (channel->manager->zeroout)
		ꦮꦭꦶ (channel->manager->zeroout)(channel, block, count);

	ꦮꦭꦶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ꦪꦼꦤ꧀ (count == 0)
		size = io->block_size;
	ꦭꦶꦪ ꦪꦼꦤ꧀ (count > 0)
		size = io->block_size * count;
	ꦭꦶꦪ
		size = -count;

	ꦪꦼꦤ꧀ (io->align > 0) {
		ꦪꦼꦤ꧀ ((unsigned) io->align > size)
			size = io->align;
		ꦮꦭꦶ ext2fs_get_memalign(size, io->align, ptr);
	} ꦭꦶꦪ
		ꦮꦭꦶ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ꦪꦼꦤ꧀ (!io->manager->cache_readahead)
		ꦮꦭꦶ EXT2_ET_OP_NOT_SUPPORTED;

	ꦮꦭꦶ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ꦪꦼꦤ꧀ (!io->manager->flock)
		ꦮꦭꦶ EXT2_ET_OP_NOT_SUPPORTED;

	ꦮꦭꦶ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ꦪꦼꦤ꧀ (!io->manager->flock)
		ꦮꦭꦶ EXT2_ET_OP_NOT_SUPPORTED;

	ꦮꦭꦶ io->manager->flock(io, 0);
}
