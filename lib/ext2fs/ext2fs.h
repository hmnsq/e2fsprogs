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
#अन्यथा
#define EXT2FS_ATTR(x)
#endif

#ifndef __nonstring
#ifdef __has_attribute
#यदि __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#अन्यथा
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#अन्यथा
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

#ifdef CONFIG_TDB
#define EXT2FS_NO_TDB_UNUSED
#अन्यथा
#define EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#endif

#ifdef __cplusplus
बाह्य "C" {
#endif

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#यदि (!defined(__GNUC__) && !defined(__WATCOMC__))
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
#अन्तर्भाव <sys/types.h>
#endif

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
#endif /* EXT2_FLAT_INCLUDES */

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
#endif

#अन्तर्भाव "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ifdef _MSC_VER
#define EXT2_QSORT_TYPE पूर्णाङ्क __cdecl
#अन्यथा
#define EXT2_QSORT_TYPE पूर्णाङ्क
#endif

प्रकारनिर्देश संरचना struct_ext2_filsys *ext2_filsys;

#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

संरचना ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#define EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


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

#define BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
संरचना ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
संरचना ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	पूर्णाङ्क	blockcnt;
};

प्रकारनिर्देश संरचना ext2_struct_dblist *ext2_dblist;

#define DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#define EXT2_FILE_WRITE		0x0001
#define EXT2_FILE_CREATE	0x0002

#define EXT2_FILE_MASK		0x00FF

#define EXT2_FILE_BUF_DIRTY	0x4000
#define EXT2_FILE_BUF_VALID	0x2000

प्रकारनिर्देश संरचना ext2_file *ext2_file_t;

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

संरचना blk_alloc_ctx;
संरचना opaque_ext2_group_desc;

संरचना struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	पूर्णाङ्क				flags;
	वर्ण *				device_name;
	संरचना ext2_super_block	* 	super;
	अचिन्हित पूर्णाङ्क			blocksize;
	पूर्णाङ्क				fragsize;
	dgrp_t				group_desc_गन्ती;
	अचिन्हित दीर्घ			desc_blocks;
	संरचना opaque_ext2_group_desc *	group_desc;
	अचिन्हित पूर्णाङ्क			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				संरचना ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				संरचना ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	संरचना ext2_super_block *	orig_super;
	संरचना ext2_image_hdr *		image_header;
	__u32				umask;
	समय_प्रकार				now;
	पूर्णाङ्क				cluster_ratio_bits;
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
	शून्य *				priv_data;

	/*
	 * Inode cache
	 */
	संरचना ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, संरचना blk_alloc_ctx *ctx);
	शून्य (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	शून्य *mmp_buf;
	शून्य *mmp_cmp;
	पूर्णाङ्क mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	दीर्घ mmp_last_written;

	/* progress operation functions */
	संरचना ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	वर्ण				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	शून्य (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t अङ्क,
					पूर्णाङ्क inuse);

	/* hashmap for SHA of data blocks */
	संरचना ext2fs_hashmap* block_sha_map;

	अचर संरचना ext2fs_nls_table *encoding;
};

#यदि EXT2_FLAT_INCLUDES
#अन्तर्भाव "e2_bitops.h"
#अन्यथा
#अन्तर्भाव <ext2fs/bitops.h>
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
#define BLOCK_गन्ती_IND		(-1)
#define BLOCK_गन्ती_DIND	(-2)
#define BLOCK_गन्ती_TIND	(-3)
#define BLOCK_गन्ती_TRANSLATOR	(-4)

#define BLOCK_ALLOC_UNKNOWN	0
#define BLOCK_ALLOC_DATA	1
#define BLOCK_ALLOC_METADATA	2

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
#define EXT2_BMOVE_GET_DBLIST	0x0001
#define EXT2_BMOVE_DEBUG	0x0002
#endif

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#define EXT2_EXTENT_FLAGS_LEAF		0x0001
#define EXT2_EXTENT_FLAGS_UNINIT	0x0002
#define EXT2_EXTENT_FLAGS_SECOND_VISIT	0x0004

संरचना ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

प्रकारनिर्देश संरचना ext2_extent_handle *ext2_extent_handle_t;
प्रकारनिर्देश संरचना ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#define EXT2_EXTENT_CURRENT	0x0000
#define EXT2_EXTENT_MOVE_MASK	0x000F
#define EXT2_EXTENT_जरा	0x0001
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
#define EXT2_EXTENT_घुसाउ_AFTER	0x0001 /* insert after handle loc'n */
#define EXT2_EXTENT_घुसाउ_NOSPLIT	0x0002 /* insert may not cause split */

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
संरचना ext2_extent_info {
	पूर्णाङ्क		curr_entry;
	पूर्णाङ्क		curr_level;
	पूर्णाङ्क		अङ्क_entries;
	पूर्णाङ्क		max_entries;
	पूर्णाङ्क		max_depth;
	पूर्णाङ्क		bytes_avail;
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
प्रकारनिर्देश संरचना ext2_struct_inode_scan *ext2_inode_scan;

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
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_माथिल्लो << 32))

स्थिर अन्तःस्थ __u32 __encode_extra_time(समय_प्रकार seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#यदि (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#endif
	फर्काऊ extra | (nsec << EXT4_EPOCH_BITS);
}
स्थिर अन्तःस्थ समय_प्रकार __decode_extra_sec(समय_प्रकार seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#यदि (SIZEOF_TIME_T > 4)
	यदि (extra & EXT4_EPOCH_MASK)
		seconds += ((समय_प्रकार)(extra & EXT4_EPOCH_MASK) << 32);
#endif
	फर्काऊ seconds;
}
स्थिर अन्तःस्थ __u32 __decode_extra_nsec(__u32 extra)
{
	फर्काऊ (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#define ext2fs_inode_actual_size(inode)					      \
	((माप_प्रकार)(EXT2_GOOD_OLD_INODE_SIZE +				      \
		  (परिमाण(*inode) > EXT2_GOOD_OLD_INODE_SIZE ?		      \
		   ((संरचना ext2_inode_large *)(inode))->i_extra_isize : 0)))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#define ext2fs_inode_xtime_set(inode, field, sec)			      \
गर {									      \
	यदि (ext2fs_inode_includes(ext2fs_inode_actual_size(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((संरचना ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} अन्यथा {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} जबसम्म (0)
#define ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_size(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((संरचना ext2_inode_large *)(inode))->field ## _extra) :      \
		(समय_प्रकार)(inode)->field)

स्थिर अन्तःस्थ शून्य __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   समय_प्रकार seconds)
{
	*lo = seconds & 0xffffffff;
#यदि (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#अन्यथा
	*hi = 0;
#endif
}
स्थिर अन्तःस्थ समय_प्रकार __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#यदि (SIZEOF_TIME_T == 4)
	फर्काऊ *lo;
#अन्यथा
	फर्काऊ ((समय_प्रकार)(*hi) << 32) | *lo;
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

प्रकारनिर्देश संरचना ext2_icount *ext2_icount_t;

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

#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) फर्काऊ (code)

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
#अन्यथा
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
#define EXT2_FS_अङ्क_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#यदि defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
प्रकारनिर्देश संरचना stat64 ext2fs_struct_stat;
#अन्यथा
प्रकारनिर्देश संरचना stat ext2fs_struct_stat;
#endif

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#define EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
संरचना ext2_xattr_handle;
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
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	फर्काऊ ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_needs_large_file_feature(अचिन्हित दीर्घ दीर्घ file_size)
{
	फर्काऊ file_size >= 0x80000000ULL;
}

/* alloc.c */
बाह्य शून्य ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
बाह्य errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, पूर्णाङ्क mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
बाह्य errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
बाह्य errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
बाह्य errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   संरचना blk_alloc_ctx *ctx);
बाह्य errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, पूर्णाङ्क अङ्क,
					ext2fs_block_bitmap map,
					blk_t *ret);
बाह्य errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, पूर्णाङ्क अङ्क,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    वर्ण *block_buf, blk_t *ret);
बाह्य errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     वर्ण *block_buf, blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     वर्ण *block_buf, blk64_t *ret,
				     संरचना blk_alloc_ctx *ctx);

बाह्य शून्य ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode *inode, blk64_t lblk);
बाह्य शून्य ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
बाह्य शून्य ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	शून्य (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्क, पूर्णाङ्क inuse),
	शून्य (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्क, पूर्णाङ्क inuse));
#define EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#define EXT2_NEWRANGE_MIN_LENGTH	(0x2)
#define EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#define EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#define EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#define EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
बाह्य पूर्णाङ्क ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
बाह्य errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
बाह्य शून्य ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
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
शून्य ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णाङ्क inuse);
शून्य ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse);
शून्य ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्क, पूर्णाङ्क inuse);

/* alloc_tables.c */
बाह्य errcode_t ext2fs_allocate_tables(ext2_filsys fs);
बाह्य errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
बाह्य errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णाङ्क size);
बाह्य errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णाङ्क ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णाङ्क ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
बाह्य errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
बाह्य पूर्णाङ्क ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
बाह्य शून्य ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
बाह्य errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
बाह्य पूर्णाङ्क ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

बाह्य errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    पूर्णाङ्क size);
बाह्य errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
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
बाह्य शून्य ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
बाह्य errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
बाह्य पूर्णाङ्क ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
बाह्य पूर्णाङ्क ext2fs_u32_list_गन्ती(ext2_u32_list bb);

/* bb_compat */
बाह्य errcode_t badblocks_list_create(badblocks_list *ret, पूर्णाङ्क size);
बाह्य errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
बाह्य पूर्णाङ्क badblocks_list_test(badblocks_list bb, blk_t blk);
बाह्य errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
बाह्य पूर्णाङ्क badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
बाह्य शून्य badblocks_list_iterate_end(badblocks_iterate iter);
बाह्य शून्य badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
बाह्य errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
बाह्य शून्य ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
बाह्य शून्य ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
बाह्य errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      अचर वर्ण *descr,
					      ext2fs_block_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   अचर वर्ण *descr,
						   ext2fs_block_bitmap *ret);
बाह्य पूर्णाङ्क ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
बाह्य errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      अचर वर्ण *descr,
					      ext2fs_inode_bitmap *ret);
बाह्य errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
बाह्य errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
बाह्य errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
बाह्य शून्य ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य शून्य ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
बाह्य errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_inode_bitmap bmap);
बाह्य errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_inode_bitmap bmap);
बाह्य errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
					    ext2fs_block_bitmap bmap);
बाह्य errcode_t ext2fs_resize_block_bitmap2(__u64 new_end,
					     __u64 new_real_end,
					     ext2fs_block_bitmap bmap);
बाह्य errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
					     ext2fs_block_bitmap bm2);
बाह्य errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
बाह्य errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *in);
बाह्य errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, माप_प्रकार अङ्क,
					 शून्य *in);
बाह्य errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *out);
बाह्य errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, माप_प्रकार अङ्क,
					 शून्य *out);
बाह्य errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *in);
बाह्य errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, माप_प्रकार अङ्क,
					 शून्य *in);
बाह्य errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *out);
बाह्य errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, माप_प्रकार अङ्क,
					 शून्य *out);

/* blknum.c */
बाह्य __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
बाह्य __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
बाह्य dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
बाह्य blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णाङ्क ext2fs_group_blocks_गन्ती(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					 संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					संरचना ext2_inode *inode);
बाह्य blk64_t ext2fs_blocks_गन्ती(संरचना ext2_super_block *super);
बाह्य शून्य ext2fs_blocks_गन्ती_set(संरचना ext2_super_block *super,
				    blk64_t blk);
बाह्य शून्य ext2fs_blocks_गन्ती_add(संरचना ext2_super_block *super,
				    blk64_t blk);
बाह्य blk64_t ext2fs_r_blocks_गन्ती(संरचना ext2_super_block *super);
बाह्य शून्य ext2fs_r_blocks_गन्ती_set(संरचना ext2_super_block *super,
				      blk64_t blk);
बाह्य शून्य ext2fs_r_blocks_गन्ती_add(संरचना ext2_super_block *super,
				      blk64_t blk);
बाह्य blk64_t ext2fs_free_blocks_गन्ती(संरचना ext2_super_block *super);
बाह्य शून्य ext2fs_free_blocks_गन्ती_set(संरचना ext2_super_block *super,
					 blk64_t blk);
बाह्य शून्य ext2fs_free_blocks_गन्ती_add(संरचना ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
बाह्य संरचना ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  संरचना opaque_ext2_group_desc *gdp,
					  dgrp_t group);
बाह्य blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
बाह्य __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
बाह्य blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
बाह्य __u32 ext2fs_bg_free_blocks_गन्ती(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_free_blocks_गन्ती_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
बाह्य __u32 ext2fs_bg_free_inodes_गन्ती(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_free_inodes_गन्ती_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
बाह्य __u32 ext2fs_bg_used_dirs_गन्ती(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_used_dirs_गन्ती_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
बाह्य __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
बाह्य __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णाङ्क ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
बाह्य शून्य ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
बाह्य शून्य ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
बाह्य __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
बाह्य शून्य ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
बाह्य blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     अचर संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_file_acl_block_set(ext2_filsys fs,
				      संरचना ext2_inode *inode, blk64_t blk);
बाह्य errcode_t ext2fs_inode_size_set(ext2_filsys fs, संरचना ext2_inode *inode,
				       ext2_off64_t size);

/* block.c */
बाह्य errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      पूर्णाङ्क	flags,
				      वर्ण *block_buf,
				      पूर्णाङ्क (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  पूर्णाङ्क	blockcnt,
						  शून्य	*priv_data),
				      शून्य *priv_data);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				पूर्णाङ्क	flags,
				वर्ण *block_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णाङ्क		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *block_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णाङ्क		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data);

/* bmap.c */
बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     वर्ण *block_buf, पूर्णाङ्क bmap_flags,
			     blk_t block, blk_t *phys_blk);
बाह्य errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode,
			      वर्ण *block_buf, पूर्णाङ्क bmap_flags, blk64_t block,
			      पूर्णाङ्क *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   संरचना ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#यदि 0
/* bmove.c */
बाह्य errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    पूर्णाङ्क flags);
#endif

/* check_desc.c */
बाह्य errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
बाह्य errcode_t ext2fs_close(ext2_filsys fs);
बाह्य errcode_t ext2fs_close2(ext2_filsys fs, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_close_free(ext2_filsys *fs);
बाह्य errcode_t ext2fs_flush(ext2_filsys fs);
बाह्य errcode_t ext2fs_flush2(ext2_filsys fs, पूर्णाङ्क flags);
बाह्य पूर्णाङ्क ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
बाह्य errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
बाह्य पूर्णाङ्क ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    पूर्णाङ्क *ret_meta_bg);
बाह्य शून्य ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
बाह्य __u32 ext2fs_crc32_be(__u32 crc, अचिन्हित वर्ण अचर *p, माप_प्रकार len);
बाह्य __u32 ext2fs_crc32c_le(__u32 crc, अचिन्हित वर्ण अचर *p, माप_प्रकार len);

/* csum.c */
बाह्य शून्य ext2fs_init_csum_seed(ext2_filsys fs);
बाह्य errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, संरचना mmp_struct *mmp);
बाह्य पूर्णाङ्क ext2fs_mmp_csum_verify(ext2_filsys, संरचना mmp_struct *mmp);
बाह्य पूर्णाङ्क ext2fs_verify_csum_type(ext2_filsys fs, संरचना ext2_super_block *sb);
बाह्य errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    संरचना ext2_super_block *sb);
बाह्य पूर्णाङ्क ext2fs_superblock_csum_verify(ext2_filsys fs,
					 संरचना ext2_super_block *sb);
बाह्य errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					संरचना ext2_ext_attr_header *hdr);
बाह्य पूर्णाङ्क ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     संरचना ext2_ext_attr_header *hdr);
#define EXT2_DIRENT_TAIL(block, blocksize) \
	((संरचना ext2_dir_entry_tail *)(((वर्ण *)(block)) + \
	(blocksize) - परिमाण(संरचना ext2_dir_entry_tail)))

बाह्य शून्य ext2fs_initialize_dirent_tail(ext2_filsys fs,
					  संरचना ext2_dir_entry_tail *t);
बाह्य पूर्णाङ्क ext2fs_dirent_has_tail(ext2_filsys fs,
				  संरचना ext2_dir_entry *dirent);
बाह्य पूर्णाङ्क ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     संरचना ext2_dir_entry *dirent);
बाह्य पूर्णाङ्क ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					संरचना ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   संरचना ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  संरचना ext2_dir_entry *dirent,
					  संरचना ext2_dx_countlimit **cc,
					  पूर्णाङ्क *offset);
बाह्य errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				संरचना ext2_dir_entry *dirent,
				__u32 *crc, संरचना ext2_dx_tail **ret_t);
बाह्य errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      संरचना ext3_extent_header *eh);
बाह्य पूर्णाङ्क ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   संरचना ext3_extent_header *eh);
बाह्य errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      वर्ण *bitmap, पूर्णाङ्क size);
बाह्य पूर्णाङ्क ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   वर्ण *bitmap, पूर्णाङ्क size);
बाह्य errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      वर्ण *bitmap, पूर्णाङ्क size);
बाह्य पूर्णाङ्क ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   वर्ण *bitmap, पूर्णाङ्क size);
बाह्य errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       संरचना ext2_inode_large *inode);
बाह्य पूर्णाङ्क ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_inode_large *inode);
बाह्य शून्य ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णाङ्क ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
बाह्य errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
बाह्य __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
बाह्य errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
बाह्य errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णाङ्क blockcnt);
बाह्य errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य शून्य ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(अचर शून्य *,
							   अचर शून्य *));
बाह्य शून्य ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(अचर शून्य *,
							    अचर शून्य *));
बाह्य errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_entry *db_info,
		    शून्य	*priv_data),
	शून्य *priv_data);
बाह्य errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		    शून्य	*priv_data),
	शून्य *priv_data);
बाह्य errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	पूर्णाङ्क (*func)(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		    शून्य	*priv_data),
	अचिन्हित दीर्घ दीर्घ start,
	अचिन्हित दीर्घ दीर्घ गन्ती,
	शून्य *priv_data);
बाह्य errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णाङ्क blockcnt);
बाह्य errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
बाह्य पूर्णाङ्क ext2fs_dblist_गन्ती(ext2_dblist dblist);
बाह्य blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
बाह्य errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					संरचना ext2_db_entry **entry);
बाह्य errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					संरचना ext2_db_entry2 **entry);
बाह्य errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
बाह्य errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  पूर्णाङ्क	flags,
				  वर्ण	*block_buf,
				  पूर्णाङ्क (*func)(ext2_ino_t	dir,
					      पूर्णाङ्क		entry,
					      संरचना ext2_dir_entry *dirent,
					      पूर्णाङ्क	offset,
					      पूर्णाङ्क	blocksize,
					      वर्ण	*buf,
					      शून्य	*priv_data),
				  शून्य *priv_data);

#यदि 0
/* digest_encode.c */
#define EXT2FS_DIGEST_SIZE EXT2FS_SHA256_LENGTH
बाह्य पूर्णाङ्क ext2fs_digest_encode(अचर वर्ण *src, पूर्णाङ्क len, वर्ण *dst);
बाह्य पूर्णाङ्क ext2fs_digest_decode(अचर वर्ण *src, पूर्णाङ्क len, वर्ण *dst);
#endif

/* dirblock.c */
बाह्य errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       शून्य *buf);
बाह्य errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					शून्य *buf, पूर्णाङ्क flags, ext2_ino_t ino);
बाह्य errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					शून्य *buf);
बाह्य errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 शून्य *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 शून्य *buf, पूर्णाङ्क flags, ext2_ino_t ino);

/* dirhash.c */
बाह्य errcode_t ext2fs_dirhash(पूर्णाङ्क version, अचर वर्ण *name, पूर्णाङ्क len,
				अचर __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

बाह्य errcode_t ext2fs_dirhash2(पूर्णाङ्क version, अचर वर्ण *name, पूर्णाङ्क len,
				 अचर संरचना ext2fs_nls_table *charset,
				 पूर्णाङ्क hash_flags,
				 अचर __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
बाह्य errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    संरचना ext2_dir_entry *dirent,
				    अचिन्हित पूर्णाङ्क *rec_len);
बाह्य errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    अचिन्हित पूर्णाङ्क len,
				    संरचना ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *block_buf,
			      पूर्णाङ्क (*func)(संरचना ext2_dir_entry *dirent,
					  पूर्णाङ्क	offset,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_data),
			      शून्य *priv_data);
बाह्य errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *block_buf,
			      पूर्णाङ्क (*func)(ext2_ino_t	dir,
					  पूर्णाङ्क	entry,
					  संरचना ext2_dir_entry *dirent,
					  पूर्णाङ्क	offset,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_data),
			      शून्य *priv_data);

/* dupfs.c */
बाह्य errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
बाह्य errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
बाह्य __u32 ext2fs_ext_attr_hash_entry(संरचना ext2_ext_attr_entry *entry,
					शून्य *data);
बाह्य __u32 ext2fs_ext_attr_hash_entry_signed(संरचना ext2_ext_attr_entry *entry,
					       शून्य *data);
बाह्य errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     संरचना ext2_ext_attr_entry *entry,
					     शून्य *data, __u32 *hash);
बाह्य errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     संरचना ext2_ext_attr_entry *entry,
					     शून्य *data, __u32 *hash,
					     __u32 *signed_hash);
बाह्य errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, शून्य *buf);
बाह्य errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       शून्य *buf);
बाह्य errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       शून्य *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       शून्य *buf);
बाह्य errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       शून्य *buf);
बाह्य errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       शून्य *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   वर्ण *block_buf,
					   पूर्णाङ्क adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   वर्ण *block_buf,
					   पूर्णाङ्क adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   वर्ण *block_buf,
					   पूर्णाङ्क adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णाङ्क (*func)(वर्ण *name, वर्ण *मान,
					    माप_प्रकार मान_len, शून्य *data),
				शून्य *data);
errcode_t ext2fs_xattr_get(संरचना ext2_xattr_handle *h, अचर वर्ण *साँचो,
			   शून्य **मान, माप_प्रकार *मान_len);
errcode_t ext2fs_xattr_set(संरचना ext2_xattr_handle *handle,
			   अचर वर्ण *साँचो,
			   अचर शून्य *मान,
			   माप_प्रकार मान_len);
errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      अचर वर्ण *साँचो);
errcode_t ext2fs_xattr_remove_all(संरचना ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(संरचना ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode_large *inode);
errcode_t ext2fs_xattrs_गन्ती(संरचना ext2_xattr_handle *handle, माप_प्रकार *गन्ती);
errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      माप_प्रकार *size);
#define XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(संरचना ext2_xattr_handle *handle,
			      अचिन्हित पूर्णाङ्क *new_flags, अचिन्हित पूर्णाङ्क *old_flags);
बाह्य शून्य ext2fs_ext_attr_block_rehash(संरचना ext2_ext_attr_header *header,
					 संरचना ext2_ext_attr_entry *end);
बाह्य __u32 ext2fs_get_ea_inode_hash(संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_set_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash);
बाह्य __u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode);
बाह्य शून्य ext2fs_set_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_गन्ती);

/* extent.c */
बाह्य errcode_t ext2fs_extent_header_verify(शून्य *ptr, पूर्णाङ्क size);
बाह्य errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
बाह्य errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
बाह्य शून्य ext2fs_extent_free(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   पूर्णाङ्क flags, संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_गाँठो_split(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				       संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_घुसाउ(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				      संरचना ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					संरचना ext2_extent_info *info);
बाह्य errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
बाह्य errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     पूर्णाङ्क leaf_level, blk64_t blk);
बाह्य errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
बाह्य माप_प्रकार ext2fs_max_extent_depth(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      संरचना ext2_inode *inode);
बाह्य errcode_t ext2fs_गन्ती_blocks(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode, blk64_t *ret_गन्ती);
बाह्य errcode_t ext2fs_decode_extent(संरचना ext2fs_extent *to, शून्य *from,
				      पूर्णाङ्क len);

/* fallocate.c */
#define EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#define EXT2_FALLOCATE_FORCE_INIT	(0x2)
#define EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#define EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#define EXT2_FALLOCATE_ALL_FLAGS	(0xF)
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
बाह्य ext2_ino_t ext2fs_file_get_inode_अङ्क(ext2_file_t file);
बाह्य errcode_t ext2fs_file_close(ext2_file_t file);
बाह्य errcode_t ext2fs_file_flush(ext2_file_t file);
बाह्य errcode_t ext2fs_file_read(ext2_file_t file, शून्य *buf,
				  अचिन्हित पूर्णाङ्क wanted, अचिन्हित पूर्णाङ्क *got);
बाह्य errcode_t ext2fs_file_write(ext2_file_t file, अचर शून्य *buf,
				   अचिन्हित पूर्णाङ्क nbytes, अचिन्हित पूर्णाङ्क *written);
बाह्य errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   पूर्णाङ्क whence, __u64 *ret_pos);
बाह्य errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   पूर्णाङ्क whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size);
बाह्य ext2_off_t ext2fs_file_get_size(ext2_file_t file);
बाह्य errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size);
बाह्य errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size);

/* finddev.c */
बाह्य वर्ण *ext2fs_find_block_device(dev_t device);

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
					    __u32 start, __u32 end,
					    __u32 real_end,
					    अचर वर्ण *descr, वर्ण *init_map,
					    ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						अचर वर्ण *descr,
						ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
बाह्य शून्य ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
बाह्य errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
बाह्य शून्य ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
बाह्य errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
बाह्य errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
बाह्य errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अङ्क,
						 शून्य *out);
बाह्य errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अङ्क,
						 शून्य *in);
बाह्य errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
बाह्य errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णाङ्क type, __u64 start, __u64 end,
				    __u64 real_end,
				    अचर वर्ण *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
शून्य ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
शून्य ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_गन्ती_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_गन्ती_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
बाह्य अचिन्हित पूर्णाङ्क ext2fs_list_backups(ext2_filsys fs, अचिन्हित पूर्णाङ्क *three,
				अचिन्हित पूर्णाङ्क *five, अचिन्हित पूर्णाङ्क *seven);

/* getenv.c */
बाह्य वर्ण *ext2fs_safe_getenv(अचर वर्ण *arg);

/* get_num_dirs.c */
बाह्य errcode_t ext2fs_get_अङ्क_dirs(ext2_filsys fs, ext2_ino_t *ret_अङ्क_dirs);

/* getsize.c */
बाह्य errcode_t ext2fs_get_device_size(अचर वर्ण *file, पूर्णाङ्क blocksize,
					blk_t *retblocks);
बाह्य errcode_t ext2fs_get_device_size2(अचर वर्ण *file, पूर्णाङ्क blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
बाह्य पूर्णाङ्क ext2fs_get_dio_alignment(पूर्णाङ्क fd);
errcode_t ext2fs_get_device_sectsize(अचर वर्ण *file, पूर्णाङ्क *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(अचर वर्ण *file, पूर्णाङ्क *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्क_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्क_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b);

/* imager.c */
बाह्य errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, शून्य *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, शून्य *buf);

/* initialize.c */
बाह्य errcode_t ext2fs_initialize(अचर वर्ण *name, पूर्णाङ्क flags,
				   संरचना ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, पूर्णाङ्क super_only);

/* icount.c */
बाह्य शून्य ext2fs_free_icount(ext2_icount_t icount);
बाह्य errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, वर्ण *tdb_dir,
					  पूर्णाङ्क flags, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णाङ्क flags,
				       अचिन्हित पूर्णाङ्क size,
				       ext2_icount_t hint, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णाङ्क flags,
				      अचिन्हित पूर्णाङ्क size,
				      ext2_icount_t *ret);
बाह्य errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
बाह्य errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 गन्ती);
बाह्य ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, खाता *);

/* inline.c */

बाह्य errcode_t ext2fs_get_memalign(अचिन्हित दीर्घ size,
				     अचिन्हित दीर्घ align, शून्य *ptr);

/* inline_data.c */
बाह्य errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino,
					 माप_प्रकार *size);
बाह्य errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					शून्य *buf, माप_प्रकार *size);
बाह्य errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode *inode,
					शून्य *buf, माप_प्रकार size);

/* inode.c */
बाह्य errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   अचिन्हित पूर्णाङ्क cache_size);
बाह्य शून्य ext2fs_free_inode_cache(संरचना ext2_inode_cache *icache);
बाह्य errcode_t ext2fs_flush_icache(ext2_filsys fs);
बाह्य errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    संरचना ext2_inode *inode,
					    पूर्णाङ्क bufsize);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8
बाह्य errcode_t ext2fs_open_inode_scan(ext2_filsys fs, पूर्णाङ्क buffer_blocks,
				  ext2_inode_scan *ret_scan);
बाह्य शून्य ext2fs_close_inode_scan(ext2_inode_scan scan);
बाह्य errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       संरचना ext2_inode *inode);
बाह्य errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   पूर्णाङ्क	group);
बाह्य शून्य ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 शून्य * priv_data),
	 शून्य *done_group_data);
बाह्य पूर्णाङ्क ext2fs_inode_scan_flags(ext2_inode_scan scan, पूर्णाङ्क set_flags,
				   पूर्णाङ्क clear_flags);
बाह्य errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					संरचना ext2_inode * inode,
					पूर्णाङ्क bufsize);
बाह्य errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    संरचना ext2_inode * inode,
				    पूर्णाङ्क bufsize, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode * inode,
					 पूर्णाङ्क bufsize);
बाह्य errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode * inode,
				     पूर्णाङ्क bufsize, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode);
बाह्य errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
बाह्य errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
बाह्य io_manager inode_io_manager;
बाह्य errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					वर्ण **name);
बाह्य errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode *inode,
					 वर्ण **name);

/* ismounted.c */
बाह्य errcode_t ext2fs_check_if_mounted(अचर वर्ण *file, पूर्णाङ्क *mount_flags);
बाह्य errcode_t ext2fs_check_mount_point(अचर वर्ण *device, पूर्णाङ्क *mount_flags,
					  वर्ण *mtpt, पूर्णाङ्क mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
बाह्य errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode,
			      वर्ण *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
बाह्य errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, अचर वर्ण *name,
			 पूर्णाङ्क namelen, वर्ण *buf, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
			अचर वर्ण *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
			      अचर वर्ण *name, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
पूर्णाङ्क ext2fs_native_flag(शून्य);

/* newdir.c */
बाह्य errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, वर्ण **block);
बाह्य errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
बाह्य अचर संरचना ext2fs_nls_table *ext2fs_load_nls_table(पूर्णाङ्क encoding);
बाह्य पूर्णाङ्क ext2fs_check_encoded_name(अचर संरचना ext2fs_nls_table *table,
				     वर्ण *s, माप_प्रकार len, वर्ण **pos);
बाह्य पूर्णाङ्क ext2fs_casefold_cmp(अचर संरचना ext2fs_nls_table *table,
			       अचर अचिन्हित वर्ण *str1, माप_प्रकार len1,
			       अचर अचिन्हित वर्ण *str2, माप_प्रकार len2);

/* mkdir.c */
बाह्य errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      अचर वर्ण *name);
बाह्य errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, अचिन्हित दीर्घ flags,
			       पूर्णाङ्क link_flags, अचर वर्ण *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
संरचना ext2fs_journal_params {
	blk_t अङ्क_journal_blocks;
	blk_t अङ्क_fc_blocks;
};
बाह्य errcode_t ext2fs_get_journal_params(
		संरचना ext2fs_journal_params *params, ext2_filsys fs);
बाह्य errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, पूर्णाङ्क अङ्क,
				    blk_t *ret_blk, पूर्णाङ्क *ret_गन्ती);
बाह्य errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क अङ्क,
				     blk64_t *ret_blk, पूर्णाङ्क *ret_गन्ती);
बाह्य errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 अङ्क_blocks, पूर्णाङ्क flags,
						  वर्ण  **ret_jsb);
बाह्य errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  संरचना ext2fs_journal_params *params,
						  पूर्णाङ्क flags, वर्ण  **ret_jsb);
बाह्य errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
बाह्य errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t अङ्क_blocks,
					  पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t अङ्क_blocks,
					   blk64_t goal, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    संरचना ext2fs_journal_params *params,
				    blk64_t goal, पूर्णाङ्क flags);
बाह्य पूर्णाङ्क ext2fs_default_journal_size(__u64 अङ्क_blocks);
बाह्य पूर्णाङ्क ext2fs_journal_sb_start(पूर्णाङ्क blocksize);

/* openfs.c */
बाह्य errcode_t ext2fs_open(अचर वर्ण *name, पूर्णाङ्क flags, पूर्णाङ्क superblock,
			     अचिन्हित पूर्णाङ्क block_size, io_manager manager,
			     ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_open2(अचर वर्ण *name, अचर वर्ण *io_options,
			      पूर्णाङ्क flags, पूर्णाङ्क superblock,
			      अचिन्हित पूर्णाङ्क block_size, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
बाह्य blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs,
					blk64_t group_block, dgrp_t i);
बाह्य blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block,
					 dgrp_t i);
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
बाह्य errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अङ्क_blocks);
बाह्य errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
बाह्य e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
बाह्य __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      वर्ण *buf);
बाह्य errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   वर्ण *buf);
बाह्य पूर्णाङ्क ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, वर्ण *buf);

/* get_pathname.c */
बाह्य errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       वर्ण **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, अचर वर्ण *name,
		      ext2_ino_t ino, पूर्णाङ्क flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, अचर वर्ण *name,
			ext2_ino_t ino, पूर्णाङ्क flags);
पूर्णाङ्क ext2fs_dir_is_dx(ext2_filsys fs, अचर संरचना ext2_inode *inode);
शून्य ext2fs_inc_nlink(ext2_filsys fs, संरचना ext2_inode *inode);
शून्य ext2fs_dec_nlink(संरचना ext2_inode *inode);
पूर्णाङ्क ext2fs_dir_link_max(ext2_filsys fs, संरचना ext2_inode_large *inode);
पूर्णाङ्क ext2fs_dir_link_empty(संरचना ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 अचर वर्ण *name, अचर वर्ण *target);
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
अचिन्हित ext2fs_mmp_new_seq(शून्य);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, शून्य **ptr);

/* read_bb.c */
बाह्य errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
बाह्य errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, खाता *f,
				      ext2_badblocks_list *bb_list,
				      शून्य *priv_data,
				      शून्य (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      वर्ण *badstr,
						      शून्य *priv_data));
बाह्य errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, खाता *f,
				     ext2_badblocks_list *bb_list,
				     शून्य (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
बाह्य errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
बाह्य errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णाङ्क flags, पूर्णाङ्क अङ्क_threads);
बाह्य errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
बाह्य errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#define EXT2FS_SHA256_LENGTH 32
#यदि 0
बाह्य शून्य ext2fs_sha256(अचर अचिन्हित वर्ण *in, अचिन्हित दीर्घ in_size,
		   अचिन्हित वर्ण out[EXT2FS_SHA256_LENGTH]);
#endif

/* sha512.c */
#define EXT2FS_SHA512_LENGTH 64
बाह्य शून्य ext2fs_sha512(अचर अचिन्हित वर्ण *in, अचिन्हित दीर्घ in_size,
			  अचिन्हित वर्ण out[EXT2FS_SHA512_LENGTH]);

/* swapfs.c */
बाह्य errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, वर्ण *buf, माप_प्रकार size,
					पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, वर्ण *buf, माप_प्रकार size,
					 पूर्णाङ्क flags);
बाह्य errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags);
बाह्य शून्य ext2fs_swap_ext_attr(वर्ण *to, वर्ण *from, पूर्णाङ्क bufsize,
				 पूर्णाङ्क has_header);
बाह्य शून्य ext2fs_swap_ext_attr_header(संरचना ext2_ext_attr_header *to_header,
					संरचना ext2_ext_attr_header *from_hdr);
बाह्य शून्य ext2fs_swap_ext_attr_entry(संरचना ext2_ext_attr_entry *to_entry,
				       संरचना ext2_ext_attr_entry *from_entry);
बाह्य शून्य ext2fs_swap_super(संरचना ext2_super_block * super);
बाह्य शून्य ext2fs_swap_group_desc(संरचना ext2_group_desc *gdp);
बाह्य शून्य ext2fs_swap_group_desc2(ext2_filsys, संरचना ext2_group_desc *gdp);
बाह्य शून्य ext2fs_swap_inode_full(ext2_filsys fs, संरचना ext2_inode_large *t,
				   संरचना ext2_inode_large *f, पूर्णाङ्क hostorder,
				   पूर्णाङ्क bufsize);
बाह्य शून्य ext2fs_swap_inode(ext2_filsys fs,संरचना ext2_inode *t,
			      संरचना ext2_inode *f, पूर्णाङ्क hostorder);
बाह्य शून्य ext2fs_swap_mmp(संरचना mmp_struct *mmp);

/* unix_io.c */
बाह्य पूर्णाङ्क ext2fs_open_file(अचर वर्ण *pathname, पूर्णाङ्क flags, mode_t mode);
बाह्य पूर्णाङ्क ext2fs_stat(अचर वर्ण *path, ext2fs_struct_stat *buf);
बाह्य पूर्णाङ्क ext2fs_fstat(पूर्णाङ्क fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
बाह्य पूर्णाङ्क ext2fs_inode_has_valid_blocks(संरचना ext2_inode *inode);
बाह्य पूर्णाङ्क ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  संरचना ext2_inode *inode);

/* version.c */
बाह्य पूर्णाङ्क ext2fs_parse_version_string(अचर वर्ण *ver_string);
बाह्य पूर्णाङ्क ext2fs_get_library_version(अचर वर्ण **ver_string,
				      अचर वर्ण **date_string);

/* write_bb_file.c */
बाह्य errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      अचिन्हित पूर्णाङ्क flags,
				      खाता *f);


/* inline functions */
#ifdef NO_INLINE_FUNCS
बाह्य errcode_t ext2fs_get_mem(अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य errcode_t ext2fs_get_memzero(अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य errcode_t ext2fs_get_array(अचिन्हित दीर्घ गन्ती,
				  अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य errcode_t ext2fs_get_arrayzero(अचिन्हित दीर्घ गन्ती,
				      अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य errcode_t ext2fs_free_mem(शून्य *ptr);
बाह्य errcode_t ext2fs_resize_mem(अचिन्हित दीर्घ old_size,
				   अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य errcode_t ext2fs_resize_array(अचिन्हित दीर्घ old_गन्ती, अचिन्हित दीर्घ गन्ती,
				     अचिन्हित दीर्घ size, शून्य *ptr);
बाह्य शून्य ext2fs_mark_super_dirty(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_changed(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_changed(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_valid(ext2_filsys fs);
बाह्य शून्य ext2fs_unmark_valid(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_valid(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_ib_dirty(ext2_filsys fs);
बाह्य शून्य ext2fs_mark_bb_dirty(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_ib_dirty(ext2_filsys fs);
बाह्य पूर्णाङ्क ext2fs_test_bb_dirty(ext2_filsys fs);
बाह्य dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
बाह्य dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
बाह्य blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
बाह्य blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
बाह्य blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
				      संरचना ext2_inode *inode);
बाह्य पूर्णाङ्क ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णाङ्क blocks);
बाह्य पूर्णाङ्क ext2fs_log2_u32(__u32 arg);
बाह्य पूर्णाङ्क ext2fs_log2_u64(__u64 arg);
बाह्य पूर्णाङ्क ext2fs_log10_u32(__u32 arg);
बाह्य पूर्णाङ्क ext2fs_log10_u64(__u64 arg);
बाह्य अचिन्हित पूर्णाङ्क ext2fs_div_ceil(अचिन्हित पूर्णाङ्क a, अचिन्हित पूर्णाङ्क b);
बाह्य __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
बाह्य पूर्णाङ्क ext2fs_dirent_name_len(अचर संरचना ext2_dir_entry *entry);
बाह्य शून्य ext2fs_dirent_set_name_len(संरचना ext2_dir_entry *entry, पूर्णाङ्क len);
बाह्य पूर्णाङ्क ext2fs_dirent_file_type(अचर संरचना ext2_dir_entry *entry);
बाह्य शून्य ext2fs_dirent_set_file_type(संरचना ext2_dir_entry *entry, पूर्णाङ्क type);
बाह्य संरचना ext2_inode *ext2fs_inode(संरचना ext2_inode_large * large_inode);
बाह्य अचर संरचना ext2_inode *ext2fs_const_inode(अचर संरचना ext2_inode_large * large_inode);
बाह्य पूर्णाङ्क ext2fs_inodes_per_orphan_block(ext2_filsys fs);
बाह्य संरचना ext4_orphan_block_tail *ext2fs_orphan_block_tail(ext2_filsys fs,
							       वर्ण *buf);
#endif

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#यदि (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ बाह्य
#अन्यथा
#यदि (__STDC_VERSION__ >= 199901L)
#define _INLINE_ अन्तःस्थ
#अन्यथा
#ifdef __GNUC__
#define _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#define _INLINE_ बाह्य अन्तःस्थ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif

#ifndef EXT2_CUSTOM_MEMORY_ROUTINES
#अन्तर्भाव <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(अचिन्हित दीर्घ size, शून्य *ptr)
{
	शून्य *pp;

	pp = दो_स्मृति(size);
	यदि (!pp)
		फर्काऊ EXT2_ET_NO_MEMORY;
	स_नकल(ptr, &pp, परिमाण (pp));
	फर्काऊ 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(अचिन्हित दीर्घ size, शून्य *ptr)
{
	शून्य *pp;

	pp = दो_स्मृति(size);
	यदि (!pp)
		फर्काऊ EXT2_ET_NO_MEMORY;
	स_रखो(pp, 0, size);
	स_नकल(ptr, &pp, परिमाण(pp));
	फर्काऊ 0;
}

_INLINE_ errcode_t ext2fs_get_array(अचिन्हित दीर्घ गन्ती, अचिन्हित दीर्घ size,
				    शून्य *ptr)
{
	यदि (गन्ती && (~0UL)/गन्ती < size)
		फर्काऊ EXT2_ET_NO_MEMORY;
	फर्काऊ ext2fs_get_mem(गन्ती*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(अचिन्हित दीर्घ गन्ती,
					अचिन्हित दीर्घ size, शून्य *ptr)
{
	यदि (गन्ती && (~0UL)/गन्ती < size)
		फर्काऊ EXT2_ET_NO_MEMORY;

	फर्काऊ ext2fs_get_memzero((माप_प्रकार)गन्ती * size, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(शून्य *ptr)
{
	शून्य *p;

	स_नकल(&p, ptr, परिमाण(p));
	मुक्त(p);
	p = 0;
	स_नकल(ptr, &p, परिमाण(p));
	फर्काऊ 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(अचिन्हित दीर्घ EXT2FS_ATTR((unused)) old_size,
				     अचिन्हित दीर्घ size, शून्य *ptr)
{
	शून्य *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	स_नकल(&p, ptr, परिमाण(p));
	p = पुनः_स्मृति(p, size);
	यदि (!p)
		फर्काऊ EXT2_ET_NO_MEMORY;
	स_नकल(ptr, &p, परिमाण(p));
	फर्काऊ 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(अचिन्हित दीर्घ size,
				       अचिन्हित दीर्घ old_गन्ती,
				       अचिन्हित दीर्घ गन्ती, शून्य *ptr)
{
	अचिन्हित दीर्घ old_size;
	errcode_t retval;

	यदि (गन्ती && (~0UL)/गन्ती < size)
		फर्काऊ EXT2_ET_NO_MEMORY;

	size *= गन्ती;
	old_size = size * old_गन्ती;
	retval = ext2fs_resize_mem(old_size, size, ptr);
	यदि (retval)
		फर्काऊ retval;

	यदि (size > old_size) {
		शून्य *p;

		स_नकल(&p, ptr, परिमाण(p));
		स_रखो((वर्ण *)p + old_size, 0, size - old_size);
		स_नकल(ptr, &p, परिमाण(p));
	}

	फर्काऊ 0;
}
#endif	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ शून्य ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ शून्य ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ पूर्णाङ्क ext2fs_test_changed(ext2_filsys fs)
{
	फर्काऊ (fs->flags & EXT2_FLAG_CHANGED);
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
	फर्काऊ (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ शून्य ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ शून्य ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ पूर्णाङ्क ext2fs_test_ib_dirty(ext2_filsys fs)
{
	फर्काऊ (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ पूर्णाङ्क ext2fs_test_bb_dirty(ext2_filsys fs)
{
	फर्काऊ (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	फर्काऊ ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	फर्काऊ (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	फर्काऊ (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	फर्काऊ (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	फर्काऊ (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

_INLINE_ पूर्णाङ्क ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णाङ्क blocks)
{
	पूर्णाङ्क csum_size = 0;

	यदि ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_size = परिमाण(संरचना ext2_dx_tail);
	फर्काऊ blocks * ((fs->blocksize - (8 + csum_size)) /
						परिमाण(संरचना ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log2_u32(__u32 arg)
{
	पूर्णाङ्क l = 0;

	arg >>= 1;
	जबसम्म (arg) {
		l++;
		arg >>= 1;
	}
	फर्काऊ l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log2_u64(__u64 arg)
{
	पूर्णाङ्क l = 0;

	arg >>= 1;
	जबसम्म (arg) {
		l++;
		arg >>= 1;
	}
	फर्काऊ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log10_u32(__u32 arg)
{
	पूर्णाङ्क l = 0;

	arg /= 10;
	जबसम्म (arg) {
		l++;
		arg /= 10;
	}
	फर्काऊ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णाङ्क ext2fs_log10_u64(__u64 arg)
{
	पूर्णाङ्क l = 0;

	arg /= 10;
	जबसम्म (arg) {
		l++;
		arg /= 10;
	}
	फर्काऊ l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ अचिन्हित पूर्णाङ्क ext2fs_div_ceil(अचिन्हित पूर्णाङ्क a, अचिन्हित पूर्णाङ्क b)
{
	यदि (!a)
		फर्काऊ 0;
	फर्काऊ ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	यदि (!a)
		फर्काऊ 0;
	फर्काऊ ((a - 1) / b) + 1;
}

_INLINE_ पूर्णाङ्क ext2fs_dirent_name_len(अचर संरचना ext2_dir_entry *entry)
{
	फर्काऊ entry->name_len & 0xff;
}

_INLINE_ शून्य ext2fs_dirent_set_name_len(संरचना ext2_dir_entry *entry, पूर्णाङ्क len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ पूर्णाङ्क ext2fs_dirent_file_type(अचर संरचना ext2_dir_entry *entry)
{
	फर्काऊ entry->name_len >> 8;
}

_INLINE_ शून्य ext2fs_dirent_set_file_type(संरचना ext2_dir_entry *entry, पूर्णाङ्क type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ संरचना ext2_inode *ext2fs_inode(संरचना ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	फर्काऊ (संरचना ext2_inode *) large_inode;
}

_INLINE_ अचर संरचना ext2_inode *
ext2fs_const_inode(अचर संरचना ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	फर्काऊ (अचर संरचना ext2_inode *) large_inode;
}

_INLINE_ पूर्णाङ्क ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	फर्काऊ (fs->blocksize - परिमाण(संरचना ext4_orphan_block_tail)) /
		परिमाण(__u32);
}

_INLINE_ संरचना ext4_orphan_block_tail *
ext2fs_orphan_block_tail(ext2_filsys fs, वर्ण *buf)
{
	फर्काऊ (संरचना ext4_orphan_block_tail *)(buf + fs->blocksize -
		परिमाण(संरचना ext4_orphan_block_tail));
}

#undef _INLINE_
#endif

/* htree levels for ext4 */
#define EXT4_HTREE_LEVEL_COMPAT 2
#define EXT4_HTREE_LEVEL	3

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क ext2_dir_htree_level(ext2_filsys fs)
{
	यदि (ext2fs_has_feature_largedir(fs->super))
		फर्काऊ EXT4_HTREE_LEVEL;

	फर्काऊ EXT4_HTREE_LEVEL_COMPAT;
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
स्थिर अन्तःस्थ शून्य ext2fs_set_dtime(ext2_filsys fs, संरचना ext2_inode *inode)
{
	__u32	t;

	यदि (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	अन्यथा
		t = समय(रिक्त) & 0xFFFFFFFF;
	यदि (t < fs->super->s_inodes_गन्ती)
		t = fs->super->s_inodes_गन्ती;
	inode->i_dtime = t;
}

#ifdef __cplusplus
}
#endif

#endif /* _EXT2FS_EXT2FS_H */
