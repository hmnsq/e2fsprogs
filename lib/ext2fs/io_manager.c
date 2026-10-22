/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᬬᬾᬦ᭄ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᬬᬾᬦ᭄ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᬬᬾᬦ᭄ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (!opts)
		ᬯᬮᬶ 0;

	ᬬᬾᬦ᭄ (!channel->manager->set_option)
		ᬯᬮᬶ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ᬬᬾᬦ᭄ (!options)
		ᬯᬮᬶ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ᬲᭂᬤᭂᬮ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ᬬᬾᬦ᭄ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ᬬᬾᬦ᭄ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ᬬᬾᬦ᭄ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ᬯᬮᬶ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (channel->manager->write_byte)
		ᬯᬮᬶ channel->manager->write_byte(channel, offset,
						    count, data);

	ᬯᬮᬶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (channel->manager->read_blk64)
		ᬯᬮᬶ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ᬬᬾᬦ᭄ ((block >> 32) != 0)
		ᬯᬮᬶ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᬯᬮᬶ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (channel->manager->write_blk64)
		ᬯᬮᬶ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ᬬᬾᬦ᭄ ((block >> 32) != 0)
		ᬯᬮᬶ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ᬯᬮᬶ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (channel->manager->discard)
		ᬯᬮᬶ (channel->manager->discard)(channel, block, count);

	ᬯᬮᬶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ᬬᬾᬦ᭄ (channel->manager->zeroout)
		ᬯᬮᬶ (channel->manager->zeroout)(channel, block, count);

	ᬯᬮᬶ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ᬬᬾᬦ᭄ (count == 0)
		size = io->block_size;
	ᬮᬶᬬ ᬬᬾᬦ᭄ (count > 0)
		size = io->block_size * count;
	ᬮᬶᬬ
		size = -count;

	ᬬᬾᬦ᭄ (io->align > 0) {
		ᬬᬾᬦ᭄ ((unsigned) io->align > size)
			size = io->align;
		ᬯᬮᬶ ext2fs_get_memalign(size, io->align, ptr);
	} ᬮᬶᬬ
		ᬯᬮᬶ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ᬬᬾᬦ᭄ (!io->manager->cache_readahead)
		ᬯᬮᬶ EXT2_ET_OP_NOT_SUPPORTED;

	ᬯᬮᬶ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ᬬᬾᬦ᭄ (!io->manager->flock)
		ᬯᬮᬶ EXT2_ET_OP_NOT_SUPPORTED;

	ᬯᬮᬶ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ᬬᬾᬦ᭄ (!io->manager->flock)
		ᬯᬮᬶ EXT2_ET_OP_NOT_SUPPORTED;

	ᬯᬮᬶ io->manager->flock(io, 0);
}
