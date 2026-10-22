/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#အကယ်၍ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (!opts)
		ပြန်ပေး 0;

	အကယ်၍ (!channel->manager->set_option)
		ပြန်ပေး EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	အကယ်၍ (!options)
		ပြန်ပေး EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	နေစဉ် (ptr && *ptr) {
		next = strchr(ptr, '&');
		အကယ်၍ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		အကယ်၍ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		အကယ်၍ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ပြန်ပေး retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ကိန်းပြည့် count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (channel->manager->write_byte)
		ပြန်ပေး channel->manager->write_byte(channel, offset,
						    count, data);

	ပြန်ပေး EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ကိန်းပြည့် count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (channel->manager->read_blk64)
		ပြန်ပေး (channel->manager->read_blk64)(channel, block,
						      count, data);

	အကယ်၍ ((block >> 32) != 0)
		ပြန်ပေး EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ပြန်ပေး (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ကိန်းပြည့် count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (channel->manager->write_blk64)
		ပြန်ပေး (channel->manager->write_blk64)(channel, block,
						       count, data);

	အကယ်၍ ((block >> 32) != 0)
		ပြန်ပေး EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ပြန်ပေး (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (channel->manager->discard)
		ပြန်ပေး (channel->manager->discard)(channel, block, count);

	ပြန်ပေး EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	အကယ်၍ (channel->manager->zeroout)
		ပြန်ပေး (channel->manager->zeroout)(channel, block, count);

	ပြန်ပေး EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ကိန်းပြည့် count, void *ptr)
{
	size_t	size;

	အကယ်၍ (count == 0)
		size = io->block_size;
	မဟုတ်ပါက အကယ်၍ (count > 0)
		size = io->block_size * count;
	မဟုတ်ပါက
		size = -count;

	အကယ်၍ (io->align > 0) {
		အကယ်၍ ((unsigned) io->align > size)
			size = io->align;
		ပြန်ပေး ext2fs_get_memalign(size, io->align, ptr);
	} မဟုတ်ပါက
		ပြန်ပေး ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	အကယ်၍ (!io->manager->cache_readahead)
		ပြန်ပေး EXT2_ET_OP_NOT_SUPPORTED;

	ပြန်ပေး io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ကိန်းပြည့် flock_flags)
{
	အကယ်၍ (!io->manager->flock)
		ပြန်ပေး EXT2_ET_OP_NOT_SUPPORTED;

	ပြန်ပေး io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	အကယ်၍ (!io->manager->flock)
		ပြန်ပေး EXT2_ET_OP_NOT_SUPPORTED;

	ပြန်ပေး io->manager->flock(io, 0);
}
