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
#وإلا
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#إذا __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#وإلا
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#وإلا
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#وإلا
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
خارجي "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#إذا (!defined(__GNUC__) && !defined(__WATCOMC__))
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
#ضمن <sys/types.h>
#endif

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
#endif /* EXT2_FLAT_INCLUDES */

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
#endif

#ضمن "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE صحيح __cdecl
#وإلا
#define EXT2_QSORT_TYPE صحيح
#endif

تعريفنوع بنية struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

بنية ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
تعريفنوع بنية ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


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

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
بنية ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
بنية ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	صحيح	blockcnt;
};

تعريفنوع بنية ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

تعريفنوع بنية ext2_file *ext2_file_t;

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

بنية blk_alloc_ctx;
بنية opaque_ext2_group_desc;

بنية struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	صحيح				flags;
	حرف *				device_name;
	بنية ext2_super_block	* 	super;
	unsigned صحيح			blocksize;
	صحيح				fragsize;
	dgrp_t				group_desc_سانى;
	unsigned long			desc_blocks;
	بنية opaque_ext2_group_desc *	group_desc;
	unsigned صحيح			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				بنية ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	بنية ext2_super_block *	orig_super;
	بنية ext2_image_hdr *		image_header;
	__u32				umask;
	time_t				now;
	صحيح				cluster_ratio_bits;
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
	فراغ *				priv_data;

	/*
	 * Inode cache
	 */
	بنية ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, بنية blk_alloc_ctx *ctx);
	فراغ (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, صحيح inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	فراغ *mmp_buf;
	فراغ *mmp_cmp;
	صحيح mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* progress operation functions */
	بنية ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	حرف				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	فراغ (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t رەقەم,
					صحيح inuse);

	/* hashmap for SHA of data blocks */
	بنية ext2fs_hashmap* block_sha_map;

	ثابت بنية ext2fs_nls_table *encoding;
};

#إذا EXT2_FLAT_INCLUDES
#ضمن "e2_bitops.h"
#وإلا
#ضمن <ext2fs/bitops.h>
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
#define BLOCK_سانى_IND		(-1)
#define BLOCK_سانى_DIND	(-2)
#define BLOCK_سانى_TIND	(-3)
#define BLOCK_سانى_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_DATA	1
#define BLOCK_ALLOC_METADATA	2

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
#define EXT2_BMOVE_GET_DBLIST	0x0001
#define EXT2_BMOVE_DEBUG	0x0002
#endif

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#define EXT2_EXTENT_FLAGS_LEAF		0x0001
#define EXT2_EXTENT_FLAGS_UNINIT	0x0002
#define EXT2_EXTENT_FLAGS_SECOND_VISIT	0x0004

بنية ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

تعريفنوع بنية ext2_extent_handle *ext2_extent_handle_t;
تعريفنوع بنية ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_يىلتىز	0x0001
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
#define EXT2_EXTENT_قىستۇرۇش_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_قىستۇرۇش_NOSPLIT	0x0002 /* insert may not cause split */

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
بنية ext2_extent_info {
	صحيح		curr_entry;
	صحيح		curr_level;
	صحيح		رەقەم_entries;
	صحيح		max_entries;
	صحيح		max_depth;
	صحيح		bytes_avail;
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
تعريفنوع بنية ext2_struct_inode_scan *ext2_inode_scan;

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
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_يۇقىرى << 32))

ساكن مضمن __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#إذا (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	أرجع extra | (nsec << EXT4_EPOCH_BITS);
}
ساكن مضمن time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#إذا (SIZEOF_TIME_T > 4)
	إذا (extra & EXT4_EPOCH_MASK)
		seconds += ((time_t)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	أرجع seconds;
}
ساكن مضمن __u32 __decode_extra_nsec(__u32 extra)
{
	أرجع (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_size(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_SIZE +				      \
		  (حجم(*inode) > EXT2_GOOD_OLD_INODE_SIZE ?		      \
		   ((بنية ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
افعل {									      \
	إذا (ext2fs_inode_includes(ext2fs_inode_actual_size(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((بنية ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} وإلا {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} طالما (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_size(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((بنية ext2_inode_large *)(inode))->field ## _extra) :      \
		(time_t)(inode)->field)

ساكن مضمن فراغ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#إذا (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#وإلا
	*hi = 0;
#endif
}
ساكن مضمن time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#إذا (SIZEOF_TIME_T == 4)
	أرجع *lo;
#وإلا
	أرجع ((time_t)(*hi) << 32) | *lo;
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

تعريفنوع بنية ext2_icount *ext2_icount_t;

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

#define EXT2_CHECK_MAGIC(بنية, code) \
	  إذا ((بنية)->magic != (code)) أرجع (code)

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
#وإلا
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
#define EXT2_FS_رەقەم_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#إذا defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
تعريفنوع بنية stat64 ext2fs_struct_stat;
#وإلا
تعريفنوع بنية stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
بنية ext2_xattr_handle;
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
ساكن مضمن صحيح ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	أرجع ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ساكن مضمن صحيح ext2fs_needs_large_file_feature(unsigned long long file_size)
{
	أرجع file_size >= 0x80000000ULL;
}

/* alloc.c */
خارجي فراغ ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
خارجي errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, صحيح mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
خارجي errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
خارجي errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
خارجي errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   بنية blk_alloc_ctx *ctx);
خارجي errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, صحيح رەقەم,
					ext2fs_block_bitmap map,
					blk_t *ret);
خارجي errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, صحيح رەقەم,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
خارجي errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    حرف *block_buf, blk_t *ret);
خارجي errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     حرف *block_buf, blk64_t *ret);
خارجي errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     حرف *block_buf, blk64_t *ret,
				     بنية blk_alloc_ctx *ctx);

خارجي فراغ ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, blk64_t lblk);
خارجي فراغ ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
خارجي فراغ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t رەقەم, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t رەقەم, صحيح inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_LENGTH	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
خارجي صحيح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
خارجي errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
خارجي فراغ ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
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
فراغ ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse);
فراغ ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse);
فراغ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t رەقەم, صحيح inuse);

/* alloc_tables.c */
خارجي errcode_t ext2fs_allocate_tables(ext2_filsys fs);
خارجي errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
خارجي errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح size);
خارجي errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
خارجي صحيح ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
خارجي errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
خارجي صحيح ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
خارجي فراغ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
خارجي errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
خارجي صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

خارجي errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    صحيح size);
خارجي errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
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
خارجي فراغ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
خارجي errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
خارجي صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
خارجي صحيح ext2fs_u32_list_سانى(ext2_u32_list bb);

/* bb_compat */
خارجي errcode_t badblocks_list_create(badblocks_list *ret, صحيح size);
خارجي errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
خارجي صحيح badblocks_list_test(badblocks_list bb, blk_t blk);
خارجي errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
خارجي صحيح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
خارجي فراغ badblocks_list_iterate_end(badblocks_iterate iter);
خارجي فراغ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
خارجي errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
خارجي فراغ ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
خارجي فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
خارجي errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_block_bitmap *ret);
خارجي errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ثابت حرف *descr,
						   ext2fs_block_bitmap *ret);
خارجي صحيح ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
خارجي errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ثابت حرف *descr,
					      ext2fs_inode_bitmap *ret);
خارجي errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
خارجي errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
خارجي errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
خارجي فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
خارجي فراغ ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
خارجي errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
خارجي errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
خارجي errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
خارجي errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
خارجي errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
خارجي errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned صحيح رەقەم,
					فراغ *in);
خارجي errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t رەقەم,
					 فراغ *in);
خارجي errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned صحيح رەقەم,
					فراغ *out);
خارجي errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t رەقەم,
					 فراغ *out);
خارجي errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned صحيح رەقەم,
					فراغ *in);
خارجي errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t رەقەم,
					 فراغ *in);
خارجي errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned صحيح رەقەم,
					فراغ *out);
خارجي errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t رەقەم,
					 فراغ *out);

/* blknum.c */
خارجي __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
خارجي __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
خارجي dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
خارجي blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
خارجي blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
خارجي صحيح ext2fs_group_blocks_سانى(ext2_filsys fs, dgrp_t group);
خارجي blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					 بنية ext2_inode *inode);
خارجي blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     بنية ext2_inode *inode);
خارجي blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					بنية ext2_inode *inode);
خارجي blk64_t ext2fs_blocks_سانى(بنية ext2_super_block *super);
خارجي فراغ ext2fs_blocks_سانى_set(بنية ext2_super_block *super,
				    blk64_t blk);
خارجي فراغ ext2fs_blocks_سانى_add(بنية ext2_super_block *super,
				    blk64_t blk);
خارجي blk64_t ext2fs_r_blocks_سانى(بنية ext2_super_block *super);
خارجي فراغ ext2fs_r_blocks_سانى_set(بنية ext2_super_block *super,
				      blk64_t blk);
خارجي فراغ ext2fs_r_blocks_سانى_add(بنية ext2_super_block *super,
				      blk64_t blk);
خارجي blk64_t ext2fs_free_blocks_سانى(بنية ext2_super_block *super);
خارجي فراغ ext2fs_free_blocks_سانى_set(بنية ext2_super_block *super,
					 blk64_t blk);
خارجي فراغ ext2fs_free_blocks_سانى_add(بنية ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
خارجي بنية ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  بنية opaque_ext2_group_desc *gdp,
					  dgrp_t group);
خارجي blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
خارجي blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
خارجي __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
خارجي blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
خارجي blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
خارجي __u32 ext2fs_bg_free_blocks_سانى(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_free_blocks_سانى_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
خارجي __u32 ext2fs_bg_free_inodes_سانى(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_free_inodes_سانى_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
خارجي __u32 ext2fs_bg_used_dirs_سانى(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_used_dirs_سانى_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
خارجي __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
خارجي __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
خارجي صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
خارجي فراغ ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
خارجي فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
خارجي __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
خارجي فراغ ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
خارجي blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     ثابت بنية ext2_inode *inode);
خارجي فراغ ext2fs_file_acl_block_set(ext2_filsys fs,
				      بنية ext2_inode *inode, blk64_t blk);
خارجي errcode_t ext2fs_inode_size_set(ext2_filsys fs, بنية ext2_inode *inode,
				       ext2_off64_t size);

/* block.c */
خارجي errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      صحيح	flags,
				      حرف *block_buf,
				      صحيح (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  صحيح	blockcnt,
						  فراغ	*priv_data),
				      فراغ *priv_data);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				صحيح	flags,
				حرف *block_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_offset,
					    فراغ	*priv_data),
				فراغ *priv_data);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *block_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_offset,
					    فراغ	*priv_data),
				فراغ *priv_data);

/* bmap.c */
خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *block_buf, صحيح bmap_flags,
			     blk_t block, blk_t *phys_blk);
خارجي errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *block_buf, صحيح bmap_flags, blk64_t block,
			      صحيح *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#إذا 0
/* bmove.c */
خارجي errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    صحيح flags);
#endif

/* check_desc.c */
خارجي errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
خارجي errcode_t ext2fs_close(ext2_filsys fs);
خارجي errcode_t ext2fs_close2(ext2_filsys fs, صحيح flags);
خارجي errcode_t ext2fs_close_free(ext2_filsys *fs);
خارجي errcode_t ext2fs_flush(ext2_filsys fs);
خارجي errcode_t ext2fs_flush2(ext2_filsys fs, صحيح flags);
خارجي صحيح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
خارجي errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
خارجي صحيح ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    صحيح *ret_meta_bg);
خارجي فراغ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
خارجي __u32 ext2fs_crc32_be(__u32 crc, unsigned حرف ثابت *p, size_t len);
خارجي __u32 ext2fs_crc32c_le(__u32 crc, unsigned حرف ثابت *p, size_t len);

/* csum.c */
خارجي فراغ ext2fs_init_csum_seed(ext2_filsys fs);
خارجي errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_mmp_csum_verify(ext2_filsys, بنية mmp_struct *mmp);
خارجي صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_block *sb);
خارجي errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    بنية ext2_super_block *sb);
خارجي صحيح ext2fs_superblock_csum_verify(ext2_filsys fs,
					 بنية ext2_super_block *sb);
خارجي errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					بنية ext2_ext_attr_header *hdr);
خارجي صحيح ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     بنية ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_TAIL(block, blocksize) \
	((بنية ext2_dir_entry_tail *)(((حرف *)(block)) + \
	(blocksize) - حجم(بنية ext2_dir_entry_tail)))

خارجي فراغ ext2fs_initialize_dirent_tail(ext2_filsys fs,
					  بنية ext2_dir_entry_tail *t);
خارجي صحيح ext2fs_dirent_has_tail(ext2_filsys fs,
				  بنية ext2_dir_entry *dirent);
خارجي صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     بنية ext2_dir_entry *dirent);
خارجي صحيح ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_entry *dirent);
خارجي errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   بنية ext2_dir_entry *dirent);
خارجي errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  بنية ext2_dir_entry *dirent,
					  بنية ext2_dx_countlimit **cc,
					  صحيح *offset);
خارجي errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				بنية ext2_dir_entry *dirent,
				__u32 *crc, بنية ext2_dx_tail **ret_t);
خارجي errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      بنية ext3_extent_header *eh);
خارجي صحيح ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   بنية ext3_extent_header *eh);
خارجي errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      حرف *bitmap, صحيح size);
خارجي صحيح ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   حرف *bitmap, صحيح size);
خارجي errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      حرف *bitmap, صحيح size);
خارجي صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   حرف *bitmap, صحيح size);
خارجي errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext2_inode_large *inode);
خارجي صحيح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_inode_large *inode);
خارجي فراغ ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
خارجي صحيح ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
خارجي errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
خارجي __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
خارجي errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
خارجي errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي فراغ ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							   ثابت فراغ *));
خارجي فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
							    ثابت فراغ *));
خارجي errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry *db_info,
		    فراغ	*priv_data),
	فراغ *priv_data);
خارجي errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_data),
	فراغ *priv_data);
خارجي errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	صحيح (*func)(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		    فراغ	*priv_data),
	unsigned long long start,
	unsigned long long سانى,
	فراغ *priv_data);
خارجي errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, صحيح blockcnt);
خارجي errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
خارجي errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
خارجي صحيح ext2fs_dblist_سانى(ext2_dblist dblist);
خارجي blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
خارجي errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					بنية ext2_db_entry **entry);
خارجي errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					بنية ext2_db_entry2 **entry);
خارجي errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
خارجي errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  صحيح	flags,
				  حرف	*block_buf,
				  صحيح (*func)(ext2_ino_t	dir,
					      صحيح		entry,
					      بنية ext2_dir_entry *dirent,
					      صحيح	offset,
					      صحيح	blocksize,
					      حرف	*buf,
					      فراغ	*priv_data),
				  فراغ *priv_data);

#إذا 0
/* digest_encode.c */
#define EXT2FS_DIGEST_SIZE EXT2FS_SHA256_LENGTH
خارجي صحيح ext2fs_digest_encode(ثابت حرف *src, صحيح len, حرف *dst);
خارجي صحيح ext2fs_digest_decode(ثابت حرف *src, صحيح len, حرف *dst);
#endif

/* dirblock.c */
خارجي errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					فراغ *buf, صحيح flags, ext2_ino_t ino);
خارجي errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					فراغ *buf);
خارجي errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 فراغ *buf, صحيح flags);
خارجي errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 فراغ *buf, صحيح flags, ext2_ino_t ino);

/* dirhash.c */
خارجي errcode_t ext2fs_dirhash(صحيح version, ثابت حرف *name, صحيح len,
				ثابت __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

خارجي errcode_t ext2fs_dirhash2(صحيح version, ثابت حرف *name, صحيح len,
				 ثابت بنية ext2fs_nls_table *charset,
				 صحيح hash_flags,
				 ثابت __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
خارجي errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    بنية ext2_dir_entry *dirent,
				    unsigned صحيح *rec_len);
خارجي errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    unsigned صحيح len,
				    بنية ext2_dir_entry *dirent);
خارجي errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *block_buf,
			      صحيح (*func)(بنية ext2_dir_entry *dirent,
					  صحيح	offset,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_data),
			      فراغ *priv_data);
خارجي errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *block_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح	entry,
					  بنية ext2_dir_entry *dirent,
					  صحيح	offset,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_data),
			      فراغ *priv_data);

/* dupfs.c */
خارجي errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
خارجي errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
خارجي __u32 ext2fs_ext_attr_hash_entry(بنية ext2_ext_attr_entry *entry,
					فراغ *data);
خارجي __u32 ext2fs_ext_attr_hash_entry_signed(بنية ext2_ext_attr_entry *entry,
					       فراغ *data);
خارجي errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     بنية ext2_ext_attr_entry *entry,
					     فراغ *data, __u32 *hash);
خارجي errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     بنية ext2_ext_attr_entry *entry,
					     فراغ *data, __u32 *hash,
					     __u32 *signed_hash);
خارجي errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       فراغ *buf);
خارجي errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       فراغ *buf);
خارجي errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       فراغ *buf, ext2_ino_t inum);
خارجي errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   حرف *block_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   حرف *block_buf,
					   صحيح adjust, __u32 *newcount);
خارجي errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   حرف *block_buf,
					   صحيح adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قىممەت,
					    size_t قىممەت_len, فراغ *data),
				فراغ *data);
errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *ئاچقۇچ,
			   فراغ **قىممەت, size_t *قىممەت_len);
errcode_t ext2fs_xattr_set(بنية ext2_xattr_handle *handle,
			   ثابت حرف *ئاچقۇچ,
			   ثابت فراغ *قىممەت,
			   size_t قىممەت_len);
errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *ئاچقۇچ);
errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode);
errcode_t ext2fs_xattrs_سانى(بنية ext2_xattr_handle *handle, size_t *سانى);
errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      unsigned صحيح *new_flags, unsigned صحيح *old_flags);
خارجي فراغ ext2fs_ext_attr_block_rehash(بنية ext2_ext_attr_header *header,
					 بنية ext2_ext_attr_entry *end);
خارجي __u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode);
خارجي فراغ ext2fs_set_ea_inode_hash(بنية ext2_inode *inode, __u32 hash);
خارجي __u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode);
خارجي فراغ ext2fs_set_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_سانى);

/* extent.c */
خارجي errcode_t ext2fs_extent_header_verify(فراغ *ptr, صحيح size);
خارجي errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
خارجي errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
خارجي فراغ ext2fs_extent_free(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   صحيح flags, بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_تۈگۈن_split(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, صحيح flags,
				       بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_قىستۇرۇش(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent);
خارجي errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					صحيح flags);
خارجي errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags);
خارجي errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					بنية ext2_extent_info *info);
خارجي errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
خارجي errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     صحيح leaf_level, blk64_t blk);
خارجي errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
خارجي size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle);
خارجي errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      بنية ext2_inode *inode);
خارجي errcode_t ext2fs_سانى_blocks(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode, blk64_t *ret_سانى);
خارجي errcode_t ext2fs_decode_extent(بنية ext2fs_extent *to, فراغ *from,
				      صحيح len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
			   بنية ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
خارجي errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode,
				   صحيح flags, ext2_file_t *ret);
خارجي errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  صحيح flags, ext2_file_t *ret);
خارجي ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
بنية ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
خارجي ext2_ino_t ext2fs_file_get_inode_رەقەم(ext2_file_t file);
خارجي errcode_t ext2fs_file_close(ext2_file_t file);
خارجي errcode_t ext2fs_file_flush(ext2_file_t file);
خارجي errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
				  unsigned صحيح wanted, unsigned صحيح *got);
خارجي errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
				   unsigned صحيح nbytes, unsigned صحيح *written);
خارجي errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   صحيح whence, __u64 *ret_pos);
خارجي errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   صحيح whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size);
خارجي ext2_off_t ext2fs_file_get_size(ext2_file_t file);
خارجي errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size);
خارجي errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size);

/* finddev.c */
خارجي حرف *ext2fs_find_block_device(dev_t device);

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
					    __u32 start, __u32 end,
					    __u32 real_end,
					    ثابت حرف *descr, حرف *init_map,
					    ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						ثابت حرف *descr,
						ext2fs_generic_bitmap *ret);
خارجي errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
خارجي فراغ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
خارجي فراغ ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
خارجي errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
خارجي errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
خارجي errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 رەقەم,
						 فراغ *out);
خارجي errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 رەقەم,
						 فراغ *in);
خارجي errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
خارجي errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 start, __u64 end,
				    __u64 real_end,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
فراغ ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned صحيح رەقەم,
					فراغ *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned صحيح رەقەم,
					فراغ *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_سانى_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_سانى_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
خارجي unsigned صحيح ext2fs_list_backups(ext2_filsys fs, unsigned صحيح *three,
				unsigned صحيح *five, unsigned صحيح *seven);

/* getenv.c */
خارجي حرف *ext2fs_safe_getenv(ثابت حرف *arg);

/* get_num_dirs.c */
خارجي errcode_t ext2fs_get_رەقەم_dirs(ext2_filsys fs, ext2_ino_t *ret_رەقەم_dirs);

/* getsize.c */
خارجي errcode_t ext2fs_get_device_size(ثابت حرف *file, صحيح blocksize,
					blk_t *retblocks);
خارجي errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
خارجي صحيح ext2fs_get_dio_alignment(صحيح fd);
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t رەقەم_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t رەقەم_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b);

/* imager.c */
خارجي errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_super_read(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحيح fd, صحيح flags);
خارجي errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحيح fd, صحيح flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, فراغ *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, فراغ *buf);

/* initialize.c */
خارجي errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
				   بنية ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحيح super_only);

/* icount.c */
خارجي فراغ ext2fs_free_icount(ext2_icount_t icount);
خارجي errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, حرف *tdb_dir,
					  صحيح flags, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags,
				       unsigned صحيح size,
				       ext2_icount_t hint, ext2_icount_t *ret);
خارجي errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
				      unsigned صحيح size,
				      ext2_icount_t *ret);
خارجي errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
خارجي errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
خارجي errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 سانى);
خارجي ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

خارجي errcode_t ext2fs_get_memalign(unsigned long size,
				     unsigned long align, فراغ *ptr);

/* inline_data.c */
خارجي errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino,
					 size_t *size);
خارجي errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t *size);
خارجي errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode *inode,
					فراغ *buf, size_t size);

/* inode.c */
خارجي errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned صحيح cache_size);
خارجي فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache);
خارجي errcode_t ext2fs_flush_icache(ext2_filsys fs);
خارجي errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    بنية ext2_inode *inode,
					    صحيح bufsize);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8
خارجي errcode_t ext2fs_open_inode_scan(ext2_filsys fs, صحيح buffer_blocks,
				  ext2_inode_scan *ret_scan);
خارجي فراغ ext2fs_close_inode_scan(ext2_inode_scan scan);
خارجي errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       بنية ext2_inode *inode);
خارجي errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   صحيح	group);
خارجي فراغ ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 فراغ * priv_data),
	 فراغ *done_group_data);
خارجي صحيح ext2fs_inode_scan_flags(ext2_inode_scan scan, صحيح set_flags,
				   صحيح clear_flags);
خارجي errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					بنية ext2_inode * inode,
					صحيح bufsize);
خارجي errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    بنية ext2_inode * inode,
				    صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode * inode,
					 صحيح bufsize);
خارجي errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode * inode,
				     صحيح bufsize, صحيح flags);
خارجي errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode);
خارجي errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
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
خارجي errcode_t ext2fs_check_mount_point(ثابت حرف *device, صحيح *mount_flags,
					  حرف *mtpt, صحيح mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
خارجي errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode,
			      حرف *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
خارجي errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			 صحيح namelen, حرف *buf, ext2_ino_t *inode);
خارجي errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t يىلتىز, ext2_ino_t cwd,
			ثابت حرف *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t يىلتىز, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode);
خارجي errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t يىلتىز, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
صحيح ext2fs_native_flag(فراغ);

/* newdir.c */
خارجي errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, حرف **block);
خارجي errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
خارجي ثابت بنية ext2fs_nls_table *ext2fs_load_nls_table(صحيح encoding);
خارجي صحيح ext2fs_check_encoded_name(ثابت بنية ext2fs_nls_table *table,
				     حرف *s, size_t len, حرف **pos);
خارجي صحيح ext2fs_casefold_cmp(ثابت بنية ext2fs_nls_table *table,
			       ثابت unsigned حرف *str1, size_t len1,
			       ثابت unsigned حرف *str2, size_t len2);

/* mkdir.c */
خارجي errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      ثابت حرف *name);
خارجي errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, unsigned long flags,
			       صحيح link_flags, ثابت حرف *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
بنية ext2fs_journal_params {
	blk_t رەقەم_journal_blocks;
	blk_t رەقەم_fc_blocks;
};
خارجي errcode_t ext2fs_get_journal_params(
		بنية ext2fs_journal_params *params, ext2_filsys fs);
خارجي errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, صحيح رەقەم,
				    blk_t *ret_blk, صحيح *ret_سانى);
خارجي errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح رەقەم,
				     blk64_t *ret_blk, صحيح *ret_سانى);
خارجي errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 رەقەم_blocks, صحيح flags,
						  حرف  **ret_jsb);
خارجي errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  بنية ext2fs_journal_params *params,
						  صحيح flags, حرف  **ret_jsb);
خارجي errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
خارجي errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t رەقەم_blocks,
					  صحيح flags);
خارجي errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t رەقەم_blocks,
					   blk64_t goal, صحيح flags);
خارجي errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    بنية ext2fs_journal_params *params,
				    blk64_t goal, صحيح flags);
خارجي صحيح ext2fs_default_journal_size(__u64 رەقەم_blocks);
خارجي صحيح ext2fs_journal_sb_start(صحيح blocksize);

/* openfs.c */
خارجي errcode_t ext2fs_open(ثابت حرف *name, صحيح flags, صحيح superblock,
			     unsigned صحيح block_size, io_manager manager,
			     ext2_filsys *ret_fs);
خارجي errcode_t ext2fs_open2(ثابت حرف *name, ثابت حرف *io_options,
			      صحيح flags, صحيح superblock,
			      unsigned صحيح block_size, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
خارجي blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
خارجي blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
خارجي errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t رەقەم_blocks);
خارجي errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
خارجي e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
خارجي __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      حرف *buf);
خارجي errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   حرف *buf);
خارجي صحيح ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
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
صحيح ext2fs_dir_link_empty(بنية ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *target);
صحيح ext2fs_is_fast_symlink(بنية ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, فراغ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحيح immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
unsigned ext2fs_mmp_new_seq(فراغ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, فراغ **ptr);

/* read_bb.c */
خارجي errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
خارجي errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      فراغ *priv_data,
				      فراغ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      حرف *badstr,
						      فراغ *priv_data));
خارجي errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     فراغ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
خارجي errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
خارجي errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح رەقەم_threads);
خارجي errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
خارجي errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
خارجي errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
خارجي errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2FS_SHA256_LENGTH 32
#إذا 0
خارجي فراغ ext2fs_sha256(ثابت unsigned حرف *in, unsigned long in_size,
		   unsigned حرف out[EXT2FS_SHA256_LENGTH]);
#endif

/* sha512.c */
#define EXT2FS_SHA512_LENGTH 64
خارجي فراغ ext2fs_sha512(ثابت unsigned حرف *in, unsigned long in_size,
			  unsigned حرف out[EXT2FS_SHA512_LENGTH]);

/* swapfs.c */
خارجي errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf, size_t size,
					صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf, size_t size,
					 صحيح flags);
خارجي errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, حرف *buf, صحيح flags);
خارجي فراغ ext2fs_swap_ext_attr(حرف *to, حرف *from, صحيح bufsize,
				 صحيح has_header);
خارجي فراغ ext2fs_swap_ext_attr_header(بنية ext2_ext_attr_header *to_header,
					بنية ext2_ext_attr_header *from_hdr);
خارجي فراغ ext2fs_swap_ext_attr_entry(بنية ext2_ext_attr_entry *to_entry,
				       بنية ext2_ext_attr_entry *from_entry);
خارجي فراغ ext2fs_swap_super(بنية ext2_super_block * super);
خارجي فراغ ext2fs_swap_group_desc(بنية ext2_group_desc *gdp);
خارجي فراغ ext2fs_swap_group_desc2(ext2_filsys, بنية ext2_group_desc *gdp);
خارجي فراغ ext2fs_swap_inode_full(ext2_filsys fs, بنية ext2_inode_large *t,
				   بنية ext2_inode_large *f, صحيح hostorder,
				   صحيح bufsize);
خارجي فراغ ext2fs_swap_inode(ext2_filsys fs,بنية ext2_inode *t,
			      بنية ext2_inode *f, صحيح hostorder);
خارجي فراغ ext2fs_swap_mmp(بنية mmp_struct *mmp);

/* unix_io.c */
خارجي صحيح ext2fs_open_file(ثابت حرف *pathname, صحيح flags, mode_t mode);
خارجي صحيح ext2fs_stat(ثابت حرف *path, ext2fs_struct_stat *buf);
خارجي صحيح ext2fs_fstat(صحيح fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
خارجي صحيح ext2fs_inode_has_valid_blocks(بنية ext2_inode *inode);
خارجي صحيح ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  بنية ext2_inode *inode);

/* version.c */
خارجي صحيح ext2fs_parse_version_string(ثابت حرف *ver_string);
خارجي صحيح ext2fs_get_library_version(ثابت حرف **ver_string,
				      ثابت حرف **date_string);

/* write_bb_file.c */
خارجي errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      unsigned صحيح flags,
				      FILE *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
خارجي errcode_t ext2fs_get_mem(unsigned long size, فراغ *ptr);
خارجي errcode_t ext2fs_get_memzero(unsigned long size, فراغ *ptr);
خارجي errcode_t ext2fs_get_array(unsigned long سانى,
				  unsigned long size, فراغ *ptr);
خارجي errcode_t ext2fs_get_arrayzero(unsigned long سانى,
				      unsigned long size, فراغ *ptr);
خارجي errcode_t ext2fs_free_mem(فراغ *ptr);
خارجي errcode_t ext2fs_resize_mem(unsigned long old_size,
				   unsigned long size, فراغ *ptr);
خارجي errcode_t ext2fs_resize_array(unsigned long old_سانى, unsigned long سانى,
				     unsigned long size, فراغ *ptr);
خارجي فراغ ext2fs_mark_super_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_changed(ext2_filsys fs);
خارجي صحيح ext2fs_test_changed(ext2_filsys fs);
خارجي فراغ ext2fs_mark_valid(ext2_filsys fs);
خارجي فراغ ext2fs_unmark_valid(ext2_filsys fs);
خارجي صحيح ext2fs_test_valid(ext2_filsys fs);
خارجي فراغ ext2fs_mark_ib_dirty(ext2_filsys fs);
خارجي فراغ ext2fs_mark_bb_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_ib_dirty(ext2_filsys fs);
خارجي صحيح ext2fs_test_bb_dirty(ext2_filsys fs);
خارجي dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
خارجي dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
خارجي blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
خارجي blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
خارجي blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
				      بنية ext2_inode *inode);
خارجي صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح blocks);
خارجي صحيح ext2fs_log2_u32(__u32 arg);
خارجي صحيح ext2fs_log2_u64(__u64 arg);
خارجي صحيح ext2fs_log10_u32(__u32 arg);
خارجي صحيح ext2fs_log10_u64(__u64 arg);
خارجي unsigned صحيح ext2fs_div_ceil(unsigned صحيح a, unsigned صحيح b);
خارجي __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
خارجي صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_entry *entry);
خارجي فراغ ext2fs_dirent_set_name_len(بنية ext2_dir_entry *entry, صحيح len);
خارجي صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_entry *entry);
خارجي فراغ ext2fs_dirent_set_file_type(بنية ext2_dir_entry *entry, صحيح type);
خارجي بنية ext2_inode *ext2fs_inode(بنية ext2_inode_large * large_inode);
خارجي ثابت بنية ext2_inode *ext2fs_const_inode(ثابت بنية ext2_inode_large * large_inode);
خارجي صحيح ext2fs_inodes_per_orphan_block(ext2_filsys fs);
خارجي بنية ext4_orphan_block_tail *ext2fs_orphan_block_tail(ext2_filsys fs,
							       حرف *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ خارجي
#وإلا
#إذا (__STDC_VERSION__ >= 199901L)
#define _INLINE_ مضمن
#وإلا
#ifdef __GNUC__
#define _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#define _INLINE_ خارجي مضمن
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#ضمن <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long size, فراغ *ptr)
{
	فراغ *pp;

	pp = malloc(size);
	إذا (!pp)
		أرجع EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, حجم (pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long size, فراغ *ptr)
{
	فراغ *pp;

	pp = malloc(size);
	إذا (!pp)
		أرجع EXT2_ET_NO_MEMORY;
	memset(pp, 0, size);
	memcpy(ptr, &pp, حجم(pp));
	أرجع 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long سانى, unsigned long size,
				    فراغ *ptr)
{
	إذا (سانى && (~0UL)/سانى < size)
		أرجع EXT2_ET_NO_MEMORY;
	أرجع ext2fs_get_mem(سانى*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long سانى,
					unsigned long size, فراغ *ptr)
{
	إذا (سانى && (~0UL)/سانى < size)
		أرجع EXT2_ET_NO_MEMORY;

	أرجع ext2fs_get_memzero((size_t)سانى * size, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(فراغ *ptr)
{
	فراغ *p;

	memcpy(&p, ptr, حجم(p));
	حرر(p);
	p = 0;
	memcpy(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_size,
				     unsigned long size, فراغ *ptr)
{
	فراغ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, حجم(p));
	p = realloc(p, size);
	إذا (!p)
		أرجع EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, حجم(p));
	أرجع 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(unsigned long size,
				       unsigned long old_سانى,
				       unsigned long سانى, فراغ *ptr)
{
	unsigned long old_size;
	errcode_t retval;

	إذا (سانى && (~0UL)/سانى < size)
		أرجع EXT2_ET_NO_MEMORY;

	size *= سانى;
	old_size = size * old_سانى;
	retval = ext2fs_resize_mem(old_size, size, ptr);
	إذا (retval)
		أرجع retval;

	إذا (size > old_size) {
		فراغ *p;

		memcpy(&p, ptr, حجم(p));
		memset((حرف *)p + old_size, 0, size - old_size);
		memcpy(ptr, &p, حجم(p));
	}

	أرجع 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ فراغ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ فراغ ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ صحيح ext2fs_test_changed(ext2_filsys fs)
{
	أرجع (fs->flags & EXT2_FLAG_CHANGED);
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
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ فراغ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
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
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	أرجع ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	أرجع (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	أرجع (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	أرجع (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

_INLINE_ صحيح ext2fs_htree_intnode_maxrecs(ext2_filsys fs, صحيح blocks)
{
	صحيح csum_size = 0;

	إذا ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_size = حجم(بنية ext2_dx_tail);
	أرجع blocks * ((fs->blocksize - (8 + csum_size)) /
						حجم(بنية ext2_dx_entry));
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
_INLINE_ unsigned صحيح ext2fs_div_ceil(unsigned صحيح a, unsigned صحيح b)
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

_INLINE_ صحيح ext2fs_dirent_name_len(ثابت بنية ext2_dir_entry *entry)
{
	أرجع entry->name_len & 0xff;
}

_INLINE_ فراغ ext2fs_dirent_set_name_len(بنية ext2_dir_entry *entry, صحيح len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ صحيح ext2fs_dirent_file_type(ثابت بنية ext2_dir_entry *entry)
{
	أرجع entry->name_len >> 8;
}

_INLINE_ فراغ ext2fs_dirent_set_file_type(بنية ext2_dir_entry *entry, صحيح type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
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

_INLINE_ صحيح ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	أرجع (fs->blocksize - حجم(بنية ext4_orphan_block_tail)) /
		حجم(__u32);
}

_INLINE_ بنية ext4_orphan_block_tail *
ext2fs_orphan_block_tail(ext2_filsys fs, حرف *buf)
{
	أرجع (بنية ext4_orphan_block_tail *)(buf + fs->blocksize -
		حجم(بنية ext4_orphan_block_tail));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

ساكن مضمن unsigned صحيح ext2_dir_htree_level(ext2_filsys fs)
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
ساكن مضمن فراغ ext2fs_set_dtime(ext2_filsys fs, بنية ext2_inode *inode)
{
	__u32	t;

	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	وإلا
		t = time(عدم) & 0xFFFFFFFF;
	إذا (t < fs->super->s_inodes_سانى)
		t = fs->super->s_inodes_سانى;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
