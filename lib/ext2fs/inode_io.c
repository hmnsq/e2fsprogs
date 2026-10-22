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

#include "config.h"
#include <stdio.h>
#include <string.h>
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#නම් HAVE_ERRNO_H
#include <errno.h>
#endif
#include <time.h>

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  නම් ((sa_nracanaa)->magic != (code)) ආපසු (code)

sa_nracanaa inode_private_data {
	පූර්ණ_සංඛ්‍යා				magic;
	var_na				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	sa_nracanaa ext2_inode		inode;
	පූර්ණ_සංඛ්‍යා				flags;
	sa_nracanaa inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

static sa_nracanaa inode_private_data *top_intern;
static පූර්ණ_සංඛ්‍යා ino_unique = 0;

static errcode_t inode_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel);
static errcode_t inode_close(io_channel channel);
static errcode_t inode_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize);
static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *data);
static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *data);
static errcode_t inode_flush(io_channel channel);
static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				පූර්ණ_සංඛ්‍යා size, const void *data);
static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, පූර්ණ_සංඛ්‍යා count, void *data);
static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, පූර්ණ_සංඛ්‍යා count, const void *data);

static sa_nracanaa struct_io_manager struct_inode_manager = {
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
				  sa_nracanaa ext2_inode *inode,
				  var_na **name)
{
	sa_nracanaa inode_private_data 	*data;
	errcode_t			retval;

	නම් ((retval = ext2fs_get_mem(parimaa_na(sa_nracanaa inode_private_data),
				     &data)))
		ආපසු retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	නම් (inode) {
		memcpy(&data->inode, inode, parimaa_na(sa_nracanaa ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = top_intern;
	top_intern = data;
	*name = data->name;
	ආපසු 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 var_na **name)
{
	ආපසු ext2fs_inode_io_intern2(fs, ino, NULL, name);
}


static errcode_t inode_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel)
{
	io_channel	io = NULL;
	sa_nracanaa inode_private_data *prev, *data = NULL;
	errcode_t	retval;
	පූර්ණ_සංඛ්‍යා		open_flags;

	නම් (name == 0)
		ආපසු EXT2_ET_BAD_DEVICE_NAME;

	සඳහා (data = top_intern, prev = NULL; data;
	     prev = data, data = data->next)
		නම් (strcmp(name, data->name) == 0)
			bha_nga;
	නම් (!data)
		ආපසු ENOENT;
	නම් (prev)
		prev->next = data->next;
	නැතහොත්
		top_intern = data->next;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	නම් (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	නම් (retval)
		goto cleanup;

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
	නම් (retval)
		goto cleanup;

	*channel = io;
	ආපසු 0;

cleanup:
	නම් (io && io->name)
		ext2fs_free_mem(&io->name);
	නම් (data)
		ext2fs_free_mem(&data);
	නම් (io)
		ext2fs_free_mem(&io);
	ආපසු retval;
}

static errcode_t inode_close(io_channel channel)
{
	sa_nracanaa inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	නම් (--channel->refcount > 0)
		ආපසු 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	නම් (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ආපසු retval;
}

static errcode_t inode_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize)
{
	sa_nracanaa inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	ආපසු 0;
}


static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, පූර්ණ_සංඛ්‍යා count, void *buf)
{
	sa_nracanaa inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	නම් ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ආපසු retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	ආපසු ext2fs_file_read(data->file, buf, count, 0);
}

static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *buf)
{
	ආපසු inode_read_blk64(channel, block, count, buf);
}

static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	sa_nracanaa inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	නම් ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ආපසු retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	ආපසු ext2fs_file_write(data->file, buf, count, 0);
}

static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	ආපසු inode_write_blk64(channel, block, count, buf);
}

static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 පූර්ණ_සංඛ්‍යා size, const void *buf)
{
	sa_nracanaa inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	නම් ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		ආපසු retval;

	ආපසු ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
static errcode_t inode_flush(io_channel channel)
{
	sa_nracanaa inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ආපසු ext2fs_file_flush(data->file);
}

