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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)

ساخت inode_نجی_ڈیٹا {
	صحیح				magic;
	حرف				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ساخت ext2_inode		inode;
	صحیح				flags;
	ساخت inode_نجی_ڈیٹا	*اگلا;
};

#تعریف_کرو CHANNEL_HAS_INODE	0x8000

ساکن ساخت inode_نجی_ڈیٹا *چوٹی_intern;
ساکن صحیح ino_unique = 0;

ساکن errcode_t inode_open(ثابت حرف *name, صحیح flags, io_channel *channel);
ساکن errcode_t inode_close(io_channel channel);
ساکن errcode_t inode_سیٹ_blksize(io_channel channel, صحیح blksize);
ساکن errcode_t inode_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *ڈیٹا);
ساکن errcode_t inode_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *ڈیٹا);
ساکن errcode_t inode_flush(io_channel channel);
ساکن errcode_t inode_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				صحیح سائز, ثابت خالی *ڈیٹا);
ساکن errcode_t inode_read_blk64(io_channel channel,
				غیرعلامتی طویل طویل بلاک, صحیح گنتی, خالی *ڈیٹا);
ساکن errcode_t inode_write_blk64(io_channel channel,
				غیرعلامتی طویل طویل بلاک, صحیح گنتی, ثابت خالی *ڈیٹا);

ساکن ساخت struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.سیٹ_blksize	= inode_سیٹ_blksize,
	.read_blk	= inode_read_blk,
	.write_blk	= inode_write_blk,
	.flush		= inode_flush,
	.write_byte	= inode_write_byte,
	.read_blk64	= inode_read_blk64,
	.write_blk64	= inode_write_blk64
};

io_manager inode_io_manager = &struct_inode_manager;

errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
				  ساخت ext2_inode *inode,
				  حرف **name)
{
	ساخت inode_نجی_ڈیٹا 	*ڈیٹا;
	errcode_t			retval;

	اگر ((retval = ext2fs_get_mem(حجم(ساخت inode_نجی_ڈیٹا),
				     &ڈیٹا)))
		واپس retval;
	ڈیٹا->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(ڈیٹا->name, "%u:%d", ino, ino_unique++);
	ڈیٹا->file = 0;
	ڈیٹا->fs = fs;
	ڈیٹا->ino = ino;
	ڈیٹا->flags = 0;
	اگر (inode) {
		حافظہ_نقل(&ڈیٹا->inode, inode, حجم(ساخت ext2_inode));
		ڈیٹا->flags |= CHANNEL_HAS_INODE;
	}
	ڈیٹا->اگلا = چوٹی_intern;
	چوٹی_intern = ڈیٹا;
	*name = ڈیٹا->name;
	واپس 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 حرف **name)
{
	واپس ext2fs_inode_io_intern2(fs, ino, تہی, name);
}


ساکن errcode_t inode_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	io_channel	io = تہی;
	ساخت inode_نجی_ڈیٹا *پچھلا, *ڈیٹا = تہی;
	errcode_t	retval;
	صحیح		open_flags;

	اگر (name == 0)
		واپس EXT2_ET_BAD_DEVICE_NAME;

	برائے (ڈیٹا = چوٹی_intern, پچھلا = تہی; ڈیٹا;
	     پچھلا = ڈیٹا, ڈیٹا = ڈیٹا->اگلا)
		اگر (strcmp(name, ڈیٹا->name) == 0)
			توڑو;
	اگر (!ڈیٹا)
		واپس ENOENT;
	اگر (پچھلا)
		پچھلا->اگلا = ڈیٹا->اگلا;
	ورنہ
		چوٹی_intern = ڈیٹا->اگلا;

	retval = ext2fs_get_mem(حجم(ساخت struct_io_channel), &io);
	اگر (retval)
		جاؤ cleanup;
	memset(io, 0, حجم(ساخت struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(io->name, name);
	io->نجی_ڈیٹا = ڈیٹا;
	io->بلاک_سائز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(ڈیٹا->fs, ڈیٹا->ino,
				   (ڈیٹا->flags & CHANNEL_HAS_INODE) ?
				   &ڈیٹا->inode : 0, open_flags,
				   &ڈیٹا->file);
	اگر (retval)
		جاؤ cleanup;

	*channel = io;
	واپس 0;

cleanup:
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (ڈیٹا)
		ext2fs_free_mem(&ڈیٹا);
	اگر (io)
		ext2fs_free_mem(&io);
	واپس retval;
}

ساکن errcode_t inode_close(io_channel channel)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		واپس 0;

	retval = ext2fs_file_close(ڈیٹا->file);

	ext2fs_free_mem(&channel->نجی_ڈیٹا);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	واپس retval;
}

ساکن errcode_t inode_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->بلاک_سائز = blksize;
	واپس 0;
}


ساکن errcode_t inode_read_blk64(io_channel channel,
				غیرعلامتی طویل طویل بلاک, صحیح گنتی, خالی *buf)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_llseek(ڈیٹا->file,
				(ext2_off64_t)(بلاک * channel->بلاک_سائز),
				EXT2_SEEK_سیٹ, 0)))
		واپس retval;

	گنتی = (گنتی < 0) ? -گنتی : (گنتی * channel->بلاک_سائز);

	واپس ext2fs_file_read(ڈیٹا->file, buf, گنتی, 0);
}

ساکن errcode_t inode_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	واپس inode_read_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t inode_write_blk64(io_channel channel,
				غیرعلامتی طویل طویل بلاک, صحیح گنتی, ثابت خالی *buf)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_llseek(ڈیٹا->file,
				(ext2_off64_t) (بلاک * channel->بلاک_سائز),
				EXT2_SEEK_سیٹ, 0)))
		واپس retval;

	گنتی = (گنتی < 0) ? -گنتی : (گنتی * channel->بلاک_سائز);

	واپس ext2fs_file_write(ڈیٹا->file, buf, گنتی, 0);
}

ساکن errcode_t inode_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	واپس inode_write_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t inode_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				 صحیح سائز, ثابت خالی *buf)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	اگر ((retval = ext2fs_file_lseek(ڈیٹا->file, انحراف,
					EXT2_SEEK_سیٹ, 0)))
		واپس retval;

	واپس ext2fs_file_write(ڈیٹا->file, buf, سائز, 0);
}

/*
 * Flush data buffers to disk.
 */
ساکن errcode_t inode_flush(io_channel channel)
{
	ساخت inode_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت inode_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	واپس ext2fs_file_flush(ڈیٹا->file);
}

