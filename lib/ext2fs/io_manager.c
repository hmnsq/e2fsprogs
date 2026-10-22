/*
 * io_manager.c --- the I/O manager abstraction
 */

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ଅଚଳ ଅକ୍ଷର *opts)
{
	errcode_t retval = 0;
	ଅକ୍ଷର *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (!opts)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!channel->manager->set_option)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ଯଦି (!options)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ଯେତେବେଳେ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ଯଦି (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ଯଦି (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ଯଦି (retval)
			ଭାଙ୍ଗ;
		ptr = next;
	}
	ମୁକ୍ତ(options);
	ଫେରନ୍ତୁ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (channel->manager->write_byte)
		ଫେରନ୍ତୁ channel->manager->write_byte(channel, offset,
						    ଗଣନା_2, data);

	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (channel->manager->read_blk64)
		ଫେରନ୍ତୁ (channel->manager->read_blk64)(channel, block,
						      ଗଣନା_2, data);

	ଯଦି ((block >> 32) != 0)
		ଫେରନ୍ତୁ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ଫେରନ୍ତୁ (channel->manager->read_blk)(channel, (unsigned long) block,
					     ଗଣନା_2, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (channel->manager->write_blk64)
		ଫେରନ୍ତୁ (channel->manager->write_blk64)(channel, block,
						       ଗଣନା_2, data);

	ଯଦି ((block >> 32) != 0)
		ଫେରନ୍ତୁ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ଫେରନ୍ତୁ (channel->manager->write_blk)(channel, (unsigned long) block,
					     ଗଣନା_2, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long ଗଣନା_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (channel->manager->discard)
		ଫେରନ୍ତୁ (channel->manager->discard)(channel, block, ଗଣନା_2);

	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long ଗଣନା_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ଯଦି (channel->manager->zeroout)
		ଫେରନ୍ତୁ (channel->manager->zeroout)(channel, block, ଗଣନା_2);

	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *ptr)
{
	size_t	size;

	ଯଦି (ଗଣନା_2 == 0)
		size = io->block_size;
	ଅନ୍ୟଥା ଯଦି (ଗଣନା_2 > 0)
		size = io->block_size * ଗଣନା_2;
	ଅନ୍ୟଥା
		size = -ଗଣନା_2;

	ଯଦି (io->align > 0) {
		ଯଦି ((unsigned) io->align > size)
			size = io->align;
		ଫେରନ୍ତୁ ext2fs_get_memalign(size, io->align, ptr);
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long ଗଣନା_2)
{
	ଯଦି (!io->manager->cache_readahead)
		ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;

	ଫେରନ୍ତୁ io->manager->cache_readahead(io, block, ଗଣନା_2);
}

errcode_t io_channel_flock(io_channel io, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flock_flags)
{
	ଯଦି (!io->manager->flock)
		ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;

	ଫେରନ୍ତୁ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ଯଦି (!io->manager->flock)
		ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;

	ଫେରନ୍ତୁ io->manager->flock(io, 0);
}
