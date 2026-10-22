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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <errno.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_وخت_H
#ضمن <sys/time.h>
#د_که_پای

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
			صحيح کوډ, __u64 arg)
{
#که_نه_تعریف_شوی OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->بنسټ_error_کوډ+کوډ,
			"#%llu for %s", (بینښه اوږد اوږد) arg,
			bitmap->description);
	وإلا
		com_err(0, bitmap->بنسټ_error_کوډ + کوډ, "#%llu",
			(بینښه اوږد اوږد) arg);
#د_که_پای
}

#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
#تعریف_کړه INC_STAT(map, name) map->stats.name
#وإلا
#تعریف_کړه INC_STAT(map, name) ;;
#د_که_پای


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 پیل, __u64 پای_ټکی,
				    __u64 real_پای_ټکی,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	بنية ext2_bitmap_ops	*ops;
	ext2_ino_t عدد_dirs;
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
		retval = ext2fs_get_عدد_dirs(fs, &عدد_dirs);
		إذا (retval || عدد_dirs > (fs->super->s_inodes_شمېر / 320))
			ops = &ext2fs_blkmap64_bitarray;
		وإلا
			ops = &ext2fs_blkmap64_rbtree;
		اكسر;
	افتراضي:
		أرجع EINVAL;
	}

	retval = ext2fs_get_memzero(حجم(بنية ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	إذا (retval)
		أرجع retval;

#که_تعریف_شوی ENABLE_BMAP_STATS
	إذا (gettimeofday(&bitmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		أرجع 1;
	}
	bitmap->stats.type = type;
#د_که_پای

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->پیل = پیل;
	bitmap->پای_ټکی = پای_ټکی;
	bitmap->real_پای_ټکی = real_پای_ټکی;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_بټونه = 0;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلاک_BITMAP64:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_بلاک_MARK;
		bitmap->cluster_بټونه = fs->cluster_نسبت_بټونه;
		اكسر;
	افتراضي:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(مزي_اوږدوالی(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			أرجع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		أرجع retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	أرجع 0;
}

#که_تعریف_شوی ENABLE_BMAP_STATS
ساكن فراغ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2_bmap_statistics *stats = &bitmap->stats;
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	عائم mark_seq_perc = 0.0, test_seq_perc = 0.0;
	عائم mark_شا_perc = 0.0, test_شا_perc = 0.0;
	بنية timeval now;
	مزدوج inuse;

	إذا (stats->test_شمېر) {
		test_seq_perc = ((عائم)stats->test_seq /
				 stats->test_شمېر) * 100;
		test_شا_perc = ((عائم)stats->test_شا /
				  stats->test_شمېر) * 100;
	}

	إذا (stats->mark_شمېر) {
		mark_seq_perc = ((عائم)stats->mark_seq /
				 stats->mark_شمېر) * 100;
		mark_شا_perc = ((عائم)stats->mark_شا /
				  stats->mark_شمېر) * 100;
	}

	إذا (gettimeofday(&now, (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		أرجع;
	}

	inuse = (مزدوج) now.tv_sec + \
		(((مزدوج) now.tv_usec) * 0.000001);
	inuse -= (مزدوج) stats->created.tv_sec + \
		(((مزدوج) stats->created.tv_usec) * 0.000001);
#د_که_پای /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_پای_ټکی - bitmap->پیل);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_شمېر, stats->resize_شمېر);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_شمېر, stats->unmark_شمېر);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_شمېر, stats->mark_ext_شمېر);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_شمېر, stats->test_ext_شمېر);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->سېټ_range_شمېر, stats->get_range_شمېر);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_شمېر, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_شا, test_شا_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_شا, mark_شا_perc, inuse);
#د_که_پای /* ENABLE_BMAP_STATS_OPS */
}
#د_که_پای

فراغ ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		أرجع;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

#که_تعریف_شوی ENABLE_BMAP_STATS
	إذا (ext2fs_خوندي_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#د_که_پای

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
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(src))
		أرجع ext2fs_copy_generic_bitmap(gen_src, dest);

	إذا (!EXT2FS_IS_64_BITMAP(src))
		أرجع EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(حجم(بنية ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	إذا (retval)
		أرجع retval;


#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	src->stats.copy_شمېر++;
#د_که_پای
#که_تعریف_شوی ENABLE_BMAP_STATS
	إذا (gettimeofday(&new_bmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		أرجع 1;
	}
	new_bmap->stats.type = src->stats.type;
#د_که_پای

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->پیل = src->پیل;
	new_bmap->پای_ټکی = src->پای_ټکی;
	new_bmap->real_پای_ټکی = src->real_پای_ټکی;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->بنسټ_error_کوډ = src->بنسټ_error_کوډ;
	new_bmap->cluster_بټونه = src->cluster_بټونه;

	descr = src->description;
	إذا (descr) {
		retval = ext2fs_get_mem(مزي_اوږدوالی(descr)+10, &new_descr);
		إذا (retval) {
			ext2fs_free_mem(&new_bmap);
			أرجع retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	إذا (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		أرجع retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	أرجع 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_پای_ټکی,
				     __u64 new_real_پای_ټکی)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap))
		أرجع ext2fs_resize_generic_bitmap(gen_bmap->magic, new_پای_ټکی,
						    new_real_پای_ټکی, gen_bmap);

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, resize_شمېر);

	أرجع bmap->bitmap_ops->resize_bmap(bmap, new_پای_ټکی, new_real_پای_ټکی);
}

errcode_t ext2fs_fudge_generic_bmap_پای_ټکی(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 پای_ټکی, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحيح retval;

		retval = ext2fs_fudge_generic_bitmap_پای_ټکی(gen_bitmap,
							 bitmap->magic,
							 neq, پای_ټکی, &tmp_oend);
		إذا (oend)
			*oend = tmp_oend;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	إذا (پای_ټکی > bitmap->real_پای_ټکی)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->پای_ټکی;
	bitmap->پای_ټکی = پای_ټکی;
	أرجع 0;
}

__u64 ext2fs_get_generic_bmap_پیل(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_پیل(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->پیل;
}

__u64 ext2fs_get_generic_bmap_پای_ټکی(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_پای_ټکی(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->پای_ټکی;
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
		أرجع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			أرجع 0;
		}
		أرجع ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع 0;

	arg >>= bitmap->cluster_بټونه;

#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	إذا (arg == bitmap->stats.وروستی_marked + 1)
		bitmap->stats.mark_seq++;
	إذا (arg < bitmap->stats.وروستی_marked)
		bitmap->stats.mark_شا++;
	bitmap->stats.وروستی_marked = arg;
	bitmap->stats.mark_شمېر++;
#د_که_پای

	إذا ((arg < bitmap->پیل) || (arg > bitmap->پای_ټکی)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		أرجع 0;
	}

	أرجع bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			أرجع 0;
		}
		أرجع ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع 0;

	arg >>= bitmap->cluster_بټونه;

	INC_STAT(bitmap, unmark_شمېر);

	إذا ((arg < bitmap->پیل) || (arg > bitmap->پای_ټکی)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		أرجع 0;
	}

	أرجع bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	إذا (!bitmap)
		أرجع 0;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			أرجع 0;
		}
		أرجع ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع 0;

	arg >>= bitmap->cluster_بټونه;

#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_شمېر++;
	إذا (arg == bitmap->stats.وروستی_tested + 1)
		bitmap->stats.test_seq++;
	إذا (arg < bitmap->stats.وروستی_tested)
		bitmap->stats.test_شا++;
	bitmap->stats.وروستی_tested = arg;
#د_که_پای

	إذا ((arg < bitmap->پیل) || (arg > bitmap->پای_ټکی)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		أرجع 0;
	}

	أرجع bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_سېټ_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 پیل, بینښه صحيح عدد,
					فراغ *دننه)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((پیل+عدد-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_سېټ_generic_bitmap_range(gen_bmap, bmap->magic,
						       پیل, عدد, دننه);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, سېټ_range_شمېر);

	أرجع bmap->bitmap_ops->سېټ_bmap_range(bmap, پیل, عدد, دننه);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 پیل, بینښه صحيح عدد,
					فراغ *بهر)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((پیل+عدد-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       پیل, عدد, بهر);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, get_range_شمېر);

	أرجع bmap->bitmap_ops->get_bmap_range(bmap, پیل, عدد, بهر);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	إذا (!bm1 || !bm2)
		أرجع EINVAL;
	إذا (bm1->magic != bm2->magic)
		أرجع EINVAL;

	/* Now we know both bitmaps have the same magic */
	إذا (EXT2FS_IS_32_BITMAP(bm1))
		أرجع ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	إذا (!EXT2FS_IS_64_BITMAP(bm1))
		أرجع EINVAL;

	إذا ((bm1->پیل != bm2->پیل) ||
	    (bm1->پای_ټکی != bm2->پای_ټکی))
		أرجع neq;

	لكل (i = bm1->پیل; i < bm1->پای_ټکی; i++) {
		صحيح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		إذا (ret1 != ret2) {
			أرجع neq;
		}
	}

	أرجع 0;
}

فراغ ext2fs_سېټ_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پیل, عدد;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_سېټ_generic_bitmap_padding(gen_bmap);
		أرجع;
	}

	پیل = bmap->پای_ټکی + 1;
	عدد = bmap->real_پای_ټکی - bmap->پای_ټکی;
	bmap->bitmap_ops->mark_bmap_extent(bmap, پیل, عدد);
	/* XXX ought to warn on error */
}

صحيح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				    blk64_t بلاک, بینښه صحيح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پای_ټکی = بلاک + عدد;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (عدد == 1)
		أرجع !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, بلاک);

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_test_بلاک_bitmap_range(
			(ext2fs_generic_bitmap) bmap, بلاک, عدد);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, test_ext_شمېر);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بټونه;
	پای_ټکی += (1ULL << bmap->cluster_بټونه) - 1;
	پای_ټکی >>= bmap->cluster_بټونه;
	عدد = پای_ټکی - بلاک;

	إذا ((بلاک < bmap->پیل) || (بلاک > bmap->پای_ټکی) ||
	    (بلاک+عدد-1 > bmap->پای_ټکی)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_TEST, بلاک,
				   bmap->description);
		أرجع EINVAL;
	}

	أرجع bmap->bitmap_ops->test_clear_bmap_extent(bmap, بلاک, عدد);
}

فراغ ext2fs_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				     blk64_t بلاک, بینښه صحيح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پای_ټکی = بلاک + عدد;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_mark_بلاک_bitmap_range((ext2fs_generic_bitmap) bmap,
					       بلاک, عدد);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, mark_ext_شمېر);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بټونه;
	پای_ټکی += (1ULL << bmap->cluster_بټونه) - 1;
	پای_ټکی >>= bmap->cluster_بټونه;
	عدد = پای_ټکی - بلاک;

	إذا ((بلاک < bmap->پیل) || (بلاک > bmap->پای_ټکی) ||
	    (بلاک+عدد-1 > bmap->پای_ټکی)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_MARK, بلاک,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, بلاک, عدد);
}

فراغ ext2fs_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				       blk64_t بلاک, بینښه صحيح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پای_ټکی = بلاک + عدد;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_unmark_بلاک_bitmap_range((ext2fs_generic_bitmap) bmap,
						 بلاک, عدد);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, unmark_ext_شمېر);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بټونه;
	پای_ټکی += (1ULL << bmap->cluster_بټونه) - 1;
	پای_ټکی >>= bmap->cluster_بټونه;
	عدد = پای_ټکی - بلاک;

	إذا ((بلاک < bmap->پیل) || (بلاک > bmap->پای_ټکی) ||
	    (بلاک+عدد-1 > bmap->پای_ټکی)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_UNMARK, بلاک,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, بلاک, عدد);
}

فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت حرف *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#که_نه_تعریف_شوی OMIT_COM_ERR
	إذا (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	وإلا
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#د_که_پای
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاک_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_بلاک_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, بل, b_پای_ټکی, c_پای_ټکی;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	إذا (fs->cluster_نسبت_بټونه == ext2fs_get_bitmap_granularity(gen_bmap))
		أرجع 0;	/* Nothing to do */

	retval = ext2fs_allocate_بلاک_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	إذا (retval)
		أرجع retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->پیل;
	b_پای_ټکی = bmap->پای_ټکی;
	bmap->پای_ټکی = bmap->real_پای_ټکی;
	c_پای_ټکی = cmap->پای_ټکی;
	cmap->پای_ټکی = cmap->real_پای_ټکی;
	طالما (i < bmap->real_پای_ټکی) {
		retval = ext2fs_پیدا_کړه_لومړی_سېټ_بلاک_bitmap2(gen_bmap,
						i, bmap->real_پای_ټکی, &بل);
		إذا (retval)
			اكسر;
		ext2fs_mark_بلاک_bitmap2(gen_cmap, بل);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, بل) + 1);
	}
	bmap->پای_ټکی = b_پای_ټکی;
	cmap->پای_ټکی = c_پای_ټکی;
	ext2fs_free_بلاک_bitmap(gen_bmap);
	*bitmap = (ext2fs_بلاک_bitmap) cmap;
	أرجع 0;
}

errcode_t ext2fs_پیدا_کړه_لومړی_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((پیل) & ~0xffffffffULL) ||
		    ((پای_ټکی) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, پیل);
			أرجع EINVAL;
		}

		retval = ext2fs_پیدا_کړه_لومړی_zero_generic_bitmap(bitmap, پیل,
							       پای_ټکی, &blk);
		إذا (retval == 0)
			*بهر = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = پیل >> bmap64->cluster_بټونه;
	cend = پای_ټکی >> bmap64->cluster_بټونه;

	إذا (cstart < bmap64->پیل || cend > bmap64->پای_ټکی || پیل > پای_ټکی) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, پیل);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->پیدا_کړه_لومړی_zero) {
		retval = bmap64->bitmap_ops->پیدا_کړه_لومړی_zero(bmap64, cstart,
							     cend, &cout);
		إذا (retval)
			أرجع retval;
	وموندل:
		cout <<= bmap64->cluster_بټونه;
		*بهر = (cout >= پیل) ? cout : پیل;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب وموندل;

	أرجع ENOENT;
}

errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((پیل) & ~0xffffffffULL) ||
		    ((پای_ټکی) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, پیل);
			أرجع EINVAL;
		}

		retval = ext2fs_پیدا_کړه_لومړی_سېټ_generic_bitmap(bitmap, پیل,
							      پای_ټکی, &blk);
		إذا (retval == 0)
			*بهر = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = پیل >> bmap64->cluster_بټونه;
	cend = پای_ټکی >> bmap64->cluster_بټونه;

	إذا (cstart < bmap64->پیل || cend > bmap64->پای_ټکی || پیل > پای_ټکی) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, پیل);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->پیدا_کړه_لومړی_سېټ) {
		retval = bmap64->bitmap_ops->پیدا_کړه_لومړی_سېټ(bmap64, cstart,
							    cend, &cout);
		إذا (retval)
			أرجع retval;
	وموندل:
		cout <<= bmap64->cluster_بټونه;
		*بهر = (cout >= پیل) ? cout : پیل;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب وموندل;

	أرجع ENOENT;
}

errcode_t ext2fs_شمېر_کارول_شوی_بلاکونه(ext2_filsys fs, blk64_t پیل,
				   blk64_t پای_ټکی, blk64_t *بهر)
{
	blk64_t		بل;
	blk64_t		tot_سېټ = 0;
	errcode_t	retval = 0;

	طالما (پیل < پای_ټکی) {
		retval = ext2fs_پیدا_کړه_لومړی_سېټ_بلاک_bitmap2(fs->بلاک_map,
							پیل, پای_ټکی, &بل);
		إذا (retval) {
			إذا (retval == ENOENT)
				retval = 0;
			اكسر;
		}
		پیل = بل;

		retval = ext2fs_پیدا_کړه_لومړی_zero_بلاک_bitmap2(fs->بلاک_map,
							پیل, پای_ټکی, &بل);
		إذا (retval == 0) {
			tot_سېټ += بل - پیل;
			پیل  = بل + 1;
		} وإلا إذا (retval == ENOENT) {
			retval = 0;
			tot_سېټ += پای_ټکی - پیل + 1;
			اكسر;
		} وإلا
			اكسر;
	}

	إذا (!retval)
		*بهر = tot_سېټ;
	أرجع retval;
}

errcode_t ext2fs_شمېر_کارول_شوی_clusters(ext2_filsys fs, blk64_t پیل,
				     blk64_t پای_ټکی, blk64_t *بهر)
{
	blk64_t		tot_سېټ = 0;
	errcode_t	retval = 0;

	retval = ext2fs_شمېر_کارول_شوی_بلاکونه(fs, پیل, پای_ټکی, &tot_سېټ);

	إذا (!retval)
		*بهر = EXT2_FS_عدد_B2_C(fs, tot_سېټ);
	أرجع retval;
}
