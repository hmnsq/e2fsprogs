/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#यदि_अपरिभाषितम् _EXT2_EXT_ATTR_H
#परिभाषय _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#परिभाषय EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#परिभाषय EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#परिभाषय EXT2_EXT_ATTR_REFCOUNT_MAX	1024

संरचना ext2_ext_attr_शीर्षकम् {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_खण्डानि;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

संरचना ext2_ext_attr_प्रविष्टिः {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_सूचकांक;	/* attribute name index */
	__u16	e_मानम्_offs;	/* offset in disk block of value */
	__u32	e_मानम्_inum;	/* inode in which the value is stored */
	__u32	e_मानम्_आकार;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#यदि 0
	वर्ण	e_name[0];	/* attribute name */
#यदि_अन्तः
};

#परिभाषय EXT2_EXT_ATTR_PAD_द्वयङ्काः		2
#परिभाषय EXT2_EXT_ATTR_PAD		((अचिह्नित) 1<<EXT2_EXT_ATTR_PAD_द्वयङ्काः)
#परिभाषय EXT2_EXT_ATTR_आवर्तनम्		(EXT2_EXT_ATTR_PAD-1)
#परिभाषय EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_आवर्तनम् + \
	परिमाण(संरचना ext2_ext_attr_प्रविष्टिः)) & ~EXT2_EXT_ATTR_आवर्तनम्)
#परिभाषय EXT2_EXT_ATTR_अग्र(प्रविष्टिः) \
	( (संरचना ext2_ext_attr_प्रविष्टिः *)( \
	  (वर्ण *)(प्रविष्टिः) + EXT2_EXT_ATTR_LEN((प्रविष्टिः)->e_name_len)) )
#परिभाषय EXT2_EXT_ATTR_आकार(आकार) \
	(((आकार) + EXT2_EXT_ATTR_आवर्तनम्) & ~EXT2_EXT_ATTR_आवर्तनम्)
#परिभाषय EXT2_EXT_IS_चरमः_प्रविष्टिः(प्रविष्टिः) (*((__u32 *)(प्रविष्टिः)) == 0UL)
#परिभाषय EXT2_EXT_ATTR_NAME(प्रविष्टिः) \
	(((वर्ण *) (प्रविष्टिः)) + परिमाण(संरचना ext2_ext_attr_प्रविष्टिः))
#परिभाषय EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_आवर्तनम् + \
	परिमाण(संरचना ext2_xattr_प्रविष्टिः)) & ~EXT2_EXT_ATTR_आवर्तनम्)
#परिभाषय EXT2_XATTR_आकार(आकार) \
	(((आकार) + EXT2_EXT_ATTR_आवर्तनम्) & ~EXT2_EXT_ATTR_आवर्तनम्)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#परिभाषय EXT2_XATTR_आकार_MAX (64 * 1024)

#यदि_परिभाषितम् __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
बाह्य पूर्णाङ्क ext2_get_ext_attr(संरचना inode *, ध्रुव वर्ण *, वर्ण *, size_t, पूर्णाङ्क);
बाह्य पूर्णाङ्क ext2_समुच्चयः_ext_attr(संरचना inode *, ध्रुव वर्ण *, वर्ण *, size_t, पूर्णाङ्क);
बाह्य शून्य ext2_ext_attr_free_inode(संरचना inode *inode);
बाह्य शून्य ext2_ext_attr_put_super(संरचना super_खण्डम् *sb);
बाह्य पूर्णाङ्क ext2_ext_attr_init(शून्य);
बाह्य शून्य ext2_ext_attr_समाप्तम्(शून्य);
# अन्यथा
#  define ext2_get_ext_attr रिक्त
#  define ext2_समुच्चयः_ext_attr रिक्त
# endif
#यदि_अन्तः  /* __KERNEL__ */
#यदि_अन्तः  /* _EXT2_EXT_ATTR_H */
