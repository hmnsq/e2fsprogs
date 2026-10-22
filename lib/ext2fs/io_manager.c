/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ถ้า HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ถ้า HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ถ้า HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (!opts)
		คืน 0;

	ถ้า (!channel->manager->set_option)
		คืน EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ถ้า (!options)
		คืน EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ขณะ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ถ้า (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ถ้า (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ถ้า (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	คืน retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				จำนวนเต็ม count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (channel->manager->write_byte)
		คืน channel->manager->write_byte(channel, offset,
						    count, data);

	คืน EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 จำนวนเต็ม count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (channel->manager->read_blk64)
		คืน (channel->manager->read_blk64)(channel, block,
						      count, data);

	ถ้า ((block >> 32) != 0)
		คืน EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	คืน (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 จำนวนเต็ม count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (channel->manager->write_blk64)
		คืน (channel->manager->write_blk64)(channel, block,
						       count, data);

	ถ้า ((block >> 32) != 0)
		คืน EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	คืน (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (channel->manager->discard)
		คืน (channel->manager->discard)(channel, block, count);

	คืน EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ถ้า (channel->manager->zeroout)
		คืน (channel->manager->zeroout)(channel, block, count);

	คืน EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, จำนวนเต็ม count, void *ptr)
{
	size_t	size;

	ถ้า (count == 0)
		size = io->block_size;
	มิฉะนั้น ถ้า (count > 0)
		size = io->block_size * count;
	มิฉะนั้น
		size = -count;

	ถ้า (io->align > 0) {
		ถ้า ((unsigned) io->align > size)
			size = io->align;
		คืน ext2fs_get_memalign(size, io->align, ptr);
	} มิฉะนั้น
		คืน ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ถ้า (!io->manager->cache_readahead)
		คืน EXT2_ET_OP_NOT_SUPPORTED;

	คืน io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned จำนวนเต็ม flock_flags)
{
	ถ้า (!io->manager->flock)
		คืน EXT2_ET_OP_NOT_SUPPORTED;

	คืน io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ถ้า (!io->manager->flock)
		คืน EXT2_ET_OP_NOT_SUPPORTED;

	คืน io->manager->flock(io, 0);
}
