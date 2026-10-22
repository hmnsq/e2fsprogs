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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#شامل <errno.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_وقت_H
#شامل <sys/time.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"

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

ساکن خالی warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			صحیح رمز, __u64 arg)
{
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
	اگر (bitmap->description)
		com_err(0, bitmap->بنیاد_error_رمز+رمز,
			"#%llu for %s", (غیرعلامتی طویل طویل) arg,
			bitmap->description);
	ورنہ
		com_err(0, bitmap->بنیاد_error_رمز + رمز, "#%llu",
			(غیرعلامتی طویل طویل) arg);
#اگر_ختم
}

#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
#تعریف_کرو INC_STAT(map, name) map->stats.name
#ورنہ
#تعریف_کرو INC_STAT(map, name) ;;
#اگر_ختم


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحیح type, __u64 آغاز, __u64 انتہا,
				    __u64 real_انتہا,
				    ثابت حرف *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	ساخت ext2_bitmap_ops	*ops;
	ext2_ino_t عدد_dirs;
	errcode_t retval;

	اگر (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	انتخاب (type) {
	حالت EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		توڑو;
	حالت EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		توڑو;
	حالت EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_عدد_dirs(fs, &عدد_dirs);
		اگر (retval || عدد_dirs > (fs->super->s_inodes_گنتی / 320))
			ops = &ext2fs_blkmap64_bitarray;
		ورنہ
			ops = &ext2fs_blkmap64_rbtree;
		توڑو;
	طےشدہ:
		واپس EINVAL;
	}

	retval = ext2fs_get_memzero(حجم(ساخت ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	اگر (retval)
		واپس retval;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS
	اگر (gettimeofday(&bitmap->stats.created,
			 (ساخت timezone *) تہی) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		واپس 1;
	}
	bitmap->stats.type = type;
#اگر_ختم

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->آغاز = آغاز;
	bitmap->انتہا = انتہا;
	bitmap->real_انتہا = real_انتہا;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_بٹ = 0;
	انتخاب (magic) {
	حالت EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_INODE_MARK;
		توڑو;
	حالت EXT2_ET_MAGIC_بلاک_BITMAP64:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_بلاک_MARK;
		bitmap->cluster_بٹ = fs->cluster_تناسب_بٹ;
		توڑو;
	طےشدہ:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_GENERIC_MARK;
	}
	اگر (descr) {
		retval = ext2fs_get_mem(لڑی_لمبائی(descr)+1, &bitmap->description);
		اگر (retval) {
			ext2fs_free_mem(&bitmap);
			واپس retval;
		}
		strcpy(bitmap->description, descr);
	} ورنہ
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	اگر (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		واپس retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	واپس 0;
}

#اگر_تعریف_شدہ ENABLE_BMAP_STATS
ساکن خالی ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	ساخت ext2_bmap_statistics *stats = &bitmap->stats;
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	اعشاری mark_seq_perc = 0.0, test_seq_perc = 0.0;
	اعشاری mark_عقب_perc = 0.0, test_عقب_perc = 0.0;
	ساخت timeval now;
	دوہرا inuse;

	اگر (stats->test_گنتی) {
		test_seq_perc = ((اعشاری)stats->test_seq /
				 stats->test_گنتی) * 100;
		test_عقب_perc = ((اعشاری)stats->test_عقب /
				  stats->test_گنتی) * 100;
	}

	اگر (stats->mark_گنتی) {
		mark_seq_perc = ((اعشاری)stats->mark_seq /
				 stats->mark_گنتی) * 100;
		mark_عقب_perc = ((اعشاری)stats->mark_عقب /
				  stats->mark_گنتی) * 100;
	}

	اگر (gettimeofday(&now, (ساخت timezone *) تہی) == -1) {
		perror("gettimeofday");
		واپس;
	}

	inuse = (دوہرا) now.tv_sec + \
		(((دوہرا) now.tv_usec) * 0.000001);
	inuse -= (دوہرا) stats->created.tv_sec + \
		(((دوہرا) stats->created.tv_usec) * 0.000001);
#اگر_ختم /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_انتہا - bitmap->آغاز);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_گنتی, stats->resize_گنتی);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_گنتی, stats->unmark_گنتی);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_گنتی, stats->mark_ext_گنتی);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_گنتی, stats->test_ext_گنتی);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->سیٹ_range_گنتی, stats->get_range_گنتی);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_گنتی, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_عقب, test_عقب_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_عقب, mark_عقب_perc, inuse);
#اگر_ختم /* ENABLE_BMAP_STATS_OPS */
}
#اگر_ختم

خالی ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		واپس;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		واپس;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS
	اگر (ext2fs_محفوظ_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#اگر_ختم

	bmap->bitmap_ops->free_bmap(bmap);

	اگر (bmap->description) {
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

	اگر (!src)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(src))
		واپس ext2fs_copy_generic_bitmap(gen_src, dest);

	اگر (!EXT2FS_IS_64_BITMAP(src))
		واپس EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(حجم(ساخت ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	اگر (retval)
		واپس retval;


#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	src->stats.copy_گنتی++;
#اگر_ختم
#اگر_تعریف_شدہ ENABLE_BMAP_STATS
	اگر (gettimeofday(&new_bmap->stats.created,
			 (ساخت timezone *) تہی) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		واپس 1;
	}
	new_bmap->stats.type = src->stats.type;
#اگر_ختم

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->آغاز = src->آغاز;
	new_bmap->انتہا = src->انتہا;
	new_bmap->real_انتہا = src->real_انتہا;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->بنیاد_error_رمز = src->بنیاد_error_رمز;
	new_bmap->cluster_بٹ = src->cluster_بٹ;

	descr = src->description;
	اگر (descr) {
		retval = ext2fs_get_mem(لڑی_لمبائی(descr)+10, &new_descr);
		اگر (retval) {
			ext2fs_free_mem(&new_bmap);
			واپس retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	اگر (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		واپس retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	واپس 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_انتہا,
				     __u64 new_real_انتہا)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap))
		واپس ext2fs_resize_generic_bitmap(gen_bmap->magic, new_انتہا,
						    new_real_انتہا, gen_bmap);

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس EINVAL;

	INC_STAT(bmap, resize_گنتی);

	واپس bmap->bitmap_ops->resize_bmap(bmap, new_انتہا, new_real_انتہا);
}

errcode_t ext2fs_fudge_generic_bmap_انتہا(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 انتہا, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحیح retval;

		retval = ext2fs_fudge_generic_bitmap_انتہا(gen_bitmap,
							 bitmap->magic,
							 neq, انتہا, &tmp_oend);
		اگر (oend)
			*oend = tmp_oend;
		واپس retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس EINVAL;

	اگر (انتہا > bitmap->real_انتہا)
		واپس neq;
	اگر (oend)
		*oend = bitmap->انتہا;
	bitmap->انتہا = انتہا;
	واپس 0;
}

__u64 ext2fs_get_generic_bmap_آغاز(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		واپس ext2fs_get_generic_bitmap_آغاز(gen_bitmap);

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس EINVAL;

	واپس bitmap->آغاز;
}

__u64 ext2fs_get_generic_bmap_انتہا(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		واپس ext2fs_get_generic_bitmap_انتہا(gen_bitmap);

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس EINVAL;

	واپس bitmap->انتہا;
}

خالی ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	ورنہ
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

صحیح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		واپس 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			واپس 0;
		}
		واپس ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس 0;

	arg >>= bitmap->cluster_بٹ;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	اگر (arg == bitmap->stats.آخری_marked + 1)
		bitmap->stats.mark_seq++;
	اگر (arg < bitmap->stats.آخری_marked)
		bitmap->stats.mark_عقب++;
	bitmap->stats.آخری_marked = arg;
	bitmap->stats.mark_گنتی++;
#اگر_ختم

	اگر ((arg < bitmap->آغاز) || (arg > bitmap->انتہا)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		واپس 0;
	}

	واپس bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

صحیح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		واپس 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			واپس 0;
		}
		واپس ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس 0;

	arg >>= bitmap->cluster_بٹ;

	INC_STAT(bitmap, unmark_گنتی);

	اگر ((arg < bitmap->آغاز) || (arg > bitmap->انتہا)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		واپس 0;
	}

	واپس bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

صحیح ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	اگر (!bitmap)
		واپس 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			واپس 0;
		}
		واپس ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس 0;

	arg >>= bitmap->cluster_بٹ;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_گنتی++;
	اگر (arg == bitmap->stats.آخری_tested + 1)
		bitmap->stats.test_seq++;
	اگر (arg < bitmap->stats.آخری_tested)
		bitmap->stats.test_عقب++;
	bitmap->stats.آخری_tested = arg;
#اگر_ختم

	اگر ((arg < bitmap->آغاز) || (arg > bitmap->انتہا)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		واپس 0;
	}

	واپس bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_سیٹ_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((آغاز+عدد-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			واپس EINVAL;
		}
		واپس ext2fs_سیٹ_generic_bitmap_range(gen_bmap, bmap->magic,
						       آغاز, عدد, اندر);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس EINVAL;

	INC_STAT(bmap, سیٹ_range_گنتی);

	واپس bmap->bitmap_ops->سیٹ_bmap_range(bmap, آغاز, عدد, اندر);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((آغاز+عدد-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			واپس EINVAL;
		}
		واپس ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       آغاز, عدد, برآمد);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس EINVAL;

	INC_STAT(bmap, get_range_گنتی);

	واپس bmap->bitmap_ops->get_bmap_range(bmap, آغاز, عدد, برآمد);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	اگر (!bm1 || !bm2)
		واپس EINVAL;
	اگر (bm1->magic != bm2->magic)
		واپس EINVAL;

	/* Now we know both bitmaps have the same magic */
	اگر (EXT2FS_IS_32_BITMAP(bm1))
		واپس ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	اگر (!EXT2FS_IS_64_BITMAP(bm1))
		واپس EINVAL;

	اگر ((bm1->آغاز != bm2->آغاز) ||
	    (bm1->انتہا != bm2->انتہا))
		واپس neq;

	برائے (i = bm1->آغاز; i < bm1->انتہا; i++) {
		صحیح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		اگر (ret1 != ret2) {
			واپس neq;
		}
	}

	واپس 0;
}

خالی ext2fs_سیٹ_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	آغاز, عدد;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_سیٹ_generic_bitmap_padding(gen_bmap);
		واپس;
	}

	آغاز = bmap->انتہا + 1;
	عدد = bmap->real_انتہا - bmap->انتہا;
	bmap->bitmap_ops->mark_bmap_extent(bmap, آغاز, عدد);
	/* XXX ought to warn on error */
}

صحیح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				    blk64_t بلاک, غیرعلامتی صحیح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتہا = بلاک + عدد;

	اگر (!bmap)
		واپس EINVAL;

	اگر (عدد == 1)
		واپس !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, بلاک);

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			واپس EINVAL;
		}
		واپس ext2fs_test_بلاک_bitmap_range(
			(ext2fs_generic_bitmap) bmap, بلاک, عدد);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس EINVAL;

	INC_STAT(bmap, test_ext_گنتی);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بٹ;
	انتہا += (1ULL << bmap->cluster_بٹ) - 1;
	انتہا >>= bmap->cluster_بٹ;
	عدد = انتہا - بلاک;

	اگر ((بلاک < bmap->آغاز) || (بلاک > bmap->انتہا) ||
	    (بلاک+عدد-1 > bmap->انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_TEST, بلاک,
				   bmap->description);
		واپس EINVAL;
	}

	واپس bmap->bitmap_ops->test_clear_bmap_extent(bmap, بلاک, عدد);
}

خالی ext2fs_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				     blk64_t بلاک, غیرعلامتی صحیح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتہا = بلاک + عدد;

	اگر (!bmap)
		واپس;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			واپس;
		}
		ext2fs_mark_بلاک_bitmap_range((ext2fs_generic_bitmap) bmap,
					       بلاک, عدد);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس;

	INC_STAT(bmap, mark_ext_گنتی);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بٹ;
	انتہا += (1ULL << bmap->cluster_بٹ) - 1;
	انتہا >>= bmap->cluster_بٹ;
	عدد = انتہا - بلاک;

	اگر ((بلاک < bmap->آغاز) || (بلاک > bmap->انتہا) ||
	    (بلاک+عدد-1 > bmap->انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_MARK, بلاک,
				   bmap->description);
		واپس;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, بلاک, عدد);
}

خالی ext2fs_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap gen_bmap,
				       blk64_t بلاک, غیرعلامتی صحیح عدد)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	انتہا = بلاک + عدد;

	اگر (!bmap)
		واپس;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((بلاک & ~0xffffffffULL) ||
		    ((بلاک+عدد-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			واپس;
		}
		ext2fs_unmark_بلاک_bitmap_range((ext2fs_generic_bitmap) bmap,
						 بلاک, عدد);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس;

	INC_STAT(bmap, unmark_ext_گنتی);

	/* convert to clusters if necessary */
	بلاک >>= bmap->cluster_بٹ;
	انتہا += (1ULL << bmap->cluster_بٹ) - 1;
	انتہا >>= bmap->cluster_بٹ;
	عدد = انتہا - بلاک;

	اگر ((بلاک < bmap->آغاز) || (بلاک > bmap->انتہا) ||
	    (بلاک+عدد-1 > bmap->انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_UNMARK, بلاک,
				   bmap->description);
		واپس;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, بلاک, عدد);
}

خالی ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت حرف *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
	اگر (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	ورنہ
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#اگر_ختم
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_بلاک_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_بلاک_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, اگلا, b_انتہا, c_انتہا;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	اگر (fs->cluster_تناسب_بٹ == ext2fs_get_bitmap_granularity(gen_bmap))
		واپس 0;	/* Nothing to do */

	retval = ext2fs_allocate_بلاک_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	اگر (retval)
		واپس retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->آغاز;
	b_انتہا = bmap->انتہا;
	bmap->انتہا = bmap->real_انتہا;
	c_انتہا = cmap->انتہا;
	cmap->انتہا = cmap->real_انتہا;
	جب (i < bmap->real_انتہا) {
		retval = ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(gen_bmap,
						i, bmap->real_انتہا, &اگلا);
		اگر (retval)
			توڑو;
		ext2fs_mark_بلاک_bitmap2(gen_cmap, اگلا);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, اگلا) + 1);
	}
	bmap->انتہا = b_انتہا;
	cmap->انتہا = c_انتہا;
	ext2fs_free_بلاک_bitmap(gen_bmap);
	*bitmap = (ext2fs_بلاک_bitmap) cmap;
	واپس 0;
}

errcode_t ext2fs_ڈھونڈو_پہلا_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	اگر (!bitmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		اگر (((آغاز) & ~0xffffffffULL) ||
		    ((انتہا) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, آغاز);
			واپس EINVAL;
		}

		retval = ext2fs_ڈھونڈو_پہلا_zero_generic_bitmap(bitmap, آغاز,
							       انتہا, &blk);
		اگر (retval == 0)
			*برآمد = blk;
		واپس retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس EINVAL;

	cstart = آغاز >> bmap64->cluster_بٹ;
	cend = انتہا >> bmap64->cluster_بٹ;

	اگر (cstart < bmap64->آغاز || cend > bmap64->انتہا || آغاز > انتہا) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, آغاز);
		واپس EINVAL;
	}

	اگر (bmap64->bitmap_ops->ڈھونڈو_پہلا_zero) {
		retval = bmap64->bitmap_ops->ڈھونڈو_پہلا_zero(bmap64, cstart,
							     cend, &cout);
		اگر (retval)
			واپس retval;
	ملا:
		cout <<= bmap64->cluster_بٹ;
		*برآمد = (cout >= آغاز) ? cout : آغاز;
		واپس 0;
	}

	برائے (cout = cstart; cout <= cend; cout++)
		اگر (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			جاؤ ملا;

	واپس ENOENT;
}

errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	اگر (!bitmap)
		واپس EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		اگر (((آغاز) & ~0xffffffffULL) ||
		    ((انتہا) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, آغاز);
			واپس EINVAL;
		}

		retval = ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bitmap(bitmap, آغاز,
							      انتہا, &blk);
		اگر (retval == 0)
			*برآمد = blk;
		واپس retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		واپس EINVAL;

	cstart = آغاز >> bmap64->cluster_بٹ;
	cend = انتہا >> bmap64->cluster_بٹ;

	اگر (cstart < bmap64->آغاز || cend > bmap64->انتہا || آغاز > انتہا) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, آغاز);
		واپس EINVAL;
	}

	اگر (bmap64->bitmap_ops->ڈھونڈو_پہلا_سیٹ) {
		retval = bmap64->bitmap_ops->ڈھونڈو_پہلا_سیٹ(bmap64, cstart,
							    cend, &cout);
		اگر (retval)
			واپس retval;
	ملا:
		cout <<= bmap64->cluster_بٹ;
		*برآمد = (cout >= آغاز) ? cout : آغاز;
		واپس 0;
	}

	برائے (cout = cstart; cout <= cend; cout++)
		اگر (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			جاؤ ملا;

	واپس ENOENT;
}

errcode_t ext2fs_گنتی_مستعمل_بلاکیں(ext2_filsys fs, blk64_t آغاز,
				   blk64_t انتہا, blk64_t *برآمد)
{
	blk64_t		اگلا;
	blk64_t		tot_سیٹ = 0;
	errcode_t	retval = 0;

	جب (آغاز < انتہا) {
		retval = ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(fs->بلاک_map,
							آغاز, انتہا, &اگلا);
		اگر (retval) {
			اگر (retval == ENOENT)
				retval = 0;
			توڑو;
		}
		آغاز = اگلا;

		retval = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(fs->بلاک_map,
							آغاز, انتہا, &اگلا);
		اگر (retval == 0) {
			tot_سیٹ += اگلا - آغاز;
			آغاز  = اگلا + 1;
		} ورنہ اگر (retval == ENOENT) {
			retval = 0;
			tot_سیٹ += انتہا - آغاز + 1;
			توڑو;
		} ورنہ
			توڑو;
	}

	اگر (!retval)
		*برآمد = tot_سیٹ;
	واپس retval;
}

errcode_t ext2fs_گنتی_مستعمل_clusters(ext2_filsys fs, blk64_t آغاز,
				     blk64_t انتہا, blk64_t *برآمد)
{
	blk64_t		tot_سیٹ = 0;
	errcode_t	retval = 0;

	retval = ext2fs_گنتی_مستعمل_بلاکیں(fs, آغاز, انتہا, &tot_سیٹ);

	اگر (!retval)
		*برآمد = EXT2_FS_عدد_B2_C(fs, tot_سیٹ);
	واپس retval;
}
