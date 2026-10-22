/* SPDX-License-Identifier: GPL-2.0 */

#నిర్వచితం_కాకపోతే __FAST_COMMIT_H__
#నిర్వచించు __FAST_COMMIT_H__

#అన్తర్భావ "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#నిర్వచించు EXT4_FC_ట్యాగ్_కూడు_RANGE		0x0001
#నిర్వచించు EXT4_FC_ట్యాగ్_DEL_RANGE		0x0002
#నిర్వచించు EXT4_FC_ట్యాగ్_CREAT		0x0003
#నిర్వచించు EXT4_FC_ట్యాగ్_LINK		0x0004
#నిర్వచించు EXT4_FC_ట్యాగ్_UNLINK		0x0005
#నిర్వచించు EXT4_FC_ట్యాగ్_INODE		0x0006
#నిర్వచించు EXT4_FC_ట్యాగ్_PAD			0x0007
#నిర్వచించు EXT4_FC_ట్యాగ్_తోక		0x0008
#నిర్వచించు EXT4_FC_ట్యాగ్_తల		0x0009

#నిర్వచించు EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
నిర్మాణం ext4_fc_tl {
	__le16 fc_ట్యాగ్;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
నిర్మాణం ext4_fc_తల {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
నిర్మాణం ext4_fc_కూడు_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
నిర్మాణం ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
నిర్మాణం ext4_fc_dentry_info {
	__le32 fc_మాతృ_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
నిర్మాణం ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
నిర్మాణం ext4_fc_తోక {
	__le32 fc_tid;
	__le32 fc_చక్రీయ_తనిఖీ;
};

/*
 * Fast commit reason codes
 */
గణన {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_ప్రారంభం_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_మార్పిడి_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_దత్తాంశం,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#నిర్వచితమైతే __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
నిర్మాణం ext4_fc_dentry_update {
	పూర్ణాంకం fcd_op;		/* Type of update create / unlink / link */
	పూర్ణాంకం fcd_మాతృ;		/* Parent inode number */
	పూర్ణాంకం fcd_ino;		/* Inode number */
	నిర్మాణం qstr fcd_name;	/* Dirent name */
	గుర్తులేని అక్షరం fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	నిర్మాణం list_తల fcd_list;
};

నిర్మాణం ext4_fc_stats {
	గుర్తులేని పూర్ణాంకం fc_ineligible_reason_లెక్క[EXT4_FC_REASON_MAX];
	గుర్తులేని దీర్ఘ fc_అంకె_commits;
	గుర్తులేని దీర్ఘ fc_ineligible_commits;
	గుర్తులేని దీర్ఘ fc_numblks;
};

#నిర్వచించు EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
నిర్మాణం ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	పూర్ణాంకం ino, len;
};

/*
 * Fast commit replay state.
 */
నిర్మాణం ext4_fc_replay_state {
	పూర్ణాంకం fc_replay_అంకె_ట్యాగులు;
	పూర్ణాంకం fc_replay_expected_off;
	పూర్ణాంకం fc_current_pass;
	పూర్ణాంకం fc_cur_ట్యాగ్;
	పూర్ణాంకం fc_చక్రీయ_తనిఖీ;
	నిర్మాణం ext4_fc_alloc_region *fc_regions;
	పూర్ణాంకం fc_regions_కొలత, fc_regions_వాడిన, fc_regions_valid;
	పూర్ణాంకం *fc_modified_inodes;
	పూర్ణాంకం fc_modified_inodes_వాడిన, fc_modified_inodes_కొలత;
};

#నిర్వచించు region_చివరి(__region) (((__region)->lblk) + ((__region)->len) - 1)
#అయితే_ముగింపు

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *tag2str(__u16 ట్యాగ్)
{
	ఎంపిక (ట్యాగ్) {
	స్థితి EXT4_FC_ట్యాగ్_LINK:
		తిరిగి "ADD_ENTRY";
	స్థితి EXT4_FC_ట్యాగ్_UNLINK:
		తిరిగి "DEL_ENTRY";
	స్థితి EXT4_FC_ట్యాగ్_కూడు_RANGE:
		తిరిగి "ADD_RANGE";
	స్థితి EXT4_FC_ట్యాగ్_CREAT:
		తిరిగి "CREAT_DENTRY";
	స్థితి EXT4_FC_ట్యాగ్_DEL_RANGE:
		తిరిగి "DEL_RANGE";
	స్థితి EXT4_FC_ట్యాగ్_INODE:
		తిరిగి "INODE";
	స్థితి EXT4_FC_ట్యాగ్_PAD:
		తిరిగి "PAD";
	స్థితి EXT4_FC_ట్యాగ్_తోక:
		తిరిగి "TAIL";
	స్థితి EXT4_FC_ట్యాగ్_తల:
		తిరిగి "HEAD";
	అప్రమేయ:
		తిరిగి "ERROR";
	}
}

/* Get length of a particular tlv */
స్థిర అంతఃవరుస పూర్ణాంకం ext4_fc_ట్యాగ్_len(నిర్మాణం ext4_fc_tl *tl)
{
	తిరిగి le16_గమ్యం_cpu(tl->fc_len);
}

#అయితే_ముగింపు /* __FAST_COMMIT_H__ */
