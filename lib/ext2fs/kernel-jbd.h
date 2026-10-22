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

#اگر_تعریف_نشده _LINUX_JBD_H
#تعریف_کن _LINUX_JBD_H

#ضمن "jfs_compat.h"
#تعریف_کن JFS_DEBUG
#تعریف_کن jfs_debug jbd_debug

#اگر_تعریف_نشده __GNUC__
#تعریف_کن __FUNCTION__ ""
#پایان_اگر

#تعریف_کن journal_oom_retry 0

#اگر_تعریف_شده CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#تعریف_کن JBD_EXPENSIVE_CHECKING
خارجي صحيح journal_enable_debug;
#وإلا
#تعریف_کن journal_enable_debug (-1)
#پایان_اگر /* !CONFIG_JBD_DEBUG */

#اگر_تعریف_شده __STDC__
#تعریف_کن jbd_debug(n, f, a...)						\
	افعل {								\
		إذا ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} طالما (0)
#وإلا
#تعریف_کن jbd_debug(x)		/* AIX doesn't do STDC */
#پایان_اگر /* !__STDC__ */

خارجي فراغ * __jbd_kmalloc (حرف *where, size_t اندازه, صحيح flags, صحيح retry);
#تعریف_کن jbd_kmalloc(اندازه, flags) \
	__jbd_kmalloc(__FUNCTION__, (اندازه), (flags), journal_oom_retry)
#تعریف_کن jbd_rep_kmalloc(اندازه, flags) \
	__jbd_kmalloc(__FUNCTION__, (اندازه), (flags), 1)

#تعریف_کن JBD2_MIN_JOURNAL_بلوکها 1024
#تعریف_کن JBD2_MAX_JOURNAL_بلوکها (JBD2_MIN_JOURNAL_بلوکها * 10000)
#تعریف_کن JBD2_DEFAULT_FAST_COMMIT_بلوکها 256

/*
 * Internal structures used by the logging mechanism:
 */

#تعریف_کن JBD2_MAGIC_عدد 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#تعریف_کن JBD2_DESCRIPTOR_بلوک	1
#تعریف_کن JBD2_COMMIT_بلوک	2
#تعریف_کن JBD2_SUPERBLOCK_V1	3
#تعریف_کن JBD2_SUPERBLOCK_V2	4
#تعریف_کن JBD2_REVOKE_بلوک	5
#تعریف_کن JBD2_FC_بلوک		6

/*
 * Standard header for all descriptor blocks:
 */
تعريفنوع بنية journal_سرآیند_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_سرآیند_t;

/*
 * Checksum types.
 */
#تعریف_کن JBD2_بررسی_چرخهای_32_CHKSUM   1
#تعریف_کن JBD2_MD5_CHKSUM     2
#تعریف_کن JBD2_SHA1_CHKSUM    3
#تعریف_کن JBD2_بررسی_چرخهای_32_C_CHKSUM  4

#تعریف_کن JBD2_بررسی_چرخهای_32_CHKSUM_اندازه 4

#تعریف_کن JBD2_CHECKSUM_بایتها (32 / حجم(__u32))
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
بنية commit_سرآیند {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	بیعلامت حرف	h_chksum_type;
	بیعلامت حرف	h_chksum_اندازه;
	بیعلامت حرف	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_بایتها];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
تعريفنوع بنية journal_بلوک_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_بالا; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_بلوک_tag3_t;

تعريفنوع بنية journal_بلوک_برچسب_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_بالا; /* most-significant high 32bits. */
} journal_بلوک_برچسب_t;

/* Tail of descriptor or revoke block, for checksumming */
بنية jbd2_journal_بلوک_دم {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
تعريفنوع بنية journal_revoke_سرآیند_s
{
	journal_سرآیند_t r_سرآیند;
	__be32		 r_شمار;	/* Count of bytes used in the block */
} jbd2_journal_revoke_سرآیند_t;

/* Definitions for the journal tag flags word: */
#تعریف_کن JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#تعریف_کن JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#تعریف_کن JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#تعریف_کن JBD2_FLAG_آخرین_برچسب	8	/* last tag in this descriptor block */


#تعریف_کن UUID_اندازه 16
#تعریف_کن JBD2_USERS_MAX 48
#تعریف_کن JBD2_USERS_اندازه (UUID_اندازه * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
تعريفنوع بنية journal_superblock_s
{
/* 0x0000 */
	journal_سرآیند_t s_سرآیند;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_اولی;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_شروع;		/* blocknr of start of log */

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
	__be32	s_max_trans_داده;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_رقم_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_اندازه];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#تعریف_کن JBD2_HAS_COMPAT_FEATURE(j,ماسک)					\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_مقصد_be32((ماسک))))
#تعریف_کن JBD2_HAS_RO_COMPAT_FEATURE(j,ماسک)				\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_مقصد_be32((ماسک))))
#تعریف_کن JBD2_HAS_INCOMPAT_FEATURE(j,ماسک)				\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_مقصد_be32((ماسک))))

#تعریف_کن JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#تعریف_کن JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#تعریف_کن JBD2_FEATURE_INCOMPAT_64_بیت		0x00000002
#تعریف_کن JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#تعریف_کن JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#تعریف_کن JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#تعریف_کن JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#تعریف_کن JBD2_معلوم_COMPAT_FEATURES	0
#تعریف_کن JBD2_معلوم_ROCOMPAT_FEATURES	0
#تعریف_کن JBD2_معلوم_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_بیت|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#اگر_تعریف_شده NO_INLINE_FUNCS
خارجي size_t journal_برچسب_بایتها(journal_t *journal);
خارجي صحيح jbd2_journal_has_csum_v2or3(journal_t *journal);
خارجي صحيح jbd2_journal_get_رقم_fc_blks(journal_superblock_t *jsb);
خارجي صحيح tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((استفادهنشده));
خارجي صحيح tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((استفادهنشده));
#پایان_اگر

#إذا (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#اگر_تعریف_شده E2FSCK_INCLUDE_INLINE_FUNCS
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کن _INLINE_ خارجي مضمن
#وإلا
#تعریف_کن _INLINE_ مضمن
#پایان_اگر
#وإلا /* !E2FSCK_INCLUDE_INLINE FUNCS */
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کن _INLINE_ مضمن
#وإلا /* not C99 */
#اگر_تعریف_شده __GNUC__
#تعریف_کن _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#تعریف_کن _INLINE_ خارجي مضمن
#پایان_اگر /* __GNUC__ */
#پایان_اگر /* __STDC_VERSION__ >= 199901L */
#پایان_اگر /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#تعریف_کن JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#تعریف_کن JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#تعریف_کن JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_مجموعه_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#وإلا
#تعریف_کن JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_مجموعه_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#تعریف_کن JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_مجموعه_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#تعریف_کن JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_مجموعه_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#پایان_اگر /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64بیت,		64بیت_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#إذا (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_برچسب_بایتها(journal_t *journal)
{
	size_t sz;

	إذا (jbd2_has_feature_csum3(journal))
		أرجع حجم(journal_بلوک_tag3_t);

	sz = حجم(journal_بلوک_برچسب_t);

	إذا (jbd2_has_feature_csum2(journal))
		sz += حجم(__u16);

	إذا (jbd2_has_feature_64bit(journal))
		أرجع sz;

	أرجع sz - حجم(__u32);
}

_INLINE_ صحيح jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	إذا (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		أرجع 1;

	أرجع 0;
}

_INLINE_ صحيح jbd2_journal_get_رقم_fc_blks(journal_superblock_t *jsb)
{
	صحيح رقم_fc_بلوکها = be32_مقصد_cpu(jsb->s_رقم_fc_blks);

	أرجع رقم_fc_بلوکها ? رقم_fc_بلوکها : JBD2_DEFAULT_FAST_COMMIT_بلوکها;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ صحيح tid_gt(tid_t x, tid_t y)
{
	صحيح difference = (x - y);
	أرجع (difference > 0);
}

_INLINE_ صحيح tid_geq(tid_t x, tid_t y)
{
	صحيح difference = (x - y);
	أرجع (difference >= 0);
}
#پایان_اگر /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#لغو_تعریف _INLINE_

خارجي صحيح journal_بلوکها_در_هر_صفحه(بنية inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#تعریف_کن BJ_None		0	/* Not journaled */
#تعریف_کن BJ_Sync_داده	1	/* Normal data: flush before commit */
#تعریف_کن BJ_Async_داده	2	/* writepage data: wait on it before commit */
#تعریف_کن BJ_Metadata	3	/* Normal journaled metadata */
#تعریف_کن BJ_Forget	4	/* Buffer superseded by this transaction */
#تعریف_کن BJ_IO		5	/* Buffer is for temporary IO use */
#تعریف_کن BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#تعریف_کن BJ_LogCtl	7	/* Buffer contains log descriptors */
#تعریف_کن BJ_Reserved	8	/* Buffer is reserved for access by journal */
#تعریف_کن BJ_Types	9

خارجي صحيح jbd_بلوکها_در_هر_صفحه(بنية inode *inode);

#پایان_اگر	/* _LINUX_JBD_H */
