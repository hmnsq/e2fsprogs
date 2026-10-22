/*
 * io_manager.c --- the I/O manager abstraction
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ثابت نویسه *opts)
{
	errcode_t retval = 0;
	نویسه *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (!opts)
		بازگشت 0;

	اگر (!channel->manager->set_option)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	اگر (!options)
		بازگشت EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	وقتی (ptr && *ptr) {
		next = strchr(ptr, '&');
		اگر (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		اگر (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		اگر (retval)
			بشکن;
		ptr = next;
	}
	آزاد(options);
	بازگشت retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				صحیح شمار, ثابت تهی *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->write_byte)
		بازگشت channel->manager->write_byte(channel, offset,
						    شمار, data);

	بازگشت EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 صحیح شمار, تهی *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->read_blk64)
		بازگشت (channel->manager->read_blk64)(channel, block,
						      شمار, data);

	اگر ((block >> 32) != 0)
		بازگشت EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	بازگشت (channel->manager->read_blk)(channel, (unsigned long) block,
					     شمار, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 صحیح شمار, ثابت تهی *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->write_blk64)
		بازگشت (channel->manager->write_blk64)(channel, block,
						       شمار, data);

	اگر ((block >> 32) != 0)
		بازگشت EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	بازگشت (channel->manager->write_blk)(channel, (unsigned long) block,
					     شمار, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long شمار)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->discard)
		بازگشت (channel->manager->discard)(channel, block, شمار);

	بازگشت EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long شمار)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->zeroout)
		بازگشت (channel->manager->zeroout)(channel, block, شمار);

	بازگشت EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحیح شمار, تهی *ptr)
{
	size_t	size;

	اگر (شمار == 0)
		size = io->block_size;
	وگرنه اگر (شمار > 0)
		size = io->block_size * شمار;
	وگرنه
		size = -شمار;

	اگر (io->align > 0) {
		اگر ((unsigned) io->align > size)
			size = io->align;
		بازگشت ext2fs_get_memalign(size, io->align, ptr);
	} وگرنه
		بازگشت ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long شمار)
{
	اگر (!io->manager->cache_readahead)
		بازگشت EXT2_ET_OP_NOT_SUPPORTED;

	بازگشت io->manager->cache_readahead(io, block, شمار);
}

errcode_t io_channel_flock(io_channel io, unsigned صحیح flock_flags)
{
	اگر (!io->manager->flock)
		بازگشت EXT2_ET_OP_NOT_SUPPORTED;

	بازگشت io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	اگر (!io->manager->flock)
		بازگشت EXT2_ET_OP_NOT_SUPPORTED;

	بازگشت io->manager->flock(io, 0);
}
