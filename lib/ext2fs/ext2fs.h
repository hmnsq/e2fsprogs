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

#నిర్వచితం_కాకపోతే _EXT2FS_EXT2FS_H
#నిర్వచించు _EXT2FS_EXT2FS_H

#నిర్వచితమైతే __GNUC__
#నిర్వచించు EXT2FS_ATTR(x) __attribute__(x)
#లేకపోతే
#నిర్వచించు EXT2FS_ATTR(x)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే __nonstring
#నిర్వచితమైతే __has_attribute
#ఒకవేళ __has_attribute(__nonstring__)
#నిర్వచించు __nonstring                    __attribute__((__nonstring__))
#లేకపోతే
#నిర్వచించు __nonstring
#అయితే_ముగింపు /* __has_attribute(__nonstring__) */
#లేకపోతే
# define __nonstring
#అయితే_ముగింపు /* __has_attribute */
#అయితే_ముగింపు /* __nonstring */

#నిర్వచితమైతే CONFIG_TDB
#నిర్వచించు EXT2_FS_NO_TDB_వాడని
#లేకపోతే
#నిర్వచించు EXT2_FS_NO_TDB_వాడని	EXT2FS_ATTR((వాడని))
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#ఒకవేళ (!defined(__GNUC__) && !defined(__WATCOMC__))
#నిర్వచించు NO_INLINE_FUNCS
#అయితే_ముగింపు

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#నిర్వచించు SUPERBLOCK_స్థానభ్రంశం	1024
#నిర్వచించు SUPERBLOCK_కొలత		1024

#నిర్వచించు UUID_STR_కొలత 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#నిర్వచించు EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#నిర్వచితమైతే HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <errno.h>

#ఒకవేళ EXT2_FLAT_INCLUDES
#అన్తర్భావ "e2_types.h"
#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext3_extents.h"
#లేకపోతే
#అన్తర్భావ <ext2fs/ext2_types.h>
#అన్తర్భావ <ext2fs/ext2_fs.h>
#అన్తర్భావ <ext2fs/ext3_extents.h>
#అయితే_ముగింపు /* EXT2_FLAT_INCLUDES */

రకనిర్వచనం __u32 __bitwise		ext2_ino_t;
రకనిర్వచనం __u32 __bitwise		blk_t;
రకనిర్వచనం __u64 __bitwise		blk64_t;
రకనిర్వచనం __u32 __bitwise		dgrp_t;
రకనిర్వచనం __s32 __bitwise		ext2_off_t;
రకనిర్వచనం __s64 __bitwise		ext2_off64_t;
రకనిర్వచనం __s64 __bitwise		e2_blkcnt_t;
రకనిర్వచనం __u32 __bitwise		ext2_dirhash_t;

#ఒకవేళ EXT2_FLAT_INCLUDES
#అన్తర్భావ "com_err.h"
#అన్తర్భావ "ext2_io.h"
#అన్తర్భావ "ext2_err.h"
#అన్తర్భావ "ext2_ext_attr.h"
#లేకపోతే
#అన్తర్భావ <et/com_err.h>
#అన్తర్భావ <ext2fs/ext2_io.h>
#అన్తర్భావ <ext2fs/ext2_err.h>
#అన్తర్భావ <ext2fs/ext2_ext_attr.h>
#అయితే_ముగింపు

#అన్తర్భావ "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#నిర్వచితమైతే _MSC_VER
#నిర్వచించు EXT2_QSORT_TYPE పూర్ణాంకం __cdecl
#లేకపోతే
#నిర్వచించు EXT2_QSORT_TYPE పూర్ణాంకం
#అయితే_ముగింపు

రకనిర్వచనం నిర్మాణం struct_ext2_filsys *ext2_filsys;

#నిర్వచించు EXT2FS_MARK_ERROR 	0
#నిర్వచించు EXT2FS_UNMARK_ERROR 	1
#నిర్వచించు EXT2FS_TEST_ERROR	2

నిర్మాణం ext2fs_struct_generic_bitmap_పునాది {
	errcode_t		magic;
	ext2_filsys 		fs;
};

రకనిర్వచనం నిర్మాణం ext2fs_struct_generic_bitmap_పునాది *ext2fs_generic_bitmap;
రకనిర్వచనం నిర్మాణం ext2fs_struct_generic_bitmap_పునాది *ext2fs_inode_bitmap;
రకనిర్వచనం నిర్మాణం ext2fs_struct_generic_bitmap_పునాది *ext2fs_ఖండం_bitmap;

#నిర్వచించు EXT2_మొదటి_INODE(s)	EXT2_మొదటి_INO(s)


/*
 * Badblocks list definitions
 */

రకనిర్వచనం నిర్మాణం ext2_struct_u32_list *ext2_badblocks_list;
రకనిర్వచనం నిర్మాణం ext2_struct_u32_iterate *ext2_badblocks_iterate;

రకనిర్వచనం నిర్మాణం ext2_struct_u32_list *ext2_u32_list;
రకనిర్వచనం నిర్మాణం ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
రకనిర్వచనం నిర్మాణం ext2_struct_u32_list *badblocks_list;
రకనిర్వచనం నిర్మాణం ext2_struct_u32_iterate *badblocks_iterate;

#నిర్వచించు BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
నిర్మాణం ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
నిర్మాణం ext2_db_నమోదు {
	ext2_ino_t	ino;
	blk_t	blk;
	పూర్ణాంకం	blockcnt;
};

రకనిర్వచనం నిర్మాణం ext2_struct_dblist *ext2_dblist;

#నిర్వచించు DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#నిర్వచించు EXT2_FILE_WRITE		0x0001
#నిర్వచించు EXT2_FILE_CREATE	0x0002

#నిర్వచించు EXT2_FILE_ముసుగు		0x00FF

#నిర్వచించు EXT2_FILE_BUF_DIRTY	0x4000
#నిర్వచించు EXT2_FILE_BUF_VALID	0x2000

రకనిర్వచనం నిర్మాణం ext2_file *ext2_file_t;

#నిర్వచించు EXT2_SEEK_సమితి	0
#నిర్వచించు EXT2_SEEK_CUR	1
#నిర్వచించు EXT2_SEEK_చివర	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#నిర్వచించు EXT2_FLAG_RW			0x01
#నిర్వచించు EXT2_FLAG_మారింది		0x02
#నిర్వచించు EXT2_FLAG_DIRTY			0x04
#నిర్వచించు EXT2_FLAG_VALID			0x08
#నిర్వచించు EXT2_FLAG_IB_DIRTY		0x10
#నిర్వచించు EXT2_FLAG_BB_DIRTY		0x20
#నిర్వచించు EXT2_FLAG_మార్పిడి_బైట్లు		0x40
#నిర్వచించు EXT2_FLAG_మార్పిడి_బైట్లు_READ	0x80
#నిర్వచించు EXT2_FLAG_మార్పిడి_బైట్లు_WRITE	0x100
#నిర్వచించు EXT2_FLAG_MASTER_SB_ONLY	0x200
#నిర్వచించు EXT2_FLAG_FORCE			0x400
#నిర్వచించు EXT2_FLAG_SUPER_ONLY		0x800
#నిర్వచించు EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#నిర్వచించు EXT2_FLAG_IMAGE_FILE		0x2000
#నిర్వచించు EXT2_FLAG_EXCLUSIVE		0x4000
#నిర్వచించు EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#నిర్వచించు EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#నిర్వచించు EXT2_FLAG_64_బిట్లు		0x20000
#నిర్వచించు EXT2_FLAG_PRINT_పురోగతి	0x40000
#నిర్వచించు EXT2_FLAG_ప్రత్యక్ష_IO		0x80000
#నిర్వచించు EXT2_FLAG_SKIP_MMP		0x100000
#నిర్వచించు EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#నిర్వచించు EXT2_FLAG_SHARE_DUP		0x400000
#నిర్వచించు EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#నిర్వచించు EXT2_FLAG_BBITMAP_తోక_PROBLEM	0x1000000
#నిర్వచించు EXT2_FLAG_IBITMAP_తోక_PROBLEM	0x2000000
#నిర్వచించు EXT2_FLAG_దారాలు		0x4000000
#నిర్వచించు EXT2_FLAG_IGNORE_మార్పిడి_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#నిర్వచించు EXT2_FLAG2_USE_FAKE_సమయం	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#నిర్వచించు EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#నిర్వచించు EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#నిర్వచించు EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#నిర్వచించు EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#నిర్వచించు EXT2_JOURNAL_గమ్యం_FC_BLKS_నిష్పత్తి		64

నిర్మాణం blk_alloc_ctx;
నిర్మాణం opaque_ext2_సమూహం_desc;

నిర్మాణం struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	పూర్ణాంకం				flags;
	అక్షరం *				device_name;
	నిర్మాణం ext2_super_ఖండం	* 	super;
	గుర్తులేని పూర్ణాంకం			blocksize;
	పూర్ణాంకం				fragsize;
	dgrp_t				సమూహం_desc_లెక్క;
	గుర్తులేని దీర్ఘ			desc_ఖండాలు;
	నిర్మాణం opaque_ext2_సమూహం_desc *	సమూహం_desc;
	గుర్తులేని పూర్ణాంకం			inode_ఖండాలు_చొప్పున_సమూహం;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_ఖండం_bitmap		ఖండం_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_ఖండాలు)(ext2_filsys fs, ext2_ino_t ino, blk_t *ఖండాలు);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				నిర్మాణం ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				నిర్మాణం ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	నిర్మాణం ext2_super_ఖండం *	orig_super;
	నిర్మాణం ext2_image_hdr *		image_శీర్షిక;
	__u32				umask;
	time_t				now;
	పూర్ణాంకం				cluster_నిష్పత్తి_బిట్లు;
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
	శూన్య *				priv_దత్తాంశం;

	/*
	 * Inode cache
	 */
	నిర్మాణం ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_ఖండం)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, నిర్మాణం blk_alloc_ctx *ctx);
	శూన్య (*ఖండం_alloc_stats)(ext2_filsys fs, blk64_t blk, పూర్ణాంకం inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	శూన్య *mmp_buf;
	శూన్య *mmp_cmp;
	పూర్ణాంకం mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	దీర్ఘ mmp_చివరి_written;

	/* progress operation functions */
	నిర్మాణం ext2fs_పురోగతి_ops *పురోగతి_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_విత్తనం;

	io_channel			journal_io;
	అక్షరం				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	శూన్య (*ఖండం_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t అంకె,
					పూర్ణాంకం inuse);

	/* hashmap for SHA of data blocks */
	నిర్మాణం ext2fs_hashmap* ఖండం_sha_map;

	స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *encoding;
};

#ఒకవేళ EXT2_FLAT_INCLUDES
#అన్తర్భావ "e2_bitops.h"
#లేకపోతే
#అన్తర్భావ <ext2fs/bitops.h>
#అయితే_ముగింపు

/*
 * 64-bit bitmap backend types
 */
#నిర్వచించు EXT2FS_BMAP64_BITARRAY	1
#నిర్వచించు EXT2FS_BMAP64_RBTREE	2
#నిర్వచించు EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#నిర్వచించు ఖండం_మారింది			1
#నిర్వచించు ఖండం_ABORT			2
#నిర్వచించు ఖండం_ERROR			4
#నిర్వచించు ఖండం_INLINE_దత్తాంశం_మారింది	8

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
#నిర్వచించు ఖండం_FLAG_APPEND	1
#నిర్వచించు ఖండం_FLAG_HOLE		1
#నిర్వచించు ఖండం_FLAG_లోతు_TRAVERSE	2
#నిర్వచించు ఖండం_FLAG_దత్తాంశం_ONLY	4
#నిర్వచించు ఖండం_FLAG_READ_ONLY	8

#నిర్వచించు ఖండం_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#నిర్వచించు ఖండం_లెక్క_IND		(-1)
#నిర్వచించు ఖండం_లెక్క_DIND	(-2)
#నిర్వచించు ఖండం_లెక్క_TIND	(-3)
#నిర్వచించు ఖండం_లెక్క_TRANSLATOR	(-4)

#నిర్వచించు ఖండం_ALLOC_UNKNOWN	0
#నిర్వచించు ఖండం_ALLOC_దత్తాంశం	1
#నిర్వచించు ఖండం_ALLOC_METADATA	2

నిర్మాణం blk_alloc_ctx {
	ext2_ino_t		ino;
	నిర్మాణం ext2_inode	*inode;
	blk64_t			lblk;
	పూర్ణాంకం			flags;
};

#ఒకవేళ 0
/*
 * Flags for ext2fs_move_blocks
 */
#నిర్వచించు EXT2_BMOVE_GET_DBLIST	0x0001
#నిర్వచించు EXT2_BMOVE_DEBUG	0x0002
#అయితే_ముగింపు

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#నిర్వచించు EXT2_EXTENT_FLAGS_ఆకు		0x0001
#నిర్వచించు EXT2_EXTENT_FLAGS_UNINIT	0x0002
#నిర్వచించు EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు	0x0004

నిర్మాణం ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

రకనిర్వచనం నిర్మాణం ext2_extent_handle *ext2_extent_handle_t;
రకనిర్వచనం నిర్మాణం ext2_extent_మార్గం *ext2_extent_మార్గం_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#నిర్వచించు EXT2_EXTENT_CURRENT	0x0000
#నిర్వచించు EXT2_EXTENT_MOVE_ముసుగు	0x000F
#నిర్వచించు EXT2_EXTENT_మూలం	0x0001
#నిర్వచించు EXT2_EXTENT_చివరి_ఆకు	0x0002
#నిర్వచించు EXT2_EXTENT_మొదటి_SIB	0x0003
#నిర్వచించు EXT2_EXTENT_చివరి_SIB	0x0004
#నిర్వచించు EXT2_EXTENT_తదుపరి_SIB	0x0005
#నిర్వచించు EXT2_EXTENT_PREV_SIB	0x0006
#నిర్వచించు EXT2_EXTENT_తదుపరి_ఆకు	0x0007
#నిర్వచించు EXT2_EXTENT_PREV_ఆకు	0x0008
#నిర్వచించు EXT2_EXTENT_తదుపరి	0x0009
#నిర్వచించు EXT2_EXTENT_PREV	0x000A
#నిర్వచించు EXT2_EXTENT_పైకి		0x000B
#నిర్వచించు EXT2_EXTENT_కిందికి	0x000C
#నిర్వచించు EXT2_EXTENT_కిందికి_AND_చివరి 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#నిర్వచించు EXT2_EXTENT_చొప్పించు_AFTER	0x0001 /* insert after handle loc'n */
#నిర్వచించు EXT2_EXTENT_చొప్పించు_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#నిర్వచించు EXT2_EXTENT_DELETE_KEEP_ఖాళీలు	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#నిర్వచించు EXT2_EXTENT_సమితి_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
నిర్మాణం ext2_extent_info {
	పూర్ణాంకం		curr_నమోదు;
	పూర్ణాంకం		curr_level;
	పూర్ణాంకం		అంకె_entries;
	పూర్ణాంకం		max_entries;
	పూర్ణాంకం		max_లోతు;
	పూర్ణాంకం		బైట్లు_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#నిర్వచించు EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#నిర్వచించు DIRENT_మారింది	1
#నిర్వచించు DIRENT_ABORT	2
#నిర్వచించు DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#నిర్వచించు DIRENT_FLAG_INCLUDE_ఖాళీలు	1
#నిర్వచించు DIRENT_FLAG_INCLUDE_REMOVED	2
#నిర్వచించు DIRENT_FLAG_INCLUDE_CSUM	4
#నిర్వచించు DIRENT_FLAG_INCLUDE_INLINE_దత్తాంశం 8

#నిర్వచించు DIRENT_DOT_FILE		1
#నిర్వచించు DIRENT_DOT_DOT_FILE	2
#నిర్వచించు DIRENT_OTHER_FILE	3
#నిర్వచించు DIRENT_DELETED_FILE	4
#నిర్వచించు DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
రకనిర్వచనం నిర్మాణం ext2_struct_inode_పరిశీలన *ext2_inode_పరిశీలన;

/*
 * ext2fs_scan flags
 */
#నిర్వచించు EXT2_SF_CHK_BADBLOCKS	0x0001
#నిర్వచించు EXT2_SF_BAD_INODE_BLK	0x0002
#నిర్వచించు EXT2_SF_BAD_EXTRA_బైట్లు	0x0004
#నిర్వచించు EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#నిర్వచించు EXT2_SF_DO_LAZY		0x0010
#నిర్వచించు EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#నిర్వచించు EXT2_MF_MOUNTED		1
#నిర్వచించు EXT2_MF_ISROOT		2
#నిర్వచించు EXT2_MF_READONLY	4
#నిర్వచించు EXT2_MF_మార్పిడి		8
#నిర్వచించు EXT2_MF_BUSY		16
#నిర్వచించు EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#నిర్వచించు LINUX_S_IFMT  00170000
#నిర్వచించు LINUX_S_IFSOCK 0140000
#నిర్వచించు LINUX_S_IFLNK	 0120000
#నిర్వచించు LINUX_S_IFREG  0100000
#నిర్వచించు LINUX_S_IFBLK  0060000
#నిర్వచించు LINUX_S_IFDIR  0040000
#నిర్వచించు LINUX_S_IFCHR  0020000
#నిర్వచించు LINUX_S_IFIFO  0010000
#నిర్వచించు LINUX_S_ISUID  0004000
#నిర్వచించు LINUX_S_ISGID  0002000
#నిర్వచించు LINUX_S_ISVTX  0001000

#నిర్వచించు LINUX_S_IRWXU 00700
#నిర్వచించు LINUX_S_IRUSR 00400
#నిర్వచించు LINUX_S_IWUSR 00200
#నిర్వచించు LINUX_S_IXUSR 00100

#నిర్వచించు LINUX_S_IRWXG 00070
#నిర్వచించు LINUX_S_IRGRP 00040
#నిర్వచించు LINUX_S_IWGRP 00020
#నిర్వచించు LINUX_S_IXGRP 00010

#నిర్వచించు LINUX_S_IRWXO 00007
#నిర్వచించు LINUX_S_IROTH 00004
#నిర్వచించు LINUX_S_IWOTH 00002
#నిర్వచించు LINUX_S_IXOTH 00001

#నిర్వచించు LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#నిర్వచించు LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#నిర్వచించు LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#నిర్వచించు LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#నిర్వచించు LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#నిర్వచించు LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#నిర్వచించు LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#నిర్వచించు EXT2_I_కొలత(i)	((i)->i_కొలత | ((__u64) (i)->i_కొలత_ఎగువ << 32))

స్థిర అంతఃవరుస __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((వాడని)),
					__u32 nsec)
{
	__u32 extra = 0;

#ఒకవేళ (SIZEOF_సమయం_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_ముసుగు;
#అయితే_ముగింపు
	తిరిగి extra | (nsec << EXT4_EPOCH_బిట్లు);
}
స్థిర అంతఃవరుస time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((వాడని)))
{
#ఒకవేళ (SIZEOF_సమయం_T > 4)
	ఒకవేళ (extra & EXT4_EPOCH_ముసుగు)
		seconds += ((time_t)(extra & EXT4_EPOCH_ముసుగు) << 32);
#అయితే_ముగింపు
	తిరిగి seconds;
}
స్థిర అంతఃవరుస __u32 __decode_extra_nsec(__u32 extra)
{
	తిరిగి (extra & EXT4_NSEC_ముసుగు) >> EXT4_EPOCH_బిట్లు;
}
#నిర్వచించు ext2fs_inode_actual_కొలత(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_కొలత +				      \
		  (పరిమాణం(*inode) > EXT2_GOOD_OLD_INODE_కొలత ?		      \
		   ((నిర్మాణం ext2_inode_large *)(inode))->i_extra_isize : 0)))
#నిర్వచించు clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#నిర్వచించు ext2fs_inode_xtime_సమితి(inode, క్షేత్రం, sec)			      \
చేయి {									      \
	ఒకవేళ (ext2fs_inode_includes(ext2fs_inode_actual_కొలత(inode),	      \
				  క్షేత్రం ## _extra)) {			      \
		(inode)->క్షేత్రం = (__u32)(sec & 0xfffffff);		      \
		((నిర్మాణం ext2_inode_large *)(inode))->క్షేత్రం ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} లేకపోతే {							      \
		(inode)->క్షేత్రం = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} అయితే (0)
#నిర్వచించు ext2fs_inode_xtime_get(inode, క్షేత్రం)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_కొలత(inode), క్షేత్రం ## _extra) ?    \
	__decode_extra_sec((inode)->క్షేత్రం,				      \
		((నిర్మాణం ext2_inode_large *)(inode))->క్షేత్రం ## _extra) :      \
		(time_t)(inode)->క్షేత్రం)

స్థిర అంతఃవరుస శూన్య __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((వాడని)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#ఒకవేళ (SIZEOF_సమయం_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_ముసుగు;
#లేకపోతే
	*hi = 0;
#అయితే_ముగింపు
}
స్థిర అంతఃవరుస time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((వాడని)))
{
#ఒకవేళ (SIZEOF_సమయం_T == 4)
	తిరిగి *lo;
#లేకపోతే
	తిరిగి ((time_t)(*hi) << 32) | *lo;
#అయితే_ముగింపు
}
#నిర్వచించు ext2fs_సమితి_tstamp(sb, క్షేత్రం, seconds) \
	__sb_set_tstamp(&(sb)->క్షేత్రం, &(sb)->క్షేత్రం ## _hi, seconds)
#నిర్వచించు ext2fs_get_tstamp(sb, క్షేత్రం) \
	__sb_get_tstamp(&(sb)->క్షేత్రం, &(sb)->క్షేత్రం ## _hi)

/*
 * ext2_icount_t abstraction
 */
#నిర్వచించు EXT2_ICOUNT_OPT_INCREMENT	0x01
#నిర్వచించు EXT2_ICOUNT_OPT_FULLMAP		0x02

రకనిర్వచనం నిర్మాణం ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#నిర్వచించు BMAP_ALLOC	0x0001
#నిర్వచించు BMAP_సమితి	0x0002
#నిర్వచించు BMAP_UNINIT	0x0004
#నిర్వచించు BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#నిర్వచించు BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#నిర్వచించు READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#నిర్వచించు WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#నిర్వచించు IMAGER_FLAG_INODEMAP	1
#నిర్వచించు IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

/*
 * Features supported by this version of the library
 */
#నిర్వచించు EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_సూచీ|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#నిర్వచితమైతే CONFIG_MMP
#నిర్వచించు EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#లేకపోతే
#నిర్వచించు EXT4_LIB_INCOMPAT_MMP		(0)
#అయితే_ముగింపు

#నిర్వచించు EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_బిట్టు|\
					 EXT4_FEATURE_INCOMPAT_INLINE_దత్తాంశం|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_విత్తనం|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#నిర్వచించు EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_ఖండాలు |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#నిర్వచించు EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#నిర్వచించు EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#నిర్వచించు EXT2_FS_CLUSTER_నిష్పత్తి(fs)	(1 << (fs)->cluster_నిష్పత్తి_బిట్లు)
#నిర్వచించు EXT2_FS_CLUSTER_ముసుగు(fs)		(EXT2_FS_CLUSTER_నిష్పత్తి(fs) - 1)
#నిర్వచించు EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_నిష్పత్తి_బిట్లు)
/* Translate a cluster number to a block number */
#నిర్వచించు EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_నిష్పత్తి_బిట్లు)
/* Translate # of blks to # of clusters */
#నిర్వచించు EXT2_FS_అంకె_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_ముసుగు(fs)) >> \
					 (fs)->cluster_నిష్పత్తి_బిట్లు)

#ఒకవేళ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
రకనిర్వచనం నిర్మాణం stat64 ext2fs_struct_stat;
#లేకపోతే
రకనిర్వచనం నిర్మాణం stat ext2fs_struct_stat;
#అయితే_ముగింపు

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#నిర్వచించు EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
నిర్మాణం ext2_xattr_handle;
#నిర్వచించు XATTR_ABORT	1
#నిర్వచించు XATTR_మారింది	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#నిర్వచించు EXT2FS_BITMAPS_WRITE		0x0001
#నిర్వచించు EXT2_FS_BITMAPS_ఖండం		0x0002
#నిర్వచించు EXT2FS_BITMAPS_INODE		0x0004
#నిర్వచించు EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#నిర్వచించు EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#నిర్వచించు EXT2_FS_LINK_FT_ముసుగు	0x0007
#నిర్వచించు EXT2FS_LINK_APPEND	0x0010
#నిర్వచించు EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_has_సమూహం_desc_csum(ext2_filsys fs)
{
	తిరిగి ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_needs_large_file_feature(గుర్తులేని దీర్ఘ దీర్ఘ file_కొలత)
{
	తిరిగి file_కొలత >= 0x80000000ULL;
}

/* alloc.c */
బాహ్య శూన్య ext2fs_clear_ఖండం_uninit(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, పూర్ణాంకం బహుళకం,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
బాహ్య errcode_t ext2fs_new_ఖండం(ext2_filsys fs, blk_t goal,
				  ext2fs_ఖండం_bitmap map, blk_t *ret);
బాహ్య errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_ఖండం_bitmap map, blk64_t *ret);
బాహ్య errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_ఖండం_bitmap map, blk64_t *ret,
				   నిర్మాణం blk_alloc_ctx *ctx);
బాహ్య errcode_t ext2fs_get_free_ఖండాలు(ext2_filsys fs, blk_t ప్రారంభం,
					blk_t ముగింపు, పూర్ణాంకం అంకె,
					ext2fs_ఖండం_bitmap map,
					blk_t *ret);
బాహ్య errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ప్రారంభం,
					 blk64_t ముగింపు, పూర్ణాంకం అంకె,
					 ext2fs_ఖండం_bitmap map,
					 blk64_t *ret);
బాహ్య errcode_t ext2fs_alloc_ఖండం(ext2_filsys fs, blk_t goal,
				    అక్షరం *ఖండం_buf, blk_t *ret);
బాహ్య errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     అక్షరం *ఖండం_buf, blk64_t *ret);
బాహ్య errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     అక్షరం *ఖండం_buf, blk64_t *ret,
				     నిర్మాణం blk_alloc_ctx *ctx);

బాహ్య శూన్య ext2fs_సమితి_alloc_ఖండం_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_కనుగొను_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       నిర్మాణం ext2_inode *inode, blk64_t lblk);
బాహ్య శూన్య ext2fs_సమితి_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
బాహ్య శూన్య ext2fs_సమితి_ఖండం_alloc_stats_range_callback(ext2_filsys fs,
	శూన్య (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse),
	శూన్య (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse));
#నిర్వచించు EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#నిర్వచించు EXT2_NEWRANGE_MIN_నిడివి	(0x2)
#నిర్వచించు EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			   blk64_t len, ext2fs_ఖండం_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#నిర్వచించు EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#నిర్వచించు EXT2_ALLOCRANGE_ZERO_ఖండాలు	(0x2)
#నిర్వచించు EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
బాహ్య పూర్ణాంకం ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t సమూహం,
					ext2fs_ఖండం_bitmap bmap);
బాహ్య errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t సమూహం,
					       ext2fs_ఖండం_bitmap bmap,
					       blk_t *desc_ఖండాలు);
బాహ్య శూన్య ext2fs_సమితి_ఖండం_alloc_stats_callback(ext2_filsys fs,
						  శూన్య (*func)(ext2_filsys fs,
							       blk64_t blk,
							       పూర్ణాంకం inuse),
						  శూన్య (**old)(ext2_filsys fs,
							       blk64_t blk,
							       పూర్ణాంకం inuse));

/* alloc_stats.c */
శూన్య ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, పూర్ణాంకం inuse);
శూన్య ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       పూర్ణాంకం inuse, పూర్ణాంకం isdir);
శూన్య ext2fs_ఖండం_alloc_stats(ext2_filsys fs, blk_t blk, పూర్ణాంకం inuse);
శూన్య ext2fs_ఖండం_alloc_stats2(ext2_filsys fs, blk64_t blk, పూర్ణాంకం inuse);
శూన్య ext2fs_ఖండం_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse);

/* alloc_tables.c */
బాహ్య errcode_t ext2fs_allocate_tables(ext2_filsys fs);
బాహ్య errcode_t ext2fs_allocate_సమూహం_పట్టిక(ext2_filsys fs, dgrp_t సమూహం,
					     ext2fs_ఖండం_bitmap bmap);

/* badblocks.c */
బాహ్య errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, పూర్ణాంకం కొలత);
బాహ్య errcode_t ext2fs_u32_list_కూడు(ext2_u32_list bb, __u32 blk);
బాహ్య పూర్ణాంకం ext2fs_u32_list_కనుగొను(ext2_u32_list bb, __u32 blk);
బాహ్య పూర్ణాంకం ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
బాహ్య errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
బాహ్య పూర్ణాంకం ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
బాహ్య శూన్య ext2fs_u32_list_iterate_చివర(ext2_u32_iterate iter);
బాహ్య errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
బాహ్య పూర్ణాంకం ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

బాహ్య errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    పూర్ణాంకం కొలత);
బాహ్య errcode_t ext2fs_badblocks_list_కూడు(ext2_badblocks_list bb,
					   blk_t blk);
బాహ్య పూర్ణాంకం ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
బాహ్య పూర్ణాంకం ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
బాహ్య శూన్య ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
బాహ్య errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
బాహ్య పూర్ణాంకం ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
బాహ్య శూన్య ext2fs_badblocks_list_iterate_చివర(ext2_badblocks_iterate iter);
బాహ్య errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
బాహ్య పూర్ణాంకం ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
బాహ్య పూర్ణాంకం ext2fs_u32_list_లెక్క(ext2_u32_list bb);

/* bb_compat */
బాహ్య errcode_t badblocks_list_create(badblocks_list *ret, పూర్ణాంకం కొలత);
బాహ్య errcode_t badblocks_list_కూడు(badblocks_list bb, blk_t blk);
బాహ్య పూర్ణాంకం badblocks_list_test(badblocks_list bb, blk_t blk);
బాహ్య errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
బాహ్య పూర్ణాంకం badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
బాహ్య శూన్య badblocks_list_iterate_చివర(badblocks_iterate iter);
బాహ్య శూన్య badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
బాహ్య errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
బాహ్య శూన్య ext2fs_free_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap);
బాహ్య శూన్య ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
బాహ్య errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
బాహ్య errcode_t ext2fs_allocate_ఖండం_bitmap(ext2_filsys fs,
					      స్థిరరాశి అక్షరం *descr,
					      ext2fs_ఖండం_bitmap *ret);
బాహ్య errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   స్థిరరాశి అక్షరం *descr,
						   ext2fs_ఖండం_bitmap *ret);
బాహ్య పూర్ణాంకం ext2fs_get_bitmap_granularity(ext2fs_ఖండం_bitmap bitmap);
బాహ్య errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      స్థిరరాశి అక్షరం *descr,
					      ext2fs_inode_bitmap *ret);
బాహ్య errcode_t ext2fs_fudge_inode_bitmap_చివర(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t చివర, ext2_ino_t *oend);
బాహ్య errcode_t ext2fs_fudge_ఖండం_bitmap_చివర(ext2fs_ఖండం_bitmap bitmap,
					       blk_t చివర, blk_t *oend);
బాహ్య errcode_t ext2fs_fudge_ఖండం_bitmap_end2(ext2fs_ఖండం_bitmap bitmap,
					 blk64_t చివర, blk64_t *oend);
బాహ్య శూన్య ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
బాహ్య శూన్య ext2fs_clear_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap);
బాహ్య errcode_t ext2fs_resize_inode_bitmap(__u32 new_చివర, __u32 new_real_చివర,
					    ext2fs_inode_bitmap bmap);
బాహ్య errcode_t ext2fs_resize_inode_bitmap2(__u64 new_చివర,
					     __u64 new_real_చివర,
					     ext2fs_inode_bitmap bmap);
బాహ్య errcode_t ext2fs_resize_ఖండం_bitmap(__u32 new_చివర, __u32 new_real_చివర,
					    ext2fs_ఖండం_bitmap bmap);
బాహ్య errcode_t ext2fs_resize_ఖండం_bitmap2(__u64 new_చివర,
					     __u64 new_real_చివర,
					     ext2fs_ఖండం_bitmap bmap);
బాహ్య errcode_t ext2fs_compare_ఖండం_bitmap(ext2fs_ఖండం_bitmap bm1,
					     ext2fs_ఖండం_bitmap bm2);
బాహ్య errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
బాహ్య errcode_t ext2fs_సమితి_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల);
బాహ్య errcode_t ext2fs_సమితి_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ప్రారంభం, size_t అంకె,
					 శూన్య *లోపల);
బాహ్య errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం);
బాహ్య errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ప్రారంభం, size_t అంకె,
					 శూన్య *బహిర్గతం);
బాహ్య errcode_t ext2fs_సమితి_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bmap,
					blk_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల);
బాహ్య errcode_t ext2fs_సమితి_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bmap,
					 blk64_t ప్రారంభం, size_t అంకె,
					 శూన్య *లోపల);
బాహ్య errcode_t ext2fs_get_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bmap,
					blk_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం);
బాహ్య errcode_t ext2fs_get_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bmap,
					 blk64_t ప్రారంభం, size_t అంకె,
					 శూన్య *బహిర్గతం);

/* blknum.c */
బాహ్య __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య __u32 ext2fs_ఖండం_bitmap_checksum(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య dgrp_t ext2fs_సమూహం_యొక్క_blk2(ext2_filsys fs, blk64_t);
బాహ్య blk64_t ext2fs_సమూహం_మొదటి_block2(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk64_t ext2fs_సమూహం_చివరి_block2(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య పూర్ణాంకం ext2fs_సమూహం_ఖండాలు_లెక్క(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk64_t ext2fs_inode_దత్తాంశం_blocks2(ext2_filsys fs,
					 నిర్మాణం ext2_inode *inode);
బాహ్య blk64_t ext2fs_inode_i_ఖండాలు(ext2_filsys fs,
				     నిర్మాణం ext2_inode *inode);
బాహ్య blk64_t ext2fs_get_stat_i_ఖండాలు(ext2_filsys fs,
					నిర్మాణం ext2_inode *inode);
బాహ్య blk64_t ext2fs_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super);
బాహ్య శూన్య ext2fs_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super,
				    blk64_t blk);
బాహ్య శూన్య ext2fs_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super,
				    blk64_t blk);
బాహ్య blk64_t ext2fs_r_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super);
బాహ్య శూన్య ext2fs_r_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super,
				      blk64_t blk);
బాహ్య శూన్య ext2fs_r_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super,
				      blk64_t blk);
బాహ్య blk64_t ext2fs_free_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super);
బాహ్య శూన్య ext2fs_free_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super,
					 blk64_t blk);
బాహ్య శూన్య ext2fs_free_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
బాహ్య నిర్మాణం ext2_సమూహం_desc *ext2fs_సమూహం_desc(ext2_filsys fs,
					  నిర్మాణం opaque_ext2_సమూహం_desc *gdp,
					  dgrp_t సమూహం);
బాహ్య blk64_t ext2fs_ఖండం_bitmap_csum(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk64_t ext2fs_ఖండం_bitmap_loc(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_ఖండం_bitmap_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					blk64_t blk);
బాహ్య __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_inode_bitmap_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					blk64_t blk);
బాహ్య blk64_t ext2fs_inode_పట్టిక_loc(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_inode_పట్టిక_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం,
				       blk64_t blk);
బాహ్య __u32 ext2fs_bg_free_ఖండాలు_లెక్క(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					 __u32 n);
బాహ్య __u32 ext2fs_bg_free_inodes_లెక్క(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_free_inodes_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					 __u32 n);
బాహ్య __u32 ext2fs_bg_వాడిన_dirs_లెక్క(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_వాడిన_dirs_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం,
				       __u32 n);
బాహ్య __u32 ext2fs_bg_itable_వాడని(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_itable_వాడని_సమితి(ext2_filsys fs, dgrp_t సమూహం,
				     __u32 n);
బాహ్య __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య పూర్ణాంకం ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flag);
బాహ్య శూన్య ext2fs_bg_flags_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flags);
బాహ్య శూన్య ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flags);
బాహ్య __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య శూన్య ext2fs_bg_checksum_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u16 checksum);
బాహ్య blk64_t ext2fs_file_acl_ఖండం(ext2_filsys fs,
				     స్థిరరాశి నిర్మాణం ext2_inode *inode);
బాహ్య శూన్య ext2fs_file_acl_ఖండం_సమితి(ext2_filsys fs,
				      నిర్మాణం ext2_inode *inode, blk64_t blk);
బాహ్య errcode_t ext2fs_inode_కొలత_సమితి(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				       ext2_off64_t కొలత);

/* block.c */
బాహ్య errcode_t ext2fs_ఖండం_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      పూర్ణాంకం	flags,
				      అక్షరం *ఖండం_buf,
				      పూర్ణాంకం (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  పూర్ణాంకం	blockcnt,
						  శూన్య	*priv_దత్తాంశం),
				      శూన్య *priv_దత్తాంశం);
errcode_t ext2fs_ఖండం_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				పూర్ణాంకం	flags,
				అక్షరం *ఖండం_buf,
				పూర్ణాంకం (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    పూర్ణాంకం		ref_స్థానభ్రంశం,
					    శూన్య	*priv_దత్తాంశం),
				శూన్య *priv_దత్తాంశం);
errcode_t ext2fs_ఖండం_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				పూర్ణాంకం	flags,
				అక్షరం *ఖండం_buf,
				పూర్ణాంకం (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    పూర్ణాంకం		ref_స్థానభ్రంశం,
					    శూన్య	*priv_దత్తాంశం),
				శూన్య *priv_దత్తాంశం);

/* bmap.c */
బాహ్య errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode *inode,
			     అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags,
			     blk_t ఖండం, blk_t *phys_blk);
బాహ్య errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      నిర్మాణం ext2_inode *inode,
			      అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags, blk64_t ఖండం,
			      పూర్ణాంకం *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_ఖండం(ext2_filsys fs, ext2_ino_t ino,
				   నిర్మాణం ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#ఒకవేళ 0
/* bmove.c */
బాహ్య errcode_t ext2fs_move_ఖండాలు(ext2_filsys fs,
				    ext2fs_ఖండం_bitmap reserve,
				    ext2fs_ఖండం_bitmap alloc_map,
				    పూర్ణాంకం flags);
#అయితే_ముగింపు

/* check_desc.c */
బాహ్య errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
బాహ్య errcode_t ext2fs_close(ext2_filsys fs);
బాహ్య errcode_t ext2fs_close2(ext2_filsys fs, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_close_free(ext2_filsys *fs);
బాహ్య errcode_t ext2fs_flush(ext2_filsys fs);
బాహ్య errcode_t ext2fs_flush2(ext2_filsys fs, పూర్ణాంకం flags);
బాహ్య పూర్ణాంకం ext2fs_bg_has_super(ext2_filsys fs, dgrp_t సమూహం_ఖండం);
బాహ్య errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t సమూహం,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_వాడిన_blks);
బాహ్య పూర్ణాంకం ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t సమూహం,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    పూర్ణాంకం *ret_meta_bg);
బాహ్య శూన్య ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
బాహ్య __u32 ext2fs_చక్రీయ_తనిఖీ_32_be(__u32 చక్రీయ_తనిఖీ, గుర్తులేని అక్షరం స్థిరరాశి *p, size_t len);
బాహ్య __u32 ext2fs_crc32c_le(__u32 చక్రీయ_తనిఖీ, గుర్తులేని అక్షరం స్థిరరాశి *p, size_t len);

/* csum.c */
బాహ్య శూన్య ext2fs_init_csum_విత్తనం(ext2_filsys fs);
బాహ్య errcode_t ext2fs_mmp_csum_సమితి(ext2_filsys fs, నిర్మాణం mmp_struct *mmp);
బాహ్య పూర్ణాంకం ext2fs_mmp_csum_verify(ext2_filsys, నిర్మాణం mmp_struct *mmp);
బాహ్య పూర్ణాంకం ext2fs_verify_csum_type(ext2_filsys fs, నిర్మాణం ext2_super_ఖండం *sb);
బాహ్య errcode_t ext2fs_superblock_csum_సమితి(ext2_filsys fs,
					    నిర్మాణం ext2_super_ఖండం *sb);
బాహ్య పూర్ణాంకం ext2fs_superblock_csum_verify(ext2_filsys fs,
					 నిర్మాణం ext2_super_ఖండం *sb);
బాహ్య errcode_t ext2fs_ext_attr_ఖండం_csum_సమితి(ext2_filsys fs,
					ext2_ino_t inum, blk64_t ఖండం,
					నిర్మాణం ext2_ext_attr_శీర్షిక *hdr);
బాహ్య పూర్ణాంకం ext2fs_ext_attr_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t ఖండం,
					     నిర్మాణం ext2_ext_attr_శీర్షిక *hdr);
#నిర్వచించు EXT2_DIRENT_తోక(ఖండం, blocksize) \
	((నిర్మాణం ext2_dir_నమోదు_తోక *)(((అక్షరం *)(ఖండం)) + \
	(blocksize) - పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక)))

బాహ్య శూన్య ext2fs_initialize_dirent_తోక(ext2_filsys fs,
					  నిర్మాణం ext2_dir_నమోదు_తోక *t);
బాహ్య పూర్ణాంకం ext2fs_dirent_has_తోక(ext2_filsys fs,
				  నిర్మాణం ext2_dir_నమోదు *dirent);
బాహ్య పూర్ణాంకం ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     నిర్మాణం ext2_dir_నమోదు *dirent);
బాహ్య పూర్ణాంకం ext2fs_dir_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					నిర్మాణం ext2_dir_నమోదు *dirent);
బాహ్య errcode_t ext2fs_dir_ఖండం_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
					   నిర్మాణం ext2_dir_నమోదు *dirent);
బాహ్య errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  నిర్మాణం ext2_dir_నమోదు *dirent,
					  నిర్మాణం ext2_dx_countlimit **cc,
					  పూర్ణాంకం *స్థానభ్రంశం);
బాహ్య errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				నిర్మాణం ext2_dir_నమోదు *dirent,
				__u32 *చక్రీయ_తనిఖీ, నిర్మాణం ext2_dx_తోక **ret_t);
బాహ్య errcode_t ext2fs_extent_ఖండం_csum_సమితి(ext2_filsys fs,
					      ext2_ino_t inum,
					      నిర్మాణం ext3_extent_శీర్షిక *eh);
బాహ్య పూర్ణాంకం ext2fs_extent_ఖండం_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   నిర్మాణం ext3_extent_శీర్షిక *eh);
బాహ్య errcode_t ext2fs_ఖండం_bitmap_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					      అక్షరం *bitmap, పూర్ణాంకం కొలత);
బాహ్య పూర్ణాంకం ext2fs_ఖండం_bitmap_csum_verify(ext2_filsys fs, dgrp_t సమూహం,
					   అక్షరం *bitmap, పూర్ణాంకం కొలత);
బాహ్య errcode_t ext2fs_inode_bitmap_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం,
					      అక్షరం *bitmap, పూర్ణాంకం కొలత);
బాహ్య పూర్ణాంకం ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t సమూహం,
					   అక్షరం *bitmap, పూర్ణాంకం కొలత);
బాహ్య errcode_t ext2fs_inode_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
				       నిర్మాణం ext2_inode_large *inode);
బాహ్య పూర్ణాంకం ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    నిర్మాణం ext2_inode_large *inode);
బాహ్య శూన్య ext2fs_సమూహం_desc_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య పూర్ణాంకం ext2fs_సమూహం_desc_csum_verify(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య errcode_t ext2fs_సమితి_gdt_csum(ext2_filsys fs);
బాహ్య __u16 ext2fs_సమూహం_desc_csum(ext2_filsys fs, dgrp_t సమూహం);

/* dblist.c */
బాహ్య errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
బాహ్య errcode_t ext2fs_కూడు_dir_ఖండం(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, పూర్ణాంకం blockcnt);
బాహ్య errcode_t ext2fs_కూడు_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
బాహ్య శూన్య ext2fs_dblist_క్రమీకరించు(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(స్థిరరాశి శూన్య *,
							   స్థిరరాశి శూన్య *));
బాహ్య శూన్య ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(స్థిరరాశి శూన్య *,
							    స్థిరరాశి శూన్య *));
బాహ్య errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	పూర్ణాంకం (*func)(ext2_filsys fs, నిర్మాణం ext2_db_నమోదు *db_info,
		    శూన్య	*priv_దత్తాంశం),
	శూన్య *priv_దత్తాంశం);
బాహ్య errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	పూర్ణాంకం (*func)(ext2_filsys fs, నిర్మాణం ext2_db_entry2 *db_info,
		    శూన్య	*priv_దత్తాంశం),
	శూన్య *priv_దత్తాంశం);
బాహ్య errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	పూర్ణాంకం (*func)(ext2_filsys fs, నిర్మాణం ext2_db_entry2 *db_info,
		    శూన్య	*priv_దత్తాంశం),
	గుర్తులేని దీర్ఘ దీర్ఘ ప్రారంభం,
	గుర్తులేని దీర్ఘ దీర్ఘ లెక్క,
	శూన్య *priv_దత్తాంశం);
బాహ్య errcode_t ext2fs_సమితి_dir_ఖండం(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, పూర్ణాంకం blockcnt);
బాహ్య errcode_t ext2fs_సమితి_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
బాహ్య errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
బాహ్య పూర్ణాంకం ext2fs_dblist_లెక్క(ext2_dblist dblist);
బాహ్య blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
బాహ్య errcode_t ext2fs_dblist_get_చివరి(ext2_dblist dblist,
					నిర్మాణం ext2_db_నమోదు **నమోదు_2);
బాహ్య errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					నిర్మాణం ext2_db_entry2 **నమోదు_2);
బాహ్య errcode_t ext2fs_dblist_drop_చివరి(ext2_dblist dblist);

/* dblist_dir.c */
బాహ్య errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  పూర్ణాంకం	flags,
				  అక్షరం	*ఖండం_buf,
				  పూర్ణాంకం (*func)(ext2_ino_t	dir,
					      పూర్ణాంకం		నమోదు_2,
					      నిర్మాణం ext2_dir_నమోదు *dirent,
					      పూర్ణాంకం	స్థానభ్రంశం,
					      పూర్ణాంకం	blocksize,
					      అక్షరం	*buf,
					      శూన్య	*priv_దత్తాంశం),
				  శూన్య *priv_దత్తాంశం);

#ఒకవేళ 0
/* digest_encode.c */
#నిర్వచించు EXT2_FS_DIGEST_కొలత EXT2_FS_SHA256_నిడివి
బాహ్య పూర్ణాంకం ext2fs_digest_encode(స్థిరరాశి అక్షరం *src, పూర్ణాంకం len, అక్షరం *dst);
బాహ్య పూర్ణాంకం ext2fs_digest_decode(స్థిరరాశి అక్షరం *src, పూర్ణాంకం len, అక్షరం *dst);
#అయితే_ముగింపు

/* dirblock.c */
బాహ్య errcode_t ext2fs_read_dir_ఖండం(ext2_filsys fs, blk_t ఖండం,
				       శూన్య *buf);
బాహ్య errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ఖండం,
					శూన్య *buf, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ఖండం,
					శూన్య *buf, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ఖండం,
					శూన్య *buf, పూర్ణాంకం flags, ext2_ino_t ino);
బాహ్య errcode_t ext2fs_write_dir_ఖండం(ext2_filsys fs, blk_t ఖండం,
					శూన్య *buf);
బాహ్య errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ఖండం,
					 శూన్య *buf, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ఖండం,
					 శూన్య *buf, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ఖండం,
					 శూన్య *buf, పూర్ణాంకం flags, ext2_ino_t ino);

/* dirhash.c */
బాహ్య errcode_t ext2fs_dirhash(పూర్ణాంకం సంస్కరణ, స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
				స్థిరరాశి __u32 *విత్తనం,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

బాహ్య errcode_t ext2fs_dirhash2(పూర్ణాంకం సంస్కరణ, స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
				 స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *charset,
				 పూర్ణాంకం hash_flags,
				 స్థిరరాశి __u32 *విత్తనం,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
బాహ్య errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    నిర్మాణం ext2_dir_నమోదు *dirent,
				    గుర్తులేని పూర్ణాంకం *rec_len);
బాహ్య errcode_t ext2fs_సమితి_rec_len(ext2_filsys fs,
				    గుర్తులేని పూర్ణాంకం len,
				    నిర్మాణం ext2_dir_నమోదు *dirent);
బాహ్య errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      పూర్ణాంకం flags,
			      అక్షరం *ఖండం_buf,
			      పూర్ణాంకం (*func)(నిర్మాణం ext2_dir_నమోదు *dirent,
					  పూర్ణాంకం	స్థానభ్రంశం,
					  పూర్ణాంకం	blocksize,
					  అక్షరం	*buf,
					  శూన్య	*priv_దత్తాంశం),
			      శూన్య *priv_దత్తాంశం);
బాహ్య errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      పూర్ణాంకం flags,
			      అక్షరం *ఖండం_buf,
			      పూర్ణాంకం (*func)(ext2_ino_t	dir,
					  పూర్ణాంకం	నమోదు_2,
					  నిర్మాణం ext2_dir_నమోదు *dirent,
					  పూర్ణాంకం	స్థానభ్రంశం,
					  పూర్ణాంకం	blocksize,
					  అక్షరం	*buf,
					  శూన్య	*priv_దత్తాంశం),
			      శూన్య *priv_దత్తాంశం);

/* dupfs.c */
బాహ్య errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
బాహ్య errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
బాహ్య __u32 ext2fs_ext_attr_hash_నమోదు(నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
					శూన్య *దత్తాంశం);
బాహ్య __u32 ext2fs_ext_attr_hash_నమోదు_signed(నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
					       శూన్య *దత్తాంశం);
బాహ్య errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
					     శూన్య *దత్తాంశం, __u32 *hash);
బాహ్య errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
					     శూన్య *దత్తాంశం, __u32 *hash,
					     __u32 *signed_hash);
బాహ్య errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ఖండం, శూన్య *buf);
బాహ్య errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ఖండం,
				       శూన్య *buf);
బాహ్య errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ఖండం,
				       శూన్య *buf, ext2_ino_t inum);
బాహ్య errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ఖండం,
				       శూన్య *buf);
బాహ్య errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ఖండం,
				       శూన్య *buf);
బాహ్య errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ఖండం,
				       శూన్య *buf, ext2_ino_t inum);
బాహ్య errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   అక్షరం *ఖండం_buf,
					   పూర్ణాంకం adjust, __u32 *newcount);
బాహ్య errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   అక్షరం *ఖండం_buf,
					   పూర్ణాంకం adjust, __u32 *newcount);
బాహ్య errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   అక్షరం *ఖండం_buf,
					   పూర్ణాంకం adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(నిర్మాణం ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(నిర్మాణం ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(నిర్మాణం ext2_xattr_handle *handle,
				   నిర్మాణం ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(నిర్మాణం ext2_xattr_handle *h,
				పూర్ణాంకం (*func)(అక్షరం *name, అక్షరం *విలువ,
					    size_t విలువ_len, శూన్య *దత్తాంశం),
				శూన్య *దత్తాంశం);
errcode_t ext2fs_xattr_get(నిర్మాణం ext2_xattr_handle *h, స్థిరరాశి అక్షరం *కీలకం,
			   శూన్య **విలువ, size_t *విలువ_len);
errcode_t ext2fs_xattr_సమితి(నిర్మాణం ext2_xattr_handle *handle,
			   స్థిరరాశి అక్షరం *కీలకం,
			   స్థిరరాశి శూన్య *విలువ,
			   size_t విలువ_len);
errcode_t ext2fs_xattr_remove(నిర్మాణం ext2_xattr_handle *handle,
			      స్థిరరాశి అక్షరం *కీలకం);
errcode_t ext2fs_xattr_remove_all(నిర్మాణం ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(నిర్మాణం ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       నిర్మాణం ext2_inode_large *inode);
errcode_t ext2fs_xattrs_లెక్క(నిర్మాణం ext2_xattr_handle *handle, size_t *లెక్క);
errcode_t ext2fs_xattr_inode_max_కొలత(ext2_filsys fs, ext2_ino_t ino,
				      size_t *కొలత);
#నిర్వచించు XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(నిర్మాణం ext2_xattr_handle *handle,
			      గుర్తులేని పూర్ణాంకం *new_flags, గుర్తులేని పూర్ణాంకం *old_flags);
బాహ్య శూన్య ext2fs_ext_attr_ఖండం_rehash(నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక,
					 నిర్మాణం ext2_ext_attr_నమోదు *చివర);
బాహ్య __u32 ext2fs_get_ea_inode_hash(నిర్మాణం ext2_inode *inode);
బాహ్య శూన్య ext2fs_సమితి_ea_inode_hash(నిర్మాణం ext2_inode *inode, __u32 hash);
బాహ్య __u64 ext2fs_get_ea_inode_ref(నిర్మాణం ext2_inode *inode);
బాహ్య శూన్య ext2fs_సమితి_ea_inode_ref(నిర్మాణం ext2_inode *inode, __u64 ref_లెక్క);

/* extent.c */
బాహ్య errcode_t ext2fs_extent_శీర్షిక_verify(శూన్య *ptr, పూర్ణాంకం కొలత);
బాహ్య errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
బాహ్య errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					నిర్మాణం ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
బాహ్య శూన్య ext2fs_extent_free(ext2_extent_handle_t handle);
బాహ్య errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   పూర్ణాంకం flags, నిర్మాణం ext2fs_extent *extent);
బాహ్య errcode_t ext2fs_extent_కణుపు_విభజించు(ext2_extent_handle_t handle);
బాహ్య errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, పూర్ణాంకం flags,
				       నిర్మాణం ext2fs_extent *extent);
బాహ్య errcode_t ext2fs_extent_చొప్పించు(ext2_extent_handle_t handle, పూర్ణాంకం flags,
				      నిర్మాణం ext2fs_extent *extent);
బాహ్య errcode_t ext2fs_extent_సమితి_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					నిర్మాణం ext2_extent_info *info);
బాహ్య errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
బాహ్య errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     పూర్ణాంకం ఆకు_level, blk64_t blk);
బాహ్య errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
బాహ్య size_t ext2fs_max_extent_లోతు(ext2_extent_handle_t handle);
బాహ్య errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      నిర్మాణం ext2_inode *inode);
బాహ్య errcode_t ext2fs_లెక్క_ఖండాలు(ext2_filsys fs, ext2_ino_t ino,
				     నిర్మాణం ext2_inode *inode, blk64_t *ret_లెక్క);
బాహ్య errcode_t ext2fs_decode_extent(నిర్మాణం ext2fs_extent *గమ్యం, శూన్య *from,
				      పూర్ణాంకం len);

/* fallocate.c */
#నిర్వచించు EXT2_FALLOCATE_ZERO_ఖండాలు	(0x1)
#నిర్వచించు EXT2_FALLOCATE_FORCE_INIT	(0x2)
#నిర్వచించు EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#నిర్వచించు EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#నిర్వచించు EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, పూర్ణాంకం flags, ext2_ino_t ino,
			   నిర్మాణం ext2_inode *inode, blk64_t goal,
			   blk64_t ప్రారంభం, blk64_t len);

/* fileio.c */
బాహ్య errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   నిర్మాణం ext2_inode *inode,
				   పూర్ణాంకం flags, ext2_file_t *ret);
బాహ్య errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  పూర్ణాంకం flags, ext2_file_t *ret);
బాహ్య ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
నిర్మాణం ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
బాహ్య ext2_ino_t ext2fs_file_get_inode_అంకె(ext2_file_t file);
బాహ్య errcode_t ext2fs_file_close(ext2_file_t file);
బాహ్య errcode_t ext2fs_file_flush(ext2_file_t file);
బాహ్య errcode_t ext2fs_file_read(ext2_file_t file, శూన్య *buf,
				  గుర్తులేని పూర్ణాంకం wanted, గుర్తులేని పూర్ణాంకం *got);
బాహ్య errcode_t ext2fs_file_write(ext2_file_t file, స్థిరరాశి శూన్య *buf,
				   గుర్తులేని పూర్ణాంకం nbytes, గుర్తులేని పూర్ణాంకం *written);
బాహ్య errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 స్థానభ్రంశం,
				   పూర్ణాంకం whence, __u64 *ret_pos);
బాహ్య errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t స్థానభ్రంశం,
				   పూర్ణాంకం whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_కొలత);
బాహ్య ext2_off_t ext2fs_file_get_కొలత(ext2_file_t file);
బాహ్య errcode_t ext2fs_file_సమితి_కొలత(ext2_file_t file, ext2_off_t కొలత);
బాహ్య errcode_t ext2fs_file_సమితి_size2(ext2_file_t file, ext2_off64_t కొలత);

/* finddev.c */
బాహ్య అక్షరం *ext2fs_కనుగొను_ఖండం_device(dev_t device);

/* flushb.c */
బాహ్య errcode_t ext2fs_sync_device(పూర్ణాంకం fd, పూర్ణాంకం flushb);

/* freefs.c */
బాహ్య శూన్య ext2fs_free(ext2_filsys fs);
బాహ్య శూన్య ext2fs_free_dblist(ext2_dblist dblist);
బాహ్య శూన్య ext2fs_badblocks_list_free(ext2_badblocks_list bb);
బాహ్య శూన్య ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
బాహ్య శూన్య ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
బాహ్య errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 ప్రారంభం, __u32 చివర,
					    __u32 real_చివర,
					    స్థిరరాశి అక్షరం *descr, అక్షరం *init_map,
					    ext2fs_generic_bitmap *ret);
బాహ్య errcode_t ext2fs_allocate_generic_bitmap(__u32 ప్రారంభం,
						__u32 చివర,
						__u32 real_చివర,
						స్థిరరాశి అక్షరం *descr,
						ext2fs_generic_bitmap *ret);
బాహ్య errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
బాహ్య శూన్య ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
బాహ్య errcode_t ext2fs_fudge_generic_bitmap_చివర(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t చివర,
						 ext2_ino_t *oend);
బాహ్య శూన్య ext2fs_సమితి_generic_bitmap_padding(ext2fs_generic_bitmap map);
బాహ్య errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_చివర,
					      __u32 new_real_చివర,
					      ext2fs_generic_bitmap bmap);
బాహ్య errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
బాహ్య errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ప్రారంభం, __u32 అంకె,
						 శూన్య *బహిర్గతం);
బాహ్య errcode_t ext2fs_సమితి_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ప్రారంభం, __u32 అంకె,
						 శూన్య *లోపల);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ప్రారంభం, __u32 చివర,
						       __u32 *బహిర్గతం);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_సమితి_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ప్రారంభం, __u32 చివర,
						       __u32 *బహిర్గతం);

/* gen_bitmap64.c */
శూన్య ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    పూర్ణాంకం type, __u64 ప్రారంభం, __u64 చివర,
				    __u64 real_చివర,
				    స్థిరరాశి అక్షరం *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
శూన్య ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_చివర(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 చివర, __u64 *oend);
శూన్య ext2fs_సమితి_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_చివర,
				     __u64 new_real_చివర);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం);
errcode_t ext2fs_సమితి_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ఖండం_bitmap *bitmap);
errcode_t ext2fs_లెక్క_వాడిన_clusters(ext2_filsys fs, blk64_t ప్రారంభం,
				     blk64_t చివర, blk64_t *బహిర్గతం);
errcode_t ext2fs_లెక్క_వాడిన_ఖండాలు(ext2_filsys fs, blk64_t ప్రారంభం,
				   blk64_t చివర, blk64_t *బహిర్గతం);
బాహ్య గుర్తులేని పూర్ణాంకం ext2fs_list_backups(ext2_filsys fs, గుర్తులేని పూర్ణాంకం *three,
				గుర్తులేని పూర్ణాంకం *five, గుర్తులేని పూర్ణాంకం *seven);

/* getenv.c */
బాహ్య అక్షరం *ext2fs_సురక్షిత_getenv(స్థిరరాశి అక్షరం *arg);

/* get_num_dirs.c */
బాహ్య errcode_t ext2fs_get_అంకె_dirs(ext2_filsys fs, ext2_ino_t *ret_అంకె_dirs);

/* getsize.c */
బాహ్య errcode_t ext2fs_get_device_కొలత(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
					blk_t *retblocks);
బాహ్య errcode_t ext2fs_get_device_size2(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
బాహ్య పూర్ణాంకం ext2fs_get_dio_alignment(పూర్ణాంకం fd);
errcode_t ext2fs_get_device_sectsize(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_కూడు_ఖండాలు(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				 blk64_t అంకె_ఖండాలు);
errcode_t ext2fs_iblk_sub_ఖండాలు(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				 blk64_t అంకె_ఖండాలు);
errcode_t ext2fs_iblk_సమితి(ext2_filsys fs, నిర్మాణం ext2_inode *inode, blk64_t b);

/* imager.c */
బాహ్య errcode_t ext2fs_image_inode_write(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_image_inode_read(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_image_super_write(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_image_super_read(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_ఖండం(ext2_filsys fs, blk_t blk, శూన్య *buf);
errcode_t ext2fs_write_ind_ఖండం(ext2_filsys fs, blk_t blk, శూన్య *buf);

/* initialize.c */
బాహ్య errcode_t ext2fs_initialize(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags,
				   నిర్మాణం ext2_super_ఖండం *param,
				   io_manager manager, ext2_filsys *ret_fs);
బాహ్య errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, పూర్ణాంకం super_only);

/* icount.c */
బాహ్య శూన్య ext2fs_free_icount(ext2_icount_t icount);
బాహ్య errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, అక్షరం *tdb_dir,
					  పూర్ణాంకం flags, ext2_icount_t *ret);
బాహ్య errcode_t ext2fs_create_icount2(ext2_filsys fs, పూర్ణాంకం flags,
				       గుర్తులేని పూర్ణాంకం కొలత,
				       ext2_icount_t hint, ext2_icount_t *ret);
బాహ్య errcode_t ext2fs_create_icount(ext2_filsys fs, పూర్ణాంకం flags,
				      గుర్తులేని పూర్ణాంకం కొలత,
				      ext2_icount_t *ret);
బాహ్య errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
బాహ్య errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
బాహ్య errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
బాహ్య errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 లెక్క);
బాహ్య ext2_ino_t ext2fs_get_icount_కొలత(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

బాహ్య errcode_t ext2fs_get_memalign(గుర్తులేని దీర్ఘ కొలత,
				     గుర్తులేని దీర్ఘ align, శూన్య *ptr);

/* inline_data.c */
బాహ్య errcode_t ext2fs_inline_దత్తాంశం_init(ext2_filsys fs, ext2_ino_t ino);
బాహ్య errcode_t ext2fs_inline_దత్తాంశం_కొలత(ext2_filsys fs, ext2_ino_t ino,
					 size_t *కొలత);
బాహ్య errcode_t ext2fs_inline_దత్తాంశం_get(ext2_filsys fs, ext2_ino_t ino,
					నిర్మాణం ext2_inode *inode,
					శూన్య *buf, size_t *కొలత);
బాహ్య errcode_t ext2fs_inline_దత్తాంశం_సమితి(ext2_filsys fs, ext2_ino_t ino,
					నిర్మాణం ext2_inode *inode,
					శూన్య *buf, size_t కొలత);

/* inode.c */
బాహ్య errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   గుర్తులేని పూర్ణాంకం cache_కొలత);
బాహ్య శూన్య ext2fs_free_inode_cache(నిర్మాణం ext2_inode_cache *icache);
బాహ్య errcode_t ext2fs_flush_icache(ext2_filsys fs);
బాహ్య errcode_t ext2fs_get_తదుపరి_inode_నిండిన(ext2_inode_పరిశీలన పరిశీలన,
					    ext2_ino_t *ino,
					    నిర్మాణం ext2_inode *inode,
					    పూర్ణాంకం bufsize);
#నిర్వచించు EXT2_INODE_పరిశీలన_DEFAULT_బఫరు_ఖండాలు	8
బాహ్య errcode_t ext2fs_open_inode_పరిశీలన(ext2_filsys fs, పూర్ణాంకం బఫరు_ఖండాలు,
				  ext2_inode_పరిశీలన *ret_పరిశీలన);
బాహ్య శూన్య ext2fs_close_inode_పరిశీలన(ext2_inode_పరిశీలన పరిశీలన);
బాహ్య errcode_t ext2fs_get_తదుపరి_inode(ext2_inode_పరిశీలన పరిశీలన, ext2_ino_t *ino,
			       నిర్మాణం ext2_inode *inode);
బాహ్య errcode_t ext2fs_inode_పరిశీలన_goto_blockgroup(ext2_inode_పరిశీలన పరిశీలన,
						   పూర్ణాంకం	సమూహం);
బాహ్య శూన్య ext2fs_సమితి_inode_callback
	(ext2_inode_పరిశీలన పరిశీలన,
	 errcode_t (*పూర్తి_సమూహం)(ext2_filsys fs,
				 ext2_inode_పరిశీలన పరిశీలన,
				 dgrp_t సమూహం,
				 శూన్య * priv_దత్తాంశం),
	 శూన్య *పూర్తి_సమూహం_దత్తాంశం);
బాహ్య పూర్ణాంకం ext2fs_inode_పరిశీలన_flags(ext2_inode_పరిశీలన పరిశీలన, పూర్ణాంకం సమితి_flags,
				   పూర్ణాంకం clear_flags);
బాహ్య errcode_t ext2fs_read_inode_నిండిన(ext2_filsys fs, ext2_ino_t ino,
					నిర్మాణం ext2_inode * inode,
					పూర్ణాంకం bufsize);
బాహ్య errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    నిర్మాణం ext2_inode * inode);
బాహ్య errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    నిర్మాణం ext2_inode * inode,
				    పూర్ణాంకం bufsize, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_write_inode_నిండిన(ext2_filsys fs, ext2_ino_t ino,
					 నిర్మాణం ext2_inode * inode,
					 పూర్ణాంకం bufsize);
బాహ్య errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    నిర్మాణం ext2_inode * inode);
బాహ్య errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     నిర్మాణం ext2_inode * inode,
				     పూర్ణాంకం bufsize, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    నిర్మాణం ext2_inode * inode);
బాహ్య errcode_t ext2fs_get_ఖండాలు(ext2_filsys fs, ext2_ino_t ino, blk_t *ఖండాలు);
బాహ్య errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
బాహ్య io_manager inode_io_manager;
బాహ్య errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					అక్షరం **name);
బాహ్య errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 నిర్మాణం ext2_inode *inode,
					 అక్షరం **name);

/* ismounted.c */
బాహ్య errcode_t ext2fs_check_if_mounted(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *mount_flags);
బాహ్య errcode_t ext2fs_check_mount_బిందువు(స్థిరరాశి అక్షరం *device, పూర్ణాంకం *mount_flags,
					  అక్షరం *mtpt, పూర్ణాంకం mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
బాహ్య errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      నిర్మాణం ext2_inode *inode,
			      అక్షరం *ఖండం_buf, blk64_t ప్రారంభం,
			      blk64_t చివర);

/* namei.c */
బాహ్య errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, స్థిరరాశి అక్షరం *name,
			 పూర్ణాంకం namelen, అక్షరం *buf, ext2_ino_t *inode);
బాహ్య errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
			స్థిరరాశి అక్షరం *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
			      స్థిరరాశి అక్షరం *name, ext2_ino_t *inode);
బాహ్య errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
పూర్ణాంకం ext2fs_native_flag(శూన్య);

/* newdir.c */
బాహ్య errcode_t ext2fs_new_dir_ఖండం(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t మాతృ_ino, అక్షరం **ఖండం);
బాహ్య errcode_t ext2fs_new_dir_inline_దత్తాంశం(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t మాతృ_ino, __u32 *iblock);

/* nls_utf8.c */
బాహ్య స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *ext2fs_ఎక్కించు_nls_పట్టిక(పూర్ణాంకం encoding);
బాహ్య పూర్ణాంకం ext2fs_check_encoded_name(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
				     అక్షరం *s, size_t len, అక్షరం **pos);
బాహ్య పూర్ణాంకం ext2fs_casefold_cmp(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			       స్థిరరాశి గుర్తులేని అక్షరం *str1, size_t len1,
			       స్థిరరాశి గుర్తులేని అక్షరం *str2, size_t len2);

/* mkdir.c */
బాహ్య errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t మాతృ, ext2_ino_t inum,
			      స్థిరరాశి అక్షరం *name);
బాహ్య errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t మాతృ,
			       ext2_ino_t ino, గుర్తులేని దీర్ఘ flags,
			       పూర్ణాంకం link_flags, స్థిరరాశి అక్షరం *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
నిర్మాణం ext2fs_journal_params {
	blk_t అంకె_journal_ఖండాలు;
	blk_t అంకె_fc_ఖండాలు;
};
బాహ్య errcode_t ext2fs_get_journal_params(
		నిర్మాణం ext2fs_journal_params *params, ext2_filsys fs);
బాహ్య errcode_t ext2fs_zero_ఖండాలు(ext2_filsys fs, blk_t blk, పూర్ణాంకం అంకె,
				    blk_t *ret_blk, పూర్ణాంకం *ret_లెక్క);
బాహ్య errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, పూర్ణాంకం అంకె,
				     blk64_t *ret_blk, పూర్ణాంకం *ret_లెక్క);
బాహ్య errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 అంకె_ఖండాలు, పూర్ణాంకం flags,
						  అక్షరం  **ret_jsb);
బాహ్య errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  నిర్మాణం ext2fs_journal_params *params,
						  పూర్ణాంకం flags, అక్షరం  **ret_jsb);
బాహ్య errcode_t ext2fs_కూడు_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
బాహ్య errcode_t ext2fs_కూడు_journal_inode(ext2_filsys fs, blk_t అంకె_ఖండాలు,
					  పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_కూడు_journal_inode2(ext2_filsys fs, blk_t అంకె_ఖండాలు,
					   blk64_t goal, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_కూడు_journal_inode3(ext2_filsys fs,
				    నిర్మాణం ext2fs_journal_params *params,
				    blk64_t goal, పూర్ణాంకం flags);
బాహ్య పూర్ణాంకం ext2fs_default_journal_కొలత(__u64 అంకె_ఖండాలు);
బాహ్య పూర్ణాంకం ext2fs_journal_sb_ప్రారంభం(పూర్ణాంకం blocksize);

/* openfs.c */
బాహ్య errcode_t ext2fs_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, పూర్ణాంకం superblock,
			     గుర్తులేని పూర్ణాంకం ఖండం_కొలత, io_manager manager,
			     ext2_filsys *ret_fs);
బాహ్య errcode_t ext2fs_open2(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *io_options,
			      పూర్ణాంకం flags, పూర్ణాంకం superblock,
			      గుర్తులేని పూర్ణాంకం ఖండం_కొలత, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
బాహ్య blk64_t ext2fs_descriptor_ఖండం_loc2(ext2_filsys fs,
					blk64_t సమూహం_ఖండం, dgrp_t i);
బాహ్య blk_t ext2fs_descriptor_ఖండం_loc(ext2_filsys fs, blk_t సమూహం_ఖండం,
					 dgrp_t i);
errcode_t ext2fs_get_దత్తాంశం_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_సమితి_దత్తాంశం_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_గమ్యం_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
బాహ్య errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t అంకె_ఖండాలు);
బాహ్య errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
బాహ్య e2_blkcnt_t ext2fs_default_orphan_file_ఖండాలు(ext2_filsys fs);
బాహ్య __u32 ext2fs_do_orphan_file_ఖండం_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      అక్షరం *buf);
బాహ్య errcode_t ext2fs_orphan_file_ఖండం_csum_సమితి(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   అక్షరం *buf);
బాహ్య పూర్ణాంకం ext2fs_orphan_file_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, అక్షరం *buf);

/* get_pathname.c */
బాహ్య errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       అక్షరం **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, స్థిరరాశి అక్షరం *name,
		      ext2_ino_t ino, పూర్ణాంకం flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, స్థిరరాశి అక్షరం *name,
			ext2_ino_t ino, పూర్ణాంకం flags);
పూర్ణాంకం ext2fs_dir_is_dx(ext2_filsys fs, స్థిరరాశి నిర్మాణం ext2_inode *inode);
శూన్య ext2fs_inc_nlink(ext2_filsys fs, నిర్మాణం ext2_inode *inode);
శూన్య ext2fs_dec_nlink(నిర్మాణం ext2_inode *inode);
పూర్ణాంకం ext2fs_dir_link_max(ext2_filsys fs, నిర్మాణం ext2_inode_large *inode);
పూర్ణాంకం ext2fs_dir_link_ఖాళీలు(నిర్మాణం ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t మాతృ, ext2_ino_t ino,
			 స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *లక్ష్యం);
పూర్ణాంకం ext2fs_is_fast_symlink(నిర్మాణం ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, శూన్య *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, శూన్య *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_ప్రారంభం(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, పూర్ణాంకం immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
గుర్తులేని ext2fs_mmp_new_seq(శూన్య);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, శూన్య **ptr);

/* read_bb.c */
బాహ్య errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
బాహ్య errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      శూన్య *priv_దత్తాంశం,
				      శూన్య (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      అక్షరం *badstr,
						      శూన్య *priv_దత్తాంశం));
బాహ్య errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     శూన్య (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
బాహ్య errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
బాహ్య errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, పూర్ణాంకం flags, పూర్ణాంకం అంకె_దారాలు);
బాహ్య errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
బాహ్య errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
బాహ్య errcode_t ext2fs_read_ఖండం_bitmap(ext2_filsys fs);
బాహ్య errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
బాహ్య errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
బాహ్య errcode_t ext2fs_write_ఖండం_bitmap (ext2_filsys fs);

/*sha256.c */
#నిర్వచించు EXT2_FS_SHA256_నిడివి 32
#ఒకవేళ 0
బాహ్య శూన్య ext2fs_sha256(స్థిరరాశి గుర్తులేని అక్షరం *లోపల, గుర్తులేని దీర్ఘ లోపల_కొలత,
		   గుర్తులేని అక్షరం బహిర్గతం[EXT2_FS_SHA256_నిడివి]);
#అయితే_ముగింపు

/* sha512.c */
#నిర్వచించు EXT2_FS_SHA512_నిడివి 64
బాహ్య శూన్య ext2fs_sha512(స్థిరరాశి గుర్తులేని అక్షరం *లోపల, గుర్తులేని దీర్ఘ లోపల_కొలత,
			  గుర్తులేని అక్షరం బహిర్గతం[EXT2_FS_SHA512_నిడివి]);

/* swapfs.c */
బాహ్య errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, అక్షరం *buf, size_t కొలత,
					పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_dirent_swab_లోపల(ext2_filsys fs, అక్షరం *buf, పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, అక్షరం *buf, size_t కొలత,
					 పూర్ణాంకం flags);
బాహ్య errcode_t ext2fs_dirent_swab_బహిర్గతం(ext2_filsys fs, అక్షరం *buf, పూర్ణాంకం flags);
బాహ్య శూన్య ext2fs_మార్పిడి_ext_attr(అక్షరం *గమ్యం, అక్షరం *from, పూర్ణాంకం bufsize,
				 పూర్ణాంకం has_శీర్షిక);
బాహ్య శూన్య ext2fs_మార్పిడి_ext_attr_శీర్షిక(నిర్మాణం ext2_ext_attr_శీర్షిక *గమ్యం_శీర్షిక,
					నిర్మాణం ext2_ext_attr_శీర్షిక *from_hdr);
బాహ్య శూన్య ext2fs_మార్పిడి_ext_attr_నమోదు(నిర్మాణం ext2_ext_attr_నమోదు *గమ్యం_నమోదు,
				       నిర్మాణం ext2_ext_attr_నమోదు *from_నమోదు);
బాహ్య శూన్య ext2fs_మార్పిడి_super(నిర్మాణం ext2_super_ఖండం * super);
బాహ్య శూన్య ext2fs_మార్పిడి_సమూహం_desc(నిర్మాణం ext2_సమూహం_desc *gdp);
బాహ్య శూన్య ext2fs_మార్పిడి_సమూహం_desc2(ext2_filsys, నిర్మాణం ext2_సమూహం_desc *gdp);
బాహ్య శూన్య ext2fs_మార్పిడి_inode_నిండిన(ext2_filsys fs, నిర్మాణం ext2_inode_large *t,
				   నిర్మాణం ext2_inode_large *f, పూర్ణాంకం hostorder,
				   పూర్ణాంకం bufsize);
బాహ్య శూన్య ext2fs_మార్పిడి_inode(ext2_filsys fs,నిర్మాణం ext2_inode *t,
			      నిర్మాణం ext2_inode *f, పూర్ణాంకం hostorder);
బాహ్య శూన్య ext2fs_మార్పిడి_mmp(నిర్మాణం mmp_struct *mmp);

/* unix_io.c */
బాహ్య పూర్ణాంకం ext2fs_open_file(స్థిరరాశి అక్షరం *pathname, పూర్ణాంకం flags, mode_t బహుళకం);
బాహ్య పూర్ణాంకం ext2fs_stat(స్థిరరాశి అక్షరం *మార్గం, ext2fs_struct_stat *buf);
బాహ్య పూర్ణాంకం ext2fs_fstat(పూర్ణాంకం fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
బాహ్య పూర్ణాంకం ext2fs_inode_has_valid_ఖండాలు(నిర్మాణం ext2_inode *inode);
బాహ్య పూర్ణాంకం ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  నిర్మాణం ext2_inode *inode);

/* version.c */
బాహ్య పూర్ణాంకం ext2fs_parse_సంస్కరణ_string(స్థిరరాశి అక్షరం *ver_string);
బాహ్య పూర్ణాంకం ext2fs_get_library_సంస్కరణ(స్థిరరాశి అక్షరం **ver_string,
				      స్థిరరాశి అక్షరం **date_string);

/* write_bb_file.c */
బాహ్య errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      గుర్తులేని పూర్ణాంకం flags,
				      FILE *f);


/* inline functions */
#నిర్వచితమైతే NO_INLINE_FUNCS
బాహ్య errcode_t ext2fs_get_mem(గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య errcode_t ext2fs_get_memzero(గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య errcode_t ext2fs_get_array(గుర్తులేని దీర్ఘ లెక్క,
				  గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య errcode_t ext2fs_get_arrayzero(గుర్తులేని దీర్ఘ లెక్క,
				      గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య errcode_t ext2fs_free_mem(శూన్య *ptr);
బాహ్య errcode_t ext2fs_resize_mem(గుర్తులేని దీర్ఘ old_కొలత,
				   గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య errcode_t ext2fs_resize_array(గుర్తులేని దీర్ఘ old_లెక్క, గుర్తులేని దీర్ఘ లెక్క,
				     గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr);
బాహ్య శూన్య ext2fs_mark_super_dirty(ext2_filsys fs);
బాహ్య శూన్య ext2fs_mark_మారింది(ext2_filsys fs);
బాహ్య పూర్ణాంకం ext2fs_test_మారింది(ext2_filsys fs);
బాహ్య శూన్య ext2fs_mark_valid(ext2_filsys fs);
బాహ్య శూన్య ext2fs_unmark_valid(ext2_filsys fs);
బాహ్య పూర్ణాంకం ext2fs_test_valid(ext2_filsys fs);
బాహ్య శూన్య ext2fs_mark_ib_dirty(ext2_filsys fs);
బాహ్య శూన్య ext2fs_mark_bb_dirty(ext2_filsys fs);
బాహ్య పూర్ణాంకం ext2fs_test_ib_dirty(ext2_filsys fs);
బాహ్య పూర్ణాంకం ext2fs_test_bb_dirty(ext2_filsys fs);
బాహ్య dgrp_t ext2fs_సమూహం_యొక్క_blk(ext2_filsys fs, blk_t blk);
బాహ్య dgrp_t ext2fs_సమూహం_యొక్క_ino(ext2_filsys fs, ext2_ino_t ino);
బాహ్య blk_t ext2fs_సమూహం_మొదటి_ఖండం(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk_t ext2fs_సమూహం_చివరి_ఖండం(ext2_filsys fs, dgrp_t సమూహం);
బాహ్య blk_t ext2fs_inode_దత్తాంశం_ఖండాలు(ext2_filsys fs,
				      నిర్మాణం ext2_inode *inode);
బాహ్య పూర్ణాంకం ext2fs_htree_intnode_maxrecs(ext2_filsys fs, పూర్ణాంకం ఖండాలు);
బాహ్య పూర్ణాంకం ext2fs_log2_u32(__u32 arg);
బాహ్య పూర్ణాంకం ext2fs_log2_u64(__u64 arg);
బాహ్య పూర్ణాంకం ext2fs_log10_u32(__u32 arg);
బాహ్య పూర్ణాంకం ext2fs_log10_u64(__u64 arg);
బాహ్య గుర్తులేని పూర్ణాంకం ext2fs_div_ceil(గుర్తులేని పూర్ణాంకం a, గుర్తులేని పూర్ణాంకం b);
బాహ్య __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
బాహ్య పూర్ణాంకం ext2fs_dirent_name_len(స్థిరరాశి నిర్మాణం ext2_dir_నమోదు *నమోదు_2);
బాహ్య శూన్య ext2fs_dirent_సమితి_name_len(నిర్మాణం ext2_dir_నమోదు *నమోదు_2, పూర్ణాంకం len);
బాహ్య పూర్ణాంకం ext2fs_dirent_file_type(స్థిరరాశి నిర్మాణం ext2_dir_నమోదు *నమోదు_2);
బాహ్య శూన్య ext2fs_dirent_సమితి_file_type(నిర్మాణం ext2_dir_నమోదు *నమోదు_2, పూర్ణాంకం type);
బాహ్య నిర్మాణం ext2_inode *ext2fs_inode(నిర్మాణం ext2_inode_large * large_inode);
బాహ్య స్థిరరాశి నిర్మాణం ext2_inode *ext2fs_const_inode(స్థిరరాశి నిర్మాణం ext2_inode_large * large_inode);
బాహ్య పూర్ణాంకం ext2fs_inodes_చొప్పున_orphan_ఖండం(ext2_filsys fs);
బాహ్య నిర్మాణం ext4_orphan_ఖండం_తోక *ext2fs_orphan_ఖండం_తోక(ext2_filsys fs,
							       అక్షరం *buf);
#అయితే_ముగింపు

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#ఒకవేళ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#నిర్వచితమైతే INCLUDE_INLINE_FUNCS
#నిర్వచించు _INLINE_ బాహ్య
#లేకపోతే
#ఒకవేళ (__STDC_VERSION__ >= 199901L)
#నిర్వచించు _INLINE_ అంతఃవరుస
#లేకపోతే
#నిర్వచితమైతే __GNUC__
#నిర్వచించు _INLINE_ బాహ్య __inline__
#లేకపోతే				/* For Watcom C */
#నిర్వచించు _INLINE_ బాహ్య అంతఃవరుస
#అయితే_ముగింపు /* __GNUC__ */
#అయితే_ముగింపు /* __STDC_VERSION__ >= 199901L */
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే EXT2_CUSTOM_జ్ఞాపకం_ROUTINES
#అన్తర్భావ <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr)
{
	శూన్య *pp;

	pp = జ్ఞాపకం_ఇవ్వు(కొలత);
	ఒకవేళ (!pp)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	జ్ఞాపకం_నకలు(ptr, &pp, పరిమాణం (pp));
	తిరిగి 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr)
{
	శూన్య *pp;

	pp = జ్ఞాపకం_ఇవ్వు(కొలత);
	ఒకవేళ (!pp)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	memset(pp, 0, కొలత);
	జ్ఞాపకం_నకలు(ptr, &pp, పరిమాణం(pp));
	తిరిగి 0;
}

_INLINE_ errcode_t ext2fs_get_array(గుర్తులేని దీర్ఘ లెక్క, గుర్తులేని దీర్ఘ కొలత,
				    శూన్య *ptr)
{
	ఒకవేళ (లెక్క && (~0UL)/లెక్క < కొలత)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	తిరిగి ext2fs_get_mem(లెక్క*కొలత, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(గుర్తులేని దీర్ఘ లెక్క,
					గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr)
{
	ఒకవేళ (లెక్క && (~0UL)/లెక్క < కొలత)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	తిరిగి ext2fs_get_memzero((size_t)లెక్క * కొలత, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(శూన్య *ptr)
{
	శూన్య *p;

	జ్ఞాపకం_నకలు(&p, ptr, పరిమాణం(p));
	విడుదల(p);
	p = 0;
	జ్ఞాపకం_నకలు(ptr, &p, పరిమాణం(p));
	తిరిగి 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(గుర్తులేని దీర్ఘ EXT2FS_ATTR((వాడని)) old_కొలత,
				     గుర్తులేని దీర్ఘ కొలత, శూన్య *ptr)
{
	శూన్య *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	జ్ఞాపకం_నకలు(&p, ptr, పరిమాణం(p));
	p = realloc(p, కొలత);
	ఒకవేళ (!p)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	జ్ఞాపకం_నకలు(ptr, &p, పరిమాణం(p));
	తిరిగి 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(గుర్తులేని దీర్ఘ కొలత,
				       గుర్తులేని దీర్ఘ old_లెక్క,
				       గుర్తులేని దీర్ఘ లెక్క, శూన్య *ptr)
{
	గుర్తులేని దీర్ఘ old_కొలత;
	errcode_t retval;

	ఒకవేళ (లెక్క && (~0UL)/లెక్క < కొలత)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	కొలత *= లెక్క;
	old_కొలత = కొలత * old_లెక్క;
	retval = ext2fs_resize_mem(old_కొలత, కొలత, ptr);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (కొలత > old_కొలత) {
		శూన్య *p;

		జ్ఞాపకం_నకలు(&p, ptr, పరిమాణం(p));
		memset((అక్షరం *)p + old_కొలత, 0, కొలత - old_కొలత);
		జ్ఞాపకం_నకలు(ptr, &p, పరిమాణం(p));
	}

	తిరిగి 0;
}
#అయితే_ముగింపు	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ శూన్య ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_మారింది;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ శూన్య ext2fs_mark_మారింది(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_మారింది;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ పూర్ణాంకం ext2fs_test_మారింది(ext2_filsys fs)
{
	తిరిగి (fs->flags & EXT2_FLAG_మారింది);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ శూన్య ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ శూన్య ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ పూర్ణాంకం ext2fs_test_valid(ext2_filsys fs)
{
	తిరిగి (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ శూన్య ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_మారింది;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ శూన్య ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_మారింది;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ పూర్ణాంకం ext2fs_test_ib_dirty(ext2_filsys fs)
{
	తిరిగి (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ పూర్ణాంకం ext2fs_test_bb_dirty(ext2_filsys fs)
{
	తిరిగి (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_సమూహం_యొక్క_blk(ext2_filsys fs, blk_t blk)
{
	తిరిగి ext2fs_సమూహం_యొక్క_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_సమూహం_యొక్క_ino(ext2_filsys fs, ext2_ino_t ino)
{
	తిరిగి (ino - 1) / fs->super->s_inodes_చొప్పున_సమూహం;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_సమూహం_మొదటి_ఖండం(ext2_filsys fs, dgrp_t సమూహం)
{
	తిరిగి (blk_t) ext2fs_సమూహం_మొదటి_block2(fs, సమూహం);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_సమూహం_చివరి_ఖండం(ext2_filsys fs, dgrp_t సమూహం)
{
	తిరిగి (blk_t) ext2fs_సమూహం_చివరి_block2(fs, సమూహం);
}

_INLINE_ blk_t ext2fs_inode_దత్తాంశం_ఖండాలు(ext2_filsys fs,
					నిర్మాణం ext2_inode *inode)
{
	తిరిగి (blk_t) ext2fs_inode_దత్తాంశం_blocks2(fs, inode);
}

_INLINE_ పూర్ణాంకం ext2fs_htree_intnode_maxrecs(ext2_filsys fs, పూర్ణాంకం ఖండాలు)
{
	పూర్ణాంకం csum_కొలత = 0;

	ఒకవేళ ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dx_తోక);
	తిరిగి ఖండాలు * ((fs->blocksize - (8 + csum_కొలత)) /
						పరిమాణం(నిర్మాణం ext2_dx_నమోదు));
}

/*
 * log base 2, rounded down
 */
_INLINE_ పూర్ణాంకం ext2fs_log2_u32(__u32 arg)
{
	పూర్ణాంకం l = 0;

	arg >>= 1;
	అయితే (arg) {
		l++;
		arg >>= 1;
	}
	తిరిగి l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ పూర్ణాంకం ext2fs_log2_u64(__u64 arg)
{
	పూర్ణాంకం l = 0;

	arg >>= 1;
	అయితే (arg) {
		l++;
		arg >>= 1;
	}
	తిరిగి l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ పూర్ణాంకం ext2fs_log10_u32(__u32 arg)
{
	పూర్ణాంకం l = 0;

	arg /= 10;
	అయితే (arg) {
		l++;
		arg /= 10;
	}
	తిరిగి l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ పూర్ణాంకం ext2fs_log10_u64(__u64 arg)
{
	పూర్ణాంకం l = 0;

	arg /= 10;
	అయితే (arg) {
		l++;
		arg /= 10;
	}
	తిరిగి l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ గుర్తులేని పూర్ణాంకం ext2fs_div_ceil(గుర్తులేని పూర్ణాంకం a, గుర్తులేని పూర్ణాంకం b)
{
	ఒకవేళ (!a)
		తిరిగి 0;
	తిరిగి ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	ఒకవేళ (!a)
		తిరిగి 0;
	తిరిగి ((a - 1) / b) + 1;
}

_INLINE_ పూర్ణాంకం ext2fs_dirent_name_len(స్థిరరాశి నిర్మాణం ext2_dir_నమోదు *నమోదు_2)
{
	తిరిగి నమోదు_2->name_len & 0xff;
}

_INLINE_ శూన్య ext2fs_dirent_సమితి_name_len(నిర్మాణం ext2_dir_నమోదు *నమోదు_2, పూర్ణాంకం len)
{
	నమోదు_2->name_len = (నమోదు_2->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ పూర్ణాంకం ext2fs_dirent_file_type(స్థిరరాశి నిర్మాణం ext2_dir_నమోదు *నమోదు_2)
{
	తిరిగి నమోదు_2->name_len >> 8;
}

_INLINE_ శూన్య ext2fs_dirent_సమితి_file_type(నిర్మాణం ext2_dir_నమోదు *నమోదు_2, పూర్ణాంకం type)
{
	నమోదు_2->name_len = (నమోదు_2->name_len & 0xff) | (type << 8);
}

_INLINE_ నిర్మాణం ext2_inode *ext2fs_inode(నిర్మాణం ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	తిరిగి (నిర్మాణం ext2_inode *) large_inode;
}

_INLINE_ స్థిరరాశి నిర్మాణం ext2_inode *
ext2fs_const_inode(స్థిరరాశి నిర్మాణం ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	తిరిగి (స్థిరరాశి నిర్మాణం ext2_inode *) large_inode;
}

_INLINE_ పూర్ణాంకం ext2fs_inodes_చొప్పున_orphan_ఖండం(ext2_filsys fs)
{
	తిరిగి (fs->blocksize - పరిమాణం(నిర్మాణం ext4_orphan_ఖండం_తోక)) /
		పరిమాణం(__u32);
}

_INLINE_ నిర్మాణం ext4_orphan_ఖండం_తోక *
ext2fs_orphan_ఖండం_తోక(ext2_filsys fs, అక్షరం *buf)
{
	తిరిగి (నిర్మాణం ext4_orphan_ఖండం_తోక *)(buf + fs->blocksize -
		పరిమాణం(నిర్మాణం ext4_orphan_ఖండం_తోక));
}

#నిర్వచనం_రద్దు _INLINE_
#అయితే_ముగింపు

/* htree levels for ext4 */
#నిర్వచించు EXT4_HTREE_LEVEL_COMPAT 2
#నిర్వచించు EXT4_HTREE_LEVEL	3

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం ext2_dir_htree_level(ext2_filsys fs)
{
	ఒకవేళ (ext2fs_has_feature_largedir(fs->super))
		తిరిగి EXT4_HTREE_LEVEL;

	తిరిగి EXT4_HTREE_LEVEL_COMPAT;
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
స్థిర అంతఃవరుస శూన్య ext2fs_సమితి_dtime(ext2_filsys fs, నిర్మాణం ext2_inode *inode)
{
	__u32	t;

	ఒకవేళ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_సమయం))
		t = fs->now & 0xFFFFFFFF;
	లేకపోతే
		t = time(ఖాళీ) & 0xFFFFFFFF;
	ఒకవేళ (t < fs->super->s_inodes_లెక్క)
		t = fs->super->s_inodes_లెక్క;
	inode->i_dtime = t;
}

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు /* _EXT2FS_EXT2FS_H */
