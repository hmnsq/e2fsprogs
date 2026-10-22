/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _LINUX_FIEMAP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _LINUX_FIEMAP_H

ਬਣਤਰ fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_ਲੰਮਾਈ;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

ਬਣਤਰ fiemap {
	__u64 fm_ਸ਼ੁਰੂ;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_ਲੰਮਾਈ;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_ਗਿਣਤੀ;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wpedantic"
#ਜੇ_ਅੰਤ
	ਬਣਤਰ fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ
};

#ਜੇ defined(__linux__) && !defined(FS_IOC_FIEMAP)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FS_IOC_FIEMAP	_IOWR('f', 11, ਬਣਤਰ fiemap)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && !defined(FS_IOC_GETSTATE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, ਬਣਤਰ fiemap)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_MAX_ਵਿੱਥ	(~0ULL)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_ਆਖਰੀ		0x00000001 /* Last extent in file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_ਅੰਕੜੇ_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_ਅੰਕੜੇ_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_ਅੰਕੜੇ_ਪੂਛ		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#ਜੇ_ਅੰਤ /* _LINUX_FIEMAP_H */
