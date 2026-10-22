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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  सचे ((संरचना)->magic != (code)) पटिनिवत्त (code)

संरचना inode_private_data {
	पूर्णाङ्क				magic;
	वर्ण				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	संरचना ext2_inode		inode;
	पूर्णाङ्क				flags;
	संरचना inode_private_data	*next;
};

#define CHANNEL_HAS_INODE	0x8000

स्थिर संरचना inode_private_data *शिखरम्_intern;
स्थिर पूर्णाङ्क ino_unique = 0;

स्थिर errcode_t inode_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t inode_close(io_channel channel);
स्थिर errcode_t inode_set_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *data);
स्थिर errcode_t inode_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *data);
स्थिर errcode_t inode_flush(io_channel channel);
स्थिर errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				पूर्णाङ्क size, ध्रुव शून्य *data);
स्थिर errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, पूर्णाङ्क गणनम्, शून्य *data);
स्थिर errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, पूर्णाङ्क गणनम्, ध्रुव शून्य *data);

स्थिर संरचना struct_io_manager struct_inode_manager = {
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
				  संरचना ext2_inode *inode,
				  वर्ण **name)
{
	संरचना inode_private_data 	*data;
	errcode_t			retval;

	सचे ((retval = ext2fs_get_mem(परिमाण(संरचना inode_private_data),
				     &data)))
		पटिनिवत्त retval;
	data->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(data->name, "%u:%d", ino, ino_unique++);
	data->file = 0;
	data->fs = fs;
	data->ino = ino;
	data->flags = 0;
	सचे (inode) {
		memcpy(&data->inode, inode, परिमाण(संरचना ext2_inode));
		data->flags |= CHANNEL_HAS_INODE;
	}
	data->next = शिखरम्_intern;
	शिखरम्_intern = data;
	*name = data->name;
	पटिनिवत्त 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 वर्ण **name)
{
	पटिनिवत्त ext2fs_inode_io_intern2(fs, ino, रिक्त, name);
}


स्थिर errcode_t inode_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना inode_private_data *prev, *data = रिक्त;
	errcode_t	retval;
	पूर्णाङ्क		open_flags;

	सचे (name == 0)
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;

	अत्थाय (data = शिखरम्_intern, prev = रिक्त; data;
	     prev = data, data = data->next)
		सचे (strcmp(name, data->name) == 0)
			भङ्ग;
	सचे (!data)
		पटिनिवत्त ENOENT;
	सचे (prev)
		prev->next = data->next;
	अञ्ञथा
		शिखरम्_intern = data->next;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	सचे (retval)
		गच्छ cleanup;
	memset(io, 0, परिमाण(संरचना struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	सचे (retval)
		गच्छ cleanup;

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
	सचे (retval)
		गच्छ cleanup;

	*channel = io;
	पटिनिवत्त 0;

cleanup:
	सचे (io && io->name)
		ext2fs_free_mem(&io->name);
	सचे (data)
		ext2fs_free_mem(&data);
	सचे (io)
		ext2fs_free_mem(&io);
	पटिनिवत्त retval;
}

स्थिर errcode_t inode_close(io_channel channel)
{
	संरचना inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	सचे (--channel->refcount > 0)
		पटिनिवत्त 0;

	retval = ext2fs_file_close(data->file);

	ext2fs_free_mem(&channel->private_data);
	सचे (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	पटिनिवत्त retval;
}

स्थिर errcode_t inode_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_size = blksize;
	पटिनिवत्त 0;
}


स्थिर errcode_t inode_read_blk64(io_channel channel,
				unsigned long long block, पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	सचे ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t)(block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		पटिनिवत्त retval;

	गणनम् = (गणनम् < 0) ? -गणनम् : (गणनम् * channel->block_size);

	पटिनिवत्त ext2fs_file_read(data->file, buf, गणनम्, 0);
}

स्थिर errcode_t inode_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	पटिनिवत्त inode_read_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t inode_write_blk64(io_channel channel,
				unsigned long long block, पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना inode_private_data *data;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	सचे ((retval = ext2fs_file_llseek(data->file,
				(ext2_off64_t) (block * channel->block_size),
				EXT2_SEEK_SET, 0)))
		पटिनिवत्त retval;

	गणनम् = (गणनम् < 0) ? -गणनम् : (गणनम् * channel->block_size);

	पटिनिवत्त ext2fs_file_write(data->file, buf, गणनम्, 0);
}

स्थिर errcode_t inode_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	पटिनिवत्त inode_write_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t inode_write_byte(io_channel channel, unsigned long offset,
				 पूर्णाङ्क size, ध्रुव शून्य *buf)
{
	संरचना inode_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	सचे ((retval = ext2fs_file_lseek(data->file, offset,
					EXT2_SEEK_SET, 0)))
		पटिनिवत्त retval;

	पटिनिवत्त ext2fs_file_write(data->file, buf, size, 0);
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t inode_flush(io_channel channel)
{
	संरचना inode_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना inode_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	पटिनिवत्त ext2fs_file_flush(data->file);
}

