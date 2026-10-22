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

#अगर_अघोषित _EXT2FS_EXT2FS_H
#घोषणा _EXT2FS_EXT2FS_H

#अगर_घोषित __GNUC__
#घोषणा EXT2FS_ATTR(x) __attribute__(x)
#अन्यथा
#घोषणा EXT2FS_ATTR(x)
#पूर्ण_अगर

#अगर_अघोषित __nonstring
#अगर_घोषित __has_attribute
#अगर __has_attribute(__nonstring__)
#घोषणा __nonstring                    __attribute__((__nonstring__))
#अन्यथा
#घोषणा __nonstring
#पूर्ण_अगर /* __has_attribute(__nonstring__) */
#अन्यथा
# define __nonstring
#पूर्ण_अगर /* __has_attribute */
#पूर्ण_अगर /* __nonstring */

#अगर_घोषित CONFIG_TDB
#घोषणा EXT2FS_NO_TDB_UNUSED
#अन्यथा
#घोषणा EXT2FS_NO_TDB_UNUSED	EXT2FS_ATTR((unused))
#पूर्ण_अगर

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#अगर (!defined(__GNUC__) && !defined(__WATCOMC__))
#घोषणा NO_INLINE_FUNCS
#पूर्ण_अगर

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#घोषणा SUPERBLOCK_OFFSET	1024
#घोषणा SUPERBLOCK_आकार		1024

#घोषणा UUID_STR_आकार 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#घोषणा EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#अगर_घोषित HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <time.h>
#समावेश <errno.h>

#अगर EXT2_FLAT_INCLUDES
#समावेश "e2_types.h"
#समावेश "ext2_fs.h"
#समावेश "ext3_extents.h"
#अन्यथा
#समावेश <ext2fs/ext2_types.h>
#समावेश <ext2fs/ext2_fs.h>
#समावेश <ext2fs/ext3_extents.h>
#पूर्ण_अगर /* EXT2_FLAT_INCLUDES */

प्रकार __u32 __bitwise		ext2_ino_t;
प्रकार __u32 __bitwise		blk_t;
प्रकार __u64 __bitwise		blk64_t;
प्रकार __u32 __bitwise		dgrp_t;
प्रकार __s32 __bitwise		ext2_off_t;
प्रकार __s64 __bitwise		ext2_off64_t;
प्रकार __s64 __bitwise		e2_blkcnt_t;
प्रकार __u32 __bitwise		ext2_dirhash_t;

#अगर EXT2_FLAT_INCLUDES
#समावेश "com_err.h"
#समावेश "ext2_io.h"
#समावेश "ext2_err.h"
#समावेश "ext2_ext_attr.h"
#अन्यथा
#समावेश <et/com_err.h>
#समावेश <ext2fs/ext2_io.h>
#समावेश <ext2fs/ext2_err.h>
#समावेश <ext2fs/ext2_ext_attr.h>
#पूर्ण_अगर

#समावेश "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#अगर_घोषित _MSC_VER
#घोषणा EXT2_QSORT_TYPE पूर्णांक __cdecl
#अन्यथा
#घोषणा EXT2_QSORT_TYPE पूर्णांक
#पूर्ण_अगर

प्रकार काष्ठा struct_ext2_filsys *ext2_filsys;

#घोषणा EXT2FS_MARK_ERROR 	0
#घोषणा EXT2FS_UNMARK_ERROR 	1
#घोषणा EXT2FS_TEST_ERROR	2

काष्ठा ext2fs_struct_generic_bitmap_base {
	errcode_t		magic;
	ext2_filsys 		fs;
};

प्रकार काष्ठा ext2fs_struct_generic_bitmap_base *ext2fs_generic_bitmap;
प्रकार काष्ठा ext2fs_struct_generic_bitmap_base *ext2fs_inode_bitmap;
प्रकार काष्ठा ext2fs_struct_generic_bitmap_base *ext2fs_block_bitmap;

#घोषणा EXT2_FIRST_INODE(s)	EXT2_FIRST_INO(s)


/*
 * Badblocks list definitions
 */

प्रकार काष्ठा ext2_struct_u32_list *ext2_badblocks_list;
प्रकार काष्ठा ext2_struct_u32_iterate *ext2_badblocks_iterate;

प्रकार काष्ठा ext2_struct_u32_list *ext2_u32_list;
प्रकार काष्ठा ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
प्रकार काष्ठा ext2_struct_u32_list *badblocks_list;
प्रकार काष्ठा ext2_struct_u32_iterate *badblocks_iterate;

#घोषणा BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
काष्ठा ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
काष्ठा ext2_db_entry {
	ext2_ino_t	ino;
	blk_t	blk;
	पूर्णांक	blockcnt;
};

प्रकार काष्ठा ext2_struct_dblist *ext2_dblist;

#घोषणा DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#घोषणा EXT2_FILE_WRITE		0x0001
#घोषणा EXT2_FILE_CREATE	0x0002

#घोषणा EXT2_FILE_MASK		0x00FF

#घोषणा EXT2_FILE_BUF_DIRTY	0x4000
#घोषणा EXT2_FILE_BUF_VALID	0x2000

प्रकार काष्ठा ext2_file *ext2_file_t;

#घोषणा EXT2_SEEK_SET	0
#घोषणा EXT2_SEEK_CUR	1
#घोषणा EXT2_SEEK_END	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#घोषणा EXT2_FLAG_RW			0x01
#घोषणा EXT2_FLAG_CHANGED		0x02
#घोषणा EXT2_FLAG_DIRTY			0x04
#घोषणा EXT2_FLAG_VALID			0x08
#घोषणा EXT2_FLAG_IB_DIRTY		0x10
#घोषणा EXT2_FLAG_BB_DIRTY		0x20
#घोषणा EXT2_FLAG_SWAP_BYTES		0x40
#घोषणा EXT2_FLAG_SWAP_BYTES_READ	0x80
#घोषणा EXT2_FLAG_SWAP_BYTES_WRITE	0x100
#घोषणा EXT2_FLAG_MASTER_SB_ONLY	0x200
#घोषणा EXT2_FLAG_FORCE			0x400
#घोषणा EXT2_FLAG_SUPER_ONLY		0x800
#घोषणा EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#घोषणा EXT2_FLAG_IMAGE_FILE		0x2000
#घोषणा EXT2_FLAG_EXCLUSIVE		0x4000
#घोषणा EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#घोषणा EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#घोषणा EXT2_FLAG_64BITS		0x20000
#घोषणा EXT2_FLAG_PRINT_PROGRESS	0x40000
#घोषणा EXT2_FLAG_DIRECT_IO		0x80000
#घोषणा EXT2_FLAG_SKIP_MMP		0x100000
#घोषणा EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#घोषणा EXT2_FLAG_SHARE_DUP		0x400000
#घोषणा EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#घोषणा EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM	0x1000000
#घोषणा EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM	0x2000000
#घोषणा EXT2_FLAG_THREADS		0x4000000
#घोषणा EXT2_FLAG_IGNORE_SWAP_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#घोषणा EXT2_FLAG2_USE_FAKE_TIME	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#घोषणा EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#घोषणा EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#घोषणा EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#घोषणा EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#घोषणा EXT2_JOURNAL_TO_FC_BLKS_RATIO		64

काष्ठा blk_alloc_ctx;
काष्ठा opaque_ext2_group_desc;

काष्ठा struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	पूर्णांक				flags;
	अक्षर *				device_name;
	काष्ठा ext2_super_block	* 	super;
	अचिन्हित पूर्णांक			blocksize;
	पूर्णांक				fragsize;
	dgrp_t				group_desc_गिनती;
	अचिन्हित दीर्घ			desc_blocks;
	काष्ठा opaque_ext2_group_desc *	group_desc;
	अचिन्हित पूर्णांक			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				काष्ठा ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				काष्ठा ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	काष्ठा ext2_super_block *	orig_super;
	काष्ठा ext2_image_hdr *		image_header;
	__u32				umask;
	समय_प्रकार				now;
	पूर्णांक				cluster_ratio_bits;
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
	व्योम *				priv_तथ्य;

	/*
	 * Inode cache
	 */
	काष्ठा ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, काष्ठा blk_alloc_ctx *ctx);
	व्योम (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, पूर्णांक inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	व्योम *mmp_buf;
	व्योम *mmp_cmp;
	पूर्णांक mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	दीर्घ mmp_last_written;

	/* progress operation functions */
	काष्ठा ext2fs_progress_ops *progress_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	अक्षर				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, पूर्णांक flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	व्योम (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t अंक,
					पूर्णांक inuse);

	/* hashmap for SHA of data blocks */
	काष्ठा ext2fs_hashmap* block_sha_map;

	स्थिर काष्ठा ext2fs_nls_table *encoding;
};

#अगर EXT2_FLAT_INCLUDES
#समावेश "e2_bitops.h"
#अन्यथा
#समावेश <ext2fs/bitops.h>
#पूर्ण_अगर

/*
 * 64-bit bitmap backend types
 */
#घोषणा EXT2FS_BMAP64_BITARRAY	1
#घोषणा EXT2FS_BMAP64_RBTREE	2
#घोषणा EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#घोषणा BLOCK_CHANGED			1
#घोषणा BLOCK_ABORT			2
#घोषणा BLOCK_ERROR			4
#घोषणा BLOCK_INLINE_तथ्य_CHANGED	8

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
#घोषणा BLOCK_FLAG_APPEND	1
#घोषणा BLOCK_FLAG_HOLE		1
#घोषणा BLOCK_FLAG_DEPTH_TRAVERSE	2
#घोषणा BLOCK_FLAG_तथ्य_ONLY	4
#घोषणा BLOCK_FLAG_READ_ONLY	8

#घोषणा BLOCK_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#घोषणा BLOCK_गिनती_IND		(-1)
#घोषणा BLOCK_गिनती_DIND	(-2)
#घोषणा BLOCK_गिनती_TIND	(-3)
#घोषणा BLOCK_गिनती_TRANSLATOR	(-4)

#घोषणा BLOCK_ALLOC_UNKNOWN	0
#घोषणा BLOCK_ALLOC_तथ्य	1
#घोषणा BLOCK_ALLOC_METADATA	2

काष्ठा blk_alloc_ctx {
	ext2_ino_t		ino;
	काष्ठा ext2_inode	*inode;
	blk64_t			lblk;
	पूर्णांक			flags;
};

#अगर 0
/*
 * Flags for ext2fs_move_blocks
 */
#घोषणा EXT2_BMOVE_GET_DBLIST	0x0001
#घोषणा EXT2_BMOVE_DEBUG	0x0002
#पूर्ण_अगर

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#घोषणा EXT2_EXTENT_FLAGS_LEAF		0x0001
#घोषणा EXT2_EXTENT_FLAGS_UNINIT	0x0002
#घोषणा EXT2_EXTENT_FLAGS_SECOND_VISIT	0x0004

काष्ठा ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

प्रकार काष्ठा ext2_extent_handle *ext2_extent_handle_t;
प्रकार काष्ठा ext2_extent_path *ext2_extent_path_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#घोषणा EXT2_EXTENT_CURRENT	0x0000
#घोषणा EXT2_EXTENT_MOVE_MASK	0x000F
#घोषणा EXT2_EXTENT_मूल	0x0001
#घोषणा EXT2_EXTENT_LAST_LEAF	0x0002
#घोषणा EXT2_EXTENT_FIRST_SIB	0x0003
#घोषणा EXT2_EXTENT_LAST_SIB	0x0004
#घोषणा EXT2_EXTENT_अग्र_SIB	0x0005
#घोषणा EXT2_EXTENT_पूर्व_SIB	0x0006
#घोषणा EXT2_EXTENT_अग्र_LEAF	0x0007
#घोषणा EXT2_EXTENT_पूर्व_LEAF	0x0008
#घोषणा EXT2_EXTENT_अग्र	0x0009
#घोषणा EXT2_EXTENT_पूर्व	0x000A
#घोषणा EXT2_EXTENT_UP		0x000B
#घोषणा EXT2_EXTENT_DOWN	0x000C
#घोषणा EXT2_EXTENT_DOWN_AND_LAST 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#घोषणा EXT2_EXTENT_जोड़ो_AFTER	0x0001 /* insert after handle loc'n */
#घोषणा EXT2_EXTENT_जोड़ो_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#घोषणा EXT2_EXTENT_DELETE_KEEP_EMPTY	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#घोषणा EXT2_EXTENT_SET_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
काष्ठा ext2_extent_info {
	पूर्णांक		curr_entry;
	पूर्णांक		curr_level;
	पूर्णांक		अंक_entries;
	पूर्णांक		max_entries;
	पूर्णांक		max_depth;
	पूर्णांक		bytes_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#घोषणा EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#घोषणा DIRENT_CHANGED	1
#घोषणा DIRENT_ABORT	2
#घोषणा DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#घोषणा DIRENT_FLAG_INCLUDE_EMPTY	1
#घोषणा DIRENT_FLAG_INCLUDE_REMOVED	2
#घोषणा DIRENT_FLAG_INCLUDE_CSUM	4
#घोषणा DIRENT_FLAG_INCLUDE_INLINE_तथ्य 8

#घोषणा DIRENT_DOT_FILE		1
#घोषणा DIRENT_DOT_DOT_FILE	2
#घोषणा DIRENT_OTHER_FILE	3
#घोषणा DIRENT_DELETED_FILE	4
#घोषणा DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
प्रकार काष्ठा ext2_struct_inode_scan *ext2_inode_scan;

/*
 * ext2fs_scan flags
 */
#घोषणा EXT2_SF_CHK_BADBLOCKS	0x0001
#घोषणा EXT2_SF_BAD_INODE_BLK	0x0002
#घोषणा EXT2_SF_BAD_EXTRA_BYTES	0x0004
#घोषणा EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#घोषणा EXT2_SF_DO_LAZY		0x0010
#घोषणा EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#घोषणा EXT2_MF_MOUNTED		1
#घोषणा EXT2_MF_ISROOT		2
#घोषणा EXT2_MF_READONLY	4
#घोषणा EXT2_MF_SWAP		8
#घोषणा EXT2_MF_BUSY		16
#घोषणा EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#घोषणा LINUX_S_IFMT  00170000
#घोषणा LINUX_S_IFSOCK 0140000
#घोषणा LINUX_S_IFLNK	 0120000
#घोषणा LINUX_S_IFREG  0100000
#घोषणा LINUX_S_IFBLK  0060000
#घोषणा LINUX_S_IFDIR  0040000
#घोषणा LINUX_S_IFCHR  0020000
#घोषणा LINUX_S_IFIFO  0010000
#घोषणा LINUX_S_ISUID  0004000
#घोषणा LINUX_S_ISGID  0002000
#घोषणा LINUX_S_ISVTX  0001000

#घोषणा LINUX_S_IRWXU 00700
#घोषणा LINUX_S_IRUSR 00400
#घोषणा LINUX_S_IWUSR 00200
#घोषणा LINUX_S_IXUSR 00100

#घोषणा LINUX_S_IRWXG 00070
#घोषणा LINUX_S_IRGRP 00040
#घोषणा LINUX_S_IWGRP 00020
#घोषणा LINUX_S_IXGRP 00010

#घोषणा LINUX_S_IRWXO 00007
#घोषणा LINUX_S_IROTH 00004
#घोषणा LINUX_S_IWOTH 00002
#घोषणा LINUX_S_IXOTH 00001

#घोषणा LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#घोषणा LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#घोषणा LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#घोषणा LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#घोषणा LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#घोषणा LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#घोषणा LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#घोषणा EXT2_I_आकार(i)	((i)->i_आकार | ((__u64) (i)->i_आकार_ऊपरी << 32))

अटल अंतरभूत __u32 __encode_extra_time(समय_प्रकार seconds EXT2FS_ATTR((unused)),
					__u32 nsec)
{
	__u32 extra = 0;

#अगर (SIZEOF_TIME_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_MASK;
#पूर्ण_अगर
	वापस extra | (nsec << EXT4_EPOCH_BITS);
}
अटल अंतरभूत समय_प्रकार __decode_extra_sec(समय_प्रकार seconds,
					__u32 extra EXT2FS_ATTR((unused)))
{
#अगर (SIZEOF_TIME_T > 4)
	अगर (extra & EXT4_EPOCH_MASK)
		seconds += ((समय_प्रकार)(extra & EXT4_EPOCH_MASK) << 32);
#पूर्ण_अगर
	वापस seconds;
}
अटल अंतरभूत __u32 __decode_extra_nsec(__u32 extra)
{
	वापस (extra & EXT4_NSEC_MASK) >> EXT4_EPOCH_BITS;
}
#घोषणा ext2fs_inode_actual_आकार(inode)					      \
	((माप_प्रकार)(EXT2_GOOD_OLD_INODE_आकार +				      \
		  (माप(*inode) > EXT2_GOOD_OLD_INODE_आकार ?		      \
		   ((काष्ठा ext2_inode_large *)(inode))->i_extra_isize : 0)))
#घोषणा clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#घोषणा ext2fs_inode_xtime_set(inode, field, sec)			      \
करो {									      \
	अगर (ext2fs_inode_includes(ext2fs_inode_actual_आकार(inode),	      \
				  field ## _extra)) {			      \
		(inode)->field = (__u32)(sec & 0xfffffff);		      \
		((काष्ठा ext2_inode_large *)(inode))->field ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} अन्यथा {							      \
		(inode)->field = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} जबतक (0)
#घोषणा ext2fs_inode_xtime_get(inode, field)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_आकार(inode), field ## _extra) ?    \
	__decode_extra_sec((inode)->field,				      \
		((काष्ठा ext2_inode_large *)(inode))->field ## _extra) :      \
		(समय_प्रकार)(inode)->field)

अटल अंतरभूत व्योम __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)),
				   समय_प्रकार seconds)
{
	*lo = seconds & 0xffffffff;
#अगर (SIZEOF_TIME_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_MASK;
#अन्यथा
	*hi = 0;
#पूर्ण_अगर
}
अटल अंतरभूत समय_प्रकार __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((unused)))
{
#अगर (SIZEOF_TIME_T == 4)
	वापस *lo;
#अन्यथा
	वापस ((समय_प्रकार)(*hi) << 32) | *lo;
#पूर्ण_अगर
}
#घोषणा ext2fs_set_tstamp(sb, field, seconds) \
	__sb_set_tstamp(&(sb)->field, &(sb)->field ## _hi, seconds)
#घोषणा ext2fs_get_tstamp(sb, field) \
	__sb_get_tstamp(&(sb)->field, &(sb)->field ## _hi)

/*
 * ext2_icount_t abstraction
 */
#घोषणा EXT2_ICOUNT_OPT_INCREMENT	0x01
#घोषणा EXT2_ICOUNT_OPT_FULLMAP		0x02

प्रकार काष्ठा ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#घोषणा BMAP_ALLOC	0x0001
#घोषणा BMAP_SET	0x0002
#घोषणा BMAP_UNINIT	0x0004
#घोषणा BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#घोषणा BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#घोषणा READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#घोषणा WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#घोषणा IMAGER_FLAG_INODEMAP	1
#घोषणा IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)

/*
 * Features supported by this version of the library
 */
#घोषणा EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_सूचकांक|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#अगर_घोषित CONFIG_MMP
#घोषणा EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#अन्यथा
#घोषणा EXT4_LIB_INCOMPAT_MMP		(0)
#पूर्ण_अगर

#घोषणा EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64BIT|\
					 EXT4_FEATURE_INCOMPAT_INLINE_तथ्य|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_SEED|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#घोषणा EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
#घोषणा EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#घोषणा EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#घोषणा EXT2FS_CLUSTER_RATIO(fs)	(1 << (fs)->cluster_ratio_bits)
#घोषणा EXT2FS_CLUSTER_MASK(fs)		(EXT2FS_CLUSTER_RATIO(fs) - 1)
#घोषणा EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_ratio_bits)
/* Translate a cluster number to a block number */
#घोषणा EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_ratio_bits)
/* Translate # of blks to # of clusters */
#घोषणा EXT2_FS_अंक_B2_C(fs, blks)	(((blks) + EXT2FS_CLUSTER_MASK(fs)) >> \
					 (fs)->cluster_ratio_bits)

#अगर defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
प्रकार काष्ठा stat64 ext2fs_struct_stat;
#अन्यथा
प्रकार काष्ठा stat ext2fs_struct_stat;
#पूर्ण_अगर

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#घोषणा EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
काष्ठा ext2_xattr_handle;
#घोषणा XATTR_ABORT	1
#घोषणा XATTR_CHANGED	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#घोषणा EXT2FS_BITMAPS_WRITE		0x0001
#घोषणा EXT2FS_BITMAPS_BLOCK		0x0002
#घोषणा EXT2FS_BITMAPS_INODE		0x0004
#घोषणा EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#घोषणा EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#घोषणा EXT2FS_LINK_FT_MASK	0x0007
#घोषणा EXT2FS_LINK_APPEND	0x0010
#घोषणा EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
अटल अंतरभूत पूर्णांक ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	वापस ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
अटल अंतरभूत पूर्णांक ext2fs_needs_large_file_feature(अचिन्हित दीर्घ दीर्घ file_आकार)
{
	वापस file_आकार >= 0x80000000ULL;
}

/* alloc.c */
बाह्य व्योम ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group);
बाह्य errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, पूर्णांक mode,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
बाह्य errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
				  ext2fs_block_bitmap map, blk_t *ret);
बाह्य errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret);
बाह्य errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_block_bitmap map, blk64_t *ret,
				   काष्ठा blk_alloc_ctx *ctx);
बाह्य errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start,
					blk_t finish, पूर्णांक अंक,
					ext2fs_block_bitmap map,
					blk_t *ret);
बाह्य errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start,
					 blk64_t finish, पूर्णांक अंक,
					 ext2fs_block_bitmap map,
					 blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
				    अक्षर *block_buf, blk_t *ret);
बाह्य errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     अक्षर *block_buf, blk64_t *ret);
बाह्य errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     अक्षर *block_buf, blk64_t *ret,
				     काष्ठा blk_alloc_ctx *ctx);

बाह्य व्योम ext2fs_set_alloc_block_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       काष्ठा ext2_inode *inode, blk64_t lblk);
बाह्य व्योम ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, पूर्णांक flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, पूर्णांक flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
बाह्य व्योम ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	व्योम (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse),
	व्योम (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse));
#घोषणा EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#घोषणा EXT2_NEWRANGE_MIN_लंबाई	(0x2)
#घोषणा EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, पूर्णांक flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#घोषणा EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#घोषणा EXT2_ALLOCRANGE_ZERO_BLOCKS	(0x2)
#घोषणा EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, पूर्णांक flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
बाह्य पूर्णांक ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t group,
					ext2fs_block_bitmap bmap);
बाह्य errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t group,
					       ext2fs_block_bitmap bmap,
					       blk_t *desc_blocks);
बाह्य व्योम ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
						  व्योम (*func)(ext2_filsys fs,
							       blk64_t blk,
							       पूर्णांक inuse),
						  व्योम (**old)(ext2_filsys fs,
							       blk64_t blk,
							       पूर्णांक inuse));

/* alloc_stats.c */
व्योम ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, पूर्णांक inuse);
व्योम ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       पूर्णांक inuse, पूर्णांक isdir);
व्योम ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णांक inuse);
व्योम ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णांक inuse);
व्योम ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse);

/* alloc_tables.c */
बाह्य errcode_t ext2fs_allocate_tables(ext2_filsys fs);
बाह्य errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
					     ext2fs_block_bitmap bmap);

/* badblocks.c */
बाह्य errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णांक आकार);
बाह्य errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णांक ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk);
बाह्य पूर्णांक ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
बाह्य errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
बाह्य पूर्णांक ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
बाह्य व्योम ext2fs_u32_list_iterate_end(ext2_u32_iterate iter);
बाह्य errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
बाह्य पूर्णांक ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

बाह्य errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    पूर्णांक आकार);
बाह्य errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb,
					   blk_t blk);
बाह्य पूर्णांक ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
बाह्य पूर्णांक ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
बाह्य व्योम ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
बाह्य errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
बाह्य पूर्णांक ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
बाह्य व्योम ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter);
बाह्य errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
बाह्य पूर्णांक ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
बाह्य पूर्णांक ext2fs_u32_list_गिनती(ext2_u32_list bb);

/* bb_compat */
बाह्य errcode_t badblocks_list_create(badblocks_list *ret, पूर्णांक आकार);
बाह्य errcode_t badblocks_list_add(badblocks_list bb, blk_t blk);
बाह्य पूर्णांक badblocks_list_test(badblocks_list bb, blk_t blk);
बाह्य errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
बाह्य पूर्णांक badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
बाह्य व्योम badblocks_list_iterate_end(badblocks_iterate iter);
बाह्य व्योम badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
बाह्य errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
बाह्य व्योम ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap);
बाह्य व्योम ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
बाह्य errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
					      स्थिर अक्षर *descr,
					      ext2fs_block_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   स्थिर अक्षर *descr,
						   ext2fs_block_bitmap *ret);
बाह्य पूर्णांक ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap);
बाह्य errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      स्थिर अक्षर *descr,
					      ext2fs_inode_bitmap *ret);
बाह्य errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t end, ext2_ino_t *oend);
बाह्य errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					       blk_t end, blk_t *oend);
बाह्य errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend);
बाह्य व्योम ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य व्योम ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap);
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
					ext2_ino_t start, अचिन्हित पूर्णांक अंक,
					व्योम *in);
बाह्य errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, माप_प्रकार अंक,
					 व्योम *in);
बाह्य errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिन्हित पूर्णांक अंक,
					व्योम *out);
बाह्य errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, माप_प्रकार अंक,
					 व्योम *out);
बाह्य errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, अचिन्हित पूर्णांक अंक,
					व्योम *in);
बाह्य errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, माप_प्रकार अंक,
					 व्योम *in);
बाह्य errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, अचिन्हित पूर्णांक अंक,
					व्योम *out);
बाह्य errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, माप_प्रकार अंक,
					 व्योम *out);

/* blknum.c */
बाह्य __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group);
बाह्य __u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);
बाह्य dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t);
बाह्य blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णांक ext2fs_group_blocks_गिनती(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_inode_तथ्य_blocks2(ext2_filsys fs,
					 काष्ठा ext2_inode *inode);
बाह्य blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
				     काष्ठा ext2_inode *inode);
बाह्य blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
					काष्ठा ext2_inode *inode);
बाह्य blk64_t ext2fs_blocks_गिनती(काष्ठा ext2_super_block *super);
बाह्य व्योम ext2fs_blocks_गिनती_set(काष्ठा ext2_super_block *super,
				    blk64_t blk);
बाह्य व्योम ext2fs_blocks_गिनती_add(काष्ठा ext2_super_block *super,
				    blk64_t blk);
बाह्य blk64_t ext2fs_r_blocks_गिनती(काष्ठा ext2_super_block *super);
बाह्य व्योम ext2fs_r_blocks_गिनती_set(काष्ठा ext2_super_block *super,
				      blk64_t blk);
बाह्य व्योम ext2fs_r_blocks_गिनती_add(काष्ठा ext2_super_block *super,
				      blk64_t blk);
बाह्य blk64_t ext2fs_free_blocks_गिनती(काष्ठा ext2_super_block *super);
बाह्य व्योम ext2fs_free_blocks_गिनती_set(काष्ठा ext2_super_block *super,
					 blk64_t blk);
बाह्य व्योम ext2fs_free_blocks_गिनती_add(काष्ठा ext2_super_block *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
बाह्य काष्ठा ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  काष्ठा opaque_ext2_group_desc *gdp,
					  dgrp_t group);
बाह्य blk64_t ext2fs_block_bitmap_csum(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
बाह्य __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t group);
बाह्य blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group,
					blk64_t blk);
बाह्य blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group,
				       blk64_t blk);
बाह्य __u32 ext2fs_bg_free_blocks_गिनती(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_free_blocks_गिनती_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
बाह्य __u32 ext2fs_bg_free_inodes_गिनती(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_free_inodes_गिनती_set(ext2_filsys fs, dgrp_t group,
					 __u32 n);
बाह्य __u32 ext2fs_bg_used_dirs_गिनती(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_used_dirs_गिनती_set(ext2_filsys fs, dgrp_t group,
				       __u32 n);
बाह्य __u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group,
				     __u32 n);
बाह्य __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णांक ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag);
बाह्य व्योम ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
बाह्य व्योम ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags);
बाह्य __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group);
बाह्य व्योम ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum);
बाह्य blk64_t ext2fs_file_acl_block(ext2_filsys fs,
				     स्थिर काष्ठा ext2_inode *inode);
बाह्य व्योम ext2fs_file_acl_block_set(ext2_filsys fs,
				      काष्ठा ext2_inode *inode, blk64_t blk);
बाह्य errcode_t ext2fs_inode_आकार_set(ext2_filsys fs, काष्ठा ext2_inode *inode,
				       ext2_off64_t आकार);

/* block.c */
बाह्य errcode_t ext2fs_block_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      पूर्णांक	flags,
				      अक्षर *block_buf,
				      पूर्णांक (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  पूर्णांक	blockcnt,
						  व्योम	*priv_तथ्य),
				      व्योम *priv_तथ्य);
errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				पूर्णांक	flags,
				अक्षर *block_buf,
				पूर्णांक (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णांक		ref_offset,
					    व्योम	*priv_तथ्य),
				व्योम *priv_तथ्य);
errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णांक	flags,
				अक्षर *block_buf,
				पूर्णांक (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णांक		ref_offset,
					    व्योम	*priv_तथ्य),
				व्योम *priv_तथ्य);

/* bmap.c */
बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode *inode,
			     अक्षर *block_buf, पूर्णांक bmap_flags,
			     blk_t block, blk_t *phys_blk);
बाह्य errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      काष्ठा ext2_inode *inode,
			      अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
			      पूर्णांक *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   काष्ठा ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#अगर 0
/* bmove.c */
बाह्य errcode_t ext2fs_move_blocks(ext2_filsys fs,
				    ext2fs_block_bitmap reserve,
				    ext2fs_block_bitmap alloc_map,
				    पूर्णांक flags);
#पूर्ण_अगर

/* check_desc.c */
बाह्य errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
बाह्य errcode_t ext2fs_close(ext2_filsys fs);
बाह्य errcode_t ext2fs_close2(ext2_filsys fs, पूर्णांक flags);
बाह्य errcode_t ext2fs_close_free(ext2_filsys *fs);
बाह्य errcode_t ext2fs_flush(ext2_filsys fs);
बाह्य errcode_t ext2fs_flush2(ext2_filsys fs, पूर्णांक flags);
बाह्य पूर्णांक ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group_block);
बाह्य errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t group,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_used_blks);
बाह्य पूर्णांक ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t group,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    पूर्णांक *ret_meta_bg);
बाह्य व्योम ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
बाह्य __u32 ext2fs_crc32_be(__u32 crc, अचिन्हित अक्षर स्थिर *p, माप_प्रकार len);
बाह्य __u32 ext2fs_crc32c_le(__u32 crc, अचिन्हित अक्षर स्थिर *p, माप_प्रकार len);

/* csum.c */
बाह्य व्योम ext2fs_init_csum_seed(ext2_filsys fs);
बाह्य errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, काष्ठा mmp_struct *mmp);
बाह्य पूर्णांक ext2fs_mmp_csum_verify(ext2_filsys, काष्ठा mmp_struct *mmp);
बाह्य पूर्णांक ext2fs_verify_csum_type(ext2_filsys fs, काष्ठा ext2_super_block *sb);
बाह्य errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
					    काष्ठा ext2_super_block *sb);
बाह्य पूर्णांक ext2fs_superblock_csum_verify(ext2_filsys fs,
					 काष्ठा ext2_super_block *sb);
बाह्य errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs,
					ext2_ino_t inum, blk64_t block,
					काष्ठा ext2_ext_attr_header *hdr);
बाह्य पूर्णांक ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t block,
					     काष्ठा ext2_ext_attr_header *hdr);
#घोषणा EXT2_DIRENT_पुच्छ(block, blocksize) \
	((काष्ठा ext2_dir_entry_पुच्छ *)(((अक्षर *)(block)) + \
	(blocksize) - माप(काष्ठा ext2_dir_entry_पुच्छ)))

बाह्य व्योम ext2fs_initialize_dirent_पुच्छ(ext2_filsys fs,
					  काष्ठा ext2_dir_entry_पुच्छ *t);
बाह्य पूर्णांक ext2fs_dirent_has_पुच्छ(ext2_filsys fs,
				  काष्ठा ext2_dir_entry *dirent);
बाह्य पूर्णांक ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     काष्ठा ext2_dir_entry *dirent);
बाह्य पूर्णांक ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					काष्ठा ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					   काष्ठा ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  काष्ठा ext2_dir_entry *dirent,
					  काष्ठा ext2_dx_countlimit **cc,
					  पूर्णांक *offset);
बाह्य errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				काष्ठा ext2_dir_entry *dirent,
				__u32 *crc, काष्ठा ext2_dx_पुच्छ **ret_t);
बाह्य errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs,
					      ext2_ino_t inum,
					      काष्ठा ext3_extent_header *eh);
बाह्य पूर्णांक ext2fs_extent_block_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   काष्ठा ext3_extent_header *eh);
बाह्य errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      अक्षर *bitmap, पूर्णांक आकार);
बाह्य पूर्णांक ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   अक्षर *bitmap, पूर्णांक आकार);
बाह्य errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
					      अक्षर *bitmap, पूर्णांक आकार);
बाह्य पूर्णांक ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
					   अक्षर *bitmap, पूर्णांक आकार);
बाह्य errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       काष्ठा ext2_inode_large *inode);
बाह्य पूर्णांक ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    काष्ठा ext2_inode_large *inode);
बाह्य व्योम ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group);
बाह्य पूर्णांक ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group);
बाह्य errcode_t ext2fs_set_gdt_csum(ext2_filsys fs);
बाह्य __u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group);

/* dblist.c */
बाह्य errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
बाह्य errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णांक blockcnt);
बाह्य errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य व्योम ext2fs_dblist_sort(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(स्थिर व्योम *,
							   स्थिर व्योम *));
बाह्य व्योम ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(स्थिर व्योम *,
							    स्थिर व्योम *));
बाह्य errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	पूर्णांक (*func)(ext2_filsys fs, काष्ठा ext2_db_entry *db_info,
		    व्योम	*priv_तथ्य),
	व्योम *priv_तथ्य);
बाह्य errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	पूर्णांक (*func)(ext2_filsys fs, काष्ठा ext2_db_entry2 *db_info,
		    व्योम	*priv_तथ्य),
	व्योम *priv_तथ्य);
बाह्य errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	पूर्णांक (*func)(ext2_filsys fs, काष्ठा ext2_db_entry2 *db_info,
		    व्योम	*priv_तथ्य),
	अचिन्हित दीर्घ दीर्घ start,
	अचिन्हित दीर्घ दीर्घ गिनती,
	व्योम *priv_तथ्य);
बाह्य errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, पूर्णांक blockcnt);
बाह्य errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
बाह्य errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
बाह्य पूर्णांक ext2fs_dblist_गिनती(ext2_dblist dblist);
बाह्य blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
बाह्य errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
					काष्ठा ext2_db_entry **entry);
बाह्य errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					काष्ठा ext2_db_entry2 **entry);
बाह्य errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist);

/* dblist_dir.c */
बाह्य errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  पूर्णांक	flags,
				  अक्षर	*block_buf,
				  पूर्णांक (*func)(ext2_ino_t	dir,
					      पूर्णांक		entry,
					      काष्ठा ext2_dir_entry *dirent,
					      पूर्णांक	offset,
					      पूर्णांक	blocksize,
					      अक्षर	*buf,
					      व्योम	*priv_तथ्य),
				  व्योम *priv_तथ्य);

#अगर 0
/* digest_encode.c */
#घोषणा EXT2_FS_DIGEST_आकार EXT2_FS_SHA256_लंबाई
बाह्य पूर्णांक ext2fs_digest_encode(स्थिर अक्षर *src, पूर्णांक len, अक्षर *dst);
बाह्य पूर्णांक ext2fs_digest_decode(स्थिर अक्षर *src, पूर्णांक len, अक्षर *dst);
#पूर्ण_अगर

/* dirblock.c */
बाह्य errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				       व्योम *buf);
बाह्य errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
					व्योम *buf, पूर्णांक flags);
बाह्य errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
					व्योम *buf, पूर्णांक flags);
बाह्य errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
					व्योम *buf, पूर्णांक flags, ext2_ino_t ino);
बाह्य errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
					व्योम *buf);
बाह्य errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
					 व्योम *buf, पूर्णांक flags);
बाह्य errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
					 व्योम *buf, पूर्णांक flags);
बाह्य errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
					 व्योम *buf, पूर्णांक flags, ext2_ino_t ino);

/* dirhash.c */
बाह्य errcode_t ext2fs_dirhash(पूर्णांक version, स्थिर अक्षर *name, पूर्णांक len,
				स्थिर __u32 *seed,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

बाह्य errcode_t ext2fs_dirhash2(पूर्णांक version, स्थिर अक्षर *name, पूर्णांक len,
				 स्थिर काष्ठा ext2fs_nls_table *charset,
				 पूर्णांक hash_flags,
				 स्थिर __u32 *seed,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
बाह्य errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    काष्ठा ext2_dir_entry *dirent,
				    अचिन्हित पूर्णांक *rec_len);
बाह्य errcode_t ext2fs_set_rec_len(ext2_filsys fs,
				    अचिन्हित पूर्णांक len,
				    काष्ठा ext2_dir_entry *dirent);
बाह्य errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णांक flags,
			      अक्षर *block_buf,
			      पूर्णांक (*func)(काष्ठा ext2_dir_entry *dirent,
					  पूर्णांक	offset,
					  पूर्णांक	blocksize,
					  अक्षर	*buf,
					  व्योम	*priv_तथ्य),
			      व्योम *priv_तथ्य);
बाह्य errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णांक flags,
			      अक्षर *block_buf,
			      पूर्णांक (*func)(ext2_ino_t	dir,
					  पूर्णांक	entry,
					  काष्ठा ext2_dir_entry *dirent,
					  पूर्णांक	offset,
					  पूर्णांक	blocksize,
					  अक्षर	*buf,
					  व्योम	*priv_तथ्य),
			      व्योम *priv_तथ्य);

/* dupfs.c */
बाह्य errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
बाह्य errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
बाह्य __u32 ext2fs_ext_attr_hash_entry(काष्ठा ext2_ext_attr_entry *entry,
					व्योम *तथ्य);
बाह्य __u32 ext2fs_ext_attr_hash_entry_signed(काष्ठा ext2_ext_attr_entry *entry,
					       व्योम *तथ्य);
बाह्य errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     काष्ठा ext2_ext_attr_entry *entry,
					     व्योम *तथ्य, __u32 *hash);
बाह्य errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     काष्ठा ext2_ext_attr_entry *entry,
					     व्योम *तथ्य, __u32 *hash,
					     __u32 *signed_hash);
बाह्य errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, व्योम *buf);
बाह्य errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block,
				       व्योम *buf);
बाह्य errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block,
				       व्योम *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block,
				       व्योम *buf);
बाह्य errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block,
				       व्योम *buf);
बाह्य errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block,
				       व्योम *buf, ext2_ino_t inum);
बाह्य errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   अक्षर *block_buf,
					   पूर्णांक adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   अक्षर *block_buf,
					   पूर्णांक adjust, __u32 *newcount);
बाह्य errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   अक्षर *block_buf,
					   पूर्णांक adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(काष्ठा ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(काष्ठा ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(काष्ठा ext2_xattr_handle *handle,
				   काष्ठा ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(काष्ठा ext2_xattr_handle *h,
				पूर्णांक (*func)(अक्षर *name, अक्षर *मूल्य,
					    माप_प्रकार मूल्य_len, व्योम *तथ्य),
				व्योम *तथ्य);
errcode_t ext2fs_xattr_get(काष्ठा ext2_xattr_handle *h, स्थिर अक्षर *कुंजी,
			   व्योम **मूल्य, माप_प्रकार *मूल्य_len);
errcode_t ext2fs_xattr_set(काष्ठा ext2_xattr_handle *handle,
			   स्थिर अक्षर *कुंजी,
			   स्थिर व्योम *मूल्य,
			   माप_प्रकार मूल्य_len);
errcode_t ext2fs_xattr_remove(काष्ठा ext2_xattr_handle *handle,
			      स्थिर अक्षर *कुंजी);
errcode_t ext2fs_xattr_remove_all(काष्ठा ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(काष्ठा ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       काष्ठा ext2_inode_large *inode);
errcode_t ext2fs_xattrs_गिनती(काष्ठा ext2_xattr_handle *handle, माप_प्रकार *गिनती);
errcode_t ext2fs_xattr_inode_max_आकार(ext2_filsys fs, ext2_ino_t ino,
				      माप_प्रकार *आकार);
#घोषणा XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(काष्ठा ext2_xattr_handle *handle,
			      अचिन्हित पूर्णांक *new_flags, अचिन्हित पूर्णांक *old_flags);
बाह्य व्योम ext2fs_ext_attr_block_rehash(काष्ठा ext2_ext_attr_header *header,
					 काष्ठा ext2_ext_attr_entry *end);
बाह्य __u32 ext2fs_get_ea_inode_hash(काष्ठा ext2_inode *inode);
बाह्य व्योम ext2fs_set_ea_inode_hash(काष्ठा ext2_inode *inode, __u32 hash);
बाह्य __u64 ext2fs_get_ea_inode_ref(काष्ठा ext2_inode *inode);
बाह्य व्योम ext2fs_set_ea_inode_ref(काष्ठा ext2_inode *inode, __u64 ref_गिनती);

/* extent.c */
बाह्य errcode_t ext2fs_extent_header_verify(व्योम *ptr, पूर्णांक आकार);
बाह्य errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
बाह्य errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					काष्ठा ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
बाह्य व्योम ext2fs_extent_free(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   पूर्णांक flags, काष्ठा ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_गाँठ_split(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, पूर्णांक flags,
				       काष्ठा ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_जोड़ो(ext2_extent_handle_t handle, पूर्णांक flags,
				      काष्ठा ext2fs_extent *extent);
बाह्य errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					पूर्णांक flags);
बाह्य errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णांक flags);
बाह्य errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					काष्ठा ext2_extent_info *info);
बाह्य errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
बाह्य errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     पूर्णांक leaf_level, blk64_t blk);
बाह्य errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
बाह्य माप_प्रकार ext2fs_max_extent_depth(ext2_extent_handle_t handle);
बाह्य errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      काष्ठा ext2_inode *inode);
बाह्य errcode_t ext2fs_गिनती_blocks(ext2_filsys fs, ext2_ino_t ino,
				     काष्ठा ext2_inode *inode, blk64_t *ret_गिनती);
बाह्य errcode_t ext2fs_decode_extent(काष्ठा ext2fs_extent *to, व्योम *from,
				      पूर्णांक len);

/* fallocate.c */
#घोषणा EXT2_FALLOCATE_ZERO_BLOCKS	(0x1)
#घोषणा EXT2_FALLOCATE_FORCE_INIT	(0x2)
#घोषणा EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#घोषणा EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#घोषणा EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, पूर्णांक flags, ext2_ino_t ino,
			   काष्ठा ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len);

/* fileio.c */
बाह्य errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   काष्ठा ext2_inode *inode,
				   पूर्णांक flags, ext2_file_t *ret);
बाह्य errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  पूर्णांक flags, ext2_file_t *ret);
बाह्य ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
काष्ठा ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
बाह्य ext2_ino_t ext2fs_file_get_inode_अंक(ext2_file_t file);
बाह्य errcode_t ext2fs_file_close(ext2_file_t file);
बाह्य errcode_t ext2fs_file_flush(ext2_file_t file);
बाह्य errcode_t ext2fs_file_read(ext2_file_t file, व्योम *buf,
				  अचिन्हित पूर्णांक wanted, अचिन्हित पूर्णांक *got);
बाह्य errcode_t ext2fs_file_write(ext2_file_t file, स्थिर व्योम *buf,
				   अचिन्हित पूर्णांक nbytes, अचिन्हित पूर्णांक *written);
बाह्य errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
				   पूर्णांक whence, __u64 *ret_pos);
बाह्य errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
				   पूर्णांक whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_आकार);
बाह्य ext2_off_t ext2fs_file_get_आकार(ext2_file_t file);
बाह्य errcode_t ext2fs_file_set_आकार(ext2_file_t file, ext2_off_t आकार);
बाह्य errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t आकार);

/* finddev.c */
बाह्य अक्षर *ext2fs_find_block_device(dev_t device);

/* flushb.c */
बाह्य errcode_t ext2fs_sync_device(पूर्णांक fd, पूर्णांक flushb);

/* freefs.c */
बाह्य व्योम ext2fs_free(ext2_filsys fs);
बाह्य व्योम ext2fs_free_dblist(ext2_dblist dblist);
बाह्य व्योम ext2fs_badblocks_list_free(ext2_badblocks_list bb);
बाह्य व्योम ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
बाह्य व्योम ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
बाह्य errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 start, __u32 end,
					    __u32 real_end,
					    स्थिर अक्षर *descr, अक्षर *init_map,
					    ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
						__u32 end,
						__u32 real_end,
						स्थिर अक्षर *descr,
						ext2fs_generic_bitmap *ret);
बाह्य errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
बाह्य व्योम ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
बाह्य errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t end,
						 ext2_ino_t *oend);
बाह्य व्योम ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap map);
बाह्य errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_end,
					      __u32 new_real_end,
					      ext2fs_generic_bitmap bmap);
बाह्य errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
बाह्य errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अंक,
						 व्योम *out);
बाह्य errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 start, __u32 अंक,
						 व्योम *in);
बाह्य errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);
बाह्य errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 start, __u32 end,
						       __u32 *out);

/* gen_bitmap64.c */
व्योम ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णांक type, __u64 start, __u64 end,
				    __u64 real_end,
				    स्थिर अक्षर *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
व्योम ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend);
व्योम ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_end,
				     __u64 new_real_end);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिन्हित पूर्णांक अंक,
					व्योम *out);
errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, अचिन्हित पूर्णांक अंक,
					व्योम *in);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap);
errcode_t ext2fs_गिनती_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out);
errcode_t ext2fs_गिनती_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out);
बाह्य अचिन्हित पूर्णांक ext2fs_list_backups(ext2_filsys fs, अचिन्हित पूर्णांक *three,
				अचिन्हित पूर्णांक *five, अचिन्हित पूर्णांक *seven);

/* getenv.c */
बाह्य अक्षर *ext2fs_safe_getenv(स्थिर अक्षर *arg);

/* get_num_dirs.c */
बाह्य errcode_t ext2fs_get_अंक_dirs(ext2_filsys fs, ext2_ino_t *ret_अंक_dirs);

/* getsize.c */
बाह्य errcode_t ext2fs_get_device_आकार(स्थिर अक्षर *file, पूर्णांक blocksize,
					blk_t *retblocks);
बाह्य errcode_t ext2fs_get_device_size2(स्थिर अक्षर *file, पूर्णांक blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
बाह्य पूर्णांक ext2fs_get_dio_alignment(पूर्णांक fd);
errcode_t ext2fs_get_device_sectsize(स्थिर अक्षर *file, पूर्णांक *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(स्थिर अक्षर *file, पूर्णांक *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, काष्ठा ext2_inode *inode,
				 blk64_t अंक_blocks);
errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, काष्ठा ext2_inode *inode,
				 blk64_t अंक_blocks);
errcode_t ext2fs_iblk_set(ext2_filsys fs, काष्ठा ext2_inode *inode, blk64_t b);

/* imager.c */
बाह्य errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);
बाह्य errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);
बाह्य errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);
बाह्य errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);
बाह्य errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);
बाह्य errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, व्योम *buf);
errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, व्योम *buf);

/* initialize.c */
बाह्य errcode_t ext2fs_initialize(स्थिर अक्षर *name, पूर्णांक flags,
				   काष्ठा ext2_super_block *param,
				   io_manager manager, ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, पूर्णांक super_only);

/* icount.c */
बाह्य व्योम ext2fs_free_icount(ext2_icount_t icount);
बाह्य errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, अक्षर *tdb_dir,
					  पूर्णांक flags, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णांक flags,
				       अचिन्हित पूर्णांक आकार,
				       ext2_icount_t hint, ext2_icount_t *ret);
बाह्य errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णांक flags,
				      अचिन्हित पूर्णांक आकार,
				      ext2_icount_t *ret);
बाह्य errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
बाह्य errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
बाह्य errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 गिनती);
बाह्य ext2_ino_t ext2fs_get_icount_आकार(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, खाता *);

/* inline.c */

बाह्य errcode_t ext2fs_get_memalign(अचिन्हित दीर्घ आकार,
				     अचिन्हित दीर्घ align, व्योम *ptr);

/* inline_data.c */
बाह्य errcode_t ext2fs_inline_तथ्य_init(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_तथ्य_आकार(ext2_filsys fs, ext2_ino_t ino,
					 माप_प्रकार *आकार);
बाह्य errcode_t ext2fs_inline_तथ्य_get(ext2_filsys fs, ext2_ino_t ino,
					काष्ठा ext2_inode *inode,
					व्योम *buf, माप_प्रकार *आकार);
बाह्य errcode_t ext2fs_inline_तथ्य_set(ext2_filsys fs, ext2_ino_t ino,
					काष्ठा ext2_inode *inode,
					व्योम *buf, माप_प्रकार आकार);

/* inode.c */
बाह्य errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   अचिन्हित पूर्णांक cache_आकार);
बाह्य व्योम ext2fs_free_inode_cache(काष्ठा ext2_inode_cache *icache);
बाह्य errcode_t ext2fs_flush_icache(ext2_filsys fs);
बाह्य errcode_t ext2fs_get_अग्र_inode_full(ext2_inode_scan scan,
					    ext2_ino_t *ino,
					    काष्ठा ext2_inode *inode,
					    पूर्णांक bufsize);
#घोषणा EXT2_INODE_SCAN_DEFAULT_बफर_BLOCKS	8
बाह्य errcode_t ext2fs_open_inode_scan(ext2_filsys fs, पूर्णांक बफर_blocks,
				  ext2_inode_scan *ret_scan);
बाह्य व्योम ext2fs_close_inode_scan(ext2_inode_scan scan);
बाह्य errcode_t ext2fs_get_अग्र_inode(ext2_inode_scan scan, ext2_ino_t *ino,
			       काष्ठा ext2_inode *inode);
बाह्य errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
						   पूर्णांक	group);
बाह्य व्योम ext2fs_set_inode_callback
	(ext2_inode_scan scan,
	 errcode_t (*done_group)(ext2_filsys fs,
				 ext2_inode_scan scan,
				 dgrp_t group,
				 व्योम * priv_तथ्य),
	 व्योम *done_group_तथ्य);
बाह्य पूर्णांक ext2fs_inode_scan_flags(ext2_inode_scan scan, पूर्णांक set_flags,
				   पूर्णांक clear_flags);
बाह्य errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					काष्ठा ext2_inode * inode,
					पूर्णांक bufsize);
बाह्य errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    काष्ठा ext2_inode * inode);
बाह्य errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    काष्ठा ext2_inode * inode,
				    पूर्णांक bufsize, पूर्णांक flags);
बाह्य errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
					 काष्ठा ext2_inode * inode,
					 पूर्णांक bufsize);
बाह्य errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    काष्ठा ext2_inode * inode);
बाह्य errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     काष्ठा ext2_inode * inode,
				     पूर्णांक bufsize, पूर्णांक flags);
बाह्य errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    काष्ठा ext2_inode * inode);
बाह्य errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
बाह्य errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
बाह्य io_manager inode_io_manager;
बाह्य errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					अक्षर **name);
बाह्य errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 काष्ठा ext2_inode *inode,
					 अक्षर **name);

/* ismounted.c */
बाह्य errcode_t ext2fs_check_if_mounted(स्थिर अक्षर *file, पूर्णांक *mount_flags);
बाह्य errcode_t ext2fs_check_mount_point(स्थिर अक्षर *device, पूर्णांक *mount_flags,
					  अक्षर *mtpt, पूर्णांक mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
बाह्य errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      काष्ठा ext2_inode *inode,
			      अक्षर *block_buf, blk64_t start,
			      blk64_t end);

/* namei.c */
बाह्य errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, स्थिर अक्षर *name,
			 पूर्णांक namelen, अक्षर *buf, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			स्थिर अक्षर *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			      स्थिर अक्षर *name, ext2_ino_t *inode);
बाह्य errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
पूर्णांक ext2fs_native_flag(व्योम);

/* newdir.c */
बाह्य errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, अक्षर **block);
बाह्य errcode_t ext2fs_new_dir_inline_तथ्य(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t parent_ino, __u32 *iblock);

/* nls_utf8.c */
बाह्य स्थिर काष्ठा ext2fs_nls_table *ext2fs_load_nls_table(पूर्णांक encoding);
बाह्य पूर्णांक ext2fs_check_encoded_name(स्थिर काष्ठा ext2fs_nls_table *table,
				     अक्षर *s, माप_प्रकार len, अक्षर **pos);
बाह्य पूर्णांक ext2fs_casefold_cmp(स्थिर काष्ठा ext2fs_nls_table *table,
			       स्थिर अचिन्हित अक्षर *str1, माप_प्रकार len1,
			       स्थिर अचिन्हित अक्षर *str2, माप_प्रकार len2);

/* mkdir.c */
बाह्य errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t inum,
			      स्थिर अक्षर *name);
बाह्य errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent,
			       ext2_ino_t ino, अचिन्हित दीर्घ flags,
			       पूर्णांक link_flags, स्थिर अक्षर *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
काष्ठा ext2fs_journal_params {
	blk_t अंक_journal_blocks;
	blk_t अंक_fc_blocks;
};
बाह्य errcode_t ext2fs_get_journal_params(
		काष्ठा ext2fs_journal_params *params, ext2_filsys fs);
बाह्य errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, पूर्णांक अंक,
				    blk_t *ret_blk, पूर्णांक *ret_गिनती);
बाह्य errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णांक अंक,
				     blk64_t *ret_blk, पूर्णांक *ret_गिनती);
बाह्य errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 अंक_blocks, पूर्णांक flags,
						  अक्षर  **ret_jsb);
बाह्य errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  काष्ठा ext2fs_journal_params *params,
						  पूर्णांक flags, अक्षर  **ret_jsb);
बाह्य errcode_t ext2fs_add_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
बाह्य errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t अंक_blocks,
					  पूर्णांक flags);
बाह्य errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t अंक_blocks,
					   blk64_t goal, पूर्णांक flags);
बाह्य errcode_t ext2fs_add_journal_inode3(ext2_filsys fs,
				    काष्ठा ext2fs_journal_params *params,
				    blk64_t goal, पूर्णांक flags);
बाह्य पूर्णांक ext2fs_default_journal_आकार(__u64 अंक_blocks);
बाह्य पूर्णांक ext2fs_journal_sb_start(पूर्णांक blocksize);

/* openfs.c */
बाह्य errcode_t ext2fs_open(स्थिर अक्षर *name, पूर्णांक flags, पूर्णांक superblock,
			     अचिन्हित पूर्णांक block_आकार, io_manager manager,
			     ext2_filsys *ret_fs);
बाह्य errcode_t ext2fs_open2(स्थिर अक्षर *name, स्थिर अक्षर *io_options,
			      पूर्णांक flags, पूर्णांक superblock,
			      अचिन्हित पूर्णांक block_आकार, io_manager manager,
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
errcode_t ext2fs_get_तथ्य_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_set_तथ्य_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
बाह्य errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अंक_blocks);
बाह्य errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
बाह्य e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs);
बाह्य __u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      अक्षर *buf);
बाह्य errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   अक्षर *buf);
बाह्य पूर्णांक ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, अक्षर *buf);

/* get_pathname.c */
बाह्य errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       अक्षर **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, स्थिर अक्षर *name,
		      ext2_ino_t ino, पूर्णांक flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, स्थिर अक्षर *name,
			ext2_ino_t ino, पूर्णांक flags);
पूर्णांक ext2fs_dir_is_dx(ext2_filsys fs, स्थिर काष्ठा ext2_inode *inode);
व्योम ext2fs_inc_nlink(ext2_filsys fs, काष्ठा ext2_inode *inode);
व्योम ext2fs_dec_nlink(काष्ठा ext2_inode *inode);
पूर्णांक ext2fs_dir_link_max(ext2_filsys fs, काष्ठा ext2_inode_large *inode);
पूर्णांक ext2fs_dir_link_empty(काष्ठा ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 स्थिर अक्षर *name, स्थिर अक्षर *target);
पूर्णांक ext2fs_is_fast_symlink(काष्ठा ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, व्योम *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, व्योम *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_start(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, पूर्णांक immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
अचिन्हित ext2fs_mmp_new_seq(व्योम);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, व्योम **ptr);

/* read_bb.c */
बाह्य errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
बाह्य errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, खाता *f,
				      ext2_badblocks_list *bb_list,
				      व्योम *priv_तथ्य,
				      व्योम (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      अक्षर *badstr,
						      व्योम *priv_तथ्य));
बाह्य errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, खाता *f,
				     ext2_badblocks_list *bb_list,
				     व्योम (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
बाह्य errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
बाह्य errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णांक flags, पूर्णांक अंक_threads);
बाह्य errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
बाह्य errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
बाह्य errcode_t ext2fs_write_block_bitmap (ext2_filsys fs);

/*sha256.c */
#घोषणा EXT2_FS_SHA256_लंबाई 32
#अगर 0
बाह्य व्योम ext2fs_sha256(स्थिर अचिन्हित अक्षर *in, अचिन्हित दीर्घ in_आकार,
		   अचिन्हित अक्षर out[EXT2_FS_SHA256_लंबाई]);
#पूर्ण_अगर

/* sha512.c */
#घोषणा EXT2_FS_SHA512_लंबाई 64
बाह्य व्योम ext2fs_sha512(स्थिर अचिन्हित अक्षर *in, अचिन्हित दीर्घ in_आकार,
			  अचिन्हित अक्षर out[EXT2_FS_SHA512_लंबाई]);

/* swapfs.c */
बाह्य errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, अक्षर *buf, माप_प्रकार आकार,
					पूर्णांक flags);
बाह्य errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, अक्षर *buf, पूर्णांक flags);
बाह्य errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, अक्षर *buf, माप_प्रकार आकार,
					 पूर्णांक flags);
बाह्य errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, अक्षर *buf, पूर्णांक flags);
बाह्य व्योम ext2fs_swap_ext_attr(अक्षर *to, अक्षर *from, पूर्णांक bufsize,
				 पूर्णांक has_header);
बाह्य व्योम ext2fs_swap_ext_attr_header(काष्ठा ext2_ext_attr_header *to_header,
					काष्ठा ext2_ext_attr_header *from_hdr);
बाह्य व्योम ext2fs_swap_ext_attr_entry(काष्ठा ext2_ext_attr_entry *to_entry,
				       काष्ठा ext2_ext_attr_entry *from_entry);
बाह्य व्योम ext2fs_swap_super(काष्ठा ext2_super_block * super);
बाह्य व्योम ext2fs_swap_group_desc(काष्ठा ext2_group_desc *gdp);
बाह्य व्योम ext2fs_swap_group_desc2(ext2_filsys, काष्ठा ext2_group_desc *gdp);
बाह्य व्योम ext2fs_swap_inode_full(ext2_filsys fs, काष्ठा ext2_inode_large *t,
				   काष्ठा ext2_inode_large *f, पूर्णांक hostorder,
				   पूर्णांक bufsize);
बाह्य व्योम ext2fs_swap_inode(ext2_filsys fs,काष्ठा ext2_inode *t,
			      काष्ठा ext2_inode *f, पूर्णांक hostorder);
बाह्य व्योम ext2fs_swap_mmp(काष्ठा mmp_struct *mmp);

/* unix_io.c */
बाह्य पूर्णांक ext2fs_open_file(स्थिर अक्षर *pathname, पूर्णांक flags, mode_t mode);
बाह्य पूर्णांक ext2fs_stat(स्थिर अक्षर *path, ext2fs_struct_stat *buf);
बाह्य पूर्णांक ext2fs_fstat(पूर्णांक fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
बाह्य पूर्णांक ext2fs_inode_has_valid_blocks(काष्ठा ext2_inode *inode);
बाह्य पूर्णांक ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  काष्ठा ext2_inode *inode);

/* version.c */
बाह्य पूर्णांक ext2fs_parse_version_string(स्थिर अक्षर *ver_string);
बाह्य पूर्णांक ext2fs_get_library_version(स्थिर अक्षर **ver_string,
				      स्थिर अक्षर **date_string);

/* write_bb_file.c */
बाह्य errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      अचिन्हित पूर्णांक flags,
				      खाता *f);


/* inline functions */
#अगर_घोषित NO_INLINE_FUNCS
बाह्य errcode_t ext2fs_get_mem(अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य errcode_t ext2fs_get_memzero(अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य errcode_t ext2fs_get_array(अचिन्हित दीर्घ गिनती,
				  अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य errcode_t ext2fs_get_arrayzero(अचिन्हित दीर्घ गिनती,
				      अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य errcode_t ext2fs_free_mem(व्योम *ptr);
बाह्य errcode_t ext2fs_resize_mem(अचिन्हित दीर्घ old_आकार,
				   अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य errcode_t ext2fs_resize_array(अचिन्हित दीर्घ old_गिनती, अचिन्हित दीर्घ गिनती,
				     अचिन्हित दीर्घ आकार, व्योम *ptr);
बाह्य व्योम ext2fs_mark_super_dirty(ext2_filsys fs);
बाह्य व्योम ext2fs_mark_changed(ext2_filsys fs);
बाह्य पूर्णांक ext2fs_test_changed(ext2_filsys fs);
बाह्य व्योम ext2fs_mark_valid(ext2_filsys fs);
बाह्य व्योम ext2fs_unmark_valid(ext2_filsys fs);
बाह्य पूर्णांक ext2fs_test_valid(ext2_filsys fs);
बाह्य व्योम ext2fs_mark_ib_dirty(ext2_filsys fs);
बाह्य व्योम ext2fs_mark_bb_dirty(ext2_filsys fs);
बाह्य पूर्णांक ext2fs_test_ib_dirty(ext2_filsys fs);
बाह्य पूर्णांक ext2fs_test_bb_dirty(ext2_filsys fs);
बाह्य dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk);
बाह्य dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino);
बाह्य blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group);
बाह्य blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group);
बाह्य blk_t ext2fs_inode_तथ्य_blocks(ext2_filsys fs,
				      काष्ठा ext2_inode *inode);
बाह्य पूर्णांक ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णांक blocks);
बाह्य पूर्णांक ext2fs_log2_u32(__u32 arg);
बाह्य पूर्णांक ext2fs_log2_u64(__u64 arg);
बाह्य पूर्णांक ext2fs_log10_u32(__u32 arg);
बाह्य पूर्णांक ext2fs_log10_u64(__u64 arg);
बाह्य अचिन्हित पूर्णांक ext2fs_div_ceil(अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b);
बाह्य __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
बाह्य पूर्णांक ext2fs_dirent_name_len(स्थिर काष्ठा ext2_dir_entry *entry);
बाह्य व्योम ext2fs_dirent_set_name_len(काष्ठा ext2_dir_entry *entry, पूर्णांक len);
बाह्य पूर्णांक ext2fs_dirent_file_type(स्थिर काष्ठा ext2_dir_entry *entry);
बाह्य व्योम ext2fs_dirent_set_file_type(काष्ठा ext2_dir_entry *entry, पूर्णांक type);
बाह्य काष्ठा ext2_inode *ext2fs_inode(काष्ठा ext2_inode_large * large_inode);
बाह्य स्थिर काष्ठा ext2_inode *ext2fs_const_inode(स्थिर काष्ठा ext2_inode_large * large_inode);
बाह्य पूर्णांक ext2fs_inodes_per_orphan_block(ext2_filsys fs);
बाह्य काष्ठा ext4_orphan_block_पुच्छ *ext2fs_orphan_block_पुच्छ(ext2_filsys fs,
							       अक्षर *buf);
#पूर्ण_अगर

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#अगर (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#अगर_घोषित INCLUDE_INLINE_FUNCS
#घोषणा _INLINE_ बाह्य
#अन्यथा
#अगर (__STDC_VERSION__ >= 199901L)
#घोषणा _INLINE_ अंतरभूत
#अन्यथा
#अगर_घोषित __GNUC__
#घोषणा _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#घोषणा _INLINE_ बाह्य अंतरभूत
#पूर्ण_अगर /* __GNUC__ */
#पूर्ण_अगर /* __STDC_VERSION__ >= 199901L */
#पूर्ण_अगर

#अगर_अघोषित EXT2_CUSTOM_MEMORY_ROUTINES
#समावेश <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(अचिन्हित दीर्घ आकार, व्योम *ptr)
{
	व्योम *pp;

	pp = दो_स्मृति(आकार);
	अगर (!pp)
		वापस EXT2_ET_NO_MEMORY;
	स_नकल(ptr, &pp, माप (pp));
	वापस 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(अचिन्हित दीर्घ आकार, व्योम *ptr)
{
	व्योम *pp;

	pp = दो_स्मृति(आकार);
	अगर (!pp)
		वापस EXT2_ET_NO_MEMORY;
	स_रखो(pp, 0, आकार);
	स_नकल(ptr, &pp, माप(pp));
	वापस 0;
}

_INLINE_ errcode_t ext2fs_get_array(अचिन्हित दीर्घ गिनती, अचिन्हित दीर्घ आकार,
				    व्योम *ptr)
{
	अगर (गिनती && (~0UL)/गिनती < आकार)
		वापस EXT2_ET_NO_MEMORY;
	वापस ext2fs_get_mem(गिनती*आकार, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(अचिन्हित दीर्घ गिनती,
					अचिन्हित दीर्घ आकार, व्योम *ptr)
{
	अगर (गिनती && (~0UL)/गिनती < आकार)
		वापस EXT2_ET_NO_MEMORY;

	वापस ext2fs_get_memzero((माप_प्रकार)गिनती * आकार, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(व्योम *ptr)
{
	व्योम *p;

	स_नकल(&p, ptr, माप(p));
	मुक्त(p);
	p = 0;
	स_नकल(ptr, &p, माप(p));
	वापस 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(अचिन्हित दीर्घ EXT2FS_ATTR((unused)) old_आकार,
				     अचिन्हित दीर्घ आकार, व्योम *ptr)
{
	व्योम *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	स_नकल(&p, ptr, माप(p));
	p = पुनः_स्मृति(p, आकार);
	अगर (!p)
		वापस EXT2_ET_NO_MEMORY;
	स_नकल(ptr, &p, माप(p));
	वापस 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(अचिन्हित दीर्घ आकार,
				       अचिन्हित दीर्घ old_गिनती,
				       अचिन्हित दीर्घ गिनती, व्योम *ptr)
{
	अचिन्हित दीर्घ old_आकार;
	errcode_t retval;

	अगर (गिनती && (~0UL)/गिनती < आकार)
		वापस EXT2_ET_NO_MEMORY;

	आकार *= गिनती;
	old_आकार = आकार * old_गिनती;
	retval = ext2fs_resize_mem(old_आकार, आकार, ptr);
	अगर (retval)
		वापस retval;

	अगर (आकार > old_आकार) {
		व्योम *p;

		स_नकल(&p, ptr, माप(p));
		स_रखो((अक्षर *)p + old_आकार, 0, आकार - old_आकार);
		स_नकल(ptr, &p, माप(p));
	}

	वापस 0;
}
#पूर्ण_अगर	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ व्योम ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ व्योम ext2fs_mark_changed(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ पूर्णांक ext2fs_test_changed(ext2_filsys fs)
{
	वापस (fs->flags & EXT2_FLAG_CHANGED);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ व्योम ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ व्योम ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ पूर्णांक ext2fs_test_valid(ext2_filsys fs)
{
	वापस (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ व्योम ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ व्योम ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_CHANGED;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ पूर्णांक ext2fs_test_ib_dirty(ext2_filsys fs)
{
	वापस (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ पूर्णांक ext2fs_test_bb_dirty(ext2_filsys fs)
{
	वापस (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	वापस ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	वापस (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	वापस (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	वापस (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_तथ्य_blocks(ext2_filsys fs,
					काष्ठा ext2_inode *inode)
{
	वापस (blk_t) ext2fs_inode_तथ्य_blocks2(fs, inode);
}

_INLINE_ पूर्णांक ext2fs_htree_intnode_maxrecs(ext2_filsys fs, पूर्णांक blocks)
{
	पूर्णांक csum_आकार = 0;

	अगर ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_आकार = माप(काष्ठा ext2_dx_पुच्छ);
	वापस blocks * ((fs->blocksize - (8 + csum_आकार)) /
						माप(काष्ठा ext2_dx_entry));
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णांक ext2fs_log2_u32(__u32 arg)
{
	पूर्णांक l = 0;

	arg >>= 1;
	जबतक (arg) {
		l++;
		arg >>= 1;
	}
	वापस l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ पूर्णांक ext2fs_log2_u64(__u64 arg)
{
	पूर्णांक l = 0;

	arg >>= 1;
	जबतक (arg) {
		l++;
		arg >>= 1;
	}
	वापस l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णांक ext2fs_log10_u32(__u32 arg)
{
	पूर्णांक l = 0;

	arg /= 10;
	जबतक (arg) {
		l++;
		arg /= 10;
	}
	वापस l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ पूर्णांक ext2fs_log10_u64(__u64 arg)
{
	पूर्णांक l = 0;

	arg /= 10;
	जबतक (arg) {
		l++;
		arg /= 10;
	}
	वापस l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ अचिन्हित पूर्णांक ext2fs_div_ceil(अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b)
{
	अगर (!a)
		वापस 0;
	वापस ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	अगर (!a)
		वापस 0;
	वापस ((a - 1) / b) + 1;
}

_INLINE_ पूर्णांक ext2fs_dirent_name_len(स्थिर काष्ठा ext2_dir_entry *entry)
{
	वापस entry->name_len & 0xff;
}

_INLINE_ व्योम ext2fs_dirent_set_name_len(काष्ठा ext2_dir_entry *entry, पूर्णांक len)
{
	entry->name_len = (entry->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ पूर्णांक ext2fs_dirent_file_type(स्थिर काष्ठा ext2_dir_entry *entry)
{
	वापस entry->name_len >> 8;
}

_INLINE_ व्योम ext2fs_dirent_set_file_type(काष्ठा ext2_dir_entry *entry, पूर्णांक type)
{
	entry->name_len = (entry->name_len & 0xff) | (type << 8);
}

_INLINE_ काष्ठा ext2_inode *ext2fs_inode(काष्ठा ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	वापस (काष्ठा ext2_inode *) large_inode;
}

_INLINE_ स्थिर काष्ठा ext2_inode *
ext2fs_const_inode(स्थिर काष्ठा ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	वापस (स्थिर काष्ठा ext2_inode *) large_inode;
}

_INLINE_ पूर्णांक ext2fs_inodes_per_orphan_block(ext2_filsys fs)
{
	वापस (fs->blocksize - माप(काष्ठा ext4_orphan_block_पुच्छ)) /
		माप(__u32);
}

_INLINE_ काष्ठा ext4_orphan_block_पुच्छ *
ext2fs_orphan_block_पुच्छ(ext2_filsys fs, अक्षर *buf)
{
	वापस (काष्ठा ext4_orphan_block_पुच्छ *)(buf + fs->blocksize -
		माप(काष्ठा ext4_orphan_block_पुच्छ));
}

#अघोषित _INLINE_
#पूर्ण_अगर

/* htree levels for ext4 */
#घोषणा EXT4_HTREE_LEVEL_COMPAT 2
#घोषणा EXT4_HTREE_LEVEL	3

अटल अंतरभूत अचिन्हित पूर्णांक ext2_dir_htree_level(ext2_filsys fs)
{
	अगर (ext2fs_has_feature_largedir(fs->super))
		वापस EXT4_HTREE_LEVEL;

	वापस EXT4_HTREE_LEVEL_COMPAT;
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
अटल अंतरभूत व्योम ext2fs_set_dtime(ext2_filsys fs, काष्ठा ext2_inode *inode)
{
	__u32	t;

	अगर (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		t = fs->now & 0xFFFFFFFF;
	अन्यथा
		t = समय(शून्य) & 0xFFFFFFFF;
	अगर (t < fs->super->s_inodes_गिनती)
		t = fs->super->s_inodes_गिनती;
	inode->i_dtime = t;
}

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर /* _EXT2FS_EXT2FS_H */
