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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)

ساخت test_نجی_ڈیٹا {
	صحیح	magic;
	io_channel real;
	صحیح flags;
	FILE *outfile;
	غیرعلامتی طویل بلاک;
	صحیح read_abort_گنتی, write_abort_گنتی;
	خالی (*read_blk)(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err);
	خالی (*write_blk)(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err);
	خالی (*سیٹ_blksize)(صحیح blksize, errcode_t err);
	خالی (*write_byte)(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err);
	خالی (*read_blk64)(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err);
	خالی (*write_blk64)(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
خالی (*test_io_cb_read_blk)
	(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err) = 0;
خالی (*test_io_cb_write_blk)
	(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err) = 0;
خالی (*test_io_cb_read_blk64)
	(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err) = 0;
خالی (*test_io_cb_write_blk64)
	(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err) = 0;
خالی (*test_io_cb_سیٹ_blksize)
	(صحیح blksize, errcode_t err) = 0;
خالی (*test_io_cb_write_byte)
	(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err) = 0;

/*
 * Test flags
 */
#تعریف_کرو TEST_FLAG_READ			0x01
#تعریف_کرو TEST_FLAG_WRITE			0x02
#تعریف_کرو TEST_FLAG_سیٹ_BLKSIZE		0x04
#تعریف_کرو TEST_FLAG_FLUSH			0x08
#تعریف_کرو TEST_FLAG_DUMP			0x10
#تعریف_کرو TEST_FLAG_سیٹ_OPTION		0x20
#تعریف_کرو TEST_FLAG_DISCARD		0x40
#تعریف_کرو TEST_FLAG_READAHEAD		0x80
#تعریف_کرو TEST_FLAG_ZEROOUT		0x100

ساکن خالی test_dump_بلاک(io_channel channel,
			    ساخت test_نجی_ڈیٹا *ڈیٹا,
			    غیرعلامتی طویل بلاک, ثابت خالی *buf)
{
	ثابت غیرعلامتی حرف *cp;
	FILE *f = ڈیٹا->outfile;
	صحیح	i;
	غیرعلامتی طویل	cksum = 0;

	برائے (i=0, cp = buf; i < channel->بلاک_سائز; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", بلاک, cksum);
	برائے (i=0, cp = buf; i < channel->بلاک_سائز; i++, cp++) {
		اگر ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ساکن errcode_t test_flush(io_channel channel)
{
	ساخت test_نجی_ڈیٹا	*ڈیٹا;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *)channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_flush(ڈیٹا->real);

	اگر (ڈیٹا->flags & TEST_FLAG_FLUSH)
		fprintf(ڈیٹا->outfile, "Test_io: flush() returned %s\n",
			retval ? error_پیغام(retval) : "OK");

	واپس retval;
}

ساکن خالی test_abort(io_channel channel, غیرعلامتی طویل بلاک)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	FILE *f;

	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	f = ڈیٹا->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", بلاک);
	fflush(f);
	abort();
}

ساکن errcode_t test_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	io_channel	io = تہی;
	ساخت test_نجی_ڈیٹا *ڈیٹا = تہی;
	errcode_t	retval;
	حرف		*قدر;

	اگر (name == 0)
		واپس EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(ساخت struct_io_channel), &io);
	اگر (retval)
		جاؤ cleanup;
	memset(io, 0, حجم(ساخت struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(ساخت test_نجی_ڈیٹا), &ڈیٹا);
	اگر (retval)
		جاؤ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(io->name, name);
	io->نجی_ڈیٹا = ڈیٹا;
	io->بلاک_سائز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(ڈیٹا, 0, حجم(ساخت test_نجی_ڈیٹا));
	ڈیٹا->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	اگر (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &ڈیٹا->real);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ {
		ڈیٹا->real = 0;
	}
	ڈیٹا->read_blk =	test_io_cb_read_blk;
	ڈیٹا->write_blk =	test_io_cb_write_blk;
	ڈیٹا->سیٹ_blksize =	test_io_cb_سیٹ_blksize;
	ڈیٹا->write_byte =	test_io_cb_write_byte;
	ڈیٹا->read_blk64 =	test_io_cb_read_blk64;
	ڈیٹا->write_blk64 =	test_io_cb_write_blk64;

	ڈیٹا->outfile = تہی;
	اگر ((قدر = ext2fs_محفوظ_getenv("TEST_IO_LOGFILE")) != تہی)
		ڈیٹا->outfile = fopen(قدر, "w");
	اگر (!ڈیٹا->outfile)
		ڈیٹا->outfile = stderr;

	ڈیٹا->flags = 0;
	اگر ((قدر = ext2fs_محفوظ_getenv("TEST_IO_FLAGS")) != تہی)
		ڈیٹا->flags = strtoul(قدر, تہی, 0);

	ڈیٹا->بلاک = 0;
	اگر ((قدر = ext2fs_محفوظ_getenv("TEST_IO_BLOCK")) != تہی)
		ڈیٹا->بلاک = strtoul(قدر, تہی, 0);

	ڈیٹا->read_abort_گنتی = 0;
	اگر ((قدر = ext2fs_محفوظ_getenv("TEST_IO_READ_ABORT")) != تہی)
		ڈیٹا->read_abort_گنتی = strtoul(قدر, تہی, 0);

	ڈیٹا->write_abort_گنتی = 0;
	اگر ((قدر = ext2fs_محفوظ_getenv("TEST_IO_WRITE_ABORT")) != تہی)
		ڈیٹا->write_abort_گنتی = strtoul(قدر, تہی, 0);

	اگر (ڈیٹا->real) {
		io->align = ڈیٹا->real->align;
		اگر (ڈیٹا->real->flags & CHANNEL_FLAGS_دھاگے)
			io->flags |= CHANNEL_FLAGS_دھاگے;
	}

	*channel = io;
	واپس 0;

cleanup:
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (io)
		ext2fs_free_mem(&io);
	اگر (ڈیٹا)
		ext2fs_free_mem(&ڈیٹا);
	واپس retval;
}

ساکن errcode_t test_close(io_channel channel)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		واپس 0;

	اگر (ڈیٹا->real)
		retval = io_channel_close(ڈیٹا->real);

	اگر (ڈیٹا->outfile && ڈیٹا->outfile != stderr)
		fclose(ڈیٹا->outfile);

	ext2fs_free_mem(&channel->نجی_ڈیٹا);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	واپس retval;
}

ساکن errcode_t test_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real) {
		retval = io_channel_سیٹ_blksize(ڈیٹا->real, blksize);
		channel->align = ڈیٹا->real->align;
	}
	اگر (ڈیٹا->سیٹ_blksize)
		ڈیٹا->سیٹ_blksize(blksize, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_سیٹ_BLKSIZE)
		fprintf(ڈیٹا->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_پیغام(retval) : "OK");
	channel->بلاک_سائز = blksize;
	واپس retval;
}


ساکن errcode_t test_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_read_blk(ڈیٹا->real, بلاک, گنتی, buf);
	اگر (ڈیٹا->read_blk)
		ڈیٹا->read_blk(بلاک, گنتی, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_READ)
		fprintf(ڈیٹا->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	اگر (ڈیٹا->بلاک && ڈیٹا->بلاک == بلاک) {
		اگر (ڈیٹا->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, ڈیٹا, بلاک, buf);
		اگر (--ڈیٹا->read_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	واپس retval;
}

ساکن errcode_t test_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, ثابت خالی *buf)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_write_blk(ڈیٹا->real, بلاک, گنتی, buf);
	اگر (ڈیٹا->write_blk)
		ڈیٹا->write_blk(بلاک, گنتی, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_WRITE)
		fprintf(ڈیٹا->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	اگر (ڈیٹا->بلاک && ڈیٹا->بلاک == بلاک) {
		اگر (ڈیٹا->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, ڈیٹا, بلاک, buf);
		اگر (--ڈیٹا->write_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	واپس retval;
}

ساکن errcode_t test_read_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_read_blk64(ڈیٹا->real, بلاک, گنتی, buf);
	اگر (ڈیٹا->read_blk64)
		ڈیٹا->read_blk64(بلاک, گنتی, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_READ)
		fprintf(ڈیٹا->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	اگر (ڈیٹا->بلاک && ڈیٹا->بلاک == بلاک) {
		اگر (ڈیٹا->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, ڈیٹا, بلاک, buf);
		اگر (--ڈیٹا->read_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	واپس retval;
}

ساکن errcode_t test_write_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, ثابت خالی *buf)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_write_blk64(ڈیٹا->real, بلاک, گنتی, buf);
	اگر (ڈیٹا->write_blk64)
		ڈیٹا->write_blk64(بلاک, گنتی, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_WRITE)
		fprintf(ڈیٹا->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	اگر (ڈیٹا->بلاک && ڈیٹا->بلاک == بلاک) {
		اگر (ڈیٹا->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, ڈیٹا, بلاک, buf);
		اگر (--ڈیٹا->write_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	واپس retval;
}

ساکن errcode_t test_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
			       صحیح گنتی, ثابت خالی *buf)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real && ڈیٹا->real->manager->write_byte)
		retval = io_channel_write_byte(ڈیٹا->real, انحراف, گنتی, buf);
	اگر (ڈیٹا->write_byte)
		ڈیٹا->write_byte(انحراف, گنتی, retval);
	اگر (ڈیٹا->flags & TEST_FLAG_WRITE)
		fprintf(ڈیٹا->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			انحراف, گنتی, retval ? error_پیغام(retval) : "OK");
	واپس retval;
}

ساکن errcode_t test_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	اگر (ڈیٹا->flags & TEST_FLAG_سیٹ_OPTION)
		fprintf(ڈیٹا->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	اگر (ڈیٹا->real && ڈیٹا->real->manager->سیٹ_option) {
		retval = (ڈیٹا->real->manager->سیٹ_option)(ڈیٹا->real,
							   option, arg);
		اگر (ڈیٹا->flags & TEST_FLAG_سیٹ_OPTION)
			fprintf(ڈیٹا->outfile, "returned %s\n",
				retval ? error_پیغام(retval) : "OK");
	} ورنہ {
		اگر (ڈیٹا->flags & TEST_FLAG_سیٹ_OPTION)
			fprintf(ڈیٹا->outfile, "not implemented\n");
	}
	واپس retval;
}

ساکن errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real && ڈیٹا->real->manager->get_stats) {
		retval = (ڈیٹا->real->manager->get_stats)(ڈیٹا->real, stats);
	}
	واپس retval;
}

ساکن errcode_t test_discard(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_discard(ڈیٹا->real, بلاک, گنتی);
	اگر (ڈیٹا->flags & TEST_FLAG_DISCARD)
		fprintf(ڈیٹا->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	واپس retval;
}

ساکن errcode_t test_cache_readahead(io_channel channel,
				      غیرعلامتی طویل طویل بلاک,
				      غیرعلامتی طویل طویل گنتی)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_cache_readahead(ڈیٹا->real, بلاک, گنتی);
	اگر (ڈیٹا->flags & TEST_FLAG_READAHEAD)
		fprintf(ڈیٹا->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	واپس retval;
}

ساکن errcode_t test_zeroout(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت test_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت test_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_zeroout(ڈیٹا->real, بلاک, گنتی);
	اگر (ڈیٹا->flags & TEST_FLAG_ZEROOUT)
		fprintf(ڈیٹا->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_پیغام(retval) : "OK");
	واپس retval;
}

ساکن ساخت struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.سیٹ_blksize	= test_سیٹ_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.سیٹ_option	= test_سیٹ_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
