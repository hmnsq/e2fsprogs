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

#include "config.h"
#include <stdio.h>
#include <string.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "bmap64.h"

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

static void warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			ціле code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	якщо (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	інакше
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#інакше
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ціле type, __u64 start, __u64 end,
				    __u64 real_end,
				    const char *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	struct ext2_bitmap_ops	*ops;
	ext2_ino_t num_dirs;
	errcode_t retval;

	якщо (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	switch (type) {
	case EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		break;
	case EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		break;
	case EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_num_dirs(fs, &num_dirs);
		якщо (retval || num_dirs > (fs->super->s_inodes_count / 320))
			ops = &ext2fs_blkmap64_bitarray;
		інакше
			ops = &ext2fs_blkmap64_rbtree;
		break;
	default:
		повернути EINVAL;
	}

	retval = ext2fs_get_memzero(sizeof(struct ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	якщо (retval)
		повернути retval;

#ifdef ENABLE_BMAP_STATS
	якщо (gettimeofday(&bitmap->stats.created,
			 (struct timezone *) NULL) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		повернути 1;
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
	switch (magic) {
	case EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		break;
	case EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		break;
	default:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	якщо (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		якщо (retval) {
			ext2fs_free_mem(&bitmap);
			повернути retval;
		}
		strcpy(bitmap->description, descr);
	} інакше
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	якщо (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		повернути retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	повернути 0;
}

#ifdef ENABLE_BMAP_STATS
static void ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	float mark_seq_perc = 0.0, test_seq_perc = 0.0;
	float mark_back_perc = 0.0, test_back_perc = 0.0;
	struct timeval now;
	double inuse;

	якщо (stats->test_count) {
		test_seq_perc = ((float)stats->test_seq /
				 stats->test_count) * 100;
		test_back_perc = ((float)stats->test_back /
				  stats->test_count) * 100;
	}

	якщо (stats->mark_count) {
		mark_seq_perc = ((float)stats->mark_seq /
				 stats->mark_count) * 100;
		mark_back_perc = ((float)stats->mark_back /
				  stats->mark_count) * 100;
	}

	якщо (gettimeofday(&now, (struct timezone *) NULL) == -1) {
		perror("gettimeofday");
		повернути;
	}

	inuse = (double) now.tv_sec + \
		(((double) now.tv_usec) * 0.000001);
	inuse -= (double) stats->created.tv_sec + \
		(((double) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_count, stats->resize_count);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_count, stats->unmark_count);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_count, stats->mark_ext_count);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_count, stats->test_ext_count);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_count, stats->get_range_count);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_count, stats->test_seq, test_seq_perc);
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

void ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	якщо (!bmap)
		повернути;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		повернути;
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути;

#ifdef ENABLE_BMAP_STATS
	якщо (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	якщо (bmap->description) {
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
	char *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	якщо (!src)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(src))
		повернути ext2fs_copy_generic_bitmap(gen_src, dest);

	якщо (!EXT2FS_IS_64_BITMAP(src))
		повернути EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(sizeof(struct ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	якщо (retval)
		повернути retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_count++;
#endif
#ifdef ENABLE_BMAP_STATS
	якщо (gettimeofday(&new_bmap->stats.created,
			 (struct timezone *) NULL) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		повернути 1;
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
	якщо (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		якщо (retval) {
			ext2fs_free_mem(&new_bmap);
			повернути retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	якщо (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		повернути retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	повернути 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	якщо (!bmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bmap))
		повернути ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути EINVAL;

	INC_STAT(bmap, resize_count);

	повернути bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (!bitmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		ціле retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		якщо (oend)
			*oend = tmp_oend;
		повернути retval;
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути EINVAL;

	якщо (end > bitmap->real_end)
		повернути neq;
	якщо (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	повернути 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (!bitmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bitmap))
		повернути ext2fs_get_generic_bitmap_start(gen_bitmap);

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути EINVAL;

	повернути bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (!bitmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bitmap))
		повернути ext2fs_get_generic_bitmap_end(gen_bitmap);

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути EINVAL;

	повернути bitmap->end;
}

void ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	інакше
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

ціле ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (!bitmap)
		повернути 0;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		якщо (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			повернути 0;
		}
		повернути ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	якщо (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	якщо (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_count++;
#endif

	якщо ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		повернути 0;
	}

	повернути bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

ціле ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	якщо (!bitmap)
		повернути 0;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		якщо (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			повернути 0;
		}
		повернути ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_count);

	якщо ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		повернути 0;
	}

	повернути bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

ціле ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	якщо (!bitmap)
		повернути 0;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		якщо (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			повернути 0;
		}
		повернути ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_count++;
	якщо (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	якщо (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	якщо ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		повернути 0;
	}

	повернути bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned ціле num,
					void *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	якщо (!bmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		якщо ((start+num-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			повернути EINVAL;
		}
		повернути ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, num, in);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути EINVAL;

	INC_STAT(bmap, set_range_count);

	повернути bmap->bitmap_ops->set_bmap_range(bmap, start, num, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned ціле num,
					void *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	якщо (!bmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		якщо ((start+num-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			повернути EINVAL;
		}
		повернути ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, num, out);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути EINVAL;

	INC_STAT(bmap, get_range_count);

	повернути bmap->bitmap_ops->get_bmap_range(bmap, start, num, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	якщо (!bm1 || !bm2)
		повернути EINVAL;
	якщо (bm1->magic != bm2->magic)
		повернути EINVAL;

	/* Now we know both bitmaps have the same magic */
	якщо (EXT2FS_IS_32_BITMAP(bm1))
		повернути ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	якщо (!EXT2FS_IS_64_BITMAP(bm1))
		повернути EINVAL;

	якщо ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		повернути neq;

	для (i = bm1->start; i < bm1->end; i++) {
		ціле ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		якщо (ret1 != ret2) {
			повернути neq;
		}
	}

	повернути 0;
}

void ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, num;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		повернути;
	}

	start = bmap->end + 1;
	num = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, num);
	/* XXX ought to warn on error */
}

ціле ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned ціле num)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + num;

	якщо (!bmap)
		повернути EINVAL;

	якщо (num == 1)
		повернути !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		якщо ((block & ~0xffffffffULL) ||
		    ((block+num-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			повернути EINVAL;
		}
		повернути ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, num);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути EINVAL;

	INC_STAT(bmap, test_ext_count);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	num = end - block;

	якщо ((block < bmap->start) || (block > bmap->end) ||
	    (block+num-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		повернути EINVAL;
	}

	повернути bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, num);
}

void ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned ціле num)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + num;

	якщо (!bmap)
		повернути;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		якщо ((block & ~0xffffffffULL) ||
		    ((block+num-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			повернути;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, num);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути;

	INC_STAT(bmap, mark_ext_count);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	num = end - block;

	якщо ((block < bmap->start) || (block > bmap->end) ||
	    (block+num-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		повернути;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, num);
}

void ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned ціле num)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + num;

	якщо (!bmap)
		повернути;

	якщо (EXT2FS_IS_32_BITMAP(bmap)) {
		якщо ((block & ~0xffffffffULL) ||
		    ((block+num-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			повернути;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, num);
	}

	якщо (!EXT2FS_IS_64_BITMAP(bmap))
		повернути;

	INC_STAT(bmap, unmark_ext_count);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	num = end - block;

	якщо ((block < bmap->start) || (block > bmap->end) ||
	    (block+num-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		повернути;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, num);
}

void ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, const char *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	якщо (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	інакше
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
	якщо (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		повернути 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	якщо (retval)
		повернути retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	поки (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		якщо (retval)
			break;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	повернути 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	якщо (!bitmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		якщо (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			повернути EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		якщо (retval == 0)
			*out = blk;
		повернути retval;
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	якщо (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		повернути EINVAL;
	}

	якщо (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		якщо (retval)
			повернути retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		повернути 0;
	}

	для (cout = cstart; cout <= cend; cout++)
		якщо (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			goto found;

	повернути ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	якщо (!bitmap)
		повернути EINVAL;

	якщо (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		якщо (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			повернути EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		якщо (retval == 0)
			*out = blk;
		повернути retval;
	}

	якщо (!EXT2FS_IS_64_BITMAP(bitmap))
		повернути EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	якщо (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		повернути EINVAL;
	}

	якщо (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		якщо (retval)
			повернути retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		повернути 0;
	}

	для (cout = cstart; cout <= cend; cout++)
		якщо (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			goto found;

	повернути ENOENT;
}

errcode_t ext2fs_count_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	поки (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		якщо (retval) {
			якщо (retval == ENOENT)
				retval = 0;
			break;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		якщо (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} інакше якщо (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			break;
		} інакше
			break;
	}

	якщо (!retval)
		*out = tot_set;
	повернути retval;
}

errcode_t ext2fs_count_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_count_used_blocks(fs, start, end, &tot_set);

	якщо (!retval)
		*out = EXT2FS_NUM_B2C(fs, tot_set);
	повернути retval;
}
