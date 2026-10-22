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

#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif

#شامل "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

ایستا درونخطی صحیح ext2fsP_is_disk_device(mode_t mode)
{
#اگر defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	بازگشت S_ISBLK(mode) || S_ISCHR(mode);
#وگرنه
	بازگشت S_ISBLK(mode);
#endif
}

ایستا درونخطی time_t ext2fsP_get_time(ext2_filsys fs)
{
	اگر (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		بازگشت fs->now;
	بازگشت time(پوچ);
}

/*
 * Badblocks list
 */
ساختار ext2_struct_u32_list {
	صحیح	magic;
	صحیح	رقم;
	صحیح	size;
	__u32	*list;
	صحیح	badblocks_flags;
};

ساختار ext2_struct_u32_iterate {
	صحیح			magic;
	ext2_u32_list		bb;
	صحیح			ptr;
};


/*
 * Directory block iterator definition
 */
ساختار ext2_struct_dblist {
	صحیح			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	شمار;
	صحیح			sorted;
	ساختار ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ساختار dir_context {
	ext2_ino_t		dir;
	صحیح		flags;
	نویسه		*buf;
	unsigned صحیح	buflen;
	صحیح (*func)(ext2_ino_t	dir,
		    صحیح	entry,
		    ساختار ext2_dir_entry *dirent,
		    صحیح	offset,
		    صحیح	blocksize,
		    نویسه	*buf,
		    تهی	*priv_data);
	تهی		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ساختار ext2_inode_cache {
	تهی *				buffer;
	blk64_t				buffer_blk;
	صحیح				cache_last;
	unsigned صحیح			cache_size;
	صحیح				refcount;
	ساختار ext2_inode_cache_ent	*cache;
};

ساختار ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ساختار ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ساختار ext2fs_nls_table {
	صحیح version;
	ثابت ساختار ext2fs_nls_ops *ops;
};

ساختار ext2fs_nls_ops {
	صحیح (*casefold)(ثابت ساختار ext2fs_nls_table *charset,
			ثابت unsigned نویسه *str, size_t len,
			unsigned نویسه *dest, size_t dlen);
	صحیح (*validate)(ثابت ساختار ext2fs_nls_table *table,
			نویسه *s, size_t len, نویسه **pos);
	صحیح (*casefold_cmp)(ثابت ساختار ext2fs_nls_table *table,
			    ثابت unsigned نویسه *str1, size_t len1,
			    ثابت unsigned نویسه *str2, size_t len2);
};

/* Function prototypes */

بیرونی صحیح ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    صحیح			ref_offset,
				    تهی		*priv_data);

بیرونی errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
بیرونی errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
بیرونی صحیح ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  تهی *priv_data);

/* Generic numeric progress meter */

ساختار ext2fs_numeric_progress_struct {
	__u64		max;
	صحیح		log_max;
	صحیح		skip_progress;
};

/*
 * progress callback functions
 */
ساختار ext2fs_progress_ops {
	تهی (*init)(ext2_filsys fs,
		     ساختار ext2fs_numeric_progress_struct * progress,
		     ثابت نویسه *label, __u64 max);
	تهی (*update)(ext2_filsys fs,
		       ساختار ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	تهی (*close)(ext2_filsys fs,
		      ساختار ext2fs_numeric_progress_struct * progress,
		      ثابت نویسه *message);
};

بیرونی ساختار ext2fs_progress_ops ext2fs_numeric_progress_ops;

بیرونی تهی ext2fs_numeric_progress_init(ext2_filsys fs,
					 ساختار ext2fs_numeric_progress_struct * progress,
					 ثابت نویسه *label, __u64 max);
بیرونی تهی ext2fs_numeric_progress_update(ext2_filsys fs,
					   ساختار ext2fs_numeric_progress_struct * progress,
					   __u64 val);
بیرونی تهی ext2fs_numeric_progress_close(ext2_filsys fs,
					  ساختار ext2fs_numeric_progress_struct * progress,
					  ثابت نویسه *message);

/*
 * 64-bit bitmap support
 */

بیرونی errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   صحیح type, __u64 start, __u64 end,
					   __u64 real_end,
					   ثابت نویسه * description,
					   ext2fs_generic_bitmap *bmap);

بیرونی تهی ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

بیرونی errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

بیرونی errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
بیرونی errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
بیرونی صحیح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
بیرونی صحیح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
بیرونی صحیح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
بیرونی errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned صحیح رقم,
					       تهی *in);
بیرونی errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned صحیح رقم,
					       تهی *out);
بیرونی تهی ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ثابت نویسه *func);

بیرونی صحیح ext2fs_mem_is_zero(ثابت نویسه *mem, size_t len);

بیرونی صحیح ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    ساختار ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
تعریفنوع تهی (*ext2_exit_fn)(تهی *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, تهی *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, تهی *data);

#define ARRAY_SIZE(array)			\
        (اندازه(array) / اندازه(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((تهی)اندازه(نویسه[1 - 2*!!(cond)]))
