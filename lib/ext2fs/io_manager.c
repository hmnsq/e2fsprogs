/*
 * io_manager.c --- the I/O manager abstraction
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, स्थिरांक अक्षर *opts)
{
	errcode_t retval = 0;
	अक्षर *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (!opts)
		परत 0;

	जर (!channel->manager->set_option)
		परत EXT2_ET_INVALID_ARGUMENT;

	options = दो_स्मृति(म_माप(opts)+1);
	जर (!options)
		परत EXT2_ET_NO_MEMORY;
	म_नकल(options, opts);
	ptr = options;

	पर्यंत (ptr && *ptr) {
		next = म_अक्षर(ptr, '&');
		जर (next)
			*next++ = 0;

		arg = म_अक्षर(ptr, '=');
		जर (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		जर (retval)
			थांबा;
		ptr = next;
	}
	मुक्त(options);
	परत retval;
}

errcode_t io_channel_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णांक मोजणी, स्थिरांक शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (channel->manager->write_byte)
		परत channel->manager->write_byte(channel, offset,
						    मोजणी, data);

	परत EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णांक मोजणी, शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (channel->manager->read_blk64)
		परत (channel->manager->read_blk64)(channel, block,
						      मोजणी, data);

	जर ((block >> 32) != 0)
		परत EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	परत (channel->manager->read_blk)(channel, (अचिन्हित दीर्घ) block,
					     मोजणी, data);
}

errcode_t io_channel_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णांक मोजणी, स्थिरांक शून्य *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (channel->manager->write_blk64)
		परत (channel->manager->write_blk64)(channel, block,
						       मोजणी, data);

	जर ((block >> 32) != 0)
		परत EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	परत (channel->manager->write_blk)(channel, (अचिन्हित दीर्घ) block,
					     मोजणी, data);
}

errcode_t io_channel_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ मोजणी)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (channel->manager->discard)
		परत (channel->manager->discard)(channel, block, मोजणी);

	परत EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ मोजणी)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	जर (channel->manager->zeroout)
		परत (channel->manager->zeroout)(channel, block, मोजणी);

	परत EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णांक मोजणी, शून्य *ptr)
{
	माप_प्रकार	size;

	जर (मोजणी == 0)
		size = io->block_size;
	अन्यथा जर (मोजणी > 0)
		size = io->block_size * मोजणी;
	अन्यथा
		size = -मोजणी;

	जर (io->align > 0) {
		जर ((अचिन्हित) io->align > size)
			size = io->align;
		परत ext2fs_get_memalign(size, io->align, ptr);
	} अन्यथा
		परत ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ मोजणी)
{
	जर (!io->manager->cache_readahead)
		परत EXT2_ET_OP_NOT_SUPPORTED;

	परत io->manager->cache_readahead(io, block, मोजणी);
}

errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णांक flock_flags)
{
	जर (!io->manager->flock)
		परत EXT2_ET_OP_NOT_SUPPORTED;

	परत io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	जर (!io->manager->flock)
		परत EXT2_ET_OP_NOT_SUPPORTED;

	परत io->manager->flock(io, 0);
}
