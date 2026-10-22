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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_RESOURCE_H
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR(x) __attribute__(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR(x)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEBUG

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
# define dbg_printf(f, a...)  ಮಾಡು {ಬರೆ(f, ## a); fflush(stdout); } ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define dbg_printf(f, a...)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)
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
#ವ್ಯಾಖ್ಯಾನಿಸು E2UNDO_MAGIC "E2UNDO02"
#ವ್ಯಾಖ್ಯಾನಿಸು KEYBLOCK_MAGIC 0xCADECADE

#ವ್ಯಾಖ್ಯಾನಿಸು E2_UNDO_STATE_ಪೂರ್ಣಗೊಂಡ	0x1	/* undo file is complete */

#ವ್ಯಾಖ್ಯಾನಿಸು E2_UNDO_MIN_ಖಂಡ_ಗಾತ್ರ	1024	/* undo blocks are no less than 1KB */
#ವ್ಯಾಖ್ಯಾನಿಸು E2_UNDO_MAX_ಖಂಡ_ಗಾತ್ರ	1048576	/* undo blocks are no more than 1MB */

ರಚನೆ undo_ಶೀರ್ಷಿಕೆ {
	ಅಕ್ಷರ magic[8];		/* "E2UNDO02" */
	__le64 ಅಂಕೆ_ಕೀಲಿಗಳು;	/* how many keys? */
	__le64 super_ಸ್ಥಾನಾಂತರ;	/* where in the file is the superblock copy? */
	__le64 ಕೀಲಿ_ಸ್ಥಾನಾಂತರ;	/* where do the key/data block chunks start? */
	__le32 ಖಂಡ_ಗಾತ್ರ;	/* block size of the undo file */
	__le32 fs_ಖಂಡ_ಗಾತ್ರ;	/* block size of the target device */
	__le32 sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_ಸ್ಥಾನಾಂತರ;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 ಶೀರ್ಷಿಕೆ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;	/* crc32c of this header (but not this field) */
};

#ವ್ಯಾಖ್ಯಾನಿಸು E2_UNDO_MAX_EXTENT_ಖಂಡಗಳು	512	/* max extent size, in blocks */

ರಚನೆ undo_ಕೀಲಿ {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;		/* crc32c of the block */
	__le32 ಗಾತ್ರ_2;		/* how many bytes in this block? */
};

ರಚನೆ undo_ಕೀಲಿ_ಖಂಡ {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;		/* block checksum */
	__le64 reserved;	/* zero */

#ಆದರೆ __STDC_VERSION__ >= 199901L
	ರಚನೆ undo_ಕೀಲಿ ಕೀಲಿಗಳು[];		/* keys, which come immediately after */
#ಇಲ್ಲದಿದ್ದರೆ
	ರಚನೆ undo_ಕೀಲಿ ಕೀಲಿಗಳು[0];	/* keys, which come immediately after */
#ಆದರೆ_ಅಂತ್ಯ
};

ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_ಅಂಕೆ;			/* next free block */
	blk64_t ಕೀಲಿ_blk_ಅಂಕೆ;			/* current key block location */
	blk64_t super_blk_ಅಂಕೆ;			/* superblock location */
	blk64_t ಮೊದಲ_ಕೀಲಿ_blk;			/* first key block location */
	ರಚನೆ undo_ಕೀಲಿ_ಖಂಡ *keyb;
	size_t ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ;

	/* The backing io channel */
	io_channel real;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t ಸ್ಥಾನಾಂತರ;

	ext2fs_ಖಂಡ_bitmap written_ಖಂಡ_map;
	ರಚನೆ struct_ext2_filsys fake_fs;
	ಅಕ್ಷರ *tdb_file;
	ರಚನೆ undo_ಶೀರ್ಷಿಕೆ hdr;
};
#ವ್ಯಾಖ್ಯಾನಿಸು ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ(d) (((d)->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ / ಗಾತ್ರ(ರಚನೆ undo_ಕೀಲಿ)) - 1)

#ವ್ಯಾಖ್ಯಾನಿಸು E2_UNDO_FEATURE_COMPAT_FS_ಸ್ಥಾನಾಂತರ 0x1	/* the filesystem offset */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ e2undo_ಗಣ_feature_fs_ಸ್ಥಾನಾಂತರ(ರಚನೆ undo_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ) {
	ಶೀರ್ಷಿಕೆ->f_compat |= ext2fs_le32_ಗಮ್ಯ_cpu(E2_UNDO_FEATURE_COMPAT_FS_ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ e2undo_clear_feature_fs_ಸ್ಥಾನಾಂತರ(ರಚನೆ undo_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ) {
	ಶೀರ್ಷಿಕೆ->f_compat &= ~ext2fs_le32_ಗಮ್ಯ_cpu(E2_UNDO_FEATURE_COMPAT_FS_ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ io_manager undo_io_backing_manager;
ಸ್ಥಿರ ಅಕ್ಷರ *tdb_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ actual_ಗಾತ್ರ;

errcode_t ಗಣ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ಗಣ_undo_io_backup_file(ಅಕ್ಷರ *file_name)
{
	tdb_file = strdup(file_name);

	ಆದರೆ (tdb_file == ಖಾಲಿ) {
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t write_undo_indexes(ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ flush)
{
	errcode_t retval;
	ರಚನೆ ext2_super_ಖಂಡ super;
	io_channel channel;
	ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ;
	__u32 sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	/* Spit out a key block, if there's any data */
	ಆದರೆ (ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ) {
		ದತ್ತಾಂಶ->keyb->magic = ext2fs_cpu_ಗಮ್ಯ_le32(KEYBLOCK_MAGIC);
		ದತ್ತಾಂಶ->keyb->ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
		ದತ್ತಾಂಶ->keyb->ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_cpu_ಗಮ್ಯ_le32(
					 ext2fs_crc32c_le(~0,
					 (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ದತ್ತಾಂಶ->keyb,
					 ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ));
		dbg_printf("Writing keyblock to blk %llu\n", ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ);
		retval = io_channel_write_blk64(ದತ್ತಾಂಶ->undo_file,
						ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ,
						1, ದತ್ತಾಂಶ->keyb);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		/* Move on to the next key block if it's full. */
		ಆದರೆ (ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ == ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ(ದತ್ತಾಂಶ)) {
			memset(ದತ್ತಾಂಶ->keyb, 0, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
			ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ = 0;
			ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ = ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ;
			ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ++;
		}
	}

	/* Prepare superblock for write */
	channel = ದತ್ತಾಂಶ->real;
	ಖಂಡ_ಗಾತ್ರ = channel->ಖಂಡ_ಗಾತ್ರ;

	io_channel_ಗಣ_blksize(channel, SUPERBLOCK_ಸ್ಥಾನಾಂತರ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ಗಾತ್ರ, &super);
	ಆದರೆ (retval)
		ಹೋಗು err_ಹೊರಹೋಗು;
	sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&super, SUPERBLOCK_ಗಾತ್ರ);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ->hdr.magic, E2UNDO_MAGIC, ಗಾತ್ರ(ದತ್ತಾಂಶ->hdr.magic));
	ದತ್ತಾಂಶ->hdr.ಅಂಕೆ_ಕೀಲಿಗಳು = ext2fs_cpu_ಗಮ್ಯ_le64(ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು);
	ದತ್ತಾಂಶ->hdr.super_ಸ್ಥಾನಾಂತರ = ext2fs_cpu_ಗಮ್ಯ_le64(ದತ್ತಾಂಶ->super_blk_ಅಂಕೆ);
	ದತ್ತಾಂಶ->hdr.ಕೀಲಿ_ಸ್ಥಾನಾಂತರ = ext2fs_cpu_ಗಮ್ಯ_le64(ದತ್ತಾಂಶ->ಮೊದಲ_ಕೀಲಿ_blk);
	ದತ್ತಾಂಶ->hdr.fs_ಖಂಡ_ಗಾತ್ರ = ext2fs_cpu_ಗಮ್ಯ_le32(ಖಂಡ_ಗಾತ್ರ);
	ದತ್ತಾಂಶ->hdr.sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_cpu_ಗಮ್ಯ_le32(sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ದತ್ತಾಂಶ->hdr.fs_ಸ್ಥಾನಾಂತರ = ext2fs_cpu_ಗಮ್ಯ_le64(ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ);
	ಆದರೆ (ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ)
		e2undo_ಗಣ_feature_fs_ಸ್ಥಾನಾಂತರ(&ದತ್ತಾಂಶ->hdr);
	ಇಲ್ಲದಿದ್ದರೆ
		e2undo_clear_feature_fs_ಸ್ಥಾನಾಂತರ(&ದತ್ತಾಂಶ->hdr);
	hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&ದತ್ತಾಂಶ->hdr,
				   ಗಾತ್ರ(ದತ್ತಾಂಶ->hdr) -
				   ಗಾತ್ರ(ದತ್ತಾಂಶ->hdr.ಶೀರ್ಷಿಕೆ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ));
	ದತ್ತಾಂಶ->hdr.ಶೀರ್ಷಿಕೆ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_cpu_ಗಮ್ಯ_le32(hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	retval = io_channel_write_blk64(ದತ್ತಾಂಶ->undo_file, 0,
					-(ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ದತ್ತಾಂಶ->hdr),
					&ದತ್ತಾಂಶ->hdr);
	ಆದರೆ (retval)
		ಹೋಗು err_ಹೊರಹೋಗು;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", ದತ್ತಾಂಶ->super_blk_ಅಂಕೆ);
	retval = io_channel_write_blk64(ದತ್ತಾಂಶ->undo_file, ದತ್ತಾಂಶ->super_blk_ಅಂಕೆ,
					-SUPERBLOCK_ಗಾತ್ರ, &super);
	ಆದರೆ (retval)
		ಹೋಗು err_ಹೊರಹೋಗು;

	ಆದರೆ (flush)
		retval = io_channel_flush(ದತ್ತಾಂಶ->undo_file);
err_ಹೊರಹೋಗು:
	io_channel_ಗಣ_blksize(channel, ಖಂಡ_ಗಾತ್ರ);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_ಸಿದ್ಧತೆ_tdb(ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;
	errcode_t retval;

	ಆದರೆ (ದತ್ತಾಂಶ->tdb_written == 1)
		ಹಿಂತಿರುಗಿ 0;

	ದತ್ತಾಂಶ->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&ದತ್ತಾಂಶ->fake_fs, 0, ಗಾತ್ರ(ದತ್ತಾಂಶ->fake_fs));
	ದತ್ತಾಂಶ->fake_fs.blocksize = ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	retval = ext2fs_alloc_generic_bmap(&ದತ್ತಾಂಶ->fake_fs,
				EXT2_ET_MAGIC_ಖಂಡ_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &ದತ್ತಾಂಶ->written_ಖಂಡ_map);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ, &ದತ್ತಾಂಶ->keyb);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ = ದತ್ತಾಂಶ->ಮೊದಲ_ಕೀಲಿ_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	dbg_printf("Keys per block %llu\n", ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ(ದತ್ತಾಂಶ));
	ದತ್ತಾಂಶ->hdr.ಖಂಡ_ಗಾತ್ರ = ext2fs_cpu_ಗಮ್ಯ_le32(ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	io_channel_ಗಣ_blksize(ದತ್ತಾಂಶ->undo_file, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);

	/* Ensure that we have space for header blocks */
	ಗಾಗಿ (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(ದತ್ತಾಂಶ->undo_file, i, 1,
					       ದತ್ತಾಂಶ->keyb);
		ಆದರೆ (retval)
			memset(ದತ್ತಾಂಶ->keyb, 0, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
		retval = io_channel_write_blk64(ದತ್ತಾಂಶ->undo_file, i, 1,
						ದತ್ತಾಂಶ->keyb);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		retval = io_channel_flush(ದತ್ತಾಂಶ->undo_file);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	memset(ದತ್ತಾಂಶ->keyb, 0, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t undo_write_tdb(io_channel channel,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2)

{
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, sz;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ_ಅಂಕೆ, backing_blk_ಅಂಕೆ;
	errcode_t retval = 0;
	ext2_loff_t ಸ್ಥಾನಾಂತರ;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *read_ptr;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಕೊನೆ_ಖಂಡ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ದತ್ತಾಂಶ_ಗಾತ್ರ;
	ರಚನೆ undo_ಕೀಲಿ *ಕೀಲಿ;
	__u32 blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಆದರೆ (ದತ್ತಾಂಶ->undo_file == ಖಾಲಿ) {
		/*
		 * Transaction database not initialized
		 */
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ಎಣಿಕೆ_2 == 1)
		ಗಾತ್ರ_2 = channel->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಎಣಿಕೆ_2 < 0)
			ಗಾತ್ರ_2 = -ಎಣಿಕೆ_2;
		ಇಲ್ಲದಿದ್ದರೆ
			ಗಾತ್ರ_2 = ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ;
	}

	retval = undo_ಸಿದ್ಧತೆ_tdb(ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	ಸ್ಥಾನಾಂತರ = (ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ) + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ ;
	ಖಂಡ_ಅಂಕೆ = ಸ್ಥಾನಾಂತರ / ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	ಕೊನೆ_ಖಂಡ = (ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ_2 - 1) / ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;

	ಆಗ (ಖಂಡ_ಅಂಕೆ <= ಕೊನೆ_ಖಂಡ) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(ದತ್ತಾಂಶ->written_ಖಂಡ_map,
						   ಖಂಡ_ಅಂಕೆ)) {
			/* Try the next block */
			ಖಂಡ_ಅಂಕೆ++;
			ಮುಂದುವರಿ;
		}
		ext2fs_mark_ಖಂಡ_bitmap2(ದತ್ತಾಂಶ->written_ಖಂಡ_map, ಖಂಡ_ಅಂಕೆ);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		ಸ್ಥಾನಾಂತರ = ಖಂಡ_ಅಂಕೆ * ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ +
				(ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ % ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
		backing_blk_ಅಂಕೆ = (ಸ್ಥಾನಾಂತರ - ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ) / channel->ಖಂಡ_ಗಾತ್ರ;

		retval = ext2fs_get_mem(ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ, &read_ptr);
		ಆದರೆ (retval) {
			ಹಿಂತಿರುಗಿ retval;
		}

		memset(read_ptr, 0, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
		actual_ಗಾತ್ರ = 0;
		ಆದರೆ ((ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ % channel->ಖಂಡ_ಗಾತ್ರ) == 0)
			sz = ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ / channel->ಖಂಡ_ಗಾತ್ರ;
		ಇಲ್ಲದಿದ್ದರೆ
			sz = -ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;
		retval = io_channel_read_blk64(ದತ್ತಾಂಶ->real, backing_blk_ಅಂಕೆ,
					     sz, read_ptr);
		ಆದರೆ (retval) {
			ಆದರೆ (retval != EXT2_ET_SHORT_READ) {
				ಬಿಡುಗಡೆ(read_ptr);
				ಹಿಂತಿರುಗಿ retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			ದತ್ತಾಂಶ_ಗಾತ್ರ = actual_ಗಾತ್ರ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ದತ್ತಾಂಶ_ಗಾತ್ರ = ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ;
		}
		ಆದರೆ (ದತ್ತಾಂಶ_ಗಾತ್ರ == 0) {
			ಬಿಡುಗಡೆ(read_ptr);
			ಖಂಡ_ಅಂಕೆ++;
			ಮುಂದುವರಿ;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       ದತ್ತಾಂಶ_ಗಾತ್ರ, backing_blk_ಅಂಕೆ, ಖಂಡ, ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ);
		ಆದರೆ ((ದತ್ತಾಂಶ_ಗಾತ್ರ % ದತ್ತಾಂಶ->undo_file->ಖಂಡ_ಗಾತ್ರ) == 0)
			sz = ದತ್ತಾಂಶ_ಗಾತ್ರ / ದತ್ತಾಂಶ->undo_file->ಖಂಡ_ಗಾತ್ರ;
		ಇಲ್ಲದಿದ್ದರೆ
			sz = -ದತ್ತಾಂಶ_ಗಾತ್ರ;;
		/* extend this key? */
		ಆದರೆ (ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ) {
			ಕೀಲಿ = ದತ್ತಾಂಶ->keyb->ಕೀಲಿಗಳು + ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ - 1;
			keysz = ext2fs_le32_ಗಮ್ಯ_cpu(ಕೀಲಿ->ಗಾತ್ರ_2);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಕೀಲಿ = ಖಾಲಿ;
			keysz = 0;
		}
		ಆದರೆ (ಕೀಲಿ != ಖಾಲಿ &&
		    (ext2fs_le64_ಗಮ್ಯ_cpu(ಕೀಲಿ->fsblk) * channel->ಖಂಡ_ಗಾತ್ರ +
		     channel->ಖಂಡ_ಗಾತ್ರ - 1 +
		     keysz) / channel->ಖಂಡ_ಗಾತ್ರ == backing_blk_ಅಂಕೆ &&
		    E2_UNDO_MAX_EXTENT_ಖಂಡಗಳು * ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ >
		    keysz + ದತ್ತಾಂಶ_ಗಾತ್ರ) {
			blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_le32_ಗಮ್ಯ_cpu(ಕೀಲಿ->blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
			blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, read_ptr, ದತ್ತಾಂಶ_ಗಾತ್ರ);
			ಕೀಲಿ->blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_cpu_ಗಮ್ಯ_le32(blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
			ಕೀಲಿ->ಗಾತ್ರ_2 = ext2fs_cpu_ಗಮ್ಯ_le32(keysz + ದತ್ತಾಂಶ_ಗಾತ್ರ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು++;
			ಕೀಲಿ = ದತ್ತಾಂಶ->keyb->ಕೀಲಿಗಳು + ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ;
			ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ++;
			ಕೀಲಿ->fsblk = ext2fs_cpu_ಗಮ್ಯ_le64(backing_blk_ಅಂಕೆ);
			blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, read_ptr, ದತ್ತಾಂಶ_ಗಾತ್ರ);
			ಕೀಲಿ->blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_cpu_ಗಮ್ಯ_le32(blk_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
			ಕೀಲಿ->ಗಾತ್ರ_2 = ext2fs_cpu_ಗಮ್ಯ_le32(ದತ್ತಾಂಶ_ಗಾತ್ರ);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       ಖಂಡ_ಅಂಕೆ,
		       ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ,
		       sz, ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು - 1);
		retval = io_channel_write_blk64(ದತ್ತಾಂಶ->undo_file,
					ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ, sz, read_ptr);
		ಆದರೆ (retval) {
			ಬಿಡುಗಡೆ(read_ptr);
			ಹಿಂತಿರುಗಿ retval;
		}
		ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ++;
		ಬಿಡುಗಡೆ(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(ದತ್ತಾಂಶ, 0);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		/* Next block */
		ಖಂಡ_ಅಂಕೆ++;
	}

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_io_read_error(io_channel channel ATTR((ಬಳಸದ)),
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ ATTR((ಬಳಸದ)),
				    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 ATTR((ಬಳಸದ)),
				    ಶೂನ್ಯ *ದತ್ತಾಂಶ ATTR((ಬಳಸದ)),
				    size_t ಗಾತ್ರ_2 ATTR((ಬಳಸದ)),
				    ಪೂರ್ಣಾಂಕ actual,
				    errcode_t error ATTR((ಬಳಸದ)))
{
	actual_ಗಾತ್ರ = actual;
	ಹಿಂತಿರುಗಿ error;
}

ಸ್ಥಿರ ಶೂನ್ಯ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_filesystem(ರಚನೆ undo_ಶೀರ್ಷಿಕೆ *hdr, io_channel undo_file,
			    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ blocksize, blk64_t super_ಖಂಡ,
			    io_channel channel)
{
	ರಚನೆ ext2_super_ಖಂಡ super, *sb;
	ಅಕ್ಷರ *buf;
	__u32 sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	errcode_t retval;

	io_channel_ಗಣ_blksize(channel, SUPERBLOCK_ಸ್ಥಾನಾಂತರ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ಗಾತ್ರ, &super);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = io_channel_read_blk64(undo_file, super_ಖಂಡ,
				       -SUPERBLOCK_ಗಾತ್ರ, buf);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;
	sb = (ರಚನೆ ext2_super_ಖಂಡ *)buf;
	sb->s_magic = ~sb->s_magic;
	ಆದರೆ (memcmp(&super, buf, ಗಾತ್ರ(super))) {
		retval = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)buf, SUPERBLOCK_ಗಾತ್ರ);
	ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(hdr->sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) != sb_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) {
		retval = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

ಹೊರಹೋಗು:
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ಸ್ಥಿರ errcode_t try_reopen_undo_file(ಪೂರ್ಣಾಂಕ undo_fd,
				      ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ undo_ಶೀರ್ಷಿಕೆ hdr;
	ರಚನೆ undo_ಕೀಲಿ *dkey;
	ext2fs_struct_stat statbuf;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ blocksize, fs_blocksize;
	blk64_t super_ಖಂಡ, lblk;
	size_t ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2, i;
	__u32 hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ಕೀಲಿ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	ಆದರೆ (retval)
		ಹೋಗು bad_file;
	ಆದರೆ (statbuf.st_ಗಾತ್ರ == 0)
		ಹೋಗು ಹೊರಹೋಗು;

	/* check the file header */
	retval = io_channel_read_blk64(ದತ್ತಾಂಶ->undo_file, 0, -(ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(hdr),
				       &hdr);
	ಆದರೆ (retval)
		ಹೋಗು bad_file;

	ಆದರೆ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    ಗಾತ್ರ(hdr.magic)))
		ಹೋಗು bad_file;
	hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&hdr,
				   ಗಾತ್ರ(ರಚನೆ undo_ಶೀರ್ಷಿಕೆ) -
				   ಗಾತ್ರ(__u32));
	ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(hdr.ಶೀರ್ಷಿಕೆ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) != hdr_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ)
		ಹೋಗು bad_file;
	blocksize = ext2fs_le32_ಗಮ್ಯ_cpu(hdr.ಖಂಡ_ಗಾತ್ರ);
	fs_blocksize = ext2fs_le32_ಗಮ್ಯ_cpu(hdr.fs_ಖಂಡ_ಗಾತ್ರ);
	ಆದರೆ (blocksize > E2_UNDO_MAX_ಖಂಡ_ಗಾತ್ರ ||
	    blocksize < E2_UNDO_MIN_ಖಂಡ_ಗಾತ್ರ ||
	    !blocksize || !fs_blocksize)
		ಹೋಗು bad_file;
	super_ಖಂಡ = ext2fs_le64_ಗಮ್ಯ_cpu(hdr.super_ಸ್ಥಾನಾಂತರ);
	ಅಂಕೆ_ಕೀಲಿಗಳು = ext2fs_le64_ಗಮ್ಯ_cpu(hdr.ಅಂಕೆ_ಕೀಲಿಗಳು);
	io_channel_ಗಣ_blksize(ದತ್ತಾಂಶ->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	ಆದರೆ (hdr.f_incompat || hdr.f_rocompat)
		ಹೋಗು bad_file;

	/* Superblock matches this FS? */
	ಆದರೆ (check_filesystem(&hdr, ದತ್ತಾಂಶ->undo_file, blocksize, super_ಖಂಡ,
			     ದತ್ತಾಂಶ->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Try to set ourselves up */
	ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ = blocksize;
	retval = undo_ಸಿದ್ಧತೆ_tdb(ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹೋಗು bad_file;
	ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು = ಅಂಕೆ_ಕೀಲಿಗಳು;
	ದತ್ತಾಂಶ->super_blk_ಅಂಕೆ = super_ಖಂಡ;
	ದತ್ತಾಂಶ->ಮೊದಲ_ಕೀಲಿ_blk = ext2fs_le64_ಗಮ್ಯ_cpu(hdr.ಕೀಲಿ_ಸ್ಥಾನಾಂತರ);

	/* load the written block map */
	ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2 = ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ(ದತ್ತಾಂಶ);
	lblk = ದತ್ತಾಂಶ->ಮೊದಲ_ಕೀಲಿ_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   ಅಂಕೆ_ಕೀಲಿಗಳು, ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2, blocksize);
	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_ಕೀಲಿಗಳು; i += ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2) {
		size_t j, max_j;
		__le32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

		ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ = lblk;
		retval = io_channel_read_blk64(ದತ್ತಾಂಶ->undo_file,
					       lblk, 1, ದತ್ತಾಂಶ->keyb);
		ಆದರೆ (retval)
			ಹೋಗು bad_ಕೀಲಿ_replay;

		/* check keys */
		ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(ದತ್ತಾಂಶ->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ಹೋಗು bad_ಕೀಲಿ_replay;
		}
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ದತ್ತಾಂಶ->keyb->ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
		ದತ್ತಾಂಶ->keyb->ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
		ಕೀಲಿ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ದತ್ತಾಂಶ->keyb,
					   blocksize);
		ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) != ಕೀಲಿ_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ಹೋಗು bad_ಕೀಲಿ_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು - i;
		ಆದರೆ (max_j > ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2)
			max_j = ಕೀಲಿಗಳು_ಪ್ರತಿ_ಖಂಡ_2;
		ಗಾಗಿ (j = 0, dkey = ದತ್ತಾಂಶ->keyb->ಕೀಲಿಗಳು;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_ಗಮ್ಯ_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t ಗಾತ್ರ_2 = ext2fs_le32_ಗಮ್ಯ_cpu(dkey->ಗಾತ್ರ_2);

			ext2fs_mark_ಖಂಡ_bitmap_range2(ದತ್ತಾಂಶ->written_ಖಂಡ_map,
					 undo_blk,
					(ಗಾತ್ರ_2 + blocksize - 1) / blocksize);
			lblk += (ಗಾತ್ರ_2 + blocksize - 1) / blocksize;
			ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ = lblk;
			ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ, ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ, ದತ್ತಾಂಶ->ಅಂಕೆ_ಕೀಲಿಗಳು,
		   ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ);

	ದತ್ತಾಂಶ->hdr.state = hdr.state & ~E2_UNDO_STATE_ಪೂರ್ಣಗೊಂಡ;
	ದತ್ತಾಂಶ->hdr.f_compat = hdr.f_compat;
	ದತ್ತಾಂಶ->hdr.f_incompat = hdr.f_incompat;
	ದತ್ತಾಂಶ->hdr.f_rocompat = hdr.f_rocompat;
	ಹಿಂತಿರುಗಿ retval;

bad_ಕೀಲಿ_replay:
	ದತ್ತಾಂಶ->ಕೀಲಿ_blk_ಅಂಕೆ = ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ = 0;
	ದತ್ತಾಂಶ->ಕೀಲಿಗಳು_ಒಳ_ಖಂಡ = 0;
	ext2fs_free_mem(&ದತ್ತಾಂಶ->keyb);
	ext2fs_free_generic_bitmap(ದತ್ತಾಂಶ->written_ಖಂಡ_map);
	ದತ್ತಾಂಶ->tdb_written = 0;
	ಹೋಗು ಹೊರಹೋಗು;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ undo_atexit(ಶೂನ್ಯ *p)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = p;
	errcode_t err;

	err = write_undo_indexes(ದತ್ತಾಂಶ, 1);
	io_channel_close(ದತ್ತಾಂಶ->undo_file);

	com_err(ದತ್ತಾಂಶ->tdb_file, err, "while force-closing undo file");
}

ಸ್ಥಿರ errcode_t undo_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	io_channel	io = ಖಾಲಿ;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_ಎಳೆಗಳು;
	ಆದರೆ (name == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_io_channel), &io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(io, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ), &ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &io->name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(io->name, name);
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	io->ಖಂಡ_ಗಾತ್ರ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ));
	ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ದತ್ತಾಂಶ->super_blk_ಅಂಕೆ = 1;
	ದತ್ತಾಂಶ->ಮೊದಲ_ಕೀಲಿ_blk = 2;
	ದತ್ತಾಂಶ->undo_blk_ಅಂಕೆ = 3;

	ಆದರೆ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &ದತ್ತಾಂಶ->real);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;

		ದತ್ತಾಂಶ->tdb_file = strdup(tdb_file);
		ಆದರೆ (ದತ್ತಾಂಶ->tdb_file == ಖಾಲಿ)
			ಹೋಗು cleanup;
		undo_fd = ext2fs_open_file(ದತ್ತಾಂಶ->tdb_file, O_RDWR | O_CREAT,
					   0600);
		ಆದರೆ (undo_fd < 0)
			ಹೋಗು cleanup;

		retval = undo_io_backing_manager->open(ದತ್ತಾಂಶ->tdb_file,
						       IO_FLAG_RW,
						       &ದತ್ತಾಂಶ->undo_file);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ದತ್ತಾಂಶ->real = ಖಾಲಿ;
		ದತ್ತಾಂಶ->undo_file = ಖಾಲಿ;
	}

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (ದತ್ತಾಂಶ->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	ಆದರೆ (ದತ್ತಾಂಶ->real)
		undo_err_handler_init(ದತ್ತಾಂಶ->real);

	ಆದರೆ (ದತ್ತಾಂಶ->undo_file) {
		retval = try_reopen_undo_file(undo_fd, ದತ್ತಾಂಶ);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
	retval = ext2fs_ಕೂಡಿಸು_exit_fn(undo_atexit, ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	*channel = io;
	ಆದರೆ (undo_fd >= 0)
		close(undo_fd);
	ಹಿಂತಿರುಗಿ retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, ದತ್ತಾಂಶ);
	ಆದರೆ (undo_fd >= 0)
		close(undo_fd);
	ಆದರೆ (ದತ್ತಾಂಶ && ದತ್ತಾಂಶ->undo_file)
		io_channel_close(ದತ್ತಾಂಶ->undo_file);
	ಆದರೆ (ದತ್ತಾಂಶ && ದತ್ತಾಂಶ->tdb_file)
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ->tdb_file);
	ಆದರೆ (ದತ್ತಾಂಶ && ದತ್ತಾಂಶ->real)
		io_channel_close(ದತ್ತಾಂಶ->real);
	ಆದರೆ (ದತ್ತಾಂಶ)
		ext2fs_free_mem(&ದತ್ತಾಂಶ);
	ಆದರೆ (io && io->name)
		ext2fs_free_mem(&io->name);
	ಆದರೆ (io)
		ext2fs_free_mem(&io);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_close(io_channel channel)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
		ಹಿಂತಿರುಗಿ 0;
	/* Before closing write the file system identity */
	ಆದರೆ (!ext2fs_ಸುರಕ್ಷಿತ_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		ದತ್ತಾಂಶ->hdr.state = ext2fs_cpu_ಗಮ್ಯ_le32(E2_UNDO_STATE_ಪೂರ್ಣಗೊಂಡ);
	err = write_undo_indexes(ದತ್ತಾಂಶ, 1);
	ext2fs_remove_exit_fn(undo_atexit, ದತ್ತಾಂಶ);
	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_close(ದತ್ತಾಂಶ->real);
	ಆದರೆ (ದತ್ತಾಂಶ->tdb_file)
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ->tdb_file);
	ಆದರೆ (ದತ್ತಾಂಶ->undo_file)
		io_channel_close(ದತ್ತಾಂಶ->undo_file);
	ext2fs_free_mem(&ದತ್ತಾಂಶ->keyb);
	ಆದರೆ (ದತ್ತಾಂಶ->written_ಖಂಡ_map)
		ext2fs_free_generic_bitmap(ದತ್ತಾಂಶ->written_ಖಂಡ_map);
	ext2fs_free_mem(&channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಆದರೆ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (blksize > E2_UNDO_MAX_ಖಂಡ_ಗಾತ್ರ || blksize < E2_UNDO_MIN_ಖಂಡ_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_ಗಣ_blksize(ದತ್ತಾಂಶ->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	ಆದರೆ (!ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ || !ದತ್ತಾಂಶ->tdb_written)
		ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ = blksize;
	channel->ಖಂಡ_ಗಾತ್ರ = blksize;
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_read_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	errcode_t	retval = 0;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_read_blk64(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ undo_read_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t undo_write_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ಖಂಡ, ಎಣಿಕೆ_2);
	ಆದರೆ (retval)
		 ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_write_blk64(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2, buf);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ undo_write_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t undo_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				 ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ blk_ಅಂಕೆ, ಎಣಿಕೆ_2;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = ಸ್ಥಾನಾಂತರ + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;
	blk_ಅಂಕೆ = location/channel->ಖಂಡ_ಗಾತ್ರ;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ಎಣಿಕೆ_2 = (ಗಾತ್ರ_2 + (location % channel->ಖಂಡ_ಗಾತ್ರ) +
			channel->ಖಂಡ_ಗಾತ್ರ  -1)/channel->ಖಂಡ_ಗಾತ್ರ;
	retval = undo_write_tdb(channel, blk_ಅಂಕೆ, ಎಣಿಕೆ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ದತ್ತಾಂಶ->real && ದತ್ತಾಂಶ->real->manager->write_byte)
		retval = io_channel_write_byte(ದತ್ತಾಂಶ->real, ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_2, buf);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_discard(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಂಕ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ಎಣಿಕೆ_2 > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
		ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
	icount = ಎಣಿಕೆ_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ಖಂಡ, icount);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_discard(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_zeroout(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಂಕ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ಎಣಿಕೆ_2 > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
		ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
	icount = ಎಣಿಕೆ_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ಖಂಡ, icount);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_zeroout(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_cache_readahead(io_channel channel,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_cache_readahead(ದತ್ತಾಂಶ->real, ಖಂಡ, ಎಣಿಕೆ_2);

	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Flush data buffers to disk.
 */
ಸ್ಥಿರ errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = io_channel_flush(ದತ್ತಾಂಶ->real);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_ಗಣ_option(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	errcode_t	retval = 0;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tmp;
	ಅಕ್ಷರ *ಕೊನೆ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (!strcmp(option, "tdb_data_size")) {
		ಆದರೆ (!arg)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		tmp = strtoul(arg, &ಕೊನೆ, 0);
		ಆದರೆ (*ಕೊನೆ)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ಆದರೆ (tmp > E2_UNDO_MAX_ಖಂಡ_ಗಾತ್ರ || tmp < E2_UNDO_MIN_ಖಂಡ_ಗಾತ್ರ)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ಆದರೆ (!ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ || !ದತ್ತಾಂಶ->tdb_written) {
			ದತ್ತಾಂಶ->tdb_written = -1;
			ದತ್ತಾಂಶ->tdb_ದತ್ತಾಂಶ_ಗಾತ್ರ = tmp;
		}
		ಹಿಂತಿರುಗಿ 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	ಆದರೆ (ದತ್ತಾಂಶ->real && ದತ್ತಾಂಶ->real->manager->ಗಣ_option) {
		retval = ದತ್ತಾಂಶ->real->manager->ಗಣ_option(ದತ್ತಾಂಶ->real,
							option, arg);
	}
	ಆದರೆ (!retval && !strcmp(option, "offset")) {
		ಆದರೆ (!arg)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		tmp = strtoul(arg, &ಕೊನೆ, 0);
		ಆದರೆ (*ಕೊನೆ)
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
		ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ = tmp;
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ದತ್ತಾಂಶ = (ರಚನೆ undo_ಖಾಸಗಿ_ದತ್ತಾಂಶ *) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(ದತ್ತಾಂಶ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ಆದರೆ (ದತ್ತಾಂಶ->real)
		retval = (ದತ್ತಾಂಶ->real->manager->get_stats)(ದತ್ತಾಂಶ->real, stats);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.ಗಣ_blksize	= undo_ಗಣ_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.ಗಣ_option	= undo_ಗಣ_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
