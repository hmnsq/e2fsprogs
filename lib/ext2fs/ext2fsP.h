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

#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ext2fsP_is_disk_device(mode_t mode)
{
#ಯದಿ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ಪ್ರತ್ಯಾಗಮ S_ISBLK(mode) || S_ISCHR(mode);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ S_ISBLK(mode);
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ time_t ext2fsP_get_time(ext2_filsys fs)
{
	ಯದಿ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		ಪ್ರತ್ಯಾಗಮ fs->now;
	ಪ್ರತ್ಯಾಗಮ time(ರಿಕ್ತ);
}

/*
 * Badblocks list
 */
ಸಂರಚನಾ ext2_struct_u32_list {
	ಪೂರ್ಣಾಙ್ಕ	magic;
	ಪೂರ್ಣಾಙ್ಕ	ಅಂಕೆ;
	ಪೂರ್ಣಾಙ್ಕ	size;
	__u32	*list;
	ಪೂರ್ಣಾಙ್ಕ	badblocks_flags;
};

ಸಂರಚನಾ ext2_struct_u32_iterate {
	ಪೂರ್ಣಾಙ್ಕ			magic;
	ext2_u32_list		bb;
	ಪೂರ್ಣಾಙ್ಕ			ptr;
};


/*
 * Directory block iterator definition
 */
ಸಂರಚನಾ ext2_struct_dblist {
	ಪೂರ್ಣಾಙ್ಕ			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	ಎಣಿಕೆ;
	ಪೂರ್ಣಾಙ್ಕ			sorted;
	ಸಂರಚನಾ ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ಸಂರಚನಾ dir_context {
	ext2_ino_t		dir;
	ಪೂರ್ಣಾಙ್ಕ		flags;
	ವರ್ಣ		*buf;
	unsigned ಪೂರ್ಣಾಙ್ಕ	buflen;
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_ino_t	dir,
		    ಪೂರ್ಣಾಙ್ಕ	entry,
		    ಸಂರಚನಾ ext2_dir_entry *dirent,
		    ಪೂರ್ಣಾಙ್ಕ	offset,
		    ಪೂರ್ಣಾಙ್ಕ	blocksize,
		    ವರ್ಣ	*buf,
		    ಶೂನ್ಯ	*priv_data);
	ಶೂನ್ಯ		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ಸಂರಚನಾ ext2_inode_cache {
	ಶೂನ್ಯ *				buffer;
	blk64_t				buffer_blk;
	ಪೂರ್ಣಾಙ್ಕ				cache_last;
	unsigned ಪೂರ್ಣಾಙ್ಕ			cache_size;
	ಪೂರ್ಣಾಙ್ಕ				refcount;
	ಸಂರಚನಾ ext2_inode_cache_ent	*cache;
};

ಸಂರಚನಾ ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ಸಂರಚನಾ ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ಸಂರಚನಾ ext2fs_nls_table {
	ಪೂರ್ಣಾಙ್ಕ version;
	ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_ops *ops;
};

ಸಂರಚನಾ ext2fs_nls_ops {
	ಪೂರ್ಣಾಙ್ಕ (*casefold)(ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *charset,
			ಧ್ರುವ unsigned ವರ್ಣ *str, size_t len,
			unsigned ವರ್ಣ *dest, size_t dlen);
	ಪೂರ್ಣಾಙ್ಕ (*validate)(ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *table,
			ವರ್ಣ *s, size_t len, ವರ್ಣ **pos);
	ಪೂರ್ಣಾಙ್ಕ (*casefold_cmp)(ಧ್ರುವ ಸಂರಚನಾ ext2fs_nls_table *table,
			    ಧ್ರುವ unsigned ವರ್ಣ *str1, size_t len1,
			    ಧ್ರುವ unsigned ವರ್ಣ *str2, size_t len2);
};

/* Function prototypes */

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    ಪೂರ್ಣಾಙ್ಕ			ref_offset,
				    ಶೂನ್ಯ		*priv_data);

ಬಾಹ್ಯ errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  ಶೂನ್ಯ *priv_data);

/* Generic numeric progress meter */

ಸಂರಚನಾ ext2fs_numeric_progress_struct {
	__u64		max;
	ಪೂರ್ಣಾಙ್ಕ		log_max;
	ಪೂರ್ಣಾಙ್ಕ		skip_progress;
};

/*
 * progress callback functions
 */
ಸಂರಚನಾ ext2fs_progress_ops {
	ಶೂನ್ಯ (*init)(ext2_filsys fs,
		     ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
		     ಧ್ರುವ ವರ್ಣ *label, __u64 max);
	ಶೂನ್ಯ (*update)(ext2_filsys fs,
		       ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	ಶೂನ್ಯ (*close)(ext2_filsys fs,
		      ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
		      ಧ್ರುವ ವರ್ಣ *message);
};

ಬಾಹ್ಯ ಸಂರಚನಾ ext2fs_progress_ops ext2fs_numeric_progress_ops;

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_progress_init(ext2_filsys fs,
					 ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
					 ಧ್ರುವ ವರ್ಣ *label, __u64 max);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_progress_update(ext2_filsys fs,
					   ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
					   __u64 val);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_numeric_progress_close(ext2_filsys fs,
					  ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
					  ಧ್ರುವ ವರ್ಣ *message);

/*
 * 64-bit bitmap support
 */

ಬಾಹ್ಯ errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   ಪೂರ್ಣಾಙ್ಕ type, __u64 start, __u64 end,
					   __u64 real_end,
					   ಧ್ರುವ ವರ್ಣ * description,
					   ext2fs_generic_bitmap *bmap);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

ಬಾಹ್ಯ errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

ಬಾಹ್ಯ errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
ಬಾಹ್ಯ errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ಬಾಹ್ಯ errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					       ಶೂನ್ಯ *in);
ಬಾಹ್ಯ errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
					       ಶೂನ್ಯ *out);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ಧ್ರುವ ವರ್ಣ *func);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_mem_is_zero(ಧ್ರುವ ವರ್ಣ *mem, size_t len);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    ಸಂರಚನಾ ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ext2_exit_fn)(ಶೂನ್ಯ *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, ಶೂನ್ಯ *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, ಶೂನ್ಯ *data);

#define ARRAY_SIZE(array)			\
        (ಪರಿಮಾಣ(array) / ಪರಿಮಾಣ(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((ಶೂನ್ಯ)ಪರಿಮಾಣ(ವರ್ಣ[1 - 2*!!(cond)]))
