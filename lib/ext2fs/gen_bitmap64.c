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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <errno.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#અન્તર્ભાવ <sys/time.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "bmap64.h"

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

સ્થિર શૂન્ય warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			પૂર્ણાંક code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	જો (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	નહીંતર
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#નહીંતર
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    પૂર્ણાંક type, __u64 start, __u64 end,
				    __u64 real_end,
				    અચલ અક્ષર *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	સંરચના ext2_bitmap_ops	*ops;
	ext2_ino_t અંક_dirs;
	errcode_t retval;

	જો (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	પસંદગી (type) {
	સ્થિતિ EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		તોડો;
	સ્થિતિ EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		તોડો;
	સ્થિતિ EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_અંક_dirs(fs, &અંક_dirs);
		જો (retval || અંક_dirs > (fs->super->s_inodes_ગણતરી / 320))
			ops = &ext2fs_blkmap64_bitarray;
		નહીંતર
			ops = &ext2fs_blkmap64_rbtree;
		તોડો;
	મૂળભૂત:
		પાછા EINVAL;
	}

	retval = ext2fs_get_memzero(કદ(સંરચના ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	જો (retval)
		પાછા retval;

#ifdef ENABLE_BMAP_STATS
	જો (gettimeofday(&bitmap->stats.created,
			 (સંરચના timezone *) ખાલી) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		પાછા 1;
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
	પસંદગી (magic) {
	સ્થિતિ EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		તોડો;
	સ્થિતિ EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		તોડો;
	મૂળભૂત:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	જો (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		જો (retval) {
			ext2fs_free_mem(&bitmap);
			પાછા retval;
		}
		strcpy(bitmap->description, descr);
	} નહીંતર
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	જો (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		પાછા retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	પાછા 0;
}

#ifdef ENABLE_BMAP_STATS
સ્થિર શૂન્ય ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	સંરચના ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	દશાંશ mark_seq_perc = 0.0, test_seq_perc = 0.0;
	દશાંશ mark_back_perc = 0.0, test_back_perc = 0.0;
	સંરચના timeval now;
	દ્વિગુણ inuse;

	જો (stats->test_ગણતરી) {
		test_seq_perc = ((દશાંશ)stats->test_seq /
				 stats->test_ગણતરી) * 100;
		test_back_perc = ((દશાંશ)stats->test_back /
				  stats->test_ગણતરી) * 100;
	}

	જો (stats->mark_ગણતરી) {
		mark_seq_perc = ((દશાંશ)stats->mark_seq /
				 stats->mark_ગણતરી) * 100;
		mark_back_perc = ((દશાંશ)stats->mark_back /
				  stats->mark_ગણતરી) * 100;
	}

	જો (gettimeofday(&now, (સંરચના timezone *) ખાલી) == -1) {
		perror("gettimeofday");
		પાછા;
	}

	inuse = (દ્વિગુણ) now.tv_sec + \
		(((દ્વિગુણ) now.tv_usec) * 0.000001);
	inuse -= (દ્વિગુણ) stats->created.tv_sec + \
		(((દ્વિગુણ) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_ગણતરી, stats->resize_ગણતરી);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_ગણતરી, stats->unmark_ગણતરી);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_ગણતરી, stats->mark_ext_ગણતરી);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_ગણતરી, stats->test_ext_ગણતરી);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_ગણતરી, stats->get_range_ગણતરી);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_ગણતરી, stats->test_seq, test_seq_perc);
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

શૂન્ય ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	જો (!bmap)
		પાછા;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		પાછા;
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા;

#ifdef ENABLE_BMAP_STATS
	જો (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	જો (bmap->description) {
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
	અક્ષર *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	જો (!src)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(src))
		પાછા ext2fs_copy_generic_bitmap(gen_src, dest);

	જો (!EXT2FS_IS_64_BITMAP(src))
		પાછા EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(કદ(સંરચના ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	જો (retval)
		પાછા retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_ગણતરી++;
#endif
#ifdef ENABLE_BMAP_STATS
	જો (gettimeofday(&new_bmap->stats.created,
			 (સંરચના timezone *) ખાલી) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		પાછા 1;
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
	જો (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		જો (retval) {
			ext2fs_free_mem(&new_bmap);
			પાછા retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	જો (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		પાછા retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	પાછા 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	જો (!bmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bmap))
		પાછા ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા EINVAL;

	INC_STAT(bmap, resize_ગણતરી);

	પાછા bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (!bitmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		પૂર્ણાંક retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		જો (oend)
			*oend = tmp_oend;
		પાછા retval;
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા EINVAL;

	જો (end > bitmap->real_end)
		પાછા neq;
	જો (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	પાછા 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (!bitmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bitmap))
		પાછા ext2fs_get_generic_bitmap_start(gen_bitmap);

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા EINVAL;

	પાછા bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (!bitmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bitmap))
		પાછા ext2fs_get_generic_bitmap_end(gen_bitmap);

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા EINVAL;

	પાછા bitmap->end;
}

શૂન્ય ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	નહીંતર
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

પૂર્ણાંક ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (!bitmap)
		પાછા 0;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		જો (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			પાછા 0;
		}
		પાછા ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	જો (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	જો (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_ગણતરી++;
#endif

	જો ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		પાછા 0;
	}

	પાછા bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

પૂર્ણાંક ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	જો (!bitmap)
		પાછા 0;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		જો (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			પાછા 0;
		}
		પાછા ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_ગણતરી);

	જો ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		પાછા 0;
	}

	પાછા bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

પૂર્ણાંક ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	જો (!bitmap)
		પાછા 0;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		જો (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			પાછા 0;
		}
		પાછા ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_ગણતરી++;
	જો (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	જો (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	જો ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		પાછા 0;
	}

	પાછા bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned પૂર્ણાંક અંક,
					શૂન્ય *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	જો (!bmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		જો ((start+અંક-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			પાછા EINVAL;
		}
		પાછા ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, અંક, in);
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા EINVAL;

	INC_STAT(bmap, set_range_ગણતરી);

	પાછા bmap->bitmap_ops->set_bmap_range(bmap, start, અંક, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned પૂર્ણાંક અંક,
					શૂન્ય *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	જો (!bmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		જો ((start+અંક-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			પાછા EINVAL;
		}
		પાછા ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, અંક, out);
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા EINVAL;

	INC_STAT(bmap, get_range_ગણતરી);

	પાછા bmap->bitmap_ops->get_bmap_range(bmap, start, અંક, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	જો (!bm1 || !bm2)
		પાછા EINVAL;
	જો (bm1->magic != bm2->magic)
		પાછા EINVAL;

	/* Now we know both bitmaps have the same magic */
	જો (EXT2FS_IS_32_BITMAP(bm1))
		પાછા ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	જો (!EXT2FS_IS_64_BITMAP(bm1))
		પાછા EINVAL;

	જો ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		પાછા neq;

	માટે (i = bm1->start; i < bm1->end; i++) {
		પૂર્ણાંક ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		જો (ret1 != ret2) {
			પાછા neq;
		}
	}

	પાછા 0;
}

શૂન્ય ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, અંક;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		પાછા;
	}

	start = bmap->end + 1;
	અંક = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, અંક);
	/* XXX ought to warn on error */
}

પૂર્ણાંક ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned પૂર્ણાંક અંક)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + અંક;

	જો (!bmap)
		પાછા EINVAL;

	જો (અંક == 1)
		પાછા !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		જો ((block & ~0xffffffffULL) ||
		    ((block+અંક-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			પાછા EINVAL;
		}
		પાછા ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, અંક);
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા EINVAL;

	INC_STAT(bmap, test_ext_ગણતરી);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	અંક = end - block;

	જો ((block < bmap->start) || (block > bmap->end) ||
	    (block+અંક-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		પાછા EINVAL;
	}

	પાછા bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, અંક);
}

શૂન્ય ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned પૂર્ણાંક અંક)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + અંક;

	જો (!bmap)
		પાછા;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		જો ((block & ~0xffffffffULL) ||
		    ((block+અંક-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			પાછા;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, અંક);
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા;

	INC_STAT(bmap, mark_ext_ગણતરી);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	અંક = end - block;

	જો ((block < bmap->start) || (block > bmap->end) ||
	    (block+અંક-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		પાછા;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, અંક);
}

શૂન્ય ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned પૂર્ણાંક અંક)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + અંક;

	જો (!bmap)
		પાછા;

	જો (EXT2FS_IS_32_BITMAP(bmap)) {
		જો ((block & ~0xffffffffULL) ||
		    ((block+અંક-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			પાછા;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, અંક);
	}

	જો (!EXT2FS_IS_64_BITMAP(bmap))
		પાછા;

	INC_STAT(bmap, unmark_ext_ગણતરી);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	અંક = end - block;

	જો ((block < bmap->start) || (block > bmap->end) ||
	    (block+અંક-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		પાછા;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, અંક);
}

શૂન્ય ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, અચલ અક્ષર *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	જો (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	નહીંતર
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
	જો (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		પાછા 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	જો (retval)
		પાછા retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	જ્યારે (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		જો (retval)
			તોડો;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	પાછા 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	જો (!bitmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		જો (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			પાછા EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		જો (retval == 0)
			*out = blk;
		પાછા retval;
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	જો (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		પાછા EINVAL;
	}

	જો (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		જો (retval)
			પાછા retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		પાછા 0;
	}

	માટે (cout = cstart; cout <= cend; cout++)
		જો (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			જાઓ found;

	પાછા ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	જો (!bitmap)
		પાછા EINVAL;

	જો (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		જો (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			પાછા EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		જો (retval == 0)
			*out = blk;
		પાછા retval;
	}

	જો (!EXT2FS_IS_64_BITMAP(bitmap))
		પાછા EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	જો (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		પાછા EINVAL;
	}

	જો (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		જો (retval)
			પાછા retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		પાછા 0;
	}

	માટે (cout = cstart; cout <= cend; cout++)
		જો (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			જાઓ found;

	પાછા ENOENT;
}

errcode_t ext2fs_ગણતરી_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	જ્યારે (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		જો (retval) {
			જો (retval == ENOENT)
				retval = 0;
			તોડો;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		જો (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} નહીંતર જો (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			તોડો;
		} નહીંતર
			તોડો;
	}

	જો (!retval)
		*out = tot_set;
	પાછા retval;
}

errcode_t ext2fs_ગણતરી_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_ગણતરી_used_blocks(fs, start, end, &tot_set);

	જો (!retval)
		*out = EXT2_FS_અંક_B2_C(fs, tot_set);
	પાછા retval;
}
