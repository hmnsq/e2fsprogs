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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#ifdef __linux__
#അന്തര്ഭാവ <sys/utsname.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_SYS_RESOURCE_H
#അന്തര്ഭാവ <sys/resource.h>
#endif
#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#അല്ലെങ്കിൽ
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  ചെയ്യുക {printf(f, ## a); fflush(stdout); } ആകുമ്പോൾ (0)
#അല്ലെങ്കിൽ
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)
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

ഘടന undo_header {
	അക്ഷരം magic[8];		/* "E2UNDO02" */
	__le64 അക്കം_താക്കോലുകൾ;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 താക്കോൽ_offset;	/* where do the key/data block chunks start? */
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

ഘടന undo_താക്കോൽ {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

ഘടന undo_താക്കോൽ_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#എങ്കിൽ __STDC_VERSION__ >= 199901L
	ഘടന undo_താക്കോൽ താക്കോലുകൾ[];		/* keys, which come immediately after */
#അല്ലെങ്കിൽ
	ഘടന undo_താക്കോൽ താക്കോലുകൾ[0];	/* keys, which come immediately after */
#endif
};

ഘടന undo_private_data {
	പൂർണ്ണസംഖ്യ	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_അക്കം;			/* next free block */
	blk64_t താക്കോൽ_blk_അക്കം;			/* current key block location */
	blk64_t super_blk_അക്കം;			/* superblock location */
	blk64_t first_താക്കോൽ_blk;			/* first key block location */
	ഘടന undo_താക്കോൽ_block *keyb;
	size_t അക്കം_താക്കോലുകൾ, താക്കോലുകൾ_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	പൂർണ്ണസംഖ്യ tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	ഘടന struct_ext2_filsys fake_fs;
	അക്ഷരം *tdb_file;
	ഘടന undo_header hdr;
};
#define താക്കോലുകൾ_PER_BLOCK(d) (((d)->tdb_data_size / വലുപ്പം(ഘടന undo_താക്കോൽ)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

സ്ഥിതം വരിയിൽ ശൂന്യം e2undo_set_feature_fs_offset(ഘടന undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

സ്ഥിതം വരിയിൽ ശൂന്യം e2undo_clear_feature_fs_offset(ഘടന undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

സ്ഥിതം io_manager undo_io_backing_manager;
സ്ഥിതം അക്ഷരം *tdb_file;
സ്ഥിതം പൂർണ്ണസംഖ്യ actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	തിരികെ 0;
}

errcode_t set_undo_io_backup_file(അക്ഷരം *file_name)
{
	tdb_file = strdup(file_name);

	എങ്കിൽ (tdb_file == ശൂന്യ) {
		തിരികെ EXT2_ET_NO_MEMORY;
	}

	തിരികെ 0;
}

സ്ഥിതം errcode_t write_undo_indexes(ഘടന undo_private_data *data, പൂർണ്ണസംഖ്യ flush)
{
	errcode_t retval;
	ഘടന ext2_super_block super;
	io_channel channel;
	പൂർണ്ണസംഖ്യ block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	എങ്കിൽ (data->താക്കോലുകൾ_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned അക്ഷരം *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->താക്കോൽ_blk_അക്കം);
		retval = io_channel_write_blk64(data->undo_file,
						data->താക്കോൽ_blk_അക്കം,
						1, data->keyb);
		എങ്കിൽ (retval)
			തിരികെ retval;
		/* Move on to the next key block if it's full. */
		എങ്കിൽ (data->താക്കോലുകൾ_in_block == താക്കോലുകൾ_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->താക്കോലുകൾ_in_block = 0;
			data->താക്കോൽ_blk_അക്കം = data->undo_blk_അക്കം;
			data->undo_blk_അക്കം++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	എങ്കിൽ (retval)
		പോകുക err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, വലുപ്പം(data->hdr.magic));
	data->hdr.അക്കം_താക്കോലുകൾ = ext2fs_cpu_to_le64(data->അക്കം_താക്കോലുകൾ);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_അക്കം);
	data->hdr.താക്കോൽ_offset = ext2fs_cpu_to_le64(data->first_താക്കോൽ_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	എങ്കിൽ (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	അല്ലെങ്കിൽ
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)&data->hdr,
				   വലുപ്പം(data->hdr) -
				   വലുപ്പം(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(പൂർണ്ണസംഖ്യ)വലുപ്പം(data->hdr),
					&data->hdr);
	എങ്കിൽ (retval)
		പോകുക err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_അക്കം);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_അക്കം,
					-SUPERBLOCK_SIZE, &super);
	എങ്കിൽ (retval)
		പോകുക err_out;

	എങ്കിൽ (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_setup_tdb(ഘടന undo_private_data *data)
{
	പൂർണ്ണസംഖ്യ i;
	errcode_t retval;

	എങ്കിൽ (data->tdb_written == 1)
		തിരികെ 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, വലുപ്പം(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	എങ്കിൽ (retval)
		തിരികെ retval;
	data->താക്കോൽ_blk_അക്കം = data->first_താക്കോൽ_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", താക്കോലുകൾ_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	വേണ്ടി (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		എങ്കിൽ (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		എങ്കിൽ (retval)
			തിരികെ retval;
		retval = io_channel_flush(data->undo_file);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	തിരികെ 0;
}

സ്ഥിതം errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം)

{
	പൂർണ്ണസംഖ്യ size, sz;
	unsigned long long block_അക്കം, backing_blk_അക്കം;
	errcode_t retval = 0;
	ext2_loff_t offset;
	ഘടന undo_private_data *data;
	unsigned അക്ഷരം *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	ഘടന undo_താക്കോൽ *താക്കോൽ;
	__u32 blk_crc;

	data = (ഘടന undo_private_data *) channel->private_data;

	എങ്കിൽ (data->undo_file == ശൂന്യ) {
		/*
		 * Transaction database not initialized
		 */
		തിരികെ 0;
	}

	എങ്കിൽ (എണ്ണം == 1)
		size = channel->block_size;
	അല്ലെങ്കിൽ {
		എങ്കിൽ (എണ്ണം < 0)
			size = -എണ്ണം;
		അല്ലെങ്കിൽ
			size = എണ്ണം * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	എങ്കിൽ (retval)
		തിരികെ retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_അക്കം = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	ആകുമ്പോൾ (block_അക്കം <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		എങ്കിൽ (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_അക്കം)) {
			/* Try the next block */
			block_അക്കം++;
			തുടരുക;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_അക്കം);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_അക്കം * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_അക്കം = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		എങ്കിൽ (retval) {
			തിരികെ retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		എങ്കിൽ ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		അല്ലെങ്കിൽ
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_അക്കം,
					     sz, read_ptr);
		എങ്കിൽ (retval) {
			എങ്കിൽ (retval != EXT2_ET_SHORT_READ) {
				സ്വതന്ത്രം(read_ptr);
				തിരികെ retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} അല്ലെങ്കിൽ {
			data_size = data->tdb_data_size;
		}
		എങ്കിൽ (data_size == 0) {
			സ്വതന്ത്രം(read_ptr);
			block_അക്കം++;
			തുടരുക;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_അക്കം, block, data->tdb_data_size);
		എങ്കിൽ ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		അല്ലെങ്കിൽ
			sz = -data_size;;
		/* extend this key? */
		എങ്കിൽ (data->താക്കോലുകൾ_in_block) {
			താക്കോൽ = data->keyb->താക്കോലുകൾ + data->താക്കോലുകൾ_in_block - 1;
			keysz = ext2fs_le32_to_cpu(താക്കോൽ->size);
		} അല്ലെങ്കിൽ {
			താക്കോൽ = ശൂന്യ;
			keysz = 0;
		}
		എങ്കിൽ (താക്കോൽ != ശൂന്യ &&
		    (ext2fs_le64_to_cpu(താക്കോൽ->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_അക്കം &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(താക്കോൽ->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			താക്കോൽ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			താക്കോൽ->size = ext2fs_cpu_to_le32(keysz + data_size);
		} അല്ലെങ്കിൽ {
			data->അക്കം_താക്കോലുകൾ++;
			താക്കോൽ = data->keyb->താക്കോലുകൾ + data->താക്കോലുകൾ_in_block;
			data->താക്കോലുകൾ_in_block++;
			താക്കോൽ->fsblk = ext2fs_cpu_to_le64(backing_blk_അക്കം);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			താക്കോൽ->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			താക്കോൽ->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_അക്കം,
		       data->undo_blk_അക്കം,
		       sz, data->അക്കം_താക്കോലുകൾ - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_അക്കം, sz, read_ptr);
		എങ്കിൽ (retval) {
			സ്വതന്ത്രം(read_ptr);
			തിരികെ retval;
		}
		data->undo_blk_അക്കം++;
		സ്വതന്ത്രം(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		എങ്കിൽ (retval)
			തിരികെ retval;

		/* Next block */
		block_അക്കം++;
	}

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    പൂർണ്ണസംഖ്യ എണ്ണം ATTR((unused)),
				    ശൂന്യം *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    പൂർണ്ണസംഖ്യ actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	തിരികെ error;
}

സ്ഥിതം ശൂന്യം undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_filesystem(ഘടന undo_header *hdr, io_channel undo_file,
			    unsigned പൂർണ്ണസംഖ്യ blocksize, blk64_t super_block,
			    io_channel channel)
{
	ഘടന ext2_super_block super, *sb;
	അക്ഷരം *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	എങ്കിൽ (retval)
		പോകുക out;
	sb = (ഘടന ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	എങ്കിൽ (memcmp(&super, buf, വലുപ്പം(super))) {
		retval = -1;
		പോകുക out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)buf, SUPERBLOCK_SIZE);
	എങ്കിൽ (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		പോകുക out;
	}

out:
	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
സ്ഥിതം errcode_t try_reopen_undo_file(പൂർണ്ണസംഖ്യ undo_fd,
				      ഘടന undo_private_data *data)
{
	ഘടന undo_header hdr;
	ഘടന undo_താക്കോൽ *dkey;
	ext2fs_struct_stat statbuf;
	unsigned പൂർണ്ണസംഖ്യ blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t അക്കം_താക്കോലുകൾ, താക്കോലുകൾ_per_block, i;
	__u32 hdr_crc, താക്കോൽ_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	എങ്കിൽ (retval)
		പോകുക bad_file;
	എങ്കിൽ (statbuf.st_size == 0)
		പോകുക out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(പൂർണ്ണസംഖ്യ)വലുപ്പം(hdr),
				       &hdr);
	എങ്കിൽ (retval)
		പോകുക bad_file;

	എങ്കിൽ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    വലുപ്പം(hdr.magic)))
		പോകുക bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)&hdr,
				   വലുപ്പം(ഘടന undo_header) -
				   വലുപ്പം(__u32));
	എങ്കിൽ (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		പോകുക bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	എങ്കിൽ (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		പോകുക bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	അക്കം_താക്കോലുകൾ = ext2fs_le64_to_cpu(hdr.അക്കം_താക്കോലുകൾ);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	എങ്കിൽ (hdr.f_incompat || hdr.f_rocompat)
		പോകുക bad_file;

	/* Superblock matches this FS? */
	എങ്കിൽ (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		പോകുക out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	എങ്കിൽ (retval)
		പോകുക bad_file;
	data->അക്കം_താക്കോലുകൾ = അക്കം_താക്കോലുകൾ;
	data->super_blk_അക്കം = super_block;
	data->first_താക്കോൽ_blk = ext2fs_le64_to_cpu(hdr.താക്കോൽ_offset);

	/* load the written block map */
	താക്കോലുകൾ_per_block = താക്കോലുകൾ_PER_BLOCK(data);
	lblk = data->first_താക്കോൽ_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   അക്കം_താക്കോലുകൾ, താക്കോലുകൾ_per_block, blocksize);
	വേണ്ടി (i = 0; i < അക്കം_താക്കോലുകൾ; i += താക്കോലുകൾ_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->താക്കോൽ_blk_അക്കം = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		എങ്കിൽ (retval)
			പോകുക bad_താക്കോൽ_replay;

		/* check keys */
		എങ്കിൽ (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			പോകുക bad_താക്കോൽ_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		താക്കോൽ_crc = ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)data->keyb,
					   blocksize);
		എങ്കിൽ (ext2fs_le32_to_cpu(crc) != താക്കോൽ_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			പോകുക bad_താക്കോൽ_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->അക്കം_താക്കോലുകൾ - i;
		എങ്കിൽ (max_j > താക്കോലുകൾ_per_block)
			max_j = താക്കോലുകൾ_per_block;
		വേണ്ടി (j = 0, dkey = data->keyb->താക്കോലുകൾ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_അക്കം = lblk;
			data->താക്കോലുകൾ_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->താക്കോൽ_blk_അക്കം, data->undo_blk_അക്കം, data->അക്കം_താക്കോലുകൾ,
		   data->താക്കോലുകൾ_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	തിരികെ retval;

bad_താക്കോൽ_replay:
	data->താക്കോൽ_blk_അക്കം = data->undo_blk_അക്കം = 0;
	data->താക്കോലുകൾ_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	പോകുക out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	തിരികെ retval;
}

സ്ഥിതം ശൂന്യം undo_atexit(ശൂന്യം *p)
{
	ഘടന undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

സ്ഥിതം errcode_t undo_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	io_channel	io = ശൂന്യ;
	ഘടന undo_private_data *data = ശൂന്യ;
	പൂർണ്ണസംഖ്യ		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	എങ്കിൽ (name == 0)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_io_channel), &io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(io, 0, വലുപ്പം(ഘടന struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന undo_private_data), &data);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, വലുപ്പം(ഘടന undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_അക്കം = 1;
	data->first_താക്കോൽ_blk = 2;
	data->undo_blk_അക്കം = 3;

	എങ്കിൽ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		എങ്കിൽ (retval)
			പോകുക cleanup;

		data->tdb_file = strdup(tdb_file);
		എങ്കിൽ (data->tdb_file == ശൂന്യ)
			പോകുക cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		എങ്കിൽ (undo_fd < 0)
			പോകുക cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		data->real = ശൂന്യ;
		data->undo_file = ശൂന്യ;
	}

	എങ്കിൽ (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	എങ്കിൽ (data->real)
		undo_err_handler_init(data->real);

	എങ്കിൽ (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	*channel = io;
	എങ്കിൽ (undo_fd >= 0)
		close(undo_fd);
	തിരികെ retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	എങ്കിൽ (undo_fd >= 0)
		close(undo_fd);
	എങ്കിൽ (data && data->undo_file)
		io_channel_close(data->undo_file);
	എങ്കിൽ (data && data->tdb_file)
		സ്വതന്ത്രം(data->tdb_file);
	എങ്കിൽ (data && data->real)
		io_channel_close(data->real);
	എങ്കിൽ (data)
		ext2fs_free_mem(&data);
	എങ്കിൽ (io && io->name)
		ext2fs_free_mem(&io->name);
	എങ്കിൽ (io)
		ext2fs_free_mem(&io);
	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_close(io_channel channel)
{
	ഘടന undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
		തിരികെ 0;
	/* Before closing write the file system identity */
	എങ്കിൽ (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	എങ്കിൽ (data->real)
		retval = io_channel_close(data->real);
	എങ്കിൽ (data->tdb_file)
		സ്വതന്ത്രം(data->tdb_file);
	എങ്കിൽ (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	എങ്കിൽ (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	എങ്കിൽ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	എങ്കിൽ (err)
		തിരികെ err;
	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	ഘടന undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	എങ്കിൽ (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	errcode_t	retval = 0;
	ഘടന undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_read_blk64(data->real, block, എണ്ണം, buf);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	തിരികെ undo_read_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, എണ്ണം);
	എങ്കിൽ (retval)
		 തിരികെ retval;
	എങ്കിൽ (data->real)
		retval = io_channel_write_blk64(data->real, block, എണ്ണം, buf);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ undo_write_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 പൂർണ്ണസംഖ്യ size, സ്ഥിരം ശൂന്യം *buf)
{
	ഘടന undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_അക്കം, എണ്ണം;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_അക്കം = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	എണ്ണം = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_അക്കം, എണ്ണം);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന undo_private_data *data;
	errcode_t	retval = 0;
	പൂർണ്ണസംഖ്യ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (എണ്ണം > INT_MAX)
		തിരികെ EXT2_ET_UNIMPLEMENTED;
	icount = എണ്ണം;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (data->real)
		retval = io_channel_discard(data->real, block, എണ്ണം);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long എണ്ണം)
{
	ഘടന undo_private_data *data;
	errcode_t	retval = 0;
	പൂർണ്ണസംഖ്യ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (എണ്ണം > INT_MAX)
		തിരികെ EXT2_ET_UNIMPLEMENTED;
	icount = എണ്ണം;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (data->real)
		retval = io_channel_zeroout(data->real, block, എണ്ണം);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long എണ്ണം)
{
	ഘടന undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_cache_readahead(data->real, block, എണ്ണം);

	തിരികെ retval;
}

/*
 * Flush data buffers to disk.
 */
സ്ഥിതം errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ഘടന undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = io_channel_flush(data->real);

	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_set_option(io_channel channel, സ്ഥിരം അക്ഷരം *option,
				 സ്ഥിരം അക്ഷരം *arg)
{
	errcode_t	retval = 0;
	ഘടന undo_private_data *data;
	unsigned long tmp;
	അക്ഷരം *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (!strcmp(option, "tdb_data_size")) {
		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		എങ്കിൽ (*end)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		എങ്കിൽ (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		എങ്കിൽ (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		തിരികെ 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	എങ്കിൽ (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	എങ്കിൽ (!retval && !strcmp(option, "offset")) {
		എങ്കിൽ (!arg)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		എങ്കിൽ (*end)
			തിരികെ EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ഘടന undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (ഘടന undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	എങ്കിൽ (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	തിരികെ retval;
}

സ്ഥിതം ഘടന struct_io_manager struct_undo_manager = {
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
