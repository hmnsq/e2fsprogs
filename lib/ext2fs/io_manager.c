/*
 * io_manager.c --- the I/O manager abstraction
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

errcode_t io_channel_సమితి_options(io_channel channel, స్థిరరాశి అక్షరం *opts)
{
	errcode_t retval = 0;
	అక్షరం *తదుపరి, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (!opts)
		తిరిగి 0;

	ఒకవేళ (!channel->manager->సమితి_option)
		తిరిగి EXT2_ET_INVALID_వాదం;

	options = జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(opts)+1);
	ఒకవేళ (!options)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	strcpy(options, opts);
	ptr = options;

	అయితే (ptr && *ptr) {
		తదుపరి = strchr(ptr, '&');
		ఒకవేళ (తదుపరి)
			*తదుపరి++ = 0;

		arg = strchr(ptr, '=');
		ఒకవేళ (arg)
			*arg++ = 0;

		retval = (channel->manager->సమితి_option)(channel, ptr, arg);
		ఒకవేళ (retval)
			ఆపు;
		ptr = తదుపరి;
	}
	విడుదల(options);
	తిరిగి retval;
}

errcode_t io_channel_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (channel->manager->write_byte)
		తిరిగి channel->manager->write_byte(channel, స్థానభ్రంశం,
						    లెక్క, దత్తాంశం);

	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				 పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (channel->manager->read_blk64)
		తిరిగి (channel->manager->read_blk64)(channel, ఖండం,
						      లెక్క, దత్తాంశం);

	ఒకవేళ ((ఖండం >> 32) != 0)
		తిరిగి EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	తిరిగి (channel->manager->read_blk)(channel, (గుర్తులేని దీర్ఘ) ఖండం,
					     లెక్క, దత్తాంశం);
}

errcode_t io_channel_write_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				 పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (channel->manager->write_blk64)
		తిరిగి (channel->manager->write_blk64)(channel, ఖండం,
						       లెక్క, దత్తాంశం);

	ఒకవేళ ((ఖండం >> 32) != 0)
		తిరిగి EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	తిరిగి (channel->manager->write_blk)(channel, (గుర్తులేని దీర్ఘ) ఖండం,
					     లెక్క, దత్తాంశం);
}

errcode_t io_channel_discard(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (channel->manager->discard)
		తిరిగి (channel->manager->discard)(channel, ఖండం, లెక్క);

	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ఒకవేళ (channel->manager->zeroout)
		తిరిగి (channel->manager->zeroout)(channel, ఖండం, లెక్క);

	తిరిగి EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, పూర్ణాంకం లెక్క, శూన్య *ptr)
{
	size_t	కొలత;

	ఒకవేళ (లెక్క == 0)
		కొలత = io->ఖండం_కొలత;
	లేకపోతే ఒకవేళ (లెక్క > 0)
		కొలత = io->ఖండం_కొలత * లెక్క;
	లేకపోతే
		కొలత = -లెక్క;

	ఒకవేళ (io->align > 0) {
		ఒకవేళ ((గుర్తులేని) io->align > కొలత)
			కొలత = io->align;
		తిరిగి ext2fs_get_memalign(కొలత, io->align, ptr);
	} లేకపోతే
		తిరిగి ext2fs_get_mem(కొలత, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	ఒకవేళ (!io->manager->cache_readahead)
		తిరిగి EXT2_ET_OP_NOT_SUPPORTED;

	తిరిగి io->manager->cache_readahead(io, ఖండం, లెక్క);
}

errcode_t io_channel_flock(io_channel io, గుర్తులేని పూర్ణాంకం flock_flags)
{
	ఒకవేళ (!io->manager->flock)
		తిరిగి EXT2_ET_OP_NOT_SUPPORTED;

	తిరిగి io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ఒకవేళ (!io->manager->flock)
		తిరిగి EXT2_ET_OP_NOT_SUPPORTED;

	తిరిగి io->manager->flock(io, 0);
}
