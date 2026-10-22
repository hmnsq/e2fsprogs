/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#اگر_غیر_تعریف_شدہ _EXT2_EXT_ATTR_H
#تعریف_کرو _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#تعریف_کرو EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#تعریف_کرو EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#تعریف_کرو EXT2_EXT_ATTR_REFCOUNT_MAX	1024

ساخت ext2_ext_attr_سرخی {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_بلاکیں;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

ساخت ext2_ext_attr_اندراج {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_اشاریہ;	/* attribute name index */
	__u16	e_قدر_offs;	/* offset in disk block of value */
	__u32	e_قدر_inum;	/* inode in which the value is stored */
	__u32	e_قدر_سائز;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#اگر 0
	حرف	e_name[0];	/* attribute name */
#اگر_ختم
};

#تعریف_کرو EXT2_EXT_ATTR_PAD_بٹ		2
#تعریف_کرو EXT2_EXT_ATTR_PAD		((غیرعلامتی) 1<<EXT2_EXT_ATTR_PAD_بٹ)
#تعریف_کرو EXT2_EXT_ATTR_باری		(EXT2_EXT_ATTR_PAD-1)
#تعریف_کرو EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_باری + \
	حجم(ساخت ext2_ext_attr_اندراج)) & ~EXT2_EXT_ATTR_باری)
#تعریف_کرو EXT2_EXT_ATTR_اگلا(اندراج_2) \
	( (ساخت ext2_ext_attr_اندراج *)( \
	  (حرف *)(اندراج_2) + EXT2_EXT_ATTR_LEN((اندراج_2)->e_name_len)) )
#تعریف_کرو EXT2_EXT_ATTR_سائز(سائز) \
	(((سائز) + EXT2_EXT_ATTR_باری) & ~EXT2_EXT_ATTR_باری)
#تعریف_کرو EXT2_EXT_IS_آخری_اندراج(اندراج_2) (*((__u32 *)(اندراج_2)) == 0UL)
#تعریف_کرو EXT2_EXT_ATTR_NAME(اندراج_2) \
	(((حرف *) (اندراج_2)) + حجم(ساخت ext2_ext_attr_اندراج))
#تعریف_کرو EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_باری + \
	حجم(ساخت ext2_xattr_اندراج)) & ~EXT2_EXT_ATTR_باری)
#تعریف_کرو EXT2_XATTR_سائز(سائز) \
	(((سائز) + EXT2_EXT_ATTR_باری) & ~EXT2_EXT_ATTR_باری)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#تعریف_کرو EXT2_XATTR_سائز_MAX (64 * 1024)

#اگر_تعریف_شدہ __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
خارجی صحیح ext2_get_ext_attr(ساخت inode *, ثابت حرف *, حرف *, size_t, صحیح);
خارجی صحیح ext2_سیٹ_ext_attr(ساخت inode *, ثابت حرف *, حرف *, size_t, صحیح);
خارجی خالی ext2_ext_attr_free_inode(ساخت inode *inode);
خارجی خالی ext2_ext_attr_put_super(ساخت super_بلاک *sb);
خارجی صحیح ext2_ext_attr_init(خالی);
خارجی خالی ext2_ext_attr_مکمل(خالی);
# ورنہ
#  define ext2_get_ext_attr تہی
#  define ext2_سیٹ_ext_attr تہی
# endif
#اگر_ختم  /* __KERNEL__ */
#اگر_ختم  /* _EXT2_EXT_ATTR_H */
