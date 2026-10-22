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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#ifdef __linux__
#அந்தர்பாவ <sys/utsname.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_SYS_RESOURCE_H
#அந்தர்பாவ <sys/resource.h>
#endif
#அந்தர்பாவ <limits.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#இல்லையெனில்
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  செய் {printf(f, ## a); fflush(stdout); } போது (0)
#இல்லையெனில்
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)
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

கட்டமைப்பு undo_header {
	எழுத்து magic[8];		/* "E2UNDO02" */
	__le64 இலக்கம்_சாவிகள்;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 சாவி_offset;	/* where do the key/data block chunks start? */
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

கட்டமைப்பு undo_சாவி {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

கட்டமைப்பு undo_சாவி_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#எனில் __STDC_VERSION__ >= 199901L
	கட்டமைப்பு undo_சாவி சாவிகள்[];		/* keys, which come immediately after */
#இல்லையெனில்
	கட்டமைப்பு undo_சாவி சாவிகள்[0];	/* keys, which come immediately after */
#endif
};

கட்டமைப்பு undo_private_data {
	முழு	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_இலக்கம்;			/* next free block */
	blk64_t சாவி_blk_இலக்கம்;			/* current key block location */
	blk64_t super_blk_இலக்கம்;			/* superblock location */
	blk64_t first_சாவி_blk;			/* first key block location */
	கட்டமைப்பு undo_சாவி_block *keyb;
	size_t இலக்கம்_சாவிகள், சாவிகள்_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	முழு tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	கட்டமைப்பு struct_ext2_filsys fake_fs;
	எழுத்து *tdb_file;
	கட்டமைப்பு undo_header hdr;
};
#define சாவிகள்_PER_BLOCK(d) (((d)->tdb_data_size / அளவு(கட்டமைப்பு undo_சாவி)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

நிலையான உள்வரி வெறுமை e2undo_set_feature_fs_offset(கட்டமைப்பு undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

நிலையான உள்வரி வெறுமை e2undo_clear_feature_fs_offset(கட்டமைப்பு undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

நிலையான io_manager undo_io_backing_manager;
நிலையான எழுத்து *tdb_file;
நிலையான முழு actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	திரும்பு 0;
}

errcode_t set_undo_io_backup_file(எழுத்து *file_name)
{
	tdb_file = strdup(file_name);

	எனில் (tdb_file == வெற்று) {
		திரும்பு EXT2_ET_NO_MEMORY;
	}

	திரும்பு 0;
}

நிலையான errcode_t write_undo_indexes(கட்டமைப்பு undo_private_data *data, முழு flush)
{
	errcode_t retval;
	கட்டமைப்பு ext2_super_block super;
	io_channel channel;
	முழு block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	எனில் (data->சாவிகள்_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned எழுத்து *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->சாவி_blk_இலக்கம்);
		retval = io_channel_write_blk64(data->undo_file,
						data->சாவி_blk_இலக்கம்,
						1, data->keyb);
		எனில் (retval)
			திரும்பு retval;
		/* Move on to the next key block if it's full. */
		எனில் (data->சாவிகள்_in_block == சாவிகள்_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->சாவிகள்_in_block = 0;
			data->சாவி_blk_இலக்கம் = data->undo_blk_இலக்கம்;
			data->undo_blk_இலக்கம்++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	எனில் (retval)
		செல் err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned எழுத்து *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, அளவு(data->hdr.magic));
	data->hdr.இலக்கம்_சாவிகள் = ext2fs_cpu_to_le64(data->இலக்கம்_சாவிகள்);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_இலக்கம்);
	data->hdr.சாவி_offset = ext2fs_cpu_to_le64(data->first_சாவி_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	எனில் (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	இல்லையெனில்
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned எழுத்து *)&data->hdr,
				   அளவு(data->hdr) -
				   அளவு(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(முழு)அளவு(data->hdr),
					&data->hdr);
	எனில் (retval)
		செல் err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_இலக்கம்);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_இலக்கம்,
					-SUPERBLOCK_SIZE, &super);
	எனில் (retval)
		செல் err_out;

	எனில் (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	திரும்பு retval;
}

நிலையான errcode_t undo_setup_tdb(கட்டமைப்பு undo_private_data *data)
{
	முழு i;
	errcode_t retval;

	எனில் (data->tdb_written == 1)
		திரும்பு 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, அளவு(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	எனில் (retval)
		திரும்பு retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	எனில் (retval)
		திரும்பு retval;
	data->சாவி_blk_இலக்கம் = data->first_சாவி_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", சாவிகள்_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	க்கு (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		எனில் (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		எனில் (retval)
			திரும்பு retval;
		retval = io_channel_flush(data->undo_file);
		எனில் (retval)
			திரும்பு retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	திரும்பு 0;
}

நிலையான errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, முழு மொத்தம்)

{
	முழு size, sz;
	unsigned long long block_இலக்கம், backing_blk_இலக்கம்;
	errcode_t retval = 0;
	ext2_loff_t offset;
	கட்டமைப்பு undo_private_data *data;
	unsigned எழுத்து *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	கட்டமைப்பு undo_சாவி *சாவி;
	__u32 blk_crc;

	data = (கட்டமைப்பு undo_private_data *) channel->private_data;

	எனில் (data->undo_file == வெற்று) {
		/*
		 * Transaction database not initialized
		 */
		திரும்பு 0;
	}

	எனில் (மொத்தம் == 1)
		size = channel->block_size;
	இல்லையெனில் {
		எனில் (மொத்தம் < 0)
			size = -மொத்தம்;
		இல்லையெனில்
			size = மொத்தம் * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	எனில் (retval)
		திரும்பு retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_இலக்கம் = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	போது (block_இலக்கம் <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		எனில் (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_இலக்கம்)) {
			/* Try the next block */
			block_இலக்கம்++;
			தொடர்;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_இலக்கம்);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_இலக்கம் * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_இலக்கம் = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		எனில் (retval) {
			திரும்பு retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		எனில் ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		இல்லையெனில்
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_இலக்கம்,
					     sz, read_ptr);
		எனில் (retval) {
			எனில் (retval != EXT2_ET_SHORT_READ) {
				விடுவி(read_ptr);
				திரும்பு retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} இல்லையெனில் {
			data_size = data->tdb_data_size;
		}
		எனில் (data_size == 0) {
			விடுவி(read_ptr);
			block_இலக்கம்++;
			தொடர்;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_இலக்கம், block, data->tdb_data_size);
		எனில் ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		இல்லையெனில்
			sz = -data_size;;
		/* extend this key? */
		எனில் (data->சாவிகள்_in_block) {
			சாவி = data->keyb->சாவிகள் + data->சாவிகள்_in_block - 1;
			keysz = ext2fs_le32_to_cpu(சாவி->size);
		} இல்லையெனில் {
			சாவி = வெற்று;
			keysz = 0;
		}
		எனில் (சாவி != வெற்று &&
		    (ext2fs_le64_to_cpu(சாவி->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_இலக்கம் &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(சாவி->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			சாவி->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			சாவி->size = ext2fs_cpu_to_le32(keysz + data_size);
		} இல்லையெனில் {
			data->இலக்கம்_சாவிகள்++;
			சாவி = data->keyb->சாவிகள் + data->சாவிகள்_in_block;
			data->சாவிகள்_in_block++;
			சாவி->fsblk = ext2fs_cpu_to_le64(backing_blk_இலக்கம்);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			சாவி->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			சாவி->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_இலக்கம்,
		       data->undo_blk_இலக்கம்,
		       sz, data->இலக்கம்_சாவிகள் - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_இலக்கம், sz, read_ptr);
		எனில் (retval) {
			விடுவி(read_ptr);
			திரும்பு retval;
		}
		data->undo_blk_இலக்கம்++;
		விடுவி(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		எனில் (retval)
			திரும்பு retval;

		/* Next block */
		block_இலக்கம்++;
	}

	திரும்பு retval;
}

நிலையான errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    முழு மொத்தம் ATTR((unused)),
				    வெறுமை *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    முழு actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	திரும்பு error;
}

நிலையான வெறுமை undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

நிலையான முழு check_filesystem(கட்டமைப்பு undo_header *hdr, io_channel undo_file,
			    unsigned முழு blocksize, blk64_t super_block,
			    io_channel channel)
{
	கட்டமைப்பு ext2_super_block super, *sb;
	எழுத்து *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	எனில் (retval)
		திரும்பு retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	எனில் (retval)
		செல் out;
	sb = (கட்டமைப்பு ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	எனில் (memcmp(&super, buf, அளவு(super))) {
		retval = -1;
		செல் out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned எழுத்து *)buf, SUPERBLOCK_SIZE);
	எனில் (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		செல் out;
	}

out:
	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
நிலையான errcode_t try_reopen_undo_file(முழு undo_fd,
				      கட்டமைப்பு undo_private_data *data)
{
	கட்டமைப்பு undo_header hdr;
	கட்டமைப்பு undo_சாவி *dkey;
	ext2fs_struct_stat statbuf;
	unsigned முழு blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t இலக்கம்_சாவிகள், சாவிகள்_per_block, i;
	__u32 hdr_crc, சாவி_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	எனில் (retval)
		செல் bad_file;
	எனில் (statbuf.st_size == 0)
		செல் out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(முழு)அளவு(hdr),
				       &hdr);
	எனில் (retval)
		செல் bad_file;

	எனில் (memcmp(hdr.magic, E2UNDO_MAGIC,
		    அளவு(hdr.magic)))
		செல் bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned எழுத்து *)&hdr,
				   அளவு(கட்டமைப்பு undo_header) -
				   அளவு(__u32));
	எனில் (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		செல் bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	எனில் (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		செல் bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	இலக்கம்_சாவிகள் = ext2fs_le64_to_cpu(hdr.இலக்கம்_சாவிகள்);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	எனில் (hdr.f_incompat || hdr.f_rocompat)
		செல் bad_file;

	/* Superblock matches this FS? */
	எனில் (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		செல் out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	எனில் (retval)
		செல் bad_file;
	data->இலக்கம்_சாவிகள் = இலக்கம்_சாவிகள்;
	data->super_blk_இலக்கம் = super_block;
	data->first_சாவி_blk = ext2fs_le64_to_cpu(hdr.சாவி_offset);

	/* load the written block map */
	சாவிகள்_per_block = சாவிகள்_PER_BLOCK(data);
	lblk = data->first_சாவி_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   இலக்கம்_சாவிகள், சாவிகள்_per_block, blocksize);
	க்கு (i = 0; i < இலக்கம்_சாவிகள்; i += சாவிகள்_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->சாவி_blk_இலக்கம் = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		எனில் (retval)
			செல் bad_சாவி_replay;

		/* check keys */
		எனில் (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			செல் bad_சாவி_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		சாவி_crc = ext2fs_crc32c_le(~0, (unsigned எழுத்து *)data->keyb,
					   blocksize);
		எனில் (ext2fs_le32_to_cpu(crc) != சாவி_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			செல் bad_சாவி_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->இலக்கம்_சாவிகள் - i;
		எனில் (max_j > சாவிகள்_per_block)
			max_j = சாவிகள்_per_block;
		க்கு (j = 0, dkey = data->keyb->சாவிகள்;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_இலக்கம் = lblk;
			data->சாவிகள்_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->சாவி_blk_இலக்கம், data->undo_blk_இலக்கம், data->இலக்கம்_சாவிகள்,
		   data->சாவிகள்_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	திரும்பு retval;

bad_சாவி_replay:
	data->சாவி_blk_இலக்கம் = data->undo_blk_இலக்கம் = 0;
	data->சாவிகள்_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	செல் out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	திரும்பு retval;
}

நிலையான வெறுமை undo_atexit(வெறுமை *p)
{
	கட்டமைப்பு undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

நிலையான errcode_t undo_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	io_channel	io = வெற்று;
	கட்டமைப்பு undo_private_data *data = வெற்று;
	முழு		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	எனில் (name == 0)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_io_channel), &io);
	எனில் (retval)
		செல் cleanup;
	memset(io, 0, அளவு(கட்டமைப்பு struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு undo_private_data), &data);
	எனில் (retval)
		செல் cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	எனில் (retval)
		செல் cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, அளவு(கட்டமைப்பு undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_இலக்கம் = 1;
	data->first_சாவி_blk = 2;
	data->undo_blk_இலக்கம் = 3;

	எனில் (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		எனில் (retval)
			செல் cleanup;

		data->tdb_file = strdup(tdb_file);
		எனில் (data->tdb_file == வெற்று)
			செல் cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		எனில் (undo_fd < 0)
			செல் cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		எனில் (retval)
			செல் cleanup;
	} இல்லையெனில் {
		data->real = வெற்று;
		data->undo_file = வெற்று;
	}

	எனில் (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	எனில் (data->real)
		undo_err_handler_init(data->real);

	எனில் (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		எனில் (retval)
			செல் cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	எனில் (retval)
		செல் cleanup;

	*channel = io;
	எனில் (undo_fd >= 0)
		close(undo_fd);
	திரும்பு retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	எனில் (undo_fd >= 0)
		close(undo_fd);
	எனில் (data && data->undo_file)
		io_channel_close(data->undo_file);
	எனில் (data && data->tdb_file)
		விடுவி(data->tdb_file);
	எனில் (data && data->real)
		io_channel_close(data->real);
	எனில் (data)
		ext2fs_free_mem(&data);
	எனில் (io && io->name)
		ext2fs_free_mem(&io->name);
	எனில் (io)
		ext2fs_free_mem(&io);
	திரும்பு retval;
}

நிலையான errcode_t undo_close(io_channel channel)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
		திரும்பு 0;
	/* Before closing write the file system identity */
	எனில் (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	எனில் (data->real)
		retval = io_channel_close(data->real);
	எனில் (data->tdb_file)
		விடுவி(data->tdb_file);
	எனில் (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	எனில் (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	எனில் (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	எனில் (err)
		திரும்பு err;
	திரும்பு retval;
}

நிலையான errcode_t undo_set_blksize(io_channel channel, முழு blksize)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	எனில் (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	திரும்பு retval;
}

நிலையான errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	errcode_t	retval = 0;
	கட்டமைப்பு undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_read_blk64(data->real, block, மொத்தம், buf);

	திரும்பு retval;
}

நிலையான errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	திரும்பு undo_read_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, மொத்தம்);
	எனில் (retval)
		 திரும்பு retval;
	எனில் (data->real)
		retval = io_channel_write_blk64(data->real, block, மொத்தம், buf);

	திரும்பு retval;
}

நிலையான errcode_t undo_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	திரும்பு undo_write_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 முழு size, மாறிலி வெறுமை *buf)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_இலக்கம், மொத்தம்;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_இலக்கம் = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	மொத்தம் = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_இலக்கம், மொத்தம்);
	எனில் (retval)
		திரும்பு retval;
	எனில் (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	திரும்பு retval;
}

நிலையான errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	retval = 0;
	முழு icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (மொத்தம் > INT_MAX)
		திரும்பு EXT2_ET_UNIMPLEMENTED;
	icount = மொத்தம்;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	எனில் (retval)
		திரும்பு retval;
	எனில் (data->real)
		retval = io_channel_discard(data->real, block, மொத்தம்);

	திரும்பு retval;
}

நிலையான errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long மொத்தம்)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	retval = 0;
	முழு icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (மொத்தம் > INT_MAX)
		திரும்பு EXT2_ET_UNIMPLEMENTED;
	icount = மொத்தம்;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	எனில் (retval)
		திரும்பு retval;
	எனில் (data->real)
		retval = io_channel_zeroout(data->real, block, மொத்தம்);

	திரும்பு retval;
}

நிலையான errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long மொத்தம்)
{
	கட்டமைப்பு undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_cache_readahead(data->real, block, மொத்தம்);

	திரும்பு retval;
}

/*
 * Flush data buffers to disk.
 */
நிலையான errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	கட்டமைப்பு undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (data->real)
		retval = io_channel_flush(data->real);

	திரும்பு retval;
}

நிலையான errcode_t undo_set_option(io_channel channel, மாறிலி எழுத்து *option,
				 மாறிலி எழுத்து *arg)
{
	errcode_t	retval = 0;
	கட்டமைப்பு undo_private_data *data;
	unsigned long tmp;
	எழுத்து *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (!strcmp(option, "tdb_data_size")) {
		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		எனில் (*end)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		எனில் (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		எனில் (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		திரும்பு 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	எனில் (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	எனில் (!retval && !strcmp(option, "offset")) {
		எனில் (!arg)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		எனில் (*end)
			திரும்பு EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	திரும்பு retval;
}

நிலையான errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	கட்டமைப்பு undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (கட்டமைப்பு undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	எனில் (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	திரும்பு retval;
}

நிலையான கட்டமைப்பு struct_io_manager struct_undo_manager = {
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
