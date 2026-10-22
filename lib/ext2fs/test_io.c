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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

بنية test_نجی_انکڑے {
	صحيح	magic;
	io_channel real;
	صحيح flags;
	FILE *outfile;
	بنانشانی لما بلاک;
	صحيح read_abort_گنتی, write_abort_گنتی;
	فراغ (*read_blk)(بنانشانی لما بلاک, صحيح گنتی, errcode_t err);
	فراغ (*write_blk)(بنانشانی لما بلاک, صحيح گنتی, errcode_t err);
	فراغ (*سیٹ_blksize)(صحيح blksize, errcode_t err);
	فراغ (*write_byte)(بنانشانی لما بلاک, صحيح گنتی, errcode_t err);
	فراغ (*read_blk64)(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err);
	فراغ (*write_blk64)(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
فراغ (*test_io_cb_read_blk)
	(بنانشانی لما بلاک, صحيح گنتی, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk)
	(بنانشانی لما بلاک, صحيح گنتی, errcode_t err) = 0;
فراغ (*test_io_cb_read_blk64)
	(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk64)
	(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err) = 0;
فراغ (*test_io_cb_سیٹ_blksize)
	(صحيح blksize, errcode_t err) = 0;
فراغ (*test_io_cb_write_byte)
	(بنانشانی لما بلاک, صحيح گنتی, errcode_t err) = 0;

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

ساكن فراغ test_dump_بلاک(io_channel channel,
			    بنية test_نجی_انکڑے *انکڑے,
			    بنانشانی لما بلاک, ثابت فراغ *buf)
{
	ثابت بنانشانی حرف *cp;
	FILE *f = انکڑے->outfile;
	صحيح	i;
	بنانشانی لما	cksum = 0;

	لكل (i=0, cp = buf; i < channel->بلاک_ناپ; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", بلاک, cksum);
	لكل (i=0, cp = buf; i < channel->بلاک_ناپ; i++, cp++) {
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
	بنية test_نجی_انکڑے	*انکڑے;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *)channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_flush(انکڑے->real);

	إذا (انکڑے->flags & TEST_FLAG_FLUSH)
		fprintf(انکڑے->outfile, "Test_io: flush() returned %s\n",
			retval ? error_سنیہا(retval) : "OK");

	أرجع retval;
}

ساكن فراغ test_abort(io_channel channel, بنانشانی لما بلاک)
{
	بنية test_نجی_انکڑے *انکڑے;
	FILE *f;

	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	f = انکڑے->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", بلاک);
	fflush(f);
	abort();
}

ساكن errcode_t test_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية test_نجی_انکڑے *انکڑے = عدم;
	errcode_t	retval;
	حرف		*قدر;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية test_نجی_انکڑے), &انکڑے);
	إذا (retval)
		اذهب cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->نجی_انکڑے = انکڑے;
	io->بلاک_ناپ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(انکڑے, 0, حجم(بنية test_نجی_انکڑے));
	انکڑے->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	إذا (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &انکڑے->real);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		انکڑے->real = 0;
	}
	انکڑے->read_blk =	test_io_cb_read_blk;
	انکڑے->write_blk =	test_io_cb_write_blk;
	انکڑے->سیٹ_blksize =	test_io_cb_سیٹ_blksize;
	انکڑے->write_byte =	test_io_cb_write_byte;
	انکڑے->read_blk64 =	test_io_cb_read_blk64;
	انکڑے->write_blk64 =	test_io_cb_write_blk64;

	انکڑے->outfile = عدم;
	إذا ((قدر = ext2fs_محفوظ_getenv("TEST_IO_LOGFILE")) != عدم)
		انکڑے->outfile = fopen(قدر, "w");
	إذا (!انکڑے->outfile)
		انکڑے->outfile = stderr;

	انکڑے->flags = 0;
	إذا ((قدر = ext2fs_محفوظ_getenv("TEST_IO_FLAGS")) != عدم)
		انکڑے->flags = strtoul(قدر, عدم, 0);

	انکڑے->بلاک = 0;
	إذا ((قدر = ext2fs_محفوظ_getenv("TEST_IO_BLOCK")) != عدم)
		انکڑے->بلاک = strtoul(قدر, عدم, 0);

	انکڑے->read_abort_گنتی = 0;
	إذا ((قدر = ext2fs_محفوظ_getenv("TEST_IO_READ_ABORT")) != عدم)
		انکڑے->read_abort_گنتی = strtoul(قدر, عدم, 0);

	انکڑے->write_abort_گنتی = 0;
	إذا ((قدر = ext2fs_محفوظ_getenv("TEST_IO_WRITE_ABORT")) != عدم)
		انکڑے->write_abort_گنتی = strtoul(قدر, عدم, 0);

	إذا (انکڑے->real) {
		io->align = انکڑے->real->align;
		إذا (انکڑے->real->flags & CHANNEL_FLAGS_دھاگے)
			io->flags |= CHANNEL_FLAGS_دھاگے;
	}

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	إذا (انکڑے)
		ext2fs_free_mem(&انکڑے);
	أرجع retval;
}

ساكن errcode_t test_close(io_channel channel)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	إذا (انکڑے->real)
		retval = io_channel_close(انکڑے->real);

	إذا (انکڑے->outfile && انکڑے->outfile != stderr)
		fclose(انکڑے->outfile);

	ext2fs_free_mem(&channel->نجی_انکڑے);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t test_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real) {
		retval = io_channel_سیٹ_blksize(انکڑے->real, blksize);
		channel->align = انکڑے->real->align;
	}
	إذا (انکڑے->سیٹ_blksize)
		انکڑے->سیٹ_blksize(blksize, retval);
	إذا (انکڑے->flags & TEST_FLAG_سیٹ_BLKSIZE)
		fprintf(انکڑے->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_سنیہا(retval) : "OK");
	channel->بلاک_ناپ = blksize;
	أرجع retval;
}


ساكن errcode_t test_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_read_blk(انکڑے->real, بلاک, گنتی, buf);
	إذا (انکڑے->read_blk)
		انکڑے->read_blk(بلاک, گنتی, retval);
	إذا (انکڑے->flags & TEST_FLAG_READ)
		fprintf(انکڑے->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	إذا (انکڑے->بلاک && انکڑے->بلاک == بلاک) {
		إذا (انکڑے->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, انکڑے, بلاک, buf);
		إذا (--انکڑے->read_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, ثابت فراغ *buf)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_write_blk(انکڑے->real, بلاک, گنتی, buf);
	إذا (انکڑے->write_blk)
		انکڑے->write_blk(بلاک, گنتی, retval);
	إذا (انکڑے->flags & TEST_FLAG_WRITE)
		fprintf(انکڑے->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	إذا (انکڑے->بلاک && انکڑے->بلاک == بلاک) {
		إذا (انکڑے->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, انکڑے, بلاک, buf);
		إذا (--انکڑے->write_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_read_blk64(io_channel channel, بنانشانی لما لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_read_blk64(انکڑے->real, بلاک, گنتی, buf);
	إذا (انکڑے->read_blk64)
		انکڑے->read_blk64(بلاک, گنتی, retval);
	إذا (انکڑے->flags & TEST_FLAG_READ)
		fprintf(انکڑے->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	إذا (انکڑے->بلاک && انکڑے->بلاک == بلاک) {
		إذا (انکڑے->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, انکڑے, بلاک, buf);
		إذا (--انکڑے->read_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk64(io_channel channel, بنانشانی لما لما بلاک,
			       صحيح گنتی, ثابت فراغ *buf)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_write_blk64(انکڑے->real, بلاک, گنتی, buf);
	إذا (انکڑے->write_blk64)
		انکڑے->write_blk64(بلاک, گنتی, retval);
	إذا (انکڑے->flags & TEST_FLAG_WRITE)
		fprintf(انکڑے->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	إذا (انکڑے->بلاک && انکڑے->بلاک == بلاک) {
		إذا (انکڑے->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, انکڑے, بلاک, buf);
		إذا (--انکڑے->write_abort_گنتی == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_byte(io_channel channel, بنانشانی لما وتھ,
			       صحيح گنتی, ثابت فراغ *buf)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real && انکڑے->real->manager->write_byte)
		retval = io_channel_write_byte(انکڑے->real, وتھ, گنتی, buf);
	إذا (انکڑے->write_byte)
		انکڑے->write_byte(وتھ, گنتی, retval);
	إذا (انکڑے->flags & TEST_FLAG_WRITE)
		fprintf(انکڑے->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			وتھ, گنتی, retval ? error_سنیہا(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	إذا (انکڑے->flags & TEST_FLAG_سیٹ_OPTION)
		fprintf(انکڑے->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	إذا (انکڑے->real && انکڑے->real->manager->سیٹ_option) {
		retval = (انکڑے->real->manager->سیٹ_option)(انکڑے->real,
							   option, arg);
		إذا (انکڑے->flags & TEST_FLAG_سیٹ_OPTION)
			fprintf(انکڑے->outfile, "returned %s\n",
				retval ? error_سنیہا(retval) : "OK");
	} وإلا {
		إذا (انکڑے->flags & TEST_FLAG_سیٹ_OPTION)
			fprintf(انکڑے->outfile, "not implemented\n");
	}
	أرجع retval;
}

ساكن errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real && انکڑے->real->manager->get_stats) {
		retval = (انکڑے->real->manager->get_stats)(انکڑے->real, stats);
	}
	أرجع retval;
}

ساكن errcode_t test_discard(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_discard(انکڑے->real, بلاک, گنتی);
	إذا (انکڑے->flags & TEST_FLAG_DISCARD)
		fprintf(انکڑے->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_cache_readahead(io_channel channel,
				      بنانشانی لما لما بلاک,
				      بنانشانی لما لما گنتی)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_cache_readahead(انکڑے->real, بلاک, گنتی);
	إذا (انکڑے->flags & TEST_FLAG_READAHEAD)
		fprintf(انکڑے->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_zeroout(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية test_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية test_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_zeroout(انکڑے->real, بلاک, گنتی);
	إذا (انکڑے->flags & TEST_FLAG_ZEROOUT)
		fprintf(انکڑے->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			بلاک, گنتی, retval ? error_سنیہا(retval) : "OK");
	أرجع retval;
}

ساكن بنية struct_io_manager struct_test_manager = {
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
