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
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <errno.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
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
	यदि (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (अचिन्हित दीर्घ दीर्घ) arg,
			bitmap->description);
	अन्यथा
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(अचिन्हित दीर्घ दीर्घ) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#अन्यथा
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णाङ्क type, __u64 start, __u64 end,
				    __u64 real_end,
				    अचर वर्ण *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	संरचना ext2_bitmap_ops	*ops;
	ext2_ino_t अङ्क_dirs;
	errcode_t retval;

	यदि (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	विकल्प (type) {
	पक्ष EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		रोक;
	पक्ष EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		रोक;
	पक्ष EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_अङ्क_dirs(fs, &अङ्क_dirs);
		यदि (retval || अङ्क_dirs > (fs->super->s_inodes_गन्ती / 320))
			ops = &ext2fs_blkmap64_bitarray;
		अन्यथा
			ops = &ext2fs_blkmap64_rbtree;
		रोक;
	स्वभाव:
		फर्काऊ EINVAL;
	}

	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	यदि (retval)
		फर्काऊ retval;

#ifdef ENABLE_BMAP_STATS
	यदि (gettimeofday(&bitmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		लिखो_त्रुटि("gettimeofday");
		ext2fs_free_mem(&bitmap);
		फर्काऊ 1;
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
		रोक;
	पक्ष EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		रोक;
	स्वभाव:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	यदि (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+1, &bitmap->description);
		यदि (retval) {
			ext2fs_free_mem(&bitmap);
			फर्काऊ retval;
		}
		म_नकल(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	यदि (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		फर्काऊ retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	फर्काऊ 0;
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

	यदि (stats->test_गन्ती) {
		test_seq_perc = ((भिन्न)stats->test_seq /
				 stats->test_गन्ती) * 100;
		test_back_perc = ((भिन्न)stats->test_back /
				  stats->test_गन्ती) * 100;
	}

	यदि (stats->mark_गन्ती) {
		mark_seq_perc = ((भिन्न)stats->mark_seq /
				 stats->mark_गन्ती) * 100;
		mark_back_perc = ((भिन्न)stats->mark_back /
				  stats->mark_गन्ती) * 100;
	}

	यदि (gettimeofday(&now, (संरचना timezone *) रिक्त) == -1) {
		लिखो_त्रुटि("gettimeofday");
		फर्काऊ;
	}

	inuse = (द्विगुण) now.tv_sec + \
		(((द्विगुण) now.tv_usec) * 0.000001);
	inuse -= (द्विगुण) stats->created.tv_sec + \
		(((द्विगुण) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	ख_लिखो(मानक_त्रुटि, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	ख_लिखो(मानक_त्रुटि, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	ख_लिखो(मानक_त्रुटि, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	ख_लिखो(मानक_त्रुटि, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_गन्ती, stats->resize_गन्ती);
	ख_लिखो(मानक_त्रुटि, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_गन्ती, stats->unmark_गन्ती);
	ख_लिखो(मानक_त्रुटि, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_गन्ती, stats->mark_ext_गन्ती);
	ख_लिखो(मानक_त्रुटि, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_गन्ती, stats->test_ext_गन्ती);
	ख_लिखो(मानक_त्रुटि, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_गन्ती, stats->get_range_गन्ती);
	ख_लिखो(मानक_त्रुटि, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_गन्ती, stats->test_seq, test_seq_perc);
	ख_लिखो(मानक_त्रुटि, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_back, test_back_perc);
	ख_लिखो(मानक_त्रुटि, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_back, mark_back_perc, inuse);
#endif /* ENABLE_BMAP_STATS_OPS */
}
#endif

शून्य ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		फर्काऊ;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		फर्काऊ;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ;

#ifdef ENABLE_BMAP_STATS
	यदि (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

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
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(src))
		फर्काऊ ext2fs_copy_generic_bitmap(gen_src, dest);

	यदि (!EXT2FS_IS_64_BITMAP(src))
		फर्काऊ EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(परिमाण(संरचना ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	यदि (retval)
		फर्काऊ retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_गन्ती++;
#endif
#ifdef ENABLE_BMAP_STATS
	यदि (gettimeofday(&new_bmap->stats.created,
			 (संरचना timezone *) रिक्त) == -1) {
		लिखो_त्रुटि("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		फर्काऊ 1;
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
	यदि (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+10, &new_descr);
		यदि (retval) {
			ext2fs_free_mem(&new_bmap);
			फर्काऊ retval;
		}
		म_नकल(new_descr, "copy of ");
		म_जोड़ो(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	यदि (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		फर्काऊ retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	फर्काऊ 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap))
		फर्काऊ ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ EINVAL;

	INC_STAT(bmap, resize_गन्ती);

	फर्काऊ bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		पूर्णाङ्क retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		यदि (oend)
			*oend = tmp_oend;
		फर्काऊ retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ EINVAL;

	यदि (end > bitmap->real_end)
		फर्काऊ neq;
	यदि (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	फर्काऊ 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap))
		फर्काऊ ext2fs_get_generic_bitmap_start(gen_bitmap);

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ EINVAL;

	फर्काऊ bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap))
		फर्काऊ ext2fs_get_generic_bitmap_end(gen_bitmap);

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ EINVAL;

	फर्काऊ bitmap->end;
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
		फर्काऊ 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			फर्काऊ 0;
		}
		फर्काऊ ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	यदि (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	यदि (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_गन्ती++;
#endif

	यदि ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		फर्काऊ 0;
	}

	फर्काऊ bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	यदि (!bitmap)
		फर्काऊ 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			फर्काऊ 0;
		}
		फर्काऊ ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_गन्ती);

	यदि ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		फर्काऊ 0;
	}

	फर्काऊ bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	यदि (!bitmap)
		फर्काऊ 0;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			फर्काऊ 0;
		}
		फर्काऊ ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_गन्ती++;
	यदि (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	यदि (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	यदि ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		फर्काऊ 0;
	}

	फर्काऊ bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((start+अङ्क-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			फर्काऊ EINVAL;
		}
		फर्काऊ ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्क, in);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ EINVAL;

	INC_STAT(bmap, set_range_गन्ती);

	फर्काऊ bmap->bitmap_ops->set_bmap_range(bmap, start, अङ्क, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिन्हित पूर्णाङ्क अङ्क,
					शून्य *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	यदि (!bmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((start+अङ्क-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			फर्काऊ EINVAL;
		}
		फर्काऊ ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अङ्क, out);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ EINVAL;

	INC_STAT(bmap, get_range_गन्ती);

	फर्काऊ bmap->bitmap_ops->get_bmap_range(bmap, start, अङ्क, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	यदि (!bm1 || !bm2)
		फर्काऊ EINVAL;
	यदि (bm1->magic != bm2->magic)
		फर्काऊ EINVAL;

	/* Now we know both bitmaps have the same magic */
	यदि (EXT2FS_IS_32_BITMAP(bm1))
		फर्काऊ ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	यदि (!EXT2FS_IS_64_BITMAP(bm1))
		फर्काऊ EINVAL;

	यदि ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		फर्काऊ neq;

	लागि (i = bm1->start; i < bm1->end; i++) {
		पूर्णाङ्क ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		यदि (ret1 != ret2) {
			फर्काऊ neq;
		}
	}

	फर्काऊ 0;
}

शून्य ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, अङ्क;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		फर्काऊ;
	}

	start = bmap->end + 1;
	अङ्क = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, अङ्क);
	/* XXX ought to warn on error */
}

पूर्णाङ्क ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, अचिन्हित पूर्णाङ्क अङ्क)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्क;

	यदि (!bmap)
		फर्काऊ EINVAL;

	यदि (अङ्क == 1)
		फर्काऊ !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((block & ~0xffffffffULL) ||
		    ((block+अङ्क-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			फर्काऊ EINVAL;
		}
		फर्काऊ ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, अङ्क);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ EINVAL;

	INC_STAT(bmap, test_ext_गन्ती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्क = end - block;

	यदि ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्क-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		फर्काऊ EINVAL;
	}

	फर्काऊ bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, अङ्क);
}

शून्य ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, अचिन्हित पूर्णाङ्क अङ्क)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्क;

	यदि (!bmap)
		फर्काऊ;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((block & ~0xffffffffULL) ||
		    ((block+अङ्क-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			फर्काऊ;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, अङ्क);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ;

	INC_STAT(bmap, mark_ext_गन्ती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्क = end - block;

	यदि ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्क-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		फर्काऊ;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, अङ्क);
}

शून्य ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, अचिन्हित पूर्णाङ्क अङ्क)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अङ्क;

	यदि (!bmap)
		फर्काऊ;

	यदि (EXT2FS_IS_32_BITMAP(bmap)) {
		यदि ((block & ~0xffffffffULL) ||
		    ((block+अङ्क-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			फर्काऊ;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, अङ्क);
	}

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		फर्काऊ;

	INC_STAT(bmap, unmark_ext_गन्ती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अङ्क = end - block;

	यदि ((block < bmap->start) || (block > bmap->end) ||
	    (block+अङ्क-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		फर्काऊ;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, अङ्क);
}

शून्य ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, अचर वर्ण *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	यदि (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	अन्यथा
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
	यदि (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		फर्काऊ 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	यदि (retval)
		फर्काऊ retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	जबसम्म (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		यदि (retval)
			रोक;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	फर्काऊ 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	यदि (!bitmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		यदि (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			फर्काऊ EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		यदि (retval == 0)
			*out = blk;
		फर्काऊ retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	यदि (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		फर्काऊ EINVAL;
	}

	यदि (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		यदि (retval)
			फर्काऊ retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		फर्काऊ 0;
	}

	लागि (cout = cstart; cout <= cend; cout++)
		यदि (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ found;

	फर्काऊ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	यदि (!bitmap)
		फर्काऊ EINVAL;

	यदि (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		यदि (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			फर्काऊ EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		यदि (retval == 0)
			*out = blk;
		फर्काऊ retval;
	}

	यदि (!EXT2FS_IS_64_BITMAP(bitmap))
		फर्काऊ EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	यदि (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		फर्काऊ EINVAL;
	}

	यदि (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		यदि (retval)
			फर्काऊ retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		फर्काऊ 0;
	}

	लागि (cout = cstart; cout <= cend; cout++)
		यदि (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			गच्छ found;

	फर्काऊ ENOENT;
}

errcode_t ext2fs_गन्ती_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	जबसम्म (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		यदि (retval) {
			यदि (retval == ENOENT)
				retval = 0;
			रोक;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		यदि (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} अन्यथा यदि (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			रोक;
		} अन्यथा
			रोक;
	}

	यदि (!retval)
		*out = tot_set;
	फर्काऊ retval;
}

errcode_t ext2fs_गन्ती_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_गन्ती_used_blocks(fs, start, end, &tot_set);

	यदि (!retval)
		*out = EXT2_FS_अङ्क_B2_C(fs, tot_set);
	फर्काऊ retval;
}
