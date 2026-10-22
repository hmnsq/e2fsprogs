/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_مجموعه_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *بعدی, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (!opts)
		أرجع 0;

	إذا (!channel->manager->مجموعه_option)
		أرجع EXT2_ET_INVALID_آرگومان;

	options = حافظه_بده(طول_رشته(opts)+1);
	إذا (!options)
		أرجع EXT2_ET_NO_حافظه;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		بعدی = strchr(ptr, '&');
		إذا (بعدی)
			*بعدی++ = 0;

		arg = strchr(ptr, '=');
		إذا (arg)
			*arg++ = 0;

		retval = (channel->manager->مجموعه_option)(channel, ptr, arg);
		إذا (retval)
			اكسر;
		ptr = بعدی;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				صحيح شمار, ثابت فراغ *داده)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, جابه_جایی,
						    شمار, داده);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
				 صحيح شمار, فراغ *داده)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, بلوک,
						      شمار, داده);

	إذا ((بلوک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (بیعلامت دراز) بلوک,
					     شمار, داده);
}

errcode_t io_channel_write_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
				 صحيح شمار, ثابت فراغ *داده)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, بلوک,
						       شمار, داده);

	إذا ((بلوک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (بیعلامت دراز) بلوک,
					     شمار, داده);
}

errcode_t io_channel_discard(io_channel channel, بیعلامت دراز دراز بلوک,
			     بیعلامت دراز دراز شمار)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, بلوک, شمار);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, بیعلامت دراز دراز بلوک,
			     بیعلامت دراز دراز شمار)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, بلوک, شمار);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح شمار, فراغ *ptr)
{
	size_t	اندازه;

	إذا (شمار == 0)
		اندازه = io->بلوک_اندازه;
	وإلا إذا (شمار > 0)
		اندازه = io->بلوک_اندازه * شمار;
	وإلا
		اندازه = -شمار;

	إذا (io->align > 0) {
		إذا ((بیعلامت) io->align > اندازه)
			اندازه = io->align;
		أرجع ext2fs_get_memalign(اندازه, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(اندازه, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, بیعلامت دراز دراز بلوک,
				     بیعلامت دراز دراز شمار)
{
	إذا (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, بلوک, شمار);
}

errcode_t io_channel_flock(io_channel io, بیعلامت صحيح flock_flags)
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
