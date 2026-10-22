/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (!opts)
		ສົ່ງຄືນ 0;

	ຖ້າ (!channel->manager->set_option)
		ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ຖ້າ (!options)
		ສົ່ງຄືນ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ໃນຂະນະທີ່ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ຖ້າ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ຖ້າ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ຖ້າ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ສົ່ງຄືນ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ຈຳນວນເຕັມ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (channel->manager->write_byte)
		ສົ່ງຄືນ channel->manager->write_byte(channel, offset,
						    count, data);

	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ຈຳນວນເຕັມ count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (channel->manager->read_blk64)
		ສົ່ງຄືນ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ຖ້າ ((block >> 32) != 0)
		ສົ່ງຄືນ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ສົ່ງຄືນ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ຈຳນວນເຕັມ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (channel->manager->write_blk64)
		ສົ່ງຄືນ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ຖ້າ ((block >> 32) != 0)
		ສົ່ງຄືນ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ສົ່ງຄືນ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (channel->manager->discard)
		ສົ່ງຄືນ (channel->manager->discard)(channel, block, count);

	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ຖ້າ (channel->manager->zeroout)
		ສົ່ງຄືນ (channel->manager->zeroout)(channel, block, count);

	ສົ່ງຄືນ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ຈຳນວນເຕັມ count, void *ptr)
{
	size_t	size;

	ຖ້າ (count == 0)
		size = io->block_size;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (count > 0)
		size = io->block_size * count;
	ບໍ່ດັ່ງນັ້ນ
		size = -count;

	ຖ້າ (io->align > 0) {
		ຖ້າ ((unsigned) io->align > size)
			size = io->align;
		ສົ່ງຄືນ ext2fs_get_memalign(size, io->align, ptr);
	} ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ຖ້າ (!io->manager->cache_readahead)
		ສົ່ງຄືນ EXT2_ET_OP_NOT_SUPPORTED;

	ສົ່ງຄືນ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ຈຳນວນເຕັມ flock_flags)
{
	ຖ້າ (!io->manager->flock)
		ສົ່ງຄືນ EXT2_ET_OP_NOT_SUPPORTED;

	ສົ່ງຄືນ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ຖ້າ (!io->manager->flock)
		ສົ່ງຄືນ EXT2_ET_OP_NOT_SUPPORTED;

	ສົ່ງຄືນ io->manager->flock(io, 0);
}
