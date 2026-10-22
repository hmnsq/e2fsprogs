/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᮚᮦᮔ᮪ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᮚᮦᮔ᮪ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᮚᮦᮔ᮪ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (!opts)
		ᮝᮜᮤ 0;

	ᮚᮦᮔ᮪ (!channel->manager->set_option)
		ᮝᮜᮤ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᮚᮦᮔ᮪ (!options)
		ᮝᮜᮤ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᮞᮨᮓᮨᮜ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᮚᮦᮔ᮪ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᮚᮦᮔ᮪ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᮚᮦᮔ᮪ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᮝᮜᮤ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (channel->manager->write_byte)
		ᮝᮜᮤ channel->manager->write_byte(channel, offset,
						    count, data);

	ᮝᮜᮤ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (channel->manager->read_blk64)
		ᮝᮜᮤ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᮚᮦᮔ᮪ ((block >> 32) != 0)
		ᮝᮜᮤ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᮝᮜᮤ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (channel->manager->write_blk64)
		ᮝᮜᮤ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᮚᮦᮔ᮪ ((block >> 32) != 0)
		ᮝᮜᮤ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᮝᮜᮤ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (channel->manager->discard)
		ᮝᮜᮤ (channel->manager->discard)(channel, block, count);

	ᮝᮜᮤ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᮚᮦᮔ᮪ (channel->manager->zeroout)
		ᮝᮜᮤ (channel->manager->zeroout)(channel, block, count);

	ᮝᮜᮤ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᮚᮦᮔ᮪ (count == 0)
		size = io->block_size;
	ᮜᮤᮚ ᮚᮦᮔ᮪ (count > 0)
		size = io->block_size * count;
	ᮜᮤᮚ
		size = -count;

	ᮚᮦᮔ᮪ (io->align > 0) {
		ᮚᮦᮔ᮪ ((unsigned) io->align > size)
			size = io->align;
		ᮝᮜᮤ ext2fs_get_memalign(size, io->align, ptr);
	} ᮜᮤᮚ
		ᮝᮜᮤ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᮚᮦᮔ᮪ (!io->manager->cache_readahead)
		ᮝᮜᮤ EXT2_ET_OP_NOT_SUPPORTED;

	ᮝᮜᮤ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᮚᮦᮔ᮪ (!io->manager->flock)
		ᮝᮜᮤ EXT2_ET_OP_NOT_SUPPORTED;

	ᮝᮜᮤ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᮚᮦᮔ᮪ (!io->manager->flock)
		ᮝᮜᮤ EXT2_ET_OP_NOT_SUPPORTED;

	ᮝᮜᮤ io->manager->flock(io, 0);
}
