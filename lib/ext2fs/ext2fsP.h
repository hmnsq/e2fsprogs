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

#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif

#अन्तर्भाव "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

स्थिर अन्तःस्थ पूर्णाङ्क ext2fsP_is_disk_device(mode_t mode)
{
#यदि defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	फर्काऊ S_ISBLK(mode) || S_ISCHR(mode);
#अन्यथा
	फर्काऊ S_ISBLK(mode);
#endif
}

स्थिर अन्तःस्थ समय_प्रकार ext2fsP_get_time(ext2_filsys fs)
{
	यदि (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		फर्काऊ fs->now;
	फर्काऊ समय(रिक्त);
}

/*
 * Badblocks list
 */
संरचना ext2_struct_u32_list {
	पूर्णाङ्क	magic;
	पूर्णाङ्क	अङ्क;
	पूर्णाङ्क	size;
	__u32	*list;
	पूर्णाङ्क	badblocks_flags;
};

संरचना ext2_struct_u32_iterate {
	पूर्णाङ्क			magic;
	ext2_u32_list		bb;
	पूर्णाङ्क			ptr;
};


/*
 * Directory block iterator definition
 */
संरचना ext2_struct_dblist {
	पूर्णाङ्क			magic;
	ext2_filsys		fs;
	अचिन्हित दीर्घ दीर्घ	size;
	अचिन्हित दीर्घ दीर्घ	गन्ती;
	पूर्णाङ्क			sorted;
	संरचना ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
संरचना dir_context {
	ext2_ino_t		dir;
	पूर्णाङ्क		flags;
	वर्ण		*buf;
	अचिन्हित पूर्णाङ्क	buflen;
	पूर्णाङ्क (*func)(ext2_ino_t	dir,
		    पूर्णाङ्क	entry,
		    संरचना ext2_dir_entry *dirent,
		    पूर्णाङ्क	offset,
		    पूर्णाङ्क	blocksize,
		    वर्ण	*buf,
		    शून्य	*priv_data);
	शून्य		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
संरचना ext2_inode_cache {
	शून्य *				buffer;
	blk64_t				buffer_blk;
	पूर्णाङ्क				cache_last;
	अचिन्हित पूर्णाङ्क			cache_size;
	पूर्णाङ्क				refcount;
	संरचना ext2_inode_cache_ent	*cache;
};

संरचना ext2_inode_cache_ent {
	ext2_ino_t		ino;
	संरचना ext2_inode	*inode;
};

/*
 * NLS definitions
 */
संरचना ext2fs_nls_table {
	पूर्णाङ्क version;
	अचर संरचना ext2fs_nls_ops *ops;
};

संरचना ext2fs_nls_ops {
	पूर्णाङ्क (*casefold)(अचर संरचना ext2fs_nls_table *charset,
			अचर अचिन्हित वर्ण *str, माप_प्रकार len,
			अचिन्हित वर्ण *dest, माप_प्रकार dlen);
	पूर्णाङ्क (*validate)(अचर संरचना ext2fs_nls_table *table,
			वर्ण *s, माप_प्रकार len, वर्ण **pos);
	पूर्णाङ्क (*casefold_cmp)(अचर संरचना ext2fs_nls_table *table,
			    अचर अचिन्हित वर्ण *str1, माप_प्रकार len1,
			    अचर अचिन्हित वर्ण *str2, माप_प्रकार len2);
};

/* Function prototypes */

बाह्य पूर्णाङ्क ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    पूर्णाङ्क			ref_offset,
				    शून्य		*priv_data);

बाह्य errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
बाह्य पूर्णाङ्क ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  शून्य *priv_data);

/* Generic numeric progress meter */

संरचना ext2fs_numeric_progress_struct {
	__u64		max;
	पूर्णाङ्क		log_max;
	पूर्णाङ्क		skip_progress;
};

/*
 * progress callback functions
 */
संरचना ext2fs_progress_ops {
	शून्य (*init)(ext2_filsys fs,
		     संरचना ext2fs_numeric_progress_struct * progress,
		     अचर वर्ण *label, __u64 max);
	शून्य (*update)(ext2_filsys fs,
		       संरचना ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	शून्य (*बंद)(ext2_filsys fs,
		      संरचना ext2fs_numeric_progress_struct * progress,
		      अचर वर्ण *message);
};

बाह्य संरचना ext2fs_progress_ops ext2fs_numeric_progress_ops;

बाह्य शून्य ext2fs_numeric_progress_init(ext2_filsys fs,
					 संरचना ext2fs_numeric_progress_struct * progress,
					 अचर वर्ण *label, __u64 max);
बाह्य शून्य ext2fs_numeric_progress_update(ext2_filsys fs,
					   संरचना ext2fs_numeric_progress_struct * progress,
					   __u64 val);
बाह्य शून्य ext2fs_numeric_progress_close(ext2_filsys fs,
					  संरचना ext2fs_numeric_progress_struct * progress,
					  अचर वर्ण *message);

/*
 * 64-bit bitmap support
 */

बाह्य errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   पूर्णाङ्क type, __u64 start, __u64 end,
					   __u64 real_end,
					   अचर वर्ण * description,
					   ext2fs_generic_bitmap *bmap);

बाह्य शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

बाह्य errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

बाह्य errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
बाह्य errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
बाह्य पूर्णाङ्क ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
बाह्य पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					       शून्य *in);
बाह्य errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					       शून्य *out);
बाह्य शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,अचर वर्ण *func);

बाह्य पूर्णाङ्क ext2fs_mem_is_zero(अचर वर्ण *mem, माप_प्रकार len);

बाह्य पूर्णाङ्क ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    संरचना ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
प्रकारनिर्देश शून्य (*ext2_exit_fn)(शून्य *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, शून्य *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, शून्य *data);

#define ARRAY_SIZE(array)			\
        (परिमाण(array) / परिमाण(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((शून्य)परिमाण(वर्ण[1 - 2*!!(cond)]))
