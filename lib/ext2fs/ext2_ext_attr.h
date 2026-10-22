/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#جيڪڏهن_وصف_نه_ٿيل _EXT2_EXT_ATTR_H
#وصف_ڪريو _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#وصف_ڪريو EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#وصف_ڪريو EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#وصف_ڪريو EXT2_EXT_ATTR_REFCOUNT_MAX	1024

بنية ext2_ext_attr_سرو {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_بلاڪون;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

بنية ext2_ext_attr_داخلا {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_اشاريو;	/* attribute name index */
	__u16	e_قدر_offs;	/* offset in disk block of value */
	__u32	e_قدر_inum;	/* inode in which the value is stored */
	__u32	e_قدر_سائيز;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#إذا 0
	حرف	e_name[0];	/* attribute name */
#جيڪڏهن_پورو
};

#وصف_ڪريو EXT2_EXT_ATTR_PAD_بٽ		2
#وصف_ڪريو EXT2_EXT_ATTR_PAD		((بنانشان) 1<<EXT2_EXT_ATTR_PAD_بٽ)
#وصف_ڪريو EXT2_EXT_ATTR_ڦيرو		(EXT2_EXT_ATTR_PAD-1)
#وصف_ڪريو EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ڦيرو + \
	حجم(بنية ext2_ext_attr_داخلا)) & ~EXT2_EXT_ATTR_ڦيرو)
#وصف_ڪريو EXT2_EXT_ATTR_ايندڙ(داخلا) \
	( (بنية ext2_ext_attr_داخلا *)( \
	  (حرف *)(داخلا) + EXT2_EXT_ATTR_LEN((داخلا)->e_name_len)) )
#وصف_ڪريو EXT2_EXT_ATTR_سائيز(سائيز) \
	(((سائيز) + EXT2_EXT_ATTR_ڦيرو) & ~EXT2_EXT_ATTR_ڦيرو)
#وصف_ڪريو EXT2_EXT_IS_آخري_داخلا(داخلا) (*((__u32 *)(داخلا)) == 0UL)
#وصف_ڪريو EXT2_EXT_ATTR_NAME(داخلا) \
	(((حرف *) (داخلا)) + حجم(بنية ext2_ext_attr_داخلا))
#وصف_ڪريو EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ڦيرو + \
	حجم(بنية ext2_xattr_داخلا)) & ~EXT2_EXT_ATTR_ڦيرو)
#وصف_ڪريو EXT2_XATTR_سائيز(سائيز) \
	(((سائيز) + EXT2_EXT_ATTR_ڦيرو) & ~EXT2_EXT_ATTR_ڦيرو)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#وصف_ڪريو EXT2_XATTR_سائيز_MAX (64 * 1024)

#جيڪڏهن_وصف_ٿيل __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
خارجي صحيح ext2_get_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي صحيح ext2_سيٽ_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي فراغ ext2_ext_attr_free_inode(بنية inode *inode);
خارجي فراغ ext2_ext_attr_put_super(بنية super_بلاڪ *sb);
خارجي صحيح ext2_ext_attr_init(فراغ);
خارجي فراغ ext2_ext_attr_پورو(فراغ);
# وإلا
#  define ext2_get_ext_attr عدم
#  define ext2_سيٽ_ext_attr عدم
# endif
#جيڪڏهن_پورو  /* __KERNEL__ */
#جيڪڏهن_پورو  /* _EXT2_EXT_ATTR_H */
