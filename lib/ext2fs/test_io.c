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

#include "config.h"
#include <stdio.h>
#include <string.h>
#если HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#если HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#если HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  если ((struct)->magic != (code)) вернуть (code)

struct test_private_data {
	целое	magic;
	io_channel real;
	целое flags;
	FILE *outfile;
	unsigned long block;
	целое read_abort_count, write_abort_count;
	void (*read_blk)(unsigned long block, целое count, errcode_t err);
	void (*write_blk)(unsigned long block, целое count, errcode_t err);
	void (*set_blksize)(целое blksize, errcode_t err);
	void (*write_byte)(unsigned long block, целое count, errcode_t err);
	void (*read_blk64)(unsigned long long block, целое count, errcode_t err);
	void (*write_blk64)(unsigned long long block, целое count, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
void (*test_io_cb_read_blk)
	(unsigned long block, целое count, errcode_t err) = 0;
void (*test_io_cb_write_blk)
	(unsigned long block, целое count, errcode_t err) = 0;
void (*test_io_cb_read_blk64)
	(unsigned long long block, целое count, errcode_t err) = 0;
void (*test_io_cb_write_blk64)
	(unsigned long long block, целое count, errcode_t err) = 0;
void (*test_io_cb_set_blksize)
	(целое blksize, errcode_t err) = 0;
void (*test_io_cb_write_byte)
	(unsigned long block, целое count, errcode_t err) = 0;

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

static void test_dump_block(io_channel channel,
			    struct test_private_data *data,
			    unsigned long block, const void *buf)
{
	const unsigned char *cp;
	FILE *f = data->outfile;
	целое	i;
	unsigned long	cksum = 0;

	для (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	для (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		если ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
static errcode_t test_flush(io_channel channel)
{
	struct test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_flush(data->real);

	если (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	вернуть retval;
}

static void test_abort(io_channel channel, unsigned long block)
{
	struct test_private_data *data;
	FILE *f;

	data = (struct test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

static errcode_t test_open(const char *name, целое flags, io_channel *channel)
{
	io_channel	io = NULL;
	struct test_private_data *data = NULL;
	errcode_t	retval;
	char		*value;

	если (name == 0)
		вернуть EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	если (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(sizeof(struct test_private_data), &data);
	если (retval)
		goto cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	если (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, sizeof(struct test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	если (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		если (retval)
			goto cleanup;
	} иначе {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = NULL;
	если ((value = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != NULL)
		data->outfile = fopen(value, "w");
	если (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	если ((value = ext2fs_safe_getenv("TEST_IO_FLAGS")) != NULL)
		data->flags = strtoul(value, NULL, 0);

	data->block = 0;
	если ((value = ext2fs_safe_getenv("TEST_IO_BLOCK")) != NULL)
		data->block = strtoul(value, NULL, 0);

	data->read_abort_count = 0;
	если ((value = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != NULL)
		data->read_abort_count = strtoul(value, NULL, 0);

	data->write_abort_count = 0;
	если ((value = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != NULL)
		data->write_abort_count = strtoul(value, NULL, 0);

	если (data->real) {
		io->align = data->real->align;
		если (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	вернуть 0;

cleanup:
	если (io && io->name)
		ext2fs_free_mem(&io->name);
	если (io)
		ext2fs_free_mem(&io);
	если (data)
		ext2fs_free_mem(&data);
	вернуть retval;
}

static errcode_t test_close(io_channel channel)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (--channel->refcount > 0)
		вернуть 0;

	если (data->real)
		retval = io_channel_close(data->real);

	если (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	если (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	вернуть retval;
}

static errcode_t test_set_blksize(io_channel channel, целое blksize)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	если (data->set_blksize)
		data->set_blksize(blksize, retval);
	если (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	вернуть retval;
}


static errcode_t test_read_blk(io_channel channel, unsigned long block,
			       целое count, void *buf)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_read_blk(data->real, block, count, buf);
	если (data->read_blk)
		data->read_blk(block, count, retval);
	если (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	если (data->block && data->block == block) {
		если (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		если (--data->read_abort_count == 0)
			test_abort(channel, block);
	}
	вернуть retval;
}

static errcode_t test_write_blk(io_channel channel, unsigned long block,
			       целое count, const void *buf)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_write_blk(data->real, block, count, buf);
	если (data->write_blk)
		data->write_blk(block, count, retval);
	если (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	если (data->block && data->block == block) {
		если (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		если (--data->write_abort_count == 0)
			test_abort(channel, block);
	}
	вернуть retval;
}

static errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       целое count, void *buf)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_read_blk64(data->real, block, count, buf);
	если (data->read_blk64)
		data->read_blk64(block, count, retval);
	если (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	если (data->block && data->block == block) {
		если (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		если (--data->read_abort_count == 0)
			test_abort(channel, block);
	}
	вернуть retval;
}

static errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       целое count, const void *buf)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_write_blk64(data->real, block, count, buf);
	если (data->write_blk64)
		data->write_blk64(block, count, retval);
	если (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	если (data->block && data->block == block) {
		если (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		если (--data->write_abort_count == 0)
			test_abort(channel, block);
	}
	вернуть retval;
}

static errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       целое count, const void *buf)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, count, buf);
	если (data->write_byte)
		data->write_byte(offset, count, retval);
	если (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, count, retval ? error_message(retval) : "OK");
	вернуть retval;
}

static errcode_t test_set_option(io_channel channel, const char *option,
				 const char *arg)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	если (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	если (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		если (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} иначе {
		если (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	вернуть retval;
}

static errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	вернуть retval;
}

static errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_discard(data->real, block, count);
	если (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	вернуть retval;
}

static errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_cache_readahead(data->real, block, count);
	если (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	вернуть retval;
}

static errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	struct test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	если (data->real)
		retval = io_channel_zeroout(data->real, block, count);
	если (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	вернуть retval;
}

static struct struct_io_manager struct_test_manager = {
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
