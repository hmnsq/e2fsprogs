/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᯀᯮᯝ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᯀᯮᯝ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᯀᯮᯝ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (!opts)
		ᯀᯮᯞ 0;

	ᯀᯮᯝ (!channel->manager->set_option)
		ᯀᯮᯞ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᯀᯮᯝ (!options)
		ᯀᯮᯞ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᯀᯬᯝ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᯀᯮᯝ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᯀᯮᯝ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᯀᯮᯝ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᯀᯮᯞ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (channel->manager->write_byte)
		ᯀᯮᯞ channel->manager->write_byte(channel, offset,
						    count, data);

	ᯀᯮᯞ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (channel->manager->read_blk64)
		ᯀᯮᯞ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᯀᯮᯝ ((block >> 32) != 0)
		ᯀᯮᯞ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᯀᯮᯞ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (channel->manager->write_blk64)
		ᯀᯮᯞ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᯀᯮᯝ ((block >> 32) != 0)
		ᯀᯮᯞ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᯀᯮᯞ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (channel->manager->discard)
		ᯀᯮᯞ (channel->manager->discard)(channel, block, count);

	ᯀᯮᯞ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᯀᯮᯝ (channel->manager->zeroout)
		ᯀᯮᯞ (channel->manager->zeroout)(channel, block, count);

	ᯀᯮᯞ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᯀᯮᯝ (count == 0)
		size = io->block_size;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (count > 0)
		size = io->block_size * count;
	ᯀᯮᯝᯂᯪ
		size = -count;

	ᯀᯮᯝ (io->align > 0) {
		ᯀᯮᯝ ((unsigned) io->align > size)
			size = io->align;
		ᯀᯮᯞ ext2fs_get_memalign(size, io->align, ptr);
	} ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᯀᯮᯝ (!io->manager->cache_readahead)
		ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;

	ᯀᯮᯞ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᯀᯮᯝ (!io->manager->flock)
		ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;

	ᯀᯮᯞ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᯀᯮᯝ (!io->manager->flock)
		ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;

	ᯀᯮᯞ io->manager->flock(io, 0);
}
