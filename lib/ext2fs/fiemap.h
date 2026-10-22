/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#यदि_अपरिभाषितम् _LINUX_FIEMAP_H
#परिभाषय _LINUX_FIEMAP_H

संरचना fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_दीर्घता;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

संरचना fiemap {
	__u64 fm_start;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_दीर्घता;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_गणनम्;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wpedantic"
#यदि_अन्तः
	संरचना fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः
};

#यदि defined(__linux__) && !defined(FS_IOC_FIEMAP)
#परिभाषय FS_IOC_FIEMAP	_IOWR('f', 11, संरचना fiemap)
#यदि_अन्तः

#यदि defined(__linux__) && !defined(FS_IOC_GETSTATE)
#परिभाषय EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#यदि_अन्तः

#यदि defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#परिभाषय EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, संरचना fiemap)
#यदि_अन्तः

#यदि defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#परिभाषय EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#यदि_अन्तः

#परिभाषय FIEMAP_MAX_विस्थापनम्	(~0ULL)

#परिभाषय FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#परिभाषय FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#परिभाषय FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#परिभाषय FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#परिभाषय FIEMAP_EXTENT_चरमः		0x00000001 /* Last extent in file. */
#परिभाषय FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#परिभाषय FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#परिभाषय FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#परिभाषय FIEMAP_EXTENT_दत्त_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#परिभाषय FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#परिभाषय FIEMAP_EXTENT_दत्त_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#परिभाषय FIEMAP_EXTENT_दत्त_पुच्छ		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#परिभाषय FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#परिभाषय FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#परिभाषय FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#परिभाषय EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#यदि_अन्तः /* _LINUX_FIEMAP_H */
