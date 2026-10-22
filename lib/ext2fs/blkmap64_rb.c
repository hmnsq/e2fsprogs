/*
 * blkmap64_rb.c --- Simple rb-tree implementation for bitmaps
 *
 * (C)2010 Red Hat, Inc., Lukas Czerner <lczerner@redhat.com>
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
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_LINUX_TYPES_H
#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"
#ಅನ್ತರ್ಭಾವ "rbtree.h"

#ಅನ್ತರ್ಭಾವ <limits.h>

ರಚನೆ bmap_rb_extent {
	ರಚನೆ rb_ಗಂಟು ಗಂಟು;
	__u64 ಆರಂಭ;
	__u64 ಎಣಿಕೆ_2;
};

ರಚನೆ ext2fs_rb_ಖಾಸಗಿ {
	ರಚನೆ rb_ಬೇರು ಬೇರು;
	ರಚನೆ bmap_rb_extent *wcursor;
	ರಚನೆ bmap_rb_extent *rcursor;
	ರಚನೆ bmap_rb_extent *rcursor_ಮುಂದಿನ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	__u64 mark_ದೊರಕಿತು;
	__u64 test_ದೊರಕಿತು;
#ಆದರೆ_ಅಂತ್ಯ
};

ಸಾಲೊಳಗೆ ಸ್ಥಿರ ರಚನೆ bmap_rb_extent *ಗಂಟು_ಗಮ್ಯ_extent(ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ಹಿಂತಿರುಗಿ (ರಚನೆ bmap_rb_extent *) ಗಂಟು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_ಸೇರಿಸು_extent(__u64 ಆರಂಭ, __u64 ಎಣಿಕೆ_2,
			    ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *);
ಸ್ಥಿರ ಶೂನ್ಯ rb_get_new_extent(ರಚನೆ bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_RB
ಸ್ಥಿರ ಶೂನ್ಯ print_ವೃಕ್ಷ(ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
	ರಚನೆ bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ಗಂಟು = ext2fs_rb_ಮೊದಲ(ಬೇರು);
	ಗಾಗಿ (ಗಂಟು = ext2fs_rb_ಮೊದಲ(ಬೇರು); ಗಂಟು != ಖಾಲಿ; 
	     ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು)) {
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ,
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ check_ವೃಕ್ಷ(ರಚನೆ rb_ಬೇರು *ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು;
	ರಚನೆ bmap_rb_extent *ext, *old = ಖಾಲಿ;

	ಗಾಗಿ (ಗಂಟು = ext2fs_rb_ಮೊದಲ(ಬೇರು); ಗಂಟು;
	     ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು)) {
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		ಆದರೆ (ext->ಎಣಿಕೆ_2 == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ,
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2,
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಎಣಿಕೆ_2);
			ಹೋಗು err_ಹೊರಹೋಗು;
		}
		ಆದರೆ (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2 < ext->ಆರಂಭ) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ,
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2,
				(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಎಣಿಕೆ_2);
			ಹೋಗು err_ಹೊರಹೋಗು;
		}

		ಆದರೆ (old) {
			ಆದರೆ (old->ಆರಂಭ > ext->ಆರಂಭ) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಆರಂಭ,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಆರಂಭ + old->ಎಣಿಕೆ_2,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಎಣಿಕೆ_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಎಣಿಕೆ_2);
				ಹೋಗು err_ಹೊರಹೋಗು;
			}
			ಆದರೆ ((old->ಆರಂಭ + old->ಎಣಿಕೆ_2) >= ext->ಆರಂಭ) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಆರಂಭ,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಆರಂಭ + old->ಎಣಿಕೆ_2,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) old->ಎಣಿಕೆ_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2,
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ext->ಎಣಿಕೆ_2);
				ಹೋಗು err_ಹೊರಹೋಗು;
			}
		}
		old = ext;
	}
	ಹಿಂತಿರುಗಿ;

err_ಹೊರಹೋಗು:
	fprintf(stderr, "%s\n", ಸಂದೇಶ);
	print_ವೃಕ್ಷ(ಬೇರು);
	ನಿರ್ಗಮಿಸು(1);
}
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು check_ವೃಕ್ಷ(ಬೇರು, ಸಂದೇಶ) ಮಾಡು {} ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು print_ವೃಕ್ಷ(ಬೇರು) ಮಾಡು {} ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ rb_get_new_extent(ರಚನೆ bmap_rb_extent **ext, __u64 ಆರಂಭ,
			     __u64 ಎಣಿಕೆ_2)
{
	ರಚನೆ bmap_rb_extent *new_ext;
	ಪೂರ್ಣಾಂಕ retval;

	retval = ext2fs_get_mem(ಗಾತ್ರ (ರಚನೆ bmap_rb_extent),
				&new_ext);
	ಆದರೆ (retval)
		abort();

	new_ext->ಆರಂಭ = ಆರಂಭ;
	new_ext->ಎಣಿಕೆ_2 = ಎಣಿಕೆ_2;
	*ext = new_ext;
}

ಸಾಲೊಳಗೆ
ಸ್ಥಿರ ಶೂನ್ಯ rb_free_extent(ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp,
			   ರಚನೆ bmap_rb_extent *ext)
{
	ಆದರೆ (bp->wcursor == ext)
		bp->wcursor = ಖಾಲಿ;
	ಆದರೆ (bp->rcursor == ext)
		bp->rcursor = ಖಾಲಿ;
	ಆದರೆ (bp->rcursor_ಮುಂದಿನ == ext)
		bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;
	ext2fs_free_mem(&ext);
}

ಸ್ಥಿರ errcode_t rb_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (ext2fs_generic_bitmap_64 bitmap)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(ಗಾತ್ರ (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ), &bp);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	bp->ಬೇರು = RB_ಬೇರು;
	bp->rcursor = ಖಾಲಿ;
	bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;
	bp->wcursor = ಖಾಲಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	bp->test_ದೊರಕಿತು = 0;
	bp->mark_ದೊರಕಿತು = 0;
#ಆದರೆ_ಅಂತ್ಯ

	bitmap->ಖಾಸಗಿ = (ಶೂನ್ಯ *) bp;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((ಬಳಸದ)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (bitmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_free_ವೃಕ್ಷ(ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ bmap_rb_extent *ext;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು, *ಮುಂದಿನ;

	ಗಾಗಿ (ಗಂಟು = ext2fs_rb_ಮೊದಲ(ಬೇರು); ಗಂಟು; ಗಂಟು = ಮುಂದಿನ) {
		ಮುಂದಿನ = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು);
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
		ext2fs_free_mem(&ext);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;

	rb_free_ವೃಕ್ಷ(&bp->ಬೇರು);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ಸ್ಥಿರ errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *src_bp, *dest_bp;
	ರಚನೆ bmap_rb_extent *src_ext, *dest_ext;
	ರಚನೆ rb_ಗಂಟು *dest_ಗಂಟು, *src_ಗಂಟು, *dest_ಕೊನೆಯ, **n;
	errcode_t retval = 0;

	retval = rb_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (dest);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	src_bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) src->ಖಾಸಗಿ;
	dest_bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) dest->ಖಾಸಗಿ;
	src_bp->rcursor = ಖಾಲಿ;
	dest_bp->rcursor = ಖಾಲಿ;

	src_ಗಂಟು = ext2fs_rb_ಮೊದಲ(&src_bp->ಬೇರು);
	ಆಗ (src_ಗಂಟು) {
		src_ext = ಗಂಟು_ಗಮ್ಯ_extent(src_ಗಂಟು);
		retval = ext2fs_get_mem(ಗಾತ್ರ (ರಚನೆ bmap_rb_extent),
					&dest_ext);
		ಆದರೆ (retval)
			ನಿಲ್ಲಿಸು;

		ಸ್ಮರಣೆ_ನಕಲು(dest_ext, src_ext, ಗಾತ್ರ(ರಚನೆ bmap_rb_extent));

		dest_ಗಂಟು = &dest_ext->ಗಂಟು;
		n = &dest_bp->ಬೇರು.rb_ಗಂಟು;

		dest_ಕೊನೆಯ = ಖಾಲಿ;
		ಆದರೆ (*n) {
			dest_ಕೊನೆಯ = ext2fs_rb_ಕೊನೆಯ(&dest_bp->ಬೇರು);
			n = &(dest_ಕೊನೆಯ)->rb_ಬಲ;
		}

		ext2fs_rb_link_ಗಂಟು(dest_ಗಂಟು, dest_ಕೊನೆಯ, n);
		ext2fs_rb_ಸೇರಿಸು_color(dest_ಗಂಟು, &dest_bp->ಬೇರು);

		src_ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(src_ಗಂಟು);
	}

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_truncate(__u64 new_max, ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ರಚನೆ bmap_rb_extent *ext;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು;

	ಗಂಟು = ext2fs_rb_ಕೊನೆಯ(ಬೇರು);
	ಆಗ (ಗಂಟು) {
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);

		ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2 - 1) <= new_max)
			ನಿಲ್ಲಿಸು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ext->ಆರಂಭ > new_max) {
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			ext2fs_free_mem(&ext);
			ಗಂಟು = ext2fs_rb_ಕೊನೆಯ(ಬೇರು);
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ
			ext->ಎಣಿಕೆ_2 = new_max - ext->ಆರಂಭ + 1;
	}
}

ಸ್ಥಿರ errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_ಕೊನೆ, __u64 new_real_ಕೊನೆ)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bmap->ಖಾಸಗಿ;
	bp->rcursor = ಖಾಲಿ;
	bp->wcursor = ಖಾಲಿ;

	rb_truncate(((new_ಕೊನೆ < bmap->ಕೊನೆ) ? new_ಕೊನೆ : bmap->ಕೊನೆ) - bmap->ಆರಂಭ,
		    &bp->ಬೇರು);

	bmap->ಕೊನೆ = new_ಕೊನೆ;
	bmap->real_ಕೊನೆ = new_real_ಕೊನೆ;

	ಆದರೆ (bmap->ಕೊನೆ < bmap->real_ಕೊನೆ)
		rb_ಸೇರಿಸು_extent(bmap->ಕೊನೆ + 1 - bmap->ಆರಂಭ,
				 bmap->real_ಕೊನೆ - bmap->ಕೊನೆ, bp);
	ಹಿಂತಿರುಗಿ 0;

}

ಸಾಲೊಳಗೆ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rb_test_ಬಿಟ್(ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp, __u64 ಬಿಟ್)
{
	ರಚನೆ bmap_rb_extent *rcursor, *ಮುಂದಿನ_ext = ಖಾಲಿ;
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, *ಮುಂದಿನ;
	ರಚನೆ rb_ಗಂಟು **n = &bp->ಬೇರು.rb_ಗಂಟು;
	ರಚನೆ bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ಆದರೆ (!rcursor)
		ಹೋಗು ಹುಡುಕು_ವೃಕ್ಷ;

	ಆದರೆ (ಬಿಟ್ >= rcursor->ಆರಂಭ && ಬಿಟ್ < rcursor->ಆರಂಭ + rcursor->ಎಣಿಕೆ_2) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
		bp->test_ದೊರಕಿತು++;
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 1;
	}

	ಮುಂದಿನ_ext = bp->rcursor_ಮುಂದಿನ;
	ಆದರೆ (!ಮುಂದಿನ_ext) {
		ಮುಂದಿನ = ext2fs_rb_ಮುಂದಿನ(&rcursor->ಗಂಟು);
		ಆದರೆ (ಮುಂದಿನ)
			ಮುಂದಿನ_ext = ಗಂಟು_ಗಮ್ಯ_extent(ಮುಂದಿನ);
		bp->rcursor_ಮುಂದಿನ = ಮುಂದಿನ_ext;
	}
	ಆದರೆ (ಮುಂದಿನ_ext) {
		ಆದರೆ ((ಬಿಟ್ >= rcursor->ಆರಂಭ + rcursor->ಎಣಿಕೆ_2) &&
		    (ಬಿಟ್ < ಮುಂದಿನ_ext->ಆರಂಭ)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BMAP_STATS_OPS
			bp->test_ದೊರಕಿತು++;
#ಆದರೆ_ಅಂತ್ಯ
			ಹಿಂತಿರುಗಿ 0;
		}
	}
	bp->rcursor = ಖಾಲಿ;
	bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;

	rcursor = bp->wcursor;
	ಆದರೆ (!rcursor)
		ಹೋಗು ಹುಡುಕು_ವೃಕ್ಷ;

	ಆದರೆ (ಬಿಟ್ >= rcursor->ಆರಂಭ && ಬಿಟ್ < rcursor->ಆರಂಭ + rcursor->ಎಣಿಕೆ_2)
		ಹಿಂತಿರುಗಿ 1;

ಹುಡುಕು_ವೃಕ್ಷ:

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಬಿಟ್ < ext->ಆರಂಭ)
			n = &(*n)->rb_ಎಡ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಿಟ್ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2))
			n = &(*n)->rb_ಬಲ;
		ಇಲ್ಲದಿದ್ದರೆ {
			bp->rcursor = ext;
			bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_ಸೇರಿಸು_extent(__u64 ಆರಂಭ, __u64 ಎಣಿಕೆ_2,
			    ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp)
{
	ರಚನೆ rb_ಬೇರು *ಬೇರು = &bp->ಬೇರು;
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, **n = &ಬೇರು->rb_ಗಂಟು;
	ರಚನೆ rb_ಗಂಟು *new_ಗಂಟು, *ಗಂಟು, *ಮುಂದಿನ;
	ರಚನೆ bmap_rb_extent *new_ext;
	ರಚನೆ bmap_rb_extent *ext;
	ಪೂರ್ಣಾಂಕ retval = 0;

	ಆದರೆ (ಎಣಿಕೆ_2 == 0)
		ಹಿಂತಿರುಗಿ 0;

	bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;
	ext = bp->wcursor;
	ಆದರೆ (ext) {
		ಆದರೆ (ಆರಂಭ >= ext->ಆರಂಭ &&
		    ಆರಂಭ <= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
			bp->mark_ದೊರಕಿತು++;
#ಆದರೆ_ಅಂತ್ಯ
			ಹೋಗು got_extent;
		}
	}

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);

		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ > (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
		} ಇಲ್ಲದಿದ್ದರೆ {
got_extent:
			ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) <= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2))
				ಹಿಂತಿರುಗಿ 1;

			ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) == ಆರಂಭ)
				retval = 0;
			ಇಲ್ಲದಿದ್ದರೆ
				retval = 1;

			ಎಣಿಕೆ_2 += (ಆರಂಭ - ext->ಆರಂಭ);
			ಆರಂಭ = ext->ಆರಂಭ;
			new_ext = ext;
			new_ಗಂಟು = &ext->ಗಂಟು;

			ಹೋಗು skip_ಸೇರಿಸು;
		}
	}

	rb_get_new_extent(&new_ext, ಆರಂಭ, ಎಣಿಕೆ_2);

	new_ಗಂಟು = &new_ext->ಗಂಟು;
	ext2fs_rb_link_ಗಂಟು(new_ಗಂಟು, ಪೋಷಕ, n);
	ext2fs_rb_ಸೇರಿಸು_color(new_ಗಂಟು, ಬೇರು);
	bp->wcursor = new_ext;

	ಗಂಟು = ext2fs_rb_prev(new_ಗಂಟು);
	ಆದರೆ (ಗಂಟು) {
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) == ಆರಂಭ) {
			ಆರಂಭ = ext->ಆರಂಭ;
			ಎಣಿಕೆ_2 += ext->ಎಣಿಕೆ_2;
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
		}
	}

skip_ಸೇರಿಸು:
	/* See if we can merge extent to the right */
	ಗಾಗಿ (ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(new_ಗಂಟು); ಗಂಟು != ಖಾಲಿ; ಗಂಟು = ಮುಂದಿನ) {
		ಮುಂದಿನ = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು);
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);

		ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) <= ಆರಂಭ)
			ಮುಂದುವರಿ;

		/* No more merging */
		ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) < ext->ಆರಂಭ)
			ನಿಲ್ಲಿಸು;

		/* ext is embedded in new_ext interval */
		ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
		/* merge ext with new_ext */
			ಎಣಿಕೆ_2 += ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) -
				  (ಆರಂಭ + ಎಣಿಕೆ_2));
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ನಿಲ್ಲಿಸು;
		}
	}

	new_ext->ಆರಂಭ = ಆರಂಭ;
	new_ext->ಎಣಿಕೆ_2 = ಎಣಿಕೆ_2;

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_remove_extent(__u64 ಆರಂಭ, __u64 ಎಣಿಕೆ_2,
			    ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp)
{
	ರಚನೆ rb_ಬೇರು *ಬೇರು = &bp->ಬೇರು;
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, **n = &ಬೇರು->rb_ಗಂಟು;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು;
	ರಚನೆ bmap_rb_extent *ext;
	__u64 new_ಆರಂಭ, new_ಎಣಿಕೆ;
	ಪೂರ್ಣಾಂಕ retval = 0;

	ಆದರೆ (ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(ಬೇರು))
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ ((ಆರಂಭ > ext->ಆರಂಭ) &&
		    (ಆರಂಭ + ಎಣಿಕೆ_2) < (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			/* We have to split extent into two */
			new_ಆರಂಭ = ಆರಂಭ + ಎಣಿಕೆ_2;
			new_ಎಣಿಕೆ = (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) - new_ಆರಂಭ;

			ext->ಎಣಿಕೆ_2 = ಆರಂಭ - ext->ಆರಂಭ;

			rb_ಸೇರಿಸು_extent(new_ಆರಂಭ, new_ಎಣಿಕೆ, bp);
			ಹಿಂತಿರುಗಿ 1;
		}

		ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			ext->ಎಣಿಕೆ_2 = ಆರಂಭ - ext->ಆರಂಭ;
			retval = 1;
		}

		ಆದರೆ (0 == ext->ಎಣಿಕೆ_2) {
			ಪೋಷಕ = ext2fs_rb_ಮುಂದಿನ(&ext->ಗಂಟು);
			ext2fs_rb_erase(&ext->ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (ಆರಂಭ == ext->ಆರಂಭ) {
			ext->ಆರಂಭ += ಎಣಿಕೆ_2;
			ext->ಎಣಿಕೆ_2 -= ಎಣಿಕೆ_2;
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	ಗಾಗಿ (; ಪೋಷಕ != ಖಾಲಿ; ಪೋಷಕ = ಗಂಟು) {
		ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(ಪೋಷಕ);
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) <= ಆರಂಭ)
			ಮುಂದುವರಿ;

		/* No more extents to be removed/truncated */
		ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) < ext->ಆರಂಭ)
			ನಿಲ್ಲಿಸು;

		/* The entire extent is within the region to be removed */
		ಆದರೆ ((ಆರಂಭ + ಎಣಿಕೆ_2) >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			ext2fs_rb_erase(ಪೋಷಕ, ಬೇರು);
			rb_free_extent(bp, ext);
			retval = 1;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* modify the last extent in region to be removed */
			ext->ಎಣಿಕೆ_2 -= ((ಆರಂಭ + ಎಣಿಕೆ_2) - ext->ಆರಂಭ);
			ext->ಆರಂಭ = ಆರಂಭ + ಎಣಿಕೆ_2;
			retval = 1;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ಪೂರ್ಣಾಂಕ retval;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	arg -= bitmap->ಆರಂಭ;

	retval = rb_ಸೇರಿಸು_extent(arg, 1, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ಪೂರ್ಣಾಂಕ retval;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	arg -= bitmap->ಆರಂಭ;

	retval = rb_remove_extent(arg, 1, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);

	ಹಿಂತಿರುಗಿ retval;
}

ಸಾಲೊಳಗೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	arg -= bitmap->ಆರಂಭ;

	ಹಿಂತಿರುಗಿ rb_test_ಬಿಟ್(bp, arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	arg -= bitmap->ಆರಂಭ;

	rb_ಸೇರಿಸು_extent(arg, ಅಂಕೆ, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	arg -= bitmap->ಆರಂಭ;

	rb_remove_extent(arg, ಅಂಕೆ, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, **n;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು, *ಮುಂದಿನ;
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ರಚನೆ bmap_rb_extent *ext;
	ಪೂರ್ಣಾಂಕ retval = 1;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	ಆರಂಭ -= bitmap->ಆರಂಭ;

	ಆದರೆ (len == 0 || ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(&bp->ಬೇರು))
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಗಂಟು = ಪೋಷಕ;
	ಆಗ (ಗಂಟು) {
		ಮುಂದಿನ = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು);
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		ಗಂಟು = ಮುಂದಿನ;

		ಆದರೆ ((ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2) <= ಆರಂಭ)
			ಮುಂದುವರಿ;

		/* No more merging */
		ಆದರೆ ((ಆರಂಭ + len) <= ext->ಆರಂಭ)
			ನಿಲ್ಲಿಸು;

		retval = 0;
		ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t rb_ಗಣ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಒಳ)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *cp = ಒಳ;
	size_t i;
	ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಗಣ = -1;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ; i++) {
		ಆದರೆ ((i & 7) == 0) {
			ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = cp[i/8];
			ಆದರೆ (c == 0xFF) {
				ಆದರೆ (ಮೊದಲ_ಗಣ == -1)
					ಮೊದಲ_ಗಣ = i;
				i += 7;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ ((c == 0x00) && (ಮೊದಲ_ಗಣ == -1)) {
				i += 7;
				ಮುಂದುವರಿ;
			}
		}
		ಆದರೆ (ext2fs_test_ಬಿಟ್(i, ಒಳ)) {
			ಆದರೆ (ಮೊದಲ_ಗಣ == -1)
				ಮೊದಲ_ಗಣ = i;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (ಮೊದಲ_ಗಣ == -1)
			ಮುಂದುವರಿ;

		rb_ಸೇರಿಸು_extent(ಆರಂಭ + ಮೊದಲ_ಗಣ - bitmap->ಆರಂಭ,
				 i - ಮೊದಲ_ಗಣ, bp);
		check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
		ಮೊದಲ_ಗಣ = -1;
	}
	ಆದರೆ (ಮೊದಲ_ಗಣ != -1) {
		rb_ಸೇರಿಸು_extent(ಆರಂಭ + ಮೊದಲ_ಗಣ - bitmap->ಆರಂಭ,
				 ಅಂಕೆ - ಮೊದಲ_ಗಣ, bp);
		check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಹೊರಹೋಗು)
{

	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, *ಮುಂದಿನ, **n;
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ರಚನೆ bmap_rb_extent *ext;
	__u64 ಎಣಿಕೆ_2, pos;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	ಆರಂಭ -= bitmap->ಆರಂಭ;

	ಆದರೆ (ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(&bp->ಬೇರು))
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
		} ಇಲ್ಲದಿದ್ದರೆ
			ನಿಲ್ಲಿಸು;
	}

	memset(ಹೊರಹೋಗು, 0, (ಅಂಕೆ + 7) >> 3);

	ಗಾಗಿ (; ಪೋಷಕ != ಖಾಲಿ; ಪೋಷಕ = ಮುಂದಿನ) {
		ಮುಂದಿನ = ext2fs_rb_ಮುಂದಿನ(ಪೋಷಕ);
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);

		pos = ext->ಆರಂಭ;
		ಎಣಿಕೆ_2 = ext->ಎಣಿಕೆ_2;
		ಆದರೆ (pos >= ಆರಂಭ + ಅಂಕೆ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (pos < ಆರಂಭ) {
			ಆದರೆ (pos + ಎಣಿಕೆ_2 <  ಆರಂಭ)
				ಮುಂದುವರಿ;
			ಎಣಿಕೆ_2 -= ಆರಂಭ - pos;
			pos = ಆರಂಭ;
		}
		ಆದರೆ (pos + ಎಣಿಕೆ_2 > ಆರಂಭ + ಅಂಕೆ)
			ಎಣಿಕೆ_2 = ಆರಂಭ + ಅಂಕೆ - pos;

		ಆಗ (ಎಣಿಕೆ_2 > 0) {
			ಆದರೆ ((ಎಣಿಕೆ_2 >= 8) &&
			    ((pos - ಆರಂಭ) % 8) == 0) {
				ಪೂರ್ಣಾಂಕ nbytes = ಎಣಿಕೆ_2 >> 3;
				ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = (pos - ಆರಂಭ) >> 3;

				memset(((ಅಕ್ಷರ *) ಹೊರಹೋಗು) + ಸ್ಥಾನಾಂತರ, 0xFF, nbytes);
				pos += nbytes << 3;
				ಎಣಿಕೆ_2 -= nbytes << 3;
				ಮುಂದುವರಿ;
			}
			ext2fs_fast_ಗಣ_bit64((pos - ಆರಂಭ), ಹೊರಹೋಗು);
			pos++;
			ಎಣಿಕೆ_2--;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;

	rb_free_ವೃಕ್ಷ(&bp->ಬೇರು);
	bp->rcursor = ಖಾಲಿ;
	bp->rcursor_ಮುಂದಿನ = ಖಾಲಿ;
	bp->wcursor = ಖಾಲಿ;
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ errcode_t rb_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, **n;
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ರಚನೆ bmap_rb_extent *ext;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	ಆರಂಭ -= bitmap->ಆರಂಭ;
	ಕೊನೆ -= bitmap->ಆರಂಭ;

	ಆದರೆ (ಆರಂಭ > ಕೊನೆ)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(&bp->ಬೇರು))
		ಹಿಂತಿರುಗಿ ENOENT;

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2 <= ಕೊನೆ) {
			*ಹೊರಹೋಗು = ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2 + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ ENOENT;
	}

	*ಹೊರಹೋಗು = ಆರಂಭ + bitmap->ಆರಂಭ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t rb_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ರಚನೆ rb_ಗಂಟು *ಪೋಷಕ = ಖಾಲಿ, **n;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು;
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ರಚನೆ bmap_rb_extent *ext;

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	ಆರಂಭ -= bitmap->ಆರಂಭ;
	ಕೊನೆ -= bitmap->ಆರಂಭ;

	ಆದರೆ (ಆರಂಭ > ಕೊನೆ)
		ಹಿಂತಿರುಗಿ EINVAL;

	ಆದರೆ (ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(&bp->ಬೇರು))
		ಹಿಂತಿರುಗಿ ENOENT;

	ಆಗ (*n) {
		ಪೋಷಕ = *n;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಪೋಷಕ);
		ಆದರೆ (ಆರಂಭ < ext->ಆರಂಭ) {
			n = &(*n)->rb_ಎಡ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆರಂಭ >= (ext->ಆರಂಭ + ext->ಎಣಿಕೆ_2)) {
			n = &(*n)->rb_ಬಲ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* The start bit is set */
			*ಹೊರಹೋಗು = ಆರಂಭ + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಗಂಟು = ಪೋಷಕ;
	ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
	ಆದರೆ (ext->ಆರಂಭ < ಆರಂಭ) {
		ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು);
		ಆದರೆ (ಗಂಟು == ಖಾಲಿ)
			ಹಿಂತಿರುಗಿ ENOENT;
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
	}
	ಆದರೆ (ext->ಆರಂಭ <= ಕೊನೆ) {
		*ಹೊರಹೋಗು = ext->ಆರಂಭ + bitmap->ಆರಂಭ;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ENOENT;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
ಸ್ಥಿರ ಶೂನ್ಯ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *bp;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
	ರಚನೆ bmap_rb_extent *ext;
	__u64 ಎಣಿಕೆ_2 = 0;
	__u64 max_ಗಾತ್ರ = 0;
	__u64 min_ಗಾತ್ರ = ULONG_MAX;
	__u64 ಗಾತ್ರ_2 = 0, avg_ಗಾತ್ರ = 0;
	ದ್ವಿಗುಣ eff;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ದ್ವಿಗುಣ m_ದೊರಕಿತು = 0.0, t_ದೊರಕಿತು = 0.0;
#ಆದರೆ_ಅಂತ್ಯ

	bp = (ರಚನೆ ext2fs_rb_ಖಾಸಗಿ *) bitmap->ಖಾಸಗಿ;

	ಗಾಗಿ (ಗಂಟು = ext2fs_rb_ಮೊದಲ(&bp->ಬೇರು); ಗಂಟು != ಖಾಲಿ;
	     ಗಂಟು = ext2fs_rb_ಮುಂದಿನ(ಗಂಟು)) {
		ext = ಗಂಟು_ಗಮ್ಯ_extent(ಗಂಟು);
		ಎಣಿಕೆ_2++;
		ಆದರೆ (ext->ಎಣಿಕೆ_2 > max_ಗಾತ್ರ)
			max_ಗಾತ್ರ = ext->ಎಣಿಕೆ_2;
		ಆದರೆ (ext->ಎಣಿಕೆ_2 < min_ಗಾತ್ರ)
			min_ಗಾತ್ರ = ext->ಎಣಿಕೆ_2;
		ಗಾತ್ರ_2 += ext->ಎಣಿಕೆ_2;
	}

	ಆದರೆ (ಎಣಿಕೆ_2)
		avg_ಗಾತ್ರ = ಗಾತ್ರ_2 / ಎಣಿಕೆ_2;
	ಆದರೆ (min_ಗಾತ್ರ == ULONG_MAX)
		min_ಗಾತ್ರ = 0;
	eff = (ದ್ವಿಗುಣ)((ಎಣಿಕೆ_2 * ಗಾತ್ರ(ರಚನೆ bmap_rb_extent)) << 3) /
	      (bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ಎಣಿಕೆ + bitmap->stats.mark_ext_ಎಣಿಕೆ;
	test_all = bitmap->stats.test_ಎಣಿಕೆ + bitmap->stats.test_ext_ಎಣಿಕೆ;
	ಆದರೆ (mark_all)
		m_ದೊರಕಿತು = ((ದ್ವಿಗುಣ)bp->mark_ದೊರಕಿತು / mark_all) * 100;
	ಆದರೆ (test_all)
		t_ದೊರಕಿತು = ((ದ್ವಿಗುಣ)bp->test_ದೊರಕಿತು / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_ದೊರಕಿತು, t_ದೊರಕಿತು, bp->mark_ದೊರಕಿತು, m_ದೊರಕಿತು);
#ಆದರೆ_ಅಂತ್ಯ
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಎಣಿಕೆ_2, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)
		((ಎಣಿಕೆ_2 * ಗಾತ್ರ(ರಚನೆ bmap_rb_extent)) +
		 ಗಾತ್ರ(ರಚನೆ ext2fs_rb_ಖಾಸಗಿ)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) min_ಗಾತ್ರ);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) max_ಗಾತ್ರ, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) avg_ಗಾತ್ರ);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಗಾತ್ರ_2,
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಶೂನ್ಯ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ಬಳಸದ)))
{
}
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
	.type = EXT2FS_BMAP64_RBTREE,
	.new_bmap = rb_new_bmap,
	.free_bmap = rb_free_bmap,
	.copy_bmap = rb_copy_bmap,
	.resize_bmap = rb_resize_bmap,
	.mark_bmap = rb_mark_bmap,
	.unmark_bmap = rb_unmark_bmap,
	.test_bmap = rb_test_bmap,
	.test_clear_bmap_extent = rb_test_clear_bmap_extent,
	.mark_bmap_extent = rb_mark_bmap_extent,
	.unmark_bmap_extent = rb_unmark_bmap_extent,
	.ಗಣ_bmap_range = rb_ಗಣ_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero = rb_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero,
	.ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ = rb_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ,
};
