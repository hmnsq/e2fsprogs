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

#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः

#अन्तर्भाव "ext2fs.h"

#परिभाषय EXT2FS_MAX_NESTED_LINKS  8

स्थिर अन्तःस्थ पूर्णाङ्क ext2fsP_is_disk_device(mode_t बहुलकम्)
{
#यदि defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	प्रत्यागम S_ISBLK(बहुलकम्) || S_ISCHR(बहुलकम्);
#अन्यथा
	प्रत्यागम S_ISBLK(बहुलकम्);
#यदि_अन्तः
}

स्थिर अन्तःस्थ time_t ext2fsP_get_time(ext2_filsys fs)
{
	यदि (fs->now || (fs->flags2 & EXT2_FLAG2_USE_FAKE_TIME))
		प्रत्यागम fs->now;
	प्रत्यागम time(रिक्त);
}

/*
 * Badblocks list
 */
संरचना ext2_struct_u32_list {
	पूर्णाङ्क	magic;
	पूर्णाङ्क	अङ्कः;
	पूर्णाङ्क	आकार;
	__u32	*list;
	पूर्णाङ्क	badblocks_flags;
};

संरचना ext2_struct_u32_iterate {
	पूर्णाङ्क			magic;
	ext2_u32_list		bb;
	पूर्णाङ्क			ptr;
};


/*
 * Directory block iterator definition
 */
संरचना ext2_struct_dblist {
	पूर्णाङ्क			magic;
	ext2_filsys		fs;
	अचिह्नित दीर्घ दीर्घ	आकार;
	अचिह्नित दीर्घ दीर्घ	गणनम्;
	पूर्णाङ्क			sorted;
	संरचना ext2_db_entry2 *	list;
};

/*
 * For directory iterators
 */
संरचना dir_context {
	ext2_ino_t		dir;
	पूर्णाङ्क		flags;
	वर्ण		*buf;
	अचिह्नित पूर्णाङ्क	buflen;
	पूर्णाङ्क (*func)(ext2_ino_t	dir,
		    पूर्णाङ्क	प्रविष्टिः,
		    संरचना ext2_dir_प्रविष्टिः *dirent,
		    पूर्णाङ्क	विस्थापनम्,
		    पूर्णाङ्क	blocksize,
		    वर्ण	*buf,
		    शून्य	*priv_दत्त);
	शून्य		*priv_दत्त;
	errcode_t	errcode;
};

/*
 * Inode cache structure
 */
संरचना ext2_inode_cache {
	शून्य *				बफर;
	blk64_t				बफर_blk;
	पूर्णाङ्क				cache_चरमः;
	अचिह्नित पूर्णाङ्क			cache_आकार;
	पूर्णाङ्क				refcount;
	संरचना ext2_inode_cache_ent	*cache;
};

संरचना ext2_inode_cache_ent {
	ext2_ino_t		ino;
	संरचना ext2_inode	*inode;
};

/*
 * NLS definitions
 */
संरचना ext2fs_nls_सारणी {
	पूर्णाङ्क संस्करणम्;
	ध्रुव संरचना ext2fs_nls_ops *ops;
};

संरचना ext2fs_nls_ops {
	पूर्णाङ्क (*casefold)(ध्रुव संरचना ext2fs_nls_सारणी *charset,
			ध्रुव अचिह्नित वर्ण *str, size_t len,
			अचिह्नित वर्ण *dest, size_t dlen);
	पूर्णाङ्क (*validate)(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			वर्ण *s, size_t len, वर्ण **pos);
	पूर्णाङ्क (*casefold_cmp)(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			    ध्रुव अचिह्नित वर्ण *str1, size_t len1,
			    ध्रुव अचिह्नित वर्ण *str2, size_t len2);
};

/* Function prototypes */

बाह्य पूर्णाङ्क ext2fs_process_dir_खण्डम्(ext2_filsys  	fs,
				    blk64_t		*blocknr,
				    e2_blkcnt_t		blockcnt,
				    blk64_t		ref_खण्डम्,
				    पूर्णाङ्क			ref_विस्थापनम्,
				    शून्य		*priv_दत्त);

बाह्य errcode_t ext2fs_inline_दत्त_ea_remove(ext2_filsys fs, ext2_ino_t ino);
बाह्य errcode_t ext2fs_inline_दत्त_expand(ext2_filsys fs, ext2_ino_t ino);
बाह्य पूर्णाङ्क ext2fs_inline_दत्त_dir_iterate(ext2_filsys fs,
					  ext2_ino_t ino,
					  शून्य *priv_दत्त);

/* Generic numeric progress meter */

संरचना ext2fs_numeric_प्रगतिः_struct {
	__u64		max;
	पूर्णाङ्क		log_max;
	पूर्णाङ्क		skip_प्रगतिः;
};

/*
 * progress callback functions
 */
संरचना ext2fs_प्रगतिः_ops {
	शून्य (*init)(ext2_filsys fs,
		     संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
		     ध्रुव वर्ण *label, __u64 max);
	शून्य (*update)(ext2_filsys fs,
		       संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
		       __u64 val);
	शून्य (*close)(ext2_filsys fs,
		      संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
		      ध्रुव वर्ण *सन्देशः_3);
};

बाह्य संरचना ext2fs_प्रगतिः_ops ext2fs_numeric_प्रगतिः_ops;

बाह्य शून्य ext2fs_numeric_प्रगतिः_init(ext2_filsys fs,
					 संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
					 ध्रुव वर्ण *label, __u64 max);
बाह्य शून्य ext2fs_numeric_प्रगतिः_update(ext2_filsys fs,
					   संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
					   __u64 val);
बाह्य शून्य ext2fs_numeric_प्रगतिः_close(ext2_filsys fs,
					  संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
					  ध्रुव वर्ण *सन्देशः_3);

/*
 * 64-bit bitmap support
 */

बाह्य errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
					   पूर्णाङ्क type, __u64 start, __u64 अन्तः,
					   __u64 real_अन्तः,
					   ध्रुव वर्ण * description,
					   ext2fs_generic_bitmap *bmap);

बाह्य शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap);

बाह्य errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap src,
					  ext2fs_generic_bitmap *dest);

बाह्य errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap bmap,
					    __u64 new_अन्तः,
					    __u64 new_real_अन्तः);
बाह्य errcode_t ext2fs_fudge_generic_bmap_अन्तः(ext2fs_generic_bitmap bitmap,
					       errcode_t neq,
					       __u64 अन्तः, __u64 *oend);
बाह्य पूर्णाङ्क ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      __u64 arg);
बाह्य पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    __u64 arg);
बाह्य errcode_t ext2fs_समुच्चयः_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					       शून्य *अन्तर्);
बाह्य errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap bitmap,
					       __u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					       शून्य *बहिर्गमनम्);
बाह्य शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap bitmap,ध्रुव वर्ण *func);

बाह्य पूर्णाङ्क ext2fs_mem_is_zero(ध्रुव वर्ण *mem, size_t len);

बाह्य पूर्णाङ्क ext2fs_file_खण्डम्_विस्थापनम्_too_big(ext2_filsys fs,
					    संरचना ext2_inode *inode,
					    blk64_t विस्थापनम्);

/* atexit support */
प्रकारनिर्देश शून्य (*ext2_exit_fn)(शून्य *);
errcode_t ext2fs_सङ्कलय_exit_fn(ext2_exit_fn fn, शून्य *दत्त);
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn fn, शून्य *दत्त);

#परिभाषय ARRAY_आकार(array)			\
        (परिमाण(array) / परिमाण(array[0]))

#परिभाषय EXT2FS_BUILD_BUG_ON(cond) ((शून्य)परिमाण(वर्ण[1 - 2*!!(cond)]))
