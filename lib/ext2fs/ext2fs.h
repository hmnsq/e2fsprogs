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
#وگرنه
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#اگر __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#وگرنه
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#وگرنه
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#وگرنه
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
بیرونی "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#اگر (!defined(__GNUC__) && !defined(__WATCOMC__))
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
#شامل <sys/types.h>
#endif

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <time.h>
#شامل <errno.h>

#اگر EXT2_FLAT_INCLUDES
#شامل "e2_types.h"
#شامل "ext2_fs.h"
#شامل "ext3_extents.h"
#وگرنه
#شامل <ext2fs/ext2_types.h>
#شامل <ext2fs/ext2_fs.h>
#شامل <ext2fs/ext3_extents.h>
#endif /* EXT2_FLAT_INCLUDES */

تعریفنوع __u32 __bitwise		ext2_ino_t;
تعریفنوع __u32 __bitwise		blk_t;
تعریفنوع __u64 __bitwise		blk64_t;
تعریفنوع __u32 __bitwise		dgrp_t;
تعریفنوع __s32 __bitwise		ext2_off_t;
تعریفنوع __s64 __bitwise		ext2_off64_t;
تعریفنوع __s64 __bitwise		e2_blkcnt_t;
تعریفنوع __u32 __bitwise		ext2_dirhash_t;

#اگر EXT2_FLAT_INCLUDES
#شامل "com_err.h"
#شامل "ext2_io.h"
#شامل "ext2_err.h"
#شامل "ext2_ext_attr.h"
#وگرنه
#شامل <et/com_err.h>
#شامل <ext2fs/ext2_io.h>
#شامل <ext2fs/ext2_err.h>
#شامل <ext2fs/ext2_ext_attr.h>
#endif

#شامل "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE صحیح __cdecl
#وگرنه
#define EXT2_QSORT_TYPE صحیح
#endif

تعریفنوع ساختار struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

ساختار ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

تعریفنوع ساختار ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
تعریفنوع ساختار ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
تعریفنوع ساختار ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

تعریفنوع ساختار ext2_struct_u32_list *ext2_badblocks_list;
تعریفنوع ساختار ext2_struct_u32_iterate *ext2_badblocks_iterate;

تعریفنوع ساختار ext2_struct_u32_list *ext2_u32_list;
تعریفنوع ساختار ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
تعریفنوع ساختار ext2_struct_u32_list *badblocks_list;
تعریفنوع ساختار ext2_struct_u32_iterate *badblocks_iterate;

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ساختار ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ساختار ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	صحیح	blockcnt;
};

تعریفنوع ساختار ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

تعریفنوع ساختار ext2_file *ext2_file_t;

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

ساختار blk_alloc_ctx;
ساختار opaque_ext2_group_desc;

ساختار struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	صحیح				flags;
	نویسه *				device_name;
	ساختار ext2_super_block	* 	super;
	unsigned صحیح			blocksize;
	صحیح				fragsize;
	dgrp_t				group_desc_شمار;
	unsigned long			desc_blocks;
	ساختار opaque_ext2_group_desc *	group_desc;
	unsigned صحیح			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ساختار ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ساختار ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ساختار ext2_super_block *	orig_super;
	ساختار ext2_image_hdr *		image_header;
	__u32				umask;
	time_t				now;
	صحیح				cluster_ratio_bits;
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
	تهی *				priv_data;

	/*
	 * Inode cache
	 */
	ساختار ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ساختار blk_alloc_ctx *ctx);
	تهی (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, صحیح inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	تهی *mmp_buf;
	تهی *mmp_cmp;
	صحیح mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* progress operation functions */
	ساختار ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	نویسه				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	تهی (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t رقم,
					صحیح inuse);

	/* hashmap for SHA of data blocks */
	ساختار ext2fs_hashmap* block_sha_map;

	ثابت ساختار ext2fs_nls_table *encoding;
};

#اگر EXT2_FLAT_INCLUDES
#شامل "e2_bitops.h"
#وگرنه
#شامل <ext2fs/bitops.h>
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
#define BLOCK_شمار_IND		(-1)
#define BLOCK_شمار_DIND	(-2)
#define BLOCK_شمار_TIND	(-3)
#define BLOCK_شمار_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_DATA	1
#define BLOCK_ALLOC_METADATA	2

ساختار blk_alloc_ctx {
	ext2_ino_t		ino;
	ساختار ext2_inode	*inode;
	blk64_t			lblk;
	صحیح			flags;
};

#اگر 0
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

ساختار ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

تعریفنوع ساختار ext2_extent_handle *ext2_extent_handle_t;
تعریفنوع ساختار ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_ریشه	0x0001
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
#define EXT2_EXTENT_درج_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_درج_NOSPLIT	0x0002 /* insert may not cause split */

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
ساختار ext2_extent_info {
	صحیح		curr_entry;
	صحیح		curr_level;
	صحیح		رقم_entries;
	صحیح		max_entries;
	صحیح		max_depth;
	صحیح		bytes_avail;
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
تعریفنوع ساختار ext2_struct_inode_scan *ext2_inode_scan;

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
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_بالا << 32))

ایستا درونخطی __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#اگر (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	بازگشت extra | (nsec << EXT4_EPOCH_BITS);
}
ایستا درونخطی time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#اگر (SIZEOF_TIME_T > 4)
	اگر (extra & EXT4_EPOCH_MASK)
		seconds += ((time_t)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	بازگشت seconds;
}
ایستا درونخطی __u32 __decode_extra_nsec(__u32 extra)
{
	بازگشت (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_size(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_SIZE +				      \
		  (اندازه(*inode) > EXT2_GOOD_OLD_INODE_SIZE ?		      \
		   ((ساختار ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
انجامبده {									      \
	اگر (ext2fs_inode_includes(ext2fs_inode_actual_size(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((ساختار ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} وگرنه {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} وقتی (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_size(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((ساختار ext2_inode_large *)(inode))->field ## _extra) :      \
		(time_t)(inode)->field)

ایستا درونخطی تهی __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#اگر (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#وگرنه
	*hi = 0;
#endif
}
ایستا درونخطی time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#اگر (SIZEOF_TIME_T == 4)
	بازگشت *lo;
#وگرنه
	بازگشت ((time_t)(*hi) << 32) | *lo;
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

تعریفنوع ساختار ext2_icount *ext2_icount_t;

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

#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)

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
#وگرنه
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
#define EXT2_FS_رقم_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#اگر defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
تعریفنوع ساختار stat64 ext2fs_struct_stat;
#وگرنه
تعریفنوع ساختار stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ساختار ext2_xattr_handle;
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
ایستا درونخطی صحیح ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	بازگشت ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ایستا درونخطی صحیح ext2fs_needs_large_file_feature(unsigned long long file_size)
{
	بازگشت file_size >= 0x80000000ULL;
}

/* alloc.c */
بیرونی تهی ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
بیرونی errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, صحیح mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
بیرونی errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
بیرونی errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
بیرونی errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   ساختار blk_alloc_ctx *ctx);
بیرونی errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, صحیح رقم,
					ext2fs_block_bitmap map,
					blk_t *ret);
بیرونی errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, صحیح رقم,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
بیرونی errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    نویسه *block_buf, blk_t *ret);
بیرونی errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     نویسه *block_buf, blk64_t *ret);
بیرونی errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     نویسه *block_buf, blk64_t *ret,
				     ساختار blk_alloc_ctx *ctx);

بیرونی تهی ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ساختار ext2_inode *inode, blk64_t lblk);
بیرونی تهی ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
بیرونی تهی ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	تهی (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحیح inuse),
	تهی (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحیح inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_LENGTH	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
بیرونی صحیح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
بیرونی errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
بیرونی تهی ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
						  تهی (*func)(ext2_filsys fs,
							       blk64_t blk,
							       صحیح inuse),
						  تهی (**old)(ext2_filsys fs,
							       blk64_t blk,
							       صحیح inuse));

/* alloc_stats.c */
تهی ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحیح inuse);
تهی ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحیح inuse, صحیح isdir);
تهی ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, صحیح inuse);
تهی ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, صحیح inuse);
تهی ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحیح inuse);

/* alloc_tables.c */
بیرونی errcode_t ext2fs_allocate_tables(ext2_filsys fs);
بیرونی errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
بیرونی errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحیح size);
بیرونی errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
بیرونی صحیح ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
بیرونی صحیح ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
بیرونی errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
بیرونی صحیح ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
بیرونی تهی ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
بیرونی errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
بیرونی صحیح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

بیرونی errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    صحیح size);
بیرونی errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
					   blk_t blk);
بیرونی صحیح ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
بیرونی صحیح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
بیرونی تهی ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
بیرونی errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
بیرونی صحیح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
بیرونی تهی ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
بیرونی errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
بیرونی صحیح ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
بیرونی صحیح ext2fs_u32_list_شمار(ext2_u32_list bb);

/* bb_compat */
بیرونی errcode_t badblocks_list_create(badblocks_list *ret, صحیح size);
بیرونی errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
بیرونی صحیح badblocks_list_test(badblocks_list bb, blk_t blk);
بیرونی errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
بیرونی صحیح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
بیرونی تهی badblocks_list_iterate_end(badblocks_iterate iter);
بیرونی تهی badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
بیرونی errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
بیرونی تهی ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
بیرونی تهی ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
بیرونی errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
بیرونی errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      ثابت نویسه *descr,
					      ext2fs_block_bitmap *ret);
بیرونی errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ثابت نویسه *descr,
						   ext2fs_block_bitmap *ret);
بیرونی صحیح ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
بیرونی errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ثابت نویسه *descr,
					      ext2fs_inode_bitmap *ret);
بیرونی errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
بیرونی errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
بیرونی errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
بیرونی تهی ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
بیرونی تهی ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
بیرونی errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
بیرونی errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
بیرونی errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
بیرونی errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
بیرونی errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
بیرونی errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
بیرونی errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned صحیح رقم,
					تهی *in);
بیرونی errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t رقم,
					 تهی *in);
بیرونی errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned صحیح رقم,
					تهی *out);
بیرونی errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t رقم,
					 تهی *out);
بیرونی errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned صحیح رقم,
					تهی *in);
بیرونی errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t رقم,
					 تهی *in);
بیرونی errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned صحیح رقم,
					تهی *out);
بیرونی errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t رقم,
					 تهی *out);

/* blknum.c */
بیرونی __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
بیرونی __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
بیرونی dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
بیرونی blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
بیرونی blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
بیرونی صحیح ext2fs_group_blocks_شمار(ext2_filsys fs, dgrp_t group);
بیرونی blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					 ساختار ext2_inode *inode);
بیرونی blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     ساختار ext2_inode *inode);
بیرونی blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					ساختار ext2_inode *inode);
بیرونی blk64_t ext2fs_blocks_شمار(ساختار ext2_super_block *super);
بیرونی تهی ext2fs_blocks_شمار_set(ساختار ext2_super_block *super,
				    blk64_t blk);
بیرونی تهی ext2fs_blocks_شمار_add(ساختار ext2_super_block *super,
				    blk64_t blk);
بیرونی blk64_t ext2fs_r_blocks_شمار(ساختار ext2_super_block *super);
بیرونی تهی ext2fs_r_blocks_شمار_set(ساختار ext2_super_block *super,
				      blk64_t blk);
بیرونی تهی ext2fs_r_blocks_شمار_add(ساختار ext2_super_block *super,
				      blk64_t blk);
بیرونی blk64_t ext2fs_free_blocks_شمار(ساختار ext2_super_block *super);
بیرونی تهی ext2fs_free_blocks_شمار_set(ساختار ext2_super_block *super,
					 blk64_t blk);
بیرونی تهی ext2fs_free_blocks_شمار_add(ساختار ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
بیرونی ساختار ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  ساختار opaque_ext2_group_desc *gdp,
					  dgrp_t group);
بیرونی blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
بیرونی blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
بیرونی __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
بیرونی blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
بیرونی blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
بیرونی __u32 ext2fs_bg_free_blocks_شمار(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_free_blocks_شمار_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
بیرونی __u32 ext2fs_bg_free_inodes_شمار(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_free_inodes_شمار_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
بیرونی __u32 ext2fs_bg_used_dirs_شمار(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_used_dirs_شمار_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
بیرونی __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
بیرونی __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
بیرونی صحیح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
بیرونی تهی ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
بیرونی تهی ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
بیرونی __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
بیرونی تهی ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
بیرونی blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     ثابت ساختار ext2_inode *inode);
بیرونی تهی ext2fs_file_acl_block_set(ext2_filsys fs,
				      ساختار ext2_inode *inode, blk64_t blk);
بیرونی errcode_t ext2fs_inode_size_set(ext2_filsys fs, ساختار ext2_inode *inode,
				       ext2_off64_t size);

/* block.c */
بیرونی errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      صحیح	flags,
				      نویسه *block_buf,
				      صحیح (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  صحیح	blockcnt,
						  تهی	*priv_data),
				      تهی *priv_data);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				صحیح	flags,
				نویسه *block_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحیح		ref_offset,
					    تهی	*priv_data),
				تهی *priv_data);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				نویسه *block_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحیح		ref_offset,
					    تهی	*priv_data),
				تهی *priv_data);

/* bmap.c */
بیرونی errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ساختار ext2_inode *inode,
			     نویسه *block_buf, صحیح bmap_flags,
			     blk_t block, blk_t *phys_blk);
بیرونی errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ساختار ext2_inode *inode,
			      نویسه *block_buf, صحیح bmap_flags, blk64_t block,
			      صحیح *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   ساختار ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#اگر 0
/* bmove.c */
بیرونی errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    صحیح flags);
#endif

/* check_desc.c */
بیرونی errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
بیرونی errcode_t ext2fs_close(ext2_filsys fs);
بیرونی errcode_t ext2fs_close2(ext2_filsys fs, صحیح flags);
بیرونی errcode_t ext2fs_close_free(ext2_filsys *fs);
بیرونی errcode_t ext2fs_flush(ext2_filsys fs);
بیرونی errcode_t ext2fs_flush2(ext2_filsys fs, صحیح flags);
بیرونی صحیح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
بیرونی errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
بیرونی صحیح ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    صحیح *ret_meta_bg);
بیرونی تهی ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
بیرونی __u32 ext2fs_crc32_be(__u32 crc, unsigned نویسه ثابت *p, size_t len);
بیرونی __u32 ext2fs_crc32c_le(__u32 crc, unsigned نویسه ثابت *p, size_t len);

/* csum.c */
بیرونی تهی ext2fs_init_csum_seed(ext2_filsys fs);
بیرونی errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ساختار mmp_struct *mmp);
بیرونی صحیح ext2fs_mmp_csum_verify(ext2_filsys, ساختار mmp_struct *mmp);
بیرونی صحیح ext2fs_verify_csum_type(ext2_filsys fs, ساختار ext2_super_block *sb);
بیرونی errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    ساختار ext2_super_block *sb);
بیرونی صحیح ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ساختار ext2_super_block *sb);
بیرونی errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					ساختار ext2_ext_attr_header *hdr);
بیرونی صحیح ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     ساختار ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_TAIL(block, blocksize) \
	((ساختار ext2_dir_entry_tail *)(((نویسه *)(block)) + \
	(blocksize) - اندازه(ساختار ext2_dir_entry_tail)))

بیرونی تهی ext2fs_initialize_dirent_tail(ext2_filsys fs,
					  ساختار ext2_dir_entry_tail *t);
بیرونی صحیح ext2fs_dirent_has_tail(ext2_filsys fs,
				  ساختار ext2_dir_entry *dirent);
بیرونی صحیح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ساختار ext2_dir_entry *dirent);
بیرونی صحیح ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ساختار ext2_dir_entry *dirent);
بیرونی errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   ساختار ext2_dir_entry *dirent);
بیرونی errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ساختار ext2_dir_entry *dirent,
					  ساختار ext2_dx_countlimit **cc,
					  صحیح *offset);
بیرونی errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ساختار ext2_dir_entry *dirent,
				__u32 *crc, ساختار ext2_dx_tail **ret_t);
بیرونی errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      ساختار ext3_extent_header *eh);
بیرونی صحیح ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ساختار ext3_extent_header *eh);
بیرونی errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      نویسه *bitmap, صحیح size);
بیرونی صحیح ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   نویسه *bitmap, صحیح size);
بیرونی errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      نویسه *bitmap, صحیح size);
بیرونی صحیح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   نویسه *bitmap, صحیح size);
بیرونی errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ساختار ext2_inode_large *inode);
بیرونی صحیح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ساختار ext2_inode_large *inode);
بیرونی تهی ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
بیرونی صحیح ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
بیرونی errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
بیرونی __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
بیرونی errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
بیرونی errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحیح blockcnt);
بیرونی errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
بیرونی تهی ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ثابت تهی *,
							   ثابت تهی *));
بیرونی تهی ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ثابت تهی *,
							    ثابت تهی *));
بیرونی errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساختار ext2_db_entry *db_info,
		    تهی	*priv_data),
	تهی *priv_data);
بیرونی errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساختار ext2_db_entry2 *db_info,
		    تهی	*priv_data),
	تهی *priv_data);
بیرونی errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	صحیح (*func)(ext2_filsys fs, ساختار ext2_db_entry2 *db_info,
		    تهی	*priv_data),
	unsigned long long start,
	unsigned long long شمار,
	تهی *priv_data);
بیرونی errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحیح blockcnt);
بیرونی errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
بیرونی errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
بیرونی صحیح ext2fs_dblist_شمار(ext2_dblist dblist);
بیرونی blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
بیرونی errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					ساختار ext2_db_entry **entry);
بیرونی errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ساختار ext2_db_entry2 **entry);
بیرونی errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
بیرونی errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  صحیح	flags,
				  نویسه	*block_buf,
				  صحیح (*func)(ext2_ino_t	dir,
					      صحیح		entry,
					      ساختار ext2_dir_entry *dirent,
					      صحیح	offset,
					      صحیح	blocksize,
					      نویسه	*buf,
					      تهی	*priv_data),
				  تهی *priv_data);

#اگر 0
/* digest_encode.c */
#define EXT2FS_DIGEST_SIZE EXT2FS_SHA256_LENGTH
بیرونی صحیح ext2fs_digest_encode(ثابت نویسه *src, صحیح len, نویسه *dst);
بیرونی صحیح ext2fs_digest_decode(ثابت نویسه *src, صحیح len, نویسه *dst);
#endif

/* dirblock.c */
بیرونی errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       تهی *buf);
بیرونی errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					تهی *buf, صحیح flags);
بیرونی errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					تهی *buf, صحیح flags);
بیرونی errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					تهی *buf, صحیح flags, ext2_ino_t ino);
بیرونی errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					تهی *buf);
بیرونی errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 تهی *buf, صحیح flags);
بیرونی errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 تهی *buf, صحیح flags);
بیرونی errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 تهی *buf, صحیح flags, ext2_ino_t ino);

/* dirhash.c */
بیرونی errcode_t ext2fs_dirhash(صحیح version, ثابت نویسه *name, صحیح len,
				ثابت __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

بیرونی errcode_t ext2fs_dirhash2(صحیح version, ثابت نویسه *name, صحیح len,
				 ثابت ساختار ext2fs_nls_table *charset,
				 صحیح hash_flags,
				 ثابت __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
بیرونی errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ساختار ext2_dir_entry *dirent,
				    unsigned صحیح *rec_len);
بیرونی errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    unsigned صحیح len,
				    ساختار ext2_dir_entry *dirent);
بیرونی errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      نویسه *block_buf,
			      صحیح (*func)(ساختار ext2_dir_entry *dirent,
					  صحیح	offset,
					  صحیح	blocksize,
					  نویسه	*buf,
					  تهی	*priv_data),
			      تهی *priv_data);
بیرونی errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      نویسه *block_buf,
			      صحیح (*func)(ext2_ino_t	dir,
					  صحیح	entry,
					  ساختار ext2_dir_entry *dirent,
					  صحیح	offset,
					  صحیح	blocksize,
					  نویسه	*buf,
					  تهی	*priv_data),
			      تهی *priv_data);

/* dupfs.c */
بیرونی errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
بیرونی errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
بیرونی __u32 ext2fs_ext_attr_hash_entry(ساختار ext2_ext_attr_entry *entry,
					تهی *data);
بیرونی __u32 ext2fs_ext_attr_hash_entry_signed(ساختار ext2_ext_attr_entry *entry,
					       تهی *data);
بیرونی errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ساختار ext2_ext_attr_entry *entry,
					     تهی *data, __u32 *hash);
بیرونی errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ساختار ext2_ext_attr_entry *entry,
					     تهی *data, __u32 *hash,
					     __u32 *signed_hash);
بیرونی errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, تهی *buf);
بیرونی errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       تهی *buf);
بیرونی errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       تهی *buf, ext2_ino_t inum);
بیرونی errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       تهی *buf);
بیرونی errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       تهی *buf);
بیرونی errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       تهی *buf, ext2_ino_t inum);
بیرونی errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   نویسه *block_buf,
					   صحیح adjust, __u32 *newcount);
بیرونی errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   نویسه *block_buf,
					   صحیح adjust, __u32 *newcount);
بیرونی errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   نویسه *block_buf,
					   صحیح adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ساختار ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ساختار ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ساختار ext2_xattr_handle *handle,
				   ساختار ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ساختار ext2_xattr_handle *h,
				صحیح (*func)(نویسه *name, نویسه *مقدار,
					    size_t مقدار_len, تهی *data),
				تهی *data);
errcode_t ext2fs_xattr_get(ساختار ext2_xattr_handle *h, ثابت نویسه *کلید,
			   تهی **مقدار, size_t *مقدار_len);
errcode_t ext2fs_xattr_set(ساختار ext2_xattr_handle *handle,
			   ثابت نویسه *کلید,
			   ثابت تهی *مقدار,
			   size_t مقدار_len);
errcode_t ext2fs_xattr_remove(ساختار ext2_xattr_handle *handle,
			      ثابت نویسه *کلید);
errcode_t ext2fs_xattr_remove_all(ساختار ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ساختار ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ساختار ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ساختار ext2_inode_large *inode);
errcode_t ext2fs_xattrs_شمار(ساختار ext2_xattr_handle *handle, size_t *شمار);
errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ساختار ext2_xattr_handle *handle,
			      unsigned صحیح *new_flags, unsigned صحیح *old_flags);
بیرونی تهی ext2fs_ext_attr_block_rehash(ساختار ext2_ext_attr_header *header,
					 ساختار ext2_ext_attr_entry *end);
بیرونی __u32 ext2fs_get_ea_inode_hash(ساختار ext2_inode *inode);
بیرونی تهی ext2fs_set_ea_inode_hash(ساختار ext2_inode *inode, __u32 hash);
بیرونی __u64 ext2fs_get_ea_inode_ref(ساختار ext2_inode *inode);
بیرونی تهی ext2fs_set_ea_inode_ref(ساختار ext2_inode *inode, __u64 ref_شمار);

/* extent.c */
بیرونی errcode_t ext2fs_extent_header_verify(تهی *ptr, صحیح size);
بیرونی errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
بیرونی errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ساختار ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
بیرونی تهی ext2fs_extent_free(ext2_extent_handle_t handle);
بیرونی errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   صحیح flags, ساختار ext2fs_extent *extent);
بیرونی errcode_t ext2fs_extent_گره_split(ext2_extent_handle_t handle);
بیرونی errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, صحیح flags,
				       ساختار ext2fs_extent *extent);
بیرونی errcode_t ext2fs_extent_درج(ext2_extent_handle_t handle, صحیح flags,
				      ساختار ext2fs_extent *extent);
بیرونی errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					صحیح flags);
بیرونی errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحیح flags);
بیرونی errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ساختار ext2_extent_info *info);
بیرونی errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
بیرونی errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     صحیح leaf_level, blk64_t blk);
بیرونی errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
بیرونی size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle);
بیرونی errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ساختار ext2_inode *inode);
بیرونی errcode_t ext2fs_شمار_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ساختار ext2_inode *inode, blk64_t *ret_شمار);
بیرونی errcode_t ext2fs_decode_extent(ساختار ext2fs_extent *to, تهی *from,
				      صحیح len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
			   ساختار ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
بیرونی errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ساختار ext2_inode *inode,
				   صحیح flags, ext2_file_t *ret);
بیرونی errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  صحیح flags, ext2_file_t *ret);
بیرونی ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ساختار ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
بیرونی ext2_ino_t ext2fs_file_get_inode_رقم(ext2_file_t file);
بیرونی errcode_t ext2fs_file_close(ext2_file_t file);
بیرونی errcode_t ext2fs_file_flush(ext2_file_t file);
بیرونی errcode_t ext2fs_file_read(ext2_file_t file, تهی *buf,
				  unsigned صحیح wanted, unsigned صحیح *got);
بیرونی errcode_t ext2fs_file_write(ext2_file_t file, ثابت تهی *buf,
				   unsigned صحیح nbytes, unsigned صحیح *written);
بیرونی errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   صحیح whence, __u64 *ret_pos);
بیرونی errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   صحیح whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size);
بیرونی ext2_off_t ext2fs_file_get_size(ext2_file_t file);
بیرونی errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size);
بیرونی errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size);

/* finddev.c */
بیرونی نویسه *ext2fs_find_block_device(dev_t device);

/* flushb.c */
بیرونی errcode_t ext2fs_sync_device(صحیح fd, صحیح flushb);

/* freefs.c */
بیرونی تهی ext2fs_free(ext2_filsys fs);
بیرونی تهی ext2fs_free_dblist(ext2_dblist dblist);
بیرونی تهی ext2fs_badblocks_list_free(ext2_badblocks_list bb);
بیرونی تهی ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
بیرونی تهی ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
بیرونی errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 end,
					    __u32 real_end,
					    ثابت نویسه *descr, نویسه *init_map,
					    ext2fs_generic_bitmap *ret);
بیرونی errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						ثابت نویسه *descr,
						ext2fs_generic_bitmap *ret);
بیرونی errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
بیرونی تهی ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
بیرونی errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
بیرونی تهی ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
بیرونی errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
بیرونی errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
بیرونی errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 رقم,
						 تهی *out);
بیرونی errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 رقم,
						 تهی *in);
بیرونی errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
بیرونی errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
تهی ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحیح type, __u64 start, __u64 end,
				    __u64 real_end,
				    ثابت نویسه *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
تهی ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
تهی ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned صحیح رقم,
					تهی *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned صحیح رقم,
					تهی *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_شمار_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_شمار_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
بیرونی unsigned صحیح ext2fs_list_backups(ext2_filsys fs, unsigned صحیح *three,
				unsigned صحیح *five, unsigned صحیح *seven);

/* getenv.c */
بیرونی نویسه *ext2fs_safe_getenv(ثابت نویسه *arg);

/* get_num_dirs.c */
بیرونی errcode_t ext2fs_get_رقم_dirs(ext2_filsys fs, ext2_ino_t *ret_رقم_dirs);

/* getsize.c */
بیرونی errcode_t ext2fs_get_device_size(ثابت نویسه *file, صحیح blocksize,
					blk_t *retblocks);
بیرونی errcode_t ext2fs_get_device_size2(ثابت نویسه *file, صحیح blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
بیرونی صحیح ext2fs_get_dio_alignment(صحیح fd);
errcode_t ext2fs_get_device_sectsize(ثابت نویسه *file, صحیح *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ثابت نویسه *file, صحیح *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ساختار ext2_inode *inode,
				 blk64_t رقم_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ساختار ext2_inode *inode,
				 blk64_t رقم_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, ساختار ext2_inode *inode, blk64_t b);

/* imager.c */
بیرونی errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحیح fd, صحیح flags);
بیرونی errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحیح fd, صحیح flags);
بیرونی errcode_t ext2fs_image_super_write(ext2_filsys fs, صحیح fd, صحیح flags);
بیرونی errcode_t ext2fs_image_super_read(ext2_filsys fs, صحیح fd, صحیح flags);
بیرونی errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحیح fd, صحیح flags);
بیرونی errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحیح fd, صحیح flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, تهی *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, تهی *buf);

/* initialize.c */
بیرونی errcode_t ext2fs_initialize(ثابت نویسه *name, صحیح flags,
				   ساختار ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
بیرونی errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحیح super_only);

/* icount.c */
بیرونی تهی ext2fs_free_icount(ext2_icount_t icount);
بیرونی errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, نویسه *tdb_dir,
					  صحیح flags, ext2_icount_t *ret);
بیرونی errcode_t ext2fs_create_icount2(ext2_filsys fs, صحیح flags,
				       unsigned صحیح size,
				       ext2_icount_t hint, ext2_icount_t *ret);
بیرونی errcode_t ext2fs_create_icount(ext2_filsys fs, صحیح flags,
				      unsigned صحیح size,
				      ext2_icount_t *ret);
بیرونی errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
بیرونی errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
بیرونی errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
بیرونی errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 شمار);
بیرونی ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

بیرونی errcode_t ext2fs_get_memalign(unsigned long size,
				     unsigned long align, تهی *ptr);

/* inline_data.c */
بیرونی errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino);
بیرونی errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino,
					 size_t *size);
بیرونی errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
					ساختار ext2_inode *inode,
					تهی *buf, size_t *size);
بیرونی errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
					ساختار ext2_inode *inode,
					تهی *buf, size_t size);

/* inode.c */
بیرونی errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned صحیح cache_size);
بیرونی تهی ext2fs_free_inode_cache(ساختار ext2_inode_cache *icache);
بیرونی errcode_t ext2fs_flush_icache(ext2_filsys fs);
بیرونی errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    ساختار ext2_inode *inode,
					    صحیح bufsize);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8
بیرونی errcode_t ext2fs_open_inode_scan(ext2_filsys fs, صحیح buffer_blocks,
				  ext2_inode_scan *ret_scan);
بیرونی تهی ext2fs_close_inode_scan(ext2_inode_scan scan);
بیرونی errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       ساختار ext2_inode *inode);
بیرونی errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   صحیح	group);
بیرونی تهی ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 تهی * priv_data),
	 تهی *done_group_data);
بیرونی صحیح ext2fs_inode_scan_flags(ext2_inode_scan scan, صحیح set_flags,
				   صحیح clear_flags);
بیرونی errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					ساختار ext2_inode * inode,
					صحیح bufsize);
بیرونی errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساختار ext2_inode * inode);
بیرونی errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ساختار ext2_inode * inode,
				    صحیح bufsize, صحیح flags);
بیرونی errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 ساختار ext2_inode * inode,
					 صحیح bufsize);
بیرونی errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساختار ext2_inode * inode);
بیرونی errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ساختار ext2_inode * inode,
				     صحیح bufsize, صحیح flags);
بیرونی errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساختار ext2_inode * inode);
بیرونی errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
بیرونی errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
بیرونی io_manager inode_io_manager;
بیرونی errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					نویسه **name);
بیرونی errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ساختار ext2_inode *inode,
					 نویسه **name);

/* ismounted.c */
بیرونی errcode_t ext2fs_check_if_mounted(ثابت نویسه *file, صحیح *mount_flags);
بیرونی errcode_t ext2fs_check_mount_point(ثابت نویسه *device, صحیح *mount_flags,
					  نویسه *mtpt, صحیح mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
بیرونی errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ساختار ext2_inode *inode,
			      نویسه *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
بیرونی errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت نویسه *name,
			 صحیح namelen, نویسه *buf, ext2_ino_t *inode);
بیرونی errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			ثابت نویسه *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			      ثابت نویسه *name, ext2_ino_t *inode);
بیرونی errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
صحیح ext2fs_native_flag(تهی);

/* newdir.c */
بیرونی errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, نویسه **block);
بیرونی errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
بیرونی ثابت ساختار ext2fs_nls_table *ext2fs_load_nls_table(صحیح encoding);
بیرونی صحیح ext2fs_check_encoded_name(ثابت ساختار ext2fs_nls_table *table,
				     نویسه *s, size_t len, نویسه **pos);
بیرونی صحیح ext2fs_casefold_cmp(ثابت ساختار ext2fs_nls_table *table,
			       ثابت unsigned نویسه *str1, size_t len1,
			       ثابت unsigned نویسه *str2, size_t len2);

/* mkdir.c */
بیرونی errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      ثابت نویسه *name);
بیرونی errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, unsigned long flags,
			       صحیح link_flags, ثابت نویسه *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ساختار ext2fs_journal_params {
	blk_t رقم_journal_blocks;
	blk_t رقم_fc_blocks;
};
بیرونی errcode_t ext2fs_get_journal_params(
		ساختار ext2fs_journal_params *params, ext2_filsys fs);
بیرونی errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, صحیح رقم,
				    blk_t *ret_blk, صحیح *ret_شمار);
بیرونی errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحیح رقم,
				     blk64_t *ret_blk, صحیح *ret_شمار);
بیرونی errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 رقم_blocks, صحیح flags,
						  نویسه  **ret_jsb);
بیرونی errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ساختار ext2fs_journal_params *params,
						  صحیح flags, نویسه  **ret_jsb);
بیرونی errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
بیرونی errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t رقم_blocks,
					  صحیح flags);
بیرونی errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t رقم_blocks,
					   blk64_t goal, صحیح flags);
بیرونی errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    ساختار ext2fs_journal_params *params,
				    blk64_t goal, صحیح flags);
بیرونی صحیح ext2fs_default_journal_size(__u64 رقم_blocks);
بیرونی صحیح ext2fs_journal_sb_start(صحیح blocksize);

/* openfs.c */
بیرونی errcode_t ext2fs_open(ثابت نویسه *name, صحیح flags, صحیح superblock,
			     unsigned صحیح block_size, io_manager manager,
			     ext2_filsys *ret_fs);
بیرونی errcode_t ext2fs_open2(ثابت نویسه *name, ثابت نویسه *io_options,
			      صحیح flags, صحیح superblock,
			      unsigned صحیح block_size, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
بیرونی blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
بیرونی blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
بیرونی errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t رقم_blocks);
بیرونی errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
بیرونی e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
بیرونی __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      نویسه *buf);
بیرونی errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   نویسه *buf);
بیرونی صحیح ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, نویسه *buf);

/* get_pathname.c */
بیرونی errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       نویسه **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت نویسه *name,
		      ext2_ino_t ino, صحیح flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ثابت نویسه *name,
			ext2_ino_t ino, صحیح flags);
صحیح ext2fs_dir_is_dx(ext2_filsys fs, ثابت ساختار ext2_inode *inode);
تهی ext2fs_inc_nlink(ext2_filsys fs, ساختار ext2_inode *inode);
تهی ext2fs_dec_nlink(ساختار ext2_inode *inode);
صحیح ext2fs_dir_link_max(ext2_filsys fs, ساختار ext2_inode_large *inode);
صحیح ext2fs_dir_link_empty(ساختار ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ثابت نویسه *name, ثابت نویسه *target);
صحیح ext2fs_is_fast_symlink(ساختار ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, تهی *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, تهی *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحیح immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
unsigned ext2fs_mmp_new_seq(تهی);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, تهی **ptr);

/* read_bb.c */
بیرونی errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
بیرونی errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      تهی *priv_data,
				      تهی (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      نویسه *badstr,
						      تهی *priv_data));
بیرونی errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     تهی (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
بیرونی errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
بیرونی errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحیح flags, صحیح رقم_threads);
بیرونی errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
بیرونی errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
بیرونی errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
بیرونی errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
بیرونی errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
بیرونی errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2FS_SHA256_LENGTH 32
#اگر 0
بیرونی تهی ext2fs_sha256(ثابت unsigned نویسه *in, unsigned long in_size,
		   unsigned نویسه out[EXT2FS_SHA256_LENGTH]);
#endif

/* sha512.c */
#define EXT2FS_SHA512_LENGTH 64
بیرونی تهی ext2fs_sha512(ثابت unsigned نویسه *in, unsigned long in_size,
			  unsigned نویسه out[EXT2FS_SHA512_LENGTH]);

/* swapfs.c */
بیرونی errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, نویسه *buf, size_t size,
					صحیح flags);
بیرونی errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, نویسه *buf, صحیح flags);
بیرونی errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, نویسه *buf, size_t size,
					 صحیح flags);
بیرونی errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, نویسه *buf, صحیح flags);
بیرونی تهی ext2fs_swap_ext_attr(نویسه *to, نویسه *from, صحیح bufsize,
				 صحیح has_header);
بیرونی تهی ext2fs_swap_ext_attr_header(ساختار ext2_ext_attr_header *to_header,
					ساختار ext2_ext_attr_header *from_hdr);
بیرونی تهی ext2fs_swap_ext_attr_entry(ساختار ext2_ext_attr_entry *to_entry,
				       ساختار ext2_ext_attr_entry *from_entry);
بیرونی تهی ext2fs_swap_super(ساختار ext2_super_block * super);
بیرونی تهی ext2fs_swap_group_desc(ساختار ext2_group_desc *gdp);
بیرونی تهی ext2fs_swap_group_desc2(ext2_filsys, ساختار ext2_group_desc *gdp);
بیرونی تهی ext2fs_swap_inode_full(ext2_filsys fs, ساختار ext2_inode_large *t,
				   ساختار ext2_inode_large *f, صحیح hostorder,
				   صحیح bufsize);
بیرونی تهی ext2fs_swap_inode(ext2_filsys fs,ساختار ext2_inode *t,
			      ساختار ext2_inode *f, صحیح hostorder);
بیرونی تهی ext2fs_swap_mmp(ساختار mmp_struct *mmp);

/* unix_io.c */
بیرونی صحیح ext2fs_open_file(ثابت نویسه *pathname, صحیح flags, mode_t mode);
بیرونی صحیح ext2fs_stat(ثابت نویسه *path, ext2fs_struct_stat *buf);
بیرونی صحیح ext2fs_fstat(صحیح fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
بیرونی صحیح ext2fs_inode_has_valid_blocks(ساختار ext2_inode *inode);
بیرونی صحیح ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ساختار ext2_inode *inode);

/* version.c */
بیرونی صحیح ext2fs_parse_version_string(ثابت نویسه *ver_string);
بیرونی صحیح ext2fs_get_library_version(ثابت نویسه **ver_string,
				      ثابت نویسه **date_string);

/* write_bb_file.c */
بیرونی errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      unsigned صحیح flags,
				      FILE *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
بیرونی errcode_t ext2fs_get_mem(unsigned long size, تهی *ptr);
بیرونی errcode_t ext2fs_get_memzero(unsigned long size, تهی *ptr);
بیرونی errcode_t ext2fs_get_array(unsigned long شمار,
				  unsigned long size, تهی *ptr);
بیرونی errcode_t ext2fs_get_arrayzero(unsigned long شمار,
				      unsigned long size, تهی *ptr);
بیرونی errcode_t ext2fs_free_mem(تهی *ptr);
بیرونی errcode_t ext2fs_resize_mem(unsigned long old_size,
				   unsigned long size, تهی *ptr);
بیرونی errcode_t ext2fs_resize_array(unsigned long old_شمار, unsigned long شمار,
				     unsigned long size, تهی *ptr);
بیرونی تهی ext2fs_mark_super_dirty(ext2_filsys fs);
بیرونی تهی ext2fs_mark_changed(ext2_filsys fs);
بیرونی صحیح ext2fs_test_changed(ext2_filsys fs);
بیرونی تهی ext2fs_mark_valid(ext2_filsys fs);
بیرونی تهی ext2fs_unmark_valid(ext2_filsys fs);
بیرونی صحیح ext2fs_test_valid(ext2_filsys fs);
بیرونی تهی ext2fs_mark_ib_dirty(ext2_filsys fs);
بیرونی تهی ext2fs_mark_bb_dirty(ext2_filsys fs);
بیرونی صحیح ext2fs_test_ib_dirty(ext2_filsys fs);
بیرونی صحیح ext2fs_test_bb_dirty(ext2_filsys fs);
بیرونی dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
بیرونی dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
بیرونی blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
بیرونی blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
بیرونی blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
				      ساختار ext2_inode *inode);
بیرونی صحیح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحیح blocks);
بیرونی صحیح ext2fs_log2_u32(__u32 arg);
بیرونی صحیح ext2fs_log2_u64(__u64 arg);
بیرونی صحیح ext2fs_log10_u32(__u32 arg);
بیرونی صحیح ext2fs_log10_u64(__u64 arg);
بیرونی unsigned صحیح ext2fs_div_ceil(unsigned صحیح a, unsigned صحیح b);
بیرونی __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
بیرونی صحیح ext2fs_dirent_name_len(ثابت ساختار ext2_dir_entry *entry);
بیرونی تهی ext2fs_dirent_set_name_len(ساختار ext2_dir_entry *entry, صحیح len);
بیرونی صحیح ext2fs_dirent_file_type(ثابت ساختار ext2_dir_entry *entry);
بیرونی تهی ext2fs_dirent_set_file_type(ساختار ext2_dir_entry *entry, صحیح type);
بیرونی ساختار ext2_inode *ext2fs_inode(ساختار ext2_inode_large * large_inode);
بیرونی ثابت ساختار ext2_inode *ext2fs_const_inode(ثابت ساختار ext2_inode_large * large_inode);
بیرونی صحیح ext2fs_inodes_per_orphan_block(ext2_filsys fs);
بیرونی ساختار ext4_orphan_block_tail *ext2fs_orphan_block_tail(ext2_filsys fs,
							       نویسه *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#اگر (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ بیرونی
#وگرنه
#اگر (__STDC_VERSION__ >= 199901L)
#define _INLINE_ درونخطی
#وگرنه
#ifdef __GNUC__
#define _INLINE_ بیرونی __inline__
#وگرنه				/* For Watcom C */
#define _INLINE_ بیرونی درونخطی
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#شامل <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long size, تهی *ptr)
{
	تهی *pp;

	pp = malloc(size);
	اگر (!pp)
		بازگشت EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, اندازه (pp));
	بازگشت 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long size, تهی *ptr)
{
	تهی *pp;

	pp = malloc(size);
	اگر (!pp)
		بازگشت EXT2_ET_NO_MEMORY;
	memset(pp, 0, size);
	memcpy(ptr, &pp, اندازه(pp));
	بازگشت 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long شمار, unsigned long size,
				    تهی *ptr)
{
	اگر (شمار && (~0UL)/شمار < size)
		بازگشت EXT2_ET_NO_MEMORY;
	بازگشت ext2fs_get_mem(شمار*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long شمار,
					unsigned long size, تهی *ptr)
{
	اگر (شمار && (~0UL)/شمار < size)
		بازگشت EXT2_ET_NO_MEMORY;

	بازگشت ext2fs_get_memzero((size_t)شمار * size, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(تهی *ptr)
{
	تهی *p;

	memcpy(&p, ptr, اندازه(p));
	آزاد(p);
	p = 0;
	memcpy(ptr, &p, اندازه(p));
	بازگشت 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_size,
				     unsigned long size, تهی *ptr)
{
	تهی *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, اندازه(p));
	p = realloc(p, size);
	اگر (!p)
		بازگشت EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, اندازه(p));
	بازگشت 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(unsigned long size,
				       unsigned long old_شمار,
				       unsigned long شمار, تهی *ptr)
{
	unsigned long old_size;
	errcode_t retval;

	اگر (شمار && (~0UL)/شمار < size)
		بازگشت EXT2_ET_NO_MEMORY;

	size *= شمار;
	old_size = size * old_شمار;
	retval = ext2fs_resize_mem(old_size, size, ptr);
	اگر (retval)
		بازگشت retval;

	اگر (size > old_size) {
		تهی *p;

		memcpy(&p, ptr, اندازه(p));
		memset((نویسه *)p + old_size, 0, size - old_size);
		memcpy(ptr, &p, اندازه(p));
	}

	بازگشت 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ تهی ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ تهی ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ صحیح ext2fs_test_changed(ext2_filsys fs)
{
	بازگشت (fs->flags & EXT2_FLAG_CHANGED);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ تهی ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ تهی ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ صحیح ext2fs_test_valid(ext2_filsys fs)
{
	بازگشت (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ تهی ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ تهی ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ صحیح ext2fs_test_ib_dirty(ext2_filsys fs)
{
	بازگشت (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ صحیح ext2fs_test_bb_dirty(ext2_filsys fs)
{
	بازگشت (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	بازگشت ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	بازگشت (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	بازگشت (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	بازگشت (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					ساختار ext2_inode *inode)
{
	بازگشت (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

_INLINE_ صحیح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحیح blocks)
{
	صحیح csum_size = 0;

	اگر ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_size = اندازه(ساختار ext2_dx_tail);
	بازگشت blocks * ((fs->blocksize - (8 + csum_size)) /
						اندازه(ساختار ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحیح ext2fs_log2_u32(__u32 arg)
{
	صحیح l = 0;

	arg >>= 1;
	وقتی (arg) {
		l++;
		arg >>= 1;
	}
	بازگشت l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ صحیح ext2fs_log2_u64(__u64 arg)
{
	صحیح l = 0;

	arg >>= 1;
	وقتی (arg) {
		l++;
		arg >>= 1;
	}
	بازگشت l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحیح ext2fs_log10_u32(__u32 arg)
{
	صحیح l = 0;

	arg /= 10;
	وقتی (arg) {
		l++;
		arg /= 10;
	}
	بازگشت l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ صحیح ext2fs_log10_u64(__u64 arg)
{
	صحیح l = 0;

	arg /= 10;
	وقتی (arg) {
		l++;
		arg /= 10;
	}
	بازگشت l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ unsigned صحیح ext2fs_div_ceil(unsigned صحیح a, unsigned صحیح b)
{
	اگر (!a)
		بازگشت 0;
	بازگشت ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	اگر (!a)
		بازگشت 0;
	بازگشت ((a - 1) / b) + 1;
}

_INLINE_ صحیح ext2fs_dirent_name_len(ثابت ساختار ext2_dir_entry *entry)
{
	بازگشت entry->name_len & 0xff;
}

_INLINE_ تهی ext2fs_dirent_set_name_len(ساختار ext2_dir_entry *entry, صحیح len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ صحیح ext2fs_dirent_file_type(ثابت ساختار ext2_dir_entry *entry)
{
	بازگشت entry->name_len >> 8;
}

_INLINE_ تهی ext2fs_dirent_set_file_type(ساختار ext2_dir_entry *entry, صحیح type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ ساختار ext2_inode *ext2fs_inode(ساختار ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	بازگشت (ساختار ext2_inode *) large_inode;
}

_INLINE_ ثابت ساختار ext2_inode *
ext2fs_const_inode(ثابت ساختار ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	بازگشت (ثابت ساختار ext2_inode *) large_inode;
}

_INLINE_ صحیح ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	بازگشت (fs->blocksize - اندازه(ساختار ext4_orphan_block_tail)) /
		اندازه(__u32);
}

_INLINE_ ساختار ext4_orphan_block_tail *
ext2fs_orphan_block_tail(ext2_filsys fs, نویسه *buf)
{
	بازگشت (ساختار ext4_orphan_block_tail *)(buf + fs->blocksize -
		اندازه(ساختار ext4_orphan_block_tail));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

ایستا درونخطی unsigned صحیح ext2_dir_htree_level(ext2_filsys fs)
{
	اگر (ext2fs_has_feature_largedir(fs->super))
		بازگشت EXT4_HTREE_LEVEL;

	بازگشت EXT4_HTREE_LEVEL_COMPAT;
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
ایستا درونخطی تهی ext2fs_set_dtime(ext2_filsys fs, ساختار ext2_inode *inode)
{
	__u32	t;

	اگر (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	وگرنه
		t = time(پوچ) & 0xFFFFFFFF;
	اگر (t < fs->super->s_inodes_شمار)
		t = fs->super->s_inodes_شمار;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
