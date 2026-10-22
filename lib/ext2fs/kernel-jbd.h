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

#నిర్వచితం_కాకపోతే _LINUX_JBD_H
#నిర్వచించు _LINUX_JBD_H

#అన్తర్భావ "jfs_compat.h"
#నిర్వచించు JFS_DEBUG
#నిర్వచించు jfs_debug jbd_debug

#నిర్వచితం_కాకపోతే __GNUC__
#నిర్వచించు __FUNCTION__ ""
#అయితే_ముగింపు

#నిర్వచించు journal_oom_retry 0

#నిర్వచితమైతే CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#నిర్వచించు JBD_EXPENSIVE_CHECKING
బాహ్య పూర్ణాంకం journal_enable_debug;
#లేకపోతే
#నిర్వచించు journal_enable_debug (-1)
#అయితే_ముగింపు /* !CONFIG_JBD_DEBUG */

#నిర్వచితమైతే __STDC__
#నిర్వచించు jbd_debug(n, f, a...)						\
	చేయి {								\
		ఒకవేళ ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} అయితే (0)
#లేకపోతే
#నిర్వచించు jbd_debug(x)		/* AIX doesn't do STDC */
#అయితే_ముగింపు /* !__STDC__ */

బాహ్య శూన్య * __jbd_kmalloc (అక్షరం *where, size_t కొలత, పూర్ణాంకం flags, పూర్ణాంకం retry);
#నిర్వచించు jbd_kmalloc(కొలత, flags) \
	__jbd_kmalloc(__FUNCTION__, (కొలత), (flags), journal_oom_retry)
#నిర్వచించు jbd_rep_kmalloc(కొలత, flags) \
	__jbd_kmalloc(__FUNCTION__, (కొలత), (flags), 1)

#నిర్వచించు JBD2_MIN_JOURNAL_ఖండాలు 1024
#నిర్వచించు JBD2_MAX_JOURNAL_ఖండాలు (JBD2_MIN_JOURNAL_ఖండాలు * 10000)
#నిర్వచించు JBD2_DEFAULT_FAST_COMMIT_ఖండాలు 256

/*
 * Internal structures used by the logging mechanism:
 */

#నిర్వచించు JBD2_MAGIC_సంఖ్య 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#నిర్వచించు JBD2_DESCRIPTOR_ఖండం	1
#నిర్వచించు JBD2_COMMIT_ఖండం	2
#నిర్వచించు JBD2_SUPERBLOCK_V1	3
#నిర్వచించు JBD2_SUPERBLOCK_V2	4
#నిర్వచించు JBD2_REVOKE_ఖండం	5
#నిర్వచించు JBD2_FC_ఖండం		6

/*
 * Standard header for all descriptor blocks:
 */
రకనిర్వచనం నిర్మాణం journal_శీర్షిక_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_శీర్షిక_t;

/*
 * Checksum types.
 */
#నిర్వచించు JBD2_చక్రీయ_తనిఖీ_32_CHKSUM   1
#నిర్వచించు JBD2_MD5_CHKSUM     2
#నిర్వచించు JBD2_SHA1_CHKSUM    3
#నిర్వచించు JBD2_చక్రీయ_తనిఖీ_32_C_CHKSUM  4

#నిర్వచించు JBD2_చక్రీయ_తనిఖీ_32_CHKSUM_కొలత 4

#నిర్వచించు JBD2_CHECKSUM_బైట్లు (32 / పరిమాణం(__u32))
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
నిర్మాణం commit_శీర్షిక {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	గుర్తులేని అక్షరం	h_chksum_type;
	గుర్తులేని అక్షరం	h_chksum_కొలత;
	గుర్తులేని అక్షరం	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_బైట్లు];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
రకనిర్వచనం నిర్మాణం journal_ఖండం_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_ఎగువ; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_ఖండం_tag3_t;

రకనిర్వచనం నిర్మాణం journal_ఖండం_ట్యాగ్_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_ఎగువ; /* most-significant high 32bits. */
} journal_ఖండం_ట్యాగ్_t;

/* Tail of descriptor or revoke block, for checksumming */
నిర్మాణం jbd2_journal_ఖండం_తోక {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
రకనిర్వచనం నిర్మాణం journal_revoke_శీర్షిక_s
{
	journal_శీర్షిక_t r_శీర్షిక;
	__be32		 r_లెక్క;	/* Count of bytes used in the block */
} jbd2_journal_revoke_శీర్షిక_t;

/* Definitions for the journal tag flags word: */
#నిర్వచించు JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#నిర్వచించు JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#నిర్వచించు JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#నిర్వచించు JBD2_FLAG_చివరి_ట్యాగ్	8	/* last tag in this descriptor block */


#నిర్వచించు UUID_కొలత 16
#నిర్వచించు JBD2_USERS_MAX 48
#నిర్వచించు JBD2_USERS_కొలత (UUID_కొలత * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
రకనిర్వచనం నిర్మాణం journal_superblock_s
{
/* 0x0000 */
	journal_శీర్షిక_t s_శీర్షిక;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_మొదటి;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_ప్రారంభం;		/* blocknr of start of log */

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
	__be32	s_max_trans_దత్తాంశం;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_అంకె_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_కొలత];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#నిర్వచించు JBD2_HAS_COMPAT_FEATURE(j,ముసుగు)					\
	((j)->j_format_సంస్కరణ >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_గమ్యం_be32((ముసుగు))))
#నిర్వచించు JBD2_HAS_RO_COMPAT_FEATURE(j,ముసుగు)				\
	((j)->j_format_సంస్కరణ >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_గమ్యం_be32((ముసుగు))))
#నిర్వచించు JBD2_HAS_INCOMPAT_FEATURE(j,ముసుగు)				\
	((j)->j_format_సంస్కరణ >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_గమ్యం_be32((ముసుగు))))

#నిర్వచించు JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#నిర్వచించు JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#నిర్వచించు JBD2_FEATURE_INCOMPAT_64_బిట్టు		0x00000002
#నిర్వచించు JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#నిర్వచించు JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#నిర్వచించు JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#నిర్వచించు JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#నిర్వచించు JBD2_తెలిసిన_COMPAT_FEATURES	0
#నిర్వచించు JBD2_తెలిసిన_ROCOMPAT_FEATURES	0
#నిర్వచించు JBD2_తెలిసిన_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_బిట్టు|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#నిర్వచితమైతే NO_INLINE_FUNCS
బాహ్య size_t journal_ట్యాగ్_బైట్లు(journal_t *journal);
బాహ్య పూర్ణాంకం jbd2_journal_has_csum_v2or3(journal_t *journal);
బాహ్య పూర్ణాంకం jbd2_journal_get_అంకె_fc_blks(journal_superblock_t *jsb);
బాహ్య పూర్ణాంకం tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((వాడని));
బాహ్య పూర్ణాంకం tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((వాడని));
#అయితే_ముగింపు

#ఒకవేళ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#నిర్వచితమైతే E2FSCK_INCLUDE_INLINE_FUNCS
#ఒకవేళ (__STDC_VERSION__ >= 199901L)
#నిర్వచించు _INLINE_ బాహ్య అంతఃవరుస
#లేకపోతే
#నిర్వచించు _INLINE_ అంతఃవరుస
#అయితే_ముగింపు
#లేకపోతే /* !E2FSCK_INCLUDE_INLINE FUNCS */
#ఒకవేళ (__STDC_VERSION__ >= 199901L)
#నిర్వచించు _INLINE_ అంతఃవరుస
#లేకపోతే /* not C99 */
#నిర్వచితమైతే __GNUC__
#నిర్వచించు _INLINE_ బాహ్య __inline__
#లేకపోతే				/* For Watcom C */
#నిర్వచించు _INLINE_ బాహ్య అంతఃవరుస
#అయితే_ముగింపు /* __GNUC__ */
#అయితే_ముగింపు /* __STDC_VERSION__ >= 199901L */
#అయితే_ముగింపు /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#నిర్వచించు JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j); \
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j) \
{ \
	తిరిగి ((j)->j_format_సంస్కరణ >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j); \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#నిర్వచించు JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j) \
{ \
	తిరిగి ((j)->j_format_సంస్కరణ >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j); \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#నిర్వచించు JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j); \
_INLINE_ పూర్ణాంకం jbd2_has_feature_##name(journal_t *j) \
{ \
	తిరిగి ((j)->j_format_సంస్కరణ >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j); \
_INLINE_ శూన్య jbd2_సమితి_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ శూన్య jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_గమ్యం_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#లేకపోతే
#నిర్వచించు JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
బాహ్య పూర్ణాంకం jbd2_has_feature_##name(journal_t *j); \
బాహ్య శూన్య jbd2_సమితి_feature##name(journal_t *j); \
బాహ్య శూన్య jbd2_clear_feature_##name(journal_t *j);

#నిర్వచించు JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
బాహ్య పూర్ణాంకం jbd2_has_feature_##name(journal_t *j); \
బాహ్య శూన్య jbd2_సమితి_feature##name(journal_t *j); \
బాహ్య శూన్య jbd2_clear_feature_##name(journal_t *j);

#నిర్వచించు JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
బాహ్య పూర్ణాంకం jbd2_has_feature_##name(journal_t *j); \
బాహ్య శూన్య jbd2_సమితి_feature##name(journal_t *j); \
బాహ్య శూన్య jbd2_clear_feature_##name(journal_t *j);

#అయితే_ముగింపు /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64బిట్టు,		64బిట్టు_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#ఒకవేళ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_ట్యాగ్_బైట్లు(journal_t *journal)
{
	size_t sz;

	ఒకవేళ (jbd2_has_feature_csum3(journal))
		తిరిగి పరిమాణం(journal_ఖండం_tag3_t);

	sz = పరిమాణం(journal_ఖండం_ట్యాగ్_t);

	ఒకవేళ (jbd2_has_feature_csum2(journal))
		sz += పరిమాణం(__u16);

	ఒకవేళ (jbd2_has_feature_64bit(journal))
		తిరిగి sz;

	తిరిగి sz - పరిమాణం(__u32);
}

_INLINE_ పూర్ణాంకం jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	ఒకవేళ (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		తిరిగి 1;

	తిరిగి 0;
}

_INLINE_ పూర్ణాంకం jbd2_journal_get_అంకె_fc_blks(journal_superblock_t *jsb)
{
	పూర్ణాంకం అంకె_fc_ఖండాలు = be32_గమ్యం_cpu(jsb->s_అంకె_fc_blks);

	తిరిగి అంకె_fc_ఖండాలు ? అంకె_fc_ఖండాలు : JBD2_DEFAULT_FAST_COMMIT_ఖండాలు;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ పూర్ణాంకం tid_gt(tid_t x, tid_t y)
{
	పూర్ణాంకం difference = (x - y);
	తిరిగి (difference > 0);
}

_INLINE_ పూర్ణాంకం tid_geq(tid_t x, tid_t y)
{
	పూర్ణాంకం difference = (x - y);
	తిరిగి (difference >= 0);
}
#అయితే_ముగింపు /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#నిర్వచనం_రద్దు _INLINE_

బాహ్య పూర్ణాంకం journal_ఖండాలు_చొప్పున_పుట(నిర్మాణం inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#నిర్వచించు BJ_None		0	/* Not journaled */
#నిర్వచించు BJ_Sync_దత్తాంశం	1	/* Normal data: flush before commit */
#నిర్వచించు BJ_Async_దత్తాంశం	2	/* writepage data: wait on it before commit */
#నిర్వచించు BJ_Metadata	3	/* Normal journaled metadata */
#నిర్వచించు BJ_Forget	4	/* Buffer superseded by this transaction */
#నిర్వచించు BJ_IO		5	/* Buffer is for temporary IO use */
#నిర్వచించు BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#నిర్వచించు BJ_LogCtl	7	/* Buffer contains log descriptors */
#నిర్వచించు BJ_Reserved	8	/* Buffer is reserved for access by journal */
#నిర్వచించు BJ_Types	9

బాహ్య పూర్ణాంకం jbd_ఖండాలు_చొప్పున_పుట(నిర్మాణం inode *inode);

#అయితే_ముగింపు	/* _LINUX_JBD_H */
