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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

ഘടന test_private_data {
	പൂർണ്ണസംഖ്യ	magic;
	io_channel real;
	പൂർണ്ണസംഖ്യ flags;
	FILE *outfile;
	unsigned long block;
	പൂർണ്ണസംഖ്യ read_abort_എണ്ണം, write_abort_എണ്ണം;
	ശൂന്യം (*read_blk)(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err);
	ശൂന്യം (*write_blk)(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err);
	ശൂന്യം (*set_blksize)(പൂർണ്ണസംഖ്യ blksize, errcode_t err);
	ശൂന്യം (*write_byte)(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err);
	ശൂന്യം (*read_blk64)(unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err);
	ശൂന്യം (*write_blk64)(unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
ശൂന്യം (*test_io_cb_read_blk)
	(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err) = 0;
ശൂന്യം (*test_io_cb_write_blk)
	(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err) = 0;
ശൂന്യം (*test_io_cb_read_blk64)
	(unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err) = 0;
ശൂന്യം (*test_io_cb_write_blk64)
	(unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err) = 0;
ശൂന്യം (*test_io_cb_set_blksize)
	(പൂർണ്ണസംഖ്യ blksize, errcode_t err) = 0;
ശൂന്യം (*test_io_cb_write_byte)
	(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err) = 0;

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

സ്ഥിതം ശൂന്യം test_dump_block(io_channel channel,
			    ഘടന test_private_data *data,
			    unsigned long block, സ്ഥിരം ശൂന്യം *buf)
{
	സ്ഥിരം unsigned അക്ഷരം *cp;
	FILE *f = data->outfile;
	പൂർണ്ണസംഖ്യ	i;
	unsigned long	cksum = 0;

	വേണ്ടി (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	വേണ്ടി (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		എങ്കിൽ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
സ്ഥിതം errcode_t test_flush(io_channel channel)
{
	ഘടന test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_flush(data->real);

	എങ്കിൽ (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	തിരികെ retval;
}

സ്ഥിതം ശൂന്യം test_abort(io_channel channel, unsigned long block)
{
	ഘടന test_private_data *data;
	FILE *f;

	data = (ഘടന test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

സ്ഥിതം errcode_t test_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	io_channel	io = ശൂന്യ;
	ഘടന test_private_data *data = ശൂന്യ;
	errcode_t	retval;
	അക്ഷരം		*മൂല്യം;

	എങ്കിൽ (name == 0)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_io_channel), &io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(io, 0, വലുപ്പം(ഘടന struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന test_private_data), &data);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, വലുപ്പം(ഘടന test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	എങ്കിൽ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = ശൂന്യ;
	എങ്കിൽ ((മൂല്യം = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != ശൂന്യ)
		data->outfile = fopen(മൂല്യം, "w");
	എങ്കിൽ (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	എങ്കിൽ ((മൂല്യം = ext2fs_safe_getenv("TEST_IO_FLAGS")) != ശൂന്യ)
		data->flags = strtoul(മൂല്യം, ശൂന്യ, 0);

	data->block = 0;
	എങ്കിൽ ((മൂല്യം = ext2fs_safe_getenv("TEST_IO_BLOCK")) != ശൂന്യ)
		data->block = strtoul(മൂല്യം, ശൂന്യ, 0);

	data->read_abort_എണ്ണം = 0;
	എങ്കിൽ ((മൂല്യം = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != ശൂന്യ)
		data->read_abort_എണ്ണം = strtoul(മൂല്യം, ശൂന്യ, 0);

	data->write_abort_എണ്ണം = 0;
	എങ്കിൽ ((മൂല്യം = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != ശൂന്യ)
		data->write_abort_എണ്ണം = strtoul(മൂല്യം, ശൂന്യ, 0);

	എങ്കിൽ (data->real) {
		io->align = data->real->align;
		എങ്കിൽ (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	തിരികെ 0;

cleanup:
	എങ്കിൽ (io && io->name)
		ext2fs_free_mem(&io->name);
	എങ്കിൽ (io)
		ext2fs_free_mem(&io);
	എങ്കിൽ (data)
		ext2fs_free_mem(&data);
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_close(io_channel channel)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
		തിരികെ 0;

	എങ്കിൽ (data->real)
		retval = io_channel_close(data->real);

	എങ്കിൽ (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	എങ്കിൽ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	എങ്കിൽ (data->set_blksize)
		data->set_blksize(blksize, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	തിരികെ retval;
}


സ്ഥിതം errcode_t test_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_read_blk(data->real, block, എണ്ണം, buf);
	എങ്കിൽ (data->read_blk)
		data->read_blk(block, എണ്ണം, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	എങ്കിൽ (data->block && data->block == block) {
		എങ്കിൽ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		എങ്കിൽ (--data->read_abort_എണ്ണം == 0)
			test_abort(channel, block);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_write_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_write_blk(data->real, block, എണ്ണം, buf);
	എങ്കിൽ (data->write_blk)
		data->write_blk(block, എണ്ണം, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	എങ്കിൽ (data->block && data->block == block) {
		എങ്കിൽ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		എങ്കിൽ (--data->write_abort_എണ്ണം == 0)
			test_abort(channel, block);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_read_blk64(data->real, block, എണ്ണം, buf);
	എങ്കിൽ (data->read_blk64)
		data->read_blk64(block, എണ്ണം, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	എങ്കിൽ (data->block && data->block == block) {
		എങ്കിൽ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		എങ്കിൽ (--data->read_abort_എണ്ണം == 0)
			test_abort(channel, block);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_write_blk64(data->real, block, എണ്ണം, buf);
	എങ്കിൽ (data->write_blk64)
		data->write_blk64(block, എണ്ണം, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	എങ്കിൽ (data->block && data->block == block) {
		എങ്കിൽ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		എങ്കിൽ (--data->write_abort_എണ്ണം == 0)
			test_abort(channel, block);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, എണ്ണം, buf);
	എങ്കിൽ (data->write_byte)
		data->write_byte(offset, എണ്ണം, retval);
	എങ്കിൽ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, എണ്ണം, retval ? error_message(retval) : "OK");
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_set_option(io_channel channel, സ്ഥിരം അക്ഷരം *option,
				 സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	എങ്കിൽ (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	എങ്കിൽ (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		എങ്കിൽ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_discard(data->real, block, എണ്ണം);
	എങ്കിൽ (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long എണ്ണം)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_cache_readahead(data->real, block, എണ്ണം);
	എങ്കിൽ (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	തിരികെ retval;
}

സ്ഥിതം errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_zeroout(data->real, block, എണ്ണം);
	എങ്കിൽ (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, എണ്ണം, retval ? error_message(retval) : "OK");
	തിരികെ retval;
}

സ്ഥിതം ഘടന struct_io_manager struct_test_manager = {
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
