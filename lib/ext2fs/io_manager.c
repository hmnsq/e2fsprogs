/*
 * io_manager.c --- the I/O manager abstraction
 */

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

errcode_t io_channel_set_options(io_channel channel, மாறிலி எழுத்து *opts)
{
	errcode_t retval = 0;
	எழுத்து *next, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (!opts)
		திரும்பு 0;

	எனில் (!channel->manager->set_option)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	options = malloc(strlen(opts)+1);
	எனில் (!options)
		திரும்பு EXT2_ET_NO_MEMORY;
	strcpy(options, opts);
	ptr = options;

	போது (ptr && *ptr) {
		next = strchr(ptr, '&');
		எனில் (next)
			*next++ = 0;

		arg = strchr(ptr, '=');
		எனில் (arg)
			*arg++ = 0;

		retval = (channel->manager->set_option)(channel, ptr, arg);
		எனில் (retval)
			நிறுத்து;
		ptr = next;
	}
	விடுவி(options);
	திரும்பு retval;
}

errcode_t io_channel_write_byte(io_channel channel, unsigned long offset,
				முழு மொத்தம், மாறிலி வெறுமை *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (channel->manager->write_byte)
		திரும்பு channel->manager->write_byte(channel, offset,
						    மொத்தம், data);

	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 முழு மொத்தம், வெறுமை *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (channel->manager->read_blk64)
		திரும்பு (channel->manager->read_blk64)(channel, block,
						      மொத்தம், data);

	எனில் ((block >> 32) != 0)
		திரும்பு EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	திரும்பு (channel->manager->read_blk)(channel, (unsigned long) block,
					     மொத்தம், data);
}

errcode_t io_channel_write_blk64(io_channel channel, unsigned long long block,
				 முழு மொத்தம், மாறிலி வெறுமை *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (channel->manager->write_blk64)
		திரும்பு (channel->manager->write_blk64)(channel, block,
						       மொத்தம், data);

	எனில் ((block >> 32) != 0)
		திரும்பு EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	திரும்பு (channel->manager->write_blk)(channel, (unsigned long) block,
					     மொத்தம், data);
}

errcode_t io_channel_discard(io_channel channel, unsigned long long block,
			     unsigned long long மொத்தம்)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (channel->manager->discard)
		திரும்பு (channel->manager->discard)(channel, block, மொத்தம்);

	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, unsigned long long block,
			     unsigned long long மொத்தம்)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	எனில் (channel->manager->zeroout)
		திரும்பு (channel->manager->zeroout)(channel, block, மொத்தம்);

	திரும்பு EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, முழு மொத்தம், வெறுமை *ptr)
{
	size_t	size;

	எனில் (மொத்தம் == 0)
		size = io->block_size;
	இல்லையெனில் எனில் (மொத்தம் > 0)
		size = io->block_size * மொத்தம்;
	இல்லையெனில்
		size = -மொத்தம்;

	எனில் (io->align > 0) {
		எனில் ((unsigned) io->align > size)
			size = io->align;
		திரும்பு ext2fs_get_memalign(size, io->align, ptr);
	} இல்லையெனில்
		திரும்பு ext2fs_get_mem(size, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, unsigned long long block,
				     unsigned long long மொத்தம்)
{
	எனில் (!io->manager->cache_readahead)
		திரும்பு EXT2_ET_OP_NOT_SUPPORTED;

	திரும்பு io->manager->cache_readahead(io, block, மொத்தம்);
}

errcode_t io_channel_flock(io_channel io, unsigned முழு flock_flags)
{
	எனில் (!io->manager->flock)
		திரும்பு EXT2_ET_OP_NOT_SUPPORTED;

	திரும்பு io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	எனில் (!io->manager->flock)
		திரும்பு EXT2_ET_OP_NOT_SUPPORTED;

	திரும்பு io->manager->flock(io, 0);
}
