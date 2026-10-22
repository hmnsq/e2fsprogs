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
# define dbg_printf(f, a...)  করো {printf(f, ## a); fflush(stdout); } যখন (0)
#অন্যথা
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)
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

#define E2_UNDO_MIN_BLOCK_আকার	1024	/* undo blocks are no less than 1KB */
#define E2_UNDO_MAX_BLOCK_আকার	1048576	/* undo blocks are no more than 1MB */

কাঠামো undo_header {
	অক্ষর magic[8];		/* "E2UNDO02" */
	__le64 অঙ্ক_চাবিগুলি;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 চাবি_offset;	/* where do the key/data block chunks start? */
	__le32 block_আকার;	/* block size of the undo file */
	__le32 fs_block_আকার;	/* block size of the target device */
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

কাঠামো undo_চাবি {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 আকার_2;		/* how many bytes in this block? */
};

কাঠামো undo_চাবি_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#যদি __STDC_VERSION__ >= 199901L
	কাঠামো undo_চাবি চাবিগুলি[];		/* keys, which come immediately after */
#অন্যথা
	কাঠামো undo_চাবি চাবিগুলি[0];	/* keys, which come immediately after */
#endif
};

কাঠামো undo_private_তথ্য {
	পূর্ণসংখ্যা	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_অঙ্ক;			/* next free block */
	blk64_t চাবি_blk_অঙ্ক;			/* current key block location */
	blk64_t super_blk_অঙ্ক;			/* superblock location */
	blk64_t first_চাবি_blk;			/* first key block location */
	কাঠামো undo_চাবি_block *keyb;
	size_t অঙ্ক_চাবিগুলি, চাবিগুলি_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_তথ্য_আকার;
	পূর্ণসংখ্যা tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	কাঠামো struct_ext2_filsys fake_fs;
	অক্ষর *tdb_file;
	কাঠামো undo_header hdr;
};
#define চাবিগুলি_PER_BLOCK(d) (((d)->tdb_তথ্য_আকার / আকার(কাঠামো undo_চাবি)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

স্থির অন্তঃরেখ শূন্য e2undo_set_feature_fs_offset(কাঠামো undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

স্থির অন্তঃরেখ শূন্য e2undo_clear_feature_fs_offset(কাঠামো undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

স্থির io_manager undo_io_backing_manager;
স্থির অক্ষর *tdb_file;
স্থির পূর্ণসংখ্যা actual_আকার;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ফেরত 0;
}

errcode_t set_undo_io_backup_file(অক্ষর *file_name)
{
	tdb_file = strdup(file_name);

	যদি (tdb_file == ফাঁকা) {
		ফেরত EXT2_ET_NO_MEMORY;
	}

	ফেরত 0;
}

স্থির errcode_t write_undo_indexes(কাঠামো undo_private_তথ্য *তথ্য, পূর্ণসংখ্যা flush)
{
	errcode_t retval;
	কাঠামো ext2_super_block super;
	io_channel channel;
	পূর্ণসংখ্যা block_আকার;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	যদি (তথ্য->চাবিগুলি_in_block) {
		তথ্য->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		তথ্য->keyb->crc = 0;
		তথ্য->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned অক্ষর *)তথ্য->keyb,
					 তথ্য->tdb_তথ্য_আকার));
		dbg_printf("Writing keyblock to blk %llu\n", তথ্য->চাবি_blk_অঙ্ক);
		retval = io_channel_write_blk64(তথ্য->undo_file,
						তথ্য->চাবি_blk_অঙ্ক,
						1, তথ্য->keyb);
		যদি (retval)
			ফেরত retval;
		/* Move on to the next key block if it's full. */
		যদি (তথ্য->চাবিগুলি_in_block == চাবিগুলি_PER_BLOCK(তথ্য)) {
			memset(তথ্য->keyb, 0, তথ্য->tdb_তথ্য_আকার);
			তথ্য->চাবিগুলি_in_block = 0;
			তথ্য->চাবি_blk_অঙ্ক = তথ্য->undo_blk_অঙ্ক;
			তথ্য->undo_blk_অঙ্ক++;
		}
	}

	/* Prepare superblock for write */
	channel = তথ্য->real;
	block_আকার = channel->block_আকার;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_আকার, &super);
	যদি (retval)
		যাও err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned অক্ষর *)&super, SUPERBLOCK_আকার);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(তথ্য->hdr.magic, E2UNDO_MAGIC, আকার(তথ্য->hdr.magic));
	তথ্য->hdr.অঙ্ক_চাবিগুলি = ext2fs_cpu_to_le64(তথ্য->অঙ্ক_চাবিগুলি);
	তথ্য->hdr.super_offset = ext2fs_cpu_to_le64(তথ্য->super_blk_অঙ্ক);
	তথ্য->hdr.চাবি_offset = ext2fs_cpu_to_le64(তথ্য->first_চাবি_blk);
	তথ্য->hdr.fs_block_আকার = ext2fs_cpu_to_le32(block_আকার);
	তথ্য->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	তথ্য->hdr.fs_offset = ext2fs_cpu_to_le64(তথ্য->offset);
	যদি (তথ্য->offset)
		e2undo_set_feature_fs_offset(&তথ্য->hdr);
	অন্যথা
		e2undo_clear_feature_fs_offset(&তথ্য->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned অক্ষর *)&তথ্য->hdr,
				   আকার(তথ্য->hdr) -
				   আকার(তথ্য->hdr.header_crc));
	তথ্য->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(তথ্য->undo_file, 0,
					-(পূর্ণসংখ্যা)আকার(তথ্য->hdr),
					&তথ্য->hdr);
	যদি (retval)
		যাও err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", তথ্য->super_blk_অঙ্ক);
	retval = io_channel_write_blk64(তথ্য->undo_file, তথ্য->super_blk_অঙ্ক,
					-SUPERBLOCK_আকার, &super);
	যদি (retval)
		যাও err_out;

	যদি (flush)
		retval = io_channel_flush(তথ্য->undo_file);
err_out:
	io_channel_set_blksize(channel, block_আকার);
	ফেরত retval;
}

স্থির errcode_t undo_setup_tdb(কাঠামো undo_private_তথ্য *তথ্য)
{
	পূর্ণসংখ্যা i;
	errcode_t retval;

	যদি (তথ্য->tdb_written == 1)
		ফেরত 0;

	তথ্য->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&তথ্য->fake_fs, 0, আকার(তথ্য->fake_fs));
	তথ্য->fake_fs.blocksize = তথ্য->tdb_তথ্য_আকার;
	retval = ext2fs_alloc_generic_bmap(&তথ্য->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &তথ্য->written_block_map);
	যদি (retval)
		ফেরত retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(তথ্য->tdb_তথ্য_আকার, &তথ্য->keyb);
	যদি (retval)
		ফেরত retval;
	তথ্য->চাবি_blk_অঙ্ক = তথ্য->first_চাবি_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", তথ্য->tdb_তথ্য_আকার);
	dbg_printf("Keys per block %llu\n", চাবিগুলি_PER_BLOCK(তথ্য));
	তথ্য->hdr.block_আকার = ext2fs_cpu_to_le32(তথ্য->tdb_তথ্য_আকার);
	io_channel_set_blksize(তথ্য->undo_file, তথ্য->tdb_তথ্য_আকার);

	/* Ensure that we have space for header blocks */
	জন্য (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(তথ্য->undo_file, i, 1,
					       তথ্য->keyb);
		যদি (retval)
			memset(তথ্য->keyb, 0, তথ্য->tdb_তথ্য_আকার);
		retval = io_channel_write_blk64(তথ্য->undo_file, i, 1,
						তথ্য->keyb);
		যদি (retval)
			ফেরত retval;
		retval = io_channel_flush(তথ্য->undo_file);
		যদি (retval)
			ফেরত retval;
	}
	memset(তথ্য->keyb, 0, তথ্য->tdb_তথ্য_আকার);
	ফেরত 0;
}

স্থির errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, পূর্ণসংখ্যা গুনতি)

{
	পূর্ণসংখ্যা আকার_2, sz;
	unsigned long long block_অঙ্ক, backing_blk_অঙ্ক;
	errcode_t retval = 0;
	ext2_loff_t offset;
	কাঠামো undo_private_তথ্য *তথ্য;
	unsigned অক্ষর *read_ptr;
	unsigned long long end_block;
	unsigned long long তথ্য_আকার;
	কাঠামো undo_চাবি *চাবি;
	__u32 blk_crc;

	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;

	যদি (তথ্য->undo_file == ফাঁকা) {
		/*
		 * Transaction database not initialized
		 */
		ফেরত 0;
	}

	যদি (গুনতি == 1)
		আকার_2 = channel->block_আকার;
	অন্যথা {
		যদি (গুনতি < 0)
			আকার_2 = -গুনতি;
		অন্যথা
			আকার_2 = গুনতি * channel->block_আকার;
	}

	retval = undo_setup_tdb(তথ্য);
	যদি (retval)
		ফেরত retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_আকার) + তথ্য->offset ;
	block_অঙ্ক = offset / তথ্য->tdb_তথ্য_আকার;
	end_block = (offset + আকার_2 - 1) / তথ্য->tdb_তথ্য_আকার;

	যখন (block_অঙ্ক <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		যদি (ext2fs_test_block_bitmap2(তথ্য->written_block_map,
						   block_অঙ্ক)) {
			/* Try the next block */
			block_অঙ্ক++;
			চালু;
		}
		ext2fs_mark_block_bitmap2(তথ্য->written_block_map, block_অঙ্ক);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_অঙ্ক * তথ্য->tdb_তথ্য_আকার +
				(তথ্য->offset % তথ্য->tdb_তথ্য_আকার);
		backing_blk_অঙ্ক = (offset - তথ্য->offset) / channel->block_আকার;

		retval = ext2fs_get_mem(তথ্য->tdb_তথ্য_আকার, &read_ptr);
		যদি (retval) {
			ফেরত retval;
		}

		memset(read_ptr, 0, তথ্য->tdb_তথ্য_আকার);
		actual_আকার = 0;
		যদি ((তথ্য->tdb_তথ্য_আকার % channel->block_আকার) == 0)
			sz = তথ্য->tdb_তথ্য_আকার / channel->block_আকার;
		অন্যথা
			sz = -তথ্য->tdb_তথ্য_আকার;
		retval = io_channel_read_blk64(তথ্য->real, backing_blk_অঙ্ক,
					     sz, read_ptr);
		যদি (retval) {
			যদি (retval != EXT2_ET_SHORT_READ) {
				মুক্ত(read_ptr);
				ফেরত retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			তথ্য_আকার = actual_আকার;
		} অন্যথা {
			তথ্য_আকার = তথ্য->tdb_তথ্য_আকার;
		}
		যদি (তথ্য_আকার == 0) {
			মুক্ত(read_ptr);
			block_অঙ্ক++;
			চালু;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       তথ্য_আকার, backing_blk_অঙ্ক, block, তথ্য->tdb_তথ্য_আকার);
		যদি ((তথ্য_আকার % তথ্য->undo_file->block_আকার) == 0)
			sz = তথ্য_আকার / তথ্য->undo_file->block_আকার;
		অন্যথা
			sz = -তথ্য_আকার;;
		/* extend this key? */
		যদি (তথ্য->চাবিগুলি_in_block) {
			চাবি = তথ্য->keyb->চাবিগুলি + তথ্য->চাবিগুলি_in_block - 1;
			keysz = ext2fs_le32_to_cpu(চাবি->আকার_2);
		} অন্যথা {
			চাবি = ফাঁকা;
			keysz = 0;
		}
		যদি (চাবি != ফাঁকা &&
		    (ext2fs_le64_to_cpu(চাবি->fsblk) * channel->block_আকার +
		     channel->block_আকার - 1 +
		     keysz) / channel->block_আকার == backing_blk_অঙ্ক &&
		    E2UNDO_MAX_EXTENT_BLOCKS * তথ্য->tdb_তথ্য_আকার >
		    keysz + তথ্য_আকার) {
			blk_crc = ext2fs_le32_to_cpu(চাবি->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, তথ্য_আকার);
			চাবি->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			চাবি->আকার_2 = ext2fs_cpu_to_le32(keysz + তথ্য_আকার);
		} অন্যথা {
			তথ্য->অঙ্ক_চাবিগুলি++;
			চাবি = তথ্য->keyb->চাবিগুলি + তথ্য->চাবিগুলি_in_block;
			তথ্য->চাবিগুলি_in_block++;
			চাবি->fsblk = ext2fs_cpu_to_le64(backing_blk_অঙ্ক);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, তথ্য_আকার);
			চাবি->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			চাবি->আকার_2 = ext2fs_cpu_to_le32(তথ্য_আকার);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_অঙ্ক,
		       তথ্য->undo_blk_অঙ্ক,
		       sz, তথ্য->অঙ্ক_চাবিগুলি - 1);
		retval = io_channel_write_blk64(তথ্য->undo_file,
					তথ্য->undo_blk_অঙ্ক, sz, read_ptr);
		যদি (retval) {
			মুক্ত(read_ptr);
			ফেরত retval;
		}
		তথ্য->undo_blk_অঙ্ক++;
		মুক্ত(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(তথ্য, 0);
		যদি (retval)
			ফেরত retval;

		/* Next block */
		block_অঙ্ক++;
	}

	ফেরত retval;
}

স্থির errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    পূর্ণসংখ্যা গুনতি ATTR((unused)),
				    শূন্য *তথ্য ATTR((unused)),
				    size_t আকার_2 ATTR((unused)),
				    পূর্ণসংখ্যা actual,
				    errcode_t error ATTR((unused)))
{
	actual_আকার = actual;
	ফেরত error;
}

স্থির শূন্য undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

স্থির পূর্ণসংখ্যা check_filesystem(কাঠামো undo_header *hdr, io_channel undo_file,
			    unsigned পূর্ণসংখ্যা blocksize, blk64_t super_block,
			    io_channel channel)
{
	কাঠামো ext2_super_block super, *sb;
	অক্ষর *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_আকার, &super);
	যদি (retval)
		ফেরত retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	যদি (retval)
		ফেরত retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_আকার, buf);
	যদি (retval)
		যাও out;
	sb = (কাঠামো ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	যদি (memcmp(&super, buf, আকার(super))) {
		retval = -1;
		যাও out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned অক্ষর *)buf, SUPERBLOCK_আকার);
	যদি (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		যাও out;
	}

out:
	ext2fs_free_mem(&buf);
	ফেরত retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
স্থির errcode_t try_reopen_undo_file(পূর্ণসংখ্যা undo_fd,
				      কাঠামো undo_private_তথ্য *তথ্য)
{
	কাঠামো undo_header hdr;
	কাঠামো undo_চাবি *dkey;
	ext2fs_struct_stat statbuf;
	unsigned পূর্ণসংখ্যা blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t অঙ্ক_চাবিগুলি, চাবিগুলি_per_block, i;
	__u32 hdr_crc, চাবি_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	যদি (retval)
		যাও bad_file;
	যদি (statbuf.st_আকার == 0)
		যাও out;

	/* check the file header */
	retval = io_channel_read_blk64(তথ্য->undo_file, 0, -(পূর্ণসংখ্যা)আকার(hdr),
				       &hdr);
	যদি (retval)
		যাও bad_file;

	যদি (memcmp(hdr.magic, E2UNDO_MAGIC,
		    আকার(hdr.magic)))
		যাও bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned অক্ষর *)&hdr,
				   আকার(কাঠামো undo_header) -
				   আকার(__u32));
	যদি (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		যাও bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_আকার);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_আকার);
	যদি (blocksize > E2_UNDO_MAX_BLOCK_আকার ||
	    blocksize < E2_UNDO_MIN_BLOCK_আকার ||
	    !blocksize || !fs_blocksize)
		যাও bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	অঙ্ক_চাবিগুলি = ext2fs_le64_to_cpu(hdr.অঙ্ক_চাবিগুলি);
	io_channel_set_blksize(তথ্য->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	যদি (hdr.f_incompat || hdr.f_rocompat)
		যাও bad_file;

	/* Superblock matches this FS? */
	যদি (check_filesystem(&hdr, তথ্য->undo_file, blocksize, super_block,
			     তথ্য->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		যাও out;
	}

	/* Try to set ourselves up */
	তথ্য->tdb_তথ্য_আকার = blocksize;
	retval = undo_setup_tdb(তথ্য);
	যদি (retval)
		যাও bad_file;
	তথ্য->অঙ্ক_চাবিগুলি = অঙ্ক_চাবিগুলি;
	তথ্য->super_blk_অঙ্ক = super_block;
	তথ্য->first_চাবি_blk = ext2fs_le64_to_cpu(hdr.চাবি_offset);

	/* load the written block map */
	চাবিগুলি_per_block = চাবিগুলি_PER_BLOCK(তথ্য);
	lblk = তথ্য->first_চাবি_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   অঙ্ক_চাবিগুলি, চাবিগুলি_per_block, blocksize);
	জন্য (i = 0; i < অঙ্ক_চাবিগুলি; i += চাবিগুলি_per_block) {
		size_t j, max_j;
		__le32 crc;

		তথ্য->চাবি_blk_অঙ্ক = lblk;
		retval = io_channel_read_blk64(তথ্য->undo_file,
					       lblk, 1, তথ্য->keyb);
		যদি (retval)
			যাও bad_চাবি_replay;

		/* check keys */
		যদি (ext2fs_le32_to_cpu(তথ্য->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			যাও bad_চাবি_replay;
		}
		crc = তথ্য->keyb->crc;
		তথ্য->keyb->crc = 0;
		চাবি_crc = ext2fs_crc32c_le(~0, (unsigned অক্ষর *)তথ্য->keyb,
					   blocksize);
		যদি (ext2fs_le32_to_cpu(crc) != চাবি_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			যাও bad_চাবি_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = তথ্য->অঙ্ক_চাবিগুলি - i;
		যদি (max_j > চাবিগুলি_per_block)
			max_j = চাবিগুলি_per_block;
		জন্য (j = 0, dkey = তথ্য->keyb->চাবিগুলি;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t আকার_2 = ext2fs_le32_to_cpu(dkey->আকার_2);

			ext2fs_mark_block_bitmap_range2(তথ্য->written_block_map,
					 undo_blk,
					(আকার_2 + blocksize - 1) / blocksize);
			lblk += (আকার_2 + blocksize - 1) / blocksize;
			তথ্য->undo_blk_অঙ্ক = lblk;
			তথ্য->চাবিগুলি_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   তথ্য->চাবি_blk_অঙ্ক, তথ্য->undo_blk_অঙ্ক, তথ্য->অঙ্ক_চাবিগুলি,
		   তথ্য->চাবিগুলি_in_block);

	তথ্য->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	তথ্য->hdr.f_compat = hdr.f_compat;
	তথ্য->hdr.f_incompat = hdr.f_incompat;
	তথ্য->hdr.f_rocompat = hdr.f_rocompat;
	ফেরত retval;

bad_চাবি_replay:
	তথ্য->চাবি_blk_অঙ্ক = তথ্য->undo_blk_অঙ্ক = 0;
	তথ্য->চাবিগুলি_in_block = 0;
	ext2fs_free_mem(&তথ্য->keyb);
	ext2fs_free_generic_bitmap(তথ্য->written_block_map);
	তথ্য->tdb_written = 0;
	যাও out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	ফেরত retval;
}

স্থির শূন্য undo_atexit(শূন্য *p)
{
	কাঠামো undo_private_তথ্য *তথ্য = p;
	errcode_t err;

	err = write_undo_indexes(তথ্য, 1);
	io_channel_close(তথ্য->undo_file);

	com_err(তথ্য->tdb_file, err, "while force-closing undo file");
}

স্থির errcode_t undo_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	io_channel	io = ফাঁকা;
	কাঠামো undo_private_তথ্য *তথ্য = ফাঁকা;
	পূর্ণসংখ্যা		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	যদি (name == 0)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(আকার(কাঠামো struct_io_channel), &io);
	যদি (retval)
		যাও cleanup;
	memset(io, 0, আকার(কাঠামো struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(আকার(কাঠামো undo_private_তথ্য), &তথ্য);
	যদি (retval)
		যাও cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	যদি (retval)
		যাও cleanup;

	strcpy(io->name, name);
	io->private_তথ্য = তথ্য;
	io->block_আকার = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(তথ্য, 0, আকার(কাঠামো undo_private_তথ্য));
	তথ্য->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	তথ্য->super_blk_অঙ্ক = 1;
	তথ্য->first_চাবি_blk = 2;
	তথ্য->undo_blk_অঙ্ক = 3;

	যদি (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &তথ্য->real);
		যদি (retval)
			যাও cleanup;

		তথ্য->tdb_file = strdup(tdb_file);
		যদি (তথ্য->tdb_file == ফাঁকা)
			যাও cleanup;
		undo_fd = ext2fs_open_file(তথ্য->tdb_file, O_RDWR | O_CREAT,
					   0600);
		যদি (undo_fd < 0)
			যাও cleanup;

		retval = undo_io_backing_manager->open(তথ্য->tdb_file,
						       IO_FLAG_RW,
						       &তথ্য->undo_file);
		যদি (retval)
			যাও cleanup;
	} অন্যথা {
		তথ্য->real = ফাঁকা;
		তথ্য->undo_file = ফাঁকা;
	}

	যদি (তথ্য->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (তথ্য->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	যদি (তথ্য->real)
		undo_err_handler_init(তথ্য->real);

	যদি (তথ্য->undo_file) {
		retval = try_reopen_undo_file(undo_fd, তথ্য);
		যদি (retval)
			যাও cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, তথ্য);
	যদি (retval)
		যাও cleanup;

	*channel = io;
	যদি (undo_fd >= 0)
		close(undo_fd);
	ফেরত retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, তথ্য);
	যদি (undo_fd >= 0)
		close(undo_fd);
	যদি (তথ্য && তথ্য->undo_file)
		io_channel_close(তথ্য->undo_file);
	যদি (তথ্য && তথ্য->tdb_file)
		মুক্ত(তথ্য->tdb_file);
	যদি (তথ্য && তথ্য->real)
		io_channel_close(তথ্য->real);
	যদি (তথ্য)
		ext2fs_free_mem(&তথ্য);
	যদি (io && io->name)
		ext2fs_free_mem(&io->name);
	যদি (io)
		ext2fs_free_mem(&io);
	ফেরত retval;
}

স্থির errcode_t undo_close(io_channel channel)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (--channel->refcount > 0)
		ফেরত 0;
	/* Before closing write the file system identity */
	যদি (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		তথ্য->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(তথ্য, 1);
	ext2fs_remove_exit_fn(undo_atexit, তথ্য);
	যদি (তথ্য->real)
		retval = io_channel_close(তথ্য->real);
	যদি (তথ্য->tdb_file)
		মুক্ত(তথ্য->tdb_file);
	যদি (তথ্য->undo_file)
		io_channel_close(তথ্য->undo_file);
	ext2fs_free_mem(&তথ্য->keyb);
	যদি (তথ্য->written_block_map)
		ext2fs_free_generic_bitmap(তথ্য->written_block_map);
	ext2fs_free_mem(&channel->private_তথ্য);
	যদি (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	যদি (err)
		ফেরত err;
	ফেরত retval;
}

স্থির errcode_t undo_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (blksize > E2_UNDO_MAX_BLOCK_আকার || blksize < E2_UNDO_MIN_BLOCK_আকার)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (তথ্য->real)
		retval = io_channel_set_blksize(তথ্য->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	যদি (!তথ্য->tdb_তথ্য_আকার || !তথ্য->tdb_written)
		তথ্য->tdb_তথ্য_আকার = blksize;
	channel->block_আকার = blksize;
	ফেরত retval;
}

স্থির errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	errcode_t	retval = 0;
	কাঠামো undo_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_read_blk64(তথ্য->real, block, গুনতি, buf);

	ফেরত retval;
}

স্থির errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	ফেরত undo_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, গুনতি);
	যদি (retval)
		 ফেরত retval;
	যদি (তথ্য->real)
		retval = io_channel_write_blk64(তথ্য->real, block, গুনতি, buf);

	ফেরত retval;
}

স্থির errcode_t undo_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ফেরত undo_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 পূর্ণসংখ্যা আকার_2, ধ্রুবক শূন্য *buf)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_অঙ্ক, গুনতি;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + তথ্য->offset;
	blk_অঙ্ক = location/channel->block_আকার;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	গুনতি = (আকার_2 + (location % channel->block_আকার) +
			channel->block_আকার  -1)/channel->block_আকার;
	retval = undo_write_tdb(channel, blk_অঙ্ক, গুনতি);
	যদি (retval)
		ফেরত retval;
	যদি (তথ্য->real && তথ্য->real->manager->write_byte)
		retval = io_channel_write_byte(তথ্য->real, offset, আকার_2, buf);

	ফেরত retval;
}

স্থির errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	retval = 0;
	পূর্ণসংখ্যা icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (গুনতি > INT_MAX)
		ফেরত EXT2_ET_UNIMPLEMENTED;
	icount = গুনতি;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	যদি (retval)
		ফেরত retval;
	যদি (তথ্য->real)
		retval = io_channel_discard(তথ্য->real, block, গুনতি);

	ফেরত retval;
}

স্থির errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long গুনতি)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	retval = 0;
	পূর্ণসংখ্যা icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (গুনতি > INT_MAX)
		ফেরত EXT2_ET_UNIMPLEMENTED;
	icount = গুনতি;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	যদি (retval)
		ফেরত retval;
	যদি (তথ্য->real)
		retval = io_channel_zeroout(তথ্য->real, block, গুনতি);

	ফেরত retval;
}

স্থির errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long গুনতি)
{
	কাঠামো undo_private_তথ্য *তথ্য;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_cache_readahead(তথ্য->real, block, গুনতি);

	ফেরত retval;
}

/*
 * Flush data buffers to disk.
 */
স্থির errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	কাঠামো undo_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = io_channel_flush(তথ্য->real);

	ফেরত retval;
}

স্থির errcode_t undo_set_option(io_channel channel, ধ্রুবক অক্ষর *option,
				 ধ্রুবক অক্ষর *arg)
{
	errcode_t	retval = 0;
	কাঠামো undo_private_তথ্য *তথ্য;
	unsigned long tmp;
	অক্ষর *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (!strcmp(option, "tdb_data_size")) {
		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		যদি (*end)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		যদি (tmp > E2_UNDO_MAX_BLOCK_আকার || tmp < E2_UNDO_MIN_BLOCK_আকার)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		যদি (!তথ্য->tdb_তথ্য_আকার || !তথ্য->tdb_written) {
			তথ্য->tdb_written = -1;
			তথ্য->tdb_তথ্য_আকার = tmp;
		}
		ফেরত 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	যদি (তথ্য->real && তথ্য->real->manager->set_option) {
		retval = তথ্য->real->manager->set_option(তথ্য->real,
							option, arg);
	}
	যদি (!retval && !strcmp(option, "offset")) {
		যদি (!arg)
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		যদি (*end)
			ফেরত EXT2_ET_INVALID_ARGUMENT;
		তথ্য->offset = tmp;
	}
	ফেরত retval;
}

স্থির errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	কাঠামো undo_private_তথ্য *তথ্য;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	তথ্য = (কাঠামো undo_private_তথ্য *) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(তথ্য, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	যদি (তথ্য->real)
		retval = (তথ্য->real->manager->get_stats)(তথ্য->real, stats);

	ফেরত retval;
}

স্থির কাঠামো struct_io_manager struct_undo_manager = {
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
