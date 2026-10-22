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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  إذا ((بنية)->magic != (code)) أرجع (code)

بنية test_private_data {
	صحيح	magic;
	io_channel real;
	صحيح flags;
	FILE *outfile;
	unsigned long block;
	صحيح read_abort_کيراءن, write_abort_کيراءن;
	فراغ (*read_blk)(unsigned long block, صحيح کيراءن, errcode_t err);
	فراغ (*write_blk)(unsigned long block, صحيح کيراءن, errcode_t err);
	فراغ (*set_blksize)(صحيح blksize, errcode_t err);
	فراغ (*write_byte)(unsigned long block, صحيح کيراءن, errcode_t err);
	فراغ (*read_blk64)(unsigned long long block, صحيح کيراءن, errcode_t err);
	فراغ (*write_blk64)(unsigned long long block, صحيح کيراءن, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
فراغ (*test_io_cb_read_blk)
	(unsigned long block, صحيح کيراءن, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk)
	(unsigned long block, صحيح کيراءن, errcode_t err) = 0;
فراغ (*test_io_cb_read_blk64)
	(unsigned long long block, صحيح کيراءن, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk64)
	(unsigned long long block, صحيح کيراءن, errcode_t err) = 0;
فراغ (*test_io_cb_set_blksize)
	(صحيح blksize, errcode_t err) = 0;
فراغ (*test_io_cb_write_byte)
	(unsigned long block, صحيح کيراءن, errcode_t err) = 0;

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

ساكن فراغ test_dump_block(io_channel channel,
			    بنية test_private_data *data,
			    unsigned long block, ثابت فراغ *buf)
{
	ثابت unsigned حرف *cp;
	FILE *f = data->outfile;
	صحيح	i;
	unsigned long	cksum = 0;

	لكل (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	لكل (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		إذا ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t test_flush(io_channel channel)
{
	بنية test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_flush(data->real);

	إذا (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	أرجع retval;
}

ساكن فراغ test_abort(io_channel channel, unsigned long block)
{
	بنية test_private_data *data;
	FILE *f;

	data = (بنية test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

ساكن errcode_t test_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية test_private_data *data = عدم;
	errcode_t	retval;
	حرف		*نيلاي;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية test_private_data), &data);
	إذا (retval)
		اذهب cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, حجم(بنية test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	إذا (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = عدم;
	إذا ((نيلاي = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != عدم)
		data->outfile = fopen(نيلاي, "w");
	إذا (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	إذا ((نيلاي = ext2fs_safe_getenv("TEST_IO_FLAGS")) != عدم)
		data->flags = strtoul(نيلاي, عدم, 0);

	data->block = 0;
	إذا ((نيلاي = ext2fs_safe_getenv("TEST_IO_BLOCK")) != عدم)
		data->block = strtoul(نيلاي, عدم, 0);

	data->read_abort_کيراءن = 0;
	إذا ((نيلاي = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != عدم)
		data->read_abort_کيراءن = strtoul(نيلاي, عدم, 0);

	data->write_abort_کيراءن = 0;
	إذا ((نيلاي = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != عدم)
		data->write_abort_کيراءن = strtoul(نيلاي, عدم, 0);

	إذا (data->real) {
		io->align = data->real->align;
		إذا (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	إذا (data)
		ext2fs_free_mem(&data);
	أرجع retval;
}

ساكن errcode_t test_close(io_channel channel)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	إذا (data->real)
		retval = io_channel_close(data->real);

	إذا (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t test_set_blksize(io_channel channel, صحيح blksize)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	إذا (data->set_blksize)
		data->set_blksize(blksize, retval);
	إذا (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	أرجع retval;
}


ساكن errcode_t test_read_blk(io_channel channel, unsigned long block,
			       صحيح کيراءن, فراغ *buf)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_read_blk(data->real, block, کيراءن, buf);
	إذا (data->read_blk)
		data->read_blk(block, کيراءن, retval);
	إذا (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	إذا (data->block && data->block == block) {
		إذا (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		إذا (--data->read_abort_کيراءن == 0)
			test_abort(channel, block);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk(io_channel channel, unsigned long block,
			       صحيح کيراءن, ثابت فراغ *buf)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_write_blk(data->real, block, کيراءن, buf);
	إذا (data->write_blk)
		data->write_blk(block, کيراءن, retval);
	إذا (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	إذا (data->block && data->block == block) {
		إذا (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		إذا (--data->write_abort_کيراءن == 0)
			test_abort(channel, block);
	}
	أرجع retval;
}

ساكن errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       صحيح کيراءن, فراغ *buf)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_read_blk64(data->real, block, کيراءن, buf);
	إذا (data->read_blk64)
		data->read_blk64(block, کيراءن, retval);
	إذا (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	إذا (data->block && data->block == block) {
		إذا (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		إذا (--data->read_abort_کيراءن == 0)
			test_abort(channel, block);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       صحيح کيراءن, ثابت فراغ *buf)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_write_blk64(data->real, block, کيراءن, buf);
	إذا (data->write_blk64)
		data->write_blk64(block, کيراءن, retval);
	إذا (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	إذا (data->block && data->block == block) {
		إذا (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		إذا (--data->write_abort_کيراءن == 0)
			test_abort(channel, block);
	}
	أرجع retval;
}

ساكن errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       صحيح کيراءن, ثابت فراغ *buf)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, کيراءن, buf);
	إذا (data->write_byte)
		data->write_byte(offset, کيراءن, retval);
	إذا (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, کيراءن, retval ? error_message(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_set_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	إذا (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	إذا (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		إذا (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} وإلا {
		إذا (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	أرجع retval;
}

ساكن errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	أرجع retval;
}

ساكن errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long کيراءن)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_discard(data->real, block, کيراءن);
	إذا (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long کيراءن)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_cache_readahead(data->real, block, کيراءن);
	إذا (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long کيراءن)
{
	بنية test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_zeroout(data->real, block, کيراءن);
	إذا (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, کيراءن, retval ? error_message(retval) : "OK");
	أرجع retval;
}

ساكن بنية struct_io_manager struct_test_manager = {
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
