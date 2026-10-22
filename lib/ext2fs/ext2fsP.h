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

#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif

#ضمن "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

ساكن مضمن صحيح ext2fsP_is_disk_device(mode_t mode)
{
#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	أرجع S_ISBLK(mode) || S_ISCHR(mode);
#وإلا
	أرجع S_ISBLK(mode);
#endif
}

ساكن مضمن time_t ext2fsP_get_time(ext2_filsys fs)
{
	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		أرجع fs->now;
	أرجع time(عدم);
}

/*
 * Badblocks list
 */
بنية ext2_struct_u32_list {
	صحيح	magic;
	صحيح	ژمارە;
	صحيح	size;
	__u32	*list;
	صحيح	badblocks_flags;
};

بنية ext2_struct_u32_iterate {
	صحيح			magic;
	ext2_u32_list		bb;
	صحيح			ptr;
};


/*
 * Directory block iterator definition
 */
بنية ext2_struct_dblist {
	صحيح			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	ژماردن;
	صحيح			sorted;
	بنية ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
بنية dir_context {
	ext2_ino_t		dir;
	صحيح		flags;
	حرف		*buf;
	unsigned صحيح	buflen;
	صحيح (*func)(ext2_ino_t	dir,
		    صحيح	entry,
		    بنية ext2_dir_entry *dirent,
		    صحيح	offset,
		    صحيح	blocksize,
		    حرف	*buf,
		    فراغ	*priv_data);
	فراغ		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
بنية ext2_inode_cache {
	فراغ *				buffer;
	blk64_t				buffer_blk;
	صحيح				cache_last;
	unsigned صحيح			cache_size;
	صحيح				refcount;
	بنية ext2_inode_cache_ent	*cache;
};

بنية ext2_inode_cache_ent {
	ext2_ino_t		ino;
	بنية ext2_inode	*inode;
};

/*
 * NLS definitions
 */
بنية ext2fs_nls_table {
	صحيح version;
	ثابت بنية ext2fs_nls_ops *ops;
};

بنية ext2fs_nls_ops {
	صحيح (*casefold)(ثابت بنية ext2fs_nls_table *charset,
			ثابت unsigned حرف *str, size_t len,
			unsigned حرف *dest, size_t dlen);
	صحيح (*validate)(ثابت بنية ext2fs_nls_table *table,
			حرف *s, size_t len, حرف **pos);
	صحيح (*casefold_cmp)(ثابت بنية ext2fs_nls_table *table,
			    ثابت unsigned حرف *str1, size_t len1,
			    ثابت unsigned حرف *str2, size_t len2);
};

/* Function prototypes */

خارجي صحيح ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    صحيح			ref_offset,
				    فراغ		*priv_data);

خارجي errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
خارجي صحيح ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  فراغ *priv_data);

/* Generic numeric progress meter */

بنية ext2fs_numeric_progress_struct {
	__u64		max;
	صحيح		log_max;
	صحيح		skip_progress;
};

/*
 * progress callback functions
 */
بنية ext2fs_progress_ops {
	فراغ (*init)(ext2_filsys fs,
		     بنية ext2fs_numeric_progress_struct * progress,
		     ثابت حرف *label, __u64 max);
	فراغ (*update)(ext2_filsys fs,
		       بنية ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	فراغ (*close)(ext2_filsys fs,
		      بنية ext2fs_numeric_progress_struct * progress,
		      ثابت حرف *message);
};

خارجي بنية ext2fs_progress_ops ext2fs_numeric_progress_ops;

خارجي فراغ ext2fs_numeric_progress_init(ext2_filsys fs,
					 بنية ext2fs_numeric_progress_struct * progress,
					 ثابت حرف *label, __u64 max);
خارجي فراغ ext2fs_numeric_progress_update(ext2_filsys fs,
					   بنية ext2fs_numeric_progress_struct * progress,
					   __u64 val);
خارجي فراغ ext2fs_numeric_progress_close(ext2_filsys fs,
					  بنية ext2fs_numeric_progress_struct * progress,
					  ثابت حرف *message);

/*
 * 64-bit bitmap support
 */

خارجي errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   صحيح type, __u64 start, __u64 end,
					   __u64 real_end,
					   ثابت حرف * description,
					   ext2fs_generic_bitmap *bmap);

خارجي فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

خارجي errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

خارجي errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
خارجي errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned صحيح ژمارە,
					       فراغ *in);
خارجي errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned صحيح ژمارە,
					       فراغ *out);
خارجي فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ثابت حرف *func);

خارجي صحيح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len);

خارجي صحيح ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    بنية ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
تعريفنوع فراغ (*ext2_exit_fn)(فراغ *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, فراغ *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, فراغ *data);

#define ARRAY_SIZE(array)			\
        (حجم(array) / حجم(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((فراغ)حجم(حرف[1 - 2*!!(cond)]))
