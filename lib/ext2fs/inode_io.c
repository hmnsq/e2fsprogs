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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)

ساختار inode_private_data {
	صحیح				magic;
	نویسه				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ساختار ext2_inode		inode;
	صحیح				flags;
	ساختار inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

ایستا ساختار inode_private_data *بالاترین_intern;
ایستا صحیح ino_unique = 0;

ایستا errcode_t inode_open(ثابت نویسه *name, صحیح flags, io_channel *channel);
ایستا errcode_t inode_close(io_channel channel);
ایستا errcode_t inode_set_blksize(io_channel channel, صحیح blksize);
ایستا errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *data);
ایستا errcode_t inode_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *data);
ایستا errcode_t inode_flush(io_channel channel);
ایستا errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				صحیح size, ثابت تهی *data);
ایستا errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, صحیح شمار, تهی *data);
ایستا errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, صحیح شمار, ثابت تهی *data);

ایستا ساختار struct_io_manager struct_inode_manager = {
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
				  ساختار ext2_inode *inode,
				  نویسه **name)
{
	ساختار inode_private_data 	*data;
	errcode_t			retval;

	اگر ((retval = ext2fs_get_mem(اندازه(ساختار inode_private_data),
				     &data)))
		بازگشت retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	اگر (inode) {
		memcpy(&data->inode, inode, اندازه(ساختار ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = بالاترین_intern;
	بالاترین_intern = data;
	*name = data->name;
	بازگشت 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 نویسه **name)
{
	بازگشت ext2fs_inode_io_intern2(fs, ino, پوچ, name);
}


ایستا errcode_t inode_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	io_channel	io = پوچ;
	ساختار inode_private_data *prev, *data = پوچ;
	errcode_t	retval;
	صحیح		open_flags;

	اگر (name == 0)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;

	برای (data = بالاترین_intern, prev = پوچ; data;
	     prev = data, data = data->next)
		اگر (strcmp(name, data->name) == 0)
			بشکن;
	اگر (!data)
		بازگشت ENOENT;
	اگر (prev)
		prev->next = data->next;
	وگرنه
		بالاترین_intern = data->next;

	retval = ext2fs_get_mem(اندازه(ساختار struct_io_channel), &io);
	اگر (retval)
		برو cleanup;
	memset(io, 0, اندازه(ساختار struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	اگر (retval)
		برو cleanup;

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
	اگر (retval)
		برو cleanup;

	*channel = io;
	بازگشت 0;

cleanup:
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (data)
		ext2fs_free_mem(&data);
	اگر (io)
		ext2fs_free_mem(&io);
	بازگشت retval;
}

ایستا errcode_t inode_close(io_channel channel)
{
	ساختار inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		بازگشت 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	بازگشت retval;
}

ایستا errcode_t inode_set_blksize(io_channel channel, صحیح blksize)
{
	ساختار inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	بازگشت 0;
}


ایستا errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, صحیح شمار, تهی *buf)
{
	ساختار inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		بازگشت retval;

	شمار = (شمار < 0) ? -شمار : (شمار * channel->block_size);

	بازگشت ext2fs_file_read(data->file, buf, شمار, 0);
}

ایستا errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	بازگشت inode_read_blk64(channel, block, شمار, buf);
}

ایستا errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, صحیح شمار, ثابت تهی *buf)
{
	ساختار inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		بازگشت retval;

	شمار = (شمار < 0) ? -شمار : (شمار * channel->block_size);

	بازگشت ext2fs_file_write(data->file, buf, شمار, 0);
}

ایستا errcode_t inode_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *buf)
{
	بازگشت inode_write_blk64(channel, block, شمار, buf);
}

ایستا errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 صحیح size, ثابت تهی *buf)
{
	ساختار inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		بازگشت retval;

	بازگشت ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
ایستا errcode_t inode_flush(io_channel channel)
{
	ساختار inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	بازگشت ext2fs_file_flush(data->file);
}

