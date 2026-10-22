/*
 * gen_bitmap64.c --- routines to read, write, and manipulate the new qinode and
 * block bitmaps.
 *
 * Copyright (C) 2007, 2008 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <errno.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_TIME_H
#अन्तर्भाव <sys/time.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"

/*
 * Design of 64-bit bitmaps
 *
 * In order maintain ABI compatibility with programs that don't
 * understand about 64-bit blocks/inodes,
 * ext2fs_allocate_inode_bitmap() and ext2fs_allocate_block_bitmap()
 * will create old-style bitmaps unless the application passes the
 * flag EXT2_FLAG_64BITS to ext2fs_open().  If this flag is
 * passed, then we know the application has been recompiled, so we can
 * use the new-style bitmaps.  If it is not passed, we have to return
 * an error if trying to open a filesystem which needs 64-bit bitmaps.
 *
 * The new bitmaps use a new set of structure magic numbers, so that
 * both the old-style and new-style interfaces can identify which
 * version of the data structure was used.  Both the old-style and
 * new-style interfaces will support either type of bitmap, although
 * of course 64-bit operation will only be possible when both the
 * new-style interface and the new-style bitmap are used.
 *
 * For example, the new bitmap interfaces will check the structure
 * magic numbers and so will be able to detect old-stype bitmap.  If
 * they see an old-style bitmap, they will pass it to the gen_bitmap.c
 * functions for handling.  The same will be true for the old
 * interfaces as well.
 *
 * The new-style interfaces will have several different back-end
 * implementations, so we can support different encodings that are
 * appropriate for different applications.  In general the default
 * should be whatever makes sense, and what the application/library
 * will use.  However, e2fsck may need specialized implementations for
 * its own uses.  For example, when doing parent directory pointer
 * loop detections in pass 3, the bitmap will *always* be sparse, so
 * e2fsck can request an encoding which is optimized for that.
 */

स्थिर शून्य warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			पूर्णाङ्क संहिता, __u64 arg)
{
#यदि_अपरिभाषितम् OMIT_COM_ERR
	यदि (bitmap->description)
		com_err(0, bitmap->आधारः_error_संहिता+संहिता,
			"#%llu for %s", (अचिह्नित दीर्घ दीर्घ) arg,
			bitmap->description);
	अन्यथा
		com_err(0, bitmap->आधारः_error_संहिता + संहिता, "#%llu",
			(अचिह्नित दीर्घ दीर्घ) arg);
#यदि_अन्तः
}

#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
#परिभाषय INC_STAT(map, name) map->stats.name
#अन्यथा
#परिभाषय INC_STAT(map, name) ;;
#यदि_अन्तः


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णाङ्क type, __u64 start, __u64 अन्तः,
				    __u64 real_अन्तः,
				    ध्रुव वर्ण *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	संरचना ext2_bitmap_ops	*ops;
	ext2_ino_t अङ्कः_dirs;
	errcode_t retval;

	यदि (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	विकल्प (type) {
	पक्ष EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		भङ्ग;
	पक्ष EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		भङ्ग;
	पक्ष EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_अङ्कः_dirs(fs, &अङ्कः_dirs);
		यदि (retval || अङ्कः_dirs > (fs->super->s_inodes_गणनम् / 320))
			ops = &ext2fs_blkmap64_bitarray;
		अन्यथा
			ops = &ext2fs_blkmap64_rbtree;
		भङ्ग;
	स्वभाव:
		प्रत्यागम EINVAL;
	}

	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	यदि (retval)
		प्रत्यागम retval;

#यदि_परिभाषितम् ENABLE_BMAP_STATS
	यदि (gettimeofday(&bitmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		प्रत्यागम 1;
	}
	bitmap->stats.type = type;
#यदि_अन्तः

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->अन्तः = अन्तः;
	bitmap->real_अन्तः = real_अन्तः;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_द्वयङ्काः = 0;
	विकल्प (magic) {
	पक्ष EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_INODE_MARK;
		भङ्ग;
	पक्ष EXT2_ET_MAGIC_खण्डम्_BITMAP64:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_खण्डम्_MARK;
		bitmap->cluster_द्वयङ्काः = fs->cluster_अनुपातः_द्वयङ्काः;
		भङ्ग;
	स्वभाव:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_GENERIC_MARK;
	}
	यदि (descr) {
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(descr)+1, &bitmap->description);
		यदि (retval) {
			ext2fs_free_mem(&bitmap);
			प्रत्यागम retval;
		}
		वाक्यं_प्रतिलिख(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	यदि (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		प्रत्यागम retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	प्रत्यागम 0;
}

#यदि_परिभाषितम् ENABLE_BMAP_STATS
स्थिर शून्य ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2_bmap_statistics *stats = &bitmap->stats;
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	भिन्न mark_seq_perc = 0.0, test_seq_perc = 0.0;
	भिन्न mark_पृष्ठभागः_perc = 0.0, test_पृष्ठभागः_perc = 0.0;
	संरचना timeval now;
	द्विगुण inuse;

	यदि (stats->test_गणनम्) {
		test_seq_perc = ((भिन्न)stats->test_seq /
				 stats->test_गणनम्) * 100;
		test_पृष्ठभागः_perc = ((भिन्न)stats->test_पृष्ठभागः /
				  stats->test_गणनम्) * 100;
	}

	यदि (stats->mark_गणनम्) {
		mark_seq_perc = ((भिन्न)stats->mark_seq /
				 stats->mark_गणनम्) * 100;
		mark_पृष्ठभागः_perc = ((भिन्न)stats->mark_पृष्ठभागः /
				  stats->mark_गणनम्) * 100;
	}

	यदि (gettimeofday(&now, (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		प्रत्यागम;
	}

	inuse = (द्विगुण) now.tv_sec + \
		(((द्विगुण) now.tv_usec) * 0.000001);
	inuse -= (द्विगुण) stats->created.tv_sec + \
		(((द्विगुण) stats->created.tv_usec) * 0.000001);
#यदि_अन्तः /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_अन्तः - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_गणनम्, stats->resize_गणनम्);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_गणनम्, stats->unmark_गणनम्);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_गणनम्, stats->mark_ext_गणनम्);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_गणनम्, stats->test_ext_गणनम्);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->समुच्चयः_range_गणनम्, stats->get_range_गणनम्);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_गणनम्, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_पृष्ठभागः, test_पृष्ठभागः_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_पृष्ठभागः, mark_पृष्ठभागः_perc, inuse);
#यदि_अन्तः /* ENABLE_BMAP_STATS_OPS */
}
#यदि_अन्तः

शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		प्रत्यागम;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		प्रत्यागम;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

#यदि_परिभाषितम् ENABLE_BMAP_STATS
	यदि (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#यदि_अन्तः

	bmap->bitmap_ops->free_bmap(bmap);

	यदि (bmap->description) {
		ext2fs_free_mem(&bmap->description);
		bmap->description = 0;
	}
	bmap->magic = 0;
	ext2fs_free_mem(&bmap);
}

errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap gen_src,
				   ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_64 src = (ext2fs_generic_bitmap_64) gen_src;
	वर्ण *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	यदि (!src)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(src))
		प्रत्यागम ext2fs_copy_generic_bitmap(gen_src, dest);

	यदि (!EXT2FS_IS_64_BITMAP(src))
		प्रत्यागम EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	यदि (retval)
		प्रत्यागम retval;


#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	src->stats.copy_गणनम्++;
#यदि_अन्तः
#यदि_परिभाषितम् ENABLE_BMAP_STATS
	यदि (gettimeofday(&new_bmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		प्रत्यागम 1;
	}
	new_bmap->stats.type = src->stats.type;
#यदि_अन्तः

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->start = src->start;
	new_bmap->अन्तः = src->अन्तः;
	new_bmap->real_अन्तः = src->real_अन्तः;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->आधारः_error_संहिता = src->आधारः_error_संहिता;
	new_bmap->cluster_द्वयङ्काः = src->cluster_द्वयङ्काः;

	descr = src->description;
	यदि (descr) {
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(descr)+10, &new_descr);
		यदि (retval) {
			ext2fs_free_mem(&new_bmap);
			प्रत्यागम retval;
		}
		वाक्यं_प्रतिलिख(new_descr, "copy of ");
		वाक्यं_योजय(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	यदि (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		प्रत्यागम retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	प्रत्यागम 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_अन्तः,
				     __u64 new_real_अन्तः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap))
		प्रत्यागम ext2fs_resize_generic_bitmap(gen_bmap->magic, new_अन्तः,
						    new_real_अन्तः, gen_bmap);

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, resize_गणनम्);

	प्रत्यागम bmap->bitmap_ops->resize_bmap(bmap, new_अन्तः, new_real_अन्तः);
}

errcode_t ext2fs_fudge_generic_bmap_अन्तः(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 अन्तः, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		पूर्णाङ्क retval;

		retval = ext2fs_fudge_generic_bitmap_अन्तः(gen_bitmap,
							 bitmap->magic,
							 neq, अन्तः, &tmp_oend);
		यदि (oend)
			*oend = tmp_oend;
		प्रत्यागम retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	यदि (अन्तः > bitmap->real_अन्तः)
		प्रत्यागम neq;
	यदि (oend)
		*oend = bitmap->अन्तः;
	bitmap->अन्तः = अन्तः;
	प्रत्यागम 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap))
		प्रत्यागम ext2fs_get_generic_bitmap_start(gen_bitmap);

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	प्रत्यागम bitmap->start;
}

__u64 ext2fs_get_generic_bmap_अन्तः(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap))
		प्रत्यागम ext2fs_get_generic_bitmap_अन्तः(gen_bitmap);

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	प्रत्यागम bitmap->अन्तः;
}

शून्य ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	अन्यथा
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

पूर्णाङ्क ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		प्रत्यागम 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_द्वयङ्काः;

#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	यदि (arg == bitmap->stats.चरमः_marked + 1)
		bitmap->stats.mark_seq++;
	यदि (arg < bitmap->stats.चरमः_marked)
		bitmap->stats.mark_पृष्ठभागः++;
	bitmap->stats.चरमः_marked = arg;
	bitmap->stats.mark_गणनम्++;
#यदि_अन्तः

	यदि ((arg < bitmap->start) || (arg > bitmap->अन्तः)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		प्रत्यागम 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_द्वयङ्काः;

	INC_STAT(bitmap, unmark_गणनम्);

	यदि ((arg < bitmap->start) || (arg > bitmap->अन्तः)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	यदि (!bitmap)
		प्रत्यागम 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_द्वयङ्काः;

#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_गणनम्++;
	यदि (arg == bitmap->stats.चरमः_tested + 1)
		bitmap->stats.test_seq++;
	यदि (arg < bitmap->stats.चरमः_tested)
		bitmap->stats.test_पृष्ठभागः++;
	bitmap->stats.चरमः_tested = arg;
#यदि_अन्तः

	यदि ((arg < bitmap->start) || (arg > bitmap->अन्तः)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_समुच्चयः_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((start+अङ्कः-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_समुच्चयः_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्कः, अन्तर्);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, समुच्चयः_range_गणनम्);

	प्रत्यागम bmap->bitmap_ops->समुच्चयः_bmap_range(bmap, start, अङ्कः, अन्तर्);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((start+अङ्कः-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्कः, बहिर्गमनम्);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, get_range_गणनम्);

	प्रत्यागम bmap->bitmap_ops->get_bmap_range(bmap, start, अङ्कः, बहिर्गमनम्);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	यदि (!bm1 || !bm2)
		प्रत्यागम EINVAL;
	यदि (bm1->magic != bm2->magic)
		प्रत्यागम EINVAL;

	/* Now we know both bitmaps have the same magic */
	यदि (EXT2FS_IS_32_BITMAP(bm1))
		प्रत्यागम ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	यदि (!EXT2FS_IS_64_BITMAP(bm1))
		प्रत्यागम EINVAL;

	यदि ((bm1->start != bm2->start) ||
	    (bm1->अन्तः != bm2->अन्तः))
		प्रत्यागम neq;

	प्रति (i = bm1->start; i < bm1->अन्तः; i++) {
		पूर्णाङ्क ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		यदि (ret1 != ret2) {
			प्रत्यागम neq;
		}
	}

	प्रत्यागम 0;
}

शून्य ext2fs_समुच्चयः_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, अङ्कः;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_समुच्चयः_generic_bitmap_padding(gen_bmap);
		प्रत्यागम;
	}

	start = bmap->अन्तः + 1;
	अङ्कः = bmap->real_अन्तः - bmap->अन्तः;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, अङ्कः);
	/* XXX ought to warn on error */
}

पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap gen_bmap,
				    blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	अन्तः = खण्डम् + अङ्कः;

	यदि (!bmap)
		प्रत्यागम EINVAL;

	यदि (अङ्कः == 1)
		प्रत्यागम !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, खण्डम्);

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((खण्डम् & ~0xffffffffULL) ||
		    ((खण्डम्+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_test_खण्डम्_bitmap_range(
			(ext2fs_generic_bitmap) bmap, खण्डम्, अङ्कः);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, test_ext_गणनम्);

	/* convert to clusters if necessary */
	खण्डम् >>= bmap->cluster_द्वयङ्काः;
	अन्तः += (1ULL << bmap->cluster_द्वयङ्काः) - 1;
	अन्तः >>= bmap->cluster_द्वयङ्काः;
	अङ्कः = अन्तः - खण्डम्;

	यदि ((खण्डम् < bmap->start) || (खण्डम् > bmap->अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bmap->अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_TEST, खण्डम्,
				   bmap->description);
		प्रत्यागम EINVAL;
	}

	प्रत्यागम bmap->bitmap_ops->test_clear_bmap_extent(bmap, खण्डम्, अङ्कः);
}

शून्य ext2fs_mark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap gen_bmap,
				     blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	अन्तः = खण्डम् + अङ्कः;

	यदि (!bmap)
		प्रत्यागम;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((खण्डम् & ~0xffffffffULL) ||
		    ((खण्डम्+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम;
		}
		ext2fs_mark_खण्डम्_bitmap_range((ext2fs_generic_bitmap) bmap,
					       खण्डम्, अङ्कः);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

	INC_STAT(bmap, mark_ext_गणनम्);

	/* convert to clusters if necessary */
	खण्डम् >>= bmap->cluster_द्वयङ्काः;
	अन्तः += (1ULL << bmap->cluster_द्वयङ्काः) - 1;
	अन्तः >>= bmap->cluster_द्वयङ्काः;
	अङ्कः = अन्तः - खण्डम्;

	यदि ((खण्डम् < bmap->start) || (खण्डम् > bmap->अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bmap->अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_MARK, खण्डम्,
				   bmap->description);
		प्रत्यागम;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, खण्डम्, अङ्कः);
}

शून्य ext2fs_unmark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap gen_bmap,
				       blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	अन्तः = खण्डम् + अङ्कः;

	यदि (!bmap)
		प्रत्यागम;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((खण्डम् & ~0xffffffffULL) ||
		    ((खण्डम्+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम;
		}
		ext2fs_unmark_खण्डम्_bitmap_range((ext2fs_generic_bitmap) bmap,
						 खण्डम्, अङ्कः);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

	INC_STAT(bmap, unmark_ext_गणनम्);

	/* convert to clusters if necessary */
	खण्डम् >>= bmap->cluster_द्वयङ्काः;
	अन्तः += (1ULL << bmap->cluster_द्वयङ्काः) - 1;
	अन्तः >>= bmap->cluster_द्वयङ्काः;
	अङ्कः = अन्तः - खण्डम्;

	यदि ((खण्डम् < bmap->start) || (खण्डम् > bmap->अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bmap->अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_UNMARK, खण्डम्,
				   bmap->description);
		प्रत्यागम;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, खण्डम्, अङ्कः);
}

शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ध्रुव वर्ण *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#यदि_अपरिभाषितम् OMIT_COM_ERR
	यदि (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	अन्यथा
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#यदि_अन्तः
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_खण्डम्_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_खण्डम्_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, अग्र, b_अन्तः, c_अन्तः;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	यदि (fs->cluster_अनुपातः_द्वयङ्काः == ext2fs_get_bitmap_granularity(gen_bmap))
		प्रत्यागम 0;	/* Nothing to do */

	retval = ext2fs_allocate_खण्डम्_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	यदि (retval)
		प्रत्यागम retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_अन्तः = bmap->अन्तः;
	bmap->अन्तः = bmap->real_अन्तः;
	c_अन्तः = cmap->अन्तः;
	cmap->अन्तः = cmap->real_अन्तः;
	यावत् (i < bmap->real_अन्तः) {
		retval = ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(gen_bmap,
						i, bmap->real_अन्तः, &अग्र);
		यदि (retval)
			भङ्ग;
		ext2fs_mark_खण्डम्_bitmap2(gen_cmap, अग्र);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, अग्र) + 1);
	}
	bmap->अन्तः = b_अन्तः;
	cmap->अन्तः = c_अन्तः;
	ext2fs_free_खण्डम्_bitmap(gen_bmap);
	*bitmap = (ext2fs_खण्डम्_bitmap) cmap;
	प्रत्यागम 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	यदि (!bitmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		यदि (((start) & ~0xffffffffULL) ||
		    ((अन्तः) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			प्रत्यागम EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       अन्तः, &blk);
		यदि (retval == 0)
			*बहिर्गमनम् = blk;
		प्रत्यागम retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	cstart = start >> bmap64->cluster_द्वयङ्काः;
	cend = अन्तः >> bmap64->cluster_द्वयङ्काः;

	यदि (cstart < bmap64->start || cend > bmap64->अन्तः || start > अन्तः) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	यदि (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		यदि (retval)
			प्रत्यागम retval;
	प्राप्तम्:
		cout <<= bmap64->cluster_द्वयङ्काः;
		*बहिर्गमनम् = (cout >= start) ? cout : start;
		प्रत्यागम 0;
	}

	प्रति (cout = cstart; cout <= cend; cout++)
		यदि (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ प्राप्तम्;

	प्रत्यागम ENOENT;
}

errcode_t ext2fs_find_first_समुच्चयः_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	यदि (!bitmap)
		प्रत्यागम EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		यदि (((start) & ~0xffffffffULL) ||
		    ((अन्तः) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			प्रत्यागम EINVAL;
		}

		retval = ext2fs_find_first_समुच्चयः_generic_bitmap(bitmap, start,
							      अन्तः, &blk);
		यदि (retval == 0)
			*बहिर्गमनम् = blk;
		प्रत्यागम retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	cstart = start >> bmap64->cluster_द्वयङ्काः;
	cend = अन्तः >> bmap64->cluster_द्वयङ्काः;

	यदि (cstart < bmap64->start || cend > bmap64->अन्तः || start > अन्तः) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	यदि (bmap64->bitmap_ops->find_first_समुच्चयः) {
		retval = bmap64->bitmap_ops->find_first_समुच्चयः(bmap64, cstart,
							    cend, &cout);
		यदि (retval)
			प्रत्यागम retval;
	प्राप्तम्:
		cout <<= bmap64->cluster_द्वयङ्काः;
		*बहिर्गमनम् = (cout >= start) ? cout : start;
		प्रत्यागम 0;
	}

	प्रति (cout = cstart; cout <= cend; cout++)
		यदि (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ प्राप्तम्;

	प्रत्यागम ENOENT;
}

errcode_t ext2fs_गणनम्_प्रयुक्तम्_खण्डानि(ext2_filsys fs, blk64_t start,
				   blk64_t अन्तः, blk64_t *बहिर्गमनम्)
{
	blk64_t		अग्र;
	blk64_t		tot_समुच्चयः = 0;
	errcode_t	retval = 0;

	यावत् (start < अन्तः) {
		retval = ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(fs->खण्डम्_map,
							start, अन्तः, &अग्र);
		यदि (retval) {
			यदि (retval == ENOENT)
				retval = 0;
			भङ्ग;
		}
		start = अग्र;

		retval = ext2fs_find_first_zero_खण्डम्_bitmap2(fs->खण्डम्_map,
							start, अन्तः, &अग्र);
		यदि (retval == 0) {
			tot_समुच्चयः += अग्र - start;
			start  = अग्र + 1;
		} अन्यथा यदि (retval == ENOENT) {
			retval = 0;
			tot_समुच्चयः += अन्तः - start + 1;
			भङ्ग;
		} अन्यथा
			भङ्ग;
	}

	यदि (!retval)
		*बहिर्गमनम् = tot_समुच्चयः;
	प्रत्यागम retval;
}

errcode_t ext2fs_गणनम्_प्रयुक्तम्_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t अन्तः, blk64_t *बहिर्गमनम्)
{
	blk64_t		tot_समुच्चयः = 0;
	errcode_t	retval = 0;

	retval = ext2fs_गणनम्_प्रयुक्तम्_खण्डानि(fs, start, अन्तः, &tot_समुच्चयः);

	यदि (!retval)
		*बहिर्गमनम् = EXT2_FS_अङ्कः_B2_C(fs, tot_समुच्चयः);
	प्रत्यागम retval;
}
