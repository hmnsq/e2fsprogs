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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#ifdef __linux__
#অন্তর্ভাব <sys/utsname.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#যদি HAVE_SYS_RESOURCE_H
#অন্তর্ভাব <sys/resource.h>
#endif
#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#অন্যথা
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  কুরু {printf(f, ## a); fflush(stdout); } যাবত্ (0)
#অন্যথা
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) প্রত্যাগম (code)
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

সংরচনা undo_header {
	বর্ণ magic[8];		/* "E2UNDO02" */
	__le64 অঙ্ক_চাবিগুলি;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 চাবি_offset;	/* where do the key/data block chunks start? */
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

সংরচনা undo_চাবি {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

সংরচনা undo_চাবি_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#যদি __STDC_VERSION__ >= 199901L
	সংরচনা undo_চাবি চাবিগুলি[];		/* keys, which come immediately after */
#অন্যথা
	সংরচনা undo_চাবি চাবিগুলি[0];	/* keys, which come immediately after */
#endif
};

সংরচনা undo_private_data {
	পূর্ণাঙ্ক	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_অঙ্ক;			/* next free block */
	blk64_t চাবি_blk_অঙ্ক;			/* current key block location */
	blk64_t super_blk_অঙ্ক;			/* superblock location */
	blk64_t first_চাবি_blk;			/* first key block location */
	সংরচনা undo_চাবি_block *keyb;
	size_t অঙ্ক_চাবিগুলি, চাবিগুলি_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	পূর্ণাঙ্ক tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	সংরচনা struct_ext2_filsys fake_fs;
	বর্ণ *tdb_file;
	সংরচনা undo_header hdr;
};
#define চাবিগুলি_PER_BLOCK(d) (((d)->tdb_data_size / পরিমাণ(সংরচনা undo_চাবি)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

স্থির অন্তঃস্থ শূন্য e2undo_set_feature_fs_offset(সংরচনা undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

স্থির অন্তঃস্থ শূন্য e2undo_clear_feature_fs_offset(সংরচনা undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

স্থির io_manager undo_io_backing_manager;
স্থির বর্ণ *tdb_file;
স্থির পূর্ণাঙ্ক actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	প্রত্যাগম 0;
}

errcode_t set_undo_io_backup_file(বর্ণ *file_name)
{
	tdb_file = strdup(file_name);

	যদি (tdb_file == রিক্ত) {
		প্রত্যাগম EXT2_ET_NO_MEMORY;
	}

	প্রত্যাগম 0;
}

স্থির errcode_t write_undo_indexes(সংরচনা undo_private_data *data, পূর্ণাঙ্ক flush)
{
	errcode_t retval;
	সংরচনা ext2_super_block super;
	io_channel channel;
	পূর্ণাঙ্ক block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	যদি (data->চাবিগুলি_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned বর্ণ *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->চাবি_blk_অঙ্ক);
		retval = io_channel_write_blk64(data->undo_file,
						data->চাবি_blk_অঙ্ক,
						1, data->keyb);
		যদি (retval)
			প্রত্যাগম retval;
		/* Move on to the next key block if it's full. */
		যদি (data->চাবিগুলি_in_block == চাবিগুলি_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->চাবিগুলি_in_block = 0;
			data->চাবি_blk_অঙ্ক = data->undo_blk_অঙ্ক;
			data->undo_blk_অঙ্ক++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	যদি (retval)
		গচ্ছ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned বর্ণ *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, পরিমাণ(data->hdr.magic));
	data->hdr.অঙ্ক_চাবিগুলি = ext2fs_cpu_to_le64(data->অঙ্ক_চাবিগুলি);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_অঙ্ক);
	data->hdr.চাবি_offset = ext2fs_cpu_to_le64(data->first_চাবি_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	যদি (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	অন্যথা
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned বর্ণ *)&data->hdr,
				   পরিমাণ(data->hdr) -
				   পরিমাণ(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(পূর্ণাঙ্ক)পরিমাণ(data->hdr),
					&data->hdr);
	যদি (retval)
		গচ্ছ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_অঙ্ক);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_অঙ্ক,
					-SUPERBLOCK_SIZE, &super);
	যদি (retval)
		গচ্ছ err_out;

	যদি (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	প্রত্যাগম retval;
}

স্থির errcode_t undo_setup_tdb(সংরচনা undo_private_data *data)
{
	পূর্ণাঙ্ক i;
	errcode_t retval;

	যদি (data->tdb_written == 1)
		প্রত্যাগম 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, পরিমাণ(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	যদি (retval)
		প্রত্যাগম retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	যদি (retval)
		প্রত্যাগম retval;
	data->চাবি_blk_অঙ্ক = data->first_চাবি_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", চাবিগুলি_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	প্রতি (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		যদি (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		যদি (retval)
			প্রত্যাগম retval;
		retval = io_channel_flush(data->undo_file);
		যদি (retval)
			প্রত্যাগম retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	প্রত্যাগম 0;
}

স্থির errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, পূর্ণাঙ্ক গুনতি)

{
	পূর্ণাঙ্ক size, sz;
	unsigned long long block_অঙ্ক, backing_blk_অঙ্ক;
	errcode_t retval = 0;
	ext2_loff_t offset;
	সংরচনা undo_private_data *data;
	unsigned বর্ণ *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	সংরচনা undo_চাবি *চাবি;
	__u32 blk_crc;

	data = (সংরচনা undo_private_data *) channel->private_data;

	যদি (data->undo_file == রিক্ত) {
		/*
		 * Transaction database not initialized
		 */
		প্রত্যাগম 0;
	}

	যদি (গুনতি == 1)
		size = channel->block_size;
	অন্যথা {
		যদি (গুনতি < 0)
			size = -গুনতি;
		অন্যথা
			size = গুনতি * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	যদি (retval)
		প্রত্যাগম retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_অঙ্ক = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	যাবত্ (block_অঙ্ক <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		যদি (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_অঙ্ক)) {
			/* Try the next block */
			block_অঙ্ক++;
			অনুবর্ত;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_অঙ্ক);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_অঙ্ক * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_অঙ্ক = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		যদি (retval) {
			প্রত্যাগম retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		যদি ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		অন্যথা
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_অঙ্ক,
					     sz, read_ptr);
		যদি (retval) {
			যদি (retval != EXT2_ET_SHORT_READ) {
				মুক্ত(read_ptr);
				প্রত্যাগম retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} অন্যথা {
			data_size = data->tdb_data_size;
		}
		যদি (data_size == 0) {
			মুক্ত(read_ptr);
			block_অঙ্ক++;
			অনুবর্ত;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_অঙ্ক, block, data->tdb_data_size);
		যদি ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		অন্যথা
			sz = -data_size;;
		/* extend this key? */
		যদি (data->চাবিগুলি_in_block) {
			চাবি = data->keyb->চাবিগুলি + data->চাবিগুলি_in_block - 1;
			keysz = ext2fs_le32_to_cpu(চাবি->size);
		} অন্যথা {
			চাবি = রিক্ত;
			keysz = 0;
		}
		যদি (চাবি != রিক্ত &&
		    (ext2fs_le64_to_cpu(চাবি->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_অঙ্ক &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(চাবি->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			চাবি->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			চাবি->size = ext2fs_cpu_to_le32(keysz + data_size);
		} অন্যথা {
			data->অঙ্ক_চাবিগুলি++;
			চাবি = data->keyb->চাবিগুলি + data->চাবিগুলি_in_block;
			data->চাবিগুলি_in_block++;
			চাবি->fsblk = ext2fs_cpu_to_le64(backing_blk_অঙ্ক);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			চাবি->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			চাবি->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_অঙ্ক,
		       data->undo_blk_অঙ্ক,
		       sz, data->অঙ্ক_চাবিগুলি - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_অঙ্ক, sz, read_ptr);
		যদি (retval) {
			মুক্ত(read_ptr);
			প্রত্যাগম retval;
		}
		data->undo_blk_অঙ্ক++;
		মুক্ত(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		যদি (retval)
			প্রত্যাগম retval;

		/* Next block */
		block_অঙ্ক++;
	}

	প্রত্যাগম retval;
}

স্থির errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    পূর্ণাঙ্ক গুনতি ATTR((unused)),
				    শূন্য *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    পূর্ণাঙ্ক actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	প্রত্যাগম error;
}

স্থির শূন্য undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

স্থির পূর্ণাঙ্ক check_filesystem(সংরচনা undo_header *hdr, io_channel undo_file,
			    unsigned পূর্ণাঙ্ক blocksize, blk64_t super_block,
			    io_channel channel)
{
	সংরচনা ext2_super_block super, *sb;
	বর্ণ *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	যদি (retval)
		প্রত্যাগম retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	যদি (retval)
		প্রত্যাগম retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	যদি (retval)
		গচ্ছ out;
	sb = (সংরচনা ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	যদি (memcmp(&super, buf, পরিমাণ(super))) {
		retval = -1;
		গচ্ছ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned বর্ণ *)buf, SUPERBLOCK_SIZE);
	যদি (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		গচ্ছ out;
	}

out:
	ext2fs_free_mem(&buf);
	প্রত্যাগম retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
স্থির errcode_t try_reopen_undo_file(পূর্ণাঙ্ক undo_fd,
				      সংরচনা undo_private_data *data)
{
	সংরচনা undo_header hdr;
	সংরচনা undo_চাবি *dkey;
	ext2fs_struct_stat statbuf;
	unsigned পূর্ণাঙ্ক blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t অঙ্ক_চাবিগুলি, চাবিগুলি_per_block, i;
	__u32 hdr_crc, চাবি_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	যদি (retval)
		গচ্ছ bad_file;
	যদি (statbuf.st_size == 0)
		গচ্ছ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(পূর্ণাঙ্ক)পরিমাণ(hdr),
				       &hdr);
	যদি (retval)
		গচ্ছ bad_file;

	যদি (memcmp(hdr.magic, E2UNDO_MAGIC,
		    পরিমাণ(hdr.magic)))
		গচ্ছ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned বর্ণ *)&hdr,
				   পরিমাণ(সংরচনা undo_header) -
				   পরিমাণ(__u32));
	যদি (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		গচ্ছ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	যদি (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		গচ্ছ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	অঙ্ক_চাবিগুলি = ext2fs_le64_to_cpu(hdr.অঙ্ক_চাবিগুলি);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	যদি (hdr.f_incompat || hdr.f_rocompat)
		গচ্ছ bad_file;

	/* Superblock matches this FS? */
	যদি (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		গচ্ছ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	যদি (retval)
		গচ্ছ bad_file;
	data->অঙ্ক_চাবিগুলি = অঙ্ক_চাবিগুলি;
	data->super_blk_অঙ্ক = super_block;
	data->first_চাবি_blk = ext2fs_le64_to_cpu(hdr.চাবি_offset);

	/* load the written block map */
	চাবিগুলি_per_block = চাবিগুলি_PER_BLOCK(data);
	lblk = data->first_চাবি_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   অঙ্ক_চাবিগুলি, চাবিগুলি_per_block, blocksize);
	প্রতি (i = 0; i < অঙ্ক_চাবিগুলি; i += চাবিগুলি_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->চাবি_blk_অঙ্ক = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		যদি (retval)
			গচ্ছ bad_চাবি_replay;

		/* check keys */
		যদি (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			গচ্ছ bad_চাবি_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		চাবি_crc = ext2fs_crc32c_le(~0, (unsigned বর্ণ *)data->keyb,
					   blocksize);
		যদি (ext2fs_le32_to_cpu(crc) != চাবি_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			গচ্ছ bad_চাবি_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->অঙ্ক_চাবিগুলি - i;
		যদি (max_j > চাবিগুলি_per_block)
			max_j = চাবিগুলি_per_block;
		প্রতি (j = 0, dkey = data->keyb->চাবিগুলি;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_অঙ্ক = lblk;
			data->চাবিগুলি_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->চাবি_blk_অঙ্ক, data->undo_blk_অঙ্ক, data->অঙ্ক_চাবিগুলি,
		   data->চাবিগুলি_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	প্রত্যাগম retval;

bad_চাবি_replay:
	data->চাবি_blk_অঙ্ক = data->undo_blk_অঙ্ক = 0;
	data->চাবিগুলি_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	গচ্ছ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	প্রত্যাগম retval;
}

স্থির শূন্য undo_atexit(শূন্য *p)
{
	সংরচনা undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

স্থির errcode_t undo_open(ধ্রুব বর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	io_channel	io = রিক্ত;
	সংরচনা undo_private_data *data = রিক্ত;
	পূর্ণাঙ্ক		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	যদি (name == 0)
		প্রত্যাগম EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা struct_io_channel), &io);
	যদি (retval)
		গচ্ছ cleanup;
	memset(io, 0, পরিমাণ(সংরচনা struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা undo_private_data), &data);
	যদি (retval)
		গচ্ছ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		গচ্ছ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, পরিমাণ(সংরচনা undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_অঙ্ক = 1;
	data->first_চাবি_blk = 2;
	data->undo_blk_অঙ্ক = 3;

	যদি (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		যদি (retval)
			গচ্ছ cleanup;

		data->tdb_file = strdup(tdb_file);
		যদি (data->tdb_file == রিক্ত)
			গচ্ছ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		যদি (undo_fd < 0)
			গচ্ছ cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা {
		data->real = রিক্ত;
		data->undo_file = রিক্ত;
	}

	যদি (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	যদি (data->real)
		undo_err_handler_init(data->real);

	যদি (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		যদি (retval)
			গচ্ছ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	যদি (retval)
		গচ্ছ cleanup;

	*channel = io;
	যদি (undo_fd >= 0)
		close(undo_fd);
	প্রত্যাগম retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	যদি (undo_fd >= 0)
		close(undo_fd);
	যদি (data && data->undo_file)
		io_channel_close(data->undo_file);
	যদি (data && data->tdb_file)
		মুক্ত(data->tdb_file);
	যদি (data && data->real)
		io_channel_close(data->real);
	যদি (data)
		ext2fs_free_mem(&data);
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (io)
		ext2fs_free_mem(&io);
	প্রত্যাগম retval;
}

স্থির errcode_t undo_close(io_channel channel)
{
	সংরচনা undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		প্রত্যাগম 0;
	/* Before closing write the file system identity */
	যদি (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	যদি (data->real)
		retval = io_channel_close(data->real);
	যদি (data->tdb_file)
		মুক্ত(data->tdb_file);
	যদি (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	যদি (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	যদি (err)
		প্রত্যাগম err;
	প্রত্যাগম retval;
}

স্থির errcode_t undo_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	সংরচনা undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;

	যদি (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	যদি (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	প্রত্যাগম retval;
}

স্থির errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	errcode_t	retval = 0;
	সংরচনা undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_read_blk64(data->real, block, গুনতি, buf);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	প্রত্যাগম undo_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf)
{
	সংরচনা undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, গুনতি);
	যদি (retval)
		 প্রত্যাগম retval;
	যদি (data->real)
		retval = io_channel_write_blk64(data->real, block, গুনতি, buf);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf)
{
	প্রত্যাগম undo_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণাঙ্ক size, ধ্রুব শূন্য *buf)
{
	সংরচনা undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_অঙ্ক, গুনতি;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_অঙ্ক = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	গুনতি = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_অঙ্ক, গুনতি);
	যদি (retval)
		প্রত্যাগম retval;
	যদি (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা undo_private_data *data;
	errcode_t	retval = 0;
	পূর্ণাঙ্ক icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (গুনতি > INT_MAX)
		প্রত্যাগম EXT2_ET_UNIMPLEMENTED;
	icount = গুনতি;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	যদি (retval)
		প্রত্যাগম retval;
	যদি (data->real)
		retval = io_channel_discard(data->real, block, গুনতি);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	সংরচনা undo_private_data *data;
	errcode_t	retval = 0;
	পূর্ণাঙ্ক icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (গুনতি > INT_MAX)
		প্রত্যাগম EXT2_ET_UNIMPLEMENTED;
	icount = গুনতি;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	যদি (retval)
		প্রত্যাগম retval;
	যদি (data->real)
		retval = io_channel_zeroout(data->real, block, গুনতি);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	সংরচনা undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_cache_readahead(data->real, block, গুনতি);

	প্রত্যাগম retval;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	সংরচনা undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (data->real)
		retval = io_channel_flush(data->real);

	প্রত্যাগম retval;
}

স্থির errcode_t undo_set_option(io_channel channel, ধ্রুব বর্ণ *option,
				 ধ্রুব বর্ণ *arg)
{
	errcode_t	retval = 0;
	সংরচনা undo_private_data *data;
	unsigned long tmp;
	বর্ণ *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!strcmp(option, "tdb_data_size")) {
		যদি (!arg)
			প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		যদি (*end)
			প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;
		যদি (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;
		যদি (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		প্রত্যাগম 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	যদি (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	যদি (!retval && !strcmp(option, "offset")) {
		যদি (!arg)
			প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		যদি (*end)
			প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	প্রত্যাগম retval;
}

স্থির errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	সংরচনা undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (সংরচনা undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	প্রত্যাগম retval;
}

স্থির সংরচনা struct_io_manager struct_undo_manager = {
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
