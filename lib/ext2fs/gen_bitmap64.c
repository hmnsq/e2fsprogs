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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_ಸಮಯ_H
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"

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

ಸ್ಥಿರ ಶೂನ್ಯ warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, __u64 arg)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
	ಆದರೆ (bitmap->description)
		com_err(0, bitmap->ತಳ_error_ಸಂಕೇತ+ಸಂಕೇತ_2,
			"#%llu for %s", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) arg,
			bitmap->description);
	ಇಲ್ಲದಿದ್ದರೆ
		com_err(0, bitmap->ತಳ_error_ಸಂಕೇತ + ಸಂಕೇತ_2, "#%llu",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) arg);
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
#ವ್ಯಾಖ್ಯಾನಿಸು INC_STAT(map, name) map->stats.name
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು INC_STAT(map, name) ;;
#ಆದರೆ_ಅಂತ್ಯ


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ಪೂರ್ಣಾಂಕ type, __u64 ಆರಂಭ, __u64 ಕೊನೆ,
				    __u64 real_ಕೊನೆ,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	ರಚನೆ ext2_bitmap_ops	*ops;
	ext2_ino_t ಅಂಕೆ_dirs;
	errcode_t retval;

	ಆದರೆ (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	ಆಯ್ಕೆ (type) {
	ಸ್ಥಿತಿ EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_ಅಂಕೆ_dirs(fs, &ಅಂಕೆ_dirs);
		ಆದರೆ (retval || ಅಂಕೆ_dirs > (fs->super->s_inodes_ಎಣಿಕೆ / 320))
			ops = &ext2fs_blkmap64_bitarray;
		ಇಲ್ಲದಿದ್ದರೆ
			ops = &ext2fs_blkmap64_rbtree;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	retval = ext2fs_get_memzero(ಗಾತ್ರ(ರಚನೆ ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
	ಆದರೆ (gettimeofday(&bitmap->stats.created,
			 (ರಚನೆ timezone *) ಖಾಲಿ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		ಹಿಂತಿರುಗಿ 1;
	}
	bitmap->stats.type = type;
#ಆದರೆ_ಅಂತ್ಯ

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ಆರಂಭ = ಆರಂಭ;
	bitmap->ಕೊನೆ = ಕೊನೆ;
	bitmap->real_ಕೊನೆ = real_ಕೊನೆ;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_ಬಿಟ್ಟುಗಳು = 0;
	ಆಯ್ಕೆ (magic) {
	ಸ್ಥಿತಿ EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_INODE_MARK;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_ET_MAGIC_ಖಂಡ_BITMAP64:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_ಖಂಡ_MARK;
		bitmap->cluster_ಬಿಟ್ಟುಗಳು = fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_GENERIC_MARK;
	}
	ಆದರೆ (descr) {
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(descr)+1, &bitmap->description);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&bitmap);
			ಹಿಂತಿರುಗಿ retval;
		}
		strcpy(bitmap->description, descr);
	} ಇಲ್ಲದಿದ್ದರೆ
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ಹಿಂತಿರುಗಿ retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
ಸ್ಥಿರ ಶೂನ್ಯ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	ರಚನೆ ext2_bmap_statistics *stats = &bitmap->stats;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	ದಶಮಾಂಶ mark_seq_perc = 0.0, test_seq_perc = 0.0;
	ದಶಮಾಂಶ mark_ಹಿಂಭಾಗ_perc = 0.0, test_ಹಿಂಭಾಗ_perc = 0.0;
	ರಚನೆ timeval now;
	ದ್ವಿಗುಣ inuse;

	ಆದರೆ (stats->test_ಎಣಿಕೆ) {
		test_seq_perc = ((ದಶಮಾಂಶ)stats->test_seq /
				 stats->test_ಎಣಿಕೆ) * 100;
		test_ಹಿಂಭಾಗ_perc = ((ದಶಮಾಂಶ)stats->test_ಹಿಂಭಾಗ /
				  stats->test_ಎಣಿಕೆ) * 100;
	}

	ಆದರೆ (stats->mark_ಎಣಿಕೆ) {
		mark_seq_perc = ((ದಶಮಾಂಶ)stats->mark_seq /
				 stats->mark_ಎಣಿಕೆ) * 100;
		mark_ಹಿಂಭಾಗ_perc = ((ದಶಮಾಂಶ)stats->mark_ಹಿಂಭಾಗ /
				  stats->mark_ಎಣಿಕೆ) * 100;
	}

	ಆದರೆ (gettimeofday(&now, (ರಚನೆ timezone *) ಖಾಲಿ) == -1) {
		perror("gettimeofday");
		ಹಿಂತಿರುಗಿ;
	}

	inuse = (ದ್ವಿಗುಣ) now.tv_sec + \
		(((ದ್ವಿಗುಣ) now.tv_usec) * 0.000001);
	inuse -= (ದ್ವಿಗುಣ) stats->created.tv_sec + \
		(((ದ್ವಿಗುಣ) stats->created.tv_usec) * 0.000001);
#ಆದರೆ_ಅಂತ್ಯ /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_ಎಣಿಕೆ, stats->resize_ಎಣಿಕೆ);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_ಎಣಿಕೆ, stats->unmark_ಎಣಿಕೆ);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_ಎಣಿಕೆ, stats->mark_ext_ಎಣಿಕೆ);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_ಎಣಿಕೆ, stats->test_ext_ಎಣಿಕೆ);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->ಗಣ_range_ಎಣಿಕೆ, stats->get_range_ಎಣಿಕೆ);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_ಎಣಿಕೆ, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_ಹಿಂಭಾಗ, test_ಹಿಂಭಾಗ_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_ಹಿಂಭಾಗ, mark_ಹಿಂಭಾಗ_perc, inuse);
#ಆದರೆ_ಅಂತ್ಯ /* ENABLE_BMAP_STATS_OPS */
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#ಆದರೆ_ಅಂತ್ಯ

	bmap->bitmap_ops->free_bmap(bmap);

	ಆದರೆ (bmap->description) {
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
	ಅಕ್ಷರ *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	ಆದರೆ (!src)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(src))
		ಹಿಂತಿರುಗಿ ext2fs_copy_generic_bitmap(gen_src, dest);

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(src))
		ಹಿಂತಿರುಗಿ EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(ಗಾತ್ರ(ರಚನೆ ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	src->stats.copy_ಎಣಿಕೆ++;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
	ಆದರೆ (gettimeofday(&new_bmap->stats.created,
			 (ರಚನೆ timezone *) ಖಾಲಿ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		ಹಿಂತಿರುಗಿ 1;
	}
	new_bmap->stats.type = src->stats.type;
#ಆದರೆ_ಅಂತ್ಯ

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->ಆರಂಭ = src->ಆರಂಭ;
	new_bmap->ಕೊನೆ = src->ಕೊನೆ;
	new_bmap->real_ಕೊನೆ = src->real_ಕೊನೆ;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->ತಳ_error_ಸಂಕೇತ = src->ತಳ_error_ಸಂಕೇತ;
	new_bmap->cluster_ಬಿಟ್ಟುಗಳು = src->cluster_ಬಿಟ್ಟುಗಳು;

	descr = src->description;
	ಆದರೆ (descr) {
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(descr)+10, &new_descr);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&new_bmap);
			ಹಿಂತಿರುಗಿ retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		ಹಿಂತಿರುಗಿ retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_ಕೊನೆ,
				     __u64 new_real_ಕೊನೆ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ ext2fs_resize_generic_bitmap(gen_bmap->magic, new_ಕೊನೆ,
						    new_real_ಕೊನೆ, gen_bmap);

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	INC_STAT(bmap, resize_ಎಣಿಕೆ);

	ಹಿಂತಿರುಗಿ bmap->bitmap_ops->resize_bmap(bmap, new_ಕೊನೆ, new_real_ಕೊನೆ);
}

errcode_t ext2fs_fudge_generic_bmap_ಕೊನೆ(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 ಕೊನೆ, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		ಪೂರ್ಣಾಂಕ retval;

		retval = ext2fs_fudge_generic_bitmap_ಕೊನೆ(gen_bitmap,
							 bitmap->magic,
							 neq, ಕೊನೆ, &tmp_oend);
		ಆದರೆ (oend)
			*oend = tmp_oend;
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (ಕೊನೆ > bitmap->real_ಕೊನೆ)
		ಹಿಂತಿರುಗಿ neq;
	ಆದರೆ (oend)
		*oend = bitmap->ಕೊನೆ;
	bitmap->ಕೊನೆ = ಕೊನೆ;
	ಹಿಂತಿರುಗಿ 0;
}

__u64 ext2fs_get_generic_bmap_ಆರಂಭ(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಆರಂಭ(gen_bitmap);

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	ಹಿಂತಿರುಗಿ bitmap->ಆರಂಭ;
}

__u64 ext2fs_get_generic_bmap_ಕೊನೆ(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಕೊನೆ(gen_bitmap);

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	ಹಿಂತಿರುಗಿ bitmap->ಕೊನೆ;
}

ಶೂನ್ಯ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	ಇಲ್ಲದಿದ್ದರೆ
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

ಪೂರ್ಣಾಂಕ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			ಹಿಂತಿರುಗಿ 0;
		}
		ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ 0;

	arg >>= bitmap->cluster_ಬಿಟ್ಟುಗಳು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	ಆದರೆ (arg == bitmap->stats.ಕೊನೆಯ_marked + 1)
		bitmap->stats.mark_seq++;
	ಆದರೆ (arg < bitmap->stats.ಕೊನೆಯ_marked)
		bitmap->stats.mark_ಹಿಂಭಾಗ++;
	bitmap->stats.ಕೊನೆಯ_marked = arg;
	bitmap->stats.mark_ಎಣಿಕೆ++;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ ((arg < bitmap->ಆರಂಭ) || (arg > bitmap->ಕೊನೆ)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

ಪೂರ್ಣಾಂಕ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ಹಿಂತಿರುಗಿ 0;
		}
		ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ 0;

	arg >>= bitmap->cluster_ಬಿಟ್ಟುಗಳು;

	INC_STAT(bitmap, unmark_ಎಣಿಕೆ);

	ಆದರೆ ((arg < bitmap->ಆರಂಭ) || (arg > bitmap->ಕೊನೆ)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

ಪೂರ್ಣಾಂಕ ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			ಹಿಂತಿರುಗಿ 0;
		}
		ಹಿಂತಿರುಗಿ ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ 0;

	arg >>= bitmap->cluster_ಬಿಟ್ಟುಗಳು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_ಎಣಿಕೆ++;
	ಆದರೆ (arg == bitmap->stats.ಕೊನೆಯ_tested + 1)
		bitmap->stats.test_seq++;
	ಆದರೆ (arg < bitmap->stats.ಕೊನೆಯ_tested)
		bitmap->stats.test_ಹಿಂಭಾಗ++;
	bitmap->stats.ಕೊನೆಯ_tested = arg;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ ((arg < bitmap->ಆರಂಭ) || (arg > bitmap->ಕೊನೆ)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_ಗಣ_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ಆದರೆ ((ಆರಂಭ+ಅಂಕೆ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ಹಿಂತಿರುಗಿ EINVAL;
		}
		ಹಿಂತಿರುಗಿ ext2fs_ಗಣ_generic_bitmap_range(gen_bmap, bmap->magic,
						       ಆರಂಭ, ಅಂಕೆ, ಒಳ);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	INC_STAT(bmap, ಗಣ_range_ಎಣಿಕೆ);

	ಹಿಂತಿರುಗಿ bmap->bitmap_ops->ಗಣ_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಒಳ);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ಆದರೆ ((ಆರಂಭ+ಅಂಕೆ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ಹಿಂತಿರುಗಿ EINVAL;
		}
		ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	INC_STAT(bmap, get_range_ಎಣಿಕೆ);

	ಹಿಂತಿರುಗಿ bmap->bitmap_ops->get_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	ಆದರೆ (!bm1 || !bm2)
		ಹಿಂತಿರುಗಿ EINVAL;
	ಆದರೆ (bm1->magic != bm2->magic)
		ಹಿಂತಿರುಗಿ EINVAL;

	/* Now we know both bitmaps have the same magic */
	ಆದರೆ (EXT2FS_IS_32_BITMAP(bm1))
		ಹಿಂತಿರುಗಿ ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bm1))
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ ((bm1->ಆರಂಭ != bm2->ಆರಂಭ) ||
	    (bm1->ಕೊನೆ != bm2->ಕೊನೆ))
		ಹಿಂತಿರುಗಿ neq;

	ಗಾಗಿ (i = bm1->ಆರಂಭ; i < bm1->ಕೊನೆ; i++) {
		ಪೂರ್ಣಾಂಕ ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		ಆದರೆ (ret1 != ret2) {
			ಹಿಂತಿರುಗಿ neq;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ ext2fs_ಗಣ_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ಆರಂಭ, ಅಂಕೆ;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_ಗಣ_generic_bitmap_padding(gen_bmap);
		ಹಿಂತಿರುಗಿ;
	}

	ಆರಂಭ = bmap->ಕೊನೆ + 1;
	ಅಂಕೆ = bmap->real_ಕೊನೆ - bmap->ಕೊನೆ;
	bmap->bitmap_ops->mark_bmap_extent(bmap, ಆರಂಭ, ಅಂಕೆ);
	/* XXX ought to warn on error */
}

ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap gen_bmap,
				    blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ಕೊನೆ = ಖಂಡ + ಅಂಕೆ;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (ಅಂಕೆ == 1)
		ಹಿಂತಿರುಗಿ !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, ಖಂಡ);

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ಆದರೆ ((ಖಂಡ & ~0xffffffffULL) ||
		    ((ಖಂಡ+ಅಂಕೆ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ಹಿಂತಿರುಗಿ EINVAL;
		}
		ಹಿಂತಿರುಗಿ ext2fs_test_ಖಂಡ_bitmap_range(
			(ext2fs_generic_bitmap) bmap, ಖಂಡ, ಅಂಕೆ);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	INC_STAT(bmap, test_ext_ಎಣಿಕೆ);

	/* convert to clusters if necessary */
	ಖಂಡ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಕೊನೆ += (1ULL << bmap->cluster_ಬಿಟ್ಟುಗಳು) - 1;
	ಕೊನೆ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಅಂಕೆ = ಕೊನೆ - ಖಂಡ;

	ಆದರೆ ((ಖಂಡ < bmap->ಆರಂಭ) || (ಖಂಡ > bmap->ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bmap->ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_TEST, ಖಂಡ,
				   bmap->description);
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	ಹಿಂತಿರುಗಿ bmap->bitmap_ops->test_clear_bmap_extent(bmap, ಖಂಡ, ಅಂಕೆ);
}

ಶೂನ್ಯ ext2fs_mark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap gen_bmap,
				     blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ಕೊನೆ = ಖಂಡ + ಅಂಕೆ;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ಆದರೆ ((ಖಂಡ & ~0xffffffffULL) ||
		    ((ಖಂಡ+ಅಂಕೆ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ಹಿಂತಿರುಗಿ;
		}
		ext2fs_mark_ಖಂಡ_bitmap_range((ext2fs_generic_bitmap) bmap,
					       ಖಂಡ, ಅಂಕೆ);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ;

	INC_STAT(bmap, mark_ext_ಎಣಿಕೆ);

	/* convert to clusters if necessary */
	ಖಂಡ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಕೊನೆ += (1ULL << bmap->cluster_ಬಿಟ್ಟುಗಳು) - 1;
	ಕೊನೆ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಅಂಕೆ = ಕೊನೆ - ಖಂಡ;

	ಆದರೆ ((ಖಂಡ < bmap->ಆರಂಭ) || (ಖಂಡ > bmap->ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bmap->ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_MARK, ಖಂಡ,
				   bmap->description);
		ಹಿಂತಿರುಗಿ;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, ಖಂಡ, ಅಂಕೆ);
}

ಶೂನ್ಯ ext2fs_unmark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap gen_bmap,
				       blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ಕೊನೆ = ಖಂಡ + ಅಂಕೆ;

	ಆದರೆ (!bmap)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bmap)) {
		ಆದರೆ ((ಖಂಡ & ~0xffffffffULL) ||
		    ((ಖಂಡ+ಅಂಕೆ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ಹಿಂತಿರುಗಿ;
		}
		ext2fs_unmark_ಖಂಡ_bitmap_range((ext2fs_generic_bitmap) bmap,
						 ಖಂಡ, ಅಂಕೆ);
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ;

	INC_STAT(bmap, unmark_ext_ಎಣಿಕೆ);

	/* convert to clusters if necessary */
	ಖಂಡ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಕೊನೆ += (1ULL << bmap->cluster_ಬಿಟ್ಟುಗಳು) - 1;
	ಕೊನೆ >>= bmap->cluster_ಬಿಟ್ಟುಗಳು;
	ಅಂಕೆ = ಕೊನೆ - ಖಂಡ;

	ಆದರೆ ((ಖಂಡ < bmap->ಆರಂಭ) || (ಖಂಡ > bmap->ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bmap->ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_UNMARK, ಖಂಡ,
				   bmap->description);
		ಹಿಂತಿರುಗಿ;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, ಖಂಡ, ಅಂಕೆ);
}

ಶೂನ್ಯ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
	ಆದರೆ (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	ಇಲ್ಲದಿದ್ದರೆ
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#ಆದರೆ_ಅಂತ್ಯ
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ಖಂಡ_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_ಖಂಡ_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, ಮುಂದಿನ, b_ಕೊನೆ, c_ಕೊನೆ;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	ಆದರೆ (fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು == ext2fs_get_bitmap_granularity(gen_bmap))
		ಹಿಂತಿರುಗಿ 0;	/* Nothing to do */

	retval = ext2fs_allocate_ಖಂಡ_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->ಆರಂಭ;
	b_ಕೊನೆ = bmap->ಕೊನೆ;
	bmap->ಕೊನೆ = bmap->real_ಕೊನೆ;
	c_ಕೊನೆ = cmap->ಕೊನೆ;
	cmap->ಕೊನೆ = cmap->real_ಕೊನೆ;
	ಆಗ (i < bmap->real_ಕೊನೆ) {
		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(gen_bmap,
						i, bmap->real_ಕೊನೆ, &ಮುಂದಿನ);
		ಆದರೆ (retval)
			ನಿಲ್ಲಿಸು;
		ext2fs_mark_ಖಂಡ_bitmap2(gen_cmap, ಮುಂದಿನ);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, ಮುಂದಿನ) + 1);
	}
	bmap->ಕೊನೆ = b_ಕೊನೆ;
	cmap->ಕೊನೆ = c_ಕೊನೆ;
	ext2fs_free_ಖಂಡ_bitmap(gen_bmap);
	*bitmap = (ext2fs_ಖಂಡ_bitmap) cmap;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ಆದರೆ (((ಆರಂಭ) & ~0xffffffffULL) ||
		    ((ಕೊನೆ) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ಆರಂಭ);
			ಹಿಂತಿರುಗಿ EINVAL;
		}

		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bitmap(bitmap, ಆರಂಭ,
							       ಕೊನೆ, &blk);
		ಆದರೆ (retval == 0)
			*ಹೊರಹೋಗು = blk;
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	cstart = ಆರಂಭ >> bmap64->cluster_ಬಿಟ್ಟುಗಳು;
	cend = ಕೊನೆ >> bmap64->cluster_ಬಿಟ್ಟುಗಳು;

	ಆದರೆ (cstart < bmap64->ಆರಂಭ || cend > bmap64->ಕೊನೆ || ಆರಂಭ > ಕೊನೆ) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ಆರಂಭ);
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	ಆದರೆ (bmap64->bitmap_ops->ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero) {
		retval = bmap64->bitmap_ops->ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero(bmap64, cstart,
							     cend, &cout);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	ಸಿಕ್ಕಿತು:
		cout <<= bmap64->cluster_ಬಿಟ್ಟುಗಳು;
		*ಹೊರಹೋಗು = (cout >= ಆರಂಭ) ? cout : ಆರಂಭ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾಗಿ (cout = cstart; cout <= cend; cout++)
		ಆದರೆ (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			ಹೋಗು ಸಿಕ್ಕಿತು;

	ಹಿಂತಿರುಗಿ ENOENT;
}

errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ಆದರೆ (!bitmap)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ಆದರೆ (((ಆರಂಭ) & ~0xffffffffULL) ||
		    ((ಕೊನೆ) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ಆರಂಭ);
			ಹಿಂತಿರುಗಿ EINVAL;
		}

		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bitmap(bitmap, ಆರಂಭ,
							      ಕೊನೆ, &blk);
		ಆದರೆ (retval == 0)
			*ಹೊರಹೋಗು = blk;
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bitmap))
		ಹಿಂತಿರುಗಿ EINVAL;

	cstart = ಆರಂಭ >> bmap64->cluster_ಬಿಟ್ಟುಗಳು;
	cend = ಕೊನೆ >> bmap64->cluster_ಬಿಟ್ಟುಗಳು;

	ಆದರೆ (cstart < bmap64->ಆರಂಭ || cend > bmap64->ಕೊನೆ || ಆರಂಭ > ಕೊನೆ) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ಆರಂಭ);
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	ಆದರೆ (bmap64->bitmap_ops->ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ) {
		retval = bmap64->bitmap_ops->ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ(bmap64, cstart,
							    cend, &cout);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	ಸಿಕ್ಕಿತು:
		cout <<= bmap64->cluster_ಬಿಟ್ಟುಗಳು;
		*ಹೊರಹೋಗು = (cout >= ಆರಂಭ) ? cout : ಆರಂಭ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾಗಿ (cout = cstart; cout <= cend; cout++)
		ಆದರೆ (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			ಹೋಗು ಸಿಕ್ಕಿತು;

	ಹಿಂತಿರುಗಿ ENOENT;
}

errcode_t ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_ಖಂಡಗಳು(ext2_filsys fs, blk64_t ಆರಂಭ,
				   blk64_t ಕೊನೆ, blk64_t *ಹೊರಹೋಗು)
{
	blk64_t		ಮುಂದಿನ;
	blk64_t		tot_ಗಣ = 0;
	errcode_t	retval = 0;

	ಆಗ (ಆರಂಭ < ಕೊನೆ) {
		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(fs->ಖಂಡ_map,
							ಆರಂಭ, ಕೊನೆ, &ಮುಂದಿನ);
		ಆದರೆ (retval) {
			ಆದರೆ (retval == ENOENT)
				retval = 0;
			ನಿಲ್ಲಿಸು;
		}
		ಆರಂಭ = ಮುಂದಿನ;

		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(fs->ಖಂಡ_map,
							ಆರಂಭ, ಕೊನೆ, &ಮುಂದಿನ);
		ಆದರೆ (retval == 0) {
			tot_ಗಣ += ಮುಂದಿನ - ಆರಂಭ;
			ಆರಂಭ  = ಮುಂದಿನ + 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval == ENOENT) {
			retval = 0;
			tot_ಗಣ += ಕೊನೆ - ಆರಂಭ + 1;
			ನಿಲ್ಲಿಸು;
		} ಇಲ್ಲದಿದ್ದರೆ
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (!retval)
		*ಹೊರಹೋಗು = tot_ಗಣ;
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_clusters(ext2_filsys fs, blk64_t ಆರಂಭ,
				     blk64_t ಕೊನೆ, blk64_t *ಹೊರಹೋಗು)
{
	blk64_t		tot_ಗಣ = 0;
	errcode_t	retval = 0;

	retval = ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_ಖಂಡಗಳು(fs, ಆರಂಭ, ಕೊನೆ, &tot_ಗಣ);

	ಆದರೆ (!retval)
		*ಹೊರಹೋಗು = EXT2_FS_ಅಂಕೆ_B2_C(fs, tot_ಗಣ);
	ಹಿಂತಿರುಗಿ retval;
}
