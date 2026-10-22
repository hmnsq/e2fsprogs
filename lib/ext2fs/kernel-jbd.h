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

#यदि_अपरिभाषितम् _LINUX_JBD_H
#परिभाषय _LINUX_JBD_H

#अन्तर्भाव "jfs_compat.h"
#परिभाषय JFS_DEBUG
#परिभाषय jfs_debug jbd_debug

#यदि_अपरिभाषितम् __GNUC__
#परिभाषय __FUNCTION__ ""
#यदि_अन्तः

#परिभाषय journal_oom_retry 0

#यदि_परिभाषितम् CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#परिभाषय JBD_EXPENSIVE_CHECKING
बाह्य पूर्णाङ्क journal_enable_debug;
#अन्यथा
#परिभाषय journal_enable_debug (-1)
#यदि_अन्तः /* !CONFIG_JBD_DEBUG */

#यदि_परिभाषितम् __STDC__
#परिभाषय jbd_debug(n, f, a...)						\
	कुरु {								\
		यदि ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} यावत् (0)
#अन्यथा
#परिभाषय jbd_debug(x)		/* AIX doesn't do STDC */
#यदि_अन्तः /* !__STDC__ */

बाह्य शून्य * __jbd_kmalloc (वर्ण *where, size_t आकार, पूर्णाङ्क flags, पूर्णाङ्क retry);
#परिभाषय jbd_kmalloc(आकार, flags) \
	__jbd_kmalloc(__FUNCTION__, (आकार), (flags), journal_oom_retry)
#परिभाषय jbd_rep_kmalloc(आकार, flags) \
	__jbd_kmalloc(__FUNCTION__, (आकार), (flags), 1)

#परिभाषय JBD2_MIN_JOURNAL_खण्डानि 1024
#परिभाषय JBD2_MAX_JOURNAL_खण्डानि (JBD2_MIN_JOURNAL_खण्डानि * 10000)
#परिभाषय JBD2_DEFAULT_FAST_COMMIT_खण्डानि 256

/*
 * Internal structures used by the logging mechanism:
 */

#परिभाषय JBD2_MAGIC_सङ्ख्या 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#परिभाषय JBD2_DESCRIPTOR_खण्डम्	1
#परिभाषय JBD2_COMMIT_खण्डम्	2
#परिभाषय JBD2_SUPERBLOCK_V1	3
#परिभाषय JBD2_SUPERBLOCK_V2	4
#परिभाषय JBD2_REVOKE_खण्डम्	5
#परिभाषय JBD2_FC_खण्डम्		6

/*
 * Standard header for all descriptor blocks:
 */
प्रकारनिर्देश संरचना journal_शीर्षकम्_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_शीर्षकम्_t;

/*
 * Checksum types.
 */
#परिभाषय JBD2_चक्रीयपरीक्षा_32_CHKSUM   1
#परिभाषय JBD2_MD5_CHKSUM     2
#परिभाषय JBD2_SHA1_CHKSUM    3
#परिभाषय JBD2_चक्रीयपरीक्षा_32_C_CHKSUM  4

#परिभाषय JBD2_चक्रीयपरीक्षा_32_CHKSUM_आकार 4

#परिभाषय JBD2_CHECKSUM_अष्टकानि (32 / परिमाण(__u32))
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
संरचना commit_शीर्षकम् {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	अचिह्नित वर्ण	h_chksum_type;
	अचिह्नित वर्ण	h_chksum_आकार;
	अचिह्नित वर्ण	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_अष्टकानि];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
प्रकारनिर्देश संरचना journal_खण्डम्_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_उच्चः; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_खण्डम्_tag3_t;

प्रकारनिर्देश संरचना journal_खण्डम्_चिह्नकम्_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_उच्चः; /* most-significant high 32bits. */
} journal_खण्डम्_चिह्नकम्_t;

/* Tail of descriptor or revoke block, for checksumming */
संरचना jbd2_journal_खण्डम्_पुच्छ {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
प्रकारनिर्देश संरचना journal_revoke_शीर्षकम्_s
{
	journal_शीर्षकम्_t r_शीर्षकम्;
	__be32		 r_गणनम्;	/* Count of bytes used in the block */
} jbd2_journal_revoke_शीर्षकम्_t;

/* Definitions for the journal tag flags word: */
#परिभाषय JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#परिभाषय JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#परिभाषय JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#परिभाषय JBD2_FLAG_चरमः_चिह्नकम्	8	/* last tag in this descriptor block */


#परिभाषय UUID_आकार 16
#परिभाषय JBD2_USERS_MAX 48
#परिभाषय JBD2_USERS_आकार (UUID_आकार * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
प्रकारनिर्देश संरचना journal_superblock_s
{
/* 0x0000 */
	journal_शीर्षकम्_t s_शीर्षकम्;

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
	__be32	s_max_trans_दत्त;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_अङ्कः_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_आकार];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#परिभाषय JBD2_HAS_COMPAT_FEATURE(j,आवरणम्)					\
	((j)->j_format_संस्करणम् >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_गन्तव्यम्_be32((आवरणम्))))
#परिभाषय JBD2_HAS_RO_COMPAT_FEATURE(j,आवरणम्)				\
	((j)->j_format_संस्करणम् >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_गन्तव्यम्_be32((आवरणम्))))
#परिभाषय JBD2_HAS_INCOMPAT_FEATURE(j,आवरणम्)				\
	((j)->j_format_संस्करणम् >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_गन्तव्यम्_be32((आवरणम्))))

#परिभाषय JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#परिभाषय JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#परिभाषय JBD2_FEATURE_INCOMPAT_64BIT		0x00000002
#परिभाषय JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#परिभाषय JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#परिभाषय JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#परिभाषय JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#परिभाषय JBD2_ज्ञातम्_COMPAT_FEATURES	0
#परिभाषय JBD2_ज्ञातम्_ROCOMPAT_FEATURES	0
#परिभाषय JBD2_ज्ञातम्_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64BIT|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#यदि_परिभाषितम् NO_INLINE_FUNCS
बाह्य size_t journal_चिह्नकम्_अष्टकानि(journal_t *journal);
बाह्य पूर्णाङ्क jbd2_journal_has_csum_v2or3(journal_t *journal);
बाह्य पूर्णाङ्क jbd2_journal_get_अङ्कः_fc_blks(journal_superblock_t *jsb);
बाह्य पूर्णाङ्क tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((अप्रयुक्तम्));
बाह्य पूर्णाङ्क tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((अप्रयुक्तम्));
#यदि_अन्तः

#यदि (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#यदि_परिभाषितम् E2FSCK_INCLUDE_INLINE_FUNCS
#यदि (__STDC_VERSION__ >= 199901L)
#परिभाषय _INLINE_ बाह्य अन्तःस्थ
#अन्यथा
#परिभाषय _INLINE_ अन्तःस्थ
#यदि_अन्तः
#अन्यथा /* !E2FSCK_INCLUDE_INLINE FUNCS */
#यदि (__STDC_VERSION__ >= 199901L)
#परिभाषय _INLINE_ अन्तःस्थ
#अन्यथा /* not C99 */
#यदि_परिभाषितम् __GNUC__
#परिभाषय _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#परिभाषय _INLINE_ बाह्य अन्तःस्थ
#यदि_अन्तः /* __GNUC__ */
#यदि_अन्तः /* __STDC_VERSION__ >= 199901L */
#यदि_अन्तः /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#परिभाषय JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j); \
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j) \
{ \
	प्रत्यागम ((j)->j_format_संस्करणम् >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#परिभाषय JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j) \
{ \
	प्रत्यागम ((j)->j_format_संस्करणम् >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#परिभाषय JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j); \
_INLINE_ पूर्णाङ्क jbd2_has_feature_##name(journal_t *j) \
{ \
	प्रत्यागम ((j)->j_format_संस्करणम् >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
_INLINE_ शून्य jbd2_समुच्चयः_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ शून्य jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_गन्तव्यम्_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#अन्यथा
#परिभाषय JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
बाह्य पूर्णाङ्क jbd2_has_feature_##name(journal_t *j); \
बाह्य शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
बाह्य शून्य jbd2_clear_feature_##name(journal_t *j);

#परिभाषय JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
बाह्य पूर्णाङ्क jbd2_has_feature_##name(journal_t *j); \
बाह्य शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
बाह्य शून्य jbd2_clear_feature_##name(journal_t *j);

#परिभाषय JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
बाह्य पूर्णाङ्क jbd2_has_feature_##name(journal_t *j); \
बाह्य शून्य jbd2_समुच्चयः_feature##name(journal_t *j); \
बाह्य शून्य jbd2_clear_feature_##name(journal_t *j);

#यदि_अन्तः /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64bit,		64BIT)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#यदि (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_चिह्नकम्_अष्टकानि(journal_t *journal)
{
	size_t sz;

	यदि (jbd2_has_feature_csum3(journal))
		प्रत्यागम परिमाण(journal_खण्डम्_tag3_t);

	sz = परिमाण(journal_खण्डम्_चिह्नकम्_t);

	यदि (jbd2_has_feature_csum2(journal))
		sz += परिमाण(__u16);

	यदि (jbd2_has_feature_64bit(journal))
		प्रत्यागम sz;

	प्रत्यागम sz - परिमाण(__u32);
}

_INLINE_ पूर्णाङ्क jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	यदि (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

_INLINE_ पूर्णाङ्क jbd2_journal_get_अङ्कः_fc_blks(journal_superblock_t *jsb)
{
	पूर्णाङ्क अङ्कः_fc_खण्डानि = be32_गन्तव्यम्_cpu(jsb->s_अङ्कः_fc_blks);

	प्रत्यागम अङ्कः_fc_खण्डानि ? अङ्कः_fc_खण्डानि : JBD2_DEFAULT_FAST_COMMIT_खण्डानि;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ पूर्णाङ्क tid_gt(tid_t x, tid_t y)
{
	पूर्णाङ्क difference = (x - y);
	प्रत्यागम (difference > 0);
}

_INLINE_ पूर्णाङ्क tid_geq(tid_t x, tid_t y)
{
	पूर्णाङ्क difference = (x - y);
	प्रत्यागम (difference >= 0);
}
#यदि_अन्तः /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#अपरिभाषय _INLINE_

बाह्य पूर्णाङ्क journal_खण्डानि_प्रत्येकम्_page(संरचना inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#परिभाषय BJ_None		0	/* Not journaled */
#परिभाषय BJ_Sync_दत्त	1	/* Normal data: flush before commit */
#परिभाषय BJ_Async_दत्त	2	/* writepage data: wait on it before commit */
#परिभाषय BJ_Metadata	3	/* Normal journaled metadata */
#परिभाषय BJ_Forget	4	/* Buffer superseded by this transaction */
#परिभाषय BJ_IO		5	/* Buffer is for temporary IO use */
#परिभाषय BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#परिभाषय BJ_LogCtl	7	/* Buffer contains log descriptors */
#परिभाषय BJ_Reserved	8	/* Buffer is reserved for access by journal */
#परिभाषय BJ_Types	9

बाह्य पूर्णाङ्क jbd_खण्डानि_प्रत्येकम्_page(संरचना inode *inode);

#यदि_अन्तः	/* _LINUX_JBD_H */
