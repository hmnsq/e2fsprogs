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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जर ((संरचना)->magic != (code)) परत (code)

संरचना test_private_data {
	पूर्णांक	magic;
	io_channel real;
	पूर्णांक flags;
	खाता *outfile;
	अचिन्हित दीर्घ block;
	पूर्णांक read_abort_मोजणी, write_abort_मोजणी;
	शून्य (*read_blk)(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err);
	शून्य (*write_blk)(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err);
	शून्य (*set_blksize)(पूर्णांक blksize, errcode_t err);
	शून्य (*write_byte)(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err);
	शून्य (*read_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी, errcode_t err);
	शून्य (*write_blk64)(अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
शून्य (*test_io_cb_read_blk)
	(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk)
	(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err) = 0;
शून्य (*test_io_cb_read_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी, errcode_t err) = 0;
शून्य (*test_io_cb_set_blksize)
	(पूर्णांक blksize, errcode_t err) = 0;
शून्य (*test_io_cb_write_byte)
	(अचिन्हित दीर्घ block, पूर्णांक मोजणी, errcode_t err) = 0;

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
			    अचिन्हित दीर्घ block, स्थिरांक शून्य *buf)
{
	स्थिरांक अचिन्हित अक्षर *cp;
	खाता *f = data->outfile;
	पूर्णांक	i;
	अचिन्हित दीर्घ	cksum = 0;

	साठी (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	ख_लिखो(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	साठी (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		जर ((i % 16) == 0)
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

	जर (data->real)
		retval = io_channel_flush(data->real);

	जर (data->flags & TEST_FLAG_FLUSH)
		ख_लिखो(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	परत retval;
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

स्थिर errcode_t test_open(स्थिरांक अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना test_private_data *data = रिक्त;
	errcode_t	retval;
	अक्षर		*मूल्य;

	जर (name == 0)
		परत EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(आकार(संरचना struct_io_channel), &io);
	जर (retval)
		जा cleanup;
	स_रखो(io, 0, आकार(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(आकार(संरचना test_private_data), &data);
	जर (retval)
		जा cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	जर (retval)
		जा cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	स_रखो(data, 0, आकार(संरचना test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	जर (test_io_backing_manager) {
		retval = test_io_backing_manager->खोलो(name, flags,
						       &data->real);
		जर (retval)
			जा cleanup;
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
	जर ((मूल्य = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != रिक्त)
		data->outfile = ख_खोलो(मूल्य, "w");
	जर (!data->outfile)
		data->outfile = मानक_त्रुटि;

	data->flags = 0;
	जर ((मूल्य = ext2fs_safe_getenv("TEST_IO_FLAGS")) != रिक्त)
		data->flags = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->block = 0;
	जर ((मूल्य = ext2fs_safe_getenv("TEST_IO_BLOCK")) != रिक्त)
		data->block = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->read_abort_मोजणी = 0;
	जर ((मूल्य = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != रिक्त)
		data->read_abort_मोजणी = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	data->write_abort_मोजणी = 0;
	जर ((मूल्य = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != रिक्त)
		data->write_abort_मोजणी = म_से_अदीर्घ(मूल्य, रिक्त, 0);

	जर (data->real) {
		io->align = data->real->align;
		जर (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	परत 0;

cleanup:
	जर (io && io->name)
		ext2fs_free_mem(&io->name);
	जर (io)
		ext2fs_free_mem(&io);
	जर (data)
		ext2fs_free_mem(&data);
	परत retval;
}

स्थिर errcode_t test_close(io_channel channel)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (--channel->refcount > 0)
		परत 0;

	जर (data->real)
		retval = io_channel_close(data->real);

	जर (data->outfile && data->outfile != मानक_त्रुटि)
		ख_बंद(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	जर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	परत retval;
}

स्थिर errcode_t test_set_blksize(io_channel channel, पूर्णांक blksize)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	जर (data->set_blksize)
		data->set_blksize(blksize, retval);
	जर (data->flags & TEST_FLAG_SET_BLKSIZE)
		ख_लिखो(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	परत retval;
}


स्थिर errcode_t test_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_read_blk(data->real, block, मोजणी, buf);
	जर (data->read_blk)
		data->read_blk(block, मोजणी, retval);
	जर (data->flags & TEST_FLAG_READ)
		ख_लिखो(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	जर (data->block && data->block == block) {
		जर (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		जर (--data->read_abort_मोजणी == 0)
			test_abort(channel, block);
	}
	परत retval;
}

स्थिर errcode_t test_write_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_write_blk(data->real, block, मोजणी, buf);
	जर (data->write_blk)
		data->write_blk(block, मोजणी, retval);
	जर (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	जर (data->block && data->block == block) {
		जर (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		जर (--data->write_abort_मोजणी == 0)
			test_abort(channel, block);
	}
	परत retval;
}

स्थिर errcode_t test_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_read_blk64(data->real, block, मोजणी, buf);
	जर (data->read_blk64)
		data->read_blk64(block, मोजणी, retval);
	जर (data->flags & TEST_FLAG_READ)
		ख_लिखो(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	जर (data->block && data->block == block) {
		जर (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		जर (--data->read_abort_मोजणी == 0)
			test_abort(channel, block);
	}
	परत retval;
}

स्थिर errcode_t test_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_write_blk64(data->real, block, मोजणी, buf);
	जर (data->write_blk64)
		data->write_blk64(block, मोजणी, retval);
	जर (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	जर (data->block && data->block == block) {
		जर (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		जर (--data->write_abort_मोजणी == 0)
			test_abort(channel, block);
	}
	परत retval;
}

स्थिर errcode_t test_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
			       पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, मोजणी, buf);
	जर (data->write_byte)
		data->write_byte(offset, मोजणी, retval);
	जर (data->flags & TEST_FLAG_WRITE)
		ख_लिखो(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, मोजणी, retval ? error_message(retval) : "OK");
	परत retval;
}

स्थिर errcode_t test_set_option(io_channel channel, स्थिरांक अक्षर *option,
				 स्थिरांक अक्षर *arg)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	जर (data->flags & TEST_FLAG_SET_OPTION)
		ख_लिखो(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	जर (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		जर (data->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} अन्यथा {
		जर (data->flags & TEST_FLAG_SET_OPTION)
			ख_लिखो(data->outfile, "not implemented\n");
	}
	परत retval;
}

स्थिर errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	परत retval;
}

स्थिर errcode_t test_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_discard(data->real, block, मोजणी);
	जर (data->flags & TEST_FLAG_DISCARD)
		ख_लिखो(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	परत retval;
}

स्थिर errcode_t test_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_cache_readahead(data->real, block, मोजणी);
	जर (data->flags & TEST_FLAG_READAHEAD)
		ख_लिखो(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	परत retval;
}

स्थिर errcode_t test_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_zeroout(data->real, block, मोजणी);
	जर (data->flags & TEST_FLAG_ZEROOUT)
		ख_लिखो(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, मोजणी, retval ? error_message(retval) : "OK");
	परत retval;
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
