/*
 * io_manager.c --- the I/O manager abstraction
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ध्रुव वर्ण *opts)
{
	errcode_t retval = 0;
	वर्ण *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (!opts)
		प्रत्यागम 0;

	जइ (!channel->manager->set_option)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	जइ (!options)
		प्रत्यागम EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	जाव (ptr && *ptr) {
		next = strchr(ptr, '&');
		जइ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		जइ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		जइ (retval)
			भङ्ग;
		ptr = next;
	}
	मुक्त(options);
	प्रत्यागम retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (channel->manager->write_byte)
		प्रत्यागम channel->manager->write_byte(channel, offset,
						    गणनम्, data);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 पूर्णाङ्क गणनम्, शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (channel->manager->read_blk64)
		प्रत्यागम (channel->manager->read_blk64)(channel, block,
						      गणनम्, data);

	जइ ((block >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->read_blk)(channel, (unsigned long) block,
					     गणनम्, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 पूर्णाङ्क गणनम्, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (channel->manager->write_blk64)
		प्रत्यागम (channel->manager->write_blk64)(channel, block,
						       गणनम्, data);

	जइ ((block >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->write_blk)(channel, (unsigned long) block,
					     गणनम्, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (channel->manager->discard)
		प्रत्यागम (channel->manager->discard)(channel, block, गणनम्);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जइ (channel->manager->zeroout)
		प्रत्यागम (channel->manager->zeroout)(channel, block, गणनम्);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णाङ्क गणनम्, शून्य *ptr)
{
	size_t	size;

	जइ (गणनम् == 0)
		size = io->block_size;
	अण्णहा जइ (गणनम् > 0)
		size = io->block_size * गणनम्;
	अण्णहा
		size = -गणनम्;

	जइ (io->align > 0) {
		जइ ((unsigned) io->align > size)
			size = io->align;
		प्रत्यागम ext2fs_get_memalign(size, io->align, ptr);
	} अण्णहा
		प्रत्यागम ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long गणनम्)
{
	जइ (!io->manager->cache_readahead)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->cache_readahead(io, block, गणनम्);
}

errcode_t io_channel_flock(io_channel io, unsigned पूर्णाङ्क flock_flags)
{
	जइ (!io->manager->flock)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	जइ (!io->manager->flock)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->flock(io, 0);
}
