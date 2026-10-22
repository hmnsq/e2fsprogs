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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)

கட்டமைப்பு test_private_data {
	முழு	magic;
	io_channel real;
	முழு flags;
	FILE *outfile;
	unsigned long block;
	முழு read_abort_மொத்தம், write_abort_மொத்தம்;
	வெறுமை (*read_blk)(unsigned long block, முழு மொத்தம், errcode_t err);
	வெறுமை (*write_blk)(unsigned long block, முழு மொத்தம், errcode_t err);
	வெறுமை (*set_blksize)(முழு blksize, errcode_t err);
	வெறுமை (*write_byte)(unsigned long block, முழு மொத்தம், errcode_t err);
	வெறுமை (*read_blk64)(unsigned long long block, முழு மொத்தம், errcode_t err);
	வெறுமை (*write_blk64)(unsigned long long block, முழு மொத்தம், errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
வெறுமை (*test_io_cb_read_blk)
	(unsigned long block, முழு மொத்தம், errcode_t err) = 0;
வெறுமை (*test_io_cb_write_blk)
	(unsigned long block, முழு மொத்தம், errcode_t err) = 0;
வெறுமை (*test_io_cb_read_blk64)
	(unsigned long long block, முழு மொத்தம், errcode_t err) = 0;
வெறுமை (*test_io_cb_write_blk64)
	(unsigned long long block, முழு மொத்தம், errcode_t err) = 0;
வெறுமை (*test_io_cb_set_blksize)
	(முழு blksize, errcode_t err) = 0;
வெறுமை (*test_io_cb_write_byte)
	(unsigned long block, முழு மொத்தம், errcode_t err) = 0;

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

நிலையான வெறுமை test_dump_block(io_channel channel,
			    கட்டமைப்பு test_private_data *data,
			    unsigned long block, மாறிலி வெறுமை *buf)
{
	மாறிலி unsigned எழுத்து *cp;
	FILE *f = data->outfile;
	முழு	i;
	unsigned long	cksum = 0;

	க்கு (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	க்கு (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		எனில் ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
நிலையான errcode_t test_flush(io_channel channel)
{
	கட்டமைப்பு test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_flush(data->real);

	எனில் (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	திரும்பு retval;
}

நிலையான வெறுமை test_abort(io_channel channel, unsigned long block)
{
	கட்டமைப்பு test_private_data *data;
	FILE *f;

	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

நிலையான errcode_t test_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	io_channel	io = வெற்று;
	கட்டமைப்பு test_private_data *data = வெற்று;
	errcode_t	retval;
	எழுத்து		*மதிப்பு;

	எனில் (name == 0)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_io_channel), &io);
	எனில் (retval)
		செல் cleanup;
	memset(io, 0, அளவு(கட்டமைப்பு struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு test_private_data), &data);
	எனில் (retval)
		செல் cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	எனில் (retval)
		செல் cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, அளவு(கட்டமைப்பு test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	எனில் (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		எனில் (retval)
			செல் cleanup;
	} இல்லையெனில் {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = வெற்று;
	எனில் ((மதிப்பு = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != வெற்று)
		data->outfile = fopen(மதிப்பு, "w");
	எனில் (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	எனில் ((மதிப்பு = ext2fs_safe_getenv("TEST_IO_FLAGS")) != வெற்று)
		data->flags = strtoul(மதிப்பு, வெற்று, 0);

	data->block = 0;
	எனில் ((மதிப்பு = ext2fs_safe_getenv("TEST_IO_BLOCK")) != வெற்று)
		data->block = strtoul(மதிப்பு, வெற்று, 0);

	data->read_abort_மொத்தம் = 0;
	எனில் ((மதிப்பு = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != வெற்று)
		data->read_abort_மொத்தம் = strtoul(மதிப்பு, வெற்று, 0);

	data->write_abort_மொத்தம் = 0;
	எனில் ((மதிப்பு = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != வெற்று)
		data->write_abort_மொத்தம் = strtoul(மதிப்பு, வெற்று, 0);

	எனில் (data->real) {
		io->align = data->real->align;
		எனில் (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	திரும்பு 0;

cleanup:
	எனில் (io && io->name)
		ext2fs_free_mem(&io->name);
	எனில் (io)
		ext2fs_free_mem(&io);
	எனில் (data)
		ext2fs_free_mem(&data);
	திரும்பு retval;
}

நிலையான errcode_t test_close(io_channel channel)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
		திரும்பு 0;

	எனில் (data->real)
		retval = io_channel_close(data->real);

	எனில் (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	எனில் (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	திரும்பு retval;
}

நிலையான errcode_t test_set_blksize(io_channel channel, முழு blksize)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	எனில் (data->set_blksize)
		data->set_blksize(blksize, retval);
	எனில் (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	திரும்பு retval;
}


நிலையான errcode_t test_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_read_blk(data->real, block, மொத்தம், buf);
	எனில் (data->read_blk)
		data->read_blk(block, மொத்தம், retval);
	எனில் (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	எனில் (data->block && data->block == block) {
		எனில் (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		எனில் (--data->read_abort_மொத்தம் == 0)
			test_abort(channel, block);
	}
	திரும்பு retval;
}

நிலையான errcode_t test_write_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_write_blk(data->real, block, மொத்தம், buf);
	எனில் (data->write_blk)
		data->write_blk(block, மொத்தம், retval);
	எனில் (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	எனில் (data->block && data->block == block) {
		எனில் (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		எனில் (--data->write_abort_மொத்தம் == 0)
			test_abort(channel, block);
	}
	திரும்பு retval;
}

நிலையான errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_read_blk64(data->real, block, மொத்தம், buf);
	எனில் (data->read_blk64)
		data->read_blk64(block, மொத்தம், retval);
	எனில் (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	எனில் (data->block && data->block == block) {
		எனில் (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		எனில் (--data->read_abort_மொத்தம் == 0)
			test_abort(channel, block);
	}
	திரும்பு retval;
}

நிலையான errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_write_blk64(data->real, block, மொத்தம், buf);
	எனில் (data->write_blk64)
		data->write_blk64(block, மொத்தம், retval);
	எனில் (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	எனில் (data->block && data->block == block) {
		எனில் (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		எனில் (--data->write_abort_மொத்தம் == 0)
			test_abort(channel, block);
	}
	திரும்பு retval;
}

நிலையான errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, மொத்தம், buf);
	எனில் (data->write_byte)
		data->write_byte(offset, மொத்தம், retval);
	எனில் (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, மொத்தம், retval ? error_message(retval) : "OK");
	திரும்பு retval;
}

நிலையான errcode_t test_set_option(io_channel channel, மாறிலி எழுத்து *option,
				 மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	எனில் (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	எனில் (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		எனில் (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} இல்லையெனில் {
		எனில் (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	திரும்பு retval;
}

நிலையான errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	திரும்பு retval;
}

நிலையான errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_discard(data->real, block, மொத்தம்);
	எனில் (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	திரும்பு retval;
}

நிலையான errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long மொத்தம்)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_cache_readahead(data->real, block, மொத்தம்);
	எனில் (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	திரும்பு retval;
}

நிலையான errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_zeroout(data->real, block, மொத்தம்);
	எனில் (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, மொத்தம், retval ? error_message(retval) : "OK");
	திரும்பு retval;
}

நிலையான கட்டமைப்பு struct_io_manager struct_test_manager = {
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
