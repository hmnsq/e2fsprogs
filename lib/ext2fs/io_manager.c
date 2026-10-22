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

errcode_t io_channel_set_options(io_channel channel, अचर वर्ण *opts)
{
	errcode_t retval = 0;
	वर्ण *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (!opts)
		फर्काऊ 0;

	यदि (!channel->manager->set_option)
		फर्काऊ EXT2_ET_INVALID_ARGUMENT;

	options = दो_स्मृति(म_माप(opts)+1);
	यदि (!options)
		फर्काऊ EXT2_ET_NO_MEMORY;
	म_नकल(options, opts);
	ptr = options;

	जबसम्म (ptr && *ptr) {
		next = म_अक्षर(ptr, '&');
		यदि (next)
			*next++ = 0;

		arg = म_अक्षर(ptr, '=');
		यदि (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		यदि (retval)
			रोक;
		ptr = next;
	}
	मुक्त(options);
	फर्काऊ retval;
}

errcode_t io_channel_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णाङ्क गन्ती, अचर शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_byte)
		फर्काऊ channel->manager->write_byte(channel, offset,
						    गन्ती, data);

	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णाङ्क गन्ती, शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->read_blk64)
		फर्काऊ (channel->manager->read_blk64)(channel, block,
						      गन्ती, data);

	यदि ((block >> 32) != 0)
		फर्काऊ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	फर्काऊ (channel->manager->read_blk)(channel, (अचिन्हित दीर्घ) block,
					     गन्ती, data);
}

errcode_t io_channel_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णाङ्क गन्ती, अचर शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_blk64)
		फर्काऊ (channel->manager->write_blk64)(channel, block,
						       गन्ती, data);

	यदि ((block >> 32) != 0)
		फर्काऊ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	फर्काऊ (channel->manager->write_blk)(channel, (अचिन्हित दीर्घ) block,
					     गन्ती, data);
}

errcode_t io_channel_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गन्ती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->discard)
		फर्काऊ (channel->manager->discard)(channel, block, गन्ती);

	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गन्ती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->zeroout)
		फर्काऊ (channel->manager->zeroout)(channel, block, गन्ती);

	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णाङ्क गन्ती, शून्य *ptr)
{
	माप_प्रकार	size;

	यदि (गन्ती == 0)
		size = io->block_size;
	अन्यथा यदि (गन्ती > 0)
		size = io->block_size * गन्ती;
	अन्यथा
		size = -गन्ती;

	यदि (io->align > 0) {
		यदि ((अचिन्हित) io->align > size)
			size = io->align;
		फर्काऊ ext2fs_get_memalign(size, io->align, ptr);
	} अन्यथा
		फर्काऊ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ गन्ती)
{
	यदि (!io->manager->cache_readahead)
		फर्काऊ EXT2_ET_OP_NOT_SUPPORTED;

	फर्काऊ io->manager->cache_readahead(io, block, गन्ती);
}

errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णाङ्क flock_flags)
{
	यदि (!io->manager->flock)
		फर्काऊ EXT2_ET_OP_NOT_SUPPORTED;

	फर्काऊ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	यदि (!io->manager->flock)
		फर्काऊ EXT2_ET_OP_NOT_SUPPORTED;

	फर्काऊ io->manager->flock(io, 0);
}
