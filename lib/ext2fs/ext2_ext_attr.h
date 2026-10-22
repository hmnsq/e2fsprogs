/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#که_نه_تعریف_شوی _EXT2_EXT_ATTR_H
#تعریف_کړه _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#تعریف_کړه EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#تعریف_کړه EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#تعریف_کړه EXT2_EXT_ATTR_REFCOUNT_MAX	1024

بنية ext2_ext_attr_سرلیک {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_بلاکونه;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

بنية ext2_ext_attr_ثبت {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_شاخص;	/* attribute name index */
	__u16	e_ارزښت_offs;	/* offset in disk block of value */
	__u32	e_ارزښت_inum;	/* inode in which the value is stored */
	__u32	e_ارزښت_اندازه;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#إذا 0
	حرف	e_name[0];	/* attribute name */
#د_که_پای
};

#تعریف_کړه EXT2_EXT_ATTR_PAD_بټونه		2
#تعریف_کړه EXT2_EXT_ATTR_PAD		((بینښه) 1<<EXT2_EXT_ATTR_PAD_بټونه)
#تعریف_کړه EXT2_EXT_ATTR_پړاو		(EXT2_EXT_ATTR_PAD-1)
#تعریف_کړه EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_پړاو + \
	حجم(بنية ext2_ext_attr_ثبت)) & ~EXT2_EXT_ATTR_پړاو)
#تعریف_کړه EXT2_EXT_ATTR_بل(ثبت) \
	( (بنية ext2_ext_attr_ثبت *)( \
	  (حرف *)(ثبت) + EXT2_EXT_ATTR_LEN((ثبت)->e_name_len)) )
#تعریف_کړه EXT2_EXT_ATTR_اندازه(اندازه) \
	(((اندازه) + EXT2_EXT_ATTR_پړاو) & ~EXT2_EXT_ATTR_پړاو)
#تعریف_کړه EXT2_EXT_IS_وروستی_ثبت(ثبت) (*((__u32 *)(ثبت)) == 0UL)
#تعریف_کړه EXT2_EXT_ATTR_NAME(ثبت) \
	(((حرف *) (ثبت)) + حجم(بنية ext2_ext_attr_ثبت))
#تعریف_کړه EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_پړاو + \
	حجم(بنية ext2_xattr_ثبت)) & ~EXT2_EXT_ATTR_پړاو)
#تعریف_کړه EXT2_XATTR_اندازه(اندازه) \
	(((اندازه) + EXT2_EXT_ATTR_پړاو) & ~EXT2_EXT_ATTR_پړاو)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#تعریف_کړه EXT2_XATTR_اندازه_MAX (64 * 1024)

#که_تعریف_شوی __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
خارجي صحيح ext2_get_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي صحيح ext2_سېټ_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي فراغ ext2_ext_attr_free_inode(بنية inode *inode);
خارجي فراغ ext2_ext_attr_put_super(بنية super_بلاک *sb);
خارجي صحيح ext2_ext_attr_init(فراغ);
خارجي فراغ ext2_ext_attr_بشپړ(فراغ);
# وإلا
#  define ext2_get_ext_attr عدم
#  define ext2_سېټ_ext_attr عدم
# endif
#د_که_پای  /* __KERNEL__ */
#د_که_پای  /* _EXT2_EXT_ATTR_H */
