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
#endif
#شامل <fcntl.h>
#شامل <time.h>
#شامل <errno.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#شامل <sys/time.h>
#endif

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

ایستا تهی warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			صحیح code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	اگر (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	وگرنه
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#وگرنه
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    صحیح type, __u64 start, __u64 end,
				    __u64 real_end,
				    ثابت نویسه *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	ساختار ext2_bitmap_ops	*ops;
	ext2_ino_t رقم_dirs;
	errcode_t retval;

	اگر (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	انتخاب (type) {
	حالت EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		بشکن;
	حالت EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		بشکن;
	حالت EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_رقم_dirs(fs, &رقم_dirs);
		اگر (retval || رقم_dirs > (fs->super->s_inodes_شمار / 320))
			ops = &ext2fs_blkmap64_bitarray;
		وگرنه
			ops = &ext2fs_blkmap64_rbtree;
		بشکن;
	پیشفرض:
		بازگشت EINVAL;
	}

	retval = ext2fs_get_memzero(اندازه(ساختار ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	اگر (retval)
		بازگشت retval;

#ifdef ENABLE_BMAP_STATS
	اگر (gettimeofday(&bitmap->stats.created,
			 (ساختار timezone *) پوچ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		بازگشت 1;
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
	انتخاب (magic) {
	حالت EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		بشکن;
	حالت EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		بشکن;
	پیشفرض:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	اگر (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		اگر (retval) {
			ext2fs_free_mem(&bitmap);
			بازگشت retval;
		}
		strcpy(bitmap->description, descr);
	} وگرنه
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	اگر (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		بازگشت retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	بازگشت 0;
}

#ifdef ENABLE_BMAP_STATS
ایستا تهی ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	ساختار ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	اعشاری mark_seq_perc = 0.0, test_seq_perc = 0.0;
	اعشاری mark_back_perc = 0.0, test_back_perc = 0.0;
	ساختار timeval now;
	دوگانه inuse;

	اگر (stats->test_شمار) {
		test_seq_perc = ((اعشاری)stats->test_seq /
				 stats->test_شمار) * 100;
		test_back_perc = ((اعشاری)stats->test_back /
				  stats->test_شمار) * 100;
	}

	اگر (stats->mark_شمار) {
		mark_seq_perc = ((اعشاری)stats->mark_seq /
				 stats->mark_شمار) * 100;
		mark_back_perc = ((اعشاری)stats->mark_back /
				  stats->mark_شمار) * 100;
	}

	اگر (gettimeofday(&now, (ساختار timezone *) پوچ) == -1) {
		perror("gettimeofday");
		بازگشت;
	}

	inuse = (دوگانه) now.tv_sec + \
		(((دوگانه) now.tv_usec) * 0.000001);
	inuse -= (دوگانه) stats->created.tv_sec + \
		(((دوگانه) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
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
		stats->set_range_شمار, stats->get_range_شمار);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_شمار, stats->test_seq, test_seq_perc);
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

تهی ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		بازگشت;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		بازگشت;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت;

#ifdef ENABLE_BMAP_STATS
	اگر (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

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
	نویسه *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	اگر (!src)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(src))
		بازگشت ext2fs_copy_generic_bitmap(gen_src, dest);

	اگر (!EXT2FS_IS_64_BITMAP(src))
		بازگشت EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(اندازه(ساختار ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	اگر (retval)
		بازگشت retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_شمار++;
#endif
#ifdef ENABLE_BMAP_STATS
	اگر (gettimeofday(&new_bmap->stats.created,
			 (ساختار timezone *) پوچ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		بازگشت 1;
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
	اگر (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		اگر (retval) {
			ext2fs_free_mem(&new_bmap);
			بازگشت retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	اگر (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		بازگشت retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	بازگشت 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap))
		بازگشت ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت EINVAL;

	INC_STAT(bmap, resize_شمار);

	بازگشت bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		صحیح retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		اگر (oend)
			*oend = tmp_oend;
		بازگشت retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت EINVAL;

	اگر (end > bitmap->real_end)
		بازگشت neq;
	اگر (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	بازگشت 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		بازگشت ext2fs_get_generic_bitmap_start(gen_bitmap);

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت EINVAL;

	بازگشت bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		بازگشت ext2fs_get_generic_bitmap_end(gen_bitmap);

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت EINVAL;

	بازگشت bitmap->end;
}

تهی ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	وگرنه
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

صحیح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		بازگشت 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			بازگشت 0;
		}
		بازگشت ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	اگر (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	اگر (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_شمار++;
#endif

	اگر ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		بازگشت 0;
	}

	بازگشت bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

صحیح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	اگر (!bitmap)
		بازگشت 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			بازگشت 0;
		}
		بازگشت ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_شمار);

	اگر ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		بازگشت 0;
	}

	بازگشت bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

صحیح ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	اگر (!bitmap)
		بازگشت 0;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			بازگشت 0;
		}
		بازگشت ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_شمار++;
	اگر (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	اگر (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	اگر ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		بازگشت 0;
	}

	بازگشت bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned صحیح رقم,
					تهی *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((start+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			بازگشت EINVAL;
		}
		بازگشت ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, رقم, in);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت EINVAL;

	INC_STAT(bmap, set_range_شمار);

	بازگشت bmap->bitmap_ops->set_bmap_range(bmap, start, رقم, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned صحیح رقم,
					تهی *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	اگر (!bmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((start+رقم-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			بازگشت EINVAL;
		}
		بازگشت ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, رقم, out);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت EINVAL;

	INC_STAT(bmap, get_range_شمار);

	بازگشت bmap->bitmap_ops->get_bmap_range(bmap, start, رقم, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	اگر (!bm1 || !bm2)
		بازگشت EINVAL;
	اگر (bm1->magic != bm2->magic)
		بازگشت EINVAL;

	/* Now we know both bitmaps have the same magic */
	اگر (EXT2FS_IS_32_BITMAP(bm1))
		بازگشت ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	اگر (!EXT2FS_IS_64_BITMAP(bm1))
		بازگشت EINVAL;

	اگر ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		بازگشت neq;

	برای (i = bm1->start; i < bm1->end; i++) {
		صحیح ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		اگر (ret1 != ret2) {
			بازگشت neq;
		}
	}

	بازگشت 0;
}

تهی ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, رقم;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		بازگشت;
	}

	start = bmap->end + 1;
	رقم = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, رقم);
	/* XXX ought to warn on error */
}

صحیح ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned صحیح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	اگر (!bmap)
		بازگشت EINVAL;

	اگر (رقم == 1)
		بازگشت !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			بازگشت EINVAL;
		}
		بازگشت ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, رقم);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت EINVAL;

	INC_STAT(bmap, test_ext_شمار);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	اگر ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		بازگشت EINVAL;
	}

	بازگشت bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, رقم);
}

تهی ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned صحیح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	اگر (!bmap)
		بازگشت;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			بازگشت;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, رقم);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت;

	INC_STAT(bmap, mark_ext_شمار);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	اگر ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		بازگشت;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, رقم);
}

تهی ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned صحیح رقم)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + رقم;

	اگر (!bmap)
		بازگشت;

	اگر (EXT2FS_IS_32_BITMAP(bmap)) {
		اگر ((block & ~0xffffffffULL) ||
		    ((block+رقم-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			بازگشت;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, رقم);
	}

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		بازگشت;

	INC_STAT(bmap, unmark_ext_شمار);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	رقم = end - block;

	اگر ((block < bmap->start) || (block > bmap->end) ||
	    (block+رقم-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		بازگشت;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, رقم);
}

تهی ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ثابت نویسه *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	اگر (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	وگرنه
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
	اگر (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		بازگشت 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	اگر (retval)
		بازگشت retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	وقتی (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		اگر (retval)
			بشکن;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	بازگشت 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	اگر (!bitmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		اگر (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			بازگشت EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		اگر (retval == 0)
			*out = blk;
		بازگشت retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	اگر (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		بازگشت EINVAL;
	}

	اگر (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		اگر (retval)
			بازگشت retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		بازگشت 0;
	}

	برای (cout = cstart; cout <= cend; cout++)
		اگر (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			برو found;

	بازگشت ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	اگر (!bitmap)
		بازگشت EINVAL;

	اگر (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		اگر (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			بازگشت EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		اگر (retval == 0)
			*out = blk;
		بازگشت retval;
	}

	اگر (!EXT2FS_IS_64_BITMAP(bitmap))
		بازگشت EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	اگر (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		بازگشت EINVAL;
	}

	اگر (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		اگر (retval)
			بازگشت retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		بازگشت 0;
	}

	برای (cout = cstart; cout <= cend; cout++)
		اگر (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			برو found;

	بازگشت ENOENT;
}

errcode_t ext2fs_شمار_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	وقتی (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		اگر (retval) {
			اگر (retval == ENOENT)
				retval = 0;
			بشکن;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		اگر (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} وگرنه اگر (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			بشکن;
		} وگرنه
			بشکن;
	}

	اگر (!retval)
		*out = tot_set;
	بازگشت retval;
}

errcode_t ext2fs_شمار_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_شمار_used_blocks(fs, start, end, &tot_set);

	اگر (!retval)
		*out = EXT2_FS_رقم_B2_C(fs, tot_set);
	بازگشت retval;
}
