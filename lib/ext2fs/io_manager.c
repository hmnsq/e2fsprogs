/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (!opts)
		أرجع 0;

	إذا (!channel->manager->set_option)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	إذا (!options)
		أرجع EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		next = strchr(ptr, '&');
		إذا (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		إذا (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		إذا (retval)
			اكسر;
		ptr = next;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				صحيح گنتی, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, offset,
						    گنتی, data);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 صحيح گنتی, فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, block,
						      گنتی, data);

	إذا ((block >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (unsigned long) block,
					     گنتی, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 صحيح گنتی, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, block,
						       گنتی, data);

	إذا ((block >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (unsigned long) block,
					     گنتی, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, block, گنتی);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, block, گنتی);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح گنتی, فراغ *ptr)
{
	size_t	size;

	إذا (گنتی == 0)
		size = io->block_size;
	وإلا إذا (گنتی > 0)
		size = io->block_size * گنتی;
	وإلا
		size = -گنتی;

	إذا (io->align > 0) {
		إذا ((unsigned) io->align > size)
			size = io->align;
		أرجع ext2fs_get_memalign(size, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long گنتی)
{
	إذا (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, block, گنتی);
}

errcode_t io_channel_flock(io_channel io, unsigned صحيح flock_flags)
{
	إذا (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	إذا (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, 0);
}
