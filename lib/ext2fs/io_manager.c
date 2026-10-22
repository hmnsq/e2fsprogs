/*
 * io_manager.c --- the I/O manager abstraction
 */

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ধ্রুবক অক্ষর *opts)
{
	errcode_t retval = 0;
	অক্ষর *পরবর্তী, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (!opts)
		ফেরত 0;

	যদি (!channel->manager->set_option)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	যদি (!options)
		ফেরত EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	যখন (ptr && *ptr) {
		পরবর্তী = strchr(ptr, '&');
		যদি (পরবর্তী)
			*পরবর্তী++ = 0;

		arg = strchr(ptr, '=');
		যদি (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		যদি (retval)
			বিরতি;
		ptr = পরবর্তী;
	}
	মুক্ত(options);
	ফেরত retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *তথ্য)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->write_byte)
		ফেরত channel->manager->write_byte(channel, offset,
						    গুনতি, তথ্য);

	ফেরত EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 পূর্ণসংখ্যা গুনতি, শূন্য *তথ্য)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->read_blk64)
		ফেরত (channel->manager->read_blk64)(channel, block,
						      গুনতি, তথ্য);

	যদি ((block >> 32) != 0)
		ফেরত EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ফেরত (channel->manager->read_blk)(channel, (unsigned long) block,
					     গুনতি, তথ্য);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *তথ্য)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->write_blk64)
		ফেরত (channel->manager->write_blk64)(channel, block,
						       গুনতি, তথ্য);

	যদি ((block >> 32) != 0)
		ফেরত EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ফেরত (channel->manager->write_blk)(channel, (unsigned long) block,
					     গুনতি, তথ্য);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->discard)
		ফেরত (channel->manager->discard)(channel, block, গুনতি);

	ফেরত EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	যদি (channel->manager->zeroout)
		ফেরত (channel->manager->zeroout)(channel, block, গুনতি);

	ফেরত EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, পূর্ণসংখ্যা গুনতি, শূন্য *ptr)
{
	size_t	আকার_2;

	যদি (গুনতি == 0)
		আকার_2 = io->block_আকার;
	অন্যথা যদি (গুনতি > 0)
		আকার_2 = io->block_আকার * গুনতি;
	অন্যথা
		আকার_2 = -গুনতি;

	যদি (io->align > 0) {
		যদি ((unsigned) io->align > আকার_2)
			আকার_2 = io->align;
		ফেরত ext2fs_get_memalign(আকার_2, io->align, ptr);
	} অন্যথা
		ফেরত ext2fs_get_mem(আকার_2, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long গুনতি)
{
	যদি (!io->manager->cache_readahead)
		ফেরত EXT2_ET_OP_NOT_SUPPORTED;

	ফেরত io->manager->cache_readahead(io, block, গুনতি);
}

errcode_t io_channel_flock(io_channel io, unsigned পূর্ণসংখ্যা flock_flags)
{
	যদি (!io->manager->flock)
		ফেরত EXT2_ET_OP_NOT_SUPPORTED;

	ফেরত io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	যদি (!io->manager->flock)
		ফেরত EXT2_ET_OP_NOT_SUPPORTED;

	ফেরত io->manager->flock(io, 0);
}
