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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)

ಸಂರಚನಾ test_private_data {
	ಪೂರ್ಣಾಙ್ಕ	magic;
	io_channel real;
	ಪೂರ್ಣಾಙ್ಕ flags;
	FILE *outfile;
	unsigned long block;
	ಪೂರ್ಣಾಙ್ಕ read_abort_ಎಣಿಕೆ, write_abort_ಎಣಿಕೆ;
	ಶೂನ್ಯ (*read_blk)(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err);
	ಶೂನ್ಯ (*write_blk)(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err);
	ಶೂನ್ಯ (*set_blksize)(ಪೂರ್ಣಾಙ್ಕ blksize, errcode_t err);
	ಶೂನ್ಯ (*write_byte)(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err);
	ಶೂನ್ಯ (*read_blk64)(unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err);
	ಶೂನ್ಯ (*write_blk64)(unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
ಶೂನ್ಯ (*test_io_cb_read_blk)
	(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_blk)
	(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_read_blk64)
	(unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_blk64)
	(unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_set_blksize)
	(ಪೂರ್ಣಾಙ್ಕ blksize, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_byte)
	(unsigned long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, errcode_t err) = 0;

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

ಸ್ಥಿರ ಶೂನ್ಯ test_dump_block(io_channel channel,
			    ಸಂರಚನಾ test_private_data *data,
			    unsigned long block, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಧ್ರುವ unsigned ವರ್ಣ *cp;
	FILE *f = data->outfile;
	ಪೂರ್ಣಾಙ್ಕ	i;
	unsigned long	cksum = 0;

	ಪ್ರತಿ (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", block, cksum);
	ಪ್ರತಿ (i=0, cp = buf; i < channel->block_size; i++, cp++) {
		ಯದಿ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t test_flush(io_channel channel)
{
	ಸಂರಚನಾ test_private_data	*data;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *)channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_flush(data->real);

	ಯದಿ (data->flags & TEST_FLAG_FLUSH)
		fprintf(data->outfile, "Test_io: flush() returned %s\n",
			retval ? error_message(retval) : "OK");

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ test_abort(io_channel channel, unsigned long block)
{
	ಸಂರಚನಾ test_private_data *data;
	FILE *f;

	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	f = data->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", block);
	fflush(f);
	abort();
}

ಸ್ಥಿರ errcode_t test_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	io_channel	io = ರಿಕ್ತ;
	ಸಂರಚನಾ test_private_data *data = ರಿಕ್ತ;
	errcode_t	retval;
	ವರ್ಣ		*ಮೌಲ್ಯ;

	ಯದಿ (name == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel), &io);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(io, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ test_private_data), &data);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(data, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ test_private_data));
	data->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ಯದಿ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &data->real);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		data->real = 0;
	}
	data->read_blk =	test_io_cb_read_blk;
	data->write_blk =	test_io_cb_write_blk;
	data->set_blksize =	test_io_cb_set_blksize;
	data->write_byte =	test_io_cb_write_byte;
	data->read_blk64 =	test_io_cb_read_blk64;
	data->write_blk64 =	test_io_cb_write_blk64;

	data->outfile = ರಿಕ್ತ;
	ಯದಿ ((ಮೌಲ್ಯ = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != ರಿಕ್ತ)
		data->outfile = fopen(ಮೌಲ್ಯ, "w");
	ಯದಿ (!data->outfile)
		data->outfile = stderr;

	data->flags = 0;
	ಯದಿ ((ಮೌಲ್ಯ = ext2fs_safe_getenv("TEST_IO_FLAGS")) != ರಿಕ್ತ)
		data->flags = strtoul(ಮೌಲ್ಯ, ರಿಕ್ತ, 0);

	data->block = 0;
	ಯದಿ ((ಮೌಲ್ಯ = ext2fs_safe_getenv("TEST_IO_BLOCK")) != ರಿಕ್ತ)
		data->block = strtoul(ಮೌಲ್ಯ, ರಿಕ್ತ, 0);

	data->read_abort_ಎಣಿಕೆ = 0;
	ಯದಿ ((ಮೌಲ್ಯ = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != ರಿಕ್ತ)
		data->read_abort_ಎಣಿಕೆ = strtoul(ಮೌಲ್ಯ, ರಿಕ್ತ, 0);

	data->write_abort_ಎಣಿಕೆ = 0;
	ಯದಿ ((ಮೌಲ್ಯ = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != ರಿಕ್ತ)
		data->write_abort_ಎಣಿಕೆ = strtoul(ಮೌಲ್ಯ, ರಿಕ್ತ, 0);

	ಯದಿ (data->real) {
		io->align = data->real->align;
		ಯದಿ (data->real->flags & CHANNEL_FLAGS_THREADS)
			io->flags |= CHANNEL_FLAGS_THREADS;
	}

	*channel = io;
	ಪ್ರತ್ಯಾಗಮ 0;

cleanup:
	ಯದಿ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಯದಿ (io)
		ext2fs_free_mem(&io);
	ಯದಿ (data)
		ext2fs_free_mem(&data);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_close(io_channel channel)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (--channel->refcount > 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (data->real)
		retval = io_channel_close(data->real);

	ಯದಿ (data->outfile && data->outfile != stderr)
		fclose(data->outfile);

	ext2fs_free_mem(&channel->private_data);
	ಯದಿ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real) {
		retval = io_channel_set_blksize(data->real, blksize);
		channel->align = data->real->align;
	}
	ಯದಿ (data->set_blksize)
		data->set_blksize(blksize, retval);
	ಯದಿ (data->flags & TEST_FLAG_SET_BLKSIZE)
		fprintf(data->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_message(retval) : "OK");
	channel->block_size = blksize;
	ಪ್ರತ್ಯಾಗಮ retval;
}


ಸ್ಥಿರ errcode_t test_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_read_blk(data->real, block, ಎಣಿಕೆ, buf);
	ಯದಿ (data->read_blk)
		data->read_blk(block, ಎಣಿಕೆ, retval);
	ಯದಿ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಯದಿ (data->block && data->block == block) {
		ಯದಿ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ಯದಿ (--data->read_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, block);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_write_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_write_blk(data->real, block, ಎಣಿಕೆ, buf);
	ಯದಿ (data->write_blk)
		data->write_blk(block, ಎಣಿಕೆ, retval);
	ಯದಿ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಯದಿ (data->block && data->block == block) {
		ಯದಿ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ಯದಿ (--data->write_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, block);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_read_blk64(io_channel channel, unsigned long long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_read_blk64(data->real, block, ಎಣಿಕೆ, buf);
	ಯದಿ (data->read_blk64)
		data->read_blk64(block, ಎಣಿಕೆ, retval);
	ಯದಿ (data->flags & TEST_FLAG_READ)
		fprintf(data->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಯದಿ (data->block && data->block == block) {
		ಯದಿ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ಯದಿ (--data->read_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, block);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_write_blk64(io_channel channel, unsigned long long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_write_blk64(data->real, block, ಎಣಿಕೆ, buf);
	ಯದಿ (data->write_blk64)
		data->write_blk64(block, ಎಣಿಕೆ, retval);
	ಯದಿ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಯದಿ (data->block && data->block == block) {
		ಯದಿ (data->flags & TEST_FLAG_DUMP)
			test_dump_block(channel, data, block, buf);
		ಯದಿ (--data->write_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, block);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_write_byte(io_channel channel, unsigned long offset,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, ಎಣಿಕೆ, buf);
	ಯದಿ (data->write_byte)
		data->write_byte(offset, ಎಣಿಕೆ, retval);
	ಯದಿ (data->flags & TEST_FLAG_WRITE)
		fprintf(data->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			offset, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_set_option(io_channel channel, ಧ್ರುವ ವರ್ಣ *option,
				 ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ಯದಿ (data->flags & TEST_FLAG_SET_OPTION)
		fprintf(data->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ಯದಿ (data->real && data->real->manager->set_option) {
		retval = (data->real->manager->set_option)(data->real,
							   option, arg);
		ಯದಿ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "returned %s\n",
				retval ? error_message(retval) : "OK");
	} ಅನ್ಯಥಾ {
		ಯದಿ (data->flags & TEST_FLAG_SET_OPTION)
			fprintf(data->outfile, "not implemented\n");
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real && data->real->manager->get_stats) {
		retval = (data->real->manager->get_stats)(data->real, stats);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_discard(io_channel channel, unsigned long long block,
			      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_discard(data->real, block, ಎಣಿಕೆ);
	ಯದಿ (data->flags & TEST_FLAG_DISCARD)
		fprintf(data->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_cache_readahead(data->real, block, ಎಣಿಕೆ);
	ಯದಿ (data->flags & TEST_FLAG_READAHEAD)
		fprintf(data->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t test_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ test_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ test_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_zeroout(data->real, block, ಎಣಿಕೆ);
	ಯದಿ (data->flags & TEST_FLAG_ZEROOUT)
		fprintf(data->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			block, ಎಣಿಕೆ, retval ? error_message(retval) : "OK");
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_test_manager = {
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
