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
#ᬬᬾᬦ᭄ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᬬᬾᬦ᭄ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᬬᬾᬦ᭄ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  ᬬᬾᬦ᭄ ((sa_nracanaa)->magic != (code)) ᬯᬮᬶ (code)

sa_nracanaa test_private_data {
	puur_naa_nka	magic;
	io_channel real;
	puur_naa_nka flags;
	FILE *outfile;
	unsigned long block;
	puur_naa_nka read_abort_count, write_abort_count;
	void (*read_blk)(unsigned long block, puur_naa_nka count, errcode_t err);
	void (*write_blk)(unsigned long block, puur_naa_nka count, errcode_t err);
	void (*set_blksize)(puur_naa_nka blksize, errcode_t err);
	void (*write_byte)(unsigned long block, puur_naa_nka count, errcode_t err);
	void (*read_blk64)(unsigned long long block, puur_naa_nka count, errcode_t err);
	void (*write_blk64)(unsigned long long block, puur_naa_nka count, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
void (*test_io_cb_read_blk)
	(unsigned long block, puur_naa_nka count, errcode_t err) = 0;
void (*test_io_cb_write_blk)
	(unsigned long block, puur_naa_nka count, errcode_t err) = 0;
void (*test_io_cb_read_blk64)
	(unsigned long long block, puur_naa_nka count, errcode_t err) = 0;
void (*test_io_cb_write_blk64)
	(unsigned long long block, puur_naa_nka count, errcode_t err) = 0;
void (*test_io_cb_set_blksize)
	(puur_naa_nka blksize, errcode_t err) = 0;
void (*test_io_cb_write_byte)
	(unsigned long block, puur_naa_nka count, errcode_t err) = 0;

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
			    sa_nracanaa test_private_data *data,
			    unsigned long block, const void *buf)
{
	const unsigned var_na *cp;
	FILE *f = data->outfile;
	puur_naa_nka	i;
	unsigned long	cksum = 0;

	for (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	for (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		ᬬᬾᬦ᭄ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
static errcode_t test_flush(io_channel channel)
{
	sa_nracanaa test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_flush(data->real);

	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	ᬯᬮᬶ retval;
}

static void test_abort(io_channel channel, unsigned long block)
{
	sa_nracanaa test_private_data *data;
	FILE *f;

	data = (sa_nracanaa test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

static errcode_t test_open(const var_na *name, puur_naa_nka flags, io_channel *channel)
{
	io_channel	io = NULL;
	sa_nracanaa test_private_data *data = NULL;
	errcode_t	retval;
	var_na		*value;

	ᬬᬾᬦ᭄ (name == 0)
		ᬯᬮᬶ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	ᬬᬾᬦ᭄ (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa test_private_data), &data);
	ᬬᬾᬦ᭄ (retval)
		goto cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ᬬᬾᬦ᭄ (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, parimaa_na(sa_nracanaa test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ᬬᬾᬦ᭄ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		ᬬᬾᬦ᭄ (retval)
			goto cleanup;
	} ᬮᬶᬬ {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = NULL;
	ᬬᬾᬦ᭄ ((value = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != NULL)
		data->outfile = fopen(value, "w");
	ᬬᬾᬦ᭄ (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	ᬬᬾᬦ᭄ ((value = ext2fs_safe_getenv("TEST_IO_FLAGS")) != NULL)
		data->flags = strtoul(value, NULL, 0);

	data->block = 0;
	ᬬᬾᬦ᭄ ((value = ext2fs_safe_getenv("TEST_IO_BLOCK")) != NULL)
		data->block = strtoul(value, NULL, 0);

	data->read_abort_count = 0;
	ᬬᬾᬦ᭄ ((value = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != NULL)
		data->read_abort_count = strtoul(value, NULL, 0);

	data->write_abort_count = 0;
	ᬬᬾᬦ᭄ ((value = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != NULL)
		data->write_abort_count = strtoul(value, NULL, 0);

	ᬬᬾᬦ᭄ (data->real) {
		io->align = data->real->align;
		ᬬᬾᬦ᭄ (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	ᬯᬮᬶ 0;

cleanup:
	ᬬᬾᬦ᭄ (io && io->name)
		ext2fs_free_mem(&io->name);
	ᬬᬾᬦ᭄ (io)
		ext2fs_free_mem(&io);
	ᬬᬾᬦ᭄ (data)
		ext2fs_free_mem(&data);
	ᬯᬮᬶ retval;
}

static errcode_t test_close(io_channel channel)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (--channel->refcount > 0)
		ᬯᬮᬶ 0;

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_close(data->real);

	ᬬᬾᬦ᭄ (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	ᬬᬾᬦ᭄ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ᬯᬮᬶ retval;
}

static errcode_t test_set_blksize(io_channel channel, puur_naa_nka blksize)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	ᬬᬾᬦ᭄ (data->set_blksize)
		data->set_blksize(blksize, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	ᬯᬮᬶ retval;
}


static errcode_t test_read_blk(io_channel channel, unsigned long block,
			       puur_naa_nka count, void *buf)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_read_blk(data->real, block, count, buf);
	ᬬᬾᬦ᭄ (data->read_blk)
		data->read_blk(block, count, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬬᬾᬦ᭄ (data->block && data->block == block) {
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ᬬᬾᬦ᭄ (--data->read_abort_count == 0)
			test_abort(channel, block);
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_write_blk(io_channel channel, unsigned long block,
			       puur_naa_nka count, const void *buf)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_write_blk(data->real, block, count, buf);
	ᬬᬾᬦ᭄ (data->write_blk)
		data->write_blk(block, count, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬬᬾᬦ᭄ (data->block && data->block == block) {
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ᬬᬾᬦ᭄ (--data->write_abort_count == 0)
			test_abort(channel, block);
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       puur_naa_nka count, void *buf)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_read_blk64(data->real, block, count, buf);
	ᬬᬾᬦ᭄ (data->read_blk64)
		data->read_blk64(block, count, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬬᬾᬦ᭄ (data->block && data->block == block) {
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ᬬᬾᬦ᭄ (--data->read_abort_count == 0)
			test_abort(channel, block);
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       puur_naa_nka count, const void *buf)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_write_blk64(data->real, block, count, buf);
	ᬬᬾᬦ᭄ (data->write_blk64)
		data->write_blk64(block, count, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬬᬾᬦ᭄ (data->block && data->block == block) {
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ᬬᬾᬦ᭄ (--data->write_abort_count == 0)
			test_abort(channel, block);
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       puur_naa_nka count, const void *buf)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, count, buf);
	ᬬᬾᬦ᭄ (data->write_byte)
		data->write_byte(offset, count, retval);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, count, retval ? error_message(retval) : "OK");
	ᬯᬮᬶ retval;
}

static errcode_t test_set_option(io_channel channel, const var_na *option,
				 const var_na *arg)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ᬬᬾᬦ᭄ (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} ᬮᬶᬬ {
		ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	ᬯᬮᬶ retval;
}

static errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_discard(data->real, block, count);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬯᬮᬶ retval;
}

static errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_cache_readahead(data->real, block, count);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬯᬮᬶ retval;
}

static errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ᬬᬾᬦ᭄ (data->real)
		retval = io_channel_zeroout(data->real, block, count);
	ᬬᬾᬦ᭄ (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, count, retval ? error_message(retval) : "OK");
	ᬯᬮᬶ retval;
}

static sa_nracanaa struct_io_manager struct_test_manager = {
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
