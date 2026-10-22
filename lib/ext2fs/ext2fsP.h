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

#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_MAX_NESTED_LINKS  8

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fsP_is_disk_device(mode_t ਬਹੁਲਕ)
{
#ਜੇ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	ਵਾਪਸ S_ISBLK(ਬਹੁਲਕ) || S_ISCHR(ਬਹੁਲਕ);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ S_ISBLK(ਬਹੁਲਕ);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ time_t ext2fs_P_get_ਸਮਾਂ(ext2_filsys fs)
{
	ਜੇ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_ਸਮਾਂ))
		ਵਾਪਸ fs->now;
	ਵਾਪਸ time(ਨਿਰਾ);
}

/*
 * Badblocks list
 */
ਬਣਤਰ ext2_struct_u32_list {
	ਪੂਰਨ	magic;
	ਪੂਰਨ	ਅੰਕ;
	ਪੂਰਨ	ਮਾਪ;
	__u32	*list;
	ਪੂਰਨ	badblocks_flags;
};

ਬਣਤਰ ext2_struct_u32_iterate {
	ਪੂਰਨ			magic;
	ext2_u32_list		bb;
	ਪੂਰਨ			ptr;
};


/*
 * Directory block iterator definition
 */
ਬਣਤਰ ext2_struct_dblist {
	ਪੂਰਨ			magic;
	ext2_filsys		fs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਗਿਣਤੀ_2;
	ਪੂਰਨ			sorted;
	ਬਣਤਰ ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ਬਣਤਰ dir_context {
	ext2_ino_t		dir;
	ਪੂਰਨ		flags;
	ਅੱਖਰ		*buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	buflen;
	ਪੂਰਨ (*func)(ext2_ino_t	dir,
		    ਪੂਰਨ	ਇੰਦਰਾਜ,
		    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
		    ਪੂਰਨ	ਵਿੱਥ,
		    ਪੂਰਨ	blocksize,
		    ਅੱਖਰ	*buf,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	ਖਾਲੀ		*priv_ਅੰਕੜੇ;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ਬਣਤਰ ext2_inode_cache {
	ਖਾਲੀ *				ਬਫਰ;
	blk64_t				ਬਫਰ_blk;
	ਪੂਰਨ				cache_ਆਖਰੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ			cache_ਮਾਪ;
	ਪੂਰਨ				refcount;
	ਬਣਤਰ ext2_inode_cache_ent	*cache;
};

ਬਣਤਰ ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ਬਣਤਰ ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ {
	ਪੂਰਨ ਸੰਸਕਰਣ;
	ਅਚਲ ਬਣਤਰ ext2fs_nls_ops *ops;
};

ਬਣਤਰ ext2fs_nls_ops {
	ਪੂਰਨ (*casefold)(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *charset,
			ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str, size_t len,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *dest, size_t dlen);
	ਪੂਰਨ (*validate)(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			ਅੱਖਰ *s, size_t len, ਅੱਖਰ **pos);
	ਪੂਰਨ (*casefold_cmp)(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str1, size_t len1,
			    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str2, size_t len2);
};

/* Function prototypes */

ਬਾਹਰੀ ਪੂਰਨ ext2fs_process_dir_ਖੰਡ(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_ਖੰਡ,
				    ਪੂਰਨ			ref_ਵਿੱਥ,
				    ਖਾਲੀ		*priv_ਅੰਕੜੇ);

ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_ea_remove(ext2_filsys fs, ext2_ino_t ino);
ਬਾਹਰੀ errcode_t ext2fs_inline_ਅੰਕੜੇ_expand(ext2_filsys fs, ext2_ino_t ino);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_inline_ਅੰਕੜੇ_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  ਖਾਲੀ *priv_ਅੰਕੜੇ);

/* Generic numeric progress meter */

ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct {
	__u64		max;
	ਪੂਰਨ		log_max;
	ਪੂਰਨ		skip_ਤਰੱਕੀ;
};

/*
 * progress callback functions
 */
ਬਣਤਰ ext2fs_ਤਰੱਕੀ_ops {
	ਖਾਲੀ (*init)(ext2_filsys fs,
		     ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
		     ਅਚਲ ਅੱਖਰ *label, __u64 max);
	ਖਾਲੀ (*update)(ext2_filsys fs,
		       ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
		       __u64 val);
	ਖਾਲੀ (*close)(ext2_filsys fs,
		      ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
		      ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_3);
};

ਬਾਹਰੀ ਬਣਤਰ ext2fs_ਤਰੱਕੀ_ops ext2fs_numeric_ਤਰੱਕੀ_ops;

ਬਾਹਰੀ ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_init(ext2_filsys fs,
					 ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
					 ਅਚਲ ਅੱਖਰ *label, __u64 max);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_update(ext2_filsys fs,
					   ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
					   __u64 val);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_close(ext2_filsys fs,
					  ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
					  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_3);

/*
 * 64-bit bitmap support
 */

ਬਾਹਰੀ errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   ਪੂਰਨ type, __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ,
					   __u64 real_ਸਿਰਾ,
					   ਅਚਲ ਅੱਖਰ * description,
					   ext2fs_generic_bitmap *bmap);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

ਬਾਹਰੀ errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

ਬਾਹਰੀ errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_ਸਿਰਾ,
					    __u64 new_real_ਸਿਰਾ);
ਬਾਹਰੀ errcode_t ext2fs_fudge_generic_bmap_ਸਿਰਾ(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 ਸਿਰਾ, __u64 *oend);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
ਬਾਹਰੀ errcode_t ext2fs_ਸੈੱਟ_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					       ਖਾਲੀ *ਵਿੱਚ);
ਬਾਹਰੀ errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					       ਖਾਲੀ *ਬਾਹਰਲਾ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ਅਚਲ ਅੱਖਰ *func);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_mem_is_zero(ਅਚਲ ਅੱਖਰ *mem, size_t len);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_file_ਖੰਡ_ਵਿੱਥ_too_big(ext2_filsys fs,
					    ਬਣਤਰ ext2_inode *inode,
					    blk64_t ਵਿੱਥ);

/* atexit support */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ext2_exit_fn)(ਖਾਲੀ *);
errcode_t ext2fs_ਜੋੜੋ_exit_fn(ext2_exit_fn fn, ਖਾਲੀ *ਅੰਕੜੇ);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, ਖਾਲੀ *ਅੰਕੜੇ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARRAY_ਮਾਪ(array)			\
        (ਆਕਾਰ(array) / ਆਕਾਰ(array[0]))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_BUILD_BUG_ON(cond) ((ਖਾਲੀ)ਆਕਾਰ(ਅੱਖਰ[1 - 2*!!(cond)]))
