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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)

بنية inode_ذاتي_معلومات {
	صحيح				magic;
	حرف				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	بنية ext2_inode		inode;
	صحيح				flags;
	بنية inode_ذاتي_معلومات	*ايندڙ;
};

#وصف_ڪريو CHANNEL_HAS_INODE	0x8000

ساكن بنية inode_ذاتي_معلومات *چوٽي_intern;
ساكن صحيح ino_unique = 0;

ساكن errcode_t inode_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t inode_close(io_channel channel);
ساكن errcode_t inode_سيٽ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t inode_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *معلومات);
ساكن errcode_t inode_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *معلومات);
ساكن errcode_t inode_flush(io_channel channel);
ساكن errcode_t inode_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				صحيح سائيز, ثابت فراغ *معلومات);
ساكن errcode_t inode_read_blk64(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, فراغ *معلومات);
ساكن errcode_t inode_write_blk64(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, ثابت فراغ *معلومات);

ساكن بنية struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.سيٽ_blksize	= inode_سيٽ_blksize,
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
	بنية inode_ذاتي_معلومات 	*معلومات;
	errcode_t			retval;

	إذا ((retval = ext2fs_get_mem(حجم(بنية inode_ذاتي_معلومات),
				     &معلومات)))
		أرجع retval;
	معلومات->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(معلومات->name, "%u:%d", ino, ino_unique++);
	معلومات->file = 0;
	معلومات->fs = fs;
	معلومات->ino = ino;
	معلومات->flags = 0;
	إذا (inode) {
		يادگيري_نقل(&معلومات->inode, inode, حجم(بنية ext2_inode));
		معلومات->flags |= CHANNEL_HAS_INODE;
	}
	معلومات->ايندڙ = چوٽي_intern;
	چوٽي_intern = معلومات;
	*name = معلومات->name;
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
	بنية inode_ذاتي_معلومات *prev, *معلومات = عدم;
	errcode_t	retval;
	صحيح		open_flags;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	لكل (معلومات = چوٽي_intern, prev = عدم; معلومات;
	     prev = معلومات, معلومات = معلومات->ايندڙ)
		إذا (strcmp(name, معلومات->name) == 0)
			اكسر;
	إذا (!معلومات)
		أرجع ENOENT;
	إذا (prev)
		prev->ايندڙ = معلومات->ايندڙ;
	وإلا
		چوٽي_intern = معلومات->ايندڙ;

	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(لڙي_ڊيگهه(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->ذاتي_معلومات = معلومات;
	io->بلاڪ_سائيز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(معلومات->fs, معلومات->ino,
				   (معلومات->flags & CHANNEL_HAS_INODE) ?
				   &معلومات->inode : 0, open_flags,
				   &معلومات->file);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (معلومات)
		ext2fs_free_mem(&معلومات);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t inode_close(io_channel channel)
{
	بنية inode_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	retval = ext2fs_file_close(معلومات->file);

	ext2fs_free_mem(&channel->ذاتي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t inode_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	بنية inode_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->بلاڪ_سائيز = blksize;
	أرجع 0;
}


ساكن errcode_t inode_read_blk64(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, فراغ *buf)
{
	بنية inode_ذاتي_معلومات *معلومات;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(معلومات->file,
				(ext2_off64_t)(بلاڪ * channel->بلاڪ_سائيز),
				EXT2_SEEK_سيٽ, 0)))
		أرجع retval;

	ڳڻپ = (ڳڻپ < 0) ? -ڳڻپ : (ڳڻپ * channel->بلاڪ_سائيز);

	أرجع ext2fs_file_read(معلومات->file, buf, ڳڻپ, 0);
}

ساكن errcode_t inode_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	أرجع inode_read_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t inode_write_blk64(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, ثابت فراغ *buf)
{
	بنية inode_ذاتي_معلومات *معلومات;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_llseek(معلومات->file,
				(ext2_off64_t) (بلاڪ * channel->بلاڪ_سائيز),
				EXT2_SEEK_سيٽ, 0)))
		أرجع retval;

	ڳڻپ = (ڳڻپ < 0) ? -ڳڻپ : (ڳڻپ * channel->بلاڪ_سائيز);

	أرجع ext2fs_file_write(معلومات->file, buf, ڳڻپ, 0);
}

ساكن errcode_t inode_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	أرجع inode_write_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t inode_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				 صحيح سائيز, ثابت فراغ *buf)
{
	بنية inode_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	إذا ((retval = ext2fs_file_lseek(معلومات->file, وٿي,
					EXT2_SEEK_سيٽ, 0)))
		أرجع retval;

	أرجع ext2fs_file_write(معلومات->file, buf, سائيز, 0);
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t inode_flush(io_channel channel)
{
	بنية inode_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية inode_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	أرجع ext2fs_file_flush(معلومات->file);
}

