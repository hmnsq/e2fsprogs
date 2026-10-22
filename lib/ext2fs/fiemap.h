/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _LINUX_FIEMAP_H
#ವ್ಯಾಖ್ಯಾನಿಸು _LINUX_FIEMAP_H

ರಚನೆ fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_ಉದ್ದಳತೆ;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

ರಚನೆ fiemap {
	__u64 fm_ಆರಂಭ;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_ಉದ್ದಳತೆ;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_ಎಣಿಕೆ;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wpedantic"
#ಆದರೆ_ಅಂತ್ಯ
	ರಚನೆ fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ
};

#ಆದರೆ defined(__linux__) && !defined(FS_IOC_FIEMAP)
#ವ್ಯಾಖ್ಯಾನಿಸು FS_IOC_FIEMAP	_IOWR('f', 11, ರಚನೆ fiemap)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && !defined(FS_IOC_GETSTATE)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, ರಚನೆ fiemap)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_MAX_ಸ್ಥಾನಾಂತರ	(~0ULL)

#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_ಕೊನೆಯ		0x00000001 /* Last extent in file. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_ದತ್ತಾಂಶ_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_ದತ್ತಾಂಶ_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_ದತ್ತಾಂಶ_ಬಾಲ		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#ವ್ಯಾಖ್ಯಾನಿಸು FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#ಆದರೆ_ಅಂತ್ಯ /* _LINUX_FIEMAP_H */
