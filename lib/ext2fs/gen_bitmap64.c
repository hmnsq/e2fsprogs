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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <errno.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#অন্তর্ভাব <sys/time.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"
#অন্তর্ভাব "bmap64.h"

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

স্থির শূন্য warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			পূর্ণসংখ্যা code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	যদি (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	অন্যথা
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#অন্যথা
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    পূর্ণসংখ্যা type, __u64 start, __u64 end,
				    __u64 real_end,
				    ধ্রুবক অক্ষর *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	কাঠামো ext2_bitmap_ops	*ops;
	ext2_ino_t অঙ্ক_dirs;
	errcode_t retval;

	যদি (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	নির্বাচন (type) {
	ক্ষেত্র EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		বিরতি;
	ক্ষেত্র EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		বিরতি;
	ক্ষেত্র EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_অঙ্ক_dirs(fs, &অঙ্ক_dirs);
		যদি (retval || অঙ্ক_dirs > (fs->super->s_inodes_গুনতি / 320))
			ops = &ext2fs_blkmap64_bitarray;
		অন্যথা
			ops = &ext2fs_blkmap64_rbtree;
		বিরতি;
	পূর্বনির্ধারিত:
		ফেরত EINVAL;
	}

	retval = ext2fs_get_memzero(আকার(কাঠামো ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	যদি (retval)
		ফেরত retval;

#ifdef ENABLE_BMAP_STATS
	যদি (gettimeofday(&bitmap->stats.created,
			 (কাঠামো timezone *) ফাঁকা) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		ফেরত 1;
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
	নির্বাচন (magic) {
	ক্ষেত্র EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		বিরতি;
	ক্ষেত্র EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		বিরতি;
	পূর্বনির্ধারিত:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	যদি (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		যদি (retval) {
			ext2fs_free_mem(&bitmap);
			ফেরত retval;
		}
		strcpy(bitmap->description, descr);
	} অন্যথা
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	যদি (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ফেরত retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	ফেরত 0;
}

#ifdef ENABLE_BMAP_STATS
স্থির শূন্য ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	কাঠামো ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	দশমিক mark_seq_perc = 0.0, test_seq_perc = 0.0;
	দশমিক mark_back_perc = 0.0, test_back_perc = 0.0;
	কাঠামো timeval now;
	দ্বিগুণ inuse;

	যদি (stats->test_গুনতি) {
		test_seq_perc = ((দশমিক)stats->test_seq /
				 stats->test_গুনতি) * 100;
		test_back_perc = ((দশমিক)stats->test_back /
				  stats->test_গুনতি) * 100;
	}

	যদি (stats->mark_গুনতি) {
		mark_seq_perc = ((দশমিক)stats->mark_seq /
				 stats->mark_গুনতি) * 100;
		mark_back_perc = ((দশমিক)stats->mark_back /
				  stats->mark_গুনতি) * 100;
	}

	যদি (gettimeofday(&now, (কাঠামো timezone *) ফাঁকা) == -1) {
		perror("gettimeofday");
		ফেরত;
	}

	inuse = (দ্বিগুণ) now.tv_sec + \
		(((দ্বিগুণ) now.tv_usec) * 0.000001);
	inuse -= (দ্বিগুণ) stats->created.tv_sec + \
		(((দ্বিগুণ) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_গুনতি, stats->resize_গুনতি);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_গুনতি, stats->unmark_গুনতি);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_গুনতি, stats->mark_ext_গুনতি);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_গুনতি, stats->test_ext_গুনতি);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_গুনতি, stats->get_range_গুনতি);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_গুনতি, stats->test_seq, test_seq_perc);
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

শূন্য ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	যদি (!bmap)
		ফেরত;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		ফেরত;
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত;

#ifdef ENABLE_BMAP_STATS
	যদি (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	যদি (bmap->description) {
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
	অক্ষর *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	যদি (!src)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(src))
		ফেরত ext2fs_copy_generic_bitmap(gen_src, dest);

	যদি (!EXT2FS_IS_64_BITMAP(src))
		ফেরত EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(আকার(কাঠামো ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	যদি (retval)
		ফেরত retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_গুনতি++;
#endif
#ifdef ENABLE_BMAP_STATS
	যদি (gettimeofday(&new_bmap->stats.created,
			 (কাঠামো timezone *) ফাঁকা) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		ফেরত 1;
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
	যদি (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		যদি (retval) {
			ext2fs_free_mem(&new_bmap);
			ফেরত retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	যদি (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		ফেরত retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	ফেরত 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	যদি (!bmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bmap))
		ফেরত ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত EINVAL;

	INC_STAT(bmap, resize_গুনতি);

	ফেরত bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (!bitmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		পূর্ণসংখ্যা retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		যদি (oend)
			*oend = tmp_oend;
		ফেরত retval;
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত EINVAL;

	যদি (end > bitmap->real_end)
		ফেরত neq;
	যদি (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ফেরত 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (!bitmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bitmap))
		ফেরত ext2fs_get_generic_bitmap_start(gen_bitmap);

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত EINVAL;

	ফেরত bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (!bitmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bitmap))
		ফেরত ext2fs_get_generic_bitmap_end(gen_bitmap);

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত EINVAL;

	ফেরত bitmap->end;
}

শূন্য ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	অন্যথা
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

পূর্ণসংখ্যা ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (!bitmap)
		ফেরত 0;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			ফেরত 0;
		}
		ফেরত ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	যদি (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	যদি (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_গুনতি++;
#endif

	যদি ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		ফেরত 0;
	}

	ফেরত bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

পূর্ণসংখ্যা ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	যদি (!bitmap)
		ফেরত 0;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ফেরত 0;
		}
		ফেরত ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_গুনতি);

	যদি ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		ফেরত 0;
	}

	ফেরত bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

পূর্ণসংখ্যা ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	যদি (!bitmap)
		ফেরত 0;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			ফেরত 0;
		}
		ফেরত ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_গুনতি++;
	যদি (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	যদি (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	যদি ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		ফেরত 0;
	}

	ফেরত bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	যদি (!bmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		যদি ((start+অঙ্ক-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ফেরত EINVAL;
		}
		ফেরত ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, অঙ্ক, in);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত EINVAL;

	INC_STAT(bmap, set_range_গুনতি);

	ফেরত bmap->bitmap_ops->set_bmap_range(bmap, start, অঙ্ক, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned পূর্ণসংখ্যা অঙ্ক,
					শূন্য *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	যদি (!bmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		যদি ((start+অঙ্ক-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ফেরত EINVAL;
		}
		ফেরত ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, অঙ্ক, out);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত EINVAL;

	INC_STAT(bmap, get_range_গুনতি);

	ফেরত bmap->bitmap_ops->get_bmap_range(bmap, start, অঙ্ক, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	যদি (!bm1 || !bm2)
		ফেরত EINVAL;
	যদি (bm1->magic != bm2->magic)
		ফেরত EINVAL;

	/* Now we know both bitmaps have the same magic */
	যদি (EXT2FS_IS_32_BITMAP(bm1))
		ফেরত ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	যদি (!EXT2FS_IS_64_BITMAP(bm1))
		ফেরত EINVAL;

	যদি ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		ফেরত neq;

	জন্য (i = bm1->start; i < bm1->end; i++) {
		পূর্ণসংখ্যা ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		যদি (ret1 != ret2) {
			ফেরত neq;
		}
	}

	ফেরত 0;
}

শূন্য ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, অঙ্ক;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		ফেরত;
	}

	start = bmap->end + 1;
	অঙ্ক = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, অঙ্ক);
	/* XXX ought to warn on error */
}

পূর্ণসংখ্যা ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + অঙ্ক;

	যদি (!bmap)
		ফেরত EINVAL;

	যদি (অঙ্ক == 1)
		ফেরত !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		যদি ((block & ~0xffffffffULL) ||
		    ((block+অঙ্ক-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ফেরত EINVAL;
		}
		ফেরত ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, অঙ্ক);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত EINVAL;

	INC_STAT(bmap, test_ext_গুনতি);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	অঙ্ক = end - block;

	যদি ((block < bmap->start) || (block > bmap->end) ||
	    (block+অঙ্ক-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		ফেরত EINVAL;
	}

	ফেরত bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, অঙ্ক);
}

শূন্য ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + অঙ্ক;

	যদি (!bmap)
		ফেরত;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		যদি ((block & ~0xffffffffULL) ||
		    ((block+অঙ্ক-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ফেরত;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, অঙ্ক);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত;

	INC_STAT(bmap, mark_ext_গুনতি);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	অঙ্ক = end - block;

	যদি ((block < bmap->start) || (block > bmap->end) ||
	    (block+অঙ্ক-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		ফেরত;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, অঙ্ক);
}

শূন্য ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + অঙ্ক;

	যদি (!bmap)
		ফেরত;

	যদি (EXT2FS_IS_32_BITMAP(bmap)) {
		যদি ((block & ~0xffffffffULL) ||
		    ((block+অঙ্ক-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ফেরত;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, অঙ্ক);
	}

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		ফেরত;

	INC_STAT(bmap, unmark_ext_গুনতি);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	অঙ্ক = end - block;

	যদি ((block < bmap->start) || (block > bmap->end) ||
	    (block+অঙ্ক-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		ফেরত;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, অঙ্ক);
}

শূন্য ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ধ্রুবক অক্ষর *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	যদি (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	অন্যথা
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
	blk64_t			i, পরবর্তী, b_end, c_end;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	যদি (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		ফেরত 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	যদি (retval)
		ফেরত retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	যখন (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &পরবর্তী);
		যদি (retval)
			বিরতি;
		ext2fs_mark_block_bitmap2(gen_cmap, পরবর্তী);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, পরবর্তী) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	ফেরত 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	যদি (!bitmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		যদি (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			ফেরত EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		যদি (retval == 0)
			*out = blk;
		ফেরত retval;
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	যদি (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		ফেরত EINVAL;
	}

	যদি (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		যদি (retval)
			ফেরত retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		ফেরত 0;
	}

	জন্য (cout = cstart; cout <= cend; cout++)
		যদি (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			যাও found;

	ফেরত ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	যদি (!bitmap)
		ফেরত EINVAL;

	যদি (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		যদি (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			ফেরত EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		যদি (retval == 0)
			*out = blk;
		ফেরত retval;
	}

	যদি (!EXT2FS_IS_64_BITMAP(bitmap))
		ফেরত EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	যদি (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		ফেরত EINVAL;
	}

	যদি (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		যদি (retval)
			ফেরত retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		ফেরত 0;
	}

	জন্য (cout = cstart; cout <= cend; cout++)
		যদি (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			যাও found;

	ফেরত ENOENT;
}

errcode_t ext2fs_গুনতি_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		পরবর্তী;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	যখন (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &পরবর্তী);
		যদি (retval) {
			যদি (retval == ENOENT)
				retval = 0;
			বিরতি;
		}
		start = পরবর্তী;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &পরবর্তী);
		যদি (retval == 0) {
			tot_set += পরবর্তী - start;
			start  = পরবর্তী + 1;
		} অন্যথা যদি (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			বিরতি;
		} অন্যথা
			বিরতি;
	}

	যদি (!retval)
		*out = tot_set;
	ফেরত retval;
}

errcode_t ext2fs_গুনতি_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_গুনতি_used_blocks(fs, start, end, &tot_set);

	যদি (!retval)
		*out = EXT2_FS_অঙ্ক_B2_C(fs, tot_set);
	ফেরত retval;
}
