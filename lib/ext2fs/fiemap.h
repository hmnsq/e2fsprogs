/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#اگر_تعریف_نشده _LINUX_FIEMAP_H
#تعریف_کن _LINUX_FIEMAP_H

بنية fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_درازی;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

بنية fiemap {
	__u64 fm_شروع;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_درازی;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_شمار;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#إذا __GNUC_PREREQ (4, 8)
#رهنمود GCC diagnostic افزودن
#رهنمود GCC diagnostic ignored "-Wpedantic"
#پایان_اگر
	بنية fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#إذا __GNUC_PREREQ (4, 8)
#رهنمود GCC diagnostic برداشت
#پایان_اگر
};

#إذا defined(__linux__) && !defined(FS_IOC_FIEMAP)
#تعریف_کن FS_IOC_FIEMAP	_IOWR('f', 11, بنية fiemap)
#پایان_اگر

#إذا defined(__linux__) && !defined(FS_IOC_GETSTATE)
#تعریف_کن EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#پایان_اگر

#إذا defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#تعریف_کن EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, بنية fiemap)
#پایان_اگر

#إذا defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#تعریف_کن EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#پایان_اگر

#تعریف_کن FIEMAP_MAX_جابه_جایی	(~0ULL)

#تعریف_کن FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#تعریف_کن FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#تعریف_کن FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#تعریف_کن FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#تعریف_کن FIEMAP_EXTENT_آخرین		0x00000001 /* Last extent in file. */
#تعریف_کن FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#تعریف_کن FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#تعریف_کن FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#تعریف_کن FIEMAP_EXTENT_داده_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#تعریف_کن FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#تعریف_کن FIEMAP_EXTENT_داده_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#تعریف_کن FIEMAP_EXTENT_داده_دم		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#تعریف_کن FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#تعریف_کن FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#تعریف_کن FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#تعریف_کن EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#پایان_اگر /* _LINUX_FIEMAP_H */
