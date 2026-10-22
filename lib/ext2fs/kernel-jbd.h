/*
 * linux/include/linux/jbd.h
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>
 *
 * Copyright 1998-2000 Red Hat, Inc --- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * Definitions for transaction data structures for the buffer cache
 * filesystem journaling support.
 */

#अगर_अघोषित _LINUX_JBD_H
#घोषणा _LINUX_JBD_H

#समावेश "jfs_compat.h"
#घोषणा JFS_DEBUG
#घोषणा jfs_debug jbd_debug

#अगर_अघोषित __GNUC__
#घोषणा __FUNCTION__ ""
#पूर्ण_अगर

#घोषणा journal_oom_retry 0

#अगर_घोषित CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#घोषणा JBD_EXPENSIVE_CHECKING
बाह्य पूर्णांक journal_enable_debug;
#अन्यथा
#घोषणा journal_enable_debug (-1)
#पूर्ण_अगर /* !CONFIG_JBD_DEBUG */

#अगर_घोषित __STDC__
#घोषणा jbd_debug(n, f, a...)						\
	करो {								\
		अगर ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__खाता__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} जबतक (0)
#अन्यथा
#घोषणा jbd_debug(x)		/* AIX doesn't do STDC */
#पूर्ण_अगर /* !__STDC__ */

बाह्य व्योम * __jbd_kmalloc (अक्षर *where, माप_प्रकार आकार, पूर्णांक flags, पूर्णांक retry);
#घोषणा jbd_kmalloc(आकार, flags) \
	__jbd_kmalloc(__FUNCTION__, (आकार), (flags), journal_oom_retry)
#घोषणा jbd_rep_kmalloc(आकार, flags) \
	__jbd_kmalloc(__FUNCTION__, (आकार), (flags), 1)

#घोषणा JBD2_MIN_JOURNAL_BLOCKS 1024
#घोषणा JBD2_MAX_JOURNAL_BLOCKS (JBD2_MIN_JOURNAL_BLOCKS * 10000)
#घोषणा JBD2_DEFAULT_FAST_COMMIT_BLOCKS 256

/*
 * Internal structures used by the logging mechanism:
 */

#घोषणा JBD2_MAGIC_संख्या 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#घोषणा JBD2_DESCRIPTOR_BLOCK	1
#घोषणा JBD2_COMMIT_BLOCK	2
#घोषणा JBD2_SUPERBLOCK_V1	3
#घोषणा JBD2_SUPERBLOCK_V2	4
#घोषणा JBD2_REVOKE_BLOCK	5
#घोषणा JBD2_FC_BLOCK		6

/*
 * Standard header for all descriptor blocks:
 */
प्रकार काष्ठा journal_header_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_header_t;

/*
 * Checksum types.
 */
#घोषणा JBD2_CRC32_CHKSUM   1
#घोषणा JBD2_MD5_CHKSUM     2
#घोषणा JBD2_SHA1_CHKSUM    3
#घोषणा JBD2_CRC32C_CHKSUM  4

#घोषणा JBD2_CRC32_CHKSUM_आकार 4

#घोषणा JBD2_CHECKSUM_BYTES (32 / माप(__u32))
/*
 * Commit block header for storing transactional checksums:
 *
 * NOTE: If FEATURE_COMPAT_CHECKSUM (checksum v1) is set, the h_chksum*
 * fields are used to store a checksum of the descriptor and data blocks.
 *
 * If FEATURE_INCOMPAT_CSUM_V2 (checksum v2) is set, then the h_chksum
 * field is used to store crc32c(uuid+commit_block).  Each journal metadata
 * block gets its own checksum, and data block checksums are stored in
 * journal_block_tag (in the descriptor).  The other h_chksum* fields are
 * not used.
 *
 * If FEATURE_INCOMPAT_CSUM_V3 is set, the descriptor block uses
 * journal_block_tag3_t to store a full 32-bit checksum.  Everything else
 * is the same as v2.
 *
 * Checksum v1, v2, and v3 are mutually exclusive features.
 */
काष्ठा commit_header {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	अचिन्हित अक्षर	h_chksum_type;
	अचिन्हित अक्षर	h_chksum_आकार;
	अचिन्हित अक्षर	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_BYTES];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
प्रकार काष्ठा journal_block_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_ऊपरी; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_block_tag3_t;

प्रकार काष्ठा journal_block_tag_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_ऊपरी; /* most-significant high 32bits. */
} journal_block_tag_t;

/* Tail of descriptor or revoke block, for checksumming */
काष्ठा jbd2_journal_block_पुच्छ {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
प्रकार काष्ठा journal_revoke_header_s
{
	journal_header_t r_header;
	__be32		 r_गिनती;	/* Count of bytes used in the block */
} jbd2_journal_revoke_header_t;

/* Definitions for the journal tag flags word: */
#घोषणा JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#घोषणा JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#घोषणा JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#घोषणा JBD2_FLAG_LAST_TAG	8	/* last tag in this descriptor block */


#घोषणा UUID_आकार 16
#घोषणा JBD2_USERS_MAX 48
#घोषणा JBD2_USERS_आकार (UUID_आकार * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
प्रकार काष्ठा journal_superblock_s
{
/* 0x0000 */
	journal_header_t s_header;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_first;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_start;		/* blocknr of start of log */

/* 0x0020 */
	/* Error value, as set by journal_abort(). */
	__s32	s_errno;

/* 0x0024 */
	/* Remaining fields are only valid in a version-2 superblock */
	__be32	s_feature_compat; 	/* compatible feature set */
	__be32	s_feature_incompat; 	/* incompatible feature set */
	__be32	s_feature_ro_compat; 	/* readonly-compatible feature set */
/* 0x0030 */
	__u8	s_uuid[16];		/* 128-bit uuid for journal */

/* 0x0040 */
	__be32	s_nr_users;		/* Nr of filesystems sharing log */

	__be32	s_dynsuper;		/* Blocknr of dynamic superblock copy*/

/* 0x0048 */
	__be32	s_max_transaction;	/* Limit of journal blocks per trans.*/
	__be32	s_max_trans_तथ्य;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_अंक_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_आकार];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#घोषणा JBD2_HAS_COMPAT_FEATURE(j,mask)					\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_to_be32((mask))))
#घोषणा JBD2_HAS_RO_COMPAT_FEATURE(j,mask)				\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_to_be32((mask))))
#घोषणा JBD2_HAS_INCOMPAT_FEATURE(j,mask)				\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_to_be32((mask))))

#घोषणा JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#घोषणा JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#घोषणा JBD2_FEATURE_INCOMPAT_64BIT		0x00000002
#घोषणा JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#घोषणा JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#घोषणा JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#घोषणा JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#घोषणा JBD2_KNOWN_COMPAT_FEATURES	0
#घोषणा JBD2_KNOWN_ROCOMPAT_FEATURES	0
#घोषणा JBD2_KNOWN_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64BIT|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#अगर_घोषित NO_INLINE_FUNCS
बाह्य माप_प्रकार journal_tag_bytes(journal_t *journal);
बाह्य पूर्णांक jbd2_journal_has_csum_v2or3(journal_t *journal);
बाह्य पूर्णांक jbd2_journal_get_अंक_fc_blks(journal_superblock_t *jsb);
बाह्य पूर्णांक tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((unused));
बाह्य पूर्णांक tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((unused));
#पूर्ण_अगर

#अगर (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#अगर_घोषित E2FSCK_INCLUDE_INLINE_FUNCS
#अगर (__STDC_VERSION__ >= 199901L)
#घोषणा _INLINE_ बाह्य अंतरभूत
#अन्यथा
#घोषणा _INLINE_ अंतरभूत
#पूर्ण_अगर
#अन्यथा /* !E2FSCK_INCLUDE_INLINE FUNCS */
#अगर (__STDC_VERSION__ >= 199901L)
#घोषणा _INLINE_ अंतरभूत
#अन्यथा /* not C99 */
#अगर_घोषित __GNUC__
#घोषणा _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#घोषणा _INLINE_ बाह्य अंतरभूत
#पूर्ण_अगर /* __GNUC__ */
#पूर्ण_अगर /* __STDC_VERSION__ >= 199901L */
#पूर्ण_अगर /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#घोषणा JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j); \
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j) \
{ \
	वापस ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#घोषणा JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j) \
{ \
	वापस ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#घोषणा JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j); \
_INLINE_ पूर्णांक jbd2_has_feature_##name(journal_t *j) \
{ \
	वापस ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ व्योम jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#अन्यथा
#घोषणा JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
बाह्य पूर्णांक jbd2_has_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_set_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_clear_feature_##name(journal_t *j);

#घोषणा JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
बाह्य पूर्णांक jbd2_has_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_set_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_clear_feature_##name(journal_t *j);

#घोषणा JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
बाह्य पूर्णांक jbd2_has_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_set_feature_##name(journal_t *j); \
बाह्य व्योम jbd2_clear_feature_##name(journal_t *j);

#पूर्ण_अगर /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64bit,		64BIT)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#अगर (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ माप_प्रकार journal_tag_bytes(journal_t *journal)
{
	माप_प्रकार sz;

	अगर (jbd2_has_feature_csum3(journal))
		वापस माप(journal_block_tag3_t);

	sz = माप(journal_block_tag_t);

	अगर (jbd2_has_feature_csum2(journal))
		sz += माप(__u16);

	अगर (jbd2_has_feature_64bit(journal))
		वापस sz;

	वापस sz - माप(__u32);
}

_INLINE_ पूर्णांक jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	अगर (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		वापस 1;

	वापस 0;
}

_INLINE_ पूर्णांक jbd2_journal_get_अंक_fc_blks(journal_superblock_t *jsb)
{
	पूर्णांक अंक_fc_blocks = be32_to_cpu(jsb->s_अंक_fc_blks);

	वापस अंक_fc_blocks ? अंक_fc_blocks : JBD2_DEFAULT_FAST_COMMIT_BLOCKS;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ पूर्णांक tid_gt(tid_t x, tid_t y)
{
	पूर्णांक difference = (x - y);
	वापस (difference > 0);
}

_INLINE_ पूर्णांक tid_geq(tid_t x, tid_t y)
{
	पूर्णांक difference = (x - y);
	वापस (difference >= 0);
}
#पूर्ण_अगर /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#अघोषित _INLINE_

बाह्य पूर्णांक journal_blocks_per_page(काष्ठा inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#घोषणा BJ_None		0	/* Not journaled */
#घोषणा BJ_Sync_तथ्य	1	/* Normal data: flush before commit */
#घोषणा BJ_Async_तथ्य	2	/* writepage data: wait on it before commit */
#घोषणा BJ_Metadata	3	/* Normal journaled metadata */
#घोषणा BJ_Forget	4	/* Buffer superseded by this transaction */
#घोषणा BJ_IO		5	/* Buffer is for temporary IO use */
#घोषणा BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#घोषणा BJ_LogCtl	7	/* Buffer contains log descriptors */
#घोषणा BJ_Reserved	8	/* Buffer is reserved for access by journal */
#घोषणा BJ_Types	9

बाह्य पूर्णांक jbd_blocks_per_page(काष्ठा inode *inode);

#पूर्ण_अगर	/* _LINUX_JBD_H */
