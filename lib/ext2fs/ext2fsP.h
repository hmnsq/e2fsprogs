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

#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو

#ضمن "ext2fs.h"

#وصف_ڪريو EXT2FS_MAX_NESTED_LINKS  8

ساكن مضمن صحيح ext2fsP_is_disk_device(mode_t گهڻائي)
{
#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	أرجع S_ISBLK(گهڻائي) || S_ISCHR(گهڻائي);
#وإلا
	أرجع S_ISBLK(گهڻائي);
#جيڪڏهن_پورو
}

ساكن مضمن time_t ext2fs_P_get_وقت(ext2_filsys fs)
{
	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_وقت))
		أرجع fs->now;
	أرجع time(عدم);
}

/*
 * Badblocks list
 */
بنية ext2_struct_u32_list {
	صحيح	magic;
	صحيح	انگ;
	صحيح	سائيز;
	__u32	*list;
	صحيح	badblocks_flags;
};

بنية ext2_struct_u32_iterate {
	صحيح			magic;
	ext2_u32_list		bb;
	صحيح			ptr;
};


/*
 * Directory block iterator definition
 */
بنية ext2_struct_dblist {
	صحيح			magic;
	ext2_filsys		fs;
	بنانشان ڊگهو ڊگهو	سائيز;
	بنانشان ڊگهو ڊگهو	ڳڻپ;
	صحيح			sorted;
	بنية ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
بنية dir_context {
	ext2_ino_t		dir;
	صحيح		flags;
	حرف		*buf;
	بنانشان صحيح	buflen;
	صحيح (*func)(ext2_ino_t	dir,
		    صحيح	داخلا,
		    بنية ext2_dir_داخلا *dirent,
		    صحيح	وٿي,
		    صحيح	blocksize,
		    حرف	*buf,
		    فراغ	*priv_معلومات);
	فراغ		*priv_معلومات;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
بنية ext2_inode_cache {
	فراغ *				بفر;
	blk64_t				بفر_blk;
	صحيح				cache_آخري;
	بنانشان صحيح			cache_سائيز;
	صحيح				refcount;
	بنية ext2_inode_cache_ent	*cache;
};

بنية ext2_inode_cache_ent {
	ext2_ino_t		ino;
	بنية ext2_inode	*inode;
};

/*
 * NLS definitions
 */
بنية ext2fs_nls_جدول {
	صحيح نسخو;
	ثابت بنية ext2fs_nls_ops *ops;
};

بنية ext2fs_nls_ops {
	صحيح (*casefold)(ثابت بنية ext2fs_nls_جدول *charset,
			ثابت بنانشان حرف *str, size_t len,
			بنانشان حرف *dest, size_t dlen);
	صحيح (*validate)(ثابت بنية ext2fs_nls_جدول *جدول,
			حرف *s, size_t len, حرف **pos);
	صحيح (*casefold_cmp)(ثابت بنية ext2fs_nls_جدول *جدول,
			    ثابت بنانشان حرف *str1, size_t len1,
			    ثابت بنانشان حرف *str2, size_t len2);
};

/* Function prototypes */

خارجي صحيح ext2fs_process_dir_بلاڪ(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_بلاڪ,
				    صحيح			ref_وٿي,
				    فراغ		*priv_معلومات);

خارجي errcode_t ext2fs_inline_معلومات_ea_remove(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_معلومات_expand(ext2_filsys fs, ext2_ino_t ino);
خارجي صحيح ext2fs_inline_معلومات_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  فراغ *priv_معلومات);

/* Generic numeric progress meter */

بنية ext2fs_numeric_ترقي_struct {
	__u64		max;
	صحيح		log_max;
	صحيح		skip_ترقي;
};

/*
 * progress callback functions
 */
بنية ext2fs_ترقي_ops {
	فراغ (*init)(ext2_filsys fs,
		     بنية ext2fs_numeric_ترقي_struct * ترقي,
		     ثابت حرف *label, __u64 max);
	فراغ (*update)(ext2_filsys fs,
		       بنية ext2fs_numeric_ترقي_struct * ترقي,
		       __u64 val);
	فراغ (*close)(ext2_filsys fs,
		      بنية ext2fs_numeric_ترقي_struct * ترقي,
		      ثابت حرف *پيغام);
};

خارجي بنية ext2fs_ترقي_ops ext2fs_numeric_ترقي_ops;

خارجي فراغ ext2fs_numeric_ترقي_init(ext2_filsys fs,
					 بنية ext2fs_numeric_ترقي_struct * ترقي,
					 ثابت حرف *label, __u64 max);
خارجي فراغ ext2fs_numeric_ترقي_update(ext2_filsys fs,
					   بنية ext2fs_numeric_ترقي_struct * ترقي,
					   __u64 val);
خارجي فراغ ext2fs_numeric_ترقي_close(ext2_filsys fs,
					  بنية ext2fs_numeric_ترقي_struct * ترقي,
					  ثابت حرف *پيغام);

/*
 * 64-bit bitmap support
 */

خارجي errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   صحيح type, __u64 شروعات, __u64 پڇاڙي,
					   __u64 real_پڇاڙي,
					   ثابت حرف * description,
					   ext2fs_generic_bitmap *bmap);

خارجي فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

خارجي errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

خارجي errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_پڇاڙي,
					    __u64 new_real_پڇاڙي);
خارجي errcode_t ext2fs_fudge_generic_bmap_پڇاڙي(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 پڇاڙي, __u64 *oend);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي errcode_t ext2fs_سيٽ_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 شروعات, بنانشان صحيح انگ,
					       فراغ *اندريون);
خارجي errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 شروعات, بنانشان صحيح انگ,
					       فراغ *ٻاهريون);
خارجي فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ثابت حرف *func);

خارجي صحيح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len);

خارجي صحيح ext2fs_file_بلاڪ_وٿي_too_big(ext2_filsys fs,
					    بنية ext2_inode *inode,
					    blk64_t وٿي);

/* atexit support */
تعريفنوع فراغ (*ext2_exit_fn)(فراغ *);
errcode_t ext2fs_جوڙيو_exit_fn(ext2_exit_fn fn, فراغ *معلومات);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, فراغ *معلومات);

#وصف_ڪريو ARRAY_سائيز(array)			\
        (حجم(array) / حجم(array[0]))

#وصف_ڪريو EXT2FS_BUILD_BUG_ON(cond) ((فراغ)حجم(حرف[1 - 2*!!(cond)]))
