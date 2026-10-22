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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)

ಸಂರಚನಾ inode_private_data {
	ಪೂರ್ಣಾಙ್ಕ				magic;
	ವರ್ಣ				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ಸಂರಚನಾ ext2_inode		inode;
	ಪೂರ್ಣಾಙ್ಕ				flags;
	ಸಂರಚನಾ inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

ಸ್ಥಿರ ಸಂರಚನಾ inode_private_data *ತುದಿ_intern;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ino_unique = 0;

ಸ್ಥಿರ errcode_t inode_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t inode_close(io_channel channel);
ಸ್ಥಿರ errcode_t inode_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize);
ಸ್ಥಿರ errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t inode_flush(io_channel channel);
ಸ್ಥಿರ errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				ಪೂರ್ಣಾಙ್ಕ size, ಧ್ರುವ ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *data);

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_inode_manager = {
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
				  ಸಂರಚನಾ ext2_inode *inode,
				  ವರ್ಣ **name)
{
	ಸಂರಚನಾ inode_private_data 	*data;
	errcode_t			retval;

	ಯದಿ ((retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ inode_private_data),
				     &data)))
		ಪ್ರತ್ಯಾಗಮ retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	ಯದಿ (inode) {
		memcpy(&data->inode, inode, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = ತುದಿ_intern;
	ತುದಿ_intern = data;
	*name = data->name;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 ವರ್ಣ **name)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_inode_io_intern2(fs, ino, ರಿಕ್ತ, name);
}


ಸ್ಥಿರ errcode_t inode_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	io_channel	io = ರಿಕ್ತ;
	ಸಂರಚನಾ inode_private_data *prev, *data = ರಿಕ್ತ;
	errcode_t	retval;
	ಪೂರ್ಣಾಙ್ಕ		open_flags;

	ಯದಿ (name == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;

	ಪ್ರತಿ (data = ತುದಿ_intern, prev = ರಿಕ್ತ; data;
	     prev = data, data = data->next)
		ಯದಿ (strcmp(name, data->name) == 0)
			ಭಙ್ಗ;
	ಯದಿ (!data)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	ಯದಿ (prev)
		prev->next = data->next;
	ಅನ್ಯಥಾ
		ತುದಿ_intern = data->next;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel), &io);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(io, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

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
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	*channel = io;
	ಪ್ರತ್ಯಾಗಮ 0;

cleanup:
	ಯದಿ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಯದಿ (data)
		ext2fs_free_mem(&data);
	ಯದಿ (io)
		ext2fs_free_mem(&io);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t inode_close(io_channel channel)
{
	ಸಂರಚನಾ inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಯದಿ (--channel->refcount > 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	ಯದಿ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t inode_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	ಸಂರಚನಾ inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	ಪ್ರತ್ಯಾಗಮ 0;
}


ಸ್ಥಿರ errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಯದಿ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ಪ್ರತ್ಯಾಗಮ retval;

	ಎಣಿಕೆ = (ಎಣಿಕೆ < 0) ? -ಎಣಿಕೆ : (ಎಣಿಕೆ * channel->block_size);

	ಪ್ರತ್ಯಾಗಮ ext2fs_file_read(data->file, buf, ಎಣಿಕೆ, 0);
}

ಸ್ಥಿರ errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ inode_read_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಯದಿ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ಪ್ರತ್ಯಾಗಮ retval;

	ಎಣಿಕೆ = (ಎಣಿಕೆ < 0) ? -ಎಣಿಕೆ : (ಎಣಿಕೆ * channel->block_size);

	ಪ್ರತ್ಯಾಗಮ ext2fs_file_write(data->file, buf, ಎಣಿಕೆ, 0);
}

ಸ್ಥಿರ errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ inode_write_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 ಪೂರ್ಣಾಙ್ಕ size, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಯದಿ ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t inode_flush(io_channel channel)
{
	ಸಂರಚನಾ inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಪ್ರತ್ಯಾಗಮ ext2fs_file_flush(data->file);
}

