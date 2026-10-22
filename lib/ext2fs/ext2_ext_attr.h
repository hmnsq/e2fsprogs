/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _EXT2_EXT_ATTR_H
#ವ್ಯಾಖ್ಯಾನಿಸು _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_REFCOUNT_MAX	1024

ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_ಖಂಡಗಳು;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

ರಚನೆ ext2_ext_attr_ನಮೂದು {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_ಸೂಚ್ಯಂಕ;	/* attribute name index */
	__u16	e_ಮೌಲ್ಯ_offs;	/* offset in disk block of value */
	__u32	e_ಮೌಲ್ಯ_inum;	/* inode in which the value is stored */
	__u32	e_ಮೌಲ್ಯ_ಗಾತ್ರ;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#ಆದರೆ 0
	ಅಕ್ಷರ	e_name[0];	/* attribute name */
#ಆದರೆ_ಅಂತ್ಯ
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_PAD_ಬಿಟ್ಟುಗಳು		2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_PAD		((ಚಿಹ್ನೆರಹಿತ) 1<<EXT2_EXT_ATTR_PAD_ಬಿಟ್ಟುಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_ಸುತ್ತು		(EXT2_EXT_ATTR_PAD-1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ಸುತ್ತು + \
	ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು)) & ~EXT2_EXT_ATTR_ಸುತ್ತು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_ಮುಂದಿನ(ನಮೂದು) \
	( (ರಚನೆ ext2_ext_attr_ನಮೂದು *)( \
	  (ಅಕ್ಷರ *)(ನಮೂದು) + EXT2_EXT_ATTR_LEN((ನಮೂದು)->e_name_len)) )
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_ಗಾತ್ರ(ಗಾತ್ರ_2) \
	(((ಗಾತ್ರ_2) + EXT2_EXT_ATTR_ಸುತ್ತು) & ~EXT2_EXT_ATTR_ಸುತ್ತು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_IS_ಕೊನೆಯ_ನಮೂದು(ನಮೂದು) (*((__u32 *)(ನಮೂದು)) == 0UL)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXT_ATTR_NAME(ನಮೂದು) \
	(((ಅಕ್ಷರ *) (ನಮೂದು)) + ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು))
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ಸುತ್ತು + \
	ಗಾತ್ರ(ರಚನೆ ext2_xattr_ನಮೂದು)) & ~EXT2_EXT_ATTR_ಸುತ್ತು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_XATTR_ಗಾತ್ರ(ಗಾತ್ರ_2) \
	(((ಗಾತ್ರ_2) + EXT2_EXT_ATTR_ಸುತ್ತು) & ~EXT2_EXT_ATTR_ಸುತ್ತು)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_XATTR_ಗಾತ್ರ_MAX (64 * 1024)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2_get_ext_attr(ರಚನೆ inode *, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, ಅಕ್ಷರ *, size_t, ಪೂರ್ಣಾಂಕ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2_ಗಣ_ext_attr(ರಚನೆ inode *, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, ಅಕ್ಷರ *, size_t, ಪೂರ್ಣಾಂಕ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2_ext_attr_free_inode(ರಚನೆ inode *inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2_ext_attr_put_super(ರಚನೆ super_ಖಂಡ *sb);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2_ext_attr_init(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2_ext_attr_ಮುಗಿದ(ಶೂನ್ಯ);
# ಇಲ್ಲದಿದ್ದರೆ
#  define ext2_get_ext_attr ಖಾಲಿ
#  define ext2_ಗಣ_ext_attr ಖಾಲಿ
# endif
#ಆದರೆ_ಅಂತ್ಯ  /* __KERNEL__ */
#ಆದರೆ_ಅಂತ್ಯ  /* _EXT2_EXT_ATTR_H */
