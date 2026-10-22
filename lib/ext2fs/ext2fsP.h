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

#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif

#അന്തര്ഭാവ "ext2fs.h"

#define EXT2FS_MAX_NESTED_LINKS  8

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ext2fsP_is_disk_device(mode_t mode)
{
#എങ്കിൽ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	തിരികെ S_ISBLK(mode) || S_ISCHR(mode);
#അല്ലെങ്കിൽ
	തിരികെ S_ISBLK(mode);
#endif
}

സ്ഥിതം വരിയിൽ time_t ext2fsP_get_time(ext2_filsys fs)
{
	എങ്കിൽ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		തിരികെ fs->now;
	തിരികെ time(ശൂന്യ);
}

/*
 * Badblocks list
 */
ഘടന ext2_struct_u32_list {
	പൂർണ്ണസംഖ്യ	magic;
	പൂർണ്ണസംഖ്യ	അക്കം;
	പൂർണ്ണസംഖ്യ	size;
	__u32	*list;
	പൂർണ്ണസംഖ്യ	badblocks_flags;
};

ഘടന ext2_struct_u32_iterate {
	പൂർണ്ണസംഖ്യ			magic;
	ext2_u32_list		bb;
	പൂർണ്ണസംഖ്യ			ptr;
};


/*
 * Directory block iterator definition
 */
ഘടന ext2_struct_dblist {
	പൂർണ്ണസംഖ്യ			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	എണ്ണം;
	പൂർണ്ണസംഖ്യ			sorted;
	ഘടന ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ഘടന dir_context {
	ext2_ino_t		dir;
	പൂർണ്ണസംഖ്യ		flags;
	അക്ഷരം		*buf;
	unsigned പൂർണ്ണസംഖ്യ	buflen;
	പൂർണ്ണസംഖ്യ (*func)(ext2_ino_t	dir,
		    പൂർണ്ണസംഖ്യ	entry,
		    ഘടന ext2_dir_entry *dirent,
		    പൂർണ്ണസംഖ്യ	offset,
		    പൂർണ്ണസംഖ്യ	blocksize,
		    അക്ഷരം	*buf,
		    ശൂന്യം	*priv_data);
	ശൂന്യം		*priv_data;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ഘടന ext2_inode_cache {
	ശൂന്യം *				buffer;
	blk64_t				buffer_blk;
	പൂർണ്ണസംഖ്യ				cache_last;
	unsigned പൂർണ്ണസംഖ്യ			cache_size;
	പൂർണ്ണസംഖ്യ				refcount;
	ഘടന ext2_inode_cache_ent	*cache;
};

ഘടന ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ഘടന ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ഘടന ext2fs_nls_table {
	പൂർണ്ണസംഖ്യ version;
	സ്ഥിരം ഘടന ext2fs_nls_ops *ops;
};

ഘടന ext2fs_nls_ops {
	പൂർണ്ണസംഖ്യ (*casefold)(സ്ഥിരം ഘടന ext2fs_nls_table *charset,
			സ്ഥിരം unsigned അക്ഷരം *str, size_t len,
			unsigned അക്ഷരം *dest, size_t dlen);
	പൂർണ്ണസംഖ്യ (*validate)(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			അക്ഷരം *s, size_t len, അക്ഷരം **pos);
	പൂർണ്ണസംഖ്യ (*casefold_cmp)(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			    സ്ഥിരം unsigned അക്ഷരം *str1, size_t len1,
			    സ്ഥിരം unsigned അക്ഷരം *str2, size_t len2);
};

/* Function prototypes */

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_process_dir_block(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_block,
				    പൂർണ്ണസംഖ്യ			ref_offset,
				    ശൂന്യം		*priv_data);

ബാഹ്യ errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino);
ബാഹ്യ errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_inline_data_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  ശൂന്യം *priv_data);

/* Generic numeric progress meter */

ഘടന ext2fs_numeric_progress_struct {
	__u64		max;
	പൂർണ്ണസംഖ്യ		log_max;
	പൂർണ്ണസംഖ്യ		skip_progress;
};

/*
 * progress callback functions
 */
ഘടന ext2fs_progress_ops {
	ശൂന്യം (*init)(ext2_filsys fs,
		     ഘടന ext2fs_numeric_progress_struct * progress,
		     സ്ഥിരം അക്ഷരം *label, __u64 max);
	ശൂന്യം (*update)(ext2_filsys fs,
		       ഘടന ext2fs_numeric_progress_struct * progress,
		       __u64 val);
	ശൂന്യം (*close)(ext2_filsys fs,
		      ഘടന ext2fs_numeric_progress_struct * progress,
		      സ്ഥിരം അക്ഷരം *message);
};

ബാഹ്യ ഘടന ext2fs_progress_ops ext2fs_numeric_progress_ops;

ബാഹ്യ ശൂന്യം ext2fs_numeric_progress_init(ext2_filsys fs,
					 ഘടന ext2fs_numeric_progress_struct * progress,
					 സ്ഥിരം അക്ഷരം *label, __u64 max);
ബാഹ്യ ശൂന്യം ext2fs_numeric_progress_update(ext2_filsys fs,
					   ഘടന ext2fs_numeric_progress_struct * progress,
					   __u64 val);
ബാഹ്യ ശൂന്യം ext2fs_numeric_progress_close(ext2_filsys fs,
					  ഘടന ext2fs_numeric_progress_struct * progress,
					  സ്ഥിരം അക്ഷരം *message);

/*
 * 64-bit bitmap support
 */

ബാഹ്യ errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   പൂർണ്ണസംഖ്യ type, __u64 start, __u64 end,
					   __u64 real_end,
					   സ്ഥിരം അക്ഷരം * description,
					   ext2fs_generic_bitmap *bmap);

ബാഹ്യ ശൂന്യം ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

ബാഹ്യ errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

ബാഹ്യ errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_end,
					    __u64 new_real_end);
ബാഹ്യ errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 end, __u64 *oend);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ബാഹ്യ errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					       ശൂന്യം *in);
ബാഹ്യ errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					       ശൂന്യം *out);
ബാഹ്യ ശൂന്യം ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,സ്ഥിരം അക്ഷരം *func);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mem_is_zero(സ്ഥിരം അക്ഷരം *mem, size_t len);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_file_block_offset_too_big(ext2_filsys fs,
					    ഘടന ext2_inode *inode,
					    blk64_t offset);

/* atexit support */
തരനിർവചനം ശൂന്യം (*ext2_exit_fn)(ശൂന്യം *);
errcode_t ext2fs_add_exit_fn(ext2_exit_fn fn, ശൂന്യം *data);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, ശൂന്യം *data);

#define ARRAY_SIZE(array)			\
        (വലുപ്പം(array) / വലുപ്പം(array[0]))

#define EXT2FS_BUILD_BUG_ON(cond) ((ശൂന്യം)വലുപ്പം(അക്ഷരം[1 - 2*!!(cond)]))
