/*
 * undo_io.c --- This is the undo io manager that copies the old data that
 * copies the old data being overwritten into a tdb database
 *
 * Copyright IBM Corporation, 2007
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#ifdef __linux__
#شامل <sys/utsname.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#اگر HAVE_SYS_RESOURCE_H
#شامل <sys/resource.h>
#endif
#شامل <limits.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#وگرنه
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  انجامبده {printf(f, ## a); fflush(stdout); } وقتی (0)
#وگرنه
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)
/*
 * Undo file format: The file is cut up into undo_header.block_size blocks.
 * The first block contains the header.
 * The second block contains the superblock.
 * There is then a repeating series of blocks as follows:
 *   A key block, which contains undo_keys to map the following data blocks.
 *   Data blocks
 * (Note that there are pointers to the first key block and the sb, so this
 * order isn't strictly necessary.)
 */
#define E2UNDO_MAGIC "E2UNDO02"
#define KEYBLOCK_MAGIC 0xCADECADE

#define E2UNDO_STATE_FINISHED	0x1	/* undo file is complete */

#define E2UNDO_MIN_BLOCK_SIZE	1024	/* undo blocks are no less than 1KB */
#define E2UNDO_MAX_BLOCK_SIZE	1048576	/* undo blocks are no more than 1MB */

ساختار undo_header {
	نویسه magic[8];		/* "E2UNDO02" */
	__le64 رقم_کلیدها;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 کلید_offset;	/* where do the key/data block chunks start? */
	__le32 block_size;	/* block size of the undo file */
	__le32 fs_block_size;	/* block size of the target device */
	__le32 sb_crc;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_offset;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 header_crc;	/* crc32c of this header (but not this field) */
};

#define E2UNDO_MAX_EXTENT_BLOCKS	512	/* max extent size, in blocks */

ساختار undo_کلید {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

ساختار undo_کلید_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#اگر __STDC_VERSION__ >= 199901L
	ساختار undo_کلید کلیدها[];		/* keys, which come immediately after */
#وگرنه
	ساختار undo_کلید کلیدها[0];	/* keys, which come immediately after */
#endif
};

ساختار undo_private_data {
	صحیح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_رقم;			/* next free block */
	blk64_t کلید_blk_رقم;			/* current key block location */
	blk64_t super_blk_رقم;			/* superblock location */
	blk64_t first_کلید_blk;			/* first key block location */
	ساختار undo_کلید_block *keyb;
	size_t رقم_کلیدها, کلیدها_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	صحیح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	ساختار struct_ext2_filsys fake_fs;
	نویسه *tdb_file;
	ساختار undo_header hdr;
};
#define کلیدها_PER_BLOCK(d) (((d)->tdb_data_size / اندازه(ساختار undo_کلید)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

ایستا درونخطی تهی e2undo_set_feature_fs_offset(ساختار undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ایستا درونخطی تهی e2undo_clear_feature_fs_offset(ساختار undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ایستا io_manager undo_io_backing_manager;
ایستا نویسه *tdb_file;
ایستا صحیح actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	بازگشت 0;
}

errcode_t set_undo_io_backup_file(نویسه *file_name)
{
	tdb_file = strdup(file_name);

	اگر (tdb_file == پوچ) {
		بازگشت EXT2_ET_NO_MEMORY;
	}

	بازگشت 0;
}

ایستا errcode_t write_undo_indexes(ساختار undo_private_data *data, صحیح flush)
{
	errcode_t retval;
	ساختار ext2_super_block super;
	io_channel channel;
	صحیح block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	اگر (data->کلیدها_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned نویسه *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->کلید_blk_رقم);
		retval = io_channel_write_blk64(data->undo_file,
						data->کلید_blk_رقم,
						1, data->keyb);
		اگر (retval)
			بازگشت retval;
		/* Move on to the next key block if it's full. */
		اگر (data->کلیدها_in_block == کلیدها_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->کلیدها_in_block = 0;
			data->کلید_blk_رقم = data->undo_blk_رقم;
			data->undo_blk_رقم++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	اگر (retval)
		برو err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned نویسه *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, اندازه(data->hdr.magic));
	data->hdr.رقم_کلیدها = ext2fs_cpu_to_le64(data->رقم_کلیدها);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_رقم);
	data->hdr.کلید_offset = ext2fs_cpu_to_le64(data->first_کلید_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	اگر (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	وگرنه
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned نویسه *)&data->hdr,
				   اندازه(data->hdr) -
				   اندازه(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(صحیح)اندازه(data->hdr),
					&data->hdr);
	اگر (retval)
		برو err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_رقم);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_رقم,
					-SUPERBLOCK_SIZE, &super);
	اگر (retval)
		برو err_out;

	اگر (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	بازگشت retval;
}

ایستا errcode_t undo_setup_tdb(ساختار undo_private_data *data)
{
	صحیح i;
	errcode_t retval;

	اگر (data->tdb_written == 1)
		بازگشت 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, اندازه(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	اگر (retval)
		بازگشت retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	اگر (retval)
		بازگشت retval;
	data->کلید_blk_رقم = data->first_کلید_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", کلیدها_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	برای (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		اگر (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		اگر (retval)
			بازگشت retval;
		retval = io_channel_flush(data->undo_file);
		اگر (retval)
			بازگشت retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	بازگشت 0;
}

ایستا errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, صحیح شمار)

{
	صحیح size, sz;
	unsigned long long block_رقم, backing_blk_رقم;
	errcode_t retval = 0;
	ext2_loff_t offset;
	ساختار undo_private_data *data;
	unsigned نویسه *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	ساختار undo_کلید *کلید;
	__u32 blk_crc;

	data = (ساختار undo_private_data *) channel->private_data;

	اگر (data->undo_file == پوچ) {
		/*
		 * Transaction database not initialized
		 */
		بازگشت 0;
	}

	اگر (شمار == 1)
		size = channel->block_size;
	وگرنه {
		اگر (شمار < 0)
			size = -شمار;
		وگرنه
			size = شمار * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	اگر (retval)
		بازگشت retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_رقم = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	وقتی (block_رقم <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		اگر (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_رقم)) {
			/* Try the next block */
			block_رقم++;
			ادامه;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_رقم);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_رقم * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_رقم = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		اگر (retval) {
			بازگشت retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		اگر ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		وگرنه
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_رقم,
					     sz, read_ptr);
		اگر (retval) {
			اگر (retval != EXT2_ET_SHORT_READ) {
				آزاد(read_ptr);
				بازگشت retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} وگرنه {
			data_size = data->tdb_data_size;
		}
		اگر (data_size == 0) {
			آزاد(read_ptr);
			block_رقم++;
			ادامه;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_رقم, block, data->tdb_data_size);
		اگر ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		وگرنه
			sz = -data_size;;
		/* extend this key? */
		اگر (data->کلیدها_in_block) {
			کلید = data->keyb->کلیدها + data->کلیدها_in_block - 1;
			keysz = ext2fs_le32_to_cpu(کلید->size);
		} وگرنه {
			کلید = پوچ;
			keysz = 0;
		}
		اگر (کلید != پوچ &&
		    (ext2fs_le64_to_cpu(کلید->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_رقم &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(کلید->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			کلید->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			کلید->size = ext2fs_cpu_to_le32(keysz + data_size);
		} وگرنه {
			data->رقم_کلیدها++;
			کلید = data->keyb->کلیدها + data->کلیدها_in_block;
			data->کلیدها_in_block++;
			کلید->fsblk = ext2fs_cpu_to_le64(backing_blk_رقم);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			کلید->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			کلید->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_رقم,
		       data->undo_blk_رقم,
		       sz, data->رقم_کلیدها - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_رقم, sz, read_ptr);
		اگر (retval) {
			آزاد(read_ptr);
			بازگشت retval;
		}
		data->undo_blk_رقم++;
		آزاد(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		اگر (retval)
			بازگشت retval;

		/* Next block */
		block_رقم++;
	}

	بازگشت retval;
}

ایستا errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    صحیح شمار ATTR((unused)),
				    تهی *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    صحیح actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	بازگشت error;
}

ایستا تهی undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ایستا صحیح check_filesystem(ساختار undo_header *hdr, io_channel undo_file,
			    unsigned صحیح blocksize, blk64_t super_block,
			    io_channel channel)
{
	ساختار ext2_super_block super, *sb;
	نویسه *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	اگر (retval)
		بازگشت retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	اگر (retval)
		بازگشت retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	اگر (retval)
		برو out;
	sb = (ساختار ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	اگر (memcmp(&super, buf, اندازه(super))) {
		retval = -1;
		برو out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned نویسه *)buf, SUPERBLOCK_SIZE);
	اگر (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		برو out;
	}

out:
	ext2fs_free_mem(&buf);
	بازگشت retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ایستا errcode_t try_reopen_undo_file(صحیح undo_fd,
				      ساختار undo_private_data *data)
{
	ساختار undo_header hdr;
	ساختار undo_کلید *dkey;
	ext2fs_struct_stat statbuf;
	unsigned صحیح blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t رقم_کلیدها, کلیدها_per_block, i;
	__u32 hdr_crc, کلید_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	اگر (retval)
		برو bad_file;
	اگر (statbuf.st_size == 0)
		برو out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(صحیح)اندازه(hdr),
				       &hdr);
	اگر (retval)
		برو bad_file;

	اگر (memcmp(hdr.magic, E2UNDO_MAGIC,
		    اندازه(hdr.magic)))
		برو bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned نویسه *)&hdr,
				   اندازه(ساختار undo_header) -
				   اندازه(__u32));
	اگر (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		برو bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	اگر (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		برو bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	رقم_کلیدها = ext2fs_le64_to_cpu(hdr.رقم_کلیدها);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	اگر (hdr.f_incompat || hdr.f_rocompat)
		برو bad_file;

	/* Superblock matches this FS? */
	اگر (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		برو out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	اگر (retval)
		برو bad_file;
	data->رقم_کلیدها = رقم_کلیدها;
	data->super_blk_رقم = super_block;
	data->first_کلید_blk = ext2fs_le64_to_cpu(hdr.کلید_offset);

	/* load the written block map */
	کلیدها_per_block = کلیدها_PER_BLOCK(data);
	lblk = data->first_کلید_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   رقم_کلیدها, کلیدها_per_block, blocksize);
	برای (i = 0; i < رقم_کلیدها; i += کلیدها_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->کلید_blk_رقم = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		اگر (retval)
			برو bad_کلید_replay;

		/* check keys */
		اگر (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			برو bad_کلید_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		کلید_crc = ext2fs_crc32c_le(~0, (unsigned نویسه *)data->keyb,
					   blocksize);
		اگر (ext2fs_le32_to_cpu(crc) != کلید_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			برو bad_کلید_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->رقم_کلیدها - i;
		اگر (max_j > کلیدها_per_block)
			max_j = کلیدها_per_block;
		برای (j = 0, dkey = data->keyb->کلیدها;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_رقم = lblk;
			data->کلیدها_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->کلید_blk_رقم, data->undo_blk_رقم, data->رقم_کلیدها,
		   data->کلیدها_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	بازگشت retval;

bad_کلید_replay:
	data->کلید_blk_رقم = data->undo_blk_رقم = 0;
	data->کلیدها_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	برو out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	بازگشت retval;
}

ایستا تهی undo_atexit(تهی *p)
{
	ساختار undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

ایستا errcode_t undo_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	io_channel	io = پوچ;
	ساختار undo_private_data *data = پوچ;
	صحیح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	اگر (name == 0)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(اندازه(ساختار struct_io_channel), &io);
	اگر (retval)
		برو cleanup;
	memset(io, 0, اندازه(ساختار struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(اندازه(ساختار undo_private_data), &data);
	اگر (retval)
		برو cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	اگر (retval)
		برو cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, اندازه(ساختار undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_رقم = 1;
	data->first_کلید_blk = 2;
	data->undo_blk_رقم = 3;

	اگر (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		اگر (retval)
			برو cleanup;

		data->tdb_file = strdup(tdb_file);
		اگر (data->tdb_file == پوچ)
			برو cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		اگر (undo_fd < 0)
			برو cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		اگر (retval)
			برو cleanup;
	} وگرنه {
		data->real = پوچ;
		data->undo_file = پوچ;
	}

	اگر (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	اگر (data->real)
		undo_err_handler_init(data->real);

	اگر (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		اگر (retval)
			برو cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	اگر (retval)
		برو cleanup;

	*channel = io;
	اگر (undo_fd >= 0)
		close(undo_fd);
	بازگشت retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	اگر (undo_fd >= 0)
		close(undo_fd);
	اگر (data && data->undo_file)
		io_channel_close(data->undo_file);
	اگر (data && data->tdb_file)
		آزاد(data->tdb_file);
	اگر (data && data->real)
		io_channel_close(data->real);
	اگر (data)
		ext2fs_free_mem(&data);
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (io)
		ext2fs_free_mem(&io);
	بازگشت retval;
}

ایستا errcode_t undo_close(io_channel channel)
{
	ساختار undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		بازگشت 0;
	/* Before closing write the file system identity */
	اگر (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	اگر (data->real)
		retval = io_channel_close(data->real);
	اگر (data->tdb_file)
		آزاد(data->tdb_file);
	اگر (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	اگر (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	اگر (err)
		بازگشت err;
	بازگشت retval;
}

ایستا errcode_t undo_set_blksize(io_channel channel, صحیح blksize)
{
	ساختار undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	اگر (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	بازگشت retval;
}

ایستا errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       صحیح شمار, تهی *buf)
{
	errcode_t	retval = 0;
	ساختار undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_read_blk64(data->real, block, شمار, buf);

	بازگشت retval;
}

ایستا errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	بازگشت undo_read_blk64(channel, block, شمار, buf);
}

ایستا errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				صحیح شمار, ثابت تهی *buf)
{
	ساختار undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, شمار);
	اگر (retval)
		 بازگشت retval;
	اگر (data->real)
		retval = io_channel_write_blk64(data->real, block, شمار, buf);

	بازگشت retval;
}

ایستا errcode_t undo_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *buf)
{
	بازگشت undo_write_blk64(channel, block, شمار, buf);
}

ایستا errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 صحیح size, ثابت تهی *buf)
{
	ساختار undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_رقم, شمار;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_رقم = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	شمار = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_رقم, شمار);
	اگر (retval)
		بازگشت retval;
	اگر (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	بازگشت retval;
}

ایستا errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار undo_private_data *data;
	errcode_t	retval = 0;
	صحیح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (شمار > INT_MAX)
		بازگشت EXT2_ET_UNIMPLEMENTED;
	icount = شمار;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	اگر (retval)
		بازگشت retval;
	اگر (data->real)
		retval = io_channel_discard(data->real, block, شمار);

	بازگشت retval;
}

ایستا errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long شمار)
{
	ساختار undo_private_data *data;
	errcode_t	retval = 0;
	صحیح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (شمار > INT_MAX)
		بازگشت EXT2_ET_UNIMPLEMENTED;
	icount = شمار;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	اگر (retval)
		بازگشت retval;
	اگر (data->real)
		retval = io_channel_zeroout(data->real, block, شمار);

	بازگشت retval;
}

ایستا errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long شمار)
{
	ساختار undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_cache_readahead(data->real, block, شمار);

	بازگشت retval;
}

/*
 * Flush data buffers to disk.
 */
ایستا errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ساختار undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (data->real)
		retval = io_channel_flush(data->real);

	بازگشت retval;
}

ایستا errcode_t undo_set_option(io_channel channel, ثابت نویسه *option,
				 ثابت نویسه *arg)
{
	errcode_t	retval = 0;
	ساختار undo_private_data *data;
	unsigned long tmp;
	نویسه *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!strcmp(option, "tdb_data_size")) {
		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		اگر (*end)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		اگر (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		اگر (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		بازگشت 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	اگر (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	اگر (!retval && !strcmp(option, "offset")) {
		اگر (!arg)
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		اگر (*end)
			بازگشت EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	بازگشت retval;
}

ایستا errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ساختار undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ساختار undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	بازگشت retval;
}

ایستا ساختار struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.set_blksize	= undo_set_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.set_option	= undo_set_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
