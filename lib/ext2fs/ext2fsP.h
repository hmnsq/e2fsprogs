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
#د_که_پای

#ضمن "ext2fs.h"

#تعریف_کړه EXT2FS_MAX_NESTED_LINKS  8

ساكن مضمن صحيح ext2fsP_is_disk_device(mode_t ډېرپېښ)
{
#إذا defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	أرجع S_ISBLK(ډېرپېښ) || S_ISCHR(ډېرپېښ);
#وإلا
	أرجع S_ISBLK(ډېرپېښ);
#د_که_پای
}

ساكن مضمن time_t ext2fs_P_get_وخت(ext2_filsys fs)
{
	إذا (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_وخت))
		أرجع fs->now;
	أرجع time(عدم);
}

/*
 * Badblocks list
 */
بنية ext2_struct_u32_list {
	صحيح	magic;
	صحيح	عدد;
	صحيح	اندازه;
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
	بینښه اوږد اوږد	اندازه;
	بینښه اوږد اوږد	شمېر;
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
	بینښه صحيح	buflen;
	صحيح (*func)(ext2_ino_t	dir,
		    صحيح	ثبت,
		    بنية ext2_dir_ثبت *dirent,
		    صحيح	دننه_واټن,
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
	صحيح				cache_وروستی;
	بینښه صحيح			cache_اندازه;
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
	صحيح نسخه;
	ثابت بنية ext2fs_nls_ops *ops;
};

بنية ext2fs_nls_ops {
	صحيح (*casefold)(ثابت بنية ext2fs_nls_جدول *charset,
			ثابت بینښه حرف *str, size_t len,
			بینښه حرف *dest, size_t dlen);
	صحيح (*validate)(ثابت بنية ext2fs_nls_جدول *جدول,
			حرف *s, size_t len, حرف **pos);
	صحيح (*casefold_cmp)(ثابت بنية ext2fs_nls_جدول *جدول,
			    ثابت بینښه حرف *str1, size_t len1,
			    ثابت بینښه حرف *str2, size_t len2);
};

/* Function prototypes */

خارجي صحيح ext2fs_process_dir_بلاک(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_بلاک,
				    صحيح			ref_دننه_واټن,
				    فراغ		*priv_معلومات);

خارجي errcode_t ext2fs_inline_معلومات_ea_remove(ext2_filsys fs, ext2_ino_t ino);
خارجي errcode_t ext2fs_inline_معلومات_expand(ext2_filsys fs, ext2_ino_t ino);
خارجي صحيح ext2fs_inline_معلومات_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  فراغ *priv_معلومات);

/* Generic numeric progress meter */

بنية ext2fs_numeric_پرمختګ_struct {
	__u64		max;
	صحيح		log_max;
	صحيح		skip_پرمختګ;
};

/*
 * progress callback functions
 */
بنية ext2fs_پرمختګ_ops {
	فراغ (*init)(ext2_filsys fs,
		     بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
		     ثابت حرف *label, __u64 max);
	فراغ (*update)(ext2_filsys fs,
		       بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
		       __u64 val);
	فراغ (*close)(ext2_filsys fs,
		      بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
		      ثابت حرف *پیغام_3);
};

خارجي بنية ext2fs_پرمختګ_ops ext2fs_numeric_پرمختګ_ops;

خارجي فراغ ext2fs_numeric_پرمختګ_init(ext2_filsys fs,
					 بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
					 ثابت حرف *label, __u64 max);
خارجي فراغ ext2fs_numeric_پرمختګ_update(ext2_filsys fs,
					   بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
					   __u64 val);
خارجي فراغ ext2fs_numeric_پرمختګ_close(ext2_filsys fs,
					  بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
					  ثابت حرف *پیغام_3);

/*
 * 64-bit bitmap support
 */

خارجي errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   صحيح type, __u64 پیل, __u64 پای_ټکی,
					   __u64 real_پای_ټکی,
					   ثابت حرف * description,
					   ext2fs_generic_bitmap *bmap);

خارجي فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

خارجي errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

خارجي errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_پای_ټکی,
					    __u64 new_real_پای_ټکی);
خارجي errcode_t ext2fs_fudge_generic_bmap_پای_ټکی(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 پای_ټکی, __u64 *oend);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجي errcode_t ext2fs_سېټ_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 پیل, بینښه صحيح عدد,
					       فراغ *دننه);
خارجي errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 پیل, بینښه صحيح عدد,
					       فراغ *بهر);
خارجي فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ثابت حرف *func);

خارجي صحيح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len);

خارجي صحيح ext2fs_file_بلاک_دننه_واټن_too_big(ext2_filsys fs,
					    بنية ext2_inode *inode,
					    blk64_t دننه_واټن);

/* atexit support */
تعريفنوع فراغ (*ext2_exit_fn)(فراغ *);
errcode_t ext2fs_جمع_کړه_exit_fn(ext2_exit_fn fn, فراغ *معلومات);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, فراغ *معلومات);

#تعریف_کړه ARRAY_اندازه(array)			\
        (حجم(array) / حجم(array[0]))

#تعریف_کړه EXT2FS_BUILD_BUG_ON(cond) ((فراغ)حجم(حرف[1 - 2*!!(cond)]))
