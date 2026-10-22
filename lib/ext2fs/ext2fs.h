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

#اگر_غیر_تعریف_شدہ _EXT2FS_EXT2FS_H
#تعریف_کرو _EXT2FS_EXT2FS_H

#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو EXT2FS_ATTR(x) __attribute__(x)
#ورنہ
#تعریف_کرو EXT2FS_ATTR(x)
#اگر_ختم

#اگر_غیر_تعریف_شدہ __nonstring
#اگر_تعریف_شدہ __has_attribute
#اگر __has_attribute(__nonstring__)
#تعریف_کرو __nonstring                    __attribute__((__nonstring__))
#ورنہ
#تعریف_کرو __nonstring
#اگر_ختم /* __has_attribute(__nonstring__) */
#ورنہ
# define __nonstring
#اگر_ختم /* __has_attribute */
#اگر_ختم /* __nonstring */

#اگر_تعریف_شدہ CONFIG_TDB
#تعریف_کرو EXT2_FS_NO_TDB_غیر_مستعمل
#ورنہ
#تعریف_کرو EXT2_FS_NO_TDB_غیر_مستعمل	EXT2FS_ATTR((غیر_مستعمل))
#اگر_ختم

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#اگر (!defined(__GNUC__) && !defined(__WATCOMC__))
#تعریف_کرو NO_INLINE_FUNCS
#اگر_ختم

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#تعریف_کرو SUPERBLOCK_انحراف	1024
#تعریف_کرو SUPERBLOCK_سائز		1024

#تعریف_کرو UUID_STR_سائز 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#تعریف_کرو EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#اگر_تعریف_شدہ HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <time.h>
#شامل <errno.h>

#اگر EXT2_FLAT_INCLUDES
#شامل "e2_types.h"
#شامل "ext2_fs.h"
#شامل "ext3_extents.h"
#ورنہ
#شامل <ext2fs/ext2_types.h>
#شامل <ext2fs/ext2_fs.h>
#شامل <ext2fs/ext3_extents.h>
#اگر_ختم /* EXT2_FLAT_INCLUDES */

قسمتعریف __u32 __bitwise		ext2_ino_t;
قسمتعریف __u32 __bitwise		blk_t;
قسمتعریف __u64 __bitwise		blk64_t;
قسمتعریف __u32 __bitwise		dgrp_t;
قسمتعریف __s32 __bitwise		ext2_off_t;
قسمتعریف __s64 __bitwise		ext2_off64_t;
قسمتعریف __s64 __bitwise		e2_blkcnt_t;
قسمتعریف __u32 __bitwise		ext2_dirhash_t;

#اگر EXT2_FLAT_INCLUDES
#شامل "com_err.h"
#شامل "ext2_io.h"
#شامل "ext2_err.h"
#شامل "ext2_ext_attr.h"
#ورنہ
#شامل <et/com_err.h>
#شامل <ext2fs/ext2_io.h>
#شامل <ext2fs/ext2_err.h>
#شامل <ext2fs/ext2_ext_attr.h>
#اگر_ختم

#شامل "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#اگر_تعریف_شدہ _MSC_VER
#تعریف_کرو EXT2_QSORT_TYPE صحیح __cdecl
#ورنہ
#تعریف_کرو EXT2_QSORT_TYPE صحیح
#اگر_ختم

قسمتعریف ساخت struct_ext2_filsys *ext2_filsys;

#تعریف_کرو EXT2FS_MARK_ERROR 	0
#تعریف_کرو EXT2FS_UNMARK_ERROR 	1
#تعریف_کرو EXT2FS_TEST_ERROR	2

ساخت ext2fs_struct_generic_bitmap_بنیاد {
	errcode_t		magic;
	ext2_filsys 		fs;
};

قسمتعریف ساخت ext2fs_struct_generic_bitmap_بنیاد *ext2fs_generic_bitmap;
قسمتعریف ساخت ext2fs_struct_generic_bitmap_بنیاد *ext2fs_inode_bitmap;
قسمتعریف ساخت ext2fs_struct_generic_bitmap_بنیاد *ext2fs_بلاک_bitmap;

#تعریف_کرو EXT2_پہلا_INODE(s)	EXT2_پہلا_INO(s)


/*
 * Badblocks list definitions
 */

قسمتعریف ساخت ext2_struct_u32_list *ext2_badblocks_list;
قسمتعریف ساخت ext2_struct_u32_iterate *ext2_badblocks_iterate;

قسمتعریف ساخت ext2_struct_u32_list *ext2_u32_list;
قسمتعریف ساخت ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
قسمتعریف ساخت ext2_struct_u32_list *badblocks_list;
قسمتعریف ساخت ext2_struct_u32_iterate *badblocks_iterate;

#تعریف_کرو BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ساخت ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ساخت ext2_db_اندراج {
	ext2_ino_t	ino;
	blk_t	blk;
	صحیح	blockcnt;
};

قسمتعریف ساخت ext2_struct_dblist *ext2_dblist;

#تعریف_کرو DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#تعریف_کرو EXT2_FILE_WRITE		0x0001
#تعریف_کرو EXT2_FILE_CREATE	0x0002

#تعریف_کرو EXT2_FILE_نقاب		0x00FF

#تعریف_کرو EXT2_FILE_BUF_DIRTY	0x4000
#تعریف_کرو EXT2_FILE_BUF_VALID	0x2000

قسمتعریف ساخت ext2_file *ext2_file_t;

#تعریف_کرو EXT2_SEEK_سیٹ	0
#تعریف_کرو EXT2_SEEK_CUR	1
#تعریف_کرو EXT2_SEEK_انتہا	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#تعریف_کرو EXT2_FLAG_RW			0x01
#تعریف_کرو EXT2_FLAG_بدلا		0x02
#تعریف_کرو EXT2_FLAG_DIRTY			0x04
#تعریف_کرو EXT2_FLAG_VALID			0x08
#تعریف_کرو EXT2_FLAG_IB_DIRTY		0x10
#تعریف_کرو EXT2_FLAG_BB_DIRTY		0x20
#تعریف_کرو EXT2_FLAG_تبادلہ_بائٹیں		0x40
#تعریف_کرو EXT2_FLAG_تبادلہ_بائٹیں_READ	0x80
#تعریف_کرو EXT2_FLAG_تبادلہ_بائٹیں_WRITE	0x100
#تعریف_کرو EXT2_FLAG_MASTER_SB_ONLY	0x200
#تعریف_کرو EXT2_FLAG_FORCE			0x400
#تعریف_کرو EXT2_FLAG_SUPER_ONLY		0x800
#تعریف_کرو EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#تعریف_کرو EXT2_FLAG_IMAGE_FILE		0x2000
#تعریف_کرو EXT2_FLAG_EXCLUSIVE		0x4000
#تعریف_کرو EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#تعریف_کرو EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#تعریف_کرو EXT2_FLAG_64_بٹ		0x20000
#تعریف_کرو EXT2_FLAG_PRINT_پیش_رفت	0x40000
#تعریف_کرو EXT2_FLAG_براہ_راست_IO		0x80000
#تعریف_کرو EXT2_FLAG_SKIP_MMP		0x100000
#تعریف_کرو EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#تعریف_کرو EXT2_FLAG_SHARE_DUP		0x400000
#تعریف_کرو EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#تعریف_کرو EXT2_FLAG_BBITMAP_دم_PROBLEM	0x1000000
#تعریف_کرو EXT2_FLAG_IBITMAP_دم_PROBLEM	0x2000000
#تعریف_کرو EXT2_FLAG_دھاگے		0x4000000
#تعریف_کرو EXT2_FLAG_IGNORE_تبادلہ_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#تعریف_کرو EXT2_FLAG2_USE_FAKE_وقت	0x000000001

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
#تعریف_کرو EXT2_JOURNAL_مقصد_FC_BLKS_تناسب		64

ساخت blk_alloc_ctx;
ساخت opaque_ext2_گروہ_desc;

ساخت struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	صحیح				flags;
	حرف *				device_name;
	ساخت ext2_super_بلاک	* 	super;
	غیرعلامتی صحیح			blocksize;
	صحیح				fragsize;
	dgrp_t				گروہ_desc_گنتی;
	غیرعلامتی طویل			desc_بلاکیں;
	ساخت opaque_ext2_گروہ_desc *	گروہ_desc;
	غیرعلامتی صحیح			inode_بلاکیں_فی_گروہ;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_بلاک_bitmap		بلاک_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_بلاکیں)(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکیں);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ساخت ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ساخت ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ساخت ext2_super_بلاک *	orig_super;
	ساخت ext2_image_hdr *		image_سرخی;
	__u32				umask;
	time_t				now;
	صحیح				cluster_تناسب_بٹ;
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
	خالی *				priv_ڈیٹا;

	/*
	 * Inode cache
	 */
	ساخت ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_بلاک)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ساخت blk_alloc_ctx *ctx);
	خالی (*بلاک_alloc_stats)(ext2_filsys fs, blk64_t blk, صحیح inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	خالی *mmp_buf;
	خالی *mmp_cmp;
	صحیح mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	طویل mmp_آخری_written;

	/* progress operation functions */
	ساخت ext2fs_پیش_رفت_ops *پیش_رفت_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_بیج;

	io_channel			journal_io;
	حرف				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	خالی (*بلاک_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t عدد,
					صحیح inuse);

	/* hashmap for SHA of data blocks */
	ساخت ext2fs_hashmap* بلاک_sha_map;

	ثابت ساخت ext2fs_nls_جدول *encoding;
};

#اگر EXT2_FLAT_INCLUDES
#شامل "e2_bitops.h"
#ورنہ
#شامل <ext2fs/bitops.h>
#اگر_ختم

/*
 * 64-bit bitmap backend types
 */
#تعریف_کرو EXT2FS_BMAP64_BITARRAY	1
#تعریف_کرو EXT2FS_BMAP64_RBTREE	2
#تعریف_کرو EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#تعریف_کرو بلاک_بدلا			1
#تعریف_کرو بلاک_ABORT			2
#تعریف_کرو بلاک_ERROR			4
#تعریف_کرو بلاک_INLINE_ڈیٹا_بدلا	8

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
#تعریف_کرو بلاک_FLAG_گہرائی_TRAVERSE	2
#تعریف_کرو بلاک_FLAG_ڈیٹا_ONLY	4
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
#تعریف_کرو بلاک_ALLOC_ڈیٹا	1
#تعریف_کرو بلاک_ALLOC_METADATA	2

ساخت blk_alloc_ctx {
	ext2_ino_t		ino;
	ساخت ext2_inode	*inode;
	blk64_t			lblk;
	صحیح			flags;
};

#اگر 0
/*
 * Flags for ext2fs_move_blocks
 */
#تعریف_کرو EXT2_BMOVE_GET_DBLIST	0x0001
#تعریف_کرو EXT2_BMOVE_DEBUG	0x0002
#اگر_ختم

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#تعریف_کرو EXT2_EXTENT_FLAGS_برگ		0x0001
#تعریف_کرو EXT2_EXTENT_FLAGS_UNINIT	0x0002
#تعریف_کرو EXT2_EXTENT_FLAGS_دوسرا_دورہ	0x0004

ساخت ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

قسمتعریف ساخت ext2_extent_handle *ext2_extent_handle_t;
قسمتعریف ساخت ext2_extent_راستہ *ext2_extent_راستہ_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#تعریف_کرو EXT2_EXTENT_CURRENT	0x0000
#تعریف_کرو EXT2_EXTENT_MOVE_نقاب	0x000F
#تعریف_کرو EXT2_EXTENT_جڑ	0x0001
#تعریف_کرو EXT2_EXTENT_آخری_برگ	0x0002
#تعریف_کرو EXT2_EXTENT_پہلا_SIB	0x0003
#تعریف_کرو EXT2_EXTENT_آخری_SIB	0x0004
#تعریف_کرو EXT2_EXTENT_اگلا_SIB	0x0005
#تعریف_کرو EXT2_EXTENT_پچھلا_SIB	0x0006
#تعریف_کرو EXT2_EXTENT_اگلا_برگ	0x0007
#تعریف_کرو EXT2_EXTENT_پچھلا_برگ	0x0008
#تعریف_کرو EXT2_EXTENT_اگلا	0x0009
#تعریف_کرو EXT2_EXTENT_پچھلا	0x000A
#تعریف_کرو EXT2_EXTENT_اوپر		0x000B
#تعریف_کرو EXT2_EXTENT_نیچے_کو	0x000C
#تعریف_کرو EXT2_EXTENT_نیچے_کو_AND_آخری 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#تعریف_کرو EXT2_EXTENT_داخل_کرو_AFTER	0x0001 /* insert after handle loc'n */
#تعریف_کرو EXT2_EXTENT_داخل_کرو_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#تعریف_کرو EXT2_EXTENT_DELETE_KEEP_خالی_خانے	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#تعریف_کرو EXT2_EXTENT_سیٹ_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
ساخت ext2_extent_info {
	صحیح		curr_اندراج;
	صحیح		curr_level;
	صحیح		عدد_entries;
	صحیح		max_entries;
	صحیح		max_گہرائی;
	صحیح		بائٹیں_avail;
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
#تعریف_کرو DIRENT_بدلا	1
#تعریف_کرو DIRENT_ABORT	2
#تعریف_کرو DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#تعریف_کرو DIRENT_FLAG_INCLUDE_خالی_خانے	1
#تعریف_کرو DIRENT_FLAG_INCLUDE_REMOVED	2
#تعریف_کرو DIRENT_FLAG_INCLUDE_CSUM	4
#تعریف_کرو DIRENT_FLAG_INCLUDE_INLINE_ڈیٹا 8

#تعریف_کرو DIRENT_DOT_FILE		1
#تعریف_کرو DIRENT_DOT_DOT_FILE	2
#تعریف_کرو DIRENT_OTHER_FILE	3
#تعریف_کرو DIRENT_DELETED_FILE	4
#تعریف_کرو DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
قسمتعریف ساخت ext2_struct_inode_جائزہ *ext2_inode_جائزہ;

/*
 * ext2fs_scan flags
 */
#تعریف_کرو EXT2_SF_CHK_BADBLOCKS	0x0001
#تعریف_کرو EXT2_SF_BAD_INODE_BLK	0x0002
#تعریف_کرو EXT2_SF_BAD_EXTRA_بائٹیں	0x0004
#تعریف_کرو EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#تعریف_کرو EXT2_SF_DO_LAZY		0x0010
#تعریف_کرو EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#تعریف_کرو EXT2_MF_MOUNTED		1
#تعریف_کرو EXT2_MF_ISROOT		2
#تعریف_کرو EXT2_MF_READONLY	4
#تعریف_کرو EXT2_MF_تبادلہ		8
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
#تعریف_کرو EXT2_I_سائز(i)	((i)->i_سائز | ((__u64) (i)->i_سائز_بلند << 32))

ساکن اندرونی __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((غیر_مستعمل)),
					__u32 nsec)
{
	__u32 extra = 0;

#اگر (SIZEOF_وقت_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_نقاب;
#اگر_ختم
	واپس extra | (nsec << EXT4_EPOCH_بٹ);
}
ساکن اندرونی time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((غیر_مستعمل)))
{
#اگر (SIZEOF_وقت_T > 4)
	اگر (extra & EXT4_EPOCH_نقاب)
		seconds += ((time_t)(extra & EXT4_EPOCH_نقاب) << 32);
#اگر_ختم
	واپس seconds;
}
ساکن اندرونی __u32 __decode_extra_nsec(__u32 extra)
{
	واپس (extra & EXT4_NSEC_نقاب) >> EXT4_EPOCH_بٹ;
}
#تعریف_کرو ext2fs_inode_actual_سائز(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_سائز +				      \
		  (حجم(*inode) > EXT2_GOOD_OLD_INODE_سائز ?		      \
		   ((ساخت ext2_inode_large *)(inode))->i_extra_isize : 0)))
#تعریف_کرو clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#تعریف_کرو ext2fs_inode_xtime_سیٹ(inode, میدان, sec)			      \
کرو {									      \
	اگر (ext2fs_inode_includes(ext2fs_inode_actual_سائز(inode),	      \
				  میدان ## _extra)) {			      \
		(inode)->میدان = (__u32)(sec & 0xfffffff);		      \
		((ساخت ext2_inode_large *)(inode))->میدان ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} ورنہ {							      \
		(inode)->میدان = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} جب (0)
#تعریف_کرو ext2fs_inode_xtime_get(inode, میدان)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_سائز(inode), میدان ## _extra) ?    \
	__decode_extra_sec((inode)->میدان,				      \
		((ساخت ext2_inode_large *)(inode))->میدان ## _extra) :      \
		(time_t)(inode)->میدان)

ساکن اندرونی خالی __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((غیر_مستعمل)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#اگر (SIZEOF_وقت_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_نقاب;
#ورنہ
	*hi = 0;
#اگر_ختم
}
ساکن اندرونی time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((غیر_مستعمل)))
{
#اگر (SIZEOF_وقت_T == 4)
	واپس *lo;
#ورنہ
	واپس ((time_t)(*hi) << 32) | *lo;
#اگر_ختم
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

قسمتعریف ساخت ext2_icount *ext2_icount_t;

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

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)

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

#اگر_تعریف_شدہ CONFIG_MMP
#تعریف_کرو EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#ورنہ
#تعریف_کرو EXT4_LIB_INCOMPAT_MMP		(0)
#اگر_ختم

#تعریف_کرو EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_ثنائی_ہندسہ|\
					 EXT4_FEATURE_INCOMPAT_INLINE_ڈیٹا|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_بلاکیں |\
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

#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
قسمتعریف ساخت stat64 ext2fs_struct_stat;
#ورنہ
قسمتعریف ساخت stat ext2fs_struct_stat;
#اگر_ختم

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#تعریف_کرو EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ساخت ext2_xattr_handle;
#تعریف_کرو XATTR_ABORT	1
#تعریف_کرو XATTR_بدلا	2

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
ساکن اندرونی صحیح ext2fs_has_گروہ_desc_csum(ext2_filsys fs)
{
	واپس ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ساکن اندرونی صحیح ext2fs_needs_large_file_feature(غیرعلامتی طویل طویل file_سائز)
{
	واپس file_سائز >= 0x80000000ULL;
}

/* alloc.c */
خارجی خالی ext2fs_clear_بلاک_uninit(ext2_filsys fs, dgrp_t گروہ);
خارجی errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, صحیح منوال,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
خارجی errcode_t ext2fs_new_بلاک(ext2_filsys fs, blk_t goal,
				  ext2fs_بلاک_bitmap map, blk_t *ret);
خارجی errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاک_bitmap map, blk64_t *ret);
خارجی errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_بلاک_bitmap map, blk64_t *ret,
				   ساخت blk_alloc_ctx *ctx);
خارجی errcode_t ext2fs_get_free_بلاکیں(ext2_filsys fs, blk_t آغاز,
					blk_t اختتام, صحیح عدد,
					ext2fs_بلاک_bitmap map,
					blk_t *ret);
خارجی errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t آغاز,
					 blk64_t اختتام, صحیح عدد,
					 ext2fs_بلاک_bitmap map,
					 blk64_t *ret);
خارجی errcode_t ext2fs_alloc_بلاک(ext2_filsys fs, blk_t goal,
				    حرف *بلاک_buf, blk_t *ret);
خارجی errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     حرف *بلاک_buf, blk64_t *ret);
خارجی errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     حرف *بلاک_buf, blk64_t *ret,
				     ساخت blk_alloc_ctx *ctx);

خارجی خالی ext2fs_سیٹ_alloc_بلاک_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_ڈھونڈو_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ساخت ext2_inode *inode, blk64_t lblk);
خارجی خالی ext2fs_سیٹ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
خارجی خالی ext2fs_سیٹ_بلاک_alloc_stats_range_callback(ext2_filsys fs,
	خالی (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse),
	خالی (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse));
#تعریف_کرو EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#تعریف_کرو EXT2_NEWRANGE_MIN_لمبائی	(0x2)
#تعریف_کرو EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			   blk64_t len, ext2fs_بلاک_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#تعریف_کرو EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#تعریف_کرو EXT2_ALLOCRANGE_ZERO_بلاکیں	(0x2)
#تعریف_کرو EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
خارجی صحیح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t گروہ,
					ext2fs_بلاک_bitmap bmap);
خارجی errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t گروہ,
					       ext2fs_بلاک_bitmap bmap,
					       blk_t *desc_بلاکیں);
خارجی خالی ext2fs_سیٹ_بلاک_alloc_stats_callback(ext2_filsys fs,
						  خالی (*func)(ext2_filsys fs,
							       blk64_t blk,
							       صحیح inuse),
						  خالی (**old)(ext2_filsys fs,
							       blk64_t blk,
							       صحیح inuse));

/* alloc_stats.c */
خالی ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحیح inuse);
خالی ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحیح inuse, صحیح isdir);
خالی ext2fs_بلاک_alloc_stats(ext2_filsys fs, blk_t blk, صحیح inuse);
خالی ext2fs_بلاک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحیح inuse);
خالی ext2fs_بلاک_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse);

/* alloc_tables.c */
خارجی errcode_t ext2fs_allocate_tables(ext2_filsys fs);
خارجی errcode_t ext2fs_allocate_گروہ_جدول(ext2_filsys fs, dgrp_t گروہ,
					     ext2fs_بلاک_bitmap bmap);

/* badblocks.c */
خارجی errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحیح سائز);
خارجی errcode_t ext2fs_u32_list_جوڑو(ext2_u32_list bb, __u32 blk);
خارجی صحیح ext2fs_u32_list_ڈھونڈو(ext2_u32_list bb, __u32 blk);
خارجی صحیح ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
خارجی errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
خارجی صحیح ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
خارجی خالی ext2fs_u32_list_iterate_انتہا(ext2_u32_iterate iter);
خارجی errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
خارجی صحیح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

خارجی errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    صحیح سائز);
خارجی errcode_t ext2fs_badblocks_list_جوڑو(ext2_badblocks_list bb,
					   blk_t blk);
خارجی صحیح ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
خارجی صحیح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
خارجی خالی ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
خارجی errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
خارجی صحیح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
خارجی خالی ext2fs_badblocks_list_iterate_انتہا(ext2_badblocks_iterate iter);
خارجی errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
خارجی صحیح ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
خارجی صحیح ext2fs_u32_list_گنتی(ext2_u32_list bb);

/* bb_compat */
خارجی errcode_t badblocks_list_create(badblocks_list *ret, صحیح سائز);
خارجی errcode_t badblocks_list_جوڑو(badblocks_list bb, blk_t blk);
خارجی صحیح badblocks_list_test(badblocks_list bb, blk_t blk);
خارجی errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
خارجی صحیح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
خارجی خالی badblocks_list_iterate_انتہا(badblocks_iterate iter);
خارجی خالی badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
خارجی errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
خارجی خالی ext2fs_free_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap);
خارجی خالی ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجی errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
خارجی errcode_t ext2fs_allocate_بلاک_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_بلاک_bitmap *ret);
خارجی errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ثابت حرف *descr,
						   ext2fs_بلاک_bitmap *ret);
خارجی صحیح ext2fs_get_bitmap_granularity(ext2fs_بلاک_bitmap bitmap);
خارجی errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_inode_bitmap *ret);
خارجی errcode_t ext2fs_fudge_inode_bitmap_انتہا(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t انتہا, ext2_ino_t *oend);
خارجی errcode_t ext2fs_fudge_بلاک_bitmap_انتہا(ext2fs_بلاک_bitmap bitmap,
					       blk_t انتہا, blk_t *oend);
خارجی errcode_t ext2fs_fudge_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t انتہا, blk64_t *oend);
خارجی خالی ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجی خالی ext2fs_clear_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap);
خارجی errcode_t ext2fs_resize_inode_bitmap(__u32 new_انتہا, __u32 new_real_انتہا,
					    ext2fs_inode_bitmap bmap);
خارجی errcode_t ext2fs_resize_inode_bitmap2(__u64 new_انتہا,
					     __u64 new_real_انتہا,
					     ext2fs_inode_bitmap bmap);
خارجی errcode_t ext2fs_resize_بلاک_bitmap(__u32 new_انتہا, __u32 new_real_انتہا,
					    ext2fs_بلاک_bitmap bmap);
خارجی errcode_t ext2fs_resize_بلاک_bitmap2(__u64 new_انتہا,
					     __u64 new_real_انتہا,
					     ext2fs_بلاک_bitmap bmap);
خارجی errcode_t ext2fs_compare_بلاک_bitmap(ext2fs_بلاک_bitmap bm1,
					     ext2fs_بلاک_bitmap bm2);
خارجی errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
خارجی errcode_t ext2fs_سیٹ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر);
خارجی errcode_t ext2fs_سیٹ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 آغاز, size_t عدد,
					 خالی *اندر);
خارجی errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد);
خارجی errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 آغاز, size_t عدد,
					 خالی *برآمد);
خارجی errcode_t ext2fs_سیٹ_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر);
خارجی errcode_t ext2fs_سیٹ_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t آغاز, size_t عدد,
					 خالی *اندر);
خارجی errcode_t ext2fs_get_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد);
خارجی errcode_t ext2fs_get_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t آغاز, size_t عدد,
					 خالی *برآمد);

/* blknum.c */
خارجی __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجی __u32 ext2fs_بلاک_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجی dgrp_t ext2fs_گروہ_کا_blk2(ext2_filsys fs, blk64_t);
خارجی blk64_t ext2fs_گروہ_پہلا_block2(ext2_filsys fs, dgrp_t گروہ);
خارجی blk64_t ext2fs_گروہ_آخری_block2(ext2_filsys fs, dgrp_t گروہ);
خارجی صحیح ext2fs_گروہ_بلاکیں_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجی blk64_t ext2fs_inode_ڈیٹا_blocks2(ext2_filsys fs,
					 ساخت ext2_inode *inode);
خارجی blk64_t ext2fs_inode_i_بلاکیں(ext2_filsys fs,
				     ساخت ext2_inode *inode);
خارجی blk64_t ext2fs_get_stat_i_بلاکیں(ext2_filsys fs,
					ساخت ext2_inode *inode);
خارجی blk64_t ext2fs_بلاکیں_گنتی(ساخت ext2_super_بلاک *super);
خارجی خالی ext2fs_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super,
				    blk64_t blk);
خارجی خالی ext2fs_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super,
				    blk64_t blk);
خارجی blk64_t ext2fs_r_بلاکیں_گنتی(ساخت ext2_super_بلاک *super);
خارجی خالی ext2fs_r_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super,
				      blk64_t blk);
خارجی خالی ext2fs_r_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super,
				      blk64_t blk);
خارجی blk64_t ext2fs_free_بلاکیں_گنتی(ساخت ext2_super_بلاک *super);
خارجی خالی ext2fs_free_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super,
					 blk64_t blk);
خارجی خالی ext2fs_free_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
خارجی ساخت ext2_گروہ_desc *ext2fs_گروہ_desc(ext2_filsys fs,
					  ساخت opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ);
خارجی blk64_t ext2fs_بلاک_bitmap_csum(ext2_filsys fs, dgrp_t گروہ);
خارجی blk64_t ext2fs_بلاک_bitmap_loc(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_بلاک_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					blk64_t blk);
خارجی __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t گروہ);
خارجی blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_inode_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					blk64_t blk);
خارجی blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_inode_جدول_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       blk64_t blk);
خارجی __u32 ext2fs_bg_free_بلاکیں_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_free_بلاکیں_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					 __u32 n);
خارجی __u32 ext2fs_bg_free_inodes_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_free_inodes_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					 __u32 n);
خارجی __u32 ext2fs_bg_مستعمل_dirs_گنتی(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_مستعمل_dirs_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       __u32 n);
خارجی __u32 ext2fs_bg_itable_غیر_مستعمل(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_itable_غیر_مستعمل_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				     __u32 n);
خارجی __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروہ);
خارجی صحیح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flag);
خارجی خالی ext2fs_bg_flags_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags);
خارجی خالی ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags);
خارجی __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروہ);
خارجی خالی ext2fs_bg_checksum_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 checksum);
خارجی blk64_t ext2fs_file_acl_بلاک(ext2_filsys fs,
				     ثابت ساخت ext2_inode *inode);
خارجی خالی ext2fs_file_acl_بلاک_سیٹ(ext2_filsys fs,
				      ساخت ext2_inode *inode, blk64_t blk);
خارجی errcode_t ext2fs_inode_سائز_سیٹ(ext2_filsys fs, ساخت ext2_inode *inode,
				       ext2_off64_t سائز);

/* block.c */
خارجی errcode_t ext2fs_بلاک_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      صحیح	flags,
				      حرف *بلاک_buf,
				      صحیح (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  صحیح	blockcnt,
						  خالی	*priv_ڈیٹا),
				      خالی *priv_ڈیٹا);
errcode_t ext2fs_بلاک_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				صحیح	flags,
				حرف *بلاک_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحیح		ref_انحراف,
					    خالی	*priv_ڈیٹا),
				خالی *priv_ڈیٹا);
errcode_t ext2fs_بلاک_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				حرف *بلاک_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحیح		ref_انحراف,
					    خالی	*priv_ڈیٹا),
				خالی *priv_ڈیٹا);

/* bmap.c */
خارجی errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode *inode,
			     حرف *بلاک_buf, صحیح bmap_flags,
			     blk_t بلاک, blk_t *phys_blk);
خارجی errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ساخت ext2_inode *inode,
			      حرف *بلاک_buf, صحیح bmap_flags, blk64_t بلاک,
			      صحیح *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_بلاک(ext2_filsys fs, ext2_ino_t ino,
				   ساخت ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#اگر 0
/* bmove.c */
خارجی errcode_t ext2fs_move_بلاکیں(ext2_filsys fs,
				    ext2fs_بلاک_bitmap reserve,
				    ext2fs_بلاک_bitmap alloc_map,
				    صحیح flags);
#اگر_ختم

/* check_desc.c */
خارجی errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
خارجی errcode_t ext2fs_close(ext2_filsys fs);
خارجی errcode_t ext2fs_close2(ext2_filsys fs, صحیح flags);
خارجی errcode_t ext2fs_close_free(ext2_filsys *fs);
خارجی errcode_t ext2fs_flush(ext2_filsys fs);
خارجی errcode_t ext2fs_flush2(ext2_filsys fs, صحیح flags);
خارجی صحیح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروہ_بلاک);
خارجی errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t گروہ,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_مستعمل_blks);
خارجی صحیح ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t گروہ,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    صحیح *ret_meta_bg);
خارجی خالی ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
خارجی __u32 ext2fs_چکری_جانچ_32_be(__u32 چکری_جانچ, غیرعلامتی حرف ثابت *p, size_t len);
خارجی __u32 ext2fs_crc32c_le(__u32 چکری_جانچ, غیرعلامتی حرف ثابت *p, size_t len);

/* csum.c */
خارجی خالی ext2fs_init_csum_بیج(ext2_filsys fs);
خارجی errcode_t ext2fs_mmp_csum_سیٹ(ext2_filsys fs, ساخت mmp_struct *mmp);
خارجی صحیح ext2fs_mmp_csum_verify(ext2_filsys, ساخت mmp_struct *mmp);
خارجی صحیح ext2fs_verify_csum_type(ext2_filsys fs, ساخت ext2_super_بلاک *sb);
خارجی errcode_t ext2fs_superblock_csum_سیٹ(ext2_filsys fs,
					    ساخت ext2_super_بلاک *sb);
خارجی صحیح ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ساخت ext2_super_بلاک *sb);
خارجی errcode_t ext2fs_ext_attr_بلاک_csum_سیٹ(ext2_filsys fs,
					ext2_ino_t inum, blk64_t بلاک,
					ساخت ext2_ext_attr_سرخی *hdr);
خارجی صحیح ext2fs_ext_attr_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t بلاک,
					     ساخت ext2_ext_attr_سرخی *hdr);
#تعریف_کرو EXT2_DIRENT_دم(بلاک, blocksize) \
	((ساخت ext2_dir_اندراج_دم *)(((حرف *)(بلاک)) + \
	(blocksize) - حجم(ساخت ext2_dir_اندراج_دم)))

خارجی خالی ext2fs_initialize_dirent_دم(ext2_filsys fs,
					  ساخت ext2_dir_اندراج_دم *t);
خارجی صحیح ext2fs_dirent_has_دم(ext2_filsys fs,
				  ساخت ext2_dir_اندراج *dirent);
خارجی صحیح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ساخت ext2_dir_اندراج *dirent);
خارجی صحیح ext2fs_dir_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ساخت ext2_dir_اندراج *dirent);
خارجی errcode_t ext2fs_dir_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					   ساخت ext2_dir_اندراج *dirent);
خارجی errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ساخت ext2_dir_اندراج *dirent,
					  ساخت ext2_dx_countlimit **cc,
					  صحیح *انحراف);
خارجی errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ساخت ext2_dir_اندراج *dirent,
				__u32 *چکری_جانچ, ساخت ext2_dx_دم **ret_t);
خارجی errcode_t ext2fs_extent_بلاک_csum_سیٹ(ext2_filsys fs,
					      ext2_ino_t inum,
					      ساخت ext3_extent_سرخی *eh);
خارجی صحیح ext2fs_extent_بلاک_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ساخت ext3_extent_سرخی *eh);
خارجی errcode_t ext2fs_بلاک_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					      حرف *bitmap, صحیح سائز);
خارجی صحیح ext2fs_بلاک_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
					   حرف *bitmap, صحیح سائز);
خارجی errcode_t ext2fs_inode_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
					      حرف *bitmap, صحیح سائز);
خارجی صحیح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
					   حرف *bitmap, صحیح سائز);
خارجی errcode_t ext2fs_inode_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				       ساخت ext2_inode_large *inode);
خارجی صحیح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ساخت ext2_inode_large *inode);
خارجی خالی ext2fs_گروہ_desc_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ);
خارجی صحیح ext2fs_گروہ_desc_csum_verify(ext2_filsys fs, dgrp_t گروہ);
خارجی errcode_t ext2fs_سیٹ_gdt_csum(ext2_filsys fs);
خارجی __u16 ext2fs_گروہ_desc_csum(ext2_filsys fs, dgrp_t گروہ);

/* dblist.c */
خارجی errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
خارجی errcode_t ext2fs_جوڑو_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحیح blockcnt);
خارجی errcode_t ext2fs_جوڑو_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجی خالی ext2fs_dblist_ترتیب_دو(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ثابت خالی *,
							   ثابت خالی *));
خارجی خالی ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ثابت خالی *,
							    ثابت خالی *));
خارجی errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساخت ext2_db_اندراج *db_info,
		    خالی	*priv_ڈیٹا),
	خالی *priv_ڈیٹا);
خارجی errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساخت ext2_db_entry2 *db_info,
		    خالی	*priv_ڈیٹا),
	خالی *priv_ڈیٹا);
خارجی errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساخت ext2_db_entry2 *db_info,
		    خالی	*priv_ڈیٹا),
	غیرعلامتی طویل طویل آغاز,
	غیرعلامتی طویل طویل گنتی,
	خالی *priv_ڈیٹا);
خارجی errcode_t ext2fs_سیٹ_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحیح blockcnt);
خارجی errcode_t ext2fs_سیٹ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجی errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
خارجی صحیح ext2fs_dblist_گنتی(ext2_dblist dblist);
خارجی blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
خارجی errcode_t ext2fs_dblist_get_آخری(ext2_dblist dblist,
					ساخت ext2_db_اندراج **اندراج_2);
خارجی errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ساخت ext2_db_entry2 **اندراج_2);
خارجی errcode_t ext2fs_dblist_drop_آخری(ext2_dblist dblist);

/* dblist_dir.c */
خارجی errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  صحیح	flags,
				  حرف	*بلاک_buf,
				  صحیح (*func)(ext2_ino_t	dir,
					      صحیح		اندراج_2,
					      ساخت ext2_dir_اندراج *dirent,
					      صحیح	انحراف,
					      صحیح	blocksize,
					      حرف	*buf,
					      خالی	*priv_ڈیٹا),
				  خالی *priv_ڈیٹا);

#اگر 0
/* digest_encode.c */
#تعریف_کرو EXT2_FS_DIGEST_سائز EXT2_FS_SHA256_لمبائی
خارجی صحیح ext2fs_digest_encode(ثابت حرف *src, صحیح len, حرف *dst);
خارجی صحیح ext2fs_digest_decode(ثابت حرف *src, صحیح len, حرف *dst);
#اگر_ختم

/* dirblock.c */
خارجی errcode_t ext2fs_read_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				       خالی *buf);
خارجی errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاک,
					خالی *buf, صحیح flags);
خارجی errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاک,
					خالی *buf, صحیح flags);
خارجی errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاک,
					خالی *buf, صحیح flags, ext2_ino_t ino);
خارجی errcode_t ext2fs_write_dir_بلاک(ext2_filsys fs, blk_t بلاک,
					خالی *buf);
خارجی errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاک,
					 خالی *buf, صحیح flags);
خارجی errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاک,
					 خالی *buf, صحیح flags);
خارجی errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاک,
					 خالی *buf, صحیح flags, ext2_ino_t ino);

/* dirhash.c */
خارجی errcode_t ext2fs_dirhash(صحیح نسخہ, ثابت حرف *name, صحیح len,
				ثابت __u32 *بیج,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

خارجی errcode_t ext2fs_dirhash2(صحیح نسخہ, ثابت حرف *name, صحیح len,
				 ثابت ساخت ext2fs_nls_جدول *charset,
				 صحیح hash_flags,
				 ثابت __u32 *بیج,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
خارجی errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ساخت ext2_dir_اندراج *dirent,
				    غیرعلامتی صحیح *rec_len);
خارجی errcode_t ext2fs_سیٹ_rec_len(ext2_filsys fs,
				    غیرعلامتی صحیح len,
				    ساخت ext2_dir_اندراج *dirent);
خارجی errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      حرف *بلاک_buf,
			      صحیح (*func)(ساخت ext2_dir_اندراج *dirent,
					  صحیح	انحراف,
					  صحیح	blocksize,
					  حرف	*buf,
					  خالی	*priv_ڈیٹا),
			      خالی *priv_ڈیٹا);
خارجی errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      حرف *بلاک_buf,
			      صحیح (*func)(ext2_ino_t	dir,
					  صحیح	اندراج_2,
					  ساخت ext2_dir_اندراج *dirent,
					  صحیح	انحراف,
					  صحیح	blocksize,
					  حرف	*buf,
					  خالی	*priv_ڈیٹا),
			      خالی *priv_ڈیٹا);

/* dupfs.c */
خارجی errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
خارجی errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
خارجی __u32 ext2fs_ext_attr_hash_اندراج(ساخت ext2_ext_attr_اندراج *اندراج_2,
					خالی *ڈیٹا);
خارجی __u32 ext2fs_ext_attr_hash_اندراج_signed(ساخت ext2_ext_attr_اندراج *اندراج_2,
					       خالی *ڈیٹا);
خارجی errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ساخت ext2_ext_attr_اندراج *اندراج_2,
					     خالی *ڈیٹا, __u32 *hash);
خارجی errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ساخت ext2_ext_attr_اندراج *اندراج_2,
					     خالی *ڈیٹا, __u32 *hash,
					     __u32 *signed_hash);
خارجی errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاک, خالی *buf);
خارجی errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاک,
				       خالی *buf);
خارجی errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاک,
				       خالی *buf, ext2_ino_t inum);
خارجی errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاک,
				       خالی *buf);
خارجی errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاک,
				       خالی *buf);
خارجی errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاک,
				       خالی *buf, ext2_ino_t inum);
خارجی errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   حرف *بلاک_buf,
					   صحیح adjust, __u32 *newcount);
خارجی errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   حرف *بلاک_buf,
					   صحیح adjust, __u32 *newcount);
خارجی errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   حرف *بلاک_buf,
					   صحیح adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ساخت ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ساخت ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ساخت ext2_xattr_handle *handle,
				   ساخت ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ساخت ext2_xattr_handle *h,
				صحیح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, خالی *ڈیٹا),
				خالی *ڈیٹا);
errcode_t ext2fs_xattr_get(ساخت ext2_xattr_handle *h, ثابت حرف *کلید,
			   خالی **قدر, size_t *قدر_len);
errcode_t ext2fs_xattr_سیٹ(ساخت ext2_xattr_handle *handle,
			   ثابت حرف *کلید,
			   ثابت خالی *قدر,
			   size_t قدر_len);
errcode_t ext2fs_xattr_remove(ساخت ext2_xattr_handle *handle,
			      ثابت حرف *کلید);
errcode_t ext2fs_xattr_remove_all(ساخت ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ساخت ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ساخت ext2_inode_large *inode);
errcode_t ext2fs_xattrs_گنتی(ساخت ext2_xattr_handle *handle, size_t *گنتی);
errcode_t ext2fs_xattr_inode_max_سائز(ext2_filsys fs, ext2_ino_t ino,
				      size_t *سائز);
#تعریف_کرو XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ساخت ext2_xattr_handle *handle,
			      غیرعلامتی صحیح *new_flags, غیرعلامتی صحیح *old_flags);
خارجی خالی ext2fs_ext_attr_بلاک_rehash(ساخت ext2_ext_attr_سرخی *سرخی,
					 ساخت ext2_ext_attr_اندراج *انتہا);
خارجی __u32 ext2fs_get_ea_inode_hash(ساخت ext2_inode *inode);
خارجی خالی ext2fs_سیٹ_ea_inode_hash(ساخت ext2_inode *inode, __u32 hash);
خارجی __u64 ext2fs_get_ea_inode_ref(ساخت ext2_inode *inode);
خارجی خالی ext2fs_سیٹ_ea_inode_ref(ساخت ext2_inode *inode, __u64 ref_گنتی);

/* extent.c */
خارجی errcode_t ext2fs_extent_سرخی_verify(خالی *ptr, صحیح سائز);
خارجی errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
خارجی errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ساخت ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
خارجی خالی ext2fs_extent_free(ext2_extent_handle_t handle);
خارجی errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   صحیح flags, ساخت ext2fs_extent *extent);
خارجی errcode_t ext2fs_extent_گرہ_بانٹو(ext2_extent_handle_t handle);
خارجی errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, صحیح flags,
				       ساخت ext2fs_extent *extent);
خارجی errcode_t ext2fs_extent_داخل_کرو(ext2_extent_handle_t handle, صحیح flags,
				      ساخت ext2fs_extent *extent);
خارجی errcode_t ext2fs_extent_سیٹ_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					صحیح flags);
خارجی errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحیح flags);
خارجی errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ساخت ext2_extent_info *info);
خارجی errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
خارجی errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     صحیح برگ_level, blk64_t blk);
خارجی errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
خارجی size_t ext2fs_max_extent_گہرائی(ext2_extent_handle_t handle);
خارجی errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ساخت ext2_inode *inode);
خارجی errcode_t ext2fs_گنتی_بلاکیں(ext2_filsys fs, ext2_ino_t ino,
				     ساخت ext2_inode *inode, blk64_t *ret_گنتی);
خارجی errcode_t ext2fs_decode_extent(ساخت ext2fs_extent *مقصد, خالی *from,
				      صحیح len);

/* fallocate.c */
#تعریف_کرو EXT2_FALLOCATE_ZERO_بلاکیں	(0x1)
#تعریف_کرو EXT2_FALLOCATE_FORCE_INIT	(0x2)
#تعریف_کرو EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#تعریف_کرو EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#تعریف_کرو EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
			   ساخت ext2_inode *inode, blk64_t goal,
			   blk64_t آغاز, blk64_t len);

/* fileio.c */
خارجی errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ساخت ext2_inode *inode,
				   صحیح flags, ext2_file_t *ret);
خارجی errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  صحیح flags, ext2_file_t *ret);
خارجی ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ساخت ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
خارجی ext2_ino_t ext2fs_file_get_inode_عدد(ext2_file_t file);
خارجی errcode_t ext2fs_file_close(ext2_file_t file);
خارجی errcode_t ext2fs_file_flush(ext2_file_t file);
خارجی errcode_t ext2fs_file_read(ext2_file_t file, خالی *buf,
				  غیرعلامتی صحیح wanted, غیرعلامتی صحیح *got);
خارجی errcode_t ext2fs_file_write(ext2_file_t file, ثابت خالی *buf,
				   غیرعلامتی صحیح nbytes, غیرعلامتی صحیح *written);
خارجی errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 انحراف,
				   صحیح whence, __u64 *ret_pos);
خارجی errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t انحراف,
				   صحیح whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_سائز);
خارجی ext2_off_t ext2fs_file_get_سائز(ext2_file_t file);
خارجی errcode_t ext2fs_file_سیٹ_سائز(ext2_file_t file, ext2_off_t سائز);
خارجی errcode_t ext2fs_file_سیٹ_size2(ext2_file_t file, ext2_off64_t سائز);

/* finddev.c */
خارجی حرف *ext2fs_ڈھونڈو_بلاک_device(dev_t device);

/* flushb.c */
خارجی errcode_t ext2fs_sync_device(صحیح fd, صحیح flushb);

/* freefs.c */
خارجی خالی ext2fs_free(ext2_filsys fs);
خارجی خالی ext2fs_free_dblist(ext2_dblist dblist);
خارجی خالی ext2fs_badblocks_list_free(ext2_badblocks_list bb);
خارجی خالی ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
خارجی خالی ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
خارجی errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 آغاز, __u32 انتہا,
					    __u32 real_انتہا,
					    ثابت حرف *descr, حرف *init_map,
					    ext2fs_generic_bitmap *ret);
خارجی errcode_t ext2fs_allocate_generic_bitmap(__u32 آغاز,
						__u32 انتہا,
						__u32 real_انتہا,
						ثابت حرف *descr,
						ext2fs_generic_bitmap *ret);
خارجی errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
خارجی خالی ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
خارجی errcode_t ext2fs_fudge_generic_bitmap_انتہا(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t انتہا,
						 ext2_ino_t *oend);
خارجی خالی ext2fs_سیٹ_generic_bitmap_padding(ext2fs_generic_bitmap map);
خارجی errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_انتہا,
					      __u32 new_real_انتہا,
					      ext2fs_generic_bitmap bmap);
خارجی errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
خارجی errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 آغاز, __u32 عدد,
						 خالی *برآمد);
خارجی errcode_t ext2fs_سیٹ_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 آغاز, __u32 عدد,
						 خالی *اندر);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 آغاز, __u32 انتہا,
						       __u32 *برآمد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 آغاز, __u32 انتہا,
						       __u32 *برآمد);

/* gen_bitmap64.c */
خالی ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحیح type, __u64 آغاز, __u64 انتہا,
				    __u64 real_انتہا,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
خالی ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_انتہا(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 انتہا, __u64 *oend);
خالی ext2fs_سیٹ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_انتہا,
				     __u64 new_real_انتہا);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد);
errcode_t ext2fs_سیٹ_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاک_bitmap *bitmap);
errcode_t ext2fs_گنتی_مستعمل_clusters(ext2_filsys fs, blk64_t آغاز,
				     blk64_t انتہا, blk64_t *برآمد);
errcode_t ext2fs_گنتی_مستعمل_بلاکیں(ext2_filsys fs, blk64_t آغاز,
				   blk64_t انتہا, blk64_t *برآمد);
خارجی غیرعلامتی صحیح ext2fs_list_backups(ext2_filsys fs, غیرعلامتی صحیح *three,
				غیرعلامتی صحیح *five, غیرعلامتی صحیح *seven);

/* getenv.c */
خارجی حرف *ext2fs_محفوظ_getenv(ثابت حرف *arg);

/* get_num_dirs.c */
خارجی errcode_t ext2fs_get_عدد_dirs(ext2_filsys fs, ext2_ino_t *ret_عدد_dirs);

/* getsize.c */
خارجی errcode_t ext2fs_get_device_سائز(ثابت حرف *file, صحیح blocksize,
					blk_t *retblocks);
خارجی errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحیح blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
خارجی صحیح ext2fs_get_dio_alignment(صحیح fd);
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحیح *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحیح *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_جوڑو_بلاکیں(ext2_filsys fs, ساخت ext2_inode *inode,
				 blk64_t عدد_بلاکیں);
errcode_t ext2fs_iblk_sub_بلاکیں(ext2_filsys fs, ساخت ext2_inode *inode,
				 blk64_t عدد_بلاکیں);
errcode_t ext2fs_iblk_سیٹ(ext2_filsys fs, ساخت ext2_inode *inode, blk64_t b);

/* imager.c */
خارجی errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحیح fd, صحیح flags);
خارجی errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحیح fd, صحیح flags);
خارجی errcode_t ext2fs_image_super_write(ext2_filsys fs, صحیح fd, صحیح flags);
خارجی errcode_t ext2fs_image_super_read(ext2_filsys fs, صحیح fd, صحیح flags);
خارجی errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحیح fd, صحیح flags);
خارجی errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحیح fd, صحیح flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_بلاک(ext2_filsys fs, blk_t blk, خالی *buf);
errcode_t ext2fs_write_ind_بلاک(ext2_filsys fs, blk_t blk, خالی *buf);

/* initialize.c */
خارجی errcode_t ext2fs_initialize(ثابت حرف *name, صحیح flags,
				   ساخت ext2_super_بلاک *param,
				   io_manager manager, ext2_filsys *ret_fs);
خارجی errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحیح super_only);

/* icount.c */
خارجی خالی ext2fs_free_icount(ext2_icount_t icount);
خارجی errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, حرف *tdb_dir,
					  صحیح flags, ext2_icount_t *ret);
خارجی errcode_t ext2fs_create_icount2(ext2_filsys fs, صحیح flags,
				       غیرعلامتی صحیح سائز,
				       ext2_icount_t hint, ext2_icount_t *ret);
خارجی errcode_t ext2fs_create_icount(ext2_filsys fs, صحیح flags,
				      غیرعلامتی صحیح سائز,
				      ext2_icount_t *ret);
خارجی errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
خارجی errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجی errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجی errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 گنتی);
خارجی ext2_ino_t ext2fs_get_icount_سائز(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

خارجی errcode_t ext2fs_get_memalign(غیرعلامتی طویل سائز,
				     غیرعلامتی طویل align, خالی *ptr);

/* inline_data.c */
خارجی errcode_t ext2fs_inline_ڈیٹا_init(ext2_filsys fs, ext2_ino_t ino);
خارجی errcode_t ext2fs_inline_ڈیٹا_سائز(ext2_filsys fs, ext2_ino_t ino,
					 size_t *سائز);
خارجی errcode_t ext2fs_inline_ڈیٹا_get(ext2_filsys fs, ext2_ino_t ino,
					ساخت ext2_inode *inode,
					خالی *buf, size_t *سائز);
خارجی errcode_t ext2fs_inline_ڈیٹا_سیٹ(ext2_filsys fs, ext2_ino_t ino,
					ساخت ext2_inode *inode,
					خالی *buf, size_t سائز);

/* inode.c */
خارجی errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   غیرعلامتی صحیح cache_سائز);
خارجی خالی ext2fs_free_inode_cache(ساخت ext2_inode_cache *icache);
خارجی errcode_t ext2fs_flush_icache(ext2_filsys fs);
خارجی errcode_t ext2fs_get_اگلا_inode_بھرا(ext2_inode_جائزہ جائزہ,
					    ext2_ino_t *ino,
					    ساخت ext2_inode *inode,
					    صحیح bufsize);
#تعریف_کرو EXT2_INODE_جائزہ_DEFAULT_بفر_بلاکیں	8
خارجی errcode_t ext2fs_open_inode_جائزہ(ext2_filsys fs, صحیح بفر_بلاکیں,
				  ext2_inode_جائزہ *ret_جائزہ);
خارجی خالی ext2fs_close_inode_جائزہ(ext2_inode_جائزہ جائزہ);
خارجی errcode_t ext2fs_get_اگلا_inode(ext2_inode_جائزہ جائزہ, ext2_ino_t *ino,
			       ساخت ext2_inode *inode);
خارجی errcode_t ext2fs_inode_جائزہ_goto_blockgroup(ext2_inode_جائزہ جائزہ,
						   صحیح	گروہ);
خارجی خالی ext2fs_سیٹ_inode_callback
	(ext2_inode_جائزہ جائزہ,
	 errcode_t (*مکمل_گروہ)(ext2_filsys fs,
				 ext2_inode_جائزہ جائزہ,
				 dgrp_t گروہ,
				 خالی * priv_ڈیٹا),
	 خالی *مکمل_گروہ_ڈیٹا);
خارجی صحیح ext2fs_inode_جائزہ_flags(ext2_inode_جائزہ جائزہ, صحیح سیٹ_flags,
				   صحیح clear_flags);
خارجی errcode_t ext2fs_read_inode_بھرا(ext2_filsys fs, ext2_ino_t ino,
					ساخت ext2_inode * inode,
					صحیح bufsize);
خارجی errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساخت ext2_inode * inode);
خارجی errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ساخت ext2_inode * inode,
				    صحیح bufsize, صحیح flags);
خارجی errcode_t ext2fs_write_inode_بھرا(ext2_filsys fs, ext2_ino_t ino,
					 ساخت ext2_inode * inode,
					 صحیح bufsize);
خارجی errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساخت ext2_inode * inode);
خارجی errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ساخت ext2_inode * inode,
				     صحیح bufsize, صحیح flags);
خارجی errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساخت ext2_inode * inode);
خارجی errcode_t ext2fs_get_بلاکیں(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکیں);
خارجی errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
خارجی io_manager inode_io_manager;
خارجی errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					حرف **name);
خارجی errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ساخت ext2_inode *inode,
					 حرف **name);

/* ismounted.c */
خارجی errcode_t ext2fs_check_if_mounted(ثابت حرف *file, صحیح *mount_flags);
خارجی errcode_t ext2fs_check_mount_نقطہ(ثابت حرف *device, صحیح *mount_flags,
					  حرف *mtpt, صحیح mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
خارجی errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ساخت ext2_inode *inode,
			      حرف *بلاک_buf, blk64_t آغاز,
			      blk64_t انتہا);

/* namei.c */
خارجی errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			 صحیح namelen, حرف *buf, ext2_ino_t *inode);
خارجی errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ثابت حرف *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode);
خارجی errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
صحیح ext2fs_native_flag(خالی);

/* newdir.c */
خارجی errcode_t ext2fs_new_dir_بلاک(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t والد_ino, حرف **بلاک);
خارجی errcode_t ext2fs_new_dir_inline_ڈیٹا(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t والد_ino, __u32 *iblock);

/* nls_utf8.c */
خارجی ثابت ساخت ext2fs_nls_جدول *ext2fs_بھرو_nls_جدول(صحیح encoding);
خارجی صحیح ext2fs_check_encoded_name(ثابت ساخت ext2fs_nls_جدول *جدول,
				     حرف *s, size_t len, حرف **pos);
خارجی صحیح ext2fs_casefold_cmp(ثابت ساخت ext2fs_nls_جدول *جدول,
			       ثابت غیرعلامتی حرف *str1, size_t len1,
			       ثابت غیرعلامتی حرف *str2, size_t len2);

/* mkdir.c */
خارجی errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t inum,
			      ثابت حرف *name);
خارجی errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t والد,
			       ext2_ino_t ino, غیرعلامتی طویل flags,
			       صحیح link_flags, ثابت حرف *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ساخت ext2fs_journal_params {
	blk_t عدد_journal_بلاکیں;
	blk_t عدد_fc_بلاکیں;
};
خارجی errcode_t ext2fs_get_journal_params(
		ساخت ext2fs_journal_params *params, ext2_filsys fs);
خارجی errcode_t ext2fs_zero_بلاکیں(ext2_filsys fs, blk_t blk, صحیح عدد,
				    blk_t *ret_blk, صحیح *ret_گنتی);
خارجی errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحیح عدد,
				     blk64_t *ret_blk, صحیح *ret_گنتی);
خارجی errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 عدد_بلاکیں, صحیح flags,
						  حرف  **ret_jsb);
خارجی errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ساخت ext2fs_journal_params *params,
						  صحیح flags, حرف  **ret_jsb);
خارجی errcode_t ext2fs_جوڑو_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
خارجی errcode_t ext2fs_جوڑو_journal_inode(ext2_filsys fs, blk_t عدد_بلاکیں,
					  صحیح flags);
خارجی errcode_t ext2fs_جوڑو_journal_inode2(ext2_filsys fs, blk_t عدد_بلاکیں,
					   blk64_t goal, صحیح flags);
خارجی errcode_t ext2fs_جوڑو_journal_inode3(ext2_filsys fs,
				    ساخت ext2fs_journal_params *params,
				    blk64_t goal, صحیح flags);
خارجی صحیح ext2fs_default_journal_سائز(__u64 عدد_بلاکیں);
خارجی صحیح ext2fs_journal_sb_آغاز(صحیح blocksize);

/* openfs.c */
خارجی errcode_t ext2fs_open(ثابت حرف *name, صحیح flags, صحیح superblock,
			     غیرعلامتی صحیح بلاک_سائز, io_manager manager,
			     ext2_filsys *ret_fs);
خارجی errcode_t ext2fs_open2(ثابت حرف *name, ثابت حرف *io_options,
			      صحیح flags, صحیح superblock,
			      غیرعلامتی صحیح بلاک_سائز, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
خارجی blk64_t ext2fs_descriptor_بلاک_loc2(ext2_filsys fs,
					blk64_t گروہ_بلاک, dgrp_t i);
خارجی blk_t ext2fs_descriptor_بلاک_loc(ext2_filsys fs, blk_t گروہ_بلاک,
					 dgrp_t i);
errcode_t ext2fs_get_ڈیٹا_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_سیٹ_ڈیٹا_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_مقصد_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
خارجی errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t عدد_بلاکیں);
خارجی errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
خارجی e2_blkcnt_t ext2fs_default_orphan_file_بلاکیں(ext2_filsys fs);
خارجی __u32 ext2fs_do_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      حرف *buf);
خارجی errcode_t ext2fs_orphan_file_بلاک_csum_سیٹ(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   حرف *buf);
خارجی صحیح ext2fs_orphan_file_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, حرف *buf);

/* get_pathname.c */
خارجی errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       حرف **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
		      ext2_ino_t ino, صحیح flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			ext2_ino_t ino, صحیح flags);
صحیح ext2fs_dir_is_dx(ext2_filsys fs, ثابت ساخت ext2_inode *inode);
خالی ext2fs_inc_nlink(ext2_filsys fs, ساخت ext2_inode *inode);
خالی ext2fs_dec_nlink(ساخت ext2_inode *inode);
صحیح ext2fs_dir_link_max(ext2_filsys fs, ساخت ext2_inode_large *inode);
صحیح ext2fs_dir_link_خالی_خانے(ساخت ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *ہدف);
صحیح ext2fs_is_fast_symlink(ساخت ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, خالی *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, خالی *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_آغاز(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحیح immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
غیرعلامتی ext2fs_mmp_new_seq(خالی);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, خالی **ptr);

/* read_bb.c */
خارجی errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
خارجی errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      خالی *priv_ڈیٹا,
				      خالی (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      حرف *badstr,
						      خالی *priv_ڈیٹا));
خارجی errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     خالی (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
خارجی errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
خارجی errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحیح flags, صحیح عدد_دھاگے);
خارجی errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
خارجی errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
خارجی errcode_t ext2fs_read_بلاک_bitmap(ext2_filsys fs);
خارجی errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
خارجی errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
خارجی errcode_t ext2fs_write_بلاک_bitmap (ext2_filsys fs);

/*sha256.c */
#تعریف_کرو EXT2_FS_SHA256_لمبائی 32
#اگر 0
خارجی خالی ext2fs_sha256(ثابت غیرعلامتی حرف *اندر, غیرعلامتی طویل اندر_سائز,
		   غیرعلامتی حرف برآمد[EXT2_FS_SHA256_لمبائی]);
#اگر_ختم

/* sha512.c */
#تعریف_کرو EXT2_FS_SHA512_لمبائی 64
خارجی خالی ext2fs_sha512(ثابت غیرعلامتی حرف *اندر, غیرعلامتی طویل اندر_سائز,
			  غیرعلامتی حرف برآمد[EXT2_FS_SHA512_لمبائی]);

/* swapfs.c */
خارجی errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf, size_t سائز,
					صحیح flags);
خارجی errcode_t ext2fs_dirent_swab_اندر(ext2_filsys fs, حرف *buf, صحیح flags);
خارجی errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf, size_t سائز,
					 صحیح flags);
خارجی errcode_t ext2fs_dirent_swab_برآمد(ext2_filsys fs, حرف *buf, صحیح flags);
خارجی خالی ext2fs_تبادلہ_ext_attr(حرف *مقصد, حرف *from, صحیح bufsize,
				 صحیح has_سرخی);
خارجی خالی ext2fs_تبادلہ_ext_attr_سرخی(ساخت ext2_ext_attr_سرخی *مقصد_سرخی,
					ساخت ext2_ext_attr_سرخی *from_hdr);
خارجی خالی ext2fs_تبادلہ_ext_attr_اندراج(ساخت ext2_ext_attr_اندراج *مقصد_اندراج,
				       ساخت ext2_ext_attr_اندراج *from_اندراج);
خارجی خالی ext2fs_تبادلہ_super(ساخت ext2_super_بلاک * super);
خارجی خالی ext2fs_تبادلہ_گروہ_desc(ساخت ext2_گروہ_desc *gdp);
خارجی خالی ext2fs_تبادلہ_گروہ_desc2(ext2_filsys, ساخت ext2_گروہ_desc *gdp);
خارجی خالی ext2fs_تبادلہ_inode_بھرا(ext2_filsys fs, ساخت ext2_inode_large *t,
				   ساخت ext2_inode_large *f, صحیح hostorder,
				   صحیح bufsize);
خارجی خالی ext2fs_تبادلہ_inode(ext2_filsys fs,ساخت ext2_inode *t,
			      ساخت ext2_inode *f, صحیح hostorder);
خارجی خالی ext2fs_تبادلہ_mmp(ساخت mmp_struct *mmp);

/* unix_io.c */
خارجی صحیح ext2fs_open_file(ثابت حرف *pathname, صحیح flags, mode_t منوال);
خارجی صحیح ext2fs_stat(ثابت حرف *راستہ, ext2fs_struct_stat *buf);
خارجی صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
خارجی صحیح ext2fs_inode_has_valid_بلاکیں(ساخت ext2_inode *inode);
خارجی صحیح ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ساخت ext2_inode *inode);

/* version.c */
خارجی صحیح ext2fs_parse_نسخہ_string(ثابت حرف *ver_string);
خارجی صحیح ext2fs_get_library_نسخہ(ثابت حرف **ver_string,
				      ثابت حرف **date_string);

/* write_bb_file.c */
خارجی errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      غیرعلامتی صحیح flags,
				      FILE *f);


/* inline functions */
#اگر_تعریف_شدہ NO_INLINE_FUNCS
خارجی errcode_t ext2fs_get_mem(غیرعلامتی طویل سائز, خالی *ptr);
خارجی errcode_t ext2fs_get_memzero(غیرعلامتی طویل سائز, خالی *ptr);
خارجی errcode_t ext2fs_get_array(غیرعلامتی طویل گنتی,
				  غیرعلامتی طویل سائز, خالی *ptr);
خارجی errcode_t ext2fs_get_arrayzero(غیرعلامتی طویل گنتی,
				      غیرعلامتی طویل سائز, خالی *ptr);
خارجی errcode_t ext2fs_free_mem(خالی *ptr);
خارجی errcode_t ext2fs_resize_mem(غیرعلامتی طویل old_سائز,
				   غیرعلامتی طویل سائز, خالی *ptr);
خارجی errcode_t ext2fs_resize_array(غیرعلامتی طویل old_گنتی, غیرعلامتی طویل گنتی,
				     غیرعلامتی طویل سائز, خالی *ptr);
خارجی خالی ext2fs_mark_super_dirty(ext2_filsys fs);
خارجی خالی ext2fs_mark_بدلا(ext2_filsys fs);
خارجی صحیح ext2fs_test_بدلا(ext2_filsys fs);
خارجی خالی ext2fs_mark_valid(ext2_filsys fs);
خارجی خالی ext2fs_unmark_valid(ext2_filsys fs);
خارجی صحیح ext2fs_test_valid(ext2_filsys fs);
خارجی خالی ext2fs_mark_ib_dirty(ext2_filsys fs);
خارجی خالی ext2fs_mark_bb_dirty(ext2_filsys fs);
خارجی صحیح ext2fs_test_ib_dirty(ext2_filsys fs);
خارجی صحیح ext2fs_test_bb_dirty(ext2_filsys fs);
خارجی dgrp_t ext2fs_گروہ_کا_blk(ext2_filsys fs, blk_t blk);
خارجی dgrp_t ext2fs_گروہ_کا_ino(ext2_filsys fs, ext2_ino_t ino);
خارجی blk_t ext2fs_گروہ_پہلا_بلاک(ext2_filsys fs, dgrp_t گروہ);
خارجی blk_t ext2fs_گروہ_آخری_بلاک(ext2_filsys fs, dgrp_t گروہ);
خارجی blk_t ext2fs_inode_ڈیٹا_بلاکیں(ext2_filsys fs,
				      ساخت ext2_inode *inode);
خارجی صحیح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحیح بلاکیں);
خارجی صحیح ext2fs_log2_u32(__u32 arg);
خارجی صحیح ext2fs_log2_u64(__u64 arg);
خارجی صحیح ext2fs_log10_u32(__u32 arg);
خارجی صحیح ext2fs_log10_u64(__u64 arg);
خارجی غیرعلامتی صحیح ext2fs_div_ceil(غیرعلامتی صحیح a, غیرعلامتی صحیح b);
خارجی __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
خارجی صحیح ext2fs_dirent_name_len(ثابت ساخت ext2_dir_اندراج *اندراج_2);
خارجی خالی ext2fs_dirent_سیٹ_name_len(ساخت ext2_dir_اندراج *اندراج_2, صحیح len);
خارجی صحیح ext2fs_dirent_file_type(ثابت ساخت ext2_dir_اندراج *اندراج_2);
خارجی خالی ext2fs_dirent_سیٹ_file_type(ساخت ext2_dir_اندراج *اندراج_2, صحیح type);
خارجی ساخت ext2_inode *ext2fs_inode(ساخت ext2_inode_large * large_inode);
خارجی ثابت ساخت ext2_inode *ext2fs_const_inode(ثابت ساخت ext2_inode_large * large_inode);
خارجی صحیح ext2fs_inodes_فی_orphan_بلاک(ext2_filsys fs);
خارجی ساخت ext4_orphan_بلاک_دم *ext2fs_orphan_بلاک_دم(ext2_filsys fs,
							       حرف *buf);
#اگر_ختم

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#اگر (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#اگر_تعریف_شدہ INCLUDE_INLINE_FUNCS
#تعریف_کرو _INLINE_ خارجی
#ورنہ
#اگر (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ اندرونی
#ورنہ
#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو _INLINE_ خارجی __inline__
#ورنہ				/* For Watcom C */
#تعریف_کرو _INLINE_ خارجی اندرونی
#اگر_ختم /* __GNUC__ */
#اگر_ختم /* __STDC_VERSION__ >= 199901L */
#اگر_ختم

#اگر_غیر_تعریف_شدہ EXT2_CUSTOM_حافظہ_ROUTINES
#شامل <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(غیرعلامتی طویل سائز, خالی *ptr)
{
	خالی *pp;

	pp = حافظہ_دو(سائز);
	اگر (!pp)
		واپس EXT2_ET_NO_حافظہ;
	حافظہ_نقل(ptr, &pp, حجم (pp));
	واپس 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(غیرعلامتی طویل سائز, خالی *ptr)
{
	خالی *pp;

	pp = حافظہ_دو(سائز);
	اگر (!pp)
		واپس EXT2_ET_NO_حافظہ;
	memset(pp, 0, سائز);
	حافظہ_نقل(ptr, &pp, حجم(pp));
	واپس 0;
}

_INLINE_ errcode_t ext2fs_get_array(غیرعلامتی طویل گنتی, غیرعلامتی طویل سائز,
				    خالی *ptr)
{
	اگر (گنتی && (~0UL)/گنتی < سائز)
		واپس EXT2_ET_NO_حافظہ;
	واپس ext2fs_get_mem(گنتی*سائز, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(غیرعلامتی طویل گنتی,
					غیرعلامتی طویل سائز, خالی *ptr)
{
	اگر (گنتی && (~0UL)/گنتی < سائز)
		واپس EXT2_ET_NO_حافظہ;

	واپس ext2fs_get_memzero((size_t)گنتی * سائز, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(خالی *ptr)
{
	خالی *p;

	حافظہ_نقل(&p, ptr, حجم(p));
	آزاد(p);
	p = 0;
	حافظہ_نقل(ptr, &p, حجم(p));
	واپس 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(غیرعلامتی طویل EXT2FS_ATTR((غیر_مستعمل)) old_سائز,
				     غیرعلامتی طویل سائز, خالی *ptr)
{
	خالی *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	حافظہ_نقل(&p, ptr, حجم(p));
	p = realloc(p, سائز);
	اگر (!p)
		واپس EXT2_ET_NO_حافظہ;
	حافظہ_نقل(ptr, &p, حجم(p));
	واپس 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(غیرعلامتی طویل سائز,
				       غیرعلامتی طویل old_گنتی,
				       غیرعلامتی طویل گنتی, خالی *ptr)
{
	غیرعلامتی طویل old_سائز;
	errcode_t retval;

	اگر (گنتی && (~0UL)/گنتی < سائز)
		واپس EXT2_ET_NO_حافظہ;

	سائز *= گنتی;
	old_سائز = سائز * old_گنتی;
	retval = ext2fs_resize_mem(old_سائز, سائز, ptr);
	اگر (retval)
		واپس retval;

	اگر (سائز > old_سائز) {
		خالی *p;

		حافظہ_نقل(&p, ptr, حجم(p));
		memset((حرف *)p + old_سائز, 0, سائز - old_سائز);
		حافظہ_نقل(ptr, &p, حجم(p));
	}

	واپس 0;
}
#اگر_ختم	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ خالی ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_بدلا;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ خالی ext2fs_mark_بدلا(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_بدلا;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ صحیح ext2fs_test_بدلا(ext2_filsys fs)
{
	واپس (fs->flags & EXT2_FLAG_بدلا);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ خالی ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ خالی ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ صحیح ext2fs_test_valid(ext2_filsys fs)
{
	واپس (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ خالی ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_بدلا;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ خالی ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_بدلا;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ صحیح ext2fs_test_ib_dirty(ext2_filsys fs)
{
	واپس (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ صحیح ext2fs_test_bb_dirty(ext2_filsys fs)
{
	واپس (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_گروہ_کا_blk(ext2_filsys fs, blk_t blk)
{
	واپس ext2fs_گروہ_کا_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_گروہ_کا_ino(ext2_filsys fs, ext2_ino_t ino)
{
	واپس (ino - 1) / fs->super->s_inodes_فی_گروہ;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروہ_پہلا_بلاک(ext2_filsys fs, dgrp_t گروہ)
{
	واپس (blk_t) ext2fs_گروہ_پہلا_block2(fs, گروہ);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_گروہ_آخری_بلاک(ext2_filsys fs, dgrp_t گروہ)
{
	واپس (blk_t) ext2fs_گروہ_آخری_block2(fs, گروہ);
}

_INLINE_ blk_t ext2fs_inode_ڈیٹا_بلاکیں(ext2_filsys fs,
					ساخت ext2_inode *inode)
{
	واپس (blk_t) ext2fs_inode_ڈیٹا_blocks2(fs, inode);
}

_INLINE_ صحیح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحیح بلاکیں)
{
	صحیح csum_سائز = 0;

	اگر ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_سائز = حجم(ساخت ext2_dx_دم);
	واپس بلاکیں * ((fs->blocksize - (8 + csum_سائز)) /
						حجم(ساخت ext2_dx_اندراج));
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحیح ext2fs_log2_u32(__u32 arg)
{
	صحیح l = 0;

	arg >>= 1;
	جب (arg) {
		l++;
		arg >>= 1;
	}
	واپس l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحیح ext2fs_log2_u64(__u64 arg)
{
	صحیح l = 0;

	arg >>= 1;
	جب (arg) {
		l++;
		arg >>= 1;
	}
	واپس l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحیح ext2fs_log10_u32(__u32 arg)
{
	صحیح l = 0;

	arg /= 10;
	جب (arg) {
		l++;
		arg /= 10;
	}
	واپس l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحیح ext2fs_log10_u64(__u64 arg)
{
	صحیح l = 0;

	arg /= 10;
	جب (arg) {
		l++;
		arg /= 10;
	}
	واپس l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ غیرعلامتی صحیح ext2fs_div_ceil(غیرعلامتی صحیح a, غیرعلامتی صحیح b)
{
	اگر (!a)
		واپس 0;
	واپس ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	اگر (!a)
		واپس 0;
	واپس ((a - 1) / b) + 1;
}

_INLINE_ صحیح ext2fs_dirent_name_len(ثابت ساخت ext2_dir_اندراج *اندراج_2)
{
	واپس اندراج_2->name_len & 0xff;
}

_INLINE_ خالی ext2fs_dirent_سیٹ_name_len(ساخت ext2_dir_اندراج *اندراج_2, صحیح len)
{
	اندراج_2->name_len = (اندراج_2->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ صحیح ext2fs_dirent_file_type(ثابت ساخت ext2_dir_اندراج *اندراج_2)
{
	واپس اندراج_2->name_len >> 8;
}

_INLINE_ خالی ext2fs_dirent_سیٹ_file_type(ساخت ext2_dir_اندراج *اندراج_2, صحیح type)
{
	اندراج_2->name_len = (اندراج_2->name_len & 0xff) | (type << 8);
}

_INLINE_ ساخت ext2_inode *ext2fs_inode(ساخت ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	واپس (ساخت ext2_inode *) large_inode;
}

_INLINE_ ثابت ساخت ext2_inode *
ext2fs_const_inode(ثابت ساخت ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	واپس (ثابت ساخت ext2_inode *) large_inode;
}

_INLINE_ صحیح ext2fs_inodes_فی_orphan_بلاک(ext2_filsys fs)
{
	واپس (fs->blocksize - حجم(ساخت ext4_orphan_بلاک_دم)) /
		حجم(__u32);
}

_INLINE_ ساخت ext4_orphan_بلاک_دم *
ext2fs_orphan_بلاک_دم(ext2_filsys fs, حرف *buf)
{
	واپس (ساخت ext4_orphan_بلاک_دم *)(buf + fs->blocksize -
		حجم(ساخت ext4_orphan_بلاک_دم));
}

#تعریف_منسوخ _INLINE_
#اگر_ختم

/* htree levels for ext4 */
#تعریف_کرو EXT4_HTREE_LEVEL_COMPAT 2
#تعریف_کرو EXT4_HTREE_LEVEL	3

ساکن اندرونی غیرعلامتی صحیح ext2_dir_htree_level(ext2_filsys fs)
{
	اگر (ext2fs_has_feature_largedir(fs->super))
		واپس EXT4_HTREE_LEVEL;

	واپس EXT4_HTREE_LEVEL_COMPAT;
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
ساکن اندرونی خالی ext2fs_سیٹ_dtime(ext2_filsys fs, ساخت ext2_inode *inode)
{
	__u32	t;

	اگر (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_وقت))
		t = fs->now & 0xFFFFFFFF;
	ورنہ
		t = time(تہی) & 0xFFFFFFFF;
	اگر (t < fs->super->s_inodes_گنتی)
		t = fs->super->s_inodes_گنتی;
	inode->i_dtime = t;
}

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم /* _EXT2FS_EXT2FS_H */
