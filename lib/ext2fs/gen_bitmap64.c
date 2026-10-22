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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <errno.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#அந்தர்பாவ <sys/time.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "bmap64.h"

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

நிலையான வெறுமை warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			முழு code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	எனில் (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	இல்லையெனில்
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#இல்லையெனில்
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    முழு type, __u64 start, __u64 end,
				    __u64 real_end,
				    மாறிலி எழுத்து *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	கட்டமைப்பு ext2_bitmap_ops	*ops;
	ext2_ino_t இலக்கம்_dirs;
	errcode_t retval;

	எனில் (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	தேர்வு (type) {
	நிலை EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		நிறுத்து;
	நிலை EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		நிறுத்து;
	நிலை EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_இலக்கம்_dirs(fs, &இலக்கம்_dirs);
		எனில் (retval || இலக்கம்_dirs > (fs->super->s_inodes_மொத்தம் / 320))
			ops = &ext2fs_blkmap64_bitarray;
		இல்லையெனில்
			ops = &ext2fs_blkmap64_rbtree;
		நிறுத்து;
	இயல்பு:
		திரும்பு EINVAL;
	}

	retval = ext2fs_get_memzero(அளவு(கட்டமைப்பு ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	எனில் (retval)
		திரும்பு retval;

#ifdef ENABLE_BMAP_STATS
	எனில் (gettimeofday(&bitmap->stats.created,
			 (கட்டமைப்பு timezone *) வெற்று) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		திரும்பு 1;
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
	தேர்வு (magic) {
	நிலை EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		நிறுத்து;
	நிலை EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		நிறுத்து;
	இயல்பு:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	எனில் (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		எனில் (retval) {
			ext2fs_free_mem(&bitmap);
			திரும்பு retval;
		}
		strcpy(bitmap->description, descr);
	} இல்லையெனில்
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	எனில் (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		திரும்பு retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	திரும்பு 0;
}

#ifdef ENABLE_BMAP_STATS
நிலையான வெறுமை ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	கட்டமைப்பு ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	தசம mark_seq_perc = 0.0, test_seq_perc = 0.0;
	தசம mark_back_perc = 0.0, test_back_perc = 0.0;
	கட்டமைப்பு timeval now;
	இரட்டை inuse;

	எனில் (stats->test_மொத்தம்) {
		test_seq_perc = ((தசம)stats->test_seq /
				 stats->test_மொத்தம்) * 100;
		test_back_perc = ((தசம)stats->test_back /
				  stats->test_மொத்தம்) * 100;
	}

	எனில் (stats->mark_மொத்தம்) {
		mark_seq_perc = ((தசம)stats->mark_seq /
				 stats->mark_மொத்தம்) * 100;
		mark_back_perc = ((தசம)stats->mark_back /
				  stats->mark_மொத்தம்) * 100;
	}

	எனில் (gettimeofday(&now, (கட்டமைப்பு timezone *) வெற்று) == -1) {
		perror("gettimeofday");
		திரும்பு;
	}

	inuse = (இரட்டை) now.tv_sec + \
		(((இரட்டை) now.tv_usec) * 0.000001);
	inuse -= (இரட்டை) stats->created.tv_sec + \
		(((இரட்டை) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_மொத்தம், stats->resize_மொத்தம்);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_மொத்தம், stats->unmark_மொத்தம்);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_மொத்தம், stats->mark_ext_மொத்தம்);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_மொத்தம், stats->test_ext_மொத்தம்);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_மொத்தம், stats->get_range_மொத்தம்);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_மொத்தம், stats->test_seq, test_seq_perc);
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

வெறுமை ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	எனில் (!bmap)
		திரும்பு;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		திரும்பு;
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு;

#ifdef ENABLE_BMAP_STATS
	எனில் (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	எனில் (bmap->description) {
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
	எழுத்து *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	எனில் (!src)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(src))
		திரும்பு ext2fs_copy_generic_bitmap(gen_src, dest);

	எனில் (!EXT2FS_IS_64_BITMAP(src))
		திரும்பு EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(அளவு(கட்டமைப்பு ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	எனில் (retval)
		திரும்பு retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_மொத்தம்++;
#endif
#ifdef ENABLE_BMAP_STATS
	எனில் (gettimeofday(&new_bmap->stats.created,
			 (கட்டமைப்பு timezone *) வெற்று) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		திரும்பு 1;
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
	எனில் (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		எனில் (retval) {
			ext2fs_free_mem(&new_bmap);
			திரும்பு retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	எனில் (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		திரும்பு retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	திரும்பு 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	எனில் (!bmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bmap))
		திரும்பு ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு EINVAL;

	INC_STAT(bmap, resize_மொத்தம்);

	திரும்பு bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (!bitmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		முழு retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		எனில் (oend)
			*oend = tmp_oend;
		திரும்பு retval;
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு EINVAL;

	எனில் (end > bitmap->real_end)
		திரும்பு neq;
	எனில் (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	திரும்பு 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (!bitmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap))
		திரும்பு ext2fs_get_generic_bitmap_start(gen_bitmap);

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு EINVAL;

	திரும்பு bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (!bitmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap))
		திரும்பு ext2fs_get_generic_bitmap_end(gen_bitmap);

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு EINVAL;

	திரும்பு bitmap->end;
}

வெறுமை ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	இல்லையெனில்
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

முழு ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (!bitmap)
		திரும்பு 0;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			திரும்பு 0;
		}
		திரும்பு ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	எனில் (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	எனில் (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_மொத்தம்++;
#endif

	எனில் ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		திரும்பு 0;
	}

	திரும்பு bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

முழு ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	எனில் (!bitmap)
		திரும்பு 0;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			திரும்பு 0;
		}
		திரும்பு ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_மொத்தம்);

	எனில் ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		திரும்பு 0;
	}

	திரும்பு bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

முழு ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	எனில் (!bitmap)
		திரும்பு 0;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			திரும்பு 0;
		}
		திரும்பு ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_மொத்தம்++;
	எனில் (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	எனில் (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	எனில் ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		திரும்பு 0;
	}

	திரும்பு bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned முழு இலக்கம்,
					வெறுமை *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	எனில் (!bmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		எனில் ((start+இலக்கம்-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			திரும்பு EINVAL;
		}
		திரும்பு ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, இலக்கம், in);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு EINVAL;

	INC_STAT(bmap, set_range_மொத்தம்);

	திரும்பு bmap->bitmap_ops->set_bmap_range(bmap, start, இலக்கம், in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned முழு இலக்கம்,
					வெறுமை *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	எனில் (!bmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		எனில் ((start+இலக்கம்-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			திரும்பு EINVAL;
		}
		திரும்பு ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, இலக்கம், out);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு EINVAL;

	INC_STAT(bmap, get_range_மொத்தம்);

	திரும்பு bmap->bitmap_ops->get_bmap_range(bmap, start, இலக்கம், out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	எனில் (!bm1 || !bm2)
		திரும்பு EINVAL;
	எனில் (bm1->magic != bm2->magic)
		திரும்பு EINVAL;

	/* Now we know both bitmaps have the same magic */
	எனில் (EXT2FS_IS_32_BITMAP(bm1))
		திரும்பு ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	எனில் (!EXT2FS_IS_64_BITMAP(bm1))
		திரும்பு EINVAL;

	எனில் ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		திரும்பு neq;

	க்கு (i = bm1->start; i < bm1->end; i++) {
		முழு ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		எனில் (ret1 != ret2) {
			திரும்பு neq;
		}
	}

	திரும்பு 0;
}

வெறுமை ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, இலக்கம்;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		திரும்பு;
	}

	start = bmap->end + 1;
	இலக்கம் = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, இலக்கம்);
	/* XXX ought to warn on error */
}

முழு ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned முழு இலக்கம்)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + இலக்கம்;

	எனில் (!bmap)
		திரும்பு EINVAL;

	எனில் (இலக்கம் == 1)
		திரும்பு !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		எனில் ((block & ~0xffffffffULL) ||
		    ((block+இலக்கம்-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			திரும்பு EINVAL;
		}
		திரும்பு ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, இலக்கம்);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு EINVAL;

	INC_STAT(bmap, test_ext_மொத்தம்);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	இலக்கம் = end - block;

	எனில் ((block < bmap->start) || (block > bmap->end) ||
	    (block+இலக்கம்-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		திரும்பு EINVAL;
	}

	திரும்பு bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, இலக்கம்);
}

வெறுமை ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned முழு இலக்கம்)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + இலக்கம்;

	எனில் (!bmap)
		திரும்பு;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		எனில் ((block & ~0xffffffffULL) ||
		    ((block+இலக்கம்-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			திரும்பு;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, இலக்கம்);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு;

	INC_STAT(bmap, mark_ext_மொத்தம்);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	இலக்கம் = end - block;

	எனில் ((block < bmap->start) || (block > bmap->end) ||
	    (block+இலக்கம்-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		திரும்பு;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, இலக்கம்);
}

வெறுமை ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned முழு இலக்கம்)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + இலக்கம்;

	எனில் (!bmap)
		திரும்பு;

	எனில் (EXT2FS_IS_32_BITMAP(bmap)) {
		எனில் ((block & ~0xffffffffULL) ||
		    ((block+இலக்கம்-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			திரும்பு;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, இலக்கம்);
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bmap))
		திரும்பு;

	INC_STAT(bmap, unmark_ext_மொத்தம்);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	இலக்கம் = end - block;

	எனில் ((block < bmap->start) || (block > bmap->end) ||
	    (block+இலக்கம்-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		திரும்பு;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, இலக்கம்);
}

வெறுமை ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, மாறிலி எழுத்து *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	எனில் (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	இல்லையெனில்
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
	எனில் (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		திரும்பு 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	எனில் (retval)
		திரும்பு retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	போது (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		எனில் (retval)
			நிறுத்து;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	திரும்பு 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	எனில் (!bitmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		எனில் (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			திரும்பு EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		எனில் (retval == 0)
			*out = blk;
		திரும்பு retval;
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	எனில் (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		திரும்பு EINVAL;
	}

	எனில் (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		எனில் (retval)
			திரும்பு retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		திரும்பு 0;
	}

	க்கு (cout = cstart; cout <= cend; cout++)
		எனில் (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			செல் found;

	திரும்பு ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	எனில் (!bitmap)
		திரும்பு EINVAL;

	எனில் (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		எனில் (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			திரும்பு EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		எனில் (retval == 0)
			*out = blk;
		திரும்பு retval;
	}

	எனில் (!EXT2FS_IS_64_BITMAP(bitmap))
		திரும்பு EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	எனில் (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		திரும்பு EINVAL;
	}

	எனில் (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		எனில் (retval)
			திரும்பு retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		திரும்பு 0;
	}

	க்கு (cout = cstart; cout <= cend; cout++)
		எனில் (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			செல் found;

	திரும்பு ENOENT;
}

errcode_t ext2fs_மொத்தம்_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	போது (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		எனில் (retval) {
			எனில் (retval == ENOENT)
				retval = 0;
			நிறுத்து;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		எனில் (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} இல்லையெனில் எனில் (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			நிறுத்து;
		} இல்லையெனில்
			நிறுத்து;
	}

	எனில் (!retval)
		*out = tot_set;
	திரும்பு retval;
}

errcode_t ext2fs_மொத்தம்_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_மொத்தம்_used_blocks(fs, start, end, &tot_set);

	எனில் (!retval)
		*out = EXT2_FS_இலக்கம்_B2_C(fs, tot_set);
	திரும்பு retval;
}
