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

#གལ་ཏེ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

static anta_hstha ཧྲིལ ext2fsP_is_disk_device(mode_t mode)
{
#གལ་ཏེ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ཕྱིར S_ISBLK(mode) || S_ISCHR(mode);
#དེ་མིན
	ཕྱིར S_ISBLK(mode);
#endif
}

static anta_hstha time_t ext2fsP_get_time(ext2_filsys fs)
{
	གལ་ཏེ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		ཕྱིར fs->now;
	ཕྱིར time(NULL);
}

/*
 * Badblocks list
 */
sa_nracanaa ext2_struct_u32_list {
	ཧྲིལ	magic;
	ཧྲིལ	num;
	ཧྲིལ	size;
	__u32	*list;
	ཧྲིལ	badblocks_flags;
};

sa_nracanaa ext2_struct_u32_iterate {
	ཧྲིལ			magic;
	ext2_u32_list		bb;
	ཧྲིལ			ptr;
};


/*
 * Directory block iterator definition
 */
sa_nracanaa ext2_struct_dblist {
	ཧྲིལ			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	count;
	ཧྲིལ			sorted;
	sa_nracanaa ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
sa_nracanaa dir_context {
	ext2_ino_t		dir;
	ཧྲིལ		flags;
	var_na		*buf;
	unsigned ཧྲིལ	buflen;
	ཧྲིལ (*func)(ext2_ino_t	dir,
		    ཧྲིལ	entry,
		    sa_nracanaa ext2_dir_entry *dirent,
		    ཧྲིལ	offset,
		    ཧྲིལ	blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
sa_nracanaa ext2_inode_cache {
	void *				buffer;
	blk64_t				buffer_blk;
	ཧྲིལ				cache_last;
	unsigned ཧྲིལ			cache_size;
	ཧྲིལ				refcount;
	sa_nracanaa ext2_inode_cache_ent	*cache;
};

sa_nracanaa ext2_inode_cache_ent {
	ext2_ino_t		ino;
	sa_nracanaa ext2_inode	*inode;
};

/*
 * NLS definitions
 */
sa_nracanaa ext2fs_nls_table {
	ཧྲིལ version;
	const sa_nracanaa ext2fs_nls_ops *ops;
};

sa_nracanaa ext2fs_nls_ops {
	ཧྲིལ (*casefold)(const sa_nracanaa ext2fs_nls_table *charset,
			const unsigned var_na *str, size_t len,
			unsigned var_na *dest, size_t dlen);
	ཧྲིལ (*validate)(const sa_nracanaa ext2fs_nls_table *table,
			var_na *s, size_t len, var_na **pos);
	ཧྲིལ (*casefold_cmp)(const sa_nracanaa ext2fs_nls_table *table,
			    const unsigned var_na *str1, size_t len1,
			    const unsigned var_na *str2, size_t len2);
};

/* Function prototypes */

extern ཧྲིལ ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    ཧྲིལ			ref_offset,
				    void		*priv_data);

extern errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
extern errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
extern ཧྲིལ ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  void *priv_data);

/* Generic numeric progress meter */

sa_nracanaa ext2fs_numeric_progress_struct {
	__u64		max;
	ཧྲིལ		log_max;
	ཧྲིལ		skip_progress;
};

/*
 * progress callback functions
 */
sa_nracanaa ext2fs_progress_ops {
	void (*init)(ext2_filsys fs,
		     sa_nracanaa ext2fs_numeric_progress_struct * progress,
		     const var_na *label, __u64 max);
	void (*update)(ext2_filsys fs,
		       sa_nracanaa ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	void (*close)(ext2_filsys fs,
		      sa_nracanaa ext2fs_numeric_progress_struct * progress,
		      const var_na *message);
};

extern sa_nracanaa ext2fs_progress_ops ext2fs_numeric_progress_ops;

extern void ext2fs_numeric_progress_init(ext2_filsys fs,
					 sa_nracanaa ext2fs_numeric_progress_struct * progress,
					 const var_na *label, __u64 max);
extern void ext2fs_numeric_progress_update(ext2_filsys fs,
					   sa_nracanaa ext2fs_numeric_progress_struct * progress,
					   __u64 val);
extern void ext2fs_numeric_progress_close(ext2_filsys fs,
					  sa_nracanaa ext2fs_numeric_progress_struct * progress,
					  const var_na *message);

/*
 * 64-bit bitmap support
 */

extern errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   ཧྲིལ type, __u64 start, __u64 end,
					   __u64 real_end,
					   const var_na * description,
					   ext2fs_generic_bitmap *bmap);

extern void ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

extern errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

extern errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
extern errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
extern ཧྲིལ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern ཧྲིལ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
extern ཧྲིལ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ཧྲིལ num,
					       void *in);
extern errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ཧྲིལ num,
					       void *out);
extern void ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,const var_na *func);

extern ཧྲིལ ext2fs_mem_is_zero(const var_na *mem, size_t len);

extern ཧྲིལ ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    sa_nracanaa ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
typedef void (*ext2_exit_fn)(void *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, void *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, void *data);

#define ARRAY_SIZE(array)			\
        (parimaa_na(array) / parimaa_na(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((void)parimaa_na(var_na[1 - 2*!!(cond)]))
