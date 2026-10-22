/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#నిర్వచితం_కాకపోతే _EXT2_EXT_ATTR_H
#నిర్వచించు _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#నిర్వచించు EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#నిర్వచించు EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#నిర్వచించు EXT2_EXT_ATTR_REFCOUNT_MAX	1024

నిర్మాణం ext2_ext_attr_శీర్షిక {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_ఖండాలు;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

నిర్మాణం ext2_ext_attr_నమోదు {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_సూచీ;	/* attribute name index */
	__u16	e_విలువ_offs;	/* offset in disk block of value */
	__u32	e_విలువ_inum;	/* inode in which the value is stored */
	__u32	e_విలువ_కొలత;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#ఒకవేళ 0
	అక్షరం	e_name[0];	/* attribute name */
#అయితే_ముగింపు
};

#నిర్వచించు EXT2_EXT_ATTR_PAD_బిట్లు		2
#నిర్వచించు EXT2_EXT_ATTR_PAD		((గుర్తులేని) 1<<EXT2_EXT_ATTR_PAD_బిట్లు)
#నిర్వచించు EXT2_EXT_ATTR_విడత		(EXT2_EXT_ATTR_PAD-1)
#నిర్వచించు EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_విడత + \
	పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు)) & ~EXT2_EXT_ATTR_విడత)
#నిర్వచించు EXT2_EXT_ATTR_తదుపరి(నమోదు_2) \
	( (నిర్మాణం ext2_ext_attr_నమోదు *)( \
	  (అక్షరం *)(నమోదు_2) + EXT2_EXT_ATTR_LEN((నమోదు_2)->e_name_len)) )
#నిర్వచించు EXT2_EXT_ATTR_కొలత(కొలత) \
	(((కొలత) + EXT2_EXT_ATTR_విడత) & ~EXT2_EXT_ATTR_విడత)
#నిర్వచించు EXT2_EXT_IS_చివరి_నమోదు(నమోదు_2) (*((__u32 *)(నమోదు_2)) == 0UL)
#నిర్వచించు EXT2_EXT_ATTR_NAME(నమోదు_2) \
	(((అక్షరం *) (నమోదు_2)) + పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు))
#నిర్వచించు EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_విడత + \
	పరిమాణం(నిర్మాణం ext2_xattr_నమోదు)) & ~EXT2_EXT_ATTR_విడత)
#నిర్వచించు EXT2_XATTR_కొలత(కొలత) \
	(((కొలత) + EXT2_EXT_ATTR_విడత) & ~EXT2_EXT_ATTR_విడత)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#నిర్వచించు EXT2_XATTR_కొలత_MAX (64 * 1024)

#నిర్వచితమైతే __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
బాహ్య పూర్ణాంకం ext2_get_ext_attr(నిర్మాణం inode *, స్థిరరాశి అక్షరం *, అక్షరం *, size_t, పూర్ణాంకం);
బాహ్య పూర్ణాంకం ext2_సమితి_ext_attr(నిర్మాణం inode *, స్థిరరాశి అక్షరం *, అక్షరం *, size_t, పూర్ణాంకం);
బాహ్య శూన్య ext2_ext_attr_free_inode(నిర్మాణం inode *inode);
బాహ్య శూన్య ext2_ext_attr_put_super(నిర్మాణం super_ఖండం *sb);
బాహ్య పూర్ణాంకం ext2_ext_attr_init(శూన్య);
బాహ్య శూన్య ext2_ext_attr_పూర్తి(శూన్య);
# లేకపోతే
#  define ext2_get_ext_attr ఖాళీ
#  define ext2_సమితి_ext_attr ఖాళీ
# endif
#అయితే_ముగింపు  /* __KERNEL__ */
#అయితే_ముగింపు  /* _EXT2_EXT_ATTR_H */
