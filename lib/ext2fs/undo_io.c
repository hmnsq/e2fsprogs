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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ifdef __linux__
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#endif
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_SYS_RESOURCE_H
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#endif
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#ಅನ್ಯಥಾ
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  ಕುರು {printf(f, ## a); fflush(stdout); } ಯಾವತ್ (0)
#ಅನ್ಯಥಾ
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)
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

ಸಂರಚನಾ undo_header {
	ವರ್ಣ magic[8];		/* "E2UNDO02" */
	__le64 ಅಂಕೆ_ಕೀಲಿಗಳು;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 ಕೀಲಿ_offset;	/* where do the key/data block chunks start? */
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

ಸಂರಚನಾ undo_ಕೀಲಿ {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

ಸಂರಚನಾ undo_ಕೀಲಿ_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#ಯದಿ __STDC_VERSION__ >= 199901L
	ಸಂರಚನಾ undo_ಕೀಲಿ ಕೀಲಿಗಳು[];		/* keys, which come immediately after */
#ಅನ್ಯಥಾ
	ಸಂರಚನಾ undo_ಕೀಲಿ ಕೀಲಿಗಳು[0];	/* keys, which come immediately after */
#endif
};

ಸಂರಚನಾ undo_private_data {
	ಪೂರ್ಣಾಙ್ಕ	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_ಅಂಕೆ;			/* next free block */
	blk64_t ಕೀಲಿ_blk_ಅಂಕೆ;			/* current key block location */
	blk64_t super_blk_ಅಂಕೆ;			/* superblock location */
	blk64_t first_ಕೀಲಿ_blk;			/* first key block location */
	ಸಂರಚನಾ undo_ಕೀಲಿ_block *keyb;
	size_t ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	ಪೂರ್ಣಾಙ್ಕ tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	ಸಂರಚನಾ struct_ext2_filsys fake_fs;
	ವರ್ಣ *tdb_file;
	ಸಂರಚನಾ undo_header hdr;
};
#define ಕೀಲಿಗಳು_PER_BLOCK(d) (((d)->tdb_data_size / ಪರಿಮಾಣ(ಸಂರಚನಾ undo_ಕೀಲಿ)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ e2undo_set_feature_fs_offset(ಸಂರಚನಾ undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ e2undo_clear_feature_fs_offset(ಸಂರಚನಾ undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ಸ್ಥಿರ io_manager undo_io_backing_manager;
ಸ್ಥಿರ ವರ್ಣ *tdb_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t set_undo_io_backup_file(ವರ್ಣ *file_name)
{
	tdb_file = strdup(file_name);

	ಯದಿ (tdb_file == ರಿಕ್ತ) {
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t write_undo_indexes(ಸಂರಚನಾ undo_private_data *data, ಪೂರ್ಣಾಙ್ಕ flush)
{
	errcode_t retval;
	ಸಂರಚನಾ ext2_super_block super;
	io_channel channel;
	ಪೂರ್ಣಾಙ್ಕ block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	ಯದಿ (data->ಕೀಲಿಗಳು_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned ವರ್ಣ *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->ಕೀಲಿ_blk_ಅಂಕೆ);
		retval = io_channel_write_blk64(data->undo_file,
						data->ಕೀಲಿ_blk_ಅಂಕೆ,
						1, data->keyb);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		/* Move on to the next key block if it's full. */
		ಯದಿ (data->ಕೀಲಿಗಳು_in_block == ಕೀಲಿಗಳು_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->ಕೀಲಿಗಳು_in_block = 0;
			data->ಕೀಲಿ_blk_ಅಂಕೆ = data->undo_blk_ಅಂಕೆ;
			data->undo_blk_ಅಂಕೆ++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	ಯದಿ (retval)
		ಗಚ್ಛ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, ಪರಿಮಾಣ(data->hdr.magic));
	data->hdr.ಅಂಕೆ_ಕೀಲಿಗಳು = ext2fs_cpu_to_le64(data->ಅಂಕೆ_ಕೀಲಿಗಳು);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_ಅಂಕೆ);
	data->hdr.ಕೀಲಿ_offset = ext2fs_cpu_to_le64(data->first_ಕೀಲಿ_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	ಯದಿ (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	ಅನ್ಯಥಾ
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)&data->hdr,
				   ಪರಿಮಾಣ(data->hdr) -
				   ಪರಿಮಾಣ(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(data->hdr),
					&data->hdr);
	ಯದಿ (retval)
		ಗಚ್ಛ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_ಅಂಕೆ);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_ಅಂಕೆ,
					-SUPERBLOCK_SIZE, &super);
	ಯದಿ (retval)
		ಗಚ್ಛ err_out;

	ಯದಿ (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_setup_tdb(ಸಂರಚನಾ undo_private_data *data)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	errcode_t retval;

	ಯದಿ (data->tdb_written == 1)
		ಪ್ರತ್ಯಾಗಮ 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, ಪರಿಮಾಣ(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	data->ಕೀಲಿ_blk_ಅಂಕೆ = data->first_ಕೀಲಿ_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", ಕೀಲಿಗಳು_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	ಪ್ರತಿ (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		ಯದಿ (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		retval = io_channel_flush(data->undo_file);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)

{
	ಪೂರ್ಣಾಙ್ಕ size, sz;
	unsigned long long block_ಅಂಕೆ, backing_blk_ಅಂಕೆ;
	errcode_t retval = 0;
	ext2_loff_t offset;
	ಸಂರಚನಾ undo_private_data *data;
	unsigned ವರ್ಣ *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	ಸಂರಚನಾ undo_ಕೀಲಿ *ಕೀಲಿ;
	__u32 blk_crc;

	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;

	ಯದಿ (data->undo_file == ರಿಕ್ತ) {
		/*
		 * Transaction database not initialized
		 */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (ಎಣಿಕೆ == 1)
		size = channel->block_size;
	ಅನ್ಯಥಾ {
		ಯದಿ (ಎಣಿಕೆ < 0)
			size = -ಎಣಿಕೆ;
		ಅನ್ಯಥಾ
			size = ಎಣಿಕೆ * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_ಅಂಕೆ = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	ಯಾವತ್ (block_ಅಂಕೆ <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		ಯದಿ (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_ಅಂಕೆ)) {
			/* Try the next block */
			block_ಅಂಕೆ++;
			ಅನುವರ್ತ;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_ಅಂಕೆ);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_ಅಂಕೆ * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_ಅಂಕೆ = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		ಯದಿ (retval) {
			ಪ್ರತ್ಯಾಗಮ retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		ಯದಿ ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		ಅನ್ಯಥಾ
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_ಅಂಕೆ,
					     sz, read_ptr);
		ಯದಿ (retval) {
			ಯದಿ (retval != EXT2_ET_SHORT_READ) {
				ಮುಕ್ತ(read_ptr);
				ಪ್ರತ್ಯಾಗಮ retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} ಅನ್ಯಥಾ {
			data_size = data->tdb_data_size;
		}
		ಯದಿ (data_size == 0) {
			ಮುಕ್ತ(read_ptr);
			block_ಅಂಕೆ++;
			ಅನುವರ್ತ;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_ಅಂಕೆ, block, data->tdb_data_size);
		ಯದಿ ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		ಅನ್ಯಥಾ
			sz = -data_size;;
		/* extend this key? */
		ಯದಿ (data->ಕೀಲಿಗಳು_in_block) {
			ಕೀಲಿ = data->keyb->ಕೀಲಿಗಳು + data->ಕೀಲಿಗಳು_in_block - 1;
			keysz = ext2fs_le32_to_cpu(ಕೀಲಿ->size);
		} ಅನ್ಯಥಾ {
			ಕೀಲಿ = ರಿಕ್ತ;
			keysz = 0;
		}
		ಯದಿ (ಕೀಲಿ != ರಿಕ್ತ &&
		    (ext2fs_le64_to_cpu(ಕೀಲಿ->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_ಅಂಕೆ &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(ಕೀಲಿ->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			ಕೀಲಿ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ಕೀಲಿ->size = ext2fs_cpu_to_le32(keysz + data_size);
		} ಅನ್ಯಥಾ {
			data->ಅಂಕೆ_ಕೀಲಿಗಳು++;
			ಕೀಲಿ = data->keyb->ಕೀಲಿಗಳು + data->ಕೀಲಿಗಳು_in_block;
			data->ಕೀಲಿಗಳು_in_block++;
			ಕೀಲಿ->fsblk = ext2fs_cpu_to_le64(backing_blk_ಅಂಕೆ);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			ಕೀಲಿ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ಕೀಲಿ->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_ಅಂಕೆ,
		       data->undo_blk_ಅಂಕೆ,
		       sz, data->ಅಂಕೆ_ಕೀಲಿಗಳು - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_ಅಂಕೆ, sz, read_ptr);
		ಯದಿ (retval) {
			ಮುಕ್ತ(read_ptr);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		data->undo_blk_ಅಂಕೆ++;
		ಮುಕ್ತ(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		/* Next block */
		block_ಅಂಕೆ++;
	}

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ ATTR((unused)),
				    ಶೂನ್ಯ *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    ಪೂರ್ಣಾಙ್ಕ actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	ಪ್ರತ್ಯಾಗಮ error;
}

ಸ್ಥಿರ ಶೂನ್ಯ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_filesystem(ಸಂರಚನಾ undo_header *hdr, io_channel undo_file,
			    unsigned ಪೂರ್ಣಾಙ್ಕ blocksize, blk64_t super_block,
			    io_channel channel)
{
	ಸಂರಚನಾ ext2_super_block super, *sb;
	ವರ್ಣ *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	ಯದಿ (retval)
		ಗಚ್ಛ out;
	sb = (ಸಂರಚನಾ ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	ಯದಿ (memcmp(&super, buf, ಪರಿಮಾಣ(super))) {
		retval = -1;
		ಗಚ್ಛ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)buf, SUPERBLOCK_SIZE);
	ಯದಿ (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		ಗಚ್ಛ out;
	}

out:
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ಸ್ಥಿರ errcode_t try_reopen_undo_file(ಪೂರ್ಣಾಙ್ಕ undo_fd,
				      ಸಂರಚನಾ undo_private_data *data)
{
	ಸಂರಚನಾ undo_header hdr;
	ಸಂರಚನಾ undo_ಕೀಲಿ *dkey;
	ext2fs_struct_stat statbuf;
	unsigned ಪೂರ್ಣಾಙ್ಕ blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_per_block, i;
	__u32 hdr_crc, ಕೀಲಿ_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	ಯದಿ (retval)
		ಗಚ್ಛ bad_file;
	ಯದಿ (statbuf.st_size == 0)
		ಗಚ್ಛ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(hdr),
				       &hdr);
	ಯದಿ (retval)
		ಗಚ್ಛ bad_file;

	ಯದಿ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    ಪರಿಮಾಣ(hdr.magic)))
		ಗಚ್ಛ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)&hdr,
				   ಪರಿಮಾಣ(ಸಂರಚನಾ undo_header) -
				   ಪರಿಮಾಣ(__u32));
	ಯದಿ (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		ಗಚ್ಛ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	ಯದಿ (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		ಗಚ್ಛ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	ಅಂಕೆ_ಕೀಲಿಗಳು = ext2fs_le64_to_cpu(hdr.ಅಂಕೆ_ಕೀಲಿಗಳು);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	ಯದಿ (hdr.f_incompat || hdr.f_rocompat)
		ಗಚ್ಛ bad_file;

	/* Superblock matches this FS? */
	ಯದಿ (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		ಗಚ್ಛ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	ಯದಿ (retval)
		ಗಚ್ಛ bad_file;
	data->ಅಂಕೆ_ಕೀಲಿಗಳು = ಅಂಕೆ_ಕೀಲಿಗಳು;
	data->super_blk_ಅಂಕೆ = super_block;
	data->first_ಕೀಲಿ_blk = ext2fs_le64_to_cpu(hdr.ಕೀಲಿ_offset);

	/* load the written block map */
	ಕೀಲಿಗಳು_per_block = ಕೀಲಿಗಳು_PER_BLOCK(data);
	lblk = data->first_ಕೀಲಿ_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_per_block, blocksize);
	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_ಕೀಲಿಗಳು; i += ಕೀಲಿಗಳು_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->ಕೀಲಿ_blk_ಅಂಕೆ = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		ಯದಿ (retval)
			ಗಚ್ಛ bad_ಕೀಲಿ_replay;

		/* check keys */
		ಯದಿ (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ಗಚ್ಛ bad_ಕೀಲಿ_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		ಕೀಲಿ_crc = ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)data->keyb,
					   blocksize);
		ಯದಿ (ext2fs_le32_to_cpu(crc) != ಕೀಲಿ_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ಗಚ್ಛ bad_ಕೀಲಿ_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->ಅಂಕೆ_ಕೀಲಿಗಳು - i;
		ಯದಿ (max_j > ಕೀಲಿಗಳು_per_block)
			max_j = ಕೀಲಿಗಳು_per_block;
		ಪ್ರತಿ (j = 0, dkey = data->keyb->ಕೀಲಿಗಳು;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_ಅಂಕೆ = lblk;
			data->ಕೀಲಿಗಳು_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->ಕೀಲಿ_blk_ಅಂಕೆ, data->undo_blk_ಅಂಕೆ, data->ಅಂಕೆ_ಕೀಲಿಗಳು,
		   data->ಕೀಲಿಗಳು_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	ಪ್ರತ್ಯಾಗಮ retval;

bad_ಕೀಲಿ_replay:
	data->ಕೀಲಿ_blk_ಅಂಕೆ = data->undo_blk_ಅಂಕೆ = 0;
	data->ಕೀಲಿಗಳು_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	ಗಚ್ಛ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ undo_atexit(ಶೂನ್ಯ *p)
{
	ಸಂರಚನಾ undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

ಸ್ಥಿರ errcode_t undo_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	io_channel	io = ರಿಕ್ತ;
	ಸಂರಚನಾ undo_private_data *data = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	ಯದಿ (name == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel), &io);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(io, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ undo_private_data), &data);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_ಅಂಕೆ = 1;
	data->first_ಕೀಲಿ_blk = 2;
	data->undo_blk_ಅಂಕೆ = 3;

	ಯದಿ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;

		data->tdb_file = strdup(tdb_file);
		ಯದಿ (data->tdb_file == ರಿಕ್ತ)
			ಗಚ್ಛ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		ಯದಿ (undo_fd < 0)
			ಗಚ್ಛ cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		data->real = ರಿಕ್ತ;
		data->undo_file = ರಿಕ್ತ;
	}

	ಯದಿ (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	ಯದಿ (data->real)
		undo_err_handler_init(data->real);

	ಯದಿ (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	*channel = io;
	ಯದಿ (undo_fd >= 0)
		close(undo_fd);
	ಪ್ರತ್ಯಾಗಮ retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	ಯದಿ (undo_fd >= 0)
		close(undo_fd);
	ಯದಿ (data && data->undo_file)
		io_channel_close(data->undo_file);
	ಯದಿ (data && data->tdb_file)
		ಮುಕ್ತ(data->tdb_file);
	ಯದಿ (data && data->real)
		io_channel_close(data->real);
	ಯದಿ (data)
		ext2fs_free_mem(&data);
	ಯದಿ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಯದಿ (io)
		ext2fs_free_mem(&io);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_close(io_channel channel)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (--channel->refcount > 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	/* Before closing write the file system identity */
	ಯದಿ (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	ಯದಿ (data->real)
		retval = io_channel_close(data->real);
	ಯದಿ (data->tdb_file)
		ಮುಕ್ತ(data->tdb_file);
	ಯದಿ (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	ಯದಿ (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	ಯದಿ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	ಯದಿ (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	errcode_t	retval = 0;
	ಸಂರಚನಾ undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_read_blk64(data->real, block, ಎಣಿಕೆ, buf);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ undo_read_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, ಎಣಿಕೆ);
	ಯದಿ (retval)
		 ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (data->real)
		retval = io_channel_write_blk64(data->real, block, ಎಣಿಕೆ, buf);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ undo_write_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 ಪೂರ್ಣಾಙ್ಕ size, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_ಅಂಕೆ, ಎಣಿಕೆ;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_ಅಂಕೆ = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ಎಣಿಕೆ = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_ಅಂಕೆ, ಎಣಿಕೆ);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಙ್ಕ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (ಎಣಿಕೆ > INT_MAX)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
	icount = ಎಣಿಕೆ;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (data->real)
		retval = io_channel_discard(data->real, block, ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಙ್ಕ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (ಎಣಿಕೆ > INT_MAX)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
	icount = ಎಣಿಕೆ;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (data->real)
		retval = io_channel_zeroout(data->real, block, ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ಎಣಿಕೆ)
{
	ಸಂರಚನಾ undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_cache_readahead(data->real, block, ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ಸಂರಚನಾ undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = io_channel_flush(data->real);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_set_option(io_channel channel, ಧ್ರುವ ವರ್ಣ *option,
				 ಧ್ರುವ ವರ್ಣ *arg)
{
	errcode_t	retval = 0;
	ಸಂರಚನಾ undo_private_data *data;
	unsigned long tmp;
	ವರ್ಣ *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (!strcmp(option, "tdb_data_size")) {
		ಯದಿ (!arg)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		ಯದಿ (*end)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
		ಯದಿ (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
		ಯದಿ (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	ಯದಿ (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	ಯದಿ (!retval && !strcmp(option, "offset")) {
		ಯದಿ (!arg)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		ಯದಿ (*end)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ಸಂರಚನಾ undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ಸಂರಚನಾ undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಯದಿ (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_undo_manager = {
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
