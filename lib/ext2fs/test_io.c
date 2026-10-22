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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)

સંરચના test_private_data {
	પૂર્ણાંક	magic;
	io_channel real;
	પૂર્ણાંક flags;
	FILE *outfile;
	unsigned long block;
	પૂર્ણાંક read_abort_ગણતરી, write_abort_ગણતરી;
	શૂન્ય (*read_blk)(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err);
	શૂન્ય (*write_blk)(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err);
	શૂન્ય (*set_blksize)(પૂર્ણાંક blksize, errcode_t err);
	શૂન્ય (*write_byte)(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err);
	શૂન્ય (*read_blk64)(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err);
	શૂન્ય (*write_blk64)(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
શૂન્ય (*test_io_cb_read_blk)
	(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err) = 0;
શૂન્ય (*test_io_cb_write_blk)
	(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err) = 0;
શૂન્ય (*test_io_cb_read_blk64)
	(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err) = 0;
શૂન્ય (*test_io_cb_write_blk64)
	(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err) = 0;
શૂન્ય (*test_io_cb_set_blksize)
	(પૂર્ણાંક blksize, errcode_t err) = 0;
શૂન્ય (*test_io_cb_write_byte)
	(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err) = 0;

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

સ્થિર શૂન્ય test_dump_block(io_channel channel,
			    સંરચના test_private_data *data,
			    unsigned long block, અચલ શૂન્ય *buf)
{
	અચલ unsigned અક્ષર *cp;
	FILE *f = data->outfile;
	પૂર્ણાંક	i;
	unsigned long	cksum = 0;

	માટે (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	માટે (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		જો ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
સ્થિર errcode_t test_flush(io_channel channel)
{
	સંરચના test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_flush(data->real);

	જો (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	પાછા retval;
}

સ્થિર શૂન્ય test_abort(io_channel channel, unsigned long block)
{
	સંરચના test_private_data *data;
	FILE *f;

	data = (સંરચના test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

સ્થિર errcode_t test_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	io_channel	io = ખાલી;
	સંરચના test_private_data *data = ખાલી;
	errcode_t	retval;
	અક્ષર		*કિંમત;

	જો (name == 0)
		પાછા EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(કદ(સંરચના struct_io_channel), &io);
	જો (retval)
		જાઓ cleanup;
	memset(io, 0, કદ(સંરચના struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(કદ(સંરચના test_private_data), &data);
	જો (retval)
		જાઓ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	જો (retval)
		જાઓ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, કદ(સંરચના test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	જો (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		જો (retval)
			જાઓ cleanup;
	} નહીંતર {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = ખાલી;
	જો ((કિંમત = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != ખાલી)
		data->outfile = fopen(કિંમત, "w");
	જો (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	જો ((કિંમત = ext2fs_safe_getenv("TEST_IO_FLAGS")) != ખાલી)
		data->flags = strtoul(કિંમત, ખાલી, 0);

	data->block = 0;
	જો ((કિંમત = ext2fs_safe_getenv("TEST_IO_BLOCK")) != ખાલી)
		data->block = strtoul(કિંમત, ખાલી, 0);

	data->read_abort_ગણતરી = 0;
	જો ((કિંમત = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != ખાલી)
		data->read_abort_ગણતરી = strtoul(કિંમત, ખાલી, 0);

	data->write_abort_ગણતરી = 0;
	જો ((કિંમત = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != ખાલી)
		data->write_abort_ગણતરી = strtoul(કિંમત, ખાલી, 0);

	જો (data->real) {
		io->align = data->real->align;
		જો (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	પાછા 0;

cleanup:
	જો (io && io->name)
		ext2fs_free_mem(&io->name);
	જો (io)
		ext2fs_free_mem(&io);
	જો (data)
		ext2fs_free_mem(&data);
	પાછા retval;
}

સ્થિર errcode_t test_close(io_channel channel)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (--channel->refcount > 0)
		પાછા 0;

	જો (data->real)
		retval = io_channel_close(data->real);

	જો (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	જો (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	પાછા retval;
}

સ્થિર errcode_t test_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	જો (data->set_blksize)
		data->set_blksize(blksize, retval);
	જો (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	પાછા retval;
}


સ્થિર errcode_t test_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_read_blk(data->real, block, ગણતરી, buf);
	જો (data->read_blk)
		data->read_blk(block, ગણતરી, retval);
	જો (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	જો (data->block && data->block == block) {
		જો (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		જો (--data->read_abort_ગણતરી == 0)
			test_abort(channel, block);
	}
	પાછા retval;
}

સ્થિર errcode_t test_write_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_write_blk(data->real, block, ગણતરી, buf);
	જો (data->write_blk)
		data->write_blk(block, ગણતરી, retval);
	જો (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	જો (data->block && data->block == block) {
		જો (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		જો (--data->write_abort_ગણતરી == 0)
			test_abort(channel, block);
	}
	પાછા retval;
}

સ્થિર errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_read_blk64(data->real, block, ગણતરી, buf);
	જો (data->read_blk64)
		data->read_blk64(block, ગણતરી, retval);
	જો (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	જો (data->block && data->block == block) {
		જો (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		જો (--data->read_abort_ગણતરી == 0)
			test_abort(channel, block);
	}
	પાછા retval;
}

સ્થિર errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_write_blk64(data->real, block, ગણતરી, buf);
	જો (data->write_blk64)
		data->write_blk64(block, ગણતરી, retval);
	જો (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	જો (data->block && data->block == block) {
		જો (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		જો (--data->write_abort_ગણતરી == 0)
			test_abort(channel, block);
	}
	પાછા retval;
}

સ્થિર errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, ગણતરી, buf);
	જો (data->write_byte)
		data->write_byte(offset, ગણતરી, retval);
	જો (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, ગણતરી, retval ? error_message(retval) : "OK");
	પાછા retval;
}

સ્થિર errcode_t test_set_option(io_channel channel, અચલ અક્ષર *option,
				 અચલ અક્ષર *arg)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	જો (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	જો (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		જો (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} નહીંતર {
		જો (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	પાછા retval;
}

સ્થિર errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	પાછા retval;
}

સ્થિર errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_discard(data->real, block, ગણતરી);
	જો (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	પાછા retval;
}

સ્થિર errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ગણતરી)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_cache_readahead(data->real, block, ગણતરી);
	જો (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	પાછા retval;
}

સ્થિર errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_zeroout(data->real, block, ગણતરી);
	જો (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, ગણતરી, retval ? error_message(retval) : "OK");
	પાછા retval;
}

સ્થિર સંરચના struct_io_manager struct_test_manager = {
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
