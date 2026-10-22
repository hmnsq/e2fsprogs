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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <errno.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_సమయం_H
#అన్తర్భావ <sys/time.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "bmap64.h"

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

స్థిర శూన్య warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			పూర్ణాంకం సంకేతం_2, __u64 arg)
{
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
	ఒకవేళ (bitmap->description)
		com_err(0, bitmap->పునాది_error_సంకేతం+సంకేతం_2,
			"#%llu for %s", (గుర్తులేని దీర్ఘ దీర్ఘ) arg,
			bitmap->description);
	లేకపోతే
		com_err(0, bitmap->పునాది_error_సంకేతం + సంకేతం_2, "#%llu",
			(గుర్తులేని దీర్ఘ దీర్ఘ) arg);
#అయితే_ముగింపు
}

#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
#నిర్వచించు INC_STAT(map, name) map->stats.name
#లేకపోతే
#నిర్వచించు INC_STAT(map, name) ;;
#అయితే_ముగింపు


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    పూర్ణాంకం type, __u64 ప్రారంభం, __u64 చివర,
				    __u64 real_చివర,
				    స్థిరరాశి అక్షరం *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	నిర్మాణం ext2_bitmap_ops	*ops;
	ext2_ino_t అంకె_dirs;
	errcode_t retval;

	ఒకవేళ (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	ఎంపిక (type) {
	స్థితి EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		ఆపు;
	స్థితి EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		ఆపు;
	స్థితి EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_అంకె_dirs(fs, &అంకె_dirs);
		ఒకవేళ (retval || అంకె_dirs > (fs->super->s_inodes_లెక్క / 320))
			ops = &ext2fs_blkmap64_bitarray;
		లేకపోతే
			ops = &ext2fs_blkmap64_rbtree;
		ఆపు;
	అప్రమేయ:
		తిరిగి EINVAL;
	}

	retval = ext2fs_get_memzero(పరిమాణం(నిర్మాణం ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	ఒకవేళ (retval)
		తిరిగి retval;

#నిర్వచితమైతే ENABLE_BMAP_STATS
	ఒకవేళ (gettimeofday(&bitmap->stats.created,
			 (నిర్మాణం timezone *) ఖాళీ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		తిరిగి 1;
	}
	bitmap->stats.type = type;
#అయితే_ముగింపు

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ప్రారంభం = ప్రారంభం;
	bitmap->చివర = చివర;
	bitmap->real_చివర = real_చివర;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_బిట్లు = 0;
	ఎంపిక (magic) {
	స్థితి EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_INODE_MARK;
		ఆపు;
	స్థితి EXT2_ET_MAGIC_ఖండం_BITMAP64:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_ఖండం_MARK;
		bitmap->cluster_బిట్లు = fs->cluster_నిష్పత్తి_బిట్లు;
		ఆపు;
	అప్రమేయ:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_GENERIC_MARK;
	}
	ఒకవేళ (descr) {
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(descr)+1, &bitmap->description);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&bitmap);
			తిరిగి retval;
		}
		strcpy(bitmap->description, descr);
	} లేకపోతే
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		తిరిగి retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	తిరిగి 0;
}

#నిర్వచితమైతే ENABLE_BMAP_STATS
స్థిర శూన్య ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	నిర్మాణం ext2_bmap_statistics *stats = &bitmap->stats;
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	దశాంశం mark_seq_perc = 0.0, test_seq_perc = 0.0;
	దశాంశం mark_వెనుక_perc = 0.0, test_వెనుక_perc = 0.0;
	నిర్మాణం timeval now;
	ద్విగుణ inuse;

	ఒకవేళ (stats->test_లెక్క) {
		test_seq_perc = ((దశాంశం)stats->test_seq /
				 stats->test_లెక్క) * 100;
		test_వెనుక_perc = ((దశాంశం)stats->test_వెనుక /
				  stats->test_లెక్క) * 100;
	}

	ఒకవేళ (stats->mark_లెక్క) {
		mark_seq_perc = ((దశాంశం)stats->mark_seq /
				 stats->mark_లెక్క) * 100;
		mark_వెనుక_perc = ((దశాంశం)stats->mark_వెనుక /
				  stats->mark_లెక్క) * 100;
	}

	ఒకవేళ (gettimeofday(&now, (నిర్మాణం timezone *) ఖాళీ) == -1) {
		perror("gettimeofday");
		తిరిగి;
	}

	inuse = (ద్విగుణ) now.tv_sec + \
		(((ద్విగుణ) now.tv_usec) * 0.000001);
	inuse -= (ద్విగుణ) stats->created.tv_sec + \
		(((ద్విగుణ) stats->created.tv_usec) * 0.000001);
#అయితే_ముగింపు /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_చివర - bitmap->ప్రారంభం);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_లెక్క, stats->resize_లెక్క);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_లెక్క, stats->unmark_లెక్క);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_లెక్క, stats->mark_ext_లెక్క);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_లెక్క, stats->test_ext_లెక్క);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->సమితి_range_లెక్క, stats->get_range_లెక్క);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_లెక్క, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_వెనుక, test_వెనుక_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_వెనుక, mark_వెనుక_perc, inuse);
#అయితే_ముగింపు /* ENABLE_BMAP_STATS_OPS */
}
#అయితే_ముగింపు

శూన్య ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ఒకవేళ (!bmap)
		తిరిగి;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		తిరిగి;
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి;

#నిర్వచితమైతే ENABLE_BMAP_STATS
	ఒకవేళ (ext2fs_సురక్షిత_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#అయితే_ముగింపు

	bmap->bitmap_ops->free_bmap(bmap);

	ఒకవేళ (bmap->description) {
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
	అక్షరం *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	ఒకవేళ (!src)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(src))
		తిరిగి ext2fs_copy_generic_bitmap(gen_src, dest);

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(src))
		తిరిగి EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(పరిమాణం(నిర్మాణం ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	ఒకవేళ (retval)
		తిరిగి retval;


#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	src->stats.copy_లెక్క++;
#అయితే_ముగింపు
#నిర్వచితమైతే ENABLE_BMAP_STATS
	ఒకవేళ (gettimeofday(&new_bmap->stats.created,
			 (నిర్మాణం timezone *) ఖాళీ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		తిరిగి 1;
	}
	new_bmap->stats.type = src->stats.type;
#అయితే_ముగింపు

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->ప్రారంభం = src->ప్రారంభం;
	new_bmap->చివర = src->చివర;
	new_bmap->real_చివర = src->real_చివర;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->పునాది_error_సంకేతం = src->పునాది_error_సంకేతం;
	new_bmap->cluster_బిట్లు = src->cluster_బిట్లు;

	descr = src->description;
	ఒకవేళ (descr) {
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(descr)+10, &new_descr);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&new_bmap);
			తిరిగి retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		తిరిగి retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	తిరిగి 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_చివర,
				     __u64 new_real_చివర)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ఒకవేళ (!bmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap))
		తిరిగి ext2fs_resize_generic_bitmap(gen_bmap->magic, new_చివర,
						    new_real_చివర, gen_bmap);

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి EINVAL;

	INC_STAT(bmap, resize_లెక్క);

	తిరిగి bmap->bitmap_ops->resize_bmap(bmap, new_చివర, new_real_చివర);
}

errcode_t ext2fs_fudge_generic_bmap_చివర(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 చివర, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (!bitmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		పూర్ణాంకం retval;

		retval = ext2fs_fudge_generic_bitmap_చివర(gen_bitmap,
							 bitmap->magic,
							 neq, చివర, &tmp_oend);
		ఒకవేళ (oend)
			*oend = tmp_oend;
		తిరిగి retval;
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి EINVAL;

	ఒకవేళ (చివర > bitmap->real_చివర)
		తిరిగి neq;
	ఒకవేళ (oend)
		*oend = bitmap->చివర;
	bitmap->చివర = చివర;
	తిరిగి 0;
}

__u64 ext2fs_get_generic_bmap_ప్రారంభం(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (!bitmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap))
		తిరిగి ext2fs_get_generic_bitmap_ప్రారంభం(gen_bitmap);

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి EINVAL;

	తిరిగి bitmap->ప్రారంభం;
}

__u64 ext2fs_get_generic_bmap_చివర(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (!bitmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap))
		తిరిగి ext2fs_get_generic_bitmap_చివర(gen_bitmap);

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి EINVAL;

	తిరిగి bitmap->చివర;
}

శూన్య ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	లేకపోతే
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

పూర్ణాంకం ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (!bitmap)
		తిరిగి 0;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			తిరిగి 0;
		}
		తిరిగి ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి 0;

	arg >>= bitmap->cluster_బిట్లు;

#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	ఒకవేళ (arg == bitmap->stats.చివరి_marked + 1)
		bitmap->stats.mark_seq++;
	ఒకవేళ (arg < bitmap->stats.చివరి_marked)
		bitmap->stats.mark_వెనుక++;
	bitmap->stats.చివరి_marked = arg;
	bitmap->stats.mark_లెక్క++;
#అయితే_ముగింపు

	ఒకవేళ ((arg < bitmap->ప్రారంభం) || (arg > bitmap->చివర)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		తిరిగి 0;
	}

	తిరిగి bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

పూర్ణాంకం ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ఒకవేళ (!bitmap)
		తిరిగి 0;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			తిరిగి 0;
		}
		తిరిగి ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి 0;

	arg >>= bitmap->cluster_బిట్లు;

	INC_STAT(bitmap, unmark_లెక్క);

	ఒకవేళ ((arg < bitmap->ప్రారంభం) || (arg > bitmap->చివర)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		తిరిగి 0;
	}

	తిరిగి bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

పూర్ణాంకం ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	ఒకవేళ (!bitmap)
		తిరిగి 0;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			తిరిగి 0;
		}
		తిరిగి ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి 0;

	arg >>= bitmap->cluster_బిట్లు;

#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_లెక్క++;
	ఒకవేళ (arg == bitmap->stats.చివరి_tested + 1)
		bitmap->stats.test_seq++;
	ఒకవేళ (arg < bitmap->stats.చివరి_tested)
		bitmap->stats.test_వెనుక++;
	bitmap->stats.చివరి_tested = arg;
#అయితే_ముగింపు

	ఒకవేళ ((arg < bitmap->ప్రారంభం) || (arg > bitmap->చివర)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		తిరిగి 0;
	}

	తిరిగి bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_సమితి_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ఒకవేళ (!bmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ఒకవేళ ((ప్రారంభం+అంకె-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			తిరిగి EINVAL;
		}
		తిరిగి ext2fs_సమితి_generic_bitmap_range(gen_bmap, bmap->magic,
						       ప్రారంభం, అంకె, లోపల);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి EINVAL;

	INC_STAT(bmap, సమితి_range_లెక్క);

	తిరిగి bmap->bitmap_ops->సమితి_bmap_range(bmap, ప్రారంభం, అంకె, లోపల);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ఒకవేళ (!bmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ఒకవేళ ((ప్రారంభం+అంకె-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			తిరిగి EINVAL;
		}
		తిరిగి ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       ప్రారంభం, అంకె, బహిర్గతం);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి EINVAL;

	INC_STAT(bmap, get_range_లెక్క);

	తిరిగి bmap->bitmap_ops->get_bmap_range(bmap, ప్రారంభం, అంకె, బహిర్గతం);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	ఒకవేళ (!bm1 || !bm2)
		తిరిగి EINVAL;
	ఒకవేళ (bm1->magic != bm2->magic)
		తిరిగి EINVAL;

	/* Now we know both bitmaps have the same magic */
	ఒకవేళ (EXT2FS_IS_32_BITMAP(bm1))
		తిరిగి ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bm1))
		తిరిగి EINVAL;

	ఒకవేళ ((bm1->ప్రారంభం != bm2->ప్రారంభం) ||
	    (bm1->చివర != bm2->చివర))
		తిరిగి neq;

	కొరకు (i = bm1->ప్రారంభం; i < bm1->చివర; i++) {
		పూర్ణాంకం ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		ఒకవేళ (ret1 != ret2) {
			తిరిగి neq;
		}
	}

	తిరిగి 0;
}

శూన్య ext2fs_సమితి_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ప్రారంభం, అంకె;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_సమితి_generic_bitmap_padding(gen_bmap);
		తిరిగి;
	}

	ప్రారంభం = bmap->చివర + 1;
	అంకె = bmap->real_చివర - bmap->చివర;
	bmap->bitmap_ops->mark_bmap_extent(bmap, ప్రారంభం, అంకె);
	/* XXX ought to warn on error */
}

పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap gen_bmap,
				    blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	చివర = ఖండం + అంకె;

	ఒకవేళ (!bmap)
		తిరిగి EINVAL;

	ఒకవేళ (అంకె == 1)
		తిరిగి !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, ఖండం);

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ఒకవేళ ((ఖండం & ~0xffffffffULL) ||
		    ((ఖండం+అంకె-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			తిరిగి EINVAL;
		}
		తిరిగి ext2fs_test_ఖండం_bitmap_range(
			(ext2fs_generic_bitmap) bmap, ఖండం, అంకె);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి EINVAL;

	INC_STAT(bmap, test_ext_లెక్క);

	/* convert to clusters if necessary */
	ఖండం >>= bmap->cluster_బిట్లు;
	చివర += (1ULL << bmap->cluster_బిట్లు) - 1;
	చివర >>= bmap->cluster_బిట్లు;
	అంకె = చివర - ఖండం;

	ఒకవేళ ((ఖండం < bmap->ప్రారంభం) || (ఖండం > bmap->చివర) ||
	    (ఖండం+అంకె-1 > bmap->చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_TEST, ఖండం,
				   bmap->description);
		తిరిగి EINVAL;
	}

	తిరిగి bmap->bitmap_ops->test_clear_bmap_extent(bmap, ఖండం, అంకె);
}

శూన్య ext2fs_mark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap gen_bmap,
				     blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	చివర = ఖండం + అంకె;

	ఒకవేళ (!bmap)
		తిరిగి;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ఒకవేళ ((ఖండం & ~0xffffffffULL) ||
		    ((ఖండం+అంకె-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			తిరిగి;
		}
		ext2fs_mark_ఖండం_bitmap_range((ext2fs_generic_bitmap) bmap,
					       ఖండం, అంకె);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి;

	INC_STAT(bmap, mark_ext_లెక్క);

	/* convert to clusters if necessary */
	ఖండం >>= bmap->cluster_బిట్లు;
	చివర += (1ULL << bmap->cluster_బిట్లు) - 1;
	చివర >>= bmap->cluster_బిట్లు;
	అంకె = చివర - ఖండం;

	ఒకవేళ ((ఖండం < bmap->ప్రారంభం) || (ఖండం > bmap->చివర) ||
	    (ఖండం+అంకె-1 > bmap->చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_MARK, ఖండం,
				   bmap->description);
		తిరిగి;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, ఖండం, అంకె);
}

శూన్య ext2fs_unmark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap gen_bmap,
				       blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	చివర = ఖండం + అంకె;

	ఒకవేళ (!bmap)
		తిరిగి;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bmap)) {
		ఒకవేళ ((ఖండం & ~0xffffffffULL) ||
		    ((ఖండం+అంకె-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			తిరిగి;
		}
		ext2fs_unmark_ఖండం_bitmap_range((ext2fs_generic_bitmap) bmap,
						 ఖండం, అంకె);
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి;

	INC_STAT(bmap, unmark_ext_లెక్క);

	/* convert to clusters if necessary */
	ఖండం >>= bmap->cluster_బిట్లు;
	చివర += (1ULL << bmap->cluster_బిట్లు) - 1;
	చివర >>= bmap->cluster_బిట్లు;
	అంకె = చివర - ఖండం;

	ఒకవేళ ((ఖండం < bmap->ప్రారంభం) || (ఖండం > bmap->చివర) ||
	    (ఖండం+అంకె-1 > bmap->చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_UNMARK, ఖండం,
				   bmap->description);
		తిరిగి;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, ఖండం, అంకె);
}

శూన్య ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, స్థిరరాశి అక్షరం *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#నిర్వచితం_కాకపోతే OMIT_COM_ERR
	ఒకవేళ (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	లేకపోతే
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#అయితే_ముగింపు
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ఖండం_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_ఖండం_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, తదుపరి, b_చివర, c_చివర;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	ఒకవేళ (fs->cluster_నిష్పత్తి_బిట్లు == ext2fs_get_bitmap_granularity(gen_bmap))
		తిరిగి 0;	/* Nothing to do */

	retval = ext2fs_allocate_ఖండం_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	ఒకవేళ (retval)
		తిరిగి retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->ప్రారంభం;
	b_చివర = bmap->చివర;
	bmap->చివర = bmap->real_చివర;
	c_చివర = cmap->చివర;
	cmap->చివర = cmap->real_చివర;
	అయితే (i < bmap->real_చివర) {
		retval = ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(gen_bmap,
						i, bmap->real_చివర, &తదుపరి);
		ఒకవేళ (retval)
			ఆపు;
		ext2fs_mark_ఖండం_bitmap2(gen_cmap, తదుపరి);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, తదుపరి) + 1);
	}
	bmap->చివర = b_చివర;
	cmap->చివర = c_చివర;
	ext2fs_free_ఖండం_bitmap(gen_bmap);
	*bitmap = (ext2fs_ఖండం_bitmap) cmap;
	తిరిగి 0;
}

errcode_t ext2fs_కనుగొను_మొదటి_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ఒకవేళ (!bitmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ఒకవేళ (((ప్రారంభం) & ~0xffffffffULL) ||
		    ((చివర) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ప్రారంభం);
			తిరిగి EINVAL;
		}

		retval = ext2fs_కనుగొను_మొదటి_zero_generic_bitmap(bitmap, ప్రారంభం,
							       చివర, &blk);
		ఒకవేళ (retval == 0)
			*బహిర్గతం = blk;
		తిరిగి retval;
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి EINVAL;

	cstart = ప్రారంభం >> bmap64->cluster_బిట్లు;
	cend = చివర >> bmap64->cluster_బిట్లు;

	ఒకవేళ (cstart < bmap64->ప్రారంభం || cend > bmap64->చివర || ప్రారంభం > చివర) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ప్రారంభం);
		తిరిగి EINVAL;
	}

	ఒకవేళ (bmap64->bitmap_ops->కనుగొను_మొదటి_zero) {
		retval = bmap64->bitmap_ops->కనుగొను_మొదటి_zero(bmap64, cstart,
							     cend, &cout);
		ఒకవేళ (retval)
			తిరిగి retval;
	దొరికింది:
		cout <<= bmap64->cluster_బిట్లు;
		*బహిర్గతం = (cout >= ప్రారంభం) ? cout : ప్రారంభం;
		తిరిగి 0;
	}

	కొరకు (cout = cstart; cout <= cend; cout++)
		ఒకవేళ (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			వెళ్ళు దొరికింది;

	తిరిగి ENOENT;
}

errcode_t ext2fs_కనుగొను_మొదటి_సమితి_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ఒకవేళ (!bitmap)
		తిరిగి EINVAL;

	ఒకవేళ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ఒకవేళ (((ప్రారంభం) & ~0xffffffffULL) ||
		    ((చివర) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ప్రారంభం);
			తిరిగి EINVAL;
		}

		retval = ext2fs_కనుగొను_మొదటి_సమితి_generic_bitmap(bitmap, ప్రారంభం,
							      చివర, &blk);
		ఒకవేళ (retval == 0)
			*బహిర్గతం = blk;
		తిరిగి retval;
	}

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bitmap))
		తిరిగి EINVAL;

	cstart = ప్రారంభం >> bmap64->cluster_బిట్లు;
	cend = చివర >> bmap64->cluster_బిట్లు;

	ఒకవేళ (cstart < bmap64->ప్రారంభం || cend > bmap64->చివర || ప్రారంభం > చివర) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ప్రారంభం);
		తిరిగి EINVAL;
	}

	ఒకవేళ (bmap64->bitmap_ops->కనుగొను_మొదటి_సమితి) {
		retval = bmap64->bitmap_ops->కనుగొను_మొదటి_సమితి(bmap64, cstart,
							    cend, &cout);
		ఒకవేళ (retval)
			తిరిగి retval;
	దొరికింది:
		cout <<= bmap64->cluster_బిట్లు;
		*బహిర్గతం = (cout >= ప్రారంభం) ? cout : ప్రారంభం;
		తిరిగి 0;
	}

	కొరకు (cout = cstart; cout <= cend; cout++)
		ఒకవేళ (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			వెళ్ళు దొరికింది;

	తిరిగి ENOENT;
}

errcode_t ext2fs_లెక్క_వాడిన_ఖండాలు(ext2_filsys fs, blk64_t ప్రారంభం,
				   blk64_t చివర, blk64_t *బహిర్గతం)
{
	blk64_t		తదుపరి;
	blk64_t		tot_సమితి = 0;
	errcode_t	retval = 0;

	అయితే (ప్రారంభం < చివర) {
		retval = ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(fs->ఖండం_map,
							ప్రారంభం, చివర, &తదుపరి);
		ఒకవేళ (retval) {
			ఒకవేళ (retval == ENOENT)
				retval = 0;
			ఆపు;
		}
		ప్రారంభం = తదుపరి;

		retval = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(fs->ఖండం_map,
							ప్రారంభం, చివర, &తదుపరి);
		ఒకవేళ (retval == 0) {
			tot_సమితి += తదుపరి - ప్రారంభం;
			ప్రారంభం  = తదుపరి + 1;
		} లేకపోతే ఒకవేళ (retval == ENOENT) {
			retval = 0;
			tot_సమితి += చివర - ప్రారంభం + 1;
			ఆపు;
		} లేకపోతే
			ఆపు;
	}

	ఒకవేళ (!retval)
		*బహిర్గతం = tot_సమితి;
	తిరిగి retval;
}

errcode_t ext2fs_లెక్క_వాడిన_clusters(ext2_filsys fs, blk64_t ప్రారంభం,
				     blk64_t చివర, blk64_t *బహిర్గతం)
{
	blk64_t		tot_సమితి = 0;
	errcode_t	retval = 0;

	retval = ext2fs_లెక్క_వాడిన_ఖండాలు(fs, ప్రారంభం, చివర, &tot_సమితి);

	ఒకవేళ (!retval)
		*బహిర్గతం = EXT2_FS_అంకె_B2_C(fs, tot_సమితి);
	తిరిగి retval;
}
