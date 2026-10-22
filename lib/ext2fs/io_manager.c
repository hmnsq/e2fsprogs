/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#𐤀𐤌 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#𐤀𐤌 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (!opts)
		أرجع 0;

	𐤀𐤌 (!channel->manager->set_option)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	𐤀𐤌 (!options)
		أرجع EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		next = strchr(ptr, '&');
		𐤀𐤌 (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		𐤀𐤌 (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		𐤀𐤌 (retval)
			اكسر;
		ptr = next;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				صحيح count, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, offset,
						    count, data);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 صحيح count, فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, block,
						      count, data);

	𐤀𐤌 ((block >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 صحيح count, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, block,
						       count, data);

	𐤀𐤌 ((block >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, block, count);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	𐤀𐤌 (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, block, count);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح count, فراغ *ptr)
{
	size_t	size;

	𐤀𐤌 (count == 0)
		size = io->block_size;
	وإلا 𐤀𐤌 (count > 0)
		size = io->block_size * count;
	وإلا
		size = -count;

	𐤀𐤌 (io->align > 0) {
		𐤀𐤌 ((unsigned) io->align > size)
			size = io->align;
		أرجع ext2fs_get_memalign(size, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	𐤀𐤌 (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned صحيح flock_flags)
{
	𐤀𐤌 (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	𐤀𐤌 (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, 0);
}
