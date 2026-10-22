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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)

কাঠামো test_private_তথ্য {
	পূর্ণসংখ্যা	magic;
	io_channel real;
	পূর্ণসংখ্যা flags;
	FILE *outfile;
	unsigned long block;
	পূর্ণসংখ্যা read_abort_গুনতি, write_abort_গুনতি;
	শূন্য (*read_blk)(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err);
	শূন্য (*write_blk)(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err);
	শূন্য (*set_blksize)(পূর্ণসংখ্যা blksize, errcode_t err);
	শূন্য (*write_byte)(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err);
	শূন্য (*read_blk64)(unsigned long long block, পূর্ণসংখ্যা গুনতি, errcode_t err);
	শূন্য (*write_blk64)(unsigned long long block, পূর্ণসংখ্যা গুনতি, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
শূন্য (*test_io_cb_read_blk)
	(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_write_blk)
	(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_read_blk64)
	(unsigned long long block, পূর্ণসংখ্যা গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_write_blk64)
	(unsigned long long block, পূর্ণসংখ্যা গুনতি, errcode_t err) = 0;
শূন্য (*test_io_cb_set_blksize)
	(পূর্ণসংখ্যা blksize, errcode_t err) = 0;
শূন্য (*test_io_cb_write_byte)
	(unsigned long block, পূর্ণসংখ্যা গুনতি, errcode_t err) = 0;

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
			    কাঠামো test_private_তথ্য *তথ্য,
			    unsigned long block, ধ্রুবক শূন্য *buf)
{
	ধ্রুবক unsigned অক্ষর *cp;
	FILE *f = তথ্য->outfile;
	পূর্ণসংখ্যা	i;
	unsigned long	cksum = 0;

	জন্য (i=0, cp = buf; i < channel->block_আকার; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	জন্য (i=0, cp = buf; i < channel->block_আকার; i++, cp++) {
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
	কাঠামো test_private_তথ্য	*তথ্য;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *)channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_flush(তথ্য->real);

	যদি (তথ্য->flags & TEST_FLAG_FLUSH)
		fprintf(তথ্য->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	ফেরত retval;
}

স্থির শূন্য test_abort(io_channel channel, unsigned long block)
{
	কাঠামো test_private_তথ্য *তথ্য;
	FILE *f;

	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	f = তথ্য->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

স্থির errcode_t test_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	io_channel	io = ফাঁকা;
	কাঠামো test_private_তথ্য *তথ্য = ফাঁকা;
	errcode_t	retval;
	অক্ষর		*মান;

	যদি (name == 0)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(আকার(কাঠামো struct_io_channel), &io);
	যদি (retval)
		যাও cleanup;
	memset(io, 0, আকার(কাঠামো struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(আকার(কাঠামো test_private_তথ্য), &তথ্য);
	যদি (retval)
		যাও cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		যাও cleanup;

	strcpy(io->name, name);
	io->private_তথ্য = তথ্য;
	io->block_আকার = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(তথ্য, 0, আকার(কাঠামো test_private_তথ্য));
	তথ্য->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	যদি (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &তথ্য->real);
		যদি (retval)
			যাও cleanup;
	} অন্যথা {
		তথ্য->real = 0;
	}
	তথ্য->read_blk =	test_io_cb_read_blk;
	তথ্য->write_blk =	test_io_cb_write_blk;
	তথ্য->set_blksize =	test_io_cb_set_blksize;
	তথ্য->write_byte =	test_io_cb_write_byte;
	তথ্য->read_blk64 =	test_io_cb_read_blk64;
	তথ্য->write_blk64 =	test_io_cb_write_blk64;

	তথ্য->outfile = ফাঁকা;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != ফাঁকা)
		তথ্য->outfile = fopen(মান, "w");
	যদি (!তথ্য->outfile)
		তথ্য->outfile = stderr;

	তথ্য->flags = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_FLAGS")) != ফাঁকা)
		তথ্য->flags = strtoul(মান, ফাঁকা, 0);

	তথ্য->block = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_BLOCK")) != ফাঁকা)
		তথ্য->block = strtoul(মান, ফাঁকা, 0);

	তথ্য->read_abort_গুনতি = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != ফাঁকা)
		তথ্য->read_abort_গুনতি = strtoul(মান, ফাঁকা, 0);

	তথ্য->write_abort_গুনতি = 0;
	যদি ((মান = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != ফাঁকা)
		তথ্য->write_abort_গুনতি = strtoul(মান, ফাঁকা, 0);

	যদি (তথ্য->real) {
		io->align = তথ্য->real->align;
		যদি (তথ্য->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	ফেরত 0;

cleanup:
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (io)
		ext2fs_free_mem(&io);
	যদি (তথ্য)
		ext2fs_free_mem(&তথ্য);
	ফেরত retval;
}

স্থির errcode_t test_close(io_channel channel)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ফেরত 0;

	যদি (তথ্য->real)
		retval = io_channel_close(তথ্য->real);

	যদি (তথ্য->outfile && তথ্য->outfile != stderr)
		fclose(তথ্য->outfile);

	ext2fs_free_mem(&channel->private_তথ্য);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ফেরত retval;
}

স্থির errcode_t test_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real) {
		retval = io_channel_set_blksize(তথ্য->real, blksize);
		channel->align = তথ্য->real->align;
	}
	যদি (তথ্য->set_blksize)
		তথ্য->set_blksize(blksize, retval);
	যদি (তথ্য->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(তথ্য->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_আকার = blksize;
	ফেরত retval;
}


স্থির errcode_t test_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_read_blk(তথ্য->real, block, গুনতি, buf);
	যদি (তথ্য->read_blk)
		তথ্য->read_blk(block, গুনতি, retval);
	যদি (তথ্য->flags & TEST_FLAG_READ)
		fprintf(তথ্য->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (তথ্য->block && তথ্য->block == block) {
		যদি (তথ্য->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, তথ্য, block, buf);
		যদি (--তথ্য->read_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ফেরত retval;
}

স্থির errcode_t test_write_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_write_blk(তথ্য->real, block, গুনতি, buf);
	যদি (তথ্য->write_blk)
		তথ্য->write_blk(block, গুনতি, retval);
	যদি (তথ্য->flags & TEST_FLAG_WRITE)
		fprintf(তথ্য->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (তথ্য->block && তথ্য->block == block) {
		যদি (তথ্য->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, তথ্য, block, buf);
		যদি (--তথ্য->write_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ফেরত retval;
}

স্থির errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_read_blk64(তথ্য->real, block, গুনতি, buf);
	যদি (তথ্য->read_blk64)
		তথ্য->read_blk64(block, গুনতি, retval);
	যদি (তথ্য->flags & TEST_FLAG_READ)
		fprintf(তথ্য->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (তথ্য->block && তথ্য->block == block) {
		যদি (তথ্য->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, তথ্য, block, buf);
		যদি (--তথ্য->read_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ফেরত retval;
}

স্থির errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_write_blk64(তথ্য->real, block, গুনতি, buf);
	যদি (তথ্য->write_blk64)
		তথ্য->write_blk64(block, গুনতি, retval);
	যদি (তথ্য->flags & TEST_FLAG_WRITE)
		fprintf(তথ্য->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	যদি (তথ্য->block && তথ্য->block == block) {
		যদি (তথ্য->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, তথ্য, block, buf);
		যদি (--তথ্য->write_abort_গুনতি == 0)
			test_abort(channel, block);
	}
	ফেরত retval;
}

স্থির errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real && তথ্য->real->manager->write_byte)
		retval = io_channel_write_byte(তথ্য->real, offset, গুনতি, buf);
	যদি (তথ্য->write_byte)
		তথ্য->write_byte(offset, গুনতি, retval);
	যদি (তথ্য->flags & TEST_FLAG_WRITE)
		fprintf(তথ্য->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, গুনতি, retval ? error_message(retval) : "OK");
	ফেরত retval;
}

স্থির errcode_t test_set_option(io_channel channel, ধ্রুবক অক্ষর *option,
				 ধ্রুবক অক্ষর *arg)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	যদি (তথ্য->flags & TEST_FLAG_SET_OPTION)
		fprintf(তথ্য->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	যদি (তথ্য->real && তথ্য->real->manager->set_option) {
		retval = (তথ্য->real->manager->set_option)(তথ্য->real,
							   option, arg);
		যদি (তথ্য->flags & TEST_FLAG_SET_OPTION)
			fprintf(তথ্য->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} অন্যথা {
		যদি (তথ্য->flags & TEST_FLAG_SET_OPTION)
			fprintf(তথ্য->outfile, "not implemented\n");
	}
	ফেরত retval;
}

স্থির errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real && তথ্য->real->manager->get_stats) {
		retval = (তথ্য->real->manager->get_stats)(তথ্য->real, stats);
	}
	ফেরত retval;
}

স্থির errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_discard(তথ্য->real, block, গুনতি);
	যদি (তথ্য->flags & TEST_FLAG_DISCARD)
		fprintf(তথ্য->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ফেরত retval;
}

স্থির errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_cache_readahead(তথ্য->real, block, গুনতি);
	যদি (তথ্য->flags & TEST_FLAG_READAHEAD)
		fprintf(তথ্য->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ফেরত retval;
}

স্থির errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো test_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো test_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_zeroout(তথ্য->real, block, গুনতি);
	যদি (তথ্য->flags & TEST_FLAG_ZEROOUT)
		fprintf(তথ্য->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, গুনতি, retval ? error_message(retval) : "OK");
	ফেরত retval;
}

স্থির কাঠামো struct_io_manager struct_test_manager = {
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
