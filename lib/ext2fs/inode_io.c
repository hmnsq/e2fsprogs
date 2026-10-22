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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)

संरचना inode_वैयक्तिकम्_दत्त {
	पूर्णाङ्क				magic;
	वर्ण				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	संरचना ext2_inode		inode;
	पूर्णाङ्क				flags;
	संरचना inode_वैयक्तिकम्_दत्त	*अग्र;
};

#परिभाषय CHANNEL_HAS_INODE	0x8000

स्थिर संरचना inode_वैयक्तिकम्_दत्त *शिखरम्_intern;
स्थिर पूर्णाङ्क ino_unique = 0;

स्थिर errcode_t inode_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t inode_close(io_channel channel);
स्थिर errcode_t inode_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t inode_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *दत्त);
स्थिर errcode_t inode_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
स्थिर errcode_t inode_flush(io_channel channel);
स्थिर errcode_t inode_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				पूर्णाङ्क आकार, ध्रुव शून्य *दत्त);
स्थिर errcode_t inode_read_blk64(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, शून्य *दत्त);
स्थिर errcode_t inode_write_blk64(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);

स्थिर संरचना struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.उद्घाटय		= inode_open,
	.close		= inode_close,
	.समुच्चयः_blksize	= inode_समुच्चयः_blksize,
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
	संरचना inode_वैयक्तिकम्_दत्त 	*दत्त;
	errcode_t			retval;

	यदि ((retval = ext2fs_get_mem(परिमाण(संरचना inode_वैयक्तिकम्_दत्त),
				     &दत्त)))
		प्रत्यागम retval;
	दत्त->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	sprintf(दत्त->name, "%u:%d", ino, ino_unique++);
	दत्त->file = 0;
	दत्त->fs = fs;
	दत्त->ino = ino;
	दत्त->flags = 0;
	यदि (inode) {
		स्मृतिं_प्रतिलिख(&दत्त->inode, inode, परिमाण(संरचना ext2_inode));
		दत्त->flags |= CHANNEL_HAS_INODE;
	}
	दत्त->अग्र = शिखरम्_intern;
	शिखरम्_intern = दत्त;
	*name = दत्त->name;
	प्रत्यागम 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 वर्ण **name)
{
	प्रत्यागम ext2fs_inode_io_intern2(fs, ino, रिक्त, name);
}


स्थिर errcode_t inode_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना inode_वैयक्तिकम्_दत्त *पूर्व, *दत्त = रिक्त;
	errcode_t	retval;
	पूर्णाङ्क		open_flags;

	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	प्रति (दत्त = शिखरम्_intern, पूर्व = रिक्त; दत्त;
	     पूर्व = दत्त, दत्त = दत्त->अग्र)
		यदि (वाक्यं_तुलय(name, दत्त->name) == 0)
			भङ्ग;
	यदि (!दत्त)
		प्रत्यागम ENOENT;
	यदि (पूर्व)
		पूर्व->अग्र = दत्त->अग्र;
	अन्यथा
		शिखरम्_intern = दत्त->अग्र;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(io, 0, परिमाण(संरचना struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(io->name, name);
	io->वैयक्तिकम्_दत्त = दत्त;
	io->खण्डम्_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(दत्त->fs, दत्त->ino,
				   (दत्त->flags & CHANNEL_HAS_INODE) ?
				   &दत्त->inode : 0, open_flags,
				   &दत्त->file);
	यदि (retval)
		गच्छ cleanup;

	*channel = io;
	प्रत्यागम 0;

cleanup:
	यदि (io && io->name)
		ext2fs_free_mem(&io->name);
	यदि (दत्त)
		ext2fs_free_mem(&दत्त);
	यदि (io)
		ext2fs_free_mem(&io);
	प्रत्यागम retval;
}

स्थिर errcode_t inode_close(io_channel channel)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;

	retval = ext2fs_file_close(दत्त->file);

	ext2fs_free_mem(&channel->वैयक्तिकम्_दत्त);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t inode_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->खण्डम्_आकार = blksize;
	प्रत्यागम 0;
}


स्थिर errcode_t inode_read_blk64(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	यदि ((retval = ext2fs_file_llseek(दत्त->file,
				(ext2_off64_t)(खण्डम् * channel->खण्डम्_आकार),
				EXT2_SEEK_समुच्चयः, 0)))
		प्रत्यागम retval;

	गणनम् = (गणनम् < 0) ? -गणनम् : (गणनम् * channel->खण्डम्_आकार);

	प्रत्यागम ext2fs_file_read(दत्त->file, buf, गणनम्, 0);
}

स्थिर errcode_t inode_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम inode_read_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t inode_write_blk64(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	यदि ((retval = ext2fs_file_llseek(दत्त->file,
				(ext2_off64_t) (खण्डम् * channel->खण्डम्_आकार),
				EXT2_SEEK_समुच्चयः, 0)))
		प्रत्यागम retval;

	गणनम् = (गणनम् < 0) ? -गणनम् : (गणनम् * channel->खण्डम्_आकार);

	प्रत्यागम ext2fs_file_write(दत्त->file, buf, गणनम्, 0);
}

स्थिर errcode_t inode_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम inode_write_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t inode_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				 पूर्णाङ्क आकार, ध्रुव शून्य *buf)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	यदि ((retval = ext2fs_file_lseek(दत्त->file, विस्थापनम्,
					EXT2_SEEK_समुच्चयः, 0)))
		प्रत्यागम retval;

	प्रत्यागम ext2fs_file_write(दत्त->file, buf, आकार, 0);
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t inode_flush(io_channel channel)
{
	संरचना inode_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना inode_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	प्रत्यागम ext2fs_file_flush(दत्त->file);
}

