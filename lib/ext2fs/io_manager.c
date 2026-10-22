/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ပေႃး HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ပေႃး HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (!opts)
		ပွၵ်ႈ 0;

	ပေႃး (!channel->manager->set_option)
		ပွၵ်ႈ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ပေႃး (!options)
		ပွၵ်ႈ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ၶၢဝ်း (ptr && *ptr) {
		next = strchr(ptr, '&');
		ပေႃး (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ပေႃး (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ပေႃး (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ပွၵ်ႈ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (channel->manager->write_byte)
		ပွၵ်ႈ channel->manager->write_byte(channel, offset,
						    count, data);

	ပွၵ်ႈ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (channel->manager->read_blk64)
		ပွၵ်ႈ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ပေႃး ((block >> 32) != 0)
		ပွၵ်ႈ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ပွၵ်ႈ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 puur_naa_nka count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (channel->manager->write_blk64)
		ပွၵ်ႈ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ပေႃး ((block >> 32) != 0)
		ပွၵ်ႈ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ပွၵ်ႈ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (channel->manager->discard)
		ပွၵ်ႈ (channel->manager->discard)(channel, block, count);

	ပွၵ်ႈ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ပေႃး (channel->manager->zeroout)
		ပွၵ်ႈ (channel->manager->zeroout)(channel, block, count);

	ပွၵ်ႈ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, puur_naa_nka count, void *ptr)
{
	size_t	size;

	ပေႃး (count == 0)
		size = io->block_size;
	ပေႃးဢမ်ႇ ပေႃး (count > 0)
		size = io->block_size * count;
	ပေႃးဢမ်ႇ
		size = -count;

	ပေႃး (io->align > 0) {
		ပေႃး ((unsigned) io->align > size)
			size = io->align;
		ပွၵ်ႈ ext2fs_get_memalign(size, io->align, ptr);
	} ပေႃးဢမ်ႇ
		ပွၵ်ႈ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ပေႃး (!io->manager->cache_readahead)
		ပွၵ်ႈ EXT2_ET_OP_NOT_SUPPORTED;

	ပွၵ်ႈ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned puur_naa_nka flock_flags)
{
	ပေႃး (!io->manager->flock)
		ပွၵ်ႈ EXT2_ET_OP_NOT_SUPPORTED;

	ပွၵ်ႈ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ပေႃး (!io->manager->flock)
		ပွၵ်ႈ EXT2_ET_OP_NOT_SUPPORTED;

	ပွၵ်ႈ io->manager->flock(io, 0);
}
