/*
  File: linux/ext2_ext_attr.h

  On-disk format of extended attributes for the ext2 filesystem.

  (C) 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _EXT2_EXT_ATTR_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _EXT2_EXT_ATTR_H
/* Magic value in attribute blocks */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_MAGIC_v1		0xEA010000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_REFCOUNT_MAX	1024

ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ {
	__u32	h_magic;	/* magic number for identification */
	__u32	h_refcount;	/* reference count */
	__u32	h_ਖੰਡਾਂ;	/* number of disk blocks used */
	__u32	h_hash;		/* hash value of all attributes */
	__u32	h_checksum;	/* crc32c(uuid+id+xattrs) */
				/* id = inum if refcount = 1, else blknum */
	__u32	h_reserved[3];	/* zero right now */
};

ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_ਸੂਚਕਾਂਕ;	/* attribute name index */
	__u16	e_ਮੁੱਲ_offs;	/* offset in disk block of value */
	__u32	e_ਮੁੱਲ_inum;	/* inode in which the value is stored */
	__u32	e_ਮੁੱਲ_ਮਾਪ;	/* size of attribute value */
	__u32	e_hash;		/* hash value of name and value */
#ਜੇ 0
	ਅੱਖਰ	e_name[0];	/* attribute name */
#ਜੇ_ਅੰਤ
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_PAD_ਬਿੱਟ		2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_PAD		((ਬਿਨਾਨਿਸ਼ਾਨੀ) 1<<EXT2_EXT_ATTR_PAD_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_ਗੇੜਾ		(EXT2_EXT_ATTR_PAD-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ਗੇੜਾ + \
	ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ)) & ~EXT2_EXT_ATTR_ਗੇੜਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_ਅਗਲਾ(ਇੰਦਰਾਜ) \
	( (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)( \
	  (ਅੱਖਰ *)(ਇੰਦਰਾਜ) + EXT2_EXT_ATTR_LEN((ਇੰਦਰਾਜ)->e_name_len)) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_ਮਾਪ(ਮਾਪ) \
	(((ਮਾਪ) + EXT2_EXT_ATTR_ਗੇੜਾ) & ~EXT2_EXT_ATTR_ਗੇੜਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_IS_ਆਖਰੀ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ) (*((__u32 *)(ਇੰਦਰਾਜ)) == 0UL)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXT_ATTR_NAME(ਇੰਦਰਾਜ) \
	(((ਅੱਖਰ *) (ਇੰਦਰਾਜ)) + ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_XATTR_LEN(name_len) \
	(((name_len) + EXT2_EXT_ATTR_ਗੇੜਾ + \
	ਆਕਾਰ(ਬਣਤਰ ext2_xattr_ਇੰਦਰਾਜ)) & ~EXT2_EXT_ATTR_ਗੇੜਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_XATTR_ਮਾਪ(ਮਾਪ) \
	(((ਮਾਪ) + EXT2_EXT_ATTR_ਗੇੜਾ) & ~EXT2_EXT_ATTR_ਗੇੜਾ)

/*
 * XATTR_SIZE_MAX as defined in include/uapi/linux/limits.h is 64k
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_XATTR_ਮਾਪ_MAX (64 * 1024)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __KERNEL__
# ifdef CONFIG_EXT2_FS_EXT_ATTR
ਬਾਹਰੀ ਪੂਰਨ ext2_get_ext_attr(ਬਣਤਰ inode *, ਅਚਲ ਅੱਖਰ *, ਅੱਖਰ *, size_t, ਪੂਰਨ);
ਬਾਹਰੀ ਪੂਰਨ ext2_ਸੈੱਟ_ext_attr(ਬਣਤਰ inode *, ਅਚਲ ਅੱਖਰ *, ਅੱਖਰ *, size_t, ਪੂਰਨ);
ਬਾਹਰੀ ਖਾਲੀ ext2_ext_attr_free_inode(ਬਣਤਰ inode *inode);
ਬਾਹਰੀ ਖਾਲੀ ext2_ext_attr_put_super(ਬਣਤਰ super_ਖੰਡ *sb);
ਬਾਹਰੀ ਪੂਰਨ ext2_ext_attr_init(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ ext2_ext_attr_ਪੂਰਾ(ਖਾਲੀ);
# ਨਹੀਂਤਾਂ
#  define ext2_get_ext_attr ਨਿਰਾ
#  define ext2_ਸੈੱਟ_ext_attr ਨਿਰਾ
# endif
#ਜੇ_ਅੰਤ  /* __KERNEL__ */
#ਜੇ_ਅੰਤ  /* _EXT2_EXT_ATTR_H */
