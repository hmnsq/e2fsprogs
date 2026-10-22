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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जइ HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जइ HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#endif
#अन्तर्भाव <limits.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#अण्णहा
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  करेहि {printf(f, ## a); fflush(stdout); } जाव (0)
#अण्णहा
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जइ ((संरचना)->magic != (code)) प्रत्यागम (code)
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

संरचना undo_header {
	वर्ण magic[8];		/* "E2UNDO02" */
	__le64 अङ्कः_कुञ्चिकाः;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 कुञ्चिका_offset;	/* where do the key/data block chunks start? */
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

संरचना undo_कुञ्चिका {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

संरचना undo_कुञ्चिका_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#जइ __STDC_VERSION__ >= 199901L
	संरचना undo_कुञ्चिका कुञ्चिकाः[];		/* keys, which come immediately after */
#अण्णहा
	संरचना undo_कुञ्चिका कुञ्चिकाः[0];	/* keys, which come immediately after */
#endif
};

संरचना undo_private_data {
	पूर्णाङ्क	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_अङ्कः;			/* next free block */
	blk64_t कुञ्चिका_blk_अङ्कः;			/* current key block location */
	blk64_t super_blk_अङ्कः;			/* superblock location */
	blk64_t first_कुञ्चिका_blk;			/* first key block location */
	संरचना undo_कुञ्चिका_block *keyb;
	size_t अङ्कः_कुञ्चिकाः, कुञ्चिकाः_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	पूर्णाङ्क tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	संरचना struct_ext2_filsys fake_fs;
	वर्ण *tdb_file;
	संरचना undo_header hdr;
};
#define कुञ्चिकाः_PER_BLOCK(d) (((d)->tdb_data_size / परिमाण(संरचना undo_कुञ्चिका)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

स्थिर अन्तःस्थ शून्य e2undo_set_feature_fs_offset(संरचना undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

स्थिर अन्तःस्थ शून्य e2undo_clear_feature_fs_offset(संरचना undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

स्थिर io_manager undo_io_backing_manager;
स्थिर वर्ण *tdb_file;
स्थिर पूर्णाङ्क actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	प्रत्यागम 0;
}

errcode_t set_undo_io_backup_file(वर्ण *file_name)
{
	tdb_file = strdup(file_name);

	जइ (tdb_file == रिक्त) {
		प्रत्यागम EXT2_ET_NO_MEMORY;
	}

	प्रत्यागम 0;
}

स्थिर errcode_t write_undo_indexes(संरचना undo_private_data *data, पूर्णाङ्क flush)
{
	errcode_t retval;
	संरचना ext2_super_block super;
	io_channel channel;
	पूर्णाङ्क block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	जइ (data->कुञ्चिकाः_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned वर्ण *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->कुञ्चिका_blk_अङ्कः);
		retval = io_channel_write_blk64(data->undo_file,
						data->कुञ्चिका_blk_अङ्कः,
						1, data->keyb);
		जइ (retval)
			प्रत्यागम retval;
		/* Move on to the next key block if it's full. */
		जइ (data->कुञ्चिकाः_in_block == कुञ्चिकाः_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->कुञ्चिकाः_in_block = 0;
			data->कुञ्चिका_blk_अङ्कः = data->undo_blk_अङ्कः;
			data->undo_blk_अङ्कः++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	जइ (retval)
		गच्छ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned वर्ण *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, परिमाण(data->hdr.magic));
	data->hdr.अङ्कः_कुञ्चिकाः = ext2fs_cpu_to_le64(data->अङ्कः_कुञ्चिकाः);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_अङ्कः);
	data->hdr.कुञ्चिका_offset = ext2fs_cpu_to_le64(data->first_कुञ्चिका_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	जइ (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	अण्णहा
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned वर्ण *)&data->hdr,
				   परिमाण(data->hdr) -
				   परिमाण(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(पूर्णाङ्क)परिमाण(data->hdr),
					&data->hdr);
	जइ (retval)
		गच्छ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_अङ्कः);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_अङ्कः,
					-SUPERBLOCK_SIZE, &super);
	जइ (retval)
		गच्छ err_out;

	जइ (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	प्रत्यागम retval;
}

स्थिर errcode_t undo_setup_tdb(संरचना undo_private_data *data)
{
	पूर्णाङ्क i;
	errcode_t retval;

	जइ (data->tdb_written == 1)
		प्रत्यागम 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, परिमाण(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	जइ (retval)
		प्रत्यागम retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	जइ (retval)
		प्रत्यागम retval;
	data->कुञ्चिका_blk_अङ्कः = data->first_कुञ्चिका_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", कुञ्चिकाः_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	प्रति (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		जइ (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		जइ (retval)
			प्रत्यागम retval;
		retval = io_channel_flush(data->undo_file);
		जइ (retval)
			प्रत्यागम retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	प्रत्यागम 0;
}

स्थिर errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, पूर्णाङ्क गणनम्)

{
	पूर्णाङ्क size, sz;
	unsigned long long block_अङ्कः, backing_blk_अङ्कः;
	errcode_t retval = 0;
	ext2_loff_t offset;
	संरचना undo_private_data *data;
	unsigned वर्ण *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	संरचना undo_कुञ्चिका *कुञ्चिका;
	__u32 blk_crc;

	data = (संरचना undo_private_data *) channel->private_data;

	जइ (data->undo_file == रिक्त) {
		/*
		 * Transaction database not initialized
		 */
		प्रत्यागम 0;
	}

	जइ (गणनम् == 1)
		size = channel->block_size;
	अण्णहा {
		जइ (गणनम् < 0)
			size = -गणनम्;
		अण्णहा
			size = गणनम् * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	जइ (retval)
		प्रत्यागम retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_अङ्कः = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	जाव (block_अङ्कः <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		जइ (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_अङ्कः)) {
			/* Try the next block */
			block_अङ्कः++;
			अनुवर्त;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_अङ्कः);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_अङ्कः * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_अङ्कः = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		जइ (retval) {
			प्रत्यागम retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		जइ ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		अण्णहा
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_अङ्कः,
					     sz, read_ptr);
		जइ (retval) {
			जइ (retval != EXT2_ET_SHORT_READ) {
				मुक्त(read_ptr);
				प्रत्यागम retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} अण्णहा {
			data_size = data->tdb_data_size;
		}
		जइ (data_size == 0) {
			मुक्त(read_ptr);
			block_अङ्कः++;
			अनुवर्त;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_अङ्कः, block, data->tdb_data_size);
		जइ ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		अण्णहा
			sz = -data_size;;
		/* extend this key? */
		जइ (data->कुञ्चिकाः_in_block) {
			कुञ्चिका = data->keyb->कुञ्चिकाः + data->कुञ्चिकाः_in_block - 1;
			keysz = ext2fs_le32_to_cpu(कुञ्चिका->size);
		} अण्णहा {
			कुञ्चिका = रिक्त;
			keysz = 0;
		}
		जइ (कुञ्चिका != रिक्त &&
		    (ext2fs_le64_to_cpu(कुञ्चिका->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_अङ्कः &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(कुञ्चिका->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			कुञ्चिका->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुञ्चिका->size = ext2fs_cpu_to_le32(keysz + data_size);
		} अण्णहा {
			data->अङ्कः_कुञ्चिकाः++;
			कुञ्चिका = data->keyb->कुञ्चिकाः + data->कुञ्चिकाः_in_block;
			data->कुञ्चिकाः_in_block++;
			कुञ्चिका->fsblk = ext2fs_cpu_to_le64(backing_blk_अङ्कः);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			कुञ्चिका->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुञ्चिका->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_अङ्कः,
		       data->undo_blk_अङ्कः,
		       sz, data->अङ्कः_कुञ्चिकाः - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_अङ्कः, sz, read_ptr);
		जइ (retval) {
			मुक्त(read_ptr);
			प्रत्यागम retval;
		}
		data->undo_blk_अङ्कः++;
		मुक्त(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		जइ (retval)
			प्रत्यागम retval;

		/* Next block */
		block_अङ्कः++;
	}

	प्रत्यागम retval;
}

स्थिर errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    पूर्णाङ्क गणनम् ATTR((unused)),
				    शून्य *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    पूर्णाङ्क actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	प्रत्यागम error;
}

स्थिर शून्य undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

स्थिर पूर्णाङ्क check_filesystem(संरचना undo_header *hdr, io_channel undo_file,
			    unsigned पूर्णाङ्क blocksize, blk64_t super_block,
			    io_channel channel)
{
	संरचना ext2_super_block super, *sb;
	वर्ण *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	जइ (retval)
		प्रत्यागम retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	जइ (retval)
		प्रत्यागम retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	जइ (retval)
		गच्छ out;
	sb = (संरचना ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	जइ (memcmp(&super, buf, परिमाण(super))) {
		retval = -1;
		गच्छ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned वर्ण *)buf, SUPERBLOCK_SIZE);
	जइ (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		गच्छ out;
	}

out:
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
स्थिर errcode_t try_reopen_undo_file(पूर्णाङ्क undo_fd,
				      संरचना undo_private_data *data)
{
	संरचना undo_header hdr;
	संरचना undo_कुञ्चिका *dkey;
	ext2fs_struct_stat statbuf;
	unsigned पूर्णाङ्क blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t अङ्कः_कुञ्चिकाः, कुञ्चिकाः_per_block, i;
	__u32 hdr_crc, कुञ्चिका_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	जइ (retval)
		गच्छ bad_file;
	जइ (statbuf.st_size == 0)
		गच्छ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(पूर्णाङ्क)परिमाण(hdr),
				       &hdr);
	जइ (retval)
		गच्छ bad_file;

	जइ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    परिमाण(hdr.magic)))
		गच्छ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned वर्ण *)&hdr,
				   परिमाण(संरचना undo_header) -
				   परिमाण(__u32));
	जइ (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		गच्छ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	जइ (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		गच्छ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	अङ्कः_कुञ्चिकाः = ext2fs_le64_to_cpu(hdr.अङ्कः_कुञ्चिकाः);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	जइ (hdr.f_incompat || hdr.f_rocompat)
		गच्छ bad_file;

	/* Superblock matches this FS? */
	जइ (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		गच्छ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	जइ (retval)
		गच्छ bad_file;
	data->अङ्कः_कुञ्चिकाः = अङ्कः_कुञ्चिकाः;
	data->super_blk_अङ्कः = super_block;
	data->first_कुञ्चिका_blk = ext2fs_le64_to_cpu(hdr.कुञ्चिका_offset);

	/* load the written block map */
	कुञ्चिकाः_per_block = कुञ्चिकाः_PER_BLOCK(data);
	lblk = data->first_कुञ्चिका_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   अङ्कः_कुञ्चिकाः, कुञ्चिकाः_per_block, blocksize);
	प्रति (i = 0; i < अङ्कः_कुञ्चिकाः; i += कुञ्चिकाः_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->कुञ्चिका_blk_अङ्कः = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		जइ (retval)
			गच्छ bad_कुञ्चिका_replay;

		/* check keys */
		जइ (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुञ्चिका_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		कुञ्चिका_crc = ext2fs_crc32c_le(~0, (unsigned वर्ण *)data->keyb,
					   blocksize);
		जइ (ext2fs_le32_to_cpu(crc) != कुञ्चिका_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुञ्चिका_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->अङ्कः_कुञ्चिकाः - i;
		जइ (max_j > कुञ्चिकाः_per_block)
			max_j = कुञ्चिकाः_per_block;
		प्रति (j = 0, dkey = data->keyb->कुञ्चिकाः;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_अङ्कः = lblk;
			data->कुञ्चिकाः_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->कुञ्चिका_blk_अङ्कः, data->undo_blk_अङ्कः, data->अङ्कः_कुञ्चिकाः,
		   data->कुञ्चिकाः_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	प्रत्यागम retval;

bad_कुञ्चिका_replay:
	data->कुञ्चिका_blk_अङ्कः = data->undo_blk_अङ्कः = 0;
	data->कुञ्चिकाः_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	गच्छ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	प्रत्यागम retval;
}

स्थिर शून्य undo_atexit(शून्य *p)
{
	संरचना undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

स्थिर errcode_t undo_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना undo_private_data *data = रिक्त;
	पूर्णाङ्क		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	जइ (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	जइ (retval)
		गच्छ cleanup;
	memset(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना undo_private_data), &data);
	जइ (retval)
		गच्छ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	जइ (retval)
		गच्छ cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, परिमाण(संरचना undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_अङ्कः = 1;
	data->first_कुञ्चिका_blk = 2;
	data->undo_blk_अङ्कः = 3;

	जइ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		जइ (retval)
			गच्छ cleanup;

		data->tdb_file = strdup(tdb_file);
		जइ (data->tdb_file == रिक्त)
			गच्छ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		जइ (undo_fd < 0)
			गच्छ cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		जइ (retval)
			गच्छ cleanup;
	} अण्णहा {
		data->real = रिक्त;
		data->undo_file = रिक्त;
	}

	जइ (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	जइ (data->real)
		undo_err_handler_init(data->real);

	जइ (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		जइ (retval)
			गच्छ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	जइ (retval)
		गच्छ cleanup;

	*channel = io;
	जइ (undo_fd >= 0)
		close(undo_fd);
	प्रत्यागम retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	जइ (undo_fd >= 0)
		close(undo_fd);
	जइ (data && data->undo_file)
		io_channel_close(data->undo_file);
	जइ (data && data->tdb_file)
		मुक्त(data->tdb_file);
	जइ (data && data->real)
		io_channel_close(data->real);
	जइ (data)
		ext2fs_free_mem(&data);
	जइ (io && io->name)
		ext2fs_free_mem(&io->name);
	जइ (io)
		ext2fs_free_mem(&io);
	प्रत्यागम retval;
}

स्थिर errcode_t undo_close(io_channel channel)
{
	संरचना undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (--channel->refcount > 0)
		प्रत्यागम 0;
	/* Before closing write the file system identity */
	जइ (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	जइ (data->real)
		retval = io_channel_close(data->real);
	जइ (data->tdb_file)
		मुक्त(data->tdb_file);
	जइ (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	जइ (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	जइ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	जइ (err)
		प्रत्यागम err;
	प्रत्यागम retval;
}

स्थिर errcode_t undo_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	जइ (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	जइ (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (data->real)
		retval = io_channel_read_blk64(data->real, block, गणनम्, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम undo_read_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, गणनम्);
	जइ (retval)
		 प्रत्यागम retval;
	जइ (data->real)
		retval = io_channel_write_blk64(data->real, block, गणनम्, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम undo_write_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 पूर्णाङ्क size, ध्रुव शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_अङ्कः, गणनम्;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_अङ्कः = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	गणनम् = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_अङ्कः, गणनम्);
	जइ (retval)
		प्रत्यागम retval;
	जइ (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long गणनम्)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (गणनम् > INT_MAX)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गणनम्;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	जइ (retval)
		प्रत्यागम retval;
	जइ (data->real)
		retval = io_channel_discard(data->real, block, गणनम्);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long गणनम्)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (गणनम् > INT_MAX)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गणनम्;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	जइ (retval)
		प्रत्यागम retval;
	जइ (data->real)
		retval = io_channel_zeroout(data->real, block, गणनम्);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long गणनम्)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (data->real)
		retval = io_channel_cache_readahead(data->real, block, गणनम्);

	प्रत्यागम retval;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (data->real)
		retval = io_channel_flush(data->real);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_set_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;
	unsigned long tmp;
	वर्ण *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (!strcmp(option, "tdb_data_size")) {
		जइ (!arg)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		जइ (*end)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		जइ (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		जइ (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		प्रत्यागम 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	जइ (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	जइ (!retval && !strcmp(option, "offset")) {
		जइ (!arg)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		जइ (*end)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	प्रत्यागम retval;
}

स्थिर errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जइ (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	प्रत्यागम retval;
}

स्थिर संरचना struct_io_manager struct_undo_manager = {
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
