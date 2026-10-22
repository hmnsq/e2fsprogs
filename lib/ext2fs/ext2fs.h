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

#यदि_अपरिभाषितम् _EXT2FS_EXT2FS_H
#परिभाषय _EXT2FS_EXT2FS_H

#यदि_परिभाषितम् __GNUC__
#परिभाषय EXT2FS_ATTR(x) __attribute__(x)
#अन्यथा
#परिभाषय EXT2FS_ATTR(x)
#यदि_अन्तः

#यदि_अपरिभाषितम् __nonstring
#यदि_परिभाषितम् __has_attribute
#यदि __has_attribute(__nonstring__)
#परिभाषय __nonstring                    __attribute__((__nonstring__))
#अन्यथा
#परिभाषय __nonstring
#यदि_अन्तः /* __has_attribute(__nonstring__) */
#अन्यथा
# define __nonstring
#यदि_अन्तः /* __has_attribute */
#यदि_अन्तः /* __nonstring */

#यदि_परिभाषितम् CONFIG_TDB
#परिभाषय EXT2_FS_NO_TDB_अप्रयुक्तम्
#अन्यथा
#परिभाषय EXT2_FS_NO_TDB_अप्रयुक्तम्	EXT2FS_ATTR((अप्रयुक्तम्))
#यदि_अन्तः

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#यदि (!defined(__GNUC__) && !defined(__WATCOMC__))
#परिभाषय NO_INLINE_FUNCS
#यदि_अन्तः

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#परिभाषय SUPERBLOCK_विस्थापनम्	1024
#परिभाषय SUPERBLOCK_आकार		1024

#परिभाषय UUID_STR_आकार 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#परिभाषय EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#यदि_परिभाषितम् HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <errno.h>

#यदि EXT2_FLAT_INCLUDES
#अन्तर्भाव "e2_types.h"
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext3_extents.h"
#अन्यथा
#अन्तर्भाव <ext2fs/ext2_types.h>
#अन्तर्भाव <ext2fs/ext2_fs.h>
#अन्तर्भाव <ext2fs/ext3_extents.h>
#यदि_अन्तः /* EXT2_FLAT_INCLUDES */

प्रकारनिर्देश __u32 __bitwise		ext2_ino_t;
प्रकारनिर्देश __u32 __bitwise		blk_t;
प्रकारनिर्देश __u64 __bitwise		blk64_t;
प्रकारनिर्देश __u32 __bitwise		dgrp_t;
प्रकारनिर्देश __s32 __bitwise		ext2_off_t;
प्रकारनिर्देश __s64 __bitwise		ext2_off64_t;
प्रकारनिर्देश __s64 __bitwise		e2_blkcnt_t;
प्रकारनिर्देश __u32 __bitwise		ext2_dirhash_t;

#यदि EXT2_FLAT_INCLUDES
#अन्तर्भाव "com_err.h"
#अन्तर्भाव "ext2_io.h"
#अन्तर्भाव "ext2_err.h"
#अन्तर्भाव "ext2_ext_attr.h"
#अन्यथा
#अन्तर्भाव <et/com_err.h>
#अन्तर्भाव <ext2fs/ext2_io.h>
#अन्तर्भाव <ext2fs/ext2_err.h>
#अन्तर्भाव <ext2fs/ext2_ext_attr.h>
#यदि_अन्तः

#अन्तर्भाव "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#यदि_परिभाषितम् _MSC_VER
#परिभाषय EXT2_QSORT_TYPE पूर्णाङ्क __cdecl
#अन्यथा
#परिभाषय EXT2_QSORT_TYPE पूर्णाङ्क
#यदि_अन्तः

प्रकारनिर्देश संरचना struct_ext2_filsys *ext2_filsys;

#परिभाषय EXT2FS_MARK_ERROR 	0
#परिभाषय EXT2FS_UNMARK_ERROR 	1
#परिभाषय EXT2FS_TEST_ERROR	2

संरचना ext2fs_struct_generic_bitmap_आधारः {
	errcode_t		magic;
	ext2_filsys 		fs;
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_आधारः *ext2fs_generic_bitmap;
प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_आधारः *ext2fs_inode_bitmap;
प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_आधारः *ext2fs_खण्डम्_bitmap;

#परिभाषय EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

प्रकारनिर्देश संरचना ext2_struct_u32_list *ext2_badblocks_list;
प्रकारनिर्देश संरचना ext2_struct_u32_iterate *ext2_badblocks_iterate;

प्रकारनिर्देश संरचना ext2_struct_u32_list *ext2_u32_list;
प्रकारनिर्देश संरचना ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
प्रकारनिर्देश संरचना ext2_struct_u32_list *badblocks_list;
प्रकारनिर्देश संरचना ext2_struct_u32_iterate *badblocks_iterate;

#परिभाषय BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
संरचना ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
संरचना ext2_db_प्रविष्टिः {
	ext2_ino_t	ino;
	blk_t	blk;
	पूर्णाङ्क	blockcnt;
};

प्रकारनिर्देश संरचना ext2_struct_dblist *ext2_dblist;

#परिभाषय DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#परिभाषय EXT2_FILE_WRITE		0x0001
#परिभाषय EXT2_FILE_CREATE	0x0002

#परिभाषय EXT2_FILE_आवरणम्		0x00FF

#परिभाषय EXT2_FILE_BUF_DIRTY	0x4000
#परिभाषय EXT2_FILE_BUF_VALID	0x2000

प्रकारनिर्देश संरचना ext2_file *ext2_file_t;

#परिभाषय EXT2_SEEK_समुच्चयः	0
#परिभाषय EXT2_SEEK_CUR	1
#परिभाषय EXT2_SEEK_अन्तः	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#परिभाषय EXT2_FLAG_RW			0x01
#परिभाषय EXT2_FLAG_परिवर्तितम्		0x02
#परिभाषय EXT2_FLAG_DIRTY			0x04
#परिभाषय EXT2_FLAG_VALID			0x08
#परिभाषय EXT2_FLAG_IB_DIRTY		0x10
#परिभाषय EXT2_FLAG_BB_DIRTY		0x20
#परिभाषय EXT2_FLAG_SWAP_अष्टकानि		0x40
#परिभाषय EXT2_FLAG_SWAP_अष्टकानि_READ	0x80
#परिभाषय EXT2_FLAG_SWAP_अष्टकानि_WRITE	0x100
#परिभाषय EXT2_FLAG_MASTER_SB_ONLY	0x200
#परिभाषय EXT2_FLAG_FORCE			0x400
#परिभाषय EXT2_FLAG_SUPER_ONLY		0x800
#परिभाषय EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#परिभाषय EXT2_FLAG_IMAGE_FILE		0x2000
#परिभाषय EXT2_FLAG_EXCLUSIVE		0x4000
#परिभाषय EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#परिभाषय EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#परिभाषय EXT2_FLAG_64_द्वयङ्काः		0x20000
#परिभाषय EXT2_FLAG_PRINT_प्रगतिः	0x40000
#परिभाषय EXT2_FLAG_प्रत्यक्षम्_IO		0x80000
#परिभाषय EXT2_FLAG_SKIP_MMP		0x100000
#परिभाषय EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#परिभाषय EXT2_FLAG_SHARE_DUP		0x400000
#परिभाषय EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#परिभाषय EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM	0x1000000
#परिभाषय EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM	0x2000000
#परिभाषय EXT2_FLAG_तन्तवः		0x4000000
#परिभाषय EXT2_FLAG_IGNORE_SWAP_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#परिभाषय EXT2_FLAG2_USE_FAKE_TIME	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#परिभाषय EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#परिभाषय EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#परिभाषय EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#परिभाषय EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#परिभाषय EXT2_JOURNAL_गन्तव्यम्_FC_BLKS_अनुपातः		64

संरचना blk_alloc_ctx;
संरचना opaque_ext2_गणः_desc;

संरचना struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	पूर्णाङ्क				flags;
	वर्ण *				device_name;
	संरचना ext2_super_खण्डम्	* 	super;
	अचिह्नित पूर्णाङ्क			blocksize;
	पूर्णाङ्क				fragsize;
	dgrp_t				गणः_desc_गणनम्;
	अचिह्नित दीर्घ			desc_खण्डानि;
	संरचना opaque_ext2_गणः_desc *	गणः_desc;
	अचिह्नित पूर्णाङ्क			inode_खण्डानि_प्रत्येकम्_गणः;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_खण्डम्_bitmap		खण्डम्_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_खण्डानि)(ext2_filsys fs, ext2_ino_t ino, blk_t *खण्डानि);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				संरचना ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				संरचना ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	संरचना ext2_super_खण्डम् *	orig_super;
	संरचना ext2_image_hdr *		image_शीर्षकम्;
	__u32				umask;
	time_t				now;
	पूर्णाङ्क				cluster_अनुपातः_द्वयङ्काः;
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
	शून्य *				priv_दत्त;

	/*
	 * Inode cache
	 */
	संरचना ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_खण्डम्)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, संरचना blk_alloc_ctx *ctx);
	शून्य (*खण्डम्_alloc_stats)(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	शून्य *mmp_buf;
	शून्य *mmp_cmp;
	पूर्णाङ्क mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	दीर्घ mmp_चरमः_written;

	/* progress operation functions */
	संरचना ext2fs_प्रगतिः_ops *प्रगतिः_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_बीजम्;

	io_channel			journal_io;
	वर्ण				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	शून्य (*खण्डम्_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t अङ्कः,
					पूर्णाङ्क inuse);

	/* hashmap for SHA of data blocks */
	संरचना ext2fs_hashmap* खण्डम्_sha_map;

	ध्रुव संरचना ext2fs_nls_सारणी *encoding;
};

#यदि EXT2_FLAT_INCLUDES
#अन्तर्भाव "e2_bitops.h"
#अन्यथा
#अन्तर्भाव <ext2fs/bitops.h>
#यदि_अन्तः

/*
 * 64-bit bitmap backend types
 */
#परिभाषय EXT2FS_BMAP64_BITARRAY	1
#परिभाषय EXT2FS_BMAP64_RBTREE	2
#परिभाषय EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#परिभाषय खण्डम्_परिवर्तितम्			1
#परिभाषय खण्डम्_ABORT			2
#परिभाषय खण्डम्_ERROR			4
#परिभाषय खण्डम्_INLINE_दत्त_परिवर्तितम्	8

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
#परिभाषय खण्डम्_FLAG_APPEND	1
#परिभाषय खण्डम्_FLAG_HOLE		1
#परिभाषय खण्डम्_FLAG_गभीरता_TRAVERSE	2
#परिभाषय खण्डम्_FLAG_दत्त_ONLY	4
#परिभाषय खण्डम्_FLAG_READ_ONLY	8

#परिभाषय खण्डम्_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#परिभाषय खण्डम्_गणनम्_IND		(-1)
#परिभाषय खण्डम्_गणनम्_DIND	(-2)
#परिभाषय खण्डम्_गणनम्_TIND	(-3)
#परिभाषय खण्डम्_गणनम्_TRANSLATOR	(-4)

#परिभाषय खण्डम्_ALLOC_UNKNOWN	0
#परिभाषय खण्डम्_ALLOC_दत्त	1
#परिभाषय खण्डम्_ALLOC_METADATA	2

संरचना blk_alloc_ctx {
	ext2_ino_t		ino;
	संरचना ext2_inode	*inode;
	blk64_t			lblk;
	पूर्णाङ्क			flags;
};

#यदि 0
/*
 * Flags for ext2fs_move_blocks
 */
#परिभाषय EXT2_BMOVE_GET_DBLIST	0x0001
#परिभाषय EXT2_BMOVE_DEBUG	0x0002
#यदि_अन्तः

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#परिभाषय EXT2_EXTENT_FLAGS_पर्णम्		0x0001
#परिभाषय EXT2_EXTENT_FLAGS_UNINIT	0x0002
#परिभाषय EXT2_EXTENT_FLAGS_SECOND_दर्शय	0x0004

संरचना ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

प्रकारनिर्देश संरचना ext2_extent_handle *ext2_extent_handle_t;
प्रकारनिर्देश संरचना ext2_extent_मार्गः *ext2_extent_मार्गः_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#परिभाषय EXT2_EXTENT_CURRENT	0x0000
#परिभाषय EXT2_EXTENT_MOVE_आवरणम्	0x000F
#परिभाषय EXT2_EXTENT_मूलम्	0x0001
#परिभाषय EXT2_EXTENT_चरमः_पर्णम्	0x0002
#परिभाषय EXT2_EXTENT_FIRST_SIB	0x0003
#परिभाषय EXT2_EXTENT_चरमः_SIB	0x0004
#परिभाषय EXT2_EXTENT_अग्र_SIB	0x0005
#परिभाषय EXT2_EXTENT_पूर्व_SIB	0x0006
#परिभाषय EXT2_EXTENT_अग्र_पर्णम्	0x0007
#परिभाषय EXT2_EXTENT_पूर्व_पर्णम्	0x0008
#परिभाषय EXT2_EXTENT_अग्र	0x0009
#परिभाषय EXT2_EXTENT_पूर्व	0x000A
#परिभाषय EXT2_EXTENT_ऊर्ध्वम्		0x000B
#परिभाषय EXT2_EXTENT_अधस्तात्	0x000C
#परिभाषय EXT2_EXTENT_अधस्तात्_AND_चरमः 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#परिभाषय EXT2_EXTENT_निवेशय_AFTER	0x0001 /* insert after handle loc'n */
#परिभाषय EXT2_EXTENT_निवेशय_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#परिभाषय EXT2_EXTENT_DELETE_KEEP_रिक्तानि	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#परिभाषय EXT2_EXTENT_समुच्चयः_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
संरचना ext2_extent_info {
	पूर्णाङ्क		curr_प्रविष्टिः;
	पूर्णाङ्क		curr_level;
	पूर्णाङ्क		अङ्कः_entries;
	पूर्णाङ्क		max_entries;
	पूर्णाङ्क		max_गभीरता;
	पूर्णाङ्क		अष्टकानि_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#परिभाषय EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#परिभाषय DIRENT_परिवर्तितम्	1
#परिभाषय DIRENT_ABORT	2
#परिभाषय DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#परिभाषय DIRENT_FLAG_INCLUDE_रिक्तानि	1
#परिभाषय DIRENT_FLAG_INCLUDE_REMOVED	2
#परिभाषय DIRENT_FLAG_INCLUDE_CSUM	4
#परिभाषय DIRENT_FLAG_INCLUDE_INLINE_दत्त 8

#परिभाषय DIRENT_DOT_FILE		1
#परिभाषय DIRENT_DOT_DOT_FILE	2
#परिभाषय DIRENT_OTHER_FILE	3
#परिभाषय DIRENT_DELETED_FILE	4
#परिभाषय DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
प्रकारनिर्देश संरचना ext2_struct_inode_अवलोकनम् *ext2_inode_अवलोकनम्;

/*
 * ext2fs_scan flags
 */
#परिभाषय EXT2_SF_CHK_BADBLOCKS	0x0001
#परिभाषय EXT2_SF_BAD_INODE_BLK	0x0002
#परिभाषय EXT2_SF_BAD_EXTRA_अष्टकानि	0x0004
#परिभाषय EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#परिभाषय EXT2_SF_DO_LAZY		0x0010
#परिभाषय EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#परिभाषय EXT2_MF_MOUNTED		1
#परिभाषय EXT2_MF_ISROOT		2
#परिभाषय EXT2_MF_READONLY	4
#परिभाषय EXT2_MF_SWAP		8
#परिभाषय EXT2_MF_BUSY		16
#परिभाषय EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#परिभाषय LINUX_S_IFMT  00170000
#परिभाषय LINUX_S_IFSOCK 0140000
#परिभाषय LINUX_S_IFLNK	 0120000
#परिभाषय LINUX_S_IFREG  0100000
#परिभाषय LINUX_S_IFBLK  0060000
#परिभाषय LINUX_S_IFDIR  0040000
#परिभाषय LINUX_S_IFCHR  0020000
#परिभाषय LINUX_S_IFIFO  0010000
#परिभाषय LINUX_S_ISUID  0004000
#परिभाषय LINUX_S_ISGID  0002000
#परिभाषय LINUX_S_ISVTX  0001000

#परिभाषय LINUX_S_IRWXU 00700
#परिभाषय LINUX_S_IRUSR 00400
#परिभाषय LINUX_S_IWUSR 00200
#परिभाषय LINUX_S_IXUSR 00100

#परिभाषय LINUX_S_IRWXG 00070
#परिभाषय LINUX_S_IRGRP 00040
#परिभाषय LINUX_S_IWGRP 00020
#परिभाषय LINUX_S_IXGRP 00010

#परिभाषय LINUX_S_IRWXO 00007
#परिभाषय LINUX_S_IROTH 00004
#परिभाषय LINUX_S_IWOTH 00002
#परिभाषय LINUX_S_IXOTH 00001

#परिभाषय LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#परिभाषय LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#परिभाषय LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#परिभाषय LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#परिभाषय LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#परिभाषय LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#परिभाषय LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#परिभाषय EXT2_I_आकार(i)	((i)->i_आकार | ((__u64) (i)->i_आकार_उच्चः << 32))

स्थिर अन्तःस्थ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((अप्रयुक्तम्)),
					__u32 nsec)
{
	__u32 extra = 0;

#यदि (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_आवरणम्;
#यदि_अन्तः
	प्रत्यागम extra | (nsec << EXT4_EPOCH_द्वयङ्काः);
}
स्थिर अन्तःस्थ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((अप्रयुक्तम्)))
{
#यदि (SIZEOF_TIME_T > 4)
	यदि (extra & EXT4_EPOCH_आवरणम्)
		seconds += ((time_t)(extra & EXT4_EPOCH_आवरणम्) << 32);
#यदि_अन्तः
	प्रत्यागम seconds;
}
स्थिर अन्तःस्थ __u32 __decode_extra_nsec(__u32 extra)
{
	प्रत्यागम (extra & EXT4_NSEC_आवरणम्) >> EXT4_EPOCH_द्वयङ्काः;
}
#परिभाषय ext2fs_inode_actual_आकार(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_आकार +				      \
		  (परिमाण(*inode) > EXT2_GOOD_OLD_INODE_आकार ?		      \
		   ((संरचना ext2_inode_large *)(inode))->i_extra_isize : 0)))
#परिभाषय clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#परिभाषय ext2fs_inode_xtime_समुच्चयः(inode, क्षेत्रम्, sec)			      \
कुरु {									      \
	यदि (ext2fs_inode_includes(ext2fs_inode_actual_आकार(inode),	      \
				  क्षेत्रम् ## _extra)) {			      \
		(inode)->क्षेत्रम् = (__u32)(sec & 0xfffffff);		      \
		((संरचना ext2_inode_large *)(inode))->क्षेत्रम् ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} अन्यथा {							      \
		(inode)->क्षेत्रम् = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} यावत् (0)
#परिभाषय ext2fs_inode_xtime_get(inode, क्षेत्रम्)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_आकार(inode), क्षेत्रम् ## _extra) ?    \
	__decode_extra_sec((inode)->क्षेत्रम्,				      \
		((संरचना ext2_inode_large *)(inode))->क्षेत्रम् ## _extra) :      \
		(time_t)(inode)->क्षेत्रम्)

स्थिर अन्तःस्थ शून्य __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((अप्रयुक्तम्)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#यदि (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_आवरणम्;
#अन्यथा
	*hi = 0;
#यदि_अन्तः
}
स्थिर अन्तःस्थ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((अप्रयुक्तम्)))
{
#यदि (SIZEOF_TIME_T == 4)
	प्रत्यागम *lo;
#अन्यथा
	प्रत्यागम ((time_t)(*hi) << 32) | *lo;
#यदि_अन्तः
}
#परिभाषय ext2fs_समुच्चयः_tstamp(sb, क्षेत्रम्, seconds) \
	__sb_set_tstamp(&(sb)->क्षेत्रम्, &(sb)->क्षेत्रम् ## _hi, seconds)
#परिभाषय ext2fs_get_tstamp(sb, क्षेत्रम्) \
	__sb_get_tstamp(&(sb)->क्षेत्रम्, &(sb)->क्षेत्रम् ## _hi)

/*
 * ext2_icount_t abstraction
 */
#परिभाषय EXT2_ICOUNT_OPT_INCREMENT	0x01
#परिभाषय EXT2_ICOUNT_OPT_FULLMAP		0x02

प्रकारनिर्देश संरचना ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#परिभाषय BMAP_ALLOC	0x0001
#परिभाषय BMAP_समुच्चयः	0x0002
#परिभाषय BMAP_UNINIT	0x0004
#परिभाषय BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#परिभाषय BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#परिभाषय READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#परिभाषय WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#परिभाषय IMAGER_FLAG_INODEMAP	1
#परिभाषय IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)

/*
 * Features supported by this version of the library
 */
#परिभाषय EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_सूचकांक|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#यदि_परिभाषितम् CONFIG_MMP
#परिभाषय EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#अन्यथा
#परिभाषय EXT4_LIB_INCOMPAT_MMP		(0)
#यदि_अन्तः

#परिभाषय EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64BIT|\
					 EXT4_FEATURE_INCOMPAT_INLINE_दत्त|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_बीजम्|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#परिभाषय EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_खण्डानि |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#परिभाषय EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#परिभाषय EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#परिभाषय EXT2_FS_CLUSTER_अनुपातः(fs)	(1 << (fs)->cluster_अनुपातः_द्वयङ्काः)
#परिभाषय EXT2_FS_CLUSTER_आवरणम्(fs)		(EXT2_FS_CLUSTER_अनुपातः(fs) - 1)
#परिभाषय EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_अनुपातः_द्वयङ्काः)
/* Translate a cluster number to a block number */
#परिभाषय EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_अनुपातः_द्वयङ्काः)
/* Translate # of blks to # of clusters */
#परिभाषय EXT2_FS_अङ्कः_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_आवरणम्(fs)) >> \
					 (fs)->cluster_अनुपातः_द्वयङ्काः)

#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
प्रकारनिर्देश संरचना stat64 ext2fs_struct_stat;
#अन्यथा
प्रकारनिर्देश संरचना stat ext2fs_struct_stat;
#यदि_अन्तः

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#परिभाषय EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
संरचना ext2_xattr_handle;
#परिभाषय XATTR_ABORT	1
#परिभाषय XATTR_परिवर्तितम्	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#परिभाषय EXT2FS_BITMAPS_WRITE		0x0001
#परिभाषय EXT2_FS_BITMAPS_खण्डम्		0x0002
#परिभाषय EXT2FS_BITMAPS_INODE		0x0004
#परिभाषय EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#परिभाषय EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#परिभाषय EXT2_FS_LINK_FT_आवरणम्	0x0007
#परिभाषय EXT2FS_LINK_APPEND	0x0010
#परिभाषय EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_has_गणः_desc_csum(ext2_filsys fs)
{
	प्रत्यागम ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_needs_large_file_feature(अचिह्नित दीर्घ दीर्घ file_आकार)
{
	प्रत्यागम file_आकार >= 0x80000000ULL;
}

/* alloc.c */
बाह्य शून्य ext2fs_clear_खण्डम्_uninit(ext2_filsys fs, dgrp_t गणः);
बाह्य errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, पूर्णाङ्क बहुलकम्,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
बाह्य errcode_t ext2fs_new_खण्डम्(ext2_filsys fs, blk_t goal,
				  ext2fs_खण्डम्_bitmap map, blk_t *ret);
बाह्य errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_खण्डम्_bitmap map, blk64_t *ret);
बाह्य errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_खण्डम्_bitmap map, blk64_t *ret,
				   संरचना blk_alloc_ctx *ctx);
बाह्य errcode_t ext2fs_get_free_खण्डानि(ext2_filsys fs, blk_t start,
					blk_t finish, पूर्णाङ्क अङ्कः,
					ext2fs_खण्डम्_bitmap map,
					blk_t *ret);
बाह्य errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, पूर्णाङ्क अङ्कः,
					 ext2fs_खण्डम्_bitmap map,
					 blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_खण्डम्(ext2_filsys fs, blk_t goal,
				    वर्ण *खण्डम्_buf, blk_t *ret);
बाह्य errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     वर्ण *खण्डम्_buf, blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     वर्ण *खण्डम्_buf, blk64_t *ret,
				     संरचना blk_alloc_ctx *ctx);

बाह्य शून्य ext2fs_समुच्चयः_alloc_खण्डम्_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode *inode, blk64_t lblk);
बाह्य शून्य ext2fs_समुच्चयः_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
बाह्य शून्य ext2fs_समुच्चयः_खण्डम्_alloc_stats_range_callback(ext2_filsys fs,
	शून्य (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse),
	शून्य (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse));
#परिभाषय EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#परिभाषय EXT2_NEWRANGE_MIN_दीर्घता	(0x2)
#परिभाषय EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			   blk64_t len, ext2fs_खण्डम्_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#परिभाषय EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#परिभाषय EXT2_ALLOCRANGE_ZERO_खण्डानि	(0x2)
#परिभाषय EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
बाह्य पूर्णाङ्क ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t गणः,
					ext2fs_खण्डम्_bitmap bmap);
बाह्य errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t गणः,
					       ext2fs_खण्डम्_bitmap bmap,
					       blk_t *desc_खण्डानि);
बाह्य शून्य ext2fs_समुच्चयः_खण्डम्_alloc_stats_callback(ext2_filsys fs,
						  शून्य (*func)(ext2_filsys fs,
							       blk64_t blk,
							       पूर्णाङ्क inuse),
						  शून्य (**old)(ext2_filsys fs,
							       blk64_t blk,
							       पूर्णाङ्क inuse));

/* alloc_stats.c */
शून्य ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, पूर्णाङ्क inuse);
शून्य ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       पूर्णाङ्क inuse, पूर्णाङ्क isdir);
शून्य ext2fs_खण्डम्_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णाङ्क inuse);
शून्य ext2fs_खण्डम्_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse);
शून्य ext2fs_खण्डम्_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse);

/* alloc_tables.c */
बाह्य errcode_t ext2fs_allocate_tables(ext2_filsys fs);
बाह्य errcode_t ext2fs_allocate_गणः_सारणी(ext2_filsys fs, dgrp_t गणः,
					     ext2fs_खण्डम्_bitmap bmap);

/* badblocks.c */
बाह्य errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णाङ्क आकार);
बाह्य errcode_t ext2fs_u32_list_सङ्कलय(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णाङ्क ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णाङ्क ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
बाह्य errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
बाह्य पूर्णाङ्क ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
बाह्य शून्य ext2fs_u32_list_iterate_अन्तः(ext2_u32_iterate iter);
बाह्य errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
बाह्य पूर्णाङ्क ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

बाह्य errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    पूर्णाङ्क आकार);
बाह्य errcode_t ext2fs_badblocks_list_सङ्कलय(ext2_badblocks_list bb,
					   blk_t blk);
बाह्य पूर्णाङ्क ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
बाह्य पूर्णाङ्क ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
बाह्य शून्य ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
बाह्य errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
बाह्य पूर्णाङ्क ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
बाह्य शून्य ext2fs_badblocks_list_iterate_अन्तः(ext2_badblocks_iterate iter);
बाह्य errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
बाह्य पूर्णाङ्क ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
बाह्य पूर्णाङ्क ext2fs_u32_list_गणनम्(ext2_u32_list bb);

/* bb_compat */
बाह्य errcode_t badblocks_list_create(badblocks_list *ret, पूर्णाङ्क आकार);
बाह्य errcode_t badblocks_list_सङ्कलय(badblocks_list bb, blk_t blk);
बाह्य पूर्णाङ्क badblocks_list_test(badblocks_list bb, blk_t blk);
बाह्य errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
बाह्य पूर्णाङ्क badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
बाह्य शून्य badblocks_list_iterate_अन्तः(badblocks_iterate iter);
बाह्य शून्य badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
बाह्य errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
बाह्य शून्य ext2fs_free_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap);
बाह्य शून्य ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
बाह्य errcode_t ext2fs_allocate_खण्डम्_bitmap(ext2_filsys fs,
					      ध्रुव वर्ण *descr,
					      ext2fs_खण्डम्_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ध्रुव वर्ण *descr,
						   ext2fs_खण्डम्_bitmap *ret);
बाह्य पूर्णाङ्क ext2fs_get_bitmap_granularity(ext2fs_खण्डम्_bitmap bitmap);
बाह्य errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ध्रुव वर्ण *descr,
					      ext2fs_inode_bitmap *ret);
बाह्य errcode_t ext2fs_fudge_inode_bitmap_अन्तः(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t अन्तः, ext2_ino_t *oend);
बाह्य errcode_t ext2fs_fudge_खण्डम्_bitmap_अन्तः(ext2fs_खण्डम्_bitmap bitmap,
					       blk_t अन्तः, blk_t *oend);
बाह्य errcode_t ext2fs_fudge_खण्डम्_bitmap_end2(ext2fs_खण्डम्_bitmap bitmap,
					 blk64_t अन्तः, blk64_t *oend);
बाह्य शून्य ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य शून्य ext2fs_clear_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap);
बाह्य errcode_t ext2fs_resize_inode_bitmap(__u32 new_अन्तः, __u32 new_real_अन्तः,
					    ext2fs_inode_bitmap bmap);
बाह्य errcode_t ext2fs_resize_inode_bitmap2(__u64 new_अन्तः,
					     __u64 new_real_अन्तः,
					     ext2fs_inode_bitmap bmap);
बाह्य errcode_t ext2fs_resize_खण्डम्_bitmap(__u32 new_अन्तः, __u32 new_real_अन्तः,
					    ext2fs_खण्डम्_bitmap bmap);
बाह्य errcode_t ext2fs_resize_खण्डम्_bitmap2(__u64 new_अन्तः,
					     __u64 new_real_अन्तः,
					     ext2fs_खण्डम्_bitmap bmap);
बाह्य errcode_t ext2fs_compare_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bm1,
					     ext2fs_खण्डम्_bitmap bm2);
बाह्य errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
बाह्य errcode_t ext2fs_समुच्चयः_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्);
बाह्य errcode_t ext2fs_समुच्चयः_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t अङ्कः,
					 शून्य *अन्तर्);
बाह्य errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्);
बाह्य errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t अङ्कः,
					 शून्य *बहिर्गमनम्);
बाह्य errcode_t ext2fs_समुच्चयः_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bmap,
					blk_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्);
बाह्य errcode_t ext2fs_समुच्चयः_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bmap,
					 blk64_t start, size_t अङ्कः,
					 शून्य *अन्तर्);
बाह्य errcode_t ext2fs_get_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bmap,
					blk_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्);
बाह्य errcode_t ext2fs_get_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bmap,
					 blk64_t start, size_t अङ्कः,
					 शून्य *बहिर्गमनम्);

/* blknum.c */
बाह्य __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t गणः);
बाह्य __u32 ext2fs_खण्डम्_bitmap_checksum(ext2_filsys fs, dgrp_t गणः);
बाह्य dgrp_t ext2fs_गणः_सम्बन्धि_blk2(ext2_filsys fs, blk64_t);
बाह्य blk64_t ext2fs_गणः_first_block2(ext2_filsys fs, dgrp_t गणः);
बाह्य blk64_t ext2fs_गणः_चरमः_block2(ext2_filsys fs, dgrp_t गणः);
बाह्य पूर्णाङ्क ext2fs_गणः_खण्डानि_गणनम्(ext2_filsys fs, dgrp_t गणः);
बाह्य blk64_t ext2fs_inode_दत्त_blocks2(ext2_filsys fs,
					 संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_inode_i_खण्डानि(ext2_filsys fs,
				     संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_get_stat_i_खण्डानि(ext2_filsys fs,
					संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super);
बाह्य शून्य ext2fs_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super,
				    blk64_t blk);
बाह्य शून्य ext2fs_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super,
				    blk64_t blk);
बाह्य blk64_t ext2fs_r_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super);
बाह्य शून्य ext2fs_r_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super,
				      blk64_t blk);
बाह्य शून्य ext2fs_r_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super,
				      blk64_t blk);
बाह्य blk64_t ext2fs_free_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super);
बाह्य शून्य ext2fs_free_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super,
					 blk64_t blk);
बाह्य शून्य ext2fs_free_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
बाह्य संरचना ext2_गणः_desc *ext2fs_गणः_desc(ext2_filsys fs,
					  संरचना opaque_ext2_गणः_desc *gdp,
					  dgrp_t गणः);
बाह्य blk64_t ext2fs_खण्डम्_bitmap_csum(ext2_filsys fs, dgrp_t गणः);
बाह्य blk64_t ext2fs_खण्डम्_bitmap_loc(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_खण्डम्_bitmap_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					blk64_t blk);
बाह्य __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t गणः);
बाह्य blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_inode_bitmap_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					blk64_t blk);
बाह्य blk64_t ext2fs_inode_सारणी_loc(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_inode_सारणी_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
				       blk64_t blk);
बाह्य __u32 ext2fs_bg_free_खण्डानि_गणनम्(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					 __u32 n);
बाह्य __u32 ext2fs_bg_free_inodes_गणनम्(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_free_inodes_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					 __u32 n);
बाह्य __u32 ext2fs_bg_प्रयुक्तम्_dirs_गणनम्(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_प्रयुक्तम्_dirs_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
				       __u32 n);
बाह्य __u32 ext2fs_bg_itable_अप्रयुक्तम्(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
				     __u32 n);
बाह्य __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t गणः);
बाह्य पूर्णाङ्क ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t गणः, __u16 bg_flag);
बाह्य शून्य ext2fs_bg_flags_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u16 bg_flags);
बाह्य शून्य ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t गणः, __u16 bg_flags);
बाह्य __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t गणः);
बाह्य शून्य ext2fs_bg_checksum_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u16 checksum);
बाह्य blk64_t ext2fs_file_acl_खण्डम्(ext2_filsys fs,
				     ध्रुव संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_file_acl_खण्डम्_समुच्चयः(ext2_filsys fs,
				      संरचना ext2_inode *inode, blk64_t blk);
बाह्य errcode_t ext2fs_inode_आकार_समुच्चयः(ext2_filsys fs, संरचना ext2_inode *inode,
				       ext2_off64_t आकार);

/* block.c */
बाह्य errcode_t ext2fs_खण्डम्_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      पूर्णाङ्क	flags,
				      वर्ण *खण्डम्_buf,
				      पूर्णाङ्क (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  पूर्णाङ्क	blockcnt,
						  शून्य	*priv_दत्त),
				      शून्य *priv_दत्त);
errcode_t ext2fs_खण्डम्_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				पूर्णाङ्क	flags,
				वर्ण *खण्डम्_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णाङ्क		ref_विस्थापनम्,
					    शून्य	*priv_दत्त),
				शून्य *priv_दत्त);
errcode_t ext2fs_खण्डम्_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *खण्डम्_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णाङ्क		ref_विस्थापनम्,
					    शून्य	*priv_दत्त),
				शून्य *priv_दत्त);

/* bmap.c */
बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags,
			     blk_t खण्डम्, blk_t *phys_blk);
बाह्य errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode,
			      वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags, blk64_t खण्डम्,
			      पूर्णाङ्क *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_खण्डम्(ext2_filsys fs, ext2_ino_t ino,
				   संरचना ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#यदि 0
/* bmove.c */
बाह्य errcode_t ext2fs_move_खण्डानि(ext2_filsys fs,
				    ext2fs_खण्डम्_bitmap reserve,
				    ext2fs_खण्डम्_bitmap alloc_map,
				    पूर्णाङ्क flags);
#यदि_अन्तः

/* check_desc.c */
बाह्य errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
बाह्य errcode_t ext2fs_close(ext2_filsys fs);
बाह्य errcode_t ext2fs_close2(ext2_filsys fs, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_close_free(ext2_filsys *fs);
बाह्य errcode_t ext2fs_flush(ext2_filsys fs);
बाह्य errcode_t ext2fs_flush2(ext2_filsys fs, पूर्णाङ्क flags);
बाह्य पूर्णाङ्क ext2fs_bg_has_super(ext2_filsys fs, dgrp_t गणः_खण्डम्);
बाह्य errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t गणः,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_प्रयुक्तम्_blks);
बाह्य पूर्णाङ्क ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t गणः,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    पूर्णाङ्क *ret_meta_bg);
बाह्य शून्य ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
बाह्य __u32 ext2fs_चक्रीयपरीक्षा_32_be(__u32 crc, अचिह्नित वर्ण ध्रुव *p, size_t len);
बाह्य __u32 ext2fs_crc32c_le(__u32 crc, अचिह्नित वर्ण ध्रुव *p, size_t len);

/* csum.c */
बाह्य शून्य ext2fs_init_csum_बीजम्(ext2_filsys fs);
बाह्य errcode_t ext2fs_mmp_csum_समुच्चयः(ext2_filsys fs, संरचना mmp_struct *mmp);
बाह्य पूर्णाङ्क ext2fs_mmp_csum_verify(ext2_filsys, संरचना mmp_struct *mmp);
बाह्य पूर्णाङ्क ext2fs_verify_csum_type(ext2_filsys fs, संरचना ext2_super_खण्डम् *sb);
बाह्य errcode_t ext2fs_superblock_csum_समुच्चयः(ext2_filsys fs,
					    संरचना ext2_super_खण्डम् *sb);
बाह्य पूर्णाङ्क ext2fs_superblock_csum_verify(ext2_filsys fs,
					 संरचना ext2_super_खण्डम् *sb);
बाह्य errcode_t ext2fs_ext_attr_खण्डम्_csum_समुच्चयः(ext2_filsys fs,
					ext2_ino_t inum, blk64_t खण्डम्,
					संरचना ext2_ext_attr_शीर्षकम् *hdr);
बाह्य पूर्णाङ्क ext2fs_ext_attr_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t खण्डम्,
					     संरचना ext2_ext_attr_शीर्षकम् *hdr);
#परिभाषय EXT2_DIRENT_पुच्छ(खण्डम्, blocksize) \
	((संरचना ext2_dir_प्रविष्टिः_पुच्छ *)(((वर्ण *)(खण्डम्)) + \
	(blocksize) - परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ)))

बाह्य शून्य ext2fs_initialize_dirent_पुच्छ(ext2_filsys fs,
					  संरचना ext2_dir_प्रविष्टिः_पुच्छ *t);
बाह्य पूर्णाङ्क ext2fs_dirent_has_पुच्छ(ext2_filsys fs,
				  संरचना ext2_dir_प्रविष्टिः *dirent);
बाह्य पूर्णाङ्क ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     संरचना ext2_dir_प्रविष्टिः *dirent);
बाह्य पूर्णाङ्क ext2fs_dir_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					संरचना ext2_dir_प्रविष्टिः *dirent);
बाह्य errcode_t ext2fs_dir_खण्डम्_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
					   संरचना ext2_dir_प्रविष्टिः *dirent);
बाह्य errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  संरचना ext2_dir_प्रविष्टिः *dirent,
					  संरचना ext2_dx_countlimit **cc,
					  पूर्णाङ्क *विस्थापनम्);
बाह्य errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				संरचना ext2_dir_प्रविष्टिः *dirent,
				__u32 *crc, संरचना ext2_dx_पुच्छ **ret_t);
बाह्य errcode_t ext2fs_extent_खण्डम्_csum_समुच्चयः(ext2_filsys fs,
					      ext2_ino_t inum,
					      संरचना ext3_extent_शीर्षकम् *eh);
बाह्य पूर्णाङ्क ext2fs_extent_खण्डम्_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   संरचना ext3_extent_शीर्षकम् *eh);
बाह्य errcode_t ext2fs_खण्डम्_bitmap_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					      वर्ण *bitmap, पूर्णाङ्क आकार);
बाह्य पूर्णाङ्क ext2fs_खण्डम्_bitmap_csum_verify(ext2_filsys fs, dgrp_t गणः,
					   वर्ण *bitmap, पूर्णाङ्क आकार);
बाह्य errcode_t ext2fs_inode_bitmap_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
					      वर्ण *bitmap, पूर्णाङ्क आकार);
बाह्य पूर्णाङ्क ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t गणः,
					   वर्ण *bitmap, पूर्णाङ्क आकार);
बाह्य errcode_t ext2fs_inode_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
				       संरचना ext2_inode_large *inode);
बाह्य पूर्णाङ्क ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_inode_large *inode);
बाह्य शून्य ext2fs_गणः_desc_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः);
बाह्य पूर्णाङ्क ext2fs_गणः_desc_csum_verify(ext2_filsys fs, dgrp_t गणः);
बाह्य errcode_t ext2fs_समुच्चयः_gdt_csum(ext2_filsys fs);
बाह्य __u16 ext2fs_गणः_desc_csum(ext2_filsys fs, dgrp_t गणः);

/* dblist.c */
बाह्य errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
बाह्य errcode_t ext2fs_सङ्कलय_dir_खण्डम्(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णाङ्क blockcnt);
बाह्य errcode_t ext2fs_सङ्कलय_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य शून्य ext2fs_dblist_क्रमय(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
							   ध्रुव शून्य *));
बाह्य शून्य ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
							    ध्रुव शून्य *));
बाह्य errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_प्रविष्टिः *db_info,
		    शून्य	*priv_दत्त),
	शून्य *priv_दत्त);
बाह्य errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		    शून्य	*priv_दत्त),
	शून्य *priv_दत्त);
बाह्य errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		    शून्य	*priv_दत्त),
	अचिह्नित दीर्घ दीर्घ start,
	अचिह्नित दीर्घ दीर्घ गणनम्,
	शून्य *priv_दत्त);
बाह्य errcode_t ext2fs_समुच्चयः_dir_खण्डम्(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णाङ्क blockcnt);
बाह्य errcode_t ext2fs_समुच्चयः_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
बाह्य पूर्णाङ्क ext2fs_dblist_गणनम्(ext2_dblist dblist);
बाह्य blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
बाह्य errcode_t ext2fs_dblist_get_चरमः(ext2_dblist dblist,
					संरचना ext2_db_प्रविष्टिः **प्रविष्टिः);
बाह्य errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					संरचना ext2_db_entry2 **प्रविष्टिः);
बाह्य errcode_t ext2fs_dblist_drop_चरमः(ext2_dblist dblist);

/* dblist_dir.c */
बाह्य errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  पूर्णाङ्क	flags,
				  वर्ण	*खण्डम्_buf,
				  पूर्णाङ्क (*func)(ext2_ino_t	dir,
					      पूर्णाङ्क		प्रविष्टिः,
					      संरचना ext2_dir_प्रविष्टिः *dirent,
					      पूर्णाङ्क	विस्थापनम्,
					      पूर्णाङ्क	blocksize,
					      वर्ण	*buf,
					      शून्य	*priv_दत्त),
				  शून्य *priv_दत्त);

#यदि 0
/* digest_encode.c */
#परिभाषय EXT2_FS_DIGEST_आकार EXT2_FS_SHA256_दीर्घता
बाह्य पूर्णाङ्क ext2fs_digest_encode(ध्रुव वर्ण *src, पूर्णाङ्क len, वर्ण *dst);
बाह्य पूर्णाङ्क ext2fs_digest_decode(ध्रुव वर्ण *src, पूर्णाङ्क len, वर्ण *dst);
#यदि_अन्तः

/* dirblock.c */
बाह्य errcode_t ext2fs_read_dir_खण्डम्(ext2_filsys fs, blk_t खण्डम्,
				       शून्य *buf);
बाह्य errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t खण्डम्,
					शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t खण्डम्,
					शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t खण्डम्,
					शून्य *buf, पूर्णाङ्क flags, ext2_ino_t ino);
बाह्य errcode_t ext2fs_write_dir_खण्डम्(ext2_filsys fs, blk_t खण्डम्,
					शून्य *buf);
बाह्य errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t खण्डम्,
					 शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t खण्डम्,
					 शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t खण्डम्,
					 शून्य *buf, पूर्णाङ्क flags, ext2_ino_t ino);

/* dirhash.c */
बाह्य errcode_t ext2fs_dirhash(पूर्णाङ्क संस्करणम्, ध्रुव वर्ण *name, पूर्णाङ्क len,
				ध्रुव __u32 *बीजम्,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

बाह्य errcode_t ext2fs_dirhash2(पूर्णाङ्क संस्करणम्, ध्रुव वर्ण *name, पूर्णाङ्क len,
				 ध्रुव संरचना ext2fs_nls_सारणी *charset,
				 पूर्णाङ्क hash_flags,
				 ध्रुव __u32 *बीजम्,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
बाह्य errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    संरचना ext2_dir_प्रविष्टिः *dirent,
				    अचिह्नित पूर्णाङ्क *rec_len);
बाह्य errcode_t ext2fs_समुच्चयः_rec_len(ext2_filsys fs,
				    अचिह्नित पूर्णाङ्क len,
				    संरचना ext2_dir_प्रविष्टिः *dirent);
बाह्य errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *खण्डम्_buf,
			      पूर्णाङ्क (*func)(संरचना ext2_dir_प्रविष्टिः *dirent,
					  पूर्णाङ्क	विस्थापनम्,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_दत्त),
			      शून्य *priv_दत्त);
बाह्य errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *खण्डम्_buf,
			      पूर्णाङ्क (*func)(ext2_ino_t	dir,
					  पूर्णाङ्क	प्रविष्टिः,
					  संरचना ext2_dir_प्रविष्टिः *dirent,
					  पूर्णाङ्क	विस्थापनम्,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_दत्त),
			      शून्य *priv_दत्त);

/* dupfs.c */
बाह्य errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
बाह्य errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
बाह्य __u32 ext2fs_ext_attr_hash_प्रविष्टिः(संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
					शून्य *दत्त);
बाह्य __u32 ext2fs_ext_attr_hash_प्रविष्टिः_signed(संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
					       शून्य *दत्त);
बाह्य errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
					     शून्य *दत्त, __u32 *hash);
बाह्य errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
					     शून्य *दत्त, __u32 *hash,
					     __u32 *signed_hash);
बाह्य errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t खण्डम्, शून्य *buf);
बाह्य errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t खण्डम्,
				       शून्य *buf);
बाह्य errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t खण्डम्,
				       शून्य *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t खण्डम्,
				       शून्य *buf);
बाह्य errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t खण्डम्,
				       शून्य *buf);
बाह्य errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t खण्डम्,
				       शून्य *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   वर्ण *खण्डम्_buf,
					   पूर्णाङ्क adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   वर्ण *खण्डम्_buf,
					   पूर्णाङ्क adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   वर्ण *खण्डम्_buf,
					   पूर्णाङ्क adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णाङ्क (*func)(वर्ण *name, वर्ण *मानम्,
					    size_t मानम्_len, शून्य *दत्त),
				शून्य *दत्त);
errcode_t ext2fs_xattr_get(संरचना ext2_xattr_handle *h, ध्रुव वर्ण *कुञ्चिका,
			   शून्य **मानम्, size_t *मानम्_len);
errcode_t ext2fs_xattr_समुच्चयः(संरचना ext2_xattr_handle *handle,
			   ध्रुव वर्ण *कुञ्चिका,
			   ध्रुव शून्य *मानम्,
			   size_t मानम्_len);
errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      ध्रुव वर्ण *कुञ्चिका);
errcode_t ext2fs_xattr_remove_all(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(संरचना ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode_large *inode);
errcode_t ext2fs_xattrs_गणनम्(संरचना ext2_xattr_handle *handle, size_t *गणनम्);
errcode_t ext2fs_xattr_inode_max_आकार(ext2_filsys fs, ext2_ino_t ino,
				      size_t *आकार);
#परिभाषय XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(संरचना ext2_xattr_handle *handle,
			      अचिह्नित पूर्णाङ्क *new_flags, अचिह्नित पूर्णाङ्क *old_flags);
बाह्य शून्य ext2fs_ext_attr_खण्डम्_rehash(संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्,
					 संरचना ext2_ext_attr_प्रविष्टिः *अन्तः);
बाह्य __u32 ext2fs_get_ea_inode_hash(संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_समुच्चयः_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash);
बाह्य __u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_समुच्चयः_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_गणनम्);

/* extent.c */
बाह्य errcode_t ext2fs_extent_शीर्षकम्_verify(शून्य *ptr, पूर्णाङ्क आकार);
बाह्य errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
बाह्य errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
बाह्य शून्य ext2fs_extent_free(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   पूर्णाङ्क flags, संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_ग्रन्थिः_विभज(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				       संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_निवेशय(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				      संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_समुच्चयः_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					संरचना ext2_extent_info *info);
बाह्य errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
बाह्य errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     पूर्णाङ्क पर्णम्_level, blk64_t blk);
बाह्य errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
बाह्य size_t ext2fs_max_extent_गभीरता(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      संरचना ext2_inode *inode);
बाह्य errcode_t ext2fs_गणनम्_खण्डानि(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode, blk64_t *ret_गणनम्);
बाह्य errcode_t ext2fs_decode_extent(संरचना ext2fs_extent *गन्तव्यम्, शून्य *from,
				      पूर्णाङ्क len);

/* fallocate.c */
#परिभाषय EXT2_FALLOCATE_ZERO_खण्डानि	(0x1)
#परिभाषय EXT2_FALLOCATE_FORCE_INIT	(0x2)
#परिभाषय EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#परिभाषय EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#परिभाषय EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, पूर्णाङ्क flags, ext2_ino_t ino,
			   संरचना ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
बाह्य errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   संरचना ext2_inode *inode,
				   पूर्णाङ्क flags, ext2_file_t *ret);
बाह्य errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  पूर्णाङ्क flags, ext2_file_t *ret);
बाह्य ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
संरचना ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
बाह्य ext2_ino_t ext2fs_file_get_inode_अङ्कः(ext2_file_t file);
बाह्य errcode_t ext2fs_file_close(ext2_file_t file);
बाह्य errcode_t ext2fs_file_flush(ext2_file_t file);
बाह्य errcode_t ext2fs_file_read(ext2_file_t file, शून्य *buf,
				  अचिह्नित पूर्णाङ्क wanted, अचिह्नित पूर्णाङ्क *got);
बाह्य errcode_t ext2fs_file_write(ext2_file_t file, ध्रुव शून्य *buf,
				   अचिह्नित पूर्णाङ्क nbytes, अचिह्नित पूर्णाङ्क *written);
बाह्य errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 विस्थापनम्,
				   पूर्णाङ्क whence, __u64 *ret_pos);
बाह्य errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t विस्थापनम्,
				   पूर्णाङ्क whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_आकार);
बाह्य ext2_off_t ext2fs_file_get_आकार(ext2_file_t file);
बाह्य errcode_t ext2fs_file_समुच्चयः_आकार(ext2_file_t file, ext2_off_t आकार);
बाह्य errcode_t ext2fs_file_समुच्चयः_size2(ext2_file_t file, ext2_off64_t आकार);

/* finddev.c */
बाह्य वर्ण *ext2fs_find_खण्डम्_device(dev_t device);

/* flushb.c */
बाह्य errcode_t ext2fs_sync_device(पूर्णाङ्क fd, पूर्णाङ्क flushb);

/* freefs.c */
बाह्य शून्य ext2fs_free(ext2_filsys fs);
बाह्य शून्य ext2fs_free_dblist(ext2_dblist dblist);
बाह्य शून्य ext2fs_badblocks_list_free(ext2_badblocks_list bb);
बाह्य शून्य ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
बाह्य शून्य ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 अन्तः,
					    __u32 real_अन्तः,
					    ध्रुव वर्ण *descr, वर्ण *init_map,
					    ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 अन्तः,
						__u32 real_अन्तः,
						ध्रुव वर्ण *descr,
						ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
बाह्य शून्य ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
बाह्य errcode_t ext2fs_fudge_generic_bitmap_अन्तः(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t अन्तः,
						 ext2_ino_t *oend);
बाह्य शून्य ext2fs_समुच्चयः_generic_bitmap_padding(ext2fs_generic_bitmap map);
बाह्य errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_अन्तः,
					      __u32 new_real_अन्तः,
					      ext2fs_generic_bitmap bmap);
बाह्य errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
बाह्य errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अङ्कः,
						 शून्य *बहिर्गमनम्);
बाह्य errcode_t ext2fs_समुच्चयः_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अङ्कः,
						 शून्य *अन्तर्);
बाह्य errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 अन्तः,
						       __u32 *बहिर्गमनम्);
बाह्य errcode_t ext2fs_find_first_समुच्चयः_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 अन्तः,
						       __u32 *बहिर्गमनम्);

/* gen_bitmap64.c */
शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णाङ्क type, __u64 start, __u64 अन्तः,
				    __u64 real_अन्तः,
				    ध्रुव वर्ण *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
शून्य ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_अन्तः(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 अन्तः, __u64 *oend);
शून्य ext2fs_समुच्चयः_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_अन्तः,
				     __u64 new_real_अन्तः);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्);
errcode_t ext2fs_समुच्चयः_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_खण्डम्_bitmap *bitmap);
errcode_t ext2fs_गणनम्_प्रयुक्तम्_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t अन्तः, blk64_t *बहिर्गमनम्);
errcode_t ext2fs_गणनम्_प्रयुक्तम्_खण्डानि(ext2_filsys fs, blk64_t start,
				   blk64_t अन्तः, blk64_t *बहिर्गमनम्);
बाह्य अचिह्नित पूर्णाङ्क ext2fs_list_backups(ext2_filsys fs, अचिह्नित पूर्णाङ्क *three,
				अचिह्नित पूर्णाङ्क *five, अचिह्नित पूर्णाङ्क *seven);

/* getenv.c */
बाह्य वर्ण *ext2fs_safe_getenv(ध्रुव वर्ण *arg);

/* get_num_dirs.c */
बाह्य errcode_t ext2fs_get_अङ्कः_dirs(ext2_filsys fs, ext2_ino_t *ret_अङ्कः_dirs);

/* getsize.c */
बाह्य errcode_t ext2fs_get_device_आकार(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
					blk_t *retblocks);
बाह्य errcode_t ext2fs_get_device_size2(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
बाह्य पूर्णाङ्क ext2fs_get_dio_alignment(पूर्णाङ्क fd);
errcode_t ext2fs_get_device_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_सङ्कलय_खण्डानि(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_खण्डानि);
errcode_t ext2fs_iblk_sub_खण्डानि(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_खण्डानि);
errcode_t ext2fs_iblk_समुच्चयः(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b);

/* imager.c */
बाह्य errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_खण्डम्(ext2_filsys fs, blk_t blk, शून्य *buf);
errcode_t ext2fs_write_ind_खण्डम्(ext2_filsys fs, blk_t blk, शून्य *buf);

/* initialize.c */
बाह्य errcode_t ext2fs_initialize(ध्रुव वर्ण *name, पूर्णाङ्क flags,
				   संरचना ext2_super_खण्डम् *param,
				   io_manager manager, ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, पूर्णाङ्क super_only);

/* icount.c */
बाह्य शून्य ext2fs_free_icount(ext2_icount_t icount);
बाह्य errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, वर्ण *tdb_dir,
					  पूर्णाङ्क flags, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णाङ्क flags,
				       अचिह्नित पूर्णाङ्क आकार,
				       ext2_icount_t hint, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णाङ्क flags,
				      अचिह्नित पूर्णाङ्क आकार,
				      ext2_icount_t *ret);
बाह्य errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
बाह्य errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 गणनम्);
बाह्य ext2_ino_t ext2fs_get_icount_आकार(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

बाह्य errcode_t ext2fs_get_memalign(अचिह्नित दीर्घ आकार,
				     अचिह्नित दीर्घ align, शून्य *ptr);

/* inline_data.c */
बाह्य errcode_t ext2fs_inline_दत्त_init(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_दत्त_आकार(ext2_filsys fs, ext2_ino_t ino,
					 size_t *आकार);
बाह्य errcode_t ext2fs_inline_दत्त_get(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					शून्य *buf, size_t *आकार);
बाह्य errcode_t ext2fs_inline_दत्त_समुच्चयः(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					शून्य *buf, size_t आकार);

/* inode.c */
बाह्य errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   अचिह्नित पूर्णाङ्क cache_आकार);
बाह्य शून्य ext2fs_free_inode_cache(संरचना ext2_inode_cache *icache);
बाह्य errcode_t ext2fs_flush_icache(ext2_filsys fs);
बाह्य errcode_t ext2fs_get_अग्र_inode_पूर्णम्(ext2_inode_अवलोकनम् अवलोकनम्,
					    ext2_ino_t *ino,
					    संरचना ext2_inode *inode,
					    पूर्णाङ्क bufsize);
#परिभाषय EXT2_INODE_अवलोकनम्_DEFAULT_बफर_खण्डानि	8
बाह्य errcode_t ext2fs_open_inode_अवलोकनम्(ext2_filsys fs, पूर्णाङ्क बफर_खण्डानि,
				  ext2_inode_अवलोकनम् *ret_अवलोकनम्);
बाह्य शून्य ext2fs_close_inode_अवलोकनम्(ext2_inode_अवलोकनम् अवलोकनम्);
बाह्य errcode_t ext2fs_get_अग्र_inode(ext2_inode_अवलोकनम् अवलोकनम्, ext2_ino_t *ino,
			       संरचना ext2_inode *inode);
बाह्य errcode_t ext2fs_inode_अवलोकनम्_goto_blockgroup(ext2_inode_अवलोकनम् अवलोकनम्,
						   पूर्णाङ्क	गणः);
बाह्य शून्य ext2fs_समुच्चयः_inode_callback
	(ext2_inode_अवलोकनम् अवलोकनम्,
	 errcode_t (*समाप्तम्_गणः)(ext2_filsys fs,
				 ext2_inode_अवलोकनम् अवलोकनम्,
				 dgrp_t गणः,
				 शून्य * priv_दत्त),
	 शून्य *समाप्तम्_गणः_दत्त);
बाह्य पूर्णाङ्क ext2fs_inode_अवलोकनम्_flags(ext2_inode_अवलोकनम् अवलोकनम्, पूर्णाङ्क समुच्चयः_flags,
				   पूर्णाङ्क clear_flags);
बाह्य errcode_t ext2fs_read_inode_पूर्णम्(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode * inode,
					पूर्णाङ्क bufsize);
बाह्य errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    संरचना ext2_inode * inode,
				    पूर्णाङ्क bufsize, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_inode_पूर्णम्(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode * inode,
					 पूर्णाङ्क bufsize);
बाह्य errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode * inode,
				     पूर्णाङ्क bufsize, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_get_खण्डानि(ext2_filsys fs, ext2_ino_t ino, blk_t *खण्डानि);
बाह्य errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
बाह्य io_manager inode_io_manager;
बाह्य errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					वर्ण **name);
बाह्य errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode *inode,
					 वर्ण **name);

/* ismounted.c */
बाह्य errcode_t ext2fs_check_if_mounted(ध्रुव वर्ण *file, पूर्णाङ्क *mount_flags);
बाह्य errcode_t ext2fs_check_mount_point(ध्रुव वर्ण *device, पूर्णाङ्क *mount_flags,
					  वर्ण *mtpt, पूर्णाङ्क mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
बाह्य errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode,
			      वर्ण *खण्डम्_buf, blk64_t start,
			      blk64_t अन्तः);

/* namei.c */
बाह्य errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ध्रुव वर्ण *name,
			 पूर्णाङ्क namelen, वर्ण *buf, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
			ध्रुव वर्ण *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
			      ध्रुव वर्ण *name, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
पूर्णाङ्क ext2fs_native_flag(शून्य);

/* newdir.c */
बाह्य errcode_t ext2fs_new_dir_खण्डम्(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, वर्ण **खण्डम्);
बाह्य errcode_t ext2fs_new_dir_inline_दत्त(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
बाह्य ध्रुव संरचना ext2fs_nls_सारणी *ext2fs_आरोपय_nls_सारणी(पूर्णाङ्क encoding);
बाह्य पूर्णाङ्क ext2fs_check_encoded_name(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
				     वर्ण *s, size_t len, वर्ण **pos);
बाह्य पूर्णाङ्क ext2fs_casefold_cmp(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			       ध्रुव अचिह्नित वर्ण *str1, size_t len1,
			       ध्रुव अचिह्नित वर्ण *str2, size_t len2);

/* mkdir.c */
बाह्य errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      ध्रुव वर्ण *name);
बाह्य errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, अचिह्नित दीर्घ flags,
			       पूर्णाङ्क link_flags, ध्रुव वर्ण *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
संरचना ext2fs_journal_params {
	blk_t अङ्कः_journal_खण्डानि;
	blk_t अङ्कः_fc_खण्डानि;
};
बाह्य errcode_t ext2fs_get_journal_params(
		संरचना ext2fs_journal_params *params, ext2_filsys fs);
बाह्य errcode_t ext2fs_zero_खण्डानि(ext2_filsys fs, blk_t blk, पूर्णाङ्क अङ्कः,
				    blk_t *ret_blk, पूर्णाङ्क *ret_गणनम्);
बाह्य errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क अङ्कः,
				     blk64_t *ret_blk, पूर्णाङ्क *ret_गणनम्);
बाह्य errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 अङ्कः_खण्डानि, पूर्णाङ्क flags,
						  वर्ण  **ret_jsb);
बाह्य errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  संरचना ext2fs_journal_params *params,
						  पूर्णाङ्क flags, वर्ण  **ret_jsb);
बाह्य errcode_t ext2fs_सङ्कलय_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
बाह्य errcode_t ext2fs_सङ्कलय_journal_inode(ext2_filsys fs, blk_t अङ्कः_खण्डानि,
					  पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_सङ्कलय_journal_inode2(ext2_filsys fs, blk_t अङ्कः_खण्डानि,
					   blk64_t goal, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_सङ्कलय_journal_inode3(ext2_filsys fs,
				    संरचना ext2fs_journal_params *params,
				    blk64_t goal, पूर्णाङ्क flags);
बाह्य पूर्णाङ्क ext2fs_default_journal_आकार(__u64 अङ्कः_खण्डानि);
बाह्य पूर्णाङ्क ext2fs_journal_sb_start(पूर्णाङ्क blocksize);

/* openfs.c */
बाह्य errcode_t ext2fs_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, पूर्णाङ्क superblock,
			     अचिह्नित पूर्णाङ्क खण्डम्_आकार, io_manager manager,
			     ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_open2(ध्रुव वर्ण *name, ध्रुव वर्ण *io_options,
			      पूर्णाङ्क flags, पूर्णाङ्क superblock,
			      अचिह्नित पूर्णाङ्क खण्डम्_आकार, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
बाह्य blk64_t ext2fs_descriptor_खण्डम्_loc2(ext2_filsys fs,
					blk64_t गणः_खण्डम्, dgrp_t i);
बाह्य blk_t ext2fs_descriptor_खण्डम्_loc(ext2_filsys fs, blk_t गणः_खण्डम्,
					 dgrp_t i);
errcode_t ext2fs_get_दत्त_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_समुच्चयः_दत्त_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_गन्तव्यम्_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
बाह्य errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अङ्कः_खण्डानि);
बाह्य errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
बाह्य e2_blkcnt_t ext2fs_default_orphan_file_खण्डानि(ext2_filsys fs);
बाह्य __u32 ext2fs_do_orphan_file_खण्डम्_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      वर्ण *buf);
बाह्य errcode_t ext2fs_orphan_file_खण्डम्_csum_समुच्चयः(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   वर्ण *buf);
बाह्य पूर्णाङ्क ext2fs_orphan_file_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, वर्ण *buf);

/* get_pathname.c */
बाह्य errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       वर्ण **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ध्रुव वर्ण *name,
		      ext2_ino_t ino, पूर्णाङ्क flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ध्रुव वर्ण *name,
			ext2_ino_t ino, पूर्णाङ्क flags);
पूर्णाङ्क ext2fs_dir_is_dx(ext2_filsys fs, ध्रुव संरचना ext2_inode *inode);
शून्य ext2fs_inc_nlink(ext2_filsys fs, संरचना ext2_inode *inode);
शून्य ext2fs_dec_nlink(संरचना ext2_inode *inode);
पूर्णाङ्क ext2fs_dir_link_max(ext2_filsys fs, संरचना ext2_inode_large *inode);
पूर्णाङ्क ext2fs_dir_link_रिक्तानि(संरचना ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ध्रुव वर्ण *name, ध्रुव वर्ण *target);
पूर्णाङ्क ext2fs_is_fast_symlink(संरचना ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, शून्य *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, शून्य *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, पूर्णाङ्क immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
अचिह्नित ext2fs_mmp_new_seq(शून्य);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, शून्य **ptr);

/* read_bb.c */
बाह्य errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
बाह्य errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      शून्य *priv_दत्त,
				      शून्य (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      वर्ण *badstr,
						      शून्य *priv_दत्त));
बाह्य errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     शून्य (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
बाह्य errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
बाह्य errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णाङ्क flags, पूर्णाङ्क अङ्कः_तन्तवः);
बाह्य errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
बाह्य errcode_t ext2fs_read_खण्डम्_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_खण्डम्_bitmap (ext2_filsys fs);

/*sha256.c */
#परिभाषय EXT2_FS_SHA256_दीर्घता 32
#यदि 0
बाह्य शून्य ext2fs_sha256(ध्रुव अचिह्नित वर्ण *अन्तर्, अचिह्नित दीर्घ अन्तर्_आकार,
		   अचिह्नित वर्ण बहिर्गमनम्[EXT2_FS_SHA256_दीर्घता]);
#यदि_अन्तः

/* sha512.c */
#परिभाषय EXT2_FS_SHA512_दीर्घता 64
बाह्य शून्य ext2fs_sha512(ध्रुव अचिह्नित वर्ण *अन्तर्, अचिह्नित दीर्घ अन्तर्_आकार,
			  अचिह्नित वर्ण बहिर्गमनम्[EXT2_FS_SHA512_दीर्घता]);

/* swapfs.c */
बाह्य errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, वर्ण *buf, size_t आकार,
					पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_अन्तर्(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, वर्ण *buf, size_t आकार,
					 पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_बहिर्गमनम्(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags);
बाह्य शून्य ext2fs_swap_ext_attr(वर्ण *गन्तव्यम्, वर्ण *from, पूर्णाङ्क bufsize,
				 पूर्णाङ्क has_शीर्षकम्);
बाह्य शून्य ext2fs_swap_ext_attr_शीर्षकम्(संरचना ext2_ext_attr_शीर्षकम् *गन्तव्यम्_शीर्षकम्,
					संरचना ext2_ext_attr_शीर्षकम् *from_hdr);
बाह्य शून्य ext2fs_swap_ext_attr_प्रविष्टिः(संरचना ext2_ext_attr_प्रविष्टिः *गन्तव्यम्_प्रविष्टिः,
				       संरचना ext2_ext_attr_प्रविष्टिः *from_प्रविष्टिः);
बाह्य शून्य ext2fs_swap_super(संरचना ext2_super_खण्डम् * super);
बाह्य शून्य ext2fs_swap_गणः_desc(संरचना ext2_गणः_desc *gdp);
बाह्य शून्य ext2fs_swap_गणः_desc2(ext2_filsys, संरचना ext2_गणः_desc *gdp);
बाह्य शून्य ext2fs_swap_inode_पूर्णम्(ext2_filsys fs, संरचना ext2_inode_large *t,
				   संरचना ext2_inode_large *f, पूर्णाङ्क hostorder,
				   पूर्णाङ्क bufsize);
बाह्य शून्य ext2fs_swap_inode(ext2_filsys fs,संरचना ext2_inode *t,
			      संरचना ext2_inode *f, पूर्णाङ्क hostorder);
बाह्य शून्य ext2fs_swap_mmp(संरचना mmp_struct *mmp);

/* unix_io.c */
बाह्य पूर्णाङ्क ext2fs_open_file(ध्रुव वर्ण *pathname, पूर्णाङ्क flags, mode_t बहुलकम्);
बाह्य पूर्णाङ्क ext2fs_stat(ध्रुव वर्ण *मार्गः, ext2fs_struct_stat *buf);
बाह्य पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
बाह्य पूर्णाङ्क ext2fs_inode_has_valid_खण्डानि(संरचना ext2_inode *inode);
बाह्य पूर्णाङ्क ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  संरचना ext2_inode *inode);

/* version.c */
बाह्य पूर्णाङ्क ext2fs_parse_संस्करणम्_string(ध्रुव वर्ण *ver_string);
बाह्य पूर्णाङ्क ext2fs_get_library_संस्करणम्(ध्रुव वर्ण **ver_string,
				      ध्रुव वर्ण **date_string);

/* write_bb_file.c */
बाह्य errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      अचिह्नित पूर्णाङ्क flags,
				      FILE *f);


/* inline functions */
#यदि_परिभाषितम् NO_INLINE_FUNCS
बाह्य errcode_t ext2fs_get_mem(अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य errcode_t ext2fs_get_memzero(अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य errcode_t ext2fs_get_array(अचिह्नित दीर्घ गणनम्,
				  अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य errcode_t ext2fs_get_arrayzero(अचिह्नित दीर्घ गणनम्,
				      अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य errcode_t ext2fs_free_mem(शून्य *ptr);
बाह्य errcode_t ext2fs_resize_mem(अचिह्नित दीर्घ old_आकार,
				   अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य errcode_t ext2fs_resize_array(अचिह्नित दीर्घ old_गणनम्, अचिह्नित दीर्घ गणनम्,
				     अचिह्नित दीर्घ आकार, शून्य *ptr);
बाह्य शून्य ext2fs_mark_super_dirty(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_परिवर्तितम्(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_परिवर्तितम्(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_valid(ext2_filsys fs);
बाह्य शून्य ext2fs_unmark_valid(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_valid(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_ib_dirty(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_bb_dirty(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_ib_dirty(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_bb_dirty(ext2_filsys fs);
बाह्य dgrp_t ext2fs_गणः_सम्बन्धि_blk(ext2_filsys fs, blk_t blk);
बाह्य dgrp_t ext2fs_गणः_सम्बन्धि_ino(ext2_filsys fs, ext2_ino_t ino);
बाह्य blk_t ext2fs_गणः_first_खण्डम्(ext2_filsys fs, dgrp_t गणः);
बाह्य blk_t ext2fs_गणः_चरमः_खण्डम्(ext2_filsys fs, dgrp_t गणः);
बाह्य blk_t ext2fs_inode_दत्त_खण्डानि(ext2_filsys fs,
				      संरचना ext2_inode *inode);
बाह्य पूर्णाङ्क ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णाङ्क खण्डानि);
बाह्य पूर्णाङ्क ext2fs_log2_u32(__u32 arg);
बाह्य पूर्णाङ्क ext2fs_log2_u64(__u64 arg);
बाह्य पूर्णाङ्क ext2fs_log10_u32(__u32 arg);
बाह्य पूर्णाङ्क ext2fs_log10_u64(__u64 arg);
बाह्य अचिह्नित पूर्णाङ्क ext2fs_div_ceil(अचिह्नित पूर्णाङ्क a, अचिह्नित पूर्णाङ्क b);
बाह्य __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
बाह्य पूर्णाङ्क ext2fs_dirent_name_len(ध्रुव संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः);
बाह्य शून्य ext2fs_dirent_समुच्चयः_name_len(संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः, पूर्णाङ्क len);
बाह्य पूर्णाङ्क ext2fs_dirent_file_type(ध्रुव संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः);
बाह्य शून्य ext2fs_dirent_समुच्चयः_file_type(संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः, पूर्णाङ्क type);
बाह्य संरचना ext2_inode *ext2fs_inode(संरचना ext2_inode_large * large_inode);
बाह्य ध्रुव संरचना ext2_inode *ext2fs_const_inode(ध्रुव संरचना ext2_inode_large * large_inode);
बाह्य पूर्णाङ्क ext2fs_inodes_प्रत्येकम्_orphan_खण्डम्(ext2_filsys fs);
बाह्य संरचना ext4_orphan_खण्डम्_पुच्छ *ext2fs_orphan_खण्डम्_पुच्छ(ext2_filsys fs,
							       वर्ण *buf);
#यदि_अन्तः

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#यदि (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#यदि_परिभाषितम् INCLUDE_INLINE_FUNCS
#परिभाषय _INLINE_ बाह्य
#अन्यथा
#यदि (__STDC_VERSION__ >= 199901L)
#परिभाषय _INLINE_ अन्तःस्थ
#अन्यथा
#यदि_परिभाषितम् __GNUC__
#परिभाषय _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#परिभाषय _INLINE_ बाह्य अन्तःस्थ
#यदि_अन्तः /* __GNUC__ */
#यदि_अन्तः /* __STDC_VERSION__ >= 199901L */
#यदि_अन्तः

#यदि_अपरिभाषितम् EXT2_CUSTOM_स्मृतिः_ROUTINES
#अन्तर्भाव <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(अचिह्नित दीर्घ आकार, शून्य *ptr)
{
	शून्य *pp;

	pp = स्मृतिं_देहि(आकार);
	यदि (!pp)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	स्मृतिं_प्रतिलिख(ptr, &pp, परिमाण (pp));
	प्रत्यागम 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(अचिह्नित दीर्घ आकार, शून्य *ptr)
{
	शून्य *pp;

	pp = स्मृतिं_देहि(आकार);
	यदि (!pp)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	स्मृतिं_पूरय(pp, 0, आकार);
	स्मृतिं_प्रतिलिख(ptr, &pp, परिमाण(pp));
	प्रत्यागम 0;
}

_INLINE_ errcode_t ext2fs_get_array(अचिह्नित दीर्घ गणनम्, अचिह्नित दीर्घ आकार,
				    शून्य *ptr)
{
	यदि (गणनम् && (~0UL)/गणनम् < आकार)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	प्रत्यागम ext2fs_get_mem(गणनम्*आकार, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(अचिह्नित दीर्घ गणनम्,
					अचिह्नित दीर्घ आकार, शून्य *ptr)
{
	यदि (गणनम् && (~0UL)/गणनम् < आकार)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	प्रत्यागम ext2fs_get_memzero((size_t)गणनम् * आकार, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(शून्य *ptr)
{
	शून्य *p;

	स्मृतिं_प्रतिलिख(&p, ptr, परिमाण(p));
	मुक्त(p);
	p = 0;
	स्मृतिं_प्रतिलिख(ptr, &p, परिमाण(p));
	प्रत्यागम 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(अचिह्नित दीर्घ EXT2FS_ATTR((अप्रयुक्तम्)) old_आकार,
				     अचिह्नित दीर्घ आकार, शून्य *ptr)
{
	शून्य *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	स्मृतिं_प्रतिलिख(&p, ptr, परिमाण(p));
	p = realloc(p, आकार);
	यदि (!p)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	स्मृतिं_प्रतिलिख(ptr, &p, परिमाण(p));
	प्रत्यागम 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(अचिह्नित दीर्घ आकार,
				       अचिह्नित दीर्घ old_गणनम्,
				       अचिह्नित दीर्घ गणनम्, शून्य *ptr)
{
	अचिह्नित दीर्घ old_आकार;
	errcode_t retval;

	यदि (गणनम् && (~0UL)/गणनम् < आकार)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	आकार *= गणनम्;
	old_आकार = आकार * old_गणनम्;
	retval = ext2fs_resize_mem(old_आकार, आकार, ptr);
	यदि (retval)
		प्रत्यागम retval;

	यदि (आकार > old_आकार) {
		शून्य *p;

		स्मृतिं_प्रतिलिख(&p, ptr, परिमाण(p));
		स्मृतिं_पूरय((वर्ण *)p + old_आकार, 0, आकार - old_आकार);
		स्मृतिं_प्रतिलिख(ptr, &p, परिमाण(p));
	}

	प्रत्यागम 0;
}
#यदि_अन्तः	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ शून्य ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_परिवर्तितम्;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ शून्य ext2fs_mark_परिवर्तितम्(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_परिवर्तितम्;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ पूर्णाङ्क ext2fs_test_परिवर्तितम्(ext2_filsys fs)
{
	प्रत्यागम (fs->flags & EXT2_FLAG_परिवर्तितम्);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ शून्य ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ शून्य ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ पूर्णाङ्क ext2fs_test_valid(ext2_filsys fs)
{
	प्रत्यागम (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ शून्य ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_परिवर्तितम्;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ शून्य ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_परिवर्तितम्;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ पूर्णाङ्क ext2fs_test_ib_dirty(ext2_filsys fs)
{
	प्रत्यागम (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ पूर्णाङ्क ext2fs_test_bb_dirty(ext2_filsys fs)
{
	प्रत्यागम (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_गणः_सम्बन्धि_blk(ext2_filsys fs, blk_t blk)
{
	प्रत्यागम ext2fs_गणः_सम्बन्धि_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_गणः_सम्बन्धि_ino(ext2_filsys fs, ext2_ino_t ino)
{
	प्रत्यागम (ino - 1) / fs->super->s_inodes_प्रत्येकम्_गणः;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_गणः_first_खण्डम्(ext2_filsys fs, dgrp_t गणः)
{
	प्रत्यागम (blk_t) ext2fs_गणः_first_block2(fs, गणः);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_गणः_चरमः_खण्डम्(ext2_filsys fs, dgrp_t गणः)
{
	प्रत्यागम (blk_t) ext2fs_गणः_चरमः_block2(fs, गणः);
}

_INLINE_ blk_t ext2fs_inode_दत्त_खण्डानि(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	प्रत्यागम (blk_t) ext2fs_inode_दत्त_blocks2(fs, inode);
}

_INLINE_ पूर्णाङ्क ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णाङ्क खण्डानि)
{
	पूर्णाङ्क csum_आकार = 0;

	यदि ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_आकार = परिमाण(संरचना ext2_dx_पुच्छ);
	प्रत्यागम खण्डानि * ((fs->blocksize - (8 + csum_आकार)) /
						परिमाण(संरचना ext2_dx_प्रविष्टिः));
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log2_u32(__u32 arg)
{
	पूर्णाङ्क l = 0;

	arg >>= 1;
	यावत् (arg) {
		l++;
		arg >>= 1;
	}
	प्रत्यागम l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log2_u64(__u64 arg)
{
	पूर्णाङ्क l = 0;

	arg >>= 1;
	यावत् (arg) {
		l++;
		arg >>= 1;
	}
	प्रत्यागम l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log10_u32(__u32 arg)
{
	पूर्णाङ्क l = 0;

	arg /= 10;
	यावत् (arg) {
		l++;
		arg /= 10;
	}
	प्रत्यागम l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log10_u64(__u64 arg)
{
	पूर्णाङ्क l = 0;

	arg /= 10;
	यावत् (arg) {
		l++;
		arg /= 10;
	}
	प्रत्यागम l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ अचिह्नित पूर्णाङ्क ext2fs_div_ceil(अचिह्नित पूर्णाङ्क a, अचिह्नित पूर्णाङ्क b)
{
	यदि (!a)
		प्रत्यागम 0;
	प्रत्यागम ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	यदि (!a)
		प्रत्यागम 0;
	प्रत्यागम ((a - 1) / b) + 1;
}

_INLINE_ पूर्णाङ्क ext2fs_dirent_name_len(ध्रुव संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः)
{
	प्रत्यागम प्रविष्टिः->name_len & 0xff;
}

_INLINE_ शून्य ext2fs_dirent_समुच्चयः_name_len(संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः, पूर्णाङ्क len)
{
	प्रविष्टिः->name_len = (प्रविष्टिः->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ पूर्णाङ्क ext2fs_dirent_file_type(ध्रुव संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः)
{
	प्रत्यागम प्रविष्टिः->name_len >> 8;
}

_INLINE_ शून्य ext2fs_dirent_समुच्चयः_file_type(संरचना ext2_dir_प्रविष्टिः *प्रविष्टिः, पूर्णाङ्क type)
{
	प्रविष्टिः->name_len = (प्रविष्टिः->name_len & 0xff) | (type << 8);
}

_INLINE_ संरचना ext2_inode *ext2fs_inode(संरचना ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	प्रत्यागम (संरचना ext2_inode *) large_inode;
}

_INLINE_ ध्रुव संरचना ext2_inode *
ext2fs_const_inode(ध्रुव संरचना ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	प्रत्यागम (ध्रुव संरचना ext2_inode *) large_inode;
}

_INLINE_ पूर्णाङ्क ext2fs_inodes_प्रत्येकम्_orphan_खण्डम्(ext2_filsys fs)
{
	प्रत्यागम (fs->blocksize - परिमाण(संरचना ext4_orphan_खण्डम्_पुच्छ)) /
		परिमाण(__u32);
}

_INLINE_ संरचना ext4_orphan_खण्डम्_पुच्छ *
ext2fs_orphan_खण्डम्_पुच्छ(ext2_filsys fs, वर्ण *buf)
{
	प्रत्यागम (संरचना ext4_orphan_खण्डम्_पुच्छ *)(buf + fs->blocksize -
		परिमाण(संरचना ext4_orphan_खण्डम्_पुच्छ));
}

#अपरिभाषय _INLINE_
#यदि_अन्तः

/* htree levels for ext4 */
#परिभाषय EXT4_HTREE_LEVEL_COMPAT 2
#परिभाषय EXT4_HTREE_LEVEL	3

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क ext2_dir_htree_level(ext2_filsys fs)
{
	यदि (ext2fs_has_feature_largedir(fs->super))
		प्रत्यागम EXT4_HTREE_LEVEL;

	प्रत्यागम EXT4_HTREE_LEVEL_COMPAT;
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
स्थिर अन्तःस्थ शून्य ext2fs_समुच्चयः_dtime(ext2_filsys fs, संरचना ext2_inode *inode)
{
	__u32	t;

	यदि (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	अन्यथा
		t = time(रिक्त) & 0xFFFFFFFF;
	यदि (t < fs->super->s_inodes_गणनम्)
		t = fs->super->s_inodes_गणनम्;
	inode->i_dtime = t;
}

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः /* _EXT2FS_EXT2FS_H */
