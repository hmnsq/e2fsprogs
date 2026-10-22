/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#अगर_अघोषित _LINUX_FIEMAP_H
#घोषणा _LINUX_FIEMAP_H

काष्ठा fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_लंबाई;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

काष्ठा fiemap {
	__u64 fm_start;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_लंबाई;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_गिनती;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wpedantic"
#पूर्ण_अगर
	काष्ठा fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर
};

#अगर defined(__linux__) && !defined(FS_IOC_FIEMAP)
#घोषणा FS_IOC_FIEMAP	_IOWR('f', 11, काष्ठा fiemap)
#पूर्ण_अगर

#अगर defined(__linux__) && !defined(FS_IOC_GETSTATE)
#घोषणा EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#पूर्ण_अगर

#अगर defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#घोषणा EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, काष्ठा fiemap)
#पूर्ण_अगर

#अगर defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#घोषणा EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#पूर्ण_अगर

#घोषणा FIEMAP_MAX_OFFSET	(~0ULL)

#घोषणा FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#घोषणा FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#घोषणा FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#घोषणा FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#घोषणा FIEMAP_EXTENT_LAST		0x00000001 /* Last extent in file. */
#घोषणा FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#घोषणा FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#घोषणा FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#घोषणा FIEMAP_EXTENT_तथ्य_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#घोषणा FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#घोषणा FIEMAP_EXTENT_तथ्य_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#घोषणा FIEMAP_EXTENT_तथ्य_पुच्छ		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#घोषणा FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#घोषणा FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#घोषणा FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#घोषणा EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#पूर्ण_अगर /* _LINUX_FIEMAP_H */
