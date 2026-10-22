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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  सचे ((संरचना)->magic != (code)) पटिनिवत्त (code)

संरचना test_private_data {
	पूर्णाङ्क	magic;
	io_channel real;
	पूर्णाङ्क flags;
	FILE *outfile;
	unsigned long block;
	पूर्णाङ्क read_abort_गणनम्, write_abort_गणनम्;
	शून्य (*read_blk)(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*write_blk)(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*set_blksize)(पूर्णाङ्क blksize, errcode_t err);
	शून्य (*write_byte)(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*read_blk64)(unsigned long long block, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*write_blk64)(unsigned long long block, पूर्णाङ्क गणनम्, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
शून्य (*test_io_cb_read_blk)
	(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk)
	(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_read_blk64)
	(unsigned long long block, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk64)
	(unsigned long long block, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_set_blksize)
	(पूर्णाङ्क blksize, errcode_t err) = 0;
शून्य (*test_io_cb_write_byte)
	(unsigned long block, पूर्णाङ्क गणनम्, errcode_t err) = 0;

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
			    unsigned long block, ध्रुव शून्य *buf)
{
	ध्रुव unsigned वर्ण *cp;
	FILE *f = data->outfile;
	पूर्णाङ्क	i;
	unsigned long	cksum = 0;

	अत्थाय (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	अत्थाय (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		सचे ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
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

	सचे (data->real)
		retval = io_channel_flush(data->real);

	सचे (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	पटिनिवत्त retval;
}

स्थिर शून्य test_abort(io_channel channel, unsigned long block)
{
	संरचना test_private_data *data;
	FILE *f;

	data = (संरचना test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

स्थिर errcode_t test_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना test_private_data *data = रिक्त;
	errcode_t	retval;
	वर्ण		*मानम्;

	सचे (name == 0)
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	सचे (retval)
		गच्छ cleanup;
	memset(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना test_private_data), &data);
	सचे (retval)
		गच्छ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	सचे (retval)
		गच्छ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, परिमाण(संरचना test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	सचे (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		सचे (retval)
			गच्छ cleanup;
	} अञ्ञथा {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = रिक्त;
	सचे ((मानम् = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != रिक्त)
		data->outfile = fopen(मानम्, "w");
	सचे (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	सचे ((मानम् = ext2fs_safe_getenv("TEST_IO_FLAGS")) != रिक्त)
		data->flags = strtoul(मानम्, रिक्त, 0);

	data->block = 0;
	सचे ((मानम् = ext2fs_safe_getenv("TEST_IO_BLOCK")) != रिक्त)
		data->block = strtoul(मानम्, रिक्त, 0);

	data->read_abort_गणनम् = 0;
	सचे ((मानम् = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != रिक्त)
		data->read_abort_गणनम् = strtoul(मानम्, रिक्त, 0);

	data->write_abort_गणनम् = 0;
	सचे ((मानम् = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != रिक्त)
		data->write_abort_गणनम् = strtoul(मानम्, रिक्त, 0);

	सचे (data->real) {
		io->align = data->real->align;
		सचे (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	पटिनिवत्त 0;

cleanup:
	सचे (io && io->name)
		ext2fs_free_mem(&io->name);
	सचे (io)
		ext2fs_free_mem(&io);
	सचे (data)
		ext2fs_free_mem(&data);
	पटिनिवत्त retval;
}

स्थिर errcode_t test_close(io_channel channel)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (--channel->refcount > 0)
		पटिनिवत्त 0;

	सचे (data->real)
		retval = io_channel_close(data->real);

	सचे (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	सचे (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	पटिनिवत्त retval;
}

स्थिर errcode_t test_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	सचे (data->set_blksize)
		data->set_blksize(blksize, retval);
	सचे (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	पटिनिवत्त retval;
}


स्थिर errcode_t test_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_read_blk(data->real, block, गणनम्, buf);
	सचे (data->read_blk)
		data->read_blk(block, गणनम्, retval);
	सचे (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	सचे (data->block && data->block == block) {
		सचे (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		सचे (--data->read_abort_गणनम् == 0)
			test_abort(channel, block);
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_write_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_write_blk(data->real, block, गणनम्, buf);
	सचे (data->write_blk)
		data->write_blk(block, गणनम्, retval);
	सचे (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	सचे (data->block && data->block == block) {
		सचे (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		सचे (--data->write_abort_गणनम् == 0)
			test_abort(channel, block);
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_read_blk64(data->real, block, गणनम्, buf);
	सचे (data->read_blk64)
		data->read_blk64(block, गणनम्, retval);
	सचे (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	सचे (data->block && data->block == block) {
		सचे (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		सचे (--data->read_abort_गणनम् == 0)
			test_abort(channel, block);
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_write_blk64(data->real, block, गणनम्, buf);
	सचे (data->write_blk64)
		data->write_blk64(block, गणनम्, retval);
	सचे (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	सचे (data->block && data->block == block) {
		सचे (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		सचे (--data->write_abort_गणनम् == 0)
			test_abort(channel, block);
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, गणनम्, buf);
	सचे (data->write_byte)
		data->write_byte(offset, गणनम्, retval);
	सचे (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, गणनम्, retval ? error_message(retval) : "OK");
	पटिनिवत्त retval;
}

स्थिर errcode_t test_set_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	सचे (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	सचे (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		सचे (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} अञ्ञथा {
		सचे (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	पटिनिवत्त retval;
}

स्थिर errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long गणनम्)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_discard(data->real, block, गणनम्);
	सचे (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	पटिनिवत्त retval;
}

स्थिर errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long गणनम्)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_cache_readahead(data->real, block, गणनम्);
	सचे (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	पटिनिवत्त retval;
}

स्थिर errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long गणनम्)
{
	संरचना test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	सचे (data->real)
		retval = io_channel_zeroout(data->real, block, गणनम्);
	सचे (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, गणनम्, retval ? error_message(retval) : "OK");
	पटिनिवत्त retval;
}

स्थिर संरचना struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
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
