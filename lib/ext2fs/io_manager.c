/*
 * io_manager.c --- the I/O manager abstraction
 */

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, स्थिर अक्षर *opts)
{
	errcode_t retval = 0;
	अक्षर *अग्र, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (!opts)
		वापस 0;

	अगर (!channel->manager->set_option)
		वापस EXT2_ET_INVALID_ARGUMENT;

	options = दो_स्मृति(म_माप(opts)+1);
	अगर (!options)
		वापस EXT2_ET_NO_MEMORY;
	म_नकल(options, opts);
	ptr = options;

	जबतक (ptr && *ptr) {
		अग्र = म_अक्षर(ptr, '&');
		अगर (अग्र)
			*अग्र++ = 0;

		arg = म_अक्षर(ptr, '=');
		अगर (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		अगर (retval)
			अवरोध;
		ptr = अग्र;
	}
	मुक्त(options);
	वापस retval;
}

errcode_t io_channel_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णांक गिनती, स्थिर व्योम *तथ्य)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (channel->manager->write_byte)
		वापस channel->manager->write_byte(channel, offset,
						    गिनती, तथ्य);

	वापस EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णांक गिनती, व्योम *तथ्य)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (channel->manager->read_blk64)
		वापस (channel->manager->read_blk64)(channel, block,
						      गिनती, तथ्य);

	अगर ((block >> 32) != 0)
		वापस EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	वापस (channel->manager->read_blk)(channel, (अचिन्हित दीर्घ) block,
					     गिनती, तथ्य);
}

errcode_t io_channel_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				 पूर्णांक गिनती, स्थिर व्योम *तथ्य)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (channel->manager->write_blk64)
		वापस (channel->manager->write_blk64)(channel, block,
						       गिनती, तथ्य);

	अगर ((block >> 32) != 0)
		वापस EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	वापस (channel->manager->write_blk)(channel, (अचिन्हित दीर्घ) block,
					     गिनती, तथ्य);
}

errcode_t io_channel_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (channel->manager->discard)
		वापस (channel->manager->discard)(channel, block, गिनती);

	वापस EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	अगर (channel->manager->zeroout)
		वापस (channel->manager->zeroout)(channel, block, गिनती);

	वापस EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णांक गिनती, व्योम *ptr)
{
	माप_प्रकार	आकार;

	अगर (गिनती == 0)
		आकार = io->block_आकार;
	अन्यथा अगर (गिनती > 0)
		आकार = io->block_आकार * गिनती;
	अन्यथा
		आकार = -गिनती;

	अगर (io->align > 0) {
		अगर ((अचिन्हित) io->align > आकार)
			आकार = io->align;
		वापस ext2fs_get_memalign(आकार, io->align, ptr);
	} अन्यथा
		वापस ext2fs_get_mem(आकार, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ गिनती)
{
	अगर (!io->manager->cache_readahead)
		वापस EXT2_ET_OP_NOT_SUPPORTED;

	वापस io->manager->cache_readahead(io, block, गिनती);
}

errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णांक flock_flags)
{
	अगर (!io->manager->flock)
		वापस EXT2_ET_OP_NOT_SUPPORTED;

	वापस io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	अगर (!io->manager->flock)
		वापस EXT2_ET_OP_NOT_SUPPORTED;

	वापस io->manager->flock(io, 0);
}
