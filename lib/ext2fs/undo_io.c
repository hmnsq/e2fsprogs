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

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर_घोषित __linux__
#समावेश <sys/utsname.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर HAVE_SYS_RESOURCE_H
#समावेश <sys/resource.h>
#पूर्ण_अगर
#समावेश <limits.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

#अगर_घोषित __GNUC__
#घोषणा ATTR(x) __attribute__(x)
#अन्यथा
#घोषणा ATTR(x)
#पूर्ण_अगर

#अघोषित DEBUG

#अगर_घोषित DEBUG
# define dbg_printf(f, a...)  करो {म_लिखो(f, ## a); ख_साफ(मानक_निकास); } जबतक (0)
#अन्यथा
# define dbg_printf(f, a...)
#पूर्ण_अगर

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)
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
#घोषणा E2UNDO_MAGIC "E2UNDO02"
#घोषणा KEYBLOCK_MAGIC 0xCADECADE

#घोषणा E2UNDO_STATE_FINISHED	0x1	/* undo file is complete */

#घोषणा E2_UNDO_MIN_BLOCK_आकार	1024	/* undo blocks are no less than 1KB */
#घोषणा E2_UNDO_MAX_BLOCK_आकार	1048576	/* undo blocks are no more than 1MB */

काष्ठा undo_header {
	अक्षर magic[8];		/* "E2UNDO02" */
	__le64 अंक_कुंजियाँ;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 कुंजी_offset;	/* where do the key/data block chunks start? */
	__le32 block_आकार;	/* block size of the undo file */
	__le32 fs_block_आकार;	/* block size of the target device */
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

#घोषणा E2UNDO_MAX_EXTENT_BLOCKS	512	/* max extent size, in blocks */

काष्ठा undo_कुंजी {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 आकार;		/* how many bytes in this block? */
};

काष्ठा undo_कुंजी_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#अगर __STDC_VERSION__ >= 199901L
	काष्ठा undo_कुंजी कुंजियाँ[];		/* keys, which come immediately after */
#अन्यथा
	काष्ठा undo_कुंजी कुंजियाँ[0];	/* keys, which come immediately after */
#पूर्ण_अगर
};

काष्ठा undo_private_तथ्य {
	पूर्णांक	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_अंक;			/* next free block */
	blk64_t कुंजी_blk_अंक;			/* current key block location */
	blk64_t super_blk_अंक;			/* superblock location */
	blk64_t first_कुंजी_blk;			/* first key block location */
	काष्ठा undo_कुंजी_block *keyb;
	माप_प्रकार अंक_कुंजियाँ, कुंजियाँ_in_block;

	/* The backing io channel */
	io_channel real;

	अचिन्हित दीर्घ दीर्घ tdb_तथ्य_आकार;
	पूर्णांक tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	काष्ठा struct_ext2_filsys fake_fs;
	अक्षर *tdb_file;
	काष्ठा undo_header hdr;
};
#घोषणा कुंजियाँ_PER_BLOCK(d) (((d)->tdb_तथ्य_आकार / माप(काष्ठा undo_कुंजी)) - 1)

#घोषणा E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

अटल अंतरभूत व्योम e2undo_set_feature_fs_offset(काष्ठा undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

अटल अंतरभूत व्योम e2undo_clear_feature_fs_offset(काष्ठा undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

अटल io_manager undo_io_backing_manager;
अटल अक्षर *tdb_file;
अटल पूर्णांक actual_आकार;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	वापस 0;
}

errcode_t set_undo_io_backup_file(अक्षर *file_name)
{
	tdb_file = strdup(file_name);

	अगर (tdb_file == शून्य) {
		वापस EXT2_ET_NO_MEMORY;
	}

	वापस 0;
}

अटल errcode_t write_undo_indexes(काष्ठा undo_private_तथ्य *तथ्य, पूर्णांक flush)
{
	errcode_t retval;
	काष्ठा ext2_super_block super;
	io_channel channel;
	पूर्णांक block_आकार;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	अगर (तथ्य->कुंजियाँ_in_block) {
		तथ्य->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		तथ्य->keyb->crc = 0;
		तथ्य->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (अचिन्हित अक्षर *)तथ्य->keyb,
					 तथ्य->tdb_तथ्य_आकार));
		dbg_printf("Writing keyblock to blk %llu\n", तथ्य->कुंजी_blk_अंक);
		retval = io_channel_write_blk64(तथ्य->undo_file,
						तथ्य->कुंजी_blk_अंक,
						1, तथ्य->keyb);
		अगर (retval)
			वापस retval;
		/* Move on to the next key block if it's full. */
		अगर (तथ्य->कुंजियाँ_in_block == कुंजियाँ_PER_BLOCK(तथ्य)) {
			स_रखो(तथ्य->keyb, 0, तथ्य->tdb_तथ्य_आकार);
			तथ्य->कुंजियाँ_in_block = 0;
			तथ्य->कुंजी_blk_अंक = तथ्य->undo_blk_अंक;
			तथ्य->undo_blk_अंक++;
		}
	}

	/* Prepare superblock for write */
	channel = तथ्य->real;
	block_आकार = channel->block_आकार;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_आकार, &super);
	अगर (retval)
		जाओ err_out;
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&super, SUPERBLOCK_आकार);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	स_नकल(तथ्य->hdr.magic, E2UNDO_MAGIC, माप(तथ्य->hdr.magic));
	तथ्य->hdr.अंक_कुंजियाँ = ext2fs_cpu_to_le64(तथ्य->अंक_कुंजियाँ);
	तथ्य->hdr.super_offset = ext2fs_cpu_to_le64(तथ्य->super_blk_अंक);
	तथ्य->hdr.कुंजी_offset = ext2fs_cpu_to_le64(तथ्य->first_कुंजी_blk);
	तथ्य->hdr.fs_block_आकार = ext2fs_cpu_to_le32(block_आकार);
	तथ्य->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	तथ्य->hdr.fs_offset = ext2fs_cpu_to_le64(तथ्य->offset);
	अगर (तथ्य->offset)
		e2undo_set_feature_fs_offset(&तथ्य->hdr);
	अन्यथा
		e2undo_clear_feature_fs_offset(&तथ्य->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&तथ्य->hdr,
				   माप(तथ्य->hdr) -
				   माप(तथ्य->hdr.header_crc));
	तथ्य->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(तथ्य->undo_file, 0,
					-(पूर्णांक)माप(तथ्य->hdr),
					&तथ्य->hdr);
	अगर (retval)
		जाओ err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", तथ्य->super_blk_अंक);
	retval = io_channel_write_blk64(तथ्य->undo_file, तथ्य->super_blk_अंक,
					-SUPERBLOCK_आकार, &super);
	अगर (retval)
		जाओ err_out;

	अगर (flush)
		retval = io_channel_flush(तथ्य->undo_file);
err_out:
	io_channel_set_blksize(channel, block_आकार);
	वापस retval;
}

अटल errcode_t undo_setup_tdb(काष्ठा undo_private_तथ्य *तथ्य)
{
	पूर्णांक i;
	errcode_t retval;

	अगर (तथ्य->tdb_written == 1)
		वापस 0;

	तथ्य->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	स_रखो(&तथ्य->fake_fs, 0, माप(तथ्य->fake_fs));
	तथ्य->fake_fs.blocksize = तथ्य->tdb_तथ्य_आकार;
	retval = ext2fs_alloc_generic_bmap(&तथ्य->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &तथ्य->written_block_map);
	अगर (retval)
		वापस retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(तथ्य->tdb_तथ्य_आकार, &तथ्य->keyb);
	अगर (retval)
		वापस retval;
	तथ्य->कुंजी_blk_अंक = तथ्य->first_कुंजी_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", तथ्य->tdb_तथ्य_आकार);
	dbg_printf("Keys per block %llu\n", कुंजियाँ_PER_BLOCK(तथ्य));
	तथ्य->hdr.block_आकार = ext2fs_cpu_to_le32(तथ्य->tdb_तथ्य_आकार);
	io_channel_set_blksize(तथ्य->undo_file, तथ्य->tdb_तथ्य_आकार);

	/* Ensure that we have space for header blocks */
	क्रम (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(तथ्य->undo_file, i, 1,
					       तथ्य->keyb);
		अगर (retval)
			स_रखो(तथ्य->keyb, 0, तथ्य->tdb_तथ्य_आकार);
		retval = io_channel_write_blk64(तथ्य->undo_file, i, 1,
						तथ्य->keyb);
		अगर (retval)
			वापस retval;
		retval = io_channel_flush(तथ्य->undo_file);
		अगर (retval)
			वापस retval;
	}
	स_रखो(तथ्य->keyb, 0, तथ्य->tdb_तथ्य_आकार);
	वापस 0;
}

अटल errcode_t undo_write_tdb(io_channel channel,
				अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती)

{
	पूर्णांक आकार, sz;
	अचिन्हित दीर्घ दीर्घ block_अंक, backing_blk_अंक;
	errcode_t retval = 0;
	ext2_loff_t offset;
	काष्ठा undo_private_तथ्य *तथ्य;
	अचिन्हित अक्षर *read_ptr;
	अचिन्हित दीर्घ दीर्घ end_block;
	अचिन्हित दीर्घ दीर्घ तथ्य_आकार;
	काष्ठा undo_कुंजी *कुंजी;
	__u32 blk_crc;

	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;

	अगर (तथ्य->undo_file == शून्य) {
		/*
		 * Transaction database not initialized
		 */
		वापस 0;
	}

	अगर (गिनती == 1)
		आकार = channel->block_आकार;
	अन्यथा {
		अगर (गिनती < 0)
			आकार = -गिनती;
		अन्यथा
			आकार = गिनती * channel->block_आकार;
	}

	retval = undo_setup_tdb(तथ्य);
	अगर (retval)
		वापस retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_आकार) + तथ्य->offset ;
	block_अंक = offset / तथ्य->tdb_तथ्य_आकार;
	end_block = (offset + आकार - 1) / तथ्य->tdb_तथ्य_आकार;

	जबतक (block_अंक <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		अगर (ext2fs_test_block_bitmap2(तथ्य->written_block_map,
						   block_अंक)) {
			/* Try the next block */
			block_अंक++;
			जारी;
		}
		ext2fs_mark_block_bitmap2(तथ्य->written_block_map, block_अंक);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_अंक * तथ्य->tdb_तथ्य_आकार +
				(तथ्य->offset % तथ्य->tdb_तथ्य_आकार);
		backing_blk_अंक = (offset - तथ्य->offset) / channel->block_आकार;

		retval = ext2fs_get_mem(तथ्य->tdb_तथ्य_आकार, &read_ptr);
		अगर (retval) {
			वापस retval;
		}

		स_रखो(read_ptr, 0, तथ्य->tdb_तथ्य_आकार);
		actual_आकार = 0;
		अगर ((तथ्य->tdb_तथ्य_आकार % channel->block_आकार) == 0)
			sz = तथ्य->tdb_तथ्य_आकार / channel->block_आकार;
		अन्यथा
			sz = -तथ्य->tdb_तथ्य_आकार;
		retval = io_channel_read_blk64(तथ्य->real, backing_blk_अंक,
					     sz, read_ptr);
		अगर (retval) {
			अगर (retval != EXT2_ET_SHORT_READ) {
				मुक्त(read_ptr);
				वापस retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			तथ्य_आकार = actual_आकार;
		} अन्यथा {
			तथ्य_आकार = तथ्य->tdb_तथ्य_आकार;
		}
		अगर (तथ्य_आकार == 0) {
			मुक्त(read_ptr);
			block_अंक++;
			जारी;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       तथ्य_आकार, backing_blk_अंक, block, तथ्य->tdb_तथ्य_आकार);
		अगर ((तथ्य_आकार % तथ्य->undo_file->block_आकार) == 0)
			sz = तथ्य_आकार / तथ्य->undo_file->block_आकार;
		अन्यथा
			sz = -तथ्य_आकार;;
		/* extend this key? */
		अगर (तथ्य->कुंजियाँ_in_block) {
			कुंजी = तथ्य->keyb->कुंजियाँ + तथ्य->कुंजियाँ_in_block - 1;
			keysz = ext2fs_le32_to_cpu(कुंजी->आकार);
		} अन्यथा {
			कुंजी = शून्य;
			keysz = 0;
		}
		अगर (कुंजी != शून्य &&
		    (ext2fs_le64_to_cpu(कुंजी->fsblk) * channel->block_आकार +
		     channel->block_आकार - 1 +
		     keysz) / channel->block_आकार == backing_blk_अंक &&
		    E2UNDO_MAX_EXTENT_BLOCKS * तथ्य->tdb_तथ्य_आकार >
		    keysz + तथ्य_आकार) {
			blk_crc = ext2fs_le32_to_cpu(कुंजी->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, तथ्य_आकार);
			कुंजी->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुंजी->आकार = ext2fs_cpu_to_le32(keysz + तथ्य_आकार);
		} अन्यथा {
			तथ्य->अंक_कुंजियाँ++;
			कुंजी = तथ्य->keyb->कुंजियाँ + तथ्य->कुंजियाँ_in_block;
			तथ्य->कुंजियाँ_in_block++;
			कुंजी->fsblk = ext2fs_cpu_to_le64(backing_blk_अंक);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, तथ्य_आकार);
			कुंजी->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			कुंजी->आकार = ext2fs_cpu_to_le32(तथ्य_आकार);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_अंक,
		       तथ्य->undo_blk_अंक,
		       sz, तथ्य->अंक_कुंजियाँ - 1);
		retval = io_channel_write_blk64(तथ्य->undo_file,
					तथ्य->undo_blk_अंक, sz, read_ptr);
		अगर (retval) {
			मुक्त(read_ptr);
			वापस retval;
		}
		तथ्य->undo_blk_अंक++;
		मुक्त(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(तथ्य, 0);
		अगर (retval)
			वापस retval;

		/* Next block */
		block_अंक++;
	}

	वापस retval;
}

अटल errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    अचिन्हित दीर्घ block ATTR((unused)),
				    पूर्णांक गिनती ATTR((unused)),
				    व्योम *तथ्य ATTR((unused)),
				    माप_प्रकार आकार ATTR((unused)),
				    पूर्णांक actual,
				    errcode_t error ATTR((unused)))
{
	actual_आकार = actual;
	वापस error;
}

अटल व्योम undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

अटल पूर्णांक check_filesystem(काष्ठा undo_header *hdr, io_channel undo_file,
			    अचिन्हित पूर्णांक blocksize, blk64_t super_block,
			    io_channel channel)
{
	काष्ठा ext2_super_block super, *sb;
	अक्षर *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_आकार, &super);
	अगर (retval)
		वापस retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	अगर (retval)
		वापस retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_आकार, buf);
	अगर (retval)
		जाओ out;
	sb = (काष्ठा ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	अगर (स_भेद(&super, buf, माप(super))) {
		retval = -1;
		जाओ out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)buf, SUPERBLOCK_आकार);
	अगर (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		जाओ out;
	}

out:
	ext2fs_free_mem(&buf);
	वापस retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
अटल errcode_t try_reopen_undo_file(पूर्णांक undo_fd,
				      काष्ठा undo_private_तथ्य *तथ्य)
{
	काष्ठा undo_header hdr;
	काष्ठा undo_कुंजी *dkey;
	ext2fs_struct_stat statbuf;
	अचिन्हित पूर्णांक blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	माप_प्रकार अंक_कुंजियाँ, कुंजियाँ_per_block, i;
	__u32 hdr_crc, कुंजी_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	अगर (retval)
		जाओ bad_file;
	अगर (statbuf.st_आकार == 0)
		जाओ out;

	/* check the file header */
	retval = io_channel_read_blk64(तथ्य->undo_file, 0, -(पूर्णांक)माप(hdr),
				       &hdr);
	अगर (retval)
		जाओ bad_file;

	अगर (स_भेद(hdr.magic, E2UNDO_MAGIC,
		    माप(hdr.magic)))
		जाओ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)&hdr,
				   माप(काष्ठा undo_header) -
				   माप(__u32));
	अगर (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		जाओ bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_आकार);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_आकार);
	अगर (blocksize > E2_UNDO_MAX_BLOCK_आकार ||
	    blocksize < E2_UNDO_MIN_BLOCK_आकार ||
	    !blocksize || !fs_blocksize)
		जाओ bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	अंक_कुंजियाँ = ext2fs_le64_to_cpu(hdr.अंक_कुंजियाँ);
	io_channel_set_blksize(तथ्य->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	अगर (hdr.f_incompat || hdr.f_rocompat)
		जाओ bad_file;

	/* Superblock matches this FS? */
	अगर (check_filesystem(&hdr, तथ्य->undo_file, blocksize, super_block,
			     तथ्य->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		जाओ out;
	}

	/* Try to set ourselves up */
	तथ्य->tdb_तथ्य_आकार = blocksize;
	retval = undo_setup_tdb(तथ्य);
	अगर (retval)
		जाओ bad_file;
	तथ्य->अंक_कुंजियाँ = अंक_कुंजियाँ;
	तथ्य->super_blk_अंक = super_block;
	तथ्य->first_कुंजी_blk = ext2fs_le64_to_cpu(hdr.कुंजी_offset);

	/* load the written block map */
	कुंजियाँ_per_block = कुंजियाँ_PER_BLOCK(तथ्य);
	lblk = तथ्य->first_कुंजी_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   अंक_कुंजियाँ, कुंजियाँ_per_block, blocksize);
	क्रम (i = 0; i < अंक_कुंजियाँ; i += कुंजियाँ_per_block) {
		माप_प्रकार j, max_j;
		__le32 crc;

		तथ्य->कुंजी_blk_अंक = lblk;
		retval = io_channel_read_blk64(तथ्य->undo_file,
					       lblk, 1, तथ्य->keyb);
		अगर (retval)
			जाओ bad_कुंजी_replay;

		/* check keys */
		अगर (ext2fs_le32_to_cpu(तथ्य->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			जाओ bad_कुंजी_replay;
		}
		crc = तथ्य->keyb->crc;
		तथ्य->keyb->crc = 0;
		कुंजी_crc = ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)तथ्य->keyb,
					   blocksize);
		अगर (ext2fs_le32_to_cpu(crc) != कुंजी_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			जाओ bad_कुंजी_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = तथ्य->अंक_कुंजियाँ - i;
		अगर (max_j > कुंजियाँ_per_block)
			max_j = कुंजियाँ_per_block;
		क्रम (j = 0, dkey = तथ्य->keyb->कुंजियाँ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			माप_प्रकार आकार = ext2fs_le32_to_cpu(dkey->आकार);

			ext2fs_mark_block_bitmap_range2(तथ्य->written_block_map,
					 undo_blk,
					(आकार + blocksize - 1) / blocksize);
			lblk += (आकार + blocksize - 1) / blocksize;
			तथ्य->undo_blk_अंक = lblk;
			तथ्य->कुंजियाँ_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   तथ्य->कुंजी_blk_अंक, तथ्य->undo_blk_अंक, तथ्य->अंक_कुंजियाँ,
		   तथ्य->कुंजियाँ_in_block);

	तथ्य->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	तथ्य->hdr.f_compat = hdr.f_compat;
	तथ्य->hdr.f_incompat = hdr.f_incompat;
	तथ्य->hdr.f_rocompat = hdr.f_rocompat;
	वापस retval;

bad_कुंजी_replay:
	तथ्य->कुंजी_blk_अंक = तथ्य->undo_blk_अंक = 0;
	तथ्य->कुंजियाँ_in_block = 0;
	ext2fs_free_mem(&तथ्य->keyb);
	ext2fs_free_generic_bitmap(तथ्य->written_block_map);
	तथ्य->tdb_written = 0;
	जाओ out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	वापस retval;
}

अटल व्योम undo_atexit(व्योम *p)
{
	काष्ठा undo_private_तथ्य *तथ्य = p;
	errcode_t err;

	err = write_undo_indexes(तथ्य, 1);
	io_channel_close(तथ्य->undo_file);

	com_err(तथ्य->tdb_file, err, "while force-closing undo file");
}

अटल errcode_t undo_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel	io = शून्य;
	काष्ठा undo_private_तथ्य *तथ्य = शून्य;
	पूर्णांक		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	अगर (name == 0)
		वापस EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(माप(काष्ठा struct_io_channel), &io);
	अगर (retval)
		जाओ cleanup;
	स_रखो(io, 0, माप(काष्ठा struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(माप(काष्ठा undo_private_तथ्य), &तथ्य);
	अगर (retval)
		जाओ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(म_माप(name)+1, &io->name);
	अगर (retval)
		जाओ cleanup;

	म_नकल(io->name, name);
	io->private_तथ्य = तथ्य;
	io->block_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	स_रखो(तथ्य, 0, माप(काष्ठा undo_private_तथ्य));
	तथ्य->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	तथ्य->super_blk_अंक = 1;
	तथ्य->first_कुंजी_blk = 2;
	तथ्य->undo_blk_अंक = 3;

	अगर (undo_io_backing_manager) {
		retval = undo_io_backing_manager->खोलो(name, flags,
						       &तथ्य->real);
		अगर (retval)
			जाओ cleanup;

		तथ्य->tdb_file = strdup(tdb_file);
		अगर (तथ्य->tdb_file == शून्य)
			जाओ cleanup;
		undo_fd = ext2fs_open_file(तथ्य->tdb_file, O_RDWR | O_CREAT,
					   0600);
		अगर (undo_fd < 0)
			जाओ cleanup;

		retval = undo_io_backing_manager->खोलो(तथ्य->tdb_file,
						       IO_FLAG_RW,
						       &तथ्य->undo_file);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा {
		तथ्य->real = शून्य;
		तथ्य->undo_file = शून्य;
	}

	अगर (तथ्य->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (तथ्य->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	अगर (तथ्य->real)
		undo_err_handler_init(तथ्य->real);

	अगर (तथ्य->undo_file) {
		retval = try_reopen_undo_file(undo_fd, तथ्य);
		अगर (retval)
			जाओ cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, तथ्य);
	अगर (retval)
		जाओ cleanup;

	*channel = io;
	अगर (undo_fd >= 0)
		बंद(undo_fd);
	वापस retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, तथ्य);
	अगर (undo_fd >= 0)
		बंद(undo_fd);
	अगर (तथ्य && तथ्य->undo_file)
		io_channel_close(तथ्य->undo_file);
	अगर (तथ्य && तथ्य->tdb_file)
		मुक्त(तथ्य->tdb_file);
	अगर (तथ्य && तथ्य->real)
		io_channel_close(तथ्य->real);
	अगर (तथ्य)
		ext2fs_free_mem(&तथ्य);
	अगर (io && io->name)
		ext2fs_free_mem(&io->name);
	अगर (io)
		ext2fs_free_mem(&io);
	वापस retval;
}

अटल errcode_t undo_close(io_channel channel)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (--channel->refcount > 0)
		वापस 0;
	/* Before closing write the file system identity */
	अगर (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		तथ्य->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(तथ्य, 1);
	ext2fs_remove_exit_fn(undo_atexit, तथ्य);
	अगर (तथ्य->real)
		retval = io_channel_close(तथ्य->real);
	अगर (तथ्य->tdb_file)
		मुक्त(तथ्य->tdb_file);
	अगर (तथ्य->undo_file)
		io_channel_close(तथ्य->undo_file);
	ext2fs_free_mem(&तथ्य->keyb);
	अगर (तथ्य->written_block_map)
		ext2fs_free_generic_bitmap(तथ्य->written_block_map);
	ext2fs_free_mem(&channel->private_तथ्य);
	अगर (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	अगर (err)
		वापस err;
	वापस retval;
}

अटल errcode_t undo_set_blksize(io_channel channel, पूर्णांक blksize)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (blksize > E2_UNDO_MAX_BLOCK_आकार || blksize < E2_UNDO_MIN_BLOCK_आकार)
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (तथ्य->real)
		retval = io_channel_set_blksize(तथ्य->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	अगर (!तथ्य->tdb_तथ्य_आकार || !तथ्य->tdb_written)
		तथ्य->tdb_तथ्य_आकार = blksize;
	channel->block_आकार = blksize;
	वापस retval;
}

अटल errcode_t undo_read_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	errcode_t	retval = 0;
	काष्ठा undo_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_read_blk64(तथ्य->real, block, गिनती, buf);

	वापस retval;
}

अटल errcode_t undo_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	वापस undo_read_blk64(channel, block, गिनती, buf);
}

अटल errcode_t undo_write_blk64(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, गिनती);
	अगर (retval)
		 वापस retval;
	अगर (तथ्य->real)
		retval = io_channel_write_blk64(तथ्य->real, block, गिनती, buf);

	वापस retval;
}

अटल errcode_t undo_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	वापस undo_write_blk64(channel, block, गिनती, buf);
}

अटल errcode_t undo_write_byte(io_channel channel, अचिन्हित दीर्घ offset,
				 पूर्णांक आकार, स्थिर व्योम *buf)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	अचिन्हित दीर्घ blk_अंक, गिनती;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + तथ्य->offset;
	blk_अंक = location/channel->block_आकार;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	गिनती = (आकार + (location % channel->block_आकार) +
			channel->block_आकार  -1)/channel->block_आकार;
	retval = undo_write_tdb(channel, blk_अंक, गिनती);
	अगर (retval)
		वापस retval;
	अगर (तथ्य->real && तथ्य->real->manager->write_byte)
		retval = io_channel_write_byte(तथ्य->real, offset, आकार, buf);

	वापस retval;
}

अटल errcode_t undo_discard(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	retval = 0;
	पूर्णांक icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (गिनती > पूर्णांक_उच्च)
		वापस EXT2_ET_UNIMPLEMENTED;
	icount = गिनती;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	अगर (retval)
		वापस retval;
	अगर (तथ्य->real)
		retval = io_channel_discard(तथ्य->real, block, गिनती);

	वापस retval;
}

अटल errcode_t undo_zeroout(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	retval = 0;
	पूर्णांक icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (गिनती > पूर्णांक_उच्च)
		वापस EXT2_ET_UNIMPLEMENTED;
	icount = गिनती;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	अगर (retval)
		वापस retval;
	अगर (तथ्य->real)
		retval = io_channel_zeroout(तथ्य->real, block, गिनती);

	वापस retval;
}

अटल errcode_t undo_cache_readahead(io_channel channel,
				      अचिन्हित दीर्घ दीर्घ block,
				      अचिन्हित दीर्घ दीर्घ गिनती)
{
	काष्ठा undo_private_तथ्य *तथ्य;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_cache_readahead(तथ्य->real, block, गिनती);

	वापस retval;
}

/*
 * Flush data buffers to disk.
 */
अटल errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	काष्ठा undo_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = io_channel_flush(तथ्य->real);

	वापस retval;
}

अटल errcode_t undo_set_option(io_channel channel, स्थिर अक्षर *option,
				 स्थिर अक्षर *arg)
{
	errcode_t	retval = 0;
	काष्ठा undo_private_तथ्य *तथ्य;
	अचिन्हित दीर्घ tmp;
	अक्षर *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (!म_भेद(option, "tdb_data_size")) {
		अगर (!arg)
			वापस EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		अगर (*end)
			वापस EXT2_ET_INVALID_ARGUMENT;
		अगर (tmp > E2_UNDO_MAX_BLOCK_आकार || tmp < E2_UNDO_MIN_BLOCK_आकार)
			वापस EXT2_ET_INVALID_ARGUMENT;
		अगर (!तथ्य->tdb_तथ्य_आकार || !तथ्य->tdb_written) {
			तथ्य->tdb_written = -1;
			तथ्य->tdb_तथ्य_आकार = tmp;
		}
		वापस 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	अगर (तथ्य->real && तथ्य->real->manager->set_option) {
		retval = तथ्य->real->manager->set_option(तथ्य->real,
							option, arg);
	}
	अगर (!retval && !म_भेद(option, "offset")) {
		अगर (!arg)
			वापस EXT2_ET_INVALID_ARGUMENT;

		tmp = म_से_अदीर्घ(arg, &end, 0);
		अगर (*end)
			वापस EXT2_ET_INVALID_ARGUMENT;
		तथ्य->offset = tmp;
	}
	वापस retval;
}

अटल errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	काष्ठा undo_private_तथ्य *तथ्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	तथ्य = (काष्ठा undo_private_तथ्य *) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(तथ्य, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	अगर (तथ्य->real)
		retval = (तथ्य->real->manager->get_stats)(तथ्य->real, stats);

	वापस retval;
}

अटल काष्ठा struct_io_manager struct_undo_manager = {
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
