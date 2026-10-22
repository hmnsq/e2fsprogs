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
#অন্যথা
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#যদি __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#অন্যথা
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#অন্যথা
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#অন্যথা
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
বাহ্যিক "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#যদি (!defined(__GNUC__) && !defined(__WATCOMC__))
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
#define SUPERBLOCK_আকার		1024

#define UUID_STR_আকার 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#define EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#ifdef HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <errno.h>

#যদি EXT2_FLAT_INCLUDES
#অন্তর্ভাব "e2_types.h"
#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext3_extents.h"
#অন্যথা
#অন্তর্ভাব <ext2fs/ext2_types.h>
#অন্তর্ভাব <ext2fs/ext2_fs.h>
#অন্তর্ভাব <ext2fs/ext3_extents.h>
#endif /* EXT2_FLAT_INCLUDES */

প্রকারলেখ __u32 __bitwise		ext2_ino_t;
প্রকারলেখ __u32 __bitwise		blk_t;
প্রকারলেখ __u64 __bitwise		blk64_t;
প্রকারলেখ __u32 __bitwise		dgrp_t;
প্রকারলেখ __s32 __bitwise		ext2_off_t;
প্রকারলেখ __s64 __bitwise		ext2_off64_t;
প্রকারলেখ __s64 __bitwise		e2_blkcnt_t;
প্রকারলেখ __u32 __bitwise		ext2_dirhash_t;

#যদি EXT2_FLAT_INCLUDES
#অন্তর্ভাব "com_err.h"
#অন্তর্ভাব "ext2_io.h"
#অন্তর্ভাব "ext2_err.h"
#অন্তর্ভাব "ext2_ext_attr.h"
#অন্যথা
#অন্তর্ভাব <et/com_err.h>
#অন্তর্ভাব <ext2fs/ext2_io.h>
#অন্তর্ভাব <ext2fs/ext2_err.h>
#অন্তর্ভাব <ext2fs/ext2_ext_attr.h>
#endif

#অন্তর্ভাব "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE পূর্ণসংখ্যা __cdecl
#অন্যথা
#define EXT2_QSORT_TYPE পূর্ণসংখ্যা
#endif

প্রকারলেখ কাঠামো struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

কাঠামো ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

প্রকারলেখ কাঠামো ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
প্রকারলেখ কাঠামো ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
প্রকারলেখ কাঠামো ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

প্রকারলেখ কাঠামো ext2_struct_u32_list *ext2_badblocks_list;
প্রকারলেখ কাঠামো ext2_struct_u32_iterate *ext2_badblocks_iterate;

প্রকারলেখ কাঠামো ext2_struct_u32_list *ext2_u32_list;
প্রকারলেখ কাঠামো ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
প্রকারলেখ কাঠামো ext2_struct_u32_list *badblocks_list;
প্রকারলেখ কাঠামো ext2_struct_u32_iterate *badblocks_iterate;

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
কাঠামো ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
কাঠামো ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	পূর্ণসংখ্যা	blockcnt;
};

প্রকারলেখ কাঠামো ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

প্রকারলেখ কাঠামো ext2_file *ext2_file_t;

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
#define EXT2_FLAG_BBITMAP_লেজ_PROBLEM	0x1000000
#define EXT2_FLAG_IBITMAP_লেজ_PROBLEM	0x2000000
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

কাঠামো blk_alloc_ctx;
কাঠামো opaque_ext2_group_desc;

কাঠামো struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	পূর্ণসংখ্যা				flags;
	অক্ষর *				device_name;
	কাঠামো ext2_super_block	* 	super;
	unsigned পূর্ণসংখ্যা			blocksize;
	পূর্ণসংখ্যা				fragsize;
	dgrp_t				group_desc_গুনতি;
	unsigned long			desc_blocks;
	কাঠামো opaque_ext2_group_desc *	group_desc;
	unsigned পূর্ণসংখ্যা			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				কাঠামো ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				কাঠামো ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	কাঠামো ext2_super_block *	orig_super;
	কাঠামো ext2_image_hdr *		image_header;
	__u32				umask;
	time_t				now;
	পূর্ণসংখ্যা				cluster_ratio_bits;
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
	শূন্য *				priv_তথ্য;

	/*
	 * Inode cache
	 */
	কাঠামো ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, কাঠামো blk_alloc_ctx *ctx);
	শূন্য (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, পূর্ণসংখ্যা inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	শূন্য *mmp_buf;
	শূন্য *mmp_cmp;
	পূর্ণসংখ্যা mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* progress operation functions */
	কাঠামো ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	অক্ষর				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	শূন্য (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t অঙ্ক,
					পূর্ণসংখ্যা inuse);

	/* hashmap for SHA of data blocks */
	কাঠামো ext2fs_hashmap* block_sha_map;

	ধ্রুবক কাঠামো ext2fs_nls_table *encoding;
};

#যদি EXT2_FLAT_INCLUDES
#অন্তর্ভাব "e2_bitops.h"
#অন্যথা
#অন্তর্ভাব <ext2fs/bitops.h>
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
#define BLOCK_INLINE_তথ্য_CHANGED	8

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
#define BLOCK_FLAG_তথ্য_ONLY	4
#define BLOCK_FLAG_READ_ONLY	8

#define BLOCK_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#define BLOCK_গুনতি_IND		(-1)
#define BLOCK_গুনতি_DIND	(-2)
#define BLOCK_গুনতি_TIND	(-3)
#define BLOCK_গুনতি_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_তথ্য	1
#define BLOCK_ALLOC_METADATA	2

কাঠামো blk_alloc_ctx {
	ext2_ino_t		ino;
	কাঠামো ext2_inode	*inode;
	blk64_t			lblk;
	পূর্ণসংখ্যা			flags;
};

#যদি 0
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

কাঠামো ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

প্রকারলেখ কাঠামো ext2_extent_handle *ext2_extent_handle_t;
প্রকারলেখ কাঠামো ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_মূল	0x0001
#define EXT2_EXTENT_LAST_LEAF	0x0002
#define EXT2_EXTENT_FIRST_SIB	0x0003
#define EXT2_EXTENT_LAST_SIB	0x0004
#define EXT2_EXTENT_পরবর্তী_SIB	0x0005
#define EXT2_EXTENT_পূর্ব_SIB	0x0006
#define EXT2_EXTENT_পরবর্তী_LEAF	0x0007
#define EXT2_EXTENT_পূর্ব_LEAF	0x0008
#define EXT2_EXTENT_পরবর্তী	0x0009
#define EXT2_EXTENT_পূর্ব	0x000A
#define EXT2_EXTENT_UP		0x000B
#define EXT2_EXTENT_DOWN	0x000C
#define EXT2_EXTENT_DOWN_AND_LAST 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#define EXT2_EXTENT_ঢোকাও_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_ঢোকাও_NOSPLIT	0x0002 /* insert may not cause split */

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
কাঠামো ext2_extent_info {
	পূর্ণসংখ্যা		curr_entry;
	পূর্ণসংখ্যা		curr_level;
	পূর্ণসংখ্যা		অঙ্ক_entries;
	পূর্ণসংখ্যা		max_entries;
	পূর্ণসংখ্যা		max_depth;
	পূর্ণসংখ্যা		bytes_avail;
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
#define DIRENT_FLAG_INCLUDE_INLINE_তথ্য 8

#define DIRENT_DOT_FILE		1
#define DIRENT_DOT_DOT_FILE	2
#define DIRENT_OTHER_FILE	3
#define DIRENT_DELETED_FILE	4
#define DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
প্রকারলেখ কাঠামো ext2_struct_inode_scan *ext2_inode_scan;

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
#define EXT2_I_আকার(i)	((i)->i_আকার | ((__u64) (i)->i_আকার_উপরের << 32))

স্থির অন্তঃরেখ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#যদি (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	ফেরত extra | (nsec << EXT4_EPOCH_BITS);
}
স্থির অন্তঃরেখ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#যদি (SIZEOF_TIME_T > 4)
	যদি (extra & EXT4_EPOCH_MASK)
		seconds += ((time_t)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	ফেরত seconds;
}
স্থির অন্তঃরেখ __u32 __decode_extra_nsec(__u32 extra)
{
	ফেরত (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_আকার(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_আকার +				      \
		  (আকার(*inode) > EXT2_GOOD_OLD_INODE_আকার ?		      \
		   ((কাঠামো ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
করো {									      \
	যদি (ext2fs_inode_includes(ext2fs_inode_actual_আকার(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((কাঠামো ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} অন্যথা {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} যখন (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_আকার(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((কাঠামো ext2_inode_large *)(inode))->field ## _extra) :      \
		(time_t)(inode)->field)

স্থির অন্তঃরেখ শূন্য __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#যদি (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#অন্যথা
	*hi = 0;
#endif
}
স্থির অন্তঃরেখ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#যদি (SIZEOF_TIME_T == 4)
	ফেরত *lo;
#অন্যথা
	ফেরত ((time_t)(*hi) << 32) | *lo;
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

প্রকারলেখ কাঠামো ext2_icount *ext2_icount_t;

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

#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)

/*
 * Features supported by this version of the library
 */
#define EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_সূচক|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#ifdef CONFIG_MMP
#define EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#অন্যথা
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
					 EXT4_FEATURE_INCOMPAT_INLINE_তথ্য|\
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
#define EXT2_FS_অঙ্ক_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#যদি defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
প্রকারলেখ কাঠামো stat64 ext2fs_struct_stat;
#অন্যথা
প্রকারলেখ কাঠামো stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
কাঠামো ext2_xattr_handle;
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
স্থির অন্তঃরেখ পূর্ণসংখ্যা ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	ফেরত ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
স্থির অন্তঃরেখ পূর্ণসংখ্যা ext2fs_needs_large_file_feature(unsigned long long file_আকার)
{
	ফেরত file_আকার >= 0x80000000ULL;
}

/* alloc.c */
বাহ্যিক শূন্য ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
বাহ্যিক errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, পূর্ণসংখ্যা mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
বাহ্যিক errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
বাহ্যিক errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
বাহ্যিক errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   কাঠামো blk_alloc_ctx *ctx);
বাহ্যিক errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, পূর্ণসংখ্যা অঙ্ক,
					ext2fs_block_bitmap map,
					blk_t *ret);
বাহ্যিক errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, পূর্ণসংখ্যা অঙ্ক,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
বাহ্যিক errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    অক্ষর *block_buf, blk_t *ret);
বাহ্যিক errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     অক্ষর *block_buf, blk64_t *ret);
বাহ্যিক errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     অক্ষর *block_buf, blk64_t *ret,
				     কাঠামো blk_alloc_ctx *ctx);

বাহ্যিক শূন্য ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       কাঠামো ext2_inode *inode, blk64_t lblk);
বাহ্যিক শূন্য ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
বাহ্যিক শূন্য ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	শূন্য (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণসংখ্যা inuse),
	শূন্য (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণসংখ্যা inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_দৈর্ঘ্য	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
বাহ্যিক পূর্ণসংখ্যা ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
বাহ্যিক errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
বাহ্যিক শূন্য ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
						  শূন্য (*func)(ext2_filsys fs,
							       blk64_t blk,
							       পূর্ণসংখ্যা inuse),
						  শূন্য (**old)(ext2_filsys fs,
							       blk64_t blk,
							       পূর্ণসংখ্যা inuse));

/* alloc_stats.c */
শূন্য ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, পূর্ণসংখ্যা inuse);
শূন্য ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       পূর্ণসংখ্যা inuse, পূর্ণসংখ্যা isdir);
শূন্য ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, পূর্ণসংখ্যা inuse);
শূন্য ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, পূর্ণসংখ্যা inuse);
শূন্য ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণসংখ্যা inuse);

/* alloc_tables.c */
বাহ্যিক errcode_t ext2fs_allocate_tables(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
বাহ্যিক errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
বাহ্যিক errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
বাহ্যিক শূন্য ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
বাহ্যিক errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

বাহ্যিক errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
					   blk_t blk);
বাহ্যিক পূর্ণসংখ্যা ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
বাহ্যিক শূন্য ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
বাহ্যিক errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
বাহ্যিক পূর্ণসংখ্যা ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
বাহ্যিক শূন্য ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
বাহ্যিক errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
বাহ্যিক পূর্ণসংখ্যা ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
বাহ্যিক পূর্ণসংখ্যা ext2fs_u32_list_গুনতি(ext2_u32_list bb);

/* bb_compat */
বাহ্যিক errcode_t badblocks_list_create(badblocks_list *ret, পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
বাহ্যিক পূর্ণসংখ্যা badblocks_list_test(badblocks_list bb, blk_t blk);
বাহ্যিক errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
বাহ্যিক পূর্ণসংখ্যা badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
বাহ্যিক শূন্য badblocks_list_iterate_end(badblocks_iterate iter);
বাহ্যিক শূন্য badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
বাহ্যিক errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
বাহ্যিক শূন্য ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
বাহ্যিক শূন্য ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
বাহ্যিক errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
বাহ্যিক errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      ধ্রুবক অক্ষর *descr,
					      ext2fs_block_bitmap *ret);
বাহ্যিক errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ধ্রুবক অক্ষর *descr,
						   ext2fs_block_bitmap *ret);
বাহ্যিক পূর্ণসংখ্যা ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
বাহ্যিক errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ধ্রুবক অক্ষর *descr,
					      ext2fs_inode_bitmap *ret);
বাহ্যিক errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
বাহ্যিক errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
বাহ্যিক errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
বাহ্যিক শূন্য ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
বাহ্যিক শূন্য ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
বাহ্যিক errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
বাহ্যিক errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
বাহ্যিক errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
বাহ্যিক errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
বাহ্যিক errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
বাহ্যিক errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
বাহ্যিক errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *in);
বাহ্যিক errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t অঙ্ক,
					 শূন্য *in);
বাহ্যিক errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *out);
বাহ্যিক errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t অঙ্ক,
					 শূন্য *out);
বাহ্যিক errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *in);
বাহ্যিক errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t অঙ্ক,
					 শূন্য *in);
বাহ্যিক errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *out);
বাহ্যিক errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t অঙ্ক,
					 শূন্য *out);

/* blknum.c */
বাহ্যিক __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
বাহ্যিক __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
বাহ্যিক dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
বাহ্যিক blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
বাহ্যিক পূর্ণসংখ্যা ext2fs_group_blocks_গুনতি(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk64_t ext2fs_inode_তথ্য_blocks2(ext2_filsys fs,
					 কাঠামো ext2_inode *inode);
বাহ্যিক blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     কাঠামো ext2_inode *inode);
বাহ্যিক blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					কাঠামো ext2_inode *inode);
বাহ্যিক blk64_t ext2fs_blocks_গুনতি(কাঠামো ext2_super_block *super);
বাহ্যিক শূন্য ext2fs_blocks_গুনতি_set(কাঠামো ext2_super_block *super,
				    blk64_t blk);
বাহ্যিক শূন্য ext2fs_blocks_গুনতি_add(কাঠামো ext2_super_block *super,
				    blk64_t blk);
বাহ্যিক blk64_t ext2fs_r_blocks_গুনতি(কাঠামো ext2_super_block *super);
বাহ্যিক শূন্য ext2fs_r_blocks_গুনতি_set(কাঠামো ext2_super_block *super,
				      blk64_t blk);
বাহ্যিক শূন্য ext2fs_r_blocks_গুনতি_add(কাঠামো ext2_super_block *super,
				      blk64_t blk);
বাহ্যিক blk64_t ext2fs_free_blocks_গুনতি(কাঠামো ext2_super_block *super);
বাহ্যিক শূন্য ext2fs_free_blocks_গুনতি_set(কাঠামো ext2_super_block *super,
					 blk64_t blk);
বাহ্যিক শূন্য ext2fs_free_blocks_গুনতি_add(কাঠামো ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
বাহ্যিক কাঠামো ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  কাঠামো opaque_ext2_group_desc *gdp,
					  dgrp_t group);
বাহ্যিক blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
বাহ্যিক __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
বাহ্যিক blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
বাহ্যিক __u32 ext2fs_bg_free_blocks_গুনতি(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_free_blocks_গুনতি_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
বাহ্যিক __u32 ext2fs_bg_free_inodes_গুনতি(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_free_inodes_গুনতি_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
বাহ্যিক __u32 ext2fs_bg_used_dirs_গুনতি(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_used_dirs_গুনতি_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
বাহ্যিক __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
বাহ্যিক __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
বাহ্যিক পূর্ণসংখ্যা ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
বাহ্যিক শূন্য ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
বাহ্যিক শূন্য ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
বাহ্যিক __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
বাহ্যিক শূন্য ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
বাহ্যিক blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     ধ্রুবক কাঠামো ext2_inode *inode);
বাহ্যিক শূন্য ext2fs_file_acl_block_set(ext2_filsys fs,
				      কাঠামো ext2_inode *inode, blk64_t blk);
বাহ্যিক errcode_t ext2fs_inode_আকার_set(ext2_filsys fs, কাঠামো ext2_inode *inode,
				       ext2_off64_t আকার_2);

/* block.c */
বাহ্যিক errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      পূর্ণসংখ্যা	flags,
				      অক্ষর *block_buf,
				      পূর্ণসংখ্যা (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  পূর্ণসংখ্যা	blockcnt,
						  শূন্য	*priv_তথ্য),
				      শূন্য *priv_তথ্য);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				পূর্ণসংখ্যা	flags,
				অক্ষর *block_buf,
				পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    পূর্ণসংখ্যা		ref_offset,
					    শূন্য	*priv_তথ্য),
				শূন্য *priv_তথ্য);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				পূর্ণসংখ্যা	flags,
				অক্ষর *block_buf,
				পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    পূর্ণসংখ্যা		ref_offset,
					    শূন্য	*priv_তথ্য),
				শূন্য *priv_তথ্য);

/* bmap.c */
বাহ্যিক errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     কাঠামো ext2_inode *inode,
			     অক্ষর *block_buf, পূর্ণসংখ্যা bmap_flags,
			     blk_t block, blk_t *phys_blk);
বাহ্যিক errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      কাঠামো ext2_inode *inode,
			      অক্ষর *block_buf, পূর্ণসংখ্যা bmap_flags, blk64_t block,
			      পূর্ণসংখ্যা *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   কাঠামো ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#যদি 0
/* bmove.c */
বাহ্যিক errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    পূর্ণসংখ্যা flags);
#endif

/* check_desc.c */
বাহ্যিক errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
বাহ্যিক errcode_t ext2fs_close(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_close2(ext2_filsys fs, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_close_free(ext2_filsys *fs);
বাহ্যিক errcode_t ext2fs_flush(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_flush2(ext2_filsys fs, পূর্ণসংখ্যা flags);
বাহ্যিক পূর্ণসংখ্যা ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
বাহ্যিক errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
বাহ্যিক পূর্ণসংখ্যা ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    পূর্ণসংখ্যা *ret_meta_bg);
বাহ্যিক শূন্য ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
বাহ্যিক __u32 ext2fs_crc32_be(__u32 crc, unsigned অক্ষর ধ্রুবক *p, size_t len);
বাহ্যিক __u32 ext2fs_crc32c_le(__u32 crc, unsigned অক্ষর ধ্রুবক *p, size_t len);

/* csum.c */
বাহ্যিক শূন্য ext2fs_init_csum_seed(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, কাঠামো mmp_struct *mmp);
বাহ্যিক পূর্ণসংখ্যা ext2fs_mmp_csum_verify(ext2_filsys, কাঠামো mmp_struct *mmp);
বাহ্যিক পূর্ণসংখ্যা ext2fs_verify_csum_type(ext2_filsys fs, কাঠামো ext2_super_block *sb);
বাহ্যিক errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    কাঠামো ext2_super_block *sb);
বাহ্যিক পূর্ণসংখ্যা ext2fs_superblock_csum_verify(ext2_filsys fs,
					 কাঠামো ext2_super_block *sb);
বাহ্যিক errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					কাঠামো ext2_ext_attr_header *hdr);
বাহ্যিক পূর্ণসংখ্যা ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     কাঠামো ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_লেজ(block, blocksize) \
	((কাঠামো ext2_dir_entry_লেজ *)(((অক্ষর *)(block)) + \
	(blocksize) - আকার(কাঠামো ext2_dir_entry_লেজ)))

বাহ্যিক শূন্য ext2fs_initialize_dirent_লেজ(ext2_filsys fs,
					  কাঠামো ext2_dir_entry_লেজ *t);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dirent_has_লেজ(ext2_filsys fs,
				  কাঠামো ext2_dir_entry *dirent);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     কাঠামো ext2_dir_entry *dirent);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					কাঠামো ext2_dir_entry *dirent);
বাহ্যিক errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   কাঠামো ext2_dir_entry *dirent);
বাহ্যিক errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  কাঠামো ext2_dir_entry *dirent,
					  কাঠামো ext2_dx_countlimit **cc,
					  পূর্ণসংখ্যা *offset);
বাহ্যিক errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				কাঠামো ext2_dir_entry *dirent,
				__u32 *crc, কাঠামো ext2_dx_লেজ **ret_t);
বাহ্যিক errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      কাঠামো ext3_extent_header *eh);
বাহ্যিক পূর্ণসংখ্যা ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   কাঠামো ext3_extent_header *eh);
বাহ্যিক errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2);
বাহ্যিক পূর্ণসংখ্যা ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       কাঠামো ext2_inode_large *inode);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    কাঠামো ext2_inode_large *inode);
বাহ্যিক শূন্য ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
বাহ্যিক পূর্ণসংখ্যা ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
বাহ্যিক errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
বাহ্যিক __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
বাহ্যিক errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
বাহ্যিক errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, পূর্ণসংখ্যা blockcnt);
বাহ্যিক errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
বাহ্যিক শূন্য ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ধ্রুবক শূন্য *,
							   ধ্রুবক শূন্য *));
বাহ্যিক শূন্য ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ধ্রুবক শূন্য *,
							    ধ্রুবক শূন্য *));
বাহ্যিক errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	পূর্ণসংখ্যা (*func)(ext2_filsys fs, কাঠামো ext2_db_entry *db_info,
		    শূন্য	*priv_তথ্য),
	শূন্য *priv_তথ্য);
বাহ্যিক errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	পূর্ণসংখ্যা (*func)(ext2_filsys fs, কাঠামো ext2_db_entry2 *db_info,
		    শূন্য	*priv_তথ্য),
	শূন্য *priv_তথ্য);
বাহ্যিক errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	পূর্ণসংখ্যা (*func)(ext2_filsys fs, কাঠামো ext2_db_entry2 *db_info,
		    শূন্য	*priv_তথ্য),
	unsigned long long start,
	unsigned long long গুনতি,
	শূন্য *priv_তথ্য);
বাহ্যিক errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, পূর্ণসংখ্যা blockcnt);
বাহ্যিক errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
বাহ্যিক errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dblist_গুনতি(ext2_dblist dblist);
বাহ্যিক blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
বাহ্যিক errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					কাঠামো ext2_db_entry **entry);
বাহ্যিক errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					কাঠামো ext2_db_entry2 **entry);
বাহ্যিক errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
বাহ্যিক errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  পূর্ণসংখ্যা	flags,
				  অক্ষর	*block_buf,
				  পূর্ণসংখ্যা (*func)(ext2_ino_t	dir,
					      পূর্ণসংখ্যা		entry,
					      কাঠামো ext2_dir_entry *dirent,
					      পূর্ণসংখ্যা	offset,
					      পূর্ণসংখ্যা	blocksize,
					      অক্ষর	*buf,
					      শূন্য	*priv_তথ্য),
				  শূন্য *priv_তথ্য);

#যদি 0
/* digest_encode.c */
#define EXT2_FS_DIGEST_আকার EXT2_FS_SHA256_দৈর্ঘ্য
বাহ্যিক পূর্ণসংখ্যা ext2fs_digest_encode(ধ্রুবক অক্ষর *src, পূর্ণসংখ্যা len, অক্ষর *dst);
বাহ্যিক পূর্ণসংখ্যা ext2fs_digest_decode(ধ্রুবক অক্ষর *src, পূর্ণসংখ্যা len, অক্ষর *dst);
#endif

/* dirblock.c */
বাহ্যিক errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       শূন্য *buf);
বাহ্যিক errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					শূন্য *buf, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					শূন্য *buf, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					শূন্য *buf, পূর্ণসংখ্যা flags, ext2_ino_t ino);
বাহ্যিক errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					শূন্য *buf);
বাহ্যিক errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 শূন্য *buf, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 শূন্য *buf, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 শূন্য *buf, পূর্ণসংখ্যা flags, ext2_ino_t ino);

/* dirhash.c */
বাহ্যিক errcode_t ext2fs_dirhash(পূর্ণসংখ্যা version, ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা len,
				ধ্রুবক __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

বাহ্যিক errcode_t ext2fs_dirhash2(পূর্ণসংখ্যা version, ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা len,
				 ধ্রুবক কাঠামো ext2fs_nls_table *charset,
				 পূর্ণসংখ্যা hash_flags,
				 ধ্রুবক __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
বাহ্যিক errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    কাঠামো ext2_dir_entry *dirent,
				    unsigned পূর্ণসংখ্যা *rec_len);
বাহ্যিক errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    unsigned পূর্ণসংখ্যা len,
				    কাঠামো ext2_dir_entry *dirent);
বাহ্যিক errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      পূর্ণসংখ্যা flags,
			      অক্ষর *block_buf,
			      পূর্ণসংখ্যা (*func)(কাঠামো ext2_dir_entry *dirent,
					  পূর্ণসংখ্যা	offset,
					  পূর্ণসংখ্যা	blocksize,
					  অক্ষর	*buf,
					  শূন্য	*priv_তথ্য),
			      শূন্য *priv_তথ্য);
বাহ্যিক errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      পূর্ণসংখ্যা flags,
			      অক্ষর *block_buf,
			      পূর্ণসংখ্যা (*func)(ext2_ino_t	dir,
					  পূর্ণসংখ্যা	entry,
					  কাঠামো ext2_dir_entry *dirent,
					  পূর্ণসংখ্যা	offset,
					  পূর্ণসংখ্যা	blocksize,
					  অক্ষর	*buf,
					  শূন্য	*priv_তথ্য),
			      শূন্য *priv_তথ্য);

/* dupfs.c */
বাহ্যিক errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
বাহ্যিক errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
বাহ্যিক __u32 ext2fs_ext_attr_hash_entry(কাঠামো ext2_ext_attr_entry *entry,
					শূন্য *তথ্য);
বাহ্যিক __u32 ext2fs_ext_attr_hash_entry_signed(কাঠামো ext2_ext_attr_entry *entry,
					       শূন্য *তথ্য);
বাহ্যিক errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     কাঠামো ext2_ext_attr_entry *entry,
					     শূন্য *তথ্য, __u32 *hash);
বাহ্যিক errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     কাঠামো ext2_ext_attr_entry *entry,
					     শূন্য *তথ্য, __u32 *hash,
					     __u32 *signed_hash);
বাহ্যিক errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, শূন্য *buf);
বাহ্যিক errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       শূন্য *buf);
বাহ্যিক errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       শূন্য *buf, ext2_ino_t inum);
বাহ্যিক errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       শূন্য *buf);
বাহ্যিক errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       শূন্য *buf);
বাহ্যিক errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       শূন্য *buf, ext2_ino_t inum);
বাহ্যিক errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   অক্ষর *block_buf,
					   পূর্ণসংখ্যা adjust, __u32 *newcount);
বাহ্যিক errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   অক্ষর *block_buf,
					   পূর্ণসংখ্যা adjust, __u32 *newcount);
বাহ্যিক errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   অক্ষর *block_buf,
					   পূর্ণসংখ্যা adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(কাঠামো ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(কাঠামো ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(কাঠামো ext2_xattr_handle *handle,
				   কাঠামো ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(কাঠামো ext2_xattr_handle *h,
				পূর্ণসংখ্যা (*func)(অক্ষর *name, অক্ষর *মান,
					    size_t মান_len, শূন্য *তথ্য),
				শূন্য *তথ্য);
errcode_t ext2fs_xattr_get(কাঠামো ext2_xattr_handle *h, ধ্রুবক অক্ষর *চাবি,
			   শূন্য **মান, size_t *মান_len);
errcode_t ext2fs_xattr_set(কাঠামো ext2_xattr_handle *handle,
			   ধ্রুবক অক্ষর *চাবি,
			   ধ্রুবক শূন্য *মান,
			   size_t মান_len);
errcode_t ext2fs_xattr_remove(কাঠামো ext2_xattr_handle *handle,
			      ধ্রুবক অক্ষর *চাবি);
errcode_t ext2fs_xattr_remove_all(কাঠামো ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     কাঠামো ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(কাঠামো ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       কাঠামো ext2_inode_large *inode);
errcode_t ext2fs_xattrs_গুনতি(কাঠামো ext2_xattr_handle *handle, size_t *গুনতি);
errcode_t ext2fs_xattr_inode_max_আকার(ext2_filsys fs, ext2_ino_t ino,
				      size_t *আকার_2);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(কাঠামো ext2_xattr_handle *handle,
			      unsigned পূর্ণসংখ্যা *new_flags, unsigned পূর্ণসংখ্যা *old_flags);
বাহ্যিক শূন্য ext2fs_ext_attr_block_rehash(কাঠামো ext2_ext_attr_header *header,
					 কাঠামো ext2_ext_attr_entry *end);
বাহ্যিক __u32 ext2fs_get_ea_inode_hash(কাঠামো ext2_inode *inode);
বাহ্যিক শূন্য ext2fs_set_ea_inode_hash(কাঠামো ext2_inode *inode, __u32 hash);
বাহ্যিক __u64 ext2fs_get_ea_inode_ref(কাঠামো ext2_inode *inode);
বাহ্যিক শূন্য ext2fs_set_ea_inode_ref(কাঠামো ext2_inode *inode, __u64 ref_গুনতি);

/* extent.c */
বাহ্যিক errcode_t ext2fs_extent_header_verify(শূন্য *ptr, পূর্ণসংখ্যা আকার_2);
বাহ্যিক errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
বাহ্যিক errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					কাঠামো ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
বাহ্যিক শূন্য ext2fs_extent_free(ext2_extent_handle_t handle);
বাহ্যিক errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   পূর্ণসংখ্যা flags, কাঠামো ext2fs_extent *extent);
বাহ্যিক errcode_t ext2fs_extent_গ্রন্থি_split(ext2_extent_handle_t handle);
বাহ্যিক errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, পূর্ণসংখ্যা flags,
				       কাঠামো ext2fs_extent *extent);
বাহ্যিক errcode_t ext2fs_extent_ঢোকাও(ext2_extent_handle_t handle, পূর্ণসংখ্যা flags,
				      কাঠামো ext2fs_extent *extent);
বাহ্যিক errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					কাঠামো ext2_extent_info *info);
বাহ্যিক errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
বাহ্যিক errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     পূর্ণসংখ্যা leaf_level, blk64_t blk);
বাহ্যিক errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
বাহ্যিক size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle);
বাহ্যিক errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      কাঠামো ext2_inode *inode);
বাহ্যিক errcode_t ext2fs_গুনতি_blocks(ext2_filsys fs, ext2_ino_t ino,
				     কাঠামো ext2_inode *inode, blk64_t *ret_গুনতি);
বাহ্যিক errcode_t ext2fs_decode_extent(কাঠামো ext2fs_extent *to, শূন্য *from,
				      পূর্ণসংখ্যা len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, পূর্ণসংখ্যা flags, ext2_ino_t ino,
			   কাঠামো ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
বাহ্যিক errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   কাঠামো ext2_inode *inode,
				   পূর্ণসংখ্যা flags, ext2_file_t *ret);
বাহ্যিক errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  পূর্ণসংখ্যা flags, ext2_file_t *ret);
বাহ্যিক ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
কাঠামো ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
বাহ্যিক ext2_ino_t ext2fs_file_get_inode_অঙ্ক(ext2_file_t file);
বাহ্যিক errcode_t ext2fs_file_close(ext2_file_t file);
বাহ্যিক errcode_t ext2fs_file_flush(ext2_file_t file);
বাহ্যিক errcode_t ext2fs_file_read(ext2_file_t file, শূন্য *buf,
				  unsigned পূর্ণসংখ্যা wanted, unsigned পূর্ণসংখ্যা *got);
বাহ্যিক errcode_t ext2fs_file_write(ext2_file_t file, ধ্রুবক শূন্য *buf,
				   unsigned পূর্ণসংখ্যা nbytes, unsigned পূর্ণসংখ্যা *written);
বাহ্যিক errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   পূর্ণসংখ্যা whence, __u64 *ret_pos);
বাহ্যিক errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   পূর্ণসংখ্যা whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_আকার);
বাহ্যিক ext2_off_t ext2fs_file_get_আকার(ext2_file_t file);
বাহ্যিক errcode_t ext2fs_file_set_আকার(ext2_file_t file, ext2_off_t আকার_2);
বাহ্যিক errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t আকার_2);

/* finddev.c */
বাহ্যিক অক্ষর *ext2fs_find_block_device(dev_t device);

/* flushb.c */
বাহ্যিক errcode_t ext2fs_sync_device(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flushb);

/* freefs.c */
বাহ্যিক শূন্য ext2fs_free(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_free_dblist(ext2_dblist dblist);
বাহ্যিক শূন্য ext2fs_badblocks_list_free(ext2_badblocks_list bb);
বাহ্যিক শূন্য ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
বাহ্যিক শূন্য ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
বাহ্যিক errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 end,
					    __u32 real_end,
					    ধ্রুবক অক্ষর *descr, অক্ষর *init_map,
					    ext2fs_generic_bitmap *ret);
বাহ্যিক errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						ধ্রুবক অক্ষর *descr,
						ext2fs_generic_bitmap *ret);
বাহ্যিক errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
বাহ্যিক শূন্য ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
বাহ্যিক errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
বাহ্যিক শূন্য ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
বাহ্যিক errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
বাহ্যিক errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
বাহ্যিক errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 অঙ্ক,
						 শূন্য *out);
বাহ্যিক errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 অঙ্ক,
						 শূন্য *in);
বাহ্যিক errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
বাহ্যিক errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
শূন্য ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    পূর্ণসংখ্যা type, __u64 start, __u64 end,
				    __u64 real_end,
				    ধ্রুবক অক্ষর *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
শূন্য ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
শূন্য ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_গুনতি_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_গুনতি_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
বাহ্যিক unsigned পূর্ণসংখ্যা ext2fs_list_backups(ext2_filsys fs, unsigned পূর্ণসংখ্যা *three,
				unsigned পূর্ণসংখ্যা *five, unsigned পূর্ণসংখ্যা *seven);

/* getenv.c */
বাহ্যিক অক্ষর *ext2fs_safe_getenv(ধ্রুবক অক্ষর *arg);

/* get_num_dirs.c */
বাহ্যিক errcode_t ext2fs_get_অঙ্ক_dirs(ext2_filsys fs, ext2_ino_t *ret_অঙ্ক_dirs);

/* getsize.c */
বাহ্যিক errcode_t ext2fs_get_device_আকার(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
					blk_t *retblocks);
বাহ্যিক errcode_t ext2fs_get_device_size2(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
বাহ্যিক পূর্ণসংখ্যা ext2fs_get_dio_alignment(পূর্ণসংখ্যা fd);
errcode_t ext2fs_get_device_sectsize(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, কাঠামো ext2_inode *inode,
				 blk64_t অঙ্ক_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, কাঠামো ext2_inode *inode,
				 blk64_t অঙ্ক_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, কাঠামো ext2_inode *inode, blk64_t b);

/* imager.c */
বাহ্যিক errcode_t ext2fs_image_inode_write(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_image_inode_read(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_image_super_write(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_image_super_read(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, শূন্য *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, শূন্য *buf);

/* initialize.c */
বাহ্যিক errcode_t ext2fs_initialize(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags,
				   কাঠামো ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
বাহ্যিক errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, পূর্ণসংখ্যা super_only);

/* icount.c */
বাহ্যিক শূন্য ext2fs_free_icount(ext2_icount_t icount);
বাহ্যিক errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, অক্ষর *tdb_dir,
					  পূর্ণসংখ্যা flags, ext2_icount_t *ret);
বাহ্যিক errcode_t ext2fs_create_icount2(ext2_filsys fs, পূর্ণসংখ্যা flags,
				       unsigned পূর্ণসংখ্যা আকার_2,
				       ext2_icount_t hint, ext2_icount_t *ret);
বাহ্যিক errcode_t ext2fs_create_icount(ext2_filsys fs, পূর্ণসংখ্যা flags,
				      unsigned পূর্ণসংখ্যা আকার_2,
				      ext2_icount_t *ret);
বাহ্যিক errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
বাহ্যিক errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
বাহ্যিক errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
বাহ্যিক errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 গুনতি);
বাহ্যিক ext2_ino_t ext2fs_get_icount_আকার(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

বাহ্যিক errcode_t ext2fs_get_memalign(unsigned long আকার_2,
				     unsigned long align, শূন্য *ptr);

/* inline_data.c */
বাহ্যিক errcode_t ext2fs_inline_তথ্য_init(ext2_filsys fs, ext2_ino_t ino);
বাহ্যিক errcode_t ext2fs_inline_তথ্য_আকার(ext2_filsys fs, ext2_ino_t ino,
					 size_t *আকার_2);
বাহ্যিক errcode_t ext2fs_inline_তথ্য_get(ext2_filsys fs, ext2_ino_t ino,
					কাঠামো ext2_inode *inode,
					শূন্য *buf, size_t *আকার_2);
বাহ্যিক errcode_t ext2fs_inline_তথ্য_set(ext2_filsys fs, ext2_ino_t ino,
					কাঠামো ext2_inode *inode,
					শূন্য *buf, size_t আকার_2);

/* inode.c */
বাহ্যিক errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned পূর্ণসংখ্যা cache_আকার);
বাহ্যিক শূন্য ext2fs_free_inode_cache(কাঠামো ext2_inode_cache *icache);
বাহ্যিক errcode_t ext2fs_flush_icache(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_get_পরবর্তী_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    কাঠামো ext2_inode *inode,
					    পূর্ণসংখ্যা bufsize);
#define EXT2_INODE_SCAN_DEFAULT_বাফার_BLOCKS	8
বাহ্যিক errcode_t ext2fs_open_inode_scan(ext2_filsys fs, পূর্ণসংখ্যা বাফার_blocks,
				  ext2_inode_scan *ret_scan);
বাহ্যিক শূন্য ext2fs_close_inode_scan(ext2_inode_scan scan);
বাহ্যিক errcode_t ext2fs_get_পরবর্তী_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       কাঠামো ext2_inode *inode);
বাহ্যিক errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   পূর্ণসংখ্যা	group);
বাহ্যিক শূন্য ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 শূন্য * priv_তথ্য),
	 শূন্য *done_group_তথ্য);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inode_scan_flags(ext2_inode_scan scan, পূর্ণসংখ্যা set_flags,
				   পূর্ণসংখ্যা clear_flags);
বাহ্যিক errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					কাঠামো ext2_inode * inode,
					পূর্ণসংখ্যা bufsize);
বাহ্যিক errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    কাঠামো ext2_inode * inode);
বাহ্যিক errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    কাঠামো ext2_inode * inode,
				    পূর্ণসংখ্যা bufsize, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 কাঠামো ext2_inode * inode,
					 পূর্ণসংখ্যা bufsize);
বাহ্যিক errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    কাঠামো ext2_inode * inode);
বাহ্যিক errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     কাঠামো ext2_inode * inode,
				     পূর্ণসংখ্যা bufsize, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    কাঠামো ext2_inode * inode);
বাহ্যিক errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
বাহ্যিক errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
বাহ্যিক io_manager inode_io_manager;
বাহ্যিক errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					অক্ষর **name);
বাহ্যিক errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 কাঠামো ext2_inode *inode,
					 অক্ষর **name);

/* ismounted.c */
বাহ্যিক errcode_t ext2fs_check_if_mounted(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা *mount_flags);
বাহ্যিক errcode_t ext2fs_check_mount_point(ধ্রুবক অক্ষর *device, পূর্ণসংখ্যা *mount_flags,
					  অক্ষর *mtpt, পূর্ণসংখ্যা mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
বাহ্যিক errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      কাঠামো ext2_inode *inode,
			      অক্ষর *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
বাহ্যিক errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ধ্রুবক অক্ষর *name,
			 পূর্ণসংখ্যা namelen, অক্ষর *buf, ext2_ino_t *inode);
বাহ্যিক errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			ধ্রুবক অক্ষর *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			      ধ্রুবক অক্ষর *name, ext2_ino_t *inode);
বাহ্যিক errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
পূর্ণসংখ্যা ext2fs_native_flag(শূন্য);

/* newdir.c */
বাহ্যিক errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, অক্ষর **block);
বাহ্যিক errcode_t ext2fs_new_dir_inline_তথ্য(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
বাহ্যিক ধ্রুবক কাঠামো ext2fs_nls_table *ext2fs_load_nls_table(পূর্ণসংখ্যা encoding);
বাহ্যিক পূর্ণসংখ্যা ext2fs_check_encoded_name(ধ্রুবক কাঠামো ext2fs_nls_table *table,
				     অক্ষর *s, size_t len, অক্ষর **pos);
বাহ্যিক পূর্ণসংখ্যা ext2fs_casefold_cmp(ধ্রুবক কাঠামো ext2fs_nls_table *table,
			       ধ্রুবক unsigned অক্ষর *str1, size_t len1,
			       ধ্রুবক unsigned অক্ষর *str2, size_t len2);

/* mkdir.c */
বাহ্যিক errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      ধ্রুবক অক্ষর *name);
বাহ্যিক errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, unsigned long flags,
			       পূর্ণসংখ্যা link_flags, ধ্রুবক অক্ষর *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
কাঠামো ext2fs_journal_params {
	blk_t অঙ্ক_journal_blocks;
	blk_t অঙ্ক_fc_blocks;
};
বাহ্যিক errcode_t ext2fs_get_journal_params(
		কাঠামো ext2fs_journal_params *params, ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, পূর্ণসংখ্যা অঙ্ক,
				    blk_t *ret_blk, পূর্ণসংখ্যা *ret_গুনতি);
বাহ্যিক errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, পূর্ণসংখ্যা অঙ্ক,
				     blk64_t *ret_blk, পূর্ণসংখ্যা *ret_গুনতি);
বাহ্যিক errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 অঙ্ক_blocks, পূর্ণসংখ্যা flags,
						  অক্ষর  **ret_jsb);
বাহ্যিক errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  কাঠামো ext2fs_journal_params *params,
						  পূর্ণসংখ্যা flags, অক্ষর  **ret_jsb);
বাহ্যিক errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
বাহ্যিক errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t অঙ্ক_blocks,
					  পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t অঙ্ক_blocks,
					   blk64_t goal, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    কাঠামো ext2fs_journal_params *params,
				    blk64_t goal, পূর্ণসংখ্যা flags);
বাহ্যিক পূর্ণসংখ্যা ext2fs_default_journal_আকার(__u64 অঙ্ক_blocks);
বাহ্যিক পূর্ণসংখ্যা ext2fs_journal_sb_start(পূর্ণসংখ্যা blocksize);

/* openfs.c */
বাহ্যিক errcode_t ext2fs_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা superblock,
			     unsigned পূর্ণসংখ্যা block_আকার, io_manager manager,
			     ext2_filsys *ret_fs);
বাহ্যিক errcode_t ext2fs_open2(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *io_options,
			      পূর্ণসংখ্যা flags, পূর্ণসংখ্যা superblock,
			      unsigned পূর্ণসংখ্যা block_আকার, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
বাহ্যিক blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
বাহ্যিক blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_তথ্য_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_তথ্য_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
বাহ্যিক errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t অঙ্ক_blocks);
বাহ্যিক errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
বাহ্যিক e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
বাহ্যিক __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      অক্ষর *buf);
বাহ্যিক errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   অক্ষর *buf);
বাহ্যিক পূর্ণসংখ্যা ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, অক্ষর *buf);

/* get_pathname.c */
বাহ্যিক errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       অক্ষর **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ধ্রুবক অক্ষর *name,
		      ext2_ino_t ino, পূর্ণসংখ্যা flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ধ্রুবক অক্ষর *name,
			ext2_ino_t ino, পূর্ণসংখ্যা flags);
পূর্ণসংখ্যা ext2fs_dir_is_dx(ext2_filsys fs, ধ্রুবক কাঠামো ext2_inode *inode);
শূন্য ext2fs_inc_nlink(ext2_filsys fs, কাঠামো ext2_inode *inode);
শূন্য ext2fs_dec_nlink(কাঠামো ext2_inode *inode);
পূর্ণসংখ্যা ext2fs_dir_link_max(ext2_filsys fs, কাঠামো ext2_inode_large *inode);
পূর্ণসংখ্যা ext2fs_dir_link_empty(কাঠামো ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *target);
পূর্ণসংখ্যা ext2fs_is_fast_symlink(কাঠামো ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, শূন্য *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, শূন্য *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, পূর্ণসংখ্যা immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
unsigned ext2fs_mmp_new_seq(শূন্য);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, শূন্য **ptr);

/* read_bb.c */
বাহ্যিক errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
বাহ্যিক errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      শূন্য *priv_তথ্য,
				      শূন্য (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      অক্ষর *badstr,
						      শূন্য *priv_তথ্য));
বাহ্যিক errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     শূন্য (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
বাহ্যিক errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
বাহ্যিক errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা অঙ্ক_threads);
বাহ্যিক errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
বাহ্যিক errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2_FS_SHA256_দৈর্ঘ্য 32
#যদি 0
বাহ্যিক শূন্য ext2fs_sha256(ধ্রুবক unsigned অক্ষর *in, unsigned long in_আকার,
		   unsigned অক্ষর out[EXT2_FS_SHA256_দৈর্ঘ্য]);
#endif

/* sha512.c */
#define EXT2_FS_SHA512_দৈর্ঘ্য 64
বাহ্যিক শূন্য ext2fs_sha512(ধ্রুবক unsigned অক্ষর *in, unsigned long in_আকার,
			  unsigned অক্ষর out[EXT2_FS_SHA512_দৈর্ঘ্য]);

/* swapfs.c */
বাহ্যিক errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, অক্ষর *buf, size_t আকার_2,
					পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, অক্ষর *buf, পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, অক্ষর *buf, size_t আকার_2,
					 পূর্ণসংখ্যা flags);
বাহ্যিক errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, অক্ষর *buf, পূর্ণসংখ্যা flags);
বাহ্যিক শূন্য ext2fs_swap_ext_attr(অক্ষর *to, অক্ষর *from, পূর্ণসংখ্যা bufsize,
				 পূর্ণসংখ্যা has_header);
বাহ্যিক শূন্য ext2fs_swap_ext_attr_header(কাঠামো ext2_ext_attr_header *to_header,
					কাঠামো ext2_ext_attr_header *from_hdr);
বাহ্যিক শূন্য ext2fs_swap_ext_attr_entry(কাঠামো ext2_ext_attr_entry *to_entry,
				       কাঠামো ext2_ext_attr_entry *from_entry);
বাহ্যিক শূন্য ext2fs_swap_super(কাঠামো ext2_super_block * super);
বাহ্যিক শূন্য ext2fs_swap_group_desc(কাঠামো ext2_group_desc *gdp);
বাহ্যিক শূন্য ext2fs_swap_group_desc2(ext2_filsys, কাঠামো ext2_group_desc *gdp);
বাহ্যিক শূন্য ext2fs_swap_inode_full(ext2_filsys fs, কাঠামো ext2_inode_large *t,
				   কাঠামো ext2_inode_large *f, পূর্ণসংখ্যা hostorder,
				   পূর্ণসংখ্যা bufsize);
বাহ্যিক শূন্য ext2fs_swap_inode(ext2_filsys fs,কাঠামো ext2_inode *t,
			      কাঠামো ext2_inode *f, পূর্ণসংখ্যা hostorder);
বাহ্যিক শূন্য ext2fs_swap_mmp(কাঠামো mmp_struct *mmp);

/* unix_io.c */
বাহ্যিক পূর্ণসংখ্যা ext2fs_open_file(ধ্রুবক অক্ষর *pathname, পূর্ণসংখ্যা flags, mode_t mode);
বাহ্যিক পূর্ণসংখ্যা ext2fs_stat(ধ্রুবক অক্ষর *path, ext2fs_struct_stat *buf);
বাহ্যিক পূর্ণসংখ্যা ext2fs_fstat(পূর্ণসংখ্যা fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
বাহ্যিক পূর্ণসংখ্যা ext2fs_inode_has_valid_blocks(কাঠামো ext2_inode *inode);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  কাঠামো ext2_inode *inode);

/* version.c */
বাহ্যিক পূর্ণসংখ্যা ext2fs_parse_version_string(ধ্রুবক অক্ষর *ver_string);
বাহ্যিক পূর্ণসংখ্যা ext2fs_get_library_version(ধ্রুবক অক্ষর **ver_string,
				      ধ্রুবক অক্ষর **date_string);

/* write_bb_file.c */
বাহ্যিক errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      unsigned পূর্ণসংখ্যা flags,
				      FILE *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
বাহ্যিক errcode_t ext2fs_get_mem(unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_get_memzero(unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_get_array(unsigned long গুনতি,
				  unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_get_arrayzero(unsigned long গুনতি,
				      unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_free_mem(শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_resize_mem(unsigned long old_আকার,
				   unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক errcode_t ext2fs_resize_array(unsigned long old_গুনতি, unsigned long গুনতি,
				     unsigned long আকার_2, শূন্য *ptr);
বাহ্যিক শূন্য ext2fs_mark_super_dirty(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_mark_changed(ext2_filsys fs);
বাহ্যিক পূর্ণসংখ্যা ext2fs_test_changed(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_mark_valid(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_unmark_valid(ext2_filsys fs);
বাহ্যিক পূর্ণসংখ্যা ext2fs_test_valid(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_mark_ib_dirty(ext2_filsys fs);
বাহ্যিক শূন্য ext2fs_mark_bb_dirty(ext2_filsys fs);
বাহ্যিক পূর্ণসংখ্যা ext2fs_test_ib_dirty(ext2_filsys fs);
বাহ্যিক পূর্ণসংখ্যা ext2fs_test_bb_dirty(ext2_filsys fs);
বাহ্যিক dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
বাহ্যিক dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
বাহ্যিক blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
বাহ্যিক blk_t ext2fs_inode_তথ্য_blocks(ext2_filsys fs,
				      কাঠামো ext2_inode *inode);
বাহ্যিক পূর্ণসংখ্যা ext2fs_htree_intnode_maxrecs(ext2_filsys fs, পূর্ণসংখ্যা blocks);
বাহ্যিক পূর্ণসংখ্যা ext2fs_log2_u32(__u32 arg);
বাহ্যিক পূর্ণসংখ্যা ext2fs_log2_u64(__u64 arg);
বাহ্যিক পূর্ণসংখ্যা ext2fs_log10_u32(__u32 arg);
বাহ্যিক পূর্ণসংখ্যা ext2fs_log10_u64(__u64 arg);
বাহ্যিক unsigned পূর্ণসংখ্যা ext2fs_div_ceil(unsigned পূর্ণসংখ্যা a, unsigned পূর্ণসংখ্যা b);
বাহ্যিক __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dirent_name_len(ধ্রুবক কাঠামো ext2_dir_entry *entry);
বাহ্যিক শূন্য ext2fs_dirent_set_name_len(কাঠামো ext2_dir_entry *entry, পূর্ণসংখ্যা len);
বাহ্যিক পূর্ণসংখ্যা ext2fs_dirent_file_type(ধ্রুবক কাঠামো ext2_dir_entry *entry);
বাহ্যিক শূন্য ext2fs_dirent_set_file_type(কাঠামো ext2_dir_entry *entry, পূর্ণসংখ্যা type);
বাহ্যিক কাঠামো ext2_inode *ext2fs_inode(কাঠামো ext2_inode_large * large_inode);
বাহ্যিক ধ্রুবক কাঠামো ext2_inode *ext2fs_const_inode(ধ্রুবক কাঠামো ext2_inode_large * large_inode);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inodes_per_orphan_block(ext2_filsys fs);
বাহ্যিক কাঠামো ext4_orphan_block_লেজ *ext2fs_orphan_block_লেজ(ext2_filsys fs,
							       অক্ষর *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#যদি (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ বাহ্যিক
#অন্যথা
#যদি (__STDC_VERSION__ >= 199901L)
#define _INLINE_ অন্তঃরেখ
#অন্যথা
#ifdef __GNUC__
#define _INLINE_ বাহ্যিক __inline__
#অন্যথা				/* For Watcom C */
#define _INLINE_ বাহ্যিক অন্তঃরেখ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#অন্তর্ভাব <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long আকার_2, শূন্য *ptr)
{
	শূন্য *pp;

	pp = malloc(আকার_2);
	যদি (!pp)
		ফেরত EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, আকার (pp));
	ফেরত 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long আকার_2, শূন্য *ptr)
{
	শূন্য *pp;

	pp = malloc(আকার_2);
	যদি (!pp)
		ফেরত EXT2_ET_NO_MEMORY;
	memset(pp, 0, আকার_2);
	memcpy(ptr, &pp, আকার(pp));
	ফেরত 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long গুনতি, unsigned long আকার_2,
				    শূন্য *ptr)
{
	যদি (গুনতি && (~0UL)/গুনতি < আকার_2)
		ফেরত EXT2_ET_NO_MEMORY;
	ফেরত ext2fs_get_mem(গুনতি*আকার_2, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long গুনতি,
					unsigned long আকার_2, শূন্য *ptr)
{
	যদি (গুনতি && (~0UL)/গুনতি < আকার_2)
		ফেরত EXT2_ET_NO_MEMORY;

	ফেরত ext2fs_get_memzero((size_t)গুনতি * আকার_2, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(শূন্য *ptr)
{
	শূন্য *p;

	memcpy(&p, ptr, আকার(p));
	মুক্ত(p);
	p = 0;
	memcpy(ptr, &p, আকার(p));
	ফেরত 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_আকার,
				     unsigned long আকার_2, শূন্য *ptr)
{
	শূন্য *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, আকার(p));
	p = realloc(p, আকার_2);
	যদি (!p)
		ফেরত EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, আকার(p));
	ফেরত 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(unsigned long আকার_2,
				       unsigned long old_গুনতি,
				       unsigned long গুনতি, শূন্য *ptr)
{
	unsigned long old_আকার;
	errcode_t retval;

	যদি (গুনতি && (~0UL)/গুনতি < আকার_2)
		ফেরত EXT2_ET_NO_MEMORY;

	আকার_2 *= গুনতি;
	old_আকার = আকার_2 * old_গুনতি;
	retval = ext2fs_resize_mem(old_আকার, আকার_2, ptr);
	যদি (retval)
		ফেরত retval;

	যদি (আকার_2 > old_আকার) {
		শূন্য *p;

		memcpy(&p, ptr, আকার(p));
		memset((অক্ষর *)p + old_আকার, 0, আকার_2 - old_আকার);
		memcpy(ptr, &p, আকার(p));
	}

	ফেরত 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ শূন্য ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ শূন্য ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_test_changed(ext2_filsys fs)
{
	ফেরত (fs->flags & EXT2_FLAG_CHANGED);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ শূন্য ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ শূন্য ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_test_valid(ext2_filsys fs)
{
	ফেরত (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ শূন্য ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ শূন্য ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_test_ib_dirty(ext2_filsys fs)
{
	ফেরত (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_test_bb_dirty(ext2_filsys fs)
{
	ফেরত (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	ফেরত ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	ফেরত (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	ফেরত (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	ফেরত (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_তথ্য_blocks(ext2_filsys fs,
					কাঠামো ext2_inode *inode)
{
	ফেরত (blk_t) ext2fs_inode_তথ্য_blocks2(fs, inode);
}

_INLINE_ পূর্ণসংখ্যা ext2fs_htree_intnode_maxrecs(ext2_filsys fs, পূর্ণসংখ্যা blocks)
{
	পূর্ণসংখ্যা csum_আকার = 0;

	যদি ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_আকার = আকার(কাঠামো ext2_dx_লেজ);
	ফেরত blocks * ((fs->blocksize - (8 + csum_আকার)) /
						আকার(কাঠামো ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_log2_u32(__u32 arg)
{
	পূর্ণসংখ্যা l = 0;

	arg >>= 1;
	যখন (arg) {
		l++;
		arg >>= 1;
	}
	ফেরত l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_log2_u64(__u64 arg)
{
	পূর্ণসংখ্যা l = 0;

	arg >>= 1;
	যখন (arg) {
		l++;
		arg >>= 1;
	}
	ফেরত l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_log10_u32(__u32 arg)
{
	পূর্ণসংখ্যা l = 0;

	arg /= 10;
	যখন (arg) {
		l++;
		arg /= 10;
	}
	ফেরত l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ পূর্ণসংখ্যা ext2fs_log10_u64(__u64 arg)
{
	পূর্ণসংখ্যা l = 0;

	arg /= 10;
	যখন (arg) {
		l++;
		arg /= 10;
	}
	ফেরত l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ unsigned পূর্ণসংখ্যা ext2fs_div_ceil(unsigned পূর্ণসংখ্যা a, unsigned পূর্ণসংখ্যা b)
{
	যদি (!a)
		ফেরত 0;
	ফেরত ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	যদি (!a)
		ফেরত 0;
	ফেরত ((a - 1) / b) + 1;
}

_INLINE_ পূর্ণসংখ্যা ext2fs_dirent_name_len(ধ্রুবক কাঠামো ext2_dir_entry *entry)
{
	ফেরত entry->name_len & 0xff;
}

_INLINE_ শূন্য ext2fs_dirent_set_name_len(কাঠামো ext2_dir_entry *entry, পূর্ণসংখ্যা len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ পূর্ণসংখ্যা ext2fs_dirent_file_type(ধ্রুবক কাঠামো ext2_dir_entry *entry)
{
	ফেরত entry->name_len >> 8;
}

_INLINE_ শূন্য ext2fs_dirent_set_file_type(কাঠামো ext2_dir_entry *entry, পূর্ণসংখ্যা type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ কাঠামো ext2_inode *ext2fs_inode(কাঠামো ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ফেরত (কাঠামো ext2_inode *) large_inode;
}

_INLINE_ ধ্রুবক কাঠামো ext2_inode *
ext2fs_const_inode(ধ্রুবক কাঠামো ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ফেরত (ধ্রুবক কাঠামো ext2_inode *) large_inode;
}

_INLINE_ পূর্ণসংখ্যা ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	ফেরত (fs->blocksize - আকার(কাঠামো ext4_orphan_block_লেজ)) /
		আকার(__u32);
}

_INLINE_ কাঠামো ext4_orphan_block_লেজ *
ext2fs_orphan_block_লেজ(ext2_filsys fs, অক্ষর *buf)
{
	ফেরত (কাঠামো ext4_orphan_block_লেজ *)(buf + fs->blocksize -
		আকার(কাঠামো ext4_orphan_block_লেজ));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা ext2_dir_htree_level(ext2_filsys fs)
{
	যদি (ext2fs_has_feature_largedir(fs->super))
		ফেরত EXT4_HTREE_LEVEL;

	ফেরত EXT4_HTREE_LEVEL_COMPAT;
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
স্থির অন্তঃরেখ শূন্য ext2fs_set_dtime(ext2_filsys fs, কাঠামো ext2_inode *inode)
{
	__u32	t;

	যদি (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	অন্যথা
		t = time(ফাঁকা) & 0xFFFFFFFF;
	যদি (t < fs->super->s_inodes_গুনতি)
		t = fs->super->s_inodes_গুনতি;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
