/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (!opts)
		ܦܢܐ 0;

	ܐܢ (!channel->manager->set_option)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ܐܢ (!options)
		ܦܢܐ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ܟܡܐ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ܐܢ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ܐܢ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ܐܢ (retval)
			اكسر;
		ptr = next;
	}
	حرر(options);
	ܦܢܐ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				صحيح count, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (channel->manager->write_byte)
		ܦܢܐ channel->manager->write_byte(channel, offset,
						    count, data);

	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 صحيح count, فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (channel->manager->read_blk64)
		ܦܢܐ (channel->manager->read_blk64)(channel, block,
						      count, data);

	ܐܢ ((block >> 32) != 0)
		ܦܢܐ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ܦܢܐ (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 صحيح count, ثابت فراغ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (channel->manager->write_blk64)
		ܦܢܐ (channel->manager->write_blk64)(channel, block,
						       count, data);

	ܐܢ ((block >> 32) != 0)
		ܦܢܐ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ܦܢܐ (channel->manager->write_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (channel->manager->discard)
		ܦܢܐ (channel->manager->discard)(channel, block, count);

	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long count)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ܐܢ (channel->manager->zeroout)
		ܦܢܐ (channel->manager->zeroout)(channel, block, count);

	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح count, فراغ *ptr)
{
	size_t	size;

	ܐܢ (count == 0)
		size = io->block_size;
	ܐܚܪܢܐ ܐܢ (count > 0)
		size = io->block_size * count;
	ܐܚܪܢܐ
		size = -count;

	ܐܢ (io->align > 0) {
		ܐܢ ((unsigned) io->align > size)
			size = io->align;
		ܦܢܐ ext2fs_get_memalign(size, io->align, ptr);
	} ܐܚܪܢܐ
		ܦܢܐ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long count)
{
	ܐܢ (!io->manager->cache_readahead)
		ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;

	ܦܢܐ io->manager->cache_readahead(io, block, count);
}

errcode_t io_channel_flock(io_channel io, unsigned صحيح flock_flags)
{
	ܐܢ (!io->manager->flock)
		ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;

	ܦܢܐ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ܐܢ (!io->manager->flock)
		ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;

	ܦܢܐ io->manager->flock(io, 0);
}
