/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#अगर_अघोषित _EXT2_EXT_ATTR_H
#घोषणा _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#घोषणा EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#घोषणा EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#घोषणा EXT2_EXT_ATTR_REFCOUNT_MAX	1024

काष्ठा ext2_ext_attr_header {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_blocks;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

काष्ठा ext2_ext_attr_entry {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_सूचकांक;	/* attribute name index */
	__u16	e_मूल्य_offs;	/* offset in disk block of value */
	__u32	e_मूल्य_inum;	/* inode in which the value is stored */
	__u32	e_मूल्य_आकार;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#अगर 0
	अक्षर	e_name[0];	/* attribute name */
#पूर्ण_अगर
};

#घोषणा EXT2_EXT_ATTR_PAD_BITS		2
#घोषणा EXT2_EXT_ATTR_PAD		((अचिन्हित) 1<<EXT2_EXT_ATTR_PAD_BITS)
#घोषणा EXT2_EXT_ATTR_ROUND		(EXT2_EXT_ATTR_PAD-1)
#घोषणा EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ROUND + \
	माप(काष्ठा ext2_ext_attr_entry)) & ~EXT2_EXT_ATTR_ROUND)
#घोषणा EXT2_EXT_ATTR_अग्र(entry) \
	( (काष्ठा ext2_ext_attr_entry *)( \
	  (अक्षर *)(entry) + EXT2_EXT_ATTR_LEN((entry)->e_name_len)) )
#घोषणा EXT2_EXT_ATTR_आकार(आकार) \
	(((आकार) + EXT2_EXT_ATTR_ROUND) & ~EXT2_EXT_ATTR_ROUND)
#घोषणा EXT2_EXT_IS_LAST_ENTRY(entry) (*((__u32 *)(entry)) == 0UL)
#घोषणा EXT2_EXT_ATTR_NAME(entry) \
	(((अक्षर *) (entry)) + माप(काष्ठा ext2_ext_attr_entry))
#घोषणा EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ROUND + \
	माप(काष्ठा ext2_xattr_entry)) & ~EXT2_EXT_ATTR_ROUND)
#घोषणा EXT2_XATTR_आकार(आकार) \
	(((आकार) + EXT2_EXT_ATTR_ROUND) & ~EXT2_EXT_ATTR_ROUND)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#घोषणा EXT2_XATTR_आकार_MAX (64 * 1024)

#अगर_घोषित __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
बाह्य पूर्णांक ext2_get_ext_attr(काष्ठा inode *, स्थिर अक्षर *, अक्षर *, माप_प्रकार, पूर्णांक);
बाह्य पूर्णांक ext2_set_ext_attr(काष्ठा inode *, स्थिर अक्षर *, अक्षर *, माप_प्रकार, पूर्णांक);
बाह्य व्योम ext2_ext_attr_free_inode(काष्ठा inode *inode);
बाह्य व्योम ext2_ext_attr_put_super(काष्ठा super_block *sb);
बाह्य पूर्णांक ext2_ext_attr_init(व्योम);
बाह्य व्योम ext2_ext_attr_done(व्योम);
# अन्यथा
#  define ext2_get_ext_attr शून्य
#  define ext2_set_ext_attr शून्य
# endif
#पूर्ण_अगर  /* __KERNEL__ */
#पूर्ण_अगर  /* _EXT2_EXT_ATTR_H */
