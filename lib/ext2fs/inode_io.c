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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)

ସଂରଚନା inode_private_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				magic;
	ଅକ୍ଷର				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ସଂରଚନା ext2_inode		inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				flags;
	ସଂରଚନା inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

ସ୍ଥିର ସଂରଚନା inode_private_data *ଶୀର୍ଷ_intern;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ino_unique = 0;

ସ୍ଥିର errcode_t inode_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel);
ସ୍ଥିର errcode_t inode_close(io_channel channel);
ସ୍ଥିର errcode_t inode_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize);
ସ୍ଥିର errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t inode_flush(io_channel channel);
ସ୍ଥିର errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅଚଳ ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *data);

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_inode_manager = {
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
				  ସଂରଚନା ext2_inode *inode,
				  ଅକ୍ଷର **name)
{
	ସଂରଚନା inode_private_data 	*data;
	errcode_t			retval;

	ଯଦି ((retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା inode_private_data),
				     &data)))
		ଫେରନ୍ତୁ retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	ଯଦି (inode) {
		memcpy(&data->inode, inode, ଆକାର(ସଂରଚନା ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = ଶୀର୍ଷ_intern;
	ଶୀର୍ଷ_intern = data;
	*name = data->name;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 ଅକ୍ଷର **name)
{
	ଫେରନ୍ତୁ ext2fs_inode_io_intern2(fs, ino, ଖାଲି, name);
}


ସ୍ଥିର errcode_t inode_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	io_channel	io = ଖାଲି;
	ସଂରଚନା inode_private_data *prev, *data = ଖାଲି;
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		open_flags;

	ଯଦି (name == 0)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;

	ପାଇଁ (data = ଶୀର୍ଷ_intern, prev = ଖାଲି; data;
	     prev = data, data = data->next)
		ଯଦି (strcmp(name, data->name) == 0)
			ଭାଙ୍ଗ;
	ଯଦି (!data)
		ଫେରନ୍ତୁ ENOENT;
	ଯଦି (prev)
		prev->next = data->next;
	ଅନ୍ୟଥା
		ଶୀର୍ଷ_intern = data->next;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_io_channel), &io);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(io, 0, ଆକାର(ସଂରଚନା struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ଯଦି (retval)
		ଯାଅ cleanup;

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
	ଯଦି (retval)
		ଯାଅ cleanup;

	*channel = io;
	ଫେରନ୍ତୁ 0;

cleanup:
	ଯଦି (io && io->name)
		ext2fs_free_mem(&io->name);
	ଯଦି (data)
		ext2fs_free_mem(&data);
	ଯଦି (io)
		ext2fs_free_mem(&io);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t inode_close(io_channel channel)
{
	ସଂରଚନା inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
		ଫେରନ୍ତୁ 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	ଯଦି (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t inode_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	ସଂରଚନା inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	ଫେରନ୍ତୁ 0;
}


ସ୍ଥିର errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ଯଦି ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ଫେରନ୍ତୁ retval;

	ଗଣନା_2 = (ଗଣନା_2 < 0) ? -ଗଣନା_2 : (ଗଣନା_2 * channel->block_size);

	ଫେରନ୍ତୁ ext2fs_file_read(data->file, buf, ଗଣନା_2, 0);
}

ସ୍ଥିର errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ inode_read_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ଯଦି ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		ଫେରନ୍ତୁ retval;

	ଗଣନା_2 = (ଗଣନା_2 < 0) ? -ଗଣନା_2 : (ଗଣନା_2 * channel->block_size);

	ଫେରନ୍ତୁ ext2fs_file_write(data->file, buf, ଗଣନା_2, 0);
}

ସ୍ଥିର errcode_t inode_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ inode_write_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ଯଦି ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
ସ୍ଥିର errcode_t inode_flush(io_channel channel)
{
	ସଂରଚନା inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ଫେରନ୍ତୁ ext2fs_file_flush(data->file);
}

