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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <errno.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_وقت_H
#ضمن <sys/time.h>
#جيڪڏهن_پورو

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
			صحيح ڪوڊ, __u64 arg)
{
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->بنياد_error_ڪوڊ+ڪوڊ,
			"#%llu for %s", (بنانشان ڊگهو ڊگهو) arg,
			bitmap->description);
	وإلا
		com_err(0, bitmap->بنياد_error_ڪوڊ + ڪوڊ, "#%llu",
			(بنانشان ڊگهو ڊگهو) arg);
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
#وصف_ڪريو INC_STAT(map, name) map->stats.name
#وإلا
#وصف_ڪريو INC_STAT(map, name) ;;
#جيڪڏهن_پورو


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحيح type, __u64 شروعات, __u64 پڇاڙي,
				    __u64 real_پڇاڙي,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	بنية ext2_bitmap_ops	*ops;
	ext2_ino_t انگ_dirs;
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
		retval = ext2fs_get_انگ_dirs(fs, &انگ_dirs);
		إذا (retval || انگ_dirs > (fs->super->s_inodes_ڳڻپ / 320))
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

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
	إذا (gettimeofday(&bitmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		أرجع 1;
	}
	bitmap->stats.type = type;
#جيڪڏهن_پورو

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->شروعات = شروعات;
	bitmap->پڇاڙي = پڇاڙي;
	bitmap->real_پڇاڙي = real_پڇاڙي;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_بٽ = 0;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلاڪ_BITMAP64:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_بلاڪ_MARK;
		bitmap->cluster_بٽ = fs->cluster_تناسب_بٽ;
		اكسر;
	افتراضي:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(لڙي_ڊيگهه(descr)+1, &bitmap->description);
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

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
ساكن فراغ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2_bmap_statistics *stats = &bitmap->stats;
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	عائم mark_seq_perc = 0.0, test_seq_perc = 0.0;
	عائم mark_پٺ_perc = 0.0, test_پٺ_perc = 0.0;
	بنية timeval now;
	مزدوج inuse;

	إذا (stats->test_ڳڻپ) {
		test_seq_perc = ((عائم)stats->test_seq /
				 stats->test_ڳڻپ) * 100;
		test_پٺ_perc = ((عائم)stats->test_پٺ /
				  stats->test_ڳڻپ) * 100;
	}

	إذا (stats->mark_ڳڻپ) {
		mark_seq_perc = ((عائم)stats->mark_seq /
				 stats->mark_ڳڻپ) * 100;
		mark_پٺ_perc = ((عائم)stats->mark_پٺ /
				  stats->mark_ڳڻپ) * 100;
	}

	إذا (gettimeofday(&now, (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		أرجع;
	}

	inuse = (مزدوج) now.tv_sec + \
		(((مزدوج) now.tv_usec) * 0.000001);
	inuse -= (مزدوج) stats->created.tv_sec + \
		(((مزدوج) stats->created.tv_usec) * 0.000001);
#جيڪڏهن_پورو /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_پڇاڙي - bitmap->شروعات);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_ڳڻپ, stats->resize_ڳڻپ);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_ڳڻپ, stats->unmark_ڳڻپ);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_ڳڻپ, stats->mark_ext_ڳڻپ);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_ڳڻپ, stats->test_ext_ڳڻپ);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->سيٽ_range_ڳڻپ, stats->get_range_ڳڻپ);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_ڳڻپ, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_پٺ, test_پٺ_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_پٺ, mark_پٺ_perc, inuse);
#جيڪڏهن_پورو /* ENABLE_BMAP_STATS_OPS */
}
#جيڪڏهن_پورو

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

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
	إذا (ext2fs_محفوظ_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#جيڪڏهن_پورو

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


#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	src->stats.copy_ڳڻپ++;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
	إذا (gettimeofday(&new_bmap->stats.created,
			 (بنية timezone *) عدم) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		أرجع 1;
	}
	new_bmap->stats.type = src->stats.type;
#جيڪڏهن_پورو

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->شروعات = src->شروعات;
	new_bmap->پڇاڙي = src->پڇاڙي;
	new_bmap->real_پڇاڙي = src->real_پڇاڙي;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->بنياد_error_ڪوڊ = src->بنياد_error_ڪوڊ;
	new_bmap->cluster_بٽ = src->cluster_بٽ;

	descr = src->description;
	إذا (descr) {
		retval = ext2fs_get_mem(لڙي_ڊيگهه(descr)+10, &new_descr);
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
				     __u64 new_پڇاڙي,
				     __u64 new_real_پڇاڙي)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap))
		أرجع ext2fs_resize_generic_bitmap(gen_bmap->magic, new_پڇاڙي,
						    new_real_پڇاڙي, gen_bmap);

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, resize_ڳڻپ);

	أرجع bmap->bitmap_ops->resize_bmap(bmap, new_پڇاڙي, new_real_پڇاڙي);
}

errcode_t ext2fs_fudge_generic_bmap_پڇاڙي(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 پڇاڙي, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحيح retval;

		retval = ext2fs_fudge_generic_bitmap_پڇاڙي(gen_bitmap,
							 bitmap->magic,
							 neq, پڇاڙي, &tmp_oend);
		إذا (oend)
			*oend = tmp_oend;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	إذا (پڇاڙي > bitmap->real_پڇاڙي)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->پڇاڙي;
	bitmap->پڇاڙي = پڇاڙي;
	أرجع 0;
}

__u64 ext2fs_get_generic_bmap_شروعات(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_شروعات(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->شروعات;
}

__u64 ext2fs_get_generic_bmap_پڇاڙي(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap))
		أرجع ext2fs_get_generic_bitmap_پڇاڙي(gen_bitmap);

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	أرجع bitmap->پڇاڙي;
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

	arg >>= bitmap->cluster_بٽ;

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	إذا (arg == bitmap->stats.آخري_marked + 1)
		bitmap->stats.mark_seq++;
	إذا (arg < bitmap->stats.آخري_marked)
		bitmap->stats.mark_پٺ++;
	bitmap->stats.آخري_marked = arg;
	bitmap->stats.mark_ڳڻپ++;
#جيڪڏهن_پورو

	إذا ((arg < bitmap->شروعات) || (arg > bitmap->پڇاڙي)) {
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

	arg >>= bitmap->cluster_بٽ;

	INC_STAT(bitmap, unmark_ڳڻپ);

	إذا ((arg < bitmap->شروعات) || (arg > bitmap->پڇاڙي)) {
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

	arg >>= bitmap->cluster_بٽ;

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_ڳڻپ++;
	إذا (arg == bitmap->stats.آخري_tested + 1)
		bitmap->stats.test_seq++;
	إذا (arg < bitmap->stats.آخري_tested)
		bitmap->stats.test_پٺ++;
	bitmap->stats.آخري_tested = arg;
#جيڪڏهن_پورو

	إذا ((arg < bitmap->شروعات) || (arg > bitmap->پڇاڙي)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		أرجع 0;
	}

	أرجع bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_سيٽ_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 شروعات, بنانشان صحيح انگ,
					فراغ *اندريون)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((شروعات+انگ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_سيٽ_generic_bitmap_range(gen_bmap, bmap->magic,
						       شروعات, انگ, اندريون);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, سيٽ_range_ڳڻپ);

	أرجع bmap->bitmap_ops->سيٽ_bmap_range(bmap, شروعات, انگ, اندريون);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((شروعات+انگ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       شروعات, انگ, ٻاهريون);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, get_range_ڳڻپ);

	أرجع bmap->bitmap_ops->get_bmap_range(bmap, شروعات, انگ, ٻاهريون);
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

	إذا ((bm1->شروعات != bm2->شروعات) ||
	    (bm1->پڇاڙي != bm2->پڇاڙي))
		أرجع neq;

	لكل (i = bm1->شروعات; i < bm1->پڇاڙي; i++) {
		صحيح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		إذا (ret1 != ret2) {
			أرجع neq;
		}
	}

	أرجع 0;
}

فراغ ext2fs_سيٽ_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	شروعات, انگ;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_سيٽ_generic_bitmap_padding(gen_bmap);
		أرجع;
	}

	شروعات = bmap->پڇاڙي + 1;
	انگ = bmap->real_پڇاڙي - bmap->پڇاڙي;
	bmap->bitmap_ops->mark_bmap_extent(bmap, شروعات, انگ);
	/* XXX ought to warn on error */
}

صحيح ext2fs_test_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap gen_bmap,
				    blk64_t بلاڪ, بنانشان صحيح انگ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پڇاڙي = بلاڪ + انگ;

	إذا (!bmap)
		أرجع EINVAL;

	إذا (انگ == 1)
		أرجع !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, بلاڪ);

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاڪ & ~0xffffffffULL) ||
		    ((بلاڪ+انگ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع EINVAL;
		}
		أرجع ext2fs_test_بلاڪ_bitmap_range(
			(ext2fs_generic_bitmap) bmap, بلاڪ, انگ);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع EINVAL;

	INC_STAT(bmap, test_ext_ڳڻپ);

	/* convert to clusters if necessary */
	بلاڪ >>= bmap->cluster_بٽ;
	پڇاڙي += (1ULL << bmap->cluster_بٽ) - 1;
	پڇاڙي >>= bmap->cluster_بٽ;
	انگ = پڇاڙي - بلاڪ;

	إذا ((بلاڪ < bmap->شروعات) || (بلاڪ > bmap->پڇاڙي) ||
	    (بلاڪ+انگ-1 > bmap->پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_TEST, بلاڪ,
				   bmap->description);
		أرجع EINVAL;
	}

	أرجع bmap->bitmap_ops->test_clear_bmap_extent(bmap, بلاڪ, انگ);
}

فراغ ext2fs_mark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap gen_bmap,
				     blk64_t بلاڪ, بنانشان صحيح انگ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پڇاڙي = بلاڪ + انگ;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاڪ & ~0xffffffffULL) ||
		    ((بلاڪ+انگ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_mark_بلاڪ_bitmap_range((ext2fs_generic_bitmap) bmap,
					       بلاڪ, انگ);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, mark_ext_ڳڻپ);

	/* convert to clusters if necessary */
	بلاڪ >>= bmap->cluster_بٽ;
	پڇاڙي += (1ULL << bmap->cluster_بٽ) - 1;
	پڇاڙي >>= bmap->cluster_بٽ;
	انگ = پڇاڙي - بلاڪ;

	إذا ((بلاڪ < bmap->شروعات) || (بلاڪ > bmap->پڇاڙي) ||
	    (بلاڪ+انگ-1 > bmap->پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_MARK, بلاڪ,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, بلاڪ, انگ);
}

فراغ ext2fs_unmark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap gen_bmap,
				       blk64_t بلاڪ, بنانشان صحيح انگ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	پڇاڙي = بلاڪ + انگ;

	إذا (!bmap)
		أرجع;

	إذا (EXT2FS_IS_32_BITMAP(bmap)) {
		إذا ((بلاڪ & ~0xffffffffULL) ||
		    ((بلاڪ+انگ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			أرجع;
		}
		ext2fs_unmark_بلاڪ_bitmap_range((ext2fs_generic_bitmap) bmap,
						 بلاڪ, انگ);
	}

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع;

	INC_STAT(bmap, unmark_ext_ڳڻپ);

	/* convert to clusters if necessary */
	بلاڪ >>= bmap->cluster_بٽ;
	پڇاڙي += (1ULL << bmap->cluster_بٽ) - 1;
	پڇاڙي >>= bmap->cluster_بٽ;
	انگ = پڇاڙي - بلاڪ;

	إذا ((بلاڪ < bmap->شروعات) || (بلاڪ > bmap->پڇاڙي) ||
	    (بلاڪ+انگ-1 > bmap->پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_UNMARK, بلاڪ,
				   bmap->description);
		أرجع;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, بلاڪ, انگ);
}

فراغ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت حرف *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
	إذا (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	وإلا
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#جيڪڏهن_پورو
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاڪ_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_بلاڪ_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, ايندڙ, b_پڇاڙي, c_پڇاڙي;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	إذا (fs->cluster_تناسب_بٽ == ext2fs_get_bitmap_granularity(gen_bmap))
		أرجع 0;	/* Nothing to do */

	retval = ext2fs_allocate_بلاڪ_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	إذا (retval)
		أرجع retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->شروعات;
	b_پڇاڙي = bmap->پڇاڙي;
	bmap->پڇاڙي = bmap->real_پڇاڙي;
	c_پڇاڙي = cmap->پڇاڙي;
	cmap->پڇاڙي = cmap->real_پڇاڙي;
	طالما (i < bmap->real_پڇاڙي) {
		retval = ext2fs_ڳوليو_پهريون_سيٽ_بلاڪ_bitmap2(gen_bmap,
						i, bmap->real_پڇاڙي, &ايندڙ);
		إذا (retval)
			اكسر;
		ext2fs_mark_بلاڪ_bitmap2(gen_cmap, ايندڙ);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, ايندڙ) + 1);
	}
	bmap->پڇاڙي = b_پڇاڙي;
	cmap->پڇاڙي = c_پڇاڙي;
	ext2fs_free_بلاڪ_bitmap(gen_bmap);
	*bitmap = (ext2fs_بلاڪ_bitmap) cmap;
	أرجع 0;
}

errcode_t ext2fs_ڳوليو_پهريون_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((شروعات) & ~0xffffffffULL) ||
		    ((پڇاڙي) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, شروعات);
			أرجع EINVAL;
		}

		retval = ext2fs_ڳوليو_پهريون_zero_generic_bitmap(bitmap, شروعات,
							       پڇاڙي, &blk);
		إذا (retval == 0)
			*ٻاهريون = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = شروعات >> bmap64->cluster_بٽ;
	cend = پڇاڙي >> bmap64->cluster_بٽ;

	إذا (cstart < bmap64->شروعات || cend > bmap64->پڇاڙي || شروعات > پڇاڙي) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, شروعات);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->ڳوليو_پهريون_zero) {
		retval = bmap64->bitmap_ops->ڳوليو_پهريون_zero(bmap64, cstart,
							     cend, &cout);
		إذا (retval)
			أرجع retval;
	مليو:
		cout <<= bmap64->cluster_بٽ;
		*ٻاهريون = (cout >= شروعات) ? cout : شروعات;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب مليو;

	أرجع ENOENT;
}

errcode_t ext2fs_ڳوليو_پهريون_سيٽ_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	إذا (!bitmap)
		أرجع EINVAL;

	إذا (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		إذا (((شروعات) & ~0xffffffffULL) ||
		    ((پڇاڙي) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, شروعات);
			أرجع EINVAL;
		}

		retval = ext2fs_ڳوليو_پهريون_سيٽ_generic_bitmap(bitmap, شروعات,
							      پڇاڙي, &blk);
		إذا (retval == 0)
			*ٻاهريون = blk;
		أرجع retval;
	}

	إذا (!EXT2FS_IS_64_BITMAP(bitmap))
		أرجع EINVAL;

	cstart = شروعات >> bmap64->cluster_بٽ;
	cend = پڇاڙي >> bmap64->cluster_بٽ;

	إذا (cstart < bmap64->شروعات || cend > bmap64->پڇاڙي || شروعات > پڇاڙي) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, شروعات);
		أرجع EINVAL;
	}

	إذا (bmap64->bitmap_ops->ڳوليو_پهريون_سيٽ) {
		retval = bmap64->bitmap_ops->ڳوليو_پهريون_سيٽ(bmap64, cstart,
							    cend, &cout);
		إذا (retval)
			أرجع retval;
	مليو:
		cout <<= bmap64->cluster_بٽ;
		*ٻاهريون = (cout >= شروعات) ? cout : شروعات;
		أرجع 0;
	}

	لكل (cout = cstart; cout <= cend; cout++)
		إذا (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			اذهب مليو;

	أرجع ENOENT;
}

errcode_t ext2fs_ڳڻپ_استعمال_ٿيل_بلاڪون(ext2_filsys fs, blk64_t شروعات,
				   blk64_t پڇاڙي, blk64_t *ٻاهريون)
{
	blk64_t		ايندڙ;
	blk64_t		tot_سيٽ = 0;
	errcode_t	retval = 0;

	طالما (شروعات < پڇاڙي) {
		retval = ext2fs_ڳوليو_پهريون_سيٽ_بلاڪ_bitmap2(fs->بلاڪ_map,
							شروعات, پڇاڙي, &ايندڙ);
		إذا (retval) {
			إذا (retval == ENOENT)
				retval = 0;
			اكسر;
		}
		شروعات = ايندڙ;

		retval = ext2fs_ڳوليو_پهريون_zero_بلاڪ_bitmap2(fs->بلاڪ_map,
							شروعات, پڇاڙي, &ايندڙ);
		إذا (retval == 0) {
			tot_سيٽ += ايندڙ - شروعات;
			شروعات  = ايندڙ + 1;
		} وإلا إذا (retval == ENOENT) {
			retval = 0;
			tot_سيٽ += پڇاڙي - شروعات + 1;
			اكسر;
		} وإلا
			اكسر;
	}

	إذا (!retval)
		*ٻاهريون = tot_سيٽ;
	أرجع retval;
}

errcode_t ext2fs_ڳڻپ_استعمال_ٿيل_clusters(ext2_filsys fs, blk64_t شروعات,
				     blk64_t پڇاڙي, blk64_t *ٻاهريون)
{
	blk64_t		tot_سيٽ = 0;
	errcode_t	retval = 0;

	retval = ext2fs_ڳڻپ_استعمال_ٿيل_بلاڪون(fs, شروعات, پڇاڙي, &tot_سيٽ);

	إذا (!retval)
		*ٻاهريون = EXT2_FS_انگ_B2_C(fs, tot_سيٽ);
	أرجع retval;
}
