/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __FAST_COMMIT_H__
#define __FAST_COMMIT_H__

#અન્તર્ભાવ "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#define EXT4_FC_TAG_ADD_RANGE		0x0001
#define EXT4_FC_TAG_DEL_RANGE		0x0002
#define EXT4_FC_TAG_CREAT		0x0003
#define EXT4_FC_TAG_LINK		0x0004
#define EXT4_FC_TAG_UNLINK		0x0005
#define EXT4_FC_TAG_INODE		0x0006
#define EXT4_FC_TAG_PAD			0x0007
#define EXT4_FC_TAG_TAIL		0x0008
#define EXT4_FC_TAG_HEAD		0x0009

#define EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
સંરચના ext4_fc_tl {
	__le16 fc_tag;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
સંરચના ext4_fc_head {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
સંરચના ext4_fc_add_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
સંરચના ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
સંરચના ext4_fc_dentry_info {
	__le32 fc_parent_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
સંરચના ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
સંરચના ext4_fc_tail {
	__le32 fc_tid;
	__le32 fc_crc;
};

/*
 * Fast commit reason codes
 */
ગણના {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_START_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_SWAP_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_DATA,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#ifdef __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
સંરચના ext4_fc_dentry_update {
	પૂર્ણાંક fcd_op;		/* Type of update create / unlink / link */
	પૂર્ણાંક fcd_parent;		/* Parent inode number */
	પૂર્ણાંક fcd_ino;		/* Inode number */
	સંરચના qstr fcd_name;	/* Dirent name */
	unsigned અક્ષર fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	સંરચના list_head fcd_list;
};

સંરચના ext4_fc_stats {
	unsigned પૂર્ણાંક fc_ineligible_reason_ગણતરી[EXT4_FC_REASON_MAX];
	unsigned long fc_અંક_commits;
	unsigned long fc_ineligible_commits;
	unsigned long fc_numblks;
};

#define EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
સંરચના ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	પૂર્ણાંક ino, len;
};

/*
 * Fast commit replay state.
 */
સંરચના ext4_fc_replay_state {
	પૂર્ણાંક fc_replay_અંક_tags;
	પૂર્ણાંક fc_replay_expected_off;
	પૂર્ણાંક fc_current_pass;
	પૂર્ણાંક fc_cur_tag;
	પૂર્ણાંક fc_crc;
	સંરચના ext4_fc_alloc_region *fc_regions;
	પૂર્ણાંક fc_regions_size, fc_regions_used, fc_regions_valid;
	પૂર્ણાંક *fc_modified_inodes;
	પૂર્ણાંક fc_modified_inodes_used, fc_modified_inodes_size;
};

#define region_last(__region) (((__region)->lblk) + ((__region)->len) - 1)
#endif

સ્થિર અંતઃરેખ અચલ અક્ષર *tag2str(__u16 tag)
{
	પસંદગી (tag) {
	સ્થિતિ EXT4_FC_TAG_LINK:
		પાછા "ADD_ENTRY";
	સ્થિતિ EXT4_FC_TAG_UNLINK:
		પાછા "DEL_ENTRY";
	સ્થિતિ EXT4_FC_TAG_ADD_RANGE:
		પાછા "ADD_RANGE";
	સ્થિતિ EXT4_FC_TAG_CREAT:
		પાછા "CREAT_DENTRY";
	સ્થિતિ EXT4_FC_TAG_DEL_RANGE:
		પાછા "DEL_RANGE";
	સ્થિતિ EXT4_FC_TAG_INODE:
		પાછા "INODE";
	સ્થિતિ EXT4_FC_TAG_PAD:
		પાછા "PAD";
	સ્થિતિ EXT4_FC_TAG_TAIL:
		પાછા "TAIL";
	સ્થિતિ EXT4_FC_TAG_HEAD:
		પાછા "HEAD";
	મૂળભૂત:
		પાછા "ERROR";
	}
}

/* Get length of a particular tlv */
સ્થિર અંતઃરેખ પૂર્ણાંક ext4_fc_tag_len(સંરચના ext4_fc_tl *tl)
{
	પાછા le16_to_cpu(tl->fc_len);
}

#endif /* __FAST_COMMIT_H__ */
