/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_سيٽ_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *ايندڙ, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (!opts)
		أرجع 0;

	إذا (!channel->manager->سيٽ_option)
		أرجع EXT2_ET_INVALID_دليل;

	options = يادگيري_ڏيو(لڙي_ڊيگهه(opts)+1);
	إذا (!options)
		أرجع EXT2_ET_NO_يادگيري;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		ايندڙ = strchr(ptr, '&');
		إذا (ايندڙ)
			*ايندڙ++ = 0;

		arg = strchr(ptr, '=');
		إذا (arg)
			*arg++ = 0;

		retval = (channel->manager->سيٽ_option)(channel, ptr, arg);
		إذا (retval)
			اكسر;
		ptr = ايندڙ;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				صحيح ڳڻپ, ثابت فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, وٿي,
						    ڳڻپ, معلومات);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
				 صحيح ڳڻپ, فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, بلاڪ,
						      ڳڻپ, معلومات);

	إذا ((بلاڪ >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (بنانشان ڊگهو) بلاڪ,
					     ڳڻپ, معلومات);
}

errcode_t io_channel_write_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
				 صحيح ڳڻپ, ثابت فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, بلاڪ,
						       ڳڻپ, معلومات);

	إذا ((بلاڪ >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (بنانشان ڊگهو) بلاڪ,
					     ڳڻپ, معلومات);
}

errcode_t io_channel_discard(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			     بنانشان ڊگهو ڊگهو ڳڻپ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, بلاڪ, ڳڻپ);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			     بنانشان ڊگهو ڊگهو ڳڻپ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, بلاڪ, ڳڻپ);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح ڳڻپ, فراغ *ptr)
{
	size_t	سائيز;

	إذا (ڳڻپ == 0)
		سائيز = io->بلاڪ_سائيز;
	وإلا إذا (ڳڻپ > 0)
		سائيز = io->بلاڪ_سائيز * ڳڻپ;
	وإلا
		سائيز = -ڳڻپ;

	إذا (io->align > 0) {
		إذا ((بنانشان) io->align > سائيز)
			سائيز = io->align;
		أرجع ext2fs_get_memalign(سائيز, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(سائيز, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, بنانشان ڊگهو ڊگهو بلاڪ,
				     بنانشان ڊگهو ڊگهو ڳڻپ)
{
	إذا (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, بلاڪ, ڳڻپ);
}

errcode_t io_channel_flock(io_channel io, بنانشان صحيح flock_flags)
{
	إذا (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	إذا (!io->manager->flock)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->flock(io, 0);
}
