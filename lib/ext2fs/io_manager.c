/*
 * io_manager.c --- the I/O manager abstraction
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t io_channel_समुच्चयः_options(io_channel channel, ध्रुव वर्ण *opts)
{
	errcode_t retval = 0;
	वर्ण *अग्र, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (!opts)
		प्रत्यागम 0;

	यदि (!channel->manager->समुच्चयः_option)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	options = स्मृतिं_देहि(वाक्यदैर्घ्यम्(opts)+1);
	यदि (!options)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	वाक्यं_प्रतिलिख(options, opts);
	ptr = options;

	यावत् (ptr && *ptr) {
		अग्र = strchr(ptr, '&');
		यदि (अग्र)
			*अग्र++ = 0;

		arg = strchr(ptr, '=');
		यदि (arg)
			*arg++ = 0;

		retval = (channel->manager->समुच्चयः_option)(channel, ptr, arg);
		यदि (retval)
			भङ्ग;
		ptr = अग्र;
	}
	मुक्त(options);
	प्रत्यागम retval;
}

errcode_t io_channel_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_byte)
		प्रत्यागम channel->manager->write_byte(channel, विस्थापनम्,
						    गणनम्, दत्त);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
				 पूर्णाङ्क गणनम्, शून्य *दत्त)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->read_blk64)
		प्रत्यागम (channel->manager->read_blk64)(channel, खण्डम्,
						      गणनम्, दत्त);

	यदि ((खण्डम् >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->read_blk)(channel, (अचिह्नित दीर्घ) खण्डम्,
					     गणनम्, दत्त);
}

errcode_t io_channel_write_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
				 पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->write_blk64)
		प्रत्यागम (channel->manager->write_blk64)(channel, खण्डम्,
						       गणनम्, दत्त);

	यदि ((खण्डम् >> 32) != 0)
		प्रत्यागम EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	प्रत्यागम (channel->manager->write_blk)(channel, (अचिह्नित दीर्घ) खण्डम्,
					     गणनम्, दत्त);
}

errcode_t io_channel_discard(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			     अचिह्नित दीर्घ दीर्घ गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->discard)
		प्रत्यागम (channel->manager->discard)(channel, खण्डम्, गणनम्);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			     अचिह्नित दीर्घ दीर्घ गणनम्)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	यदि (channel->manager->zeroout)
		प्रत्यागम (channel->manager->zeroout)(channel, खण्डम्, गणनम्);

	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, पूर्णाङ्क गणनम्, शून्य *ptr)
{
	size_t	आकार;

	यदि (गणनम् == 0)
		आकार = io->खण्डम्_आकार;
	अन्यथा यदि (गणनम् > 0)
		आकार = io->खण्डम्_आकार * गणनम्;
	अन्यथा
		आकार = -गणनम्;

	यदि (io->align > 0) {
		यदि ((अचिह्नित) io->align > आकार)
			आकार = io->align;
		प्रत्यागम ext2fs_get_memalign(आकार, io->align, ptr);
	} अन्यथा
		प्रत्यागम ext2fs_get_mem(आकार, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, अचिह्नित दीर्घ दीर्घ खण्डम्,
				     अचिह्नित दीर्घ दीर्घ गणनम्)
{
	यदि (!io->manager->cache_readahead)
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;

	प्रत्यागम io->manager->cache_readahead(io, खण्डम्, गणनम्);
}

errcode_t io_channel_flock(io_channel io, अचिह्नित पूर्णाङ्क flock_flags)
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
