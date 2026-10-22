/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#اگر_تعریف_نشده _EXT2_EXT_ATTR_H
#تعریف_کن _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#تعریف_کن EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#تعریف_کن EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#تعریف_کن EXT2_EXT_ATTR_REFCOUNT_MAX	1024

بنية ext2_ext_attr_سرآیند {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_بلوکها;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

بنية ext2_ext_attr_مدخل {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_شاخص;	/* attribute name index */
	__u16	e_مقدار_offs;	/* offset in disk block of value */
	__u32	e_مقدار_inum;	/* inode in which the value is stored */
	__u32	e_مقدار_اندازه;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#إذا 0
	حرف	e_name[0];	/* attribute name */
#پایان_اگر
};

#تعریف_کن EXT2_EXT_ATTR_PAD_بیتها		2
#تعریف_کن EXT2_EXT_ATTR_PAD		((بیعلامت) 1<<EXT2_EXT_ATTR_PAD_بیتها)
#تعریف_کن EXT2_EXT_ATTR_دور		(EXT2_EXT_ATTR_PAD-1)
#تعریف_کن EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_دور + \
	حجم(بنية ext2_ext_attr_مدخل)) & ~EXT2_EXT_ATTR_دور)
#تعریف_کن EXT2_EXT_ATTR_بعدی(مدخل) \
	( (بنية ext2_ext_attr_مدخل *)( \
	  (حرف *)(مدخل) + EXT2_EXT_ATTR_LEN((مدخل)->e_name_len)) )
#تعریف_کن EXT2_EXT_ATTR_اندازه(اندازه) \
	(((اندازه) + EXT2_EXT_ATTR_دور) & ~EXT2_EXT_ATTR_دور)
#تعریف_کن EXT2_EXT_IS_آخرین_مدخل(مدخل) (*((__u32 *)(مدخل)) == 0UL)
#تعریف_کن EXT2_EXT_ATTR_NAME(مدخل) \
	(((حرف *) (مدخل)) + حجم(بنية ext2_ext_attr_مدخل))
#تعریف_کن EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_دور + \
	حجم(بنية ext2_xattr_مدخل)) & ~EXT2_EXT_ATTR_دور)
#تعریف_کن EXT2_XATTR_اندازه(اندازه) \
	(((اندازه) + EXT2_EXT_ATTR_دور) & ~EXT2_EXT_ATTR_دور)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#تعریف_کن EXT2_XATTR_اندازه_MAX (64 * 1024)

#اگر_تعریف_شده __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
خارجي صحيح ext2_get_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي صحيح ext2_مجموعه_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي فراغ ext2_ext_attr_free_inode(بنية inode *inode);
خارجي فراغ ext2_ext_attr_put_super(بنية super_بلوک *sb);
خارجي صحيح ext2_ext_attr_init(فراغ);
خارجي فراغ ext2_ext_attr_انجامشده(فراغ);
# وإلا
#  define ext2_get_ext_attr عدم
#  define ext2_مجموعه_ext_attr عدم
# endif
#پایان_اگر  /* __KERNEL__ */
#پایان_اگر  /* _EXT2_EXT_ATTR_H */
