/*
 * io_manager.c --- the I/O manager abstraction
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ध्रुव वर्ण *opts)
{
	errcode_t retval = 0;
	वर्ण *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (!opts)
		प्रत्यागम 0;

	यदि (!channel->manager->set_option)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	options = दो_स्मृति(म_माप(opts)+1);
	यदि (!options)
		प्रत्यागम EXT2_ET_NO_MEMORY;
	म_नकल(options, opts);
	ptr = options;

	यावत् (ptr && *ptr) {
		next = म_अक्षर(ptr, '&');
		यदि (next)
			*next++ = 0;

		arg = म_अक्षर(ptr, '=');
		यदि (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		यदि (retval)
			भङ्ग;
		ptr = next;
	}
	मुक्त(options);
	प्रत्यागम retval;
}

errcode_t io_channel_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णाङ्क गिनती, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_byte)
		प्रत्यागम channel->manager->write_byte(channel, offset,
						    गिनती, data);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णाङ्क गिनती, शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->read_blk64)
		प्रत्यागम (channel->manager->read_blk64)(channel, block,
						      गिनती, data);

	यदि ((block >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->read_blk)(channel, (अचिन्हित दीर्घ) block,
					     गिनती, data);
}

errcode_t io_channel_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णाङ्क गिनती, ध्रुव शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_blk64)
		प्रत्यागम (channel->manager->write_blk64)(channel, block,
						       गिनती, data);

	यदि ((block >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->write_blk)(channel, (अचिन्हित दीर्घ) block,
					     गिनती, data);
}

errcode_t io_channel_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->discard)
		प्रत्यागम (channel->manager->discard)(channel, block, गिनती);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->zeroout)
		प्रत्यागम (channel->manager->zeroout)(channel, block, गिनती);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णाङ्क गिनती, शून्य *ptr)
{
	माप_प्रकार	size;

	यदि (गिनती == 0)
		size = io->block_size;
	अन्यथा यदि (गिनती > 0)
		size = io->block_size * गिनती;
	अन्यथा
		size = -गिनती;

	यदि (io->align > 0) {
		यदि ((अचिन्हित) io->align > size)
			size = io->align;
		प्रत्यागम ext2fs_get_memalign(size, io->align, ptr);
	} अन्यथा
		प्रत्यागम ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ गिनती)
{
	यदि (!io->manager->cache_readahead)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->cache_readahead(io, block, गिनती);
}

errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णाङ्क flock_flags)
{
	यदि (!io->manager->flock)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	यदि (!io->manager->flock)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->flock(io, 0);
}
