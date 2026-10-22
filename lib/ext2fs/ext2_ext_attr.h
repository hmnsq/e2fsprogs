/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#ifndef _EXT2_EXT_ATTR_H
#define _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#define EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#define EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#define EXT2_EXT_ATTR_REFCOUNT_MAX	1024

संरचना ext2_ext_attr_header {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_blocks;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

संरचना ext2_ext_attr_entry {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_index;	/* attribute name index */
	__u16	e_मूल्य_offs;	/* offset in disk block of value */
	__u32	e_मूल्य_inum;	/* inode in which the value is stored */
	__u32	e_मूल्य_size;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#जर 0
	अक्षर	e_name[0];	/* attribute name */
#endif
};

#define EXT2_EXT_ATTR_PAD_BITS		2
#define EXT2_EXT_ATTR_PAD		((अचिन्हित) 1<<EXT2_EXT_ATTR_PAD_BITS)
#define EXT2_EXT_ATTR_ROUND		(EXT2_EXT_ATTR_PAD-1)
#define EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ROUND + \
	आकार(संरचना ext2_ext_attr_entry)) & ~EXT2_EXT_ATTR_ROUND)
#define EXT2_EXT_ATTR_NEXT(entry) \
	( (संरचना ext2_ext_attr_entry *)( \
	  (अक्षर *)(entry) + EXT2_EXT_ATTR_LEN((entry)->e_name_len)) )
#define EXT2_EXT_ATTR_SIZE(size) \
	(((size) + EXT2_EXT_ATTR_ROUND) & ~EXT2_EXT_ATTR_ROUND)
#define EXT2_EXT_IS_LAST_ENTRY(entry) (*((__u32 *)(entry)) == 0UL)
#define EXT2_EXT_ATTR_NAME(entry) \
	(((अक्षर *) (entry)) + आकार(संरचना ext2_ext_attr_entry))
#define EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ROUND + \
	आकार(संरचना ext2_xattr_entry)) & ~EXT2_EXT_ATTR_ROUND)
#define EXT2_XATTR_SIZE(size) \
	(((size) + EXT2_EXT_ATTR_ROUND) & ~EXT2_EXT_ATTR_ROUND)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#define EXT2_XATTR_SIZE_MAX (64 * 1024)

#ifdef __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
बाह्य पूर्णांक ext2_get_ext_attr(संरचना inode *, स्थिरांक अक्षर *, अक्षर *, माप_प्रकार, पूर्णांक);
बाह्य पूर्णांक ext2_set_ext_attr(संरचना inode *, स्थिरांक अक्षर *, अक्षर *, माप_प्रकार, पूर्णांक);
बाह्य शून्य ext2_ext_attr_free_inode(संरचना inode *inode);
बाह्य शून्य ext2_ext_attr_put_super(संरचना super_block *sb);
बाह्य पूर्णांक ext2_ext_attr_init(शून्य);
बाह्य शून्य ext2_ext_attr_done(शून्य);
# अन्यथा
#  define ext2_get_ext_attr रिक्त
#  define ext2_set_ext_attr रिक्त
# endif
#endif  /* __KERNEL__ */
#endif  /* _EXT2_EXT_ATTR_H */
