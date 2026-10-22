/* SPDX-License-Identifier: GPL-2.0 */

#अगर_अघोषित __FAST_COMMIT_H__
#घोषणा __FAST_COMMIT_H__

#समावेश "jfs_compat.h"

/*
 * Note this file is present in e2fsprogs/lib/ext2fs/fast_commit.h and
 * linux/fs/ext4/fast_commit.h. These file should always be byte identical.
 */

/* Fast commit tags */
#घोषणा EXT4_FC_TAG_ADD_RANGE		0x0001
#घोषणा EXT4_FC_TAG_DEL_RANGE		0x0002
#घोषणा EXT4_FC_TAG_CREAT		0x0003
#घोषणा EXT4_FC_TAG_LINK		0x0004
#घोषणा EXT4_FC_TAG_UNLINK		0x0005
#घोषणा EXT4_FC_TAG_INODE		0x0006
#घोषणा EXT4_FC_TAG_PAD			0x0007
#घोषणा EXT4_FC_TAG_पुच्छ		0x0008
#घोषणा EXT4_FC_TAG_शीर्ष		0x0009

#घोषणा EXT4_FC_SUPPORTED_FEATURES	0x0

/* On disk fast commit tlv value structures */

/* Fast commit on disk tag length structure */
काष्ठा ext4_fc_tl {
	__le16 fc_tag;
	__le16 fc_len;
};

/* Value structure for tag EXT4_FC_TAG_HEAD. */
काष्ठा ext4_fc_शीर्ष {
	__le32 fc_features;
	__le32 fc_tid;
};

/* Value structure for EXT4_FC_TAG_ADD_RANGE. */
काष्ठा ext4_fc_add_range {
	__le32 fc_ino;
	__u8 fc_ex[12];
};

/* Value structure for tag EXT4_FC_TAG_DEL_RANGE. */
काष्ठा ext4_fc_del_range {
	__le32 fc_ino;
	__le32 fc_lblk;
	__le32 fc_len;
};

/*
 * This is the value structure for tags EXT4_FC_TAG_CREAT, EXT4_FC_TAG_LINK
 * and EXT4_FC_TAG_UNLINK.
 */
काष्ठा ext4_fc_dentry_info {
	__le32 fc_parent_ino;
	__le32 fc_ino;
	__u8 fc_dname[0];
};

/* Value structure for EXT4_FC_TAG_INODE and EXT4_FC_TAG_INODE_PARTIAL. */
काष्ठा ext4_fc_inode {
	__le32 fc_ino;
	__u8 fc_raw_inode[0];
};

/* Value structure for tag EXT4_FC_TAG_TAIL. */
काष्ठा ext4_fc_पुच्छ {
	__le32 fc_tid;
	__le32 fc_crc;
};

/*
 * Fast commit reason codes
 */
क्रमागत {
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
	EXT4_FC_REASON_INODE_JOURNAL_तथ्य,
	EXT4_FC_COMMIT_FAILED,
	EXT4_FC_REASON_MAX
};

#अगर_घोषित __KERNEL__
/*
 * In memory list of dentry updates that are performed on the file
 * system used by fast commit code.
 */
काष्ठा ext4_fc_dentry_update {
	पूर्णांक fcd_op;		/* Type of update create / unlink / link */
	पूर्णांक fcd_parent;		/* Parent inode number */
	पूर्णांक fcd_ino;		/* Inode number */
	काष्ठा qstr fcd_name;	/* Dirent name */
	अचिन्हित अक्षर fcd_iname[DNAME_INLINE_LEN];	/* Dirent name string */
	काष्ठा list_शीर्ष fcd_list;
};

काष्ठा ext4_fc_stats {
	अचिन्हित पूर्णांक fc_ineligible_reason_गिनती[EXT4_FC_REASON_MAX];
	अचिन्हित दीर्घ fc_अंक_commits;
	अचिन्हित दीर्घ fc_ineligible_commits;
	अचिन्हित दीर्घ fc_numblks;
};

#घोषणा EXT4_FC_REPLAY_REALLOC_INCREMENT	4

/*
 * Physical block regions added to different inodes due to fast commit
 * recovery. These are set during the SCAN phase. During the replay phase,
 * our allocator excludes these from its allocation. This ensures that
 * we don't accidentally allocating a block that is going to be used by
 * another inode.
 */
काष्ठा ext4_fc_alloc_region {
	ext4_lblk_t lblk;
	ext4_fsblk_t pblk;
	पूर्णांक ino, len;
};

/*
 * Fast commit replay state.
 */
काष्ठा ext4_fc_replay_state {
	पूर्णांक fc_replay_अंक_tags;
	पूर्णांक fc_replay_expected_off;
	पूर्णांक fc_current_pass;
	पूर्णांक fc_cur_tag;
	पूर्णांक fc_crc;
	काष्ठा ext4_fc_alloc_region *fc_regions;
	पूर्णांक fc_regions_आकार, fc_regions_used, fc_regions_valid;
	पूर्णांक *fc_modified_inodes;
	पूर्णांक fc_modified_inodes_used, fc_modified_inodes_आकार;
};

#घोषणा region_last(__region) (((__region)->lblk) + ((__region)->len) - 1)
#पूर्ण_अगर

अटल अंतरभूत स्थिर अक्षर *tag2str(__u16 tag)
{
	चयन (tag) {
	हाल EXT4_FC_TAG_LINK:
		वापस "ADD_ENTRY";
	हाल EXT4_FC_TAG_UNLINK:
		वापस "DEL_ENTRY";
	हाल EXT4_FC_TAG_ADD_RANGE:
		वापस "ADD_RANGE";
	हाल EXT4_FC_TAG_CREAT:
		वापस "CREAT_DENTRY";
	हाल EXT4_FC_TAG_DEL_RANGE:
		वापस "DEL_RANGE";
	हाल EXT4_FC_TAG_INODE:
		वापस "INODE";
	हाल EXT4_FC_TAG_PAD:
		वापस "PAD";
	हाल EXT4_FC_TAG_पुच्छ:
		वापस "TAIL";
	हाल EXT4_FC_TAG_शीर्ष:
		वापस "HEAD";
	शेष:
		वापस "ERROR";
	}
}

/* Get length of a particular tlv */
अटल अंतरभूत पूर्णांक ext4_fc_tag_len(काष्ठा ext4_fc_tl *tl)
{
	वापस le16_to_cpu(tl->fc_len);
}

#पूर्ण_अगर /* __FAST_COMMIT_H__ */
