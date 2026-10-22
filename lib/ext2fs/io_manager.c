/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_سېټ_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *بل, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (!opts)
		أرجع 0;

	إذا (!channel->manager->سېټ_option)
		أرجع EXT2_ET_INVALID_دلیل;

	options = حافظه_ورکړه(مزي_اوږدوالی(opts)+1);
	إذا (!options)
		أرجع EXT2_ET_NO_حافظه;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		بل = strchr(ptr, '&');
		إذا (بل)
			*بل++ = 0;

		arg = strchr(ptr, '=');
		إذا (arg)
			*arg++ = 0;

		retval = (channel->manager->سېټ_option)(channel, ptr, arg);
		إذا (retval)
			اكسر;
		ptr = بل;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, بینښه اوږد دننه_واټن,
				صحيح شمېر, ثابت فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, دننه_واټن,
						    شمېر, معلومات);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
				 صحيح شمېر, فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, بلاک,
						      شمېر, معلومات);

	إذا ((بلاک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (بینښه اوږد) بلاک,
					     شمېر, معلومات);
}

errcode_t io_channel_write_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
				 صحيح شمېر, ثابت فراغ *معلومات)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, بلاک,
						       شمېر, معلومات);

	إذا ((بلاک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (بینښه اوږد) بلاک,
					     شمېر, معلومات);
}

errcode_t io_channel_discard(io_channel channel, بینښه اوږد اوږد بلاک,
			     بینښه اوږد اوږد شمېر)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, بلاک, شمېر);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, بینښه اوږد اوږد بلاک,
			     بینښه اوږد اوږد شمېر)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, بلاک, شمېر);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح شمېر, فراغ *ptr)
{
	size_t	اندازه;

	إذا (شمېر == 0)
		اندازه = io->بلاک_اندازه;
	وإلا إذا (شمېر > 0)
		اندازه = io->بلاک_اندازه * شمېر;
	وإلا
		اندازه = -شمېر;

	إذا (io->align > 0) {
		إذا ((بینښه) io->align > اندازه)
			اندازه = io->align;
		أرجع ext2fs_get_memalign(اندازه, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(اندازه, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, بینښه اوږد اوږد بلاک,
				     بینښه اوږد اوږد شمېر)
{
	إذا (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, بلاک, شمېر);
}

errcode_t io_channel_flock(io_channel io, بینښه صحيح flock_flags)
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
