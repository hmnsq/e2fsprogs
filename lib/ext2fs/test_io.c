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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)

بنية test_خصوصی_داده {
	صحيح	magic;
	io_channel real;
	صحيح flags;
	FILE *outfile;
	بیعلامت دراز بلوک;
	صحيح read_abort_شمار, write_abort_شمار;
	فراغ (*read_blk)(بیعلامت دراز بلوک, صحيح شمار, errcode_t err);
	فراغ (*write_blk)(بیعلامت دراز بلوک, صحيح شمار, errcode_t err);
	فراغ (*مجموعه_blksize)(صحيح blksize, errcode_t err);
	فراغ (*write_byte)(بیعلامت دراز بلوک, صحيح شمار, errcode_t err);
	فراغ (*read_blk64)(بیعلامت دراز دراز بلوک, صحيح شمار, errcode_t err);
	فراغ (*write_blk64)(بیعلامت دراز دراز بلوک, صحيح شمار, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
فراغ (*test_io_cb_read_blk)
	(بیعلامت دراز بلوک, صحيح شمار, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk)
	(بیعلامت دراز بلوک, صحيح شمار, errcode_t err) = 0;
فراغ (*test_io_cb_read_blk64)
	(بیعلامت دراز دراز بلوک, صحيح شمار, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk64)
	(بیعلامت دراز دراز بلوک, صحيح شمار, errcode_t err) = 0;
فراغ (*test_io_cb_مجموعه_blksize)
	(صحيح blksize, errcode_t err) = 0;
فراغ (*test_io_cb_write_byte)
	(بیعلامت دراز بلوک, صحيح شمار, errcode_t err) = 0;

/*
 * Test flags
 */
#تعریف_کن TEST_FLAG_READ			0x01
#تعریف_کن TEST_FLAG_WRITE			0x02
#تعریف_کن TEST_FLAG_مجموعه_BLKSIZE		0x04
#تعریف_کن TEST_FLAG_FLUSH			0x08
#تعریف_کن TEST_FLAG_DUMP			0x10
#تعریف_کن TEST_FLAG_مجموعه_OPTION		0x20
#تعریف_کن TEST_FLAG_DISCARD		0x40
#تعریف_کن TEST_FLAG_READAHEAD		0x80
#تعریف_کن TEST_FLAG_ZEROOUT		0x100

ساكن فراغ test_dump_بلوک(io_channel channel,
			    بنية test_خصوصی_داده *داده,
			    بیعلامت دراز بلوک, ثابت فراغ *buf)
{
	ثابت بیعلامت حرف *cp;
	FILE *f = داده->outfile;
	صحيح	i;
	بیعلامت دراز	cksum = 0;

	لكل (i=0, cp = buf; i < channel->بلوک_اندازه; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", بلوک, cksum);
	لكل (i=0, cp = buf; i < channel->بلوک_اندازه; i++, cp++) {
		إذا ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t test_flush(io_channel channel)
{
	بنية test_خصوصی_داده	*داده;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *)channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_flush(داده->real);

	إذا (داده->flags & TEST_FLAG_FLUSH)
		fprintf(داده->outfile, "Test_io: flush() returned %s\n",
			retval ? error_پیام(retval) : "OK");

	أرجع retval;
}

ساكن فراغ test_abort(io_channel channel, بیعلامت دراز بلوک)
{
	بنية test_خصوصی_داده *داده;
	FILE *f;

	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	f = داده->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", بلوک);
	fflush(f);
	abort();
}

ساكن errcode_t test_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية test_خصوصی_داده *داده = عدم;
	errcode_t	retval;
	حرف		*مقدار;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية test_خصوصی_داده), &داده);
	إذا (retval)
		اذهب cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(طول_رشته(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->خصوصی_داده = داده;
	io->بلوک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(داده, 0, حجم(بنية test_خصوصی_داده));
	داده->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	إذا (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &داده->real);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		داده->real = 0;
	}
	داده->read_blk =	test_io_cb_read_blk;
	داده->write_blk =	test_io_cb_write_blk;
	داده->مجموعه_blksize =	test_io_cb_مجموعه_blksize;
	داده->write_byte =	test_io_cb_write_byte;
	داده->read_blk64 =	test_io_cb_read_blk64;
	داده->write_blk64 =	test_io_cb_write_blk64;

	داده->outfile = عدم;
	إذا ((مقدار = ext2fs_امن_getenv("TEST_IO_LOGFILE")) != عدم)
		داده->outfile = fopen(مقدار, "w");
	إذا (!داده->outfile)
		داده->outfile = stderr;

	داده->flags = 0;
	إذا ((مقدار = ext2fs_امن_getenv("TEST_IO_FLAGS")) != عدم)
		داده->flags = strtoul(مقدار, عدم, 0);

	داده->بلوک = 0;
	إذا ((مقدار = ext2fs_امن_getenv("TEST_IO_BLOCK")) != عدم)
		داده->بلوک = strtoul(مقدار, عدم, 0);

	داده->read_abort_شمار = 0;
	إذا ((مقدار = ext2fs_امن_getenv("TEST_IO_READ_ABORT")) != عدم)
		داده->read_abort_شمار = strtoul(مقدار, عدم, 0);

	داده->write_abort_شمار = 0;
	إذا ((مقدار = ext2fs_امن_getenv("TEST_IO_WRITE_ABORT")) != عدم)
		داده->write_abort_شمار = strtoul(مقدار, عدم, 0);

	إذا (داده->real) {
		io->align = داده->real->align;
		إذا (داده->real->flags & CHANNEL_FLAGS_نخها)
			io->flags |= CHANNEL_FLAGS_نخها;
	}

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	إذا (داده)
		ext2fs_free_mem(&داده);
	أرجع retval;
}

ساكن errcode_t test_close(io_channel channel)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	إذا (داده->real)
		retval = io_channel_close(داده->real);

	إذا (داده->outfile && داده->outfile != stderr)
		fclose(داده->outfile);

	ext2fs_free_mem(&channel->خصوصی_داده);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t test_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real) {
		retval = io_channel_مجموعه_blksize(داده->real, blksize);
		channel->align = داده->real->align;
	}
	إذا (داده->مجموعه_blksize)
		داده->مجموعه_blksize(blksize, retval);
	إذا (داده->flags & TEST_FLAG_مجموعه_BLKSIZE)
		fprintf(داده->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_پیام(retval) : "OK");
	channel->بلوک_اندازه = blksize;
	أرجع retval;
}


ساكن errcode_t test_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_read_blk(داده->real, بلوک, شمار, buf);
	إذا (داده->read_blk)
		داده->read_blk(بلوک, شمار, retval);
	إذا (داده->flags & TEST_FLAG_READ)
		fprintf(داده->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	إذا (داده->بلوک && داده->بلوک == بلوک) {
		إذا (داده->flags & TEST_FLAG_DUMP)
			test_dump_بلوک(channel, داده, بلوک, buf);
		إذا (--داده->read_abort_شمار == 0)
			test_abort(channel, بلوک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, ثابت فراغ *buf)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_write_blk(داده->real, بلوک, شمار, buf);
	إذا (داده->write_blk)
		داده->write_blk(بلوک, شمار, retval);
	إذا (داده->flags & TEST_FLAG_WRITE)
		fprintf(داده->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	إذا (داده->بلوک && داده->بلوک == بلوک) {
		إذا (داده->flags & TEST_FLAG_DUMP)
			test_dump_بلوک(channel, داده, بلوک, buf);
		إذا (--داده->write_abort_شمار == 0)
			test_abort(channel, بلوک);
	}
	أرجع retval;
}

ساكن errcode_t test_read_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_read_blk64(داده->real, بلوک, شمار, buf);
	إذا (داده->read_blk64)
		داده->read_blk64(بلوک, شمار, retval);
	إذا (داده->flags & TEST_FLAG_READ)
		fprintf(داده->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	إذا (داده->بلوک && داده->بلوک == بلوک) {
		إذا (داده->flags & TEST_FLAG_DUMP)
			test_dump_بلوک(channel, داده, بلوک, buf);
		إذا (--داده->read_abort_شمار == 0)
			test_abort(channel, بلوک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
			       صحيح شمار, ثابت فراغ *buf)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_write_blk64(داده->real, بلوک, شمار, buf);
	إذا (داده->write_blk64)
		داده->write_blk64(بلوک, شمار, retval);
	إذا (داده->flags & TEST_FLAG_WRITE)
		fprintf(داده->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	إذا (داده->بلوک && داده->بلوک == بلوک) {
		إذا (داده->flags & TEST_FLAG_DUMP)
			test_dump_بلوک(channel, داده, بلوک, buf);
		إذا (--داده->write_abort_شمار == 0)
			test_abort(channel, بلوک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
			       صحيح شمار, ثابت فراغ *buf)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real && داده->real->manager->write_byte)
		retval = io_channel_write_byte(داده->real, جابه_جایی, شمار, buf);
	إذا (داده->write_byte)
		داده->write_byte(جابه_جایی, شمار, retval);
	إذا (داده->flags & TEST_FLAG_WRITE)
		fprintf(داده->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			جابه_جایی, شمار, retval ? error_پیام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_مجموعه_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	إذا (داده->flags & TEST_FLAG_مجموعه_OPTION)
		fprintf(داده->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	إذا (داده->real && داده->real->manager->مجموعه_option) {
		retval = (داده->real->manager->مجموعه_option)(داده->real,
							   option, arg);
		إذا (داده->flags & TEST_FLAG_مجموعه_OPTION)
			fprintf(داده->outfile, "returned %s\n",
				retval ? error_پیام(retval) : "OK");
	} وإلا {
		إذا (داده->flags & TEST_FLAG_مجموعه_OPTION)
			fprintf(داده->outfile, "not implemented\n");
	}
	أرجع retval;
}

ساكن errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real && داده->real->manager->get_stats) {
		retval = (داده->real->manager->get_stats)(داده->real, stats);
	}
	أرجع retval;
}

ساكن errcode_t test_discard(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_discard(داده->real, بلوک, شمار);
	إذا (داده->flags & TEST_FLAG_DISCARD)
		fprintf(داده->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_cache_readahead(io_channel channel,
				      بیعلامت دراز دراز بلوک,
				      بیعلامت دراز دراز شمار)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_cache_readahead(داده->real, بلوک, شمار);
	إذا (داده->flags & TEST_FLAG_READAHEAD)
		fprintf(داده->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_zeroout(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية test_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية test_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_zeroout(داده->real, بلوک, شمار);
	إذا (داده->flags & TEST_FLAG_ZEROOUT)
		fprintf(داده->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			بلوک, شمار, retval ? error_پیام(retval) : "OK");
	أرجع retval;
}

ساكن بنية struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.مجموعه_blksize	= test_مجموعه_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.مجموعه_option	= test_مجموعه_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
