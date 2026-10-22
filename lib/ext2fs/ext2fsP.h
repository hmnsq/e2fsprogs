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

#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर

#समावेश "ext2fs.h"

#घोषणा EXT2FS_MAX_NESTED_LINKS  8

अटल अंतरभूत पूर्णांक ext2fsP_is_disk_device(mode_t mode)
{
#अगर defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	वापस S_ISBLK(mode) || S_ISCHR(mode);
#अन्यथा
	वापस S_ISBLK(mode);
#पूर्ण_अगर
}

अटल अंतरभूत समय_प्रकार ext2fsP_get_time(ext2_filsys fs)
{
	अगर (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		वापस fs->now;
	वापस समय(शून्य);
}

/*
 * Badblocks list
 */
काष्ठा ext2_struct_u32_list {
	पूर्णांक	magic;
	पूर्णांक	अंक;
	पूर्णांक	आकार;
	__u32	*list;
	पूर्णांक	badblocks_flags;
};

काष्ठा ext2_struct_u32_iterate {
	पूर्णांक			magic;
	ext2_u32_list		bb;
	पूर्णांक			ptr;
};


/*
 * Directory block iterator definition
 */
काष्ठा ext2_struct_dblist {
	पूर्णांक			magic;
	ext2_filsys		fs;
	अचिन्हित दीर्घ दीर्घ	आकार;
	अचिन्हित दीर्घ दीर्घ	गिनती;
	पूर्णांक			sorted;
	काष्ठा ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
काष्ठा dir_context {
	ext2_ino_t		dir;
	पूर्णांक		flags;
	अक्षर		*buf;
	अचिन्हित पूर्णांक	buflen;
	पूर्णांक (*func)(ext2_ino_t	dir,
		    पूर्णांक	entry,
		    काष्ठा ext2_dir_entry *dirent,
		    पूर्णांक	offset,
		    पूर्णांक	blocksize,
		    अक्षर	*buf,
		    व्योम	*priv_तथ्य);
	व्योम		*priv_तथ्य;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
काष्ठा ext2_inode_cache {
	व्योम *				बफर;
	blk64_t				बफर_blk;
	पूर्णांक				cache_last;
	अचिन्हित पूर्णांक			cache_आकार;
	पूर्णांक				refcount;
	काष्ठा ext2_inode_cache_ent	*cache;
};

काष्ठा ext2_inode_cache_ent {
	ext2_ino_t		ino;
	काष्ठा ext2_inode	*inode;
};

/*
 * NLS definitions
 */
काष्ठा ext2fs_nls_table {
	पूर्णांक version;
	स्थिर काष्ठा ext2fs_nls_ops *ops;
};

काष्ठा ext2fs_nls_ops {
	पूर्णांक (*casefold)(स्थिर काष्ठा ext2fs_nls_table *charset,
			स्थिर अचिन्हित अक्षर *str, माप_प्रकार len,
			अचिन्हित अक्षर *dest, माप_प्रकार dlen);
	पूर्णांक (*validate)(स्थिर काष्ठा ext2fs_nls_table *table,
			अक्षर *s, माप_प्रकार len, अक्षर **pos);
	पूर्णांक (*casefold_cmp)(स्थिर काष्ठा ext2fs_nls_table *table,
			    स्थिर अचिन्हित अक्षर *str1, माप_प्रकार len1,
			    स्थिर अचिन्हित अक्षर *str2, माप_प्रकार len2);
};

/* Function prototypes */

बाह्य पूर्णांक ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    पूर्णांक			ref_offset,
				    व्योम		*priv_तथ्य);

बाह्य errcode_t ext2fs_inline_तथ्य_ea_remove(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_तथ्य_expand(ext2_filsys fs, ext2_ino_t ino);
बाह्य पूर्णांक ext2fs_inline_तथ्य_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  व्योम *priv_तथ्य);

/* Generic numeric progress meter */

काष्ठा ext2fs_numeric_progress_struct {
	__u64		max;
	पूर्णांक		log_max;
	पूर्णांक		skip_progress;
};

/*
 * progress callback functions
 */
काष्ठा ext2fs_progress_ops {
	व्योम (*init)(ext2_filsys fs,
		     काष्ठा ext2fs_numeric_progress_struct * progress,
		     स्थिर अक्षर *label, __u64 max);
	व्योम (*update)(ext2_filsys fs,
		       काष्ठा ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	व्योम (*बंद)(ext2_filsys fs,
		      काष्ठा ext2fs_numeric_progress_struct * progress,
		      स्थिर अक्षर *message);
};

बाह्य काष्ठा ext2fs_progress_ops ext2fs_numeric_progress_ops;

बाह्य व्योम ext2fs_numeric_progress_init(ext2_filsys fs,
					 काष्ठा ext2fs_numeric_progress_struct * progress,
					 स्थिर अक्षर *label, __u64 max);
बाह्य व्योम ext2fs_numeric_progress_update(ext2_filsys fs,
					   काष्ठा ext2fs_numeric_progress_struct * progress,
					   __u64 val);
बाह्य व्योम ext2fs_numeric_progress_close(ext2_filsys fs,
					  काष्ठा ext2fs_numeric_progress_struct * progress,
					  स्थिर अक्षर *message);

/*
 * 64-bit bitmap support
 */

बाह्य errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   पूर्णांक type, __u64 start, __u64 end,
					   __u64 real_end,
					   स्थिर अक्षर * description,
					   ext2fs_generic_bitmap *bmap);

बाह्य व्योम ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

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
					       व्योम *in);
बाह्य errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिन्हित पूर्णांक अंक,
					       व्योम *out);
बाह्य व्योम ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,स्थिर अक्षर *func);

बाह्य पूर्णांक ext2fs_mem_is_zero(स्थिर अक्षर *mem, माप_प्रकार len);

बाह्य पूर्णांक ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    काष्ठा ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
प्रकार व्योम (*ext2_exit_fn)(व्योम *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, व्योम *तथ्य);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, व्योम *तथ्य);

#घोषणा ARRAY_आकार(array)			\
        (माप(array) / माप(array[0]))

#घोषणा EXT2FS_BUILD_BUG_ON(cond) ((व्योम)माप(अक्षर[1 - 2*!!(cond)]))
