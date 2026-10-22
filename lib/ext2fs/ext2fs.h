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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _EXT2FS_EXT2FS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _EXT2FS_EXT2FS_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_ATTR(x) __attribute__(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_ATTR(x)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __nonstring
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __has_attribute
#ਜੇ __has_attribute(__nonstring__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __nonstring                    __attribute__((__nonstring__))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __nonstring
#ਜੇ_ਅੰਤ /* __has_attribute(__nonstring__) */
#ਨਹੀਂਤਾਂ
# define __nonstring
#ਜੇ_ਅੰਤ /* __has_attribute */
#ਜੇ_ਅੰਤ /* __nonstring */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ	EXT2FS_ATTR((ਅਣਵਰਤਿਆ))
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*
 * Non-GNU C compilers won't necessarily understand inline
 */
#ਜੇ (!defined(__GNUC__) && !defined(__WATCOMC__))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_INLINE_FUNCS
#ਜੇ_ਅੰਤ

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUPERBLOCK_ਵਿੱਥ	1024
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUPERBLOCK_ਮਾਪ		1024

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UUID_STR_ਮਾਪ 37

/*
 * The last ext2fs revision level that this version of the library is
 * able to support.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਜੇ EXT2_FLAT_INCLUDES
#ਅਨ੍ਤਰ੍ਭਾਵ "e2_types.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext3_extents.h"
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_fs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext3_extents.h>
#ਜੇ_ਅੰਤ /* EXT2_FLAT_INCLUDES */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u32 __bitwise		ext2_ino_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u32 __bitwise		blk_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u64 __bitwise		blk64_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u32 __bitwise		dgrp_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __s32 __bitwise		ext2_off_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __s64 __bitwise		ext2_off64_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __s64 __bitwise		e2_blkcnt_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u32 __bitwise		ext2_dirhash_t;

#ਜੇ EXT2_FLAT_INCLUDES
#ਅਨ੍ਤਰ੍ਭਾਵ "com_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_ext_attr.h"
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <et/com_err.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_io.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_err.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_ext_attr.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hashmap.h"

/*
 * Portability help for Microsoft Visual C++
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_QSORT_TYPE ਪੂਰਨ __cdecl
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_QSORT_TYPE ਪੂਰਨ
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ struct_ext2_filsys *ext2_filsys;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_MARK_ERROR 	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_UNMARK_ERROR 	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_TEST_ERROR	2

ਬਣਤਰ ext2fs_struct_generic_bitmap_ਅਧਾਰ {
	errcode_t		magic;
	ext2_filsys 		fs;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_struct_generic_bitmap_ਅਧਾਰ *ext2fs_generic_bitmap;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_struct_generic_bitmap_ਅਧਾਰ *ext2fs_inode_bitmap;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_struct_generic_bitmap_ਅਧਾਰ *ext2fs_ਖੰਡ_bitmap;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਪਹਿਲਾ_INODE(s)	EXT2_ਪਹਿਲਾ_INO(s)


/*
 * Badblocks list definitions
 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_list *ext2_badblocks_list;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_iterate *ext2_badblocks_iterate;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_list *ext2_u32_list;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_iterate *ext2_u32_iterate;

/* old */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_list *badblocks_list;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_u32_iterate *badblocks_iterate;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BADBLOCKS_FLAG_DIRTY	1

/*
 * ext2_dblist structure and abstractions (see dblist.c)
 */
ਬਣਤਰ ext2_db_entry2 {
	ext2_ino_t	ino;
	blk64_t	blk;
	e2_blkcnt_t	blockcnt;
};

/* Ye Olde 32-bit version */
ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ {
	ext2_ino_t	ino;
	blk_t	blk;
	ਪੂਰਨ	blockcnt;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_dblist *ext2_dblist;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DBLIST_ABORT	1

/*
 * ext2_fileio definitions
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FILE_WRITE		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FILE_CREATE	0x0002

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FILE_ਨਕਾਬ		0x00FF

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FILE_BUF_DIRTY	0x4000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FILE_BUF_VALID	0x2000

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_file *ext2_file_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SEEK_ਸੈੱਟ	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SEEK_CUR	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SEEK_ਸਿਰਾ	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_RW			0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਬਦਲਿਆ		0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_DIRTY			0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_VALID			0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IB_DIRTY		0x10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_BB_DIRTY		0x20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ		0x40
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ_READ	0x80
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ_WRITE	0x100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_MASTER_SB_ONLY	0x200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_FORCE			0x400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_SUPER_ONLY		0x800
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IMAGE_FILE		0x2000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_EXCLUSIVE		0x4000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_64_ਬਿੱਟ		0x20000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_PRINT_ਤਰੱਕੀ	0x40000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਸਿੱਧਾ_IO		0x80000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_SKIP_MMP		0x100000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_SHARE_DUP		0x400000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IGNORE_SB_ERRORS	0x800000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_BBITMAP_ਪੂਛ_PROBLEM	0x1000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IBITMAP_ਪੂਛ_PROBLEM	0x2000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_ਧਾਗੇ		0x4000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_IGNORE_ਅਦਲਾ_ਬਦਲੀ_DIRENT	0x8000000

/*
 * Internal flags for use by the ext2fs library only
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG2_USE_FAKE_ਸਮਾਂ	0x000000001

/*
 * Special flag in the ext2 inode i_flag field that means that this is
 * a new inode.  (So that ext2_write_inode() can clear extra fields.)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NEW_INODE_FL	0x80000000

/*
 * Flags for mkjournal
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MKJOURNAL_V1_SUPER	0x0000001 /* create V1 superblock (deprecated) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MKJOURNAL_LAZYINIT	0x0000002 /* don't zero journal inode before use*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MKJOURNAL_NO_MNT_CHECK 0x0000004 /* don't check mount status */

/*
 * Normal journal area size to fast commit area size ratio. This is used to
 * set default size of fast commit area.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_JOURNAL_ਮੰਜ਼ਿਲ_FC_BLKS_ਅਨੁਪਾਤ		64

ਬਣਤਰ blk_alloc_ctx;
ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc;

ਬਣਤਰ struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	ਪੂਰਨ				flags;
	ਅੱਖਰ *				device_name;
	ਬਣਤਰ ext2_super_ਖੰਡ	* 	super;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ			blocksize;
	ਪੂਰਨ				fragsize;
	dgrp_t				ਸਮੂਹ_desc_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ			desc_ਖੰਡਾਂ;
	ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc *	ਸਮੂਹ_desc;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ			inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_ਖੰਡ_bitmap		ਖੰਡ_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_ਖੰਡਾਂ)(ext2_filsys fs, ext2_ino_t ino, blk_t *ਖੰਡਾਂ);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				ਬਣਤਰ ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				ਬਣਤਰ ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	ਬਣਤਰ ext2_super_ਖੰਡ *	orig_super;
	ਬਣਤਰ ext2_image_hdr *		image_ਸਿਰਲੇਖ;
	__u32				umask;
	time_t				now;
	ਪੂਰਨ				cluster_ਅਨੁਪਾਤ_ਬਿੱਟ;
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
	ਖਾਲੀ *				priv_ਅੰਕੜੇ;

	/*
	 * Inode cache
	 */
	ਬਣਤਰ ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_ਖੰਡ)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	errcode_t (*get_alloc_block2)(ext2_filsys fs, blk64_t goal,
				      blk64_t *ret, ਬਣਤਰ blk_alloc_ctx *ctx);
	ਖਾਲੀ (*ਖੰਡ_alloc_stats)(ext2_filsys fs, blk64_t blk, ਪੂਰਨ inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	ਖਾਲੀ *mmp_buf;
	ਖਾਲੀ *mmp_cmp;
	ਪੂਰਨ mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	ਲੰਮਾ mmp_ਆਖਰੀ_written;

	/* progress operation functions */
	ਬਣਤਰ ext2fs_ਤਰੱਕੀ_ops *ਤਰੱਕੀ_ops;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_ਬੀਜ;

	io_channel			journal_io;
	ਅੱਖਰ				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	ਖਾਲੀ (*ਖੰਡ_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t ਅੰਕ,
					ਪੂਰਨ inuse);

	/* hashmap for SHA of data blocks */
	ਬਣਤਰ ext2fs_hashmap* ਖੰਡ_sha_map;

	ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *encoding;
};

#ਜੇ EXT2_FLAT_INCLUDES
#ਅਨ੍ਤਰ੍ਭਾਵ "e2_bitops.h"
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/bitops.h>
#ਜੇ_ਅੰਤ

/*
 * 64-bit bitmap backend types
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BMAP64_BITARRAY	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BMAP64_RBTREE	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ਬਦਲਿਆ			1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ABORT			2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ERROR			4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ	8

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_APPEND	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_HOLE		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_READ_ONLY	8

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ਗਿਣਤੀ_IND		(-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ਗਿਣਤੀ_DIND	(-2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ਗਿਣਤੀ_TIND	(-3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ਗਿਣਤੀ_TRANSLATOR	(-4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ALLOC_UNKNOWN	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ALLOC_ਅੰਕੜੇ	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_ALLOC_METADATA	2

ਬਣਤਰ blk_alloc_ctx {
	ext2_ino_t		ino;
	ਬਣਤਰ ext2_inode	*inode;
	blk64_t			lblk;
	ਪੂਰਨ			flags;
};

#ਜੇ 0
/*
 * Flags for ext2fs_move_blocks
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BMOVE_GET_DBLIST	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BMOVE_DEBUG	0x0002
#ਜੇ_ਅੰਤ

/*
 * Generic (non-filesystem layout specific) extents structure
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_FLAGS_ਪੱਤੀ		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_FLAGS_UNINIT	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ	0x0004

ਬਣਤਰ ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_extent_handle *ext2_extent_handle_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_extent_ਰਸਤਾ *ext2_extent_ਰਸਤਾ_t;

/*
 * Flags used by ext2fs_extent_get()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_CURRENT	0x0000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_MOVE_ਨਕਾਬ	0x000F
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਜੜ੍ਹ	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਆਖਰੀ_ਪੱਤੀ	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਪਹਿਲਾ_SIB	0x0003
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਆਖਰੀ_SIB	0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਅਗਲਾ_SIB	0x0005
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_PREV_SIB	0x0006
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ	0x0007
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_PREV_ਪੱਤੀ	0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਅਗਲਾ	0x0009
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_PREV	0x000A
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਉੱਪਰ		0x000B
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਥੱਲੇ	0x000C
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਥੱਲੇ_AND_ਆਖਰੀ 0x000D

/*
 * Flags used by ext2fs_extent_insert()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਪਾਓ_AFTER	0x0001 /* insert after handle loc'n */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਪਾਓ_NOSPLIT	0x0002 /* insert may not cause split */

/*
 * Flags used by ext2fs_extent_delete()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_DELETE_KEEP_ਖਾਲੀ_ਥਾਵਾਂ	0x001 /* keep node if last extent gone */

/*
 * Flags used by ext2fs_extent_set_bmap()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXTENT_ਸੈੱਟ_BMAP_UNINIT	0x0001

/*
 * Data structure returned by ext2fs_extent_get_info()
 */
ਬਣਤਰ ext2_extent_info {
	ਪੂਰਨ		curr_ਇੰਦਰਾਜ;
	ਪੂਰਨ		curr_level;
	ਪੂਰਨ		ਅੰਕ_entries;
	ਪੂਰਨ		max_entries;
	ਪੂਰਨ		max_ਡੂੰਘਾਈ;
	ਪੂਰਨ		ਬਾਈਟਾਂ_avail;
	blk64_t		max_lblk;
	blk64_t		max_pblk;
	__u32		max_len;
	__u32		max_uninit_len;
};

/*
 * Flags for directory block reading and writing functions
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_ਬਦਲਿਆ	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_ABORT	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_FLAG_INCLUDE_REMOVED	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_FLAG_INCLUDE_CSUM	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_FLAG_INCLUDE_INLINE_ਅੰਕੜੇ 8

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_DOT_FILE		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_DOT_DOT_FILE	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_OTHER_FILE	3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_DELETED_FILE	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_CHECKSUM		5

/*
 * Inode scan definitions
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_struct_inode_ਜਾਂਚ *ext2_inode_ਜਾਂਚ;

/*
 * ext2fs_scan flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_CHK_BADBLOCKS	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_BAD_INODE_BLK	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_BAD_EXTRA_ਬਾਈਟਾਂ	0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_DO_LAZY		0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_MOUNTED		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_ISROOT		2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_READONLY	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_ਅਦਲਾ_ਬਦਲੀ		8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_BUSY		16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MF_EXTFS		32

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFMT  00170000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFSOCK 0140000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFLNK	 0120000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFREG  0100000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFBLK  0060000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFDIR  0040000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFCHR  0020000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IFIFO  0010000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISUID  0004000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISGID  0002000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISVTX  0001000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IRWXU 00700
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IRUSR 00400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IWUSR 00200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IXUSR 00100

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IRWXG 00070
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IRGRP 00040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IWGRP 00020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IXGRP 00010

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IRWXO 00007
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IROTH 00004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IWOTH 00002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_IXOTH 00001

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_I_ਮਾਪ(i)	((i)->i_ਮਾਪ | ((__u64) (i)->i_ਮਾਪ_ਉੱਚਾ << 32))

ਸਥਿਰ ਸਤਰਅੰਦਰ __u32 __encode_extra_time(time_t seconds EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
					__u32 nsec)
{
	__u32 extra = 0;

#ਜੇ (SIZEOF_ਸਮਾਂ_T > 4)
	extra = ((seconds - (__s32)(seconds & 0xffffffff)) >> 32) &
		EXT4_EPOCH_ਨਕਾਬ;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ extra | (nsec << EXT4_EPOCH_ਬਿੱਟ);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ time_t __decode_extra_sec(time_t seconds,
					__u32 extra EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
#ਜੇ (SIZEOF_ਸਮਾਂ_T > 4)
	ਜੇ (extra & EXT4_EPOCH_ਨਕਾਬ)
		seconds += ((time_t)(extra & EXT4_EPOCH_ਨਕਾਬ) << 32);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ seconds;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ __u32 __decode_extra_nsec(__u32 extra)
{
	ਵਾਪਸ (extra & EXT4_NSEC_ਨਕਾਬ) >> EXT4_EPOCH_ਬਿੱਟ;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_inode_actual_ਮਾਪ(inode)					      \
	((size_t)(EXT2_GOOD_OLD_INODE_ਮਾਪ +				      \
		  (ਆਕਾਰ(*inode) > EXT2_GOOD_OLD_INODE_ਮਾਪ ?		      \
		   ((ਬਣਤਰ ext2_inode_large *)(inode))->i_extra_isize : 0)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ?	      \
						       (max) : (val)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_inode_xtime_ਸੈੱਟ(inode, ਖੇਤਰ, sec)			      \
ਕਰੋ {									      \
	ਜੇ (ext2fs_inode_includes(ext2fs_inode_actual_ਮਾਪ(inode),	      \
				  ਖੇਤਰ ## _extra)) {			      \
		(inode)->ਖੇਤਰ = (__u32)(sec & 0xfffffff);		      \
		((ਬਣਤਰ ext2_inode_large *)(inode))->ਖੇਤਰ ## _extra =       \
			__encode_extra_time(sec, 0);			      \
	} ਨਹੀਂਤਾਂ {							      \
		(inode)->ਖੇਤਰ = (__u32) clamp(sec, INT32_MIN, INT32_MAX);    \
	}								      \
} ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_inode_xtime_get(inode, ਖੇਤਰ)				      \
(ext2fs_inode_includes(ext2fs_inode_actual_ਮਾਪ(inode), ਖੇਤਰ ## _extra) ?    \
	__decode_extra_sec((inode)->ਖੇਤਰ,				      \
		((ਬਣਤਰ ext2_inode_large *)(inode))->ਖੇਤਰ ## _extra) :      \
		(time_t)(inode)->ਖੇਤਰ)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __sb_set_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				   time_t seconds)
{
	*lo = seconds & 0xffffffff;
#ਜੇ (SIZEOF_ਸਮਾਂ_T > 4)
	*hi = (seconds >> 32) & EXT4_EPOCH_ਨਕਾਬ;
#ਨਹੀਂਤਾਂ
	*hi = 0;
#ਜੇ_ਅੰਤ
}
ਸਥਿਰ ਸਤਰਅੰਦਰ time_t __sb_get_tstamp(__u32 *lo, __u8 *hi EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
#ਜੇ (SIZEOF_ਸਮਾਂ_T == 4)
	ਵਾਪਸ *lo;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ ((time_t)(*hi) << 32) | *lo;
#ਜੇ_ਅੰਤ
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_ਸੈੱਟ_tstamp(sb, ਖੇਤਰ, seconds) \
	__sb_set_tstamp(&(sb)->ਖੇਤਰ, &(sb)->ਖੇਤਰ ## _hi, seconds)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_get_tstamp(sb, ਖੇਤਰ) \
	__sb_get_tstamp(&(sb)->ਖੇਤਰ, &(sb)->ਖੇਤਰ ## _hi)

/*
 * ext2_icount_t abstraction
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ICOUNT_OPT_INCREMENT	0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ICOUNT_OPT_FULLMAP		0x02

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_ALLOC	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_ਸੈੱਟ	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_UNINIT	0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_RET_UNINIT	0x0001

/*
 * Flags for ext2fs_read_inode2
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_INODE_NOCSUM	0x0001

/*
 * Flags for ext2fs_write_inode2
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_INODE_NOCSUM	0x0001

/*
 * Flags for imager.c functions
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IMAGER_FLAG_INODEMAP	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

/*
 * Features supported by this version of the library
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_FEATURE_COMPAT_SUPP	(EXT2_FEATURE_COMPAT_DIR_PREALLOC|\
					 EXT2_FEATURE_COMPAT_IMAGIC_INODES|\
					 EXT3_FEATURE_COMPAT_HAS_JOURNAL|\
					 EXT2_FEATURE_COMPAT_RESIZE_INODE|\
					 EXT2_FEATURE_COMPAT_DIR_ਸੂਚਕਾਂਕ|\
					 EXT2_FEATURE_COMPAT_EXT_ATTR|\
					 EXT4_FEATURE_COMPAT_SPARSE_SUPER2|\
					 EXT4_FEATURE_COMPAT_FAST_COMMIT|\
					 EXT4_FEATURE_COMPAT_STABLE_INODES|\
					 EXT4_FEATURE_COMPAT_ORPHAN_FILE)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_LIB_INCOMPAT_MMP		(0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|\
					 EXT3_FEATURE_INCOMPAT_JOURNAL_DEV|\
					 EXT2_FEATURE_INCOMPAT_META_BG|\
					 EXT3_FEATURE_INCOMPAT_RECOVER|\
					 EXT3_FEATURE_INCOMPAT_EXTENTS|\
					 EXT4_FEATURE_INCOMPAT_FLEX_BG|\
					 EXT4_FEATURE_INCOMPAT_EA_INODE|\
					 EXT4_LIB_INCOMPAT_MMP|\
					 EXT4_FEATURE_INCOMPAT_64_ਦੋਆਧਾਰੀ_ਅੰਕ|\
					 EXT4_FEATURE_INCOMPAT_INLINE_ਅੰਕੜੇ|\
					 EXT4_FEATURE_INCOMPAT_ENCRYPT|\
					 EXT4_FEATURE_INCOMPAT_CASEFOLD|\
					 EXT4_FEATURE_INCOMPAT_CSUM_ਬੀਜ|\
					 EXT4_FEATURE_INCOMPAT_LARGEDIR)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|\
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
					 EXT4_FEATURE_RO_COMPAT_SHARED_ਖੰਡਾਂ |\
					 EXT4_FEATURE_RO_COMPAT_VERITY |\
					 EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT)

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs)	(1 << (fs)->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_CLUSTER_ਨਕਾਬ(fs)		(EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ)
/* Translate a cluster number to a block number */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_C2B(fs, cluster)		((cluster) << (fs)->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ)
/* Translate # of blks to # of clusters */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_ਅੰਕ_B2_C(fs, blks)	(((blks) + EXT2_FS_CLUSTER_ਨਕਾਬ(fs)) >> \
					 (fs)->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ)

#ਜੇ defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ stat64 ext2fs_struct_stat;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ stat ext2fs_struct_stat;
#ਜੇ_ਅੰਤ

/*
 * For ext2fs_close2() and ext2fs_flush2(), this flag allows you to
 * avoid the fsync call.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAG_FLUSH_NO_SYNC          1

/*
 * Modify and iterate extended attributes
 */
ਬਣਤਰ ext2_xattr_handle;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XATTR_ABORT	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XATTR_ਬਦਲਿਆ	2

/*
 * flags for ext2fs_rw_bitmaps()
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BITMAPS_WRITE		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_BITMAPS_ਖੰਡ		0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BITMAPS_INODE		0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BITMAPS_VALID_FLAGS	0x0007

/*
 * flags for ext2fs_unlink()
 */

/* Forcefully unlink even if the inode number doesn't match the dirent */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_UNLINK_FORCE		0x1

/*
 * flags for ext2fs_link()
 *
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_LINK_FT_ਨਕਾਬ	0x0007
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_LINK_APPEND	0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_LINK_EXPAND	0x0020

/*
 * function prototypes
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_has_ਸਮੂਹ_desc_csum(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}

/* The LARGE_FILE feature should be set if we have stored files 2GB+ in size */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_needs_large_file_feature(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ file_ਮਾਪ)
{
	ਵਾਪਸ file_ਮਾਪ >= 0x80000000ULL;
}

/* alloc.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_clear_ਖੰਡ_uninit(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir, ਪੂਰਨ ਬਹੁਲਕ,
				  ext2fs_inode_bitmap map, ext2_ino_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_new_ਖੰਡ(ext2_filsys fs, blk_t goal,
				  ext2fs_ਖੰਡ_bitmap map, blk_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
				   ext2fs_ਖੰਡ_bitmap map, blk64_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
				   ext2fs_ਖੰਡ_bitmap map, blk64_t *ret,
				   ਬਣਤਰ blk_alloc_ctx *ctx);
ਬਾਹਰੀ errcode_t ext2fs_get_free_ਖੰਡਾਂ(ext2_filsys fs, blk_t ਸ਼ੁਰੂ,
					blk_t ਅੰਤ, ਪੂਰਨ ਅੰਕ,
					ext2fs_ਖੰਡ_bitmap map,
					blk_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ,
					 blk64_t ਅੰਤ, ਪੂਰਨ ਅੰਕ,
					 ext2fs_ਖੰਡ_bitmap map,
					 blk64_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_alloc_ਖੰਡ(ext2_filsys fs, blk_t goal,
				    ਅੱਖਰ *ਖੰਡ_buf, blk_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
				     ਅੱਖਰ *ਖੰਡ_buf, blk64_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal,
				     ਅੱਖਰ *ਖੰਡ_buf, blk64_t *ret,
				     ਬਣਤਰ blk_alloc_ctx *ctx);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_alloc_ਖੰਡ_callback(ext2_filsys fs,
					    errcode_t (*func)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret),
					    errcode_t (**old)(ext2_filsys fs,
							      blk64_t goal,
							      blk64_t *ret));
blk64_t ext2fs_ਲੱਭੋ_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ਬਣਤਰ ext2_inode *inode, blk64_t lblk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen));
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_ਖੰਡ_alloc_stats_range_callback(ext2_filsys fs,
	ਖਾਲੀ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse),
	ਖਾਲੀ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NEWRANGE_FIXED_GOAL	(0x1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NEWRANGE_MIN_ਲੰਮਾਈ	(0x2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NEWRANGE_ALL_FLAGS		(0x3)
errcode_t ext2fs_new_range(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			   blk64_t len, ext2fs_ਖੰਡ_bitmap map, blk64_t *pblk,
			   blk64_t *plen);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ALLOCRANGE_FIXED_GOAL	(0x1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ALLOCRANGE_ZERO_ਖੰਡਾਂ	(0x2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ALLOCRANGE_ALL_FLAGS	(0x3)
errcode_t ext2fs_alloc_range(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			     blk_t len, blk64_t *ret);

/* alloc_sb.c */
ਬਾਹਰੀ ਪੂਰਨ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
					dgrp_t ਸਮੂਹ,
					ext2fs_ਖੰਡ_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
					       dgrp_t ਸਮੂਹ,
					       ext2fs_ਖੰਡ_bitmap bmap,
					       blk_t *desc_ਖੰਡਾਂ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_ਖੰਡ_alloc_stats_callback(ext2_filsys fs,
						  ਖਾਲੀ (*func)(ext2_filsys fs,
							       blk64_t blk,
							       ਪੂਰਨ inuse),
						  ਖਾਲੀ (**old)(ext2_filsys fs,
							       blk64_t blk,
							       ਪੂਰਨ inuse));

/* alloc_stats.c */
ਖਾਲੀ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ਪੂਰਨ inuse);
ਖਾਲੀ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ਪੂਰਨ inuse, ਪੂਰਨ isdir);
ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats(ext2_filsys fs, blk_t blk, ਪੂਰਨ inuse);
ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats2(ext2_filsys fs, blk64_t blk, ਪੂਰਨ inuse);
ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse);

/* alloc_tables.c */
ਬਾਹਰੀ errcode_t ext2fs_allocate_tables(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_allocate_ਸਮੂਹ_ਸਾਰਣੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					     ext2fs_ਖੰਡ_bitmap bmap);

/* badblocks.c */
ਬਾਹਰੀ errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_u32_list_ਜੋੜੋ(ext2_u32_list bb, __u32 blk);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_ਲੱਭੋ(ext2_u32_list bb, __u32 blk);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_test(ext2_u32_list bb, blk_t blk);
ਬਾਹਰੀ errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					       ext2_u32_iterate *ret);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_iterate(ext2_u32_iterate iter, blk_t *blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_u32_list_iterate_ਸਿਰਾ(ext2_u32_iterate iter);
ਬਾਹਰੀ errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2);

ਬਾਹਰੀ errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret,
					    ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_badblocks_list_ਜੋੜੋ(ext2_badblocks_list bb,
					   blk_t blk);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_badblocks_list_test(ext2_badblocks_list bb,
				    blk_t blk);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk);
ਬਾਹਰੀ errcode_t
	ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					    ext2_badblocks_iterate *ret);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter,
					 blk_t *blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_badblocks_list_iterate_ਸਿਰਾ(ext2_badblocks_iterate iter);
ਬਾਹਰੀ errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				       ext2_badblocks_list *dest);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_badblocks_equal(ext2_badblocks_list bb1,
				  ext2_badblocks_list bb2);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_u32_list_ਗਿਣਤੀ(ext2_u32_list bb);

/* bb_compat */
ਬਾਹਰੀ errcode_t badblocks_list_create(badblocks_list *ret, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t badblocks_list_ਜੋੜੋ(badblocks_list bb, blk_t blk);
ਬਾਹਰੀ ਪੂਰਨ badblocks_list_test(badblocks_list bb, blk_t blk);
ਬਾਹਰੀ errcode_t badblocks_list_iterate_begin(badblocks_list bb,
					      badblocks_iterate *ret);
ਬਾਹਰੀ ਪੂਰਨ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk);
ਬਾਹਰੀ ਖਾਲੀ badblocks_list_iterate_ਸਿਰਾ(badblocks_iterate iter);
ਬਾਹਰੀ ਖਾਲੀ badblocks_list_free(badblocks_list bb);

/* bb_inode.c */
ਬਾਹਰੀ errcode_t ext2fs_update_bb_inode(ext2_filsys fs,
					ext2_badblocks_list bb_list);

/* bitmaps.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
				    ext2fs_generic_bitmap *dest);
ਬਾਹਰੀ errcode_t ext2fs_allocate_ਖੰਡ_bitmap(ext2_filsys fs,
					      ਅਚਲ ਅੱਖਰ *descr,
					      ext2fs_ਖੰਡ_bitmap *ret);
ਬਾਹਰੀ errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
						   ਅਚਲ ਅੱਖਰ *descr,
						   ext2fs_ਖੰਡ_bitmap *ret);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_get_bitmap_granularity(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
					      ਅਚਲ ਅੱਖਰ *descr,
					      ext2fs_inode_bitmap *ret);
ਬਾਹਰੀ errcode_t ext2fs_fudge_inode_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t ਸਿਰਾ, ext2_ino_t *oend);
ਬਾਹਰੀ errcode_t ext2fs_fudge_ਖੰਡ_bitmap_ਸਿਰਾ(ext2fs_ਖੰਡ_bitmap bitmap,
					       blk_t ਸਿਰਾ, blk_t *oend);
ਬਾਹਰੀ errcode_t ext2fs_fudge_ਖੰਡ_bitmap_end2(ext2fs_ਖੰਡ_bitmap bitmap,
					 blk64_t ਸਿਰਾ, blk64_t *oend);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_clear_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_resize_inode_bitmap(__u32 new_ਸਿਰਾ, __u32 new_real_ਸਿਰਾ,
					    ext2fs_inode_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_resize_inode_bitmap2(__u64 new_ਸਿਰਾ,
					     __u64 new_real_ਸਿਰਾ,
					     ext2fs_inode_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_resize_ਖੰਡ_bitmap(__u32 new_ਸਿਰਾ, __u32 new_real_ਸਿਰਾ,
					    ext2fs_ਖੰਡ_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_resize_ਖੰਡ_bitmap2(__u64 new_ਸਿਰਾ,
					     __u64 new_real_ਸਿਰਾ,
					     ext2fs_ਖੰਡ_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_compare_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bm1,
					     ext2fs_ਖੰਡ_bitmap bm2);
ਬਾਹਰੀ errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
					     ext2fs_inode_bitmap bm2);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bmap,
					blk_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bmap,
					 blk64_t ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_get_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bmap,
					blk_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_get_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bmap,
					 blk64_t ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਬਾਹਰਲਾ);

/* blknum.c */
ਬਾਹਰੀ __u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ __u32 ext2fs_ਖੰਡ_bitmap_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_blk2(ext2_filsys fs, blk64_t);
ਬਾਹਰੀ blk64_t ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk64_t ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_ਸਮੂਹ_ਖੰਡਾਂ_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk64_t ext2fs_inode_ਅੰਕੜੇ_blocks2(ext2_filsys fs,
					 ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ blk64_t ext2fs_inode_i_ਖੰਡਾਂ(ext2_filsys fs,
				     ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ blk64_t ext2fs_get_stat_i_ਖੰਡਾਂ(ext2_filsys fs,
					ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ blk64_t ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
				    blk64_t blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
				    blk64_t blk);
ਬਾਹਰੀ blk64_t ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
				      blk64_t blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
				      blk64_t blk);
ਬਾਹਰੀ blk64_t ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
					 blk64_t blk);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super,
					 blk64_t blk);
/* Block group descriptor accessor functions */
ਬਾਹਰੀ ਬਣਤਰ ext2_ਸਮੂਹ_desc *ext2fs_ਸਮੂਹ_desc(ext2_filsys fs,
					  ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc *gdp,
					  dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk64_t ext2fs_ਖੰਡ_bitmap_csum(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk64_t ext2fs_ਖੰਡ_bitmap_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਖੰਡ_bitmap_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					blk64_t blk);
ਬਾਹਰੀ __u32 ext2fs_inode_bitmap_csum(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_inode_bitmap_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					blk64_t blk);
ਬਾਹਰੀ blk64_t ext2fs_inode_ਸਾਰਣੀ_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_inode_ਸਾਰਣੀ_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				       blk64_t blk);
ਬਾਹਰੀ __u32 ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					 __u32 n);
ਬਾਹਰੀ __u32 ext2fs_bg_free_inodes_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					 __u32 n);
ਬਾਹਰੀ __u32 ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				       __u32 n);
ਬਾਹਰੀ __u32 ext2fs_bg_itable_ਅਣਵਰਤਿਆ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				     __u32 n);
ਬਾਹਰੀ __u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flag);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_flags_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flags);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flags);
ਬਾਹਰੀ __u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_bg_checksum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 checksum);
ਬਾਹਰੀ blk64_t ext2fs_file_acl_ਖੰਡ(ext2_filsys fs,
				     ਅਚਲ ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_file_acl_ਖੰਡ_ਸੈੱਟ(ext2_filsys fs,
				      ਬਣਤਰ ext2_inode *inode, blk64_t blk);
ਬਾਹਰੀ errcode_t ext2fs_inode_ਮਾਪ_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				       ext2_off64_t ਮਾਪ);

/* block.c */
ਬਾਹਰੀ errcode_t ext2fs_ਖੰਡ_iterate(ext2_filsys fs,
				      ext2_ino_t	ino,
				      ਪੂਰਨ	flags,
				      ਅੱਖਰ *ਖੰਡ_buf,
				      ਪੂਰਨ (*func)(ext2_filsys fs,
						  blk_t	*blocknr,
						  ਪੂਰਨ	blockcnt,
						  ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				      ਖਾਲੀ *priv_ਅੰਕੜੇ);
errcode_t ext2fs_ਖੰਡ_iterate2(ext2_filsys fs,
				ext2_ino_t	ino,
				ਪੂਰਨ	flags,
				ਅੱਖਰ *ਖੰਡ_buf,
				ਪੂਰਨ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ਪੂਰਨ		ref_ਵਿੱਥ,
					    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				ਖਾਲੀ *priv_ਅੰਕੜੇ);
errcode_t ext2fs_ਖੰਡ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ਪੂਰਨ	flags,
				ਅੱਖਰ *ਖੰਡ_buf,
				ਪੂਰਨ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ਪੂਰਨ		ref_ਵਿੱਥ,
					    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				ਖਾਲੀ *priv_ਅੰਕੜੇ);

/* bmap.c */
ਬਾਹਰੀ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode *inode,
			     ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags,
			     blk_t ਖੰਡ, blk_t *phys_blk);
ਬਾਹਰੀ errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino,
			      ਬਣਤਰ ext2_inode *inode,
			      ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags, blk64_t ਖੰਡ,
			      ਪੂਰਨ *ret_flags, blk64_t *phys_blk);
errcode_t ext2fs_map_cluster_ਖੰਡ(ext2_filsys fs, ext2_ino_t ino,
				   ਬਣਤਰ ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk);

#ਜੇ 0
/* bmove.c */
ਬਾਹਰੀ errcode_t ext2fs_move_ਖੰਡਾਂ(ext2_filsys fs,
				    ext2fs_ਖੰਡ_bitmap reserve,
				    ext2fs_ਖੰਡ_bitmap alloc_map,
				    ਪੂਰਨ flags);
#ਜੇ_ਅੰਤ

/* check_desc.c */
ਬਾਹਰੀ errcode_t ext2fs_check_desc(ext2_filsys fs);

/* closefs.c */
ਬਾਹਰੀ errcode_t ext2fs_close(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_close2(ext2_filsys fs, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_close_free(ext2_filsys *fs);
ਬਾਹਰੀ errcode_t ext2fs_flush(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_flush2(ext2_filsys fs, ਪੂਰਨ flags);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t ਸਮੂਹ_ਖੰਡ);
ਬਾਹਰੀ errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
				    dgrp_t ਸਮੂਹ,
				    blk64_t *ret_super_blk,
				    blk64_t *ret_old_desc_blk,
				    blk64_t *ret_new_desc_blk,
				    blk_t *ret_ਵਰਤਿਆ_blks);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_super_and_bgd_loc(ext2_filsys fs,
				    dgrp_t ਸਮੂਹ,
				    blk_t *ret_super_blk,
				    blk_t *ret_old_desc_blk,
				    blk_t *ret_new_desc_blk,
				    ਪੂਰਨ *ret_meta_bg);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_update_dynamic_rev(ext2_filsys fs);

/* crc32c.c */
ਬਾਹਰੀ __u32 ext2fs_ਚੱਕਰੀ_ਜਾਂਚ_32_be(__u32 ਚੱਕਰੀ_ਜਾਂਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *p, size_t len);
ਬਾਹਰੀ __u32 ext2fs_crc32c_le(__u32 ਚੱਕਰੀ_ਜਾਂਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *p, size_t len);

/* csum.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_init_csum_ਬੀਜ(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_mmp_csum_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ mmp_struct *mmp);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_mmp_csum_verify(ext2_filsys, ਬਣਤਰ mmp_struct *mmp);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_verify_csum_type(ext2_filsys fs, ਬਣਤਰ ext2_super_ਖੰਡ *sb);
ਬਾਹਰੀ errcode_t ext2fs_superblock_csum_ਸੈੱਟ(ext2_filsys fs,
					    ਬਣਤਰ ext2_super_ਖੰਡ *sb);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_superblock_csum_verify(ext2_filsys fs,
					 ਬਣਤਰ ext2_super_ਖੰਡ *sb);
ਬਾਹਰੀ errcode_t ext2fs_ext_attr_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs,
					ext2_ino_t inum, blk64_t ਖੰਡ,
					ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *hdr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_ext_attr_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					     blk64_t ਖੰਡ,
					     ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *hdr);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRENT_ਪੂਛ(ਖੰਡ, blocksize) \
	((ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *)(((ਅੱਖਰ *)(ਖੰਡ)) + \
	(blocksize) - ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ)))

ਬਾਹਰੀ ਖਾਲੀ ext2fs_initialize_dirent_ਪੂਛ(ext2_filsys fs,
					  ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dirent_has_ਪੂਛ(ext2_filsys fs,
				  ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				     ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dir_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
					ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent);
ਬਾਹਰੀ errcode_t ext2fs_dir_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
					   ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent);
ਬਾਹਰੀ errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
					  ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					  ਬਣਤਰ ext2_dx_countlimit **cc,
					  ਪੂਰਨ *ਵਿੱਥ);
ਬਾਹਰੀ errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
				ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
				__u32 *ਚੱਕਰੀ_ਜਾਂਚ, ਬਣਤਰ ext2_dx_ਪੂਛ **ret_t);
ਬਾਹਰੀ errcode_t ext2fs_extent_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs,
					      ext2_ino_t inum,
					      ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_extent_ਖੰਡ_csum_verify(ext2_filsys fs,
					   ext2_ino_t inum,
					   ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh);
ਬਾਹਰੀ errcode_t ext2fs_ਖੰਡ_bitmap_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					      ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_ਖੰਡ_bitmap_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					   ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_inode_bitmap_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					      ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ,
					   ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_inode_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
				       ਬਣਤਰ ext2_inode_large *inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ਬਣਤਰ ext2_inode_large *inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_ਸਮੂਹ_desc_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_gdt_csum(ext2_filsys fs);
ਬਾਹਰੀ __u16 ext2fs_ਸਮੂਹ_desc_csum(ext2_filsys fs, dgrp_t ਸਮੂਹ);

/* dblist.c */
ਬਾਹਰੀ errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_dir_ਖੰਡ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ਪੂਰਨ blockcnt);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_dblist_ਛਾਂਟੋ(ext2_dblist dblist,
			       EXT2_QSORT_TYPE (*sortfunc)(ਅਚਲ ਖਾਲੀ *,
							   ਅਚਲ ਖਾਲੀ *));
ਬਾਹਰੀ ਖਾਲੀ ext2fs_dblist_sort2(ext2_dblist dblist,
				EXT2_QSORT_TYPE (*sortfunc)(ਅਚਲ ਖਾਲੀ *,
							    ਅਚਲ ਖਾਲੀ *));
ਬਾਹਰੀ errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
	ਪੂਰਨ (*func)(ext2_filsys fs, ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ *db_info,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
	ਖਾਲੀ *priv_ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
	ਪੂਰਨ (*func)(ext2_filsys fs, ਬਣਤਰ ext2_db_entry2 *db_info,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
	ਖਾਲੀ *priv_ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
	ਪੂਰਨ (*func)(ext2_filsys fs, ਬਣਤਰ ext2_db_entry2 *db_info,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਸ਼ੁਰੂ,
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2,
	ਖਾਲੀ *priv_ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_dir_ਖੰਡ(ext2_dblist dblist, ext2_ino_t ino,
				      blk_t blk, ਪੂਰਨ blockcnt);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				       blk64_t blk, e2_blkcnt_t blockcnt);
ਬਾਹਰੀ errcode_t ext2fs_copy_dblist(ext2_dblist src,
				    ext2_dblist *dest);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dblist_ਗਿਣਤੀ(ext2_dblist dblist);
ਬਾਹਰੀ blk64_t ext2fs_dblist_count2(ext2_dblist dblist);
ਬਾਹਰੀ errcode_t ext2fs_dblist_get_ਆਖਰੀ(ext2_dblist dblist,
					ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ **ਇੰਦਰਾਜ);
ਬਾਹਰੀ errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
					ਬਣਤਰ ext2_db_entry2 **ਇੰਦਰਾਜ);
ਬਾਹਰੀ errcode_t ext2fs_dblist_drop_ਆਖਰੀ(ext2_dblist dblist);

/* dblist_dir.c */
ਬਾਹਰੀ errcode_t
	ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				  ਪੂਰਨ	flags,
				  ਅੱਖਰ	*ਖੰਡ_buf,
				  ਪੂਰਨ (*func)(ext2_ino_t	dir,
					      ਪੂਰਨ		ਇੰਦਰਾਜ,
					      ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					      ਪੂਰਨ	ਵਿੱਥ,
					      ਪੂਰਨ	blocksize,
					      ਅੱਖਰ	*buf,
					      ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				  ਖਾਲੀ *priv_ਅੰਕੜੇ);

#ਜੇ 0
/* digest_encode.c */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_DIGEST_ਮਾਪ EXT2_FS_SHA256_ਲੰਮਾਈ
ਬਾਹਰੀ ਪੂਰਨ ext2fs_digest_encode(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ len, ਅੱਖਰ *dst);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_digest_decode(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ len, ਅੱਖਰ *dst);
#ਜੇ_ਅੰਤ

/* dirblock.c */
ਬਾਹਰੀ errcode_t ext2fs_read_dir_ਖੰਡ(ext2_filsys fs, blk_t ਖੰਡ,
				       ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ਖੰਡ,
					ਖਾਲੀ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ਖੰਡ,
					ਖਾਲੀ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ਖੰਡ,
					ਖਾਲੀ *buf, ਪੂਰਨ flags, ext2_ino_t ino);
ਬਾਹਰੀ errcode_t ext2fs_write_dir_ਖੰਡ(ext2_filsys fs, blk_t ਖੰਡ,
					ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ਖੰਡ,
					 ਖਾਲੀ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ਖੰਡ,
					 ਖਾਲੀ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ਖੰਡ,
					 ਖਾਲੀ *buf, ਪੂਰਨ flags, ext2_ino_t ino);

/* dirhash.c */
ਬਾਹਰੀ errcode_t ext2fs_dirhash(ਪੂਰਨ ਸੰਸਕਰਣ, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
				ਅਚਲ __u32 *ਬੀਜ,
				ext2_dirhash_t *ret_hash,
				ext2_dirhash_t *ret_minor_hash);

ਬਾਹਰੀ errcode_t ext2fs_dirhash2(ਪੂਰਨ ਸੰਸਕਰਣ, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
				 ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *charset,
				 ਪੂਰਨ hash_flags,
				 ਅਚਲ __u32 *ਬੀਜ,
				 ext2_dirhash_t *ret_hash,
				 ext2_dirhash_t *ret_minor_hash);

/* dir_iterate.c */
ਬਾਹਰੀ errcode_t ext2fs_get_rec_len(ext2_filsys fs,
				    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *rec_len);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_rec_len(ext2_filsys fs,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
				    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent);
ਬਾਹਰੀ errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			      ext2_ino_t dir,
			      ਪੂਰਨ flags,
			      ਅੱਖਰ *ਖੰਡ_buf,
			      ਪੂਰਨ (*func)(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					  ਪੂਰਨ	ਵਿੱਥ,
					  ਪੂਰਨ	blocksize,
					  ਅੱਖਰ	*buf,
					  ਖਾਲੀ	*priv_ਅੰਕੜੇ),
			      ਖਾਲੀ *priv_ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ਪੂਰਨ flags,
			      ਅੱਖਰ *ਖੰਡ_buf,
			      ਪੂਰਨ (*func)(ext2_ino_t	dir,
					  ਪੂਰਨ	ਇੰਦਰਾਜ,
					  ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					  ਪੂਰਨ	ਵਿੱਥ,
					  ਪੂਰਨ	blocksize,
					  ਅੱਖਰ	*buf,
					  ਖਾਲੀ	*priv_ਅੰਕੜੇ),
			      ਖਾਲੀ *priv_ਅੰਕੜੇ);

/* dupfs.c */
ਬਾਹਰੀ errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest);

/* expanddir.c */
ਬਾਹਰੀ errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir);

/* ext_attr.c */
ਬਾਹਰੀ __u32 ext2fs_ext_attr_hash_ਇੰਦਰਾਜ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					ਖਾਲੀ *ਅੰਕੜੇ);
ਬਾਹਰੀ __u32 ext2fs_ext_attr_hash_ਇੰਦਰਾਜ_signed(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					       ਖਾਲੀ *ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
					     ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					     ਖਾਲੀ *ਅੰਕੜੇ, __u32 *hash);
ਬਾਹਰੀ errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
					     ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					     ਖਾਲੀ *ਅੰਕੜੇ, __u32 *hash,
					     __u32 *signed_hash);
ਬਾਹਰੀ errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ਖੰਡ, ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ਖੰਡ,
				       ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ਖੰਡ,
				       ਖਾਲੀ *buf, ext2_ino_t inum);
ਬਾਹਰੀ errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ਖੰਡ,
				       ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ਖੰਡ,
				       ਖਾਲੀ *buf);
ਬਾਹਰੀ errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ਖੰਡ,
				       ਖਾਲੀ *buf, ext2_ino_t inum);
ਬਾਹਰੀ errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					   ਅੱਖਰ *ਖੰਡ_buf,
					   ਪੂਰਨ adjust, __u32 *newcount);
ਬਾਹਰੀ errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
					   ਅੱਖਰ *ਖੰਡ_buf,
					   ਪੂਰਨ adjust, __u32 *newcount);
ਬਾਹਰੀ errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
					   ਅੱਖਰ *ਖੰਡ_buf,
					   ਪੂਰਨ adjust, __u32 *newcount,
					   ext2_ino_t inum);
errcode_t ext2fs_xattrs_write(ਬਣਤਰ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read(ਬਣਤਰ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_read_inode(ਬਣਤਰ ext2_xattr_handle *handle,
				   ਬਣਤਰ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_iterate(ਬਣਤਰ ext2_xattr_handle *h,
				ਪੂਰਨ (*func)(ਅੱਖਰ *name, ਅੱਖਰ *ਮੁੱਲ,
					    size_t ਮੁੱਲ_len, ਖਾਲੀ *ਅੰਕੜੇ),
				ਖਾਲੀ *ਅੰਕੜੇ);
errcode_t ext2fs_xattr_get(ਬਣਤਰ ext2_xattr_handle *h, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
			   ਖਾਲੀ **ਮੁੱਲ, size_t *ਮੁੱਲ_len);
errcode_t ext2fs_xattr_ਸੈੱਟ(ਬਣਤਰ ext2_xattr_handle *handle,
			   ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
			   ਅਚਲ ਖਾਲੀ *ਮੁੱਲ,
			   size_t ਮੁੱਲ_len);
errcode_t ext2fs_xattr_remove(ਬਣਤਰ ext2_xattr_handle *handle,
			      ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);
errcode_t ext2fs_xattr_remove_all(ਬਣਤਰ ext2_xattr_handle *handle);
errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_xattr_handle **handle);
errcode_t ext2fs_xattrs_close(ਬਣਤਰ ext2_xattr_handle **handle);
errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ਬਣਤਰ ext2_inode_large *inode);
errcode_t ext2fs_xattrs_ਗਿਣਤੀ(ਬਣਤਰ ext2_xattr_handle *handle, size_t *ਗਿਣਤੀ_2);
errcode_t ext2fs_xattr_inode_max_ਮਾਪ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ਮਾਪ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XATTR_HANDLE_FLAG_RAW	0x0001
errcode_t ext2fs_xattrs_flags(ਬਣਤਰ ext2_xattr_handle *handle,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *new_flags, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *old_flags);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ext_attr_ਖੰਡ_rehash(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ,
					 ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਸਿਰਾ);
ਬਾਹਰੀ __u32 ext2fs_get_ea_inode_hash(ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_ea_inode_hash(ਬਣਤਰ ext2_inode *inode, __u32 hash);
ਬਾਹਰੀ __u64 ext2fs_get_ea_inode_ref(ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_ea_inode_ref(ਬਣਤਰ ext2_inode *inode, __u64 ref_ਗਿਣਤੀ);

/* extent.c */
ਬਾਹਰੀ errcode_t ext2fs_extent_ਸਿਰਲੇਖ_verify(ਖਾਲੀ *ptr, ਪੂਰਨ ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *handle);
ਬਾਹਰੀ errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
					ਬਣਤਰ ext2_inode *inode,
					ext2_extent_handle_t *ret_handle);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_extent_free(ext2_extent_handle_t handle);
ਬਾਹਰੀ errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
				   ਪੂਰਨ flags, ਬਣਤਰ ext2fs_extent *extent);
ਬਾਹਰੀ errcode_t ext2fs_extent_ਗੰਢ_ਵੰਡੋ(ext2_extent_handle_t handle);
ਬਾਹਰੀ errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle, ਪੂਰਨ flags,
				       ਬਣਤਰ ext2fs_extent *extent);
ਬਾਹਰੀ errcode_t ext2fs_extent_ਪਾਓ(ext2_extent_handle_t handle, ਪੂਰਨ flags,
				      ਬਣਤਰ ext2fs_extent *extent);
ਬਾਹਰੀ errcode_t ext2fs_extent_ਸੈੱਟ_bmap(ext2_extent_handle_t handle,
					blk64_t logical, blk64_t physical,
					ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
					ਬਣਤਰ ext2_extent_info *info);
ਬਾਹਰੀ errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
				    blk64_t blk);
ਬਾਹਰੀ errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
				     ਪੂਰਨ ਪੱਤੀ_level, blk64_t blk);
ਬਾਹਰੀ errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle);
ਬਾਹਰੀ size_t ext2fs_max_extent_ਡੂੰਘਾਈ(ext2_extent_handle_t handle);
ਬਾਹਰੀ errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
					      ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ errcode_t ext2fs_ਗਿਣਤੀ_ਖੰਡਾਂ(ext2_filsys fs, ext2_ino_t ino,
				     ਬਣਤਰ ext2_inode *inode, blk64_t *ret_ਗਿਣਤੀ);
ਬਾਹਰੀ errcode_t ext2fs_decode_extent(ਬਣਤਰ ext2fs_extent *ਮੰਜ਼ਿਲ, ਖਾਲੀ *from,
				      ਪੂਰਨ len);

/* fallocate.c */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ	(0x1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FALLOCATE_FORCE_INIT	(0x2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FALLOCATE_FORCE_UNINIT	(0x4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FALLOCATE_INIT_BEYOND_EOF	(0x8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FALLOCATE_ALL_FLAGS	(0xF)
errcode_t ext2fs_fallocate(ext2_filsys fs, ਪੂਰਨ flags, ext2_ino_t ino,
			   ਬਣਤਰ ext2_inode *inode, blk64_t goal,
			   blk64_t ਸ਼ੁਰੂ, blk64_t len);

/* fileio.c */
ਬਾਹਰੀ errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
				   ਬਣਤਰ ext2_inode *inode,
				   ਪੂਰਨ flags, ext2_file_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
				  ਪੂਰਨ flags, ext2_file_t *ret);
ਬਾਹਰੀ ext2_filsys ext2fs_file_get_fs(ext2_file_t file);
ਬਣਤਰ ext2_inode *ext2fs_file_get_inode(ext2_file_t file);
ਬਾਹਰੀ ext2_ino_t ext2fs_file_get_inode_ਅੰਕ(ext2_file_t file);
ਬਾਹਰੀ errcode_t ext2fs_file_close(ext2_file_t file);
ਬਾਹਰੀ errcode_t ext2fs_file_flush(ext2_file_t file);
ਬਾਹਰੀ errcode_t ext2fs_file_read(ext2_file_t file, ਖਾਲੀ *buf,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ wanted, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *got);
ਬਾਹਰੀ errcode_t ext2fs_file_write(ext2_file_t file, ਅਚਲ ਖਾਲੀ *buf,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nbytes, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *written);
ਬਾਹਰੀ errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 ਵਿੱਥ,
				   ਪੂਰਨ whence, __u64 *ret_pos);
ਬਾਹਰੀ errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t ਵਿੱਥ,
				   ਪੂਰਨ whence, ext2_off_t *ret_pos);
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ਮਾਪ);
ਬਾਹਰੀ ext2_off_t ext2fs_file_get_ਮਾਪ(ext2_file_t file);
ਬਾਹਰੀ errcode_t ext2fs_file_ਸੈੱਟ_ਮਾਪ(ext2_file_t file, ext2_off_t ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_file_ਸੈੱਟ_size2(ext2_file_t file, ext2_off64_t ਮਾਪ);

/* finddev.c */
ਬਾਹਰੀ ਅੱਖਰ *ext2fs_ਲੱਭੋ_ਖੰਡ_device(dev_t device);

/* flushb.c */
ਬਾਹਰੀ errcode_t ext2fs_sync_device(ਪੂਰਨ fd, ਪੂਰਨ flushb);

/* freefs.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_dblist(ext2_dblist dblist);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_badblocks_list_free(ext2_badblocks_list bb);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_u32_list_free(ext2_u32_list bb);

/* gen_bitmap.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
					    __u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ,
					    __u32 real_ਸਿਰਾ,
					    ਅਚਲ ਅੱਖਰ *descr, ਅੱਖਰ *init_map,
					    ext2fs_generic_bitmap *ret);
ਬਾਹਰੀ errcode_t ext2fs_allocate_generic_bitmap(__u32 ਸ਼ੁਰੂ,
						__u32 ਸਿਰਾ,
						__u32 real_ਸਿਰਾ,
						ਅਚਲ ਅੱਖਰ *descr,
						ext2fs_generic_bitmap *ret);
ਬਾਹਰੀ errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap src,
					    ext2fs_generic_bitmap *dest);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_fudge_generic_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap bitmap,
						 errcode_t magic,
						 errcode_t neq,
						 ext2_ino_t ਸਿਰਾ,
						 ext2_ino_t *oend);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_generic_bitmap_padding(ext2fs_generic_bitmap map);
ਬਾਹਰੀ errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
					      __u32 new_ਸਿਰਾ,
					      __u32 new_real_ਸਿਰਾ,
					      ext2fs_generic_bitmap bmap);
ਬਾਹਰੀ errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					       ext2fs_generic_bitmap bm1,
					       ext2fs_generic_bitmap bm2);
ਬਾਹਰੀ errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ਸ਼ੁਰੂ, __u32 ਅੰਕ,
						 ਖਾਲੀ *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_generic_bitmap_range(ext2fs_generic_bitmap bmap,
						 errcode_t magic,
						 __u32 ਸ਼ੁਰੂ, __u32 ਅੰਕ,
						 ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ,
						       __u32 *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bitmap(ext2fs_generic_bitmap bitmap,
						       __u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ,
						       __u32 *ਬਾਹਰਲਾ);

/* gen_bitmap64.c */
ਖਾਲੀ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ਪੂਰਨ type, __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ,
				    __u64 real_ਸਿਰਾ,
				    ਅਚਲ ਅੱਖਰ *descr,
				    ext2fs_generic_bitmap *ret);
errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
				   ext2fs_generic_bitmap *dest);
ਖਾਲੀ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
errcode_t ext2fs_fudge_generic_bmap_ਸਿਰਾ(ext2fs_generic_bitmap bitmap,
					errcode_t neq,
					__u64 ਸਿਰਾ, __u64 *oend);
ਖਾਲੀ ext2fs_ਸੈੱਟ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
				     __u64 new_ਸਿਰਾ,
				     __u64 new_real_ਸਿਰਾ);
errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap bm1,
				      ext2fs_generic_bitmap bm2);
errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ);
errcode_t ext2fs_ਸੈੱਟ_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ);
errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ਖੰਡ_bitmap *bitmap);
errcode_t ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_clusters(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ,
				     blk64_t ਸਿਰਾ, blk64_t *ਬਾਹਰਲਾ);
errcode_t ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_ਖੰਡਾਂ(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ,
				   blk64_t ਸਿਰਾ, blk64_t *ਬਾਹਰਲਾ);
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_list_backups(ext2_filsys fs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *three,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *five, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *seven);

/* getenv.c */
ਬਾਹਰੀ ਅੱਖਰ *ext2fs_ਸੁਰੱਖਿਅਤ_getenv(ਅਚਲ ਅੱਖਰ *arg);

/* get_num_dirs.c */
ਬਾਹਰੀ errcode_t ext2fs_get_ਅੰਕ_dirs(ext2_filsys fs, ext2_ino_t *ret_ਅੰਕ_dirs);

/* getsize.c */
ਬਾਹਰੀ errcode_t ext2fs_get_device_ਮਾਪ(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
					blk_t *retblocks);
ਬਾਹਰੀ errcode_t ext2fs_get_device_size2(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
					blk64_t *retblocks);

/* getsectsize.c */
ਬਾਹਰੀ ਪੂਰਨ ext2fs_get_dio_alignment(ਪੂਰਨ fd);
errcode_t ext2fs_get_device_sectsize(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *sectsize);
errcode_t ext2fs_get_device_phys_sectsize(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *sectsize);

/* i_block.c */
errcode_t ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				 blk64_t ਅੰਕ_ਖੰਡਾਂ);
errcode_t ext2fs_iblk_sub_ਖੰਡਾਂ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				 blk64_t ਅੰਕ_ਖੰਡਾਂ);
errcode_t ext2fs_iblk_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode, blk64_t b);

/* imager.c */
ਬਾਹਰੀ errcode_t ext2fs_image_inode_write(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_image_inode_read(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_image_super_write(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_image_super_read(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags);

/* ind_block.c */
errcode_t ext2fs_read_ind_ਖੰਡ(ext2_filsys fs, blk_t blk, ਖਾਲੀ *buf);
errcode_t ext2fs_write_ind_ਖੰਡ(ext2_filsys fs, blk_t blk, ਖਾਲੀ *buf);

/* initialize.c */
ਬਾਹਰੀ errcode_t ext2fs_initialize(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags,
				   ਬਣਤਰ ext2_super_ਖੰਡ *param,
				   io_manager manager, ext2_filsys *ret_fs);
ਬਾਹਰੀ errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ਪੂਰਨ super_only);

/* icount.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_icount(ext2_icount_t icount);
ਬਾਹਰੀ errcode_t ext2fs_create_icount_tdb(ext2_filsys fs, ਅੱਖਰ *tdb_dir,
					  ਪੂਰਨ flags, ext2_icount_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_create_icount2(ext2_filsys fs, ਪੂਰਨ flags,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
				       ext2_icount_t hint, ext2_icount_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_create_icount(ext2_filsys fs, ਪੂਰਨ flags,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
				      ext2_icount_t *ret);
ਬਾਹਰੀ errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 *ret);
ਬਾਹਰੀ errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ਬਾਹਰੀ errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
					 __u16 *ret);
ਬਾਹਰੀ errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
				     __u16 ਗਿਣਤੀ_2);
ਬਾਹਰੀ ext2_ino_t ext2fs_get_icount_ਮਾਪ(ext2_icount_t icount);
errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *);

/* inline.c */

ਬਾਹਰੀ errcode_t ext2fs_get_memalign(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ align, ਖਾਲੀ *ptr);

/* inline_data.c */
ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_init(ext2_filsys fs, ext2_ino_t ino);
ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_ਮਾਪ(ext2_filsys fs, ext2_ino_t ino,
					 size_t *ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_get(ext2_filsys fs, ext2_ino_t ino,
					ਬਣਤਰ ext2_inode *inode,
					ਖਾਲੀ *buf, size_t *ਮਾਪ);
ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t ino,
					ਬਣਤਰ ext2_inode *inode,
					ਖਾਲੀ *buf, size_t ਮਾਪ);

/* inode.c */
ਬਾਹਰੀ errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cache_ਮਾਪ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_inode_cache(ਬਣਤਰ ext2_inode_cache *icache);
ਬਾਹਰੀ errcode_t ext2fs_flush_icache(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_get_ਅਗਲਾ_inode_ਭਰਿਆ(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
					    ext2_ino_t *ino,
					    ਬਣਤਰ ext2_inode *inode,
					    ਪੂਰਨ bufsize);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_INODE_ਜਾਂਚ_DEFAULT_ਬਫਰ_ਖੰਡਾਂ	8
ਬਾਹਰੀ errcode_t ext2fs_open_inode_ਜਾਂਚ(ext2_filsys fs, ਪੂਰਨ ਬਫਰ_ਖੰਡਾਂ,
				  ext2_inode_ਜਾਂਚ *ret_ਜਾਂਚ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_close_inode_ਜਾਂਚ(ext2_inode_ਜਾਂਚ ਜਾਂਚ);
ਬਾਹਰੀ errcode_t ext2fs_get_ਅਗਲਾ_inode(ext2_inode_ਜਾਂਚ ਜਾਂਚ, ext2_ino_t *ino,
			       ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ errcode_t ext2fs_inode_ਜਾਂਚ_goto_blockgroup(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
						   ਪੂਰਨ	ਸਮੂਹ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_inode_callback
	(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
	 errcode_t (*ਪੂਰਾ_ਸਮੂਹ)(ext2_filsys fs,
				 ext2_inode_ਜਾਂਚ ਜਾਂਚ,
				 dgrp_t ਸਮੂਹ,
				 ਖਾਲੀ * priv_ਅੰਕੜੇ),
	 ਖਾਲੀ *ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inode_ਜਾਂਚ_flags(ext2_inode_ਜਾਂਚ ਜਾਂਚ, ਪੂਰਨ ਸੈੱਟ_flags,
				   ਪੂਰਨ clear_flags);
ਬਾਹਰੀ errcode_t ext2fs_read_inode_ਭਰਿਆ(ext2_filsys fs, ext2_ino_t ino,
					ਬਣਤਰ ext2_inode * inode,
					ਪੂਰਨ bufsize);
ਬਾਹਰੀ errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ਬਣਤਰ ext2_inode * inode);
ਬਾਹਰੀ errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
				    ਬਣਤਰ ext2_inode * inode,
				    ਪੂਰਨ bufsize, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_write_inode_ਭਰਿਆ(ext2_filsys fs, ext2_ino_t ino,
					 ਬਣਤਰ ext2_inode * inode,
					 ਪੂਰਨ bufsize);
ਬਾਹਰੀ errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			    ਬਣਤਰ ext2_inode * inode);
ਬਾਹਰੀ errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
				     ਬਣਤਰ ext2_inode * inode,
				     ਪੂਰਨ bufsize, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
			    ਬਣਤਰ ext2_inode * inode);
ਬਾਹਰੀ errcode_t ext2fs_get_ਖੰਡਾਂ(ext2_filsys fs, ext2_ino_t ino, blk_t *ਖੰਡਾਂ);
ਬਾਹਰੀ errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino);

/* inode_io.c */
ਬਾਹਰੀ io_manager inode_io_manager;
ਬਾਹਰੀ errcode_t ext2fs_inode_io_intern(ext2_filsys fs, ext2_ino_t ino,
					ਅੱਖਰ **name);
ਬਾਹਰੀ errcode_t ext2fs_inode_io_intern2(ext2_filsys fs, ext2_ino_t ino,
					 ਬਣਤਰ ext2_inode *inode,
					 ਅੱਖਰ **name);

/* ismounted.c */
ਬਾਹਰੀ errcode_t ext2fs_check_if_mounted(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *mount_flags);
ਬਾਹਰੀ errcode_t ext2fs_check_mount_ਬਿੰਦੂ(ਅਚਲ ਅੱਖਰ *device, ਪੂਰਨ *mount_flags,
					  ਅੱਖਰ *mtpt, ਪੂਰਨ mtlen);

/* punch.c */
/*
 * NOTE: This function removes from an inode the blocks "start", "end", and
 * every block in between.
 */
ਬਾਹਰੀ errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
			      ਬਣਤਰ ext2_inode *inode,
			      ਅੱਖਰ *ਖੰਡ_buf, blk64_t ਸ਼ੁਰੂ,
			      blk64_t ਸਿਰਾ);

/* namei.c */
ਬਾਹਰੀ errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ਅਚਲ ਅੱਖਰ *name,
			 ਪੂਰਨ namelen, ਅੱਖਰ *buf, ext2_ino_t *inode);
ਬਾਹਰੀ errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
			ਅਚਲ ਅੱਖਰ *name, ext2_ino_t *inode);
errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
			      ਅਚਲ ਅੱਖਰ *name, ext2_ino_t *inode);
ਬਾਹਰੀ errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode);

/* native.c */
ਪੂਰਨ ext2fs_native_flag(ਖਾਲੀ);

/* newdir.c */
ਬਾਹਰੀ errcode_t ext2fs_new_dir_ਖੰਡ(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ਮਾਪਾ_ino, ਅੱਖਰ **ਖੰਡ);
ਬਾਹਰੀ errcode_t ext2fs_new_dir_inline_ਅੰਕੜੇ(ext2_filsys fs, ext2_ino_t dir_ino,
				ext2_ino_t ਮਾਪਾ_ino, __u32 *iblock);

/* nls_utf8.c */
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ext2fs_ਲੱਦੋ_nls_ਸਾਰਣੀ(ਪੂਰਨ encoding);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_check_encoded_name(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
				     ਅੱਖਰ *s, size_t len, ਅੱਖਰ **pos);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_casefold_cmp(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			       ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str1, size_t len1,
			       ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str2, size_t len2);

/* mkdir.c */
ਬਾਹਰੀ errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t ਮਾਪਾ, ext2_ino_t inum,
			      ਅਚਲ ਅੱਖਰ *name);
ਬਾਹਰੀ errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t ਮਾਪਾ,
			       ext2_ino_t ino, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags,
			       ਪੂਰਨ link_flags, ਅਚਲ ਅੱਖਰ *name,
			       ext2_ino_t *ret_ino);

/* mkjournal.c */
ਬਣਤਰ ext2fs_journal_params {
	blk_t ਅੰਕ_journal_ਖੰਡਾਂ;
	blk_t ਅੰਕ_fc_ਖੰਡਾਂ;
};
ਬਾਹਰੀ errcode_t ext2fs_get_journal_params(
		ਬਣਤਰ ext2fs_journal_params *params, ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_zero_ਖੰਡਾਂ(ext2_filsys fs, blk_t blk, ਪੂਰਨ ਅੰਕ,
				    blk_t *ret_blk, ਪੂਰਨ *ret_ਗਿਣਤੀ);
ਬਾਹਰੀ errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ਪੂਰਨ ਅੰਕ,
				     blk64_t *ret_blk, ਪੂਰਨ *ret_ਗਿਣਤੀ);
ਬਾਹਰੀ errcode_t ext2fs_create_journal_superblock(ext2_filsys fs,
						  __u32 ਅੰਕ_ਖੰਡਾਂ, ਪੂਰਨ flags,
						  ਅੱਖਰ  **ret_jsb);
ਬਾਹਰੀ errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
						  ਬਣਤਰ ext2fs_journal_params *params,
						  ਪੂਰਨ flags, ਅੱਖਰ  **ret_jsb);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_journal_device(ext2_filsys fs,
					   ext2_filsys journal_dev);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_journal_inode(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ,
					  ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_journal_inode2(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ,
					   blk64_t goal, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_ਜੋੜੋ_journal_inode3(ext2_filsys fs,
				    ਬਣਤਰ ext2fs_journal_params *params,
				    blk64_t goal, ਪੂਰਨ flags);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_default_journal_ਮਾਪ(__u64 ਅੰਕ_ਖੰਡਾਂ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_journal_sb_ਸ਼ੁਰੂ(ਪੂਰਨ blocksize);

/* openfs.c */
ਬਾਹਰੀ errcode_t ext2fs_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, ਪੂਰਨ superblock,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ_ਮਾਪ, io_manager manager,
			     ext2_filsys *ret_fs);
ਬਾਹਰੀ errcode_t ext2fs_open2(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *io_options,
			      ਪੂਰਨ flags, ਪੂਰਨ superblock,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ_ਮਾਪ, io_manager manager,
			      ext2_filsys *ret_fs);
/*
 * The dgrp_t argument to these two functions is not actually a group number
 * but a block number offset within a group table!  Convert with the formula
 * (group_number / groups_per_block).
 */
ਬਾਹਰੀ blk64_t ext2fs_descriptor_ਖੰਡ_loc2(ext2_filsys fs,
					blk64_t ਸਮੂਹ_ਖੰਡ, dgrp_t i);
ਬਾਹਰੀ blk_t ext2fs_descriptor_ਖੰਡ_loc(ext2_filsys fs, blk_t ਸਮੂਹ_ਖੰਡ,
					 dgrp_t i);
errcode_t ext2fs_get_ਅੰਕੜੇ_io(ext2_filsys fs, io_channel *old_io);
errcode_t ext2fs_ਸੈੱਟ_ਅੰਕੜੇ_io(ext2_filsys fs, io_channel new_io);
errcode_t ext2fs_rewrite_ਮੰਜ਼ਿਲ_io(ext2_filsys fs, io_channel new_io);

/* orphan.c */
ਬਾਹਰੀ errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ);
ਬਾਹਰੀ errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs);
ਬਾਹਰੀ e2_blkcnt_t ext2fs_default_orphan_file_ਖੰਡਾਂ(ext2_filsys fs);
ਬਾਹਰੀ __u32 ext2fs_do_orphan_file_ਖੰਡ_csum(ext2_filsys fs, ext2_ino_t ino,
					      __u32 gen, blk64_t blk,
					      ਅੱਖਰ *buf);
ਬਾਹਰੀ errcode_t ext2fs_orphan_file_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs,
						   ext2_ino_t ino, blk64_t blk,
						   ਅੱਖਰ *buf);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_orphan_file_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
						blk64_t blk, ਅੱਖਰ *buf);

/* get_pathname.c */
ਬਾਹਰੀ errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			       ਅੱਖਰ **name);

/* link.c */
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ਅਚਲ ਅੱਖਰ *name,
		      ext2_ino_t ino, ਪੂਰਨ flags);
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir, ਅਚਲ ਅੱਖਰ *name,
			ext2_ino_t ino, ਪੂਰਨ flags);
ਪੂਰਨ ext2fs_dir_is_dx(ext2_filsys fs, ਅਚਲ ਬਣਤਰ ext2_inode *inode);
ਖਾਲੀ ext2fs_inc_nlink(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode);
ਖਾਲੀ ext2fs_dec_nlink(ਬਣਤਰ ext2_inode *inode);
ਪੂਰਨ ext2fs_dir_link_max(ext2_filsys fs, ਬਣਤਰ ext2_inode_large *inode);
ਪੂਰਨ ext2fs_dir_link_ਖਾਲੀ_ਥਾਵਾਂ(ਬਣਤਰ ext2_inode *inode);

/* symlink.c */
errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ਮਾਪਾ, ext2_ino_t ino,
			 ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ);
ਪੂਰਨ ext2fs_is_fast_symlink(ਬਣਤਰ ext2_inode *inode);

/* mmp.c */
errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ਖਾਲੀ *buf);
errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ਖਾਲੀ *buf);
errcode_t ext2fs_mmp_clear(ext2_filsys fs);
errcode_t ext2fs_mmp_init(ext2_filsys fs);
errcode_t ext2fs_mmp_ਸ਼ੁਰੂ(ext2_filsys fs);
errcode_t ext2fs_mmp_update(ext2_filsys fs);
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ਪੂਰਨ immediately);
errcode_t ext2fs_mmp_stop(ext2_filsys fs);
ਬਿਨਾਨਿਸ਼ਾਨੀ ext2fs_mmp_new_seq(ਖਾਲੀ);
errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ਖਾਲੀ **ptr);

/* read_bb.c */
ਬਾਹਰੀ errcode_t ext2fs_read_bb_inode(ext2_filsys fs,
				      ext2_badblocks_list *bb_list);

/* read_bb_file.c */
ਬਾਹਰੀ errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
				      ext2_badblocks_list *bb_list,
				      ਖਾਲੀ *priv_ਅੰਕੜੇ,
				      ਖਾਲੀ (*invalid)(ext2_filsys fs,
						      blk_t blk,
						      ਅੱਖਰ *badstr,
						      ਖਾਲੀ *priv_ਅੰਕੜੇ));
ਬਾਹਰੀ errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
				     ext2_badblocks_list *bb_list,
				     ਖਾਲੀ (*invalid)(ext2_filsys fs,
						     blk_t blk));

/* res_gdt.c */
ਬਾਹਰੀ errcode_t ext2fs_create_resize_inode(ext2_filsys fs);

/* rw_bitmaps.c */
ਬਾਹਰੀ errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ਪੂਰਨ flags, ਪੂਰਨ ਅੰਕ_ਧਾਗੇ);
ਬਾਹਰੀ errcode_t ext2fs_read_bitmaps(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_read_inode_bitmap (ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_read_ਖੰਡ_bitmap(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_write_bitmaps(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs);
ਬਾਹਰੀ errcode_t ext2fs_write_ਖੰਡ_bitmap (ext2_filsys fs);

/*sha256.c */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_SHA256_ਲੰਮਾਈ 32
#ਜੇ 0
ਬਾਹਰੀ ਖਾਲੀ ext2fs_sha256(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਚ_ਮਾਪ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਾਹਰਲਾ[EXT2_FS_SHA256_ਲੰਮਾਈ]);
#ਜੇ_ਅੰਤ

/* sha512.c */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_SHA512_ਲੰਮਾਈ 64
ਬਾਹਰੀ ਖਾਲੀ ext2fs_sha512(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਚ_ਮਾਪ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਾਹਰਲਾ[EXT2_FS_SHA512_ਲੰਮਾਈ]);

/* swapfs.c */
ਬਾਹਰੀ errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, ਅੱਖਰ *buf, size_t ਮਾਪ,
					ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_dirent_swab_ਵਿੱਚ(ext2_filsys fs, ਅੱਖਰ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, ਅੱਖਰ *buf, size_t ਮਾਪ,
					 ਪੂਰਨ flags);
ਬਾਹਰੀ errcode_t ext2fs_dirent_swab_ਬਾਹਰਲਾ(ext2_filsys fs, ਅੱਖਰ *buf, ਪੂਰਨ flags);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr(ਅੱਖਰ *ਮੰਜ਼ਿਲ, ਅੱਖਰ *from, ਪੂਰਨ bufsize,
				 ਪੂਰਨ has_ਸਿਰਲੇਖ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਸਿਰਲੇਖ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ,
					ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *from_hdr);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਇੰਦਰਾਜ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ,
				       ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *from_ਇੰਦਰਾਜ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(ਬਣਤਰ ext2_super_ਖੰਡ * super);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc(ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(ext2_filsys, ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(ext2_filsys fs, ਬਣਤਰ ext2_inode_large *t,
				   ਬਣਤਰ ext2_inode_large *f, ਪੂਰਨ hostorder,
				   ਪੂਰਨ bufsize);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode(ext2_filsys fs,ਬਣਤਰ ext2_inode *t,
			      ਬਣਤਰ ext2_inode *f, ਪੂਰਨ hostorder);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_mmp(ਬਣਤਰ mmp_struct *mmp);

/* unix_io.c */
ਬਾਹਰੀ ਪੂਰਨ ext2fs_open_file(ਅਚਲ ਅੱਖਰ *pathname, ਪੂਰਨ flags, mode_t ਬਹੁਲਕ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_stat(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ext2fs_struct_stat *buf);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fstat(ਪੂਰਨ fd, ext2fs_struct_stat *buf);

/* valid_blk.c */
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inode_has_valid_ਖੰਡਾਂ(ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inode_has_valid_blocks2(ext2_filsys fs,
					  ਬਣਤਰ ext2_inode *inode);

/* version.c */
ਬਾਹਰੀ ਪੂਰਨ ext2fs_parse_ਸੰਸਕਰਣ_string(ਅਚਲ ਅੱਖਰ *ver_string);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_get_library_ਸੰਸਕਰਣ(ਅਚਲ ਅੱਖਰ **ver_string,
				      ਅਚਲ ਅੱਖਰ **date_string);

/* write_bb_file.c */
ਬਾਹਰੀ errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags,
				      FILE *f);


/* inline functions */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_INLINE_FUNCS
ਬਾਹਰੀ errcode_t ext2fs_get_mem(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_get_memzero(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_get_array(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_get_arrayzero(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_free_mem(ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_resize_mem(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ old_ਮਾਪ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t ext2fs_resize_array(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ old_ਗਿਣਤੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_super_dirty(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_ਬਦਲਿਆ(ext2_filsys fs);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਬਦਲਿਆ(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_valid(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_unmark_valid(ext2_filsys fs);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_valid(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_ib_dirty(ext2_filsys fs);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_bb_dirty(ext2_filsys fs);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ib_dirty(ext2_filsys fs);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_bb_dirty(ext2_filsys fs);
ਬਾਹਰੀ dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_blk(ext2_filsys fs, blk_t blk);
ਬਾਹਰੀ dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_ino(ext2_filsys fs, ext2_ino_t ino);
ਬਾਹਰੀ blk_t ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_ਖੰਡ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk_t ext2fs_ਸਮੂਹ_ਆਖਰੀ_ਖੰਡ(ext2_filsys fs, dgrp_t ਸਮੂਹ);
ਬਾਹਰੀ blk_t ext2fs_inode_ਅੰਕੜੇ_ਖੰਡਾਂ(ext2_filsys fs,
				      ਬਣਤਰ ext2_inode *inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ਪੂਰਨ ਖੰਡਾਂ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_log2_u32(__u32 arg);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_log2_u64(__u64 arg);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_log10_u32(__u32 arg);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_log10_u64(__u64 arg);
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_div_ceil(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b);
ਬਾਹਰੀ __u64 ext2fs_div64_ceil(__u64 a, __u64 b);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dirent_name_len(ਅਚਲ ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_dirent_ਸੈੱਟ_name_len(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਪੂਰਨ len);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_dirent_file_type(ਅਚਲ ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_dirent_ਸੈੱਟ_file_type(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਪੂਰਨ type);
ਬਾਹਰੀ ਬਣਤਰ ext2_inode *ext2fs_inode(ਬਣਤਰ ext2_inode_large * large_inode);
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ ext2_inode *ext2fs_const_inode(ਅਚਲ ਬਣਤਰ ext2_inode_large * large_inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inodes_ਪ੍ਰਤੀ_orphan_ਖੰਡ(ext2_filsys fs);
ਬਾਹਰੀ ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *ext2fs_orphan_ਖੰਡ_ਪੂਛ(ext2_filsys fs,
							       ਅੱਖਰ *buf);
#ਜੇ_ਅੰਤ

/*
 * The actual inlined functions definitions themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all!
 */
#ਜੇ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INCLUDE_INLINE_FUNCS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ
#ਨਹੀਂਤਾਂ
#ਜੇ (__STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਸਤਰਅੰਦਰ
#ਨਹੀਂਤਾਂ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ __inline__
#ਨਹੀਂਤਾਂ				/* For Watcom C */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ /* __GNUC__ */
#ਜੇ_ਅੰਤ /* __STDC_VERSION__ >= 199901L */
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EXT2_CUSTOM_ਸਮ੍ਰਿਤੀ_ROUTINES
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr)
{
	ਖਾਲੀ *pp;

	pp = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
	ਜੇ (!pp)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, &pp, ਆਕਾਰ (pp));
	ਵਾਪਸ 0;
}

_INLINE_ errcode_t ext2fs_get_memzero(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr)
{
	ਖਾਲੀ *pp;

	pp = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
	ਜੇ (!pp)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	memset(pp, 0, ਮਾਪ);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, &pp, ਆਕਾਰ(pp));
	ਵਾਪਸ 0;
}

_INLINE_ errcode_t ext2fs_get_array(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
				    ਖਾਲੀ *ptr)
{
	ਜੇ (ਗਿਣਤੀ_2 && (~0UL)/ਗਿਣਤੀ_2 < ਮਾਪ)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	ਵਾਪਸ ext2fs_get_mem(ਗਿਣਤੀ_2*ਮਾਪ, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr)
{
	ਜੇ (ਗਿਣਤੀ_2 && (~0UL)/ਗਿਣਤੀ_2 < ਮਾਪ)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	ਵਾਪਸ ext2fs_get_memzero((size_t)ਗਿਣਤੀ_2 * ਮਾਪ, ptr);
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(ਖਾਲੀ *ptr)
{
	ਖਾਲੀ *p;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&p, ptr, ਆਕਾਰ(p));
	ਮੁਕਤ(p);
	p = 0;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, &p, ਆਕਾਰ(p));
	ਵਾਪਸ 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)) old_ਮਾਪ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ptr)
{
	ਖਾਲੀ *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&p, ptr, ਆਕਾਰ(p));
	p = realloc(p, ਮਾਪ);
	ਜੇ (!p)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, &p, ਆਕਾਰ(p));
	ਵਾਪਸ 0;
}

/*
 *  Resize array.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_array(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ old_ਗਿਣਤੀ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2, ਖਾਲੀ *ptr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ old_ਮਾਪ;
	errcode_t retval;

	ਜੇ (ਗਿਣਤੀ_2 && (~0UL)/ਗਿਣਤੀ_2 < ਮਾਪ)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	ਮਾਪ *= ਗਿਣਤੀ_2;
	old_ਮਾਪ = ਮਾਪ * old_ਗਿਣਤੀ;
	retval = ext2fs_resize_mem(old_ਮਾਪ, ਮਾਪ, ptr);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (ਮਾਪ > old_ਮਾਪ) {
		ਖਾਲੀ *p;

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&p, ptr, ਆਕਾਰ(p));
		memset((ਅੱਖਰ *)p + old_ਮਾਪ, 0, ਮਾਪ - old_ਮਾਪ);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, &p, ਆਕਾਰ(p));
	}

	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ	/* Custom memory routines */

/*
 * Mark a filesystem superblock as dirty
 */
_INLINE_ ਖਾਲੀ ext2fs_mark_super_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_ਬਦਲਿਆ;
}

/*
 * Mark a filesystem as changed
 */
_INLINE_ ਖਾਲੀ ext2fs_mark_ਬਦਲਿਆ(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_ਬਦਲਿਆ;
}

/*
 * Check to see if a filesystem has changed
 */
_INLINE_ ਪੂਰਨ ext2fs_test_ਬਦਲਿਆ(ext2_filsys fs)
{
	ਵਾਪਸ (fs->flags & EXT2_FLAG_ਬਦਲਿਆ);
}

/*
 * Mark a filesystem as valid
 */
_INLINE_ ਖਾਲੀ ext2fs_mark_valid(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_VALID;
}

/*
 * Mark a filesystem as NOT valid
 */
_INLINE_ ਖਾਲੀ ext2fs_unmark_valid(ext2_filsys fs)
{
	fs->flags &= ~EXT2_FLAG_VALID;
}

/*
 * Check to see if a filesystem is valid
 */
_INLINE_ ਪੂਰਨ ext2fs_test_valid(ext2_filsys fs)
{
	ਵਾਪਸ (fs->flags & EXT2_FLAG_VALID);
}

/*
 * Mark the inode bitmap as dirty
 */
_INLINE_ ਖਾਲੀ ext2fs_mark_ib_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_IB_DIRTY | EXT2_FLAG_ਬਦਲਿਆ;
}

/*
 * Mark the block bitmap as dirty
 */
_INLINE_ ਖਾਲੀ ext2fs_mark_bb_dirty(ext2_filsys fs)
{
	fs->flags |= EXT2_FLAG_BB_DIRTY | EXT2_FLAG_ਬਦਲਿਆ;
}

/*
 * Check to see if a filesystem's inode bitmap is dirty
 */
_INLINE_ ਪੂਰਨ ext2fs_test_ib_dirty(ext2_filsys fs)
{
	ਵਾਪਸ (fs->flags & EXT2_FLAG_IB_DIRTY);
}

/*
 * Check to see if a filesystem's block bitmap is dirty
 */
_INLINE_ ਪੂਰਨ ext2fs_test_bb_dirty(ext2_filsys fs)
{
	ਵਾਪਸ (fs->flags & EXT2_FLAG_BB_DIRTY);
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_blk(ext2_filsys fs, blk_t blk)
{
	ਵਾਪਸ ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_ino(ext2_filsys fs, ext2_ino_t ino)
{
	ਵਾਪਸ (ino - 1) / fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_ਖੰਡ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਵਾਪਸ (blk_t) ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_ਸਮੂਹ_ਆਖਰੀ_ਖੰਡ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਵਾਪਸ (blk_t) ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਸਮੂਹ);
}

_INLINE_ blk_t ext2fs_inode_ਅੰਕੜੇ_ਖੰਡਾਂ(ext2_filsys fs,
					ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ (blk_t) ext2fs_inode_ਅੰਕੜੇ_blocks2(fs, inode);
}

_INLINE_ ਪੂਰਨ ext2fs_htree_intnode_maxrecs(ext2_filsys fs, ਪੂਰਨ ਖੰਡਾਂ)
{
	ਪੂਰਨ csum_ਮਾਪ = 0;

	ਜੇ ((EXT2_SB(fs->super)->s_feature_ro_compat &
	     EXT4_FEATURE_RO_COMPAT_METADATA_CSUM) != 0)
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਪੂਛ);
	ਵਾਪਸ ਖੰਡਾਂ * ((fs->blocksize - (8 + csum_ਮਾਪ)) /
						ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));
}

/*
 * log base 2, rounded down
 */
_INLINE_ ਪੂਰਨ ext2fs_log2_u32(__u32 arg)
{
	ਪੂਰਨ l = 0;

	arg >>= 1;
	ਜਦੋਂ (arg) {
		l++;
		arg >>= 1;
	}
	ਵਾਪਸ l;
}

/*
 * log base 2, rounded down
 */
_INLINE_ ਪੂਰਨ ext2fs_log2_u64(__u64 arg)
{
	ਪੂਰਨ l = 0;

	arg >>= 1;
	ਜਦੋਂ (arg) {
		l++;
		arg >>= 1;
	}
	ਵਾਪਸ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ਪੂਰਨ ext2fs_log10_u32(__u32 arg)
{
	ਪੂਰਨ l = 0;

	arg /= 10;
	ਜਦੋਂ (arg) {
		l++;
		arg /= 10;
	}
	ਵਾਪਸ l;
}

/*
 * log base 10, rounded down
 */
_INLINE_ ਪੂਰਨ ext2fs_log10_u64(__u64 arg)
{
	ਪੂਰਨ l = 0;

	arg /= 10;
	ਜਦੋਂ (arg) {
		l++;
		arg /= 10;
	}
	ਵਾਪਸ l;
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_div_ceil(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b)
{
	ਜੇ (!a)
		ਵਾਪਸ 0;
	ਵਾਪਸ ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	ਜੇ (!a)
		ਵਾਪਸ 0;
	ਵਾਪਸ ((a - 1) / b) + 1;
}

_INLINE_ ਪੂਰਨ ext2fs_dirent_name_len(ਅਚਲ ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਵਾਪਸ ਇੰਦਰਾਜ->name_len & 0xff;
}

_INLINE_ ਖਾਲੀ ext2fs_dirent_ਸੈੱਟ_name_len(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਪੂਰਨ len)
{
	ਇੰਦਰਾਜ->name_len = (ਇੰਦਰਾਜ->name_len & 0xff00) | (len & 0xff);
}

_INLINE_ ਪੂਰਨ ext2fs_dirent_file_type(ਅਚਲ ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਵਾਪਸ ਇੰਦਰਾਜ->name_len >> 8;
}

_INLINE_ ਖਾਲੀ ext2fs_dirent_ਸੈੱਟ_file_type(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਪੂਰਨ type)
{
	ਇੰਦਰਾਜ->name_len = (ਇੰਦਰਾਜ->name_len & 0xff) | (type << 8);
}

_INLINE_ ਬਣਤਰ ext2_inode *ext2fs_inode(ਬਣਤਰ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ਵਾਪਸ (ਬਣਤਰ ext2_inode *) large_inode;
}

_INLINE_ ਅਚਲ ਬਣਤਰ ext2_inode *
ext2fs_const_inode(ਅਚਲ ਬਣਤਰ ext2_inode_large * large_inode)
{
	/* It is always safe to convert large inode to a small inode */
	ਵਾਪਸ (ਅਚਲ ਬਣਤਰ ext2_inode *) large_inode;
}

_INLINE_ ਪੂਰਨ ext2fs_inodes_ਪ੍ਰਤੀ_orphan_ਖੰਡ(ext2_filsys fs)
{
	ਵਾਪਸ (fs->blocksize - ਆਕਾਰ(ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ)) /
		ਆਕਾਰ(__u32);
}

_INLINE_ ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *
ext2fs_orphan_ਖੰਡ_ਪੂਛ(ext2_filsys fs, ਅੱਖਰ *buf)
{
	ਵਾਪਸ (ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *)(buf + fs->blocksize -
		ਆਕਾਰ(ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ));
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ _INLINE_
#ਜੇ_ਅੰਤ

/* htree levels for ext4 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_HTREE_LEVEL_COMPAT 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_HTREE_LEVEL	3

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2_dir_htree_level(ext2_filsys fs)
{
	ਜੇ (ext2fs_has_feature_largedir(fs->super))
		ਵਾਪਸ EXT4_HTREE_LEVEL;

	ਵਾਪਸ EXT4_HTREE_LEVEL_COMPAT;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_ਸੈੱਟ_dtime(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode)
{
	__u32	t;

	ਜੇ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_ਸਮਾਂ))
		t = fs->now & 0xFFFFFFFF;
	ਨਹੀਂਤਾਂ
		t = time(ਨਿਰਾ) & 0xFFFFFFFF;
	ਜੇ (t < fs->super->s_inodes_ਗਿਣਤੀ)
		t = fs->super->s_inodes_ਗਿਣਤੀ;
	inode->i_dtime = t;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* _EXT2FS_EXT2FS_H */
