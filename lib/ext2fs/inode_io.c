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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) প্রত্যাগম (code)

সংরচনা inode_private_data {
	পূর্ণাঙ্ক				magic;
	বর্ণ				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	সংরচনা ext2_inode		inode;
	পূর্ণাঙ্ক				flags;
	সংরচনা inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

স্থির সংরচনা inode_private_data *শীর্ষ_intern;
স্থির পূর্ণাঙ্ক ino_unique = 0;

স্থির errcode_t inode_open(ধ্রুব বর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel);
স্থির errcode_t inode_close(io_channel channel);
স্থির errcode_t inode_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize);
স্থির errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *data);
স্থির errcode_t inode_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
স্থির errcode_t inode_flush(io_channel channel);
স্থির errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				পূর্ণাঙ্ক size, ধ্রুব শূন্য *data);
স্থির errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, পূর্ণাঙ্ক গুনতি, শূন্য *data);
স্থির errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);

স্থির সংরচনা struct_io_manager struct_inode_manager = {
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
				  সংরচনা ext2_inode *inode,
				  বর্ণ **name)
{
	সংরচনা inode_private_data 	*data;
	errcode_t			retval;

	যদি ((retval = ext2fs_get_mem(পরিমাণ(সংরচনা inode_private_data),
				     &data)))
		প্রত্যাগম retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	যদি (inode) {
		memcpy(&data->inode, inode, পরিমাণ(সংরচনা ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = শীর্ষ_intern;
	শীর্ষ_intern = data;
	*name = data->name;
	প্রত্যাগম 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 বর্ণ **name)
{
	প্রত্যাগম ext2fs_inode_io_intern2(fs, ino, রিক্ত, name);
}


স্থির errcode_t inode_open(ধ্রুব বর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	io_channel	io = রিক্ত;
	সংরচনা inode_private_data *prev, *data = রিক্ত;
	errcode_t	retval;
	পূর্ণাঙ্ক		open_flags;

	যদি (name == 0)
		প্রত্যাগম EXT2_ET_BAD_DEVICE_NAME;

	প্রতি (data = শীর্ষ_intern, prev = রিক্ত; data;
	     prev = data, data = data->next)
		যদি (strcmp(name, data->name) == 0)
			ভঙ্গ;
	যদি (!data)
		প্রত্যাগম ENOENT;
	যদি (prev)
		prev->next = data->next;
	অন্যথা
		শীর্ষ_intern = data->next;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা struct_io_channel), &io);
	যদি (retval)
		গচ্ছ cleanup;
	memset(io, 0, পরিমাণ(সংরচনা struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		গচ্ছ cleanup;

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
	যদি (retval)
		গচ্ছ cleanup;

	*channel = io;
	প্রত্যাগম 0;

cleanup:
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (data)
		ext2fs_free_mem(&data);
	যদি (io)
		ext2fs_free_mem(&io);
	প্রত্যাগম retval;
}

স্থির errcode_t inode_close(io_channel channel)
{
	সংরচনা inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		প্রত্যাগম 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	প্রত্যাগম retval;
}

স্থির errcode_t inode_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	সংরচনা inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	প্রত্যাগম 0;
}


স্থির errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	সংরচনা inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		প্রত্যাগম retval;

	গুনতি = (গুনতি < 0) ? -গুনতি : (গুনতি * channel->block_size);

	প্রত্যাগম ext2fs_file_read(data->file, buf, গুনতি, 0);
}

স্থির errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	প্রত্যাগম inode_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf)
{
	সংরচনা inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		প্রত্যাগম retval;

	গুনতি = (গুনতি < 0) ? -গুনতি : (গুনতি * channel->block_size);

	প্রত্যাগম ext2fs_file_write(data->file, buf, গুনতি, 0);
}

স্থির errcode_t inode_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf)
{
	প্রত্যাগম inode_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণাঙ্ক size, ধ্রুব শূন্য *buf)
{
	সংরচনা inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		প্রত্যাগম retval;

	প্রত্যাগম ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t inode_flush(io_channel channel)
{
	সংরচনা inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	প্রত্যাগম ext2fs_file_flush(data->file);
}

