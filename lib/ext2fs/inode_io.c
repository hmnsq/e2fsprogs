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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం				magic;
	అక్షరం				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	నిర్మాణం ext2_inode		inode;
	పూర్ణాంకం				flags;
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం	*తదుపరి;
};

#నిర్వచించు CHANNEL_HAS_INODE	0x8000

స్థిర నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *శిఖరం_intern;
స్థిర పూర్ణాంకం ino_unique = 0;

స్థిర errcode_t inode_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel);
స్థిర errcode_t inode_close(io_channel channel);
స్థిర errcode_t inode_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize);
స్థిర errcode_t inode_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
స్థిర errcode_t inode_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
స్థిర errcode_t inode_flush(io_channel channel);
స్థిర errcode_t inode_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				పూర్ణాంకం కొలత, స్థిరరాశి శూన్య *దత్తాంశం);
స్థిర errcode_t inode_read_blk64(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
స్థిర errcode_t inode_write_blk64(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);

స్థిర నిర్మాణం struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.సమితి_blksize	= inode_సమితి_blksize,
	.read_blk	= inode_read_blk,
	.write_blk	= inode_write_blk,
	.flush		= inode_flush,
	.write_byte	= inode_write_byte,
	.read_blk64	= inode_read_blk64,
	.write_blk64	= inode_write_blk64
};

io_manager inode_io_manager = &struct_inode_manager;

errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
				  నిర్మాణం ext2_inode *inode,
				  అక్షరం **name)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం 	*దత్తాంశం;
	errcode_t			retval;

	ఒకవేళ ((retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం),
				     &దత్తాంశం)))
		తిరిగి retval;
	దత్తాంశం->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(దత్తాంశం->name, "%u:%d", ino, ino_unique++);
	దత్తాంశం->file = 0;
	దత్తాంశం->fs = fs;
	దత్తాంశం->ino = ino;
	దత్తాంశం->flags = 0;
	ఒకవేళ (inode) {
		జ్ఞాపకం_నకలు(&దత్తాంశం->inode, inode, పరిమాణం(నిర్మాణం ext2_inode));
		దత్తాంశం->flags |= CHANNEL_HAS_INODE;
	}
	దత్తాంశం->తదుపరి = శిఖరం_intern;
	శిఖరం_intern = దత్తాంశం;
	*name = దత్తాంశం->name;
	తిరిగి 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 అక్షరం **name)
{
	తిరిగి ext2fs_inode_io_intern2(fs, ino, ఖాళీ, name);
}


స్థిర errcode_t inode_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	io_channel	io = ఖాళీ;
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *prev, *దత్తాంశం = ఖాళీ;
	errcode_t	retval;
	పూర్ణాంకం		open_flags;

	ఒకవేళ (name == 0)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;

	కొరకు (దత్తాంశం = శిఖరం_intern, prev = ఖాళీ; దత్తాంశం;
	     prev = దత్తాంశం, దత్తాంశం = దత్తాంశం->తదుపరి)
		ఒకవేళ (strcmp(name, దత్తాంశం->name) == 0)
			ఆపు;
	ఒకవేళ (!దత్తాంశం)
		తిరిగి ENOENT;
	ఒకవేళ (prev)
		prev->తదుపరి = దత్తాంశం->తదుపరి;
	లేకపోతే
		శిఖరం_intern = దత్తాంశం->తదుపరి;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_io_channel), &io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(io, 0, పరిమాణం(నిర్మాణం struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &io->name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(io->name, name);
	io->వ్యక్తిగత_దత్తాంశం = దత్తాంశం;
	io->ఖండం_కొలత = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(దత్తాంశం->fs, దత్తాంశం->ino,
				   (దత్తాంశం->flags & CHANNEL_HAS_INODE) ?
				   &దత్తాంశం->inode : 0, open_flags,
				   &దత్తాంశం->file);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	*channel = io;
	తిరిగి 0;

cleanup:
	ఒకవేళ (io && io->name)
		ext2fs_free_mem(&io->name);
	ఒకవేళ (దత్తాంశం)
		ext2fs_free_mem(&దత్తాంశం);
	ఒకవేళ (io)
		ext2fs_free_mem(&io);
	తిరిగి retval;
}

స్థిర errcode_t inode_close(io_channel channel)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
		తిరిగి 0;

	retval = ext2fs_file_close(దత్తాంశం->file);

	ext2fs_free_mem(&channel->వ్యక్తిగత_దత్తాంశం);
	ఒకవేళ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	తిరిగి retval;
}

స్థిర errcode_t inode_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->ఖండం_కొలత = blksize;
	తిరిగి 0;
}


స్థిర errcode_t inode_read_blk64(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, శూన్య *buf)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ఒకవేళ ((retval = ext2fs_file_llseek(దత్తాంశం->file,
				(ext2_off64_t)(ఖండం * channel->ఖండం_కొలత),
				EXT2_SEEK_సమితి, 0)))
		తిరిగి retval;

	లెక్క = (లెక్క < 0) ? -లెక్క : (లెక్క * channel->ఖండం_కొలత);

	తిరిగి ext2fs_file_read(దత్తాంశం->file, buf, లెక్క, 0);
}

స్థిర errcode_t inode_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	తిరిగి inode_read_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t inode_write_blk64(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ఒకవేళ ((retval = ext2fs_file_llseek(దత్తాంశం->file,
				(ext2_off64_t) (ఖండం * channel->ఖండం_కొలత),
				EXT2_SEEK_సమితి, 0)))
		తిరిగి retval;

	లెక్క = (లెక్క < 0) ? -లెక్క : (లెక్క * channel->ఖండం_కొలత);

	తిరిగి ext2fs_file_write(దత్తాంశం->file, buf, లెక్క, 0);
}

స్థిర errcode_t inode_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	తిరిగి inode_write_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t inode_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				 పూర్ణాంకం కొలత, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ఒకవేళ ((retval = ext2fs_file_lseek(దత్తాంశం->file, స్థానభ్రంశం,
					EXT2_SEEK_సమితి, 0)))
		తిరిగి retval;

	తిరిగి ext2fs_file_write(దత్తాంశం->file, buf, కొలత, 0);
}

/*
 * Flush data buffers to disk.
 */
స్థిర errcode_t inode_flush(io_channel channel)
{
	నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం inode_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	తిరిగి ext2fs_file_flush(దత్తాంశం->file);
}

