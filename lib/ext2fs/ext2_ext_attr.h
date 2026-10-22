/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#جے_بنا_تعریف _EXT2_EXT_ATTR_H
#تعریف_کرو _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#تعریف_کرو EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#تعریف_کرو EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#تعریف_کرو EXT2_EXT_ATTR_REFCOUNT_MAX	1024

بنية ext2_ext_attr_سرخی {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_بلاکاں;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

بنية ext2_ext_attr_اندراج {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_اشاریہ;	/* attribute name index */
	__u16	e_قدر_offs;	/* offset in disk block of value */
	__u32	e_قدر_inum;	/* inode in which the value is stored */
	__u32	e_قدر_ناپ;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#إذا 0
	حرف	e_name[0];	/* attribute name */
#جے_انت
};

#تعریف_کرو EXT2_EXT_ATTR_PAD_بٹ		2
#تعریف_کرو EXT2_EXT_ATTR_PAD		((بنانشانی) 1<<EXT2_EXT_ATTR_PAD_بٹ)
#تعریف_کرو EXT2_EXT_ATTR_گیڑا		(EXT2_EXT_ATTR_PAD-1)
#تعریف_کرو EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_گیڑا + \
	حجم(بنية ext2_ext_attr_اندراج)) & ~EXT2_EXT_ATTR_گیڑا)
#تعریف_کرو EXT2_EXT_ATTR_اگلا(اندراج) \
	( (بنية ext2_ext_attr_اندراج *)( \
	  (حرف *)(اندراج) + EXT2_EXT_ATTR_LEN((اندراج)->e_name_len)) )
#تعریف_کرو EXT2_EXT_ATTR_ناپ(ناپ) \
	(((ناپ) + EXT2_EXT_ATTR_گیڑا) & ~EXT2_EXT_ATTR_گیڑا)
#تعریف_کرو EXT2_EXT_IS_آخری_اندراج(اندراج) (*((__u32 *)(اندراج)) == 0UL)
#تعریف_کرو EXT2_EXT_ATTR_NAME(اندراج) \
	(((حرف *) (اندراج)) + حجم(بنية ext2_ext_attr_اندراج))
#تعریف_کرو EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_گیڑا + \
	حجم(بنية ext2_xattr_اندراج)) & ~EXT2_EXT_ATTR_گیڑا)
#تعریف_کرو EXT2_XATTR_ناپ(ناپ) \
	(((ناپ) + EXT2_EXT_ATTR_گیڑا) & ~EXT2_EXT_ATTR_گیڑا)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#تعریف_کرو EXT2_XATTR_ناپ_MAX (64 * 1024)

#جے_تعریف_شدہ __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
خارجي صحيح ext2_get_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي صحيح ext2_سیٹ_ext_attr(بنية inode *, ثابت حرف *, حرف *, size_t, صحيح);
خارجي فراغ ext2_ext_attr_free_inode(بنية inode *inode);
خارجي فراغ ext2_ext_attr_put_super(بنية super_بلاک *sb);
خارجي صحيح ext2_ext_attr_init(فراغ);
خارجي فراغ ext2_ext_attr_پورا(فراغ);
# وإلا
#  define ext2_get_ext_attr عدم
#  define ext2_سیٹ_ext_attr عدم
# endif
#جے_انت  /* __KERNEL__ */
#جے_انت  /* _EXT2_EXT_ATTR_H */
