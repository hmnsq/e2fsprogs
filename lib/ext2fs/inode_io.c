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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)

بنية inode_خصوصی_داده {
	صحيح				magic;
	حرف				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	بنية ext2_inode		inode;
	صحيح				flags;
	بنية inode_خصوصی_داده	*بعدی;
};

#تعریف_کن CHANNEL_HAS_INODE	0x8000

ساكن بنية inode_خصوصی_داده *بالاترین_intern;
ساكن صحيح ino_unique = 0;

ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t inode_close(io_channel channel);
ساكن errcode_t inode_مجموعه_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t inode_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *داده);
ساكن errcode_t inode_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *داده);
ساكن errcode_t inode_flush(io_channel channel);
ساكن errcode_t inode_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				صحيح اندازه, ثابت فراغ *داده);
ساكن errcode_t inode_read_blk64(io_channel channel,
				بیعلامت دراز دراز بلوک, صحيح شمار, فراغ *داده);
ساكن errcode_t inode_write_blk64(io_channel channel,
				بیعلامت دراز دراز بلوک, صحيح شمار, ثابت فراغ *داده);

ساكن بنية struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.مجموعه_blksize	= inode_مجموعه_blksize,
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
	بنية inode_خصوصی_داده 	*داده;
	errcode_t			retval;

	إذا ((retval = ext2fs_get_mem(حجم(بنية inode_خصوصی_داده),
				     &داده)))
		أرجع retval;
	داده->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(داده->name, "%u:%d", ino, ino_unique++);
	داده->file = 0;
	داده->fs = fs;
	داده->ino = ino;
	داده->flags = 0;
	إذا (inode) {
		کپی_حافظه(&داده->inode, inode, حجم(بنية ext2_inode));
		داده->flags |= CHANNEL_HAS_INODE;
	}
	داده->بعدی = بالاترین_intern;
	بالاترین_intern = داده;
	*name = داده->name;
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
	بنية inode_خصوصی_داده *prev, *داده = عدم;
	errcode_t	retval;
	صحيح		open_flags;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	لكل (داده = بالاترین_intern, prev = عدم; داده;
	     prev = داده, داده = داده->بعدی)
		إذا (strcmp(name, داده->name) == 0)
			اكسر;
	إذا (!داده)
		أرجع ENOENT;
	إذا (prev)
		prev->بعدی = داده->بعدی;
	وإلا
		بالاترین_intern = داده->بعدی;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(طول_رشته(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->خصوصی_داده = داده;
	io->بلوک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(داده->fs, داده->ino,
				   (داده->flags & CHANNEL_HAS_INODE) ?
				   &داده->inode : 0, open_flags,
				   &داده->file);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (داده)
		ext2fs_free_mem(&داده);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t inode_close(io_channel channel)
{
	بنية inode_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	retval = ext2fs_file_close(داده->file);

	ext2fs_free_mem(&channel->خصوصی_داده);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t inode_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	بنية inode_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->بلوک_اندازه = blksize;
	أرجع 0;
}


ساكن errcode_t inode_read_blk64(io_channel channel,
				بیعلامت دراز دراز بلوک, صحيح شمار, فراغ *buf)
{
	بنية inode_خصوصی_داده *داده;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(داده->file,
				(ext2_off64_t)(بلوک * channel->بلوک_اندازه),
				EXT2_SEEK_مجموعه, 0)))
		أرجع retval;

	شمار = (شمار < 0) ? -شمار : (شمار * channel->بلوک_اندازه);

	أرجع ext2fs_file_read(داده->file, buf, شمار, 0);
}

ساكن errcode_t inode_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	أرجع inode_read_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t inode_write_blk64(io_channel channel,
				بیعلامت دراز دراز بلوک, صحيح شمار, ثابت فراغ *buf)
{
	بنية inode_خصوصی_داده *داده;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(داده->file,
				(ext2_off64_t) (بلوک * channel->بلوک_اندازه),
				EXT2_SEEK_مجموعه, 0)))
		أرجع retval;

	شمار = (شمار < 0) ? -شمار : (شمار * channel->بلوک_اندازه);

	أرجع ext2fs_file_write(داده->file, buf, شمار, 0);
}

ساكن errcode_t inode_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	أرجع inode_write_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t inode_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				 صحيح اندازه, ثابت فراغ *buf)
{
	بنية inode_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_lseek(داده->file, جابه_جایی,
					EXT2_SEEK_مجموعه, 0)))
		أرجع retval;

	أرجع ext2fs_file_write(داده->file, buf, اندازه, 0);
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t inode_flush(io_channel channel)
{
	بنية inode_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية inode_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	أرجع ext2fs_file_flush(داده->file);
}

