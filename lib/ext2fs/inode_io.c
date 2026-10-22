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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#якщо HAVE_ERRNO_H
#include <errno.h>
#endif
#include <time.h>

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  якщо ((struct)->magic != (code)) повернути (code)

struct inode_private_data {
	ціле				magic;
	char				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	struct ext2_inode		inode;
	ціле				flags;
	struct inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

static struct inode_private_data *top_intern;
static ціле ino_unique = 0;

static errcode_t inode_open(const char *name, ціле flags, io_channel *channel);
static errcode_t inode_close(io_channel channel);
static errcode_t inode_set_blksize(io_channel channel, ціле blksize);
static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ціле count, void *data);
static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ціле count, const void *data);
static errcode_t inode_flush(io_channel channel);
static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				ціле size, const void *data);
static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ціле count, void *data);
static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ціле count, const void *data);

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

	якщо ((retval = ext2fs_get_mem(sizeof(struct inode_private_data),
				     &data)))
		повернути retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	якщо (inode) {
		memcpy(&data->inode, inode, sizeof(struct ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = top_intern;
	top_intern = data;
	*name = data->name;
	повернути 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 char **name)
{
	повернути ext2fs_inode_io_intern2(fs, ino, NULL, name);
}


static errcode_t inode_open(const char *name, ціле flags, io_channel *channel)
{
	io_channel	io = NULL;
	struct inode_private_data *prev, *data = NULL;
	errcode_t	retval;
	ціле		open_flags;

	якщо (name == 0)
		повернути EXT2_ET_BAD_DEVICE_NAME;

	для (data = top_intern, prev = NULL; data;
	     prev = data, data = data->next)
		якщо (strcmp(name, data->name) == 0)
			break;
	якщо (!data)
		повернути ENOENT;
	якщо (prev)
		prev->next = data->next;
	інакше
		top_intern = data->next;

	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	якщо (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	якщо (retval)
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
	якщо (retval)
		goto cleanup;

	*channel = io;
	повернути 0;

cleanup:
	якщо (io && io->name)
		ext2fs_free_mem(&io->name);
	якщо (data)
		ext2fs_free_mem(&data);
	якщо (io)
		ext2fs_free_mem(&io);
	повернути retval;
}

static errcode_t inode_close(io_channel channel)
{
	struct inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	якщо (--channel->refcount > 0)
		повернути 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	якщо (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	повернути retval;
}

static errcode_t inode_set_blksize(io_channel channel, ціле blksize)
{
	struct inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	повернути 0;
}


static errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ціле count, void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	якщо ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		повернути retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	повернути ext2fs_file_read(data->file, buf, count, 0);
}

static errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ціле count, void *buf)
{
	повернути inode_read_blk64(channel, block, count, buf);
}

static errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ціле count, const void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	якщо ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		повернути retval;

	count = (count < 0) ? -count : (count * channel->block_size);

	повернути ext2fs_file_write(data->file, buf, count, 0);
}

static errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ціле count, const void *buf)
{
	повернути inode_write_blk64(channel, block, count, buf);
}

static errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 ціле size, const void *buf)
{
	struct inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	якщо ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		повернути retval;

	повернути ext2fs_file_write(data->file, buf, size, 0);
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

	повернути ext2fs_file_flush(data->file);
}

