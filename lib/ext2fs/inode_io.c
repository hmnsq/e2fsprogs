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

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)

কাঠামো inode_private_তথ্য {
	পূর্ণসংখ্যা				magic;
	অক্ষর				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	কাঠামো ext2_inode		inode;
	পূর্ণসংখ্যা				flags;
	কাঠামো inode_private_তথ্য	*পরবর্তী;
};

#define CHANNEL_HAS_INODE	0x8000

স্থির কাঠামো inode_private_তথ্য *শীর্ষ_intern;
স্থির পূর্ণসংখ্যা ino_unique = 0;

স্থির errcode_t inode_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel);
স্থির errcode_t inode_close(io_channel channel);
স্থির errcode_t inode_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize);
স্থির errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *তথ্য);
স্থির errcode_t inode_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *তথ্য);
স্থির errcode_t inode_flush(io_channel channel);
স্থির errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				পূর্ণসংখ্যা আকার_2, ধ্রুবক শূন্য *তথ্য);
স্থির errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, পূর্ণসংখ্যা গুনতি, শূন্য *তথ্য);
স্থির errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *তথ্য);

স্থির কাঠামো struct_io_manager struct_inode_manager = {
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
				  কাঠামো ext2_inode *inode,
				  অক্ষর **name)
{
	কাঠামো inode_private_তথ্য 	*তথ্য;
	errcode_t			retval;

	যদি ((retval = ext2fs_get_mem(আকার(কাঠামো inode_private_তথ্য),
				     &তথ্য)))
		ফেরত retval;
	তথ্য->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(তথ্য->name, "%u:%d", ino, ino_unique++);
	তথ্য->file = 0;
	তথ্য->fs = fs;
	তথ্য->ino = ino;
	তথ্য->flags = 0;
	যদি (inode) {
		memcpy(&তথ্য->inode, inode, আকার(কাঠামো ext2_inode));
		তথ্য->flags |= CHANNEL_HAS_INODE;
	}
	তথ্য->পরবর্তী = শীর্ষ_intern;
	শীর্ষ_intern = তথ্য;
	*name = তথ্য->name;
	ফেরত 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 অক্ষর **name)
{
	ফেরত ext2fs_inode_io_intern2(fs, ino, ফাঁকা, name);
}


স্থির errcode_t inode_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	io_channel	io = ফাঁকা;
	কাঠামো inode_private_তথ্য *পূর্ব, *তথ্য = ফাঁকা;
	errcode_t	retval;
	পূর্ণসংখ্যা		open_flags;

	যদি (name == 0)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;

	জন্য (তথ্য = শীর্ষ_intern, পূর্ব = ফাঁকা; তথ্য;
	     পূর্ব = তথ্য, তথ্য = তথ্য->পরবর্তী)
		যদি (strcmp(name, তথ্য->name) == 0)
			বিরতি;
	যদি (!তথ্য)
		ফেরত ENOENT;
	যদি (পূর্ব)
		পূর্ব->পরবর্তী = তথ্য->পরবর্তী;
	অন্যথা
		শীর্ষ_intern = তথ্য->পরবর্তী;

	retval = ext2fs_get_mem(আকার(কাঠামো struct_io_channel), &io);
	যদি (retval)
		যাও cleanup;
	memset(io, 0, আকার(কাঠামো struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		যাও cleanup;

	strcpy(io->name, name);
	io->private_তথ্য = তথ্য;
	io->block_আকার = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(তথ্য->fs, তথ্য->ino,
				   (তথ্য->flags & CHANNEL_HAS_INODE) ?
				   &তথ্য->inode : 0, open_flags,
				   &তথ্য->file);
	যদি (retval)
		যাও cleanup;

	*channel = io;
	ফেরত 0;

cleanup:
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (তথ্য)
		ext2fs_free_mem(&তথ্য);
	যদি (io)
		ext2fs_free_mem(&io);
	ফেরত retval;
}

স্থির errcode_t inode_close(io_channel channel)
{
	কাঠামো inode_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ফেরত 0;

	retval = ext2fs_file_close(তথ্য->file);

	ext2fs_free_mem(&channel->private_তথ্য);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ফেরত retval;
}

স্থির errcode_t inode_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	কাঠামো inode_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_আকার = blksize;
	ফেরত 0;
}


স্থির errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	কাঠামো inode_private_তথ্য *তথ্য;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_llseek(তথ্য->file,
				(ext2_off64_t)(block * channel->block_আকার),
				EXT2_SEEK_SET, 0)))
		ফেরত retval;

	গুনতি = (গুনতি < 0) ? -গুনতি : (গুনতি * channel->block_আকার);

	ফেরত ext2fs_file_read(তথ্য->file, buf, গুনতি, 0);
}

স্থির errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	ফেরত inode_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো inode_private_তথ্য *তথ্য;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_llseek(তথ্য->file,
				(ext2_off64_t) (block * channel->block_আকার),
				EXT2_SEEK_SET, 0)))
		ফেরত retval;

	গুনতি = (গুনতি < 0) ? -গুনতি : (গুনতি * channel->block_আকার);

	ফেরত ext2fs_file_write(তথ্য->file, buf, গুনতি, 0);
}

স্থির errcode_t inode_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ফেরত inode_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণসংখ্যা আকার_2, ধ্রুবক শূন্য *buf)
{
	কাঠামো inode_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	যদি ((retval = ext2fs_file_lseek(তথ্য->file, offset,
					EXT2_SEEK_SET, 0)))
		ফেরত retval;

	ফেরত ext2fs_file_write(তথ্য->file, buf, আকার_2, 0);
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t inode_flush(io_channel channel)
{
	কাঠামো inode_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো inode_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ফেরত ext2fs_file_flush(তথ্য->file);
}

