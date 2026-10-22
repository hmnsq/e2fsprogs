/*
 * inode_io.c --- This is allows an inode in an ext2 filesystem image
 * 	to be accessed via the I/O manager interface.
 *
 * Copyright (C) 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  ܐܢ ((بنية)->magic != (code)) ܦܢܐ (code)

بنية inode_private_data {
	صحيح				magic;
	حرف				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	بنية ext2_inode		inode;
	صحيح				flags;
	بنية inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

ساكن بنية inode_private_data *top_intern;
ساكن صحيح ino_unique = 0;

ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t inode_close(io_channel channel);
ساكن errcode_t inode_set_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *data);
ساكن errcode_t inode_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *data);
ساكن errcode_t inode_flush(io_channel channel);
ساكن errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				صحيح size, ثابت فراغ *data);
ساكن errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, صحيح count, فراغ *data);
ساكن errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, صحيح count, ثابت فراغ *data);

ساكن بنية struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.set_blksize	= inode_set_blksize,
	.read_blk	= inode_read_blk,
	.write_blk	= inode_write_blk,
	.flush		= inode_flush,
	.write_byte	= inode_write_byte,
	.read_blk64	= inode_read_blk64,
	.write_blk64	= inode_write_blk64
};

io_manager inode_io_manager = &struct_inode_manager;

errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
				  بنية ext2_inode *inode,
				  حرف **name)
{
	بنية inode_private_data 	*data;
	errcode_t			retval;

	ܐܢ ((retval = ext2fs_get_mem(حجم(بنية inode_private_data),
				     &data)))
		ܦܢܐ retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	ܐܢ (inode) {
		memcpy(&data->inode, inode, حجم(بنية ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = top_intern;
	top_intern = data;
	*name = data->name;
	ܦܢܐ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 حرف **name)
{
	ܦܢܐ ext2fs_inode_io_intern2(fs, ino, عدم, name);
}


ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية inode_private_data *prev, *data = عدم;
	errcode_t	retval;
	صحيح		open_flags;

	ܐܢ (name == 0)
		ܦܢܐ EXT2_ET_BAD_DEVICE_NAME;

	لكل (data = top_intern, prev = عدم; data;
	     prev = data, data = data->next)
		ܐܢ (strcmp(name, data->name) == 0)
			اكسر;
	ܐܢ (!data)
		ܦܢܐ ENOENT;
	ܐܢ (prev)
		prev->next = data->next;
	ܐܚܪܢܐ
		top_intern = data->next;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	ܐܢ (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ܐܢ (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(data->fs, data->ino,
				   (data->flags & CHANNEL_HAS_INODE) ?
				   &data->inode : 0, open_flags,
				   &data->file);
	ܐܢ (retval)
		اذهب cleanup;

	*channel = io;
	ܦܢܐ 0;

cleanup:
	ܐܢ (io && io->name)
		ext2fs_free_mem(&io->name);
	ܐܢ (data)
		ext2fs_free_mem(&data);
	ܐܢ (io)
		ext2fs_free_mem(&io);
	ܦܢܐ retval;
}

ساكن errcode_t inode_close(io_channel channel)
{
	بنية inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ܐܢ (--channel->refcount > 0)
		ܦܢܐ 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	ܐܢ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ܦܢܐ retval;
}

ساكن errcode_t inode_set_blksize(io_channel channel, صحيح blksize)
{
	بنية inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	ܦܢܐ 0;
}


ساكن errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, صحيح count, فراغ *buf)
{
	بنية inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ܐܢ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ܦܢܐ retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	ܦܢܐ ext2fs_file_read(data->file, buf, count, 0);
}

ساكن errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *buf)
{
	ܦܢܐ inode_read_blk64(channel, block, count, buf);
}

ساكن errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, صحيح count, ثابت فراغ *buf)
{
	بنية inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ܐܢ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ܦܢܐ retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	ܦܢܐ ext2fs_file_write(data->file, buf, count, 0);
}

ساكن errcode_t inode_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *buf)
{
	ܦܢܐ inode_write_blk64(channel, block, count, buf);
}

ساكن errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 صحيح size, ثابت فراغ *buf)
{
	بنية inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ܐܢ ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		ܦܢܐ retval;

	ܦܢܐ ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t inode_flush(io_channel channel)
{
	بنية inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ܦܢܐ ext2fs_file_flush(data->file);
}

