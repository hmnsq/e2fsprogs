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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

بنية inode_نجی_انکڑے {
	صحيح				magic;
	حرف				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	بنية ext2_inode		inode;
	صحيح				flags;
	بنية inode_نجی_انکڑے	*اگلا;
};

#تعریف_کرو CHANNEL_HAS_INODE	0x8000

ساكن بنية inode_نجی_انکڑے *چوٹی_intern;
ساكن صحيح ino_unique = 0;

ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t inode_close(io_channel channel);
ساكن errcode_t inode_سیٹ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t inode_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *انکڑے);
ساكن errcode_t inode_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *انکڑے);
ساكن errcode_t inode_flush(io_channel channel);
ساكن errcode_t inode_write_byte(io_channel channel, بنانشانی لما وتھ,
				صحيح ناپ, ثابت فراغ *انکڑے);
ساكن errcode_t inode_read_blk64(io_channel channel,
				بنانشانی لما لما بلاک, صحيح گنتی, فراغ *انکڑے);
ساكن errcode_t inode_write_blk64(io_channel channel,
				بنانشانی لما لما بلاک, صحيح گنتی, ثابت فراغ *انکڑے);

ساكن بنية struct_io_manager struct_inode_manager = {
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
				  بنية ext2_inode *inode,
				  حرف **name)
{
	بنية inode_نجی_انکڑے 	*انکڑے;
	errcode_t			retval;

	إذا ((retval = ext2fs_get_mem(حجم(بنية inode_نجی_انکڑے),
				     &انکڑے)))
		أرجع retval;
	انکڑے->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(انکڑے->name, "%u:%d", ino, ino_unique++);
	انکڑے->file = 0;
	انکڑے->fs = fs;
	انکڑے->ino = ino;
	انکڑے->flags = 0;
	إذا (inode) {
		حافظہ_نقل(&انکڑے->inode, inode, حجم(بنية ext2_inode));
		انکڑے->flags |= CHANNEL_HAS_INODE;
	}
	انکڑے->اگلا = چوٹی_intern;
	چوٹی_intern = انکڑے;
	*name = انکڑے->name;
	أرجع 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 حرف **name)
{
	أرجع ext2fs_inode_io_intern2(fs, ino, عدم, name);
}


ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية inode_نجی_انکڑے *prev, *انکڑے = عدم;
	errcode_t	retval;
	صحيح		open_flags;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	لكل (انکڑے = چوٹی_intern, prev = عدم; انکڑے;
	     prev = انکڑے, انکڑے = انکڑے->اگلا)
		إذا (strcmp(name, انکڑے->name) == 0)
			اكسر;
	إذا (!انکڑے)
		أرجع ENOENT;
	إذا (prev)
		prev->اگلا = انکڑے->اگلا;
	وإلا
		چوٹی_intern = انکڑے->اگلا;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->نجی_انکڑے = انکڑے;
	io->بلاک_ناپ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(انکڑے->fs, انکڑے->ino,
				   (انکڑے->flags & CHANNEL_HAS_INODE) ?
				   &انکڑے->inode : 0, open_flags,
				   &انکڑے->file);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (انکڑے)
		ext2fs_free_mem(&انکڑے);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t inode_close(io_channel channel)
{
	بنية inode_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	retval = ext2fs_file_close(انکڑے->file);

	ext2fs_free_mem(&channel->نجی_انکڑے);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t inode_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	بنية inode_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->بلاک_ناپ = blksize;
	أرجع 0;
}


ساكن errcode_t inode_read_blk64(io_channel channel,
				بنانشانی لما لما بلاک, صحيح گنتی, فراغ *buf)
{
	بنية inode_نجی_انکڑے *انکڑے;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(انکڑے->file,
				(ext2_off64_t)(بلاک * channel->بلاک_ناپ),
				EXT2_SEEK_سیٹ, 0)))
		أرجع retval;

	گنتی = (گنتی < 0) ? -گنتی : (گنتی * channel->بلاک_ناپ);

	أرجع ext2fs_file_read(انکڑے->file, buf, گنتی, 0);
}

ساكن errcode_t inode_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	أرجع inode_read_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t inode_write_blk64(io_channel channel,
				بنانشانی لما لما بلاک, صحيح گنتی, ثابت فراغ *buf)
{
	بنية inode_نجی_انکڑے *انکڑے;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(انکڑے->file,
				(ext2_off64_t) (بلاک * channel->بلاک_ناپ),
				EXT2_SEEK_سیٹ, 0)))
		أرجع retval;

	گنتی = (گنتی < 0) ? -گنتی : (گنتی * channel->بلاک_ناپ);

	أرجع ext2fs_file_write(انکڑے->file, buf, گنتی, 0);
}

ساكن errcode_t inode_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	أرجع inode_write_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t inode_write_byte(io_channel channel, بنانشانی لما وتھ,
				 صحيح ناپ, ثابت فراغ *buf)
{
	بنية inode_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_lseek(انکڑے->file, وتھ,
					EXT2_SEEK_سیٹ, 0)))
		أرجع retval;

	أرجع ext2fs_file_write(انکڑے->file, buf, ناپ, 0);
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t inode_flush(io_channel channel)
{
	بنية inode_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية inode_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	أرجع ext2fs_file_flush(انکڑے->file);
}

