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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ	magic;
	io_channel real;
	ಪೂರ್ಣಾಂಕ flags;
	FILE *outfile;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ;
	ಪೂರ್ಣಾಂಕ read_abort_ಎಣಿಕೆ, write_abort_ಎಣಿಕೆ;
	ಶೂನ್ಯ (*read_blk)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
	ಶೂನ್ಯ (*write_blk)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
	ಶೂನ್ಯ (*ಗಣ_blksize)(ಪೂರ್ಣಾಂಕ blksize, errcode_t err);
	ಶೂನ್ಯ (*write_byte)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
	ಶೂನ್ಯ (*read_blk64)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
	ಶೂನ್ಯ (*write_blk64)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
ಶೂನ್ಯ (*test_io_cb_read_blk)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_blk)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_read_blk64)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_blk64)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_ಗಣ_blksize)
	(ಪೂರ್ಣಾಂಕ blksize, errcode_t err) = 0;
ಶೂನ್ಯ (*test_io_cb_write_byte)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err) = 0;

/*
 * Test flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_READ			0x01
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_WRITE			0x02
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_ಗಣ_BLKSIZE		0x04
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_FLUSH			0x08
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_DUMP			0x10
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_ಗಣ_OPTION		0x20
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_DISCARD		0x40
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_READAHEAD		0x80
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FLAG_ZEROOUT		0x100

ಸ್ಥಿರ ಶೂನ್ಯ test_dump_ಖಂಡ(io_channel channel,
			    ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *cp;
	FILE *f = ದತ್ತಾಂಶ->outfile;
	ಪೂರ್ಣಾಂಕ	i;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	cksum = 0;

	ಗಾಗಿ (i=0, cp = buf; i < channel->ಖಂಡ_ಗಾತ್ರ; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", ಖಂಡ, cksum);
	ಗಾಗಿ (i=0, cp = buf; i < channel->ಖಂಡ_ಗಾತ್ರ; i++, cp++) {
		ಆದರೆ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t test_flush(io_channel channel)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ	*ದತ್ತಾಂಶ;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *)channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_flush(ದತ್ತಾಂಶ->real);

	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_FLUSH)
		fprintf(ದತ್ತಾಂಶ->outfile, "Test_io: flush() returned %s\n",
			retval ? error_ಸಂದೇಶ(retval) : "OK");

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ test_abort(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	FILE *f;

	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	f = ದತ್ತಾಂಶ->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", ಖಂಡ);
	fflush(f);
	abort();
}

ಸ್ಥಿರ errcode_t test_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	io_channel	io = ಖಾಲಿ;
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = ಖಾಲಿ;
	errcode_t	retval;
	ಅಕ್ಷರ		*ಮೌಲ್ಯ;

	ಆದರೆ (name == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_io_channel), &io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(io, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ), &ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &io->name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(io->name, name);
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	io->ಖಂಡ_ಗಾತ್ರ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ));
	ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ಆದರೆ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &ದತ್ತಾಂಶ->real);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ದತ್ತಾಂಶ->real = 0;
	}
	ದತ್ತಾಂಶ->read_blk =	test_io_cb_read_blk;
	ದತ್ತಾಂಶ->write_blk =	test_io_cb_write_blk;
	ದತ್ತಾಂಶ->ಗಣ_blksize =	test_io_cb_ಗಣ_blksize;
	ದತ್ತಾಂಶ->write_byte =	test_io_cb_write_byte;
	ದತ್ತಾಂಶ->read_blk64 =	test_io_cb_read_blk64;
	ದತ್ತಾಂಶ->write_blk64 =	test_io_cb_write_blk64;

	ದತ್ತಾಂಶ->outfile = ಖಾಲಿ;
	ಆದರೆ ((ಮೌಲ್ಯ = ext2fs_ಸುರಕ್ಷಿತ_getenv("TEST_IO_LOGFILE")) != ಖಾಲಿ)
		ದತ್ತಾಂಶ->outfile = fopen(ಮೌಲ್ಯ, "w");
	ಆದರೆ (!ದತ್ತಾಂಶ->outfile)
		ದತ್ತಾಂಶ->outfile = stderr;

	ದತ್ತಾಂಶ->flags = 0;
	ಆದರೆ ((ಮೌಲ್ಯ = ext2fs_ಸುರಕ್ಷಿತ_getenv("TEST_IO_FLAGS")) != ಖಾಲಿ)
		ದತ್ತಾಂಶ->flags = strtoul(ಮೌಲ್ಯ, ಖಾಲಿ, 0);

	ದತ್ತಾಂಶ->ಖಂಡ = 0;
	ಆದರೆ ((ಮೌಲ್ಯ = ext2fs_ಸುರಕ್ಷಿತ_getenv("TEST_IO_BLOCK")) != ಖಾಲಿ)
		ದತ್ತಾಂಶ->ಖಂಡ = strtoul(ಮೌಲ್ಯ, ಖಾಲಿ, 0);

	ದತ್ತಾಂಶ->read_abort_ಎಣಿಕೆ = 0;
	ಆದರೆ ((ಮೌಲ್ಯ = ext2fs_ಸುರಕ್ಷಿತ_getenv("TEST_IO_READ_ABORT")) != ಖಾಲಿ)
		ದತ್ತಾಂಶ->read_abort_ಎಣಿಕೆ = strtoul(ಮೌಲ್ಯ, ಖಾಲಿ, 0);

	ದತ್ತಾಂಶ->write_abort_ಎಣಿಕೆ = 0;
	ಆದರೆ ((ಮೌಲ್ಯ = ext2fs_ಸುರಕ್ಷಿತ_getenv("TEST_IO_WRITE_ABORT")) != ಖಾಲಿ)
		ದತ್ತಾಂಶ->write_abort_ಎಣಿಕೆ = strtoul(ಮೌಲ್ಯ, ಖಾಲಿ, 0);

	ಆದರೆ (ದತ್ತಾಂಶ->real) {
		io->align = ದತ್ತಾಂಶ->real->align;
		ಆದರೆ (ದತ್ತಾಂಶ->real->flags & CHANNEL_FLAGS_ಎಳೆಗಳು)
			io->flags |= CHANNEL_FLAGS_ಎಳೆಗಳು;
	}

	*channel = io;
	ಹಿಂತಿರುಗಿ 0;

cleanup:
	ಆದರೆ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಆದರೆ (io)
		ext2fs_free_mem(&io);
	ಆದರೆ (ದತ್ತಾಂಶ)
		ext2fs_free_mem(&ದತ್ತಾಂಶ);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_close(io_channel channel)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_close(ದತ್ತಾಂಶ->real);

	ಆದರೆ (ದತ್ತಾಂಶ->outfile && ದತ್ತಾಂಶ->outfile != stderr)
		fclose(ದತ್ತಾಂಶ->outfile);

	ext2fs_free_mem(&channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಆದರೆ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real) {
		retval = io_channel_ಗಣ_blksize(ದತ್ತಾಂಶ->real, blksize);
		channel->align = ದತ್ತಾಂಶ->real->align;
	}
	ಆದರೆ (ದತ್ತಾಂಶ->ಗಣ_blksize)
		ದತ್ತಾಂಶ->ಗಣ_blksize(blksize, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_ಗಣ_BLKSIZE)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_ಸಂದೇಶ(retval) : "OK");
	channel->ಖಂಡ_ಗಾತ್ರ = blksize;
	ಹಿಂತಿರುಗಿ retval;
}


ಸ್ಥಿರ errcode_t test_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_read_blk(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	ಆದರೆ (ದತ್ತಾಂಶ->read_blk)
		ದತ್ತಾಂಶ->read_blk(ಖಂಡ, ಎಣಿಕೆ_2, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_READ)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಆದರೆ (ದತ್ತಾಂಶ->ಖಂಡ && ದತ್ತಾಂಶ->ಖಂಡ == ಖಂಡ) {
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_DUMP)
			test_dump_ಖಂಡ(channel, ದತ್ತಾಂಶ, ಖಂಡ, buf);
		ಆದರೆ (--ದತ್ತಾಂಶ->read_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, ಖಂಡ);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_write_blk(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	ಆದರೆ (ದತ್ತಾಂಶ->write_blk)
		ದತ್ತಾಂಶ->write_blk(ಖಂಡ, ಎಣಿಕೆ_2, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_WRITE)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಆದರೆ (ದತ್ತಾಂಶ->ಖಂಡ && ದತ್ತಾಂಶ->ಖಂಡ == ಖಂಡ) {
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_DUMP)
			test_dump_ಖಂಡ(channel, ದತ್ತಾಂಶ, ಖಂಡ, buf);
		ಆದರೆ (--ದತ್ತಾಂಶ->write_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, ಖಂಡ);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_read_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_read_blk64(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	ಆದರೆ (ದತ್ತಾಂಶ->read_blk64)
		ದತ್ತಾಂಶ->read_blk64(ಖಂಡ, ಎಣಿಕೆ_2, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_READ)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಆದರೆ (ದತ್ತಾಂಶ->ಖಂಡ && ದತ್ತಾಂಶ->ಖಂಡ == ಖಂಡ) {
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_DUMP)
			test_dump_ಖಂಡ(channel, ದತ್ತಾಂಶ, ಖಂಡ, buf);
		ಆದರೆ (--ದತ್ತಾಂಶ->read_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, ಖಂಡ);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_write_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_write_blk64(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);
	ಆದರೆ (ದತ್ತಾಂಶ->write_blk64)
		ದತ್ತಾಂಶ->write_blk64(ಖಂಡ, ಎಣಿಕೆ_2, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_WRITE)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಆದರೆ (ದತ್ತಾಂಶ->ಖಂಡ && ದತ್ತಾಂಶ->ಖಂಡ == ಖಂಡ) {
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_DUMP)
			test_dump_ಖಂಡ(channel, ದತ್ತಾಂಶ, ಖಂಡ, buf);
		ಆದರೆ (--ದತ್ತಾಂಶ->write_abort_ಎಣಿಕೆ == 0)
			test_abort(channel, ಖಂಡ);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real && ದತ್ತಾಂಶ->real->manager->write_byte)
		retval = io_channel_write_byte(ದತ್ತಾಂಶ->real, ಸ್ಥಾನಾಂತರ, ಎಣಿಕೆ_2, buf);
	ಆದರೆ (ದತ್ತಾಂಶ->write_byte)
		ದತ್ತಾಂಶ->write_byte(ಸ್ಥಾನಾಂತರ, ಎಣಿಕೆ_2, retval);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_WRITE)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			ಸ್ಥಾನಾಂತರ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_ಗಣ_option(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_ಗಣ_OPTION)
		fprintf(ದತ್ತಾಂಶ->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ಆದರೆ (ದತ್ತಾಂಶ->real && ದತ್ತಾಂಶ->real->manager->ಗಣ_option) {
		retval = (ದತ್ತಾಂಶ->real->manager->ಗಣ_option)(ದತ್ತಾಂಶ->real,
							   option, arg);
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_ಗಣ_OPTION)
			fprintf(ದತ್ತಾಂಶ->outfile, "returned %s\n",
				retval ? error_ಸಂದೇಶ(retval) : "OK");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_ಗಣ_OPTION)
			fprintf(ದತ್ತಾಂಶ->outfile, "not implemented\n");
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real && ದತ್ತಾಂಶ->real->manager->get_stats) {
		retval = (ದತ್ತಾಂಶ->real->manager->get_stats)(ದತ್ತಾಂಶ->real, stats);
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_discard(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_discard(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_DISCARD)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_cache_readahead(io_channel channel,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_cache_readahead(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_READAHEAD)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t test_zeroout(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ test_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_zeroout(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);
	ಆದರೆ (ದತ್ತಾಂಶ->flags & TEST_FLAG_ZEROOUT)
		fprintf(ದತ್ತಾಂಶ->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			ಖಂಡ, ಎಣಿಕೆ_2, retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.ಗಣ_blksize	= test_ಗಣ_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.ಗಣ_option	= test_ಗಣ_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
