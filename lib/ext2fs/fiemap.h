/*
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#నిర్వచితం_కాకపోతే _LINUX_FIEMAP_H
#నిర్వచించు _LINUX_FIEMAP_H

నిర్మాణం fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_నిడివి;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

నిర్మాణం fiemap {
	__u64 fm_ప్రారంభం;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_నిడివి;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_లెక్క;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wpedantic"
#అయితే_ముగింపు
	నిర్మాణం fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు
};

#ఒకవేళ defined(__linux__) && !defined(FS_IOC_FIEMAP)
#నిర్వచించు FS_IOC_FIEMAP	_IOWR('f', 11, నిర్మాణం fiemap)
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && !defined(FS_IOC_GETSTATE)
#నిర్వచించు EXT4_IOC_GETSTATE		_IOW('f', 41, __u32)
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && !defined(EXT4_IOC_GET_ES_CACHE)
#నిర్వచించు EXT4_IOC_GET_ES_CACHE		_IOWR('f', 42, నిర్మాణం fiemap)
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && !defined(EXT4_STATE_FLAG_EXT_PRECACHED)
#నిర్వచించు EXT4_STATE_FLAG_EXT_PRECACHED	0x00000001
#అయితే_ముగింపు

#నిర్వచించు FIEMAP_MAX_స్థానభ్రంశం	(~0ULL)

#నిర్వచించు FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#నిర్వచించు FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
#నిర్వచించు FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */

#నిర్వచించు FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#నిర్వచించు FIEMAP_EXTENT_చివరి		0x00000001 /* Last extent in file. */
#నిర్వచించు FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#నిర్వచించు FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#నిర్వచించు FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#నిర్వచించు FIEMAP_EXTENT_దత్తాంశం_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#నిర్వచించు FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#నిర్వచించు FIEMAP_EXTENT_దత్తాంశం_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#నిర్వచించు FIEMAP_EXTENT_దత్తాంశం_తోక		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#నిర్వచించు FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#నిర్వచించు FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */
#నిర్వచించు FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
						    * files. */

#నిర్వచించు EXT4_FIEMAP_EXTENT_HOLE		0x08000000 /* Entry in extent status
						      cache for a hole*/

#అయితే_ముగింపు /* _LINUX_FIEMAP_H */
