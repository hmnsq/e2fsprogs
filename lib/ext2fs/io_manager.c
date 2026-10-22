/*
 * io_manager.c --- the I/O manager abstraction
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ប្រសិនបើ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ប្រសិនបើ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, const var_na *opts)
{
	errcode_t retval = 0;
	var_na *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (!opts)
		ត្រឡប់ 0;

	ប្រសិនបើ (!channel->manager->set_option)
		ត្រឡប់ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ប្រសិនបើ (!options)
		ត្រឡប់ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ខណៈពេល (ptr && *ptr) {
		next = strchr(ptr, '&');
		ប្រសិនបើ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ប្រសិនបើ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ប្រសិនបើ (retval)
			bha_nga;
		ptr = next;
	}
	free(options);
	ត្រឡប់ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ចំនួនគត់ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (channel->manager->write_byte)
		ត្រឡប់ channel->manager->write_byte(channel, offset,
						    count, data);

	ត្រឡប់ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ចំនួនគត់ count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (channel->manager->read_blk64)
		ត្រឡប់ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ប្រសិនបើ ((block >> 32) != 0)
		ត្រឡប់ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ត្រឡប់ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ចំនួនគត់ count, const void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (channel->manager->write_blk64)
		ត្រឡប់ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ប្រសិនបើ ((block >> 32) != 0)
		ត្រឡប់ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ត្រឡប់ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (channel->manager->discard)
		ត្រឡប់ (channel->manager->discard)(channel, block, count);

	ត្រឡប់ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ប្រសិនបើ (channel->manager->zeroout)
		ត្រឡប់ (channel->manager->zeroout)(channel, block, count);

	ត្រឡប់ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ចំនួនគត់ count, void *ptr)
{
	size_t	size;

	ប្រសិនបើ (count == 0)
		size = io->block_size;
	បើមិនដូច្នេះ ប្រសិនបើ (count > 0)
		size = io->block_size * count;
	បើមិនដូច្នេះ
		size = -count;

	ប្រសិនបើ (io->align > 0) {
		ប្រសិនបើ ((unsigned) io->align > size)
			size = io->align;
		ត្រឡប់ ext2fs_get_memalign(size, io->align, ptr);
	} បើមិនដូច្នេះ
		ត្រឡប់ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ប្រសិនបើ (!io->manager->cache_readahead)
		ត្រឡប់ EXT2_ET_OP_NOT_SUPPORTED;

	ត្រឡប់ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned ចំនួនគត់ flock_flags)
{
	ប្រសិនបើ (!io->manager->flock)
		ត្រឡប់ EXT2_ET_OP_NOT_SUPPORTED;

	ត្រឡប់ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ប្រសិនបើ (!io->manager->flock)
		ត្រឡប់ EXT2_ET_OP_NOT_SUPPORTED;

	ត្រឡប់ io->manager->flock(io, 0);
}
