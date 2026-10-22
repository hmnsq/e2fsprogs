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

#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fsP_is_disk_device(mode_t mode)
{
#ଯଦି defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ଫେରନ୍ତୁ S_ISBLK(mode) || S_ISCHR(mode);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ S_ISBLK(mode);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ time_t ext2fsP_get_time(ext2_filsys fs)
{
	ଯଦି (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		ଫେରନ୍ତୁ fs->now;
	ଫେରନ୍ତୁ time(ଖାଲି);
}

/*
 * Badblocks list
 */
ସଂରଚନା ext2_struct_u32_list {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	magic;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ଅଙ୍କ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	size;
	__u32	*list;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	badblocks_flags;
};

ସଂରଚନା ext2_struct_u32_iterate {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			magic;
	ext2_u32_list		bb;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ptr;
};


/*
 * Directory block iterator definition
 */
ସଂରଚନା ext2_struct_dblist {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	ଗଣନା_2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			sorted;
	ସଂରଚନା ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ସଂରଚନା dir_context {
	ext2_ino_t		dir;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
	ଅକ୍ଷର		*buf;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	buflen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_ino_t	dir,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା	entry,
		    ସଂରଚନା ext2_dir_entry *dirent,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize,
		    ଅକ୍ଷର	*buf,
		    ଶୂନ୍ୟ	*priv_data);
	ଶୂନ୍ୟ		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ସଂରଚନା ext2_inode_cache {
	ଶୂନ୍ୟ *				buffer;
	blk64_t				buffer_blk;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				cache_last;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା			cache_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				refcount;
	ସଂରଚନା ext2_inode_cache_ent	*cache;
};

ସଂରଚନା ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ସଂରଚନା ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ସଂରଚନା ext2fs_nls_table {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା version;
	ଅଚଳ ସଂରଚନା ext2fs_nls_ops *ops;
};

ସଂରଚନା ext2fs_nls_ops {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*casefold)(ଅଚଳ ସଂରଚନା ext2fs_nls_table *charset,
			ଅଚଳ unsigned ଅକ୍ଷର *str, size_t len,
			unsigned ଅକ୍ଷର *dest, size_t dlen);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*validate)(ଅଚଳ ସଂରଚନା ext2fs_nls_table *table,
			ଅକ୍ଷର *s, size_t len, ଅକ୍ଷର **pos);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*casefold_cmp)(ଅଚଳ ସଂରଚନା ext2fs_nls_table *table,
			    ଅଚଳ unsigned ଅକ୍ଷର *str1, size_t len1,
			    ଅଚଳ unsigned ଅକ୍ଷର *str2, size_t len2);
};

/* Function prototypes */

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ref_offset,
				    ଶୂନ୍ୟ		*priv_data);

ବାହ୍ୟ errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
ବାହ୍ୟ errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  ଶୂନ୍ୟ *priv_data);

/* Generic numeric progress meter */

ସଂରଚନା ext2fs_numeric_progress_struct {
	__u64		max;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		log_max;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		skip_progress;
};

/*
 * progress callback functions
 */
ସଂରଚନା ext2fs_progress_ops {
	ଶୂନ୍ୟ (*init)(ext2_filsys fs,
		     ସଂରଚନା ext2fs_numeric_progress_struct * progress,
		     ଅଚଳ ଅକ୍ଷର *label, __u64 max);
	ଶୂନ୍ୟ (*update)(ext2_filsys fs,
		       ସଂରଚନା ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	ଶୂନ୍ୟ (*close)(ext2_filsys fs,
		      ସଂରଚନା ext2fs_numeric_progress_struct * progress,
		      ଅଚଳ ଅକ୍ଷର *message);
};

ବାହ୍ୟ ସଂରଚନା ext2fs_progress_ops ext2fs_numeric_progress_ops;

ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_numeric_progress_init(ext2_filsys fs,
					 ସଂରଚନା ext2fs_numeric_progress_struct * progress,
					 ଅଚଳ ଅକ୍ଷର *label, __u64 max);
ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_numeric_progress_update(ext2_filsys fs,
					   ସଂରଚନା ext2fs_numeric_progress_struct * progress,
					   __u64 val);
ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_numeric_progress_close(ext2_filsys fs,
					  ସଂରଚନା ext2fs_numeric_progress_struct * progress,
					  ଅଚଳ ଅକ୍ଷର *message);

/*
 * 64-bit bitmap support
 */

ବାହ୍ୟ errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, __u64 start, __u64 end,
					   __u64 real_end,
					   ଅଚଳ ଅକ୍ଷର * description,
					   ext2fs_generic_bitmap *bmap);

ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

ବାହ୍ୟ errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

ବାହ୍ୟ errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
ବାହ୍ୟ errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ବାହ୍ୟ errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ,
					       ଶୂନ୍ୟ *in);
ବାହ୍ୟ errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ,
					       ଶୂନ୍ୟ *out);
ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ଅଚଳ ଅକ୍ଷର *func);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_mem_is_zero(ଅଚଳ ଅକ୍ଷର *mem, size_t len);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    ସଂରଚନା ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ext2_exit_fn)(ଶୂନ୍ୟ *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, ଶୂନ୍ୟ *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, ଶୂନ୍ୟ *data);

#define ARRAY_SIZE(array)			\
        (ଆକାର(array) / ଆକାର(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((ଶୂନ୍ୟ)ଆକାର(ଅକ୍ଷର[1 - 2*!!(cond)]))
