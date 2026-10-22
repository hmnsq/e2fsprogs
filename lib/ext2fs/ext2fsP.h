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

static inline цяло ext2fsP_is_disk_device(mode_t mode)
{
#ако defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	върни S_ISBLK(mode) || S_ISCHR(mode);
#иначе
	върни S_ISBLK(mode);
#endif
}

static inline time_t ext2fsP_get_time(ext2_filsys fs)
{
	ако (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		върни fs->now;
	върни time(NULL);
}

/*
 * Badblocks list
 */
struct ext2_struct_u32_list {
	цяло	magic;
	цяло	num;
	цяло	size;
	__u32	*list;
	цяло	badblocks_flags;
};

struct ext2_struct_u32_iterate {
	цяло			magic;
	ext2_u32_list		bb;
	цяло			ptr;
};


/*
 * Directory block iterator definition
 */
struct ext2_struct_dblist {
	цяло			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	count;
	цяло			sorted;
	struct ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
struct dir_context {
	ext2_ino_t		dir;
	цяло		flags;
	char		*buf;
	unsigned цяло	buflen;
	цяло (*func)(ext2_ino_t	dir,
		    цяло	entry,
		    struct ext2_dir_entry *dirent,
		    цяло	offset,
		    цяло	blocksize,
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
	цяло				cache_last;
	unsigned цяло			cache_size;
	цяло				refcount;
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
	цяло version;
	const struct ext2fs_nls_ops *ops;
};

struct ext2fs_nls_ops {
	цяло (*casefold)(const struct ext2fs_nls_table *charset,
			const unsigned char *str, size_t len,
			unsigned char *dest, size_t dlen);
	цяло (*validate)(const struct ext2fs_nls_table *table,
			char *s, size_t len, char **pos);
	цяло (*casefold_cmp)(const struct ext2fs_nls_table *table,
			    const unsigned char *str1, size_t len1,
			    const unsigned char *str2, size_t len2);
};

/* Function prototypes */

extern цяло ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    цяло			ref_offset,
				    void		*priv_data);

extern errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
extern errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
extern цяло ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  void *priv_data);

/* Generic numeric progress meter */

struct ext2fs_numeric_progress_struct {
	__u64		max;
	цяло		log_max;
	цяло		skip_progress;
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
					   цяло type, __u64 start, __u64 end,
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
extern цяло ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern цяло ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
extern цяло ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
extern errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned цяло num,
					       void *in);
extern errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned цяло num,
					       void *out);
extern void ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,const char *func);

extern цяло ext2fs_mem_is_zero(const char *mem, size_t len);

extern цяло ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    struct ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
typedef void (*ext2_exit_fn)(void *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, void *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, void *data);

#define ARRAY_SIZE(array)			\
        (sizeof(array) / sizeof(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((void)sizeof(char[1 - 2*!!(cond)]))
