/*
 * io_manager.c --- the I/O manager abstraction
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t io_channel_سیٹ_options(io_channel channel, ثابت حرف *opts)
{
	errcode_t retval = 0;
	حرف *اگلا, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (!opts)
		واپس 0;

	اگر (!channel->manager->سیٹ_option)
		واپس EXT2_ET_INVALID_دلیل;

	options = حافظہ_دو(لڑی_لمبائی(opts)+1);
	اگر (!options)
		واپس EXT2_ET_NO_حافظہ;
	strcpy(options, opts);
	ptr = options;

	جب (ptr && *ptr) {
		اگلا = strchr(ptr, '&');
		اگر (اگلا)
			*اگلا++ = 0;

		arg = strchr(ptr, '=');
		اگر (arg)
			*arg++ = 0;

		retval = (channel->manager->سیٹ_option)(channel, ptr, arg);
		اگر (retval)
			توڑو;
		ptr = اگلا;
	}
	آزاد(options);
	واپس retval;
}

errcode_t io_channel_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				صحیح گنتی, ثابت خالی *ڈیٹا)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->write_byte)
		واپس channel->manager->write_byte(channel, انحراف,
						    گنتی, ڈیٹا);

	واپس EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
				 صحیح گنتی, خالی *ڈیٹا)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->read_blk64)
		واپس (channel->manager->read_blk64)(channel, بلاک,
						      گنتی, ڈیٹا);

	اگر ((بلاک >> 32) != 0)
		واپس EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	واپس (channel->manager->read_blk)(channel, (غیرعلامتی طویل) بلاک,
					     گنتی, ڈیٹا);
}

errcode_t io_channel_write_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
				 صحیح گنتی, ثابت خالی *ڈیٹا)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->write_blk64)
		واپس (channel->manager->write_blk64)(channel, بلاک,
						       گنتی, ڈیٹا);

	اگر ((بلاک >> 32) != 0)
		واپس EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	واپس (channel->manager->write_blk)(channel, (غیرعلامتی طویل) بلاک,
					     گنتی, ڈیٹا);
}

errcode_t io_channel_discard(io_channel channel, غیرعلامتی طویل طویل بلاک,
			     غیرعلامتی طویل طویل گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->discard)
		واپس (channel->manager->discard)(channel, بلاک, گنتی);

	واپس EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, غیرعلامتی طویل طویل بلاک,
			     غیرعلامتی طویل طویل گنتی)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	اگر (channel->manager->zeroout)
		واپس (channel->manager->zeroout)(channel, بلاک, گنتی);

	واپس EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, صحیح گنتی, خالی *ptr)
{
	size_t	سائز;

	اگر (گنتی == 0)
		سائز = io->بلاک_سائز;
	ورنہ اگر (گنتی > 0)
		سائز = io->بلاک_سائز * گنتی;
	ورنہ
		سائز = -گنتی;

	اگر (io->align > 0) {
		اگر ((غیرعلامتی) io->align > سائز)
			سائز = io->align;
		واپس ext2fs_get_memalign(سائز, io->align, ptr);
	} ورنہ
		واپس ext2fs_get_mem(سائز, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, غیرعلامتی طویل طویل بلاک,
				     غیرعلامتی طویل طویل گنتی)
{
	اگر (!io->manager->cache_readahead)
		واپس EXT2_ET_OP_NOT_SUPPORTED;

	واپس io->manager->cache_readahead(io, بلاک, گنتی);
}

errcode_t io_channel_flock(io_channel io, غیرعلامتی صحیح flock_flags)
{
	اگر (!io->manager->flock)
		واپس EXT2_ET_OP_NOT_SUPPORTED;

	واپس io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	اگر (!io->manager->flock)
		واپس EXT2_ET_OP_NOT_SUPPORTED;

	واپس io->manager->flock(io, 0);
}
