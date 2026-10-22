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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <errno.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#ضمن <sys/time.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"

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

ساكن فراغ warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			صحيح code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	وإلا
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#وإلا
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 start, __u64 end,
				    __u64 real_end,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	بنية ext2_bitmap_ops	*ops;
	ext2_ino_t رقم_dirs;
	errcode_t retval;

	إذا (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	بدل (type) {
	حالة EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		اكسر;
	حالة EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		اكسر;
	حالة EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_رقم_dirs(fs, &رقم_dirs);
		إذا (retval || رقم_dirs > (fs->super->s_inodes_تعداد / 320))
			ops = &ext2fs_blkmap64_bitarray;
		وإلا
			ops = &ext2fs_blkmap64_rbtree;
		اكسر;
	افتراضي:
		إرجاع EINVAL;
	}

	retval = ext2fs_get_memzero(حجم(بنية ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	إذا (retval)
		إرجاع retval;

#ifdef ENABLE_BMAP_STATS
	إذا (gettimeofday(&bitmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		إرجاع 1;
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
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		اكسر;
	افتراضي:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			إرجاع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		إرجاع retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	إرجاع 0;
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	عائم mark_seq_perc = 0.0, test_seq_perc = 0.0;
	عائم mark_back_perc = 0.0, test_back_perc = 0.0;
	بنية timeval now;
	مزدوج inuse;

	إذا (stats->test_تعداد) {
		test_seq_perc = ((عائم)stats->test_seq /
				 stats->test_تعداد) * 100;
		test_back_perc = ((عائم)stats->test_back /
				  stats->test_تعداد) * 100;
	}

	إذا (stats->mark_تعداد) {
		mark_seq_perc = ((عائم)stats->mark_seq /
				 stats->mark_تعداد) * 100;
		mark_back_perc = ((عائم)stats->mark_back /
				  stats->mark_تعداد) * 100;
	}

	إذا (gettimeofday(&now, (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		إرجاع;
	}

	inuse = (مزدوج) now.tv_sec + \
		(((مزدوج) now.tv_usec) * 0.000001);
	inuse -= (مزدوج) stats->created.tv_sec + \
		(((مزدوج) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_تعداد, stats->resize_تعداد);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_تعداد, stats->unmark_تعداد);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_تعداد, stats->mark_ext_تعداد);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_تعداد, stats->test_ext_تعداد);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_تعداد, stats->get_range_تعداد);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_تعداد, stats->test_seq, test_seq_perc);
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

فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		إرجاع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		إرجاع;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع;

#ifdef ENABLE_BMAP_STATS
	إذا (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	إذا (bmap->description) {
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
	حرف *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	إذا (!src)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(src))
		إرجاع ext2fs_copy_generic_bitmap(gen_src, dest);

	إذا (!EXT2FS_IS_64_BITMAP(src))
		إرجاع EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(حجم(بنية ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	إذا (retval)
		إرجاع retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_تعداد++;
#endif
#ifdef ENABLE_BMAP_STATS
	إذا (gettimeofday(&new_bmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		إرجاع 1;
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
	إذا (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		إذا (retval) {
			ext2fs_free_mem(&new_bmap);
			إرجاع retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	إذا (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		إرجاع retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	إرجاع 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap))
		إرجاع ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع EINVAL;

	INC_STAT(bmap, resize_تعداد);

	إرجاع bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحيح retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		إذا (oend)
			*oend = tmp_oend;
		إرجاع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع EINVAL;

	إذا (end > bitmap->real_end)
		إرجاع neq;
	إذا (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	إرجاع 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		إرجاع ext2fs_get_generic_bitmap_start(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع EINVAL;

	إرجاع bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		إرجاع ext2fs_get_generic_bitmap_end(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع EINVAL;

	إرجاع bitmap->end;
}

فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	وإلا
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		إرجاع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			إرجاع 0;
		}
		إرجاع ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	إذا (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	إذا (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_تعداد++;
#endif

	إذا ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		إرجاع 0;
	}

	إرجاع bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		إرجاع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			إرجاع 0;
		}
		إرجاع ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_تعداد);

	إذا ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		إرجاع 0;
	}

	إرجاع bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	إذا (!bitmap)
		إرجاع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			إرجاع 0;
		}
		إرجاع ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_تعداد++;
	إذا (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	إذا (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	إذا ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		إرجاع 0;
	}

	إرجاع bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned صحيح رقم,
					فراغ *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((start+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			إرجاع EINVAL;
		}
		إرجاع ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, رقم, in);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع EINVAL;

	INC_STAT(bmap, set_range_تعداد);

	إرجاع bmap->bitmap_ops->set_bmap_range(bmap, start, رقم, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned صحيح رقم,
					فراغ *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((start+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			إرجاع EINVAL;
		}
		إرجاع ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, رقم, out);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع EINVAL;

	INC_STAT(bmap, get_range_تعداد);

	إرجاع bmap->bitmap_ops->get_bmap_range(bmap, start, رقم, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	إذا (!bm1 || !bm2)
		إرجاع EINVAL;
	إذا (bm1->magic != bm2->magic)
		إرجاع EINVAL;

	/* Now we know both bitmaps have the same magic */
	إذا (EXT2FS_IS_32_BITMAP(bm1))
		إرجاع ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	إذا (!EXT2FS_IS_64_BITMAP(bm1))
		إرجاع EINVAL;

	إذا ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		إرجاع neq;

	لكل (i = bm1->start; i < bm1->end; i++) {
		صحيح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		إذا (ret1 != ret2) {
			إرجاع neq;
		}
	}

	إرجاع 0;
}

فراغ ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, رقم;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		إرجاع;
	}

	start = bmap->end + 1;
	رقم = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, رقم);
	/* XXX ought to warn on error */
}

صحيح ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	إذا (!bmap)
		إرجاع EINVAL;

	إذا (رقم == 1)
		إرجاع !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			إرجاع EINVAL;
		}
		إرجاع ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع EINVAL;

	INC_STAT(bmap, test_ext_تعداد);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	إذا ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		إرجاع EINVAL;
	}

	إرجاع bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, رقم);
}

فراغ ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	إذا (!bmap)
		إرجاع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			إرجاع;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع;

	INC_STAT(bmap, mark_ext_تعداد);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	إذا ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		إرجاع;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, رقم);
}

فراغ ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	إذا (!bmap)
		إرجاع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			إرجاع;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		إرجاع;

	INC_STAT(bmap, unmark_ext_تعداد);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	إذا ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		إرجاع;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, رقم);
}

فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت حرف *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	إذا (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	وإلا
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
	إذا (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		إرجاع 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	إذا (retval)
		إرجاع retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	بينما (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		إذا (retval)
			اكسر;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	إرجاع 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			إرجاع EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		إذا (retval == 0)
			*out = blk;
		إرجاع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	إذا (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		إرجاع EINVAL;
	}

	إذا (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		إذا (retval)
			إرجاع retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		إرجاع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب found;

	إرجاع ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		إرجاع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			إرجاع EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		إذا (retval == 0)
			*out = blk;
		إرجاع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		إرجاع EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	إذا (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		إرجاع EINVAL;
	}

	إذا (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		إذا (retval)
			إرجاع retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		إرجاع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب found;

	إرجاع ENOENT;
}

errcode_t ext2fs_تعداد_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	بينما (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		إذا (retval) {
			إذا (retval == ENOENT)
				retval = 0;
			اكسر;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		إذا (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} وإلا إذا (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			اكسر;
		} وإلا
			اكسر;
	}

	إذا (!retval)
		*out = tot_set;
	إرجاع retval;
}

errcode_t ext2fs_تعداد_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_تعداد_used_blocks(fs, start, end, &tot_set);

	إذا (!retval)
		*out = EXT2_FS_رقم_B2_C(fs, tot_set);
	إرجاع retval;
}
