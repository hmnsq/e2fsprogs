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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ	magic;
	io_channel real;
	ਪੂਰਨ flags;
	FILE *outfile;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ;
	ਪੂਰਨ read_abort_ਗਿਣਤੀ, write_abort_ਗਿਣਤੀ;
	ਖਾਲੀ (*read_blk)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
	ਖਾਲੀ (*write_blk)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
	ਖਾਲੀ (*ਸੈੱਟ_blksize)(ਪੂਰਨ blksize, errcode_t err);
	ਖਾਲੀ (*write_byte)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
	ਖਾਲੀ (*read_blk64)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
	ਖਾਲੀ (*write_blk64)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
};

/*
 * These global variable can be set by the test program as
 * necessary *before* calling test_open
 */
io_manager test_io_backing_manager = 0;
ਖਾਲੀ (*test_io_cb_read_blk)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err) = 0;
ਖਾਲੀ (*test_io_cb_write_blk)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err) = 0;
ਖਾਲੀ (*test_io_cb_read_blk64)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err) = 0;
ਖਾਲੀ (*test_io_cb_write_blk64)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err) = 0;
ਖਾਲੀ (*test_io_cb_ਸੈੱਟ_blksize)
	(ਪੂਰਨ blksize, errcode_t err) = 0;
ਖਾਲੀ (*test_io_cb_write_byte)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err) = 0;

/*
 * Test flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_READ			0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_WRITE			0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_ਸੈੱਟ_BLKSIZE		0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_FLUSH			0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_DUMP			0x10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_ਸੈੱਟ_OPTION		0x20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_DISCARD		0x40
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_READAHEAD		0x80
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FLAG_ZEROOUT		0x100

ਸਥਿਰ ਖਾਲੀ test_dump_ਖੰਡ(io_channel channel,
			    ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਅਚਲ ਖਾਲੀ *buf)
{
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp;
	FILE *f = ਅੰਕੜੇ->outfile;
	ਪੂਰਨ	i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	cksum = 0;

	ਲਈ (i=0, cp = buf; i < channel->ਖੰਡ_ਮਾਪ; i++, cp++) {
		cksum += *cp;
	}
	fprintf(f, "Contents of block %lu, checksum %08lu: \n", ਖੰਡ, cksum);
	ਲਈ (i=0, cp = buf; i < channel->ਖੰਡ_ਮਾਪ; i++, cp++) {
		ਜੇ ((i % 16) == 0)
			fprintf(f, "%04x: ", i);
		fprintf(f, "%02x%c", *cp, ((i % 16) == 15) ? '\n' : ' ');
	}
}

/*
 * Flush data buffers to disk.
 */
ਸਥਿਰ errcode_t test_flush(io_channel channel)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ	*ਅੰਕੜੇ;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *)channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_flush(ਅੰਕੜੇ->real);

	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_FLUSH)
		fprintf(ਅੰਕੜੇ->outfile, "Test_io: flush() returned %s\n",
			retval ? error_ਸੁਨੇਹਾ(retval) : "OK");

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਖਾਲੀ test_abort(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	FILE *f;

	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	f = ਅੰਕੜੇ->outfile;
	test_flush(channel);

	fprintf(f, "Aborting due to I/O to block %lu\n", ਖੰਡ);
	fflush(f);
	abort();
}

ਸਥਿਰ errcode_t test_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	io_channel	io = ਨਿਰਾ;
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ = ਨਿਰਾ;
	errcode_t	retval;
	ਅੱਖਰ		*ਮੁੱਲ;

	ਜੇ (name == 0)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_io_channel), &io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(io, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ), &ਅੰਕੜੇ);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	io->manager = test_io_manager;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &io->name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(io->name, name);
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	io->ਖੰਡ_ਮਾਪ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;
	io->flags = 0;

	memset(ਅੰਕੜੇ, 0, ਆਕਾਰ(ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ));
	ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_TEST_IO_CHANNEL;
	ਜੇ (test_io_backing_manager) {
		retval = test_io_backing_manager->open(name, flags,
						       &ਅੰਕੜੇ->real);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		ਅੰਕੜੇ->real = 0;
	}
	ਅੰਕੜੇ->read_blk =	test_io_cb_read_blk;
	ਅੰਕੜੇ->write_blk =	test_io_cb_write_blk;
	ਅੰਕੜੇ->ਸੈੱਟ_blksize =	test_io_cb_ਸੈੱਟ_blksize;
	ਅੰਕੜੇ->write_byte =	test_io_cb_write_byte;
	ਅੰਕੜੇ->read_blk64 =	test_io_cb_read_blk64;
	ਅੰਕੜੇ->write_blk64 =	test_io_cb_write_blk64;

	ਅੰਕੜੇ->outfile = ਨਿਰਾ;
	ਜੇ ((ਮੁੱਲ = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("TEST_IO_LOGFILE")) != ਨਿਰਾ)
		ਅੰਕੜੇ->outfile = fopen(ਮੁੱਲ, "w");
	ਜੇ (!ਅੰਕੜੇ->outfile)
		ਅੰਕੜੇ->outfile = stderr;

	ਅੰਕੜੇ->flags = 0;
	ਜੇ ((ਮੁੱਲ = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("TEST_IO_FLAGS")) != ਨਿਰਾ)
		ਅੰਕੜੇ->flags = strtoul(ਮੁੱਲ, ਨਿਰਾ, 0);

	ਅੰਕੜੇ->ਖੰਡ = 0;
	ਜੇ ((ਮੁੱਲ = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("TEST_IO_BLOCK")) != ਨਿਰਾ)
		ਅੰਕੜੇ->ਖੰਡ = strtoul(ਮੁੱਲ, ਨਿਰਾ, 0);

	ਅੰਕੜੇ->read_abort_ਗਿਣਤੀ = 0;
	ਜੇ ((ਮੁੱਲ = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("TEST_IO_READ_ABORT")) != ਨਿਰਾ)
		ਅੰਕੜੇ->read_abort_ਗਿਣਤੀ = strtoul(ਮੁੱਲ, ਨਿਰਾ, 0);

	ਅੰਕੜੇ->write_abort_ਗਿਣਤੀ = 0;
	ਜੇ ((ਮੁੱਲ = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("TEST_IO_WRITE_ABORT")) != ਨਿਰਾ)
		ਅੰਕੜੇ->write_abort_ਗਿਣਤੀ = strtoul(ਮੁੱਲ, ਨਿਰਾ, 0);

	ਜੇ (ਅੰਕੜੇ->real) {
		io->align = ਅੰਕੜੇ->real->align;
		ਜੇ (ਅੰਕੜੇ->real->flags & CHANNEL_FLAGS_ਧਾਗੇ)
			io->flags |= CHANNEL_FLAGS_ਧਾਗੇ;
	}

	*channel = io;
	ਵਾਪਸ 0;

cleanup:
	ਜੇ (io && io->name)
		ext2fs_free_mem(&io->name);
	ਜੇ (io)
		ext2fs_free_mem(&io);
	ਜੇ (ਅੰਕੜੇ)
		ext2fs_free_mem(&ਅੰਕੜੇ);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_close(io_channel channel)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
		ਵਾਪਸ 0;

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_close(ਅੰਕੜੇ->real);

	ਜੇ (ਅੰਕੜੇ->outfile && ਅੰਕੜੇ->outfile != stderr)
		fclose(ਅੰਕੜੇ->outfile);

	ext2fs_free_mem(&channel->ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਜੇ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real) {
		retval = io_channel_ਸੈੱਟ_blksize(ਅੰਕੜੇ->real, blksize);
		channel->align = ਅੰਕੜੇ->real->align;
	}
	ਜੇ (ਅੰਕੜੇ->ਸੈੱਟ_blksize)
		ਅੰਕੜੇ->ਸੈੱਟ_blksize(blksize, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_ਸੈੱਟ_BLKSIZE)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: set_blksize(%d) returned %s\n",
			blksize, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	channel->ਖੰਡ_ਮਾਪ = blksize;
	ਵਾਪਸ retval;
}


ਸਥਿਰ errcode_t test_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_read_blk(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	ਜੇ (ਅੰਕੜੇ->read_blk)
		ਅੰਕੜੇ->read_blk(ਖੰਡ, ਗਿਣਤੀ_2, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_READ)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: read_blk(%lu, %d) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਜੇ (ਅੰਕੜੇ->ਖੰਡ && ਅੰਕੜੇ->ਖੰਡ == ਖੰਡ) {
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_DUMP)
			test_dump_ਖੰਡ(channel, ਅੰਕੜੇ, ਖੰਡ, buf);
		ਜੇ (--ਅੰਕੜੇ->read_abort_ਗਿਣਤੀ == 0)
			test_abort(channel, ਖੰਡ);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_write_blk(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	ਜੇ (ਅੰਕੜੇ->write_blk)
		ਅੰਕੜੇ->write_blk(ਖੰਡ, ਗਿਣਤੀ_2, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_WRITE)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: write_blk(%lu, %d) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਜੇ (ਅੰਕੜੇ->ਖੰਡ && ਅੰਕੜੇ->ਖੰਡ == ਖੰਡ) {
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_DUMP)
			test_dump_ਖੰਡ(channel, ਅੰਕੜੇ, ਖੰਡ, buf);
		ਜੇ (--ਅੰਕੜੇ->write_abort_ਗਿਣਤੀ == 0)
			test_abort(channel, ਖੰਡ);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_read_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_read_blk64(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	ਜੇ (ਅੰਕੜੇ->read_blk64)
		ਅੰਕੜੇ->read_blk64(ਖੰਡ, ਗਿਣਤੀ_2, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_READ)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: read_blk64(%llu, %d) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਜੇ (ਅੰਕੜੇ->ਖੰਡ && ਅੰਕੜੇ->ਖੰਡ == ਖੰਡ) {
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_DUMP)
			test_dump_ਖੰਡ(channel, ਅੰਕੜੇ, ਖੰਡ, buf);
		ਜੇ (--ਅੰਕੜੇ->read_abort_ਗਿਣਤੀ == 0)
			test_abort(channel, ਖੰਡ);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_write_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_write_blk64(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);
	ਜੇ (ਅੰਕੜੇ->write_blk64)
		ਅੰਕੜੇ->write_blk64(ਖੰਡ, ਗਿਣਤੀ_2, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_WRITE)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: write_blk64(%llu, %d) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਜੇ (ਅੰਕੜੇ->ਖੰਡ && ਅੰਕੜੇ->ਖੰਡ == ਖੰਡ) {
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_DUMP)
			test_dump_ਖੰਡ(channel, ਅੰਕੜੇ, ਖੰਡ, buf);
		ਜੇ (--ਅੰਕੜੇ->write_abort_ਗਿਣਤੀ == 0)
			test_abort(channel, ਖੰਡ);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real && ਅੰਕੜੇ->real->manager->write_byte)
		retval = io_channel_write_byte(ਅੰਕੜੇ->real, ਵਿੱਥ, ਗਿਣਤੀ_2, buf);
	ਜੇ (ਅੰਕੜੇ->write_byte)
		ਅੰਕੜੇ->write_byte(ਵਿੱਥ, ਗਿਣਤੀ_2, retval);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_WRITE)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: write_byte(%lu, %d) returned %s\n",
			ਵਿੱਥ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_ਸੈੱਟ_option(io_channel channel, ਅਚਲ ਅੱਖਰ *option,
				 ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);


	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_ਸੈੱਟ_OPTION)
		fprintf(ਅੰਕੜੇ->outfile, "Test_io: set_option(%s, %s) ",
			option, arg);
	ਜੇ (ਅੰਕੜੇ->real && ਅੰਕੜੇ->real->manager->ਸੈੱਟ_option) {
		retval = (ਅੰਕੜੇ->real->manager->ਸੈੱਟ_option)(ਅੰਕੜੇ->real,
							   option, arg);
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_ਸੈੱਟ_OPTION)
			fprintf(ਅੰਕੜੇ->outfile, "returned %s\n",
				retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_ਸੈੱਟ_OPTION)
			fprintf(ਅੰਕੜੇ->outfile, "not implemented\n");
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_get_stats(io_channel channel, io_stats *stats)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real && ਅੰਕੜੇ->real->manager->get_stats) {
		retval = (ਅੰਕੜੇ->real->manager->get_stats)(ਅੰਕੜੇ->real, stats);
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_discard(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_discard(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_DISCARD)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: discard(%llu, %llu) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_cache_readahead(io_channel channel,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_cache_readahead(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_READAHEAD)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: readahead(%llu, %llu) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t test_zeroout(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ test_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_TEST_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_zeroout(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);
	ਜੇ (ਅੰਕੜੇ->flags & TEST_FLAG_ZEROOUT)
		fprintf(ਅੰਕੜੇ->outfile,
			"Test_io: zeroout(%llu, %llu) returned %s\n",
			ਖੰਡ, ਗਿਣਤੀ_2, retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_test_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Test I/O Manager",
	.open		= test_open,
	.close		= test_close,
	.ਸੈੱਟ_blksize	= test_ਸੈੱਟ_blksize,
	.read_blk	= test_read_blk,
	.write_blk	= test_write_blk,
	.flush		= test_flush,
	.write_byte	= test_write_byte,
	.ਸੈੱਟ_option	= test_ਸੈੱਟ_option,
	.get_stats	= test_get_stats,
	.read_blk64	= test_read_blk64,
	.write_blk64	= test_write_blk64,
	.discard	= test_discard,
	.cache_readahead	= test_cache_readahead,
	.zeroout	= test_zeroout,
};

io_manager test_io_manager = &struct_test_manager;
