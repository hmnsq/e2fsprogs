/* SPDX-License-Identifier: GPL-2.0 */

#اگر_غیر_تعریف_شدہ __FAST_COMMIT_H__
#تعریف_کرو __FAST_COMMIT_H__

#شامل "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#تعریف_کرو EXT4_FC_لیبل_جوڑو_RANGE		0x0001
#تعریف_کرو EXT4_FC_لیبل_DEL_RANGE		0x0002
#تعریف_کرو EXT4_FC_لیبل_CREAT		0x0003
#تعریف_کرو EXT4_FC_لیبل_LINK		0x0004
#تعریف_کرو EXT4_FC_لیبل_UNLINK		0x0005
#تعریف_کرو EXT4_FC_لیبل_INODE		0x0006
#تعریف_کرو EXT4_FC_لیبل_PAD			0x0007
#تعریف_کرو EXT4_FC_لیبل_دم		0x0008
#تعریف_کرو EXT4_FC_لیبل_سر		0x0009

#تعریف_کرو EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
ساخت ext4_fc_tl {
	__le16 fc_لیبل;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
ساخت ext4_fc_سر {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
ساخت ext4_fc_جوڑو_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
ساخت ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
ساخت ext4_fc_dentry_info {
	__le32 fc_والد_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
ساخت ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
ساخت ext4_fc_دم {
	__le32 fc_tid;
	__le32 fc_چکری_جانچ;
};

/*
 * Fast commit reason codes
 */
شمار {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_آغاز_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_تبادلہ_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_ڈیٹا,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#اگر_تعریف_شدہ __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
ساخت ext4_fc_dentry_update {
	صحیح fcd_op;		/* Type of update create / unlink / link */
	صحیح fcd_والد;		/* Parent inode number */
	صحیح fcd_ino;		/* Inode number */
	ساخت qstr fcd_name;	/* Dirent name */
	غیرعلامتی حرف fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	ساخت list_سر fcd_list;
};

ساخت ext4_fc_stats {
	غیرعلامتی صحیح fc_ineligible_reason_گنتی[EXT4_FC_REASON_MAX];
	غیرعلامتی طویل fc_عدد_commits;
	غیرعلامتی طویل fc_ineligible_commits;
	غیرعلامتی طویل fc_numblks;
};

#تعریف_کرو EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
ساخت ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	صحیح ino, len;
};

/*
 * Fast commit replay state.
 */
ساخت ext4_fc_replay_state {
	صحیح fc_replay_عدد_لیبل_فہرست;
	صحیح fc_replay_expected_off;
	صحیح fc_current_pass;
	صحیح fc_cur_لیبل;
	صحیح fc_چکری_جانچ;
	ساخت ext4_fc_alloc_region *fc_regions;
	صحیح fc_regions_سائز, fc_regions_مستعمل, fc_regions_valid;
	صحیح *fc_modified_inodes;
	صحیح fc_modified_inodes_مستعمل, fc_modified_inodes_سائز;
};

#تعریف_کرو region_آخری(__region) (((__region)->lblk) + ((__region)->len) - 1)
#اگر_ختم

ساکن اندرونی ثابت حرف *tag2str(__u16 لیبل)
{
	انتخاب (لیبل) {
	حالت EXT4_FC_لیبل_LINK:
		واپس "ADD_ENTRY";
	حالت EXT4_FC_لیبل_UNLINK:
		واپس "DEL_ENTRY";
	حالت EXT4_FC_لیبل_جوڑو_RANGE:
		واپس "ADD_RANGE";
	حالت EXT4_FC_لیبل_CREAT:
		واپس "CREAT_DENTRY";
	حالت EXT4_FC_لیبل_DEL_RANGE:
		واپس "DEL_RANGE";
	حالت EXT4_FC_لیبل_INODE:
		واپس "INODE";
	حالت EXT4_FC_لیبل_PAD:
		واپس "PAD";
	حالت EXT4_FC_لیبل_دم:
		واپس "TAIL";
	حالت EXT4_FC_لیبل_سر:
		واپس "HEAD";
	طےشدہ:
		واپس "ERROR";
	}
}

/* Get length of a particular tlv */
ساکن اندرونی صحیح ext4_fc_لیبل_len(ساخت ext4_fc_tl *tl)
{
	واپس le16_مقصد_cpu(tl->fc_len);
}

#اگر_ختم /* __FAST_COMMIT_H__ */
