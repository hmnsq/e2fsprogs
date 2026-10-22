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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)

संरचना test_वैयक्तिकम्_दत्त {
	पूर्णाङ्क	magic;
	io_channel real;
	पूर्णाङ्क flags;
	FILE *outfile;
	अचिह्नित दीर्घ खण्डम्;
	पूर्णाङ्क read_abort_गणनम्, write_abort_गणनम्;
	शून्य (*read_blk)(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*write_blk)(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*समुच्चयः_blksize)(पूर्णाङ्क blksize, errcode_t err);
	शून्य (*write_byte)(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*read_blk64)(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
	शून्य (*write_blk64)(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
शून्य (*test_io_cb_read_blk)
	(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk)
	(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_read_blk64)
	(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_write_blk64)
	(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err) = 0;
शून्य (*test_io_cb_समुच्चयः_blksize)
	(पूर्णाङ्क blksize, errcode_t err) = 0;
शून्य (*test_io_cb_write_byte)
	(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err) = 0;

/*
 * Test flags
 */
#परिभाषय TEST_FLAG_READ			0x01
#परिभाषय TEST_FLAG_WRITE			0x02
#परिभाषय TEST_FLAG_समुच्चयः_BLKSIZE		0x04
#परिभाषय TEST_FLAG_FLUSH			0x08
#परिभाषय TEST_FLAG_DUMP			0x10
#परिभाषय TEST_FLAG_समुच्चयः_OPTION		0x20
#परिभाषय TEST_FLAG_DISCARD		0x40
#परिभाषय TEST_FLAG_READAHEAD		0x80
#परिभाषय TEST_FLAG_ZEROOUT		0x100

स्थिर शून्य test_dump_खण्डम्(io_channel channel,
			    संरचना test_वैयक्तिकम्_दत्त *दत्त,
			    अचिह्नित दीर्घ खण्डम्, ध्रुव शून्य *buf)
{
	ध्रुव अचिह्नित वर्ण *cp;
	FILE *f = दत्त->outfile;
	पूर्णाङ्क	i;
	अचिह्नित दीर्घ	cksum = 0;

	प्रति (i=0, cp = buf; i < channel->खण्डम्_आकार; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", खण्डम्, cksum);
	प्रति (i=0, cp = buf; i < channel->खण्डम्_आकार; i++, cp++) {
		यदि ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t test_flush(io_channel channel)
{
	संरचना test_वैयक्तिकम्_दत्त	*दत्त;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *)channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_flush(दत्त->real);

	यदि (दत्त->flags & TEST_FLAG_FLUSH)
		fprintf(दत्त->outfile, "Test_io: flush() returned %s\n",
			retval ? error_सन्देशः(retval) : "OK");

	प्रत्यागम retval;
}

स्थिर शून्य test_abort(io_channel channel, अचिह्नित दीर्घ खण्डम्)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	FILE *f;

	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	f = दत्त->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", खण्डम्);
	fflush(f);
	abort();
}

स्थिर errcode_t test_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना test_वैयक्तिकम्_दत्त *दत्त = रिक्त;
	errcode_t	retval;
	वर्ण		*मानम्;

	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना test_वैयक्तिकम्_दत्त), &दत्त);
	यदि (retval)
		गच्छ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(io->name, name);
	io->वैयक्तिकम्_दत्त = दत्त;
	io->खण्डम्_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	स्मृतिं_पूरय(दत्त, 0, परिमाण(संरचना test_वैयक्तिकम्_दत्त));
	दत्त->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	यदि (test_io_backing_manager) {
		retval = test_io_backing_manager->उद्घाटय(name, flags,
						       &दत्त->real);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		दत्त->real = 0;
	}
	दत्त->read_blk =	test_io_cb_read_blk;
	दत्त->write_blk =	test_io_cb_write_blk;
	दत्त->समुच्चयः_blksize =	test_io_cb_समुच्चयः_blksize;
	दत्त->write_byte =	test_io_cb_write_byte;
	दत्त->read_blk64 =	test_io_cb_read_blk64;
	दत्त->write_blk64 =	test_io_cb_write_blk64;

	दत्त->outfile = रिक्त;
	यदि ((मानम् = ext2fs_safe_getenv("TEST_IO_LOGFILE")) != रिक्त)
		दत्त->outfile = fopen(मानम्, "w");
	यदि (!दत्त->outfile)
		दत्त->outfile = stderr;

	दत्त->flags = 0;
	यदि ((मानम् = ext2fs_safe_getenv("TEST_IO_FLAGS")) != रिक्त)
		दत्त->flags = strtoul(मानम्, रिक्त, 0);

	दत्त->खण्डम् = 0;
	यदि ((मानम् = ext2fs_safe_getenv("TEST_IO_BLOCK")) != रिक्त)
		दत्त->खण्डम् = strtoul(मानम्, रिक्त, 0);

	दत्त->read_abort_गणनम् = 0;
	यदि ((मानम् = ext2fs_safe_getenv("TEST_IO_READ_ABORT")) != रिक्त)
		दत्त->read_abort_गणनम् = strtoul(मानम्, रिक्त, 0);

	दत्त->write_abort_गणनम् = 0;
	यदि ((मानम् = ext2fs_safe_getenv("TEST_IO_WRITE_ABORT")) != रिक्त)
		दत्त->write_abort_गणनम् = strtoul(मानम्, रिक्त, 0);

	यदि (दत्त->real) {
		io->align = दत्त->real->align;
		यदि (दत्त->real->flags & CHANNEL_FLAGS_तन्तवः)
			io->flags |= CHANNEL_FLAGS_तन्तवः;
	}

	*channel = io;
	प्रत्यागम 0;

cleanup:
	यदि (io && io->name)
		ext2fs_free_mem(&io->name);
	यदि (io)
		ext2fs_free_mem(&io);
	यदि (दत्त)
		ext2fs_free_mem(&दत्त);
	प्रत्यागम retval;
}

स्थिर errcode_t test_close(io_channel channel)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;

	यदि (दत्त->real)
		retval = io_channel_close(दत्त->real);

	यदि (दत्त->outfile && दत्त->outfile != stderr)
		fclose(दत्त->outfile);

	ext2fs_free_mem(&channel->वैयक्तिकम्_दत्त);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	प्रत्यागम retval;
}

स्थिर errcode_t test_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real) {
		retval = io_channel_समुच्चयः_blksize(दत्त->real, blksize);
		channel->align = दत्त->real->align;
	}
	यदि (दत्त->समुच्चयः_blksize)
		दत्त->समुच्चयः_blksize(blksize, retval);
	यदि (दत्त->flags & TEST_FLAG_समुच्चयः_BLKSIZE)
		fprintf(दत्त->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_सन्देशः(retval) : "OK");
	channel->खण्डम्_आकार = blksize;
	प्रत्यागम retval;
}


स्थिर errcode_t test_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_read_blk(दत्त->real, खण्डम्, गणनम्, buf);
	यदि (दत्त->read_blk)
		दत्त->read_blk(खण्डम्, गणनम्, retval);
	यदि (दत्त->flags & TEST_FLAG_READ)
		fprintf(दत्त->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	यदि (दत्त->खण्डम् && दत्त->खण्डम् == खण्डम्) {
		यदि (दत्त->flags & TEST_FLAG_DUMP)
			test_dump_खण्डम्(channel, दत्त, खण्डम्, buf);
		यदि (--दत्त->read_abort_गणनम् == 0)
			test_abort(channel, खण्डम्);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_write_blk(दत्त->real, खण्डम्, गणनम्, buf);
	यदि (दत्त->write_blk)
		दत्त->write_blk(खण्डम्, गणनम्, retval);
	यदि (दत्त->flags & TEST_FLAG_WRITE)
		fprintf(दत्त->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	यदि (दत्त->खण्डम् && दत्त->खण्डम् == खण्डम्) {
		यदि (दत्त->flags & TEST_FLAG_DUMP)
			test_dump_खण्डम्(channel, दत्त, खण्डम्, buf);
		यदि (--दत्त->write_abort_गणनम् == 0)
			test_abort(channel, खण्डम्);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_read_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_read_blk64(दत्त->real, खण्डम्, गणनम्, buf);
	यदि (दत्त->read_blk64)
		दत्त->read_blk64(खण्डम्, गणनम्, retval);
	यदि (दत्त->flags & TEST_FLAG_READ)
		fprintf(दत्त->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	यदि (दत्त->खण्डम् && दत्त->खण्डम् == खण्डम्) {
		यदि (दत्त->flags & TEST_FLAG_DUMP)
			test_dump_खण्डम्(channel, दत्त, खण्डम्, buf);
		यदि (--दत्त->read_abort_गणनम् == 0)
			test_abort(channel, खण्डम्);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_write_blk64(दत्त->real, खण्डम्, गणनम्, buf);
	यदि (दत्त->write_blk64)
		दत्त->write_blk64(खण्डम्, गणनम्, retval);
	यदि (दत्त->flags & TEST_FLAG_WRITE)
		fprintf(दत्त->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	यदि (दत्त->खण्डम् && दत्त->खण्डम् == खण्डम्) {
		यदि (दत्त->flags & TEST_FLAG_DUMP)
			test_dump_खण्डम्(channel, दत्त, खण्डम्, buf);
		यदि (--दत्त->write_abort_गणनम् == 0)
			test_abort(channel, खण्डम्);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real && दत्त->real->manager->write_byte)
		retval = io_channel_write_byte(दत्त->real, विस्थापनम्, गणनम्, buf);
	यदि (दत्त->write_byte)
		दत्त->write_byte(विस्थापनम्, गणनम्, retval);
	यदि (दत्त->flags & TEST_FLAG_WRITE)
		fprintf(दत्त->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			विस्थापनम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_समुच्चयः_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	यदि (दत्त->flags & TEST_FLAG_समुच्चयः_OPTION)
		fprintf(दत्त->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	यदि (दत्त->real && दत्त->real->manager->समुच्चयः_option) {
		retval = (दत्त->real->manager->समुच्चयः_option)(दत्त->real,
							   option, arg);
		यदि (दत्त->flags & TEST_FLAG_समुच्चयः_OPTION)
			fprintf(दत्त->outfile, "returned %s\n",
				retval ? error_सन्देशः(retval) : "OK");
	} अन्यथा {
		यदि (दत्त->flags & TEST_FLAG_समुच्चयः_OPTION)
			fprintf(दत्त->outfile, "not implemented\n");
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real && दत्त->real->manager->get_stats) {
		retval = (दत्त->real->manager->get_stats)(दत्त->real, stats);
	}
	प्रत्यागम retval;
}

स्थिर errcode_t test_discard(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_discard(दत्त->real, खण्डम्, गणनम्);
	यदि (दत्त->flags & TEST_FLAG_DISCARD)
		fprintf(दत्त->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_cache_readahead(io_channel channel,
				      अचिह्नित दीर्घ दीर्घ खण्डम्,
				      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_cache_readahead(दत्त->real, खण्डम्, गणनम्);
	यदि (दत्त->flags & TEST_FLAG_READAHEAD)
		fprintf(दत्त->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर errcode_t test_zeroout(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना test_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना test_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_zeroout(दत्त->real, खण्डम्, गणनम्);
	यदि (दत्त->flags & TEST_FLAG_ZEROOUT)
		fprintf(दत्त->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			खण्डम्, गणनम्, retval ? error_सन्देशः(retval) : "OK");
	प्रत्यागम retval;
}

स्थिर संरचना struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.उद्घाटय		= test_open,
	.close		= test_close,
	.समुच्चयः_blksize	= test_समुच्चयः_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.समुच्चयः_option	= test_समुच्चयः_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
