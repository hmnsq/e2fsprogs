/*
 * test_io.c --- This is the Test I/O interface.
 *
 * Copyright (C) 1996 Theodore Ts'o.
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
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)

काष्ठा test_private_तथ्य {
	पूर्णांक	magic;
	io_channel real;
	पूर्णांक flags;
	खाता *outfile;
	अचिन्हित दीर्घ block;
	पूर्णांक read_abort_गिनती, write_abort_गिनती;
	व्योम (*read_blk)(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err);
	व्योम (*write_blk)(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err);
	व्योम (*set_blksize)(पूर्णांक blksize, errcode_t err);
	व्योम (*write_byte)(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err);
	व्योम (*read_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err);
	व्योम (*write_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
व्योम (*test_io_cb_read_blk)
	(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err) = 0;
व्योम (*test_io_cb_write_blk)
	(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err) = 0;
व्योम (*test_io_cb_read_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err) = 0;
व्योम (*test_io_cb_write_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err) = 0;
व्योम (*test_io_cb_set_blksize)
	(पूर्णांक blksize, errcode_t err) = 0;
व्योम (*test_io_cb_write_byte)
	(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err) = 0;

/*
 * Test flags
 */
#घोषणा TEST_FLAG_READ			0x01
#घोषणा TEST_FLAG_WRITE			0x02
#घोषणा TEST_FLAG_SET_BLKSIZE		0x04
#घोषणा TEST_FLAG_FLUSH			0x08
#घोषणा TEST_FLAG_DUMP			0x10
#घोषणा TEST_FLAG_SET_OPTION		0x20
#घोषणा TEST_FLAG_DISCARD		0x40
#घोषणा TEST_FLAG_READAHEAD		0x80
#घोषणा TEST_FLAG_ZEROOUT		0x100

अटल व्योम test_dump_block(io_channel channel,
			    काष्ठा test_private_तथ्य *तथ्य,
			    अचिन्हित दीर्घ block, स्थिर व्योम *buf)
{
	स्थिर अचिन्हित अक्षर *cp;
	खाता *f = तथ्य->outfile;
	पूर्णांक	i;
	अचिन्हित दीर्घ	cksum = 0;

	क्रम (i=0, cp = buf; i < channel->block_आकार; i++, cp++) {
		cksum += *cp;
	}
	ख_लिखो(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	क्रम (i=0, cp = buf; i < channel->block_आकार; i++, cp++) {
		अगर ((i % 16) == 0)
			ख_लिखो(f, "%04x: ", i);
		ख_लिखो(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
अटल errcode_t test_flush(io_channel channel)
{
	काष्ठा test_private_तथ्य	*तथ्य;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *)channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_flush(तथ्य->real);

	अगर (तथ्य->flags & TEST_FLAG_FLUSH)
		ख_लिखो(तथ्य->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	वापस retval;
}

अटल व्योम test_abort(io_channel channel, अचिन्हित दीर्घ block)
{
	काष्ठा test_private_तथ्य *तथ्य;
	खाता *f;

	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	f = तथ्य->outfile;
	test_flush(channel);

	ख_लिखो(f, "Aborting due to I/O to block %lu\n", block);
	ख_साफ(f);
	पात();
}

अटल errcode_t test_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel	io = शून्य;
	काष्ठा test_private_तथ्य *तथ्य = शून्य;
	errcode_t	retval;
	अक्षर		*मूल्य;

	अगर (name == 0)
		वापस EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(माप(काष्ठा struct_io_channel), &io);
	अगर (retval)
		जाओ cleanup;
	स_रखो(io, 0, माप(काष्ठा struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(माप(काष्ठा test_private_तथ्य), &तथ्य);
	अगर (retval)
		जाओ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	अगर (retval)
		जाओ cleanup;

	म_नकल(io->name, name);
	io->private_तथ्य = तथ्य;
	io->block_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	स_रखो(तथ्य, 0, माप(काष्ठा test_private_तथ्य));
	तथ्य->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	अगर (test_io_backing_manager) {
		retval = test_io_backing_manager->खोलो(name, flags,
						       &तथ्य->real);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा {
		तथ्य->real = 0;
	}
	तथ्य->read_blk =	test_io_cb_read_blk;
	तथ्य->write_blk =	test_io_cb_write_blk;
	तथ्य->set_blksize =	test_io_cb_set_blksize;
	तथ्य->write_byte =	test_io_cb_write_byte;
	तथ्य->read_blk64 =	test_io_cb_read_blk64;
	तथ्य->write_blk64 =	test_io_cb_write_blk64;

	तथ्य->outfile = शून्य;
	अगर ((मूल्य = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != शून्य)
		तथ्य->outfile = ख_खोलो(मूल्य, "w");
	अगर (!तथ्य->outfile)
		तथ्य->outfile = मानक_त्रुटि;

	तथ्य->flags = 0;
	अगर ((मूल्य = ext2fs_safe_getenv("TEST_IO_FLAGS")) != शून्य)
		तथ्य->flags = म_से_अदीर्घ(मूल्य, शून्य, 0);

	तथ्य->block = 0;
	अगर ((मूल्य = ext2fs_safe_getenv("TEST_IO_BLOCK")) != शून्य)
		तथ्य->block = म_से_अदीर्घ(मूल्य, शून्य, 0);

	तथ्य->read_abort_गिनती = 0;
	अगर ((मूल्य = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != शून्य)
		तथ्य->read_abort_गिनती = म_से_अदीर्घ(मूल्य, शून्य, 0);

	तथ्य->write_abort_गिनती = 0;
	अगर ((मूल्य = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != शून्य)
		तथ्य->write_abort_गिनती = म_से_अदीर्घ(मूल्य, शून्य, 0);

	अगर (तथ्य->real) {
		io->align = तथ्य->real->align;
		अगर (तथ्य->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	वापस 0;

cleanup:
	अगर (io && io->name)
		ext2fs_free_mem(&io->name);
	अगर (io)
		ext2fs_free_mem(&io);
	अगर (तथ्य)
		ext2fs_free_mem(&तथ्य);
	वापस retval;
}

अटल errcode_t test_close(io_channel channel)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (--channel->refcount > 0)
		वापस 0;

	अगर (तथ्य->real)
		retval = io_channel_close(तथ्य->real);

	अगर (तथ्य->outfile && तथ्य->outfile != मानक_त्रुटि)
		ख_बंद(तथ्य->outfile);

	ext2fs_free_mem(&channel->private_तथ्य);
	अगर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	वापस retval;
}

अटल errcode_t test_set_blksize(io_channel channel, पूर्णांक blksize)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real) {
		retval = io_channel_set_blksize(तथ्य->real, blksize);
		channel->align = तथ्य->real->align;
	}
	अगर (तथ्य->set_blksize)
		तथ्य->set_blksize(blksize, retval);
	अगर (तथ्य->flags & TEST_FLAG_SET_BLKSIZE)
		ख_लिखो(तथ्य->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_आकार = blksize;
	वापस retval;
}


अटल errcode_t test_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_read_blk(तथ्य->real, block, गिनती, buf);
	अगर (तथ्य->read_blk)
		तथ्य->read_blk(block, गिनती, retval);
	अगर (तथ्य->flags & TEST_FLAG_READ)
		ख_लिखो(तथ्य->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	अगर (तथ्य->block && तथ्य->block == block) {
		अगर (तथ्य->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, तथ्य, block, buf);
		अगर (--तथ्य->read_abort_गिनती == 0)
			test_abort(channel, block);
	}
	वापस retval;
}

अटल errcode_t test_write_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_write_blk(तथ्य->real, block, गिनती, buf);
	अगर (तथ्य->write_blk)
		तथ्य->write_blk(block, गिनती, retval);
	अगर (तथ्य->flags & TEST_FLAG_WRITE)
		ख_लिखो(तथ्य->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	अगर (तथ्य->block && तथ्य->block == block) {
		अगर (तथ्य->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, तथ्य, block, buf);
		अगर (--तथ्य->write_abort_गिनती == 0)
			test_abort(channel, block);
	}
	वापस retval;
}

अटल errcode_t test_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_read_blk64(तथ्य->real, block, गिनती, buf);
	अगर (तथ्य->read_blk64)
		तथ्य->read_blk64(block, गिनती, retval);
	अगर (तथ्य->flags & TEST_FLAG_READ)
		ख_लिखो(तथ्य->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	अगर (तथ्य->block && तथ्य->block == block) {
		अगर (तथ्य->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, तथ्य, block, buf);
		अगर (--तथ्य->read_abort_गिनती == 0)
			test_abort(channel, block);
	}
	वापस retval;
}

अटल errcode_t test_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_write_blk64(तथ्य->real, block, गिनती, buf);
	अगर (तथ्य->write_blk64)
		तथ्य->write_blk64(block, गिनती, retval);
	अगर (तथ्य->flags & TEST_FLAG_WRITE)
		ख_लिखो(तथ्य->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	अगर (तथ्य->block && तथ्य->block == block) {
		अगर (तथ्य->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, तथ्य, block, buf);
		अगर (--तथ्य->write_abort_गिनती == 0)
			test_abort(channel, block);
	}
	वापस retval;
}

अटल errcode_t test_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
			       पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real && तथ्य->real->manager->write_byte)
		retval = io_channel_write_byte(तथ्य->real, offset, गिनती, buf);
	अगर (तथ्य->write_byte)
		तथ्य->write_byte(offset, गिनती, retval);
	अगर (तथ्य->flags & TEST_FLAG_WRITE)
		ख_लिखो(तथ्य->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, गिनती, retval ? error_message(retval) : "OK");
	वापस retval;
}

अटल errcode_t test_set_option(io_channel channel, स्थिर अक्षर *option,
				 स्थिर अक्षर *arg)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	अगर (तथ्य->flags & TEST_FLAG_SET_OPTION)
		ख_लिखो(तथ्य->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	अगर (तथ्य->real && तथ्य->real->manager->set_option) {
		retval = (तथ्य->real->manager->set_option)(तथ्य->real,
							   option, arg);
		अगर (तथ्य->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(तथ्य->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} अन्यथा {
		अगर (तथ्य->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(तथ्य->outfile, "not implemented\n");
	}
	वापस retval;
}

अटल errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real && तथ्य->real->manager->get_stats) {
		retval = (तथ्य->real->manager->get_stats)(तथ्य->real, stats);
	}
	वापस retval;
}

अटल errcode_t test_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_discard(तथ्य->real, block, गिनती);
	अगर (तथ्य->flags & TEST_FLAG_DISCARD)
		ख_लिखो(तथ्य->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	वापस retval;
}

अटल errcode_t test_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_cache_readahead(तथ्य->real, block, गिनती);
	अगर (तथ्य->flags & TEST_FLAG_READAHEAD)
		ख_लिखो(तथ्य->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	वापस retval;
}

अटल errcode_t test_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा test_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा test_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_zeroout(तथ्य->real, block, गिनती);
	अगर (तथ्य->flags & TEST_FLAG_ZEROOUT)
		ख_लिखो(तथ्य->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	वापस retval;
}

अटल काष्ठा struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.खोलो		= test_open,
	.बंद		= test_close,
	.set_blksize	= test_set_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.set_option	= test_set_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
