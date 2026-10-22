/*
 * io_manager.c --- the I/O manager abstraction
 */

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, અચલ અક્ષર *opts)
{
	errcode_t retval = 0;
	અક્ષર *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (!opts)
		પાછા 0;

	જો (!channel->manager->set_option)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	જો (!options)
		પાછા EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	જ્યારે (ptr && *ptr) {
		next = strchr(ptr, '&');
		જો (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		જો (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		જો (retval)
			તોડો;
		ptr = next;
	}
	મુક્ત(options);
	પાછા retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (channel->manager->write_byte)
		પાછા channel->manager->write_byte(channel, offset,
						    ગણતરી, data);

	પાછા EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 પૂર્ણાંક ગણતરી, શૂન્ય *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (channel->manager->read_blk64)
		પાછા (channel->manager->read_blk64)(channel, block,
						      ગણતરી, data);

	જો ((block >> 32) != 0)
		પાછા EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	પાછા (channel->manager->read_blk)(channel, (unsigned long) block,
					     ગણતરી, data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (channel->manager->write_blk64)
		પાછા (channel->manager->write_blk64)(channel, block,
						       ગણતરી, data);

	જો ((block >> 32) != 0)
		પાછા EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	પાછા (channel->manager->write_blk)(channel, (unsigned long) block,
					     ગણતરી, data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long ગણતરી)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (channel->manager->discard)
		પાછા (channel->manager->discard)(channel, block, ગણતરી);

	પાછા EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long ગણતરી)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	જો (channel->manager->zeroout)
		પાછા (channel->manager->zeroout)(channel, block, ગણતરી);

	પાછા EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, પૂર્ણાંક ગણતરી, શૂન્ય *ptr)
{
	size_t	size;

	જો (ગણતરી == 0)
		size = io->block_size;
	નહીંતર જો (ગણતરી > 0)
		size = io->block_size * ગણતરી;
	નહીંતર
		size = -ગણતરી;

	જો (io->align > 0) {
		જો ((unsigned) io->align > size)
			size = io->align;
		પાછા ext2fs_get_memalign(size, io->align, ptr);
	} નહીંતર
		પાછા ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long ગણતરી)
{
	જો (!io->manager->cache_readahead)
		પાછા EXT2_ET_OP_NOT_SUPPORTED;

	પાછા io->manager->cache_readahead(io, block, ગણતરી);
}

errcode_t io_channel_flock(io_channel io, unsigned પૂર્ણાંક flock_flags)
{
	જો (!io->manager->flock)
		પાછા EXT2_ET_OP_NOT_SUPPORTED;

	પાછા io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	જો (!io->manager->flock)
		પાછા EXT2_ET_OP_NOT_SUPPORTED;

	પાછા io->manager->flock(io, 0);
}
