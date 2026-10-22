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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) ঘূৰাই (code)

সংরচনা test_private_data {
	পূর্ণাঙ্ক	magic;
	io_channel real;
	পূর্ণাঙ্ক flags;
	FILE *outfile;
	unsigned long block;
	পূর্ণাঙ্ক read_abort_গুনতি, write_abort_গুনতি;
	শূন্য (*read_blk)(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
	শূন্য (*write_blk)(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
	শূন্য (*set_blksize)(পূর্ণাঙ্ক blksize, errcode_t err);
	শূন্য (*write_byte)(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
	শূন্য (*read_blk64)(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
	শূন্য (*write_blk64)(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
শূন্য (*test_io_cb_read_blk)
	(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_write_blk)
	(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_read_blk64)
	(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_write_blk64)
	(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_set_blksize)
	(পূর্ণাঙ্ক blksize, errcode_t err) = 0;
শূন্য (*test_io_cb_write_byte)
	(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err) = 0;

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

স্থির শূন্য test_dump_block(io_channel channel,
			    সংরচনা test_private_data *data,
			    unsigned long block, ধ্রুৱ শূন্য *buf)
{
	ধ্রুৱ unsigned ৱর্ণ *cp;
	FILE *f = data->outfile;
	পূর্ণাঙ্ক	i;
	unsigned long	cksum = 0;

	বাবে (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	বাবে (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		যদি ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t test_flush(io_channel channel)
{
	সংরচনা test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_flush(data->real);

	যদি (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	ঘূৰাই retval;
}

স্থির শূন্য test_abort(io_channel channel, unsigned long block)
{
	সংরচনা test_private_data *data;
	FILE *f;

	data = (সংরচনা test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

স্থির errcode_t test_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	io_channel	io = রিক্ত;
	সংরচনা test_private_data *data = রিক্ত;
	errcode_t	retval;
	ৱর্ণ		*মান;

	যদি (name == 0)
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা struct_io_channel), &io);
	যদি (retval)
		গচ্ছ cleanup;
	memset(io, 0, পরিমাণ(সংরচনা struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা test_private_data), &data);
	যদি (retval)
		গচ্ছ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		গচ্ছ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, পরিমাণ(সংরচনা test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	যদি (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = রিক্ত;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != রিক্ত)
		data->outfile = fopen(মান, "w");
	যদি (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_FLAGS")) != রিক্ত)
		data->flags = strtoul(মান, রিক্ত, 0);

	data->block = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_BLOCK")) != রিক্ত)
		data->block = strtoul(মান, রিক্ত, 0);

	data->read_abort_গুনতি = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != রিক্ত)
		data->read_abort_গুনতি = strtoul(মান, রিক্ত, 0);

	data->write_abort_গুনতি = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != রিক্ত)
		data->write_abort_গুনতি = strtoul(মান, রিক্ত, 0);

	যদি (data->real) {
		io->align = data->real->align;
		যদি (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	ঘূৰাই 0;

cleanup:
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (io)
		ext2fs_free_mem(&io);
	যদি (data)
		ext2fs_free_mem(&data);
	ঘূৰাই retval;
}

স্থির errcode_t test_close(io_channel channel)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ঘূৰাই 0;

	যদি (data->real)
		retval = io_channel_close(data->real);

	যদি (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ঘূৰাই retval;
}

স্থির errcode_t test_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	যদি (data->set_blksize)
		data->set_blksize(blksize, retval);
	যদি (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	ঘূৰাই retval;
}


স্থির errcode_t test_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_read_blk(data->real, block, গুনতি, buf);
	যদি (data->read_blk)
		data->read_blk(block, গুনতি, retval);
	যদি (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (data->block && data->block == block) {
		যদি (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		যদি (--data->read_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_write_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_write_blk(data->real, block, গুনতি, buf);
	যদি (data->write_blk)
		data->write_blk(block, গুনতি, retval);
	যদি (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (data->block && data->block == block) {
		যদি (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		যদি (--data->write_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_read_blk64(data->real, block, গুনতি, buf);
	যদি (data->read_blk64)
		data->read_blk64(block, গুনতি, retval);
	যদি (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (data->block && data->block == block) {
		যদি (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		যদি (--data->read_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_write_blk64(data->real, block, গুনতি, buf);
	যদি (data->write_blk64)
		data->write_blk64(block, গুনতি, retval);
	যদি (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (data->block && data->block == block) {
		যদি (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		যদি (--data->write_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, গুনতি, buf);
	যদি (data->write_byte)
		data->write_byte(offset, গুনতি, retval);
	যদি (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, গুনতি, retval ? error_message(retval) : "OK");
	ঘূৰাই retval;
}

স্থির errcode_t test_set_option(io_channel channel, ধ্রুৱ ৱর্ণ *option,
				 ধ্রুৱ ৱর্ণ *arg)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	যদি (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	যদি (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		যদি (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} অন্যথা {
		যদি (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	ঘূৰাই retval;
}

স্থির errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_discard(data->real, block, গুনতি);
	যদি (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ঘূৰাই retval;
}

স্থির errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_cache_readahead(data->real, block, গুনতি);
	যদি (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ঘূৰাই retval;
}

স্থির errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_zeroout(data->real, block, গুনতি);
	যদি (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ঘূৰাই retval;
}

স্থির সংরচনা struct_io_manager struct_test_manager = {
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
