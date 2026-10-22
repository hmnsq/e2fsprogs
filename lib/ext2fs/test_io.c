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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)

ସଂରଚନା test_private_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	magic;
	io_channel real;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	FILE *outfile;
	unsigned long block;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_abort_ଗଣନା, write_abort_ଗଣନା;
	ଶୂନ୍ୟ (*read_blk)(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err);
	ଶୂନ୍ୟ (*write_blk)(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err);
	ଶୂନ୍ୟ (*set_blksize)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize, errcode_t err);
	ଶୂନ୍ୟ (*write_byte)(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err);
	ଶୂନ୍ୟ (*read_blk64)(unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err);
	ଶୂନ୍ୟ (*write_blk64)(unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
ଶୂନ୍ୟ (*test_io_cb_read_blk)
	(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err) = 0;
ଶୂନ୍ୟ (*test_io_cb_write_blk)
	(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err) = 0;
ଶୂନ୍ୟ (*test_io_cb_read_blk64)
	(unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err) = 0;
ଶୂନ୍ୟ (*test_io_cb_write_blk64)
	(unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err) = 0;
ଶୂନ୍ୟ (*test_io_cb_set_blksize)
	(ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize, errcode_t err) = 0;
ଶୂନ୍ୟ (*test_io_cb_write_byte)
	(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err) = 0;

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

ସ୍ଥିର ଶୂନ୍ୟ test_dump_block(io_channel channel,
			    ସଂରଚନା test_private_data *data,
			    unsigned long block, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଅଚଳ unsigned ଅକ୍ଷର *cp;
	FILE *f = data->outfile;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;
	unsigned long	cksum = 0;

	ପାଇଁ (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	ପାଇଁ (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		ଯଦି ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ସ୍ଥିର errcode_t test_flush(io_channel channel)
{
	ସଂରଚନା test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_flush(data->real);

	ଯଦି (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଶୂନ୍ୟ test_abort(io_channel channel, unsigned long block)
{
	ସଂରଚନା test_private_data *data;
	FILE *f;

	data = (ସଂରଚନା test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

ସ୍ଥିର errcode_t test_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	io_channel	io = ଖାଲି;
	ସଂରଚନା test_private_data *data = ଖାଲି;
	errcode_t	retval;
	ଅକ୍ଷର		*ମୂଲ୍ୟ;

	ଯଦି (name == 0)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_io_channel), &io);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(io, 0, ଆକାର(ସଂରଚନା struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା test_private_data), &data);
	ଯଦି (retval)
		ଯାଅ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ଯଦି (retval)
		ଯାଅ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, ଆକାର(ସଂରଚନା test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ଯଦି (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		ଯଦି (retval)
			ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = ଖାଲି;
	ଯଦି ((ମୂଲ୍ୟ = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != ଖାଲି)
		data->outfile = fopen(ମୂଲ୍ୟ, "w");
	ଯଦି (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	ଯଦି ((ମୂଲ୍ୟ = ext2fs_safe_getenv("TEST_IO_FLAGS")) != ଖାଲି)
		data->flags = strtoul(ମୂଲ୍ୟ, ଖାଲି, 0);

	data->block = 0;
	ଯଦି ((ମୂଲ୍ୟ = ext2fs_safe_getenv("TEST_IO_BLOCK")) != ଖାଲି)
		data->block = strtoul(ମୂଲ୍ୟ, ଖାଲି, 0);

	data->read_abort_ଗଣନା = 0;
	ଯଦି ((ମୂଲ୍ୟ = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != ଖାଲି)
		data->read_abort_ଗଣନା = strtoul(ମୂଲ୍ୟ, ଖାଲି, 0);

	data->write_abort_ଗଣନା = 0;
	ଯଦି ((ମୂଲ୍ୟ = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != ଖାଲି)
		data->write_abort_ଗଣନା = strtoul(ମୂଲ୍ୟ, ଖାଲି, 0);

	ଯଦି (data->real) {
		io->align = data->real->align;
		ଯଦି (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	ଫେରନ୍ତୁ 0;

cleanup:
	ଯଦି (io && io->name)
		ext2fs_free_mem(&io->name);
	ଯଦି (io)
		ext2fs_free_mem(&io);
	ଯଦି (data)
		ext2fs_free_mem(&data);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_close(io_channel channel)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
		ଫେରନ୍ତୁ 0;

	ଯଦି (data->real)
		retval = io_channel_close(data->real);

	ଯଦି (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	ଯଦି (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	ଯଦି (data->set_blksize)
		data->set_blksize(blksize, retval);
	ଯଦି (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	ଫେରନ୍ତୁ retval;
}


ସ୍ଥିର errcode_t test_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_read_blk(data->real, block, ଗଣନା_2, buf);
	ଯଦି (data->read_blk)
		data->read_blk(block, ଗଣନା_2, retval);
	ଯଦି (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଯଦି (data->block && data->block == block) {
		ଯଦି (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ଯଦି (--data->read_abort_ଗଣନା == 0)
			test_abort(channel, block);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_write_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_write_blk(data->real, block, ଗଣନା_2, buf);
	ଯଦି (data->write_blk)
		data->write_blk(block, ଗଣନା_2, retval);
	ଯଦି (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଯଦି (data->block && data->block == block) {
		ଯଦି (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ଯଦି (--data->write_abort_ଗଣନା == 0)
			test_abort(channel, block);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_read_blk64(data->real, block, ଗଣନା_2, buf);
	ଯଦି (data->read_blk64)
		data->read_blk64(block, ଗଣନା_2, retval);
	ଯଦି (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଯଦି (data->block && data->block == block) {
		ଯଦି (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ଯଦି (--data->read_abort_ଗଣନା == 0)
			test_abort(channel, block);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_write_blk64(data->real, block, ଗଣନା_2, buf);
	ଯଦି (data->write_blk64)
		data->write_blk64(block, ଗଣନା_2, retval);
	ଯଦି (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଯଦି (data->block && data->block == block) {
		ଯଦି (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ଯଦି (--data->write_abort_ଗଣନା == 0)
			test_abort(channel, block);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, ଗଣନା_2, buf);
	ଯଦି (data->write_byte)
		data->write_byte(offset, ଗଣନା_2, retval);
	ଯଦି (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_set_option(io_channel channel, ଅଚଳ ଅକ୍ଷର *option,
				 ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ଯଦି (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ଯଦି (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		ଯଦି (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} ଅନ୍ୟଥା {
		ଯଦି (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_discard(data->real, block, ଗଣନା_2);
	ଯଦି (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_cache_readahead(data->real, block, ଗଣନା_2);
	ଯଦି (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_zeroout(data->real, block, ଗଣନା_2);
	ଯଦି (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, ଗଣନା_2, retval ? error_message(retval) : "OK");
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_test_manager = {
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
