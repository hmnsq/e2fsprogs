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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) प्रत्यागम (code)

संरचना test_private_data {
	पूर्णाङ्क	magic;
	io_channel real;
	पूर्णाङ्क flags;
	खाता *outfile;
	अचिन्हित दीर्घ block;
	पूर्णाङ्क read_abort_गिनती, write_abort_गिनती;
	शून्य (*read_blk)(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err);
	शून्य (*write_blk)(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err);
	शून्य (*set_blksize)(पूर्णाङ्क blksize, errcode_t err);
	शून्य (*write_byte)(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err);
	शून्य (*read_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती, errcode_t err);
	शून्य (*write_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
शून्य (*test_io_cb_read_blk)
	(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk)
	(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err) = 0;
शून्य (*test_io_cb_read_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती, errcode_t err) = 0;
शून्य (*test_io_cb_set_blksize)
	(पूर्णाङ्क blksize, errcode_t err) = 0;
शून्य (*test_io_cb_write_byte)
	(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err) = 0;

/*
 * Test flags
 */
#define TEST_FLAG_READ			0x01
#define TEST_FLAG_WRITE			0x02
#define TEST_FLAG_SET_BLKSIZE		0x04
#define TEST_FLAG_FLUSH			0x08
#define TEST_FLAG_DUMP			0x10
#define TEST_FLAG_SET_OPTION		0x20
#define TEST_FLAG_DISCARD		0x40
#define TEST_FLAG_READAHEAD		0x80
#define TEST_FLAG_ZEROOUT		0x100

स्थिर शून्य test_dump_block(io_channel channel,
			    संरचना test_private_data *data,
			    अचिन्हित दीर्घ block, ध्रुव शून्य *buf)
{
	ध्रुव अचिन्हित वर्ण *cp;
	खाता *f = data->outfile;
	पूर्णाङ्क	i;
	अचिन्हित दीर्घ	cksum = 0;

	प्रति (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	ख_लिखो(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	प्रति (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		यदि ((i % 16) == 0)
			ख_लिखो(f, "%04x: ", i);
		ख_लिखो(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t test_flush(io_channel channel)
{
	संरचना test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_flush(data->real);

	यदि (data->flags & TEST_FLAG_FLUSH)
		ख_लिखो(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	प्रत्यागम retval;
}

स्थिर शून्य test_abort(io_channel channel, अचिन्हित दीर्घ block)
{
	संरचना test_private_data *data;
	खाता *f;

	data = (संरचना test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	ख_लिखो(f, "Aborting due to I/O to block %lu\n", block);
	ख_साफ(f);
	पात();
}

स्थिर errcode_t test_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना test_private_data *data = रिक्त;
	errcode_t	retval;
	वर्ण		*मूल्य;

	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स_रखो(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना test_private_data), &data);
	यदि (retval)
		गच्छ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	स_रखो(data, 0, परिमाण(संरचना test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	यदि (test_io_backing_manager) {
		retval = test_io_backing_manager->खोलो(name, flags,
						       &data->real);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = रिक्त;
	यदि ((मूल्य = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != रिक्त)
		data->outfile = ख_खोलो(मूल्य, "w");
	यदि (!data->outfile)
		data->outfile = मानक_त्रुटि;

	data->flags = 0;
	यदि ((मूल्य = ext2fs_safe_getenv("TEST_IO_FLAGS")) != रिक्त)
		data->flags = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->block = 0;
	यदि ((मूल्य = ext2fs_safe_getenv("TEST_IO_BLOCK")) != रिक्त)
		data->block = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->read_abort_गिनती = 0;
	यदि ((मूल्य = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != रिक्त)
		data->read_abort_गिनती = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->write_abort_गिनती = 0;
	यदि ((मूल्य = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != रिक्त)
		data->write_abort_गिनती = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	यदि (data->real) {
		io->align = data->real->align;
		यदि (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	प्रत्यागम 0;

cleanup:
	यदि (io && io->name)
		ext2fs_free_mem(&io->name);
	यदि (io)
		ext2fs_free_mem(&io);
	यदि (data)
		ext2fs_free_mem(&data);
	प्रत्यागम retval;
}

स्थिर errcode_t test_close(io_channel channel)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;

	यदि (data->real)
		retval = io_channel_close(data->real);

	यदि (data->outfile && data->outfile != मानक_त्रुटि)
		ख_बंद(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t test_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	यदि (data->set_blksize)
		data->set_blksize(blksize, retval);
	यदि (data->flags & TEST_FLAG_SET_BLKSIZE)
		ख_लिखो(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	प्रत्यागम retval;
}


स्थिर errcode_t test_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गिनती, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_read_blk(data->real, block, गिनती, buf);
	यदि (data->read_blk)
		data->read_blk(block, गिनती, retval);
	यदि (data->flags & TEST_FLAG_READ)
		ख_लिखो(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	यदि (data->block && data->block == block) {
		यदि (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		यदि (--data->read_abort_गिनती == 0)
			test_abort(channel, block);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_write_blk(data->real, block, गिनती, buf);
	यदि (data->write_blk)
		data->write_blk(block, गिनती, retval);
	यदि (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	यदि (data->block && data->block == block) {
		यदि (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		यदि (--data->write_abort_गिनती == 0)
			test_abort(channel, block);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गिनती, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_read_blk64(data->real, block, गिनती, buf);
	यदि (data->read_blk64)
		data->read_blk64(block, गिनती, retval);
	यदि (data->flags & TEST_FLAG_READ)
		ख_लिखो(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	यदि (data->block && data->block == block) {
		यदि (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		यदि (--data->read_abort_गिनती == 0)
			test_abort(channel, block);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_write_blk64(data->real, block, गिनती, buf);
	यदि (data->write_blk64)
		data->write_blk64(block, गिनती, retval);
	यदि (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	यदि (data->block && data->block == block) {
		यदि (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		यदि (--data->write_abort_गिनती == 0)
			test_abort(channel, block);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
			       पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, गिनती, buf);
	यदि (data->write_byte)
		data->write_byte(offset, गिनती, retval);
	यदि (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, गिनती, retval ? error_message(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_set_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	यदि (data->flags & TEST_FLAG_SET_OPTION)
		ख_लिखो(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	यदि (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		यदि (data->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} अन्यथा {
		यदि (data->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(data->outfile, "not implemented\n");
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_discard(data->real, block, गिनती);
	यदि (data->flags & TEST_FLAG_DISCARD)
		ख_लिखो(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_cache_readahead(data->real, block, गिनती);
	यदि (data->flags & TEST_FLAG_READAHEAD)
		ख_लिखो(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_zeroout(data->real, block, गिनती);
	यदि (data->flags & TEST_FLAG_ZEROOUT)
		ख_लिखो(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, गिनती, retval ? error_message(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर संरचना struct_io_manager struct_test_manager = {
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
