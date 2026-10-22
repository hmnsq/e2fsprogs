/* SPDX-License-Identifier: GPL-2.0 */

#جے_بنا_تعریف __FAST_COMMIT_H__
#تعریف_کرو __FAST_COMMIT_H__

#ضمن "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#تعریف_کرو EXT4_FC_ٹیگ_جوڑو_RANGE		0x0001
#تعریف_کرو EXT4_FC_ٹیگ_DEL_RANGE		0x0002
#تعریف_کرو EXT4_FC_ٹیگ_CREAT		0x0003
#تعریف_کرو EXT4_FC_ٹیگ_LINK		0x0004
#تعریف_کرو EXT4_FC_ٹیگ_UNLINK		0x0005
#تعریف_کرو EXT4_FC_ٹیگ_INODE		0x0006
#تعریف_کرو EXT4_FC_ٹیگ_PAD			0x0007
#تعریف_کرو EXT4_FC_ٹیگ_پوچھ		0x0008
#تعریف_کرو EXT4_FC_ٹیگ_سر		0x0009

#تعریف_کرو EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
بنية ext4_fc_tl {
	__le16 fc_ٹیگ;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
بنية ext4_fc_سر {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
بنية ext4_fc_جوڑو_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
بنية ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
بنية ext4_fc_dentry_info {
	__le32 fc_ماپا_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
بنية ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
بنية ext4_fc_پوچھ {
	__le32 fc_tid;
	__le32 fc_چکری_جانچ;
};

/*
 * Fast commit reason codes
 */
تعداد {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_شروع_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_وٹاندرا_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_انکڑے,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#جے_تعریف_شدہ __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
بنية ext4_fc_dentry_update {
	صحيح fcd_op;		/* Type of update create / unlink / link */
	صحيح fcd_ماپا;		/* Parent inode number */
	صحيح fcd_ino;		/* Inode number */
	بنية qstr fcd_name;	/* Dirent name */
	بنانشانی حرف fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	بنية list_سر fcd_list;
};

بنية ext4_fc_stats {
	بنانشانی صحيح fc_ineligible_reason_گنتی[EXT4_FC_REASON_MAX];
	بنانشانی لما fc_عدد_commits;
	بنانشانی لما fc_ineligible_commits;
	بنانشانی لما fc_numblks;
};

#تعریف_کرو EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
بنية ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	صحيح ino, len;
};

/*
 * Fast commit replay state.
 */
بنية ext4_fc_replay_state {
	صحيح fc_replay_عدد_ٹیگ_سوچی;
	صحيح fc_replay_expected_off;
	صحيح fc_current_pass;
	صحيح fc_cur_ٹیگ;
	صحيح fc_چکری_جانچ;
	بنية ext4_fc_alloc_region *fc_regions;
	صحيح fc_regions_ناپ, fc_regions_ورتیا, fc_regions_valid;
	صحيح *fc_modified_inodes;
	صحيح fc_modified_inodes_ورتیا, fc_modified_inodes_ناپ;
};

#تعریف_کرو region_آخری(__region) (((__region)->lblk) + ((__region)->len) - 1)
#جے_انت

ساكن مضمن ثابت حرف *tag2str(__u16 ٹیگ)
{
	بدل (ٹیگ) {
	حالة EXT4_FC_ٹیگ_LINK:
		أرجع "ADD_ENTRY";
	حالة EXT4_FC_ٹیگ_UNLINK:
		أرجع "DEL_ENTRY";
	حالة EXT4_FC_ٹیگ_جوڑو_RANGE:
		أرجع "ADD_RANGE";
	حالة EXT4_FC_ٹیگ_CREAT:
		أرجع "CREAT_DENTRY";
	حالة EXT4_FC_ٹیگ_DEL_RANGE:
		أرجع "DEL_RANGE";
	حالة EXT4_FC_ٹیگ_INODE:
		أرجع "INODE";
	حالة EXT4_FC_ٹیگ_PAD:
		أرجع "PAD";
	حالة EXT4_FC_ٹیگ_پوچھ:
		أرجع "TAIL";
	حالة EXT4_FC_ٹیگ_سر:
		أرجع "HEAD";
	افتراضي:
		أرجع "ERROR";
	}
}

/* Get length of a particular tlv */
ساكن مضمن صحيح ext4_fc_ٹیگ_len(بنية ext4_fc_tl *tl)
{
	أرجع le16_منزل_cpu(tl->fc_len);
}

#جے_انت /* __FAST_COMMIT_H__ */
