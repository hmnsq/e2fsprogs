/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const char *opts)
{
	errcode_t retval = 0;
	char *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (!opts)
		trả_về 0;

	nếu (!channel->manager->set_option)
		trả_về EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	nếu (!options)
		trả_về EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	trong_khi (ptr && *ptr) {
		next = strchr(ptr, '&');
		nếu (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		nếu (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		nếu (retval)
			break;
		ptr = next;
	}
	free(options);
	trả_về retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				số_nguyên count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (channel->manager->write_byte)
		trả_về channel->manager->write_byte(channel, offset,
						    count, data);

	trả_về EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 số_nguyên count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (channel->manager->read_blk64)
		trả_về (channel->manager->read_blk64)(channel, block,
						      count, data);

	nếu ((block >> 32) != 0)
		trả_về EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	trả_về (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 số_nguyên count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (channel->manager->write_blk64)
		trả_về (channel->manager->write_blk64)(channel, block,
						       count, data);

	nếu ((block >> 32) != 0)
		trả_về EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	trả_về (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (channel->manager->discard)
		trả_về (channel->manager->discard)(channel, block, count);

	trả_về EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	nếu (channel->manager->zeroout)
		trả_về (channel->manager->zeroout)(channel, block, count);

	trả_về EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, số_nguyên count, void *ptr)
{
	size_t	size;

	nếu (count == 0)
		size = io->block_size;
	không_thì nếu (count > 0)
		size = io->block_size * count;
	không_thì
		size = -count;

	nếu (io->align > 0) {
		nếu ((unsigned) io->align > size)
			size = io->align;
		trả_về ext2fs_get_memalign(size, io->align, ptr);
	} không_thì
		trả_về ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	nếu (!io->manager->cache_readahead)
		trả_về EXT2_ET_OP_NOT_SUPPORTED;

	trả_về io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned số_nguyên flock_flags)
{
	nếu (!io->manager->flock)
		trả_về EXT2_ET_OP_NOT_SUPPORTED;

	trả_về io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	nếu (!io->manager->flock)
		trả_về EXT2_ET_OP_NOT_SUPPORTED;

	trả_về io->manager->flock(io, 0);
}
