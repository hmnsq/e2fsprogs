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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)

काष्ठा inode_private_तथ्य {
	पूर्णांक				magic;
	अक्षर				name[32];
	ext2_file_t			file;
	ext2_filsys			fs;
	ext2_ino_t 			ino;
	काष्ठा ext2_inode		inode;
	पूर्णांक				flags;
	काष्ठा inode_private_तथ्य	*अग्र;
};

#घोषणा CHANNEL_HAS_INODE	0x8000

अटल काष्ठा inode_private_तथ्य *शीर्ष_intern;
अटल पूर्णांक ino_unique = 0;

अटल errcode_t inode_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel);
अटल errcode_t inode_close(io_channel channel);
अटल errcode_t inode_set_blksize(io_channel channel, पूर्णांक blksize);
अटल errcode_t inode_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *तथ्य);
अटल errcode_t inode_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *तथ्य);
अटल errcode_t inode_flush(io_channel channel);
अटल errcode_t inode_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णांक आकार, स्थिर व्योम *तथ्य);
अटल errcode_t inode_read_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, व्योम *तथ्य);
अटल errcode_t inode_write_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, स्थिर व्योम *तथ्य);

अटल काष्ठा struct_io_manager struct_inode_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Inode I/O Manager",
	.खोलो		= inode_open,
	.बंद		= inode_close,
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
				  काष्ठा ext2_inode *inode,
				  अक्षर **name)
{
	काष्ठा inode_private_तथ्य 	*तथ्य;
	errcode_t			retval;

	अगर ((retval = ext2fs_get_mem(माप(काष्ठा inode_private_तथ्य),
				     &तथ्य)))
		वापस retval;
	तथ्य->magic = EXT2_ET_MAGIC_INODE_IO_CHANNEL;
	प्र_लिखो(तथ्य->name, "%u:%d", ino, ino_unique++);
	तथ्य->file = 0;
	तथ्य->fs = fs;
	तथ्य->ino = ino;
	तथ्य->flags = 0;
	अगर (inode) {
		स_नकल(&तथ्य->inode, inode, माप(काष्ठा ext2_inode));
		तथ्य->flags |= CHANNEL_HAS_INODE;
	}
	तथ्य->अग्र = शीर्ष_intern;
	शीर्ष_intern = तथ्य;
	*name = तथ्य->name;
	वापस 0;
}

errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
				 अक्षर **name)
{
	वापस ext2fs_inode_io_intern2(fs, ino, शून्य, name);
}


अटल errcode_t inode_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel	io = शून्य;
	काष्ठा inode_private_तथ्य *पूर्व, *तथ्य = शून्य;
	errcode_t	retval;
	पूर्णांक		open_flags;

	अगर (name == 0)
		वापस EXT2_ET_BAD_DEVICE_NAME;

	क्रम (तथ्य = शीर्ष_intern, पूर्व = शून्य; तथ्य;
	     पूर्व = तथ्य, तथ्य = तथ्य->अग्र)
		अगर (म_भेद(name, तथ्य->name) == 0)
			अवरोध;
	अगर (!तथ्य)
		वापस ENOENT;
	अगर (पूर्व)
		पूर्व->अग्र = तथ्य->अग्र;
	अन्यथा
		शीर्ष_intern = तथ्य->अग्र;

	retval = ext2fs_get_mem(माप(काष्ठा struct_io_channel), &io);
	अगर (retval)
		जाओ cleanup;
	स_रखो(io, 0, माप(काष्ठा struct_io_channel));

	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->manager = inode_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	अगर (retval)
		जाओ cleanup;

	म_नकल(io->name, name);
	io->private_तथ्य = तथ्य;
	io->block_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	open_flags = (flags & IO_FLAG_RW) ? EXT2_FILE_WRITE : 0;
	retval = ext2fs_file_open2(तथ्य->fs, तथ्य->ino,
				   (तथ्य->flags & CHANNEL_HAS_INODE) ?
				   &तथ्य->inode : 0, open_flags,
				   &तथ्य->file);
	अगर (retval)
		जाओ cleanup;

	*channel = io;
	वापस 0;

cleanup:
	अगर (io && io->name)
		ext2fs_free_mem(&io->name);
	अगर (तथ्य)
		ext2fs_free_mem(&तथ्य);
	अगर (io)
		ext2fs_free_mem(&io);
	वापस retval;
}

अटल errcode_t inode_close(io_channel channel)
{
	काष्ठा inode_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	अगर (--channel->refcount > 0)
		वापस 0;

	retval = ext2fs_file_close(तथ्य->file);

	ext2fs_free_mem(&channel->private_तथ्य);
	अगर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	वापस retval;
}

अटल errcode_t inode_set_blksize(io_channel channel, पूर्णांक blksize)
{
	काष्ठा inode_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	channel->block_आकार = blksize;
	वापस 0;
}


अटल errcode_t inode_read_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, व्योम *buf)
{
	काष्ठा inode_private_तथ्य *तथ्य;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	अगर ((retval = ext2fs_file_llseek(तथ्य->file,
				(ext2_off64_t)(block * channel->block_आकार),
				EXT2_SEEK_SET, 0)))
		वापस retval;

	गिनती = (गिनती < 0) ? -गिनती : (गिनती * channel->block_आकार);

	वापस ext2fs_file_read(तथ्य->file, buf, गिनती, 0);
}

अटल errcode_t inode_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	वापस inode_read_blk64(channel, block, गिनती, buf);
}

अटल errcode_t inode_write_blk64(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा inode_private_तथ्य *तथ्य;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	अगर ((retval = ext2fs_file_llseek(तथ्य->file,
				(ext2_off64_t) (block * channel->block_आकार),
				EXT2_SEEK_SET, 0)))
		वापस retval;

	गिनती = (गिनती < 0) ? -गिनती : (गिनती * channel->block_आकार);

	वापस ext2fs_file_write(तथ्य->file, buf, गिनती, 0);
}

अटल errcode_t inode_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	वापस inode_write_blk64(channel, block, गिनती, buf);
}

अटल errcode_t inode_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक आकार, स्थिर व्योम *buf)
{
	काष्ठा inode_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	अगर ((retval = ext2fs_file_lseek(तथ्य->file, offset,
					EXT2_SEEK_SET, 0)))
		वापस retval;

	वापस ext2fs_file_write(तथ्य->file, buf, आकार, 0);
}

/*
 * Flush data buffers to disk.
 */
अटल errcode_t inode_flush(io_channel channel)
{
	काष्ठा inode_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा inode_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_INODE_IO_CHANNEL);

	वापस ext2fs_file_flush(तथ्य->file);
}

