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

#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2fs.h"

#నిర్వచించు EXT2FS_MAX_NESTED_LINKS  8

స్థిర అంతఃవరుస పూర్ణాంకం ext2fsP_is_disk_device(mode_t బహుళకం)
{
#ఒకవేళ defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	తిరిగి S_ISBLK(బహుళకం) || S_ISCHR(బహుళకం);
#లేకపోతే
	తిరిగి S_ISBLK(బహుళకం);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస time_t ext2fs_P_get_సమయం(ext2_filsys fs)
{
	ఒకవేళ (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_సమయం))
		తిరిగి fs->now;
	తిరిగి time(ఖాళీ);
}

/*
 * Badblocks list
 */
నిర్మాణం ext2_struct_u32_list {
	పూర్ణాంకం	magic;
	పూర్ణాంకం	అంకె;
	పూర్ణాంకం	కొలత;
	__u32	*list;
	పూర్ణాంకం	badblocks_flags;
};

నిర్మాణం ext2_struct_u32_iterate {
	పూర్ణాంకం			magic;
	ext2_u32_list		bb;
	పూర్ణాంకం			ptr;
};


/*
 * Directory block iterator definition
 */
నిర్మాణం ext2_struct_dblist {
	పూర్ణాంకం			magic;
	ext2_filsys		fs;
	గుర్తులేని దీర్ఘ దీర్ఘ	కొలత;
	గుర్తులేని దీర్ఘ దీర్ఘ	లెక్క;
	పూర్ణాంకం			sorted;
	నిర్మాణం ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
నిర్మాణం dir_context {
	ext2_ino_t		dir;
	పూర్ణాంకం		flags;
	అక్షరం		*buf;
	గుర్తులేని పూర్ణాంకం	buflen;
	పూర్ణాంకం (*func)(ext2_ino_t	dir,
		    పూర్ణాంకం	నమోదు_2,
		    నిర్మాణం ext2_dir_నమోదు *dirent,
		    పూర్ణాంకం	స్థానభ్రంశం,
		    పూర్ణాంకం	blocksize,
		    అక్షరం	*buf,
		    శూన్య	*priv_దత్తాంశం);
	శూన్య		*priv_దత్తాంశం;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
నిర్మాణం ext2_inode_cache {
	శూన్య *				బఫరు;
	blk64_t				బఫరు_blk;
	పూర్ణాంకం				cache_చివరి;
	గుర్తులేని పూర్ణాంకం			cache_కొలత;
	పూర్ణాంకం				refcount;
	నిర్మాణం ext2_inode_cache_ent	*cache;
};

నిర్మాణం ext2_inode_cache_ent {
	ext2_ino_t		ino;
	నిర్మాణం ext2_inode	*inode;
};

/*
 * NLS definitions
 */
నిర్మాణం ext2fs_nls_పట్టిక {
	పూర్ణాంకం సంస్కరణ;
	స్థిరరాశి నిర్మాణం ext2fs_nls_ops *ops;
};

నిర్మాణం ext2fs_nls_ops {
	పూర్ణాంకం (*casefold)(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *charset,
			స్థిరరాశి గుర్తులేని అక్షరం *str, size_t len,
			గుర్తులేని అక్షరం *dest, size_t dlen);
	పూర్ణాంకం (*validate)(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			అక్షరం *s, size_t len, అక్షరం **pos);
	పూర్ణాంకం (*casefold_cmp)(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			    స్థిరరాశి గుర్తులేని అక్షరం *str1, size_t len1,
			    స్థిరరాశి గుర్తులేని అక్షరం *str2, size_t len2);
};

/* Function prototypes */

బాహ్య పూర్ణాంకం ext2fs_process_dir_ఖండం(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_ఖండం,
				    పూర్ణాంకం			ref_స్థానభ్రంశం,
				    శూన్య		*priv_దత్తాంశం);

బాహ్య errcode_t ext2fs_inline_దత్తాంశం_ea_remove(ext2_filsys fs, ext2_ino_t ino);
బాహ్య errcode_t ext2fs_inline_దత్తాంశం_expand(ext2_filsys fs, ext2_ino_t ino);
బాహ్య పూర్ణాంకం ext2fs_inline_దత్తాంశం_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  శూన్య *priv_దత్తాంశం);

/* Generic numeric progress meter */

నిర్మాణం ext2fs_numeric_పురోగతి_struct {
	__u64		max;
	పూర్ణాంకం		log_max;
	పూర్ణాంకం		skip_పురోగతి;
};

/*
 * progress callback functions
 */
నిర్మాణం ext2fs_పురోగతి_ops {
	శూన్య (*init)(ext2_filsys fs,
		     నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
		     స్థిరరాశి అక్షరం *label, __u64 max);
	శూన్య (*update)(ext2_filsys fs,
		       నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
		       __u64 val);
	శూన్య (*close)(ext2_filsys fs,
		      నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
		      స్థిరరాశి అక్షరం *సందేశం_3);
};

బాహ్య నిర్మాణం ext2fs_పురోగతి_ops ext2fs_numeric_పురోగతి_ops;

బాహ్య శూన్య ext2fs_numeric_పురోగతి_init(ext2_filsys fs,
					 నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
					 స్థిరరాశి అక్షరం *label, __u64 max);
బాహ్య శూన్య ext2fs_numeric_పురోగతి_update(ext2_filsys fs,
					   నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
					   __u64 val);
బాహ్య శూన్య ext2fs_numeric_పురోగతి_close(ext2_filsys fs,
					  నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
					  స్థిరరాశి అక్షరం *సందేశం_3);

/*
 * 64-bit bitmap support
 */

బాహ్య errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   పూర్ణాంకం type, __u64 ప్రారంభం, __u64 చివర,
					   __u64 real_చివర,
					   స్థిరరాశి అక్షరం * description,
					   ext2fs_generic_bitmap *bmap);

బాహ్య శూన్య ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

బాహ్య errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

బాహ్య errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_చివర,
					    __u64 new_real_చివర);
బాహ్య errcode_t ext2fs_fudge_generic_bmap_చివర(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 చివర, __u64 *oend);
బాహ్య పూర్ణాంకం ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
బాహ్య పూర్ణాంకం ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
బాహ్య పూర్ణాంకం ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
బాహ్య errcode_t ext2fs_సమితి_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					       శూన్య *లోపల);
బాహ్య errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					       శూన్య *బహిర్గతం);
బాహ్య శూన్య ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,స్థిరరాశి అక్షరం *func);

బాహ్య పూర్ణాంకం ext2fs_mem_is_zero(స్థిరరాశి అక్షరం *mem, size_t len);

బాహ్య పూర్ణాంకం ext2fs_file_ఖండం_స్థానభ్రంశం_too_big(ext2_filsys fs,
					    నిర్మాణం ext2_inode *inode,
					    blk64_t స్థానభ్రంశం);

/* atexit support */
రకనిర్వచనం శూన్య (*ext2_exit_fn)(శూన్య *);
errcode_t ext2fs_కూడు_exit_fn(ext2_exit_fn fn, శూన్య *దత్తాంశం);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, శూన్య *దత్తాంశం);

#నిర్వచించు ARRAY_కొలత(array)			\
        (పరిమాణం(array) / పరిమాణం(array[0]))

#నిర్వచించు EXT2FS_BUILD_BUG_ON(cond) ((శూన్య)పరిమాణం(అక్షరం[1 - 2*!!(cond)]))
