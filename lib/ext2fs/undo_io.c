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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जर HAVE_SYS_RESOURCE_H
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
# define dbg_printf(f, a...)  करा {म_लिखो(f, ## a); ख_साफ(मानक_निकास); } पर्यंत (0)
#अन्यथा
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  जर ((संरचना)->magic != (code)) परत (code)
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
	अक्षर magic[8];		/* "E2UNDO02" */
	__le64 अंक_किल्ल्या;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 किल्ली_offset;	/* where do the key/data block chunks start? */
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

संरचना undo_किल्ली {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

संरचना undo_किल्ली_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#जर __STDC_VERSION__ >= 199901L
	संरचना undo_किल्ली किल्ल्या[];		/* keys, which come immediately after */
#अन्यथा
	संरचना undo_किल्ली किल्ल्या[0];	/* keys, which come immediately after */
#endif
};

संरचना undo_private_data {
	पूर्णांक	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_अंक;			/* next free block */
	blk64_t किल्ली_blk_अंक;			/* current key block location */
	blk64_t super_blk_अंक;			/* superblock location */
	blk64_t first_किल्ली_blk;			/* first key block location */
	संरचना undo_किल्ली_block *keyb;
	माप_प्रकार अंक_किल्ल्या, किल्ल्या_in_block;

	/* The backing io channel */
	io_channel real;

	अचिन्हित दीर्घ दीर्घ tdb_data_size;
	पूर्णांक tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	संरचना struct_ext2_filsys fake_fs;
	अक्षर *tdb_file;
	संरचना undo_header hdr;
};
#define किल्ल्या_PER_BLOCK(d) (((d)->tdb_data_size / आकार(संरचना undo_किल्ली)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

स्थिर ओळीत शून्य e2undo_set_feature_fs_offset(संरचना undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

स्थिर ओळीत शून्य e2undo_clear_feature_fs_offset(संरचना undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

स्थिर io_manager undo_io_backing_manager;
स्थिर अक्षर *tdb_file;
स्थिर पूर्णांक actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	परत 0;
}

errcode_t set_undo_io_backup_file(अक्षर *file_name)
{
	tdb_file = strdup(file_name);

	जर (tdb_file == रिक्त) {
		परत EXT2_ET_NO_MEMORY;
	}

	परत 0;
}

स्थिर errcode_t write_undo_indexes(संरचना undo_private_data *data, पूर्णांक flush)
{
	errcode_t retval;
	संरचना ext2_super_block super;
	io_channel channel;
	पूर्णांक block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	जर (data->किल्ल्या_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (अचिन्हित अक्षर *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->किल्ली_blk_अंक);
		retval = io_channel_write_blk64(data->undo_file,
						data->किल्ली_blk_अंक,
						1, data->keyb);
		जर (retval)
			परत retval;
		/* Move on to the next key block if it's full. */
		जर (data->किल्ल्या_in_block == किल्ल्या_PER_BLOCK(data)) {
			स_रखो(data->keyb, 0, data->tdb_data_size);
			data->किल्ल्या_in_block = 0;
			data->किल्ली_blk_अंक = data->undo_blk_अंक;
			data->undo_blk_अंक++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	जर (retval)
		जा err_out;
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	स_नकल(data->hdr.magic, E2UNDO_MAGIC, आकार(data->hdr.magic));
	data->hdr.अंक_किल्ल्या = ext2fs_cpu_to_le64(data->अंक_किल्ल्या);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_अंक);
	data->hdr.किल्ली_offset = ext2fs_cpu_to_le64(data->first_किल्ली_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	जर (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	अन्यथा
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&data->hdr,
				   आकार(data->hdr) -
				   आकार(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(पूर्णांक)आकार(data->hdr),
					&data->hdr);
	जर (retval)
		जा err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_अंक);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_अंक,
					-SUPERBLOCK_SIZE, &super);
	जर (retval)
		जा err_out;

	जर (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	परत retval;
}

स्थिर errcode_t undo_setup_tdb(संरचना undo_private_data *data)
{
	पूर्णांक i;
	errcode_t retval;

	जर (data->tdb_written == 1)
		परत 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	स_रखो(&data->fake_fs, 0, आकार(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	जर (retval)
		परत retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	जर (retval)
		परत retval;
	data->किल्ली_blk_अंक = data->first_किल्ली_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", किल्ल्या_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	साठी (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		जर (retval)
			स_रखो(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		जर (retval)
			परत retval;
		retval = io_channel_flush(data->undo_file);
		जर (retval)
			परत retval;
	}
	स_रखो(data->keyb, 0, data->tdb_data_size);
	परत 0;
}

स्थिर errcode_t undo_write_tdb(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक मोजणी)

{
	पूर्णांक size, sz;
	अचिन्हित दीर्घ दीर्घ block_अंक, backing_blk_अंक;
	errcode_t retval = 0;
	ext2_loff_t offset;
	संरचना undo_private_data *data;
	अचिन्हित अक्षर *read_ptr;
	अचिन्हित दीर्घ दीर्घ end_block;
	अचिन्हित दीर्घ दीर्घ data_size;
	संरचना undo_किल्ली *किल्ली;
	__u32 blk_crc;

	data = (संरचना undo_private_data *) channel->private_data;

	जर (data->undo_file == रिक्त) {
		/*
		 * Transaction database not initialized
		 */
		परत 0;
	}

	जर (मोजणी == 1)
		size = channel->block_size;
	अन्यथा {
		जर (मोजणी < 0)
			size = -मोजणी;
		अन्यथा
			size = मोजणी * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	जर (retval)
		परत retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_अंक = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	पर्यंत (block_अंक <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		जर (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_अंक)) {
			/* Try the next block */
			block_अंक++;
			सुरूठेवा;
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
		जर (retval) {
			परत retval;
		}

		स_रखो(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		जर ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		अन्यथा
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_अंक,
					     sz, read_ptr);
		जर (retval) {
			जर (retval != EXT2_ET_SHORT_READ) {
				मुक्त(read_ptr);
				परत retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} अन्यथा {
			data_size = data->tdb_data_size;
		}
		जर (data_size == 0) {
			मुक्त(read_ptr);
			block_अंक++;
			सुरूठेवा;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_अंक, block, data->tdb_data_size);
		जर ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		अन्यथा
			sz = -data_size;;
		/* extend this key? */
		जर (data->किल्ल्या_in_block) {
			किल्ली = data->keyb->किल्ल्या + data->किल्ल्या_in_block - 1;
			keysz = ext2fs_le32_to_cpu(किल्ली->size);
		} अन्यथा {
			किल्ली = रिक्त;
			keysz = 0;
		}
		जर (किल्ली != रिक्त &&
		    (ext2fs_le64_to_cpu(किल्ली->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_अंक &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(किल्ली->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			किल्ली->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			किल्ली->size = ext2fs_cpu_to_le32(keysz + data_size);
		} अन्यथा {
			data->अंक_किल्ल्या++;
			किल्ली = data->keyb->किल्ल्या + data->किल्ल्या_in_block;
			data->किल्ल्या_in_block++;
			किल्ली->fsblk = ext2fs_cpu_to_le64(backing_blk_अंक);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			किल्ली->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			किल्ली->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_अंक,
		       data->undo_blk_अंक,
		       sz, data->अंक_किल्ल्या - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_अंक, sz, read_ptr);
		जर (retval) {
			मुक्त(read_ptr);
			परत retval;
		}
		data->undo_blk_अंक++;
		मुक्त(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		जर (retval)
			परत retval;

		/* Next block */
		block_अंक++;
	}

	परत retval;
}

स्थिर errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    अचिन्हित दीर्घ block ATTR((unused)),
				    पूर्णांक मोजणी ATTR((unused)),
				    शून्य *data ATTR((unused)),
				    माप_प्रकार size ATTR((unused)),
				    पूर्णांक actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	परत error;
}

स्थिर शून्य undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

स्थिर पूर्णांक check_filesystem(संरचना undo_header *hdr, io_channel undo_file,
			    अचिन्हित पूर्णांक blocksize, blk64_t super_block,
			    io_channel channel)
{
	संरचना ext2_super_block super, *sb;
	अक्षर *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	जर (retval)
		परत retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	जर (retval)
		परत retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	जर (retval)
		जा out;
	sb = (संरचना ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	जर (स_भेद(&super, buf, आकार(super))) {
		retval = -1;
		जा out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)buf, SUPERBLOCK_SIZE);
	जर (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		जा out;
	}

out:
	ext2fs_free_mem(&buf);
	परत retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
स्थिर errcode_t try_reopen_undo_file(पूर्णांक undo_fd,
				      संरचना undo_private_data *data)
{
	संरचना undo_header hdr;
	संरचना undo_किल्ली *dkey;
	ext2fs_struct_stat statbuf;
	अचिन्हित पूर्णांक blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	माप_प्रकार अंक_किल्ल्या, किल्ल्या_per_block, i;
	__u32 hdr_crc, किल्ली_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	जर (retval)
		जा bad_file;
	जर (statbuf.st_size == 0)
		जा out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(पूर्णांक)आकार(hdr),
				       &hdr);
	जर (retval)
		जा bad_file;

	जर (स_भेद(hdr.magic, E2UNDO_MAGIC,
		    आकार(hdr.magic)))
		जा bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&hdr,
				   आकार(संरचना undo_header) -
				   आकार(__u32));
	जर (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		जा bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	जर (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		जा bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	अंक_किल्ल्या = ext2fs_le64_to_cpu(hdr.अंक_किल्ल्या);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	जर (hdr.f_incompat || hdr.f_rocompat)
		जा bad_file;

	/* Superblock matches this FS? */
	जर (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		जा out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	जर (retval)
		जा bad_file;
	data->अंक_किल्ल्या = अंक_किल्ल्या;
	data->super_blk_अंक = super_block;
	data->first_किल्ली_blk = ext2fs_le64_to_cpu(hdr.किल्ली_offset);

	/* load the written block map */
	किल्ल्या_per_block = किल्ल्या_PER_BLOCK(data);
	lblk = data->first_किल्ली_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   अंक_किल्ल्या, किल्ल्या_per_block, blocksize);
	साठी (i = 0; i < अंक_किल्ल्या; i += किल्ल्या_per_block) {
		माप_प्रकार j, max_j;
		__le32 crc;

		data->किल्ली_blk_अंक = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		जर (retval)
			जा bad_किल्ली_replay;

		/* check keys */
		जर (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			जा bad_किल्ली_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		किल्ली_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)data->keyb,
					   blocksize);
		जर (ext2fs_le32_to_cpu(crc) != किल्ली_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			जा bad_किल्ली_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->अंक_किल्ल्या - i;
		जर (max_j > किल्ल्या_per_block)
			max_j = किल्ल्या_per_block;
		साठी (j = 0, dkey = data->keyb->किल्ल्या;
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
			data->किल्ल्या_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->किल्ली_blk_अंक, data->undo_blk_अंक, data->अंक_किल्ल्या,
		   data->किल्ल्या_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	परत retval;

bad_किल्ली_replay:
	data->किल्ली_blk_अंक = data->undo_blk_अंक = 0;
	data->किल्ल्या_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	जा out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	परत retval;
}

स्थिर शून्य undo_atexit(शून्य *p)
{
	संरचना undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

स्थिर errcode_t undo_open(स्थिरांक अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना undo_private_data *data = रिक्त;
	पूर्णांक		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	जर (name == 0)
		परत EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(आकार(संरचना struct_io_channel), &io);
	जर (retval)
		जा cleanup;
	स_रखो(io, 0, आकार(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(आकार(संरचना undo_private_data), &data);
	जर (retval)
		जा cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	जर (retval)
		जा cleanup;

	म_नकल(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	स_रखो(data, 0, आकार(संरचना undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_अंक = 1;
	data->first_किल्ली_blk = 2;
	data->undo_blk_अंक = 3;

	जर (undo_io_backing_manager) {
		retval = undo_io_backing_manager->खोलो(name, flags,
						       &data->real);
		जर (retval)
			जा cleanup;

		data->tdb_file = strdup(tdb_file);
		जर (data->tdb_file == रिक्त)
			जा cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		जर (undo_fd < 0)
			जा cleanup;

		retval = undo_io_backing_manager->खोलो(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		जर (retval)
			जा cleanup;
	} अन्यथा {
		data->real = रिक्त;
		data->undo_file = रिक्त;
	}

	जर (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	जर (data->real)
		undo_err_handler_init(data->real);

	जर (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		जर (retval)
			जा cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	जर (retval)
		जा cleanup;

	*channel = io;
	जर (undo_fd >= 0)
		बंद(undo_fd);
	परत retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	जर (undo_fd >= 0)
		बंद(undo_fd);
	जर (data && data->undo_file)
		io_channel_close(data->undo_file);
	जर (data && data->tdb_file)
		मुक्त(data->tdb_file);
	जर (data && data->real)
		io_channel_close(data->real);
	जर (data)
		ext2fs_free_mem(&data);
	जर (io && io->name)
		ext2fs_free_mem(&io->name);
	जर (io)
		ext2fs_free_mem(&io);
	परत retval;
}

स्थिर errcode_t undo_close(io_channel channel)
{
	संरचना undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (--channel->refcount > 0)
		परत 0;
	/* Before closing write the file system identity */
	जर (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	जर (data->real)
		retval = io_channel_close(data->real);
	जर (data->tdb_file)
		मुक्त(data->tdb_file);
	जर (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	जर (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	जर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	जर (err)
		परत err;
	परत retval;
}

स्थिर errcode_t undo_set_blksize(io_channel channel, पूर्णांक blksize)
{
	संरचना undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	जर (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	परत retval;
}

स्थिर errcode_t undo_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_read_blk64(data->real, block, मोजणी, buf);

	परत retval;
}

स्थिर errcode_t undo_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक मोजणी, शून्य *buf)
{
	परत undo_read_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t undo_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, मोजणी);
	जर (retval)
		 परत retval;
	जर (data->real)
		retval = io_channel_write_blk64(data->real, block, मोजणी, buf);

	परत retval;
}

स्थिर errcode_t undo_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	परत undo_write_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t undo_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक size, स्थिरांक शून्य *buf)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	अचिन्हित दीर्घ blk_अंक, मोजणी;;

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
	मोजणी = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_अंक, मोजणी);
	जर (retval)
		परत retval;
	जर (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	परत retval;
}

स्थिर errcode_t undo_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णांक icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (मोजणी > पूर्णांक_उच्च)
		परत EXT2_ET_UNIMPLEMENTED;
	icount = मोजणी;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	जर (retval)
		परत retval;
	जर (data->real)
		retval = io_channel_discard(data->real, block, मोजणी);

	परत retval;
}

स्थिर errcode_t undo_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;
	पूर्णांक icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (मोजणी > पूर्णांक_उच्च)
		परत EXT2_ET_UNIMPLEMENTED;
	icount = मोजणी;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	जर (retval)
		परत retval;
	जर (data->real)
		retval = io_channel_zeroout(data->real, block, मोजणी);

	परत retval;
}

स्थिर errcode_t undo_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ मोजणी)
{
	संरचना undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (data->real)
		retval = io_channel_cache_readahead(data->real, block, मोजणी);

	परत retval;
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

	जर (data->real)
		retval = io_channel_flush(data->real);

	परत retval;
}

स्थिर errcode_t undo_set_option(io_channel channel, स्थिरांक अक्षर *option,
				 स्थिरांक अक्षर *arg)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;
	अचिन्हित दीर्घ tmp;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (!म_भेद(option, "tdb_data_size")) {
		जर (!arg)
			परत EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		जर (*end)
			परत EXT2_ET_INVALID_ARGUMENT;
		जर (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			परत EXT2_ET_INVALID_ARGUMENT;
		जर (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		परत 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	जर (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	जर (!retval && !म_भेद(option, "offset")) {
		जर (!arg)
			परत EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		जर (*end)
			परत EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	परत retval;
}

स्थिर errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	संरचना undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (संरचना undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	जर (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	परत retval;
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
