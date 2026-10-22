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

#ifndef _EXT2FS_EXT2FS_H
#define _EXT2FS_EXT2FS_H

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#ಅನ್ಯಥಾ
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#ಯದಿ __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#ಅನ್ಯಥಾ
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#ಅನ್ಯಥಾ
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#ಅನ್ಯಥಾ
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#ಯದಿ (!defined(__GNUC__) && !defined(__WATCOMC__))
#define NO_INLINE_FUNCS
#endif

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#define SUPERBLOCK_OFFSET	1024
#define SUPERBLOCK_SIZE		1024

#define UUID_STR_SIZE 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#define EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#ifdef HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಯದಿ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "e2_types.h"
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext3_extents.h"
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_fs.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext3_extents.h>
#endif /* EXT2_FLAT_INCLUDES */

ಪ್ರಕಾರನಿರ್ದೇಶ __u32 __bitwise		ext2_ino_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __u32 __bitwise		blk_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __u64 __bitwise		blk64_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __u32 __bitwise		dgrp_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __s32 __bitwise		ext2_off_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __s64 __bitwise		ext2_off64_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __s64 __bitwise		e2_blkcnt_t;
ಪ್ರಕಾರನಿರ್ದೇಶ __u32 __bitwise		ext2_dirhash_t;

#ಯದಿ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_io.h"
#ಅನ್ತರ್ಭಾವ "ext2_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_ext_attr.h"
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <et/com_err.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_io.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_err.h>
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_ext_attr.h>
#endif

#ಅನ್ತರ್ಭಾವ "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE ಪೂರ್ಣಾಙ್ಕ __cdecl
#ಅನ್ಯಥಾ
#define EXT2_QSORT_TYPE ಪೂರ್ಣಾಙ್ಕ
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

ಸಂರಚನಾ ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_list *ext2_badblocks_list;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_iterate *ext2_badblocks_iterate;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_list *ext2_u32_list;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_list *badblocks_list;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_u32_iterate *badblocks_iterate;

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ಸಂರಚನಾ ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ಸಂರಚನಾ ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	ಪೂರ್ಣಾಙ್ಕ	blockcnt;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_file *ext2_file_t;

#define EXT2_SEEK_SET	0
#define EXT2_SEEK_CUR	1
#define EXT2_SEEK_END	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#define EXT2_FLAG_RW			0x01
#define EXT2_FLAG_CHANGED		0x02
#define EXT2_FLAG_DIRTY			0x04
#define EXT2_FLAG_VALID			0x08
#define EXT2_FLAG_IB_DIRTY		0x10
#define EXT2_FLAG_BB_DIRTY		0x20
#define EXT2_FLAG_SWAP_BYTES		0x40
#define EXT2_FLAG_SWAP_BYTES_READ	0x80
#define EXT2_FLAG_SWAP_BYTES_WRITE	0x100
#define EXT2_FLAG_MASTER_SB_ONLY	0x200
#define EXT2_FLAG_FORCE			0x400
#define EXT2_FLAG_SUPER_ONLY		0x800
#define EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#define EXT2_FLAG_IMAGE_FILE		0x2000
#define EXT2_FLAG_EXCLUSIVE		0x4000
#define EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#define EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#define EXT2_FLAG_64BITS		0x20000
#define EXT2_FLAG_PRINT_PROGRESS	0x40000
#define EXT2_FLAG_DIRECT_IO		0x80000
#define EXT2_FLAG_SKIP_MMP		0x100000
#define EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#define EXT2_FLAG_SHARE_DUP		0x400000
#define EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#define EXT2_FLAG_BBITMAP_TAIL_PROBLEM	0x1000000
#define EXT2_FLAG_IBITMAP_TAIL_PROBLEM	0x2000000
#define EXT2_FLAG_THREADS		0x4000000
#define EXT2_FLAG_IGNORE_SWAP_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#define EXT2_FLAG2_USE_FAKE_TIME	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#define EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#define EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#define EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#define EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#define EXT2_JOURNAL_TO_FC_BLKS_RATIO		64

ಸಂರಚನಾ blk_alloc_ctx;
ಸಂರಚನಾ opaque_ext2_group_desc;

ಸಂರಚನಾ struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	ಪೂರ್ಣಾಙ್ಕ				flags;
	ವರ್ಣ *				device_name;
	ಸಂರಚನಾ ext2_super_block	* 	super;
	unsigned ಪೂರ್ಣಾಙ್ಕ			blocksize;
	ಪೂರ್ಣಾಙ್ಕ				fragsize;
	dgrp_t				group_desc_ಎಣಿಕೆ;
	unsigned long			desc_blocks;
	ಸಂರಚನಾ opaque_ext2_group_desc *	group_desc;
	unsigned ಪೂರ್ಣಾಙ್ಕ			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ಸಂರಚನಾ ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ಸಂರಚನಾ ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ಸಂರಚನಾ ext2_super_block *	orig_super;
	ಸಂರಚನಾ ext2_image_hdr *		image_header;
	__u32				umask;
	time_t				now;
	ಪೂರ್ಣಾಙ್ಕ				cluster_ratio_bits;
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
	ಶೂನ್ಯ *				priv_data;

	/*
	 * Inode cache
	 */
	ಸಂರಚನಾ ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ಸಂರಚನಾ blk_alloc_ctx *ctx);
	ಶೂನ್ಯ (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಙ್ಕ inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	ಶೂನ್ಯ *mmp_buf;
	ಶೂನ್ಯ *mmp_cmp;
	ಪೂರ್ಣಾಙ್ಕ mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* progress operation functions */
	ಸಂರಚನಾ ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	ವರ್ಣ				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	ಶೂನ್ಯ (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t ಅಂಕೆ,
					ಪೂರ್ಣಾಙ್ಕ inuse);

	/* hashmap for SHA of data blocks */
	ಸಂರಚನಾ ext2fs_hashmap* block_sha_map;

	ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *encoding;
};

#ಯದಿ EXT2_FLAT_INCLUDES
#ಅನ್ತರ್ಭಾವ "e2_bitops.h"
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <ext2fs/bitops.h>
#endif

/*
 * 64-bit bitmap backend types
 */
#define EXT2FS_BMAP64_BITARRAY	1
#define EXT2FS_BMAP64_RBTREE	2
#define EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#define BLOCK_CHANGED			1
#define BLOCK_ABORT			2
#define BLOCK_ERROR			4
#define BLOCK_INLINE_DATA_CHANGED	8

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
#define BLOCK_FLAG_APPEND	1
#define BLOCK_FLAG_HOLE		1
#define BLOCK_FLAG_DEPTH_TRAVERSE	2
#define BLOCK_FLAG_DATA_ONLY	4
#define BLOCK_FLAG_READ_ONLY	8

#define BLOCK_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#define BLOCK_ಎಣಿಕೆ_IND		(-1)
#define BLOCK_ಎಣಿಕೆ_DIND	(-2)
#define BLOCK_ಎಣಿಕೆ_TIND	(-3)
#define BLOCK_ಎಣಿಕೆ_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_DATA	1
#define BLOCK_ALLOC_METADATA	2

ಸಂರಚನಾ blk_alloc_ctx {
	ext2_ino_t		ino;
	ಸಂರಚನಾ ext2_inode	*inode;
	blk64_t			lblk;
	ಪೂರ್ಣಾಙ್ಕ			flags;
};

#ಯದಿ 0
/*
 * Flags for ext2fs_move_blocks
 */
#define EXT2_BMOVE_GET_DBLIST	0x0001
#define EXT2_BMOVE_DEBUG	0x0002
#endif

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#define EXT2_EXTENT_FLAGS_LEAF		0x0001
#define EXT2_EXTENT_FLAGS_UNINIT	0x0002
#define EXT2_EXTENT_FLAGS_SECOND_VISIT	0x0004

ಸಂರಚನಾ ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_extent_handle *ext2_extent_handle_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_ಬೇರು	0x0001
#define EXT2_EXTENT_LAST_LEAF	0x0002
#define EXT2_EXTENT_FIRST_SIB	0x0003
#define EXT2_EXTENT_LAST_SIB	0x0004
#define EXT2_EXTENT_NEXT_SIB	0x0005
#define EXT2_EXTENT_PREV_SIB	0x0006
#define EXT2_EXTENT_NEXT_LEAF	0x0007
#define EXT2_EXTENT_PREV_LEAF	0x0008
#define EXT2_EXTENT_NEXT	0x0009
#define EXT2_EXTENT_PREV	0x000A
#define EXT2_EXTENT_UP		0x000B
#define EXT2_EXTENT_DOWN	0x000C
#define EXT2_EXTENT_DOWN_AND_LAST 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#define EXT2_EXTENT_ಸೇರಿಸು_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_ಸೇರಿಸು_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#define EXT2_EXTENT_DELETE_KEEP_EMPTY	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#define EXT2_EXTENT_SET_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
ಸಂರಚನಾ ext2_extent_info {
	ಪೂರ್ಣಾಙ್ಕ		curr_entry;
	ಪೂರ್ಣಾಙ್ಕ		curr_level;
	ಪೂರ್ಣಾಙ್ಕ		ಅಂಕೆ_entries;
	ಪೂರ್ಣಾಙ್ಕ		max_entries;
	ಪೂರ್ಣಾಙ್ಕ		max_depth;
	ಪೂರ್ಣಾಙ್ಕ		bytes_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#define EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#define DIRENT_CHANGED	1
#define DIRENT_ABORT	2
#define DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#define DIRENT_FLAG_INCLUDE_EMPTY	1
#define DIRENT_FLAG_INCLUDE_REMOVED	2
#define DIRENT_FLAG_INCLUDE_CSUM	4
#define DIRENT_FLAG_INCLUDE_INLINE_DATA 8

#define DIRENT_DOT_FILE		1
#define DIRENT_DOT_DOT_FILE	2
#define DIRENT_OTHER_FILE	3
#define DIRENT_DELETED_FILE	4
#define DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_struct_inode_scan *ext2_inode_scan;

/*
 * ext2fs_scan flags
 */
#define EXT2_SF_CHK_BADBLOCKS	0x0001
#define EXT2_SF_BAD_INODE_BLK	0x0002
#define EXT2_SF_BAD_EXTRA_BYTES	0x0004
#define EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#define EXT2_SF_DO_LAZY		0x0010
#define EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#define EXT2_MF_MOUNTED		1
#define EXT2_MF_ISROOT		2
#define EXT2_MF_READONLY	4
#define EXT2_MF_SWAP		8
#define EXT2_MF_BUSY		16
#define EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#define LINUX_S_IFMT  00170000
#define LINUX_S_IFSOCK 0140000
#define LINUX_S_IFLNK	 0120000
#define LINUX_S_IFREG  0100000
#define LINUX_S_IFBLK  0060000
#define LINUX_S_IFDIR  0040000
#define LINUX_S_IFCHR  0020000
#define LINUX_S_IFIFO  0010000
#define LINUX_S_ISUID  0004000
#define LINUX_S_ISGID  0002000
#define LINUX_S_ISVTX  0001000

#define LINUX_S_IRWXU 00700
#define LINUX_S_IRUSR 00400
#define LINUX_S_IWUSR 00200
#define LINUX_S_IXUSR 00100

#define LINUX_S_IRWXG 00070
#define LINUX_S_IRGRP 00040
#define LINUX_S_IWGRP 00020
#define LINUX_S_IXGRP 00010

#define LINUX_S_IRWXO 00007
#define LINUX_S_IROTH 00004
#define LINUX_S_IWOTH 00002
#define LINUX_S_IXOTH 00001

#define LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#define LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#define LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#define LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#define LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#define LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#define LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_ಮೇಲಿನ << 32))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#ಯದಿ (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	ಪ್ರತ್ಯಾಗಮ extra | (nsec << EXT4_EPOCH_BITS);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#ಯದಿ (SIZEOF_TIME_T > 4)
	ಯದಿ (extra & EXT4_EPOCH_MASK)
		seconds += ((time_t)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	ಪ್ರತ್ಯಾಗಮ seconds;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ __u32 __decode_extra_nsec(__u32 extra)
{
	ಪ್ರತ್ಯಾಗಮ (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_size(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_SIZE +				      \
		  (ಪರಿಮಾಣ(*inode) > EXT2_GOOD_OLD_INODE_SIZE ?		      \
		   ((ಸಂರಚನಾ ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
ಕುರು {									      \
	ಯದಿ (ext2fs_inode_includes(ext2fs_inode_actual_size(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((ಸಂರಚನಾ ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} ಅನ್ಯಥಾ {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} ಯಾವತ್ (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_size(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((ಸಂರಚನಾ ext2_inode_large *)(inode))->field ## _extra) :      \
		(time_t)(inode)->field)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#ಯದಿ (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#ಅನ್ಯಥಾ
	*hi = 0;
#endif
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#ಯದಿ (SIZEOF_TIME_T == 4)
	ಪ್ರತ್ಯಾಗಮ *lo;
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ ((time_t)(*hi) << 32) | *lo;
#endif
}
#define ext2fs_set_tstamp(sb, field, seconds) \
	__sb_set_tstamp(&(sb)->field, &(sb)->field ## _hi, seconds)
#define ext2fs_get_tstamp(sb, field) \
	__sb_get_tstamp(&(sb)->field, &(sb)->field ## _hi)

/*
 * ext2_icount_t abstraction
 */
#define EXT2_ICOUNT_OPT_INCREMENT	0x01
#define EXT2_ICOUNT_OPT_FULLMAP		0x02

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#define BMAP_ALLOC	0x0001
#define BMAP_SET	0x0002
#define BMAP_UNINIT	0x0004
#define BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#define BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#define READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#define WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#define IMAGER_FLAG_INODEMAP	1
#define IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)

/*
 * Features supported by this version of the library
 */
#define EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_INDEX|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#ifdef CONFIG_MMP
#define EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#ಅನ್ಯಥಾ
#define EXT4_LIB_INCOMPAT_MMP		(0)
#endif

#define EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64BIT|\
					 EXT4_FEATURE_INCOMPAT_INLINE_DATA|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_SEED|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#define EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_BLOCKS |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#define EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#define EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#define EXT2FS_CLUSTER_RATIO(fs)	(1 << (fs)->cluster_ratio_bits)
#define EXT2FS_CLUSTER_MASK(fs)		(EXT2FS_CLUSTER_RATIO(fs) - 1)
#define EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_ratio_bits)
/* Translate a cluster number to a block number */
#define EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_ratio_bits)
/* Translate # of blks to # of clusters */
#define EXT2_FS_ಅಂಕೆ_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#ಯದಿ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ stat64 ext2fs_struct_stat;
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ಸಂರಚನಾ ext2_xattr_handle;
#define XATTR_ABORT	1
#define XATTR_CHANGED	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#define EXT2FS_BITMAPS_WRITE		0x0001
#define EXT2FS_BITMAPS_BLOCK		0x0002
#define EXT2FS_BITMAPS_INODE		0x0004
#define EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#define EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#define EXT2FS_LINK_FT_MASK	0x0007
#define EXT2FS_LINK_APPEND	0x0010
#define EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ext2fs_needs_large_file_feature(unsigned long long file_size)
{
	ಪ್ರತ್ಯಾಗಮ file_size >= 0x80000000ULL;
}

/* alloc.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, ಪೂರ್ಣಾಙ್ಕ mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   ಸಂರಚನಾ blk_alloc_ctx *ctx);
ಬಾಹ್ಯ errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ext2fs_block_bitmap map,
					blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    ವರ್ಣ *block_buf, blk_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     ವರ್ಣ *block_buf, blk64_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     ವರ್ಣ *block_buf, blk64_t *ret,
				     ಸಂರಚನಾ blk_alloc_ctx *ctx);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ಸಂರಚನಾ ext2_inode *inode, blk64_t lblk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	ಶೂನ್ಯ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse),
	ಶೂನ್ಯ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_LENGTH	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
						  ಶೂನ್ಯ (*func)(ext2_filsys fs,
							       blk64_t blk,
							       ಪೂರ್ಣಾಙ್ಕ inuse),
						  ಶೂನ್ಯ (**old)(ext2_filsys fs,
							       blk64_t blk,
							       ಪೂರ್ಣಾಙ್ಕ inuse));

/* alloc_stats.c */
ಶೂನ್ಯ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ inuse);
ಶೂನ್ಯ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ಪೂರ್ಣಾಙ್ಕ inuse, ಪೂರ್ಣಾಙ್ಕ isdir);
ಶೂನ್ಯ ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಙ್ಕ inuse);
ಶೂನ್ಯ ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಙ್ಕ inuse);
ಶೂನ್ಯ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse);

/* alloc_tables.c */
ಬಾಹ್ಯ errcode_t ext2fs_allocate_tables(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
ಬಾಹ್ಯ errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

ಬಾಹ್ಯ errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
					   blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
ಬಾಹ್ಯ errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
ಬಾಹ್ಯ errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_ಎಣಿಕೆ(ext2_u32_list bb);

/* bb_compat */
ಬಾಹ್ಯ errcode_t badblocks_list_create(badblocks_list *ret, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ badblocks_list_test(badblocks_list bb, blk_t blk);
ಬಾಹ್ಯ errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
ಬಾಹ್ಯ ಶೂನ್ಯ badblocks_list_iterate_end(badblocks_iterate iter);
ಬಾಹ್ಯ ಶೂನ್ಯ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
ಬಾಹ್ಯ errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      ಧ್ರುವ ವರ್ಣ *descr,
					      ext2fs_block_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ಧ್ರುವ ವರ್ಣ *descr,
						   ext2fs_block_bitmap *ret);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ಧ್ರುವ ವರ್ಣ *descr,
					      ext2fs_inode_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *out);
ಬಾಹ್ಯ errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *out);
ಬಾಹ್ಯ errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *out);
ಬಾಹ್ಯ errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *out);

/* blknum.c */
ಬಾಹ್ಯ __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
ಬಾಹ್ಯ blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_group_blocks_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					 ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ blk64_t ext2fs_blocks_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_super_block *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_blocks_ಎಣಿಕೆ_set(ಸಂರಚನಾ ext2_super_block *super,
				    blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_blocks_ಎಣಿಕೆ_add(ಸಂರಚನಾ ext2_super_block *super,
				    blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_r_blocks_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_super_block *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_r_blocks_ಎಣಿಕೆ_set(ಸಂರಚನಾ ext2_super_block *super,
				      blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_r_blocks_ಎಣಿಕೆ_add(ಸಂರಚನಾ ext2_super_block *super,
				      blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_free_blocks_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_super_block *super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_blocks_ಎಣಿಕೆ_set(ಸಂರಚನಾ ext2_super_block *super,
					 blk64_t blk);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_blocks_ಎಣಿಕೆ_add(ಸಂರಚನಾ ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
ಬಾಹ್ಯ ಸಂರಚನಾ ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  ಸಂರಚನಾ opaque_ext2_group_desc *gdp,
					  dgrp_t group);
ಬಾಹ್ಯ blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
ಬಾಹ್ಯ __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
ಬಾಹ್ಯ blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
ಬಾಹ್ಯ __u32 ext2fs_bg_free_blocks_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_free_blocks_ಎಣಿಕೆ_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_free_inodes_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_free_inodes_ಎಣಿಕೆ_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_used_dirs_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_used_dirs_ಎಣಿಕೆ_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
ಬಾಹ್ಯ __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
ಬಾಹ್ಯ __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
ಬಾಹ್ಯ __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
ಬಾಹ್ಯ blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     ಧ್ರುವ ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_file_acl_block_set(ext2_filsys fs,
				      ಸಂರಚನಾ ext2_inode *inode, blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_inode_size_set(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				       ext2_off64_t size);

/* block.c */
ಬಾಹ್ಯ errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      ಪೂರ್ಣಾಙ್ಕ	flags,
				      ವರ್ಣ *block_buf,
				      ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  ಪೂರ್ಣಾಙ್ಕ	blockcnt,
						  ಶೂನ್ಯ	*priv_data),
				      ಶೂನ್ಯ *priv_data);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				ಪೂರ್ಣಾಙ್ಕ	flags,
				ವರ್ಣ *block_buf,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
					    ಶೂನ್ಯ	*priv_data),
				ಶೂನ್ಯ *priv_data);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಙ್ಕ	flags,
				ವರ್ಣ *block_buf,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
					    ಶೂನ್ಯ	*priv_data),
				ಶೂನ್ಯ *priv_data);

/* bmap.c */
ಬಾಹ್ಯ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_inode *inode,
			     ವರ್ಣ *block_buf, ಪೂರ್ಣಾಙ್ಕ bmap_flags,
			     blk_t block, blk_t *phys_blk);
ಬಾಹ್ಯ errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ಸಂರಚನಾ ext2_inode *inode,
			      ವರ್ಣ *block_buf, ಪೂರ್ಣಾಙ್ಕ bmap_flags, blk64_t block,
			      ಪೂರ್ಣಾಙ್ಕ *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   ಸಂರಚನಾ ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#ಯದಿ 0
/* bmove.c */
ಬಾಹ್ಯ errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    ಪೂರ್ಣಾಙ್ಕ flags);
#endif

/* check_desc.c */
ಬಾಹ್ಯ errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
ಬಾಹ್ಯ errcode_t ext2fs_close(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_close2(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_close_free(ext2_filsys *fs);
ಬಾಹ್ಯ errcode_t ext2fs_flush(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_flush2(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
ಬಾಹ್ಯ errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    ಪೂರ್ಣಾಙ್ಕ *ret_meta_bg);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
ಬಾಹ್ಯ __u32 ext2fs_crc32_be(__u32 crc, unsigned ವರ್ಣ ಧ್ರುವ *p, size_t len);
ಬಾಹ್ಯ __u32 ext2fs_crc32c_le(__u32 crc, unsigned ವರ್ಣ ಧ್ರುವ *p, size_t len);

/* csum.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_init_csum_seed(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ಸಂರಚನಾ mmp_struct *mmp);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_mmp_csum_verify(ext2_filsys, ಸಂರಚನಾ mmp_struct *mmp);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_verify_csum_type(ext2_filsys fs, ಸಂರಚನಾ ext2_super_block *sb);
ಬಾಹ್ಯ errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    ಸಂರಚನಾ ext2_super_block *sb);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ಸಂರಚನಾ ext2_super_block *sb);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					ಸಂರಚನಾ ext2_ext_attr_header *hdr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     ಸಂರಚನಾ ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_TAIL(block, blocksize) \
	((ಸಂರಚನಾ ext2_dir_entry_tail *)(((ವರ್ಣ *)(block)) + \
	(blocksize) - ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail)))

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_initialize_dirent_tail(ext2_filsys fs,
					  ಸಂರಚನಾ ext2_dir_entry_tail *t);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_has_tail(ext2_filsys fs,
				  ಸಂರಚನಾ ext2_dir_entry *dirent);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ಸಂರಚನಾ ext2_dir_entry *dirent);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ಸಂರಚನಾ ext2_dir_entry *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   ಸಂರಚನಾ ext2_dir_entry *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ಸಂರಚನಾ ext2_dir_entry *dirent,
					  ಸಂರಚನಾ ext2_dx_countlimit **cc,
					  ಪೂರ್ಣಾಙ್ಕ *offset);
ಬಾಹ್ಯ errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ಸಂರಚನಾ ext2_dir_entry *dirent,
				__u32 *crc, ಸಂರಚನಾ ext2_dx_tail **ret_t);
ಬಾಹ್ಯ errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      ಸಂರಚನಾ ext3_extent_header *eh);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ಸಂರಚನಾ ext3_extent_header *eh);
ಬಾಹ್ಯ errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ಸಂರಚನಾ ext2_inode_large *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ಸಂರಚನಾ ext2_inode_large *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
ಬಾಹ್ಯ __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
ಬಾಹ್ಯ errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
ಬಾಹ್ಯ errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ಪೂರ್ಣಾಙ್ಕ blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ಧ್ರುವ ಶೂನ್ಯ *,
							   ಧ್ರುವ ಶೂನ್ಯ *));
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ಧ್ರುವ ಶೂನ್ಯ *,
							    ಧ್ರುವ ಶೂನ್ಯ *));
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs, ಸಂರಚನಾ ext2_db_entry *db_info,
		    ಶೂನ್ಯ	*priv_data),
	ಶೂನ್ಯ *priv_data);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs, ಸಂರಚನಾ ext2_db_entry2 *db_info,
		    ಶೂನ್ಯ	*priv_data),
	ಶೂನ್ಯ *priv_data);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs, ಸಂರಚನಾ ext2_db_entry2 *db_info,
		    ಶೂನ್ಯ	*priv_data),
	unsigned long long start,
	unsigned long long ಎಣಿಕೆ,
	ಶೂನ್ಯ *priv_data);
ಬಾಹ್ಯ errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ಪೂರ್ಣಾಙ್ಕ blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ಬಾಹ್ಯ errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dblist_ಎಣಿಕೆ(ext2_dblist dblist);
ಬಾಹ್ಯ blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					ಸಂರಚನಾ ext2_db_entry **entry);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ಸಂರಚನಾ ext2_db_entry2 **entry);
ಬಾಹ್ಯ errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
ಬಾಹ್ಯ errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  ಪೂರ್ಣಾಙ್ಕ	flags,
				  ವರ್ಣ	*block_buf,
				  ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_ino_t	dir,
					      ಪೂರ್ಣಾಙ್ಕ		entry,
					      ಸಂರಚನಾ ext2_dir_entry *dirent,
					      ಪೂರ್ಣಾಙ್ಕ	offset,
					      ಪೂರ್ಣಾಙ್ಕ	blocksize,
					      ವರ್ಣ	*buf,
					      ಶೂನ್ಯ	*priv_data),
				  ಶೂನ್ಯ *priv_data);

#ಯದಿ 0
/* digest_encode.c */
#define EXT2FS_DIGEST_SIZE EXT2FS_SHA256_LENGTH
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_digest_encode(ಧ್ರುವ ವರ್ಣ *src, ಪೂರ್ಣಾಙ್ಕ len, ವರ್ಣ *dst);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_digest_decode(ಧ್ರುವ ವರ್ಣ *src, ಪೂರ್ಣಾಙ್ಕ len, ವರ್ಣ *dst);
#endif

/* dirblock.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags, ext2_ino_t ino);

/* dirhash.c */
ಬಾಹ್ಯ errcode_t ext2fs_dirhash(ಪೂರ್ಣಾಙ್ಕ version, ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len,
				ಧ್ರುವ __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

ಬಾಹ್ಯ errcode_t ext2fs_dirhash2(ಪೂರ್ಣಾಙ್ಕ version, ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len,
				 ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *charset,
				 ಪೂರ್ಣಾಙ್ಕ hash_flags,
				 ಧ್ರುವ __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ಸಂರಚನಾ ext2_dir_entry *dirent,
				    unsigned ಪೂರ್ಣಾಙ್ಕ *rec_len);
ಬಾಹ್ಯ errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    unsigned ಪೂರ್ಣಾಙ್ಕ len,
				    ಸಂರಚನಾ ext2_dir_entry *dirent);
ಬಾಹ್ಯ errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಙ್ಕ flags,
			      ವರ್ಣ *block_buf,
			      ಪೂರ್ಣಾಙ್ಕ (*func)(ಸಂರಚನಾ ext2_dir_entry *dirent,
					  ಪೂರ್ಣಾಙ್ಕ	offset,
					  ಪೂರ್ಣಾಙ್ಕ	blocksize,
					  ವರ್ಣ	*buf,
					  ಶೂನ್ಯ	*priv_data),
			      ಶೂನ್ಯ *priv_data);
ಬಾಹ್ಯ errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಙ್ಕ flags,
			      ವರ್ಣ *block_buf,
			      ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_ino_t	dir,
					  ಪೂರ್ಣಾಙ್ಕ	entry,
					  ಸಂರಚನಾ ext2_dir_entry *dirent,
					  ಪೂರ್ಣಾಙ್ಕ	offset,
					  ಪೂರ್ಣಾಙ್ಕ	blocksize,
					  ವರ್ಣ	*buf,
					  ಶೂನ್ಯ	*priv_data),
			      ಶೂನ್ಯ *priv_data);

/* dupfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
ಬಾಹ್ಯ errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
ಬಾಹ್ಯ __u32 ext2fs_ext_attr_hash_entry(ಸಂರಚನಾ ext2_ext_attr_entry *entry,
					ಶೂನ್ಯ *data);
ಬಾಹ್ಯ __u32 ext2fs_ext_attr_hash_entry_signed(ಸಂರಚನಾ ext2_ext_attr_entry *entry,
					       ಶೂನ್ಯ *data);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ಸಂರಚನಾ ext2_ext_attr_entry *entry,
					     ಶೂನ್ಯ *data, __u32 *hash);
ಬಾಹ್ಯ errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ಸಂರಚನಾ ext2_ext_attr_entry *entry,
					     ಶೂನ್ಯ *data, __u32 *hash,
					     __u32 *signed_hash);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       ಶೂನ್ಯ *buf, ext2_ino_t inum);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       ಶೂನ್ಯ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       ಶೂನ್ಯ *buf, ext2_ino_t inum);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   ವರ್ಣ *block_buf,
					   ಪೂರ್ಣಾಙ್ಕ adjust, __u32 *newcount);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   ವರ್ಣ *block_buf,
					   ಪೂರ್ಣಾಙ್ಕ adjust, __u32 *newcount);
ಬಾಹ್ಯ errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   ವರ್ಣ *block_buf,
					   ಪೂರ್ಣಾಙ್ಕ adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ಸಂರಚನಾ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ಸಂರಚನಾ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ಸಂರಚನಾ ext2_xattr_handle *handle,
				   ಸಂರಚನಾ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ಸಂರಚನಾ ext2_xattr_handle *h,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ವರ್ಣ *name, ವರ್ಣ *ಮೌಲ್ಯ,
					    size_t ಮೌಲ್ಯ_len, ಶೂನ್ಯ *data),
				ಶೂನ್ಯ *data);
errcode_t ext2fs_xattr_get(ಸಂರಚನಾ ext2_xattr_handle *h, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
			   ಶೂನ್ಯ **ಮೌಲ್ಯ, size_t *ಮೌಲ್ಯ_len);
errcode_t ext2fs_xattr_set(ಸಂರಚನಾ ext2_xattr_handle *handle,
			   ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
			   ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ,
			   size_t ಮೌಲ್ಯ_len);
errcode_t ext2fs_xattr_remove(ಸಂರಚನಾ ext2_xattr_handle *handle,
			      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ);
errcode_t ext2fs_xattr_remove_all(ಸಂರಚನಾ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ಸಂರಚನಾ ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ಸಂರಚನಾ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_xattr_handle *handle, size_t *ಎಣಿಕೆ);
errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ಸಂರಚನಾ ext2_xattr_handle *handle,
			      unsigned ಪೂರ್ಣಾಙ್ಕ *new_flags, unsigned ಪೂರ್ಣಾಙ್ಕ *old_flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ext_attr_block_rehash(ಸಂರಚನಾ ext2_ext_attr_header *header,
					 ಸಂರಚನಾ ext2_ext_attr_entry *end);
ಬಾಹ್ಯ __u32 ext2fs_get_ea_inode_hash(ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_ea_inode_hash(ಸಂರಚನಾ ext2_inode *inode, __u32 hash);
ಬಾಹ್ಯ __u64 ext2fs_get_ea_inode_ref(ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_ea_inode_ref(ಸಂರಚನಾ ext2_inode *inode, __u64 ref_ಎಣಿಕೆ);

/* extent.c */
ಬಾಹ್ಯ errcode_t ext2fs_extent_header_verify(ಶೂನ್ಯ *ptr, ಪೂರ್ಣಾಙ್ಕ size);
ಬಾಹ್ಯ errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ಸಂರಚನಾ ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_extent_free(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   ಪೂರ್ಣಾಙ್ಕ flags, ಸಂರಚನಾ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಗಂಟು_split(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, ಪೂರ್ಣಾಙ್ಕ flags,
				       ಸಂರಚನಾ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_ಸೇರಿಸು(ext2_extent_handle_t handle, ಪೂರ್ಣಾಙ್ಕ flags,
				      ಸಂರಚನಾ ext2fs_extent *extent);
ಬಾಹ್ಯ errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ಸಂರಚನಾ ext2_extent_info *info);
ಬಾಹ್ಯ errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     ಪೂರ್ಣಾಙ್ಕ leaf_level, blk64_t blk);
ಬಾಹ್ಯ errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
ಬಾಹ್ಯ size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle);
ಬಾಹ್ಯ errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ errcode_t ext2fs_ಎಣಿಕೆ_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ಸಂರಚನಾ ext2_inode *inode, blk64_t *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_decode_extent(ಸಂರಚನಾ ext2fs_extent *to, ಶೂನ್ಯ *from,
				      ಪೂರ್ಣಾಙ್ಕ len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ext2_ino_t ino,
			   ಸಂರಚನಾ ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
ಬಾಹ್ಯ errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ಸಂರಚನಾ ext2_inode *inode,
				   ಪೂರ್ಣಾಙ್ಕ flags, ext2_file_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  ಪೂರ್ಣಾಙ್ಕ flags, ext2_file_t *ret);
ಬಾಹ್ಯ ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ಸಂರಚನಾ ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
ಬಾಹ್ಯ ext2_ino_t ext2fs_file_get_inode_ಅಂಕೆ(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_close(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_flush(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_read(ext2_file_t file, ಶೂನ್ಯ *buf,
				  unsigned ಪೂರ್ಣಾಙ್ಕ wanted, unsigned ಪೂರ್ಣಾಙ್ಕ *got);
ಬಾಹ್ಯ errcode_t ext2fs_file_write(ext2_file_t file, ಧ್ರುವ ಶೂನ್ಯ *buf,
				   unsigned ಪೂರ್ಣಾಙ್ಕ nbytes, unsigned ಪೂರ್ಣಾಙ್ಕ *written);
ಬಾಹ್ಯ errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   ಪೂರ್ಣಾಙ್ಕ whence, __u64 *ret_pos);
ಬಾಹ್ಯ errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   ಪೂರ್ಣಾಙ್ಕ whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size);
ಬಾಹ್ಯ ext2_off_t ext2fs_file_get_size(ext2_file_t file);
ಬಾಹ್ಯ errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size);
ಬಾಹ್ಯ errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size);

/* finddev.c */
ಬಾಹ್ಯ ವರ್ಣ *ext2fs_find_block_device(dev_t device);

/* flushb.c */
ಬಾಹ್ಯ errcode_t ext2fs_sync_device(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flushb);

/* freefs.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_dblist(ext2_dblist dblist);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_badblocks_list_free(ext2_badblocks_list bb);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 end,
					    __u32 real_end,
					    ಧ್ರುವ ವರ್ಣ *descr, ವರ್ಣ *init_map,
					    ext2fs_generic_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						ಧ್ರುವ ವರ್ಣ *descr,
						ext2fs_generic_bitmap *ret);
ಬಾಹ್ಯ errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
ಬಾಹ್ಯ errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
ಬಾಹ್ಯ errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
ಬಾಹ್ಯ errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 ಅಂಕೆ,
						 ಶೂನ್ಯ *out);
ಬಾಹ್ಯ errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 ಅಂಕೆ,
						 ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
ಬಾಹ್ಯ errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
ಶೂನ್ಯ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ಪೂರ್ಣಾಙ್ಕ type, __u64 start, __u64 end,
				    __u64 real_end,
				    ಧ್ರುವ ವರ್ಣ *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
ಶೂನ್ಯ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
ಶೂನ್ಯ ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					ಶೂನ್ಯ *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_ಎಣಿಕೆ_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_ಎಣಿಕೆ_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ ext2fs_list_backups(ext2_filsys fs, unsigned ಪೂರ್ಣಾಙ್ಕ *three,
				unsigned ಪೂರ್ಣಾಙ್ಕ *five, unsigned ಪೂರ್ಣಾಙ್ಕ *seven);

/* getenv.c */
ಬಾಹ್ಯ ವರ್ಣ *ext2fs_safe_getenv(ಧ್ರುವ ವರ್ಣ *arg);

/* get_num_dirs.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_ಅಂಕೆ_dirs(ext2_filsys fs, ext2_ino_t *ret_ಅಂಕೆ_dirs);

/* getsize.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_device_size(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
					blk_t *retblocks);
ಬಾಹ್ಯ errcode_t ext2fs_get_device_size2(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_get_dio_alignment(ಪೂರ್ಣಾಙ್ಕ fd);
errcode_t ext2fs_get_device_sectsize(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode, blk64_t b);

/* imager.c */
ಬಾಹ್ಯ errcode_t ext2fs_image_inode_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_inode_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_super_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_super_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf);

/* initialize.c */
ಬಾಹ್ಯ errcode_t ext2fs_initialize(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags,
				   ಸಂರಚನಾ ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
ಬಾಹ್ಯ errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ super_only);

/* icount.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_icount(ext2_icount_t icount);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, ವರ್ಣ *tdb_dir,
					  ಪೂರ್ಣಾಙ್ಕ flags, ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount2(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
				       unsigned ಪೂರ್ಣಾಙ್ಕ size,
				       ext2_icount_t hint, ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_create_icount(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
				      unsigned ಪೂರ್ಣಾಙ್ಕ size,
				      ext2_icount_t *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ಬಾಹ್ಯ errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 ಎಣಿಕೆ);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

ಬಾಹ್ಯ errcode_t ext2fs_get_memalign(unsigned long size,
				     unsigned long align, ಶೂನ್ಯ *ptr);

/* inline_data.c */
ಬಾಹ್ಯ errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino,
					 size_t *size);
ಬಾಹ್ಯ errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
					ಸಂರಚನಾ ext2_inode *inode,
					ಶೂನ್ಯ *buf, size_t *size);
ಬಾಹ್ಯ errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
					ಸಂರಚನಾ ext2_inode *inode,
					ಶೂನ್ಯ *buf, size_t size);

/* inode.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned ಪೂರ್ಣಾಙ್ಕ cache_size);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_inode_cache(ಸಂರಚನಾ ext2_inode_cache *icache);
ಬಾಹ್ಯ errcode_t ext2fs_flush_icache(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    ಸಂರಚನಾ ext2_inode *inode,
					    ಪೂರ್ಣಾಙ್ಕ bufsize);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8
ಬಾಹ್ಯ errcode_t ext2fs_open_inode_scan(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ buffer_blocks,
				  ext2_inode_scan *ret_scan);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_close_inode_scan(ext2_inode_scan scan);
ಬಾಹ್ಯ errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   ಪೂರ್ಣಾಙ್ಕ	group);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 ಶೂನ್ಯ * priv_data),
	 ಶೂನ್ಯ *done_group_data);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_scan_flags(ext2_inode_scan scan, ಪೂರ್ಣಾಙ್ಕ set_flags,
				   ಪೂರ್ಣಾಙ್ಕ clear_flags);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					ಸಂರಚನಾ ext2_inode * inode,
					ಪೂರ್ಣಾಙ್ಕ bufsize);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ಸಂರಚನಾ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ಸಂರಚನಾ ext2_inode * inode,
				    ಪೂರ್ಣಾಙ್ಕ bufsize, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 ಸಂರಚನಾ ext2_inode * inode,
					 ಪೂರ್ಣಾಙ್ಕ bufsize);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ಸಂರಚನಾ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ಸಂರಚನಾ ext2_inode * inode,
				     ಪೂರ್ಣಾಙ್ಕ bufsize, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ಸಂರಚನಾ ext2_inode * inode);
ಬಾಹ್ಯ errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
ಬಾಹ್ಯ errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
ಬಾಹ್ಯ io_manager inode_io_manager;
ಬಾಹ್ಯ errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					ವರ್ಣ **name);
ಬಾಹ್ಯ errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ಸಂರಚನಾ ext2_inode *inode,
					 ವರ್ಣ **name);

/* ismounted.c */
ಬಾಹ್ಯ errcode_t ext2fs_check_if_mounted(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *mount_flags);
ಬಾಹ್ಯ errcode_t ext2fs_check_mount_point(ಧ್ರುವ ವರ್ಣ *device, ಪೂರ್ಣಾಙ್ಕ *mount_flags,
					  ವರ್ಣ *mtpt, ಪೂರ್ಣಾಙ್ಕ mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
ಬಾಹ್ಯ errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ಸಂರಚನಾ ext2_inode *inode,
			      ವರ್ಣ *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
ಬಾಹ್ಯ errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ಧ್ರುವ ವರ್ಣ *name,
			 ಪೂರ್ಣಾಙ್ಕ namelen, ವರ್ಣ *buf, ext2_ino_t *inode);
ಬಾಹ್ಯ errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			      ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t *inode);
ಬಾಹ್ಯ errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
ಪೂರ್ಣಾಙ್ಕ ext2fs_native_flag(ಶೂನ್ಯ);

/* newdir.c */
ಬಾಹ್ಯ errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, ವರ್ಣ **block);
ಬಾಹ್ಯ errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *ext2fs_load_nls_table(ಪೂರ್ಣಾಙ್ಕ encoding);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_check_encoded_name(ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *table,
				     ವರ್ಣ *s, size_t len, ವರ್ಣ **pos);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_casefold_cmp(ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *table,
			       ಧ್ರುವ unsigned ವರ್ಣ *str1, size_t len1,
			       ಧ್ರುವ unsigned ವರ್ಣ *str2, size_t len2);

/* mkdir.c */
ಬಾಹ್ಯ errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      ಧ್ರುವ ವರ್ಣ *name);
ಬಾಹ್ಯ errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, unsigned long flags,
			       ಪೂರ್ಣಾಙ್ಕ link_flags, ಧ್ರುವ ವರ್ಣ *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ಸಂರಚನಾ ext2fs_journal_params {
	blk_t ಅಂಕೆ_journal_blocks;
	blk_t ಅಂಕೆ_fc_blocks;
};
ಬಾಹ್ಯ errcode_t ext2fs_get_journal_params(
		ಸಂರಚನಾ ext2fs_journal_params *params, ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
				    blk_t *ret_blk, ಪೂರ್ಣಾಙ್ಕ *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
				     blk64_t *ret_blk, ಪೂರ್ಣಾಙ್ಕ *ret_ಎಣಿಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 ಅಂಕೆ_blocks, ಪೂರ್ಣಾಙ್ಕ flags,
						  ವರ್ಣ  **ret_jsb);
ಬಾಹ್ಯ errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ಸಂರಚನಾ ext2fs_journal_params *params,
						  ಪೂರ್ಣಾಙ್ಕ flags, ವರ್ಣ  **ret_jsb);
ಬಾಹ್ಯ errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
ಬಾಹ್ಯ errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t ಅಂಕೆ_blocks,
					  ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t ಅಂಕೆ_blocks,
					   blk64_t goal, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    ಸಂರಚನಾ ext2fs_journal_params *params,
				    blk64_t goal, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_default_journal_size(__u64 ಅಂಕೆ_blocks);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_journal_sb_start(ಪೂರ್ಣಾಙ್ಕ blocksize);

/* openfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ superblock,
			     unsigned ಪೂರ್ಣಾಙ್ಕ block_size, io_manager manager,
			     ext2_filsys *ret_fs);
ಬಾಹ್ಯ errcode_t ext2fs_open2(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *io_options,
			      ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ superblock,
			      unsigned ಪೂರ್ಣಾಙ್ಕ block_size, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
ಬಾಹ್ಯ blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
ಬಾಹ್ಯ blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ಅಂಕೆ_blocks);
ಬಾಹ್ಯ errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
ಬಾಹ್ಯ e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
ಬಾಹ್ಯ __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      ವರ್ಣ *buf);
ಬಾಹ್ಯ errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   ವರ್ಣ *buf);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, ವರ್ಣ *buf);

/* get_pathname.c */
ಬಾಹ್ಯ errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       ವರ್ಣ **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ಧ್ರುವ ವರ್ಣ *name,
		      ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ಧ್ರುವ ವರ್ಣ *name,
			ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_is_dx(ext2_filsys fs, ಧ್ರುವ ಸಂರಚನಾ ext2_inode *inode);
ಶೂನ್ಯ ext2fs_inc_nlink(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode);
ಶೂನ್ಯ ext2fs_dec_nlink(ಸಂರಚನಾ ext2_inode *inode);
ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_link_max(ext2_filsys fs, ಸಂರಚನಾ ext2_inode_large *inode);
ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_link_empty(ಸಂರಚನಾ ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *target);
ಪೂರ್ಣಾಙ್ಕ ext2fs_is_fast_symlink(ಸಂರಚನಾ ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
unsigned ext2fs_mmp_new_seq(ಶೂನ್ಯ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ಶೂನ್ಯ **ptr);

/* read_bb.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      ಶೂನ್ಯ *priv_data,
				      ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      ವರ್ಣ *badstr,
						      ಶೂನ್ಯ *priv_data));
ಬಾಹ್ಯ errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
ಬಾಹ್ಯ errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
ಬಾಹ್ಯ errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_threads);
ಬಾಹ್ಯ errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
ಬಾಹ್ಯ errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2FS_SHA256_LENGTH 32
#ಯದಿ 0
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_sha256(ಧ್ರುವ unsigned ವರ್ಣ *in, unsigned long in_size,
		   unsigned ವರ್ಣ out[EXT2FS_SHA256_LENGTH]);
#endif

/* sha512.c */
#define EXT2FS_SHA512_LENGTH 64
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_sha512(ಧ್ರುವ unsigned ವರ್ಣ *in, unsigned long in_size,
			  unsigned ವರ್ಣ out[EXT2FS_SHA512_LENGTH]);

/* swapfs.c */
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, ವರ್ಣ *buf, size_t size,
					ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, ವರ್ಣ *buf, size_t size,
					 ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_ext_attr(ವರ್ಣ *to, ವರ್ಣ *from, ಪೂರ್ಣಾಙ್ಕ bufsize,
				 ಪೂರ್ಣಾಙ್ಕ has_header);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_ext_attr_header(ಸಂರಚನಾ ext2_ext_attr_header *to_header,
					ಸಂರಚನಾ ext2_ext_attr_header *from_hdr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_ext_attr_entry(ಸಂರಚನಾ ext2_ext_attr_entry *to_entry,
				       ಸಂರಚನಾ ext2_ext_attr_entry *from_entry);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_super(ಸಂರಚನಾ ext2_super_block * super);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_group_desc(ಸಂರಚನಾ ext2_group_desc *gdp);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_group_desc2(ext2_filsys, ಸಂರಚನಾ ext2_group_desc *gdp);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_inode_full(ext2_filsys fs, ಸಂರಚನಾ ext2_inode_large *t,
				   ಸಂರಚನಾ ext2_inode_large *f, ಪೂರ್ಣಾಙ್ಕ hostorder,
				   ಪೂರ್ಣಾಙ್ಕ bufsize);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_inode(ext2_filsys fs,ಸಂರಚನಾ ext2_inode *t,
			      ಸಂರಚನಾ ext2_inode *f, ಪೂರ್ಣಾಙ್ಕ hostorder);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_swap_mmp(ಸಂರಚನಾ mmp_struct *mmp);

/* unix_io.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_open_file(ಧ್ರುವ ವರ್ಣ *pathname, ಪೂರ್ಣಾಙ್ಕ flags, mode_t mode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_stat(ಧ್ರುವ ವರ್ಣ *path, ext2fs_struct_stat *buf);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_fstat(ಪೂರ್ಣಾಙ್ಕ fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_has_valid_blocks(ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ಸಂರಚನಾ ext2_inode *inode);

/* version.c */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_parse_version_string(ಧ್ರುವ ವರ್ಣ *ver_string);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_get_library_version(ಧ್ರುವ ವರ್ಣ **ver_string,
				      ಧ್ರುವ ವರ್ಣ **date_string);

/* write_bb_file.c */
ಬಾಹ್ಯ errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      unsigned ಪೂರ್ಣಾಙ್ಕ flags,
				      FILE *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
ಬಾಹ್ಯ errcode_t ext2fs_get_mem(unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_memzero(unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_array(unsigned long ಎಣಿಕೆ,
				  unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_get_arrayzero(unsigned long ಎಣಿಕೆ,
				      unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_free_mem(ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_resize_mem(unsigned long old_size,
				   unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t ext2fs_resize_array(unsigned long old_ಎಣಿಕೆ, unsigned long ಎಣಿಕೆ,
				     unsigned long size, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_super_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_changed(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_changed(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_unmark_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_valid(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_ib_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_bb_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_ib_dirty(ext2_filsys fs);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_bb_dirty(ext2_filsys fs);
ಬಾಹ್ಯ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
ಬಾಹ್ಯ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
ಬಾಹ್ಯ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
				      ಸಂರಚನಾ ext2_inode *inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ blocks);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_log2_u32(__u32 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_log2_u64(__u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_log10_u32(__u32 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_log10_u64(__u64 arg);
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ ext2fs_div_ceil(unsigned ಪೂರ್ಣಾಙ್ಕ a, unsigned ಪೂರ್ಣಾಙ್ಕ b);
ಬಾಹ್ಯ __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_name_len(ಧ್ರುವ ಸಂರಚನಾ ext2_dir_entry *entry);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dirent_set_name_len(ಸಂರಚನಾ ext2_dir_entry *entry, ಪೂರ್ಣಾಙ್ಕ len);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_file_type(ಧ್ರುವ ಸಂರಚನಾ ext2_dir_entry *entry);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_dirent_set_file_type(ಸಂರಚನಾ ext2_dir_entry *entry, ಪೂರ್ಣಾಙ್ಕ type);
ಬಾಹ್ಯ ಸಂರಚನಾ ext2_inode *ext2fs_inode(ಸಂರಚನಾ ext2_inode_large * large_inode);
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ ext2_inode *ext2fs_const_inode(ಧ್ರುವ ಸಂರಚನಾ ext2_inode_large * large_inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inodes_per_orphan_block(ext2_filsys fs);
ಬಾಹ್ಯ ಸಂರಚನಾ ext4_orphan_block_tail *ext2fs_orphan_block_tail(ext2_filsys fs,
							       ವರ್ಣ *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#ಯದಿ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ ಬಾಹ್ಯ
#ಅನ್ಯಥಾ
#ಯದಿ (__STDC_VERSION__ >= 199901L)
#define _INLINE_ ಅನ್ತಃಸ್ಥ
#ಅನ್ಯಥಾ
#ifdef __GNUC__
#define _INLINE_ ಬಾಹ್ಯ __inline__
#ಅನ್ಯಥಾ				/* For Watcom C */
#define _INLINE_ ಬಾಹ್ಯ ಅನ್ತಃಸ್ಥ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#ಅನ್ತರ್ಭಾವ <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long size, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *pp;

	pp = malloc(size);
	ಯದಿ (!pp)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, ಪರಿಮಾಣ (pp));
	ಪ್ರತ್ಯಾಗಮ 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long size, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *pp;

	pp = malloc(size);
	ಯದಿ (!pp)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	memset(pp, 0, size);
	memcpy(ptr, &pp, ಪರಿಮಾಣ(pp));
	ಪ್ರತ್ಯಾಗಮ 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long ಎಣಿಕೆ, unsigned long size,
				    ಶೂನ್ಯ *ptr)
{
	ಯದಿ (ಎಣಿಕೆ && (~0UL)/ಎಣಿಕೆ < size)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	ಪ್ರತ್ಯಾಗಮ ext2fs_get_mem(ಎಣಿಕೆ*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long ಎಣಿಕೆ,
					unsigned long size, ಶೂನ್ಯ *ptr)
{
	ಯದಿ (ಎಣಿಕೆ && (~0UL)/ಎಣಿಕೆ < size)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	ಪ್ರತ್ಯಾಗಮ ext2fs_get_memzero((size_t)ಎಣಿಕೆ * size, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *p;

	memcpy(&p, ptr, ಪರಿಮಾಣ(p));
	ಮುಕ್ತ(p);
	p = 0;
	memcpy(ptr, &p, ಪರಿಮಾಣ(p));
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_size,
				     unsigned long size, ಶೂನ್ಯ *ptr)
{
	ಶೂನ್ಯ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, ಪರಿಮಾಣ(p));
	p = realloc(p, size);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, ಪರಿಮಾಣ(p));
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(unsigned long size,
				       unsigned long old_ಎಣಿಕೆ,
				       unsigned long ಎಣಿಕೆ, ಶೂನ್ಯ *ptr)
{
	unsigned long old_size;
	errcode_t retval;

	ಯದಿ (ಎಣಿಕೆ && (~0UL)/ಎಣಿಕೆ < size)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	size *= ಎಣಿಕೆ;
	old_size = size * old_ಎಣಿಕೆ;
	retval = ext2fs_resize_mem(old_size, size, ptr);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (size > old_size) {
		ಶೂನ್ಯ *p;

		memcpy(&p, ptr, ಪರಿಮಾಣ(p));
		memset((ವರ್ಣ *)p + old_size, 0, size - old_size);
		memcpy(ptr, &p, ಪರಿಮಾಣ(p));
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_changed(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ (fs->flags & EXT2_FLAG_CHANGED);
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
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_valid(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ ಶೂನ್ಯ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_ib_dirty(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_bb_dirty(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	ಪ್ರತ್ಯಾಗಮ (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	ಪ್ರತ್ಯಾಗಮ (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	ಪ್ರತ್ಯಾಗಮ (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ blocks)
{
	ಪೂರ್ಣಾಙ್ಕ csum_size = 0;

	ಯದಿ ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_tail);
	ಪ್ರತ್ಯಾಗಮ blocks * ((fs->blocksize - (8 + csum_size)) /
						ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_log2_u32(__u32 arg)
{
	ಪೂರ್ಣಾಙ್ಕ l = 0;

	arg >>= 1;
	ಯಾವತ್ (arg) {
		l++;
		arg >>= 1;
	}
	ಪ್ರತ್ಯಾಗಮ l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_log2_u64(__u64 arg)
{
	ಪೂರ್ಣಾಙ್ಕ l = 0;

	arg >>= 1;
	ಯಾವತ್ (arg) {
		l++;
		arg >>= 1;
	}
	ಪ್ರತ್ಯಾಗಮ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_log10_u32(__u32 arg)
{
	ಪೂರ್ಣಾಙ್ಕ l = 0;

	arg /= 10;
	ಯಾವತ್ (arg) {
		l++;
		arg /= 10;
	}
	ಪ್ರತ್ಯಾಗಮ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_log10_u64(__u64 arg)
{
	ಪೂರ್ಣಾಙ್ಕ l = 0;

	arg /= 10;
	ಯಾವತ್ (arg) {
		l++;
		arg /= 10;
	}
	ಪ್ರತ್ಯಾಗಮ l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ unsigned ಪೂರ್ಣಾಙ್ಕ ext2fs_div_ceil(unsigned ಪೂರ್ಣಾಙ್ಕ a, unsigned ಪೂರ್ಣಾಙ್ಕ b)
{
	ಯದಿ (!a)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	ಯದಿ (!a)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ ((a - 1) / b) + 1;
}

_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_name_len(ಧ್ರುವ ಸಂರಚನಾ ext2_dir_entry *entry)
{
	ಪ್ರತ್ಯಾಗಮ entry->name_len & 0xff;
}

_INLINE_ ಶೂನ್ಯ ext2fs_dirent_set_name_len(ಸಂರಚನಾ ext2_dir_entry *entry, ಪೂರ್ಣಾಙ್ಕ len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_file_type(ಧ್ರುವ ಸಂರಚನಾ ext2_dir_entry *entry)
{
	ಪ್ರತ್ಯಾಗಮ entry->name_len >> 8;
}

_INLINE_ ಶೂನ್ಯ ext2fs_dirent_set_file_type(ಸಂರಚನಾ ext2_dir_entry *entry, ಪೂರ್ಣಾಙ್ಕ type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ ಸಂರಚನಾ ext2_inode *ext2fs_inode(ಸಂರಚನಾ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ ext2_inode *) large_inode;
}

_INLINE_ ಧ್ರುವ ಸಂರಚನಾ ext2_inode *
ext2fs_const_inode(ಧ್ರುವ ಸಂರಚನಾ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ಸಂರಚನಾ ext2_inode *) large_inode;
}

_INLINE_ ಪೂರ್ಣಾಙ್ಕ ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ (fs->blocksize - ಪರಿಮಾಣ(ಸಂರಚನಾ ext4_orphan_block_tail)) /
		ಪರಿಮಾಣ(__u32);
}

_INLINE_ ಸಂರಚನಾ ext4_orphan_block_tail *
ext2fs_orphan_block_tail(ext2_filsys fs, ವರ್ಣ *buf)
{
	ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ ext4_orphan_block_tail *)(buf + fs->blocksize -
		ಪರಿಮಾಣ(ಸಂರಚನಾ ext4_orphan_block_tail));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ ext2_dir_htree_level(ext2_filsys fs)
{
	ಯದಿ (ext2fs_has_feature_largedir(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT4_HTREE_LEVEL;

	ಪ್ರತ್ಯಾಗಮ EXT4_HTREE_LEVEL_COMPAT;
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ext2fs_set_dtime(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode)
{
	__u32	t;

	ಯದಿ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	ಅನ್ಯಥಾ
		t = time(ರಿಕ್ತ) & 0xFFFFFFFF;
	ಯದಿ (t < fs->super->s_inodes_ಎಣಿಕೆ)
		t = fs->super->s_inodes_ಎಣಿಕೆ;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
