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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#నిర్వచితమైతే __linux__
#అన్తర్భావ <sys/utsname.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_RESOURCE_H
#అన్తర్భావ <sys/resource.h>
#అయితే_ముగింపు
#అన్తర్భావ <limits.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచితమైతే __GNUC__
#నిర్వచించు ATTR(x) __attribute__(x)
#లేకపోతే
#నిర్వచించు ATTR(x)
#అయితే_ముగింపు

#నిర్వచనం_రద్దు DEBUG

#నిర్వచితమైతే DEBUG
# define dbg_printf(f, a...)  చేయి {రాయి(f, ## a); fflush(stdout); } అయితే (0)
#లేకపోతే
# define dbg_printf(f, a...)
#అయితే_ముగింపు

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)
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
#నిర్వచించు E2UNDO_MAGIC "E2UNDO02"
#నిర్వచించు KEYBLOCK_MAGIC 0xCADECADE

#నిర్వచించు E2_UNDO_STATE_ముగిసిన	0x1	/* undo file is complete */

#నిర్వచించు E2_UNDO_MIN_ఖండం_కొలత	1024	/* undo blocks are no less than 1KB */
#నిర్వచించు E2_UNDO_MAX_ఖండం_కొలత	1048576	/* undo blocks are no more than 1MB */

నిర్మాణం undo_శీర్షిక {
	అక్షరం magic[8];		/* "E2UNDO02" */
	__le64 అంకె_కీలకాలు;	/* how many keys? */
	__le64 super_స్థానభ్రంశం;	/* where in the file is the superblock copy? */
	__le64 కీలకం_స్థానభ్రంశం;	/* where do the key/data block chunks start? */
	__le32 ఖండం_కొలత;	/* block size of the undo file */
	__le32 fs_ఖండం_కొలత;	/* block size of the target device */
	__le32 sb_చక్రీయ_తనిఖీ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_స్థానభ్రంశం;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 శీర్షిక_చక్రీయ_తనిఖీ;	/* crc32c of this header (but not this field) */
};

#నిర్వచించు E2_UNDO_MAX_EXTENT_ఖండాలు	512	/* max extent size, in blocks */

నిర్మాణం undo_కీలకం {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_చక్రీయ_తనిఖీ;		/* crc32c of the block */
	__le32 కొలత;		/* how many bytes in this block? */
};

నిర్మాణం undo_కీలకం_ఖండం {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 చక్రీయ_తనిఖీ;		/* block checksum */
	__le64 reserved;	/* zero */

#ఒకవేళ __STDC_VERSION__ >= 199901L
	నిర్మాణం undo_కీలకం కీలకాలు[];		/* keys, which come immediately after */
#లేకపోతే
	నిర్మాణం undo_కీలకం కీలకాలు[0];	/* keys, which come immediately after */
#అయితే_ముగింపు
};

నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_అంకె;			/* next free block */
	blk64_t కీలకం_blk_అంకె;			/* current key block location */
	blk64_t super_blk_అంకె;			/* superblock location */
	blk64_t మొదటి_కీలకం_blk;			/* first key block location */
	నిర్మాణం undo_కీలకం_ఖండం *keyb;
	size_t అంకె_కీలకాలు, కీలకాలు_లోపల_ఖండం;

	/* The backing io channel */
	io_channel real;

	గుర్తులేని దీర్ఘ దీర్ఘ tdb_దత్తాంశం_కొలత;
	పూర్ణాంకం tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t స్థానభ్రంశం;

	ext2fs_ఖండం_bitmap written_ఖండం_map;
	నిర్మాణం struct_ext2_filsys fake_fs;
	అక్షరం *tdb_file;
	నిర్మాణం undo_శీర్షిక hdr;
};
#నిర్వచించు కీలకాలు_చొప్పున_ఖండం(d) (((d)->tdb_దత్తాంశం_కొలత / పరిమాణం(నిర్మాణం undo_కీలకం)) - 1)

#నిర్వచించు E2_UNDO_FEATURE_COMPAT_FS_స్థానభ్రంశం 0x1	/* the filesystem offset */

స్థిర అంతఃవరుస శూన్య e2undo_సమితి_feature_fs_స్థానభ్రంశం(నిర్మాణం undo_శీర్షిక *శీర్షిక) {
	శీర్షిక->f_compat |= ext2fs_le32_గమ్యం_cpu(E2_UNDO_FEATURE_COMPAT_FS_స్థానభ్రంశం);
}

స్థిర అంతఃవరుస శూన్య e2undo_clear_feature_fs_స్థానభ్రంశం(నిర్మాణం undo_శీర్షిక *శీర్షిక) {
	శీర్షిక->f_compat &= ~ext2fs_le32_గమ్యం_cpu(E2_UNDO_FEATURE_COMPAT_FS_స్థానభ్రంశం);
}

స్థిర io_manager undo_io_backing_manager;
స్థిర అక్షరం *tdb_file;
స్థిర పూర్ణాంకం actual_కొలత;

errcode_t సమితి_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	తిరిగి 0;
}

errcode_t సమితి_undo_io_backup_file(అక్షరం *file_name)
{
	tdb_file = strdup(file_name);

	ఒకవేళ (tdb_file == ఖాళీ) {
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	}

	తిరిగి 0;
}

స్థిర errcode_t write_undo_indexes(నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం, పూర్ణాంకం flush)
{
	errcode_t retval;
	నిర్మాణం ext2_super_ఖండం super;
	io_channel channel;
	పూర్ణాంకం ఖండం_కొలత;
	__u32 sb_చక్రీయ_తనిఖీ, hdr_చక్రీయ_తనిఖీ;

	/* Spit out a key block, if there's any data */
	ఒకవేళ (దత్తాంశం->కీలకాలు_లోపల_ఖండం) {
		దత్తాంశం->keyb->magic = ext2fs_cpu_గమ్యం_le32(KEYBLOCK_MAGIC);
		దత్తాంశం->keyb->చక్రీయ_తనిఖీ = 0;
		దత్తాంశం->keyb->చక్రీయ_తనిఖీ = ext2fs_cpu_గమ్యం_le32(
					 ext2fs_crc32c_le(~0,
					 (గుర్తులేని అక్షరం *)దత్తాంశం->keyb,
					 దత్తాంశం->tdb_దత్తాంశం_కొలత));
		dbg_printf("Writing keyblock to blk %llu\n", దత్తాంశం->కీలకం_blk_అంకె);
		retval = io_channel_write_blk64(దత్తాంశం->undo_file,
						దత్తాంశం->కీలకం_blk_అంకె,
						1, దత్తాంశం->keyb);
		ఒకవేళ (retval)
			తిరిగి retval;
		/* Move on to the next key block if it's full. */
		ఒకవేళ (దత్తాంశం->కీలకాలు_లోపల_ఖండం == కీలకాలు_చొప్పున_ఖండం(దత్తాంశం)) {
			memset(దత్తాంశం->keyb, 0, దత్తాంశం->tdb_దత్తాంశం_కొలత);
			దత్తాంశం->కీలకాలు_లోపల_ఖండం = 0;
			దత్తాంశం->కీలకం_blk_అంకె = దత్తాంశం->undo_blk_అంకె;
			దత్తాంశం->undo_blk_అంకె++;
		}
	}

	/* Prepare superblock for write */
	channel = దత్తాంశం->real;
	ఖండం_కొలత = channel->ఖండం_కొలత;

	io_channel_సమితి_blksize(channel, SUPERBLOCK_స్థానభ్రంశం);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_కొలత, &super);
	ఒకవేళ (retval)
		వెళ్ళు err_బహిర్గతం;
	sb_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)&super, SUPERBLOCK_కొలత);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	జ్ఞాపకం_నకలు(దత్తాంశం->hdr.magic, E2UNDO_MAGIC, పరిమాణం(దత్తాంశం->hdr.magic));
	దత్తాంశం->hdr.అంకె_కీలకాలు = ext2fs_cpu_గమ్యం_le64(దత్తాంశం->అంకె_కీలకాలు);
	దత్తాంశం->hdr.super_స్థానభ్రంశం = ext2fs_cpu_గమ్యం_le64(దత్తాంశం->super_blk_అంకె);
	దత్తాంశం->hdr.కీలకం_స్థానభ్రంశం = ext2fs_cpu_గమ్యం_le64(దత్తాంశం->మొదటి_కీలకం_blk);
	దత్తాంశం->hdr.fs_ఖండం_కొలత = ext2fs_cpu_గమ్యం_le32(ఖండం_కొలత);
	దత్తాంశం->hdr.sb_చక్రీయ_తనిఖీ = ext2fs_cpu_గమ్యం_le32(sb_చక్రీయ_తనిఖీ);
	దత్తాంశం->hdr.fs_స్థానభ్రంశం = ext2fs_cpu_గమ్యం_le64(దత్తాంశం->స్థానభ్రంశం);
	ఒకవేళ (దత్తాంశం->స్థానభ్రంశం)
		e2undo_సమితి_feature_fs_స్థానభ్రంశం(&దత్తాంశం->hdr);
	లేకపోతే
		e2undo_clear_feature_fs_స్థానభ్రంశం(&దత్తాంశం->hdr);
	hdr_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)&దత్తాంశం->hdr,
				   పరిమాణం(దత్తాంశం->hdr) -
				   పరిమాణం(దత్తాంశం->hdr.శీర్షిక_చక్రీయ_తనిఖీ));
	దత్తాంశం->hdr.శీర్షిక_చక్రీయ_తనిఖీ = ext2fs_cpu_గమ్యం_le32(hdr_చక్రీయ_తనిఖీ);
	retval = io_channel_write_blk64(దత్తాంశం->undo_file, 0,
					-(పూర్ణాంకం)పరిమాణం(దత్తాంశం->hdr),
					&దత్తాంశం->hdr);
	ఒకవేళ (retval)
		వెళ్ళు err_బహిర్గతం;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", దత్తాంశం->super_blk_అంకె);
	retval = io_channel_write_blk64(దత్తాంశం->undo_file, దత్తాంశం->super_blk_అంకె,
					-SUPERBLOCK_కొలత, &super);
	ఒకవేళ (retval)
		వెళ్ళు err_బహిర్గతం;

	ఒకవేళ (flush)
		retval = io_channel_flush(దత్తాంశం->undo_file);
err_బహిర్గతం:
	io_channel_సమితి_blksize(channel, ఖండం_కొలత);
	తిరిగి retval;
}

స్థిర errcode_t undo_అమరిక_tdb(నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	పూర్ణాంకం i;
	errcode_t retval;

	ఒకవేళ (దత్తాంశం->tdb_written == 1)
		తిరిగి 0;

	దత్తాంశం->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&దత్తాంశం->fake_fs, 0, పరిమాణం(దత్తాంశం->fake_fs));
	దత్తాంశం->fake_fs.blocksize = దత్తాంశం->tdb_దత్తాంశం_కొలత;
	retval = ext2fs_alloc_generic_bmap(&దత్తాంశం->fake_fs,
				EXT2_ET_MAGIC_ఖండం_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &దత్తాంశం->written_ఖండం_map);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(దత్తాంశం->tdb_దత్తాంశం_కొలత, &దత్తాంశం->keyb);
	ఒకవేళ (retval)
		తిరిగి retval;
	దత్తాంశం->కీలకం_blk_అంకె = దత్తాంశం->మొదటి_కీలకం_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", దత్తాంశం->tdb_దత్తాంశం_కొలత);
	dbg_printf("Keys per block %llu\n", కీలకాలు_చొప్పున_ఖండం(దత్తాంశం));
	దత్తాంశం->hdr.ఖండం_కొలత = ext2fs_cpu_గమ్యం_le32(దత్తాంశం->tdb_దత్తాంశం_కొలత);
	io_channel_సమితి_blksize(దత్తాంశం->undo_file, దత్తాంశం->tdb_దత్తాంశం_కొలత);

	/* Ensure that we have space for header blocks */
	కొరకు (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(దత్తాంశం->undo_file, i, 1,
					       దత్తాంశం->keyb);
		ఒకవేళ (retval)
			memset(దత్తాంశం->keyb, 0, దత్తాంశం->tdb_దత్తాంశం_కొలత);
		retval = io_channel_write_blk64(దత్తాంశం->undo_file, i, 1,
						దత్తాంశం->keyb);
		ఒకవేళ (retval)
			తిరిగి retval;
		retval = io_channel_flush(దత్తాంశం->undo_file);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	memset(దత్తాంశం->keyb, 0, దత్తాంశం->tdb_దత్తాంశం_కొలత);
	తిరిగి 0;
}

స్థిర errcode_t undo_write_tdb(io_channel channel,
				గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క)

{
	పూర్ణాంకం కొలత, sz;
	గుర్తులేని దీర్ఘ దీర్ఘ ఖండం_అంకె, backing_blk_అంకె;
	errcode_t retval = 0;
	ext2_loff_t స్థానభ్రంశం;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	గుర్తులేని అక్షరం *read_ptr;
	గుర్తులేని దీర్ఘ దీర్ఘ చివర_ఖండం;
	గుర్తులేని దీర్ఘ దీర్ఘ దత్తాంశం_కొలత;
	నిర్మాణం undo_కీలకం *కీలకం;
	__u32 blk_చక్రీయ_తనిఖీ;

	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;

	ఒకవేళ (దత్తాంశం->undo_file == ఖాళీ) {
		/*
		 * Transaction database not initialized
		 */
		తిరిగి 0;
	}

	ఒకవేళ (లెక్క == 1)
		కొలత = channel->ఖండం_కొలత;
	లేకపోతే {
		ఒకవేళ (లెక్క < 0)
			కొలత = -లెక్క;
		లేకపోతే
			కొలత = లెక్క * channel->ఖండం_కొలత;
	}

	retval = undo_అమరిక_tdb(దత్తాంశం);
	ఒకవేళ (retval)
		తిరిగి retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	స్థానభ్రంశం = (ఖండం * channel->ఖండం_కొలత) + దత్తాంశం->స్థానభ్రంశం ;
	ఖండం_అంకె = స్థానభ్రంశం / దత్తాంశం->tdb_దత్తాంశం_కొలత;
	చివర_ఖండం = (స్థానభ్రంశం + కొలత - 1) / దత్తాంశం->tdb_దత్తాంశం_కొలత;

	అయితే (ఖండం_అంకె <= చివర_ఖండం) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		ఒకవేళ (ext2fs_test_ఖండం_bitmap2(దత్తాంశం->written_ఖండం_map,
						   ఖండం_అంకె)) {
			/* Try the next block */
			ఖండం_అంకె++;
			కొనసాగు;
		}
		ext2fs_mark_ఖండం_bitmap2(దత్తాంశం->written_ఖండం_map, ఖండం_అంకె);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		స్థానభ్రంశం = ఖండం_అంకె * దత్తాంశం->tdb_దత్తాంశం_కొలత +
				(దత్తాంశం->స్థానభ్రంశం % దత్తాంశం->tdb_దత్తాంశం_కొలత);
		backing_blk_అంకె = (స్థానభ్రంశం - దత్తాంశం->స్థానభ్రంశం) / channel->ఖండం_కొలత;

		retval = ext2fs_get_mem(దత్తాంశం->tdb_దత్తాంశం_కొలత, &read_ptr);
		ఒకవేళ (retval) {
			తిరిగి retval;
		}

		memset(read_ptr, 0, దత్తాంశం->tdb_దత్తాంశం_కొలత);
		actual_కొలత = 0;
		ఒకవేళ ((దత్తాంశం->tdb_దత్తాంశం_కొలత % channel->ఖండం_కొలత) == 0)
			sz = దత్తాంశం->tdb_దత్తాంశం_కొలత / channel->ఖండం_కొలత;
		లేకపోతే
			sz = -దత్తాంశం->tdb_దత్తాంశం_కొలత;
		retval = io_channel_read_blk64(దత్తాంశం->real, backing_blk_అంకె,
					     sz, read_ptr);
		ఒకవేళ (retval) {
			ఒకవేళ (retval != EXT2_ET_SHORT_READ) {
				విడుదల(read_ptr);
				తిరిగి retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			దత్తాంశం_కొలత = actual_కొలత;
		} లేకపోతే {
			దత్తాంశం_కొలత = దత్తాంశం->tdb_దత్తాంశం_కొలత;
		}
		ఒకవేళ (దత్తాంశం_కొలత == 0) {
			విడుదల(read_ptr);
			ఖండం_అంకె++;
			కొనసాగు;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       దత్తాంశం_కొలత, backing_blk_అంకె, ఖండం, దత్తాంశం->tdb_దత్తాంశం_కొలత);
		ఒకవేళ ((దత్తాంశం_కొలత % దత్తాంశం->undo_file->ఖండం_కొలత) == 0)
			sz = దత్తాంశం_కొలత / దత్తాంశం->undo_file->ఖండం_కొలత;
		లేకపోతే
			sz = -దత్తాంశం_కొలత;;
		/* extend this key? */
		ఒకవేళ (దత్తాంశం->కీలకాలు_లోపల_ఖండం) {
			కీలకం = దత్తాంశం->keyb->కీలకాలు + దత్తాంశం->కీలకాలు_లోపల_ఖండం - 1;
			keysz = ext2fs_le32_గమ్యం_cpu(కీలకం->కొలత);
		} లేకపోతే {
			కీలకం = ఖాళీ;
			keysz = 0;
		}
		ఒకవేళ (కీలకం != ఖాళీ &&
		    (ext2fs_le64_గమ్యం_cpu(కీలకం->fsblk) * channel->ఖండం_కొలత +
		     channel->ఖండం_కొలత - 1 +
		     keysz) / channel->ఖండం_కొలత == backing_blk_అంకె &&
		    E2_UNDO_MAX_EXTENT_ఖండాలు * దత్తాంశం->tdb_దత్తాంశం_కొలత >
		    keysz + దత్తాంశం_కొలత) {
			blk_చక్రీయ_తనిఖీ = ext2fs_le32_గమ్యం_cpu(కీలకం->blk_చక్రీయ_తనిఖీ);
			blk_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(blk_చక్రీయ_తనిఖీ, read_ptr, దత్తాంశం_కొలత);
			కీలకం->blk_చక్రీయ_తనిఖీ = ext2fs_cpu_గమ్యం_le32(blk_చక్రీయ_తనిఖీ);
			కీలకం->కొలత = ext2fs_cpu_గమ్యం_le32(keysz + దత్తాంశం_కొలత);
		} లేకపోతే {
			దత్తాంశం->అంకె_కీలకాలు++;
			కీలకం = దత్తాంశం->keyb->కీలకాలు + దత్తాంశం->కీలకాలు_లోపల_ఖండం;
			దత్తాంశం->కీలకాలు_లోపల_ఖండం++;
			కీలకం->fsblk = ext2fs_cpu_గమ్యం_le64(backing_blk_అంకె);
			blk_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, read_ptr, దత్తాంశం_కొలత);
			కీలకం->blk_చక్రీయ_తనిఖీ = ext2fs_cpu_గమ్యం_le32(blk_చక్రీయ_తనిఖీ);
			కీలకం->కొలత = ext2fs_cpu_గమ్యం_le32(దత్తాంశం_కొలత);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       ఖండం_అంకె,
		       దత్తాంశం->undo_blk_అంకె,
		       sz, దత్తాంశం->అంకె_కీలకాలు - 1);
		retval = io_channel_write_blk64(దత్తాంశం->undo_file,
					దత్తాంశం->undo_blk_అంకె, sz, read_ptr);
		ఒకవేళ (retval) {
			విడుదల(read_ptr);
			తిరిగి retval;
		}
		దత్తాంశం->undo_blk_అంకె++;
		విడుదల(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(దత్తాంశం, 0);
		ఒకవేళ (retval)
			తిరిగి retval;

		/* Next block */
		ఖండం_అంకె++;
	}

	తిరిగి retval;
}

స్థిర errcode_t undo_io_read_error(io_channel channel ATTR((వాడని)),
				    గుర్తులేని దీర్ఘ ఖండం ATTR((వాడని)),
				    పూర్ణాంకం లెక్క ATTR((వాడని)),
				    శూన్య *దత్తాంశం ATTR((వాడని)),
				    size_t కొలత ATTR((వాడని)),
				    పూర్ణాంకం actual,
				    errcode_t error ATTR((వాడని)))
{
	actual_కొలత = actual;
	తిరిగి error;
}

స్థిర శూన్య undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

స్థిర పూర్ణాంకం check_filesystem(నిర్మాణం undo_శీర్షిక *hdr, io_channel undo_file,
			    గుర్తులేని పూర్ణాంకం blocksize, blk64_t super_ఖండం,
			    io_channel channel)
{
	నిర్మాణం ext2_super_ఖండం super, *sb;
	అక్షరం *buf;
	__u32 sb_చక్రీయ_తనిఖీ;
	errcode_t retval;

	io_channel_సమితి_blksize(channel, SUPERBLOCK_స్థానభ్రంశం);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_కొలత, &super);
	ఒకవేళ (retval)
		తిరిగి retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = io_channel_read_blk64(undo_file, super_ఖండం,
				       -SUPERBLOCK_కొలత, buf);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;
	sb = (నిర్మాణం ext2_super_ఖండం *)buf;
	sb->s_magic = ~sb->s_magic;
	ఒకవేళ (memcmp(&super, buf, పరిమాణం(super))) {
		retval = -1;
		వెళ్ళు బహిర్గతం;
	}
	sb_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)buf, SUPERBLOCK_కొలత);
	ఒకవేళ (ext2fs_le32_గమ్యం_cpu(hdr->sb_చక్రీయ_తనిఖీ) != sb_చక్రీయ_తనిఖీ) {
		retval = -1;
		వెళ్ళు బహిర్గతం;
	}

బహిర్గతం:
	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
స్థిర errcode_t try_reopen_undo_file(పూర్ణాంకం undo_fd,
				      నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం undo_శీర్షిక hdr;
	నిర్మాణం undo_కీలకం *dkey;
	ext2fs_struct_stat statbuf;
	గుర్తులేని పూర్ణాంకం blocksize, fs_blocksize;
	blk64_t super_ఖండం, lblk;
	size_t అంకె_కీలకాలు, కీలకాలు_చొప్పున_ఖండం_2, i;
	__u32 hdr_చక్రీయ_తనిఖీ, కీలకం_చక్రీయ_తనిఖీ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	ఒకవేళ (retval)
		వెళ్ళు bad_file;
	ఒకవేళ (statbuf.st_కొలత == 0)
		వెళ్ళు బహిర్గతం;

	/* check the file header */
	retval = io_channel_read_blk64(దత్తాంశం->undo_file, 0, -(పూర్ణాంకం)పరిమాణం(hdr),
				       &hdr);
	ఒకవేళ (retval)
		వెళ్ళు bad_file;

	ఒకవేళ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    పరిమాణం(hdr.magic)))
		వెళ్ళు bad_file;
	hdr_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)&hdr,
				   పరిమాణం(నిర్మాణం undo_శీర్షిక) -
				   పరిమాణం(__u32));
	ఒకవేళ (ext2fs_le32_గమ్యం_cpu(hdr.శీర్షిక_చక్రీయ_తనిఖీ) != hdr_చక్రీయ_తనిఖీ)
		వెళ్ళు bad_file;
	blocksize = ext2fs_le32_గమ్యం_cpu(hdr.ఖండం_కొలత);
	fs_blocksize = ext2fs_le32_గమ్యం_cpu(hdr.fs_ఖండం_కొలత);
	ఒకవేళ (blocksize > E2_UNDO_MAX_ఖండం_కొలత ||
	    blocksize < E2_UNDO_MIN_ఖండం_కొలత ||
	    !blocksize || !fs_blocksize)
		వెళ్ళు bad_file;
	super_ఖండం = ext2fs_le64_గమ్యం_cpu(hdr.super_స్థానభ్రంశం);
	అంకె_కీలకాలు = ext2fs_le64_గమ్యం_cpu(hdr.అంకె_కీలకాలు);
	io_channel_సమితి_blksize(దత్తాంశం->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	ఒకవేళ (hdr.f_incompat || hdr.f_rocompat)
		వెళ్ళు bad_file;

	/* Superblock matches this FS? */
	ఒకవేళ (check_filesystem(&hdr, దత్తాంశం->undo_file, blocksize, super_ఖండం,
			     దత్తాంశం->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		వెళ్ళు బహిర్గతం;
	}

	/* Try to set ourselves up */
	దత్తాంశం->tdb_దత్తాంశం_కొలత = blocksize;
	retval = undo_అమరిక_tdb(దత్తాంశం);
	ఒకవేళ (retval)
		వెళ్ళు bad_file;
	దత్తాంశం->అంకె_కీలకాలు = అంకె_కీలకాలు;
	దత్తాంశం->super_blk_అంకె = super_ఖండం;
	దత్తాంశం->మొదటి_కీలకం_blk = ext2fs_le64_గమ్యం_cpu(hdr.కీలకం_స్థానభ్రంశం);

	/* load the written block map */
	కీలకాలు_చొప్పున_ఖండం_2 = కీలకాలు_చొప్పున_ఖండం(దత్తాంశం);
	lblk = దత్తాంశం->మొదటి_కీలకం_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   అంకె_కీలకాలు, కీలకాలు_చొప్పున_ఖండం_2, blocksize);
	కొరకు (i = 0; i < అంకె_కీలకాలు; i += కీలకాలు_చొప్పున_ఖండం_2) {
		size_t j, max_j;
		__le32 చక్రీయ_తనిఖీ;

		దత్తాంశం->కీలకం_blk_అంకె = lblk;
		retval = io_channel_read_blk64(దత్తాంశం->undo_file,
					       lblk, 1, దత్తాంశం->keyb);
		ఒకవేళ (retval)
			వెళ్ళు bad_కీలకం_replay;

		/* check keys */
		ఒకవేళ (ext2fs_le32_గమ్యం_cpu(దత్తాంశం->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			వెళ్ళు bad_కీలకం_replay;
		}
		చక్రీయ_తనిఖీ = దత్తాంశం->keyb->చక్రీయ_తనిఖీ;
		దత్తాంశం->keyb->చక్రీయ_తనిఖీ = 0;
		కీలకం_చక్రీయ_తనిఖీ = ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)దత్తాంశం->keyb,
					   blocksize);
		ఒకవేళ (ext2fs_le32_గమ్యం_cpu(చక్రీయ_తనిఖీ) != కీలకం_చక్రీయ_తనిఖీ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			వెళ్ళు bad_కీలకం_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = దత్తాంశం->అంకె_కీలకాలు - i;
		ఒకవేళ (max_j > కీలకాలు_చొప్పున_ఖండం_2)
			max_j = కీలకాలు_చొప్పున_ఖండం_2;
		కొరకు (j = 0, dkey = దత్తాంశం->keyb->కీలకాలు;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_గమ్యం_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t కొలత = ext2fs_le32_గమ్యం_cpu(dkey->కొలత);

			ext2fs_mark_ఖండం_bitmap_range2(దత్తాంశం->written_ఖండం_map,
					 undo_blk,
					(కొలత + blocksize - 1) / blocksize);
			lblk += (కొలత + blocksize - 1) / blocksize;
			దత్తాంశం->undo_blk_అంకె = lblk;
			దత్తాంశం->కీలకాలు_లోపల_ఖండం = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   దత్తాంశం->కీలకం_blk_అంకె, దత్తాంశం->undo_blk_అంకె, దత్తాంశం->అంకె_కీలకాలు,
		   దత్తాంశం->కీలకాలు_లోపల_ఖండం);

	దత్తాంశం->hdr.state = hdr.state & ~E2_UNDO_STATE_ముగిసిన;
	దత్తాంశం->hdr.f_compat = hdr.f_compat;
	దత్తాంశం->hdr.f_incompat = hdr.f_incompat;
	దత్తాంశం->hdr.f_rocompat = hdr.f_rocompat;
	తిరిగి retval;

bad_కీలకం_replay:
	దత్తాంశం->కీలకం_blk_అంకె = దత్తాంశం->undo_blk_అంకె = 0;
	దత్తాంశం->కీలకాలు_లోపల_ఖండం = 0;
	ext2fs_free_mem(&దత్తాంశం->keyb);
	ext2fs_free_generic_bitmap(దత్తాంశం->written_ఖండం_map);
	దత్తాంశం->tdb_written = 0;
	వెళ్ళు బహిర్గతం;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
బహిర్గతం:
	తిరిగి retval;
}

స్థిర శూన్య undo_atexit(శూన్య *p)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం = p;
	errcode_t err;

	err = write_undo_indexes(దత్తాంశం, 1);
	io_channel_close(దత్తాంశం->undo_file);

	com_err(దత్తాంశం->tdb_file, err, "while force-closing undo file");
}

స్థిర errcode_t undo_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	io_channel	io = ఖాళీ;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం = ఖాళీ;
	పూర్ణాంకం		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_దారాలు;
	ఒకవేళ (name == 0)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_io_channel), &io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(io, 0, పరిమాణం(నిర్మాణం struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం), &దత్తాంశం);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &io->name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(io->name, name);
	io->వ్యక్తిగత_దత్తాంశం = దత్తాంశం;
	io->ఖండం_కొలత = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(దత్తాంశం, 0, పరిమాణం(నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం));
	దత్తాంశం->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	దత్తాంశం->super_blk_అంకె = 1;
	దత్తాంశం->మొదటి_కీలకం_blk = 2;
	దత్తాంశం->undo_blk_అంకె = 3;

	ఒకవేళ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &దత్తాంశం->real);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;

		దత్తాంశం->tdb_file = strdup(tdb_file);
		ఒకవేళ (దత్తాంశం->tdb_file == ఖాళీ)
			వెళ్ళు cleanup;
		undo_fd = ext2fs_open_file(దత్తాంశం->tdb_file, O_RDWR | O_CREAT,
					   0600);
		ఒకవేళ (undo_fd < 0)
			వెళ్ళు cleanup;

		retval = undo_io_backing_manager->open(దత్తాంశం->tdb_file,
						       IO_FLAG_RW,
						       &దత్తాంశం->undo_file);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే {
		దత్తాంశం->real = ఖాళీ;
		దత్తాంశం->undo_file = ఖాళీ;
	}

	ఒకవేళ (దత్తాంశం->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (దత్తాంశం->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	ఒకవేళ (దత్తాంశం->real)
		undo_err_handler_init(దత్తాంశం->real);

	ఒకవేళ (దత్తాంశం->undo_file) {
		retval = try_reopen_undo_file(undo_fd, దత్తాంశం);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
	retval = ext2fs_కూడు_exit_fn(undo_atexit, దత్తాంశం);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	*channel = io;
	ఒకవేళ (undo_fd >= 0)
		close(undo_fd);
	తిరిగి retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, దత్తాంశం);
	ఒకవేళ (undo_fd >= 0)
		close(undo_fd);
	ఒకవేళ (దత్తాంశం && దత్తాంశం->undo_file)
		io_channel_close(దత్తాంశం->undo_file);
	ఒకవేళ (దత్తాంశం && దత్తాంశం->tdb_file)
		విడుదల(దత్తాంశం->tdb_file);
	ఒకవేళ (దత్తాంశం && దత్తాంశం->real)
		io_channel_close(దత్తాంశం->real);
	ఒకవేళ (దత్తాంశం)
		ext2fs_free_mem(&దత్తాంశం);
	ఒకవేళ (io && io->name)
		ext2fs_free_mem(&io->name);
	ఒకవేళ (io)
		ext2fs_free_mem(&io);
	తిరిగి retval;
}

స్థిర errcode_t undo_close(io_channel channel)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
		తిరిగి 0;
	/* Before closing write the file system identity */
	ఒకవేళ (!ext2fs_సురక్షిత_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		దత్తాంశం->hdr.state = ext2fs_cpu_గమ్యం_le32(E2_UNDO_STATE_ముగిసిన);
	err = write_undo_indexes(దత్తాంశం, 1);
	ext2fs_remove_exit_fn(undo_atexit, దత్తాంశం);
	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_close(దత్తాంశం->real);
	ఒకవేళ (దత్తాంశం->tdb_file)
		విడుదల(దత్తాంశం->tdb_file);
	ఒకవేళ (దత్తాంశం->undo_file)
		io_channel_close(దత్తాంశం->undo_file);
	ext2fs_free_mem(&దత్తాంశం->keyb);
	ఒకవేళ (దత్తాంశం->written_ఖండం_map)
		ext2fs_free_generic_bitmap(దత్తాంశం->written_ఖండం_map);
	ext2fs_free_mem(&channel->వ్యక్తిగత_దత్తాంశం);
	ఒకవేళ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	ఒకవేళ (err)
		తిరిగి err;
	తిరిగి retval;
}

స్థిర errcode_t undo_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (blksize > E2_UNDO_MAX_ఖండం_కొలత || blksize < E2_UNDO_MIN_ఖండం_కొలత)
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_సమితి_blksize(దత్తాంశం->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	ఒకవేళ (!దత్తాంశం->tdb_దత్తాంశం_కొలత || !దత్తాంశం->tdb_written)
		దత్తాంశం->tdb_దత్తాంశం_కొలత = blksize;
	channel->ఖండం_కొలత = blksize;
	తిరిగి retval;
}

స్థిర errcode_t undo_read_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	errcode_t	retval = 0;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_read_blk64(దత్తాంశం->real, ఖండం, లెక్క, buf);

	తిరిగి retval;
}

స్థిర errcode_t undo_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	తిరిగి undo_read_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t undo_write_blk64(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ఖండం, లెక్క);
	ఒకవేళ (retval)
		 తిరిగి retval;
	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_write_blk64(దత్తాంశం->real, ఖండం, లెక్క, buf);

	తిరిగి retval;
}

స్థిర errcode_t undo_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	తిరిగి undo_write_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t undo_write_byte(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				 పూర్ణాంకం కొలత, స్థిరరాశి శూన్య *buf)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	గుర్తులేని దీర్ఘ blk_అంకె, లెక్క;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = స్థానభ్రంశం + దత్తాంశం->స్థానభ్రంశం;
	blk_అంకె = location/channel->ఖండం_కొలత;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	లెక్క = (కొలత + (location % channel->ఖండం_కొలత) +
			channel->ఖండం_కొలత  -1)/channel->ఖండం_కొలత;
	retval = undo_write_tdb(channel, blk_అంకె, లెక్క);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (దత్తాంశం->real && దత్తాంశం->real->manager->write_byte)
		retval = io_channel_write_byte(దత్తాంశం->real, స్థానభ్రంశం, కొలత, buf);

	తిరిగి retval;
}

స్థిర errcode_t undo_discard(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;
	పూర్ణాంకం icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (లెక్క > గరిష్ఠ_పూర్ణాంకం)
		తిరిగి EXT2_ET_UNIMPLEMENTED;
	icount = లెక్క;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ఖండం, icount);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_discard(దత్తాంశం->real, ఖండం, లెక్క);

	తిరిగి retval;
}

స్థిర errcode_t undo_zeroout(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;
	పూర్ణాంకం icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (లెక్క > గరిష్ఠ_పూర్ణాంకం)
		తిరిగి EXT2_ET_UNIMPLEMENTED;
	icount = లెక్క;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ఖండం, icount);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_zeroout(దత్తాంశం->real, ఖండం, లెక్క);

	తిరిగి retval;
}

స్థిర errcode_t undo_cache_readahead(io_channel channel,
				      గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				      గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_cache_readahead(దత్తాంశం->real, ఖండం, లెక్క);

	తిరిగి retval;
}

/*
 * Flush data buffers to disk.
 */
స్థిర errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = io_channel_flush(దత్తాంశం->real);

	తిరిగి retval;
}

స్థిర errcode_t undo_సమితి_option(io_channel channel, స్థిరరాశి అక్షరం *option,
				 స్థిరరాశి అక్షరం *arg)
{
	errcode_t	retval = 0;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;
	గుర్తులేని దీర్ఘ tmp;
	అక్షరం *చివర;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (!strcmp(option, "tdb_data_size")) {
		ఒకవేళ (!arg)
			తిరిగి EXT2_ET_INVALID_వాదం;

		tmp = strtoul(arg, &చివర, 0);
		ఒకవేళ (*చివర)
			తిరిగి EXT2_ET_INVALID_వాదం;
		ఒకవేళ (tmp > E2_UNDO_MAX_ఖండం_కొలత || tmp < E2_UNDO_MIN_ఖండం_కొలత)
			తిరిగి EXT2_ET_INVALID_వాదం;
		ఒకవేళ (!దత్తాంశం->tdb_దత్తాంశం_కొలత || !దత్తాంశం->tdb_written) {
			దత్తాంశం->tdb_written = -1;
			దత్తాంశం->tdb_దత్తాంశం_కొలత = tmp;
		}
		తిరిగి 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	ఒకవేళ (దత్తాంశం->real && దత్తాంశం->real->manager->సమితి_option) {
		retval = దత్తాంశం->real->manager->సమితి_option(దత్తాంశం->real,
							option, arg);
	}
	ఒకవేళ (!retval && !strcmp(option, "offset")) {
		ఒకవేళ (!arg)
			తిరిగి EXT2_ET_INVALID_వాదం;

		tmp = strtoul(arg, &చివర, 0);
		ఒకవేళ (*చివర)
			తిరిగి EXT2_ET_INVALID_వాదం;
		దత్తాంశం->స్థానభ్రంశం = tmp;
	}
	తిరిగి retval;
}

స్థిర errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *దత్తాంశం;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	దత్తాంశం = (నిర్మాణం undo_వ్యక్తిగత_దత్తాంశం *) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(దత్తాంశం, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ఒకవేళ (దత్తాంశం->real)
		retval = (దత్తాంశం->real->manager->get_stats)(దత్తాంశం->real, stats);

	తిరిగి retval;
}

స్థిర నిర్మాణం struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.సమితి_blksize	= undo_సమితి_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.సమితి_option	= undo_సమితి_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
