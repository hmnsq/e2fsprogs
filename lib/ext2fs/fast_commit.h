/* SPDX-License-Identifier: GPL-2.0 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __FAST_COMMIT_H__
#ವ್ಯಾಖ್ಯಾನಿಸು __FAST_COMMIT_H__

#ಅನ್ತರ್ಭಾವ "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_ಕೂಡಿಸು_RANGE		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_DEL_RANGE		0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_CREAT		0x0003
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_LINK		0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_UNLINK		0x0005
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_INODE		0x0006
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_PAD			0x0007
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_ಬಾಲ		0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_ಟ್ಯಾಗ್_ತಲೆ		0x0009

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
ರಚನೆ ext4_fc_tl {
	__le16 fc_ಟ್ಯಾಗ್;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
ರಚನೆ ext4_fc_ತಲೆ {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
ರಚನೆ ext4_fc_ಕೂಡಿಸು_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
ರಚನೆ ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
ರಚನೆ ext4_fc_dentry_info {
	__le32 fc_ಪೋಷಕ_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
ರಚನೆ ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
ರಚನೆ ext4_fc_ಬಾಲ {
	__le32 fc_tid;
	__le32 fc_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
};

/*
 * Fast commit reason codes
 */
ಎಣಿಕೆ {
	/*
	 * Commit status codes:
	 */
	EXT4_FC_REASON_OK = 0,
	EXT4_FC_REASON_INELIGIBLE,
	EXT4_FC_REASON_ALREADY_COMMITTED,
	EXT4_FC_REASON_FC_ಆರಂಭ_FAILED,
	EXT4_FC_REASON_FC_FAILED,

	/*
	 * Fast commit ineligiblity reasons:
	 */
	EXT4_FC_REASON_XATTR = 0,
	EXT4_FC_REASON_CROSS_RENAME,
	EXT4_FC_REASON_JOURNAL_FLAG_CHANGE,
	EXT4_FC_REASON_NOMEM,
	EXT4_FC_REASON_ಅದಲುಬದಲು_BOOT,
	EXT4_FC_REASON_RESIZE,
	EXT4_FC_REASON_RENAME_DIR,
	EXT4_FC_REASON_FALLOC_RANGE,
	EXT4_FC_REASON_INODE_JOURNAL_ದತ್ತಾಂಶ,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
ರಚನೆ ext4_fc_dentry_update {
	ಪೂರ್ಣಾಂಕ fcd_op;		/* Type of update create / unlink / link */
	ಪೂರ್ಣಾಂಕ fcd_ಪೋಷಕ;		/* Parent inode number */
	ಪೂರ್ಣಾಂಕ fcd_ino;		/* Inode number */
	ರಚನೆ qstr fcd_name;	/* Dirent name */
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	ರಚನೆ list_ತಲೆ fcd_list;
};

ರಚನೆ ext4_fc_stats {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ fc_ineligible_reason_ಎಣಿಕೆ[EXT4_FC_REASON_MAX];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fc_ಅಂಕೆ_commits;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fc_ineligible_commits;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fc_numblks;
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
ರಚನೆ ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	ಪೂರ್ಣಾಂಕ ino, len;
};

/*
 * Fast commit replay state.
 */
ರಚನೆ ext4_fc_replay_state {
	ಪೂರ್ಣಾಂಕ fc_replay_ಅಂಕೆ_ಟ್ಯಾಗುಗಳು;
	ಪೂರ್ಣಾಂಕ fc_replay_expected_off;
	ಪೂರ್ಣಾಂಕ fc_current_pass;
	ಪೂರ್ಣಾಂಕ fc_cur_ಟ್ಯಾಗ್;
	ಪೂರ್ಣಾಂಕ fc_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ರಚನೆ ext4_fc_alloc_region *fc_regions;
	ಪೂರ್ಣಾಂಕ fc_regions_ಗಾತ್ರ, fc_regions_ಬಳಸಿದ, fc_regions_valid;
	ಪೂರ್ಣಾಂಕ *fc_modified_inodes;
	ಪೂರ್ಣಾಂಕ fc_modified_inodes_ಬಳಸಿದ, fc_modified_inodes_ಗಾತ್ರ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು region_ಕೊನೆಯ(__region) (((__region)->lblk) + ((__region)->len) - 1)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tag2str(__u16 ಟ್ಯಾಗ್)
{
	ಆಯ್ಕೆ (ಟ್ಯಾಗ್) {
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_LINK:
		ಹಿಂತಿರುಗಿ "ADD_ENTRY";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_UNLINK:
		ಹಿಂತಿರುಗಿ "DEL_ENTRY";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_ಕೂಡಿಸು_RANGE:
		ಹಿಂತಿರುಗಿ "ADD_RANGE";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_CREAT:
		ಹಿಂತಿರುಗಿ "CREAT_DENTRY";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_DEL_RANGE:
		ಹಿಂತಿರುಗಿ "DEL_RANGE";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_INODE:
		ಹಿಂತಿರುಗಿ "INODE";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_PAD:
		ಹಿಂತಿರುಗಿ "PAD";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_ಬಾಲ:
		ಹಿಂತಿರುಗಿ "TAIL";
	ಸ್ಥಿತಿ EXT4_FC_ಟ್ಯಾಗ್_ತಲೆ:
		ಹಿಂತಿರುಗಿ "HEAD";
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ "ERROR";
	}
}

/* Get length of a particular tlv */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext4_fc_ಟ್ಯಾಗ್_len(ರಚನೆ ext4_fc_tl *tl)
{
	ಹಿಂತಿರುಗಿ le16_ಗಮ್ಯ_cpu(tl->fc_len);
}

#ಆದರೆ_ಅಂತ್ಯ /* __FAST_COMMIT_H__ */
