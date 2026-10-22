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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ				magic;
	ਅੱਖਰ				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	ਬਣਤਰ ext2_inode		inode;
	ਪੂਰਨ				flags;
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ	*ਅਗਲਾ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_HAS_INODE	0x8000

ਸਥਿਰ ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਸਿਖਰ_intern;
ਸਥਿਰ ਪੂਰਨ ino_unique = 0;

ਸਥਿਰ errcode_t inode_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel);
ਸਥਿਰ errcode_t inode_close(io_channel channel);
ਸਥਿਰ errcode_t inode_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize);
ਸਥਿਰ errcode_t inode_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t inode_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t inode_flush(io_channel channel);
ਸਥਿਰ errcode_t inode_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				ਪੂਰਨ ਮਾਪ, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t inode_read_blk64(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t inode_write_blk64(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.open		= inode_open,
	.close		= inode_close,
	.ਸੈੱਟ_blksize	= inode_ਸੈੱਟ_blksize,
	.read_blk	= inode_read_blk,
	.write_blk	= inode_write_blk,
	.flush		= inode_flush,
	.write_byte	= inode_write_byte,
	.read_blk64	= inode_read_blk64,
	.write_blk64	= inode_write_blk64
};

io_manager inode_io_manager = &struct_inode_manager;

errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
				  ਬਣਤਰ ext2_inode *inode,
				  ਅੱਖਰ **name)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ 	*ਅੰਕੜੇ;
	errcode_t			retval;

	ਜੇ ((retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ),
				     &ਅੰਕੜੇ)))
		ਵਾਪਸ retval;
	ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(ਅੰਕੜੇ->name, "%u:%d", ino, ino_unique++);
	ਅੰਕੜੇ->file = 0;
	ਅੰਕੜੇ->fs = fs;
	ਅੰਕੜੇ->ino = ino;
	ਅੰਕੜੇ->flags = 0;
	ਜੇ (inode) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ਅੰਕੜੇ->inode, inode, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
		ਅੰਕੜੇ->flags |= CHANNEL_HAS_INODE;
	}
	ਅੰਕੜੇ->ਅਗਲਾ = ਸਿਖਰ_intern;
	ਸਿਖਰ_intern = ਅੰਕੜੇ;
	*name = ਅੰਕੜੇ->name;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 ਅੱਖਰ **name)
{
	ਵਾਪਸ ext2fs_inode_io_intern2(fs, ino, ਨਿਰਾ, name);
}


ਸਥਿਰ errcode_t inode_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	io_channel	io = ਨਿਰਾ;
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *prev, *ਅੰਕੜੇ = ਨਿਰਾ;
	errcode_t	retval;
	ਪੂਰਨ		open_flags;

	ਜੇ (name == 0)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;

	ਲਈ (ਅੰਕੜੇ = ਸਿਖਰ_intern, prev = ਨਿਰਾ; ਅੰਕੜੇ;
	     prev = ਅੰਕੜੇ, ਅੰਕੜੇ = ਅੰਕੜੇ->ਅਗਲਾ)
		ਜੇ (strcmp(name, ਅੰਕੜੇ->name) == 0)
			ਰੋਕੋ;
	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ ENOENT;
	ਜੇ (prev)
		prev->ਅਗਲਾ = ਅੰਕੜੇ->ਅਗਲਾ;
	ਨਹੀਂਤਾਂ
		ਸਿਖਰ_intern = ਅੰਕੜੇ->ਅਗਲਾ;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_io_channel), &io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(io, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &io->name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(io->name, name);
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	io->ਖੰਡ_ਮਾਪ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(ਅੰਕੜੇ->fs, ਅੰਕੜੇ->ino,
				   (ਅੰਕੜੇ->flags & CHANNEL_HAS_INODE) ?
				   &ਅੰਕੜੇ->inode : 0, open_flags,
				   &ਅੰਕੜੇ->file);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	*channel = io;
	ਵਾਪਸ 0;

cleanup:
	ਜੇ (io && io->name)
		ext2fs_free_mem(&io->name);
	ਜੇ (ਅੰਕੜੇ)
		ext2fs_free_mem(&ਅੰਕੜੇ);
	ਜੇ (io)
		ext2fs_free_mem(&io);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t inode_close(io_channel channel)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
		ਵਾਪਸ 0;

	retval = ext2fs_file_close(ਅੰਕੜੇ->file);

	ext2fs_free_mem(&channel->ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਜੇ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t inode_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->ਖੰਡ_ਮਾਪ = blksize;
	ਵਾਪਸ 0;
}


ਸਥਿਰ errcode_t inode_read_blk64(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ਜੇ ((retval = ext2fs_file_llseek(ਅੰਕੜੇ->file,
				(ext2_off64_t)(ਖੰਡ * channel->ਖੰਡ_ਮਾਪ),
				EXT2_SEEK_ਸੈੱਟ, 0)))
		ਵਾਪਸ retval;

	ਗਿਣਤੀ_2 = (ਗਿਣਤੀ_2 < 0) ? -ਗਿਣਤੀ_2 : (ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);

	ਵਾਪਸ ext2fs_file_read(ਅੰਕੜੇ->file, buf, ਗਿਣਤੀ_2, 0);
}

ਸਥਿਰ errcode_t inode_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਵਾਪਸ inode_read_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t inode_write_blk64(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ਜੇ ((retval = ext2fs_file_llseek(ਅੰਕੜੇ->file,
				(ext2_off64_t) (ਖੰਡ * channel->ਖੰਡ_ਮਾਪ),
				EXT2_SEEK_ਸੈੱਟ, 0)))
		ਵਾਪਸ retval;

	ਗਿਣਤੀ_2 = (ਗਿਣਤੀ_2 < 0) ? -ਗਿਣਤੀ_2 : (ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);

	ਵਾਪਸ ext2fs_file_write(ਅੰਕੜੇ->file, buf, ਗਿਣਤੀ_2, 0);
}

ਸਥਿਰ errcode_t inode_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ inode_write_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t inode_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				 ਪੂਰਨ ਮਾਪ, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ਜੇ ((retval = ext2fs_file_lseek(ਅੰਕੜੇ->file, ਵਿੱਥ,
					EXT2_SEEK_ਸੈੱਟ, 0)))
		ਵਾਪਸ retval;

	ਵਾਪਸ ext2fs_file_write(ਅੰਕੜੇ->file, buf, ਮਾਪ, 0);
}

/*
 * Flush data buffers to disk.
 */
ਸਥਿਰ errcode_t inode_flush(io_channel channel)
{
	ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ inode_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	ਵਾਪਸ ext2fs_file_flush(ਅੰਕੜੇ->file);
}

