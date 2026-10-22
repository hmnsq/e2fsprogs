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

#جے_بنا_تعریف _EXT2FS_EXT2FS_H
#تعریف_کرو _EXT2FS_EXT2FS_H

#جے_تعریف_شدہ __GNUC__
#تعریف_کرو EXT2FS_ATTR(x) __attribute__(x)
#وإلا
#تعریف_کرو EXT2FS_ATTR(x)
#جے_انت

#جے_بنا_تعریف __nonstring
#جے_تعریف_شدہ __has_attribute
#إذا __has_attribute(__nonstring__)
#تعریف_کرو __nonstring                    __attribute__((__nonstring__))
#وإلا
#تعریف_کرو __nonstring
#جے_انت /* __has_attribute(__nonstring__) */
#وإلا
# define __nonstring
#جے_انت /* __has_attribute */
#جے_انت /* __nonstring */

#جے_تعریف_شدہ CONFIG_TDB
#تعریف_کرو EXT2_FS_NO_TDB_بن_ورتیا
#وإلا
#تعریف_کرو EXT2_FS_NO_TDB_بن_ورتیا	EXT2FS_ATTR((بن_ورتیا))
#جے_انت

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#إذا (!defined(__GNUC__) && !defined(__WATCOMC__))
#تعریف_کرو NO_INLINE_FUNCS
#جے_انت

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#تعریف_کرو SUPERBLOCK_وتھ	1024
#تعریف_کرو SUPERBLOCK_ناپ		1024

#تعریف_کرو UUID_STR_ناپ 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#تعریف_کرو EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#جے_تعریف_شدہ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

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
#جے_انت /* EXT2_FLAT_INCLUDES */

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
#جے_انت

#ضمن "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#جے_تعریف_شدہ _MSC_VER
#تعریف_کرو EXT2_QSORT_TYPE صحيح __cdecl
#وإلا
#تعریف_کرو EXT2_QSORT_TYPE صحيح
#جے_انت

تعريفنوع بنية struct_ext2_filsys *ext2_filsys;

#تعریف_کرو EXT2FS_MARK_ERROR 	0
#تعریف_کرو EXT2FS_UNMARK_ERROR 	1
#تعریف_کرو EXT2FS_TEST_ERROR	2

بنية ext2fs_struct_generic_bitmap_ادھار {
	errcode_t		magic;
	ext2_filsys 		fs;
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_ادھار *ext2fs_generic_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_ادھار *ext2fs_inode_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_ادھار *ext2fs_بلاک_bitmap;

#تعریف_کرو EXT2_پہلا_INODE(s)	EXT2_پہلا_INO(s)


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

#تعریف_کرو BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
بنية ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
بنية ext2_db_اندراج {
	ext2_ino_t	ino;
	blk_t	blk;
	صحيح	blockcnt;
};

تعريفنوع بنية ext2_struct_dblist *ext2_dblist;

#تعریف_کرو DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#تعریف_کرو EXT2_FILE_WRITE		0x0001
#تعریف_کرو EXT2_FILE_CREATE	0x0002

#تعریف_کرو EXT2_FILE_نقاب		0x00FF

#تعریف_کرو EXT2_FILE_BUF_DIRTY	0x4000
#تعریف_کرو EXT2_FILE_BUF_VALID	0x2000

تعريفنوع بنية ext2_file *ext2_file_t;

#تعریف_کرو EXT2_SEEK_سیٹ	0
#تعریف_کرو EXT2_SEEK_CUR	1
#تعریف_کرو EXT2_SEEK_سرا	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#تعریف_کرو EXT2_FLAG_RW			0x01
#تعریف_کرو EXT2_FLAG_بدلیا		0x02
#تعریف_کرو EXT2_FLAG_DIRTY			0x04
#تعریف_کرو EXT2_FLAG_VALID			0x08
#تعریف_کرو EXT2_FLAG_IB_DIRTY		0x10
#تعریف_کرو EXT2_FLAG_BB_DIRTY		0x20
#تعریف_کرو EXT2_FLAG_وٹاندرا_بائٹاں		0x40
#تعریف_کرو EXT2_FLAG_وٹاندرا_بائٹاں_READ	0x80
#تعریف_کرو EXT2_FLAG_وٹاندرا_بائٹاں_WRITE	0x100
#تعریف_کرو EXT2_FLAG_MASTER_SB_ONLY	0x200
#تعریف_کرو EXT2_FLAG_FORCE			0x400
#تعریف_کرو EXT2_FLAG_SUPER_ONLY		0x800
#تعریف_کرو EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#تعریف_کرو EXT2_FLAG_IMAGE_FILE		0x2000
#تعریف_کرو EXT2_FLAG_EXCLUSIVE		0x4000
#تعریف_کرو EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#تعریف_کرو EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#تعریف_کرو EXT2_FLAG_64_بٹ		0x20000
#تعریف_کرو EXT2_FLAG_PRINT_ترقی	0x40000
#تعریف_کرو EXT2_FLAG_سدھا_IO		0x80000
#تعریف_کرو EXT2_FLAG_SKIP_MMP		0x100000
#تعریف_کرو EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#تعریف_کرو EXT2_FLAG_SHARE_DUP		0x400000
#تعریف_کرو EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#تعریف_کرو EXT2_FLAG_BBITMAP_پوچھ_PROBLEM	0x1000000
#تعریف_کرو EXT2_FLAG_IBITMAP_پوچھ_PROBLEM	0x2000000
#تعریف_کرو EXT2_FLAG_دھاگے		0x4000000
#تعریف_کرو EXT2_FLAG_IGNORE_وٹاندرا_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#تعریف_کرو EXT2_FLAG2_USE_FAKE_ویلا	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#تعریف_کرو EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#تعریف_کرو EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#تعریف_کرو EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#تعریف_کرو EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#تعریف_کرو EXT2_JOURNAL_منزل_FC_BLKS_تناسب		64

بنية blk_alloc_ctx;
بنية opaque_ext2_گروہ_desc;

بنية struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	صحيح				flags;
	حرف *				device_name;
	بنية ext2_super_بلاک	* 	super;
	بنانشانی صحيح			blocksize;
	صحيح				fragsize;
	dgrp_t				گروہ_desc_گنتی;
	بنانشانی لما			desc_بلاکاں;
	بنية opaque_ext2_گروہ_desc *	گروہ_desc;
	بنانشانی صحيح			inode_بلاکاں_فی_گروہ;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_بلاک_bitmap		بلاک_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_بلاکاں)(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکاں);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	بنية ext2_super_بلاک *	orig_super;
	بنية ext2_image_hdr *		image_سرخی;
	__u32				umask;
	time_t				now;
	صحيح				cluster_تناسب_بٹ;
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
	فراغ *				priv_انکڑے;

	/*
	 * Inode cache
	 */
	بنية ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_بلاک)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, بنية blk_alloc_ctx *ctx);
	فراغ (*بلاک_alloc_stats)(ext2_filsys fs, blk64_t blk, صحيح inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	فراغ *mmp_buf;
	فراغ *mmp_cmp;
	صحيح mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	لما mmp_آخری_written;

	/* progress operation functions */
	بنية ext2fs_ترقی_ops *ترقی_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_بیج;

	io_channel			journal_io;
	حرف				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	فراغ (*بلاک_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t عدد,
					صحيح inuse);

	/* hashmap for SHA of data blocks */
	بنية ext2fs_hashmap* بلاک_sha_map;

	ثابت بنية ext2fs_nls_جدول *encoding;
};

#إذا EXT2_FLAT_INCLUDES
#ضمن "e2_bitops.h"
#وإلا
#ضمن <ext2fs/bitops.h>
#جے_انت

/*
 * 64-bit bitmap backend types
 */
#تعریف_کرو EXT2FS_BMAP64_BITARRAY	1
#تعریف_کرو EXT2FS_BMAP64_RBTREE	2
#تعریف_کرو EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#تعریف_کرو بلاک_بدلیا			1
#تعریف_کرو بلاک_ABORT			2
#تعریف_کرو بلاک_ERROR			4
#تعریف_کرو بلاک_INLINE_انکڑے_بدلیا	8

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
#تعریف_کرو بلاک_FLAG_APPEND	1
#تعریف_کرو بلاک_FLAG_HOLE		1
#تعریف_کرو بلاک_FLAG_ڈونگھائی_TRAVERSE	2
#تعریف_کرو بلاک_FLAG_انکڑے_ONLY	4
#تعریف_کرو بلاک_FLAG_READ_ONLY	8

#تعریف_کرو بلاک_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#تعریف_کرو بلاک_گنتی_IND		(-1)
#تعریف_کرو بلاک_گنتی_DIND	(-2)
#تعریف_کرو بلاک_گنتی_TIND	(-3)
#تعریف_کرو بلاک_گنتی_TRANSLATOR	(-4)

#تعریف_کرو بلاک_ALLOC_UNKNOWN	0
#تعریف_کرو بلاک_ALLOC_انکڑے	1
#تعریف_کرو بلاک_ALLOC_METADATA	2

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
#تعریف_کرو EXT2_BMOVE_GET_DBLIST	0x0001
#تعریف_کرو EXT2_BMOVE_DEBUG	0x0002
#جے_انت

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#تعریف_کرو EXT2_EXTENT_FLAGS_پتی		0x0001
#تعریف_کرو EXT2_EXTENT_FLAGS_UNINIT	0x0002
#تعریف_کرو EXT2_EXTENT_FLAGS_دوجا_ویکھو	0x0004

بنية ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

تعريفنوع بنية ext2_extent_handle *ext2_extent_handle_t;
تعريفنوع بنية ext2_extent_رستہ *ext2_extent_رستہ_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#تعریف_کرو EXT2_EXTENT_CURRENT	0x0000
#تعریف_کرو EXT2_EXTENT_MOVE_نقاب	0x000F
#تعریف_کرو EXT2_EXTENT_جڑ	0x0001
#تعریف_کرو EXT2_EXTENT_آخری_پتی	0x0002
#تعریف_کرو EXT2_EXTENT_پہلا_SIB	0x0003
#تعریف_کرو EXT2_EXTENT_آخری_SIB	0x0004
#تعریف_کرو EXT2_EXTENT_اگلا_SIB	0x0005
#تعریف_کرو EXT2_EXTENT_PREV_SIB	0x0006
#تعریف_کرو EXT2_EXTENT_اگلا_پتی	0x0007
#تعریف_کرو EXT2_EXTENT_PREV_پتی	0x0008
#تعریف_کرو EXT2_EXTENT_اگلا	0x0009
#تعریف_کرو EXT2_EXTENT_PREV	0x000A
#تعریف_کرو EXT2_EXTENT_اتے		0x000B
#تعریف_کرو EXT2_EXTENT_تھلے	0x000C
#تعریف_کرو EXT2_EXTENT_تھلے_AND_آخری 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#تعریف_کرو EXT2_EXTENT_داخل_کرو_AFTER	0x0001 /* insert after handle loc'n */
#تعریف_کرو EXT2_EXTENT_داخل_کرو_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#تعریف_کرو EXT2_EXTENT_DELETE_KEEP_خالی_تھاواں	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#تعریف_کرو EXT2_EXTENT_سیٹ_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
بنية ext2_extent_info {
	صحيح		curr_اندراج;
	صحيح		curr_level;
	صحيح		عدد_entries;
	صحيح		max_entries;
	صحيح		max_ڈونگھائی;
	صحيح		بائٹاں_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#تعریف_کرو EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#تعریف_کرو DIRENT_بدلیا	1
#تعریف_کرو DIRENT_ABORT	2
#تعریف_کرو DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#تعریف_کرو DIRENT_FLAG_INCLUDE_خالی_تھاواں	1
#تعریف_کرو DIRENT_FLAG_INCLUDE_REMOVED	2
#تعریف_کرو DIRENT_FLAG_INCLUDE_CSUM	4
#تعریف_کرو DIRENT_FLAG_INCLUDE_INLINE_انکڑے 8

#تعریف_کرو DIRENT_DOT_FILE		1
#تعریف_کرو DIRENT_DOT_DOT_FILE	2
#تعریف_کرو DIRENT_OTHER_FILE	3
#تعریف_کرو DIRENT_DELETED_FILE	4
#تعریف_کرو DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
تعريفنوع بنية ext2_struct_inode_جانچ *ext2_inode_جانچ;

/*
 * ext2fs_scan flags
 */
#تعریف_کرو EXT2_SF_CHK_BADBLOCKS	0x0001
#تعریف_کرو EXT2_SF_BAD_INODE_BLK	0x0002
#تعریف_کرو EXT2_SF_BAD_EXTRA_بائٹاں	0x0004
#تعریف_کرو EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#تعریف_کرو EXT2_SF_DO_LAZY		0x0010
#تعریف_کرو EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#تعریف_کرو EXT2_MF_MOUNTED		1
#تعریف_کرو EXT2_MF_ISROOT		2
#تعریف_کرو EXT2_MF_READONLY	4
#تعریف_کرو EXT2_MF_وٹاندرا		8
#تعریف_کرو EXT2_MF_BUSY		16
#تعریف_کرو EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#تعریف_کرو LINUX_S_IFMT  00170000
#تعریف_کرو LINUX_S_IFSOCK 0140000
#تعریف_کرو LINUX_S_IFLNK	 0120000
#تعریف_کرو LINUX_S_IFREG  0100000
#تعریف_کرو LINUX_S_IFBLK  0060000
#تعریف_کرو LINUX_S_IFDIR  0040000
#تعریف_کرو LINUX_S_IFCHR  0020000
#تعریف_کرو LINUX_S_IFIFO  0010000
#تعریف_کرو LINUX_S_ISUID  0004000
#تعریف_کرو LINUX_S_ISGID  0002000
#تعریف_کرو LINUX_S_ISVTX  0001000

#تعریف_کرو LINUX_S_IRWXU 00700
#تعریف_کرو LINUX_S_IRUSR 00400
#تعریف_کرو LINUX_S_IWUSR 00200
#تعریف_کرو LINUX_S_IXUSR 00100

#تعریف_کرو LINUX_S_IRWXG 00070
#تعریف_کرو LINUX_S_IRGRP 00040
#تعریف_کرو LINUX_S_IWGRP 00020
#تعریف_کرو LINUX_S_IXGRP 00010

#تعریف_کرو LINUX_S_IRWXO 00007
#تعریف_کرو LINUX_S_IROTH 00004
#تعریف_کرو LINUX_S_IWOTH 00002
#تعریف_کرو LINUX_S_IXOTH 00001

#تعریف_کرو LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#تعریف_کرو LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#تعریف_کرو LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#تعریف_کرو LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#تعریف_کرو LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#تعریف_کرو LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#تعریف_کرو LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#تعریف_کرو EXT2_I_ناپ(i)	((i)->i_ناپ | ((__u64) (i)->i_ناپ_بلند << 32))

ساكن مضمن __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((بن_ورتیا)),
					__u32 nsec)
{
	__u32 extra = 0;

#إذا (SIZEOF_ویلا_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_نقاب;
#جے_انت
	أرجع extra | (nsec << EXT4_EPOCH_بٹ);
}
ساكن مضمن time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((بن_ورتیا)))
{
#إذا (SIZEOF_ویلا_T > 4)
	إذا (extra & EXT4_EPOCH_نقاب)
		seconds += ((time_t)(extra & EXT4_EPOCH_نقاب) << 32);
#جے_انت
	أرجع seconds;
}
ساكن مضمن __u32 __decode_extra_nsec(__u32 extra)
{
	أرجع (extra & EXT4_NSEC_نقاب) >> EXT4_EPOCH_بٹ;
}
#تعریف_کرو ext2fs_inode_actual_ناپ(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_ناپ +				      \
		  (حجم(*inode) > EXT2_GOOD_OLD_INODE_ناپ ?		      \
		   ((بنية ext2_inode_large *)(inode))->i_extra_isize : 0)))
#تعریف_کرو clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#تعریف_کرو ext2fs_inode_xtime_سیٹ(inode, میدان, sec)			      \
افعل {									      \
	إذا (ext2fs_inode_includes(ext2fs_inode_actual_ناپ(inode),	      \
				  میدان ## _extra)) {			      \
		(inode)->میدان = (__u32)(sec & 0xfffffff);		      \
		((بنية ext2_inode_large *)(inode))->میدان ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} وإلا {							      \
		(inode)->میدان = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} طالما (0)
#تعریف_کرو ext2fs_inode_xtime_get(inode, میدان)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_ناپ(inode), میدان ## _extra) ?    \
	__decode_extra_sec((inode)->میدان,				      \
		((بنية ext2_inode_large *)(inode))->میدان ## _extra) :      \
		(time_t)(inode)->میدان)

ساكن مضمن فراغ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((بن_ورتیا)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#إذا (SIZEOF_ویلا_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_نقاب;
#وإلا
	*hi = 0;
#جے_انت
}
ساكن مضمن time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((بن_ورتیا)))
{
#إذا (SIZEOF_ویلا_T == 4)
	أرجع *lo;
#وإلا
	أرجع ((time_t)(*hi) << 32) | *lo;
#جے_انت
}
#تعریف_کرو ext2fs_سیٹ_tstamp(sb, میدان, seconds) \
	__sb_set_tstamp(&(sb)->میدان, &(sb)->میدان ## _hi, seconds)
#تعریف_کرو ext2fs_get_tstamp(sb, میدان) \
	__sb_get_tstamp(&(sb)->میدان, &(sb)->میدان ## _hi)

/*
 * ext2_icount_t abstraction
 */
#تعریف_کرو EXT2_ICOUNT_OPT_INCREMENT	0x01
#تعریف_کرو EXT2_ICOUNT_OPT_FULLMAP		0x02

تعريفنوع بنية ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#تعریف_کرو BMAP_ALLOC	0x0001
#تعریف_کرو BMAP_سیٹ	0x0002
#تعریف_کرو BMAP_UNINIT	0x0004
#تعریف_کرو BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#تعریف_کرو BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#تعریف_کرو READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#تعریف_کرو WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#تعریف_کرو IMAGER_FLAG_INODEMAP	1
#تعریف_کرو IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

/*
 * Features supported by this version of the library
 */
#تعریف_کرو EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_اشاریہ|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#جے_تعریف_شدہ CONFIG_MMP
#تعریف_کرو EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#وإلا
#تعریف_کرو EXT4_LIB_INCOMPAT_MMP		(0)
#جے_انت

#تعریف_کرو EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_ثنائی_ہندسہ|\
					 EXT4_FEATURE_INCOMPAT_INLINE_انکڑے|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_بیج|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#تعریف_کرو EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_بلاکاں |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#تعریف_کرو EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#تعریف_کرو EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#تعریف_کرو EXT2_FS_CLUSTER_تناسب(fs)	(1 << (fs)->cluster_تناسب_بٹ)
#تعریف_کرو EXT2_FS_CLUSTER_نقاب(fs)		(EXT2_FS_CLUSTER_تناسب(fs) - 1)
#تعریف_کرو EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_تناسب_بٹ)
/* Translate a cluster number to a block number */
#تعریف_کرو EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_تناسب_بٹ)
/* Translate # of blks to # of clusters */
#تعریف_کرو EXT2_FS_عدد_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_نقاب(fs)) >> \
					 (fs)->cluster_تناسب_بٹ)

#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
تعريفنوع بنية stat64 ext2fs_struct_stat;
#وإلا
تعريفنوع بنية stat ext2fs_struct_stat;
#جے_انت

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#تعریف_کرو EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
بنية ext2_xattr_handle;
#تعریف_کرو XATTR_ABORT	1
#تعریف_کرو XATTR_بدلیا	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#تعریف_کرو EXT2FS_BITMAPS_WRITE		0x0001
#تعریف_کرو EXT2_FS_BITMAPS_بلاک		0x0002
#تعریف_کرو EXT2FS_BITMAPS_INODE		0x0004
#تعریف_کرو EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#تعریف_کرو EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#تعریف_کرو EXT2_FS_LINK_FT_نقاب	0x0007
#تعریف_کرو EXT2FS_LINK_APPEND	0x0010
#تعریف_کرو EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
ساكن مضمن صحيح ext2fs_has_گروہ_desc_csum(ext2_filsys fs)
{
	أرجع ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ساكن مضمن صحيح ext2fs_needs_large_file_feature(بنانشانی لما لما file_ناپ)
{
	أرجع file_ناپ >= 0x80000000ULL;
}

/* alloc.c */
خارجي فراغ ext2fs_clear_بلاک_uninit(ext2_filsys fs, dgrp_t گروہ);
خارجي errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, صحيح منوال,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
خارجي errcode_t ext2fs_new_بلاک(ext2_filsys fs, blk_t goal,
				  ext2fs_بلاک_bitmap map, blk_t *ret);
خارجي errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاک_bitmap map, blk64_t *ret);
خارجي errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاک_bitmap map, blk64_t *ret,
				   بنية blk_alloc_ctx *ctx);
خارجي errcode_t ext2fs_get_free_بلاکاں(ext2_filsys fs, blk_t شروع,
					blk_t انت, صحيح عدد,
					ext2fs_بلاک_bitmap map,
					blk_t *ret);
خارجي errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t شروع,
					 blk64_t انت, صحيح عدد,
					 ext2fs_بلاک_bitmap map,
					 blk64_t *ret);
خارجي errcode_t ext2fs_alloc_بلاک(ext2_filsys fs, blk_t goal,
				    حرف *بلاک_buf, blk_t *ret);
خارجي errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     حرف *بلاک_buf, blk64_t *ret);
خارجي errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     حرف *بلاک_buf, blk64_t *ret,
				     بنية blk_alloc_ctx *ctx);

خارجي فراغ ext2fs_سیٹ_alloc_بلاک_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_لبھو_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, blk64_t lblk);
خارجي فراغ ext2fs_سیٹ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
خارجي فراغ ext2fs_سیٹ_بلاک_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse));
#تعریف_کرو EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#تعریف_کرو EXT2_NEWRANGE_MIN_لمائی	(0x2)
#تعریف_کرو EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			   blk64_t len, ext2fs_بلاک_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#تعریف_کرو EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#تعریف_کرو EXT2_ALLOCRANGE_ZERO_بلاکاں	(0x2)
#تعریف_کرو EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
خارجي صحيح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t گروہ,
					ext2fs_بلاک_bitmap bmap);
خارجي errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t گروہ,
					       ext2fs_بلاک_bitmap bmap,
					       blk_t *desc_بلاکاں);
خارجي فراغ ext2fs_سیٹ_بلاک_alloc_stats_callback(ext2_filsys fs,
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
فراغ ext2fs_بلاک_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse);
فراغ ext2fs_بلاک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse);
فراغ ext2fs_بلاک_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse);

/* alloc_tables.c */
خارجي errcode_t ext2fs_allocate_tables(ext2_filsys fs);
خارجي errcode_t ext2fs_allocate_گروہ_جدول(ext2_filsys fs, dgrp_t گروہ,
					     ext2fs_بلاک_bitmap bmap);

/* badblocks.c */
خارجي errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح ناپ);
خارجي errcode_t ext2fs_u32_list_جوڑو(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_لبھو(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
خارجي errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
خارجي صحيح ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
خارجي فراغ ext2fs_u32_list_iterate_سرا(ext2_u32_iterate iter);
خارجي errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
خارجي صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

خارجي errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    صحيح ناپ);
خارجي errcode_t ext2fs_badblocks_list_جوڑو(ext2_badblocks_list bb,
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
خارجي فراغ ext2fs_badblocks_list_iterate_سرا(ext2_badblocks_iterate iter);
خارجي errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
خارجي صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
خارجي صحيح ext2fs_u32_list_گنتی(ext2_u32_list bb);

/* bb_compat */
خارجي errcode_t badblocks_list_create(badblocks_list *ret, صحيح ناپ);
خارجي errcode_t badblocks_list_جوڑو(badblocks_list bb, blk_t blk);
خارجي صحيح badblocks_list_test(badblocks_list bb, blk_t blk);
خارجي errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
خارجي صحيح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
خارجي فراغ badblocks_list_iterate_سرا(badblocks_iterate iter);
خارجي فراغ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
خارجي errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
خارجي فراغ ext2fs_free_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap);
خارجي فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
خارجي errcode_t ext2fs_allocate_بلاک_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_بلاک_bitmap *ret);
خارجي errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ثابت حرف *descr,
						   ext2fs_بلاک_bitmap *ret);
خارجي صحيح ext2fs_get_bitmap_granularity(ext2fs_بلاک_bitmap bitmap);
خارجي errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_inode_bitmap *ret);
خارجي errcode_t ext2fs_fudge_inode_bitmap_سرا(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t سرا, ext2_ino_t *oend);
خارجي errcode_t ext2fs_fudge_بلاک_bitmap_سرا(ext2fs_بلاک_bitmap bitmap,
					       blk_t سرا, blk_t *oend);
خارجي errcode_t ext2fs_fudge_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t سرا, blk64_t *oend);
خارجي فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي فراغ ext2fs_clear_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap);
خارجي errcode_t ext2fs_resize_inode_bitmap(__u32 new_سرا, __u32 new_real_سرا,
					    ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_inode_bitmap2(__u64 new_سرا,
					     __u64 new_real_سرا,
					     ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_بلاک_bitmap(__u32 new_سرا, __u32 new_real_سرا,
					    ext2fs_بلاک_bitmap bmap);
خارجي errcode_t ext2fs_resize_بلاک_bitmap2(__u64 new_سرا,
					     __u64 new_real_سرا,
					     ext2fs_بلاک_bitmap bmap);
خارجي errcode_t ext2fs_compare_بلاک_bitmap(ext2fs_بلاک_bitmap bm1,
					     ext2fs_بلاک_bitmap bm2);
خارجي errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
خارجي errcode_t ext2fs_سیٹ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بنانشانی صحيح عدد,
					فراغ *وچ);
خارجي errcode_t ext2fs_سیٹ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t عدد,
					 فراغ *وچ);
خارجي errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بنانشانی صحيح عدد,
					فراغ *باہرلا);
خارجي errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t عدد,
					 فراغ *باہرلا);
خارجي errcode_t ext2fs_سیٹ_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t شروع, بنانشانی صحيح عدد,
					فراغ *وچ);
خارجي errcode_t ext2fs_سیٹ_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t شروع, size_t عدد,
					 فراغ *وچ);
خارجي errcode_t ext2fs_get_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t شروع, بنانشانی صحيح عدد,
					فراغ *باہرلا);
خارجي errcode_t ext2fs_get_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t شروع, size_t عدد,
					 فراغ *باہرلا);

/* blknum.c */
خارجي __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجي __u32 ext2fs_بلاک_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجي dgrp_t ext2fs_گروہ_دا_blk2(ext2_filsys fs, blk64_t);
خارجي blk64_t ext2fs_گروہ_پہلا_block2(ext2_filsys fs, dgrp_t گروہ);
خارجي blk64_t ext2fs_گروہ_آخری_block2(ext2_filsys fs, dgrp_t گروہ);
خارجي صحيح ext2fs_گروہ_بلاکاں_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجي blk64_t ext2fs_inode_انکڑے_blocks2(ext2_filsys fs,
					 بنية ext2_inode *inode);
خارجي blk64_t ext2fs_inode_i_بلاکاں(ext2_filsys fs,
				     بنية ext2_inode *inode);
خارجي blk64_t ext2fs_get_stat_i_بلاکاں(ext2_filsys fs,
					بنية ext2_inode *inode);
خارجي blk64_t ext2fs_بلاکاں_گنتی(بنية ext2_super_بلاک *super);
خارجي فراغ ext2fs_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super,
				    blk64_t blk);
خارجي فراغ ext2fs_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super,
				    blk64_t blk);
خارجي blk64_t ext2fs_r_بلاکاں_گنتی(بنية ext2_super_بلاک *super);
خارجي فراغ ext2fs_r_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super,
				      blk64_t blk);
خارجي فراغ ext2fs_r_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super,
				      blk64_t blk);
خارجي blk64_t ext2fs_free_بلاکاں_گنتی(بنية ext2_super_بلاک *super);
خارجي فراغ ext2fs_free_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super,
					 blk64_t blk);
خارجي فراغ ext2fs_free_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
خارجي بنية ext2_گروہ_desc *ext2fs_گروہ_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ);
خارجي blk64_t ext2fs_بلاک_bitmap_csum(ext2_filsys fs, dgrp_t گروہ);
خارجي blk64_t ext2fs_بلاک_bitmap_loc(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_بلاک_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					blk64_t blk);
خارجي __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t گروہ);
خارجي blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_inode_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					blk64_t blk);
خارجي blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_inode_جدول_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       blk64_t blk);
خارجي __u32 ext2fs_bg_free_بلاکاں_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_free_بلاکاں_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					 __u32 n);
خارجي __u32 ext2fs_bg_free_inodes_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_free_inodes_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					 __u32 n);
خارجي __u32 ext2fs_bg_ورتیا_dirs_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_ورتیا_dirs_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       __u32 n);
خارجي __u32 ext2fs_bg_itable_بن_ورتیا(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_itable_بن_ورتیا_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				     __u32 n);
خارجي __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروہ);
خارجي صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flag);
خارجي فراغ ext2fs_bg_flags_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags);
خارجي فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags);
خارجي __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجي فراغ ext2fs_bg_checksum_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 checksum);
خارجي blk64_t ext2fs_file_acl_بلاک(ext2_filsys fs,
				     ثابت بنية ext2_inode *inode);
خارجي فراغ ext2fs_file_acl_بلاک_سیٹ(ext2_filsys fs,
				      بنية ext2_inode *inode, blk64_t blk);
خارجي errcode_t ext2fs_inode_ناپ_سیٹ(ext2_filsys fs, بنية ext2_inode *inode,
				       ext2_off64_t ناپ);

/* block.c */
خارجي errcode_t ext2fs_بلاک_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      صحيح	flags,
				      حرف *بلاک_buf,
				      صحيح (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  صحيح	blockcnt,
						  فراغ	*priv_انکڑے),
				      فراغ *priv_انکڑے);
errcode_t ext2fs_بلاک_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				صحيح	flags,
				حرف *بلاک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_وتھ,
					    فراغ	*priv_انکڑے),
				فراغ *priv_انکڑے);
errcode_t ext2fs_بلاک_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_وتھ,
					    فراغ	*priv_انکڑے),
				فراغ *priv_انکڑے);

/* bmap.c */
خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *بلاک_buf, صحيح bmap_flags,
			     blk_t بلاک, blk_t *phys_blk);
خارجي errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *بلاک_buf, صحيح bmap_flags, blk64_t بلاک,
			      صحيح *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_بلاک(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#إذا 0
/* bmove.c */
خارجي errcode_t ext2fs_move_بلاکاں(ext2_filsys fs,
				    ext2fs_بلاک_bitmap reserve,
				    ext2fs_بلاک_bitmap alloc_map,
				    صحيح flags);
#جے_انت

/* check_desc.c */
خارجي errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
خارجي errcode_t ext2fs_close(ext2_filsys fs);
خارجي errcode_t ext2fs_close2(ext2_filsys fs, صحيح flags);
خارجي errcode_t ext2fs_close_free(ext2_filsys *fs);
خارجي errcode_t ext2fs_flush(ext2_filsys fs);
خارجي errcode_t ext2fs_flush2(ext2_filsys fs, صحيح flags);
خارجي صحيح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروہ_بلاک);
خارجي errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t گروہ,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_ورتیا_blks);
خارجي صحيح ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t گروہ,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    صحيح *ret_meta_bg);
خارجي فراغ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
خارجي __u32 ext2fs_چکری_جانچ_32_be(__u32 چکری_جانچ, بنانشانی حرف ثابت *p, size_t len);
خارجي __u32 ext2fs_crc32c_le(__u32 چکری_جانچ, بنانشانی حرف ثابت *p, size_t len);

/* csum.c */
خارجي فراغ ext2fs_init_csum_بیج(ext2_filsys fs);
خارجي errcode_t ext2fs_mmp_csum_سیٹ(ext2_filsys fs, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_mmp_csum_verify(ext2_filsys, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلاک *sb);
خارجي errcode_t ext2fs_superblock_csum_سیٹ(ext2_filsys fs,
					    بنية ext2_super_بلاک *sb);
خارجي صحيح ext2fs_superblock_csum_verify(ext2_filsys fs,
					 بنية ext2_super_بلاک *sb);
خارجي errcode_t ext2fs_ext_attr_بلاک_csum_سیٹ(ext2_filsys fs,
					ext2_ino_t inum, blk64_t بلاک,
					بنية ext2_ext_attr_سرخی *hdr);
خارجي صحيح ext2fs_ext_attr_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t بلاک,
					     بنية ext2_ext_attr_سرخی *hdr);
#تعریف_کرو EXT2_DIRENT_پوچھ(بلاک, blocksize) \
	((بنية ext2_dir_اندراج_پوچھ *)(((حرف *)(بلاک)) + \
	(blocksize) - حجم(بنية ext2_dir_اندراج_پوچھ)))

خارجي فراغ ext2fs_initialize_dirent_پوچھ(ext2_filsys fs,
					  بنية ext2_dir_اندراج_پوچھ *t);
خارجي صحيح ext2fs_dirent_has_پوچھ(ext2_filsys fs,
				  بنية ext2_dir_اندراج *dirent);
خارجي صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     بنية ext2_dir_اندراج *dirent);
خارجي صحيح ext2fs_dir_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_اندراج *dirent);
خارجي errcode_t ext2fs_dir_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					   بنية ext2_dir_اندراج *dirent);
خارجي errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  بنية ext2_dir_اندراج *dirent,
					  بنية ext2_dx_countlimit **cc,
					  صحيح *وتھ);
خارجي errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				بنية ext2_dir_اندراج *dirent,
				__u32 *چکری_جانچ, بنية ext2_dx_پوچھ **ret_t);
خارجي errcode_t ext2fs_extent_بلاک_csum_سیٹ(ext2_filsys fs,
					      ext2_ino_t inum,
					      بنية ext3_extent_سرخی *eh);
خارجي صحيح ext2fs_extent_بلاک_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   بنية ext3_extent_سرخی *eh);
خارجي errcode_t ext2fs_بلاک_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					      حرف *bitmap, صحيح ناپ);
خارجي صحيح ext2fs_بلاک_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
					   حرف *bitmap, صحيح ناپ);
خارجي errcode_t ext2fs_inode_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					      حرف *bitmap, صحيح ناپ);
خارجي صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
					   حرف *bitmap, صحيح ناپ);
خارجي errcode_t ext2fs_inode_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext2_inode_large *inode);
خارجي صحيح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_inode_large *inode);
خارجي فراغ ext2fs_گروہ_desc_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ);
خارجي صحيح ext2fs_گروہ_desc_csum_verify(ext2_filsys fs, dgrp_t گروہ);
خارجي errcode_t ext2fs_سیٹ_gdt_csum(ext2_filsys fs);
خارجي __u16 ext2fs_گروہ_desc_csum(ext2_filsys fs, dgrp_t گروہ);

/* dblist.c */
خارجي errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
خارجي errcode_t ext2fs_جوڑو_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_جوڑو_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي فراغ ext2fs_dblist_ترتیب_دیو(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							   ثابت فراغ *));
خارجي فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							    ثابت فراغ *));
خارجي errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_اندراج *db_info,
		    فراغ	*priv_انکڑے),
	فراغ *priv_انکڑے);
خارجي errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_انکڑے),
	فراغ *priv_انکڑے);
خارجي errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_انکڑے),
	بنانشانی لما لما شروع,
	بنانشانی لما لما گنتی,
	فراغ *priv_انکڑے);
خارجي errcode_t ext2fs_سیٹ_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_سیٹ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
خارجي صحيح ext2fs_dblist_گنتی(ext2_dblist dblist);
خارجي blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
خارجي errcode_t ext2fs_dblist_get_آخری(ext2_dblist dblist,
					بنية ext2_db_اندراج **اندراج);
خارجي errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					بنية ext2_db_entry2 **اندراج);
خارجي errcode_t ext2fs_dblist_drop_آخری(ext2_dblist dblist);

/* dblist_dir.c */
خارجي errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  صحيح	flags,
				  حرف	*بلاک_buf,
				  صحيح (*func)(ext2_ino_t	dir,
					      صحيح		اندراج,
					      بنية ext2_dir_اندراج *dirent,
					      صحيح	وتھ,
					      صحيح	blocksize,
					      حرف	*buf,
					      فراغ	*priv_انکڑے),
				  فراغ *priv_انکڑے);

#إذا 0
/* digest_encode.c */
#تعریف_کرو EXT2_FS_DIGEST_ناپ EXT2_FS_SHA256_لمائی
خارجي صحيح ext2fs_digest_encode(ثابت حرف *src, صحيح len, حرف *dst);
خارجي صحيح ext2fs_digest_decode(ثابت حرف *src, صحيح len, حرف *dst);
#جے_انت

/* dirblock.c */
خارجي errcode_t ext2fs_read_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاک,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاک,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاک,
					فراغ *buf, صحيح flags, ext2_ino_t ino);
خارجي errcode_t ext2fs_write_dir_بلاک(ext2_filsys fs, blk_t بلاک,
					فراغ *buf);
خارجي errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاک,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاک,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاک,
					 فراغ *buf, صحيح flags, ext2_ino_t ino);

/* dirhash.c */
خارجي errcode_t ext2fs_dirhash(صحيح نسخہ, ثابت حرف *name, صحيح len,
				ثابت __u32 *بیج,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

خارجي errcode_t ext2fs_dirhash2(صحيح نسخہ, ثابت حرف *name, صحيح len,
				 ثابت بنية ext2fs_nls_جدول *charset,
				 صحيح hash_flags,
				 ثابت __u32 *بیج,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
خارجي errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    بنية ext2_dir_اندراج *dirent,
				    بنانشانی صحيح *rec_len);
خارجي errcode_t ext2fs_سیٹ_rec_len(ext2_filsys fs,
				    بنانشانی صحيح len,
				    بنية ext2_dir_اندراج *dirent);
خارجي errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاک_buf,
			      صحيح (*func)(بنية ext2_dir_اندراج *dirent,
					  صحيح	وتھ,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_انکڑے),
			      فراغ *priv_انکڑے);
خارجي errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاک_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح	اندراج,
					  بنية ext2_dir_اندراج *dirent,
					  صحيح	وتھ,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_انکڑے),
			      فراغ *priv_انکڑے);

/* dupfs.c */
خارجي errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
خارجي errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
خارجي __u32 ext2fs_ext_attr_hash_اندراج(بنية ext2_ext_attr_اندراج *اندراج,
					فراغ *انکڑے);
خارجي __u32 ext2fs_ext_attr_hash_اندراج_signed(بنية ext2_ext_attr_اندراج *اندراج,
					       فراغ *انکڑے);
خارجي errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     بنية ext2_ext_attr_اندراج *اندراج,
					     فراغ *انکڑے, __u32 *hash);
خارجي errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     بنية ext2_ext_attr_اندراج *اندراج,
					     فراغ *انکڑے, __u32 *hash,
					     __u32 *signed_hash);
خارجي errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاک, فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاک,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاک,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاک,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاک,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاک,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   حرف *بلاک_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   حرف *بلاک_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   حرف *بلاک_buf,
					   صحيح adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, فراغ *انکڑے),
				فراغ *انکڑے);
errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *کلید,
			   فراغ **قدر, size_t *قدر_len);
errcode_t ext2fs_xattr_سیٹ(بنية ext2_xattr_handle *handle,
			   ثابت حرف *کلید,
			   ثابت فراغ *قدر,
			   size_t قدر_len);
errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *کلید);
errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_گنتی(بنية ext2_xattr_handle *handle, size_t *گنتی);
errcode_t ext2fs_xattr_inode_max_ناپ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ناپ);
#تعریف_کرو XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بنانشانی صحيح *new_flags, بنانشانی صحيح *old_flags);
خارجي فراغ ext2fs_ext_attr_بلاک_rehash(بنية ext2_ext_attr_سرخی *سرخی,
					 بنية ext2_ext_attr_اندراج *سرا);
خارجي __u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode);
خارجي فراغ ext2fs_سیٹ_ea_inode_hash(بنية ext2_inode *inode, __u32 hash);
خارجي __u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode);
خارجي فراغ ext2fs_سیٹ_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_گنتی);

/* extent.c */
خارجي errcode_t ext2fs_extent_سرخی_verify(فراغ *ptr, صحيح ناپ);
خارجي errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
خارجي errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
خارجي فراغ ext2fs_extent_free(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   صحيح flags, بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_گرہ_ونڈو(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, صحيح flags,
				       بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_داخل_کرو(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_سیٹ_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					صحيح flags);
خارجي errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags);
خارجي errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					بنية ext2_extent_info *info);
خارجي errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
خارجي errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     صحيح پتی_level, blk64_t blk);
خارجي errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
خارجي size_t ext2fs_max_extent_ڈونگھائی(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      بنية ext2_inode *inode);
خارجي errcode_t ext2fs_گنتی_بلاکاں(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode, blk64_t *ret_گنتی);
خارجي errcode_t ext2fs_decode_extent(بنية ext2fs_extent *منزل, فراغ *from,
				      صحيح len);

/* fallocate.c */
#تعریف_کرو EXT2_FALLOCATE_ZERO_بلاکاں	(0x1)
#تعریف_کرو EXT2_FALLOCATE_FORCE_INIT	(0x2)
#تعریف_کرو EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#تعریف_کرو EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#تعریف_کرو EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
			   بنية ext2_inode *inode, blk64_t goal,
			   blk64_t شروع, blk64_t len);

/* fileio.c */
خارجي errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode,
				   صحيح flags, ext2_file_t *ret);
خارجي errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  صحيح flags, ext2_file_t *ret);
خارجي ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
بنية ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
خارجي ext2_ino_t ext2fs_file_get_inode_عدد(ext2_file_t file);
خارجي errcode_t ext2fs_file_close(ext2_file_t file);
خارجي errcode_t ext2fs_file_flush(ext2_file_t file);
خارجي errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
				  بنانشانی صحيح wanted, بنانشانی صحيح *got);
خارجي errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
				   بنانشانی صحيح nbytes, بنانشانی صحيح *written);
خارجي errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 وتھ,
				   صحيح whence, __u64 *ret_pos);
خارجي errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t وتھ,
				   صحيح whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ناپ);
خارجي ext2_off_t ext2fs_file_get_ناپ(ext2_file_t file);
خارجي errcode_t ext2fs_file_سیٹ_ناپ(ext2_file_t file, ext2_off_t ناپ);
خارجي errcode_t ext2fs_file_سیٹ_size2(ext2_file_t file, ext2_off64_t ناپ);

/* finddev.c */
خارجي حرف *ext2fs_لبھو_بلاک_device(dev_t device);

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
					    __u32 شروع, __u32 سرا,
					    __u32 real_سرا,
					    ثابت حرف *descr, حرف *init_map,
					    ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_allocate_generic_bitmap(__u32 شروع,
						__u32 سرا,
						__u32 real_سرا,
						ثابت حرف *descr,
						ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
خارجي فراغ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_fudge_generic_bitmap_سرا(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t سرا,
						 ext2_ino_t *oend);
خارجي فراغ ext2fs_سیٹ_generic_bitmap_padding(ext2fs_generic_bitmap map);
خارجي errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_سرا,
					      __u32 new_real_سرا,
					      ext2fs_generic_bitmap bmap);
خارجي errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
خارجي errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 شروع, __u32 عدد,
						 فراغ *باہرلا);
خارجي errcode_t ext2fs_سیٹ_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 شروع, __u32 عدد,
						 فراغ *وچ);
خارجي errcode_t ext2fs_لبھو_پہلا_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 شروع, __u32 سرا,
						       __u32 *باہرلا);
خارجي errcode_t ext2fs_لبھو_پہلا_سیٹ_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 شروع, __u32 سرا,
						       __u32 *باہرلا);

/* gen_bitmap64.c */
فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 شروع, __u64 سرا,
				    __u64 real_سرا,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_سرا(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 سرا, __u64 *oend);
فراغ ext2fs_سیٹ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_سرا,
				     __u64 new_real_سرا);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 شروع, بنانشانی صحيح عدد,
					فراغ *باہرلا);
errcode_t ext2fs_سیٹ_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 شروع, بنانشانی صحيح عدد,
					فراغ *وچ);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاک_bitmap *bitmap);
errcode_t ext2fs_گنتی_ورتیا_clusters(ext2_filsys fs, blk64_t شروع,
				     blk64_t سرا, blk64_t *باہرلا);
errcode_t ext2fs_گنتی_ورتیا_بلاکاں(ext2_filsys fs, blk64_t شروع,
				   blk64_t سرا, blk64_t *باہرلا);
خارجي بنانشانی صحيح ext2fs_list_backups(ext2_filsys fs, بنانشانی صحيح *three,
				بنانشانی صحيح *five, بنانشانی صحيح *seven);

/* getenv.c */
خارجي حرف *ext2fs_محفوظ_getenv(ثابت حرف *arg);

/* get_num_dirs.c */
خارجي errcode_t ext2fs_get_عدد_dirs(ext2_filsys fs, ext2_ino_t *ret_عدد_dirs);

/* getsize.c */
خارجي errcode_t ext2fs_get_device_ناپ(ثابت حرف *file, صحيح blocksize,
					blk_t *retblocks);
خارجي errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
خارجي صحيح ext2fs_get_dio_alignment(صحيح fd);
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_جوڑو_بلاکاں(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکاں);
errcode_t ext2fs_iblk_sub_بلاکاں(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکاں);
errcode_t ext2fs_iblk_سیٹ(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b);

/* imager.c */
خارجي errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحيح fd, صحيح flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_بلاک(ext2_filsys fs, blk_t blk, فراغ *buf);
errcode_t ext2fs_write_ind_بلاک(ext2_filsys fs, blk_t blk, فراغ *buf);

/* initialize.c */
خارجي errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
				   بنية ext2_super_بلاک *param,
				   io_manager manager, ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحيح super_only);

/* icount.c */
خارجي فراغ ext2fs_free_icount(ext2_icount_t icount);
خارجي errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, حرف *tdb_dir,
					  صحيح flags, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags,
				       بنانشانی صحيح ناپ,
				       ext2_icount_t hint, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
				      بنانشانی صحيح ناپ,
				      ext2_icount_t *ret);
خارجي errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
خارجي errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 گنتی);
خارجي ext2_ino_t ext2fs_get_icount_ناپ(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

خارجي errcode_t ext2fs_get_memalign(بنانشانی لما ناپ,
				     بنانشانی لما align, فراغ *ptr);

/* inline_data.c */
خارجي errcode_t ext2fs_inline_انکڑے_init(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_انکڑے_ناپ(ext2_filsys fs, ext2_ino_t ino,
					 size_t *ناپ);
خارجي errcode_t ext2fs_inline_انکڑے_get(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t *ناپ);
خارجي errcode_t ext2fs_inline_انکڑے_سیٹ(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t ناپ);

/* inode.c */
خارجي errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   بنانشانی صحيح cache_ناپ);
خارجي فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache);
خارجي errcode_t ext2fs_flush_icache(ext2_filsys fs);
خارجي errcode_t ext2fs_get_اگلا_inode_بھریا(ext2_inode_جانچ جانچ,
					    ext2_ino_t *ino,
					    بنية ext2_inode *inode,
					    صحيح bufsize);
#تعریف_کرو EXT2_INODE_جانچ_DEFAULT_بفر_بلاکاں	8
خارجي errcode_t ext2fs_open_inode_جانچ(ext2_filsys fs, صحيح بفر_بلاکاں,
				  ext2_inode_جانچ *ret_جانچ);
خارجي فراغ ext2fs_close_inode_جانچ(ext2_inode_جانچ جانچ);
خارجي errcode_t ext2fs_get_اگلا_inode(ext2_inode_جانچ جانچ, ext2_ino_t *ino,
			       بنية ext2_inode *inode);
خارجي errcode_t ext2fs_inode_جانچ_goto_blockgroup(ext2_inode_جانچ جانچ,
						   صحيح	گروہ);
خارجي فراغ ext2fs_سیٹ_inode_callback
	(ext2_inode_جانچ جانچ,
	 errcode_t (*پورا_گروہ)(ext2_filsys fs,
				 ext2_inode_جانچ جانچ,
				 dgrp_t گروہ,
				 فراغ * priv_انکڑے),
	 فراغ *پورا_گروہ_انکڑے);
خارجي صحيح ext2fs_inode_جانچ_flags(ext2_inode_جانچ جانچ, صحيح سیٹ_flags,
				   صحيح clear_flags);
خارجي errcode_t ext2fs_read_inode_بھریا(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode * inode,
					صحيح bufsize);
خارجي errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    بنية ext2_inode * inode,
				    صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_inode_بھریا(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode * inode,
					 صحيح bufsize);
خارجي errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode * inode,
				     صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_get_بلاکاں(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکاں);
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
خارجي errcode_t ext2fs_check_mount_نقطہ(ثابت حرف *device, صحيح *mount_flags,
					  حرف *mtpt, صحيح mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
خارجي errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *بلاک_buf, blk64_t شروع,
			      blk64_t سرا);

/* namei.c */
خارجي errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			 صحيح namelen, حرف *buf, ext2_ino_t *inode);
خارجي errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ثابت حرف *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode);
خارجي errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
صحيح ext2fs_native_flag(فراغ);

/* newdir.c */
خارجي errcode_t ext2fs_new_dir_بلاک(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ماپا_ino, حرف **بلاک);
خارجي errcode_t ext2fs_new_dir_inline_انکڑے(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ماپا_ino, __u32 *iblock);

/* nls_utf8.c */
خارجي ثابت بنية ext2fs_nls_جدول *ext2fs_لدو_nls_جدول(صحيح encoding);
خارجي صحيح ext2fs_check_encoded_name(ثابت بنية ext2fs_nls_جدول *جدول,
				     حرف *s, size_t len, حرف **pos);
خارجي صحيح ext2fs_casefold_cmp(ثابت بنية ext2fs_nls_جدول *جدول,
			       ثابت بنانشانی حرف *str1, size_t len1,
			       ثابت بنانشانی حرف *str2, size_t len2);

/* mkdir.c */
خارجي errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t ماپا, ext2_ino_t inum,
			      ثابت حرف *name);
خارجي errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t ماپا,
			       ext2_ino_t ino, بنانشانی لما flags,
			       صحيح link_flags, ثابت حرف *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
بنية ext2fs_journal_params {
	blk_t عدد_journal_بلاکاں;
	blk_t عدد_fc_بلاکاں;
};
خارجي errcode_t ext2fs_get_journal_params(
		بنية ext2fs_journal_params *params, ext2_filsys fs);
خارجي errcode_t ext2fs_zero_بلاکاں(ext2_filsys fs, blk_t blk, صحيح عدد,
				    blk_t *ret_blk, صحيح *ret_گنتی);
خارجي errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح عدد,
				     blk64_t *ret_blk, صحيح *ret_گنتی);
خارجي errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 عدد_بلاکاں, صحيح flags,
						  حرف  **ret_jsb);
خارجي errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  بنية ext2fs_journal_params *params,
						  صحيح flags, حرف  **ret_jsb);
خارجي errcode_t ext2fs_جوڑو_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
خارجي errcode_t ext2fs_جوڑو_journal_inode(ext2_filsys fs, blk_t عدد_بلاکاں,
					  صحيح flags);
خارجي errcode_t ext2fs_جوڑو_journal_inode2(ext2_filsys fs, blk_t عدد_بلاکاں,
					   blk64_t goal, صحيح flags);
خارجي errcode_t ext2fs_جوڑو_journal_inode3(ext2_filsys fs,
				    بنية ext2fs_journal_params *params,
				    blk64_t goal, صحيح flags);
خارجي صحيح ext2fs_default_journal_ناپ(__u64 عدد_بلاکاں);
خارجي صحيح ext2fs_journal_sb_شروع(صحيح blocksize);

/* openfs.c */
خارجي errcode_t ext2fs_open(ثابت حرف *name, صحيح flags, صحيح superblock,
			     بنانشانی صحيح بلاک_ناپ, io_manager manager,
			     ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_open2(ثابت حرف *name, ثابت حرف *io_options,
			      صحيح flags, صحيح superblock,
			      بنانشانی صحيح بلاک_ناپ, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
خارجي blk64_t ext2fs_descriptor_بلاک_loc2(ext2_filsys fs,
					blk64_t گروہ_بلاک, dgrp_t i);
خارجي blk_t ext2fs_descriptor_بلاک_loc(ext2_filsys fs, blk_t گروہ_بلاک,
					 dgrp_t i);
errcode_t ext2fs_get_انکڑے_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_سیٹ_انکڑے_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_منزل_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
خارجي errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t عدد_بلاکاں);
خارجي errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
خارجي e2_blkcnt_t ext2fs_default_orphan_file_بلاکاں(ext2_filsys fs);
خارجي __u32 ext2fs_do_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      حرف *buf);
خارجي errcode_t ext2fs_orphan_file_بلاک_csum_سیٹ(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   حرف *buf);
خارجي صحيح ext2fs_orphan_file_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
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
صحيح ext2fs_dir_link_خالی_تھاواں(بنية ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ماپا, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *نشانہ);
صحيح ext2fs_is_fast_symlink(بنية ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_شروع(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحيح immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
بنانشانی ext2fs_mmp_new_seq(فراغ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, فراغ **ptr);

/* read_bb.c */
خارجي errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
خارجي errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      فراغ *priv_انکڑے,
				      فراغ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      حرف *badstr,
						      فراغ *priv_انکڑے));
خارجي errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     فراغ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
خارجي errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
خارجي errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح عدد_دھاگے);
خارجي errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
خارجي errcode_t ext2fs_read_بلاک_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_بلاک_bitmap (ext2_filsys fs);

/*sha256.c */
#تعریف_کرو EXT2_FS_SHA256_لمائی 32
#إذا 0
خارجي فراغ ext2fs_sha256(ثابت بنانشانی حرف *وچ, بنانشانی لما وچ_ناپ,
		   بنانشانی حرف باہرلا[EXT2_FS_SHA256_لمائی]);
#جے_انت

/* sha512.c */
#تعریف_کرو EXT2_FS_SHA512_لمائی 64
خارجي فراغ ext2fs_sha512(ثابت بنانشانی حرف *وچ, بنانشانی لما وچ_ناپ,
			  بنانشانی حرف باہرلا[EXT2_FS_SHA512_لمائی]);

/* swapfs.c */
خارجي errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf, size_t ناپ,
					صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_وچ(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf, size_t ناپ,
					 صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_باہرلا(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي فراغ ext2fs_وٹاندرا_ext_attr(حرف *منزل, حرف *from, صحيح bufsize,
				 صحيح has_سرخی);
خارجي فراغ ext2fs_وٹاندرا_ext_attr_سرخی(بنية ext2_ext_attr_سرخی *منزل_سرخی,
					بنية ext2_ext_attr_سرخی *from_hdr);
خارجي فراغ ext2fs_وٹاندرا_ext_attr_اندراج(بنية ext2_ext_attr_اندراج *منزل_اندراج,
				       بنية ext2_ext_attr_اندراج *from_اندراج);
خارجي فراغ ext2fs_وٹاندرا_super(بنية ext2_super_بلاک * super);
خارجي فراغ ext2fs_وٹاندرا_گروہ_desc(بنية ext2_گروہ_desc *gdp);
خارجي فراغ ext2fs_وٹاندرا_گروہ_desc2(ext2_filsys, بنية ext2_گروہ_desc *gdp);
خارجي فراغ ext2fs_وٹاندرا_inode_بھریا(ext2_filsys fs, بنية ext2_inode_large *t,
				   بنية ext2_inode_large *f, صحيح hostorder,
				   صحيح bufsize);
خارجي فراغ ext2fs_وٹاندرا_inode(ext2_filsys fs,بنية ext2_inode *t,
			      بنية ext2_inode *f, صحيح hostorder);
خارجي فراغ ext2fs_وٹاندرا_mmp(بنية mmp_struct *mmp);

/* unix_io.c */
خارجي صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t منوال);
خارجي صحيح ext2fs_stat(ثابت حرف *رستہ, ext2fs_struct_stat *buf);
خارجي صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
خارجي صحيح ext2fs_inode_has_valid_بلاکاں(بنية ext2_inode *inode);
خارجي صحيح ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  بنية ext2_inode *inode);

/* version.c */
خارجي صحيح ext2fs_parse_نسخہ_string(ثابت حرف *ver_string);
خارجي صحيح ext2fs_get_library_نسخہ(ثابت حرف **ver_string,
				      ثابت حرف **date_string);

/* write_bb_file.c */
خارجي errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      بنانشانی صحيح flags,
				      FILE *f);


/* inline functions */
#جے_تعریف_شدہ NO_INLINE_FUNCS
خارجي errcode_t ext2fs_get_mem(بنانشانی لما ناپ, فراغ *ptr);
خارجي errcode_t ext2fs_get_memzero(بنانشانی لما ناپ, فراغ *ptr);
خارجي errcode_t ext2fs_get_array(بنانشانی لما گنتی,
				  بنانشانی لما ناپ, فراغ *ptr);
خارجي errcode_t ext2fs_get_arrayzero(بنانشانی لما گنتی,
				      بنانشانی لما ناپ, فراغ *ptr);
خارجي errcode_t ext2fs_free_mem(فراغ *ptr);
خارجي errcode_t ext2fs_resize_mem(بنانشانی لما old_ناپ,
				   بنانشانی لما ناپ, فراغ *ptr);
خارجي errcode_t ext2fs_resize_array(بنانشانی لما old_گنتی, بنانشانی لما گنتی,
				     بنانشانی لما ناپ, فراغ *ptr);
خارجي فراغ ext2fs_mark_super_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_بدلیا(ext2_filsys fs);
خارجي صحيح ext2fs_test_بدلیا(ext2_filsys fs);
خارجي فراغ ext2fs_mark_valid(ext2_filsys fs);
خارجي فراغ ext2fs_unmark_valid(ext2_filsys fs);
خارجي صحيح ext2fs_test_valid(ext2_filsys fs);
خارجي فراغ ext2fs_mark_ib_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_bb_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_ib_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_bb_dirty(ext2_filsys fs);
خارجي dgrp_t ext2fs_گروہ_دا_blk(ext2_filsys fs, blk_t blk);
خارجي dgrp_t ext2fs_گروہ_دا_ino(ext2_filsys fs, ext2_ino_t ino);
خارجي blk_t ext2fs_گروہ_پہلا_بلاک(ext2_filsys fs, dgrp_t گروہ);
خارجي blk_t ext2fs_گروہ_آخری_بلاک(ext2_filsys fs, dgrp_t گروہ);
خارجي blk_t ext2fs_inode_انکڑے_بلاکاں(ext2_filsys fs,
				      بنية ext2_inode *inode);
خارجي صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح بلاکاں);
خارجي صحيح ext2fs_log2_u32(__u32 arg);
خارجي صحيح ext2fs_log2_u64(__u64 arg);
خارجي صحيح ext2fs_log10_u32(__u32 arg);
خارجي صحيح ext2fs_log10_u64(__u64 arg);
خارجي بنانشانی صحيح ext2fs_div_ceil(بنانشانی صحيح a, بنانشانی صحيح b);
خارجي __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
خارجي صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_اندراج *اندراج);
خارجي فراغ ext2fs_dirent_سیٹ_name_len(بنية ext2_dir_اندراج *اندراج, صحيح len);
خارجي صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_اندراج *اندراج);
خارجي فراغ ext2fs_dirent_سیٹ_file_type(بنية ext2_dir_اندراج *اندراج, صحيح type);
خارجي بنية ext2_inode *ext2fs_inode(بنية ext2_inode_large * large_inode);
خارجي ثابت بنية ext2_inode *ext2fs_const_inode(ثابت بنية ext2_inode_large * large_inode);
خارجي صحيح ext2fs_inodes_فی_orphan_بلاک(ext2_filsys fs);
خارجي بنية ext4_orphan_بلاک_پوچھ *ext2fs_orphan_بلاک_پوچھ(ext2_filsys fs,
							       حرف *buf);
#جے_انت

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#جے_تعریف_شدہ INCLUDE_INLINE_FUNCS
#تعریف_کرو _INLINE_ خارجي
#وإلا
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ مضمن
#وإلا
#جے_تعریف_شدہ __GNUC__
#تعریف_کرو _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#تعریف_کرو _INLINE_ خارجي مضمن
#جے_انت /* __GNUC__ */
#جے_انت /* __STDC_VERSION__ >= 199901L */
#جے_انت

#جے_بنا_تعریف EXT2_CUSTOM_حافظہ_ROUTINES
#ضمن <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(بنانشانی لما ناپ, فراغ *ptr)
{
	فراغ *pp;

	pp = حافظہ_دیو(ناپ);
	إذا (!pp)
		أرجع EXT2_ET_NO_حافظہ;
	حافظہ_نقل(ptr, &pp, حجم (pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(بنانشانی لما ناپ, فراغ *ptr)
{
	فراغ *pp;

	pp = حافظہ_دیو(ناپ);
	إذا (!pp)
		أرجع EXT2_ET_NO_حافظہ;
	memset(pp, 0, ناپ);
	حافظہ_نقل(ptr, &pp, حجم(pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_array(بنانشانی لما گنتی, بنانشانی لما ناپ,
				    فراغ *ptr)
{
	إذا (گنتی && (~0UL)/گنتی < ناپ)
		أرجع EXT2_ET_NO_حافظہ;
	أرجع ext2fs_get_mem(گنتی*ناپ, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(بنانشانی لما گنتی,
					بنانشانی لما ناپ, فراغ *ptr)
{
	إذا (گنتی && (~0UL)/گنتی < ناپ)
		أرجع EXT2_ET_NO_حافظہ;

	أرجع ext2fs_get_memzero((size_t)گنتی * ناپ, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(فراغ *ptr)
{
	فراغ *p;

	حافظہ_نقل(&p, ptr, حجم(p));
	حرر(p);
	p = 0;
	حافظہ_نقل(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(بنانشانی لما EXT2FS_ATTR((بن_ورتیا)) old_ناپ,
				     بنانشانی لما ناپ, فراغ *ptr)
{
	فراغ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	حافظہ_نقل(&p, ptr, حجم(p));
	p = realloc(p, ناپ);
	إذا (!p)
		أرجع EXT2_ET_NO_حافظہ;
	حافظہ_نقل(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(بنانشانی لما ناپ,
				       بنانشانی لما old_گنتی,
				       بنانشانی لما گنتی, فراغ *ptr)
{
	بنانشانی لما old_ناپ;
	errcode_t retval;

	إذا (گنتی && (~0UL)/گنتی < ناپ)
		أرجع EXT2_ET_NO_حافظہ;

	ناپ *= گنتی;
	old_ناپ = ناپ * old_گنتی;
	retval = ext2fs_resize_mem(old_ناپ, ناپ, ptr);
	إذا (retval)
		أرجع retval;

	إذا (ناپ > old_ناپ) {
		فراغ *p;

		حافظہ_نقل(&p, ptr, حجم(p));
		memset((حرف *)p + old_ناپ, 0, ناپ - old_ناپ);
		حافظہ_نقل(ptr, &p, حجم(p));
	}

	أرجع 0;
}
#جے_انت	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ فراغ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_بدلیا;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ فراغ ext2fs_mark_بدلیا(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_بدلیا;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ صحيح ext2fs_test_بدلیا(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_بدلیا);
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
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_بدلیا;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ فراغ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_بدلیا;
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
_INLINE_ dgrp_t ext2fs_گروہ_دا_blk(ext2_filsys fs, blk_t blk)
{
	أرجع ext2fs_گروہ_دا_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_گروہ_دا_ino(ext2_filsys fs, ext2_ino_t ino)
{
	أرجع (ino - 1) / fs->super->s_inodes_فی_گروہ;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروہ_پہلا_بلاک(ext2_filsys fs, dgrp_t گروہ)
{
	أرجع (blk_t) ext2fs_گروہ_پہلا_block2(fs, گروہ);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروہ_آخری_بلاک(ext2_filsys fs, dgrp_t گروہ)
{
	أرجع (blk_t) ext2fs_گروہ_آخری_block2(fs, گروہ);
}

_INLINE_ blk_t ext2fs_inode_انکڑے_بلاکاں(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (blk_t) ext2fs_inode_انکڑے_blocks2(fs, inode);
}

_INLINE_ صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح بلاکاں)
{
	صحيح csum_ناپ = 0;

	إذا ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_ناپ = حجم(بنية ext2_dx_پوچھ);
	أرجع بلاکاں * ((fs->blocksize - (8 + csum_ناپ)) /
						حجم(بنية ext2_dx_اندراج));
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
_INLINE_ بنانشانی صحيح ext2fs_div_ceil(بنانشانی صحيح a, بنانشانی صحيح b)
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

_INLINE_ صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_اندراج *اندراج)
{
	أرجع اندراج->name_len & 0xff;
}

_INLINE_ فراغ ext2fs_dirent_سیٹ_name_len(بنية ext2_dir_اندراج *اندراج, صحيح len)
{
	اندراج->name_len = (اندراج->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_اندراج *اندراج)
{
	أرجع اندراج->name_len >> 8;
}

_INLINE_ فراغ ext2fs_dirent_سیٹ_file_type(بنية ext2_dir_اندراج *اندراج, صحيح type)
{
	اندراج->name_len = (اندراج->name_len & 0xff) | (type << 8);
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

_INLINE_ صحيح ext2fs_inodes_فی_orphan_بلاک(ext2_filsys fs)
{
	أرجع (fs->blocksize - حجم(بنية ext4_orphan_بلاک_پوچھ)) /
		حجم(__u32);
}

_INLINE_ بنية ext4_orphan_بلاک_پوچھ *
ext2fs_orphan_بلاک_پوچھ(ext2_filsys fs, حرف *buf)
{
	أرجع (بنية ext4_orphan_بلاک_پوچھ *)(buf + fs->blocksize -
		حجم(بنية ext4_orphan_بلاک_پوچھ));
}

#تعریف_رد _INLINE_
#جے_انت

/* htree levels for ext4 */
#تعریف_کرو EXT4_HTREE_LEVEL_COMPAT 2
#تعریف_کرو EXT4_HTREE_LEVEL	3

ساكن مضمن بنانشانی صحيح ext2_dir_htree_level(ext2_filsys fs)
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
ساكن مضمن فراغ ext2fs_سیٹ_dtime(ext2_filsys fs, بنية ext2_inode *inode)
{
	__u32	t;

	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_ویلا))
		t = fs->now & 0xFFFFFFFF;
	وإلا
		t = time(عدم) & 0xFFFFFFFF;
	إذا (t < fs->super->s_inodes_گنتی)
		t = fs->super->s_inodes_گنتی;
	inode->i_dtime = t;
}

#جے_تعریف_شدہ __cplusplus
}
#جے_انت

#جے_انت /* _EXT2FS_EXT2FS_H */
