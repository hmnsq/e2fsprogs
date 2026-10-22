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

#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif

#अन्तर्भाव "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

स्थिर ओळीत पूर्णांक ext2fsP_is_disk_device(mode_t mode)
{
#जर defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	परत S_ISBLK(mode) || S_ISCHR(mode);
#अन्यथा
	परत S_ISBLK(mode);
#endif
}

स्थिर ओळीत समय_प्रकार ext2fsP_get_time(ext2_filsys fs)
{
	जर (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		परत fs->now;
	परत समय(रिक्त);
}

/*
 * Badblocks list
 */
संरचना ext2_struct_u32_list {
	पूर्णांक	magic;
	पूर्णांक	अंक;
	पूर्णांक	size;
	__u32	*list;
	पूर्णांक	badblocks_flags;
};

संरचना ext2_struct_u32_iterate {
	पूर्णांक			magic;
	ext2_u32_list		bb;
	पूर्णांक			ptr;
};


/*
 * Directory block iterator definition
 */
संरचना ext2_struct_dblist {
	पूर्णांक			magic;
	ext2_filsys		fs;
	अचिन्हित दीर्घ दीर्घ	size;
	अचिन्हित दीर्घ दीर्घ	मोजणी;
	पूर्णांक			sorted;
	संरचना ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
संरचना dir_context {
	ext2_ino_t		dir;
	पूर्णांक		flags;
	अक्षर		*buf;
	अचिन्हित पूर्णांक	buflen;
	पूर्णांक (*func)(ext2_ino_t	dir,
		    पूर्णांक	entry,
		    संरचना ext2_dir_entry *dirent,
		    पूर्णांक	offset,
		    पूर्णांक	blocksize,
		    अक्षर	*buf,
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
	पूर्णांक				cache_last;
	अचिन्हित पूर्णांक			cache_size;
	पूर्णांक				refcount;
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
	पूर्णांक version;
	स्थिरांक संरचना ext2fs_nls_ops *ops;
};

संरचना ext2fs_nls_ops {
	पूर्णांक (*casefold)(स्थिरांक संरचना ext2fs_nls_table *charset,
			स्थिरांक अचिन्हित अक्षर *str, माप_प्रकार len,
			अचिन्हित अक्षर *dest, माप_प्रकार dlen);
	पूर्णांक (*validate)(स्थिरांक संरचना ext2fs_nls_table *table,
			अक्षर *s, माप_प्रकार len, अक्षर **pos);
	पूर्णांक (*casefold_cmp)(स्थिरांक संरचना ext2fs_nls_table *table,
			    स्थिरांक अचिन्हित अक्षर *str1, माप_प्रकार len1,
			    स्थिरांक अचिन्हित अक्षर *str2, माप_प्रकार len2);
};

/* Function prototypes */

बाह्य पूर्णांक ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    पूर्णांक			ref_offset,
				    शून्य		*priv_data);

बाह्य errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
बाह्य पूर्णांक ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  शून्य *priv_data);

/* Generic numeric progress meter */

संरचना ext2fs_numeric_progress_struct {
	__u64		max;
	पूर्णांक		log_max;
	पूर्णांक		skip_progress;
};

/*
 * progress callback functions
 */
संरचना ext2fs_progress_ops {
	शून्य (*init)(ext2_filsys fs,
		     संरचना ext2fs_numeric_progress_struct * progress,
		     स्थिरांक अक्षर *label, __u64 max);
	शून्य (*update)(ext2_filsys fs,
		       संरचना ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	शून्य (*बंद)(ext2_filsys fs,
		      संरचना ext2fs_numeric_progress_struct * progress,
		      स्थिरांक अक्षर *message);
};

बाह्य संरचना ext2fs_progress_ops ext2fs_numeric_progress_ops;

बाह्य शून्य ext2fs_numeric_progress_init(ext2_filsys fs,
					 संरचना ext2fs_numeric_progress_struct * progress,
					 स्थिरांक अक्षर *label, __u64 max);
बाह्य शून्य ext2fs_numeric_progress_update(ext2_filsys fs,
					   संरचना ext2fs_numeric_progress_struct * progress,
					   __u64 val);
बाह्य शून्य ext2fs_numeric_progress_close(ext2_filsys fs,
					  संरचना ext2fs_numeric_progress_struct * progress,
					  स्थिरांक अक्षर *message);

/*
 * 64-bit bitmap support
 */

बाह्य errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   पूर्णांक type, __u64 start, __u64 end,
					   __u64 real_end,
					   स्थिरांक अक्षर * description,
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
बाह्य पूर्णांक ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य पूर्णांक ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
बाह्य पूर्णांक ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिन्हित पूर्णांक अंक,
					       शून्य *in);
बाह्य errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिन्हित पूर्णांक अंक,
					       शून्य *out);
बाह्य शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,स्थिरांक अक्षर *func);

बाह्य पूर्णांक ext2fs_mem_is_zero(स्थिरांक अक्षर *mem, माप_प्रकार len);

बाह्य पूर्णांक ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    संरचना ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
प्रकारव्याख्या शून्य (*ext2_exit_fn)(शून्य *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, शून्य *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, शून्य *data);

#define ARRAY_SIZE(array)			\
        (आकार(array) / आकार(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((शून्य)आकार(अक्षर[1 - 2*!!(cond)]))
