/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#නම් HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (!opts)
		ආපසු 0;

	නම් (!channel->manager->set_option)
		ආපසු EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	නම් (!options)
		ආපසු EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	අතරතුර (ptr && *ptr) {
		next = strchr(ptr, '&');
		නම් (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		නම් (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		නම් (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ආපසු retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				පූර්ණ_සංඛ්‍යා count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (channel->manager->write_byte)
		ආපසු channel->manager->write_byte(channel, offset,
						    count, data);

	ආපසු EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 පූර්ණ_සංඛ්‍යා count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (channel->manager->read_blk64)
		ආපසු (channel->manager->read_blk64)(channel, block,
						      count, data);

	නම් ((block >> 32) != 0)
		ආපසු EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ආපසු (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 පූර්ණ_සංඛ්‍යා count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (channel->manager->write_blk64)
		ආපසු (channel->manager->write_blk64)(channel, block,
						       count, data);

	නම් ((block >> 32) != 0)
		ආපසු EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ආපසු (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (channel->manager->discard)
		ආපසු (channel->manager->discard)(channel, block, count);

	ආපසු EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	නම් (channel->manager->zeroout)
		ආපසු (channel->manager->zeroout)(channel, block, count);

	ආපසු EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, පූර්ණ_සංඛ්‍යා count, void *ptr)
{
	size_t	size;

	නම් (count == 0)
		size = io->block_size;
	නැතහොත් නම් (count > 0)
		size = io->block_size * count;
	නැතහොත්
		size = -count;

	නම් (io->align > 0) {
		නම් ((unsigned) io->align > size)
			size = io->align;
		ආපසු ext2fs_get_memalign(size, io->align, ptr);
	} නැතහොත්
		ආපසු ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	නම් (!io->manager->cache_readahead)
		ආපසු EXT2_ET_OP_NOT_SUPPORTED;

	ආපසු io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned පූර්ණ_සංඛ්‍යා flock_flags)
{
	නම් (!io->manager->flock)
		ආපසු EXT2_ET_OP_NOT_SUPPORTED;

	ආපසු io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	නම් (!io->manager->flock)
		ආපසු EXT2_ET_OP_NOT_SUPPORTED;

	ආපසු io->manager->flock(io, 0);
}
