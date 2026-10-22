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

#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif

#அந்தர்பாவ "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

நிலையான உள்வரி முழு ext2fsP_is_disk_device(mode_t mode)
{
#எனில் defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	திரும்பு S_ISBLK(mode) || S_ISCHR(mode);
#இல்லையெனில்
	திரும்பு S_ISBLK(mode);
#endif
}

நிலையான உள்வரி time_t ext2fsP_get_time(ext2_filsys fs)
{
	எனில் (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		திரும்பு fs->now;
	திரும்பு time(வெற்று);
}

/*
 * Badblocks list
 */
கட்டமைப்பு ext2_struct_u32_list {
	முழு	magic;
	முழு	இலக்கம்;
	முழு	size;
	__u32	*list;
	முழு	badblocks_flags;
};

கட்டமைப்பு ext2_struct_u32_iterate {
	முழு			magic;
	ext2_u32_list		bb;
	முழு			ptr;
};


/*
 * Directory block iterator definition
 */
கட்டமைப்பு ext2_struct_dblist {
	முழு			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	மொத்தம்;
	முழு			sorted;
	கட்டமைப்பு ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
கட்டமைப்பு dir_context {
	ext2_ino_t		dir;
	முழு		flags;
	எழுத்து		*buf;
	unsigned முழு	buflen;
	முழு (*func)(ext2_ino_t	dir,
		    முழு	entry,
		    கட்டமைப்பு ext2_dir_entry *dirent,
		    முழு	offset,
		    முழு	blocksize,
		    எழுத்து	*buf,
		    வெறுமை	*priv_data);
	வெறுமை		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
கட்டமைப்பு ext2_inode_cache {
	வெறுமை *				buffer;
	blk64_t				buffer_blk;
	முழு				cache_last;
	unsigned முழு			cache_size;
	முழு				refcount;
	கட்டமைப்பு ext2_inode_cache_ent	*cache;
};

கட்டமைப்பு ext2_inode_cache_ent {
	ext2_ino_t		ino;
	கட்டமைப்பு ext2_inode	*inode;
};

/*
 * NLS definitions
 */
கட்டமைப்பு ext2fs_nls_table {
	முழு version;
	மாறிலி கட்டமைப்பு ext2fs_nls_ops *ops;
};

கட்டமைப்பு ext2fs_nls_ops {
	முழு (*casefold)(மாறிலி கட்டமைப்பு ext2fs_nls_table *charset,
			மாறிலி unsigned எழுத்து *str, size_t len,
			unsigned எழுத்து *dest, size_t dlen);
	முழு (*validate)(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			எழுத்து *s, size_t len, எழுத்து **pos);
	முழு (*casefold_cmp)(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			    மாறிலி unsigned எழுத்து *str1, size_t len1,
			    மாறிலி unsigned எழுத்து *str2, size_t len2);
};

/* Function prototypes */

வெளி முழு ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    முழு			ref_offset,
				    வெறுமை		*priv_data);

வெளி errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
வெளி errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
வெளி முழு ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  வெறுமை *priv_data);

/* Generic numeric progress meter */

கட்டமைப்பு ext2fs_numeric_progress_struct {
	__u64		max;
	முழு		log_max;
	முழு		skip_progress;
};

/*
 * progress callback functions
 */
கட்டமைப்பு ext2fs_progress_ops {
	வெறுமை (*init)(ext2_filsys fs,
		     கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
		     மாறிலி எழுத்து *label, __u64 max);
	வெறுமை (*update)(ext2_filsys fs,
		       கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	வெறுமை (*close)(ext2_filsys fs,
		      கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
		      மாறிலி எழுத்து *message);
};

வெளி கட்டமைப்பு ext2fs_progress_ops ext2fs_numeric_progress_ops;

வெளி வெறுமை ext2fs_numeric_progress_init(ext2_filsys fs,
					 கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
					 மாறிலி எழுத்து *label, __u64 max);
வெளி வெறுமை ext2fs_numeric_progress_update(ext2_filsys fs,
					   கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
					   __u64 val);
வெளி வெறுமை ext2fs_numeric_progress_close(ext2_filsys fs,
					  கட்டமைப்பு ext2fs_numeric_progress_struct * progress,
					  மாறிலி எழுத்து *message);

/*
 * 64-bit bitmap support
 */

வெளி errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   முழு type, __u64 start, __u64 end,
					   __u64 real_end,
					   மாறிலி எழுத்து * description,
					   ext2fs_generic_bitmap *bmap);

வெளி வெறுமை ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

வெளி errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

வெளி errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
வெளி errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
வெளி முழு ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
வெளி முழு ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
வெளி முழு ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
வெளி errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned முழு இலக்கம்,
					       வெறுமை *in);
வெளி errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned முழு இலக்கம்,
					       வெறுமை *out);
வெளி வெறுமை ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,மாறிலி எழுத்து *func);

வெளி முழு ext2fs_mem_is_zero(மாறிலி எழுத்து *mem, size_t len);

வெளி முழு ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    கட்டமைப்பு ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
வகைவரை வெறுமை (*ext2_exit_fn)(வெறுமை *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, வெறுமை *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, வெறுமை *data);

#define ARRAY_SIZE(array)			\
        (அளவு(array) / அளவு(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((வெறுமை)அளவு(எழுத்து[1 - 2*!!(cond)]))
