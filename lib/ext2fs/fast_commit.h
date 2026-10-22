/* SPDX-License-Identifier: GPL-2.0 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __FAST_COMMIT_H__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __FAST_COMMIT_H__

#ਅਨ੍ਤਰ੍ਭਾਵ "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_ਜੋੜੋ_RANGE		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_DEL_RANGE		0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_CREAT		0x0003
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_LINK		0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_UNLINK		0x0005
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_INODE		0x0006
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_PAD			0x0007
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_ਪੂਛ		0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_ਟੈਗ_ਸਿਰ		0x0009

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
ਬਣਤਰ ext4_fc_tl {
	__le16 fc_ਟੈਗ;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
ਬਣਤਰ ext4_fc_ਸਿਰ {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
ਬਣਤਰ ext4_fc_ਜੋੜੋ_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
ਬਣਤਰ ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
ਬਣਤਰ ext4_fc_dentry_info {
	__le32 fc_ਮਾਪਾ_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
ਬਣਤਰ ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
ਬਣਤਰ ext4_fc_ਪੂਛ {
	__le32 fc_tid;
	__le32 fc_ਚੱਕਰੀ_ਜਾਂਚ;
};

/*
 * Fast commit reason codes
 */
ਗਿਣਤੀ {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_ਸ਼ੁਰੂ_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_ਅਦਲਾ_ਬਦਲੀ_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_ਅੰਕੜੇ,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
ਬਣਤਰ ext4_fc_dentry_update {
	ਪੂਰਨ fcd_op;		/* Type of update create / unlink / link */
	ਪੂਰਨ fcd_ਮਾਪਾ;		/* Parent inode number */
	ਪੂਰਨ fcd_ino;		/* Inode number */
	ਬਣਤਰ qstr fcd_name;	/* Dirent name */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	ਬਣਤਰ list_ਸਿਰ fcd_list;
};

ਬਣਤਰ ext4_fc_stats {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fc_ineligible_reason_ਗਿਣਤੀ[EXT4_FC_REASON_MAX];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fc_ਅੰਕ_commits;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fc_ineligible_commits;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fc_numblks;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
ਬਣਤਰ ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	ਪੂਰਨ ino, len;
};

/*
 * Fast commit replay state.
 */
ਬਣਤਰ ext4_fc_replay_state {
	ਪੂਰਨ fc_replay_ਅੰਕ_ਟੈਗ_ਸੂਚੀ;
	ਪੂਰਨ fc_replay_expected_off;
	ਪੂਰਨ fc_current_pass;
	ਪੂਰਨ fc_cur_ਟੈਗ;
	ਪੂਰਨ fc_ਚੱਕਰੀ_ਜਾਂਚ;
	ਬਣਤਰ ext4_fc_alloc_region *fc_regions;
	ਪੂਰਨ fc_regions_ਮਾਪ, fc_regions_ਵਰਤਿਆ, fc_regions_valid;
	ਪੂਰਨ *fc_modified_inodes;
	ਪੂਰਨ fc_modified_inodes_ਵਰਤਿਆ, fc_modified_inodes_ਮਾਪ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ region_ਆਖਰੀ(__region) (((__region)->lblk) + ((__region)->len) - 1)
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *tag2str(__u16 ਟੈਗ)
{
	ਚੋਣ (ਟੈਗ) {
	ਹਾਲਤ EXT4_FC_ਟੈਗ_LINK:
		ਵਾਪਸ "ADD_ENTRY";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_UNLINK:
		ਵਾਪਸ "DEL_ENTRY";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_ਜੋੜੋ_RANGE:
		ਵਾਪਸ "ADD_RANGE";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_CREAT:
		ਵਾਪਸ "CREAT_DENTRY";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_DEL_RANGE:
		ਵਾਪਸ "DEL_RANGE";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_INODE:
		ਵਾਪਸ "INODE";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_PAD:
		ਵਾਪਸ "PAD";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_ਪੂਛ:
		ਵਾਪਸ "TAIL";
	ਹਾਲਤ EXT4_FC_ਟੈਗ_ਸਿਰ:
		ਵਾਪਸ "HEAD";
	ਮੂਲ:
		ਵਾਪਸ "ERROR";
	}
}

/* Get length of a particular tlv */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext4_fc_ਟੈਗ_len(ਬਣਤਰ ext4_fc_tl *tl)
{
	ਵਾਪਸ le16_ਮੰਜ਼ਿਲ_cpu(tl->fc_len);
}

#ਜੇ_ਅੰਤ /* __FAST_COMMIT_H__ */
