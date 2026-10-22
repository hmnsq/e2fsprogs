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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <errno.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#अन्तर्भाव <sys/time.h>
#endif

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
			पूर्णाङ्क code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	जइ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	अण्णहा
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#अण्णहा
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णाङ्क type, __u64 start, __u64 end,
				    __u64 real_end,
				    ध्रुव वर्ण *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	संरचना ext2_bitmap_ops	*ops;
	ext2_ino_t अङ्कः_dirs;
	errcode_t retval;

	जइ (!type)
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
		जइ (retval || अङ्कः_dirs > (fs->super->s_inodes_गणनम् / 320))
			ops = &ext2fs_blkmap64_bitarray;
		अण्णहा
			ops = &ext2fs_blkmap64_rbtree;
		भङ्ग;
	स्वभाव:
		प्रत्यागम EINVAL;
	}

	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	जइ (retval)
		प्रत्यागम retval;

#ifdef ENABLE_BMAP_STATS
	जइ (gettimeofday(&bitmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		प्रत्यागम 1;
	}
	bitmap->stats.type = type;
#endif

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_bits = 0;
	विकल्प (magic) {
	पक्ष EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		भङ्ग;
	पक्ष EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		भङ्ग;
	स्वभाव:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	जइ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		जइ (retval) {
			ext2fs_free_mem(&bitmap);
			प्रत्यागम retval;
		}
		strcpy(bitmap->description, descr);
	} अण्णहा
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	जइ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		प्रत्यागम retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	प्रत्यागम 0;
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	भिन्न mark_seq_perc = 0.0, test_seq_perc = 0.0;
	भिन्न mark_back_perc = 0.0, test_back_perc = 0.0;
	संरचना timeval now;
	द्विगुण inuse;

	जइ (stats->test_गणनम्) {
		test_seq_perc = ((भिन्न)stats->test_seq /
				 stats->test_गणनम्) * 100;
		test_back_perc = ((भिन्न)stats->test_back /
				  stats->test_गणनम्) * 100;
	}

	जइ (stats->mark_गणनम्) {
		mark_seq_perc = ((भिन्न)stats->mark_seq /
				 stats->mark_गणनम्) * 100;
		mark_back_perc = ((भिन्न)stats->mark_back /
				  stats->mark_गणनम्) * 100;
	}

	जइ (gettimeofday(&now, (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		प्रत्यागम;
	}

	inuse = (द्विगुण) now.tv_sec + \
		(((द्विगुण) now.tv_usec) * 0.000001);
	inuse -= (द्विगुण) stats->created.tv_sec + \
		(((द्विगुण) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
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
		stats->set_range_गणनम्, stats->get_range_गणनम्);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_गणनम्, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_back, test_back_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_back, mark_back_perc, inuse);
#endif /* ENABLE_BMAP_STATS_OPS */
}
#endif

शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	जइ (!bmap)
		प्रत्यागम;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		प्रत्यागम;
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

#ifdef ENABLE_BMAP_STATS
	जइ (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	जइ (bmap->description) {
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

	जइ (!src)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(src))
		प्रत्यागम ext2fs_copy_generic_bitmap(gen_src, dest);

	जइ (!EXT2FS_IS_64_BITMAP(src))
		प्रत्यागम EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	जइ (retval)
		प्रत्यागम retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_गणनम्++;
#endif
#ifdef ENABLE_BMAP_STATS
	जइ (gettimeofday(&new_bmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		प्रत्यागम 1;
	}
	new_bmap->stats.type = src->stats.type;
#endif

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->start = src->start;
	new_bmap->end = src->end;
	new_bmap->real_end = src->real_end;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->base_error_code = src->base_error_code;
	new_bmap->cluster_bits = src->cluster_bits;

	descr = src->description;
	जइ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		जइ (retval) {
			ext2fs_free_mem(&new_bmap);
			प्रत्यागम retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	जइ (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		प्रत्यागम retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	प्रत्यागम 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	जइ (!bmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bmap))
		प्रत्यागम ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, resize_गणनम्);

	प्रत्यागम bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (!bitmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		पूर्णाङ्क retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		जइ (oend)
			*oend = tmp_oend;
		प्रत्यागम retval;
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	जइ (end > bitmap->real_end)
		प्रत्यागम neq;
	जइ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	प्रत्यागम 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (!bitmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bitmap))
		प्रत्यागम ext2fs_get_generic_bitmap_start(gen_bitmap);

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	प्रत्यागम bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (!bitmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bitmap))
		प्रत्यागम ext2fs_get_generic_bitmap_end(gen_bitmap);

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	प्रत्यागम bitmap->end;
}

शून्य ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	अण्णहा
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

पूर्णाङ्क ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (!bitmap)
		प्रत्यागम 0;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		जइ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	जइ (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	जइ (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_गणनम्++;
#endif

	जइ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	जइ (!bitmap)
		प्रत्यागम 0;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		जइ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_गणनम्);

	जइ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	जइ (!bitmap)
		प्रत्यागम 0;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		जइ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			प्रत्यागम 0;
		}
		प्रत्यागम ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_गणनम्++;
	जइ (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	जइ (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	जइ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned पूर्णाङ्क अङ्कः,
					शून्य *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	जइ (!bmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		जइ ((start+अङ्कः-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्कः, in);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, set_range_गणनम्);

	प्रत्यागम bmap->bitmap_ops->set_bmap_range(bmap, start, अङ्कः, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned पूर्णाङ्क अङ्कः,
					शून्य *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	जइ (!bmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		जइ ((start+अङ्कः-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्कः, out);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, get_range_गणनम्);

	प्रत्यागम bmap->bitmap_ops->get_bmap_range(bmap, start, अङ्कः, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	जइ (!bm1 || !bm2)
		प्रत्यागम EINVAL;
	जइ (bm1->magic != bm2->magic)
		प्रत्यागम EINVAL;

	/* Now we know both bitmaps have the same magic */
	जइ (EXT2FS_IS_32_BITMAP(bm1))
		प्रत्यागम ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	जइ (!EXT2FS_IS_64_BITMAP(bm1))
		प्रत्यागम EINVAL;

	जइ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		प्रत्यागम neq;

	प्रति (i = bm1->start; i < bm1->end; i++) {
		पूर्णाङ्क ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		जइ (ret1 != ret2) {
			प्रत्यागम neq;
		}
	}

	प्रत्यागम 0;
}

शून्य ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, अङ्कः;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		प्रत्यागम;
	}

	start = bmap->end + 1;
	अङ्कः = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, अङ्कः);
	/* XXX ought to warn on error */
}

पूर्णाङ्क ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्कः;

	जइ (!bmap)
		प्रत्यागम EINVAL;

	जइ (अङ्कः == 1)
		प्रत्यागम !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		जइ ((block & ~0xffffffffULL) ||
		    ((block+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम EINVAL;
		}
		प्रत्यागम ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, अङ्कः);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम EINVAL;

	INC_STAT(bmap, test_ext_गणनम्);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्कः = end - block;

	जइ ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्कः-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		प्रत्यागम EINVAL;
	}

	प्रत्यागम bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, अङ्कः);
}

शून्य ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्कः;

	जइ (!bmap)
		प्रत्यागम;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		जइ ((block & ~0xffffffffULL) ||
		    ((block+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, अङ्कः);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

	INC_STAT(bmap, mark_ext_गणनम्);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्कः = end - block;

	जइ ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्कः-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		प्रत्यागम;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, अङ्कः);
}

शून्य ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्कः;

	जइ (!bmap)
		प्रत्यागम;

	जइ (EXT2FS_IS_32_BITMAP(bmap)) {
		जइ ((block & ~0xffffffffULL) ||
		    ((block+अङ्कः-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			प्रत्यागम;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, अङ्कः);
	}

	जइ (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम;

	INC_STAT(bmap, unmark_ext_गणनम्);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्कः = end - block;

	जइ ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्कः-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		प्रत्यागम;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, अङ्कः);
}

शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ध्रुव वर्ण *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	जइ (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	अण्णहा
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#endif
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_block_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, next, b_end, c_end;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	जइ (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		प्रत्यागम 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	जइ (retval)
		प्रत्यागम retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	जाव (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		जइ (retval)
			भङ्ग;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	प्रत्यागम 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	जइ (!bitmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		जइ (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			प्रत्यागम EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		जइ (retval == 0)
			*out = blk;
		प्रत्यागम retval;
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	जइ (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	जइ (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		जइ (retval)
			प्रत्यागम retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		प्रत्यागम 0;
	}

	प्रति (cout = cstart; cout <= cend; cout++)
		जइ (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ found;

	प्रत्यागम ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	जइ (!bitmap)
		प्रत्यागम EINVAL;

	जइ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		जइ (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			प्रत्यागम EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		जइ (retval == 0)
			*out = blk;
		प्रत्यागम retval;
	}

	जइ (!EXT2FS_IS_64_BITMAP(bitmap))
		प्रत्यागम EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	जइ (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	जइ (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		जइ (retval)
			प्रत्यागम retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		प्रत्यागम 0;
	}

	प्रति (cout = cstart; cout <= cend; cout++)
		जइ (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ found;

	प्रत्यागम ENOENT;
}

errcode_t ext2fs_गणनम्_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	जाव (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		जइ (retval) {
			जइ (retval == ENOENT)
				retval = 0;
			भङ्ग;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		जइ (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} अण्णहा जइ (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			भङ्ग;
		} अण्णहा
			भङ्ग;
	}

	जइ (!retval)
		*out = tot_set;
	प्रत्यागम retval;
}

errcode_t ext2fs_गणनम्_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_गणनम्_used_blocks(fs, start, end, &tot_set);

	जइ (!retval)
		*out = EXT2_FS_अङ्कः_B2_C(fs, tot_set);
	प्रत्यागम retval;
}
