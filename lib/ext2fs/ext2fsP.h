/*
 * ext2fsP.h --- private header file for ext2 library
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_MAX_NESTED_LINKS  8

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fsP_is_disk_device(mode_t ಬಹುಲಕ)
{
#ಆದರೆ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ಹಿಂತಿರುಗಿ S_ISBLK(ಬಹುಲಕ) || S_ISCHR(ಬಹುಲಕ);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ S_ISBLK(ಬಹುಲಕ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ time_t ext2fs_P_get_ಸಮಯ(ext2_filsys fs)
{
	ಆದರೆ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_ಸಮಯ))
		ಹಿಂತಿರುಗಿ fs->now;
	ಹಿಂತಿರುಗಿ time(ಖಾಲಿ);
}

/*
 * Badblocks list
 */
ರಚನೆ ext2_struct_u32_list {
	ಪೂರ್ಣಾಂಕ	magic;
	ಪೂರ್ಣಾಂಕ	ಅಂಕೆ;
	ಪೂರ್ಣಾಂಕ	ಗಾತ್ರ_2;
	__u32	*list;
	ಪೂರ್ಣಾಂಕ	badblocks_flags;
};

ರಚನೆ ext2_struct_u32_iterate {
	ಪೂರ್ಣಾಂಕ			magic;
	ext2_u32_list		bb;
	ಪೂರ್ಣಾಂಕ			ptr;
};


/*
 * Directory block iterator definition
 */
ರಚನೆ ext2_struct_dblist {
	ಪೂರ್ಣಾಂಕ			magic;
	ext2_filsys		fs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಎಣಿಕೆ_2;
	ಪೂರ್ಣಾಂಕ			sorted;
	ರಚನೆ ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ರಚನೆ dir_context {
	ext2_ino_t		dir;
	ಪೂರ್ಣಾಂಕ		flags;
	ಅಕ್ಷರ		*buf;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	buflen;
	ಪೂರ್ಣಾಂಕ (*func)(ext2_ino_t	dir,
		    ಪೂರ್ಣಾಂಕ	ನಮೂದು,
		    ರಚನೆ ext2_dir_ನಮೂದು *dirent,
		    ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
		    ಪೂರ್ಣಾಂಕ	blocksize,
		    ಅಕ್ಷರ	*buf,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ		*priv_ದತ್ತಾಂಶ;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ರಚನೆ ext2_inode_cache {
	ಶೂನ್ಯ *				ಬಫರ್;
	blk64_t				ಬಫರ್_blk;
	ಪೂರ್ಣಾಂಕ				cache_ಕೊನೆಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ			cache_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ				refcount;
	ರಚನೆ ext2_inode_cache_ent	*cache;
};

ರಚನೆ ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ರಚನೆ ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ {
	ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ops *ops;
};

ರಚನೆ ext2fs_nls_ops {
	ಪೂರ್ಣಾಂಕ (*casefold)(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *charset,
			ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str, size_t len,
			ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *dest, size_t dlen);
	ಪೂರ್ಣಾಂಕ (*validate)(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			ಅಕ್ಷರ *s, size_t len, ಅಕ್ಷರ **pos);
	ಪೂರ್ಣಾಂಕ (*casefold_cmp)(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str1, size_t len1,
			    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str2, size_t len2);
};

/* Function prototypes */

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_process_dir_ಖಂಡ(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_ಖಂಡ,
				    ಪೂರ್ಣಾಂಕ			ref_ಸ್ಥಾನಾಂತರ,
				    ಶೂನ್ಯ		*priv_ದತ್ತಾಂಶ);

ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ea_remove(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_expand(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_inline_ದತ್ತಾಂಶ_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

/* Generic numeric progress meter */

ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct {
	__u64		max;
	ಪೂರ್ಣಾಂಕ		log_max;
	ಪೂರ್ಣಾಂಕ		skip_ಪ್ರಗತಿ;
};

/*
 * progress callback functions
 */
ರಚನೆ ext2fs_ಪ್ರಗತಿ_ops {
	ಶೂನ್ಯ (*init)(ext2_filsys fs,
		     ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, __u64 max);
	ಶೂನ್ಯ (*update)(ext2_filsys fs,
		       ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
		       __u64 val);
	ಶೂನ್ಯ (*close)(ext2_filsys fs,
		      ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_3);
};

ಬಾಹ್ಯ ರಚನೆ ext2fs_ಪ್ರಗತಿ_ops ext2fs_numeric_ಪ್ರಗತಿ_ops;

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_init(ext2_filsys fs,
					 ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, __u64 max);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_update(ext2_filsys fs,
					   ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
					   __u64 val);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_close(ext2_filsys fs,
					  ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_3);

/*
 * 64-bit bitmap support
 */

ಬಾಹ್ಯ errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   ಪೂರ್ಣಾಂಕ type, __u64 ಆರಂಭ, __u64 ಕೊನೆ,
					   __u64 real_ಕೊನೆ,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * description,
					   ext2fs_generic_bitmap *bmap);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

ಬಾಹ್ಯ errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

ಬಾಹ್ಯ errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_ಕೊನೆ,
					    __u64 new_real_ಕೊನೆ);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_generic_bmap_ಕೊನೆ(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 ಕೊನೆ, __u64 *oend);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ಬಾಹ್ಯ errcode_t ext2fs_ಗಣ_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					       ಶೂನ್ಯ *ಒಳ);
ಬಾಹ್ಯ errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					       ಶೂನ್ಯ *ಹೊರಹೋಗು);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mem_is_zero(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem, size_t len);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_file_ಖಂಡ_ಸ್ಥಾನಾಂತರ_too_big(ext2_filsys fs,
					    ರಚನೆ ext2_inode *inode,
					    blk64_t ಸ್ಥಾನಾಂತರ);

/* atexit support */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ext2_exit_fn)(ಶೂನ್ಯ *);
errcode_t ext2fs_ಕೂಡಿಸು_exit_fn(ext2_exit_fn fn, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, ಶೂನ್ಯ *ದತ್ತಾಂಶ);

#ವ್ಯಾಖ್ಯಾನಿಸು ARRAY_ಗಾತ್ರ(array)			\
        (ಗಾತ್ರ(array) / ಗಾತ್ರ(array[0]))

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_BUILD_BUG_ON(cond) ((ಶೂನ್ಯ)ಗಾತ್ರ(ಅಕ್ಷರ[1 - 2*!!(cond)]))
