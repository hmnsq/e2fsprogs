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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

నిర్మాణం test_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం	magic;
	io_channel real;
	పూర్ణాంకం flags;
	FILE *outfile;
	గుర్తులేని దీర్ఘ ఖండం;
	పూర్ణాంకం read_abort_లెక్క, write_abort_లెక్క;
	శూన్య (*read_blk)(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
	శూన్య (*write_blk)(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
	శూన్య (*సమితి_blksize)(పూర్ణాంకం blksize, errcode_t err);
	శూన్య (*write_byte)(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
	శూన్య (*read_blk64)(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
	శూన్య (*write_blk64)(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
శూన్య (*test_io_cb_read_blk)
	(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err) = 0;
శూన్య (*test_io_cb_write_blk)
	(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err) = 0;
శూన్య (*test_io_cb_read_blk64)
	(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err) = 0;
శూన్య (*test_io_cb_write_blk64)
	(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err) = 0;
శూన్య (*test_io_cb_సమితి_blksize)
	(పూర్ణాంకం blksize, errcode_t err) = 0;
శూన్య (*test_io_cb_write_byte)
	(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err) = 0;

/*
 * Test flags
 */
#నిర్వచించు TEST_FLAG_READ			0x01
#నిర్వచించు TEST_FLAG_WRITE			0x02
#నిర్వచించు TEST_FLAG_సమితి_BLKSIZE		0x04
#నిర్వచించు TEST_FLAG_FLUSH			0x08
#నిర్వచించు TEST_FLAG_DUMP			0x10
#నిర్వచించు TEST_FLAG_సమితి_OPTION		0x20
#నిర్వచించు TEST_FLAG_DISCARD		0x40
#నిర్వచించు TEST_FLAG_READAHEAD		0x80
#నిర్వచించు TEST_FLAG_ZEROOUT		0x100

స్థిర శూన్య test_dump_ఖండం(io_channel channel,
			    నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం,
			    గుర్తులేని దీర్ఘ ఖండం, స్థిరరాశి శూన్య *buf)
{
	స్థిరరాశి గుర్తులేని అక్షరం *cp;
	FILE *f = దత్తాంశం->outfile;
	పూర్ణాంకం	i;
	గుర్తులేని దీర్ఘ	cksum = 0;

	కొరకు (i=0, cp = buf; i < channel->ఖండం_కొలత; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", ఖండం, cksum);
	కొరకు (i=0, cp = buf; i < channel->ఖండం_కొలత; i++, cp++) {
		ఒకవేళ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
స్థిర errcode_t test_flush(io_channel channel)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం	*దత్తాంశం;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *)channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_flush(దత్తాంశం->real);

	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_FLUSH)
		fprintf(దత్తాంశం->outfile, "Test_io: flush() returned %s\n",
			retval ? error_సందేశం(retval) : "OK");

	తిరిగి retval;
}

స్థిర శూన్య test_abort(io_channel channel, గుర్తులేని దీర్ఘ ఖండం)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	FILE *f;

	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	f = దత్తాంశం->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", ఖండం);
	fflush(f);
	abort();
}

స్థిర errcode_t test_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	io_channel	io = ఖాళీ;
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం = ఖాళీ;
	errcode_t	retval;
	అక్షరం		*విలువ;

	ఒకవేళ (name == 0)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_io_channel), &io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(io, 0, పరిమాణం(నిర్మాణం struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం test_వ్యక్తిగత_దత్తాంశం), &దత్తాంశం);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &io->name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(io->name, name);
	io->వ్యక్తిగత_దత్తాంశం = దత్తాంశం;
	io->ఖండం_కొలత = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(దత్తాంశం, 0, పరిమాణం(నిర్మాణం test_వ్యక్తిగత_దత్తాంశం));
	దత్తాంశం->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ఒకవేళ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &దత్తాంశం->real);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే {
		దత్తాంశం->real = 0;
	}
	దత్తాంశం->read_blk =	test_io_cb_read_blk;
	దత్తాంశం->write_blk =	test_io_cb_write_blk;
	దత్తాంశం->సమితి_blksize =	test_io_cb_సమితి_blksize;
	దత్తాంశం->write_byte =	test_io_cb_write_byte;
	దత్తాంశం->read_blk64 =	test_io_cb_read_blk64;
	దత్తాంశం->write_blk64 =	test_io_cb_write_blk64;

	దత్తాంశం->outfile = ఖాళీ;
	ఒకవేళ ((విలువ = ext2fs_సురక్షిత_getenv("TEST_IO_LOGFILE")) != ఖాళీ)
		దత్తాంశం->outfile = fopen(విలువ, "w");
	ఒకవేళ (!దత్తాంశం->outfile)
		దత్తాంశం->outfile = stderr;

	దత్తాంశం->flags = 0;
	ఒకవేళ ((విలువ = ext2fs_సురక్షిత_getenv("TEST_IO_FLAGS")) != ఖాళీ)
		దత్తాంశం->flags = strtoul(విలువ, ఖాళీ, 0);

	దత్తాంశం->ఖండం = 0;
	ఒకవేళ ((విలువ = ext2fs_సురక్షిత_getenv("TEST_IO_BLOCK")) != ఖాళీ)
		దత్తాంశం->ఖండం = strtoul(విలువ, ఖాళీ, 0);

	దత్తాంశం->read_abort_లెక్క = 0;
	ఒకవేళ ((విలువ = ext2fs_సురక్షిత_getenv("TEST_IO_READ_ABORT")) != ఖాళీ)
		దత్తాంశం->read_abort_లెక్క = strtoul(విలువ, ఖాళీ, 0);

	దత్తాంశం->write_abort_లెక్క = 0;
	ఒకవేళ ((విలువ = ext2fs_సురక్షిత_getenv("TEST_IO_WRITE_ABORT")) != ఖాళీ)
		దత్తాంశం->write_abort_లెక్క = strtoul(విలువ, ఖాళీ, 0);

	ఒకవేళ (దత్తాంశం->real) {
		io->align = దత్తాంశం->real->align;
		ఒకవేళ (దత్తాంశం->real->flags & CHANNEL_FLAGS_దారాలు)
			io->flags |= CHANNEL_FLAGS_దారాలు;
	}

	*channel = io;
	తిరిగి 0;

cleanup:
	ఒకవేళ (io && io->name)
		ext2fs_free_mem(&io->name);
	ఒకవేళ (io)
		ext2fs_free_mem(&io);
	ఒకవేళ (దత్తాంశం)
		ext2fs_free_mem(&దత్తాంశం);
	తిరిగి retval;
}

స్థిర errcode_t test_close(io_channel channel)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
		తిరిగి 0;

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_close(దత్తాంశం->real);

	ఒకవేళ (దత్తాంశం->outfile && దత్తాంశం->outfile != stderr)
		fclose(దత్తాంశం->outfile);

	ext2fs_free_mem(&channel->వ్యక్తిగత_దత్తాంశం);
	ఒకవేళ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	తిరిగి retval;
}

స్థిర errcode_t test_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real) {
		retval = io_channel_సమితి_blksize(దత్తాంశం->real, blksize);
		channel->align = దత్తాంశం->real->align;
	}
	ఒకవేళ (దత్తాంశం->సమితి_blksize)
		దత్తాంశం->సమితి_blksize(blksize, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_సమితి_BLKSIZE)
		fprintf(దత్తాంశం->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_సందేశం(retval) : "OK");
	channel->ఖండం_కొలత = blksize;
	తిరిగి retval;
}


స్థిర errcode_t test_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_read_blk(దత్తాంశం->real, ఖండం, లెక్క, buf);
	ఒకవేళ (దత్తాంశం->read_blk)
		దత్తాంశం->read_blk(ఖండం, లెక్క, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_READ)
		fprintf(దత్తాంశం->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	ఒకవేళ (దత్తాంశం->ఖండం && దత్తాంశం->ఖండం == ఖండం) {
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_DUMP)
			test_dump_ఖండం(channel, దత్తాంశం, ఖండం, buf);
		ఒకవేళ (--దత్తాంశం->read_abort_లెక్క == 0)
			test_abort(channel, ఖండం);
	}
	తిరిగి retval;
}

స్థిర errcode_t test_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_write_blk(దత్తాంశం->real, ఖండం, లెక్క, buf);
	ఒకవేళ (దత్తాంశం->write_blk)
		దత్తాంశం->write_blk(ఖండం, లెక్క, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_WRITE)
		fprintf(దత్తాంశం->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	ఒకవేళ (దత్తాంశం->ఖండం && దత్తాంశం->ఖండం == ఖండం) {
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_DUMP)
			test_dump_ఖండం(channel, దత్తాంశం, ఖండం, buf);
		ఒకవేళ (--దత్తాంశం->write_abort_లెక్క == 0)
			test_abort(channel, ఖండం);
	}
	తిరిగి retval;
}

స్థిర errcode_t test_read_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_read_blk64(దత్తాంశం->real, ఖండం, లెక్క, buf);
	ఒకవేళ (దత్తాంశం->read_blk64)
		దత్తాంశం->read_blk64(ఖండం, లెక్క, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_READ)
		fprintf(దత్తాంశం->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	ఒకవేళ (దత్తాంశం->ఖండం && దత్తాంశం->ఖండం == ఖండం) {
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_DUMP)
			test_dump_ఖండం(channel, దత్తాంశం, ఖండం, buf);
		ఒకవేళ (--దత్తాంశం->read_abort_లెక్క == 0)
			test_abort(channel, ఖండం);
	}
	తిరిగి retval;
}

స్థిర errcode_t test_write_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_write_blk64(దత్తాంశం->real, ఖండం, లెక్క, buf);
	ఒకవేళ (దత్తాంశం->write_blk64)
		దత్తాంశం->write_blk64(ఖండం, లెక్క, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_WRITE)
		fprintf(దత్తాంశం->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	ఒకవేళ (దత్తాంశం->ఖండం && దత్తాంశం->ఖండం == ఖండం) {
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_DUMP)
			test_dump_ఖండం(channel, దత్తాంశం, ఖండం, buf);
		ఒకవేళ (--దత్తాంశం->write_abort_లెక్క == 0)
			test_abort(channel, ఖండం);
	}
	తిరిగి retval;
}

స్థిర errcode_t test_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real && దత్తాంశం->real->manager->write_byte)
		retval = io_channel_write_byte(దత్తాంశం->real, స్థానభ్రంశం, లెక్క, buf);
	ఒకవేళ (దత్తాంశం->write_byte)
		దత్తాంశం->write_byte(స్థానభ్రంశం, లెక్క, retval);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_WRITE)
		fprintf(దత్తాంశం->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			స్థానభ్రంశం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	తిరిగి retval;
}

స్థిర errcode_t test_సమితి_option(io_channel channel, స్థిరరాశి అక్షరం *option,
				 స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_సమితి_OPTION)
		fprintf(దత్తాంశం->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ఒకవేళ (దత్తాంశం->real && దత్తాంశం->real->manager->సమితి_option) {
		retval = (దత్తాంశం->real->manager->సమితి_option)(దత్తాంశం->real,
							   option, arg);
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_సమితి_OPTION)
			fprintf(దత్తాంశం->outfile, "returned %s\n",
				retval ? error_సందేశం(retval) : "OK");
	} లేకపోతే {
		ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_సమితి_OPTION)
			fprintf(దత్తాంశం->outfile, "not implemented\n");
	}
	తిరిగి retval;
}

స్థిర errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real && దత్తాంశం->real->manager->get_stats) {
		retval = (దత్తాంశం->real->manager->get_stats)(దత్తాంశం->real, stats);
	}
	తిరిగి retval;
}

స్థిర errcode_t test_discard(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_discard(దత్తాంశం->real, ఖండం, లెక్క);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_DISCARD)
		fprintf(దత్తాంశం->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	తిరిగి retval;
}

స్థిర errcode_t test_cache_readahead(io_channel channel,
				      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_cache_readahead(దత్తాంశం->real, ఖండం, లెక్క);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_READAHEAD)
		fprintf(దత్తాంశం->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	తిరిగి retval;
}

స్థిర errcode_t test_zeroout(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం test_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_zeroout(దత్తాంశం->real, ఖండం, లెక్క);
	ఒకవేళ (దత్తాంశం->flags & TEST_FLAG_ZEROOUT)
		fprintf(దత్తాంశం->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			ఖండం, లెక్క, retval ? error_సందేశం(retval) : "OK");
	తిరిగి retval;
}

స్థిర నిర్మాణం struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.సమితి_blksize	= test_సమితి_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.సమితి_option	= test_సమితి_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
