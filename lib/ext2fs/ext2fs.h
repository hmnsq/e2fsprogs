/*
 * ext2fs.h --- ext2fs
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _EXT2FS_EXT2FS_H
#ವ್ಯಾಖ್ಯಾನಿಸು _EXT2FS_EXT2FS_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_ATTR(x) __attribute__(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_ATTR(x)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __nonstring
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __has_attribute
#ಆದರೆ __has_attribute(__nonstring__)
#ವ್ಯಾಖ್ಯಾನಿಸು __nonstring                    __attribute__((__nonstring__))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __nonstring
#ಆದರೆ_ಅಂತ್ಯ /* __has_attribute(__nonstring__) */
#ಇಲ್ಲದಿದ್ದರೆ
# define __nonstring
#ಆದರೆ_ಅಂತ್ಯ /* __has_attribute */
#ಆದರೆ_ಅಂತ್ಯ /* __nonstring */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_NO_TDB_ಬಳಸದ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_NO_TDB_ಬಳಸದ	EXT2FS_ATTR((ಬಳಸದ))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#ಆದರೆ (!defined(__GNUC__) && !defined(__WATCOMC__))
#ವ್ಯಾಖ್ಯಾನಿಸು NO_INLINE_FUNCS
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SUPERBLOCK_ಸ್ಥಾನಾಂತರ	1024
#ವ್ಯಾಖ್ಯಾನಿಸು SUPERBLOCK_ಗಾತ್ರ		1024

#ವ್ಯಾಖ್ಯಾನಿಸು UUID_STR_ಗಾತ್ರ 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಆದರೆ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "e2_types.h"
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext3_extents.h"
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_fs.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext3_extents.h>
#ಆದರೆ_ಅಂತ್ಯ /* EXT2_FLAT_INCLUDES */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u32 __bitwise		ext2_ino_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u32 __bitwise		blk_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u64 __bitwise		blk64_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u32 __bitwise		dgrp_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __s32 __bitwise		ext2_off_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __s64 __bitwise		ext2_off64_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __s64 __bitwise		e2_blkcnt_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u32 __bitwise		ext2_dirhash_t;

#ಆದರೆ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_io.h"
#ಅನ್ತರ್ಭಾವ "ext2_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_ext_attr.h"
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <et/com_err.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_io.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_err.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_ext_attr.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_QSORT_TYPE ಪೂರ್ಣಾಂಕ __cdecl
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_QSORT_TYPE ಪೂರ್ಣಾಂಕ
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ struct_ext2_filsys *ext2_filsys;

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_MARK_ERROR 	0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_UNMARK_ERROR 	1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_TEST_ERROR	2

ರಚನೆ ext2fs_struct_generic_bitmap_ತಳ {
	errcode_t		magic;
	ext2_filsys 		fs;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_struct_generic_bitmap_ತಳ *ext2fs_generic_bitmap;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_struct_generic_bitmap_ತಳ *ext2fs_inode_bitmap;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_struct_generic_bitmap_ತಳ *ext2fs_ಖಂಡ_bitmap;

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಮೊದಲ_INODE(s)	EXT2_ಮೊದಲ_INO(s)


/*
 * Badblocks list definitions
 */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_list *ext2_badblocks_list;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_iterate *ext2_badblocks_iterate;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_list *ext2_u32_list;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_list *badblocks_list;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_u32_iterate *badblocks_iterate;

#ವ್ಯಾಖ್ಯಾನಿಸು BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ರಚನೆ ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ರಚನೆ ext2_db_ನಮೂದು {
	ext2_ino_t	ino;
	blk_t	blk;
	ಪೂರ್ಣಾಂಕ	blockcnt;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_dblist *ext2_dblist;

#ವ್ಯಾಖ್ಯಾನಿಸು DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FILE_WRITE		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FILE_CREATE	0x0002

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FILE_ಮುಖವಾಡ		0x00FF

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FILE_BUF_DIRTY	0x4000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FILE_BUF_VALID	0x2000

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_file *ext2_file_t;

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SEEK_ಗಣ	0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SEEK_CUR	1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SEEK_ಕೊನೆ	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_RW			0x01
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ಬದಲಾಯಿತು		0x02
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_DIRTY			0x04
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_VALID			0x08
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IB_DIRTY		0x10
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_BB_DIRTY		0x20
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು		0x40
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು_READ	0x80
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು_WRITE	0x100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_MASTER_SB_ONLY	0x200
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_FORCE			0x400
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_SUPER_ONLY		0x800
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IMAGE_FILE		0x2000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_EXCLUSIVE		0x4000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು		0x20000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_PRINT_ಪ್ರಗತಿ	0x40000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ನೇರ_IO		0x80000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_SKIP_MMP		0x100000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_SHARE_DUP		0x400000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_BBITMAP_ಬಾಲ_PROBLEM	0x1000000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IBITMAP_ಬಾಲ_PROBLEM	0x2000000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_ಎಳೆಗಳು		0x4000000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_IGNORE_ಅದಲುಬದಲು_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG2_USE_FAKE_ಸಮಯ	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_JOURNAL_ಗಮ್ಯ_FC_BLKS_ಅನುಪಾತ		64

ರಚನೆ blk_alloc_ctx;
ರಚನೆ opaque_ext2_ಗುಂಪು_desc;

ರಚನೆ struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	ಪೂರ್ಣಾಂಕ				flags;
	ಅಕ್ಷರ *				device_name;
	ರಚನೆ ext2_super_ಖಂಡ	* 	super;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ			blocksize;
	ಪೂರ್ಣಾಂಕ				fragsize;
	dgrp_t				ಗುಂಪು_desc_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ			desc_ಖಂಡಗಳು;
	ರಚನೆ opaque_ext2_ಗುಂಪು_desc *	ಗುಂಪು_desc;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ			inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_ಖಂಡ_bitmap		ಖಂಡ_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_ಖಂಡಗಳು)(ext2_filsys fs, ext2_ino_t ino, blk_t *ಖಂಡಗಳು);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ರಚನೆ ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ರಚನೆ ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ರಚನೆ ext2_super_ಖಂಡ *	orig_super;
	ರಚನೆ ext2_image_hdr *		image_ಶೀರ್ಷಿಕೆ;
	__u32				umask;
	time_t				now;
	ಪೂರ್ಣಾಂಕ				cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು;
	__u16				default_bitmap_type;
	__u16				pad;
	__u32				flags2;
	/*
	 * Reserved for future expansion
	 */
	__u32				reserved[4];

	/*
	 * Reserved for the use of the calling application.
	 */
	ಶೂನ್ಯ *				priv_ದತ್ತಾಂಶ;

	/*
	 * Inode cache
	 */
	ರಚನೆ ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_ಖಂಡ)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ರಚನೆ blk_alloc_ctx *ctx);
	ಶೂನ್ಯ (*ಖಂಡ_alloc_stats)(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಂಕ inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	ಶೂನ್ಯ *mmp_buf;
	ಶೂನ್ಯ *mmp_cmp;
	ಪೂರ್ಣಾಂಕ mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	ದೀರ್ಘ mmp_ಕೊನೆಯ_written;

	/* progress operation functions */
	ರಚನೆ ext2fs_ಪ್ರಗತಿ_ops *ಪ್ರಗತಿ_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_ಬೀಜ;

	io_channel			journal_io;
	ಅಕ್ಷರ				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	ಶೂನ್ಯ (*ಖಂಡ_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t ಅಂಕೆ,
					ಪೂರ್ಣಾಂಕ inuse);

	/* hashmap for SHA of data blocks */
	ರಚನೆ ext2fs_hashmap* ಖಂಡ_sha_map;

	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *encoding;
};

#ಆದರೆ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "e2_bitops.h"
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <ext2fs/bitops.h>
#ಆದರೆ_ಅಂತ್ಯ

/*
 * 64-bit bitmap backend types
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BMAP64_BITARRAY	1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BMAP64_RBTREE	2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ಬದಲಾಯಿತು			1
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ABORT			2
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ERROR			4
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು	8

/*
 * Block iterate flags
 *
 * BLOCK_FLAG_APPEND, or BLOCK_FLAG_HOLE, indicates that the iterator
 * function should be called on blocks where the block number is zero.
 * This is used by ext2fs_expand_dir() to be able to add a new block
 * to an inode.  It can also be used for programs that want to be able
 * to deal with files that contain "holes".
 *
 * BLOCK_FLAG_DEPTH_TRAVERSE indicates that the iterator function for
 * the indirect, doubly indirect, etc. blocks should be called after
 * all of the blocks contained in the indirect blocks are processed.
 * This is useful if you are going to be deallocating blocks from an
 * inode.
 *
 * BLOCK_FLAG_DATA_ONLY indicates that the iterator function should be
 * called for data blocks only.
 *
 * BLOCK_FLAG_READ_ONLY is a promise by the caller that it will not
 * modify returned block number.
 *
 * BLOCK_FLAG_NO_LARGE is for internal use only.  It informs
 * ext2fs_block_iterate2 that large files won't be accepted.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_APPEND	1
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_HOLE		1
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_ಆಳ_TRAVERSE	2
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY	4
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_READ_ONLY	8

#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ಎಣಿಕೆ_IND		(-1)
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ಎಣಿಕೆ_DIND	(-2)
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ಎಣಿಕೆ_TIND	(-3)
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ಎಣಿಕೆ_TRANSLATOR	(-4)

#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ALLOC_UNKNOWN	0
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ALLOC_ದತ್ತಾಂಶ	1
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_ALLOC_METADATA	2

ರಚನೆ blk_alloc_ctx {
	ext2_ino_t		ino;
	ರಚನೆ ext2_inode	*inode;
	blk64_t			lblk;
	ಪೂರ್ಣಾಂಕ			flags;
};

#ಆದರೆ 0
/*
 * Flags for ext2fs_move_blocks
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BMOVE_GET_DBLIST	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BMOVE_DEBUG	0x0002
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_FLAGS_ಎಲೆ		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_FLAGS_UNINIT	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ	0x0004

ರಚನೆ ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_extent_handle *ext2_extent_handle_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_extent_ಮಾರ್ಗ *ext2_extent_ಮಾರ್ಗ_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_CURRENT	0x0000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_MOVE_ಮುಖವಾಡ	0x000F
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಬೇರು	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಕೊನೆಯ_ಎಲೆ	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಮೊದಲ_SIB	0x0003
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಕೊನೆಯ_SIB	0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಮುಂದಿನ_SIB	0x0005
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_PREV_SIB	0x0006
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ	0x0007
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_PREV_ಎಲೆ	0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಮುಂದಿನ	0x0009
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_PREV	0x000A
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಮೇಲೆ		0x000B
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಕೆಳಕ್ಕೆ	0x000C
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಕೆಳಕ್ಕೆ_AND_ಕೊನೆಯ 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಸೇರಿಸು_AFTER	0x0001 /* insert after handle loc'n */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಸೇರಿಸು_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_DELETE_KEEP_ಖಾಲಿಗಳು	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXTENT_ಗಣ_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
ರಚನೆ ext2_extent_info {
	ಪೂರ್ಣಾಂಕ		curr_ನಮೂದು;
	ಪೂರ್ಣಾಂಕ		curr_level;
	ಪೂರ್ಣಾಂಕ		ಅಂಕೆ_entries;
	ಪೂರ್ಣಾಂಕ		max_entries;
	ಪೂರ್ಣಾಂಕ		max_ಆಳ;
	ಪೂರ್ಣಾಂಕ		ಬೈಟುಗಳು_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_ಬದಲಾಯಿತು	1
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_ABORT	2
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು	1
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_FLAG_INCLUDE_REMOVED	2
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_FLAG_INCLUDE_CSUM	4
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_FLAG_INCLUDE_INLINE_ದತ್ತಾಂಶ 8

#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_DOT_FILE		1
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_DOT_DOT_FILE	2
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_OTHER_FILE	3
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_DELETED_FILE	4
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_struct_inode_ಪರಿಶೀಲನೆ *ext2_inode_ಪರಿಶೀಲನೆ;

/*
 * ext2fs_scan flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_CHK_BADBLOCKS	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_BAD_INODE_BLK	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_BAD_EXTRA_ಬೈಟುಗಳು	0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_DO_LAZY		0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_MOUNTED		1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_ISROOT		2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_READONLY	4
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_ಅದಲುಬದಲು		8
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_BUSY		16
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFMT  00170000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFSOCK 0140000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFLNK	 0120000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFREG  0100000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFBLK  0060000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFDIR  0040000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFCHR  0020000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IFIFO  0010000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISUID  0004000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISGID  0002000
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISVTX  0001000

#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IRWXU 00700
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IRUSR 00400
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IWUSR 00200
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IXUSR 00100

#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IRWXG 00070
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IRGRP 00040
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IWGRP 00020
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IXGRP 00010

#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IRWXO 00007
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IROTH 00004
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IWOTH 00002
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_IXOTH 00001

#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_I_ಗಾತ್ರ(i)	((i)->i_ಗಾತ್ರ | ((__u64) (i)->i_ಗಾತ್ರ_ಮೇಲಿನ << 32))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((ಬಳಸದ)),
					__u32 nsec)
{
	__u32 extra = 0;

#ಆದರೆ (SIZEOF_ಸಮಯ_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_ಮುಖವಾಡ;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ extra | (nsec << EXT4_EPOCH_ಬಿಟ್ಟುಗಳು);
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((ಬಳಸದ)))
{
#ಆದರೆ (SIZEOF_ಸಮಯ_T > 4)
	ಆದರೆ (extra & EXT4_EPOCH_ಮುಖವಾಡ)
		seconds += ((time_t)(extra & EXT4_EPOCH_ಮುಖವಾಡ) << 32);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ seconds;
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ __u32 __decode_extra_nsec(__u32 extra)
{
	ಹಿಂತಿರುಗಿ (extra & EXT4_NSEC_ಮುಖವಾಡ) >> EXT4_EPOCH_ಬಿಟ್ಟುಗಳು;
}
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_inode_actual_ಗಾತ್ರ(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +				      \
		  (ಗಾತ್ರ(*inode) > EXT2_GOOD_OLD_INODE_ಗಾತ್ರ ?		      \
		   ((ರಚನೆ ext2_inode_large *)(inode))->i_extra_isize : 0)))
#ವ್ಯಾಖ್ಯಾನಿಸು clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_inode_xtime_ಗಣ(inode, ಕ್ಷೇತ್ರ, sec)			      \
ಮಾಡು {									      \
	ಆದರೆ (ext2fs_inode_includes(ext2fs_inode_actual_ಗಾತ್ರ(inode),	      \
				  ಕ್ಷೇತ್ರ ## _extra)) {			      \
		(inode)->ಕ್ಷೇತ್ರ = (__u32)(sec & 0xfffffff);		      \
		((ರಚನೆ ext2_inode_large *)(inode))->ಕ್ಷೇತ್ರ ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} ಇಲ್ಲದಿದ್ದರೆ {							      \
		(inode)->ಕ್ಷೇತ್ರ = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_inode_xtime_get(inode, ಕ್ಷೇತ್ರ)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_ಗಾತ್ರ(inode), ಕ್ಷೇತ್ರ ## _extra) ?    \
	__decode_extra_sec((inode)->ಕ್ಷೇತ್ರ,				      \
		((ರಚನೆ ext2_inode_large *)(inode))->ಕ್ಷೇತ್ರ ## _extra) :      \
		(time_t)(inode)->ಕ್ಷೇತ್ರ)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((ಬಳಸದ)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#ಆದರೆ (SIZEOF_ಸಮಯ_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_ಮುಖವಾಡ;
#ಇಲ್ಲದಿದ್ದರೆ
	*hi = 0;
#ಆದರೆ_ಅಂತ್ಯ
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((ಬಳಸದ)))
{
#ಆದರೆ (SIZEOF_ಸಮಯ_T == 4)
	ಹಿಂತಿರುಗಿ *lo;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ ((time_t)(*hi) << 32) | *lo;
#ಆದರೆ_ಅಂತ್ಯ
}
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_ಗಣ_tstamp(sb, ಕ್ಷೇತ್ರ, seconds) \
	__sb_set_tstamp(&(sb)->ಕ್ಷೇತ್ರ, &(sb)->ಕ್ಷೇತ್ರ ## _hi, seconds)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_get_tstamp(sb, ಕ್ಷೇತ್ರ) \
	__sb_get_tstamp(&(sb)->ಕ್ಷೇತ್ರ, &(sb)->ಕ್ಷೇತ್ರ ## _hi)

/*
 * ext2_icount_t abstraction
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ICOUNT_OPT_INCREMENT	0x01
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ICOUNT_OPT_FULLMAP		0x02

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_ALLOC	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_ಗಣ	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_UNINIT	0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#ವ್ಯಾಖ್ಯಾನಿಸು READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#ವ್ಯಾಖ್ಯಾನಿಸು IMAGER_FLAG_INODEMAP	1
#ವ್ಯಾಖ್ಯಾನಿಸು IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

/*
 * Features supported by this version of the library
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_ಸೂಚ್ಯಂಕ|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_LIB_INCOMPAT_MMP		(0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_ಬಿಟ್|\
					 EXT4_FEATURE_INCOMPAT_INLINE_ದತ್ತಾಂಶ|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_ಬೀಜ|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
					 EXT4_FEATURE_RO_COMPAT_HUGE_FILE|\
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE|\
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK|\
					 EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE|\
					 EXT4_FEATURE_RO_COMPAT_GDT_CSUM|\
					 EXT4_FEATURE_RO_COMPAT_BIGALLOC|\
					 EXT4_FEATURE_RO_COMPAT_QUOTA|\
					 EXT4_FEATURE_RO_COMPAT_METADATA_CSUM|\
					 EXT4_FEATURE_RO_COMPAT_READONLY |\
					 EXT4_FEATURE_RO_COMPAT_PROJECT |\
					 EXT4_FEATURE_RO_COMPAT_SHARED_ಖಂಡಗಳು |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_CLUSTER_ಅನುಪಾತ(fs)	(1 << (fs)->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)		(EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು)
/* Translate a cluster number to a block number */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು)
/* Translate # of blks to # of clusters */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_ಅಂಕೆ_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)) >> \
					 (fs)->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು)

#ಆದರೆ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ stat64 ext2fs_struct_stat;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ stat ext2fs_struct_stat;
#ಆದರೆ_ಅಂತ್ಯ

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ರಚನೆ ext2_xattr_handle;
#ವ್ಯಾಖ್ಯಾನಿಸು XATTR_ABORT	1
#ವ್ಯಾಖ್ಯಾನಿಸು XATTR_ಬದಲಾಯಿತು	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BITMAPS_WRITE		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_BITMAPS_ಖಂಡ		0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BITMAPS_INODE		0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_LINK_FT_ಮುಖವಾಡ	0x0007
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_LINK_APPEND	0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_has_ಗುಂಪು_desc_csum(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_needs_large_file_feature(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ file_ಗಾತ್ರ)
{
	ಹಿಂತಿರುಗಿ file_ಗಾತ್ರ >= 0x80000000ULL;
}

/* alloc.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_ಖಂಡ_uninit(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_ಖಂಡ(ext2_filsys fs, blk_t goal,
				  ext2fs_ಖಂಡ_bitmap map, blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_ಖಂಡ_bitmap map, blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_ಖಂಡ_bitmap map, blk64_t *ret,
				   ರಚನೆ blk_alloc_ctx *ctx);
ಬಾಹ್ಯ errcode_t ext2fs_get_free_ಖಂಡಗಳು(ext2_filsys fs, blk_t ಆರಂಭ,
					blk_t ಮುಕ್ತಾಯ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ext2fs_ಖಂಡ_bitmap map,
					blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ಆರಂಭ,
					 blk64_t ಮುಕ್ತಾಯ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					 ext2fs_ಖಂಡ_bitmap map,
					 blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_ಖಂಡ(ext2_filsys fs, blk_t goal,
				    ಅಕ್ಷರ *ಖಂಡ_buf, blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t *ret,
				     ರಚನೆ blk_alloc_ctx *ctx);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_alloc_ಖಂಡ_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ರಚನೆ ext2_inode *inode, blk64_t lblk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_ಖಂಡ_alloc_stats_range_callback(ext2_filsys fs,
	ಶೂನ್ಯ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse),
	ಶೂನ್ಯ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse));
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ	(0x2)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			   blk64_t len, ext2fs_ಖಂಡ_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ALLOCRANGE_ZERO_ಖಂಡಗಳು	(0x2)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t ಗುಂಪು,
					ext2fs_ಖಂಡ_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t ಗುಂಪು,
					       ext2fs_ಖಂಡ_bitmap bmap,
					       blk_t *desc_ಖಂಡಗಳು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_ಖಂಡ_alloc_stats_callback(ext2_filsys fs,
						  ಶೂನ್ಯ (*func)(ext2_filsys fs,
							       blk64_t blk,
							       ಪೂರ್ಣಾಂಕ inuse),
						  ಶೂನ್ಯ (**old)(ext2_filsys fs,
							       blk64_t blk,
							       ಪೂರ್ಣಾಂಕ inuse));

/* alloc_stats.c */
ಶೂನ್ಯ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ಪೂರ್ಣಾಂಕ inuse);
ಶೂನ್ಯ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ಪೂರ್ಣಾಂಕ inuse, ಪೂರ್ಣಾಂಕ isdir);
ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಂಕ inuse);
ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಂಕ inuse);
ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse);

/* alloc_tables.c */
ಬಾಹ್ಯ errcode_t ext2fs_allocate_tables(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_ಗುಂಪು_ಕೋಷ್ಟಕ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					     ext2fs_ಖಂಡ_bitmap bmap);

/* badblocks.c */
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_ಕೂಡಿಸು(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_ಕಂಡುಹಿಡಿ(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_u32_list_iterate_ಕೊನೆ(ext2_u32_iterate iter);
ಬಾಹ್ಯ errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

ಬಾಹ್ಯ errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_badblocks_list_ಕೂಡಿಸು(ext2_badblocks_list bb,
					   blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_iterate_ಕೊನೆ(ext2_badblocks_iterate iter);
ಬಾಹ್ಯ errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_u32_list_ಎಣಿಕೆ(ext2_u32_list bb);

/* bb_compat */
ಬಾಹ್ಯ errcode_t badblocks_list_create(badblocks_list *ret, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t badblocks_list_ಕೂಡಿಸು(badblocks_list bb, blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ badblocks_list_test(badblocks_list bb, blk_t blk);
ಬಾಹ್ಯ errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ badblocks_list_iterate_ಕೊನೆ(badblocks_iterate iter);
ಬಾಹ್ಯ ಶೂನ್ಯ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
ಬಾಹ್ಯ errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_ಖಂಡ_bitmap(ext2_filsys fs,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
					      ext2fs_ಖಂಡ_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
						   ext2fs_ಖಂಡ_bitmap *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_get_bitmap_granularity(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
					      ext2fs_inode_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_inode_bitmap_ಕೊನೆ(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t ಕೊನೆ, ext2_ino_t *oend);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_ಖಂಡ_bitmap_ಕೊನೆ(ext2fs_ಖಂಡ_bitmap bitmap,
					       blk_t ಕೊನೆ, blk_t *oend);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_ಖಂಡ_bitmap_end2(ext2fs_ಖಂಡ_bitmap bitmap,
					 blk64_t ಕೊನೆ, blk64_t *oend);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_inode_bitmap(__u32 new_ಕೊನೆ, __u32 new_real_ಕೊನೆ,
					    ext2fs_inode_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_inode_bitmap2(__u64 new_ಕೊನೆ,
					     __u64 new_real_ಕೊನೆ,
					     ext2fs_inode_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_ಖಂಡ_bitmap(__u32 new_ಕೊನೆ, __u32 new_real_ಕೊನೆ,
					    ext2fs_ಖಂಡ_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_ಖಂಡ_bitmap2(__u64 new_ಕೊನೆ,
					     __u64 new_real_ಕೊನೆ,
					     ext2fs_ಖಂಡ_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_compare_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bm1,
					     ext2fs_ಖಂಡ_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bmap,
					blk_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bmap,
					 blk64_t ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_get_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bmap,
					blk_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_get_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bmap,
					 blk64_t ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಹೊರಹೋಗು);

/* blknum.c */
ಬಾಹ್ಯ __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ __u32 ext2fs_ಖಂಡ_bitmap_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ dgrp_t ext2fs_ಗುಂಪು_ದ_blk2(ext2_filsys fs, blk64_t);
ಬಾಹ್ಯ blk64_t ext2fs_ಗುಂಪು_ಮೊದಲ_block2(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk64_t ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ಗುಂಪು_ಖಂಡಗಳು_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk64_t ext2fs_inode_ದತ್ತಾಂಶ_blocks2(ext2_filsys fs,
					 ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_inode_i_ಖಂಡಗಳು(ext2_filsys fs,
				     ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_get_stat_i_ಖಂಡಗಳು(ext2_filsys fs,
					ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super,
				    blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super,
				    blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super,
				      blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super,
				      blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super,
					 blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
ಬಾಹ್ಯ ರಚನೆ ext2_ಗುಂಪು_desc *ext2fs_ಗುಂಪು_desc(ext2_filsys fs,
					  ರಚನೆ opaque_ext2_ಗುಂಪು_desc *gdp,
					  dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk64_t ext2fs_ಖಂಡ_bitmap_csum(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk64_t ext2fs_ಖಂಡ_bitmap_loc(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಖಂಡ_bitmap_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					blk64_t blk);
ಬಾಹ್ಯ __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_inode_bitmap_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_inode_ಕೋಷ್ಟಕ_loc(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_inode_ಕೋಷ್ಟಕ_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				       blk64_t blk);
ಬಾಹ್ಯ __u32 ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					 __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_free_inodes_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					 __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				       __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_itable_ಬಳಸದ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_itable_ಬಳಸದ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				     __u32 n);
ಬಾಹ್ಯ __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flag);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flags);
ಬಾಹ್ಯ __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_checksum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 checksum);
ಬಾಹ್ಯ blk64_t ext2fs_file_acl_ಖಂಡ(ext2_filsys fs,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_file_acl_ಖಂಡ_ಗಣ(ext2_filsys fs,
				      ರಚನೆ ext2_inode *inode, blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_inode_ಗಾತ್ರ_ಗಣ(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				       ext2_off64_t ಗಾತ್ರ_2);

/* block.c */
ಬಾಹ್ಯ errcode_t ext2fs_ಖಂಡ_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      ಪೂರ್ಣಾಂಕ	flags,
				      ಅಕ್ಷರ *ಖಂಡ_buf,
				      ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  ಪೂರ್ಣಾಂಕ	blockcnt,
						  ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
errcode_t ext2fs_ಖಂಡ_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				ಪೂರ್ಣಾಂಕ	flags,
				ಅಕ್ಷರ *ಖಂಡ_buf,
				ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
					    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
errcode_t ext2fs_ಖಂಡ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಂಕ	flags,
				ಅಕ್ಷರ *ಖಂಡ_buf,
				ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
					    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

/* bmap.c */
ಬಾಹ್ಯ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode *inode,
			     ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags,
			     blk_t ಖಂಡ, blk_t *phys_blk);
ಬಾಹ್ಯ errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ರಚನೆ ext2_inode *inode,
			      ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags, blk64_t ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_ಖಂಡ(ext2_filsys fs, ext2_ino_t ino,
				   ರಚನೆ ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#ಆದರೆ 0
/* bmove.c */
ಬಾಹ್ಯ errcode_t ext2fs_move_ಖಂಡಗಳು(ext2_filsys fs,
				    ext2fs_ಖಂಡ_bitmap reserve,
				    ext2fs_ಖಂಡ_bitmap alloc_map,
				    ಪೂರ್ಣಾಂಕ flags);
#ಆದರೆ_ಅಂತ್ಯ

/* check_desc.c */
ಬಾಹ್ಯ errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
ಬಾಹ್ಯ errcode_t ext2fs_close(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_close2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_close_free(ext2_filsys *fs);
ಬಾಹ್ಯ errcode_t ext2fs_flush(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_flush2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t ಗುಂಪು_ಖಂಡ);
ಬಾಹ್ಯ errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t ಗುಂಪು,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_ಬಳಸಿದ_blks);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t ಗುಂಪು,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    ಪೂರ್ಣಾಂಕ *ret_meta_bg);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
ಬಾಹ್ಯ __u32 ext2fs_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_be(__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p, size_t len);
ಬಾಹ್ಯ __u32 ext2fs_crc32c_le(__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p, size_t len);

/* csum.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_init_csum_ಬೀಜ(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_mmp_csum_ಗಣ(ext2_filsys fs, ರಚನೆ mmp_struct *mmp);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mmp_csum_verify(ext2_filsys, ರಚನೆ mmp_struct *mmp);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_verify_csum_type(ext2_filsys fs, ರಚನೆ ext2_super_ಖಂಡ *sb);
ಬಾಹ್ಯ errcode_t ext2fs_superblock_csum_ಗಣ(ext2_filsys fs,
					    ರಚನೆ ext2_super_ಖಂಡ *sb);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ರಚನೆ ext2_super_ಖಂಡ *sb);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_ಖಂಡ_csum_ಗಣ(ext2_filsys fs,
					ext2_ino_t inum, blk64_t ಖಂಡ,
					ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *hdr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ext_attr_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t ಖಂಡ,
					     ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *hdr);
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRENT_ಬಾಲ(ಖಂಡ, blocksize) \
	((ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *)(((ಅಕ್ಷರ *)(ಖಂಡ)) + \
	(blocksize) - ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ)))

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_initialize_dirent_ಬಾಲ(ext2_filsys fs,
					  ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dirent_has_ಬಾಲ(ext2_filsys fs,
				  ರಚನೆ ext2_dir_ನಮೂದು *dirent);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ರಚನೆ ext2_dir_ನಮೂದು *dirent);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dir_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ರಚನೆ ext2_dir_ನಮೂದು *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_dir_ಖಂಡ_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
					   ರಚನೆ ext2_dir_ನಮೂದು *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					  ರಚನೆ ext2_dx_countlimit **cc,
					  ಪೂರ್ಣಾಂಕ *ಸ್ಥಾನಾಂತರ);
ಬಾಹ್ಯ errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ರಚನೆ ext2_dir_ನಮೂದು *dirent,
				__u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ರಚನೆ ext2_dx_ಬಾಲ **ret_t);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಖಂಡ_csum_ಗಣ(ext2_filsys fs,
					      ext2_ino_t inum,
					      ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_extent_ಖಂಡ_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh);
ಬಾಹ್ಯ errcode_t ext2fs_ಖಂಡ_bitmap_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					      ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ಖಂಡ_bitmap_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು,
					   ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_inode_bitmap_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
					      ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು,
					   ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_inode_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
				       ರಚನೆ ext2_inode_large *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ರಚನೆ ext2_inode_large *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗುಂಪು_desc_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ಗುಂಪು_desc_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_gdt_csum(ext2_filsys fs);
ಬಾಹ್ಯ __u16 ext2fs_ಗುಂಪು_desc_csum(ext2_filsys fs, dgrp_t ಗುಂಪು);

/* dblist.c */
ಬಾಹ್ಯ errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_dir_ಖಂಡ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ಪೂರ್ಣಾಂಕ blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dblist_ವಿಂಗಡಿಸು(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *,
							   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *));
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *,
							    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *));
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs, ರಚನೆ ext2_db_ನಮೂದು *db_info,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
	ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs, ರಚನೆ ext2_db_entry2 *db_info,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
	ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs, ರಚನೆ ext2_db_entry2 *db_info,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಆರಂಭ,
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2,
	ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_dir_ಖಂಡ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ಪೂರ್ಣಾಂಕ blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dblist_ಎಣಿಕೆ(ext2_dblist dblist);
ಬಾಹ್ಯ blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_get_ಕೊನೆಯ(ext2_dblist dblist,
					ರಚನೆ ext2_db_ನಮೂದು **ನಮೂದು);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ರಚನೆ ext2_db_entry2 **ನಮೂದು);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_drop_ಕೊನೆಯ(ext2_dblist dblist);

/* dblist_dir.c */
ಬಾಹ್ಯ errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  ಪೂರ್ಣಾಂಕ	flags,
				  ಅಕ್ಷರ	*ಖಂಡ_buf,
				  ಪೂರ್ಣಾಂಕ (*func)(ext2_ino_t	dir,
					      ಪೂರ್ಣಾಂಕ		ನಮೂದು,
					      ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					      ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
					      ಪೂರ್ಣಾಂಕ	blocksize,
					      ಅಕ್ಷರ	*buf,
					      ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				  ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

#ಆದರೆ 0
/* digest_encode.c */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_DIGEST_ಗಾತ್ರ EXT2_FS_SHA256_ಉದ್ದಳತೆ
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_digest_encode(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ len, ಅಕ್ಷರ *dst);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_digest_decode(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ len, ಅಕ್ಷರ *dst);
#ಆದರೆ_ಅಂತ್ಯ

/* dirblock.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_ಖಂಡ(ext2_filsys fs, blk_t ಖಂಡ,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ಖಂಡ,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ಖಂಡ,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ಖಂಡ,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_ಖಂಡ(ext2_filsys fs, blk_t ಖಂಡ,
					ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ಖಂಡ,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ಖಂಡ,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ಖಂಡ,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags, ext2_ino_t ino);

/* dirhash.c */
ಬಾಹ್ಯ errcode_t ext2fs_dirhash(ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ len,
				ಸ್ಥಿರಾಂಕ __u32 *ಬೀಜ,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

ಬಾಹ್ಯ errcode_t ext2fs_dirhash2(ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ len,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *charset,
				 ಪೂರ್ಣಾಂಕ hash_flags,
				 ಸ್ಥಿರಾಂಕ __u32 *ಬೀಜ,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ರಚನೆ ext2_dir_ನಮೂದು *dirent,
				    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *rec_len);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_rec_len(ext2_filsys fs,
				    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len,
				    ರಚನೆ ext2_dir_ನಮೂದು *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಂಕ flags,
			      ಅಕ್ಷರ *ಖಂಡ_buf,
			      ಪೂರ್ಣಾಂಕ (*func)(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					  ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
					  ಪೂರ್ಣಾಂಕ	blocksize,
					  ಅಕ್ಷರ	*buf,
					  ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
			      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಂಕ flags,
			      ಅಕ್ಷರ *ಖಂಡ_buf,
			      ಪೂರ್ಣಾಂಕ (*func)(ext2_ino_t	dir,
					  ಪೂರ್ಣಾಂಕ	ನಮೂದು,
					  ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					  ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
					  ಪೂರ್ಣಾಂಕ	blocksize,
					  ಅಕ್ಷರ	*buf,
					  ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
			      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

/* dupfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
ಬಾಹ್ಯ errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
ಬಾಹ್ಯ __u32 ext2fs_ext_attr_hash_ನಮೂದು(ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
					ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಬಾಹ್ಯ __u32 ext2fs_ext_attr_hash_ನಮೂದು_signed(ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
					       ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
					     ಶೂನ್ಯ *ದತ್ತಾಂಶ, __u32 *hash);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
					     ಶೂನ್ಯ *ದತ್ತಾಂಶ, __u32 *hash,
					     __u32 *signed_hash);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ಖಂಡ, ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ಖಂಡ,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ಖಂಡ,
				       ಶೂನ್ಯ *buf, ext2_ino_t inum);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ಖಂಡ,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ಖಂಡ,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ಖಂಡ,
				       ಶೂನ್ಯ *buf, ext2_ino_t inum);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   ಅಕ್ಷರ *ಖಂಡ_buf,
					   ಪೂರ್ಣಾಂಕ adjust, __u32 *newcount);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   ಅಕ್ಷರ *ಖಂಡ_buf,
					   ಪೂರ್ಣಾಂಕ adjust, __u32 *newcount);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   ಅಕ್ಷರ *ಖಂಡ_buf,
					   ಪೂರ್ಣಾಂಕ adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ರಚನೆ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ರಚನೆ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ರಚನೆ ext2_xattr_handle *handle,
				   ರಚನೆ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ರಚನೆ ext2_xattr_handle *h,
				ಪೂರ್ಣಾಂಕ (*func)(ಅಕ್ಷರ *name, ಅಕ್ಷರ *ಮೌಲ್ಯ,
					    size_t ಮೌಲ್ಯ_len, ಶೂನ್ಯ *ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *ದತ್ತಾಂಶ);
errcode_t ext2fs_xattr_get(ರಚನೆ ext2_xattr_handle *h, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
			   ಶೂನ್ಯ **ಮೌಲ್ಯ, size_t *ಮೌಲ್ಯ_len);
errcode_t ext2fs_xattr_ಗಣ(ರಚನೆ ext2_xattr_handle *handle,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
			   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ,
			   size_t ಮೌಲ್ಯ_len);
errcode_t ext2fs_xattr_remove(ರಚನೆ ext2_xattr_handle *handle,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);
errcode_t ext2fs_xattr_remove_all(ರಚನೆ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ರಚನೆ ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ರಚನೆ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_ಎಣಿಕೆ(ರಚನೆ ext2_xattr_handle *handle, size_t *ಎಣಿಕೆ_2);
errcode_t ext2fs_xattr_inode_max_ಗಾತ್ರ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ಗಾತ್ರ_2);
#ವ್ಯಾಖ್ಯಾನಿಸು XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ರಚನೆ ext2_xattr_handle *handle,
			      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *new_flags, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *old_flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ext_attr_ಖಂಡ_rehash(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ,
					 ರಚನೆ ext2_ext_attr_ನಮೂದು *ಕೊನೆ);
ಬಾಹ್ಯ __u32 ext2fs_get_ea_inode_hash(ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_ea_inode_hash(ರಚನೆ ext2_inode *inode, __u32 hash);
ಬಾಹ್ಯ __u64 ext2fs_get_ea_inode_ref(ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_ea_inode_ref(ರಚನೆ ext2_inode *inode, __u64 ref_ಎಣಿಕೆ);

/* extent.c */
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(ಶೂನ್ಯ *ptr, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ರಚನೆ ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_extent_free(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   ಪೂರ್ಣಾಂಕ flags, ರಚನೆ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಗಂಟು_ವಿಭಜಿಸು(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, ಪೂರ್ಣಾಂಕ flags,
				       ರಚನೆ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಸೇರಿಸು(ext2_extent_handle_t handle, ಪೂರ್ಣಾಂಕ flags,
				      ರಚನೆ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಗಣ_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ರಚನೆ ext2_extent_info *info);
ಬಾಹ್ಯ errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     ಪೂರ್ಣಾಂಕ ಎಲೆ_level, blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
ಬಾಹ್ಯ size_t ext2fs_max_extent_ಆಳ(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ errcode_t ext2fs_ಎಣಿಕೆ_ಖಂಡಗಳು(ext2_filsys fs, ext2_ino_t ino,
				     ರಚನೆ ext2_inode *inode, blk64_t *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_decode_extent(ರಚನೆ ext2fs_extent *ಗಮ್ಯ, ಶೂನ್ಯ *from,
				      ಪೂರ್ಣಾಂಕ len);

/* fallocate.c */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು	(0x1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FALLOCATE_FORCE_INIT	(0x2)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ext2_ino_t ino,
			   ರಚನೆ ext2_inode *inode, blk64_t goal,
			   blk64_t ಆರಂಭ, blk64_t len);

/* fileio.c */
ಬಾಹ್ಯ errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ರಚನೆ ext2_inode *inode,
				   ಪೂರ್ಣಾಂಕ flags, ext2_file_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  ಪೂರ್ಣಾಂಕ flags, ext2_file_t *ret);
ಬಾಹ್ಯ ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ರಚನೆ ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
ಬಾಹ್ಯ ext2_ino_t ext2fs_file_get_inode_ಅಂಕೆ(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_close(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_flush(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_read(ext2_file_t file, ಶೂನ್ಯ *buf,
				  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ wanted, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *got);
ಬಾಹ್ಯ errcode_t ext2fs_file_write(ext2_file_t file, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf,
				   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nbytes, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *written);
ಬಾಹ್ಯ errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 ಸ್ಥಾನಾಂತರ,
				   ಪೂರ್ಣಾಂಕ whence, __u64 *ret_pos);
ಬಾಹ್ಯ errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t ಸ್ಥಾನಾಂತರ,
				   ಪೂರ್ಣಾಂಕ whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ಗಾತ್ರ);
ಬಾಹ್ಯ ext2_off_t ext2fs_file_get_ಗಾತ್ರ(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_ಗಣ_ಗಾತ್ರ(ext2_file_t file, ext2_off_t ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_file_ಗಣ_size2(ext2_file_t file, ext2_off64_t ಗಾತ್ರ_2);

/* finddev.c */
ಬಾಹ್ಯ ಅಕ್ಷರ *ext2fs_ಕಂಡುಹಿಡಿ_ಖಂಡ_device(dev_t device);

/* flushb.c */
ಬಾಹ್ಯ errcode_t ext2fs_sync_device(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flushb);

/* freefs.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_dblist(ext2_dblist dblist);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_free(ext2_badblocks_list bb);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 ಆರಂಭ, __u32 ಕೊನೆ,
					    __u32 real_ಕೊನೆ,
					    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr, ಅಕ್ಷರ *init_map,
					    ext2fs_generic_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_generic_bitmap(__u32 ಆರಂಭ,
						__u32 ಕೊನೆ,
						__u32 real_ಕೊನೆ,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
						ext2fs_generic_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_generic_bitmap_ಕೊನೆ(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t ಕೊನೆ,
						 ext2_ino_t *oend);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_generic_bitmap_padding(ext2fs_generic_bitmap map);
ಬಾಹ್ಯ errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_ಕೊನೆ,
					      __u32 new_real_ಕೊನೆ,
					      ext2fs_generic_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ಆರಂಭ, __u32 ಅಂಕೆ,
						 ಶೂನ್ಯ *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ಆರಂಭ, __u32 ಅಂಕೆ,
						 ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ಆರಂಭ, __u32 ಕೊನೆ,
						       __u32 *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ಆರಂಭ, __u32 ಕೊನೆ,
						       __u32 *ಹೊರಹೋಗು);

/* gen_bitmap64.c */
ಶೂನ್ಯ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ಪೂರ್ಣಾಂಕ type, __u64 ಆರಂಭ, __u64 ಕೊನೆ,
				    __u64 real_ಕೊನೆ,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
ಶೂನ್ಯ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_ಕೊನೆ(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 ಕೊನೆ, __u64 *oend);
ಶೂನ್ಯ ext2fs_ಗಣ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_ಕೊನೆ,
				     __u64 new_real_ಕೊನೆ);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು);
errcode_t ext2fs_ಗಣ_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ಖಂಡ_bitmap *bitmap);
errcode_t ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_clusters(ext2_filsys fs, blk64_t ಆರಂಭ,
				     blk64_t ಕೊನೆ, blk64_t *ಹೊರಹೋಗು);
errcode_t ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_ಖಂಡಗಳು(ext2_filsys fs, blk64_t ಆರಂಭ,
				   blk64_t ಕೊನೆ, blk64_t *ಹೊರಹೋಗು);
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_list_backups(ext2_filsys fs, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *three,
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *five, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *seven);

/* getenv.c */
ಬಾಹ್ಯ ಅಕ್ಷರ *ext2fs_ಸುರಕ್ಷಿತ_getenv(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg);

/* get_num_dirs.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_ಅಂಕೆ_dirs(ext2_filsys fs, ext2_ino_t *ret_ಅಂಕೆ_dirs);

/* getsize.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_device_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
					blk_t *retblocks);
ಬಾಹ್ಯ errcode_t ext2fs_get_device_size2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_get_dio_alignment(ಪೂರ್ಣಾಂಕ fd);
errcode_t ext2fs_get_device_sectsize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_ಖಂಡಗಳು);
errcode_t ext2fs_iblk_sub_ಖಂಡಗಳು(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_ಖಂಡಗಳು);
errcode_t ext2fs_iblk_ಗಣ(ext2_filsys fs, ರಚನೆ ext2_inode *inode, blk64_t b);

/* imager.c */
ಬಾಹ್ಯ errcode_t ext2fs_image_inode_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_inode_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_super_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_super_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_ಖಂಡ(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_write_ind_ಖಂಡ(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf);

/* initialize.c */
ಬಾಹ್ಯ errcode_t ext2fs_initialize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags,
				   ರಚನೆ ext2_super_ಖಂಡ *param,
				   io_manager manager, ext2_filsys *ret_fs);
ಬಾಹ್ಯ errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ಪೂರ್ಣಾಂಕ super_only);

/* icount.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_icount(ext2_icount_t icount);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, ಅಕ್ಷರ *tdb_dir,
					  ಪೂರ್ಣಾಂಕ flags, ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
				       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2,
				       ext2_icount_t hint, ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
				      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2,
				      ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 ಎಣಿಕೆ_2);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_icount_ಗಾತ್ರ(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

ಬಾಹ್ಯ errcode_t ext2fs_get_memalign(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ align, ಶೂನ್ಯ *ptr);

/* inline_data.c */
ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_init(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ಗಾತ್ರ(ext2_filsys fs, ext2_ino_t ino,
					 size_t *ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_get(ext2_filsys fs, ext2_ino_t ino,
					ರಚನೆ ext2_inode *inode,
					ಶೂನ್ಯ *buf, size_t *ಗಾತ್ರ_2);
ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ಗಣ(ext2_filsys fs, ext2_ino_t ino,
					ರಚನೆ ext2_inode *inode,
					ಶೂನ್ಯ *buf, size_t ಗಾತ್ರ_2);

/* inode.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cache_ಗಾತ್ರ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_inode_cache(ರಚನೆ ext2_inode_cache *icache);
ಬಾಹ್ಯ errcode_t ext2fs_flush_icache(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_get_ಮುಂದಿನ_inode_ತುಂಬಿದ(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
					    ext2_ino_t *ino,
					    ರಚನೆ ext2_inode *inode,
					    ಪೂರ್ಣಾಂಕ bufsize);
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_INODE_ಪರಿಶೀಲನೆ_DEFAULT_ಬಫರ್_ಖಂಡಗಳು	8
ಬಾಹ್ಯ errcode_t ext2fs_open_inode_ಪರಿಶೀಲನೆ(ext2_filsys fs, ಪೂರ್ಣಾಂಕ ಬಫರ್_ಖಂಡಗಳು,
				  ext2_inode_ಪರಿಶೀಲನೆ *ret_ಪರಿಶೀಲನೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_close_inode_ಪರಿಶೀಲನೆ(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ);
ಬಾಹ್ಯ errcode_t ext2fs_get_ಮುಂದಿನ_inode(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, ext2_ino_t *ino,
			       ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ errcode_t ext2fs_inode_ಪರಿಶೀಲನೆ_goto_blockgroup(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
						   ಪೂರ್ಣಾಂಕ	ಗುಂಪು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_inode_callback
	(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
	 errcode_t (*ಮುಗಿದ_ಗುಂಪು)(ext2_filsys fs,
				 ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
				 dgrp_t ಗುಂಪು,
				 ಶೂನ್ಯ * priv_ದತ್ತಾಂಶ),
	 ಶೂನ್ಯ *ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inode_ಪರಿಶೀಲನೆ_flags(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, ಪೂರ್ಣಾಂಕ ಗಣ_flags,
				   ಪೂರ್ಣಾಂಕ clear_flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode_ತುಂಬಿದ(ext2_filsys fs, ext2_ino_t ino,
					ರಚನೆ ext2_inode * inode,
					ಪೂರ್ಣಾಂಕ bufsize);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ರಚನೆ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ರಚನೆ ext2_inode * inode,
				    ಪೂರ್ಣಾಂಕ bufsize, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode_ತುಂಬಿದ(ext2_filsys fs, ext2_ino_t ino,
					 ರಚನೆ ext2_inode * inode,
					 ಪೂರ್ಣಾಂಕ bufsize);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ರಚನೆ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ರಚನೆ ext2_inode * inode,
				     ಪೂರ್ಣಾಂಕ bufsize, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ರಚನೆ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_get_ಖಂಡಗಳು(ext2_filsys fs, ext2_ino_t ino, blk_t *ಖಂಡಗಳು);
ಬಾಹ್ಯ errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
ಬಾಹ್ಯ io_manager inode_io_manager;
ಬಾಹ್ಯ errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					ಅಕ್ಷರ **name);
ಬಾಹ್ಯ errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ರಚನೆ ext2_inode *inode,
					 ಅಕ್ಷರ **name);

/* ismounted.c */
ಬಾಹ್ಯ errcode_t ext2fs_check_if_mounted(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *mount_flags);
ಬಾಹ್ಯ errcode_t ext2fs_check_mount_ಬಿಂದು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *device, ಪೂರ್ಣಾಂಕ *mount_flags,
					  ಅಕ್ಷರ *mtpt, ಪೂರ್ಣಾಂಕ mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
ಬಾಹ್ಯ errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ರಚನೆ ext2_inode *inode,
			      ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t ಆರಂಭ,
			      blk64_t ಕೊನೆ);

/* namei.c */
ಬಾಹ್ಯ errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			 ಪೂರ್ಣಾಂಕ namelen, ಅಕ್ಷರ *buf, ext2_ino_t *inode);
ಬಾಹ್ಯ errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t *inode);
ಬಾಹ್ಯ errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
ಪೂರ್ಣಾಂಕ ext2fs_native_flag(ಶೂನ್ಯ);

/* newdir.c */
ಬಾಹ್ಯ errcode_t ext2fs_new_dir_ಖಂಡ(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ಪೋಷಕ_ino, ಅಕ್ಷರ **ಖಂಡ);
ಬಾಹ್ಯ errcode_t ext2fs_new_dir_inline_ದತ್ತಾಂಶ(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ಪೋಷಕ_ino, __u32 *iblock);

/* nls_utf8.c */
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ext2fs_ತುಂಬು_nls_ಕೋಷ್ಟಕ(ಪೂರ್ಣಾಂಕ encoding);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_check_encoded_name(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
				     ಅಕ್ಷರ *s, size_t len, ಅಕ್ಷರ **pos);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_casefold_cmp(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			       ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str1, size_t len1,
			       ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str2, size_t len2);

/* mkdir.c */
ಬಾಹ್ಯ errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t ಪೋಷಕ, ext2_ino_t inum,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
ಬಾಹ್ಯ errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t ಪೋಷಕ,
			       ext2_ino_t ino, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags,
			       ಪೂರ್ಣಾಂಕ link_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ರಚನೆ ext2fs_journal_params {
	blk_t ಅಂಕೆ_journal_ಖಂಡಗಳು;
	blk_t ಅಂಕೆ_fc_ಖಂಡಗಳು;
};
ಬಾಹ್ಯ errcode_t ext2fs_get_journal_params(
		ರಚನೆ ext2fs_journal_params *params, ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_zero_ಖಂಡಗಳು(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
				    blk_t *ret_blk, ಪೂರ್ಣಾಂಕ *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
				     blk64_t *ret_blk, ಪೂರ್ಣಾಂಕ *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 ಅಂಕೆ_ಖಂಡಗಳು, ಪೂರ್ಣಾಂಕ flags,
						  ಅಕ್ಷರ  **ret_jsb);
ಬಾಹ್ಯ errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ರಚನೆ ext2fs_journal_params *params,
						  ಪೂರ್ಣಾಂಕ flags, ಅಕ್ಷರ  **ret_jsb);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_journal_inode(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು,
					  ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_journal_inode2(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು,
					   blk64_t goal, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_ಕೂಡಿಸು_journal_inode3(ext2_filsys fs,
				    ರಚನೆ ext2fs_journal_params *params,
				    blk64_t goal, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_default_journal_ಗಾತ್ರ(__u64 ಅಂಕೆ_ಖಂಡಗಳು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_journal_sb_ಆರಂಭ(ಪೂರ್ಣಾಂಕ blocksize);

/* openfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ superblock,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ, io_manager manager,
			     ext2_filsys *ret_fs);
ಬಾಹ್ಯ errcode_t ext2fs_open2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *io_options,
			      ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ superblock,
			      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
ಬಾಹ್ಯ blk64_t ext2fs_descriptor_ಖಂಡ_loc2(ext2_filsys fs,
					blk64_t ಗುಂಪು_ಖಂಡ, dgrp_t i);
ಬಾಹ್ಯ blk_t ext2fs_descriptor_ಖಂಡ_loc(ext2_filsys fs, blk_t ಗುಂಪು_ಖಂಡ,
					 dgrp_t i);
errcode_t ext2fs_get_ದತ್ತಾಂಶ_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_ಗಣ_ದತ್ತಾಂಶ_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_ಗಮ್ಯ_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು);
ಬಾಹ್ಯ errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
ಬಾಹ್ಯ e2_blkcnt_t ext2fs_default_orphan_file_ಖಂಡಗಳು(ext2_filsys fs);
ಬಾಹ್ಯ __u32 ext2fs_do_orphan_file_ಖಂಡ_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      ಅಕ್ಷರ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_orphan_file_ಖಂಡ_csum_ಗಣ(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   ಅಕ್ಷರ *buf);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_orphan_file_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, ಅಕ್ಷರ *buf);

/* get_pathname.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       ಅಕ್ಷರ **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
		      ext2_ino_t ino, ಪೂರ್ಣಾಂಕ flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			ext2_ino_t ino, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ ext2fs_dir_is_dx(ext2_filsys fs, ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *inode);
ಶೂನ್ಯ ext2fs_inc_nlink(ext2_filsys fs, ರಚನೆ ext2_inode *inode);
ಶೂನ್ಯ ext2fs_dec_nlink(ರಚನೆ ext2_inode *inode);
ಪೂರ್ಣಾಂಕ ext2fs_dir_link_max(ext2_filsys fs, ರಚನೆ ext2_inode_large *inode);
ಪೂರ್ಣಾಂಕ ext2fs_dir_link_ಖಾಲಿಗಳು(ರಚನೆ ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ಪೋಷಕ, ext2_ino_t ino,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ);
ಪೂರ್ಣಾಂಕ ext2fs_is_fast_symlink(ರಚನೆ ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_ಆರಂಭ(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
ಚಿಹ್ನೆರಹಿತ ext2fs_mmp_new_seq(ಶೂನ್ಯ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ಶೂನ್ಯ **ptr);

/* read_bb.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ,
				      ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      ಅಕ್ಷರ *badstr,
						      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ));
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
ಬಾಹ್ಯ errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು);
ಬಾಹ್ಯ errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_read_ಖಂಡ_bitmap(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_ಖಂಡ_bitmap (ext2_filsys fs);

/*sha256.c */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_SHA256_ಉದ್ದಳತೆ 32
#ಆದರೆ 0
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_sha256(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಒಳ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಳ_ಗಾತ್ರ,
		   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಹೊರಹೋಗು[EXT2_FS_SHA256_ಉದ್ದಳತೆ]);
#ಆದರೆ_ಅಂತ್ಯ

/* sha512.c */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_SHA512_ಉದ್ದಳತೆ 64
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_sha512(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಒಳ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಳ_ಗಾತ್ರ,
			  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಹೊರಹೋಗು[EXT2_FS_SHA512_ಉದ್ದಳತೆ]);

/* swapfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2,
					ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_ಒಳ(ext2_filsys fs, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2,
					 ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_ಹೊರಹೋಗು(ext2_filsys fs, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr(ಅಕ್ಷರ *ಗಮ್ಯ, ಅಕ್ಷರ *from, ಪೂರ್ಣಾಂಕ bufsize,
				 ಪೂರ್ಣಾಂಕ has_ಶೀರ್ಷಿಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr_ಶೀರ್ಷಿಕೆ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ,
					ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *from_hdr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr_ನಮೂದು(ರಚನೆ ext2_ext_attr_ನಮೂದು *ಗಮ್ಯ_ನಮೂದು,
				       ರಚನೆ ext2_ext_attr_ನಮೂದು *from_ನಮೂದು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_super(ರಚನೆ ext2_super_ಖಂಡ * super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc(ರಚನೆ ext2_ಗುಂಪು_desc *gdp);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(ext2_filsys, ರಚನೆ ext2_ಗುಂಪು_desc *gdp);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(ext2_filsys fs, ರಚನೆ ext2_inode_large *t,
				   ರಚನೆ ext2_inode_large *f, ಪೂರ್ಣಾಂಕ hostorder,
				   ಪೂರ್ಣಾಂಕ bufsize);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_inode(ext2_filsys fs,ರಚನೆ ext2_inode *t,
			      ರಚನೆ ext2_inode *f, ಪೂರ್ಣಾಂಕ hostorder);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_mmp(ರಚನೆ mmp_struct *mmp);

/* unix_io.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_open_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಪೂರ್ಣಾಂಕ flags, mode_t ಬಹುಲಕ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_stat(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ext2fs_struct_stat *buf);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fstat(ಪೂರ್ಣಾಂಕ fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inode_has_valid_ಖಂಡಗಳು(ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ರಚನೆ ext2_inode *inode);

/* version.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_parse_ಆವೃತ್ತಿ_string(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ver_string);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_get_library_ಆವೃತ್ತಿ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ver_string,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **date_string);

/* write_bb_file.c */
ಬಾಹ್ಯ errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags,
				      FILE *f);


/* inline functions */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_INLINE_FUNCS
ಬಾಹ್ಯ errcode_t ext2fs_get_mem(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_memzero(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_array(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2,
				  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_arrayzero(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_free_mem(ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_resize_mem(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ old_ಗಾತ್ರ,
				   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_resize_array(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ old_ಎಣಿಕೆ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_super_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_ಬದಲಾಯಿತು(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಬದಲಾಯಿತು(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_unmark_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_ib_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_bb_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ib_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_bb_dirty(ext2_filsys fs);
ಬಾಹ್ಯ dgrp_t ext2fs_ಗುಂಪು_ದ_blk(ext2_filsys fs, blk_t blk);
ಬಾಹ್ಯ dgrp_t ext2fs_ಗುಂಪು_ದ_ino(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ blk_t ext2fs_ಗುಂಪು_ಮೊದಲ_ಖಂಡ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk_t ext2fs_ಗುಂಪು_ಕೊನೆಯ_ಖಂಡ(ext2_filsys fs, dgrp_t ಗುಂಪು);
ಬಾಹ್ಯ blk_t ext2fs_inode_ದತ್ತಾಂಶ_ಖಂಡಗಳು(ext2_filsys fs,
				      ರಚನೆ ext2_inode *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ಪೂರ್ಣಾಂಕ ಖಂಡಗಳು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_log2_u32(__u32 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_log2_u64(__u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_log10_u32(__u32 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_log10_u64(__u64 arg);
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_div_ceil(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ a, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ b);
ಬಾಹ್ಯ __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dirent_name_len(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dirent_ಗಣ_name_len(ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು, ಪೂರ್ಣಾಂಕ len);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_dirent_file_type(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dirent_ಗಣ_file_type(ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು, ಪೂರ್ಣಾಂಕ type);
ಬಾಹ್ಯ ರಚನೆ ext2_inode *ext2fs_inode(ರಚನೆ ext2_inode_large * large_inode);
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *ext2fs_const_inode(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode_large * large_inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inodes_ಪ್ರತಿ_orphan_ಖಂಡ(ext2_filsys fs);
ಬಾಹ್ಯ ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *ext2fs_orphan_ಖಂಡ_ಬಾಲ(ext2_filsys fs,
							       ಅಕ್ಷರ *buf);
#ಆದರೆ_ಅಂತ್ಯ

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#ಆದರೆ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INCLUDE_INLINE_FUNCS
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ (__STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಸಾಲೊಳಗೆ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ __inline__
#ಇಲ್ಲದಿದ್ದರೆ				/* For Watcom C */
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ /* __GNUC__ */
#ಆದರೆ_ಅಂತ್ಯ /* __STDC_VERSION__ >= 199901L */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ EXT2_CUSTOM_ಸ್ಮರಣೆ_ROUTINES
#ಅನ್ತರ್ಭಾವ <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *pp;

	pp = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
	ಆದರೆ (!pp)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	ಸ್ಮರಣೆ_ನಕಲು(ptr, &pp, ಗಾತ್ರ (pp));
	ಹಿಂತಿರುಗಿ 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *pp;

	pp = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
	ಆದರೆ (!pp)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	memset(pp, 0, ಗಾತ್ರ_2);
	ಸ್ಮರಣೆ_ನಕಲು(ptr, &pp, ಗಾತ್ರ(pp));
	ಹಿಂತಿರುಗಿ 0;
}

_INLINE_ errcode_t ext2fs_get_array(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
				    ಶೂನ್ಯ *ptr)
{
	ಆದರೆ (ಎಣಿಕೆ_2 && (~0UL)/ಎಣಿಕೆ_2 < ಗಾತ್ರ_2)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	ಹಿಂತಿರುಗಿ ext2fs_get_mem(ಎಣಿಕೆ_2*ಗಾತ್ರ_2, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2,
					ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr)
{
	ಆದರೆ (ಎಣಿಕೆ_2 && (~0UL)/ಎಣಿಕೆ_2 < ಗಾತ್ರ_2)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	ಹಿಂತಿರುಗಿ ext2fs_get_memzero((size_t)ಎಣಿಕೆ_2 * ಗಾತ್ರ_2, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *p;

	ಸ್ಮರಣೆ_ನಕಲು(&p, ptr, ಗಾತ್ರ(p));
	ಬಿಡುಗಡೆ(p);
	p = 0;
	ಸ್ಮರಣೆ_ನಕಲು(ptr, &p, ಗಾತ್ರ(p));
	ಹಿಂತಿರುಗಿ 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ EXT2FS_ATTR((ಬಳಸದ)) old_ಗಾತ್ರ,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	ಸ್ಮರಣೆ_ನಕಲು(&p, ptr, ಗಾತ್ರ(p));
	p = realloc(p, ಗಾತ್ರ_2);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	ಸ್ಮರಣೆ_ನಕಲು(ptr, &p, ಗಾತ್ರ(p));
	ಹಿಂತಿರುಗಿ 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
				       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ old_ಎಣಿಕೆ,
				       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ptr)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ old_ಗಾತ್ರ;
	errcode_t retval;

	ಆದರೆ (ಎಣಿಕೆ_2 && (~0UL)/ಎಣಿಕೆ_2 < ಗಾತ್ರ_2)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	ಗಾತ್ರ_2 *= ಎಣಿಕೆ_2;
	old_ಗಾತ್ರ = ಗಾತ್ರ_2 * old_ಎಣಿಕೆ;
	retval = ext2fs_resize_mem(old_ಗಾತ್ರ, ಗಾತ್ರ_2, ptr);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (ಗಾತ್ರ_2 > old_ಗಾತ್ರ) {
		ಶೂನ್ಯ *p;

		ಸ್ಮರಣೆ_ನಕಲು(&p, ptr, ಗಾತ್ರ(p));
		memset((ಅಕ್ಷರ *)p + old_ಗಾತ್ರ, 0, ಗಾತ್ರ_2 - old_ಗಾತ್ರ);
		ಸ್ಮರಣೆ_ನಕಲು(ptr, &p, ಗಾತ್ರ(p));
	}

	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_ಬದಲಾಯಿತು;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_ಬದಲಾಯಿತು(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_ಬದಲಾಯಿತು;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_ಬದಲಾಯಿತು(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ (fs->flags & EXT2_FLAG_ಬದಲಾಯಿತು);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ ಶೂನ್ಯ ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_valid(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_ಬದಲಾಯಿತು;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_ಬದಲಾಯಿತು;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_ib_dirty(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_bb_dirty(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_ಗುಂಪು_ದ_blk(ext2_filsys fs, blk_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_ಗುಂಪು_ದ_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_ಗುಂಪು_ದ_ino(ext2_filsys fs, ext2_ino_t ino)
{
	ಹಿಂತಿರುಗಿ (ino - 1) / fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_ಗುಂಪು_ಮೊದಲ_ಖಂಡ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ (blk_t) ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_ಗುಂಪು_ಕೊನೆಯ_ಖಂಡ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ (blk_t) ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಗುಂಪು);
}

_INLINE_ blk_t ext2fs_inode_ದತ್ತಾಂಶ_ಖಂಡಗಳು(ext2_filsys fs,
					ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ (blk_t) ext2fs_inode_ದತ್ತಾಂಶ_blocks2(fs, inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ಪೂರ್ಣಾಂಕ ಖಂಡಗಳು)
{
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;

	ಆದರೆ ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dx_ಬಾಲ);
	ಹಿಂತಿರುಗಿ ಖಂಡಗಳು * ((fs->blocksize - (8 + csum_ಗಾತ್ರ)) /
						ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));
}

/*
 * log base 2, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_log2_u32(__u32 arg)
{
	ಪೂರ್ಣಾಂಕ l = 0;

	arg >>= 1;
	ಆಗ (arg) {
		l++;
		arg >>= 1;
	}
	ಹಿಂತಿರುಗಿ l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_log2_u64(__u64 arg)
{
	ಪೂರ್ಣಾಂಕ l = 0;

	arg >>= 1;
	ಆಗ (arg) {
		l++;
		arg >>= 1;
	}
	ಹಿಂತಿರುಗಿ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_log10_u32(__u32 arg)
{
	ಪೂರ್ಣಾಂಕ l = 0;

	arg /= 10;
	ಆಗ (arg) {
		l++;
		arg /= 10;
	}
	ಹಿಂತಿರುಗಿ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_log10_u64(__u64 arg)
{
	ಪೂರ್ಣಾಂಕ l = 0;

	arg /= 10;
	ಆಗ (arg) {
		l++;
		arg /= 10;
	}
	ಹಿಂತಿರುಗಿ l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_div_ceil(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ a, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ b)
{
	ಆದರೆ (!a)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	ಆದರೆ (!a)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ((a - 1) / b) + 1;
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_dirent_name_len(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು)
{
	ಹಿಂತಿರುಗಿ ನಮೂದು->name_len & 0xff;
}

_INLINE_ ಶೂನ್ಯ ext2fs_dirent_ಗಣ_name_len(ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು, ಪೂರ್ಣಾಂಕ len)
{
	ನಮೂದು->name_len = (ನಮೂದು->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_dirent_file_type(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು)
{
	ಹಿಂತಿರುಗಿ ನಮೂದು->name_len >> 8;
}

_INLINE_ ಶೂನ್ಯ ext2fs_dirent_ಗಣ_file_type(ರಚನೆ ext2_dir_ನಮೂದು *ನಮೂದು, ಪೂರ್ಣಾಂಕ type)
{
	ನಮೂದು->name_len = (ನಮೂದು->name_len & 0xff) | (type << 8);
}

_INLINE_ ರಚನೆ ext2_inode *ext2fs_inode(ರಚನೆ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext2_inode *) large_inode;
}

_INLINE_ ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *
ext2fs_const_inode(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ಹಿಂತಿರುಗಿ (ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *) large_inode;
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_inodes_ಪ್ರತಿ_orphan_ಖಂಡ(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ (fs->blocksize - ಗಾತ್ರ(ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ)) /
		ಗಾತ್ರ(__u32);
}

_INLINE_ ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *
ext2fs_orphan_ಖಂಡ_ಬಾಲ(ext2_filsys fs, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *)(buf + fs->blocksize -
		ಗಾತ್ರ(ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ));
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು _INLINE_
#ಆದರೆ_ಅಂತ್ಯ

/* htree levels for ext4 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_HTREE_LEVEL_COMPAT 2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_HTREE_LEVEL	3

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2_dir_htree_level(ext2_filsys fs)
{
	ಆದರೆ (ext2fs_has_feature_largedir(fs->super))
		ಹಿಂತಿರುಗಿ EXT4_HTREE_LEVEL;

	ಹಿಂತಿರುಗಿ EXT4_HTREE_LEVEL_COMPAT;
}

/*
 * We explicitly decided not to reserve space for a 64-bit dtime,
 * since it's never displayed or exposed to userspace.  The dtime
 * field is used a linked list for the ophan list, and for forensic
 * purposes when trying to determine when an inode was deleted.  So
 * right after the 2038 epoch, a deleted inode might end up with a
 * dtime which is zero or smaller than the number of inodes, which
 * will result in e2fsck reporting a potential problems.  So when we
 * set the dtime, make sure that the dtime won't be mistaken for an
 * inode number.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_ಗಣ_dtime(ext2_filsys fs, ರಚನೆ ext2_inode *inode)
{
	__u32	t;

	ಆದರೆ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_ಸಮಯ))
		t = fs->now & 0xFFFFFFFF;
	ಇಲ್ಲದಿದ್ದರೆ
		t = time(ಖಾಲಿ) & 0xFFFFFFFF;
	ಆದರೆ (t < fs->super->s_inodes_ಎಣಿಕೆ)
		t = fs->super->s_inodes_ಎಣಿಕೆ;
	inode->i_dtime = t;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* _EXT2FS_EXT2FS_H */
