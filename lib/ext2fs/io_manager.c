/*
 * io_manager.c --- the I/O manager abstraction
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, ಧ್ರುವ ವರ್ಣ *opts)
{
	errcode_t retval = 0;
	ವರ್ಣ *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (!opts)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!channel->manager->set_option)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	ಯದಿ (!options)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ಯಾವತ್ (ptr && *ptr) {
		next = strchr(ptr, '&');
		ಯದಿ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		ಯದಿ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		ಯದಿ (retval)
			ಭಙ್ಗ;
		ptr = next;
	}
	ಮುಕ್ತ(options);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (channel->manager->write_byte)
		ಪ್ರತ್ಯಾಗಮ channel->manager->write_byte(channel, offset,
						    ಎಣಿಕೆ, data);

	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (channel->manager->read_blk64)
		ಪ್ರತ್ಯಾಗಮ (channel->manager->read_blk64)(channel, block,
						      ಎಣಿಕೆ, data);

	ಯದಿ ((block >> 32) != 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ಪ್ರತ್ಯಾಗಮ (channel->manager->read_blk)(channel, (unsigned long) block,
					     ಎಣಿಕೆ, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (channel->manager->write_blk64)
		ಪ್ರತ್ಯಾಗಮ (channel->manager->write_blk64)(channel, block,
						       ಎಣಿಕೆ, data);

	ಯದಿ ((block >> 32) != 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ಪ್ರತ್ಯಾಗಮ (channel->manager->write_blk)(channel, (unsigned long) block,
					     ಎಣಿಕೆ, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long ಎಣಿಕೆ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (channel->manager->discard)
		ಪ್ರತ್ಯಾಗಮ (channel->manager->discard)(channel, block, ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long ಎಣಿಕೆ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಯದಿ (channel->manager->zeroout)
		ಪ್ರತ್ಯಾಗಮ (channel->manager->zeroout)(channel, block, ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *ptr)
{
	size_t	size;

	ಯದಿ (ಎಣಿಕೆ == 0)
		size = io->block_size;
	ಅನ್ಯಥಾ ಯದಿ (ಎಣಿಕೆ > 0)
		size = io->block_size * ಎಣಿಕೆ;
	ಅನ್ಯಥಾ
		size = -ಎಣಿಕೆ;

	ಯದಿ (io->align > 0) {
		ಯದಿ ((unsigned) io->align > size)
			size = io->align;
		ಪ್ರತ್ಯಾಗಮ ext2fs_get_memalign(size, io->align, ptr);
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long ಎಣಿಕೆ)
{
	ಯದಿ (!io->manager->cache_readahead)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_OP_NOT_SUPPORTED;

	ಪ್ರತ್ಯಾಗಮ io->manager->cache_readahead(io, block, ಎಣಿಕೆ);
}

errcode_t io_channel_flock(io_channel io, unsigned ಪೂರ್ಣಾಙ್ಕ flock_flags)
{
	ಯದಿ (!io->manager->flock)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_OP_NOT_SUPPORTED;

	ಪ್ರತ್ಯಾಗಮ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ಯದಿ (!io->manager->flock)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_OP_NOT_SUPPORTED;

	ಪ್ರತ್ಯಾಗಮ io->manager->flock(io, 0);
}
