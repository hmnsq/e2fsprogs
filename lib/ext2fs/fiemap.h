/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#جے_بنا_تعریف _LINUX_FIEMAP_H
#تعریف_کرو _LINUX_FIEMAP_H

بنية fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_لمائی;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

بنية fiemap {
	__u64 fm_شروع;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_لمائی;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_گنتی;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#إذا __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wpedantic"
#جے_انت
	بنية fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#إذا __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic نکالو
#جے_انت
};

#إذا defined(__linux__) && !defined(FS_IOC_FIEMAP)
#تعریف_کرو FS_IOC_FIEMAP	_IOWR('f', 11, بنية fiemap)
#جے_انت

#إذا defined(__linux__) && !defined(FS_IOC_GETSTATE)
#تعریف_کرو EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#جے_انت

#إذا defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#تعریف_کرو EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, بنية fiemap)
#جے_انت

#إذا defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#تعریف_کرو EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#جے_انت

#تعریف_کرو FIEMAP_MAX_وتھ	(~0ULL)

#تعریف_کرو FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#تعریف_کرو FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#تعریف_کرو FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#تعریف_کرو FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#تعریف_کرو FIEMAP_EXTENT_آخری		0x00000001 /* Last extent in file. */
#تعریف_کرو FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#تعریف_کرو FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#تعریف_کرو FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#تعریف_کرو FIEMAP_EXTENT_انکڑے_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#تعریف_کرو FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#تعریف_کرو FIEMAP_EXTENT_انکڑے_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#تعریف_کرو FIEMAP_EXTENT_انکڑے_پوچھ		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#تعریف_کرو FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#تعریف_کرو FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#تعریف_کرو FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#تعریف_کرو EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#جے_انت /* _LINUX_FIEMAP_H */
