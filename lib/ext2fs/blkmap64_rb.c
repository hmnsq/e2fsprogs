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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_LINUX_TYPES_H
#ಅನ್ತರ್ಭಾವ <linux/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"
#ಅನ್ತರ್ಭಾವ "rbtree.h"

#ಅನ್ತರ್ಭಾವ <limits.h>

ಸಂರಚನಾ bmap_rb_extent {
	ಸಂರಚನಾ rb_ಗಂಟು ಗಂಟು;
	__u64 start;
	__u64 ಎಣಿಕೆ;
};

ಸಂರಚನಾ ext2fs_rb_private {
	ಸಂರಚನಾ rb_ಬೇರು ಬೇರು;
	ಸಂರಚನಾ bmap_rb_extent *wcursor;
	ಸಂರಚನಾ bmap_rb_extent *rcursor;
	ಸಂರಚನಾ bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಸಂರಚನಾ bmap_rb_extent *ಗಂಟು_to_extent(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ bmap_rb_extent *) ಗಂಟು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_ಸೇರಿಸು_extent(__u64 start, __u64 ಎಣಿಕೆ,
			    ಸಂರಚನಾ ext2fs_rb_private *);
ಸ್ಥಿರ ಶೂನ್ಯ rb_get_new_extent(ಸಂರಚನಾ bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ಸ್ಥಿರ ಶೂನ್ಯ print_ವೃಕ್ಷ(ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
	ಸಂರಚನಾ bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ಗಂಟು = ext2fs_rb_first(ಬೇರು);
	ಪ್ರತಿ (ಗಂಟು = ext2fs_rb_first(ಬೇರು); ಗಂಟು != ರಿಕ್ತ; 
	     ಗಂಟು = ext2fs_rb_next(ಗಂಟು)) {
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->ಎಣಿಕೆ);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ check_ವೃಕ್ಷ(ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು;
	ಸಂರಚನಾ bmap_rb_extent *ext, *old = ರಿಕ್ತ;

	ಪ್ರತಿ (ಗಂಟು = ext2fs_rb_first(ಬೇರು); ಗಂಟು;
	     ಗಂಟು = ext2fs_rb_next(ಗಂಟು)) {
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		ಯದಿ (ext->ಎಣಿಕೆ == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ಎಣಿಕೆ,
				(unsigned long long) ext->ಎಣಿಕೆ);
			ಗಚ್ಛ err_out;
		}
		ಯದಿ (ext->start + ext->ಎಣಿಕೆ < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ಎಣಿಕೆ,
				(unsigned long long) ext->ಎಣಿಕೆ);
			ಗಚ್ಛ err_out;
		}

		ಯದಿ (old) {
			ಯದಿ (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ಎಣಿಕೆ,
					(unsigned long long) old->ಎಣಿಕೆ);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ಎಣಿಕೆ,
					(unsigned long long) ext->ಎಣಿಕೆ);
				ಗಚ್ಛ err_out;
			}
			ಯದಿ ((old->start + old->ಎಣಿಕೆ) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ಎಣಿಕೆ,
					(unsigned long long) old->ಎಣಿಕೆ);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ಎಣಿಕೆ,
					(unsigned long long) ext->ಎಣಿಕೆ);
				ಗಚ್ಛ err_out;
			}
		}
		old = ext;
	}
	ಪ್ರತ್ಯಾಗಮ;

err_out:
	fprintf(stderr, "%s\n", ಸಂದೇಶ);
	print_ವೃಕ್ಷ(ಬೇರು);
	exit(1);
}
#ಅನ್ಯಥಾ
#define check_ವೃಕ್ಷ(ಬೇರು, ಸಂದೇಶ) ಕುರು {} ಯಾವತ್ (0)
#define print_ವೃಕ್ಷ(ಬೇರು) ಕುರು {} ಯಾವತ್ (0)
#endif

ಸ್ಥಿರ ಶೂನ್ಯ rb_get_new_extent(ಸಂರಚನಾ bmap_rb_extent **ext, __u64 start,
			     __u64 ಎಣಿಕೆ)
{
	ಸಂರಚನಾ bmap_rb_extent *new_ext;
	ಪೂರ್ಣಾಙ್ಕ retval;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ (ಸಂರಚನಾ bmap_rb_extent),
				&new_ext);
	ಯದಿ (retval)
		abort();

	new_ext->start = start;
	new_ext->ಎಣಿಕೆ = ಎಣಿಕೆ;
	*ext = new_ext;
}

ಅನ್ತಃಸ್ಥ
ಸ್ಥಿರ ಶೂನ್ಯ rb_free_extent(ಸಂರಚನಾ ext2fs_rb_private *bp,
			   ಸಂರಚನಾ bmap_rb_extent *ext)
{
	ಯದಿ (bp->wcursor == ext)
		bp->wcursor = ರಿಕ್ತ;
	ಯದಿ (bp->rcursor == ext)
		bp->rcursor = ರಿಕ್ತ;
	ಯದಿ (bp->rcursor_next == ext)
		bp->rcursor_next = ರಿಕ್ತ;
	ext2fs_free_mem(&ext);
}

ಸ್ಥಿರ errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ (ಸಂರಚನಾ ext2fs_rb_private), &bp);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	bp->ಬೇರು = RB_ಬೇರು;
	bp->rcursor = ರಿಕ್ತ;
	bp->rcursor_next = ರಿಕ್ತ;
	bp->wcursor = ರಿಕ್ತ;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->ಗುಪ್ತ = (ಶೂನ್ಯ *) bp;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_free_ವೃಕ್ಷ(ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ bmap_rb_extent *ext;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, *next;

	ಪ್ರತಿ (ಗಂಟು = ext2fs_rb_first(ಬೇರು); ಗಂಟು; ಗಂಟು = next) {
		next = ext2fs_rb_next(ಗಂಟು);
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
		ext2fs_free_mem(&ext);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;

	rb_free_ವೃಕ್ಷ(&bp->ಬೇರು);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ಸ್ಥಿರ errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ಸಂರಚನಾ ext2fs_rb_private *src_bp, *dest_bp;
	ಸಂರಚನಾ bmap_rb_extent *src_ext, *dest_ext;
	ಸಂರಚನಾ rb_ಗಂಟು *dest_ಗಂಟು, *src_ಗಂಟು, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	src_bp = (ಸಂರಚನಾ ext2fs_rb_private *) src->ಗುಪ್ತ;
	dest_bp = (ಸಂರಚನಾ ext2fs_rb_private *) dest->ಗುಪ್ತ;
	src_bp->rcursor = ರಿಕ್ತ;
	dest_bp->rcursor = ರಿಕ್ತ;

	src_ಗಂಟು = ext2fs_rb_first(&src_bp->ಬೇರು);
	ಯಾವತ್ (src_ಗಂಟು) {
		src_ext = ಗಂಟು_to_extent(src_ಗಂಟು);
		retval = ext2fs_get_mem(ಪರಿಮಾಣ (ಸಂರಚನಾ bmap_rb_extent),
					&dest_ext);
		ಯದಿ (retval)
			ಭಙ್ಗ;

		memcpy(dest_ext, src_ext, ಪರಿಮಾಣ(ಸಂರಚನಾ bmap_rb_extent));

		dest_ಗಂಟು = &dest_ext->ಗಂಟು;
		n = &dest_bp->ಬೇರು.rb_ಗಂಟು;

		dest_last = ರಿಕ್ತ;
		ಯದಿ (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->ಬೇರು);
			n = &(dest_last)->rb_ಬಲ;
		}

		ext2fs_rb_link_ಗಂಟು(dest_ಗಂಟು, dest_last, n);
		ext2fs_rb_ಸೇರಿಸು_color(dest_ಗಂಟು, &dest_bp->ಬೇರು);

		src_ಗಂಟು = ext2fs_rb_next(src_ಗಂಟು);
	}

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_truncate(__u64 new_max, ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ bmap_rb_extent *ext;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು;

	ಗಂಟು = ext2fs_rb_last(ಬೇರು);
	ಯಾವತ್ (ಗಂಟು) {
		ext = ಗಂಟು_to_extent(ಗಂಟು);

		ಯದಿ ((ext->start + ext->ಎಣಿಕೆ - 1) <= new_max)
			ಭಙ್ಗ;
		ಅನ್ಯಥಾ ಯದಿ (ext->start > new_max) {
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			ext2fs_free_mem(&ext);
			ಗಂಟು = ext2fs_rb_last(ಬೇರು);
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ
			ext->ಎಣಿಕೆ = new_max - ext->start + 1;
	}
}

ಸ್ಥಿರ errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bmap->ಗುಪ್ತ;
	bp->rcursor = ರಿಕ್ತ;
	bp->wcursor = ರಿಕ್ತ;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->ಬೇರು);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	ಯದಿ (bmap->end < bmap->real_end)
		rb_ಸೇರಿಸು_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	ಪ್ರತ್ಯಾಗಮ 0;

}

ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rb_test_bit(ಸಂರಚನಾ ext2fs_rb_private *bp, __u64 bit)
{
	ಸಂರಚನಾ bmap_rb_extent *rcursor, *next_ext = ರಿಕ್ತ;
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, *next;
	ಸಂರಚನಾ rb_ಗಂಟು **n = &bp->ಬೇರು.rb_ಗಂಟು;
	ಸಂರಚನಾ bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ಯದಿ (!rcursor)
		ಗಚ್ಛ ಹುಡುಕು_ವೃಕ್ಷ;

	ಯದಿ (bit >= rcursor->start && bit < rcursor->start + rcursor->ಎಣಿಕೆ) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	next_ext = bp->rcursor_next;
	ಯದಿ (!next_ext) {
		next = ext2fs_rb_next(&rcursor->ಗಂಟು);
		ಯದಿ (next)
			next_ext = ಗಂಟು_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	ಯದಿ (next_ext) {
		ಯದಿ ((bit >= rcursor->start + rcursor->ಎಣಿಕೆ) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}
	bp->rcursor = ರಿಕ್ತ;
	bp->rcursor_next = ರಿಕ್ತ;

	rcursor = bp->wcursor;
	ಯದಿ (!rcursor)
		ಗಚ್ಛ ಹುಡುಕು_ವೃಕ್ಷ;

	ಯದಿ (bit >= rcursor->start && bit < rcursor->start + rcursor->ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ 1;

ಹುಡುಕು_ವೃಕ್ಷ:

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (bit < ext->start)
			n = &(*n)->rb_ಎಡ;
		ಅನ್ಯಥಾ ಯದಿ (bit >= (ext->start + ext->ಎಣಿಕೆ))
			n = &(*n)->rb_ಬಲ;
		ಅನ್ಯಥಾ {
			bp->rcursor = ext;
			bp->rcursor_next = ರಿಕ್ತ;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_ಸೇರಿಸು_extent(__u64 start, __u64 ಎಣಿಕೆ,
			    ಸಂರಚನಾ ext2fs_rb_private *bp)
{
	ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು = &bp->ಬೇರು;
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, **n = &ಬೇರು->rb_ಗಂಟು;
	ಸಂರಚನಾ rb_ಗಂಟು *new_ಗಂಟು, *ಗಂಟು, *next;
	ಸಂರಚನಾ bmap_rb_extent *new_ext;
	ಸಂರಚನಾ bmap_rb_extent *ext;
	ಪೂರ್ಣಾಙ್ಕ retval = 0;

	ಯದಿ (ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	bp->rcursor_next = ರಿಕ್ತ;
	ext = bp->wcursor;
	ಯದಿ (ext) {
		ಯದಿ (start >= ext->start &&
		    start <= (ext->start + ext->ಎಣಿಕೆ)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			ಗಚ್ಛ got_extent;
		}
	}

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);

		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
		} ಅನ್ಯಥಾ ಯದಿ (start > (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
		} ಅನ್ಯಥಾ {
got_extent:
			ಯದಿ ((start + ಎಣಿಕೆ) <= (ext->start + ext->ಎಣಿಕೆ))
				ಪ್ರತ್ಯಾಗಮ 1;

			ಯದಿ ((ext->start + ext->ಎಣಿಕೆ) == start)
				retval = 0;
			ಅನ್ಯಥಾ
				retval = 1;

			ಎಣಿಕೆ += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_ಗಂಟು = &ext->ಗಂಟು;

			ಗಚ್ಛ skip_ಸೇರಿಸು;
		}
	}

	rb_get_new_extent(&new_ext, start, ಎಣಿಕೆ);

	new_ಗಂಟು = &new_ext->ಗಂಟು;
	ext2fs_rb_link_ಗಂಟು(new_ಗಂಟು, parent, n);
	ext2fs_rb_ಸೇರಿಸು_color(new_ಗಂಟು, ಬೇರು);
	bp->wcursor = new_ext;

	ಗಂಟು = ext2fs_rb_prev(new_ಗಂಟು);
	ಯದಿ (ಗಂಟು) {
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		ಯದಿ ((ext->start + ext->ಎಣಿಕೆ) == start) {
			start = ext->start;
			ಎಣಿಕೆ += ext->ಎಣಿಕೆ;
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
		}
	}

skip_ಸೇರಿಸು:
	/* See if we can merge extent to the right */
	ಪ್ರತಿ (ಗಂಟು = ext2fs_rb_next(new_ಗಂಟು); ಗಂಟು != ರಿಕ್ತ; ಗಂಟು = next) {
		next = ext2fs_rb_next(ಗಂಟು);
		ext = ಗಂಟು_to_extent(ಗಂಟು);

		ಯದಿ ((ext->start + ext->ಎಣಿಕೆ) <= start)
			ಅನುವರ್ತ;

		/* No more merging */
		ಯದಿ ((start + ಎಣಿಕೆ) < ext->start)
			ಭಙ್ಗ;

		/* ext is embedded in new_ext interval */
		ಯದಿ ((start + ಎಣಿಕೆ) >= (ext->start + ext->ಎಣಿಕೆ)) {
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ {
		/* merge ext with new_ext */
			ಎಣಿಕೆ += ((ext->start + ext->ಎಣಿಕೆ) -
				  (start + ಎಣಿಕೆ));
			ext2fs_rb_erase(ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ಭಙ್ಗ;
		}
	}

	new_ext->start = start;
	new_ext->ಎಣಿಕೆ = ಎಣಿಕೆ;

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_remove_extent(__u64 start, __u64 ಎಣಿಕೆ,
			    ಸಂರಚನಾ ext2fs_rb_private *bp)
{
	ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು = &bp->ಬೇರು;
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, **n = &ಬೇರು->rb_ಗಂಟು;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು;
	ಸಂರಚನಾ bmap_rb_extent *ext;
	__u64 new_start, new_ಎಣಿಕೆ;
	ಪೂರ್ಣಾಙ್ಕ retval = 0;

	ಯದಿ (ext2fs_rb_empty_ಬೇರು(ಬೇರು))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ ಯದಿ (start >= (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
			ಅನುವರ್ತ;
		}

		ಯದಿ ((start > ext->start) &&
		    (start + ಎಣಿಕೆ) < (ext->start + ext->ಎಣಿಕೆ)) {
			/* We have to split extent into two */
			new_start = start + ಎಣಿಕೆ;
			new_ಎಣಿಕೆ = (ext->start + ext->ಎಣಿಕೆ) - new_start;

			ext->ಎಣಿಕೆ = start - ext->start;

			rb_ಸೇರಿಸು_extent(new_start, new_ಎಣಿಕೆ, bp);
			ಪ್ರತ್ಯಾಗಮ 1;
		}

		ಯದಿ ((start + ಎಣಿಕೆ) >= (ext->start + ext->ಎಣಿಕೆ)) {
			ext->ಎಣಿಕೆ = start - ext->start;
			retval = 1;
		}

		ಯದಿ (0 == ext->ಎಣಿಕೆ) {
			parent = ext2fs_rb_next(&ext->ಗಂಟು);
			ext2fs_rb_erase(&ext->ಗಂಟು, ಬೇರು);
			rb_free_extent(bp, ext);
			ಭಙ್ಗ;
		}

		ಯದಿ (start == ext->start) {
			ext->start += ಎಣಿಕೆ;
			ext->ಎಣಿಕೆ -= ಎಣಿಕೆ;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	ಪ್ರತಿ (; parent != ರಿಕ್ತ; parent = ಗಂಟು) {
		ಗಂಟು = ext2fs_rb_next(parent);
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ ((ext->start + ext->ಎಣಿಕೆ) <= start)
			ಅನುವರ್ತ;

		/* No more extents to be removed/truncated */
		ಯದಿ ((start + ಎಣಿಕೆ) < ext->start)
			ಭಙ್ಗ;

		/* The entire extent is within the region to be removed */
		ಯದಿ ((start + ಎಣಿಕೆ) >= (ext->start + ext->ಎಣಿಕೆ)) {
			ext2fs_rb_erase(parent, ಬೇರು);
			rb_free_extent(bp, ext);
			retval = 1;
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ {
			/* modify the last extent in region to be removed */
			ext->ಎಣಿಕೆ -= ((start + ಎಣಿಕೆ) - ext->start);
			ext->start = start + ಎಣಿಕೆ;
			retval = 1;
			ಭಙ್ಗ;
		}
	}

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಪೂರ್ಣಾಙ್ಕ retval;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	arg -= bitmap->start;

	retval = rb_ಸೇರಿಸು_extent(arg, 1, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಪೂರ್ಣಾಙ್ಕ retval;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಅನ್ತಃಸ್ಥ
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	arg -= bitmap->start;

	ಪ್ರತ್ಯಾಗಮ rb_test_bit(bp, arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	arg -= bitmap->start;

	rb_ಸೇರಿಸು_extent(arg, ಅಂಕೆ, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	arg -= bitmap->start;

	rb_remove_extent(arg, ಅಂಕೆ, bp);
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ len)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, **n;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು, *next;
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಸಂರಚನಾ bmap_rb_extent *ext;
	ಪೂರ್ಣಾಙ್ಕ retval = 1;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	start -= bitmap->start;

	ಯದಿ (len == 0 || ext2fs_rb_empty_ಬೇರು(&bp->ಬೇರು))
		ಪ್ರತ್ಯಾಗಮ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
		} ಅನ್ಯಥಾ ಯದಿ (start >= (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
		} ಅನ್ಯಥಾ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಗಂಟು = parent;
	ಯಾವತ್ (ಗಂಟು) {
		next = ext2fs_rb_next(ಗಂಟು);
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		ಗಂಟು = next;

		ಯದಿ ((ext->start + ext->ಎಣಿಕೆ) <= start)
			ಅನುವರ್ತ;

		/* No more merging */
		ಯದಿ ((start + len) <= ext->start)
			ಭಙ್ಗ;

		retval = 0;
		ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *in)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	unsigned ವರ್ಣ *cp = in;
	size_t i;
	ಪೂರ್ಣಾಙ್ಕ first_set = -1;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ; i++) {
		ಯದಿ ((i & 7) == 0) {
			unsigned ವರ್ಣ c = cp[i/8];
			ಯದಿ (c == 0xFF) {
				ಯದಿ (first_set == -1)
					first_set = i;
				i += 7;
				ಅನುವರ್ತ;
			}
			ಯದಿ ((c == 0x00) && (first_set == -1)) {
				i += 7;
				ಅನುವರ್ತ;
			}
		}
		ಯದಿ (ext2fs_test_bit(i, in)) {
			ಯದಿ (first_set == -1)
				first_set = i;
			ಅನುವರ್ತ;
		}
		ಯದಿ (first_set == -1)
			ಅನುವರ್ತ;

		rb_ಸೇರಿಸು_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
		first_set = -1;
	}
	ಯದಿ (first_set != -1) {
		rb_ಸೇರಿಸು_extent(start + first_set - bitmap->start,
				 ಅಂಕೆ - first_set, bp);
		check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *out)
{

	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, *next, **n;
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಸಂರಚನಾ bmap_rb_extent *ext;
	__u64 ಎಣಿಕೆ, pos;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	start -= bitmap->start;

	ಯದಿ (ext2fs_rb_empty_ಬೇರು(&bp->ಬೇರು))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
		} ಅನ್ಯಥಾ ಯದಿ (start >= (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
		} ಅನ್ಯಥಾ
			ಭಙ್ಗ;
	}

	memset(out, 0, (ಅಂಕೆ + 7) >> 3);

	ಪ್ರತಿ (; parent != ರಿಕ್ತ; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = ಗಂಟು_to_extent(parent);

		pos = ext->start;
		ಎಣಿಕೆ = ext->ಎಣಿಕೆ;
		ಯದಿ (pos >= start + ಅಂಕೆ)
			ಭಙ್ಗ;
		ಯದಿ (pos < start) {
			ಯದಿ (pos + ಎಣಿಕೆ <  start)
				ಅನುವರ್ತ;
			ಎಣಿಕೆ -= start - pos;
			pos = start;
		}
		ಯದಿ (pos + ಎಣಿಕೆ > start + ಅಂಕೆ)
			ಎಣಿಕೆ = start + ಅಂಕೆ - pos;

		ಯಾವತ್ (ಎಣಿಕೆ > 0) {
			ಯದಿ ((ಎಣಿಕೆ >= 8) &&
			    ((pos - start) % 8) == 0) {
				ಪೂರ್ಣಾಙ್ಕ nbytes = ಎಣಿಕೆ >> 3;
				ಪೂರ್ಣಾಙ್ಕ offset = (pos - start) >> 3;

				memset(((ವರ್ಣ *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				ಎಣಿಕೆ -= nbytes << 3;
				ಅನುವರ್ತ;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			ಎಣಿಕೆ--;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;

	rb_free_ವೃಕ್ಷ(&bp->ಬೇರು);
	bp->rcursor = ರಿಕ್ತ;
	bp->rcursor_next = ರಿಕ್ತ;
	bp->wcursor = ರಿಕ್ತ;
	check_ವೃಕ್ಷ(&bp->ಬೇರು, __func__);
}

ಸ್ಥಿರ errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, **n;
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಸಂರಚನಾ bmap_rb_extent *ext;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	start -= bitmap->start;
	end -= bitmap->start;

	ಯದಿ (start > end)
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	ಯದಿ (ext2fs_rb_empty_ಬೇರು(&bp->ಬೇರು))
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
		} ಅನ್ಯಥಾ ಯದಿ (start >= (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
		} ಅನ್ಯಥಾ ಯದಿ (ext->start + ext->ಎಣಿಕೆ <= end) {
			*out = ext->start + ext->ಎಣಿಕೆ + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ ENOENT;
	}

	*out = start + bitmap->start;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ಸಂರಚನಾ rb_ಗಂಟು *parent = ರಿಕ್ತ, **n;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು;
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಸಂರಚನಾ bmap_rb_extent *ext;

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;
	n = &bp->ಬೇರು.rb_ಗಂಟು;
	start -= bitmap->start;
	end -= bitmap->start;

	ಯದಿ (start > end)
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	ಯದಿ (ext2fs_rb_empty_ಬೇರು(&bp->ಬೇರು))
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	ಯಾವತ್ (*n) {
		parent = *n;
		ext = ಗಂಟು_to_extent(parent);
		ಯದಿ (start < ext->start) {
			n = &(*n)->rb_ಎಡ;
		} ಅನ್ಯಥಾ ಯದಿ (start >= (ext->start + ext->ಎಣಿಕೆ)) {
			n = &(*n)->rb_ಬಲ;
		} ಅನ್ಯಥಾ {
			/* The start bit is set */
			*out = start + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಗಂಟು = parent;
	ext = ಗಂಟು_to_extent(ಗಂಟು);
	ಯದಿ (ext->start < start) {
		ಗಂಟು = ext2fs_rb_next(ಗಂಟು);
		ಯದಿ (ಗಂಟು == ರಿಕ್ತ)
			ಪ್ರತ್ಯಾಗಮ ENOENT;
		ext = ಗಂಟು_to_extent(ಗಂಟು);
	}
	ಯದಿ (ext->start <= end) {
		*out = ext->start + bitmap->start;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ಸ್ಥಿರ ಶೂನ್ಯ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ಸಂರಚನಾ ext2fs_rb_private *bp;
	ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
	ಸಂರಚನಾ bmap_rb_extent *ext;
	__u64 ಎಣಿಕೆ = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	ದ್ವಿಗುಣ eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ದ್ವಿಗುಣ m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (ಸಂರಚನಾ ext2fs_rb_private *) bitmap->ಗುಪ್ತ;

	ಪ್ರತಿ (ಗಂಟು = ext2fs_rb_first(&bp->ಬೇರು); ಗಂಟು != ರಿಕ್ತ;
	     ಗಂಟು = ext2fs_rb_next(ಗಂಟು)) {
		ext = ಗಂಟು_to_extent(ಗಂಟು);
		ಎಣಿಕೆ++;
		ಯದಿ (ext->ಎಣಿಕೆ > max_size)
			max_size = ext->ಎಣಿಕೆ;
		ಯದಿ (ext->ಎಣಿಕೆ < min_size)
			min_size = ext->ಎಣಿಕೆ;
		size += ext->ಎಣಿಕೆ;
	}

	ಯದಿ (ಎಣಿಕೆ)
		avg_size = size / ಎಣಿಕೆ;
	ಯದಿ (min_size == ULONG_MAX)
		min_size = 0;
	eff = (ದ್ವಿಗುಣ)((ಎಣಿಕೆ * ಪರಿಮಾಣ(ಸಂರಚನಾ bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ಎಣಿಕೆ + bitmap->stats.mark_ext_ಎಣಿಕೆ;
	test_all = bitmap->stats.test_ಎಣಿಕೆ + bitmap->stats.test_ext_ಎಣಿಕೆ;
	ಯದಿ (mark_all)
		m_hit = ((ದ್ವಿಗುಣ)bp->mark_hit / mark_all) * 100;
	ಯದಿ (test_all)
		t_hit = ((ದ್ವಿಗುಣ)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) ಎಣಿಕೆ, (unsigned long long)
		((ಎಣಿಕೆ * ಪರಿಮಾಣ(ಸಂರಚನಾ bmap_rb_extent)) +
		 ಪರಿಮಾಣ(ಸಂರಚನಾ ext2fs_rb_private)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(unsigned long long) min_size);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(unsigned long long) max_size, (unsigned long long) avg_size);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(unsigned long long) size,
		(unsigned long long) bitmap->real_end - bitmap->start);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಶೂನ್ಯ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

ಸಂರಚನಾ ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.set_bmap_range = rb_set_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};
