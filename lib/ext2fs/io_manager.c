/*
 * io_manager.c --- the I/O manager abstraction
 */

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, സ്ഥിരം അക്ഷരം *opts)
{
	errcode_t retval = 0;
	അക്ഷരം *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (!opts)
		തിരികെ 0;

	എങ്കിൽ (!channel->manager->set_option)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	എങ്കിൽ (!options)
		തിരികെ EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	ആകുമ്പോൾ (ptr && *ptr) {
		next = strchr(ptr, '&');
		എങ്കിൽ (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		എങ്കിൽ (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		എങ്കിൽ (retval)
			നിർത്തുക;
		ptr = next;
	}
	സ്വതന്ത്രം(options);
	തിരികെ retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (channel->manager->write_byte)
		തിരികെ channel->manager->write_byte(channel, offset,
						    എണ്ണം, data);

	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (channel->manager->read_blk64)
		തിരികെ (channel->manager->read_blk64)(channel, block,
						      എണ്ണം, data);

	എങ്കിൽ ((block >> 32) != 0)
		തിരികെ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	തിരികെ (channel->manager->read_blk)(channel, (unsigned long) block,
					     എണ്ണം, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (channel->manager->write_blk64)
		തിരികെ (channel->manager->write_blk64)(channel, block,
						       എണ്ണം, data);

	എങ്കിൽ ((block >> 32) != 0)
		തിരികെ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	തിരികെ (channel->manager->write_blk)(channel, (unsigned long) block,
					     എണ്ണം, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long എണ്ണം)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (channel->manager->discard)
		തിരികെ (channel->manager->discard)(channel, block, എണ്ണം);

	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long എണ്ണം)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	എങ്കിൽ (channel->manager->zeroout)
		തിരികെ (channel->manager->zeroout)(channel, block, എണ്ണം);

	തിരികെ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *ptr)
{
	size_t	size;

	എങ്കിൽ (എണ്ണം == 0)
		size = io->block_size;
	അല്ലെങ്കിൽ എങ്കിൽ (എണ്ണം > 0)
		size = io->block_size * എണ്ണം;
	അല്ലെങ്കിൽ
		size = -എണ്ണം;

	എങ്കിൽ (io->align > 0) {
		എങ്കിൽ ((unsigned) io->align > size)
			size = io->align;
		തിരികെ ext2fs_get_memalign(size, io->align, ptr);
	} അല്ലെങ്കിൽ
		തിരികെ ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long എണ്ണം)
{
	എങ്കിൽ (!io->manager->cache_readahead)
		തിരികെ EXT2_ET_OP_NOT_SUPPORTED;

	തിരികെ io->manager->cache_readahead(io, block, എണ്ണം);
}

errcode_t io_channel_flock(io_channel io, unsigned പൂർണ്ണസംഖ്യ flock_flags)
{
	എങ്കിൽ (!io->manager->flock)
		തിരികെ EXT2_ET_OP_NOT_SUPPORTED;

	തിരികെ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	എങ്കിൽ (!io->manager->flock)
		തിരികെ EXT2_ET_OP_NOT_SUPPORTED;

	തിരികെ io->manager->flock(io, 0);
}
