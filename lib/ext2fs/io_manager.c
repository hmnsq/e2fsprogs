/*
 * io_manager.c --- the I/O manager abstraction
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t io_channel_سیٹ_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *اگلا, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (!opts)
		أرجع 0;

	إذا (!channel->manager->سیٹ_option)
		أرجع EXT2_ET_INVALID_دلیل;

	options = حافظہ_دیو(لڑی_لمبائی(opts)+1);
	إذا (!options)
		أرجع EXT2_ET_NO_حافظہ;
	strcpy(options, opts);
	ptr = options;

	طالما (ptr && *ptr) {
		اگلا = strchr(ptr, '&');
		إذا (اگلا)
			*اگلا++ = 0;

		arg = strchr(ptr, '=');
		إذا (arg)
			*arg++ = 0;

		retval = (channel->manager->سیٹ_option)(channel, ptr, arg);
		إذا (retval)
			اكسر;
		ptr = اگلا;
	}
	حرر(options);
	أرجع retval;
}

errcode_t io_channel_write_byte(io_channel channel, بنانشانی لما وتھ,
				صحيح گنتی, ثابت فراغ *انکڑے)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_byte)
		أرجع channel->manager->write_byte(channel, وتھ,
						    گنتی, انکڑے);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, بنانشانی لما لما بلاک,
				 صحيح گنتی, فراغ *انکڑے)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->read_blk64)
		أرجع (channel->manager->read_blk64)(channel, بلاک,
						      گنتی, انکڑے);

	إذا ((بلاک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->read_blk)(channel, (بنانشانی لما) بلاک,
					     گنتی, انکڑے);
}

errcode_t io_channel_write_blk64(io_channel channel, بنانشانی لما لما بلاک,
				 صحيح گنتی, ثابت فراغ *انکڑے)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->write_blk64)
		أرجع (channel->manager->write_blk64)(channel, بلاک,
						       گنتی, انکڑے);

	إذا ((بلاک >> 32) != 0)
		أرجع EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	أرجع (channel->manager->write_blk)(channel, (بنانشانی لما) بلاک,
					     گنتی, انکڑے);
}

errcode_t io_channel_discard(io_channel channel, بنانشانی لما لما بلاک,
			     بنانشانی لما لما گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->discard)
		أرجع (channel->manager->discard)(channel, بلاک, گنتی);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, بنانشانی لما لما بلاک,
			     بنانشانی لما لما گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	إذا (channel->manager->zeroout)
		أرجع (channel->manager->zeroout)(channel, بلاک, گنتی);

	أرجع EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحيح گنتی, فراغ *ptr)
{
	size_t	ناپ;

	إذا (گنتی == 0)
		ناپ = io->بلاک_ناپ;
	وإلا إذا (گنتی > 0)
		ناپ = io->بلاک_ناپ * گنتی;
	وإلا
		ناپ = -گنتی;

	إذا (io->align > 0) {
		إذا ((بنانشانی) io->align > ناپ)
			ناپ = io->align;
		أرجع ext2fs_get_memalign(ناپ, io->align, ptr);
	} وإلا
		أرجع ext2fs_get_mem(ناپ, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, بنانشانی لما لما بلاک,
				     بنانشانی لما لما گنتی)
{
	إذا (!io->manager->cache_readahead)
		أرجع EXT2_ET_OP_NOT_SUPPORTED;

	أرجع io->manager->cache_readahead(io, بلاک, گنتی);
}

errcode_t io_channel_flock(io_channel io, بنانشانی صحيح flock_flags)
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
