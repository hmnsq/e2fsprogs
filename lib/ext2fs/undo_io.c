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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ifdef __linux__
#ضمن <sys/utsname.h>
#endif
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#endif
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#وإلا
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  افعل {printf(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(بنية, code) \
	  إذا ((بنية)->magic != (code)) أرجع (code)
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

بنية undo_header {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 رەقەم_ئاچقۇچلار;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 ئاچقۇچ_offset;	/* where do the key/data block chunks start? */
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

بنية undo_ئاچقۇچ {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

بنية undo_ئاچقۇچ_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#إذا __STDC_VERSION__ >= 199901L
	بنية undo_ئاچقۇچ ئاچقۇچلار[];		/* keys, which come immediately after */
#وإلا
	بنية undo_ئاچقۇچ ئاچقۇچلار[0];	/* keys, which come immediately after */
#endif
};

بنية undo_private_data {
	صحيح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_رەقەم;			/* next free block */
	blk64_t ئاچقۇچ_blk_رەقەم;			/* current key block location */
	blk64_t super_blk_رەقەم;			/* superblock location */
	blk64_t first_ئاچقۇچ_blk;			/* first key block location */
	بنية undo_ئاچقۇچ_block *keyb;
	size_t رەقەم_ئاچقۇچلار, ئاچقۇچلار_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	صحيح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	بنية struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	بنية undo_header hdr;
};
#define ئاچقۇچلار_PER_BLOCK(d) (((d)->tdb_data_size / حجم(بنية undo_ئاچقۇچ)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

ساكن مضمن فراغ e2undo_set_feature_fs_offset(بنية undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ساكن مضمن فراغ e2undo_clear_feature_fs_offset(بنية undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

ساكن io_manager undo_io_backing_manager;
ساكن حرف *tdb_file;
ساكن صحيح actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	أرجع 0;
}

errcode_t set_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	إذا (tdb_file == عدم) {
		أرجع EXT2_ET_NO_MEMORY;
	}

	أرجع 0;
}

ساكن errcode_t write_undo_indexes(بنية undo_private_data *data, صحيح flush)
{
	errcode_t retval;
	بنية ext2_super_block super;
	io_channel channel;
	صحيح block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	إذا (data->ئاچقۇچلار_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned حرف *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->ئاچقۇچ_blk_رەقەم);
		retval = io_channel_write_blk64(data->undo_file,
						data->ئاچقۇچ_blk_رەقەم,
						1, data->keyb);
		إذا (retval)
			أرجع retval;
		/* Move on to the next key block if it's full. */
		إذا (data->ئاچقۇچلار_in_block == ئاچقۇچلار_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->ئاچقۇچلار_in_block = 0;
			data->ئاچقۇچ_blk_رەقەم = data->undo_blk_رەقەم;
			data->undo_blk_رەقەم++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	إذا (retval)
		اذهب err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned حرف *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, حجم(data->hdr.magic));
	data->hdr.رەقەم_ئاچقۇچلار = ext2fs_cpu_to_le64(data->رەقەم_ئاچقۇچلار);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_رەقەم);
	data->hdr.ئاچقۇچ_offset = ext2fs_cpu_to_le64(data->first_ئاچقۇچ_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	إذا (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	وإلا
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned حرف *)&data->hdr,
				   حجم(data->hdr) -
				   حجم(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(صحيح)حجم(data->hdr),
					&data->hdr);
	إذا (retval)
		اذهب err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_رەقەم);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_رەقەم,
					-SUPERBLOCK_SIZE, &super);
	إذا (retval)
		اذهب err_out;

	إذا (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	أرجع retval;
}

ساكن errcode_t undo_setup_tdb(بنية undo_private_data *data)
{
	صحيح i;
	errcode_t retval;

	إذا (data->tdb_written == 1)
		أرجع 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, حجم(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	إذا (retval)
		أرجع retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	إذا (retval)
		أرجع retval;
	data->ئاچقۇچ_blk_رەقەم = data->first_ئاچقۇچ_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", ئاچقۇچلار_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	لكل (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		إذا (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		إذا (retval)
			أرجع retval;
		retval = io_channel_flush(data->undo_file);
		إذا (retval)
			أرجع retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	أرجع 0;
}

ساكن errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, صحيح سانى)

{
	صحيح size, sz;
	unsigned long long block_رەقەم, backing_blk_رەقەم;
	errcode_t retval = 0;
	ext2_loff_t offset;
	بنية undo_private_data *data;
	unsigned حرف *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	بنية undo_ئاچقۇچ *ئاچقۇچ;
	__u32 blk_crc;

	data = (بنية undo_private_data *) channel->private_data;

	إذا (data->undo_file == عدم) {
		/*
		 * Transaction database not initialized
		 */
		أرجع 0;
	}

	إذا (سانى == 1)
		size = channel->block_size;
	وإلا {
		إذا (سانى < 0)
			size = -سانى;
		وإلا
			size = سانى * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	إذا (retval)
		أرجع retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_رەقەم = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	طالما (block_رەقەم <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		إذا (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_رەقەم)) {
			/* Try the next block */
			block_رەقەم++;
			استمر;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_رەقەم);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_رەقەم * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_رەقەم = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		إذا (retval) {
			أرجع retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		إذا ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		وإلا
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_رەقەم,
					     sz, read_ptr);
		إذا (retval) {
			إذا (retval != EXT2_ET_SHORT_READ) {
				حرر(read_ptr);
				أرجع retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} وإلا {
			data_size = data->tdb_data_size;
		}
		إذا (data_size == 0) {
			حرر(read_ptr);
			block_رەقەم++;
			استمر;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_رەقەم, block, data->tdb_data_size);
		إذا ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		وإلا
			sz = -data_size;;
		/* extend this key? */
		إذا (data->ئاچقۇچلار_in_block) {
			ئاچقۇچ = data->keyb->ئاچقۇچلار + data->ئاچقۇچلار_in_block - 1;
			keysz = ext2fs_le32_to_cpu(ئاچقۇچ->size);
		} وإلا {
			ئاچقۇچ = عدم;
			keysz = 0;
		}
		إذا (ئاچقۇچ != عدم &&
		    (ext2fs_le64_to_cpu(ئاچقۇچ->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_رەقەم &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(ئاچقۇچ->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			ئاچقۇچ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ئاچقۇچ->size = ext2fs_cpu_to_le32(keysz + data_size);
		} وإلا {
			data->رەقەم_ئاچقۇچلار++;
			ئاچقۇچ = data->keyb->ئاچقۇچلار + data->ئاچقۇچلار_in_block;
			data->ئاچقۇچلار_in_block++;
			ئاچقۇچ->fsblk = ext2fs_cpu_to_le64(backing_blk_رەقەم);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			ئاچقۇچ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ئاچقۇچ->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_رەقەم,
		       data->undo_blk_رەقەم,
		       sz, data->رەقەم_ئاچقۇچلار - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_رەقەم, sz, read_ptr);
		إذا (retval) {
			حرر(read_ptr);
			أرجع retval;
		}
		data->undo_blk_رەقەم++;
		حرر(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		إذا (retval)
			أرجع retval;

		/* Next block */
		block_رەقەم++;
	}

	أرجع retval;
}

ساكن errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    صحيح سانى ATTR((unused)),
				    فراغ *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    صحيح actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	أرجع error;
}

ساكن فراغ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساكن صحيح check_filesystem(بنية undo_header *hdr, io_channel undo_file,
			    unsigned صحيح blocksize, blk64_t super_block,
			    io_channel channel)
{
	بنية ext2_super_block super, *sb;
	حرف *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	إذا (retval)
		أرجع retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	إذا (retval)
		أرجع retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	إذا (retval)
		اذهب out;
	sb = (بنية ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	إذا (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		اذهب out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned حرف *)buf, SUPERBLOCK_SIZE);
	إذا (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		اذهب out;
	}

out:
	ext2fs_free_mem(&buf);
	أرجع retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ساكن errcode_t try_reopen_undo_file(صحيح undo_fd,
				      بنية undo_private_data *data)
{
	بنية undo_header hdr;
	بنية undo_ئاچقۇچ *dkey;
	ext2fs_struct_stat statbuf;
	unsigned صحيح blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t رەقەم_ئاچقۇچلار, ئاچقۇچلار_per_block, i;
	__u32 hdr_crc, ئاچقۇچ_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	إذا (retval)
		اذهب bad_file;
	إذا (statbuf.st_size == 0)
		اذهب out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(صحيح)حجم(hdr),
				       &hdr);
	إذا (retval)
		اذهب bad_file;

	إذا (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		اذهب bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned حرف *)&hdr,
				   حجم(بنية undo_header) -
				   حجم(__u32));
	إذا (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		اذهب bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	إذا (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		اذهب bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	رەقەم_ئاچقۇچلار = ext2fs_le64_to_cpu(hdr.رەقەم_ئاچقۇچلار);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	إذا (hdr.f_incompat || hdr.f_rocompat)
		اذهب bad_file;

	/* Superblock matches this FS? */
	إذا (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		اذهب out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	إذا (retval)
		اذهب bad_file;
	data->رەقەم_ئاچقۇچلار = رەقەم_ئاچقۇچلار;
	data->super_blk_رەقەم = super_block;
	data->first_ئاچقۇچ_blk = ext2fs_le64_to_cpu(hdr.ئاچقۇچ_offset);

	/* load the written block map */
	ئاچقۇچلار_per_block = ئاچقۇچلار_PER_BLOCK(data);
	lblk = data->first_ئاچقۇچ_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   رەقەم_ئاچقۇچلار, ئاچقۇچلار_per_block, blocksize);
	لكل (i = 0; i < رەقەم_ئاچقۇچلار; i += ئاچقۇچلار_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->ئاچقۇچ_blk_رەقەم = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		إذا (retval)
			اذهب bad_ئاچقۇچ_replay;

		/* check keys */
		إذا (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_ئاچقۇچ_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		ئاچقۇچ_crc = ext2fs_crc32c_le(~0, (unsigned حرف *)data->keyb,
					   blocksize);
		إذا (ext2fs_le32_to_cpu(crc) != ئاچقۇچ_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_ئاچقۇچ_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->رەقەم_ئاچقۇچلار - i;
		إذا (max_j > ئاچقۇچلار_per_block)
			max_j = ئاچقۇچلار_per_block;
		لكل (j = 0, dkey = data->keyb->ئاچقۇچلار;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_رەقەم = lblk;
			data->ئاچقۇچلار_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->ئاچقۇچ_blk_رەقەم, data->undo_blk_رەقەم, data->رەقەم_ئاچقۇچلار,
		   data->ئاچقۇچلار_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	أرجع retval;

bad_ئاچقۇچ_replay:
	data->ئاچقۇچ_blk_رەقەم = data->undo_blk_رەقەم = 0;
	data->ئاچقۇچلار_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	اذهب out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	أرجع retval;
}

ساكن فراغ undo_atexit(فراغ *p)
{
	بنية undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

ساكن errcode_t undo_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية undo_private_data *data = عدم;
	صحيح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية undo_private_data), &data);
	إذا (retval)
		اذهب cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, حجم(بنية undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_رەقەم = 1;
	data->first_ئاچقۇچ_blk = 2;
	data->undo_blk_رەقەم = 3;

	إذا (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		إذا (retval)
			اذهب cleanup;

		data->tdb_file = strdup(tdb_file);
		إذا (data->tdb_file == عدم)
			اذهب cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		إذا (undo_fd < 0)
			اذهب cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		data->real = عدم;
		data->undo_file = عدم;
	}

	إذا (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	إذا (data->real)
		undo_err_handler_init(data->real);

	إذا (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		إذا (retval)
			اذهب cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	إذا (undo_fd >= 0)
		close(undo_fd);
	أرجع retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	إذا (undo_fd >= 0)
		close(undo_fd);
	إذا (data && data->undo_file)
		io_channel_close(data->undo_file);
	إذا (data && data->tdb_file)
		حرر(data->tdb_file);
	إذا (data && data->real)
		io_channel_close(data->real);
	إذا (data)
		ext2fs_free_mem(&data);
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t undo_close(io_channel channel)
{
	بنية undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;
	/* Before closing write the file system identity */
	إذا (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	إذا (data->real)
		retval = io_channel_close(data->real);
	إذا (data->tdb_file)
		حرر(data->tdb_file);
	إذا (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	إذا (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	إذا (err)
		أرجع err;
	أرجع retval;
}

ساكن errcode_t undo_set_blksize(io_channel channel, صحيح blksize)
{
	بنية undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	إذا (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	إذا (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	أرجع retval;
}

ساكن errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       صحيح سانى, فراغ *buf)
{
	errcode_t	retval = 0;
	بنية undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_read_blk64(data->real, block, سانى, buf);

	أرجع retval;
}

ساكن errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       صحيح سانى, فراغ *buf)
{
	أرجع undo_read_blk64(channel, block, سانى, buf);
}

ساكن errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				صحيح سانى, ثابت فراغ *buf)
{
	بنية undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, سانى);
	إذا (retval)
		 أرجع retval;
	إذا (data->real)
		retval = io_channel_write_blk64(data->real, block, سانى, buf);

	أرجع retval;
}

ساكن errcode_t undo_write_blk(io_channel channel, unsigned long block,
				صحيح سانى, ثابت فراغ *buf)
{
	أرجع undo_write_blk64(channel, block, سانى, buf);
}

ساكن errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 صحيح size, ثابت فراغ *buf)
{
	بنية undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_رەقەم, سانى;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_رەقەم = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	سانى = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_رەقەم, سانى);
	إذا (retval)
		أرجع retval;
	إذا (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	أرجع retval;
}

ساكن errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long سانى)
{
	بنية undo_private_data *data;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (سانى > INT_MAX)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = سانى;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	إذا (retval)
		أرجع retval;
	إذا (data->real)
		retval = io_channel_discard(data->real, block, سانى);

	أرجع retval;
}

ساكن errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long سانى)
{
	بنية undo_private_data *data;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (سانى > INT_MAX)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = سانى;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	إذا (retval)
		أرجع retval;
	إذا (data->real)
		retval = io_channel_zeroout(data->real, block, سانى);

	أرجع retval;
}

ساكن errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long سانى)
{
	بنية undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_cache_readahead(data->real, block, سانى);

	أرجع retval;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	بنية undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (data->real)
		retval = io_channel_flush(data->real);

	أرجع retval;
}

ساكن errcode_t undo_set_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	بنية undo_private_data *data;
	unsigned long tmp;
	حرف *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "tdb_data_size")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		إذا (*end)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		إذا (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		إذا (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		أرجع 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	إذا (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	إذا (!retval && !strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		إذا (*end)
			أرجع EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	أرجع retval;
}

ساكن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	بنية undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (بنية undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	أرجع retval;
}

ساكن بنية struct_io_manager struct_undo_manager = {
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
