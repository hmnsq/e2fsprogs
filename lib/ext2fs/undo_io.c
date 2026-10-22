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

#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि_परिभाषितम् __linux__
#अन्तर्भाव <sys/utsname.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_SYS_RESOURCE_H
#अन्तर्भाव <sys/resource.h>
#यदि_अन्तः
#अन्तर्भाव <limits.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि_परिभाषितम् __GNUC__
#परिभाषय ATTR(x) __attribute__(x)
#अन्यथा
#परिभाषय ATTR(x)
#यदि_अन्तः

#अपरिभाषय DEBUG

#यदि_परिभाषितम् DEBUG
# define dbg_printf(f, a...)  कुरु {प्रारूप_लिख(f, ## a); fflush(stdout); } यावत् (0)
#अन्यथा
# define dbg_printf(f, a...)
#यदि_अन्तः

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)
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
#परिभाषय E2UNDO_MAGIC "E2UNDO02"
#परिभाषय KEYBLOCK_MAGIC 0xCADECADE

#परिभाषय E2_UNDO_STATE_परिसमाप्तम्	0x1	/* undo file is complete */

#परिभाषय E2_UNDO_MIN_खण्डम्_आकार	1024	/* undo blocks are no less than 1KB */
#परिभाषय E2_UNDO_MAX_खण्डम्_आकार	1048576	/* undo blocks are no more than 1MB */

संरचना undo_शीर्षकम् {
	वर्ण magic[8];		/* "E2UNDO02" */
	__le64 अङ्कः_कुञ्चिकाः;	/* how many keys? */
	__le64 super_विस्थापनम्;	/* where in the file is the superblock copy? */
	__le64 कुञ्चिका_विस्थापनम्;	/* where do the key/data block chunks start? */
	__le32 खण्डम्_आकार;	/* block size of the undo file */
	__le32 fs_खण्डम्_आकार;	/* block size of the target device */
	__le32 sb_crc;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_विस्थापनम्;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 शीर्षकम्_crc;	/* crc32c of this header (but not this field) */
};

#परिभाषय E2_UNDO_MAX_EXTENT_खण्डानि	512	/* max extent size, in blocks */

संरचना undo_कुञ्चिका {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 आकार;		/* how many bytes in this block? */
};

संरचना undo_कुञ्चिका_खण्डम् {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#यदि __STDC_VERSION__ >= 199901L
	संरचना undo_कुञ्चिका कुञ्चिकाः[];		/* keys, which come immediately after */
#अन्यथा
	संरचना undo_कुञ्चिका कुञ्चिकाः[0];	/* keys, which come immediately after */
#यदि_अन्तः
};

संरचना undo_वैयक्तिकम्_दत्त {
	पूर्णाङ्क	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_अङ्कः;			/* next free block */
	blk64_t कुञ्चिका_blk_अङ्कः;			/* current key block location */
	blk64_t super_blk_अङ्कः;			/* superblock location */
	blk64_t first_कुञ्चिका_blk;			/* first key block location */
	संरचना undo_कुञ्चिका_खण्डम् *keyb;
	size_t अङ्कः_कुञ्चिकाः, कुञ्चिकाः_अन्तर्_खण्डम्;

	/* The backing io channel */
	io_channel real;

	अचिह्नित दीर्घ दीर्घ tdb_दत्त_आकार;
	पूर्णाङ्क tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t विस्थापनम्;

	ext2fs_खण्डम्_bitmap written_खण्डम्_map;
	संरचना struct_ext2_filsys fake_fs;
	वर्ण *tdb_file;
	संरचना undo_शीर्षकम् hdr;
};
#परिभाषय कुञ्चिकाः_प्रत्येकम्_खण्डम्(d) (((d)->tdb_दत्त_आकार / परिमाण(संरचना undo_कुञ्चिका)) - 1)

#परिभाषय E2_UNDO_FEATURE_COMPAT_FS_विस्थापनम् 0x1	/* the filesystem offset */

स्थिर अन्तःस्थ शून्य e2undo_समुच्चयः_feature_fs_विस्थापनम्(संरचना undo_शीर्षकम् *शीर्षकम्) {
	शीर्षकम्->f_compat |= ext2fs_le32_गन्तव्यम्_cpu(E2_UNDO_FEATURE_COMPAT_FS_विस्थापनम्);
}

स्थिर अन्तःस्थ शून्य e2undo_clear_feature_fs_विस्थापनम्(संरचना undo_शीर्षकम् *शीर्षकम्) {
	शीर्षकम्->f_compat &= ~ext2fs_le32_गन्तव्यम्_cpu(E2_UNDO_FEATURE_COMPAT_FS_विस्थापनम्);
}

स्थिर io_manager undo_io_backing_manager;
स्थिर वर्ण *tdb_file;
स्थिर पूर्णाङ्क actual_आकार;

errcode_t समुच्चयः_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	प्रत्यागम 0;
}

errcode_t समुच्चयः_undo_io_backup_file(वर्ण *file_name)
{
	tdb_file = strdup(file_name);

	यदि (tdb_file == रिक्त) {
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	}

	प्रत्यागम 0;
}

स्थिर errcode_t write_undo_indexes(संरचना undo_वैयक्तिकम्_दत्त *दत्त, पूर्णाङ्क flush)
{
	errcode_t retval;
	संरचना ext2_super_खण्डम् super;
	io_channel channel;
	पूर्णाङ्क खण्डम्_आकार;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	यदि (दत्त->कुञ्चिकाः_अन्तर्_खण्डम्) {
		दत्त->keyb->magic = ext2fs_cpu_गन्तव्यम्_le32(KEYBLOCK_MAGIC);
		दत्त->keyb->crc = 0;
		दत्त->keyb->crc = ext2fs_cpu_गन्तव्यम्_le32(
					 ext2fs_crc32c_le(~0,
					 (अचिह्नित वर्ण *)दत्त->keyb,
					 दत्त->tdb_दत्त_आकार));
		dbg_printf("Writing keyblock to blk %llu\n", दत्त->कुञ्चिका_blk_अङ्कः);
		retval = io_channel_write_blk64(दत्त->undo_file,
						दत्त->कुञ्चिका_blk_अङ्कः,
						1, दत्त->keyb);
		यदि (retval)
			प्रत्यागम retval;
		/* Move on to the next key block if it's full. */
		यदि (दत्त->कुञ्चिकाः_अन्तर्_खण्डम् == कुञ्चिकाः_प्रत्येकम्_खण्डम्(दत्त)) {
			स्मृतिं_पूरय(दत्त->keyb, 0, दत्त->tdb_दत्त_आकार);
			दत्त->कुञ्चिकाः_अन्तर्_खण्डम् = 0;
			दत्त->कुञ्चिका_blk_अङ्कः = दत्त->undo_blk_अङ्कः;
			दत्त->undo_blk_अङ्कः++;
		}
	}

	/* Prepare superblock for write */
	channel = दत्त->real;
	खण्डम्_आकार = channel->खण्डम्_आकार;

	io_channel_समुच्चयः_blksize(channel, SUPERBLOCK_विस्थापनम्);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_आकार, &super);
	यदि (retval)
		गच्छ err_बहिर्गमनम्;
	sb_crc = ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)&super, SUPERBLOCK_आकार);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	स्मृतिं_प्रतिलिख(दत्त->hdr.magic, E2UNDO_MAGIC, परिमाण(दत्त->hdr.magic));
	दत्त->hdr.अङ्कः_कुञ्चिकाः = ext2fs_cpu_गन्तव्यम्_le64(दत्त->अङ्कः_कुञ्चिकाः);
	दत्त->hdr.super_विस्थापनम् = ext2fs_cpu_गन्तव्यम्_le64(दत्त->super_blk_अङ्कः);
	दत्त->hdr.कुञ्चिका_विस्थापनम् = ext2fs_cpu_गन्तव्यम्_le64(दत्त->first_कुञ्चिका_blk);
	दत्त->hdr.fs_खण्डम्_आकार = ext2fs_cpu_गन्तव्यम्_le32(खण्डम्_आकार);
	दत्त->hdr.sb_crc = ext2fs_cpu_गन्तव्यम्_le32(sb_crc);
	दत्त->hdr.fs_विस्थापनम् = ext2fs_cpu_गन्तव्यम्_le64(दत्त->विस्थापनम्);
	यदि (दत्त->विस्थापनम्)
		e2undo_समुच्चयः_feature_fs_विस्थापनम्(&दत्त->hdr);
	अन्यथा
		e2undo_clear_feature_fs_विस्थापनम्(&दत्त->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)&दत्त->hdr,
				   परिमाण(दत्त->hdr) -
				   परिमाण(दत्त->hdr.शीर्षकम्_crc));
	दत्त->hdr.शीर्षकम्_crc = ext2fs_cpu_गन्तव्यम्_le32(hdr_crc);
	retval = io_channel_write_blk64(दत्त->undo_file, 0,
					-(पूर्णाङ्क)परिमाण(दत्त->hdr),
					&दत्त->hdr);
	यदि (retval)
		गच्छ err_बहिर्गमनम्;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", दत्त->super_blk_अङ्कः);
	retval = io_channel_write_blk64(दत्त->undo_file, दत्त->super_blk_अङ्कः,
					-SUPERBLOCK_आकार, &super);
	यदि (retval)
		गच्छ err_बहिर्गमनम्;

	यदि (flush)
		retval = io_channel_flush(दत्त->undo_file);
err_बहिर्गमनम्:
	io_channel_समुच्चयः_blksize(channel, खण्डम्_आकार);
	प्रत्यागम retval;
}

स्थिर errcode_t undo_सज्जता_tdb(संरचना undo_वैयक्तिकम्_दत्त *दत्त)
{
	पूर्णाङ्क i;
	errcode_t retval;

	यदि (दत्त->tdb_written == 1)
		प्रत्यागम 0;

	दत्त->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	स्मृतिं_पूरय(&दत्त->fake_fs, 0, परिमाण(दत्त->fake_fs));
	दत्त->fake_fs.blocksize = दत्त->tdb_दत्त_आकार;
	retval = ext2fs_alloc_generic_bmap(&दत्त->fake_fs,
				EXT2_ET_MAGIC_खण्डम्_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &दत्त->written_खण्डम्_map);
	यदि (retval)
		प्रत्यागम retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(दत्त->tdb_दत्त_आकार, &दत्त->keyb);
	यदि (retval)
		प्रत्यागम retval;
	दत्त->कुञ्चिका_blk_अङ्कः = दत्त->first_कुञ्चिका_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", दत्त->tdb_दत्त_आकार);
	dbg_printf("Keys per block %llu\n", कुञ्चिकाः_प्रत्येकम्_खण्डम्(दत्त));
	दत्त->hdr.खण्डम्_आकार = ext2fs_cpu_गन्तव्यम्_le32(दत्त->tdb_दत्त_आकार);
	io_channel_समुच्चयः_blksize(दत्त->undo_file, दत्त->tdb_दत्त_आकार);

	/* Ensure that we have space for header blocks */
	प्रति (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(दत्त->undo_file, i, 1,
					       दत्त->keyb);
		यदि (retval)
			स्मृतिं_पूरय(दत्त->keyb, 0, दत्त->tdb_दत्त_आकार);
		retval = io_channel_write_blk64(दत्त->undo_file, i, 1,
						दत्त->keyb);
		यदि (retval)
			प्रत्यागम retval;
		retval = io_channel_flush(दत्त->undo_file);
		यदि (retval)
			प्रत्यागम retval;
	}
	स्मृतिं_पूरय(दत्त->keyb, 0, दत्त->tdb_दत्त_आकार);
	प्रत्यागम 0;
}

स्थिर errcode_t undo_write_tdb(io_channel channel,
				अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्)

{
	पूर्णाङ्क आकार, sz;
	अचिह्नित दीर्घ दीर्घ खण्डम्_अङ्कः, backing_blk_अङ्कः;
	errcode_t retval = 0;
	ext2_loff_t विस्थापनम्;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	अचिह्नित वर्ण *read_ptr;
	अचिह्नित दीर्घ दीर्घ अन्तः_खण्डम्;
	अचिह्नित दीर्घ दीर्घ दत्त_आकार;
	संरचना undo_कुञ्चिका *कुञ्चिका;
	__u32 blk_crc;

	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;

	यदि (दत्त->undo_file == रिक्त) {
		/*
		 * Transaction database not initialized
		 */
		प्रत्यागम 0;
	}

	यदि (गणनम् == 1)
		आकार = channel->खण्डम्_आकार;
	अन्यथा {
		यदि (गणनम् < 0)
			आकार = -गणनम्;
		अन्यथा
			आकार = गणनम् * channel->खण्डम्_आकार;
	}

	retval = undo_सज्जता_tdb(दत्त);
	यदि (retval)
		प्रत्यागम retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	विस्थापनम् = (खण्डम् * channel->खण्डम्_आकार) + दत्त->विस्थापनम् ;
	खण्डम्_अङ्कः = विस्थापनम् / दत्त->tdb_दत्त_आकार;
	अन्तः_खण्डम् = (विस्थापनम् + आकार - 1) / दत्त->tdb_दत्त_आकार;

	यावत् (खण्डम्_अङ्कः <= अन्तः_खण्डम्) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		यदि (ext2fs_test_खण्डम्_bitmap2(दत्त->written_खण्डम्_map,
						   खण्डम्_अङ्कः)) {
			/* Try the next block */
			खण्डम्_अङ्कः++;
			अनुवर्त;
		}
		ext2fs_mark_खण्डम्_bitmap2(दत्त->written_खण्डम्_map, खण्डम्_अङ्कः);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		विस्थापनम् = खण्डम्_अङ्कः * दत्त->tdb_दत्त_आकार +
				(दत्त->विस्थापनम् % दत्त->tdb_दत्त_आकार);
		backing_blk_अङ्कः = (विस्थापनम् - दत्त->विस्थापनम्) / channel->खण्डम्_आकार;

		retval = ext2fs_get_mem(दत्त->tdb_दत्त_आकार, &read_ptr);
		यदि (retval) {
			प्रत्यागम retval;
		}

		स्मृतिं_पूरय(read_ptr, 0, दत्त->tdb_दत्त_आकार);
		actual_आकार = 0;
		यदि ((दत्त->tdb_दत्त_आकार % channel->खण्डम्_आकार) == 0)
			sz = दत्त->tdb_दत्त_आकार / channel->खण्डम्_आकार;
		अन्यथा
			sz = -दत्त->tdb_दत्त_आकार;
		retval = io_channel_read_blk64(दत्त->real, backing_blk_अङ्कः,
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
			दत्त_आकार = actual_आकार;
		} अन्यथा {
			दत्त_आकार = दत्त->tdb_दत्त_आकार;
		}
		यदि (दत्त_आकार == 0) {
			मुक्त(read_ptr);
			खण्डम्_अङ्कः++;
			अनुवर्त;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       दत्त_आकार, backing_blk_अङ्कः, खण्डम्, दत्त->tdb_दत्त_आकार);
		यदि ((दत्त_आकार % दत्त->undo_file->खण्डम्_आकार) == 0)
			sz = दत्त_आकार / दत्त->undo_file->खण्डम्_आकार;
		अन्यथा
			sz = -दत्त_आकार;;
		/* extend this key? */
		यदि (दत्त->कुञ्चिकाः_अन्तर्_खण्डम्) {
			कुञ्चिका = दत्त->keyb->कुञ्चिकाः + दत्त->कुञ्चिकाः_अन्तर्_खण्डम् - 1;
			keysz = ext2fs_le32_गन्तव्यम्_cpu(कुञ्चिका->आकार);
		} अन्यथा {
			कुञ्चिका = रिक्त;
			keysz = 0;
		}
		यदि (कुञ्चिका != रिक्त &&
		    (ext2fs_le64_गन्तव्यम्_cpu(कुञ्चिका->fsblk) * channel->खण्डम्_आकार +
		     channel->खण्डम्_आकार - 1 +
		     keysz) / channel->खण्डम्_आकार == backing_blk_अङ्कः &&
		    E2_UNDO_MAX_EXTENT_खण्डानि * दत्त->tdb_दत्त_आकार >
		    keysz + दत्त_आकार) {
			blk_crc = ext2fs_le32_गन्तव्यम्_cpu(कुञ्चिका->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, दत्त_आकार);
			कुञ्चिका->blk_crc = ext2fs_cpu_गन्तव्यम्_le32(blk_crc);
			कुञ्चिका->आकार = ext2fs_cpu_गन्तव्यम्_le32(keysz + दत्त_आकार);
		} अन्यथा {
			दत्त->अङ्कः_कुञ्चिकाः++;
			कुञ्चिका = दत्त->keyb->कुञ्चिकाः + दत्त->कुञ्चिकाः_अन्तर्_खण्डम्;
			दत्त->कुञ्चिकाः_अन्तर्_खण्डम्++;
			कुञ्चिका->fsblk = ext2fs_cpu_गन्तव्यम्_le64(backing_blk_अङ्कः);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, दत्त_आकार);
			कुञ्चिका->blk_crc = ext2fs_cpu_गन्तव्यम्_le32(blk_crc);
			कुञ्चिका->आकार = ext2fs_cpu_गन्तव्यम्_le32(दत्त_आकार);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       खण्डम्_अङ्कः,
		       दत्त->undo_blk_अङ्कः,
		       sz, दत्त->अङ्कः_कुञ्चिकाः - 1);
		retval = io_channel_write_blk64(दत्त->undo_file,
					दत्त->undo_blk_अङ्कः, sz, read_ptr);
		यदि (retval) {
			मुक्त(read_ptr);
			प्रत्यागम retval;
		}
		दत्त->undo_blk_अङ्कः++;
		मुक्त(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(दत्त, 0);
		यदि (retval)
			प्रत्यागम retval;

		/* Next block */
		खण्डम्_अङ्कः++;
	}

	प्रत्यागम retval;
}

स्थिर errcode_t undo_io_read_error(io_channel channel ATTR((अप्रयुक्तम्)),
				    अचिह्नित दीर्घ खण्डम् ATTR((अप्रयुक्तम्)),
				    पूर्णाङ्क गणनम् ATTR((अप्रयुक्तम्)),
				    शून्य *दत्त ATTR((अप्रयुक्तम्)),
				    size_t आकार ATTR((अप्रयुक्तम्)),
				    पूर्णाङ्क actual,
				    errcode_t error ATTR((अप्रयुक्तम्)))
{
	actual_आकार = actual;
	प्रत्यागम error;
}

स्थिर शून्य undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

स्थिर पूर्णाङ्क check_filesystem(संरचना undo_शीर्षकम् *hdr, io_channel undo_file,
			    अचिह्नित पूर्णाङ्क blocksize, blk64_t super_खण्डम्,
			    io_channel channel)
{
	संरचना ext2_super_खण्डम् super, *sb;
	वर्ण *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_समुच्चयः_blksize(channel, SUPERBLOCK_विस्थापनम्);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_आकार, &super);
	यदि (retval)
		प्रत्यागम retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;
	retval = io_channel_read_blk64(undo_file, super_खण्डम्,
				       -SUPERBLOCK_आकार, buf);
	यदि (retval)
		गच्छ बहिर्गमनम्;
	sb = (संरचना ext2_super_खण्डम् *)buf;
	sb->s_magic = ~sb->s_magic;
	यदि (memcmp(&super, buf, परिमाण(super))) {
		retval = -1;
		गच्छ बहिर्गमनम्;
	}
	sb_crc = ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)buf, SUPERBLOCK_आकार);
	यदि (ext2fs_le32_गन्तव्यम्_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		गच्छ बहिर्गमनम्;
	}

बहिर्गमनम्:
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
				      संरचना undo_वैयक्तिकम्_दत्त *दत्त)
{
	संरचना undo_शीर्षकम् hdr;
	संरचना undo_कुञ्चिका *dkey;
	ext2fs_struct_stat statbuf;
	अचिह्नित पूर्णाङ्क blocksize, fs_blocksize;
	blk64_t super_खण्डम्, lblk;
	size_t अङ्कः_कुञ्चिकाः, कुञ्चिकाः_प्रत्येकम्_खण्डम्_2, i;
	__u32 hdr_crc, कुञ्चिका_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	यदि (retval)
		गच्छ bad_file;
	यदि (statbuf.st_आकार == 0)
		गच्छ बहिर्गमनम्;

	/* check the file header */
	retval = io_channel_read_blk64(दत्त->undo_file, 0, -(पूर्णाङ्क)परिमाण(hdr),
				       &hdr);
	यदि (retval)
		गच्छ bad_file;

	यदि (memcmp(hdr.magic, E2UNDO_MAGIC,
		    परिमाण(hdr.magic)))
		गच्छ bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)&hdr,
				   परिमाण(संरचना undo_शीर्षकम्) -
				   परिमाण(__u32));
	यदि (ext2fs_le32_गन्तव्यम्_cpu(hdr.शीर्षकम्_crc) != hdr_crc)
		गच्छ bad_file;
	blocksize = ext2fs_le32_गन्तव्यम्_cpu(hdr.खण्डम्_आकार);
	fs_blocksize = ext2fs_le32_गन्तव्यम्_cpu(hdr.fs_खण्डम्_आकार);
	यदि (blocksize > E2_UNDO_MAX_खण्डम्_आकार ||
	    blocksize < E2_UNDO_MIN_खण्डम्_आकार ||
	    !blocksize || !fs_blocksize)
		गच्छ bad_file;
	super_खण्डम् = ext2fs_le64_गन्तव्यम्_cpu(hdr.super_विस्थापनम्);
	अङ्कः_कुञ्चिकाः = ext2fs_le64_गन्तव्यम्_cpu(hdr.अङ्कः_कुञ्चिकाः);
	io_channel_समुच्चयः_blksize(दत्त->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	यदि (hdr.f_incompat || hdr.f_rocompat)
		गच्छ bad_file;

	/* Superblock matches this FS? */
	यदि (check_filesystem(&hdr, दत्त->undo_file, blocksize, super_खण्डम्,
			     दत्त->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		गच्छ बहिर्गमनम्;
	}

	/* Try to set ourselves up */
	दत्त->tdb_दत्त_आकार = blocksize;
	retval = undo_सज्जता_tdb(दत्त);
	यदि (retval)
		गच्छ bad_file;
	दत्त->अङ्कः_कुञ्चिकाः = अङ्कः_कुञ्चिकाः;
	दत्त->super_blk_अङ्कः = super_खण्डम्;
	दत्त->first_कुञ्चिका_blk = ext2fs_le64_गन्तव्यम्_cpu(hdr.कुञ्चिका_विस्थापनम्);

	/* load the written block map */
	कुञ्चिकाः_प्रत्येकम्_खण्डम्_2 = कुञ्चिकाः_प्रत्येकम्_खण्डम्(दत्त);
	lblk = दत्त->first_कुञ्चिका_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   अङ्कः_कुञ्चिकाः, कुञ्चिकाः_प्रत्येकम्_खण्डम्_2, blocksize);
	प्रति (i = 0; i < अङ्कः_कुञ्चिकाः; i += कुञ्चिकाः_प्रत्येकम्_खण्डम्_2) {
		size_t j, max_j;
		__le32 crc;

		दत्त->कुञ्चिका_blk_अङ्कः = lblk;
		retval = io_channel_read_blk64(दत्त->undo_file,
					       lblk, 1, दत्त->keyb);
		यदि (retval)
			गच्छ bad_कुञ्चिका_replay;

		/* check keys */
		यदि (ext2fs_le32_गन्तव्यम्_cpu(दत्त->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुञ्चिका_replay;
		}
		crc = दत्त->keyb->crc;
		दत्त->keyb->crc = 0;
		कुञ्चिका_crc = ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)दत्त->keyb,
					   blocksize);
		यदि (ext2fs_le32_गन्तव्यम्_cpu(crc) != कुञ्चिका_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			गच्छ bad_कुञ्चिका_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = दत्त->अङ्कः_कुञ्चिकाः - i;
		यदि (max_j > कुञ्चिकाः_प्रत्येकम्_खण्डम्_2)
			max_j = कुञ्चिकाः_प्रत्येकम्_खण्डम्_2;
		प्रति (j = 0, dkey = दत्त->keyb->कुञ्चिकाः;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_गन्तव्यम्_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t आकार = ext2fs_le32_गन्तव्यम्_cpu(dkey->आकार);

			ext2fs_mark_खण्डम्_bitmap_range2(दत्त->written_खण्डम्_map,
					 undo_blk,
					(आकार + blocksize - 1) / blocksize);
			lblk += (आकार + blocksize - 1) / blocksize;
			दत्त->undo_blk_अङ्कः = lblk;
			दत्त->कुञ्चिकाः_अन्तर्_खण्डम् = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   दत्त->कुञ्चिका_blk_अङ्कः, दत्त->undo_blk_अङ्कः, दत्त->अङ्कः_कुञ्चिकाः,
		   दत्त->कुञ्चिकाः_अन्तर्_खण्डम्);

	दत्त->hdr.state = hdr.state & ~E2_UNDO_STATE_परिसमाप्तम्;
	दत्त->hdr.f_compat = hdr.f_compat;
	दत्त->hdr.f_incompat = hdr.f_incompat;
	दत्त->hdr.f_rocompat = hdr.f_rocompat;
	प्रत्यागम retval;

bad_कुञ्चिका_replay:
	दत्त->कुञ्चिका_blk_अङ्कः = दत्त->undo_blk_अङ्कः = 0;
	दत्त->कुञ्चिकाः_अन्तर्_खण्डम् = 0;
	ext2fs_free_mem(&दत्त->keyb);
	ext2fs_free_generic_bitmap(दत्त->written_खण्डम्_map);
	दत्त->tdb_written = 0;
	गच्छ बहिर्गमनम्;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
बहिर्गमनम्:
	प्रत्यागम retval;
}

स्थिर शून्य undo_atexit(शून्य *p)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त = p;
	errcode_t err;

	err = write_undo_indexes(दत्त, 1);
	io_channel_close(दत्त->undo_file);

	com_err(दत्त->tdb_file, err, "while force-closing undo file");
}

स्थिर errcode_t undo_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel	io = रिक्त;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त = रिक्त;
	पूर्णाङ्क		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_तन्तवः;
	यदि (name == 0)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(परिमाण(संरचना struct_io_channel), &io);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(io, 0, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(परिमाण(संरचना undo_वैयक्तिकम्_दत्त), &दत्त);
	यदि (retval)
		गच्छ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &io->name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(io->name, name);
	io->वैयक्तिकम्_दत्त = दत्त;
	io->खण्डम्_आकार = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	स्मृतिं_पूरय(दत्त, 0, परिमाण(संरचना undo_वैयक्तिकम्_दत्त));
	दत्त->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	दत्त->super_blk_अङ्कः = 1;
	दत्त->first_कुञ्चिका_blk = 2;
	दत्त->undo_blk_अङ्कः = 3;

	यदि (undo_io_backing_manager) {
		retval = undo_io_backing_manager->उद्घाटय(name, flags,
						       &दत्त->real);
		यदि (retval)
			गच्छ cleanup;

		दत्त->tdb_file = strdup(tdb_file);
		यदि (दत्त->tdb_file == रिक्त)
			गच्छ cleanup;
		undo_fd = ext2fs_open_file(दत्त->tdb_file, O_RDWR | O_CREAT,
					   0600);
		यदि (undo_fd < 0)
			गच्छ cleanup;

		retval = undo_io_backing_manager->उद्घाटय(दत्त->tdb_file,
						       IO_FLAG_RW,
						       &दत्त->undo_file);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		दत्त->real = रिक्त;
		दत्त->undo_file = रिक्त;
	}

	यदि (दत्त->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (दत्त->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	यदि (दत्त->real)
		undo_err_handler_init(दत्त->real);

	यदि (दत्त->undo_file) {
		retval = try_reopen_undo_file(undo_fd, दत्त);
		यदि (retval)
			गच्छ cleanup;
	}
	retval = ext2fs_सङ्कलय_exit_fn(undo_atexit, दत्त);
	यदि (retval)
		गच्छ cleanup;

	*channel = io;
	यदि (undo_fd >= 0)
		close(undo_fd);
	प्रत्यागम retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, दत्त);
	यदि (undo_fd >= 0)
		close(undo_fd);
	यदि (दत्त && दत्त->undo_file)
		io_channel_close(दत्त->undo_file);
	यदि (दत्त && दत्त->tdb_file)
		मुक्त(दत्त->tdb_file);
	यदि (दत्त && दत्त->real)
		io_channel_close(दत्त->real);
	यदि (दत्त)
		ext2fs_free_mem(&दत्त);
	यदि (io && io->name)
		ext2fs_free_mem(&io->name);
	यदि (io)
		ext2fs_free_mem(&io);
	प्रत्यागम retval;
}

स्थिर errcode_t undo_close(io_channel channel)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (--channel->refcount > 0)
		प्रत्यागम 0;
	/* Before closing write the file system identity */
	यदि (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		दत्त->hdr.state = ext2fs_cpu_गन्तव्यम्_le32(E2_UNDO_STATE_परिसमाप्तम्);
	err = write_undo_indexes(दत्त, 1);
	ext2fs_remove_exit_fn(undo_atexit, दत्त);
	यदि (दत्त->real)
		retval = io_channel_close(दत्त->real);
	यदि (दत्त->tdb_file)
		मुक्त(दत्त->tdb_file);
	यदि (दत्त->undo_file)
		io_channel_close(दत्त->undo_file);
	ext2fs_free_mem(&दत्त->keyb);
	यदि (दत्त->written_खण्डम्_map)
		ext2fs_free_generic_bitmap(दत्त->written_खण्डम्_map);
	ext2fs_free_mem(&channel->वैयक्तिकम्_दत्त);
	यदि (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	यदि (err)
		प्रत्यागम err;
	प्रत्यागम retval;
}

स्थिर errcode_t undo_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (blksize > E2_UNDO_MAX_खण्डम्_आकार || blksize < E2_UNDO_MIN_खण्डम्_आकार)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (दत्त->real)
		retval = io_channel_समुच्चयः_blksize(दत्त->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	यदि (!दत्त->tdb_दत्त_आकार || !दत्त->tdb_written)
		दत्त->tdb_दत्त_आकार = blksize;
	channel->खण्डम्_आकार = blksize;
	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	errcode_t	retval = 0;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_read_blk64(दत्त->real, खण्डम्, गणनम्, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम undo_read_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t undo_write_blk64(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, खण्डम्, गणनम्);
	यदि (retval)
		 प्रत्यागम retval;
	यदि (दत्त->real)
		retval = io_channel_write_blk64(दत्त->real, खण्डम्, गणनम्, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम undo_write_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t undo_write_byte(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				 पूर्णाङ्क आकार, ध्रुव शून्य *buf)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	अचिह्नित दीर्घ blk_अङ्कः, गणनम्;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = विस्थापनम् + दत्त->विस्थापनम्;
	blk_अङ्कः = location/channel->खण्डम्_आकार;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	गणनम् = (आकार + (location % channel->खण्डम्_आकार) +
			channel->खण्डम्_आकार  -1)/channel->खण्डम्_आकार;
	retval = undo_write_tdb(channel, blk_अङ्कः, गणनम्);
	यदि (retval)
		प्रत्यागम retval;
	यदि (दत्त->real && दत्त->real->manager->write_byte)
		retval = io_channel_write_byte(दत्त->real, विस्थापनम्, आकार, buf);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_discard(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (गणनम् > अधिकतम_पूर्णाङ्कः)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गणनम्;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, खण्डम्, icount);
	यदि (retval)
		प्रत्यागम retval;
	यदि (दत्त->real)
		retval = io_channel_discard(दत्त->real, खण्डम्, गणनम्);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_zeroout(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;
	पूर्णाङ्क icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (गणनम् > अधिकतम_पूर्णाङ्कः)
		प्रत्यागम EXT2_ET_UNIMPLEMENTED;
	icount = गणनम्;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, खण्डम्, icount);
	यदि (retval)
		प्रत्यागम retval;
	यदि (दत्त->real)
		retval = io_channel_zeroout(दत्त->real, खण्डम्, गणनम्);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_cache_readahead(io_channel channel,
				      अचिह्नित दीर्घ दीर्घ खण्डम्,
				      अचिह्नित दीर्घ दीर्घ गणनम्)
{
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_cache_readahead(दत्त->real, खण्डम्, गणनम्);

	प्रत्यागम retval;
}

/*
 * Flush data buffers to disk.
 */
स्थिर errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (दत्त->real)
		retval = io_channel_flush(दत्त->real);

	प्रत्यागम retval;
}

स्थिर errcode_t undo_समुच्चयः_option(io_channel channel, ध्रुव वर्ण *option,
				 ध्रुव वर्ण *arg)
{
	errcode_t	retval = 0;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;
	अचिह्नित दीर्घ tmp;
	वर्ण *अन्तः;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (!वाक्यं_तुलय(option, "tdb_data_size")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		tmp = strtoul(arg, &अन्तः, 0);
		यदि (*अन्तः)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		यदि (tmp > E2_UNDO_MAX_खण्डम्_आकार || tmp < E2_UNDO_MIN_खण्डम्_आकार)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		यदि (!दत्त->tdb_दत्त_आकार || !दत्त->tdb_written) {
			दत्त->tdb_written = -1;
			दत्त->tdb_दत्त_आकार = tmp;
		}
		प्रत्यागम 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	यदि (दत्त->real && दत्त->real->manager->समुच्चयः_option) {
		retval = दत्त->real->manager->समुच्चयः_option(दत्त->real,
							option, arg);
	}
	यदि (!retval && !वाक्यं_तुलय(option, "offset")) {
		यदि (!arg)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		tmp = strtoul(arg, &अन्तः, 0);
		यदि (*अन्तः)
			प्रत्यागम EXT2_ET_INVALID_तर्कः;
		दत्त->विस्थापनम् = tmp;
	}
	प्रत्यागम retval;
}

स्थिर errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	संरचना undo_वैयक्तिकम्_दत्त *दत्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	दत्त = (संरचना undo_वैयक्तिकम्_दत्त *) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(दत्त, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	यदि (दत्त->real)
		retval = (दत्त->real->manager->get_stats)(दत्त->real, stats);

	प्रत्यागम retval;
}

स्थिर संरचना struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.उद्घाटय		= undo_open,
	.close		= undo_close,
	.समुच्चयः_blksize	= undo_समुच्चयः_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.समुच्चयः_option	= undo_समुच्चयः_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
