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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ifdef __linux__
#ଅନ୍ତର୍ଭାଵ <sys/utsname.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_SYS_RESOURCE_H
#ଅନ୍ତର୍ଭାଵ <sys/resource.h>
#endif
#ଅନ୍ତର୍ଭାଵ <limits.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#ଅନ୍ୟଥା
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  କର {printf(f, ## a); fflush(stdout); } ଯେତେବେଳେ (0)
#ଅନ୍ୟଥା
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)
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

ସଂରଚନା undo_header {
	ଅକ୍ଷର magic[8];		/* "E2UNDO02" */
	__le64 ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 ଚାବି_offset;	/* where do the key/data block chunks start? */
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

ସଂରଚନା undo_ଚାବି {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

ସଂରଚନା undo_ଚାବି_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#ଯଦି __STDC_VERSION__ >= 199901L
	ସଂରଚନା undo_ଚାବି ଚାବିଗୁଡ଼ିକ[];		/* keys, which come immediately after */
#ଅନ୍ୟଥା
	ସଂରଚନା undo_ଚାବି ଚାବିଗୁଡ଼ିକ[0];	/* keys, which come immediately after */
#endif
};

ସଂରଚନା undo_private_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_ଅଙ୍କ;			/* next free block */
	blk64_t ଚାବି_blk_ଅଙ୍କ;			/* current key block location */
	blk64_t super_blk_ଅଙ୍କ;			/* superblock location */
	blk64_t first_ଚାବି_blk;			/* first key block location */
	ସଂରଚନା undo_ଚାବି_block *keyb;
	size_t ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, ଚାବିଗୁଡ଼ିକ_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	ସଂରଚନା struct_ext2_filsys fake_fs;
	ଅକ୍ଷର *tdb_file;
	ସଂରଚନା undo_header hdr;
};
#define ଚାବିଗୁଡ଼ିକ_PER_BLOCK(d) (((d)->tdb_data_size / ଆକାର(ସଂରଚନା undo_ଚାବି)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ e2undo_set_feature_fs_offset(ସଂରଚନା undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ e2undo_clear_feature_fs_offset(ସଂରଚନା undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ସ୍ଥିର io_manager undo_io_backing_manager;
ସ୍ଥିର ଅକ୍ଷର *tdb_file;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ଫେରନ୍ତୁ 0;
}

errcode_t set_undo_io_backup_file(ଅକ୍ଷର *file_name)
{
	tdb_file = strdup(file_name);

	ଯଦି (tdb_file == ଖାଲି) {
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t write_undo_indexes(ସଂରଚନା undo_private_data *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flush)
{
	errcode_t retval;
	ସଂରଚନା ext2_super_block super;
	io_channel channel;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	ଯଦି (data->ଚାବିଗୁଡ଼ିକ_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned ଅକ୍ଷର *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->ଚାବି_blk_ଅଙ୍କ);
		retval = io_channel_write_blk64(data->undo_file,
						data->ଚାବି_blk_ଅଙ୍କ,
						1, data->keyb);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		/* Move on to the next key block if it's full. */
		ଯଦି (data->ଚାବିଗୁଡ଼ିକ_in_block == ଚାବିଗୁଡ଼ିକ_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->ଚାବିଗୁଡ଼ିକ_in_block = 0;
			data->ଚାବି_blk_ଅଙ୍କ = data->undo_blk_ଅଙ୍କ;
			data->undo_blk_ଅଙ୍କ++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	ଯଦି (retval)
		ଯାଅ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, ଆକାର(data->hdr.magic));
	data->hdr.ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = ext2fs_cpu_to_le64(data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_ଅଙ୍କ);
	data->hdr.ଚାବି_offset = ext2fs_cpu_to_le64(data->first_ଚାବି_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	ଯଦି (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	ଅନ୍ୟଥା
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)&data->hdr,
				   ଆକାର(data->hdr) -
				   ଆକାର(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(data->hdr),
					&data->hdr);
	ଯଦି (retval)
		ଯାଅ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_ଅଙ୍କ);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_ଅଙ୍କ,
					-SUPERBLOCK_SIZE, &super);
	ଯଦି (retval)
		ଯାଅ err_out;

	ଯଦି (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_setup_tdb(ସଂରଚନା undo_private_data *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	errcode_t retval;

	ଯଦି (data->tdb_written == 1)
		ଫେରନ୍ତୁ 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, ଆକାର(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	data->ଚାବି_blk_ଅଙ୍କ = data->first_ଚାବି_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", ଚାବିଗୁଡ଼ିକ_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	ପାଇଁ (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		ଯଦି (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		retval = io_channel_flush(data->undo_file);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)

{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, sz;
	unsigned long long block_ଅଙ୍କ, backing_blk_ଅଙ୍କ;
	errcode_t retval = 0;
	ext2_loff_t offset;
	ସଂରଚନା undo_private_data *data;
	unsigned ଅକ୍ଷର *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	ସଂରଚନା undo_ଚାବି *ଚାବି;
	__u32 blk_crc;

	data = (ସଂରଚନା undo_private_data *) channel->private_data;

	ଯଦି (data->undo_file == ଖାଲି) {
		/*
		 * Transaction database not initialized
		 */
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (ଗଣନା_2 == 1)
		size = channel->block_size;
	ଅନ୍ୟଥା {
		ଯଦି (ଗଣନା_2 < 0)
			size = -ଗଣନା_2;
		ଅନ୍ୟଥା
			size = ଗଣନା_2 * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_ଅଙ୍କ = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	ଯେତେବେଳେ (block_ଅଙ୍କ <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		ଯଦି (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_ଅଙ୍କ)) {
			/* Try the next block */
			block_ଅଙ୍କ++;
			ଜାରି;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_ଅଙ୍କ);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_ଅଙ୍କ * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_ଅଙ୍କ = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		ଯଦି (retval) {
			ଫେରନ୍ତୁ retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		ଯଦି ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		ଅନ୍ୟଥା
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_ଅଙ୍କ,
					     sz, read_ptr);
		ଯଦି (retval) {
			ଯଦି (retval != EXT2_ET_SHORT_READ) {
				ମୁକ୍ତ(read_ptr);
				ଫେରନ୍ତୁ retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} ଅନ୍ୟଥା {
			data_size = data->tdb_data_size;
		}
		ଯଦି (data_size == 0) {
			ମୁକ୍ତ(read_ptr);
			block_ଅଙ୍କ++;
			ଜାରି;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_ଅଙ୍କ, block, data->tdb_data_size);
		ଯଦି ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		ଅନ୍ୟଥା
			sz = -data_size;;
		/* extend this key? */
		ଯଦି (data->ଚାବିଗୁଡ଼ିକ_in_block) {
			ଚାବି = data->keyb->ଚାବିଗୁଡ଼ିକ + data->ଚାବିଗୁଡ଼ିକ_in_block - 1;
			keysz = ext2fs_le32_to_cpu(ଚାବି->size);
		} ଅନ୍ୟଥା {
			ଚାବି = ଖାଲି;
			keysz = 0;
		}
		ଯଦି (ଚାବି != ଖାଲି &&
		    (ext2fs_le64_to_cpu(ଚାବି->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_ଅଙ୍କ &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(ଚାବି->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			ଚାବି->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ଚାବି->size = ext2fs_cpu_to_le32(keysz + data_size);
		} ଅନ୍ୟଥା {
			data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ++;
			ଚାବି = data->keyb->ଚାବିଗୁଡ଼ିକ + data->ଚାବିଗୁଡ଼ିକ_in_block;
			data->ଚାବିଗୁଡ଼ିକ_in_block++;
			ଚାବି->fsblk = ext2fs_cpu_to_le64(backing_blk_ଅଙ୍କ);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			ଚାବି->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ଚାବି->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_ଅଙ୍କ,
		       data->undo_blk_ଅଙ୍କ,
		       sz, data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_ଅଙ୍କ, sz, read_ptr);
		ଯଦି (retval) {
			ମୁକ୍ତ(read_ptr);
			ଫେରନ୍ତୁ retval;
		}
		data->undo_blk_ଅଙ୍କ++;
		ମୁକ୍ତ(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		/* Next block */
		block_ଅଙ୍କ++;
	}

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 ATTR((unused)),
				    ଶୂନ୍ୟ *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	ଫେରନ୍ତୁ error;
}

ସ୍ଥିର ଶୂନ୍ୟ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_filesystem(ସଂରଚନା undo_header *hdr, io_channel undo_file,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize, blk64_t super_block,
			    io_channel channel)
{
	ସଂରଚନା ext2_super_block super, *sb;
	ଅକ୍ଷର *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	ଯଦି (retval)
		ଯାଅ out;
	sb = (ସଂରଚନା ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	ଯଦି (memcmp(&super, buf, ଆକାର(super))) {
		retval = -1;
		ଯାଅ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)buf, SUPERBLOCK_SIZE);
	ଯଦି (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		ଯାଅ out;
	}

out:
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ସ୍ଥିର errcode_t try_reopen_undo_file(ପୂର୍ଣ୍ଣସଂଖ୍ୟା undo_fd,
				      ସଂରଚନା undo_private_data *data)
{
	ସଂରଚନା undo_header hdr;
	ସଂରଚନା undo_ଚାବି *dkey;
	ext2fs_struct_stat statbuf;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, ଚାବିଗୁଡ଼ିକ_per_block, i;
	__u32 hdr_crc, ଚାବି_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	ଯଦି (retval)
		ଯାଅ bad_file;
	ଯଦି (statbuf.st_size == 0)
		ଯାଅ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(hdr),
				       &hdr);
	ଯଦି (retval)
		ଯାଅ bad_file;

	ଯଦି (memcmp(hdr.magic, E2UNDO_MAGIC,
		    ଆକାର(hdr.magic)))
		ଯାଅ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)&hdr,
				   ଆକାର(ସଂରଚନା undo_header) -
				   ଆକାର(__u32));
	ଯଦି (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		ଯାଅ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	ଯଦି (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		ଯାଅ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = ext2fs_le64_to_cpu(hdr.ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	ଯଦି (hdr.f_incompat || hdr.f_rocompat)
		ଯାଅ bad_file;

	/* Superblock matches this FS? */
	ଯଦି (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		ଯାଅ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	ଯଦି (retval)
		ଯାଅ bad_file;
	data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ;
	data->super_blk_ଅଙ୍କ = super_block;
	data->first_ଚାବି_blk = ext2fs_le64_to_cpu(hdr.ଚାବି_offset);

	/* load the written block map */
	ଚାବିଗୁଡ଼ିକ_per_block = ଚାବିଗୁଡ଼ିକ_PER_BLOCK(data);
	lblk = data->first_ଚାବି_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, ଚାବିଗୁଡ଼ିକ_per_block, blocksize);
	ପାଇଁ (i = 0; i < ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ; i += ଚାବିଗୁଡ଼ିକ_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->ଚାବି_blk_ଅଙ୍କ = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		ଯଦି (retval)
			ଯାଅ bad_ଚାବି_replay;

		/* check keys */
		ଯଦି (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ଯାଅ bad_ଚାବି_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		ଚାବି_crc = ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)data->keyb,
					   blocksize);
		ଯଦି (ext2fs_le32_to_cpu(crc) != ଚାବି_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ଯାଅ bad_ଚାବି_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ - i;
		ଯଦି (max_j > ଚାବିଗୁଡ଼ିକ_per_block)
			max_j = ଚାବିଗୁଡ଼ିକ_per_block;
		ପାଇଁ (j = 0, dkey = data->keyb->ଚାବିଗୁଡ଼ିକ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_ଅଙ୍କ = lblk;
			data->ଚାବିଗୁଡ଼ିକ_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->ଚାବି_blk_ଅଙ୍କ, data->undo_blk_ଅଙ୍କ, data->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ,
		   data->ଚାବିଗୁଡ଼ିକ_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	ଫେରନ୍ତୁ retval;

bad_ଚାବି_replay:
	data->ଚାବି_blk_ଅଙ୍କ = data->undo_blk_ଅଙ୍କ = 0;
	data->ଚାବିଗୁଡ଼ିକ_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	ଯାଅ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଶୂନ୍ୟ undo_atexit(ଶୂନ୍ୟ *p)
{
	ସଂରଚନା undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

ସ୍ଥିର errcode_t undo_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	io_channel	io = ଖାଲି;
	ସଂରଚନା undo_private_data *data = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	ଯଦି (name == 0)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_io_channel), &io);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(io, 0, ଆକାର(ସଂରଚନା struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା undo_private_data), &data);
	ଯଦି (retval)
		ଯାଅ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	ଯଦି (retval)
		ଯାଅ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, ଆକାର(ସଂରଚନା undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_ଅଙ୍କ = 1;
	data->first_ଚାବି_blk = 2;
	data->undo_blk_ଅଙ୍କ = 3;

	ଯଦି (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		ଯଦି (retval)
			ଯାଅ cleanup;

		data->tdb_file = strdup(tdb_file);
		ଯଦି (data->tdb_file == ଖାଲି)
			ଯାଅ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		ଯଦି (undo_fd < 0)
			ଯାଅ cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		ଯଦି (retval)
			ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		data->real = ଖାଲି;
		data->undo_file = ଖାଲି;
	}

	ଯଦି (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	ଯଦି (data->real)
		undo_err_handler_init(data->real);

	ଯଦି (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	ଯଦି (retval)
		ଯାଅ cleanup;

	*channel = io;
	ଯଦି (undo_fd >= 0)
		close(undo_fd);
	ଫେରନ୍ତୁ retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	ଯଦି (undo_fd >= 0)
		close(undo_fd);
	ଯଦି (data && data->undo_file)
		io_channel_close(data->undo_file);
	ଯଦି (data && data->tdb_file)
		ମୁକ୍ତ(data->tdb_file);
	ଯଦି (data && data->real)
		io_channel_close(data->real);
	ଯଦି (data)
		ext2fs_free_mem(&data);
	ଯଦି (io && io->name)
		ext2fs_free_mem(&io->name);
	ଯଦି (io)
		ext2fs_free_mem(&io);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_close(io_channel channel)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
		ଫେରନ୍ତୁ 0;
	/* Before closing write the file system identity */
	ଯଦି (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	ଯଦି (data->real)
		retval = io_channel_close(data->real);
	ଯଦି (data->tdb_file)
		ମୁକ୍ତ(data->tdb_file);
	ଯଦି (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	ଯଦି (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	ଯଦି (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	ଯଦି (err)
		ଫେରନ୍ତୁ err;
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	ଯଦି (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	errcode_t	retval = 0;
	ସଂରଚନା undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_read_blk64(data->real, block, ଗଣନା_2, buf);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ undo_read_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, ଗଣନା_2);
	ଯଦି (retval)
		 ଫେରନ୍ତୁ retval;
	ଯଦି (data->real)
		retval = io_channel_write_blk64(data->real, block, ଗଣନା_2, buf);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ undo_write_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_ଅଙ୍କ, ଗଣନା_2;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_ଅଙ୍କ = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ଗଣନା_2 = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_ଅଙ୍କ, ଗଣନା_2);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (ଗଣନା_2 > INT_MAX)
		ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
	icount = ଗଣନା_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (data->real)
		retval = io_channel_discard(data->real, block, ଗଣନା_2);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (ଗଣନା_2 > INT_MAX)
		ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
	icount = ଗଣନା_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (data->real)
		retval = io_channel_zeroout(data->real, block, ଗଣନା_2);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ଗଣନା_2)
{
	ସଂରଚନା undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_cache_readahead(data->real, block, ଗଣନା_2);

	ଫେରନ୍ତୁ retval;
}

/*
 * Flush data buffers to disk.
 */
ସ୍ଥିର errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ସଂରଚନା undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (data->real)
		retval = io_channel_flush(data->real);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_set_option(io_channel channel, ଅଚଳ ଅକ୍ଷର *option,
				 ଅଚଳ ଅକ୍ଷର *arg)
{
	errcode_t	retval = 0;
	ସଂରଚନା undo_private_data *data;
	unsigned long tmp;
	ଅକ୍ଷର *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (!strcmp(option, "tdb_data_size")) {
		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		ଯଦି (*end)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		ଯଦି (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		ଯଦି (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		ଫେରନ୍ତୁ 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	ଯଦି (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	ଯଦି (!retval && !strcmp(option, "offset")) {
		ଯଦି (!arg)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		ଯଦି (*end)
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ସଂରଚନା undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ସଂରଚନା undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ଯଦି (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_undo_manager = {
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
