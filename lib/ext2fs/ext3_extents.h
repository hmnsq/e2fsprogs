/*
 * Copyright (c) 2003,2004 Cluster File Systems, Inc, info@clusterfs.com
 * Written by Alex Tomas <alex@clusterfs.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#اگر_غیر_تعریف_شدہ _LINUX_EXT3_EXTENTS
#تعریف_کرو _LINUX_EXT3_EXTENTS

/*
 * ext3_inode has i_block array (total 60 bytes)
 * first 4 bytes are used to store:
 *  - tree depth (0 mean there is no tree yet. all extents in the inode)
 *  - number of alive extents in the inode
 */

/*
 * This is extent tail on-disk structure.
 * All other extent structures are 12 bytes long.  It turns out that
 * block_size % 12 >= 4 for at least all powers of 2 greater than 512, which
 * covers all valid ext4 block sizes.  Therefore, this tail structure can be
 * crammed into the end of the block without having to rebalance the tree.
 */
ساخت ext3_extent_دم {
	__le32	et_checksum;	/* crc32c(uuid+inum+extent_block) */
};

/*
 * this is extent on-disk structure
 * it's used at the bottom of the tree
 */
ساخت ext3_extent {
	__le32	ee_بلاک;	/* first logical block extent covers */
	__le16	ee_len;		/* number of blocks covered by extent */
	__le16	ee_آغاز_hi;	/* high 16 bits of physical block */
	__le32	ee_آغاز;	/* low 32 bigs of physical block */
};

/*
 * this is index on-disk structure
 * it's used at all the levels, but the bottom
 */
ساخت ext3_extent_idx {
	__le32	ei_بلاک;	/* index covers logical blocks from 'block' */
	__le32	ei_برگ;	/* pointer to the physical block of the next *
				 * level. leaf or next index could bet here */
	__le16	ei_برگ_hi;	/* high 16 bits of physical block */
	__le16	ei_غیر_مستعمل;
};

/*
 * each block (leaves and indexes), even inode-stored has header
 */
ساخت ext3_extent_سرخی {
	__le16	eh_magic;	/* probably will support different formats */
	__le16	eh_entries;	/* number of valid entries */
	__le16	eh_max;		/* capacity of store in entries */
	__le16	eh_گہرائی;	/* has tree real underlying blocks? */
	__le32	eh_generation;	/* generation of the tree */
};

#تعریف_کرو EXT3_EXT_MAGIC		0xf30a

/*
 * array of ext3_ext_path contains path to some extent
 * creation/lookup routines use it for traversal/splitting/etc
 * truncate uses it to simulate recursive walking
 */
ساخت ext3_ext_راستہ {
	__u32				p_بلاک;
	__u16				p_گہرائی;
	ساخت ext3_extent		*p_ext;
	ساخت ext3_extent_idx		*p_idx;
	ساخت ext3_extent_سرخی	*p_hdr;
	ساخت بفر_سر		*p_bh;
};

/*
 * EXT_INIT_MAX_LEN is the maximum number of blocks we can have in an
 * initialized extent. This is 2^15 and not (2^16 - 1), since we use the
 * MSB of ee_len field in the extent datastructure to signify if this
 * particular extent is an initialized extent or an uninitialized (i.e.
 * preallocated).
 * EXT_UNINIT_MAX_LEN is the maximum number of blocks we can have in an
 * uninitialized extent.
 * If ee_len is <= 0x8000, it is an initialized extent. Otherwise, it is an
 * uninitialized one. In other words, if MSB of ee_len is set, it is an
 * uninitialized extent with only one special scenario when ee_len = 0x8000.
 * In this case we can not have an uninitialized extent of zero length and
 * thus we make it as a special case of initialized extent with 0x8000 length.
 * This way we get better extent-to-group alignment for initialized extents.
 * Hence, the maximum number of blocks we can have in an *initialized*
 * extent is 2^15 (32768) and in an *uninitialized* extent is 2^15-1 (32767).
 */
#تعریف_کرو EXT_INIT_MAX_LEN	(1UL << 15)
#تعریف_کرو EXT_UNINIT_MAX_LEN	(EXT_INIT_MAX_LEN - 1)
#تعریف_کرو EXT_MAX_EXTENT_LBLK	(((__u64) 1 << 32) - 1)
#تعریف_کرو EXT_MAX_EXTENT_PBLK	(((__u64) 1 << 48) - 1)

#تعریف_کرو EXT_پہلا_EXTENT(__hdr__) \
	((ساخت ext3_extent *) (((حرف *) (__hdr__)) +		\
				 حجم(ساخت ext3_extent_سرخی)))
#تعریف_کرو EXT_پہلا_اشاریہ(__hdr__) \
	((ساخت ext3_extent_idx *) (((حرف *) (__hdr__)) +	\
				     حجم(ساخت ext3_extent_سرخی)))
#تعریف_کرو EXT_HAS_FREE_اشاریہ(__path__) \
	(ext2fs_le16_مقصد_cpu((__path__)->p_hdr->eh_entries) < \
	 ext2fs_le16_مقصد_cpu((__path__)->p_hdr->eh_max))
#تعریف_کرو EXT_آخری_EXTENT(__hdr__) \
	(EXT_پہلا_EXTENT((__hdr__)) + \
	ext2fs_le16_مقصد_cpu((__hdr__)->eh_entries) - 1)
#تعریف_کرو EXT_آخری_اشاریہ(__hdr__) \
	(EXT_پہلا_اشاریہ((__hdr__)) + \
	ext2fs_le16_مقصد_cpu((__hdr__)->eh_entries) - 1)
#تعریف_کرو EXT_MAX_EXTENT(__hdr__) \
	(EXT_پہلا_EXTENT((__hdr__)) + \
	ext2fs_le16_مقصد_cpu((__hdr__)->eh_max) - 1)
#تعریف_کرو EXT_MAX_اشاریہ(__hdr__) \
	(EXT_پہلا_اشاریہ((__hdr__)) + \
	ext2fs_le16_مقصد_cpu((__hdr__)->eh_max) - 1)

#اگر_ختم /* _LINUX_EXT3_EXTENTS */

