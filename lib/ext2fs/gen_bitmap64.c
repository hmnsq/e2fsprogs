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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <errno.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_زمان_H
#ضمن <sys/time.h>
#پایان_اگر

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
			صحيح کد, __u64 arg)
{
#اگر_تعریف_نشده OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->پایه_error_کد+کد,
			"#%llu for %s", (بیعلامت دراز دراز) arg,
			bitmap->description);
	وإلا
		com_err(0, bitmap->پایه_error_کد + کد, "#%llu",
			(بیعلامت دراز دراز) arg);
#پایان_اگر
}

#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
#تعریف_کن INC_STAT(map, name) map->stats.name
#وإلا
#تعریف_کن INC_STAT(map, name) ;;
#پایان_اگر


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 شروع, __u64 انتها,
				    __u64 real_انتها,
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
		إذا (retval || رقم_dirs > (fs->super->s_inodes_شمار / 320))
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

#اگر_تعریف_شده ENABLE_BMAP_STATS
	إذا (gettimeofday(&bitmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		أرجع 1;
	}
	bitmap->stats.type = type;
#پایان_اگر

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->شروع = شروع;
	bitmap->انتها = انتها;
	bitmap->real_انتها = real_انتها;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_بیتها = 0;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->پایه_error_کد = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلوک_BITMAP64:
		bitmap->پایه_error_کد = EXT2_ET_BAD_بلوک_MARK;
		bitmap->cluster_بیتها = fs->cluster_نسبت_بیتها;
		اكسر;
	افتراضي:
		bitmap->پایه_error_کد = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(طول_رشته(descr)+1, &bitmap->description);
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

#اگر_تعریف_شده ENABLE_BMAP_STATS
ساكن فراغ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2_bmap_statistics *stats = &bitmap->stats;
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	عائم mark_seq_perc = 0.0, test_seq_perc = 0.0;
	عائم mark_پشت_perc = 0.0, test_پشت_perc = 0.0;
	بنية timeval now;
	مزدوج inuse;

	إذا (stats->test_شمار) {
		test_seq_perc = ((عائم)stats->test_seq /
				 stats->test_شمار) * 100;
		test_پشت_perc = ((عائم)stats->test_پشت /
				  stats->test_شمار) * 100;
	}

	إذا (stats->mark_شمار) {
		mark_seq_perc = ((عائم)stats->mark_seq /
				 stats->mark_شمار) * 100;
		mark_پشت_perc = ((عائم)stats->mark_پشت /
				  stats->mark_شمار) * 100;
	}

	إذا (gettimeofday(&now, (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		أرجع;
	}

	inuse = (مزدوج) now.tv_sec + \
		(((مزدوج) now.tv_usec) * 0.000001);
	inuse -= (مزدوج) stats->created.tv_sec + \
		(((مزدوج) stats->created.tv_usec) * 0.000001);
#پایان_اگر /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_انتها - bitmap->شروع);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_شمار, stats->resize_شمار);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_شمار, stats->unmark_شمار);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_شمار, stats->mark_ext_شمار);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_شمار, stats->test_ext_شمار);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->مجموعه_range_شمار, stats->get_range_شمار);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_شمار, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_پشت, test_پشت_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_پشت, mark_پشت_perc, inuse);
#پایان_اگر /* ENABLE_BMAP_STATS_OPS */
}
#پایان_اگر

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

#اگر_تعریف_شده ENABLE_BMAP_STATS
	إذا (ext2fs_امن_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#پایان_اگر

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


#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	src->stats.copy_شمار++;
#پایان_اگر
#اگر_تعریف_شده ENABLE_BMAP_STATS
	إذا (gettimeofday(&new_bmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		أرجع 1;
	}
	new_bmap->stats.type = src->stats.type;
#پایان_اگر

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->شروع = src->شروع;
	new_bmap->انتها = src->انتها;
	new_bmap->real_انتها = src->real_انتها;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->پایه_error_کد = src->پایه_error_کد;
	new_bmap->cluster_بیتها = src->cluster_بیتها;

	descr = src->description;
	إذا (descr) {
		retval = ext2fs_get_mem(طول_رشته(descr)+10, &new_descr);
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
				     __u64 new_انتها,
				     __u64 new_real_انتها)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap))
		أرجع ext2fs_resize_generic_bitmap(gen_bmap->magic, new_انتها,
						    new_real_انتها, gen_bmap);

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, resize_شمار);

	أرجع bmap->bitmap_ops->resize_bmap(bmap, new_انتها, new_real_انتها);
}

errcode_t ext2fs_fudge_generic_bmap_انتها(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 انتها, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحيح retval;

		retval = ext2fs_fudge_generic_bitmap_انتها(gen_bitmap,
							 bitmap->magic,
							 neq, انتها, &tmp_oend);
		إذا (oend)
			*oend = tmp_oend;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	إذا (انتها > bitmap->real_انتها)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->انتها;
	bitmap->انتها = انتها;
	أرجع 0;
}

__u64 ext2fs_get_generic_bmap_شروع(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_شروع(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->شروع;
}

__u64 ext2fs_get_generic_bmap_انتها(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_انتها(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->انتها;
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

	arg >>= bitmap->cluster_بیتها;

#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	إذا (arg == bitmap->stats.آخرین_marked + 1)
		bitmap->stats.mark_seq++;
	إذا (arg < bitmap->stats.آخرین_marked)
		bitmap->stats.mark_پشت++;
	bitmap->stats.آخرین_marked = arg;
	bitmap->stats.mark_شمار++;
#پایان_اگر

	إذا ((arg < bitmap->شروع) || (arg > bitmap->انتها)) {
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

	arg >>= bitmap->cluster_بیتها;

	INC_STAT(bitmap, unmark_شمار);

	إذا ((arg < bitmap->شروع) || (arg > bitmap->انتها)) {
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

	arg >>= bitmap->cluster_بیتها;

#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_شمار++;
	إذا (arg == bitmap->stats.آخرین_tested + 1)
		bitmap->stats.test_seq++;
	إذا (arg < bitmap->stats.آخرین_tested)
		bitmap->stats.test_پشت++;
	bitmap->stats.آخرین_tested = arg;
#پایان_اگر

	إذا ((arg < bitmap->شروع) || (arg > bitmap->انتها)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		أرجع 0;
	}

	أرجع bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_مجموعه_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 شروع, بیعلامت صحيح رقم,
					فراغ *درون)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((شروع+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_مجموعه_generic_bitmap_range(gen_bmap, bmap->magic,
						       شروع, رقم, درون);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, مجموعه_range_شمار);

	أرجع bmap->bitmap_ops->مجموعه_bmap_range(bmap, شروع, رقم, درون);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 شروع, بیعلامت صحيح رقم,
					فراغ *بیرون)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((شروع+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       شروع, رقم, بیرون);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, get_range_شمار);

	أرجع bmap->bitmap_ops->get_bmap_range(bmap, شروع, رقم, بیرون);
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

	إذا ((bm1->شروع != bm2->شروع) ||
	    (bm1->انتها != bm2->انتها))
		أرجع neq;

	لكل (i = bm1->شروع; i < bm1->انتها; i++) {
		صحيح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		إذا (ret1 != ret2) {
			أرجع neq;
		}
	}

	أرجع 0;
}

فراغ ext2fs_مجموعه_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	شروع, رقم;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_مجموعه_generic_bitmap_padding(gen_bmap);
		أرجع;
	}

	شروع = bmap->انتها + 1;
	رقم = bmap->real_انتها - bmap->انتها;
	bmap->bitmap_ops->mark_bmap_extent(bmap, شروع, رقم);
	/* XXX ought to warn on error */
}

صحيح ext2fs_test_بلوک_bitmap_range2(ext2fs_بلوک_bitmap gen_bmap,
				    blk64_t بلوک, بیعلامت صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتها = بلوک + رقم;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (رقم == 1)
		أرجع !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, بلوک);

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلوک & ~0xffffffffULL) ||
		    ((بلوک+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_test_بلوک_bitmap_range(
			(ext2fs_generic_bitmap) bmap, بلوک, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, test_ext_شمار);

	/* convert to clusters if necessary */
	بلوک >>= bmap->cluster_بیتها;
	انتها += (1ULL << bmap->cluster_بیتها) - 1;
	انتها >>= bmap->cluster_بیتها;
	رقم = انتها - بلوک;

	إذا ((بلوک < bmap->شروع) || (بلوک > bmap->انتها) ||
	    (بلوک+رقم-1 > bmap->انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_TEST, بلوک,
				   bmap->description);
		أرجع EINVAL;
	}

	أرجع bmap->bitmap_ops->test_clear_bmap_extent(bmap, بلوک, رقم);
}

فراغ ext2fs_mark_بلوک_bitmap_range2(ext2fs_بلوک_bitmap gen_bmap,
				     blk64_t بلوک, بیعلامت صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتها = بلوک + رقم;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلوک & ~0xffffffffULL) ||
		    ((بلوک+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_mark_بلوک_bitmap_range((ext2fs_generic_bitmap) bmap,
					       بلوک, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, mark_ext_شمار);

	/* convert to clusters if necessary */
	بلوک >>= bmap->cluster_بیتها;
	انتها += (1ULL << bmap->cluster_بیتها) - 1;
	انتها >>= bmap->cluster_بیتها;
	رقم = انتها - بلوک;

	إذا ((بلوک < bmap->شروع) || (بلوک > bmap->انتها) ||
	    (بلوک+رقم-1 > bmap->انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_MARK, بلوک,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, بلوک, رقم);
}

فراغ ext2fs_unmark_بلوک_bitmap_range2(ext2fs_بلوک_bitmap gen_bmap,
				       blk64_t بلوک, بیعلامت صحيح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتها = بلوک + رقم;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلوک & ~0xffffffffULL) ||
		    ((بلوک+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_unmark_بلوک_bitmap_range((ext2fs_generic_bitmap) bmap,
						 بلوک, رقم);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, unmark_ext_شمار);

	/* convert to clusters if necessary */
	بلوک >>= bmap->cluster_بیتها;
	انتها += (1ULL << bmap->cluster_بیتها) - 1;
	انتها >>= bmap->cluster_بیتها;
	رقم = انتها - بلوک;

	إذا ((بلوک < bmap->شروع) || (بلوک > bmap->انتها) ||
	    (بلوک+رقم-1 > bmap->انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_UNMARK, بلوک,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, بلوک, رقم);
}

فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت حرف *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#اگر_تعریف_نشده OMIT_COM_ERR
	إذا (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	وإلا
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#پایان_اگر
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلوک_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_بلوک_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, بعدی, b_انتها, c_انتها;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	إذا (fs->cluster_نسبت_بیتها == ext2fs_get_bitmap_granularity(gen_bmap))
		أرجع 0;	/* Nothing to do */

	retval = ext2fs_allocate_بلوک_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	إذا (retval)
		أرجع retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->شروع;
	b_انتها = bmap->انتها;
	bmap->انتها = bmap->real_انتها;
	c_انتها = cmap->انتها;
	cmap->انتها = cmap->real_انتها;
	طالما (i < bmap->real_انتها) {
		retval = ext2fs_پیدا_کن_اولی_مجموعه_بلوک_bitmap2(gen_bmap,
						i, bmap->real_انتها, &بعدی);
		إذا (retval)
			اكسر;
		ext2fs_mark_بلوک_bitmap2(gen_cmap, بعدی);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, بعدی) + 1);
	}
	bmap->انتها = b_انتها;
	cmap->انتها = c_انتها;
	ext2fs_free_بلوک_bitmap(gen_bmap);
	*bitmap = (ext2fs_بلوک_bitmap) cmap;
	أرجع 0;
}

errcode_t ext2fs_پیدا_کن_اولی_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((شروع) & ~0xffffffffULL) ||
		    ((انتها) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, شروع);
			أرجع EINVAL;
		}

		retval = ext2fs_پیدا_کن_اولی_zero_generic_bitmap(bitmap, شروع,
							       انتها, &blk);
		إذا (retval == 0)
			*بیرون = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = شروع >> bmap64->cluster_بیتها;
	cend = انتها >> bmap64->cluster_بیتها;

	إذا (cstart < bmap64->شروع || cend > bmap64->انتها || شروع > انتها) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->پیدا_کن_اولی_zero) {
		retval = bmap64->bitmap_ops->پیدا_کن_اولی_zero(bmap64, cstart,
							     cend, &cout);
		إذا (retval)
			أرجع retval;
	یافت:
		cout <<= bmap64->cluster_بیتها;
		*بیرون = (cout >= شروع) ? cout : شروع;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب یافت;

	أرجع ENOENT;
}

errcode_t ext2fs_پیدا_کن_اولی_مجموعه_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((شروع) & ~0xffffffffULL) ||
		    ((انتها) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, شروع);
			أرجع EINVAL;
		}

		retval = ext2fs_پیدا_کن_اولی_مجموعه_generic_bitmap(bitmap, شروع,
							      انتها, &blk);
		إذا (retval == 0)
			*بیرون = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = شروع >> bmap64->cluster_بیتها;
	cend = انتها >> bmap64->cluster_بیتها;

	إذا (cstart < bmap64->شروع || cend > bmap64->انتها || شروع > انتها) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->پیدا_کن_اولی_مجموعه) {
		retval = bmap64->bitmap_ops->پیدا_کن_اولی_مجموعه(bmap64, cstart,
							    cend, &cout);
		إذا (retval)
			أرجع retval;
	یافت:
		cout <<= bmap64->cluster_بیتها;
		*بیرون = (cout >= شروع) ? cout : شروع;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب یافت;

	أرجع ENOENT;
}

errcode_t ext2fs_شمار_استفاده_شده_بلوکها(ext2_filsys fs, blk64_t شروع,
				   blk64_t انتها, blk64_t *بیرون)
{
	blk64_t		بعدی;
	blk64_t		tot_مجموعه = 0;
	errcode_t	retval = 0;

	طالما (شروع < انتها) {
		retval = ext2fs_پیدا_کن_اولی_مجموعه_بلوک_bitmap2(fs->بلوک_map,
							شروع, انتها, &بعدی);
		إذا (retval) {
			إذا (retval == ENOENT)
				retval = 0;
			اكسر;
		}
		شروع = بعدی;

		retval = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(fs->بلوک_map,
							شروع, انتها, &بعدی);
		إذا (retval == 0) {
			tot_مجموعه += بعدی - شروع;
			شروع  = بعدی + 1;
		} وإلا إذا (retval == ENOENT) {
			retval = 0;
			tot_مجموعه += انتها - شروع + 1;
			اكسر;
		} وإلا
			اكسر;
	}

	إذا (!retval)
		*بیرون = tot_مجموعه;
	أرجع retval;
}

errcode_t ext2fs_شمار_استفاده_شده_clusters(ext2_filsys fs, blk64_t شروع,
				     blk64_t انتها, blk64_t *بیرون)
{
	blk64_t		tot_مجموعه = 0;
	errcode_t	retval = 0;

	retval = ext2fs_شمار_استفاده_شده_بلوکها(fs, شروع, انتها, &tot_مجموعه);

	إذا (!retval)
		*بیرون = EXT2_FS_رقم_B2_C(fs, tot_مجموعه);
	أرجع retval;
}
