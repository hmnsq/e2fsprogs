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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)

بنية test_شخصي_معلومات {
	صحيح	magic;
	io_channel real;
	صحيح flags;
	FILE *outfile;
	بینښه اوږد بلاک;
	صحيح read_abort_شمېر, write_abort_شمېر;
	فراغ (*read_blk)(بینښه اوږد بلاک, صحيح شمېر, errcode_t err);
	فراغ (*write_blk)(بینښه اوږد بلاک, صحيح شمېر, errcode_t err);
	فراغ (*سېټ_blksize)(صحيح blksize, errcode_t err);
	فراغ (*write_byte)(بینښه اوږد بلاک, صحيح شمېر, errcode_t err);
	فراغ (*read_blk64)(بینښه اوږد اوږد بلاک, صحيح شمېر, errcode_t err);
	فراغ (*write_blk64)(بینښه اوږد اوږد بلاک, صحيح شمېر, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
فراغ (*test_io_cb_read_blk)
	(بینښه اوږد بلاک, صحيح شمېر, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk)
	(بینښه اوږد بلاک, صحيح شمېر, errcode_t err) = 0;
فراغ (*test_io_cb_read_blk64)
	(بینښه اوږد اوږد بلاک, صحيح شمېر, errcode_t err) = 0;
فراغ (*test_io_cb_write_blk64)
	(بینښه اوږد اوږد بلاک, صحيح شمېر, errcode_t err) = 0;
فراغ (*test_io_cb_سېټ_blksize)
	(صحيح blksize, errcode_t err) = 0;
فراغ (*test_io_cb_write_byte)
	(بینښه اوږد بلاک, صحيح شمېر, errcode_t err) = 0;

/*
 * Test flags
 */
#تعریف_کړه TEST_FLAG_READ			0x01
#تعریف_کړه TEST_FLAG_WRITE			0x02
#تعریف_کړه TEST_FLAG_سېټ_BLKSIZE		0x04
#تعریف_کړه TEST_FLAG_FLUSH			0x08
#تعریف_کړه TEST_FLAG_DUMP			0x10
#تعریف_کړه TEST_FLAG_سېټ_OPTION		0x20
#تعریف_کړه TEST_FLAG_DISCARD		0x40
#تعریف_کړه TEST_FLAG_READAHEAD		0x80
#تعریف_کړه TEST_FLAG_ZEROOUT		0x100

ساكن فراغ test_dump_بلاک(io_channel channel,
			    بنية test_شخصي_معلومات *معلومات,
			    بینښه اوږد بلاک, ثابت فراغ *buf)
{
	ثابت بینښه حرف *cp;
	FILE *f = معلومات->outfile;
	صحيح	i;
	بینښه اوږد	cksum = 0;

	لكل (i=0, cp = buf; i < channel->بلاک_اندازه; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", بلاک, cksum);
	لكل (i=0, cp = buf; i < channel->بلاک_اندازه; i++, cp++) {
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
	بنية test_شخصي_معلومات	*معلومات;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *)channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_flush(معلومات->real);

	إذا (معلومات->flags & TEST_FLAG_FLUSH)
		fprintf(معلومات->outfile, "Test_io: flush() returned %s\n",
			retval ? error_پیغام(retval) : "OK");

	أرجع retval;
}

ساكن فراغ test_abort(io_channel channel, بینښه اوږد بلاک)
{
	بنية test_شخصي_معلومات *معلومات;
	FILE *f;

	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	f = معلومات->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", بلاک);
	fflush(f);
	abort();
}

ساكن errcode_t test_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية test_شخصي_معلومات *معلومات = عدم;
	errcode_t	retval;
	حرف		*ارزښت;

	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية test_شخصي_معلومات), &معلومات);
	إذا (retval)
		اذهب cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(مزي_اوږدوالی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->شخصي_معلومات = معلومات;
	io->بلاک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(معلومات, 0, حجم(بنية test_شخصي_معلومات));
	معلومات->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	إذا (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &معلومات->real);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		معلومات->real = 0;
	}
	معلومات->read_blk =	test_io_cb_read_blk;
	معلومات->write_blk =	test_io_cb_write_blk;
	معلومات->سېټ_blksize =	test_io_cb_سېټ_blksize;
	معلومات->write_byte =	test_io_cb_write_byte;
	معلومات->read_blk64 =	test_io_cb_read_blk64;
	معلومات->write_blk64 =	test_io_cb_write_blk64;

	معلومات->outfile = عدم;
	إذا ((ارزښت = ext2fs_خوندي_getenv("TEST_IO_LOGFILE")) != عدم)
		معلومات->outfile = fopen(ارزښت, "w");
	إذا (!معلومات->outfile)
		معلومات->outfile = stderr;

	معلومات->flags = 0;
	إذا ((ارزښت = ext2fs_خوندي_getenv("TEST_IO_FLAGS")) != عدم)
		معلومات->flags = strtoul(ارزښت, عدم, 0);

	معلومات->بلاک = 0;
	إذا ((ارزښت = ext2fs_خوندي_getenv("TEST_IO_BLOCK")) != عدم)
		معلومات->بلاک = strtoul(ارزښت, عدم, 0);

	معلومات->read_abort_شمېر = 0;
	إذا ((ارزښت = ext2fs_خوندي_getenv("TEST_IO_READ_ABORT")) != عدم)
		معلومات->read_abort_شمېر = strtoul(ارزښت, عدم, 0);

	معلومات->write_abort_شمېر = 0;
	إذا ((ارزښت = ext2fs_خوندي_getenv("TEST_IO_WRITE_ABORT")) != عدم)
		معلومات->write_abort_شمېر = strtoul(ارزښت, عدم, 0);

	إذا (معلومات->real) {
		io->align = معلومات->real->align;
		إذا (معلومات->real->flags & CHANNEL_FLAGS_تارونه)
			io->flags |= CHANNEL_FLAGS_تارونه;
	}

	*channel = io;
	أرجع 0;

cleanup:
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	إذا (معلومات)
		ext2fs_free_mem(&معلومات);
	أرجع retval;
}

ساكن errcode_t test_close(io_channel channel)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;

	إذا (معلومات->real)
		retval = io_channel_close(معلومات->real);

	إذا (معلومات->outfile && معلومات->outfile != stderr)
		fclose(معلومات->outfile);

	ext2fs_free_mem(&channel->شخصي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	أرجع retval;
}

ساكن errcode_t test_سېټ_blksize(io_channel channel, صحيح blksize)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real) {
		retval = io_channel_سېټ_blksize(معلومات->real, blksize);
		channel->align = معلومات->real->align;
	}
	إذا (معلومات->سېټ_blksize)
		معلومات->سېټ_blksize(blksize, retval);
	إذا (معلومات->flags & TEST_FLAG_سېټ_BLKSIZE)
		fprintf(معلومات->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_پیغام(retval) : "OK");
	channel->بلاک_اندازه = blksize;
	أرجع retval;
}


ساكن errcode_t test_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_read_blk(معلومات->real, بلاک, شمېر, buf);
	إذا (معلومات->read_blk)
		معلومات->read_blk(بلاک, شمېر, retval);
	إذا (معلومات->flags & TEST_FLAG_READ)
		fprintf(معلومات->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	إذا (معلومات->بلاک && معلومات->بلاک == بلاک) {
		إذا (معلومات->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, معلومات, بلاک, buf);
		إذا (--معلومات->read_abort_شمېر == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, ثابت فراغ *buf)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_write_blk(معلومات->real, بلاک, شمېر, buf);
	إذا (معلومات->write_blk)
		معلومات->write_blk(بلاک, شمېر, retval);
	إذا (معلومات->flags & TEST_FLAG_WRITE)
		fprintf(معلومات->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	إذا (معلومات->بلاک && معلومات->بلاک == بلاک) {
		إذا (معلومات->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, معلومات, بلاک, buf);
		إذا (--معلومات->write_abort_شمېر == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_read_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_read_blk64(معلومات->real, بلاک, شمېر, buf);
	إذا (معلومات->read_blk64)
		معلومات->read_blk64(بلاک, شمېر, retval);
	إذا (معلومات->flags & TEST_FLAG_READ)
		fprintf(معلومات->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	إذا (معلومات->بلاک && معلومات->بلاک == بلاک) {
		إذا (معلومات->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, معلومات, بلاک, buf);
		إذا (--معلومات->read_abort_شمېر == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
			       صحيح شمېر, ثابت فراغ *buf)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_write_blk64(معلومات->real, بلاک, شمېر, buf);
	إذا (معلومات->write_blk64)
		معلومات->write_blk64(بلاک, شمېر, retval);
	إذا (معلومات->flags & TEST_FLAG_WRITE)
		fprintf(معلومات->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	إذا (معلومات->بلاک && معلومات->بلاک == بلاک) {
		إذا (معلومات->flags & TEST_FLAG_DUMP)
			test_dump_بلاک(channel, معلومات, بلاک, buf);
		إذا (--معلومات->write_abort_شمېر == 0)
			test_abort(channel, بلاک);
	}
	أرجع retval;
}

ساكن errcode_t test_write_byte(io_channel channel, بینښه اوږد دننه_واټن,
			       صحيح شمېر, ثابت فراغ *buf)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real && معلومات->real->manager->write_byte)
		retval = io_channel_write_byte(معلومات->real, دننه_واټن, شمېر, buf);
	إذا (معلومات->write_byte)
		معلومات->write_byte(دننه_واټن, شمېر, retval);
	إذا (معلومات->flags & TEST_FLAG_WRITE)
		fprintf(معلومات->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			دننه_واټن, شمېر, retval ? error_پیغام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_سېټ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	إذا (معلومات->flags & TEST_FLAG_سېټ_OPTION)
		fprintf(معلومات->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	إذا (معلومات->real && معلومات->real->manager->سېټ_option) {
		retval = (معلومات->real->manager->سېټ_option)(معلومات->real,
							   option, arg);
		إذا (معلومات->flags & TEST_FLAG_سېټ_OPTION)
			fprintf(معلومات->outfile, "returned %s\n",
				retval ? error_پیغام(retval) : "OK");
	} وإلا {
		إذا (معلومات->flags & TEST_FLAG_سېټ_OPTION)
			fprintf(معلومات->outfile, "not implemented\n");
	}
	أرجع retval;
}

ساكن errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real && معلومات->real->manager->get_stats) {
		retval = (معلومات->real->manager->get_stats)(معلومات->real, stats);
	}
	أرجع retval;
}

ساكن errcode_t test_discard(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_discard(معلومات->real, بلاک, شمېر);
	إذا (معلومات->flags & TEST_FLAG_DISCARD)
		fprintf(معلومات->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_cache_readahead(io_channel channel,
				      بینښه اوږد اوږد بلاک,
				      بینښه اوږد اوږد شمېر)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_cache_readahead(معلومات->real, بلاک, شمېر);
	إذا (معلومات->flags & TEST_FLAG_READAHEAD)
		fprintf(معلومات->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	أرجع retval;
}

ساكن errcode_t test_zeroout(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية test_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية test_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_zeroout(معلومات->real, بلاک, شمېر);
	إذا (معلومات->flags & TEST_FLAG_ZEROOUT)
		fprintf(معلومات->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			بلاک, شمېر, retval ? error_پیغام(retval) : "OK");
	أرجع retval;
}

ساكن بنية struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.سېټ_blksize	= test_سېټ_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.سېټ_option	= test_سېټ_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
