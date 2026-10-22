/*
 * io_manager.c --- the I/O manager abstraction
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

errcode_t io_channel_ਸੈੱਟ_options(io_channel channel, ਅਚਲ ਅੱਖਰ *opts)
{
	errcode_t retval = 0;
	ਅੱਖਰ *ਅਗਲਾ, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (!opts)
		ਵਾਪਸ 0;

	ਜੇ (!channel->manager->ਸੈੱਟ_option)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	options = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(opts)+1);
	ਜੇ (!options)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	strcpy(options, opts);
	ptr = options;

	ਜਦੋਂ (ptr && *ptr) {
		ਅਗਲਾ = strchr(ptr, '&');
		ਜੇ (ਅਗਲਾ)
			*ਅਗਲਾ++ = 0;

		arg = strchr(ptr, '=');
		ਜੇ (arg)
			*arg++ = 0;

		retval = (channel->manager->ਸੈੱਟ_option)(channel, ptr, arg);
		ਜੇ (retval)
			ਰੋਕੋ;
		ptr = ਅਗਲਾ;
	}
	ਮੁਕਤ(options);
	ਵਾਪਸ retval;
}

errcode_t io_channel_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (channel->manager->write_byte)
		ਵਾਪਸ channel->manager->write_byte(channel, ਵਿੱਥ,
						    ਗਿਣਤੀ_2, ਅੰਕੜੇ);

	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				 ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (channel->manager->read_blk64)
		ਵਾਪਸ (channel->manager->read_blk64)(channel, ਖੰਡ,
						      ਗਿਣਤੀ_2, ਅੰਕੜੇ);

	ਜੇ ((ਖੰਡ >> 32) != 0)
		ਵਾਪਸ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ਵਾਪਸ (channel->manager->read_blk)(channel, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਖੰਡ,
					     ਗਿਣਤੀ_2, ਅੰਕੜੇ);
}

errcode_t io_channel_write_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				 ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (channel->manager->write_blk64)
		ਵਾਪਸ (channel->manager->write_blk64)(channel, ਖੰਡ,
						       ਗਿਣਤੀ_2, ਅੰਕੜੇ);

	ਜੇ ((ਖੰਡ >> 32) != 0)
		ਵਾਪਸ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ਵਾਪਸ (channel->manager->write_blk)(channel, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਖੰਡ,
					     ਗਿਣਤੀ_2, ਅੰਕੜੇ);
}

errcode_t io_channel_discard(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (channel->manager->discard)
		ਵਾਪਸ (channel->manager->discard)(channel, ਖੰਡ, ਗਿਣਤੀ_2);

	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ਜੇ (channel->manager->zeroout)
		ਵਾਪਸ (channel->manager->zeroout)(channel, ਖੰਡ, ਗਿਣਤੀ_2);

	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ptr)
{
	size_t	ਮਾਪ;

	ਜੇ (ਗਿਣਤੀ_2 == 0)
		ਮਾਪ = io->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 > 0)
		ਮਾਪ = io->ਖੰਡ_ਮਾਪ * ਗਿਣਤੀ_2;
	ਨਹੀਂਤਾਂ
		ਮਾਪ = -ਗਿਣਤੀ_2;

	ਜੇ (io->align > 0) {
		ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) io->align > ਮਾਪ)
			ਮਾਪ = io->align;
		ਵਾਪਸ ext2fs_get_memalign(ਮਾਪ, io->align, ptr);
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ ext2fs_get_mem(ਮਾਪ, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਜੇ (!io->manager->cache_readahead)
		ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;

	ਵਾਪਸ io->manager->cache_readahead(io, ਖੰਡ, ਗਿਣਤੀ_2);
}

errcode_t io_channel_flock(io_channel io, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flock_flags)
{
	ਜੇ (!io->manager->flock)
		ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;

	ਵਾਪਸ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ਜੇ (!io->manager->flock)
		ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;

	ਵਾਪਸ io->manager->flock(io, 0);
}
