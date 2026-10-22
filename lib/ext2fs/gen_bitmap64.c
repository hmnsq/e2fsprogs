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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <errno.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#അന്തര്ഭാവ <sys/time.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "bmap64.h"

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

സ്ഥിതം ശൂന്യം warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			പൂർണ്ണസംഖ്യ code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	എങ്കിൽ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (unsigned long long) arg,
			bitmap->description);
	അല്ലെങ്കിൽ
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(unsigned long long) arg);
#endif
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#അല്ലെങ്കിൽ
#define INC_STAT(map, name) ;;
#endif


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    പൂർണ്ണസംഖ്യ type, __u64 start, __u64 end,
				    __u64 real_end,
				    സ്ഥിരം അക്ഷരം *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	ഘടന ext2_bitmap_ops	*ops;
	ext2_ino_t അക്കം_dirs;
	errcode_t retval;

	എങ്കിൽ (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	തിരഞ്ഞെടുപ്പ് (type) {
	അവസ്ഥ EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		നിർത്തുക;
	അവസ്ഥ EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		നിർത്തുക;
	അവസ്ഥ EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_അക്കം_dirs(fs, &അക്കം_dirs);
		എങ്കിൽ (retval || അക്കം_dirs > (fs->super->s_inodes_എണ്ണം / 320))
			ops = &ext2fs_blkmap64_bitarray;
		അല്ലെങ്കിൽ
			ops = &ext2fs_blkmap64_rbtree;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		തിരികെ EINVAL;
	}

	retval = ext2fs_get_memzero(വലുപ്പം(ഘടന ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	എങ്കിൽ (retval)
		തിരികെ retval;

#ifdef ENABLE_BMAP_STATS
	എങ്കിൽ (gettimeofday(&bitmap->stats.created,
			 (ഘടന timezone *) ശൂന്യ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		തിരികെ 1;
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
	തിരഞ്ഞെടുപ്പ് (magic) {
	അവസ്ഥ EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		നിർത്തുക;
	അവസ്ഥ EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	എങ്കിൽ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&bitmap);
			തിരികെ retval;
		}
		strcpy(bitmap->description, descr);
	} അല്ലെങ്കിൽ
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		തിരികെ retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	തിരികെ 0;
}

#ifdef ENABLE_BMAP_STATS
സ്ഥിതം ശൂന്യം ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	ഘടന ext2_bmap_statistics *stats = &bitmap->stats;
#ifdef ENABLE_BMAP_STATS_OPS
	ദശാംശം mark_seq_perc = 0.0, test_seq_perc = 0.0;
	ദശാംശം mark_back_perc = 0.0, test_back_perc = 0.0;
	ഘടന timeval now;
	ഇരട്ട inuse;

	എങ്കിൽ (stats->test_എണ്ണം) {
		test_seq_perc = ((ദശാംശം)stats->test_seq /
				 stats->test_എണ്ണം) * 100;
		test_back_perc = ((ദശാംശം)stats->test_back /
				  stats->test_എണ്ണം) * 100;
	}

	എങ്കിൽ (stats->mark_എണ്ണം) {
		mark_seq_perc = ((ദശാംശം)stats->mark_seq /
				 stats->mark_എണ്ണം) * 100;
		mark_back_perc = ((ദശാംശം)stats->mark_back /
				  stats->mark_എണ്ണം) * 100;
	}

	എങ്കിൽ (gettimeofday(&now, (ഘടന timezone *) ശൂന്യ) == -1) {
		perror("gettimeofday");
		തിരികെ;
	}

	inuse = (ഇരട്ട) now.tv_sec + \
		(((ഇരട്ട) now.tv_usec) * 0.000001);
	inuse -= (ഇരട്ട) stats->created.tv_sec + \
		(((ഇരട്ട) stats->created.tv_usec) * 0.000001);
#endif /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ifdef ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_എണ്ണം, stats->resize_എണ്ണം);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_എണ്ണം, stats->unmark_എണ്ണം);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_എണ്ണം, stats->mark_ext_എണ്ണം);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_എണ്ണം, stats->test_ext_എണ്ണം);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_എണ്ണം, stats->get_range_എണ്ണം);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_എണ്ണം, stats->test_seq, test_seq_perc);
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

ശൂന്യം ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	എങ്കിൽ (!bmap)
		തിരികെ;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		തിരികെ;
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ;

#ifdef ENABLE_BMAP_STATS
	എങ്കിൽ (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	bmap->bitmap_ops->free_bmap(bmap);

	എങ്കിൽ (bmap->description) {
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
	അക്ഷരം *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	എങ്കിൽ (!src)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(src))
		തിരികെ ext2fs_copy_generic_bitmap(gen_src, dest);

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(src))
		തിരികെ EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(വലുപ്പം(ഘടന ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	എങ്കിൽ (retval)
		തിരികെ retval;


#ifdef ENABLE_BMAP_STATS_OPS
	src->stats.copy_എണ്ണം++;
#endif
#ifdef ENABLE_BMAP_STATS
	എങ്കിൽ (gettimeofday(&new_bmap->stats.created,
			 (ഘടന timezone *) ശൂന്യ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		തിരികെ 1;
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
	എങ്കിൽ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+10, &new_descr);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&new_bmap);
			തിരികെ retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		തിരികെ retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	തിരികെ 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	എങ്കിൽ (!bmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap))
		തിരികെ ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ EINVAL;

	INC_STAT(bmap, resize_എണ്ണം);

	തിരികെ bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (!bitmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		പൂർണ്ണസംഖ്യ retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		എങ്കിൽ (oend)
			*oend = tmp_oend;
		തിരികെ retval;
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ EINVAL;

	എങ്കിൽ (end > bitmap->real_end)
		തിരികെ neq;
	എങ്കിൽ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	തിരികെ 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (!bitmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap))
		തിരികെ ext2fs_get_generic_bitmap_start(gen_bitmap);

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ EINVAL;

	തിരികെ bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (!bitmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap))
		തിരികെ ext2fs_get_generic_bitmap_end(gen_bitmap);

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ EINVAL;

	തിരികെ bitmap->end;
}

ശൂന്യം ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	അല്ലെങ്കിൽ
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

പൂർണ്ണസംഖ്യ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (!bitmap)
		തിരികെ 0;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			തിരികെ 0;
		}
		തിരികെ ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	എങ്കിൽ (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	എങ്കിൽ (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_എണ്ണം++;
#endif

	എങ്കിൽ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		തിരികെ 0;
	}

	തിരികെ bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

പൂർണ്ണസംഖ്യ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	എങ്കിൽ (!bitmap)
		തിരികെ 0;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			തിരികെ 0;
		}
		തിരികെ ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_എണ്ണം);

	എങ്കിൽ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		തിരികെ 0;
	}

	തിരികെ bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

പൂർണ്ണസംഖ്യ ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	എങ്കിൽ (!bitmap)
		തിരികെ 0;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			തിരികെ 0;
		}
		തിരികെ ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_എണ്ണം++;
	എങ്കിൽ (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	എങ്കിൽ (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#endif

	എങ്കിൽ ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		തിരികെ 0;
	}

	തിരികെ bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	എങ്കിൽ (!bmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		എങ്കിൽ ((start+അക്കം-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			തിരികെ EINVAL;
		}
		തിരികെ ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, അക്കം, in);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ EINVAL;

	INC_STAT(bmap, set_range_എണ്ണം);

	തിരികെ bmap->bitmap_ops->set_bmap_range(bmap, start, അക്കം, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, unsigned പൂർണ്ണസംഖ്യ അക്കം,
					ശൂന്യം *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	എങ്കിൽ (!bmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		എങ്കിൽ ((start+അക്കം-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			തിരികെ EINVAL;
		}
		തിരികെ ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, അക്കം, out);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ EINVAL;

	INC_STAT(bmap, get_range_എണ്ണം);

	തിരികെ bmap->bitmap_ops->get_bmap_range(bmap, start, അക്കം, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	എങ്കിൽ (!bm1 || !bm2)
		തിരികെ EINVAL;
	എങ്കിൽ (bm1->magic != bm2->magic)
		തിരികെ EINVAL;

	/* Now we know both bitmaps have the same magic */
	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bm1))
		തിരികെ ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bm1))
		തിരികെ EINVAL;

	എങ്കിൽ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		തിരികെ neq;

	വേണ്ടി (i = bm1->start; i < bm1->end; i++) {
		പൂർണ്ണസംഖ്യ ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		എങ്കിൽ (ret1 != ret2) {
			തിരികെ neq;
		}
	}

	തിരികെ 0;
}

ശൂന്യം ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, അക്കം;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		തിരികെ;
	}

	start = bmap->end + 1;
	അക്കം = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, അക്കം);
	/* XXX ought to warn on error */
}

പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + അക്കം;

	എങ്കിൽ (!bmap)
		തിരികെ EINVAL;

	എങ്കിൽ (അക്കം == 1)
		തിരികെ !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		എങ്കിൽ ((block & ~0xffffffffULL) ||
		    ((block+അക്കം-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			തിരികെ EINVAL;
		}
		തിരികെ ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, അക്കം);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ EINVAL;

	INC_STAT(bmap, test_ext_എണ്ണം);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	അക്കം = end - block;

	എങ്കിൽ ((block < bmap->start) || (block > bmap->end) ||
	    (block+അക്കം-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		തിരികെ EINVAL;
	}

	തിരികെ bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, അക്കം);
}

ശൂന്യം ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + അക്കം;

	എങ്കിൽ (!bmap)
		തിരികെ;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		എങ്കിൽ ((block & ~0xffffffffULL) ||
		    ((block+അക്കം-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			തിരികെ;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, അക്കം);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ;

	INC_STAT(bmap, mark_ext_എണ്ണം);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	അക്കം = end - block;

	എങ്കിൽ ((block < bmap->start) || (block > bmap->end) ||
	    (block+അക്കം-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		തിരികെ;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, അക്കം);
}

ശൂന്യം ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + അക്കം;

	എങ്കിൽ (!bmap)
		തിരികെ;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bmap)) {
		എങ്കിൽ ((block & ~0xffffffffULL) ||
		    ((block+അക്കം-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			തിരികെ;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, അക്കം);
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bmap))
		തിരികെ;

	INC_STAT(bmap, unmark_ext_എണ്ണം);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	അക്കം = end - block;

	എങ്കിൽ ((block < bmap->start) || (block > bmap->end) ||
	    (block+അക്കം-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		തിരികെ;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, അക്കം);
}

ശൂന്യം ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, സ്ഥിരം അക്ഷരം *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ifndef OMIT_COM_ERR
	എങ്കിൽ (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	അല്ലെങ്കിൽ
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
	എങ്കിൽ (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		തിരികെ 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	എങ്കിൽ (retval)
		തിരികെ retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	ആകുമ്പോൾ (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &next);
		എങ്കിൽ (retval)
			നിർത്തുക;
		ext2fs_mark_block_bitmap2(gen_cmap, next);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, next) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	തിരികെ 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	എങ്കിൽ (!bitmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		എങ്കിൽ (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			തിരികെ EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		എങ്കിൽ (retval == 0)
			*out = blk;
		തിരികെ retval;
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	എങ്കിൽ (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		തിരികെ EINVAL;
	}

	എങ്കിൽ (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		എങ്കിൽ (retval)
			തിരികെ retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		തിരികെ 0;
	}

	വേണ്ടി (cout = cstart; cout <= cend; cout++)
		എങ്കിൽ (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			പോകുക found;

	തിരികെ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	എങ്കിൽ (!bitmap)
		തിരികെ EINVAL;

	എങ്കിൽ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		എങ്കിൽ (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			തിരികെ EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		എങ്കിൽ (retval == 0)
			*out = blk;
		തിരികെ retval;
	}

	എങ്കിൽ (!EXT2FS_IS_64_BITMAP(bitmap))
		തിരികെ EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	എങ്കിൽ (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		തിരികെ EINVAL;
	}

	എങ്കിൽ (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		എങ്കിൽ (retval)
			തിരികെ retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		തിരികെ 0;
	}

	വേണ്ടി (cout = cstart; cout <= cend; cout++)
		എങ്കിൽ (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			പോകുക found;

	തിരികെ ENOENT;
}

errcode_t ext2fs_എണ്ണം_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		next;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	ആകുമ്പോൾ (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &next);
		എങ്കിൽ (retval) {
			എങ്കിൽ (retval == ENOENT)
				retval = 0;
			നിർത്തുക;
		}
		start = next;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &next);
		എങ്കിൽ (retval == 0) {
			tot_set += next - start;
			start  = next + 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			നിർത്തുക;
		} അല്ലെങ്കിൽ
			നിർത്തുക;
	}

	എങ്കിൽ (!retval)
		*out = tot_set;
	തിരികെ retval;
}

errcode_t ext2fs_എണ്ണം_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_എണ്ണം_used_blocks(fs, start, end, &tot_set);

	എങ്കിൽ (!retval)
		*out = EXT2_FS_അക്കം_B2_C(fs, tot_set);
	തിരികെ retval;
}
