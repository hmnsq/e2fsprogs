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

#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif

#অন্তর্ভাব "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

স্থির অন্তঃরেখ পূর্ণসংখ্যা ext2fsP_is_disk_device(mode_t mode)
{
#যদি defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ফেরত S_ISBLK(mode) || S_ISCHR(mode);
#অন্যথা
	ফেরত S_ISBLK(mode);
#endif
}

স্থির অন্তঃরেখ time_t ext2fsP_get_time(ext2_filsys fs)
{
	যদি (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		ফেরত fs->now;
	ফেরত time(ফাঁকা);
}

/*
 * Badblocks list
 */
কাঠামো ext2_struct_u32_list {
	পূর্ণসংখ্যা	magic;
	পূর্ণসংখ্যা	অঙ্ক;
	পূর্ণসংখ্যা	আকার_2;
	__u32	*list;
	পূর্ণসংখ্যা	badblocks_flags;
};

কাঠামো ext2_struct_u32_iterate {
	পূর্ণসংখ্যা			magic;
	ext2_u32_list		bb;
	পূর্ণসংখ্যা			ptr;
};


/*
 * Directory block iterator definition
 */
কাঠামো ext2_struct_dblist {
	পূর্ণসংখ্যা			magic;
	ext2_filsys		fs;
	unsigned long long	আকার_2;
	unsigned long long	গুনতি;
	পূর্ণসংখ্যা			sorted;
	কাঠামো ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
কাঠামো dir_context {
	ext2_ino_t		dir;
	পূর্ণসংখ্যা		flags;
	অক্ষর		*buf;
	unsigned পূর্ণসংখ্যা	buflen;
	পূর্ণসংখ্যা (*func)(ext2_ino_t	dir,
		    পূর্ণসংখ্যা	entry,
		    কাঠামো ext2_dir_entry *dirent,
		    পূর্ণসংখ্যা	offset,
		    পূর্ণসংখ্যা	blocksize,
		    অক্ষর	*buf,
		    শূন্য	*priv_তথ্য);
	শূন্য		*priv_তথ্য;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
কাঠামো ext2_inode_cache {
	শূন্য *				বাফার;
	blk64_t				বাফার_blk;
	পূর্ণসংখ্যা				cache_last;
	unsigned পূর্ণসংখ্যা			cache_আকার;
	পূর্ণসংখ্যা				refcount;
	কাঠামো ext2_inode_cache_ent	*cache;
};

কাঠামো ext2_inode_cache_ent {
	ext2_ino_t		ino;
	কাঠামো ext2_inode	*inode;
};

/*
 * NLS definitions
 */
কাঠামো ext2fs_nls_table {
	পূর্ণসংখ্যা version;
	ধ্রুবক কাঠামো ext2fs_nls_ops *ops;
};

কাঠামো ext2fs_nls_ops {
	পূর্ণসংখ্যা (*casefold)(ধ্রুবক কাঠামো ext2fs_nls_table *charset,
			ধ্রুবক unsigned অক্ষর *str, size_t len,
			unsigned অক্ষর *dest, size_t dlen);
	পূর্ণসংখ্যা (*validate)(ধ্রুবক কাঠামো ext2fs_nls_table *table,
			অক্ষর *s, size_t len, অক্ষর **pos);
	পূর্ণসংখ্যা (*casefold_cmp)(ধ্রুবক কাঠামো ext2fs_nls_table *table,
			    ধ্রুবক unsigned অক্ষর *str1, size_t len1,
			    ধ্রুবক unsigned অক্ষর *str2, size_t len2);
};

/* Function prototypes */

বাহ্যিক পূর্ণসংখ্যা ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    পূর্ণসংখ্যা			ref_offset,
				    শূন্য		*priv_তথ্য);

বাহ্যিক errcode_t ext2fs_inline_তথ্য_ea_remove(ext2_filsys fs, ext2_ino_t ino);
বাহ্যিক errcode_t ext2fs_inline_তথ্য_expand(ext2_filsys fs, ext2_ino_t ino);
বাহ্যিক পূর্ণসংখ্যা ext2fs_inline_তথ্য_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  শূন্য *priv_তথ্য);

/* Generic numeric progress meter */

কাঠামো ext2fs_numeric_progress_struct {
	__u64		max;
	পূর্ণসংখ্যা		log_max;
	পূর্ণসংখ্যা		skip_progress;
};

/*
 * progress callback functions
 */
কাঠামো ext2fs_progress_ops {
	শূন্য (*init)(ext2_filsys fs,
		     কাঠামো ext2fs_numeric_progress_struct * progress,
		     ধ্রুবক অক্ষর *label, __u64 max);
	শূন্য (*update)(ext2_filsys fs,
		       কাঠামো ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	শূন্য (*close)(ext2_filsys fs,
		      কাঠামো ext2fs_numeric_progress_struct * progress,
		      ধ্রুবক অক্ষর *message);
};

বাহ্যিক কাঠামো ext2fs_progress_ops ext2fs_numeric_progress_ops;

বাহ্যিক শূন্য ext2fs_numeric_progress_init(ext2_filsys fs,
					 কাঠামো ext2fs_numeric_progress_struct * progress,
					 ধ্রুবক অক্ষর *label, __u64 max);
বাহ্যিক শূন্য ext2fs_numeric_progress_update(ext2_filsys fs,
					   কাঠামো ext2fs_numeric_progress_struct * progress,
					   __u64 val);
বাহ্যিক শূন্য ext2fs_numeric_progress_close(ext2_filsys fs,
					  কাঠামো ext2fs_numeric_progress_struct * progress,
					  ধ্রুবক অক্ষর *message);

/*
 * 64-bit bitmap support
 */

বাহ্যিক errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   পূর্ণসংখ্যা type, __u64 start, __u64 end,
					   __u64 real_end,
					   ধ্রুবক অক্ষর * description,
					   ext2fs_generic_bitmap *bmap);

বাহ্যিক শূন্য ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

বাহ্যিক errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

বাহ্যিক errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
বাহ্যিক errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
বাহ্যিক পূর্ণসংখ্যা ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
বাহ্যিক পূর্ণসংখ্যা ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
বাহ্যিক পূর্ণসংখ্যা ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
বাহ্যিক errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					       শূন্য *in);
বাহ্যিক errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					       শূন্য *out);
বাহ্যিক শূন্য ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ধ্রুবক অক্ষর *func);

বাহ্যিক পূর্ণসংখ্যা ext2fs_mem_is_zero(ধ্রুবক অক্ষর *mem, size_t len);

বাহ্যিক পূর্ণসংখ্যা ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    কাঠামো ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
প্রকারলেখ শূন্য (*ext2_exit_fn)(শূন্য *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, শূন্য *তথ্য);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, শূন্য *তথ্য);

#define ARRAY_আকার(array)			\
        (আকার(array) / আকার(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((শূন্য)আকার(অক্ষর[1 - 2*!!(cond)]))
