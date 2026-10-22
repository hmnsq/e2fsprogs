/*
 * io_manager.c --- the I/O manager abstraction
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ध्रुव वर्ण *opts)
{
	errcode_t retval = 0;
	वर्ण *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (!opts)
		पटिनिवत्त 0;

	सचे (!channel->manager->set_option)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	सचे (!options)
		पटिनिवत्त EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	याव (ptr && *ptr) {
		next = strchr(ptr, '&');
		सचे (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		सचे (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		सचे (retval)
			भङ्ग;
		ptr = next;
	}
	मुक्त(options);
	पटिनिवत्त retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (channel->manager->write_byte)
		पटिनिवत्त channel->manager->write_byte(channel, offset,
						    गणनम्, data);

	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 पूर्णाङ्क गणनम्, शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (channel->manager->read_blk64)
		पटिनिवत्त (channel->manager->read_blk64)(channel, block,
						      गणनम्, data);

	सचे ((block >> 32) != 0)
		पटिनिवत्त EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	पटिनिवत्त (channel->manager->read_blk)(channel, (unsigned long) block,
					     गणनम्, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 पूर्णाङ्क गणनम्, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (channel->manager->write_blk64)
		पटिनिवत्त (channel->manager->write_blk64)(channel, block,
						       गणनम्, data);

	सचे ((block >> 32) != 0)
		पटिनिवत्त EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	पटिनिवत्त (channel->manager->write_blk)(channel, (unsigned long) block,
					     गणनम्, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (channel->manager->discard)
		पटिनिवत्त (channel->manager->discard)(channel, block, गणनम्);

	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	सचे (channel->manager->zeroout)
		पटिनिवत्त (channel->manager->zeroout)(channel, block, गणनम्);

	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णाङ्क गणनम्, शून्य *ptr)
{
	size_t	size;

	सचे (गणनम् == 0)
		size = io->block_size;
	अञ्ञथा सचे (गणनम् > 0)
		size = io->block_size * गणनम्;
	अञ्ञथा
		size = -गणनम्;

	सचे (io->align > 0) {
		सचे ((unsigned) io->align > size)
			size = io->align;
		पटिनिवत्त ext2fs_get_memalign(size, io->align, ptr);
	} अञ्ञथा
		पटिनिवत्त ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long गणनम्)
{
	सचे (!io->manager->cache_readahead)
		पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;

	पटिनिवत्त io->manager->cache_readahead(io, block, गणनम्);
}

errcode_t io_channel_flock(io_channel io, unsigned पूर्णाङ्क flock_flags)
{
	सचे (!io->manager->flock)
		पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;

	पटिनिवत्त io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	सचे (!io->manager->flock)
		पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;

	पटिनिवत्त io->manager->flock(io, 0);
}
