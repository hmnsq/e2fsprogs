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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

ഘടന inode_private_data {
	പൂർണ്ണസംഖ്യ				magic;
	അക്ഷരം				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ഘടന ext2_inode		inode;
	പൂർണ്ണസംഖ്യ				flags;
	ഘടന inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

സ്ഥിതം ഘടന inode_private_data *മുകൾ_intern;
സ്ഥിതം പൂർണ്ണസംഖ്യ ino_unique = 0;

സ്ഥിതം errcode_t inode_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel);
സ്ഥിതം errcode_t inode_close(io_channel channel);
സ്ഥിതം errcode_t inode_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize);
സ്ഥിതം errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *data);
സ്ഥിതം errcode_t inode_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *data);
സ്ഥിതം errcode_t inode_flush(io_channel channel);
സ്ഥിതം errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				പൂർണ്ണസംഖ്യ size, സ്ഥിരം ശൂന്യം *data);
സ്ഥിതം errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *data);
സ്ഥിതം errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *data);

സ്ഥിതം ഘടന struct_io_manager struct_inode_manager = {
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
				  ഘടന ext2_inode *inode,
				  അക്ഷരം **name)
{
	ഘടന inode_private_data 	*data;
	errcode_t			retval;

	എങ്കിൽ ((retval = ext2fs_get_mem(വലുപ്പം(ഘടന inode_private_data),
				     &data)))
		തിരികെ retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	എങ്കിൽ (inode) {
		memcpy(&data->inode, inode, വലുപ്പം(ഘടന ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = മുകൾ_intern;
	മുകൾ_intern = data;
	*name = data->name;
	തിരികെ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 അക്ഷരം **name)
{
	തിരികെ ext2fs_inode_io_intern2(fs, ino, ശൂന്യ, name);
}


സ്ഥിതം errcode_t inode_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	io_channel	io = ശൂന്യ;
	ഘടന inode_private_data *prev, *data = ശൂന്യ;
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		open_flags;

	എങ്കിൽ (name == 0)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;

	വേണ്ടി (data = മുകൾ_intern, prev = ശൂന്യ; data;
	     prev = data, data = data->next)
		എങ്കിൽ (strcmp(name, data->name) == 0)
			നിർത്തുക;
	എങ്കിൽ (!data)
		തിരികെ ENOENT;
	എങ്കിൽ (prev)
		prev->next = data->next;
	അല്ലെങ്കിൽ
		മുകൾ_intern = data->next;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_io_channel), &io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(io, 0, വലുപ്പം(ഘടന struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

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
	എങ്കിൽ (retval)
		പോകുക cleanup;

	*channel = io;
	തിരികെ 0;

cleanup:
	എങ്കിൽ (io && io->name)
		ext2fs_free_mem(&io->name);
	എങ്കിൽ (data)
		ext2fs_free_mem(&data);
	എങ്കിൽ (io)
		ext2fs_free_mem(&io);
	തിരികെ retval;
}

സ്ഥിതം errcode_t inode_close(io_channel channel)
{
	ഘടന inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
		തിരികെ 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	എങ്കിൽ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t inode_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	ഘടന inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	തിരികെ 0;
}


സ്ഥിതം errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	ഘടന inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	എങ്കിൽ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		തിരികെ retval;

	എണ്ണം = (എണ്ണം < 0) ? -എണ്ണം : (എണ്ണം * channel->block_size);

	തിരികെ ext2fs_file_read(data->file, buf, എണ്ണം, 0);
}

സ്ഥിതം errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	തിരികെ inode_read_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	എങ്കിൽ ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		തിരികെ retval;

	എണ്ണം = (എണ്ണം < 0) ? -എണ്ണം : (എണ്ണം * channel->block_size);

	തിരികെ ext2fs_file_write(data->file, buf, എണ്ണം, 0);
}

സ്ഥിതം errcode_t inode_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ inode_write_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 പൂർണ്ണസംഖ്യ size, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	എങ്കിൽ ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		തിരികെ retval;

	തിരികെ ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
സ്ഥിതം errcode_t inode_flush(io_channel channel)
{
	ഘടന inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	തിരികെ ext2fs_file_flush(data->file);
}

