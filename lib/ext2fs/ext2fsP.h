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

#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم

#شامل "ext2fs.h"

#تعریف_کرو EXT2FS_MAX_NESTED_LINKS  8

ساکن اندرونی صحیح ext2fsP_is_disk_device(mode_t منوال)
{
#اگر defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	واپس S_ISBLK(منوال) || S_ISCHR(منوال);
#ورنہ
	واپس S_ISBLK(منوال);
#اگر_ختم
}

ساکن اندرونی time_t ext2fs_P_get_وقت(ext2_filsys fs)
{
	اگر (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_وقت))
		واپس fs->now;
	واپس time(تہی);
}

/*
 * Badblocks list
 */
ساخت ext2_struct_u32_list {
	صحیح	magic;
	صحیح	عدد;
	صحیح	سائز;
	__u32	*list;
	صحیح	badblocks_flags;
};

ساخت ext2_struct_u32_iterate {
	صحیح			magic;
	ext2_u32_list		bb;
	صحیح			ptr;
};


/*
 * Directory block iterator definition
 */
ساخت ext2_struct_dblist {
	صحیح			magic;
	ext2_filsys		fs;
	غیرعلامتی طویل طویل	سائز;
	غیرعلامتی طویل طویل	گنتی;
	صحیح			sorted;
	ساخت ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
ساخت dir_context {
	ext2_ino_t		dir;
	صحیح		flags;
	حرف		*buf;
	غیرعلامتی صحیح	buflen;
	صحیح (*func)(ext2_ino_t	dir,
		    صحیح	اندراج_2,
		    ساخت ext2_dir_اندراج *dirent,
		    صحیح	انحراف,
		    صحیح	blocksize,
		    حرف	*buf,
		    خالی	*priv_ڈیٹا);
	خالی		*priv_ڈیٹا;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
ساخت ext2_inode_cache {
	خالی *				بفر;
	blk64_t				بفر_blk;
	صحیح				cache_آخری;
	غیرعلامتی صحیح			cache_سائز;
	صحیح				refcount;
	ساخت ext2_inode_cache_ent	*cache;
};

ساخت ext2_inode_cache_ent {
	ext2_ino_t		ino;
	ساخت ext2_inode	*inode;
};

/*
 * NLS definitions
 */
ساخت ext2fs_nls_جدول {
	صحیح نسخہ;
	ثابت ساخت ext2fs_nls_ops *ops;
};

ساخت ext2fs_nls_ops {
	صحیح (*casefold)(ثابت ساخت ext2fs_nls_جدول *charset,
			ثابت غیرعلامتی حرف *str, size_t len,
			غیرعلامتی حرف *dest, size_t dlen);
	صحیح (*validate)(ثابت ساخت ext2fs_nls_جدول *جدول,
			حرف *s, size_t len, حرف **pos);
	صحیح (*casefold_cmp)(ثابت ساخت ext2fs_nls_جدول *جدول,
			    ثابت غیرعلامتی حرف *str1, size_t len1,
			    ثابت غیرعلامتی حرف *str2, size_t len2);
};

/* Function prototypes */

خارجی صحیح ext2fs_process_dir_بلاک(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_بلاک,
				    صحیح			ref_انحراف,
				    خالی		*priv_ڈیٹا);

خارجی errcode_t ext2fs_inline_ڈیٹا_ea_remove(ext2_filsys fs, ext2_ino_t ino);
خارجی errcode_t ext2fs_inline_ڈیٹا_expand(ext2_filsys fs, ext2_ino_t ino);
خارجی صحیح ext2fs_inline_ڈیٹا_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  خالی *priv_ڈیٹا);

/* Generic numeric progress meter */

ساخت ext2fs_numeric_پیش_رفت_struct {
	__u64		max;
	صحیح		log_max;
	صحیح		skip_پیش_رفت;
};

/*
 * progress callback functions
 */
ساخت ext2fs_پیش_رفت_ops {
	خالی (*init)(ext2_filsys fs,
		     ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
		     ثابت حرف *label, __u64 max);
	خالی (*update)(ext2_filsys fs,
		       ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
		       __u64 val);
	خالی (*close)(ext2_filsys fs,
		      ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
		      ثابت حرف *پیغام_3);
};

خارجی ساخت ext2fs_پیش_رفت_ops ext2fs_numeric_پیش_رفت_ops;

خارجی خالی ext2fs_numeric_پیش_رفت_init(ext2_filsys fs,
					 ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
					 ثابت حرف *label, __u64 max);
خارجی خالی ext2fs_numeric_پیش_رفت_update(ext2_filsys fs,
					   ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
					   __u64 val);
خارجی خالی ext2fs_numeric_پیش_رفت_close(ext2_filsys fs,
					  ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
					  ثابت حرف *پیغام_3);

/*
 * 64-bit bitmap support
 */

خارجی errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   صحیح type, __u64 آغاز, __u64 انتہا,
					   __u64 real_انتہا,
					   ثابت حرف * description,
					   ext2fs_generic_bitmap *bmap);

خارجی خالی ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

خارجی errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

خارجی errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_انتہا,
					    __u64 new_real_انتہا);
خارجی errcode_t ext2fs_fudge_generic_bmap_انتہا(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 انتہا, __u64 *oend);
خارجی صحیح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجی صحیح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
خارجی صحیح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
خارجی errcode_t ext2fs_سیٹ_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 آغاز, غیرعلامتی صحیح عدد,
					       خالی *اندر);
خارجی errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 آغاز, غیرعلامتی صحیح عدد,
					       خالی *برآمد);
خارجی خالی ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ثابت حرف *func);

خارجی صحیح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len);

خارجی صحیح ext2fs_file_بلاک_انحراف_too_big(ext2_filsys fs,
					    ساخت ext2_inode *inode,
					    blk64_t انحراف);

/* atexit support */
قسمتعریف خالی (*ext2_exit_fn)(خالی *);
errcode_t ext2fs_جوڑو_exit_fn(ext2_exit_fn fn, خالی *ڈیٹا);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, خالی *ڈیٹا);

#تعریف_کرو ARRAY_سائز(array)			\
        (حجم(array) / حجم(array[0]))

#تعریف_کرو EXT2FS_BUILD_BUG_ON(cond) ((خالی)حجم(حرف[1 - 2*!!(cond)]))
