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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)

સંરચના inode_private_data {
	પૂર્ણાંક				magic;
	અક્ષર				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	સંરચના ext2_inode		inode;
	પૂર્ણાંક				flags;
	સંરચના inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

સ્થિર સંરચના inode_private_data *ટોચ_intern;
સ્થિર પૂર્ણાંક ino_unique = 0;

સ્થિર errcode_t inode_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel);
સ્થિર errcode_t inode_close(io_channel channel);
સ્થિર errcode_t inode_set_blksize(io_channel channel, પૂર્ણાંક blksize);
સ્થિર errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *data);
સ્થિર errcode_t inode_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
સ્થિર errcode_t inode_flush(io_channel channel);
સ્થિર errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				પૂર્ણાંક size, અચલ શૂન્ય *data);
સ્થિર errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, પૂર્ણાંક ગણતરી, શૂન્ય *data);
સ્થિર errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);

સ્થિર સંરચના struct_io_manager struct_inode_manager = {
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
				  સંરચના ext2_inode *inode,
				  અક્ષર **name)
{
	સંરચના inode_private_data 	*data;
	errcode_t			retval;

	જો ((retval = ext2fs_get_mem(કદ(સંરચના inode_private_data),
				     &data)))
		પાછા retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	જો (inode) {
		memcpy(&data->inode, inode, કદ(સંરચના ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = ટોચ_intern;
	ટોચ_intern = data;
	*name = data->name;
	પાછા 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 અક્ષર **name)
{
	પાછા ext2fs_inode_io_intern2(fs, ino, ખાલી, name);
}


સ્થિર errcode_t inode_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	io_channel	io = ખાલી;
	સંરચના inode_private_data *prev, *data = ખાલી;
	errcode_t	retval;
	પૂર્ણાંક		open_flags;

	જો (name == 0)
		પાછા EXT2_ET_BAD_DEVICE_NAME;

	માટે (data = ટોચ_intern, prev = ખાલી; data;
	     prev = data, data = data->next)
		જો (strcmp(name, data->name) == 0)
			તોડો;
	જો (!data)
		પાછા ENOENT;
	જો (prev)
		prev->next = data->next;
	નહીંતર
		ટોચ_intern = data->next;

	retval = ext2fs_get_mem(કદ(સંરચના struct_io_channel), &io);
	જો (retval)
		જાઓ cleanup;
	memset(io, 0, કદ(સંરચના struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	જો (retval)
		જાઓ cleanup;

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
	જો (retval)
		જાઓ cleanup;

	*channel = io;
	પાછા 0;

cleanup:
	જો (io && io->name)
		ext2fs_free_mem(&io->name);
	જો (data)
		ext2fs_free_mem(&data);
	જો (io)
		ext2fs_free_mem(&io);
	પાછા retval;
}

સ્થિર errcode_t inode_close(io_channel channel)
{
	સંરચના inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	જો (--channel->refcount > 0)
		પાછા 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	જો (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	પાછા retval;
}

સ્થિર errcode_t inode_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	સંરચના inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	પાછા 0;
}


સ્થિર errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	સંરચના inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	જો ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		પાછા retval;

	ગણતરી = (ગણતરી < 0) ? -ગણતરી : (ગણતરી * channel->block_size);

	પાછા ext2fs_file_read(data->file, buf, ગણતરી, 0);
}

સ્થિર errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પાછા inode_read_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	જો ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		પાછા retval;

	ગણતરી = (ગણતરી < 0) ? -ગણતરી : (ગણતરી * channel->block_size);

	પાછા ext2fs_file_write(data->file, buf, ગણતરી, 0);
}

સ્થિર errcode_t inode_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પાછા inode_write_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 પૂર્ણાંક size, અચલ શૂન્ય *buf)
{
	સંરચના inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	જો ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		પાછા retval;

	પાછા ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
સ્થિર errcode_t inode_flush(io_channel channel)
{
	સંરચના inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	પાછા ext2fs_file_flush(data->file);
}

