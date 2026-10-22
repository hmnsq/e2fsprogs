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

স্থির অন্তঃস্থ পূর্ণাঙ্ক ext2fsP_is_disk_device(mode_t mode)
{
#যদি defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	প্রত্যাগম S_ISBLK(mode) || S_ISCHR(mode);
#অন্যথা
	প্রত্যাগম S_ISBLK(mode);
#endif
}

স্থির অন্তঃস্থ time_t ext2fsP_get_time(ext2_filsys fs)
{
	যদি (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		প্রত্যাগম fs->now;
	প্রত্যাগম time(রিক্ত);
}

/*
 * Badblocks list
 */
সংরচনা ext2_struct_u32_list {
	পূর্ণাঙ্ক	magic;
	পূর্ণাঙ্ক	অঙ্ক;
	পূর্ণাঙ্ক	size;
	__u32	*list;
	পূর্ণাঙ্ক	badblocks_flags;
};

সংরচনা ext2_struct_u32_iterate {
	পূর্ণাঙ্ক			magic;
	ext2_u32_list		bb;
	পূর্ণাঙ্ক			ptr;
};


/*
 * Directory block iterator definition
 */
সংরচনা ext2_struct_dblist {
	পূর্ণাঙ্ক			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	গুনতি;
	পূর্ণাঙ্ক			sorted;
	সংরচনা ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
সংরচনা dir_context {
	ext2_ino_t		dir;
	পূর্ণাঙ্ক		flags;
	বর্ণ		*buf;
	unsigned পূর্ণাঙ্ক	buflen;
	পূর্ণাঙ্ক (*func)(ext2_ino_t	dir,
		    পূর্ণাঙ্ক	entry,
		    সংরচনা ext2_dir_entry *dirent,
		    পূর্ণাঙ্ক	offset,
		    পূর্ণাঙ্ক	blocksize,
		    বর্ণ	*buf,
		    শূন্য	*priv_data);
	শূন্য		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
সংরচনা ext2_inode_cache {
	শূন্য *				buffer;
	blk64_t				buffer_blk;
	পূর্ণাঙ্ক				cache_last;
	unsigned পূর্ণাঙ্ক			cache_size;
	পূর্ণাঙ্ক				refcount;
	সংরচনা ext2_inode_cache_ent	*cache;
};

সংরচনা ext2_inode_cache_ent {
	ext2_ino_t		ino;
	সংরচনা ext2_inode	*inode;
};

/*
 * NLS definitions
 */
সংরচনা ext2fs_nls_table {
	পূর্ণাঙ্ক version;
	ধ্রুব সংরচনা ext2fs_nls_ops *ops;
};

সংরচনা ext2fs_nls_ops {
	পূর্ণাঙ্ক (*casefold)(ধ্রুব সংরচনা ext2fs_nls_table *charset,
			ধ্রুব unsigned বর্ণ *str, size_t len,
			unsigned বর্ণ *dest, size_t dlen);
	পূর্ণাঙ্ক (*validate)(ধ্রুব সংরচনা ext2fs_nls_table *table,
			বর্ণ *s, size_t len, বর্ণ **pos);
	পূর্ণাঙ্ক (*casefold_cmp)(ধ্রুব সংরচনা ext2fs_nls_table *table,
			    ধ্রুব unsigned বর্ণ *str1, size_t len1,
			    ধ্রুব unsigned বর্ণ *str2, size_t len2);
};

/* Function prototypes */

বাহ্য পূর্ণাঙ্ক ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    পূর্ণাঙ্ক			ref_offset,
				    শূন্য		*priv_data);

বাহ্য errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
বাহ্য errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
বাহ্য পূর্ণাঙ্ক ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  শূন্য *priv_data);

/* Generic numeric progress meter */

সংরচনা ext2fs_numeric_progress_struct {
	__u64		max;
	পূর্ণাঙ্ক		log_max;
	পূর্ণাঙ্ক		skip_progress;
};

/*
 * progress callback functions
 */
সংরচনা ext2fs_progress_ops {
	শূন্য (*init)(ext2_filsys fs,
		     সংরচনা ext2fs_numeric_progress_struct * progress,
		     ধ্রুব বর্ণ *label, __u64 max);
	শূন্য (*update)(ext2_filsys fs,
		       সংরচনা ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	শূন্য (*close)(ext2_filsys fs,
		      সংরচনা ext2fs_numeric_progress_struct * progress,
		      ধ্রুব বর্ণ *message);
};

বাহ্য সংরচনা ext2fs_progress_ops ext2fs_numeric_progress_ops;

বাহ্য শূন্য ext2fs_numeric_progress_init(ext2_filsys fs,
					 সংরচনা ext2fs_numeric_progress_struct * progress,
					 ধ্রুব বর্ণ *label, __u64 max);
বাহ্য শূন্য ext2fs_numeric_progress_update(ext2_filsys fs,
					   সংরচনা ext2fs_numeric_progress_struct * progress,
					   __u64 val);
বাহ্য শূন্য ext2fs_numeric_progress_close(ext2_filsys fs,
					  সংরচনা ext2fs_numeric_progress_struct * progress,
					  ধ্রুব বর্ণ *message);

/*
 * 64-bit bitmap support
 */

বাহ্য errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   পূর্ণাঙ্ক type, __u64 start, __u64 end,
					   __u64 real_end,
					   ধ্রুব বর্ণ * description,
					   ext2fs_generic_bitmap *bmap);

বাহ্য শূন্য ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

বাহ্য errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

বাহ্য errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
বাহ্য errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
বাহ্য পূর্ণাঙ্ক ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
বাহ্য পূর্ণাঙ্ক ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
বাহ্য পূর্ণাঙ্ক ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
বাহ্য errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned পূর্ণাঙ্ক অঙ্ক,
					       শূন্য *in);
বাহ্য errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned পূর্ণাঙ্ক অঙ্ক,
					       শূন্য *out);
বাহ্য শূন্য ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ধ্রুব বর্ণ *func);

বাহ্য পূর্ণাঙ্ক ext2fs_mem_is_zero(ধ্রুব বর্ণ *mem, size_t len);

বাহ্য পূর্ণাঙ্ক ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    সংরচনা ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
প্রকারনির্দেশ শূন্য (*ext2_exit_fn)(শূন্য *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, শূন্য *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, শূন্য *data);

#define ARRAY_SIZE(array)			\
        (পরিমাণ(array) / পরিমাণ(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((শূন্য)পরিমাণ(বর্ণ[1 - 2*!!(cond)]))
