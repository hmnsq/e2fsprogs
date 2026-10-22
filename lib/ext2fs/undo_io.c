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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#ifdef __linux__
#અન્તર્ભાવ <sys/utsname.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_SYS_RESOURCE_H
#અન્તર્ભાવ <sys/resource.h>
#endif
#અન્તર્ભાવ <limits.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#નહીંતર
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  કરો {printf(f, ## a); fflush(stdout); } જ્યારે (0)
#નહીંતર
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)
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

સંરચના undo_header {
	અક્ષર magic[8];		/* "E2UNDO02" */
	__le64 અંક_ચાવીઓ;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 ચાવી_offset;	/* where do the key/data block chunks start? */
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

સંરચના undo_ચાવી {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

સંરચના undo_ચાવી_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#જો __STDC_VERSION__ >= 199901L
	સંરચના undo_ચાવી ચાવીઓ[];		/* keys, which come immediately after */
#નહીંતર
	સંરચના undo_ચાવી ચાવીઓ[0];	/* keys, which come immediately after */
#endif
};

સંરચના undo_private_data {
	પૂર્ણાંક	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_અંક;			/* next free block */
	blk64_t ચાવી_blk_અંક;			/* current key block location */
	blk64_t super_blk_અંક;			/* superblock location */
	blk64_t first_ચાવી_blk;			/* first key block location */
	સંરચના undo_ચાવી_block *keyb;
	size_t અંક_ચાવીઓ, ચાવીઓ_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	પૂર્ણાંક tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	સંરચના struct_ext2_filsys fake_fs;
	અક્ષર *tdb_file;
	સંરચના undo_header hdr;
};
#define ચાવીઓ_PER_BLOCK(d) (((d)->tdb_data_size / કદ(સંરચના undo_ચાવી)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

સ્થિર અંતઃરેખ શૂન્ય e2undo_set_feature_fs_offset(સંરચના undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

સ્થિર અંતઃરેખ શૂન્ય e2undo_clear_feature_fs_offset(સંરચના undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

સ્થિર io_manager undo_io_backing_manager;
સ્થિર અક્ષર *tdb_file;
સ્થિર પૂર્ણાંક actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	પાછા 0;
}

errcode_t set_undo_io_backup_file(અક્ષર *file_name)
{
	tdb_file = strdup(file_name);

	જો (tdb_file == ખાલી) {
		પાછા EXT2_ET_NO_MEMORY;
	}

	પાછા 0;
}

સ્થિર errcode_t write_undo_indexes(સંરચના undo_private_data *data, પૂર્ણાંક flush)
{
	errcode_t retval;
	સંરચના ext2_super_block super;
	io_channel channel;
	પૂર્ણાંક block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	જો (data->ચાવીઓ_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned અક્ષર *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->ચાવી_blk_અંક);
		retval = io_channel_write_blk64(data->undo_file,
						data->ચાવી_blk_અંક,
						1, data->keyb);
		જો (retval)
			પાછા retval;
		/* Move on to the next key block if it's full. */
		જો (data->ચાવીઓ_in_block == ચાવીઓ_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->ચાવીઓ_in_block = 0;
			data->ચાવી_blk_અંક = data->undo_blk_અંક;
			data->undo_blk_અંક++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	જો (retval)
		જાઓ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned અક્ષર *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, કદ(data->hdr.magic));
	data->hdr.અંક_ચાવીઓ = ext2fs_cpu_to_le64(data->અંક_ચાવીઓ);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_અંક);
	data->hdr.ચાવી_offset = ext2fs_cpu_to_le64(data->first_ચાવી_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	જો (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	નહીંતર
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned અક્ષર *)&data->hdr,
				   કદ(data->hdr) -
				   કદ(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(પૂર્ણાંક)કદ(data->hdr),
					&data->hdr);
	જો (retval)
		જાઓ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_અંક);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_અંક,
					-SUPERBLOCK_SIZE, &super);
	જો (retval)
		જાઓ err_out;

	જો (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	પાછા retval;
}

સ્થિર errcode_t undo_setup_tdb(સંરચના undo_private_data *data)
{
	પૂર્ણાંક i;
	errcode_t retval;

	જો (data->tdb_written == 1)
		પાછા 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, કદ(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	જો (retval)
		પાછા retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	જો (retval)
		પાછા retval;
	data->ચાવી_blk_અંક = data->first_ચાવી_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", ચાવીઓ_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	માટે (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		જો (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		જો (retval)
			પાછા retval;
		retval = io_channel_flush(data->undo_file);
		જો (retval)
			પાછા retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	પાછા 0;
}

સ્થિર errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, પૂર્ણાંક ગણતરી)

{
	પૂર્ણાંક size, sz;
	unsigned long long block_અંક, backing_blk_અંક;
	errcode_t retval = 0;
	ext2_loff_t offset;
	સંરચના undo_private_data *data;
	unsigned અક્ષર *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	સંરચના undo_ચાવી *ચાવી;
	__u32 blk_crc;

	data = (સંરચના undo_private_data *) channel->private_data;

	જો (data->undo_file == ખાલી) {
		/*
		 * Transaction database not initialized
		 */
		પાછા 0;
	}

	જો (ગણતરી == 1)
		size = channel->block_size;
	નહીંતર {
		જો (ગણતરી < 0)
			size = -ગણતરી;
		નહીંતર
			size = ગણતરી * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	જો (retval)
		પાછા retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_અંક = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	જ્યારે (block_અંક <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		જો (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_અંક)) {
			/* Try the next block */
			block_અંક++;
			ચાલુ;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_અંક);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_અંક * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_અંક = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		જો (retval) {
			પાછા retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		જો ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		નહીંતર
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_અંક,
					     sz, read_ptr);
		જો (retval) {
			જો (retval != EXT2_ET_SHORT_READ) {
				મુક્ત(read_ptr);
				પાછા retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} નહીંતર {
			data_size = data->tdb_data_size;
		}
		જો (data_size == 0) {
			મુક્ત(read_ptr);
			block_અંક++;
			ચાલુ;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_અંક, block, data->tdb_data_size);
		જો ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		નહીંતર
			sz = -data_size;;
		/* extend this key? */
		જો (data->ચાવીઓ_in_block) {
			ચાવી = data->keyb->ચાવીઓ + data->ચાવીઓ_in_block - 1;
			keysz = ext2fs_le32_to_cpu(ચાવી->size);
		} નહીંતર {
			ચાવી = ખાલી;
			keysz = 0;
		}
		જો (ચાવી != ખાલી &&
		    (ext2fs_le64_to_cpu(ચાવી->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_અંક &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(ચાવી->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			ચાવી->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ચાવી->size = ext2fs_cpu_to_le32(keysz + data_size);
		} નહીંતર {
			data->અંક_ચાવીઓ++;
			ચાવી = data->keyb->ચાવીઓ + data->ચાવીઓ_in_block;
			data->ચાવીઓ_in_block++;
			ચાવી->fsblk = ext2fs_cpu_to_le64(backing_blk_અંક);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			ચાવી->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			ચાવી->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_અંક,
		       data->undo_blk_અંક,
		       sz, data->અંક_ચાવીઓ - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_અંક, sz, read_ptr);
		જો (retval) {
			મુક્ત(read_ptr);
			પાછા retval;
		}
		data->undo_blk_અંક++;
		મુક્ત(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		જો (retval)
			પાછા retval;

		/* Next block */
		block_અંક++;
	}

	પાછા retval;
}

સ્થિર errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    પૂર્ણાંક ગણતરી ATTR((unused)),
				    શૂન્ય *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    પૂર્ણાંક actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	પાછા error;
}

સ્થિર શૂન્ય undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

સ્થિર પૂર્ણાંક check_filesystem(સંરચના undo_header *hdr, io_channel undo_file,
			    unsigned પૂર્ણાંક blocksize, blk64_t super_block,
			    io_channel channel)
{
	સંરચના ext2_super_block super, *sb;
	અક્ષર *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	જો (retval)
		પાછા retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	જો (retval)
		પાછા retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	જો (retval)
		જાઓ out;
	sb = (સંરચના ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	જો (memcmp(&super, buf, કદ(super))) {
		retval = -1;
		જાઓ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned અક્ષર *)buf, SUPERBLOCK_SIZE);
	જો (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		જાઓ out;
	}

out:
	ext2fs_free_mem(&buf);
	પાછા retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
સ્થિર errcode_t try_reopen_undo_file(પૂર્ણાંક undo_fd,
				      સંરચના undo_private_data *data)
{
	સંરચના undo_header hdr;
	સંરચના undo_ચાવી *dkey;
	ext2fs_struct_stat statbuf;
	unsigned પૂર્ણાંક blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t અંક_ચાવીઓ, ચાવીઓ_per_block, i;
	__u32 hdr_crc, ચાવી_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	જો (retval)
		જાઓ bad_file;
	જો (statbuf.st_size == 0)
		જાઓ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(પૂર્ણાંક)કદ(hdr),
				       &hdr);
	જો (retval)
		જાઓ bad_file;

	જો (memcmp(hdr.magic, E2UNDO_MAGIC,
		    કદ(hdr.magic)))
		જાઓ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned અક્ષર *)&hdr,
				   કદ(સંરચના undo_header) -
				   કદ(__u32));
	જો (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		જાઓ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	જો (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		જાઓ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	અંક_ચાવીઓ = ext2fs_le64_to_cpu(hdr.અંક_ચાવીઓ);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	જો (hdr.f_incompat || hdr.f_rocompat)
		જાઓ bad_file;

	/* Superblock matches this FS? */
	જો (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		જાઓ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	જો (retval)
		જાઓ bad_file;
	data->અંક_ચાવીઓ = અંક_ચાવીઓ;
	data->super_blk_અંક = super_block;
	data->first_ચાવી_blk = ext2fs_le64_to_cpu(hdr.ચાવી_offset);

	/* load the written block map */
	ચાવીઓ_per_block = ચાવીઓ_PER_BLOCK(data);
	lblk = data->first_ચાવી_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   અંક_ચાવીઓ, ચાવીઓ_per_block, blocksize);
	માટે (i = 0; i < અંક_ચાવીઓ; i += ચાવીઓ_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->ચાવી_blk_અંક = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		જો (retval)
			જાઓ bad_ચાવી_replay;

		/* check keys */
		જો (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			જાઓ bad_ચાવી_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		ચાવી_crc = ext2fs_crc32c_le(~0, (unsigned અક્ષર *)data->keyb,
					   blocksize);
		જો (ext2fs_le32_to_cpu(crc) != ચાવી_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			જાઓ bad_ચાવી_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->અંક_ચાવીઓ - i;
		જો (max_j > ચાવીઓ_per_block)
			max_j = ચાવીઓ_per_block;
		માટે (j = 0, dkey = data->keyb->ચાવીઓ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_અંક = lblk;
			data->ચાવીઓ_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->ચાવી_blk_અંક, data->undo_blk_અંક, data->અંક_ચાવીઓ,
		   data->ચાવીઓ_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	પાછા retval;

bad_ચાવી_replay:
	data->ચાવી_blk_અંક = data->undo_blk_અંક = 0;
	data->ચાવીઓ_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	જાઓ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	પાછા retval;
}

સ્થિર શૂન્ય undo_atexit(શૂન્ય *p)
{
	સંરચના undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

સ્થિર errcode_t undo_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	io_channel	io = ખાલી;
	સંરચના undo_private_data *data = ખાલી;
	પૂર્ણાંક		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	જો (name == 0)
		પાછા EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(કદ(સંરચના struct_io_channel), &io);
	જો (retval)
		જાઓ cleanup;
	memset(io, 0, કદ(સંરચના struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(કદ(સંરચના undo_private_data), &data);
	જો (retval)
		જાઓ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	જો (retval)
		જાઓ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, કદ(સંરચના undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_અંક = 1;
	data->first_ચાવી_blk = 2;
	data->undo_blk_અંક = 3;

	જો (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		જો (retval)
			જાઓ cleanup;

		data->tdb_file = strdup(tdb_file);
		જો (data->tdb_file == ખાલી)
			જાઓ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		જો (undo_fd < 0)
			જાઓ cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		જો (retval)
			જાઓ cleanup;
	} નહીંતર {
		data->real = ખાલી;
		data->undo_file = ખાલી;
	}

	જો (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	જો (data->real)
		undo_err_handler_init(data->real);

	જો (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		જો (retval)
			જાઓ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	જો (retval)
		જાઓ cleanup;

	*channel = io;
	જો (undo_fd >= 0)
		close(undo_fd);
	પાછા retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	જો (undo_fd >= 0)
		close(undo_fd);
	જો (data && data->undo_file)
		io_channel_close(data->undo_file);
	જો (data && data->tdb_file)
		મુક્ત(data->tdb_file);
	જો (data && data->real)
		io_channel_close(data->real);
	જો (data)
		ext2fs_free_mem(&data);
	જો (io && io->name)
		ext2fs_free_mem(&io->name);
	જો (io)
		ext2fs_free_mem(&io);
	પાછા retval;
}

સ્થિર errcode_t undo_close(io_channel channel)
{
	સંરચના undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (--channel->refcount > 0)
		પાછા 0;
	/* Before closing write the file system identity */
	જો (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	જો (data->real)
		retval = io_channel_close(data->real);
	જો (data->tdb_file)
		મુક્ત(data->tdb_file);
	જો (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	જો (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	જો (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	જો (err)
		પાછા err;
	પાછા retval;
}

સ્થિર errcode_t undo_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	સંરચના undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	જો (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	પાછા retval;
}

સ્થિર errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	errcode_t	retval = 0;
	સંરચના undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_read_blk64(data->real, block, ગણતરી, buf);

	પાછા retval;
}

સ્થિર errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પાછા undo_read_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	સંરચના undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, ગણતરી);
	જો (retval)
		 પાછા retval;
	જો (data->real)
		retval = io_channel_write_blk64(data->real, block, ગણતરી, buf);

	પાછા retval;
}

સ્થિર errcode_t undo_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પાછા undo_write_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 પૂર્ણાંક size, અચલ શૂન્ય *buf)
{
	સંરચના undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_અંક, ગણતરી;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_અંક = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ગણતરી = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_અંક, ગણતરી);
	જો (retval)
		પાછા retval;
	જો (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	પાછા retval;
}

સ્થિર errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના undo_private_data *data;
	errcode_t	retval = 0;
	પૂર્ણાંક icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (ગણતરી > INT_MAX)
		પાછા EXT2_ET_UNIMPLEMENTED;
	icount = ગણતરી;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	જો (retval)
		પાછા retval;
	જો (data->real)
		retval = io_channel_discard(data->real, block, ગણતરી);

	પાછા retval;
}

સ્થિર errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long ગણતરી)
{
	સંરચના undo_private_data *data;
	errcode_t	retval = 0;
	પૂર્ણાંક icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (ગણતરી > INT_MAX)
		પાછા EXT2_ET_UNIMPLEMENTED;
	icount = ગણતરી;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	જો (retval)
		પાછા retval;
	જો (data->real)
		retval = io_channel_zeroout(data->real, block, ગણતરી);

	પાછા retval;
}

સ્થિર errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long ગણતરી)
{
	સંરચના undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_cache_readahead(data->real, block, ગણતરી);

	પાછા retval;
}

/*
 * Flush data buffers to disk.
 */
સ્થિર errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	સંરચના undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (data->real)
		retval = io_channel_flush(data->real);

	પાછા retval;
}

સ્થિર errcode_t undo_set_option(io_channel channel, અચલ અક્ષર *option,
				 અચલ અક્ષર *arg)
{
	errcode_t	retval = 0;
	સંરચના undo_private_data *data;
	unsigned long tmp;
	અક્ષર *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (!strcmp(option, "tdb_data_size")) {
		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		જો (*end)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		જો (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		જો (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		પાછા 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	જો (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	જો (!retval && !strcmp(option, "offset")) {
		જો (!arg)
			પાછા EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		જો (*end)
			પાછા EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	પાછા retval;
}

સ્થિર errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	સંરચના undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (સંરચના undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	જો (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	પાછા retval;
}

સ્થિર સંરચના struct_io_manager struct_undo_manager = {
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
