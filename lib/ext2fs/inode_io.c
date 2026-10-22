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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)

கட்டமைப்பு inode_private_data {
	முழு				magic;
	எழுத்து				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	கட்டமைப்பு ext2_inode		inode;
	முழு				flags;
	கட்டமைப்பு inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

நிலையான கட்டமைப்பு inode_private_data *உச்சி_intern;
நிலையான முழு ino_unique = 0;

நிலையான errcode_t inode_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel);
நிலையான errcode_t inode_close(io_channel channel);
நிலையான errcode_t inode_set_blksize(io_channel channel, முழு blksize);
நிலையான errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *data);
நிலையான errcode_t inode_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *data);
நிலையான errcode_t inode_flush(io_channel channel);
நிலையான errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				முழு size, மாறிலி வெறுமை *data);
நிலையான errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, முழு மொத்தம், வெறுமை *data);
நிலையான errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, முழு மொத்தம், மாறிலி வெறுமை *data);

நிலையான கட்டமைப்பு struct_io_manager struct_inode_manager = {
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
				  கட்டமைப்பு ext2_inode *inode,
				  எழுத்து **name)
{
	கட்டமைப்பு inode_private_data 	*data;
	errcode_t			retval;

	எனில் ((retval = ext2fs_get_mem(அளவு(கட்டமைப்பு inode_private_data),
				     &data)))
		திரும்பு retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	எனில் (inode) {
		memcpy(&data->inode, inode, அளவு(கட்டமைப்பு ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = உச்சி_intern;
	உச்சி_intern = data;
	*name = data->name;
	திரும்பு 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 எழுத்து **name)
{
	திரும்பு ext2fs_inode_io_intern2(fs, ino, வெற்று, name);
}


நிலையான errcode_t inode_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	io_channel	io = வெற்று;
	கட்டமைப்பு inode_private_data *prev, *data = வெற்று;
	errcode_t	retval;
	முழு		open_flags;

	எனில் (name == 0)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;

	க்கு (data = உச்சி_intern, prev = வெற்று; data;
	     prev = data, data = data->next)
		எனில் (strcmp(name, data->name) == 0)
			நிறுத்து;
	எனில் (!data)
		திரும்பு ENOENT;
	எனில் (prev)
		prev->next = data->next;
	இல்லையெனில்
		உச்சி_intern = data->next;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_io_channel), &io);
	எனில் (retval)
		செல் cleanup;
	memset(io, 0, அளவு(கட்டமைப்பு struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	எனில் (retval)
		செல் cleanup;

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
	எனில் (retval)
		செல் cleanup;

	*channel = io;
	திரும்பு 0;

cleanup:
	எனில் (io && io->name)
		ext2fs_free_mem(&io->name);
	எனில் (data)
		ext2fs_free_mem(&data);
	எனில் (io)
		ext2fs_free_mem(&io);
	திரும்பு retval;
}

நிலையான errcode_t inode_close(io_channel channel)
{
	கட்டமைப்பு inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
		திரும்பு 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	எனில் (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	திரும்பு retval;
}

நிலையான errcode_t inode_set_blksize(io_channel channel, முழு blksize)
{
	கட்டமைப்பு inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	திரும்பு 0;
}


நிலையான errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, முழு மொத்தம், வெறுமை *buf)
{
	கட்டமைப்பு inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	எனில் ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		திரும்பு retval;

	மொத்தம் = (மொத்தம் < 0) ? -மொத்தம் : (மொத்தம் * channel->block_size);

	திரும்பு ext2fs_file_read(data->file, buf, மொத்தம், 0);
}

நிலையான errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	திரும்பு inode_read_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	எனில் ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		திரும்பு retval;

	மொத்தம் = (மொத்தம் < 0) ? -மொத்தம் : (மொத்தம் * channel->block_size);

	திரும்பு ext2fs_file_write(data->file, buf, மொத்தம், 0);
}

நிலையான errcode_t inode_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	திரும்பு inode_write_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 முழு size, மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	எனில் ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		திரும்பு retval;

	திரும்பு ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
நிலையான errcode_t inode_flush(io_channel channel)
{
	கட்டமைப்பு inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	திரும்பு ext2fs_file_flush(data->file);
}

