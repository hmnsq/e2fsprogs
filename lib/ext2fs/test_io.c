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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)

ساختار test_private_data {
	صحیح	magic;
	io_channel real;
	صحیح flags;
	FILE *outfile;
	unsigned long block;
	صحیح read_abort_شمار, write_abort_شمار;
	تهی (*read_blk)(unsigned long block, صحیح شمار, errcode_t err);
	تهی (*write_blk)(unsigned long block, صحیح شمار, errcode_t err);
	تهی (*set_blksize)(صحیح blksize, errcode_t err);
	تهی (*write_byte)(unsigned long block, صحیح شمار, errcode_t err);
	تهی (*read_blk64)(unsigned long long block, صحیح شمار, errcode_t err);
	تهی (*write_blk64)(unsigned long long block, صحیح شمار, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
تهی (*test_io_cb_read_blk)
	(unsigned long block, صحیح شمار, errcode_t err) = 0;
تهی (*test_io_cb_write_blk)
	(unsigned long block, صحیح شمار, errcode_t err) = 0;
تهی (*test_io_cb_read_blk64)
	(unsigned long long block, صحیح شمار, errcode_t err) = 0;
تهی (*test_io_cb_write_blk64)
	(unsigned long long block, صحیح شمار, errcode_t err) = 0;
تهی (*test_io_cb_set_blksize)
	(صحیح blksize, errcode_t err) = 0;
تهی (*test_io_cb_write_byte)
	(unsigned long block, صحیح شمار, errcode_t err) = 0;

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

ایستا تهی test_dump_block(io_channel channel,
			    ساختار test_private_data *data,
			    unsigned long block, ثابت تهی *buf)
{
	ثابت unsigned نویسه *cp;
	FILE *f = data->outfile;
	صحیح	i;
	unsigned long	cksum = 0;

	برای (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	برای (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		اگر ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ایستا errcode_t test_flush(io_channel channel)
{
	ساختار test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_flush(data->real);

	اگر (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	بازگشت retval;
}

ایستا تهی test_abort(io_channel channel, unsigned long block)
{
	ساختار test_private_data *data;
	FILE *f;

	data = (ساختار test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

ایستا errcode_t test_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	io_channel	io = پوچ;
	ساختار test_private_data *data = پوچ;
	errcode_t	retval;
	نویسه		*مقدار;

	اگر (name == 0)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(اندازه(ساختار struct_io_channel), &io);
	اگر (retval)
		برو cleanup;
	memset(io, 0, اندازه(ساختار struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(اندازه(ساختار test_private_data), &data);
	اگر (retval)
		برو cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	اگر (retval)
		برو cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, اندازه(ساختار test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	اگر (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		اگر (retval)
			برو cleanup;
	} وگرنه {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = پوچ;
	اگر ((مقدار = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != پوچ)
		data->outfile = fopen(مقدار, "w");
	اگر (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	اگر ((مقدار = ext2fs_safe_getenv("TEST_IO_FLAGS")) != پوچ)
		data->flags = strtoul(مقدار, پوچ, 0);

	data->block = 0;
	اگر ((مقدار = ext2fs_safe_getenv("TEST_IO_BLOCK")) != پوچ)
		data->block = strtoul(مقدار, پوچ, 0);

	data->read_abort_شمار = 0;
	اگر ((مقدار = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != پوچ)
		data->read_abort_شمار = strtoul(مقدار, پوچ, 0);

	data->write_abort_شمار = 0;
	اگر ((مقدار = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != پوچ)
		data->write_abort_شمار = strtoul(مقدار, پوچ, 0);

	اگر (data->real) {
		io->align = data->real->align;
		اگر (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	بازگشت 0;

cleanup:
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (io)
		ext2fs_free_mem(&io);
	اگر (data)
		ext2fs_free_mem(&data);
	بازگشت retval;
}

ایستا errcode_t test_close(io_channel channel)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		بازگشت 0;

	اگر (data->real)
		retval = io_channel_close(data->real);

	اگر (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	بازگشت retval;
}

ایستا errcode_t test_set_blksize(io_channel channel, صحیح blksize)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	اگر (data->set_blksize)
		data->set_blksize(blksize, retval);
	اگر (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	بازگشت retval;
}


ایستا errcode_t test_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_read_blk(data->real, block, شمار, buf);
	اگر (data->read_blk)
		data->read_blk(block, شمار, retval);
	اگر (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	اگر (data->block && data->block == block) {
		اگر (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		اگر (--data->read_abort_شمار == 0)
			test_abort(channel, block);
	}
	بازگشت retval;
}

ایستا errcode_t test_write_blk(io_channel channel, unsigned long block,
			       صحیح شمار, ثابت تهی *buf)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_write_blk(data->real, block, شمار, buf);
	اگر (data->write_blk)
		data->write_blk(block, شمار, retval);
	اگر (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	اگر (data->block && data->block == block) {
		اگر (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		اگر (--data->write_abort_شمار == 0)
			test_abort(channel, block);
	}
	بازگشت retval;
}

ایستا errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       صحیح شمار, تهی *buf)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_read_blk64(data->real, block, شمار, buf);
	اگر (data->read_blk64)
		data->read_blk64(block, شمار, retval);
	اگر (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	اگر (data->block && data->block == block) {
		اگر (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		اگر (--data->read_abort_شمار == 0)
			test_abort(channel, block);
	}
	بازگشت retval;
}

ایستا errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       صحیح شمار, ثابت تهی *buf)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_write_blk64(data->real, block, شمار, buf);
	اگر (data->write_blk64)
		data->write_blk64(block, شمار, retval);
	اگر (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	اگر (data->block && data->block == block) {
		اگر (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		اگر (--data->write_abort_شمار == 0)
			test_abort(channel, block);
	}
	بازگشت retval;
}

ایستا errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       صحیح شمار, ثابت تهی *buf)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, شمار, buf);
	اگر (data->write_byte)
		data->write_byte(offset, شمار, retval);
	اگر (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, شمار, retval ? error_message(retval) : "OK");
	بازگشت retval;
}

ایستا errcode_t test_set_option(io_channel channel, ثابت نویسه *option,
				 ثابت نویسه *arg)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	اگر (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	اگر (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		اگر (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} وگرنه {
		اگر (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	بازگشت retval;
}

ایستا errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	بازگشت retval;
}

ایستا errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_discard(data->real, block, شمار);
	اگر (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	بازگشت retval;
}

ایستا errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long شمار)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_cache_readahead(data->real, block, شمار);
	اگر (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	بازگشت retval;
}

ایستا errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_zeroout(data->real, block, شمار);
	اگر (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, شمار, retval ? error_message(retval) : "OK");
	بازگشت retval;
}

ایستا ساختار struct_io_manager struct_test_manager = {
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
