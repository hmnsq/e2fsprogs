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
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#만약 HAVE_ERRNO_H
#include <errno.h>
#endif
#include <time.h>

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  만약 ((struct)->magic != (code)) 반환 (code)

struct inode_private_data {
	정수				magic;
	char				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	struct ext2_inode		inode;
	정수				flags;
	struct inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

static struct inode_private_data *top_intern;
static 정수 ino_unique = 0;

static errcode_t inode_open(const char *name, 정수 flags, io_channel *channel);
static errcode_t inode_close(io_channel channel);
static errcode_t inode_set_blksize(io_channel channel, 정수 blksize);
static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       정수 count, void *data);
static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				정수 count, const void *data);
static errcode_t inode_flush(io_channel channel);
static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				정수 size, const void *data);
static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, 정수 count, void *data);
static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, 정수 count, const void *data);

static struct struct_io_manager struct_inode_manager = {
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
				  struct ext2_inode *inode,
				  char **name)
{
	struct inode_private_data 	*data;
	errcode_t			retval;

	만약 ((retval = ext2fs_get_mem(sizeof(struct inode_private_data),
				     &data)))
		반환 retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	만약 (inode) {
		memcpy(&data->inode, inode, sizeof(struct ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = top_intern;
	top_intern = data;
	*name = data->name;
	반환 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 char **name)
{
	반환 ext2fs_inode_io_intern2(fs, ino, NULL, name);
}


static errcode_t inode_open(const char *name, 정수 flags, io_channel *channel)
{
	io_channel	io = NULL;
	struct inode_private_data *prev, *data = NULL;
	errcode_t	retval;
	정수		open_flags;

	만약 (name == 0)
		반환 EXT2_ET_BAD_DEVICE_NAME;

	위해 (data = top_intern, prev = NULL; data;
	     prev = data, data = data->next)
		만약 (strcmp(name, data->name) == 0)
			break;
	만약 (!data)
		반환 ENOENT;
	만약 (prev)
		prev->next = data->next;
	아니면
		top_intern = data->next;

	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	만약 (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	만약 (retval)
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
	만약 (retval)
		goto cleanup;

	*channel = io;
	반환 0;

cleanup:
	만약 (io && io->name)
		ext2fs_free_mem(&io->name);
	만약 (data)
		ext2fs_free_mem(&data);
	만약 (io)
		ext2fs_free_mem(&io);
	반환 retval;
}

static errcode_t inode_close(io_channel channel)
{
	struct inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	만약 (--channel->refcount > 0)
		반환 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	만약 (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	반환 retval;
}

static errcode_t inode_set_blksize(io_channel channel, 정수 blksize)
{
	struct inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	반환 0;
}


static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, 정수 count, void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	만약 ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		반환 retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	반환 ext2fs_file_read(data->file, buf, count, 0);
}

static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       정수 count, void *buf)
{
	반환 inode_read_blk64(channel, block, count, buf);
}

static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, 정수 count, const void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	만약 ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		반환 retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	반환 ext2fs_file_write(data->file, buf, count, 0);
}

static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				정수 count, const void *buf)
{
	반환 inode_write_blk64(channel, block, count, buf);
}

static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 정수 size, const void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	만약 ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		반환 retval;

	반환 ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
static errcode_t inode_flush(io_channel channel)
{
	struct inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	반환 ext2fs_file_flush(data->file);
}

