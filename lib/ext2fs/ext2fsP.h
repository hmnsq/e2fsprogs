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

#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif

#અન્તર્ભાવ "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

સ્થિર અંતઃરેખ પૂર્ણાંક ext2fsP_is_disk_device(mode_t mode)
{
#જો defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	પાછા S_ISBLK(mode) || S_ISCHR(mode);
#નહીંતર
	પાછા S_ISBLK(mode);
#endif
}

સ્થિર અંતઃરેખ time_t ext2fsP_get_time(ext2_filsys fs)
{
	જો (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		પાછા fs->now;
	પાછા time(ખાલી);
}

/*
 * Badblocks list
 */
સંરચના ext2_struct_u32_list {
	પૂર્ણાંક	magic;
	પૂર્ણાંક	અંક;
	પૂર્ણાંક	size;
	__u32	*list;
	પૂર્ણાંક	badblocks_flags;
};

સંરચના ext2_struct_u32_iterate {
	પૂર્ણાંક			magic;
	ext2_u32_list		bb;
	પૂર્ણાંક			ptr;
};


/*
 * Directory block iterator definition
 */
સંરચના ext2_struct_dblist {
	પૂર્ણાંક			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	ગણતરી;
	પૂર્ણાંક			sorted;
	સંરચના ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
સંરચના dir_context {
	ext2_ino_t		dir;
	પૂર્ણાંક		flags;
	અક્ષર		*buf;
	unsigned પૂર્ણાંક	buflen;
	પૂર્ણાંક (*func)(ext2_ino_t	dir,
		    પૂર્ણાંક	entry,
		    સંરચના ext2_dir_entry *dirent,
		    પૂર્ણાંક	offset,
		    પૂર્ણાંક	blocksize,
		    અક્ષર	*buf,
		    શૂન્ય	*priv_data);
	શૂન્ય		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
સંરચના ext2_inode_cache {
	શૂન્ય *				buffer;
	blk64_t				buffer_blk;
	પૂર્ણાંક				cache_last;
	unsigned પૂર્ણાંક			cache_size;
	પૂર્ણાંક				refcount;
	સંરચના ext2_inode_cache_ent	*cache;
};

સંરચના ext2_inode_cache_ent {
	ext2_ino_t		ino;
	સંરચના ext2_inode	*inode;
};

/*
 * NLS definitions
 */
સંરચના ext2fs_nls_table {
	પૂર્ણાંક version;
	અચલ સંરચના ext2fs_nls_ops *ops;
};

સંરચના ext2fs_nls_ops {
	પૂર્ણાંક (*casefold)(અચલ સંરચના ext2fs_nls_table *charset,
			અચલ unsigned અક્ષર *str, size_t len,
			unsigned અક્ષર *dest, size_t dlen);
	પૂર્ણાંક (*validate)(અચલ સંરચના ext2fs_nls_table *table,
			અક્ષર *s, size_t len, અક્ષર **pos);
	પૂર્ણાંક (*casefold_cmp)(અચલ સંરચના ext2fs_nls_table *table,
			    અચલ unsigned અક્ષર *str1, size_t len1,
			    અચલ unsigned અક્ષર *str2, size_t len2);
};

/* Function prototypes */

બાહ્ય પૂર્ણાંક ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    પૂર્ણાંક			ref_offset,
				    શૂન્ય		*priv_data);

બાહ્ય errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
બાહ્ય errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
બાહ્ય પૂર્ણાંક ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  શૂન્ય *priv_data);

/* Generic numeric progress meter */

સંરચના ext2fs_numeric_progress_struct {
	__u64		max;
	પૂર્ણાંક		log_max;
	પૂર્ણાંક		skip_progress;
};

/*
 * progress callback functions
 */
સંરચના ext2fs_progress_ops {
	શૂન્ય (*init)(ext2_filsys fs,
		     સંરચના ext2fs_numeric_progress_struct * progress,
		     અચલ અક્ષર *label, __u64 max);
	શૂન્ય (*update)(ext2_filsys fs,
		       સંરચના ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	શૂન્ય (*close)(ext2_filsys fs,
		      સંરચના ext2fs_numeric_progress_struct * progress,
		      અચલ અક્ષર *message);
};

બાહ્ય સંરચના ext2fs_progress_ops ext2fs_numeric_progress_ops;

બાહ્ય શૂન્ય ext2fs_numeric_progress_init(ext2_filsys fs,
					 સંરચના ext2fs_numeric_progress_struct * progress,
					 અચલ અક્ષર *label, __u64 max);
બાહ્ય શૂન્ય ext2fs_numeric_progress_update(ext2_filsys fs,
					   સંરચના ext2fs_numeric_progress_struct * progress,
					   __u64 val);
બાહ્ય શૂન્ય ext2fs_numeric_progress_close(ext2_filsys fs,
					  સંરચના ext2fs_numeric_progress_struct * progress,
					  અચલ અક્ષર *message);

/*
 * 64-bit bitmap support
 */

બાહ્ય errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   પૂર્ણાંક type, __u64 start, __u64 end,
					   __u64 real_end,
					   અચલ અક્ષર * description,
					   ext2fs_generic_bitmap *bmap);

બાહ્ય શૂન્ય ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

બાહ્ય errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

બાહ્ય errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
બાહ્ય errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
બાહ્ય પૂર્ણાંક ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
બાહ્ય પૂર્ણાંક ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
બાહ્ય પૂર્ણાંક ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
બાહ્ય errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned પૂર્ણાંક અંક,
					       શૂન્ય *in);
બાહ્ય errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned પૂર્ણાંક અંક,
					       શૂન્ય *out);
બાહ્ય શૂન્ય ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,અચલ અક્ષર *func);

બાહ્ય પૂર્ણાંક ext2fs_mem_is_zero(અચલ અક્ષર *mem, size_t len);

બાહ્ય પૂર્ણાંક ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    સંરચના ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
પ્રકારવ્યાખ્યા શૂન્ય (*ext2_exit_fn)(શૂન્ય *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, શૂન્ય *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, શૂન્ય *data);

#define ARRAY_SIZE(array)			\
        (કદ(array) / કદ(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((શૂન્ય)કદ(અક્ષર[1 - 2*!!(cond)]))
