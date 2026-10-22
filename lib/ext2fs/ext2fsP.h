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

#ако HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

static inline цео ext2fsP_is_disk_device(mode_t mode)
{
#ако defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	врати S_ISBLK(mode) || S_ISCHR(mode);
#инако
	врати S_ISBLK(mode);
#endif
}

static inline time_t ext2fsP_get_time(ext2_filsys fs)
{
	ако (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		врати fs->now;
	врати time(NULL);
}

/*
 * Badblocks list
 */
struct ext2_struct_u32_list {
	цео	magic;
	цео	num;
	цео	size;
	__u32	*list;
	цео	badblocks_flags;
};

struct ext2_struct_u32_iterate {
	цео			magic;
	ext2_u32_list		bb;
	цео			ptr;
};


/*
 * Directory block iterator definition
 */
struct ext2_struct_dblist {
	цео			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	count;
	цео			sorted;
	struct ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
struct dir_context {
	ext2_ino_t		dir;
	цео		flags;
	char		*buf;
	unsigned цео	buflen;
	цео (*func)(ext2_ino_t	dir,
		    цео	entry,
		    struct ext2_dir_entry *dirent,
		    цео	offset,
		    цео	blocksize,
		    char	*buf,
		    void	*priv_data);
	void		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
struct ext2_inode_cache {
	void *				buffer;
	blk64_t				buffer_blk;
	цео				cache_last;
	unsigned цео			cache_size;
	цео				refcount;
	struct ext2_inode_cache_ent	*cache;
};

struct ext2_inode_cache_ent {
	ext2_ino_t		ino;
	struct ext2_inode	*inode;
};

/*
 * NLS definitions
 */
struct ext2fs_nls_table {
	цео version;
	const struct ext2fs_nls_ops *ops;
};

struct ext2fs_nls_ops {
	цео (*casefold)(const struct ext2fs_nls_table *charset,
			const unsigned char *str, size_t len,
			unsigned char *dest, size_t dlen);
	цео (*validate)(const struct ext2fs_nls_table *table,
			char *s, size_t len, char **pos);
	цео (*casefold_cmp)(const struct ext2fs_nls_table *table,
			    const unsigned char *str1, size_t len1,
			    const unsigned char *str2, size_t len2);
};

/* Function prototypes */

extern цео ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    цео			ref_offset,
				    void		*priv_data);

extern errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
extern errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
extern цео ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  void *priv_data);

/* Generic numeric progress meter */

struct ext2fs_numeric_progress_struct {
	__u64		max;
	цео		log_max;
	цео		skip_progress;
};

/*
 * progress callback functions
 */
struct ext2fs_progress_ops {
	void (*init)(ext2_filsys fs,
		     struct ext2fs_numeric_progress_struct * progress,
		     const char *label, __u64 max);
	void (*update)(ext2_filsys fs,
		       struct ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	void (*close)(ext2_filsys fs,
		      struct ext2fs_numeric_progress_struct * progress,
		      const char *message);
};

extern struct ext2fs_progress_ops ext2fs_numeric_progress_ops;

extern void ext2fs_numeric_progress_init(ext2_filsys fs,
					 struct ext2fs_numeric_progress_struct * progress,
					 const char *label, __u64 max);
extern void ext2fs_numeric_progress_update(ext2_filsys fs,
					   struct ext2fs_numeric_progress_struct * progress,
					   __u64 val);
extern void ext2fs_numeric_progress_close(ext2_filsys fs,
					  struct ext2fs_numeric_progress_struct * progress,
					  const char *message);

/*
 * 64-bit bitmap support
 */

extern errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   цео type, __u64 start, __u64 end,
					   __u64 real_end,
					   const char * description,
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
extern цео ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern цео ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
extern цео ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned цео num,
					       void *in);
extern errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned цео num,
					       void *out);
extern void ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,const char *func);

extern цео ext2fs_mem_is_zero(const char *mem, size_t len);

extern цео ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    struct ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
typedef void (*ext2_exit_fn)(void *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, void *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, void *data);

#define ARRAY_SIZE(array)			\
        (sizeof(array) / sizeof(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((void)sizeof(char[1 - 2*!!(cond)]))
