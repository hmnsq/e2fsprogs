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
#അല്ലെങ്കിൽ
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#എങ്കിൽ __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#അല്ലെങ്കിൽ
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#അല്ലെങ്കിൽ
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#അല്ലെങ്കിൽ
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#എങ്കിൽ (!defined(__GNUC__) && !defined(__WATCOMC__))
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
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <errno.h>

#എങ്കിൽ EXT2_FLAT_INCLUDES
#അന്തര്ഭാവ "e2_types.h"
#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext3_extents.h"
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <ext2fs/ext2_types.h>
#അന്തര്ഭാവ <ext2fs/ext2_fs.h>
#അന്തര്ഭാവ <ext2fs/ext3_extents.h>
#endif /* EXT2_FLAT_INCLUDES */

തരനിർവചനം __u32 __bitwise		ext2_ino_t;
തരനിർവചനം __u32 __bitwise		blk_t;
തരനിർവചനം __u64 __bitwise		blk64_t;
തരനിർവചനം __u32 __bitwise		dgrp_t;
തരനിർവചനം __s32 __bitwise		ext2_off_t;
തരനിർവചനം __s64 __bitwise		ext2_off64_t;
തരനിർവചനം __s64 __bitwise		e2_blkcnt_t;
തരനിർവചനം __u32 __bitwise		ext2_dirhash_t;

#എങ്കിൽ EXT2_FLAT_INCLUDES
#അന്തര്ഭാവ "com_err.h"
#അന്തര്ഭാവ "ext2_io.h"
#അന്തര്ഭാവ "ext2_err.h"
#അന്തര്ഭാവ "ext2_ext_attr.h"
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <et/com_err.h>
#അന്തര്ഭാവ <ext2fs/ext2_io.h>
#അന്തര്ഭാവ <ext2fs/ext2_err.h>
#അന്തര്ഭാവ <ext2fs/ext2_ext_attr.h>
#endif

#അന്തര്ഭാവ "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE പൂർണ്ണസംഖ്യ __cdecl
#അല്ലെങ്കിൽ
#define EXT2_QSORT_TYPE പൂർണ്ണസംഖ്യ
#endif

തരനിർവചനം ഘടന struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

ഘടന ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

തരനിർവചനം ഘടന ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
തരനിർവചനം ഘടന ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
തരനിർവചനം ഘടന ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

തരനിർവചനം ഘടന ext2_struct_u32_list *ext2_badblocks_list;
തരനിർവചനം ഘടന ext2_struct_u32_iterate *ext2_badblocks_iterate;

തരനിർവചനം ഘടന ext2_struct_u32_list *ext2_u32_list;
തരനിർവചനം ഘടന ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
തരനിർവചനം ഘടന ext2_struct_u32_list *badblocks_list;
തരനിർവചനം ഘടന ext2_struct_u32_iterate *badblocks_iterate;

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ഘടന ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ഘടന ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	പൂർണ്ണസംഖ്യ	blockcnt;
};

തരനിർവചനം ഘടന ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

തരനിർവചനം ഘടന ext2_file *ext2_file_t;

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

ഘടന blk_alloc_ctx;
ഘടന opaque_ext2_group_desc;

ഘടന struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	പൂർണ്ണസംഖ്യ				flags;
	അക്ഷരം *				device_name;
	ഘടന ext2_super_block	* 	super;
	unsigned പൂർണ്ണസംഖ്യ			blocksize;
	പൂർണ്ണസംഖ്യ				fragsize;
	dgrp_t				group_desc_എണ്ണം;
	unsigned long			desc_blocks;
	ഘടന opaque_ext2_group_desc *	group_desc;
	unsigned പൂർണ്ണസംഖ്യ			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ഘടന ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ഘടന ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ഘടന ext2_super_block *	orig_super;
	ഘടന ext2_image_hdr *		image_header;
	__u32				umask;
	time_t				now;
	പൂർണ്ണസംഖ്യ				cluster_ratio_bits;
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
	ശൂന്യം *				priv_data;

	/*
	 * Inode cache
	 */
	ഘടന ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ഘടന blk_alloc_ctx *ctx);
	ശൂന്യം (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, പൂർണ്ണസംഖ്യ inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	ശൂന്യം *mmp_buf;
	ശൂന്യം *mmp_cmp;
	പൂർണ്ണസംഖ്യ mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* progress operation functions */
	ഘടന ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	അക്ഷരം				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	ശൂന്യം (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t അക്കം,
					പൂർണ്ണസംഖ്യ inuse);

	/* hashmap for SHA of data blocks */
	ഘടന ext2fs_hashmap* block_sha_map;

	സ്ഥിരം ഘടന ext2fs_nls_table *encoding;
};

#എങ്കിൽ EXT2_FLAT_INCLUDES
#അന്തര്ഭാവ "e2_bitops.h"
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <ext2fs/bitops.h>
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
#define BLOCK_എണ്ണം_IND		(-1)
#define BLOCK_എണ്ണം_DIND	(-2)
#define BLOCK_എണ്ണം_TIND	(-3)
#define BLOCK_എണ്ണം_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_DATA	1
#define BLOCK_ALLOC_METADATA	2

ഘടന blk_alloc_ctx {
	ext2_ino_t		ino;
	ഘടന ext2_inode	*inode;
	blk64_t			lblk;
	പൂർണ്ണസംഖ്യ			flags;
};

#എങ്കിൽ 0
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

ഘടന ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

തരനിർവചനം ഘടന ext2_extent_handle *ext2_extent_handle_t;
തരനിർവചനം ഘടന ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_വേര്	0x0001
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
#define EXT2_EXTENT_തിരുകുക_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_തിരുകുക_NOSPLIT	0x0002 /* insert may not cause split */

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
ഘടന ext2_extent_info {
	പൂർണ്ണസംഖ്യ		curr_entry;
	പൂർണ്ണസംഖ്യ		curr_level;
	പൂർണ്ണസംഖ്യ		അക്കം_entries;
	പൂർണ്ണസംഖ്യ		max_entries;
	പൂർണ്ണസംഖ്യ		max_depth;
	പൂർണ്ണസംഖ്യ		bytes_avail;
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
തരനിർവചനം ഘടന ext2_struct_inode_scan *ext2_inode_scan;

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
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_മുകളിലെ << 32))

സ്ഥിതം വരിയിൽ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#എങ്കിൽ (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	തിരികെ extra | (nsec << EXT4_EPOCH_BITS);
}
സ്ഥിതം വരിയിൽ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#എങ്കിൽ (SIZEOF_TIME_T > 4)
	എങ്കിൽ (extra & EXT4_EPOCH_MASK)
		seconds += ((time_t)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	തിരികെ seconds;
}
സ്ഥിതം വരിയിൽ __u32 __decode_extra_nsec(__u32 extra)
{
	തിരികെ (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_size(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_SIZE +				      \
		  (വലുപ്പം(*inode) > EXT2_GOOD_OLD_INODE_SIZE ?		      \
		   ((ഘടന ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
ചെയ്യുക {									      \
	എങ്കിൽ (ext2fs_inode_includes(ext2fs_inode_actual_size(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((ഘടന ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} അല്ലെങ്കിൽ {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} ആകുമ്പോൾ (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_size(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((ഘടന ext2_inode_large *)(inode))->field ## _extra) :      \
		(time_t)(inode)->field)

സ്ഥിതം വരിയിൽ ശൂന്യം __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#എങ്കിൽ (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#അല്ലെങ്കിൽ
	*hi = 0;
#endif
}
സ്ഥിതം വരിയിൽ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#എങ്കിൽ (SIZEOF_TIME_T == 4)
	തിരികെ *lo;
#അല്ലെങ്കിൽ
	തിരികെ ((time_t)(*hi) << 32) | *lo;
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

തരനിർവചനം ഘടന ext2_icount *ext2_icount_t;

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

#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

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
#അല്ലെങ്കിൽ
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
#define EXT2_FS_അക്കം_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#എങ്കിൽ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
തരനിർവചനം ഘടന stat64 ext2fs_struct_stat;
#അല്ലെങ്കിൽ
തരനിർവചനം ഘടന stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ഘടന ext2_xattr_handle;
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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	തിരികെ ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ext2fs_needs_large_file_feature(unsigned long long file_size)
{
	തിരികെ file_size >= 0x80000000ULL;
}

/* alloc.c */
ബാഹ്യ ശൂന്യം ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
ബാഹ്യ errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, പൂർണ്ണസംഖ്യ mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
ബാഹ്യ errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
ബാഹ്യ errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
ബാഹ്യ errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   ഘടന blk_alloc_ctx *ctx);
ബാഹ്യ errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, പൂർണ്ണസംഖ്യ അക്കം,
					ext2fs_block_bitmap map,
					blk_t *ret);
ബാഹ്യ errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, പൂർണ്ണസംഖ്യ അക്കം,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
ബാഹ്യ errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    അക്ഷരം *block_buf, blk_t *ret);
ബാഹ്യ errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     അക്ഷരം *block_buf, blk64_t *ret);
ബാഹ്യ errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     അക്ഷരം *block_buf, blk64_t *ret,
				     ഘടന blk_alloc_ctx *ctx);

ബാഹ്യ ശൂന്യം ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ഘടന ext2_inode *inode, blk64_t lblk);
ബാഹ്യ ശൂന്യം ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
ബാഹ്യ ശൂന്യം ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	ശൂന്യം (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse),
	ശൂന്യം (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_LENGTH	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
ബാഹ്യ ശൂന്യം ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
						  ശൂന്യം (*func)(ext2_filsys fs,
							       blk64_t blk,
							       പൂർണ്ണസംഖ്യ inuse),
						  ശൂന്യം (**old)(ext2_filsys fs,
							       blk64_t blk,
							       പൂർണ്ണസംഖ്യ inuse));

/* alloc_stats.c */
ശൂന്യം ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, പൂർണ്ണസംഖ്യ inuse);
ശൂന്യം ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       പൂർണ്ണസംഖ്യ inuse, പൂർണ്ണസംഖ്യ isdir);
ശൂന്യം ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, പൂർണ്ണസംഖ്യ inuse);
ശൂന്യം ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, പൂർണ്ണസംഖ്യ inuse);
ശൂന്യം ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse);

/* alloc_tables.c */
ബാഹ്യ errcode_t ext2fs_allocate_tables(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
ബാഹ്യ errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
ബാഹ്യ errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
ബാഹ്യ ശൂന്യം ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
ബാഹ്യ errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

ബാഹ്യ errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
					   blk_t blk);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
ബാഹ്യ ശൂന്യം ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
ബാഹ്യ errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
ബാഹ്യ ശൂന്യം ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
ബാഹ്യ errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_u32_list_എണ്ണം(ext2_u32_list bb);

/* bb_compat */
ബാഹ്യ errcode_t badblocks_list_create(badblocks_list *ret, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
ബാഹ്യ പൂർണ്ണസംഖ്യ badblocks_list_test(badblocks_list bb, blk_t blk);
ബാഹ്യ errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
ബാഹ്യ പൂർണ്ണസംഖ്യ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
ബാഹ്യ ശൂന്യം badblocks_list_iterate_end(badblocks_iterate iter);
ബാഹ്യ ശൂന്യം badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
ബാഹ്യ errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
ബാഹ്യ ശൂന്യം ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
ബാഹ്യ ശൂന്യം ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
ബാഹ്യ errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      സ്ഥിരം അക്ഷരം *descr,
					      ext2fs_block_bitmap *ret);
ബാഹ്യ errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   സ്ഥിരം അക്ഷരം *descr,
						   ext2fs_block_bitmap *ret);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      സ്ഥിരം അക്ഷരം *descr,
					      ext2fs_inode_bitmap *ret);
ബാഹ്യ errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
ബാഹ്യ errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
ബാഹ്യ errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
ബാഹ്യ ശൂന്യം ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
ബാഹ്യ ശൂന്യം ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
ബാഹ്യ errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
ബാഹ്യ errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t അക്കം,
					 ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *out);
ബാഹ്യ errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t അക്കം,
					 ശൂന്യം *out);
ബാഹ്യ errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t അക്കം,
					 ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *out);
ബാഹ്യ errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t അക്കം,
					 ശൂന്യം *out);

/* blknum.c */
ബാഹ്യ __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
ബാഹ്യ __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
ബാഹ്യ dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
ബാഹ്യ blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_group_blocks_എണ്ണം(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					 ഘടന ext2_inode *inode);
ബാഹ്യ blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     ഘടന ext2_inode *inode);
ബാഹ്യ blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					ഘടന ext2_inode *inode);
ബാഹ്യ blk64_t ext2fs_blocks_എണ്ണം(ഘടന ext2_super_block *super);
ബാഹ്യ ശൂന്യം ext2fs_blocks_എണ്ണം_set(ഘടന ext2_super_block *super,
				    blk64_t blk);
ബാഹ്യ ശൂന്യം ext2fs_blocks_എണ്ണം_add(ഘടന ext2_super_block *super,
				    blk64_t blk);
ബാഹ്യ blk64_t ext2fs_r_blocks_എണ്ണം(ഘടന ext2_super_block *super);
ബാഹ്യ ശൂന്യം ext2fs_r_blocks_എണ്ണം_set(ഘടന ext2_super_block *super,
				      blk64_t blk);
ബാഹ്യ ശൂന്യം ext2fs_r_blocks_എണ്ണം_add(ഘടന ext2_super_block *super,
				      blk64_t blk);
ബാഹ്യ blk64_t ext2fs_free_blocks_എണ്ണം(ഘടന ext2_super_block *super);
ബാഹ്യ ശൂന്യം ext2fs_free_blocks_എണ്ണം_set(ഘടന ext2_super_block *super,
					 blk64_t blk);
ബാഹ്യ ശൂന്യം ext2fs_free_blocks_എണ്ണം_add(ഘടന ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
ബാഹ്യ ഘടന ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  ഘടന opaque_ext2_group_desc *gdp,
					  dgrp_t group);
ബാഹ്യ blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
ബാഹ്യ __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
ബാഹ്യ blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
ബാഹ്യ __u32 ext2fs_bg_free_blocks_എണ്ണം(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_free_blocks_എണ്ണം_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
ബാഹ്യ __u32 ext2fs_bg_free_inodes_എണ്ണം(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_free_inodes_എണ്ണം_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
ബാഹ്യ __u32 ext2fs_bg_used_dirs_എണ്ണം(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_used_dirs_എണ്ണം_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
ബാഹ്യ __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
ബാഹ്യ __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
ബാഹ്യ ശൂന്യം ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
ബാഹ്യ ശൂന്യം ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
ബാഹ്യ __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
ബാഹ്യ ശൂന്യം ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
ബാഹ്യ blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     സ്ഥിരം ഘടന ext2_inode *inode);
ബാഹ്യ ശൂന്യം ext2fs_file_acl_block_set(ext2_filsys fs,
				      ഘടന ext2_inode *inode, blk64_t blk);
ബാഹ്യ errcode_t ext2fs_inode_size_set(ext2_filsys fs, ഘടന ext2_inode *inode,
				       ext2_off64_t size);

/* block.c */
ബാഹ്യ errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      പൂർണ്ണസംഖ്യ	flags,
				      അക്ഷരം *block_buf,
				      പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  പൂർണ്ണസംഖ്യ	blockcnt,
						  ശൂന്യം	*priv_data),
				      ശൂന്യം *priv_data);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				പൂർണ്ണസംഖ്യ	flags,
				അക്ഷരം *block_buf,
				പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    പൂർണ്ണസംഖ്യ		ref_offset,
					    ശൂന്യം	*priv_data),
				ശൂന്യം *priv_data);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				പൂർണ്ണസംഖ്യ	flags,
				അക്ഷരം *block_buf,
				പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    പൂർണ്ണസംഖ്യ		ref_offset,
					    ശൂന്യം	*priv_data),
				ശൂന്യം *priv_data);

/* bmap.c */
ബാഹ്യ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ഘടന ext2_inode *inode,
			     അക്ഷരം *block_buf, പൂർണ്ണസംഖ്യ bmap_flags,
			     blk_t block, blk_t *phys_blk);
ബാഹ്യ errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ഘടന ext2_inode *inode,
			      അക്ഷരം *block_buf, പൂർണ്ണസംഖ്യ bmap_flags, blk64_t block,
			      പൂർണ്ണസംഖ്യ *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   ഘടന ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#എങ്കിൽ 0
/* bmove.c */
ബാഹ്യ errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    പൂർണ്ണസംഖ്യ flags);
#endif

/* check_desc.c */
ബാഹ്യ errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
ബാഹ്യ errcode_t ext2fs_close(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_close2(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_close_free(ext2_filsys *fs);
ബാഹ്യ errcode_t ext2fs_flush(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_flush2(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
ബാഹ്യ errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    പൂർണ്ണസംഖ്യ *ret_meta_bg);
ബാഹ്യ ശൂന്യം ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
ബാഹ്യ __u32 ext2fs_crc32_be(__u32 crc, unsigned അക്ഷരം സ്ഥിരം *p, size_t len);
ബാഹ്യ __u32 ext2fs_crc32c_le(__u32 crc, unsigned അക്ഷരം സ്ഥിരം *p, size_t len);

/* csum.c */
ബാഹ്യ ശൂന്യം ext2fs_init_csum_seed(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ഘടന mmp_struct *mmp);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mmp_csum_verify(ext2_filsys, ഘടന mmp_struct *mmp);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_verify_csum_type(ext2_filsys fs, ഘടന ext2_super_block *sb);
ബാഹ്യ errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    ഘടന ext2_super_block *sb);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ഘടന ext2_super_block *sb);
ബാഹ്യ errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					ഘടന ext2_ext_attr_header *hdr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     ഘടന ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_TAIL(block, blocksize) \
	((ഘടന ext2_dir_entry_tail *)(((അക്ഷരം *)(block)) + \
	(blocksize) - വലുപ്പം(ഘടന ext2_dir_entry_tail)))

ബാഹ്യ ശൂന്യം ext2fs_initialize_dirent_tail(ext2_filsys fs,
					  ഘടന ext2_dir_entry_tail *t);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dirent_has_tail(ext2_filsys fs,
				  ഘടന ext2_dir_entry *dirent);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ഘടന ext2_dir_entry *dirent);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ഘടന ext2_dir_entry *dirent);
ബാഹ്യ errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   ഘടന ext2_dir_entry *dirent);
ബാഹ്യ errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ഘടന ext2_dir_entry *dirent,
					  ഘടന ext2_dx_countlimit **cc,
					  പൂർണ്ണസംഖ്യ *offset);
ബാഹ്യ errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ഘടന ext2_dir_entry *dirent,
				__u32 *crc, ഘടന ext2_dx_tail **ret_t);
ബാഹ്യ errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      ഘടന ext3_extent_header *eh);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ഘടന ext3_extent_header *eh);
ബാഹ്യ errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ഘടന ext2_inode_large *inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ഘടന ext2_inode_large *inode);
ബാഹ്യ ശൂന്യം ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
ബാഹ്യ errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
ബാഹ്യ __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
ബാഹ്യ errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
ബാഹ്യ errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, പൂർണ്ണസംഖ്യ blockcnt);
ബാഹ്യ errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ബാഹ്യ ശൂന്യം ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(സ്ഥിരം ശൂന്യം *,
							   സ്ഥിരം ശൂന്യം *));
ബാഹ്യ ശൂന്യം ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(സ്ഥിരം ശൂന്യം *,
							    സ്ഥിരം ശൂന്യം *));
ബാഹ്യ errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs, ഘടന ext2_db_entry *db_info,
		    ശൂന്യം	*priv_data),
	ശൂന്യം *priv_data);
ബാഹ്യ errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs, ഘടന ext2_db_entry2 *db_info,
		    ശൂന്യം	*priv_data),
	ശൂന്യം *priv_data);
ബാഹ്യ errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs, ഘടന ext2_db_entry2 *db_info,
		    ശൂന്യം	*priv_data),
	unsigned long long start,
	unsigned long long എണ്ണം,
	ശൂന്യം *priv_data);
ബാഹ്യ errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, പൂർണ്ണസംഖ്യ blockcnt);
ബാഹ്യ errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ബാഹ്യ errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dblist_എണ്ണം(ext2_dblist dblist);
ബാഹ്യ blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
ബാഹ്യ errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					ഘടന ext2_db_entry **entry);
ബാഹ്യ errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ഘടന ext2_db_entry2 **entry);
ബാഹ്യ errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
ബാഹ്യ errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  പൂർണ്ണസംഖ്യ	flags,
				  അക്ഷരം	*block_buf,
				  പൂർണ്ണസംഖ്യ (*func)(ext2_ino_t	dir,
					      പൂർണ്ണസംഖ്യ		entry,
					      ഘടന ext2_dir_entry *dirent,
					      പൂർണ്ണസംഖ്യ	offset,
					      പൂർണ്ണസംഖ്യ	blocksize,
					      അക്ഷരം	*buf,
					      ശൂന്യം	*priv_data),
				  ശൂന്യം *priv_data);

#എങ്കിൽ 0
/* digest_encode.c */
#define EXT2FS_DIGEST_SIZE EXT2FS_SHA256_LENGTH
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_digest_encode(സ്ഥിരം അക്ഷരം *src, പൂർണ്ണസംഖ്യ len, അക്ഷരം *dst);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_digest_decode(സ്ഥിരം അക്ഷരം *src, പൂർണ്ണസംഖ്യ len, അക്ഷരം *dst);
#endif

/* dirblock.c */
ബാഹ്യ errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags, ext2_ino_t ino);
ബാഹ്യ errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags, ext2_ino_t ino);

/* dirhash.c */
ബാഹ്യ errcode_t ext2fs_dirhash(പൂർണ്ണസംഖ്യ version, സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
				സ്ഥിരം __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

ബാഹ്യ errcode_t ext2fs_dirhash2(പൂർണ്ണസംഖ്യ version, സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
				 സ്ഥിരം ഘടന ext2fs_nls_table *charset,
				 പൂർണ്ണസംഖ്യ hash_flags,
				 സ്ഥിരം __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
ബാഹ്യ errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ഘടന ext2_dir_entry *dirent,
				    unsigned പൂർണ്ണസംഖ്യ *rec_len);
ബാഹ്യ errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    unsigned പൂർണ്ണസംഖ്യ len,
				    ഘടന ext2_dir_entry *dirent);
ബാഹ്യ errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      പൂർണ്ണസംഖ്യ flags,
			      അക്ഷരം *block_buf,
			      പൂർണ്ണസംഖ്യ (*func)(ഘടന ext2_dir_entry *dirent,
					  പൂർണ്ണസംഖ്യ	offset,
					  പൂർണ്ണസംഖ്യ	blocksize,
					  അക്ഷരം	*buf,
					  ശൂന്യം	*priv_data),
			      ശൂന്യം *priv_data);
ബാഹ്യ errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      പൂർണ്ണസംഖ്യ flags,
			      അക്ഷരം *block_buf,
			      പൂർണ്ണസംഖ്യ (*func)(ext2_ino_t	dir,
					  പൂർണ്ണസംഖ്യ	entry,
					  ഘടന ext2_dir_entry *dirent,
					  പൂർണ്ണസംഖ്യ	offset,
					  പൂർണ്ണസംഖ്യ	blocksize,
					  അക്ഷരം	*buf,
					  ശൂന്യം	*priv_data),
			      ശൂന്യം *priv_data);

/* dupfs.c */
ബാഹ്യ errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
ബാഹ്യ errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
ബാഹ്യ __u32 ext2fs_ext_attr_hash_entry(ഘടന ext2_ext_attr_entry *entry,
					ശൂന്യം *data);
ബാഹ്യ __u32 ext2fs_ext_attr_hash_entry_signed(ഘടന ext2_ext_attr_entry *entry,
					       ശൂന്യം *data);
ബാഹ്യ errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ഘടന ext2_ext_attr_entry *entry,
					     ശൂന്യം *data, __u32 *hash);
ബാഹ്യ errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ഘടന ext2_ext_attr_entry *entry,
					     ശൂന്യം *data, __u32 *hash,
					     __u32 *signed_hash);
ബാഹ്യ errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       ശൂന്യം *buf, ext2_ino_t inum);
ബാഹ്യ errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       ശൂന്യം *buf);
ബാഹ്യ errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       ശൂന്യം *buf, ext2_ino_t inum);
ബാഹ്യ errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   അക്ഷരം *block_buf,
					   പൂർണ്ണസംഖ്യ adjust, __u32 *newcount);
ബാഹ്യ errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   അക്ഷരം *block_buf,
					   പൂർണ്ണസംഖ്യ adjust, __u32 *newcount);
ബാഹ്യ errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   അക്ഷരം *block_buf,
					   പൂർണ്ണസംഖ്യ adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ഘടന ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ഘടന ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ഘടന ext2_xattr_handle *handle,
				   ഘടന ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ഘടന ext2_xattr_handle *h,
				പൂർണ്ണസംഖ്യ (*func)(അക്ഷരം *name, അക്ഷരം *മൂല്യം,
					    size_t മൂല്യം_len, ശൂന്യം *data),
				ശൂന്യം *data);
errcode_t ext2fs_xattr_get(ഘടന ext2_xattr_handle *h, സ്ഥിരം അക്ഷരം *താക്കോൽ,
			   ശൂന്യം **മൂല്യം, size_t *മൂല്യം_len);
errcode_t ext2fs_xattr_set(ഘടന ext2_xattr_handle *handle,
			   സ്ഥിരം അക്ഷരം *താക്കോൽ,
			   സ്ഥിരം ശൂന്യം *മൂല്യം,
			   size_t മൂല്യം_len);
errcode_t ext2fs_xattr_remove(ഘടന ext2_xattr_handle *handle,
			      സ്ഥിരം അക്ഷരം *താക്കോൽ);
errcode_t ext2fs_xattr_remove_all(ഘടന ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ഘടന ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ഘടന ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ഘടന ext2_inode_large *inode);
errcode_t ext2fs_xattrs_എണ്ണം(ഘടന ext2_xattr_handle *handle, size_t *എണ്ണം);
errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ഘടന ext2_xattr_handle *handle,
			      unsigned പൂർണ്ണസംഖ്യ *new_flags, unsigned പൂർണ്ണസംഖ്യ *old_flags);
ബാഹ്യ ശൂന്യം ext2fs_ext_attr_block_rehash(ഘടന ext2_ext_attr_header *header,
					 ഘടന ext2_ext_attr_entry *end);
ബാഹ്യ __u32 ext2fs_get_ea_inode_hash(ഘടന ext2_inode *inode);
ബാഹ്യ ശൂന്യം ext2fs_set_ea_inode_hash(ഘടന ext2_inode *inode, __u32 hash);
ബാഹ്യ __u64 ext2fs_get_ea_inode_ref(ഘടന ext2_inode *inode);
ബാഹ്യ ശൂന്യം ext2fs_set_ea_inode_ref(ഘടന ext2_inode *inode, __u64 ref_എണ്ണം);

/* extent.c */
ബാഹ്യ errcode_t ext2fs_extent_header_verify(ശൂന്യം *ptr, പൂർണ്ണസംഖ്യ size);
ബാഹ്യ errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
ബാഹ്യ errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ഘടന ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
ബാഹ്യ ശൂന്യം ext2fs_extent_free(ext2_extent_handle_t handle);
ബാഹ്യ errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   പൂർണ്ണസംഖ്യ flags, ഘടന ext2fs_extent *extent);
ബാഹ്യ errcode_t ext2fs_extent_കണ്ണി_split(ext2_extent_handle_t handle);
ബാഹ്യ errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, പൂർണ്ണസംഖ്യ flags,
				       ഘടന ext2fs_extent *extent);
ബാഹ്യ errcode_t ext2fs_extent_തിരുകുക(ext2_extent_handle_t handle, പൂർണ്ണസംഖ്യ flags,
				      ഘടന ext2fs_extent *extent);
ബാഹ്യ errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ഘടന ext2_extent_info *info);
ബാഹ്യ errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
ബാഹ്യ errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     പൂർണ്ണസംഖ്യ leaf_level, blk64_t blk);
ബാഹ്യ errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
ബാഹ്യ size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle);
ബാഹ്യ errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ഘടന ext2_inode *inode);
ബാഹ്യ errcode_t ext2fs_എണ്ണം_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ഘടന ext2_inode *inode, blk64_t *ret_എണ്ണം);
ബാഹ്യ errcode_t ext2fs_decode_extent(ഘടന ext2fs_extent *to, ശൂന്യം *from,
				      പൂർണ്ണസംഖ്യ len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, ext2_ino_t ino,
			   ഘടന ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
ബാഹ്യ errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ഘടന ext2_inode *inode,
				   പൂർണ്ണസംഖ്യ flags, ext2_file_t *ret);
ബാഹ്യ errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  പൂർണ്ണസംഖ്യ flags, ext2_file_t *ret);
ബാഹ്യ ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ഘടന ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
ബാഹ്യ ext2_ino_t ext2fs_file_get_inode_അക്കം(ext2_file_t file);
ബാഹ്യ errcode_t ext2fs_file_close(ext2_file_t file);
ബാഹ്യ errcode_t ext2fs_file_flush(ext2_file_t file);
ബാഹ്യ errcode_t ext2fs_file_read(ext2_file_t file, ശൂന്യം *buf,
				  unsigned പൂർണ്ണസംഖ്യ wanted, unsigned പൂർണ്ണസംഖ്യ *got);
ബാഹ്യ errcode_t ext2fs_file_write(ext2_file_t file, സ്ഥിരം ശൂന്യം *buf,
				   unsigned പൂർണ്ണസംഖ്യ nbytes, unsigned പൂർണ്ണസംഖ്യ *written);
ബാഹ്യ errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   പൂർണ്ണസംഖ്യ whence, __u64 *ret_pos);
ബാഹ്യ errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   പൂർണ്ണസംഖ്യ whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size);
ബാഹ്യ ext2_off_t ext2fs_file_get_size(ext2_file_t file);
ബാഹ്യ errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size);
ബാഹ്യ errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size);

/* finddev.c */
ബാഹ്യ അക്ഷരം *ext2fs_find_block_device(dev_t device);

/* flushb.c */
ബാഹ്യ errcode_t ext2fs_sync_device(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flushb);

/* freefs.c */
ബാഹ്യ ശൂന്യം ext2fs_free(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_free_dblist(ext2_dblist dblist);
ബാഹ്യ ശൂന്യം ext2fs_badblocks_list_free(ext2_badblocks_list bb);
ബാഹ്യ ശൂന്യം ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
ബാഹ്യ ശൂന്യം ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 end,
					    __u32 real_end,
					    സ്ഥിരം അക്ഷരം *descr, അക്ഷരം *init_map,
					    ext2fs_generic_bitmap *ret);
ബാഹ്യ errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						സ്ഥിരം അക്ഷരം *descr,
						ext2fs_generic_bitmap *ret);
ബാഹ്യ errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
ബാഹ്യ ശൂന്യം ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
ബാഹ്യ ശൂന്യം ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
ബാഹ്യ errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
ബാഹ്യ errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
ബാഹ്യ errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 അക്കം,
						 ശൂന്യം *out);
ബാഹ്യ errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 അക്കം,
						 ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
ബാഹ്യ errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
ശൂന്യം ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    പൂർണ്ണസംഖ്യ type, __u64 start, __u64 end,
				    __u64 real_end,
				    സ്ഥിരം അക്ഷരം *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
ശൂന്യം ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
ശൂന്യം ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_എണ്ണം_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_എണ്ണം_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ ext2fs_list_backups(ext2_filsys fs, unsigned പൂർണ്ണസംഖ്യ *three,
				unsigned പൂർണ്ണസംഖ്യ *five, unsigned പൂർണ്ണസംഖ്യ *seven);

/* getenv.c */
ബാഹ്യ അക്ഷരം *ext2fs_safe_getenv(സ്ഥിരം അക്ഷരം *arg);

/* get_num_dirs.c */
ബാഹ്യ errcode_t ext2fs_get_അക്കം_dirs(ext2_filsys fs, ext2_ino_t *ret_അക്കം_dirs);

/* getsize.c */
ബാഹ്യ errcode_t ext2fs_get_device_size(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
					blk_t *retblocks);
ബാഹ്യ errcode_t ext2fs_get_device_size2(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_get_dio_alignment(പൂർണ്ണസംഖ്യ fd);
errcode_t ext2fs_get_device_sectsize(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ഘടന ext2_inode *inode,
				 blk64_t അക്കം_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ഘടന ext2_inode *inode,
				 blk64_t അക്കം_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, ഘടന ext2_inode *inode, blk64_t b);

/* imager.c */
ബാഹ്യ errcode_t ext2fs_image_inode_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_image_inode_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_image_super_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_image_super_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, ശൂന്യം *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, ശൂന്യം *buf);

/* initialize.c */
ബാഹ്യ errcode_t ext2fs_initialize(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags,
				   ഘടന ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
ബാഹ്യ errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, പൂർണ്ണസംഖ്യ super_only);

/* icount.c */
ബാഹ്യ ശൂന്യം ext2fs_free_icount(ext2_icount_t icount);
ബാഹ്യ errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, അക്ഷരം *tdb_dir,
					  പൂർണ്ണസംഖ്യ flags, ext2_icount_t *ret);
ബാഹ്യ errcode_t ext2fs_create_icount2(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
				       unsigned പൂർണ്ണസംഖ്യ size,
				       ext2_icount_t hint, ext2_icount_t *ret);
ബാഹ്യ errcode_t ext2fs_create_icount(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
				      unsigned പൂർണ്ണസംഖ്യ size,
				      ext2_icount_t *ret);
ബാഹ്യ errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
ബാഹ്യ errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ബാഹ്യ errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ബാഹ്യ errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 എണ്ണം);
ബാഹ്യ ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

ബാഹ്യ errcode_t ext2fs_get_memalign(unsigned long size,
				     unsigned long align, ശൂന്യം *ptr);

/* inline_data.c */
ബാഹ്യ errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino);
ബാഹ്യ errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino,
					 size_t *size);
ബാഹ്യ errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
					ഘടന ext2_inode *inode,
					ശൂന്യം *buf, size_t *size);
ബാഹ്യ errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
					ഘടന ext2_inode *inode,
					ശൂന്യം *buf, size_t size);

/* inode.c */
ബാഹ്യ errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned പൂർണ്ണസംഖ്യ cache_size);
ബാഹ്യ ശൂന്യം ext2fs_free_inode_cache(ഘടന ext2_inode_cache *icache);
ബാഹ്യ errcode_t ext2fs_flush_icache(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    ഘടന ext2_inode *inode,
					    പൂർണ്ണസംഖ്യ bufsize);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8
ബാഹ്യ errcode_t ext2fs_open_inode_scan(ext2_filsys fs, പൂർണ്ണസംഖ്യ buffer_blocks,
				  ext2_inode_scan *ret_scan);
ബാഹ്യ ശൂന്യം ext2fs_close_inode_scan(ext2_inode_scan scan);
ബാഹ്യ errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       ഘടന ext2_inode *inode);
ബാഹ്യ errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   പൂർണ്ണസംഖ്യ	group);
ബാഹ്യ ശൂന്യം ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 ശൂന്യം * priv_data),
	 ശൂന്യം *done_group_data);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inode_scan_flags(ext2_inode_scan scan, പൂർണ്ണസംഖ്യ set_flags,
				   പൂർണ്ണസംഖ്യ clear_flags);
ബാഹ്യ errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					ഘടന ext2_inode * inode,
					പൂർണ്ണസംഖ്യ bufsize);
ബാഹ്യ errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ഘടന ext2_inode * inode);
ബാഹ്യ errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ഘടന ext2_inode * inode,
				    പൂർണ്ണസംഖ്യ bufsize, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 ഘടന ext2_inode * inode,
					 പൂർണ്ണസംഖ്യ bufsize);
ബാഹ്യ errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ഘടന ext2_inode * inode);
ബാഹ്യ errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ഘടന ext2_inode * inode,
				     പൂർണ്ണസംഖ്യ bufsize, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ഘടന ext2_inode * inode);
ബാഹ്യ errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
ബാഹ്യ errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
ബാഹ്യ io_manager inode_io_manager;
ബാഹ്യ errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					അക്ഷരം **name);
ബാഹ്യ errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ഘടന ext2_inode *inode,
					 അക്ഷരം **name);

/* ismounted.c */
ബാഹ്യ errcode_t ext2fs_check_if_mounted(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *mount_flags);
ബാഹ്യ errcode_t ext2fs_check_mount_point(സ്ഥിരം അക്ഷരം *device, പൂർണ്ണസംഖ്യ *mount_flags,
					  അക്ഷരം *mtpt, പൂർണ്ണസംഖ്യ mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
ബാഹ്യ errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ഘടന ext2_inode *inode,
			      അക്ഷരം *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
ബാഹ്യ errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, സ്ഥിരം അക്ഷരം *name,
			 പൂർണ്ണസംഖ്യ namelen, അക്ഷരം *buf, ext2_ino_t *inode);
ബാഹ്യ errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
			സ്ഥിരം അക്ഷരം *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
			      സ്ഥിരം അക്ഷരം *name, ext2_ino_t *inode);
ബാഹ്യ errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
പൂർണ്ണസംഖ്യ ext2fs_native_flag(ശൂന്യം);

/* newdir.c */
ബാഹ്യ errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, അക്ഷരം **block);
ബാഹ്യ errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
ബാഹ്യ സ്ഥിരം ഘടന ext2fs_nls_table *ext2fs_load_nls_table(പൂർണ്ണസംഖ്യ encoding);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_check_encoded_name(സ്ഥിരം ഘടന ext2fs_nls_table *table,
				     അക്ഷരം *s, size_t len, അക്ഷരം **pos);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_casefold_cmp(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			       സ്ഥിരം unsigned അക്ഷരം *str1, size_t len1,
			       സ്ഥിരം unsigned അക്ഷരം *str2, size_t len2);

/* mkdir.c */
ബാഹ്യ errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      സ്ഥിരം അക്ഷരം *name);
ബാഹ്യ errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, unsigned long flags,
			       പൂർണ്ണസംഖ്യ link_flags, സ്ഥിരം അക്ഷരം *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ഘടന ext2fs_journal_params {
	blk_t അക്കം_journal_blocks;
	blk_t അക്കം_fc_blocks;
};
ബാഹ്യ errcode_t ext2fs_get_journal_params(
		ഘടന ext2fs_journal_params *params, ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, പൂർണ്ണസംഖ്യ അക്കം,
				    blk_t *ret_blk, പൂർണ്ണസംഖ്യ *ret_എണ്ണം);
ബാഹ്യ errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, പൂർണ്ണസംഖ്യ അക്കം,
				     blk64_t *ret_blk, പൂർണ്ണസംഖ്യ *ret_എണ്ണം);
ബാഹ്യ errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 അക്കം_blocks, പൂർണ്ണസംഖ്യ flags,
						  അക്ഷരം  **ret_jsb);
ബാഹ്യ errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ഘടന ext2fs_journal_params *params,
						  പൂർണ്ണസംഖ്യ flags, അക്ഷരം  **ret_jsb);
ബാഹ്യ errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
ബാഹ്യ errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t അക്കം_blocks,
					  പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t അക്കം_blocks,
					   blk64_t goal, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    ഘടന ext2fs_journal_params *params,
				    blk64_t goal, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_default_journal_size(__u64 അക്കം_blocks);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_journal_sb_start(പൂർണ്ണസംഖ്യ blocksize);

/* openfs.c */
ബാഹ്യ errcode_t ext2fs_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ superblock,
			     unsigned പൂർണ്ണസംഖ്യ block_size, io_manager manager,
			     ext2_filsys *ret_fs);
ബാഹ്യ errcode_t ext2fs_open2(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *io_options,
			      പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ superblock,
			      unsigned പൂർണ്ണസംഖ്യ block_size, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
ബാഹ്യ blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
ബാഹ്യ blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
ബാഹ്യ errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t അക്കം_blocks);
ബാഹ്യ errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
ബാഹ്യ e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
ബാഹ്യ __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      അക്ഷരം *buf);
ബാഹ്യ errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   അക്ഷരം *buf);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, അക്ഷരം *buf);

/* get_pathname.c */
ബാഹ്യ errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       അക്ഷരം **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, സ്ഥിരം അക്ഷരം *name,
		      ext2_ino_t ino, പൂർണ്ണസംഖ്യ flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, സ്ഥിരം അക്ഷരം *name,
			ext2_ino_t ino, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ ext2fs_dir_is_dx(ext2_filsys fs, സ്ഥിരം ഘടന ext2_inode *inode);
ശൂന്യം ext2fs_inc_nlink(ext2_filsys fs, ഘടന ext2_inode *inode);
ശൂന്യം ext2fs_dec_nlink(ഘടന ext2_inode *inode);
പൂർണ്ണസംഖ്യ ext2fs_dir_link_max(ext2_filsys fs, ഘടന ext2_inode_large *inode);
പൂർണ്ണസംഖ്യ ext2fs_dir_link_empty(ഘടന ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *target);
പൂർണ്ണസംഖ്യ ext2fs_is_fast_symlink(ഘടന ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ശൂന്യം *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ശൂന്യം *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, പൂർണ്ണസംഖ്യ immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
unsigned ext2fs_mmp_new_seq(ശൂന്യം);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ശൂന്യം **ptr);

/* read_bb.c */
ബാഹ്യ errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
ബാഹ്യ errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      ശൂന്യം *priv_data,
				      ശൂന്യം (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      അക്ഷരം *badstr,
						      ശൂന്യം *priv_data));
ബാഹ്യ errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     ശൂന്യം (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
ബാഹ്യ errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
ബാഹ്യ errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ അക്കം_threads);
ബാഹ്യ errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
ബാഹ്യ errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2FS_SHA256_LENGTH 32
#എങ്കിൽ 0
ബാഹ്യ ശൂന്യം ext2fs_sha256(സ്ഥിരം unsigned അക്ഷരം *in, unsigned long in_size,
		   unsigned അക്ഷരം out[EXT2FS_SHA256_LENGTH]);
#endif

/* sha512.c */
#define EXT2FS_SHA512_LENGTH 64
ബാഹ്യ ശൂന്യം ext2fs_sha512(സ്ഥിരം unsigned അക്ഷരം *in, unsigned long in_size,
			  unsigned അക്ഷരം out[EXT2FS_SHA512_LENGTH]);

/* swapfs.c */
ബാഹ്യ errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, അക്ഷരം *buf, size_t size,
					പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, അക്ഷരം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, അക്ഷരം *buf, size_t size,
					 പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, അക്ഷരം *buf, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ ശൂന്യം ext2fs_swap_ext_attr(അക്ഷരം *to, അക്ഷരം *from, പൂർണ്ണസംഖ്യ bufsize,
				 പൂർണ്ണസംഖ്യ has_header);
ബാഹ്യ ശൂന്യം ext2fs_swap_ext_attr_header(ഘടന ext2_ext_attr_header *to_header,
					ഘടന ext2_ext_attr_header *from_hdr);
ബാഹ്യ ശൂന്യം ext2fs_swap_ext_attr_entry(ഘടന ext2_ext_attr_entry *to_entry,
				       ഘടന ext2_ext_attr_entry *from_entry);
ബാഹ്യ ശൂന്യം ext2fs_swap_super(ഘടന ext2_super_block * super);
ബാഹ്യ ശൂന്യം ext2fs_swap_group_desc(ഘടന ext2_group_desc *gdp);
ബാഹ്യ ശൂന്യം ext2fs_swap_group_desc2(ext2_filsys, ഘടന ext2_group_desc *gdp);
ബാഹ്യ ശൂന്യം ext2fs_swap_inode_full(ext2_filsys fs, ഘടന ext2_inode_large *t,
				   ഘടന ext2_inode_large *f, പൂർണ്ണസംഖ്യ hostorder,
				   പൂർണ്ണസംഖ്യ bufsize);
ബാഹ്യ ശൂന്യം ext2fs_swap_inode(ext2_filsys fs,ഘടന ext2_inode *t,
			      ഘടന ext2_inode *f, പൂർണ്ണസംഖ്യ hostorder);
ബാഹ്യ ശൂന്യം ext2fs_swap_mmp(ഘടന mmp_struct *mmp);

/* unix_io.c */
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_open_file(സ്ഥിരം അക്ഷരം *pathname, പൂർണ്ണസംഖ്യ flags, mode_t mode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_stat(സ്ഥിരം അക്ഷരം *path, ext2fs_struct_stat *buf);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fstat(പൂർണ്ണസംഖ്യ fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inode_has_valid_blocks(ഘടന ext2_inode *inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ഘടന ext2_inode *inode);

/* version.c */
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_parse_version_string(സ്ഥിരം അക്ഷരം *ver_string);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_get_library_version(സ്ഥിരം അക്ഷരം **ver_string,
				      സ്ഥിരം അക്ഷരം **date_string);

/* write_bb_file.c */
ബാഹ്യ errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      unsigned പൂർണ്ണസംഖ്യ flags,
				      FILE *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
ബാഹ്യ errcode_t ext2fs_get_mem(unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_get_memzero(unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_get_array(unsigned long എണ്ണം,
				  unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_get_arrayzero(unsigned long എണ്ണം,
				      unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_free_mem(ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_resize_mem(unsigned long old_size,
				   unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ errcode_t ext2fs_resize_array(unsigned long old_എണ്ണം, unsigned long എണ്ണം,
				     unsigned long size, ശൂന്യം *ptr);
ബാഹ്യ ശൂന്യം ext2fs_mark_super_dirty(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_mark_changed(ext2_filsys fs);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_changed(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_mark_valid(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_unmark_valid(ext2_filsys fs);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_valid(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_mark_ib_dirty(ext2_filsys fs);
ബാഹ്യ ശൂന്യം ext2fs_mark_bb_dirty(ext2_filsys fs);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_ib_dirty(ext2_filsys fs);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_bb_dirty(ext2_filsys fs);
ബാഹ്യ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
ബാഹ്യ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
ബാഹ്യ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
ബാഹ്യ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
				      ഘടന ext2_inode *inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, പൂർണ്ണസംഖ്യ blocks);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_log2_u32(__u32 arg);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_log2_u64(__u64 arg);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_log10_u32(__u32 arg);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_log10_u64(__u64 arg);
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ ext2fs_div_ceil(unsigned പൂർണ്ണസംഖ്യ a, unsigned പൂർണ്ണസംഖ്യ b);
ബാഹ്യ __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dirent_name_len(സ്ഥിരം ഘടന ext2_dir_entry *entry);
ബാഹ്യ ശൂന്യം ext2fs_dirent_set_name_len(ഘടന ext2_dir_entry *entry, പൂർണ്ണസംഖ്യ len);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_dirent_file_type(സ്ഥിരം ഘടന ext2_dir_entry *entry);
ബാഹ്യ ശൂന്യം ext2fs_dirent_set_file_type(ഘടന ext2_dir_entry *entry, പൂർണ്ണസംഖ്യ type);
ബാഹ്യ ഘടന ext2_inode *ext2fs_inode(ഘടന ext2_inode_large * large_inode);
ബാഹ്യ സ്ഥിരം ഘടന ext2_inode *ext2fs_const_inode(സ്ഥിരം ഘടന ext2_inode_large * large_inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inodes_per_orphan_block(ext2_filsys fs);
ബാഹ്യ ഘടന ext4_orphan_block_tail *ext2fs_orphan_block_tail(ext2_filsys fs,
							       അക്ഷരം *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#എങ്കിൽ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ ബാഹ്യ
#അല്ലെങ്കിൽ
#എങ്കിൽ (__STDC_VERSION__ >= 199901L)
#define _INLINE_ വരിയിൽ
#അല്ലെങ്കിൽ
#ifdef __GNUC__
#define _INLINE_ ബാഹ്യ __inline__
#അല്ലെങ്കിൽ				/* For Watcom C */
#define _INLINE_ ബാഹ്യ വരിയിൽ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#അന്തര്ഭാവ <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long size, ശൂന്യം *ptr)
{
	ശൂന്യം *pp;

	pp = malloc(size);
	എങ്കിൽ (!pp)
		തിരികെ EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, വലുപ്പം (pp));
	തിരികെ 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long size, ശൂന്യം *ptr)
{
	ശൂന്യം *pp;

	pp = malloc(size);
	എങ്കിൽ (!pp)
		തിരികെ EXT2_ET_NO_MEMORY;
	memset(pp, 0, size);
	memcpy(ptr, &pp, വലുപ്പം(pp));
	തിരികെ 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long എണ്ണം, unsigned long size,
				    ശൂന്യം *ptr)
{
	എങ്കിൽ (എണ്ണം && (~0UL)/എണ്ണം < size)
		തിരികെ EXT2_ET_NO_MEMORY;
	തിരികെ ext2fs_get_mem(എണ്ണം*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long എണ്ണം,
					unsigned long size, ശൂന്യം *ptr)
{
	എങ്കിൽ (എണ്ണം && (~0UL)/എണ്ണം < size)
		തിരികെ EXT2_ET_NO_MEMORY;

	തിരികെ ext2fs_get_memzero((size_t)എണ്ണം * size, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(ശൂന്യം *ptr)
{
	ശൂന്യം *p;

	memcpy(&p, ptr, വലുപ്പം(p));
	സ്വതന്ത്രം(p);
	p = 0;
	memcpy(ptr, &p, വലുപ്പം(p));
	തിരികെ 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_size,
				     unsigned long size, ശൂന്യം *ptr)
{
	ശൂന്യം *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, വലുപ്പം(p));
	p = realloc(p, size);
	എങ്കിൽ (!p)
		തിരികെ EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, വലുപ്പം(p));
	തിരികെ 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(unsigned long size,
				       unsigned long old_എണ്ണം,
				       unsigned long എണ്ണം, ശൂന്യം *ptr)
{
	unsigned long old_size;
	errcode_t retval;

	എങ്കിൽ (എണ്ണം && (~0UL)/എണ്ണം < size)
		തിരികെ EXT2_ET_NO_MEMORY;

	size *= എണ്ണം;
	old_size = size * old_എണ്ണം;
	retval = ext2fs_resize_mem(old_size, size, ptr);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (size > old_size) {
		ശൂന്യം *p;

		memcpy(&p, ptr, വലുപ്പം(p));
		memset((അക്ഷരം *)p + old_size, 0, size - old_size);
		memcpy(ptr, &p, വലുപ്പം(p));
	}

	തിരികെ 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ ശൂന്യം ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ ശൂന്യം ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_changed(ext2_filsys fs)
{
	തിരികെ (fs->flags & EXT2_FLAG_CHANGED);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ ശൂന്യം ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ ശൂന്യം ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_valid(ext2_filsys fs)
{
	തിരികെ (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ ശൂന്യം ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ ശൂന്യം ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_ib_dirty(ext2_filsys fs)
{
	തിരികെ (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_bb_dirty(ext2_filsys fs)
{
	തിരികെ (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	തിരികെ ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	തിരികെ (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	തിരികെ (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	തിരികെ (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					ഘടന ext2_inode *inode)
{
	തിരികെ (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, പൂർണ്ണസംഖ്യ blocks)
{
	പൂർണ്ണസംഖ്യ csum_size = 0;

	എങ്കിൽ ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_size = വലുപ്പം(ഘടന ext2_dx_tail);
	തിരികെ blocks * ((fs->blocksize - (8 + csum_size)) /
						വലുപ്പം(ഘടന ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_log2_u32(__u32 arg)
{
	പൂർണ്ണസംഖ്യ l = 0;

	arg >>= 1;
	ആകുമ്പോൾ (arg) {
		l++;
		arg >>= 1;
	}
	തിരികെ l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_log2_u64(__u64 arg)
{
	പൂർണ്ണസംഖ്യ l = 0;

	arg >>= 1;
	ആകുമ്പോൾ (arg) {
		l++;
		arg >>= 1;
	}
	തിരികെ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_log10_u32(__u32 arg)
{
	പൂർണ്ണസംഖ്യ l = 0;

	arg /= 10;
	ആകുമ്പോൾ (arg) {
		l++;
		arg /= 10;
	}
	തിരികെ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_log10_u64(__u64 arg)
{
	പൂർണ്ണസംഖ്യ l = 0;

	arg /= 10;
	ആകുമ്പോൾ (arg) {
		l++;
		arg /= 10;
	}
	തിരികെ l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ unsigned പൂർണ്ണസംഖ്യ ext2fs_div_ceil(unsigned പൂർണ്ണസംഖ്യ a, unsigned പൂർണ്ണസംഖ്യ b)
{
	എങ്കിൽ (!a)
		തിരികെ 0;
	തിരികെ ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	എങ്കിൽ (!a)
		തിരികെ 0;
	തിരികെ ((a - 1) / b) + 1;
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_dirent_name_len(സ്ഥിരം ഘടന ext2_dir_entry *entry)
{
	തിരികെ entry->name_len & 0xff;
}

_INLINE_ ശൂന്യം ext2fs_dirent_set_name_len(ഘടന ext2_dir_entry *entry, പൂർണ്ണസംഖ്യ len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_dirent_file_type(സ്ഥിരം ഘടന ext2_dir_entry *entry)
{
	തിരികെ entry->name_len >> 8;
}

_INLINE_ ശൂന്യം ext2fs_dirent_set_file_type(ഘടന ext2_dir_entry *entry, പൂർണ്ണസംഖ്യ type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ ഘടന ext2_inode *ext2fs_inode(ഘടന ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	തിരികെ (ഘടന ext2_inode *) large_inode;
}

_INLINE_ സ്ഥിരം ഘടന ext2_inode *
ext2fs_const_inode(സ്ഥിരം ഘടന ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	തിരികെ (സ്ഥിരം ഘടന ext2_inode *) large_inode;
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	തിരികെ (fs->blocksize - വലുപ്പം(ഘടന ext4_orphan_block_tail)) /
		വലുപ്പം(__u32);
}

_INLINE_ ഘടന ext4_orphan_block_tail *
ext2fs_orphan_block_tail(ext2_filsys fs, അക്ഷരം *buf)
{
	തിരികെ (ഘടന ext4_orphan_block_tail *)(buf + fs->blocksize -
		വലുപ്പം(ഘടന ext4_orphan_block_tail));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ ext2_dir_htree_level(ext2_filsys fs)
{
	എങ്കിൽ (ext2fs_has_feature_largedir(fs->super))
		തിരികെ EXT4_HTREE_LEVEL;

	തിരികെ EXT4_HTREE_LEVEL_COMPAT;
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
സ്ഥിതം വരിയിൽ ശൂന്യം ext2fs_set_dtime(ext2_filsys fs, ഘടന ext2_inode *inode)
{
	__u32	t;

	എങ്കിൽ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	അല്ലെങ്കിൽ
		t = time(ശൂന്യ) & 0xFFFFFFFF;
	എങ്കിൽ (t < fs->super->s_inodes_എണ്ണം)
		t = fs->super->s_inodes_എണ്ണം;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
