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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#यदि HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#endif
#अन्तर्भाव <limits.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#अन्यथा
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  कुरु {म_लिखो(f, ## a); ख_साफ(मानक_निकास); } यावत् (0)
#अन्यथा
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) प्रत्यागम (code)
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
	__le64 अंक_कुंजियाँ;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 कुंजी_offset;	/* where do the key/data block chunks start? */
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

संरचना undo_कुंजी {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

संरचना undo_कुंजी_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#यदि __STDC_VERSION__ >= 199901L
	संरचना undo_कुंजी कुंजियाँ[];		/* keys, which come immediately after */
#अन्यथा
	संरचना undo_कुंजी कुंजियाँ[0];	/* keys, which come immediately after */
#endif
};

संरचना undo_private_data {
	पूर्णाङ्क	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_अंक;			/* next free block */
	blk64_t कुंजी_blk_अंक;			/* current key block location */
	blk64_t super_blk_अंक;			/* superblock location */
	blk64_t first_कुंजी_blk;			/* first key block location */
	संरचना undo_कुंजी_block *keyb;
	माप_प्रकार अंक_कुंजियाँ, कुंजियाँ_in_block;

	/* The backing io channel */
	io_channel real;

	अचिन्हित दीर्घ दीर्घ tdb_data_size;
	पूर्णाङ्क tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	संरचना struct_ext2_filsys fake_fs;
	वर्ण *tdb_file;
	संरचना undo_header hdr;
};
#define कुंजियाँ_PER_BLOCK(d) (((d)->tdb_data_size / परिमाण(संरचना undo_कुंजी)) - 1)

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

	यदि (tdb_file == रिक्त) {
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
	यदि (data->कुंजियाँ_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (अचिन्हित वर्ण *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->कुंजी_blk_अंक);
		retval = io_channel_write_blk64(data->undo_file,
						data->कुंजी_blk_अंक,
						1, data->keyb);
		यदि (retval)
			प्रत्यागम retval;
		/* Move on to the next key block if it's full. */
		यदि (data->कुंजियाँ_in_block == कुंजियाँ_PER_BLOCK(data)) {
			स_रखो(data->keyb, 0, data->tdb_data_size);
			data->कुंजियाँ_in_block = 0;
			data->कुंजी_blk_अंक = data->undo_blk_अंक;
			data->undo_blk_अंक++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	यदि (retval)
		गच्छ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित वर्ण *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	स_नकल(data->hdr.magic, E2UNDO_MAGIC, परिमाण(data->hdr.magic));
	data->hdr.अंक_कुंजियाँ = ext2fs_cpu_to_le64(data->अंक_कुंजियाँ);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_अंक);
	data->hdr.कुंजी_offset = ext2fs_cpu_to_le64(data->first_कुंजी_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	यदि (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	अन्यथा
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित वर्ण *)&data->hdr,
				   परिमाण(data->hdr) -
				   परिमाण(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(पूर्णाङ्क)परिमाण(data->hdr),
					&data->hdr);
	यदि (retval)
		गच्छ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_अंक);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_अंक,
					-SUPERBLOCK_SIZE, &super);
	यदि (retval)
		गच्छ err_out;

	यदि (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	प्रत्यागम retval;
}

स्थिर errcode_t undo_setup_tdb(संरचना undo_private_data *data)
{
	पूर्णाङ्क i;
	errcode_t retval;

	यदि (data->tdb_written == 1)
		प्रत्यागम 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	स_रखो(&data->fake_fs, 0, परिमाण(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	यदि (retval)
		प्रत्यागम retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	यदि (retval)
		प्रत्यागम retval;
	data->कुंजी_blk_अंक = data->first_कुंजी_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", कुंजियाँ_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	प्रति (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		यदि (retval)
			स_रखो(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		यदि (retval)
			प्रत्यागम retval;
		retval = io_channel_flush(data->undo_file);
		यदि (retval)
			प्रत्यागम retval;
	}
	स_रखो(data->keyb, 0, data->tdb_data_size);
	प्रत्यागम 0;
}

स्थिर errcode_t undo_write_tdb(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती)

{
	पूर्णाङ्क size, sz;
	अचिन्हित दीर्घ दीर्घ block_अंक, backing_blk_अंक;
	errcode_t retval = 0;
	ext2_loff_t offset;
	संरचना undo_private_data *data;
	अचिन्हित वर्ण *read_ptr;
	अचिन्हित दीर्घ दीर्घ end_block;
	अचिन्हित दीर्घ दीर्घ data_size;
	संरचना undo_कुंजी *कुंजी;
	__u32 blk_crc;

	data = (संरचना undo_private_data *) channel->private_data;

	यदि (data->undo_file == रिक्त) {
		/*
		 * Transaction database not initialized
		 */
		प्रत्यागम 0;
	}

	यदि (गिनती == 1)
		size = channel->block_size;
	अन्यथा {
		यदि (गिनती < 0)
			size = -गिनती;
		अन्यथा
			size = गिनती * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	यदि (retval)
		प्रत्यागम retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_अंक = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	यावत् (block_अंक <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		यदि (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_अंक)) {
			/* Try the next block */
			block_अंक++;
			अनुवर्त;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_अंक);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_अंक * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_अंक = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		यदि (retval) {
			प्रत्यागम retval;
		}

		स_रखो(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		यदि ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		अन्यथा
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_अंक,
					     sz, read_ptr);
		यदि (retval) {
			यदि (retval != EXT2_ET_SHORT_READ) {
				मुक्त(read_ptr);
				प्रत्यागम retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} अन्यथा {
			data_size = data->tdb_data_size;
		}
		यदि (data_size == 0) {
			मुक्त(read_ptr);
			block_अंक++;
			अनुवर्त;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_अंक, block, data->tdb_data_size);
		यदि ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		अन्यथा
			sz = -data_size;;
		/* extend this key? */
		यदि (data->कुंजियाँ_in_block) {
			कुंजी = data->keyb->कुंजियाँ + data->कुंजियाँ_in_block - 1;
			keysz = ext2fs_le32_to_cpu(कुंजी->size);
		} अन्यथा {
			कुंजी = रिक्त;
			keysz = 0;
		}
		यदि (कुंजी != रिक्त &&
		    (ext2fs_le64_to_cpu(कुंजी->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_अंक &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(कुंजी->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			कुंजी->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुंजी->size = ext2fs_cpu_to_le32(keysz + data_size);
		} अन्यथा {
			data->अंक_कुंजियाँ++;
			कुंजी = data->keyb->कुंजियाँ + data->कुंजियाँ_in_block;
			data->कुंजियाँ_in_block++;
			कुंजी->fsblk = ext2fs_cpu_to_le64(backing_blk_अंक);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			कुंजी->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुंजी->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_अंक,
		       data->undo_blk_अंक,
		       sz, data->अंक_कुंजियाँ - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_अंक, sz, read_ptr);
		यदि (retval) {
			मुक्त(read_ptr);
			प्रत्यागम retval;
		}
		data->undo_blk_अंक++;
		मुक्त(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		यदि (retval)
			प्रत्यागम retval;

		/* Next block */
		block_अंक++;
	}

	प्रत्यागम retval;
}

स्थिर errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    अचिन्हित दीर्घ block ATTR((unused)),
				    पूर्णाङ्क गिनती ATTR((unused)),
				    शून्य *data ATTR((unused)),
				    माप_प्रकार size ATTR((unused)),
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
			    अचिन्हित पूर्णाङ्क blocksize, blk64_t super_block,
			    io_channel channel)
{
	संरचना ext2_super_block super, *sb;
	वर्ण *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	यदि (retval)
		प्रत्यागम retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	यदि (retval)
		गच्छ out;
	sb = (संरचना ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	यदि (स_भेद(&super, buf, परिमाण(super))) {
		retval = -1;
		गच्छ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित वर्ण *)buf, SUPERBLOCK_SIZE);
	यदि (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
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
	संरचना undo_कुंजी *dkey;
	ext2fs_struct_stat statbuf;
	अचिन्हित पूर्णाङ्क blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	माप_प्रकार अंक_कुंजियाँ, कुंजियाँ_per_block, i;
	__u32 hdr_crc, कुंजी_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	यदि (retval)
		गच्छ bad_file;
	यदि (statbuf.st_size == 0)
		गच्छ out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(पूर्णाङ्क)परिमाण(hdr),
				       &hdr);
	यदि (retval)
		गच्छ bad_file;

	यदि (स_भेद(hdr.magic, E2UNDO_MAGIC,
		    परिमाण(hdr.magic)))
		गच्छ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित वर्ण *)&hdr,
				   परिमाण(संरचना undo_header) -
				   परिमाण(__u32));
	यदि (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		गच्छ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	यदि (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		गच्छ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	अंक_कुंजियाँ = ext2fs_le64_to_cpu(hdr.अंक_कुंजियाँ);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	यदि (hdr.f_incompat || hdr.f_rocompat)
		गच्छ bad_file;

	/* Superblock matches this FS? */
	यदि (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		गच्छ out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	यदि (retval)
		गच्छ bad_file;
	data->अंक_कुंजियाँ = अंक_कुंजियाँ;
	data->super_blk_अंक = super_block;
	data->first_कुंजी_blk = ext2fs_le64_to_cpu(hdr.कुंजी_offset);

	/* load the written block map */
	कुंजियाँ_per_block = कुंजियाँ_PER_BLOCK(data);
	lblk = data->first_कुंजी_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   अंक_कुंजियाँ, कुंजियाँ_per_block, blocksize);
	प्रति (i = 0; i < अंक_कुंजियाँ; i += कुंजियाँ_per_block) {
		माप_प्रकार j, max_j;
		__le32 crc;

		data->कुंजी_blk_अंक = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		यदि (retval)
			गच्छ bad_कुंजी_replay;

		/* check keys */
		यदि (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुंजी_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		कुंजी_crc = ext2fs_crc32c_le(~0, (अचिन्हित वर्ण *)data->keyb,
					   blocksize);
		यदि (ext2fs_le32_to_cpu(crc) != कुंजी_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुंजी_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->अंक_कुंजियाँ - i;
		यदि (max_j > कुंजियाँ_per_block)
			max_j = कुंजियाँ_per_block;
		प्रति (j = 0, dkey = data->keyb->कुंजियाँ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			माप_प्रकार size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_अंक = lblk;
			data->कुंजियाँ_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->कुंजी_blk_अंक, data->undo_blk_अंक, data->अंक_कुंजियाँ,
		   data->कुंजियाँ_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	प्रत्यागम retval;

bad_कुंजी_replay:
	data->कुंजी_blk_अंक = data->undo_blk_अंक = 0;
	data->कुंजियाँ_in_block = 0;
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
	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स_रखो(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना undo_private_data), &data);
	यदि (retval)
		गच्छ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	स_रखो(data, 0, परिमाण(संरचना undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_अंक = 1;
	data->first_कुंजी_blk = 2;
	data->undo_blk_अंक = 3;

	यदि (undo_io_backing_manager) {
		retval = undo_io_backing_manager->खोलो(name, flags,
						       &data->real);
		यदि (retval)
			गच्छ cleanup;

		data->tdb_file = strdup(tdb_file);
		यदि (data->tdb_file == रिक्त)
			गच्छ cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		यदि (undo_fd < 0)
			गच्छ cleanup;

		retval = undo_io_backing_manager->खोलो(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		data->real = रिक्त;
		data->undo_file = रिक्त;
	}

	यदि (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	यदि (data->real)
		undo_err_handler_init(data->real);

	यदि (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		यदि (retval)
			गच्छ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	यदि (retval)
		गच्छ cleanup;

	*channel = io;
	यदि (undo_fd >= 0)
		बंद(undo_fd);
	प्रत्यागम retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	यदि (undo_fd >= 0)
		बंद(undo_fd);
	यदि (data && data->undo_file)
		io_channel_close(data->undo_file);
	यदि (data && data->tdb_file)
		मुक्त(data->tdb_file);
	यदि (data && data->real)
		io_channel_close(data->real);
	यदि (data)
		ext2fs_free_mem(&data);
	यदि (io && io->name)
		ext2fs_free_mem(&io->name);
	यदि (io)
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

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;
	/* Before closing write the file system identity */
	यदि (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	यदि (data->real)
		retval = io_channel_close(data->real);
	यदि (data->tdb_file)
		मुक्त(data->tdb_file);
	यदि (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	यदि (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	यदि (err)
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

	यदि (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	यदि (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	यदि (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णाङ्क गिनती, शून्य *buf)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_read_blk64(data->real, block, गिनती, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गिनती, शून्य *buf)
{
	प्रत्यागम undo_read_blk64(channel, block, गिनती, buf);
}

स्थिर errcode_t undo_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, गिनती);
	यदि (retval)
		 प्रत्यागम retval;
	यदि (data->real)
		retval = io_channel_write_blk64(data->real, block, गिनती, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
	प्रत्यागम undo_write_blk64(channel, block, गिनती, buf);
}

स्थिर errcode_t undo_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णाङ्क size, ध्रुव शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	अचिन्हित दीर्घ blk_अंक, गिनती;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_अंक = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	गिनती = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_अंक, गिनती);
	यदि (retval)
		प्रत्यागम retval;
	यदि (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (गिनती > पूर्णांक_उच्च)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गिनती;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	यदि (retval)
		प्रत्यागम retval;
	यदि (data->real)
		retval = io_channel_discard(data->real, block, गिनती);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (गिनती > पूर्णांक_उच्च)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गिनती;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	यदि (retval)
		प्रत्यागम retval;
	यदि (data->real)
		retval = io_channel_zeroout(data->real, block, गिनती);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (data->real)
		retval = io_channel_cache_readahead(data->real, block, गिनती);

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

	यदि (data->real)
		retval = io_channel_flush(data->real);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_set_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;
	अचिन्हित दीर्घ tmp;
	वर्ण *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!म_भेद(option, "tdb_data_size")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		यदि (*end)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		यदि (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
		यदि (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		प्रत्यागम 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	यदि (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	यदि (!retval && !म_भेद(option, "offset")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		यदि (*end)
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

	यदि (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	प्रत्यागम retval;
}

स्थिर संरचना struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.खोलो		= undo_open,
	.बंद		= undo_close,
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
