/*
 * io_manager.c --- the I/O manager abstraction
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ধ্রুৱ ৱর্ণ *opts)
{
	errcode_t retval = 0;
	ৱর্ণ *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (!opts)
		ঘূৰাই 0;

	যদি (!channel->manager->set_option)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	যদি (!options)
		ঘূৰাই EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	যেতিয়ালৈ (ptr && *ptr) {
		next = strchr(ptr, '&');
		যদি (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		যদি (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		যদি (retval)
			ভঙ্গ;
		ptr = next;
	}
	মুক্ত(options);
	ঘূৰাই retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->write_byte)
		ঘূৰাই channel->manager->write_byte(channel, offset,
						    গুনতি, data);

	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 পূর্ণাঙ্ক গুনতি, শূন্য *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->read_blk64)
		ঘূৰাই (channel->manager->read_blk64)(channel, block,
						      গুনতি, data);

	যদি ((block >> 32) != 0)
		ঘূৰাই EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ঘূৰাই (channel->manager->read_blk)(channel, (unsigned long) block,
					     গুনতি, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->write_blk64)
		ঘূৰাই (channel->manager->write_blk64)(channel, block,
						       গুনতি, data);

	যদি ((block >> 32) != 0)
		ঘূৰাই EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ঘূৰাই (channel->manager->write_blk)(channel, (unsigned long) block,
					     গুনতি, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->discard)
		ঘূৰাই (channel->manager->discard)(channel, block, গুনতি);

	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->zeroout)
		ঘূৰাই (channel->manager->zeroout)(channel, block, গুনতি);

	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, পূর্ণাঙ্ক গুনতি, শূন্য *ptr)
{
	size_t	size;

	যদি (গুনতি == 0)
		size = io->block_size;
	অন্যথা যদি (গুনতি > 0)
		size = io->block_size * গুনতি;
	অন্যথা
		size = -গুনতি;

	যদি (io->align > 0) {
		যদি ((unsigned) io->align > size)
			size = io->align;
		ঘূৰাই ext2fs_get_memalign(size, io->align, ptr);
	} অন্যথা
		ঘূৰাই ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long গুনতি)
{
	যদি (!io->manager->cache_readahead)
		ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;

	ঘূৰাই io->manager->cache_readahead(io, block, গুনতি);
}

errcode_t io_channel_flock(io_channel io, unsigned পূর্ণাঙ্ক flock_flags)
{
	যদি (!io->manager->flock)
		ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;

	ঘূৰাই io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	যদি (!io->manager->flock)
		ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;

	ঘূৰাই io->manager->flock(io, 0);
}
