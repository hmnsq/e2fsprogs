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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ				magic;
	ಅಕ್ಷರ				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ರಚನೆ ext2_inode		inode;
	ಪೂರ್ಣಾಂಕ				flags;
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ	*ಮುಂದಿನ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_HAS_INODE	0x8000

ಸ್ಥಿರ ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ತುದಿ_intern;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ino_unique = 0;

ಸ್ಥಿರ errcode_t inode_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t inode_close(io_channel channel);
ಸ್ಥಿರ errcode_t inode_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize);
ಸ್ಥಿರ errcode_t inode_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t inode_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t inode_flush(io_channel channel);
ಸ್ಥಿರ errcode_t inode_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t inode_read_blk64(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t inode_write_blk64(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.ಗಣ_blksize	= inode_ಗಣ_blksize,
	.read_blk	= inode_read_blk,
	.write_blk	= inode_write_blk,
	.flush		= inode_flush,
	.write_byte	= inode_write_byte,
	.read_blk64	= inode_read_blk64,
	.write_blk64	= inode_write_blk64
};

io_manager inode_io_manager = &struct_inode_manager;

errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
				  ರಚನೆ ext2_inode *inode,
				  ಅಕ್ಷರ **name)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ 	*ದತ್ತಾಂಶ;
	errcode_t			retval;

	ಆದರೆ ((retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ),
				     &ದತ್ತಾಂಶ)))
		ಹಿಂತಿರುಗಿ retval;
	ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(ದತ್ತಾಂಶ->name, "%u:%d", ino, ino_unique++);
	ದತ್ತಾಂಶ->file = 0;
	ದತ್ತಾಂಶ->fs = fs;
	ದತ್ತಾಂಶ->ino = ino;
	ದತ್ತಾಂಶ->flags = 0;
	ಆದರೆ (inode) {
		ಸ್ಮರಣೆ_ನಕಲು(&ದತ್ತಾಂಶ->inode, inode, ಗಾತ್ರ(ರಚನೆ ext2_inode));
		ದತ್ತಾಂಶ->flags |= CHANNEL_HAS_INODE;
	}
	ದತ್ತಾಂಶ->ಮುಂದಿನ = ತುದಿ_intern;
	ತುದಿ_intern = ದತ್ತಾಂಶ;
	*name = ದತ್ತಾಂಶ->name;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 ಅಕ್ಷರ **name)
{
	ಹಿಂತಿರುಗಿ ext2fs_inode_io_intern2(fs, ino, ಖಾಲಿ, name);
}


ಸ್ಥಿರ errcode_t inode_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	io_channel	io = ಖಾಲಿ;
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *prev, *ದತ್ತಾಂಶ = ಖಾಲಿ;
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		open_flags;

	ಆದರೆ (name == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;

	ಗಾಗಿ (ದತ್ತಾಂಶ = ತುದಿ_intern, prev = ಖಾಲಿ; ದತ್ತಾಂಶ;
	     prev = ದತ್ತಾಂಶ, ದತ್ತಾಂಶ = ದತ್ತಾಂಶ->ಮುಂದಿನ)
		ಆದರೆ (strcmp(name, ದತ್ತಾಂಶ->name) == 0)
			ನಿಲ್ಲಿಸು;
	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ ENOENT;
	ಆದರೆ (prev)
		prev->ಮುಂದಿನ = ದತ್ತಾಂಶ->ಮುಂದಿನ;
	ಇಲ್ಲದಿದ್ದರೆ
		ತುದಿ_intern = ದತ್ತಾಂಶ->ಮುಂದಿನ;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_io_channel), &io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(io, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &io->name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(io->name, name);
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	io->ಖಂಡ_ಗಾತ್ರ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(ದತ್ತಾಂಶ->fs, ದತ್ತಾಂಶ->ino,
				   (ದತ್ತಾಂಶ->flags & CHANNEL_HAS_INODE) ?
				   &ದತ್ತಾಂಶ->inode : 0, open_flags,
				   &ದತ್ತಾಂಶ->file);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	*channel = io;
	ಹಿಂತಿರುಗಿ 0;

cleanup:
	ಆದರೆ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಆದರೆ (ದತ್ತಾಂಶ)
		ext2fs_free_mem(&ದತ್ತಾಂಶ);
	ಆದರೆ (io)
		ext2fs_free_mem(&io);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t inode_close(io_channel channel)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
		ಹಿಂತಿರುಗಿ 0;

	retval = ext2fs_file_close(ದತ್ತಾಂಶ->file);

	ext2fs_free_mem(&channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಆದರೆ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t inode_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->ಖಂಡ_ಗಾತ್ರ = blksize;
	ಹಿಂತಿರುಗಿ 0;
}


ಸ್ಥಿರ errcode_t inode_read_blk64(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಆದರೆ ((retval = ext2fs_file_llseek(ದತ್ತಾಂಶ->file,
				(ext2_off64_t)(ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ),
				EXT2_SEEK_ಗಣ, 0)))
		ಹಿಂತಿರುಗಿ retval;

	ಎಣಿಕೆ_2 = (ಎಣಿಕೆ_2 < 0) ? -ಎಣಿಕೆ_2 : (ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);

	ಹಿಂತಿರುಗಿ ext2fs_file_read(ದತ್ತಾಂಶ->file, buf, ಎಣಿಕೆ_2, 0);
}

ಸ್ಥಿರ errcode_t inode_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ inode_read_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t inode_write_blk64(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಆದರೆ ((retval = ext2fs_file_llseek(ದತ್ತಾಂಶ->file,
				(ext2_off64_t) (ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ),
				EXT2_SEEK_ಗಣ, 0)))
		ಹಿಂತಿರುಗಿ retval;

	ಎಣಿಕೆ_2 = (ಎಣಿಕೆ_2 < 0) ? -ಎಣಿಕೆ_2 : (ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);

	ಹಿಂತಿರುಗಿ ext2fs_file_write(ದತ್ತಾಂಶ->file, buf, ಎಣಿಕೆ_2, 0);
}

ಸ್ಥಿರ errcode_t inode_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ inode_write_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t inode_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				 ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಆದರೆ ((retval = ext2fs_file_lseek(ದತ್ತಾಂಶ->file, ಸ್ಥಾನಾಂತರ,
					EXT2_SEEK_ಗಣ, 0)))
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ ext2fs_file_write(ದತ್ತಾಂಶ->file, buf, ಗಾತ್ರ_2, 0);
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t inode_flush(io_channel channel)
{
	ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ inode_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ಹಿಂತಿರುಗಿ ext2fs_file_flush(ದತ್ತಾಂಶ->file);
}

