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

#جيڪڏهن_وصف_نه_ٿيل _EXT2FS_EXT2FS_H
#وصف_ڪريو _EXT2FS_EXT2FS_H

#جيڪڏهن_وصف_ٿيل __GNUC__
#وصف_ڪريو EXT2FS_ATTR(x) __attribute__(x)
#وإلا
#وصف_ڪريو EXT2FS_ATTR(x)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل __nonstring
#جيڪڏهن_وصف_ٿيل __has_attribute
#إذا __has_attribute(__nonstring__)
#وصف_ڪريو __nonstring                    __attribute__((__nonstring__))
#وإلا
#وصف_ڪريو __nonstring
#جيڪڏهن_پورو /* __has_attribute(__nonstring__) */
#وإلا
# define __nonstring
#جيڪڏهن_پورو /* __has_attribute */
#جيڪڏهن_پورو /* __nonstring */

#جيڪڏهن_وصف_ٿيل CONFIG_TDB
#وصف_ڪريو EXT2_FS_NO_TDB_اڻ_استعمال_ٿيل
#وإلا
#وصف_ڪريو EXT2_FS_NO_TDB_اڻ_استعمال_ٿيل	EXT2FS_ATTR((اڻ_استعمال_ٿيل))
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#إذا (!defined(__GNUC__) && !defined(__WATCOMC__))
#وصف_ڪريو NO_INLINE_FUNCS
#جيڪڏهن_پورو

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#وصف_ڪريو SUPERBLOCK_وٿي	1024
#وصف_ڪريو SUPERBLOCK_سائيز		1024

#وصف_ڪريو UUID_STR_سائيز 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#وصف_ڪريو EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#جيڪڏهن_وصف_ٿيل HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <time.h>
#ضمن <errno.h>

#إذا EXT2_FLAT_INCLUDES
#ضمن "e2_types.h"
#ضمن "ext2_fs.h"
#ضمن "ext3_extents.h"
#وإلا
#ضمن <ext2fs/ext2_types.h>
#ضمن <ext2fs/ext2_fs.h>
#ضمن <ext2fs/ext3_extents.h>
#جيڪڏهن_پورو /* EXT2_FLAT_INCLUDES */

تعريفنوع __u32 __bitwise		ext2_ino_t;
تعريفنوع __u32 __bitwise		blk_t;
تعريفنوع __u64 __bitwise		blk64_t;
تعريفنوع __u32 __bitwise		dgrp_t;
تعريفنوع __s32 __bitwise		ext2_off_t;
تعريفنوع __s64 __bitwise		ext2_off64_t;
تعريفنوع __s64 __bitwise		e2_blkcnt_t;
تعريفنوع __u32 __bitwise		ext2_dirhash_t;

#إذا EXT2_FLAT_INCLUDES
#ضمن "com_err.h"
#ضمن "ext2_io.h"
#ضمن "ext2_err.h"
#ضمن "ext2_ext_attr.h"
#وإلا
#ضمن <et/com_err.h>
#ضمن <ext2fs/ext2_io.h>
#ضمن <ext2fs/ext2_err.h>
#ضمن <ext2fs/ext2_ext_attr.h>
#جيڪڏهن_پورو

#ضمن "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#جيڪڏهن_وصف_ٿيل _MSC_VER
#وصف_ڪريو EXT2_QSORT_TYPE صحيح __cdecl
#وإلا
#وصف_ڪريو EXT2_QSORT_TYPE صحيح
#جيڪڏهن_پورو

تعريفنوع بنية struct_ext2_filsys *ext2_filsys;

#وصف_ڪريو EXT2FS_MARK_ERROR 	0
#وصف_ڪريو EXT2FS_UNMARK_ERROR 	1
#وصف_ڪريو EXT2FS_TEST_ERROR	2

بنية ext2fs_struct_generic_bitmap_بنياد {
	errcode_t		magic;
	ext2_filsys 		fs;
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_بنياد *ext2fs_generic_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_بنياد *ext2fs_inode_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_بنياد *ext2fs_بلاڪ_bitmap;

#وصف_ڪريو EXT2_پهريون_INODE(s)	EXT2_پهريون_INO(s)


/*
 * Badblocks list definitions
 */

تعريفنوع بنية ext2_struct_u32_list *ext2_badblocks_list;
تعريفنوع بنية ext2_struct_u32_iterate *ext2_badblocks_iterate;

تعريفنوع بنية ext2_struct_u32_list *ext2_u32_list;
تعريفنوع بنية ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
تعريفنوع بنية ext2_struct_u32_list *badblocks_list;
تعريفنوع بنية ext2_struct_u32_iterate *badblocks_iterate;

#وصف_ڪريو BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
بنية ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
بنية ext2_db_داخلا {
	ext2_ino_t	ino;
	blk_t	blk;
	صحيح	blockcnt;
};

تعريفنوع بنية ext2_struct_dblist *ext2_dblist;

#وصف_ڪريو DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#وصف_ڪريو EXT2_FILE_WRITE		0x0001
#وصف_ڪريو EXT2_FILE_CREATE	0x0002

#وصف_ڪريو EXT2_FILE_نقاب		0x00FF

#وصف_ڪريو EXT2_FILE_BUF_DIRTY	0x4000
#وصف_ڪريو EXT2_FILE_BUF_VALID	0x2000

تعريفنوع بنية ext2_file *ext2_file_t;

#وصف_ڪريو EXT2_SEEK_سيٽ	0
#وصف_ڪريو EXT2_SEEK_CUR	1
#وصف_ڪريو EXT2_SEEK_پڇاڙي	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#وصف_ڪريو EXT2_FLAG_RW			0x01
#وصف_ڪريو EXT2_FLAG_بدليو		0x02
#وصف_ڪريو EXT2_FLAG_DIRTY			0x04
#وصف_ڪريو EXT2_FLAG_VALID			0x08
#وصف_ڪريو EXT2_FLAG_IB_DIRTY		0x10
#وصف_ڪريو EXT2_FLAG_BB_DIRTY		0x20
#وصف_ڪريو EXT2_FLAG_مٽا_سٽا_بائيٽون		0x40
#وصف_ڪريو EXT2_FLAG_مٽا_سٽا_بائيٽون_READ	0x80
#وصف_ڪريو EXT2_FLAG_مٽا_سٽا_بائيٽون_WRITE	0x100
#وصف_ڪريو EXT2_FLAG_MASTER_SB_ONLY	0x200
#وصف_ڪريو EXT2_FLAG_FORCE			0x400
#وصف_ڪريو EXT2_FLAG_SUPER_ONLY		0x800
#وصف_ڪريو EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#وصف_ڪريو EXT2_FLAG_IMAGE_FILE		0x2000
#وصف_ڪريو EXT2_FLAG_EXCLUSIVE		0x4000
#وصف_ڪريو EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#وصف_ڪريو EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#وصف_ڪريو EXT2_FLAG_64_بٽ		0x20000
#وصف_ڪريو EXT2_FLAG_PRINT_ترقي	0x40000
#وصف_ڪريو EXT2_FLAG_سڌو_IO		0x80000
#وصف_ڪريو EXT2_FLAG_SKIP_MMP		0x100000
#وصف_ڪريو EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#وصف_ڪريو EXT2_FLAG_SHARE_DUP		0x400000
#وصف_ڪريو EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#وصف_ڪريو EXT2_FLAG_BBITMAP_پڇ_PROBLEM	0x1000000
#وصف_ڪريو EXT2_FLAG_IBITMAP_پڇ_PROBLEM	0x2000000
#وصف_ڪريو EXT2_FLAG_ڌاڳا		0x4000000
#وصف_ڪريو EXT2_FLAG_IGNORE_مٽا_سٽا_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#وصف_ڪريو EXT2_FLAG2_USE_FAKE_وقت	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#وصف_ڪريو EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#وصف_ڪريو EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#وصف_ڪريو EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#وصف_ڪريو EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#وصف_ڪريو EXT2_JOURNAL_منزل_FC_BLKS_تناسب		64

بنية blk_alloc_ctx;
بنية opaque_ext2_گروهه_desc;

بنية struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	صحيح				flags;
	حرف *				device_name;
	بنية ext2_super_بلاڪ	* 	super;
	بنانشان صحيح			blocksize;
	صحيح				fragsize;
	dgrp_t				گروهه_desc_ڳڻپ;
	بنانشان ڊگهو			desc_بلاڪون;
	بنية opaque_ext2_گروهه_desc *	گروهه_desc;
	بنانشان صحيح			inode_بلاڪون_في_گروهه;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_بلاڪ_bitmap		بلاڪ_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_بلاڪون)(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاڪون);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	بنية ext2_super_بلاڪ *	orig_super;
	بنية ext2_image_hdr *		image_سرو;
	__u32				umask;
	time_t				now;
	صحيح				cluster_تناسب_بٽ;
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
	فراغ *				priv_معلومات;

	/*
	 * Inode cache
	 */
	بنية ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_بلاڪ)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, بنية blk_alloc_ctx *ctx);
	فراغ (*بلاڪ_alloc_stats)(ext2_filsys fs, blk64_t blk, صحيح inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	فراغ *mmp_buf;
	فراغ *mmp_cmp;
	صحيح mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	ڊگهو mmp_آخري_written;

	/* progress operation functions */
	بنية ext2fs_ترقي_ops *ترقي_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_ٻج;

	io_channel			journal_io;
	حرف				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	فراغ (*بلاڪ_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t انگ,
					صحيح inuse);

	/* hashmap for SHA of data blocks */
	بنية ext2fs_hashmap* بلاڪ_sha_map;

	ثابت بنية ext2fs_nls_جدول *encoding;
};

#إذا EXT2_FLAT_INCLUDES
#ضمن "e2_bitops.h"
#وإلا
#ضمن <ext2fs/bitops.h>
#جيڪڏهن_پورو

/*
 * 64-bit bitmap backend types
 */
#وصف_ڪريو EXT2FS_BMAP64_BITARRAY	1
#وصف_ڪريو EXT2FS_BMAP64_RBTREE	2
#وصف_ڪريو EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#وصف_ڪريو بلاڪ_بدليو			1
#وصف_ڪريو بلاڪ_ABORT			2
#وصف_ڪريو بلاڪ_ERROR			4
#وصف_ڪريو بلاڪ_INLINE_معلومات_بدليو	8

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
#وصف_ڪريو بلاڪ_FLAG_APPEND	1
#وصف_ڪريو بلاڪ_FLAG_HOLE		1
#وصف_ڪريو بلاڪ_FLAG_اونهائي_TRAVERSE	2
#وصف_ڪريو بلاڪ_FLAG_معلومات_ONLY	4
#وصف_ڪريو بلاڪ_FLAG_READ_ONLY	8

#وصف_ڪريو بلاڪ_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#وصف_ڪريو بلاڪ_ڳڻپ_IND		(-1)
#وصف_ڪريو بلاڪ_ڳڻپ_DIND	(-2)
#وصف_ڪريو بلاڪ_ڳڻپ_TIND	(-3)
#وصف_ڪريو بلاڪ_ڳڻپ_TRANSLATOR	(-4)

#وصف_ڪريو بلاڪ_ALLOC_UNKNOWN	0
#وصف_ڪريو بلاڪ_ALLOC_معلومات	1
#وصف_ڪريو بلاڪ_ALLOC_METADATA	2

بنية blk_alloc_ctx {
	ext2_ino_t		ino;
	بنية ext2_inode	*inode;
	blk64_t			lblk;
	صحيح			flags;
};

#إذا 0
/*
 * Flags for ext2fs_move_blocks
 */
#وصف_ڪريو EXT2_BMOVE_GET_DBLIST	0x0001
#وصف_ڪريو EXT2_BMOVE_DEBUG	0x0002
#جيڪڏهن_پورو

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#وصف_ڪريو EXT2_EXTENT_FLAGS_پن		0x0001
#وصف_ڪريو EXT2_EXTENT_FLAGS_UNINIT	0x0002
#وصف_ڪريو EXT2_EXTENT_FLAGS_ٻيو_گهمو	0x0004

بنية ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

تعريفنوع بنية ext2_extent_handle *ext2_extent_handle_t;
تعريفنوع بنية ext2_extent_رستو *ext2_extent_رستو_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#وصف_ڪريو EXT2_EXTENT_CURRENT	0x0000
#وصف_ڪريو EXT2_EXTENT_MOVE_نقاب	0x000F
#وصف_ڪريو EXT2_EXTENT_پاڙ	0x0001
#وصف_ڪريو EXT2_EXTENT_آخري_پن	0x0002
#وصف_ڪريو EXT2_EXTENT_پهريون_SIB	0x0003
#وصف_ڪريو EXT2_EXTENT_آخري_SIB	0x0004
#وصف_ڪريو EXT2_EXTENT_ايندڙ_SIB	0x0005
#وصف_ڪريو EXT2_EXTENT_PREV_SIB	0x0006
#وصف_ڪريو EXT2_EXTENT_ايندڙ_پن	0x0007
#وصف_ڪريو EXT2_EXTENT_PREV_پن	0x0008
#وصف_ڪريو EXT2_EXTENT_ايندڙ	0x0009
#وصف_ڪريو EXT2_EXTENT_PREV	0x000A
#وصف_ڪريو EXT2_EXTENT_مٿي		0x000B
#وصف_ڪريو EXT2_EXTENT_هيٺ_ڏانهن	0x000C
#وصف_ڪريو EXT2_EXTENT_هيٺ_ڏانهن_AND_آخري 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#وصف_ڪريو EXT2_EXTENT_وجهو_AFTER	0x0001 /* insert after handle loc'n */
#وصف_ڪريو EXT2_EXTENT_وجهو_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#وصف_ڪريو EXT2_EXTENT_DELETE_KEEP_خالي_جايون	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#وصف_ڪريو EXT2_EXTENT_سيٽ_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
بنية ext2_extent_info {
	صحيح		curr_داخلا;
	صحيح		curr_level;
	صحيح		انگ_entries;
	صحيح		max_entries;
	صحيح		max_اونهائي;
	صحيح		بائيٽون_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#وصف_ڪريو EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#وصف_ڪريو DIRENT_بدليو	1
#وصف_ڪريو DIRENT_ABORT	2
#وصف_ڪريو DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#وصف_ڪريو DIRENT_FLAG_INCLUDE_خالي_جايون	1
#وصف_ڪريو DIRENT_FLAG_INCLUDE_REMOVED	2
#وصف_ڪريو DIRENT_FLAG_INCLUDE_CSUM	4
#وصف_ڪريو DIRENT_FLAG_INCLUDE_INLINE_معلومات 8

#وصف_ڪريو DIRENT_DOT_FILE		1
#وصف_ڪريو DIRENT_DOT_DOT_FILE	2
#وصف_ڪريو DIRENT_OTHER_FILE	3
#وصف_ڪريو DIRENT_DELETED_FILE	4
#وصف_ڪريو DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
تعريفنوع بنية ext2_struct_inode_جاچ *ext2_inode_جاچ;

/*
 * ext2fs_scan flags
 */
#وصف_ڪريو EXT2_SF_CHK_BADBLOCKS	0x0001
#وصف_ڪريو EXT2_SF_BAD_INODE_BLK	0x0002
#وصف_ڪريو EXT2_SF_BAD_EXTRA_بائيٽون	0x0004
#وصف_ڪريو EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#وصف_ڪريو EXT2_SF_DO_LAZY		0x0010
#وصف_ڪريو EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#وصف_ڪريو EXT2_MF_MOUNTED		1
#وصف_ڪريو EXT2_MF_ISROOT		2
#وصف_ڪريو EXT2_MF_READONLY	4
#وصف_ڪريو EXT2_MF_مٽا_سٽا		8
#وصف_ڪريو EXT2_MF_BUSY		16
#وصف_ڪريو EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#وصف_ڪريو LINUX_S_IFMT  00170000
#وصف_ڪريو LINUX_S_IFSOCK 0140000
#وصف_ڪريو LINUX_S_IFLNK	 0120000
#وصف_ڪريو LINUX_S_IFREG  0100000
#وصف_ڪريو LINUX_S_IFBLK  0060000
#وصف_ڪريو LINUX_S_IFDIR  0040000
#وصف_ڪريو LINUX_S_IFCHR  0020000
#وصف_ڪريو LINUX_S_IFIFO  0010000
#وصف_ڪريو LINUX_S_ISUID  0004000
#وصف_ڪريو LINUX_S_ISGID  0002000
#وصف_ڪريو LINUX_S_ISVTX  0001000

#وصف_ڪريو LINUX_S_IRWXU 00700
#وصف_ڪريو LINUX_S_IRUSR 00400
#وصف_ڪريو LINUX_S_IWUSR 00200
#وصف_ڪريو LINUX_S_IXUSR 00100

#وصف_ڪريو LINUX_S_IRWXG 00070
#وصف_ڪريو LINUX_S_IRGRP 00040
#وصف_ڪريو LINUX_S_IWGRP 00020
#وصف_ڪريو LINUX_S_IXGRP 00010

#وصف_ڪريو LINUX_S_IRWXO 00007
#وصف_ڪريو LINUX_S_IROTH 00004
#وصف_ڪريو LINUX_S_IWOTH 00002
#وصف_ڪريو LINUX_S_IXOTH 00001

#وصف_ڪريو LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#وصف_ڪريو LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#وصف_ڪريو LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#وصف_ڪريو LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#وصف_ڪريو LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#وصف_ڪريو LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#وصف_ڪريو LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#وصف_ڪريو EXT2_I_سائيز(i)	((i)->i_سائيز | ((__u64) (i)->i_سائيز_مٿاهون << 32))

ساكن مضمن __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
					__u32 nsec)
{
	__u32 extra = 0;

#إذا (SIZEOF_وقت_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_نقاب;
#جيڪڏهن_پورو
	أرجع extra | (nsec << EXT4_EPOCH_بٽ);
}
ساكن مضمن time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
#إذا (SIZEOF_وقت_T > 4)
	إذا (extra & EXT4_EPOCH_نقاب)
		seconds += ((time_t)(extra & EXT4_EPOCH_نقاب) << 32);
#جيڪڏهن_پورو
	أرجع seconds;
}
ساكن مضمن __u32 __decode_extra_nsec(__u32 extra)
{
	أرجع (extra & EXT4_NSEC_نقاب) >> EXT4_EPOCH_بٽ;
}
#وصف_ڪريو ext2fs_inode_actual_سائيز(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_سائيز +				      \
		  (حجم(*inode) > EXT2_GOOD_OLD_INODE_سائيز ?		      \
		   ((بنية ext2_inode_large *)(inode))->i_extra_isize : 0)))
#وصف_ڪريو clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#وصف_ڪريو ext2fs_inode_xtime_سيٽ(inode, ميدان, sec)			      \
افعل {									      \
	إذا (ext2fs_inode_includes(ext2fs_inode_actual_سائيز(inode),	      \
				  ميدان ## _extra)) {			      \
		(inode)->ميدان = (__u32)(sec & 0xfffffff);		      \
		((بنية ext2_inode_large *)(inode))->ميدان ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} وإلا {							      \
		(inode)->ميدان = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} طالما (0)
#وصف_ڪريو ext2fs_inode_xtime_get(inode, ميدان)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_سائيز(inode), ميدان ## _extra) ?    \
	__decode_extra_sec((inode)->ميدان,				      \
		((بنية ext2_inode_large *)(inode))->ميدان ## _extra) :      \
		(time_t)(inode)->ميدان)

ساكن مضمن فراغ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#إذا (SIZEOF_وقت_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_نقاب;
#وإلا
	*hi = 0;
#جيڪڏهن_پورو
}
ساكن مضمن time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
#إذا (SIZEOF_وقت_T == 4)
	أرجع *lo;
#وإلا
	أرجع ((time_t)(*hi) << 32) | *lo;
#جيڪڏهن_پورو
}
#وصف_ڪريو ext2fs_سيٽ_tstamp(sb, ميدان, seconds) \
	__sb_set_tstamp(&(sb)->ميدان, &(sb)->ميدان ## _hi, seconds)
#وصف_ڪريو ext2fs_get_tstamp(sb, ميدان) \
	__sb_get_tstamp(&(sb)->ميدان, &(sb)->ميدان ## _hi)

/*
 * ext2_icount_t abstraction
 */
#وصف_ڪريو EXT2_ICOUNT_OPT_INCREMENT	0x01
#وصف_ڪريو EXT2_ICOUNT_OPT_FULLMAP		0x02

تعريفنوع بنية ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#وصف_ڪريو BMAP_ALLOC	0x0001
#وصف_ڪريو BMAP_سيٽ	0x0002
#وصف_ڪريو BMAP_UNINIT	0x0004
#وصف_ڪريو BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#وصف_ڪريو BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#وصف_ڪريو READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#وصف_ڪريو WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#وصف_ڪريو IMAGER_FLAG_INODEMAP	1
#وصف_ڪريو IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)

/*
 * Features supported by this version of the library
 */
#وصف_ڪريو EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_اشاريو|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#جيڪڏهن_وصف_ٿيل CONFIG_MMP
#وصف_ڪريو EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#وإلا
#وصف_ڪريو EXT4_LIB_INCOMPAT_MMP		(0)
#جيڪڏهن_پورو

#وصف_ڪريو EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_ٻٽو_انگ|\
					 EXT4_FEATURE_INCOMPAT_INLINE_معلومات|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_ٻج|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#وصف_ڪريو EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_بلاڪون |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#وصف_ڪريو EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#وصف_ڪريو EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#وصف_ڪريو EXT2_FS_CLUSTER_تناسب(fs)	(1 << (fs)->cluster_تناسب_بٽ)
#وصف_ڪريو EXT2_FS_CLUSTER_نقاب(fs)		(EXT2_FS_CLUSTER_تناسب(fs) - 1)
#وصف_ڪريو EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_تناسب_بٽ)
/* Translate a cluster number to a block number */
#وصف_ڪريو EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_تناسب_بٽ)
/* Translate # of blks to # of clusters */
#وصف_ڪريو EXT2_FS_انگ_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_نقاب(fs)) >> \
					 (fs)->cluster_تناسب_بٽ)

#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
تعريفنوع بنية stat64 ext2fs_struct_stat;
#وإلا
تعريفنوع بنية stat ext2fs_struct_stat;
#جيڪڏهن_پورو

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#وصف_ڪريو EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
بنية ext2_xattr_handle;
#وصف_ڪريو XATTR_ABORT	1
#وصف_ڪريو XATTR_بدليو	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#وصف_ڪريو EXT2FS_BITMAPS_WRITE		0x0001
#وصف_ڪريو EXT2_FS_BITMAPS_بلاڪ		0x0002
#وصف_ڪريو EXT2FS_BITMAPS_INODE		0x0004
#وصف_ڪريو EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#وصف_ڪريو EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#وصف_ڪريو EXT2_FS_LINK_FT_نقاب	0x0007
#وصف_ڪريو EXT2FS_LINK_APPEND	0x0010
#وصف_ڪريو EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
ساكن مضمن صحيح ext2fs_has_گروهه_desc_csum(ext2_filsys fs)
{
	أرجع ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ساكن مضمن صحيح ext2fs_needs_large_file_feature(بنانشان ڊگهو ڊگهو file_سائيز)
{
	أرجع file_سائيز >= 0x80000000ULL;
}

/* alloc.c */
خارجي فراغ ext2fs_clear_بلاڪ_uninit(ext2_filsys fs, dgrp_t گروهه);
خارجي errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, صحيح گهڻائي,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
خارجي errcode_t ext2fs_new_بلاڪ(ext2_filsys fs, blk_t goal,
				  ext2fs_بلاڪ_bitmap map, blk_t *ret);
خارجي errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاڪ_bitmap map, blk64_t *ret);
خارجي errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاڪ_bitmap map, blk64_t *ret,
				   بنية blk_alloc_ctx *ctx);
خارجي errcode_t ext2fs_get_free_بلاڪون(ext2_filsys fs, blk_t شروعات,
					blk_t پڄاڻي, صحيح انگ,
					ext2fs_بلاڪ_bitmap map,
					blk_t *ret);
خارجي errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t شروعات,
					 blk64_t پڄاڻي, صحيح انگ,
					 ext2fs_بلاڪ_bitmap map,
					 blk64_t *ret);
خارجي errcode_t ext2fs_alloc_بلاڪ(ext2_filsys fs, blk_t goal,
				    حرف *بلاڪ_buf, blk_t *ret);
خارجي errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     حرف *بلاڪ_buf, blk64_t *ret);
خارجي errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     حرف *بلاڪ_buf, blk64_t *ret,
				     بنية blk_alloc_ctx *ctx);

خارجي فراغ ext2fs_سيٽ_alloc_بلاڪ_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_ڳوليو_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, blk64_t lblk);
خارجي فراغ ext2fs_سيٽ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
خارجي فراغ ext2fs_سيٽ_بلاڪ_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse));
#وصف_ڪريو EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#وصف_ڪريو EXT2_NEWRANGE_MIN_ڊيگهائي	(0x2)
#وصف_ڪريو EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			   blk64_t len, ext2fs_بلاڪ_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#وصف_ڪريو EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#وصف_ڪريو EXT2_ALLOCRANGE_ZERO_بلاڪون	(0x2)
#وصف_ڪريو EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
خارجي صحيح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t گروهه,
					ext2fs_بلاڪ_bitmap bmap);
خارجي errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t گروهه,
					       ext2fs_بلاڪ_bitmap bmap,
					       blk_t *desc_بلاڪون);
خارجي فراغ ext2fs_سيٽ_بلاڪ_alloc_stats_callback(ext2_filsys fs,
						  فراغ (*func)(ext2_filsys fs,
							       blk64_t blk,
							       صحيح inuse),
						  فراغ (**old)(ext2_filsys fs,
							       blk64_t blk,
							       صحيح inuse));

/* alloc_stats.c */
فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse);
فراغ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحيح inuse, صحيح isdir);
فراغ ext2fs_بلاڪ_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse);
فراغ ext2fs_بلاڪ_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse);
فراغ ext2fs_بلاڪ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse);

/* alloc_tables.c */
خارجي errcode_t ext2fs_allocate_tables(ext2_filsys fs);
خارجي errcode_t ext2fs_allocate_گروهه_جدول(ext2_filsys fs, dgrp_t گروهه,
					     ext2fs_بلاڪ_bitmap bmap);

/* badblocks.c */
خارجي errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح سائيز);
خارجي errcode_t ext2fs_u32_list_جوڙيو(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_ڳوليو(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
خارجي errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
خارجي صحيح ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
خارجي فراغ ext2fs_u32_list_iterate_پڇاڙي(ext2_u32_iterate iter);
خارجي errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
خارجي صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

خارجي errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    صحيح سائيز);
خارجي errcode_t ext2fs_badblocks_list_جوڙيو(ext2_badblocks_list bb,
					   blk_t blk);
خارجي صحيح ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
خارجي صحيح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
خارجي فراغ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
خارجي errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
خارجي صحيح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
خارجي فراغ ext2fs_badblocks_list_iterate_پڇاڙي(ext2_badblocks_iterate iter);
خارجي errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
خارجي صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
خارجي صحيح ext2fs_u32_list_ڳڻپ(ext2_u32_list bb);

/* bb_compat */
خارجي errcode_t badblocks_list_create(badblocks_list *ret, صحيح سائيز);
خارجي errcode_t badblocks_list_جوڙيو(badblocks_list bb, blk_t blk);
خارجي صحيح badblocks_list_test(badblocks_list bb, blk_t blk);
خارجي errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
خارجي صحيح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
خارجي فراغ badblocks_list_iterate_پڇاڙي(badblocks_iterate iter);
خارجي فراغ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
خارجي errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
خارجي فراغ ext2fs_free_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap);
خارجي فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
خارجي errcode_t ext2fs_allocate_بلاڪ_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_بلاڪ_bitmap *ret);
خارجي errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ثابت حرف *descr,
						   ext2fs_بلاڪ_bitmap *ret);
خارجي صحيح ext2fs_get_bitmap_granularity(ext2fs_بلاڪ_bitmap bitmap);
خارجي errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_inode_bitmap *ret);
خارجي errcode_t ext2fs_fudge_inode_bitmap_پڇاڙي(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t پڇاڙي, ext2_ino_t *oend);
خارجي errcode_t ext2fs_fudge_بلاڪ_bitmap_پڇاڙي(ext2fs_بلاڪ_bitmap bitmap,
					       blk_t پڇاڙي, blk_t *oend);
خارجي errcode_t ext2fs_fudge_بلاڪ_bitmap_end2(ext2fs_بلاڪ_bitmap bitmap,
					 blk64_t پڇاڙي, blk64_t *oend);
خارجي فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي فراغ ext2fs_clear_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap);
خارجي errcode_t ext2fs_resize_inode_bitmap(__u32 new_پڇاڙي, __u32 new_real_پڇاڙي,
					    ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_inode_bitmap2(__u64 new_پڇاڙي,
					     __u64 new_real_پڇاڙي,
					     ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_بلاڪ_bitmap(__u32 new_پڇاڙي, __u32 new_real_پڇاڙي,
					    ext2fs_بلاڪ_bitmap bmap);
خارجي errcode_t ext2fs_resize_بلاڪ_bitmap2(__u64 new_پڇاڙي,
					     __u64 new_real_پڇاڙي,
					     ext2fs_بلاڪ_bitmap bmap);
خارجي errcode_t ext2fs_compare_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bm1,
					     ext2fs_بلاڪ_bitmap bm2);
خارجي errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
خارجي errcode_t ext2fs_سيٽ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروعات, بنانشان صحيح انگ,
					فراغ *اندريون);
خارجي errcode_t ext2fs_سيٽ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروعات, size_t انگ,
					 فراغ *اندريون);
خارجي errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون);
خارجي errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروعات, size_t انگ,
					 فراغ *ٻاهريون);
خارجي errcode_t ext2fs_سيٽ_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bmap,
					blk_t شروعات, بنانشان صحيح انگ,
					فراغ *اندريون);
خارجي errcode_t ext2fs_سيٽ_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bmap,
					 blk64_t شروعات, size_t انگ,
					 فراغ *اندريون);
خارجي errcode_t ext2fs_get_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bmap,
					blk_t شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون);
خارجي errcode_t ext2fs_get_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bmap,
					 blk64_t شروعات, size_t انگ,
					 فراغ *ٻاهريون);

/* blknum.c */
خارجي __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروهه);
خارجي __u32 ext2fs_بلاڪ_bitmap_checksum(ext2_filsys fs, dgrp_t گروهه);
خارجي dgrp_t ext2fs_گروهه_جو_blk2(ext2_filsys fs, blk64_t);
خارجي blk64_t ext2fs_گروهه_پهريون_block2(ext2_filsys fs, dgrp_t گروهه);
خارجي blk64_t ext2fs_گروهه_آخري_block2(ext2_filsys fs, dgrp_t گروهه);
خارجي صحيح ext2fs_گروهه_بلاڪون_ڳڻپ(ext2_filsys fs, dgrp_t گروهه);
خارجي blk64_t ext2fs_inode_معلومات_blocks2(ext2_filsys fs,
					 بنية ext2_inode *inode);
خارجي blk64_t ext2fs_inode_i_بلاڪون(ext2_filsys fs,
				     بنية ext2_inode *inode);
خارجي blk64_t ext2fs_get_stat_i_بلاڪون(ext2_filsys fs,
					بنية ext2_inode *inode);
خارجي blk64_t ext2fs_بلاڪون_ڳڻپ(بنية ext2_super_بلاڪ *super);
خارجي فراغ ext2fs_بلاڪون_ڳڻپ_سيٽ(بنية ext2_super_بلاڪ *super,
				    blk64_t blk);
خارجي فراغ ext2fs_بلاڪون_ڳڻپ_جوڙيو(بنية ext2_super_بلاڪ *super,
				    blk64_t blk);
خارجي blk64_t ext2fs_r_بلاڪون_ڳڻپ(بنية ext2_super_بلاڪ *super);
خارجي فراغ ext2fs_r_بلاڪون_ڳڻپ_سيٽ(بنية ext2_super_بلاڪ *super,
				      blk64_t blk);
خارجي فراغ ext2fs_r_بلاڪون_ڳڻپ_جوڙيو(بنية ext2_super_بلاڪ *super,
				      blk64_t blk);
خارجي blk64_t ext2fs_free_بلاڪون_ڳڻپ(بنية ext2_super_بلاڪ *super);
خارجي فراغ ext2fs_free_بلاڪون_ڳڻپ_سيٽ(بنية ext2_super_بلاڪ *super,
					 blk64_t blk);
خارجي فراغ ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(بنية ext2_super_بلاڪ *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
خارجي بنية ext2_گروهه_desc *ext2fs_گروهه_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروهه_desc *gdp,
					  dgrp_t گروهه);
خارجي blk64_t ext2fs_بلاڪ_bitmap_csum(ext2_filsys fs, dgrp_t گروهه);
خارجي blk64_t ext2fs_بلاڪ_bitmap_loc(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_بلاڪ_bitmap_loc_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					blk64_t blk);
خارجي __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t گروهه);
خارجي blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_inode_bitmap_loc_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					blk64_t blk);
خارجي blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_inode_جدول_loc_سيٽ(ext2_filsys fs, dgrp_t گروهه,
				       blk64_t blk);
خارجي __u32 ext2fs_bg_free_بلاڪون_ڳڻپ(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					 __u32 n);
خارجي __u32 ext2fs_bg_free_inodes_ڳڻپ(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_free_inodes_ڳڻپ_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					 __u32 n);
خارجي __u32 ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ_سيٽ(ext2_filsys fs, dgrp_t گروهه,
				       __u32 n);
خارجي __u32 ext2fs_bg_itable_اڻ_استعمال_ٿيل(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(ext2_filsys fs, dgrp_t گروهه,
				     __u32 n);
خارجي __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروهه);
خارجي صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروهه, __u16 bg_flag);
خارجي فراغ ext2fs_bg_flags_سيٽ(ext2_filsys fs, dgrp_t گروهه, __u16 bg_flags);
خارجي فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروهه, __u16 bg_flags);
خارجي __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروهه);
خارجي فراغ ext2fs_bg_checksum_سيٽ(ext2_filsys fs, dgrp_t گروهه, __u16 checksum);
خارجي blk64_t ext2fs_file_acl_بلاڪ(ext2_filsys fs,
				     ثابت بنية ext2_inode *inode);
خارجي فراغ ext2fs_file_acl_بلاڪ_سيٽ(ext2_filsys fs,
				      بنية ext2_inode *inode, blk64_t blk);
خارجي errcode_t ext2fs_inode_سائيز_سيٽ(ext2_filsys fs, بنية ext2_inode *inode,
				       ext2_off64_t سائيز);

/* block.c */
خارجي errcode_t ext2fs_بلاڪ_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      صحيح	flags,
				      حرف *بلاڪ_buf,
				      صحيح (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  صحيح	blockcnt,
						  فراغ	*priv_معلومات),
				      فراغ *priv_معلومات);
errcode_t ext2fs_بلاڪ_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				صحيح	flags,
				حرف *بلاڪ_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_وٿي,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات);
errcode_t ext2fs_بلاڪ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاڪ_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_وٿي,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات);

/* bmap.c */
خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *بلاڪ_buf, صحيح bmap_flags,
			     blk_t بلاڪ, blk_t *phys_blk);
خارجي errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *بلاڪ_buf, صحيح bmap_flags, blk64_t بلاڪ,
			      صحيح *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_بلاڪ(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#إذا 0
/* bmove.c */
خارجي errcode_t ext2fs_move_بلاڪون(ext2_filsys fs,
				    ext2fs_بلاڪ_bitmap reserve,
				    ext2fs_بلاڪ_bitmap alloc_map,
				    صحيح flags);
#جيڪڏهن_پورو

/* check_desc.c */
خارجي errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
خارجي errcode_t ext2fs_close(ext2_filsys fs);
خارجي errcode_t ext2fs_close2(ext2_filsys fs, صحيح flags);
خارجي errcode_t ext2fs_close_free(ext2_filsys *fs);
خارجي errcode_t ext2fs_flush(ext2_filsys fs);
خارجي errcode_t ext2fs_flush2(ext2_filsys fs, صحيح flags);
خارجي صحيح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروهه_بلاڪ);
خارجي errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t گروهه,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_استعمال_ٿيل_blks);
خارجي صحيح ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t گروهه,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    صحيح *ret_meta_bg);
خارجي فراغ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
خارجي __u32 ext2fs_چڪري_جاچ_32_be(__u32 چڪري_جاچ, بنانشان حرف ثابت *p, size_t len);
خارجي __u32 ext2fs_crc32c_le(__u32 چڪري_جاچ, بنانشان حرف ثابت *p, size_t len);

/* csum.c */
خارجي فراغ ext2fs_init_csum_ٻج(ext2_filsys fs);
خارجي errcode_t ext2fs_mmp_csum_سيٽ(ext2_filsys fs, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_mmp_csum_verify(ext2_filsys, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلاڪ *sb);
خارجي errcode_t ext2fs_superblock_csum_سيٽ(ext2_filsys fs,
					    بنية ext2_super_بلاڪ *sb);
خارجي صحيح ext2fs_superblock_csum_verify(ext2_filsys fs,
					 بنية ext2_super_بلاڪ *sb);
خارجي errcode_t ext2fs_ext_attr_بلاڪ_csum_سيٽ(ext2_filsys fs,
					ext2_ino_t inum, blk64_t بلاڪ,
					بنية ext2_ext_attr_سرو *hdr);
خارجي صحيح ext2fs_ext_attr_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t بلاڪ,
					     بنية ext2_ext_attr_سرو *hdr);
#وصف_ڪريو EXT2_DIRENT_پڇ(بلاڪ, blocksize) \
	((بنية ext2_dir_داخلا_پڇ *)(((حرف *)(بلاڪ)) + \
	(blocksize) - حجم(بنية ext2_dir_داخلا_پڇ)))

خارجي فراغ ext2fs_initialize_dirent_پڇ(ext2_filsys fs,
					  بنية ext2_dir_داخلا_پڇ *t);
خارجي صحيح ext2fs_dirent_has_پڇ(ext2_filsys fs,
				  بنية ext2_dir_داخلا *dirent);
خارجي صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     بنية ext2_dir_داخلا *dirent);
خارجي صحيح ext2fs_dir_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_داخلا *dirent);
خارجي errcode_t ext2fs_dir_بلاڪ_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
					   بنية ext2_dir_داخلا *dirent);
خارجي errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  بنية ext2_dir_داخلا *dirent,
					  بنية ext2_dx_countlimit **cc,
					  صحيح *وٿي);
خارجي errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				بنية ext2_dir_داخلا *dirent,
				__u32 *چڪري_جاچ, بنية ext2_dx_پڇ **ret_t);
خارجي errcode_t ext2fs_extent_بلاڪ_csum_سيٽ(ext2_filsys fs,
					      ext2_ino_t inum,
					      بنية ext3_extent_سرو *eh);
خارجي صحيح ext2fs_extent_بلاڪ_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   بنية ext3_extent_سرو *eh);
خارجي errcode_t ext2fs_بلاڪ_bitmap_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					      حرف *bitmap, صحيح سائيز);
خارجي صحيح ext2fs_بلاڪ_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروهه,
					   حرف *bitmap, صحيح سائيز);
خارجي errcode_t ext2fs_inode_bitmap_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه,
					      حرف *bitmap, صحيح سائيز);
خارجي صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروهه,
					   حرف *bitmap, صحيح سائيز);
خارجي errcode_t ext2fs_inode_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext2_inode_large *inode);
خارجي صحيح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_inode_large *inode);
خارجي فراغ ext2fs_گروهه_desc_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه);
خارجي صحيح ext2fs_گروهه_desc_csum_verify(ext2_filsys fs, dgrp_t گروهه);
خارجي errcode_t ext2fs_سيٽ_gdt_csum(ext2_filsys fs);
خارجي __u16 ext2fs_گروهه_desc_csum(ext2_filsys fs, dgrp_t گروهه);

/* dblist.c */
خارجي errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
خارجي errcode_t ext2fs_جوڙيو_dir_بلاڪ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_جوڙيو_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي فراغ ext2fs_dblist_ترتيب_ڏيو(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							   ثابت فراغ *));
خارجي فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							    ثابت فراغ *));
خارجي errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_داخلا *db_info,
		    فراغ	*priv_معلومات),
	فراغ *priv_معلومات);
خارجي errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_معلومات),
	فراغ *priv_معلومات);
خارجي errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_معلومات),
	بنانشان ڊگهو ڊگهو شروعات,
	بنانشان ڊگهو ڊگهو ڳڻپ,
	فراغ *priv_معلومات);
خارجي errcode_t ext2fs_سيٽ_dir_بلاڪ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_سيٽ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
خارجي صحيح ext2fs_dblist_ڳڻپ(ext2_dblist dblist);
خارجي blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
خارجي errcode_t ext2fs_dblist_get_آخري(ext2_dblist dblist,
					بنية ext2_db_داخلا **داخلا);
خارجي errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					بنية ext2_db_entry2 **داخلا);
خارجي errcode_t ext2fs_dblist_drop_آخري(ext2_dblist dblist);

/* dblist_dir.c */
خارجي errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  صحيح	flags,
				  حرف	*بلاڪ_buf,
				  صحيح (*func)(ext2_ino_t	dir,
					      صحيح		داخلا,
					      بنية ext2_dir_داخلا *dirent,
					      صحيح	وٿي,
					      صحيح	blocksize,
					      حرف	*buf,
					      فراغ	*priv_معلومات),
				  فراغ *priv_معلومات);

#إذا 0
/* digest_encode.c */
#وصف_ڪريو EXT2_FS_DIGEST_سائيز EXT2_FS_SHA256_ڊيگهائي
خارجي صحيح ext2fs_digest_encode(ثابت حرف *src, صحيح len, حرف *dst);
خارجي صحيح ext2fs_digest_decode(ثابت حرف *src, صحيح len, حرف *dst);
#جيڪڏهن_پورو

/* dirblock.c */
خارجي errcode_t ext2fs_read_dir_بلاڪ(ext2_filsys fs, blk_t بلاڪ,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاڪ,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاڪ,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاڪ,
					فراغ *buf, صحيح flags, ext2_ino_t ino);
خارجي errcode_t ext2fs_write_dir_بلاڪ(ext2_filsys fs, blk_t بلاڪ,
					فراغ *buf);
خارجي errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاڪ,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاڪ,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاڪ,
					 فراغ *buf, صحيح flags, ext2_ino_t ino);

/* dirhash.c */
خارجي errcode_t ext2fs_dirhash(صحيح نسخو, ثابت حرف *name, صحيح len,
				ثابت __u32 *ٻج,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

خارجي errcode_t ext2fs_dirhash2(صحيح نسخو, ثابت حرف *name, صحيح len,
				 ثابت بنية ext2fs_nls_جدول *charset,
				 صحيح hash_flags,
				 ثابت __u32 *ٻج,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
خارجي errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    بنية ext2_dir_داخلا *dirent,
				    بنانشان صحيح *rec_len);
خارجي errcode_t ext2fs_سيٽ_rec_len(ext2_filsys fs,
				    بنانشان صحيح len,
				    بنية ext2_dir_داخلا *dirent);
خارجي errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاڪ_buf,
			      صحيح (*func)(بنية ext2_dir_داخلا *dirent,
					  صحيح	وٿي,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_معلومات),
			      فراغ *priv_معلومات);
خارجي errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاڪ_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح	داخلا,
					  بنية ext2_dir_داخلا *dirent,
					  صحيح	وٿي,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_معلومات),
			      فراغ *priv_معلومات);

/* dupfs.c */
خارجي errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
خارجي errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
خارجي __u32 ext2fs_ext_attr_hash_داخلا(بنية ext2_ext_attr_داخلا *داخلا,
					فراغ *معلومات);
خارجي __u32 ext2fs_ext_attr_hash_داخلا_signed(بنية ext2_ext_attr_داخلا *داخلا,
					       فراغ *معلومات);
خارجي errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     بنية ext2_ext_attr_داخلا *داخلا,
					     فراغ *معلومات, __u32 *hash);
خارجي errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     بنية ext2_ext_attr_داخلا *داخلا,
					     فراغ *معلومات, __u32 *hash,
					     __u32 *signed_hash);
خارجي errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاڪ, فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاڪ,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاڪ,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاڪ,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاڪ,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاڪ,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   حرف *بلاڪ_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   حرف *بلاڪ_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   حرف *بلاڪ_buf,
					   صحيح adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, فراغ *معلومات),
				فراغ *معلومات);
errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *ڪنجي,
			   فراغ **قدر, size_t *قدر_len);
errcode_t ext2fs_xattr_سيٽ(بنية ext2_xattr_handle *handle,
			   ثابت حرف *ڪنجي,
			   ثابت فراغ *قدر,
			   size_t قدر_len);
errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *ڪنجي);
errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_ڳڻپ(بنية ext2_xattr_handle *handle, size_t *ڳڻپ);
errcode_t ext2fs_xattr_inode_max_سائيز(ext2_filsys fs, ext2_ino_t ino,
				      size_t *سائيز);
#وصف_ڪريو XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بنانشان صحيح *new_flags, بنانشان صحيح *old_flags);
خارجي فراغ ext2fs_ext_attr_بلاڪ_rehash(بنية ext2_ext_attr_سرو *سرو,
					 بنية ext2_ext_attr_داخلا *پڇاڙي);
خارجي __u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode);
خارجي فراغ ext2fs_سيٽ_ea_inode_hash(بنية ext2_inode *inode, __u32 hash);
خارجي __u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode);
خارجي فراغ ext2fs_سيٽ_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_ڳڻپ);

/* extent.c */
خارجي errcode_t ext2fs_extent_سرو_verify(فراغ *ptr, صحيح سائيز);
خارجي errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
خارجي errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
خارجي فراغ ext2fs_extent_free(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   صحيح flags, بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_ڳنڍ_ورهايو(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, صحيح flags,
				       بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_وجهو(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_سيٽ_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					صحيح flags);
خارجي errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags);
خارجي errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					بنية ext2_extent_info *info);
خارجي errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
خارجي errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     صحيح پن_level, blk64_t blk);
خارجي errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
خارجي size_t ext2fs_max_extent_اونهائي(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      بنية ext2_inode *inode);
خارجي errcode_t ext2fs_ڳڻپ_بلاڪون(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode, blk64_t *ret_ڳڻپ);
خارجي errcode_t ext2fs_decode_extent(بنية ext2fs_extent *منزل, فراغ *from,
				      صحيح len);

/* fallocate.c */
#وصف_ڪريو EXT2_FALLOCATE_ZERO_بلاڪون	(0x1)
#وصف_ڪريو EXT2_FALLOCATE_FORCE_INIT	(0x2)
#وصف_ڪريو EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#وصف_ڪريو EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#وصف_ڪريو EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
			   بنية ext2_inode *inode, blk64_t goal,
			   blk64_t شروعات, blk64_t len);

/* fileio.c */
خارجي errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode,
				   صحيح flags, ext2_file_t *ret);
خارجي errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  صحيح flags, ext2_file_t *ret);
خارجي ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
بنية ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
خارجي ext2_ino_t ext2fs_file_get_inode_انگ(ext2_file_t file);
خارجي errcode_t ext2fs_file_close(ext2_file_t file);
خارجي errcode_t ext2fs_file_flush(ext2_file_t file);
خارجي errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
				  بنانشان صحيح wanted, بنانشان صحيح *got);
خارجي errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
				   بنانشان صحيح nbytes, بنانشان صحيح *written);
خارجي errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 وٿي,
				   صحيح whence, __u64 *ret_pos);
خارجي errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t وٿي,
				   صحيح whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_سائيز);
خارجي ext2_off_t ext2fs_file_get_سائيز(ext2_file_t file);
خارجي errcode_t ext2fs_file_سيٽ_سائيز(ext2_file_t file, ext2_off_t سائيز);
خارجي errcode_t ext2fs_file_سيٽ_size2(ext2_file_t file, ext2_off64_t سائيز);

/* finddev.c */
خارجي حرف *ext2fs_ڳوليو_بلاڪ_device(dev_t device);

/* flushb.c */
خارجي errcode_t ext2fs_sync_device(صحيح fd, صحيح flushb);

/* freefs.c */
خارجي فراغ ext2fs_free(ext2_filsys fs);
خارجي فراغ ext2fs_free_dblist(ext2_dblist dblist);
خارجي فراغ ext2fs_badblocks_list_free(ext2_badblocks_list bb);
خارجي فراغ ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
خارجي فراغ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
خارجي errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 شروعات, __u32 پڇاڙي,
					    __u32 real_پڇاڙي,
					    ثابت حرف *descr, حرف *init_map,
					    ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_allocate_generic_bitmap(__u32 شروعات,
						__u32 پڇاڙي,
						__u32 real_پڇاڙي,
						ثابت حرف *descr,
						ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
خارجي فراغ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_fudge_generic_bitmap_پڇاڙي(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t پڇاڙي,
						 ext2_ino_t *oend);
خارجي فراغ ext2fs_سيٽ_generic_bitmap_padding(ext2fs_generic_bitmap map);
خارجي errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_پڇاڙي,
					      __u32 new_real_پڇاڙي,
					      ext2fs_generic_bitmap bmap);
خارجي errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
خارجي errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 شروعات, __u32 انگ,
						 فراغ *ٻاهريون);
خارجي errcode_t ext2fs_سيٽ_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 شروعات, __u32 انگ,
						 فراغ *اندريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 شروعات, __u32 پڇاڙي,
						       __u32 *ٻاهريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_سيٽ_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 شروعات, __u32 پڇاڙي,
						       __u32 *ٻاهريون);

/* gen_bitmap64.c */
فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 شروعات, __u64 پڇاڙي,
				    __u64 real_پڇاڙي,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_پڇاڙي(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 پڇاڙي, __u64 *oend);
فراغ ext2fs_سيٽ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_پڇاڙي,
				     __u64 new_real_پڇاڙي);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون);
errcode_t ext2fs_سيٽ_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 شروعات, بنانشان صحيح انگ,
					فراغ *اندريون);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاڪ_bitmap *bitmap);
errcode_t ext2fs_ڳڻپ_استعمال_ٿيل_clusters(ext2_filsys fs, blk64_t شروعات,
				     blk64_t پڇاڙي, blk64_t *ٻاهريون);
errcode_t ext2fs_ڳڻپ_استعمال_ٿيل_بلاڪون(ext2_filsys fs, blk64_t شروعات,
				   blk64_t پڇاڙي, blk64_t *ٻاهريون);
خارجي بنانشان صحيح ext2fs_list_backups(ext2_filsys fs, بنانشان صحيح *three,
				بنانشان صحيح *five, بنانشان صحيح *seven);

/* getenv.c */
خارجي حرف *ext2fs_محفوظ_getenv(ثابت حرف *arg);

/* get_num_dirs.c */
خارجي errcode_t ext2fs_get_انگ_dirs(ext2_filsys fs, ext2_ino_t *ret_انگ_dirs);

/* getsize.c */
خارجي errcode_t ext2fs_get_device_سائيز(ثابت حرف *file, صحيح blocksize,
					blk_t *retblocks);
خارجي errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
خارجي صحيح ext2fs_get_dio_alignment(صحيح fd);
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_جوڙيو_بلاڪون(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t انگ_بلاڪون);
errcode_t ext2fs_iblk_sub_بلاڪون(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t انگ_بلاڪون);
errcode_t ext2fs_iblk_سيٽ(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b);

/* imager.c */
خارجي errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحيح fd, صحيح flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_بلاڪ(ext2_filsys fs, blk_t blk, فراغ *buf);
errcode_t ext2fs_write_ind_بلاڪ(ext2_filsys fs, blk_t blk, فراغ *buf);

/* initialize.c */
خارجي errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
				   بنية ext2_super_بلاڪ *param,
				   io_manager manager, ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحيح super_only);

/* icount.c */
خارجي فراغ ext2fs_free_icount(ext2_icount_t icount);
خارجي errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, حرف *tdb_dir,
					  صحيح flags, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags,
				       بنانشان صحيح سائيز,
				       ext2_icount_t hint, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
				      بنانشان صحيح سائيز,
				      ext2_icount_t *ret);
خارجي errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
خارجي errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 ڳڻپ);
خارجي ext2_ino_t ext2fs_get_icount_سائيز(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

خارجي errcode_t ext2fs_get_memalign(بنانشان ڊگهو سائيز,
				     بنانشان ڊگهو align, فراغ *ptr);

/* inline_data.c */
خارجي errcode_t ext2fs_inline_معلومات_init(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_معلومات_سائيز(ext2_filsys fs, ext2_ino_t ino,
					 size_t *سائيز);
خارجي errcode_t ext2fs_inline_معلومات_get(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t *سائيز);
خارجي errcode_t ext2fs_inline_معلومات_سيٽ(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t سائيز);

/* inode.c */
خارجي errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   بنانشان صحيح cache_سائيز);
خارجي فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache);
خارجي errcode_t ext2fs_flush_icache(ext2_filsys fs);
خارجي errcode_t ext2fs_get_ايندڙ_inode_ڀريل(ext2_inode_جاچ جاچ,
					    ext2_ino_t *ino,
					    بنية ext2_inode *inode,
					    صحيح bufsize);
#وصف_ڪريو EXT2_INODE_جاچ_DEFAULT_بفر_بلاڪون	8
خارجي errcode_t ext2fs_open_inode_جاچ(ext2_filsys fs, صحيح بفر_بلاڪون,
				  ext2_inode_جاچ *ret_جاچ);
خارجي فراغ ext2fs_close_inode_جاچ(ext2_inode_جاچ جاچ);
خارجي errcode_t ext2fs_get_ايندڙ_inode(ext2_inode_جاچ جاچ, ext2_ino_t *ino,
			       بنية ext2_inode *inode);
خارجي errcode_t ext2fs_inode_جاچ_goto_blockgroup(ext2_inode_جاچ جاچ,
						   صحيح	گروهه);
خارجي فراغ ext2fs_سيٽ_inode_callback
	(ext2_inode_جاچ جاچ,
	 errcode_t (*پورو_گروهه)(ext2_filsys fs,
				 ext2_inode_جاچ جاچ,
				 dgrp_t گروهه,
				 فراغ * priv_معلومات),
	 فراغ *پورو_گروهه_معلومات);
خارجي صحيح ext2fs_inode_جاچ_flags(ext2_inode_جاچ جاچ, صحيح سيٽ_flags,
				   صحيح clear_flags);
خارجي errcode_t ext2fs_read_inode_ڀريل(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode * inode,
					صحيح bufsize);
خارجي errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    بنية ext2_inode * inode,
				    صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_inode_ڀريل(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode * inode,
					 صحيح bufsize);
خارجي errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode * inode,
				     صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_get_بلاڪون(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاڪون);
خارجي errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
خارجي io_manager inode_io_manager;
خارجي errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					حرف **name);
خارجي errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode *inode,
					 حرف **name);

/* ismounted.c */
خارجي errcode_t ext2fs_check_if_mounted(ثابت حرف *file, صحيح *mount_flags);
خارجي errcode_t ext2fs_check_mount_نقطو(ثابت حرف *device, صحيح *mount_flags,
					  حرف *mtpt, صحيح mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
خارجي errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *بلاڪ_buf, blk64_t شروعات,
			      blk64_t پڇاڙي);

/* namei.c */
خارجي errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			 صحيح namelen, حرف *buf, ext2_ino_t *inode);
خارجي errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t پاڙ, ext2_ino_t cwd,
			ثابت حرف *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t پاڙ, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode);
خارجي errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t پاڙ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
صحيح ext2fs_native_flag(فراغ);

/* newdir.c */
خارجي errcode_t ext2fs_new_dir_بلاڪ(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t والد_ino, حرف **بلاڪ);
خارجي errcode_t ext2fs_new_dir_inline_معلومات(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t والد_ino, __u32 *iblock);

/* nls_utf8.c */
خارجي ثابت بنية ext2fs_nls_جدول *ext2fs_ڀريو_nls_جدول(صحيح encoding);
خارجي صحيح ext2fs_check_encoded_name(ثابت بنية ext2fs_nls_جدول *جدول,
				     حرف *s, size_t len, حرف **pos);
خارجي صحيح ext2fs_casefold_cmp(ثابت بنية ext2fs_nls_جدول *جدول,
			       ثابت بنانشان حرف *str1, size_t len1,
			       ثابت بنانشان حرف *str2, size_t len2);

/* mkdir.c */
خارجي errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t inum,
			      ثابت حرف *name);
خارجي errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t والد,
			       ext2_ino_t ino, بنانشان ڊگهو flags,
			       صحيح link_flags, ثابت حرف *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
بنية ext2fs_journal_params {
	blk_t انگ_journal_بلاڪون;
	blk_t انگ_fc_بلاڪون;
};
خارجي errcode_t ext2fs_get_journal_params(
		بنية ext2fs_journal_params *params, ext2_filsys fs);
خارجي errcode_t ext2fs_zero_بلاڪون(ext2_filsys fs, blk_t blk, صحيح انگ,
				    blk_t *ret_blk, صحيح *ret_ڳڻپ);
خارجي errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح انگ,
				     blk64_t *ret_blk, صحيح *ret_ڳڻپ);
خارجي errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 انگ_بلاڪون, صحيح flags,
						  حرف  **ret_jsb);
خارجي errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  بنية ext2fs_journal_params *params,
						  صحيح flags, حرف  **ret_jsb);
خارجي errcode_t ext2fs_جوڙيو_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
خارجي errcode_t ext2fs_جوڙيو_journal_inode(ext2_filsys fs, blk_t انگ_بلاڪون,
					  صحيح flags);
خارجي errcode_t ext2fs_جوڙيو_journal_inode2(ext2_filsys fs, blk_t انگ_بلاڪون,
					   blk64_t goal, صحيح flags);
خارجي errcode_t ext2fs_جوڙيو_journal_inode3(ext2_filsys fs,
				    بنية ext2fs_journal_params *params,
				    blk64_t goal, صحيح flags);
خارجي صحيح ext2fs_default_journal_سائيز(__u64 انگ_بلاڪون);
خارجي صحيح ext2fs_journal_sb_شروعات(صحيح blocksize);

/* openfs.c */
خارجي errcode_t ext2fs_open(ثابت حرف *name, صحيح flags, صحيح superblock,
			     بنانشان صحيح بلاڪ_سائيز, io_manager manager,
			     ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_open2(ثابت حرف *name, ثابت حرف *io_options,
			      صحيح flags, صحيح superblock,
			      بنانشان صحيح بلاڪ_سائيز, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
خارجي blk64_t ext2fs_descriptor_بلاڪ_loc2(ext2_filsys fs,
					blk64_t گروهه_بلاڪ, dgrp_t i);
خارجي blk_t ext2fs_descriptor_بلاڪ_loc(ext2_filsys fs, blk_t گروهه_بلاڪ,
					 dgrp_t i);
errcode_t ext2fs_get_معلومات_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_سيٽ_معلومات_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_منزل_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
خارجي errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t انگ_بلاڪون);
خارجي errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
خارجي e2_blkcnt_t ext2fs_default_orphan_file_بلاڪون(ext2_filsys fs);
خارجي __u32 ext2fs_do_orphan_file_بلاڪ_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      حرف *buf);
خارجي errcode_t ext2fs_orphan_file_بلاڪ_csum_سيٽ(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   حرف *buf);
خارجي صحيح ext2fs_orphan_file_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, حرف *buf);

/* get_pathname.c */
خارجي errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       حرف **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
		      ext2_ino_t ino, صحيح flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			ext2_ino_t ino, صحيح flags);
صحيح ext2fs_dir_is_dx(ext2_filsys fs, ثابت بنية ext2_inode *inode);
فراغ ext2fs_inc_nlink(ext2_filsys fs, بنية ext2_inode *inode);
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode);
صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode);
صحيح ext2fs_dir_link_خالي_جايون(بنية ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *هدف);
صحيح ext2fs_is_fast_symlink(بنية ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_شروعات(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحيح immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
بنانشان ext2fs_mmp_new_seq(فراغ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, فراغ **ptr);

/* read_bb.c */
خارجي errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
خارجي errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      فراغ *priv_معلومات,
				      فراغ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      حرف *badstr,
						      فراغ *priv_معلومات));
خارجي errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     فراغ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
خارجي errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
خارجي errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح انگ_ڌاڳا);
خارجي errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
خارجي errcode_t ext2fs_read_بلاڪ_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_بلاڪ_bitmap (ext2_filsys fs);

/*sha256.c */
#وصف_ڪريو EXT2_FS_SHA256_ڊيگهائي 32
#إذا 0
خارجي فراغ ext2fs_sha256(ثابت بنانشان حرف *اندريون, بنانشان ڊگهو اندريون_سائيز,
		   بنانشان حرف ٻاهريون[EXT2_FS_SHA256_ڊيگهائي]);
#جيڪڏهن_پورو

/* sha512.c */
#وصف_ڪريو EXT2_FS_SHA512_ڊيگهائي 64
خارجي فراغ ext2fs_sha512(ثابت بنانشان حرف *اندريون, بنانشان ڊگهو اندريون_سائيز,
			  بنانشان حرف ٻاهريون[EXT2_FS_SHA512_ڊيگهائي]);

/* swapfs.c */
خارجي errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf, size_t سائيز,
					صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_اندريون(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf, size_t سائيز,
					 صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_ٻاهريون(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي فراغ ext2fs_مٽا_سٽا_ext_attr(حرف *منزل, حرف *from, صحيح bufsize,
				 صحيح has_سرو);
خارجي فراغ ext2fs_مٽا_سٽا_ext_attr_سرو(بنية ext2_ext_attr_سرو *منزل_سرو,
					بنية ext2_ext_attr_سرو *from_hdr);
خارجي فراغ ext2fs_مٽا_سٽا_ext_attr_داخلا(بنية ext2_ext_attr_داخلا *منزل_داخلا,
				       بنية ext2_ext_attr_داخلا *from_داخلا);
خارجي فراغ ext2fs_مٽا_سٽا_super(بنية ext2_super_بلاڪ * super);
خارجي فراغ ext2fs_مٽا_سٽا_گروهه_desc(بنية ext2_گروهه_desc *gdp);
خارجي فراغ ext2fs_مٽا_سٽا_گروهه_desc2(ext2_filsys, بنية ext2_گروهه_desc *gdp);
خارجي فراغ ext2fs_مٽا_سٽا_inode_ڀريل(ext2_filsys fs, بنية ext2_inode_large *t,
				   بنية ext2_inode_large *f, صحيح hostorder,
				   صحيح bufsize);
خارجي فراغ ext2fs_مٽا_سٽا_inode(ext2_filsys fs,بنية ext2_inode *t,
			      بنية ext2_inode *f, صحيح hostorder);
خارجي فراغ ext2fs_مٽا_سٽا_mmp(بنية mmp_struct *mmp);

/* unix_io.c */
خارجي صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t گهڻائي);
خارجي صحيح ext2fs_stat(ثابت حرف *رستو, ext2fs_struct_stat *buf);
خارجي صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
خارجي صحيح ext2fs_inode_has_valid_بلاڪون(بنية ext2_inode *inode);
خارجي صحيح ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  بنية ext2_inode *inode);

/* version.c */
خارجي صحيح ext2fs_parse_نسخو_string(ثابت حرف *ver_string);
خارجي صحيح ext2fs_get_library_نسخو(ثابت حرف **ver_string,
				      ثابت حرف **date_string);

/* write_bb_file.c */
خارجي errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      بنانشان صحيح flags,
				      FILE *f);


/* inline functions */
#جيڪڏهن_وصف_ٿيل NO_INLINE_FUNCS
خارجي errcode_t ext2fs_get_mem(بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي errcode_t ext2fs_get_memzero(بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي errcode_t ext2fs_get_array(بنانشان ڊگهو ڳڻپ,
				  بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي errcode_t ext2fs_get_arrayzero(بنانشان ڊگهو ڳڻپ,
				      بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي errcode_t ext2fs_free_mem(فراغ *ptr);
خارجي errcode_t ext2fs_resize_mem(بنانشان ڊگهو old_سائيز,
				   بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي errcode_t ext2fs_resize_array(بنانشان ڊگهو old_ڳڻپ, بنانشان ڊگهو ڳڻپ,
				     بنانشان ڊگهو سائيز, فراغ *ptr);
خارجي فراغ ext2fs_mark_super_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_بدليو(ext2_filsys fs);
خارجي صحيح ext2fs_test_بدليو(ext2_filsys fs);
خارجي فراغ ext2fs_mark_valid(ext2_filsys fs);
خارجي فراغ ext2fs_unmark_valid(ext2_filsys fs);
خارجي صحيح ext2fs_test_valid(ext2_filsys fs);
خارجي فراغ ext2fs_mark_ib_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_bb_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_ib_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_bb_dirty(ext2_filsys fs);
خارجي dgrp_t ext2fs_گروهه_جو_blk(ext2_filsys fs, blk_t blk);
خارجي dgrp_t ext2fs_گروهه_جو_ino(ext2_filsys fs, ext2_ino_t ino);
خارجي blk_t ext2fs_گروهه_پهريون_بلاڪ(ext2_filsys fs, dgrp_t گروهه);
خارجي blk_t ext2fs_گروهه_آخري_بلاڪ(ext2_filsys fs, dgrp_t گروهه);
خارجي blk_t ext2fs_inode_معلومات_بلاڪون(ext2_filsys fs,
				      بنية ext2_inode *inode);
خارجي صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح بلاڪون);
خارجي صحيح ext2fs_log2_u32(__u32 arg);
خارجي صحيح ext2fs_log2_u64(__u64 arg);
خارجي صحيح ext2fs_log10_u32(__u32 arg);
خارجي صحيح ext2fs_log10_u64(__u64 arg);
خارجي بنانشان صحيح ext2fs_div_ceil(بنانشان صحيح a, بنانشان صحيح b);
خارجي __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
خارجي صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_داخلا *داخلا);
خارجي فراغ ext2fs_dirent_سيٽ_name_len(بنية ext2_dir_داخلا *داخلا, صحيح len);
خارجي صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_داخلا *داخلا);
خارجي فراغ ext2fs_dirent_سيٽ_file_type(بنية ext2_dir_داخلا *داخلا, صحيح type);
خارجي بنية ext2_inode *ext2fs_inode(بنية ext2_inode_large * large_inode);
خارجي ثابت بنية ext2_inode *ext2fs_const_inode(ثابت بنية ext2_inode_large * large_inode);
خارجي صحيح ext2fs_inodes_في_orphan_بلاڪ(ext2_filsys fs);
خارجي بنية ext4_orphan_بلاڪ_پڇ *ext2fs_orphan_بلاڪ_پڇ(ext2_filsys fs,
							       حرف *buf);
#جيڪڏهن_پورو

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#جيڪڏهن_وصف_ٿيل INCLUDE_INLINE_FUNCS
#وصف_ڪريو _INLINE_ خارجي
#وإلا
#إذا (__STDC_VERSION__ >= 199901L)
#وصف_ڪريو _INLINE_ مضمن
#وإلا
#جيڪڏهن_وصف_ٿيل __GNUC__
#وصف_ڪريو _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#وصف_ڪريو _INLINE_ خارجي مضمن
#جيڪڏهن_پورو /* __GNUC__ */
#جيڪڏهن_پورو /* __STDC_VERSION__ >= 199901L */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل EXT2_CUSTOM_يادگيري_ROUTINES
#ضمن <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(بنانشان ڊگهو سائيز, فراغ *ptr)
{
	فراغ *pp;

	pp = يادگيري_ڏيو(سائيز);
	إذا (!pp)
		أرجع EXT2_ET_NO_يادگيري;
	يادگيري_نقل(ptr, &pp, حجم (pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(بنانشان ڊگهو سائيز, فراغ *ptr)
{
	فراغ *pp;

	pp = يادگيري_ڏيو(سائيز);
	إذا (!pp)
		أرجع EXT2_ET_NO_يادگيري;
	memset(pp, 0, سائيز);
	يادگيري_نقل(ptr, &pp, حجم(pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_array(بنانشان ڊگهو ڳڻپ, بنانشان ڊگهو سائيز,
				    فراغ *ptr)
{
	إذا (ڳڻپ && (~0UL)/ڳڻپ < سائيز)
		أرجع EXT2_ET_NO_يادگيري;
	أرجع ext2fs_get_mem(ڳڻپ*سائيز, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(بنانشان ڊگهو ڳڻپ,
					بنانشان ڊگهو سائيز, فراغ *ptr)
{
	إذا (ڳڻپ && (~0UL)/ڳڻپ < سائيز)
		أرجع EXT2_ET_NO_يادگيري;

	أرجع ext2fs_get_memzero((size_t)ڳڻپ * سائيز, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(فراغ *ptr)
{
	فراغ *p;

	يادگيري_نقل(&p, ptr, حجم(p));
	حرر(p);
	p = 0;
	يادگيري_نقل(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(بنانشان ڊگهو EXT2FS_ATTR((اڻ_استعمال_ٿيل)) old_سائيز,
				     بنانشان ڊگهو سائيز, فراغ *ptr)
{
	فراغ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	يادگيري_نقل(&p, ptr, حجم(p));
	p = realloc(p, سائيز);
	إذا (!p)
		أرجع EXT2_ET_NO_يادگيري;
	يادگيري_نقل(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(بنانشان ڊگهو سائيز,
				       بنانشان ڊگهو old_ڳڻپ,
				       بنانشان ڊگهو ڳڻپ, فراغ *ptr)
{
	بنانشان ڊگهو old_سائيز;
	errcode_t retval;

	إذا (ڳڻپ && (~0UL)/ڳڻپ < سائيز)
		أرجع EXT2_ET_NO_يادگيري;

	سائيز *= ڳڻپ;
	old_سائيز = سائيز * old_ڳڻپ;
	retval = ext2fs_resize_mem(old_سائيز, سائيز, ptr);
	إذا (retval)
		أرجع retval;

	إذا (سائيز > old_سائيز) {
		فراغ *p;

		يادگيري_نقل(&p, ptr, حجم(p));
		memset((حرف *)p + old_سائيز, 0, سائيز - old_سائيز);
		يادگيري_نقل(ptr, &p, حجم(p));
	}

	أرجع 0;
}
#جيڪڏهن_پورو	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ فراغ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_بدليو;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ فراغ ext2fs_mark_بدليو(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_بدليو;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ صحيح ext2fs_test_بدليو(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_بدليو);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ فراغ ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ فراغ ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ صحيح ext2fs_test_valid(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ فراغ ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_بدليو;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ فراغ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_بدليو;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ صحيح ext2fs_test_ib_dirty(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ صحيح ext2fs_test_bb_dirty(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_گروهه_جو_blk(ext2_filsys fs, blk_t blk)
{
	أرجع ext2fs_گروهه_جو_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_گروهه_جو_ino(ext2_filsys fs, ext2_ino_t ino)
{
	أرجع (ino - 1) / fs->super->s_inodes_في_گروهه;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروهه_پهريون_بلاڪ(ext2_filsys fs, dgrp_t گروهه)
{
	أرجع (blk_t) ext2fs_گروهه_پهريون_block2(fs, گروهه);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروهه_آخري_بلاڪ(ext2_filsys fs, dgrp_t گروهه)
{
	أرجع (blk_t) ext2fs_گروهه_آخري_block2(fs, گروهه);
}

_INLINE_ blk_t ext2fs_inode_معلومات_بلاڪون(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (blk_t) ext2fs_inode_معلومات_blocks2(fs, inode);
}

_INLINE_ صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح بلاڪون)
{
	صحيح csum_سائيز = 0;

	إذا ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_سائيز = حجم(بنية ext2_dx_پڇ);
	أرجع بلاڪون * ((fs->blocksize - (8 + csum_سائيز)) /
						حجم(بنية ext2_dx_داخلا));
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحيح ext2fs_log2_u32(__u32 arg)
{
	صحيح l = 0;

	arg >>= 1;
	طالما (arg) {
		l++;
		arg >>= 1;
	}
	أرجع l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحيح ext2fs_log2_u64(__u64 arg)
{
	صحيح l = 0;

	arg >>= 1;
	طالما (arg) {
		l++;
		arg >>= 1;
	}
	أرجع l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحيح ext2fs_log10_u32(__u32 arg)
{
	صحيح l = 0;

	arg /= 10;
	طالما (arg) {
		l++;
		arg /= 10;
	}
	أرجع l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحيح ext2fs_log10_u64(__u64 arg)
{
	صحيح l = 0;

	arg /= 10;
	طالما (arg) {
		l++;
		arg /= 10;
	}
	أرجع l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ بنانشان صحيح ext2fs_div_ceil(بنانشان صحيح a, بنانشان صحيح b)
{
	إذا (!a)
		أرجع 0;
	أرجع ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	إذا (!a)
		أرجع 0;
	أرجع ((a - 1) / b) + 1;
}

_INLINE_ صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_داخلا *داخلا)
{
	أرجع داخلا->name_len & 0xff;
}

_INLINE_ فراغ ext2fs_dirent_سيٽ_name_len(بنية ext2_dir_داخلا *داخلا, صحيح len)
{
	داخلا->name_len = (داخلا->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_داخلا *داخلا)
{
	أرجع داخلا->name_len >> 8;
}

_INLINE_ فراغ ext2fs_dirent_سيٽ_file_type(بنية ext2_dir_داخلا *داخلا, صحيح type)
{
	داخلا->name_len = (داخلا->name_len & 0xff) | (type << 8);
}

_INLINE_ بنية ext2_inode *ext2fs_inode(بنية ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	أرجع (بنية ext2_inode *) large_inode;
}

_INLINE_ ثابت بنية ext2_inode *
ext2fs_const_inode(ثابت بنية ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	أرجع (ثابت بنية ext2_inode *) large_inode;
}

_INLINE_ صحيح ext2fs_inodes_في_orphan_بلاڪ(ext2_filsys fs)
{
	أرجع (fs->blocksize - حجم(بنية ext4_orphan_بلاڪ_پڇ)) /
		حجم(__u32);
}

_INLINE_ بنية ext4_orphan_بلاڪ_پڇ *
ext2fs_orphan_بلاڪ_پڇ(ext2_filsys fs, حرف *buf)
{
	أرجع (بنية ext4_orphan_بلاڪ_پڇ *)(buf + fs->blocksize -
		حجم(بنية ext4_orphan_بلاڪ_پڇ));
}

#وصف_رد _INLINE_
#جيڪڏهن_پورو

/* htree levels for ext4 */
#وصف_ڪريو EXT4_HTREE_LEVEL_COMPAT 2
#وصف_ڪريو EXT4_HTREE_LEVEL	3

ساكن مضمن بنانشان صحيح ext2_dir_htree_level(ext2_filsys fs)
{
	إذا (ext2fs_has_feature_largedir(fs->super))
		أرجع EXT4_HTREE_LEVEL;

	أرجع EXT4_HTREE_LEVEL_COMPAT;
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
ساكن مضمن فراغ ext2fs_سيٽ_dtime(ext2_filsys fs, بنية ext2_inode *inode)
{
	__u32	t;

	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_وقت))
		t = fs->now & 0xFFFFFFFF;
	وإلا
		t = time(عدم) & 0xFFFFFFFF;
	إذا (t < fs->super->s_inodes_ڳڻپ)
		t = fs->super->s_inodes_ڳڻپ;
	inode->i_dtime = t;
}

#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* _EXT2FS_EXT2FS_H */
