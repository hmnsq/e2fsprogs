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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#যদি HAVE_LINUX_TYPES_H
#অন্তর্ভাৱ <linux/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"
#অন্তর্ভাৱ "bmap64.h"
#অন্তর্ভাৱ "rbtree.h"

#অন্তর্ভাৱ <limits.h>

সংরচনা bmap_rb_extent {
	সংরচনা rb_গ্ৰন্থি গ্ৰন্থি;
	__u64 start;
	__u64 গুনতি;
};

সংরচনা ext2fs_rb_private {
	সংরচনা rb_মূল মূল;
	সংরচনা bmap_rb_extent *wcursor;
	সংরচনা bmap_rb_extent *rcursor;
	সংরচনা bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

অন্তঃস্থ স্থির সংরচনা bmap_rb_extent *গ্ৰন্থি_to_extent(সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ঘূৰাই (সংরচনা bmap_rb_extent *) গ্ৰন্থি;
}

স্থির পূর্ণাঙ্ক rb_ঢোকাও_extent(__u64 start, __u64 গুনতি,
			    সংরচনা ext2fs_rb_private *);
স্থির শূন্য rb_get_new_extent(সংরচনা bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
স্থির শূন্য print_বৃক্ষ(সংরচনা rb_মূল *মূল)
{
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
	সংরচনা bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	গ্ৰন্থি = ext2fs_rb_first(মূল);
	বাবে (গ্ৰন্থি = ext2fs_rb_first(মূল); গ্ৰন্থি != রিক্ত; 
	     গ্ৰন্থি = ext2fs_rb_next(গ্ৰন্থি)) {
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->গুনতি);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

স্থির শূন্য check_বৃক্ষ(সংরচনা rb_মূল *মূল, ধ্রুৱ ৱর্ণ *বাৰ্তা)
{
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি;
	সংরচনা bmap_rb_extent *ext, *old = রিক্ত;

	বাবে (গ্ৰন্থি = ext2fs_rb_first(মূল); গ্ৰন্থি;
	     গ্ৰন্থি = ext2fs_rb_next(গ্ৰন্থি)) {
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		যদি (ext->গুনতি == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->গুনতি,
				(unsigned long long) ext->গুনতি);
			গচ্ছ err_out;
		}
		যদি (ext->start + ext->গুনতি < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->গুনতি,
				(unsigned long long) ext->গুনতি);
			গচ্ছ err_out;
		}

		যদি (old) {
			যদি (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->গুনতি,
					(unsigned long long) old->গুনতি);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->গুনতি,
					(unsigned long long) ext->গুনতি);
				গচ্ছ err_out;
			}
			যদি ((old->start + old->গুনতি) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->গুনতি,
					(unsigned long long) old->গুনতি);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->গুনতি,
					(unsigned long long) ext->গুনতি);
				গচ্ছ err_out;
			}
		}
		old = ext;
	}
	ঘূৰাই;

err_out:
	fprintf(stderr, "%s\n", বাৰ্তা);
	print_বৃক্ষ(মূল);
	exit(1);
}
#অন্যথা
#define check_বৃক্ষ(মূল, বাৰ্তা) কৰা {} যেতিয়ালৈ (0)
#define print_বৃক্ষ(মূল) কৰা {} যেতিয়ালৈ (0)
#endif

স্থির শূন্য rb_get_new_extent(সংরচনা bmap_rb_extent **ext, __u64 start,
			     __u64 গুনতি)
{
	সংরচনা bmap_rb_extent *new_ext;
	পূর্ণাঙ্ক retval;

	retval = ext2fs_get_mem(পরিমাণ (সংরচনা bmap_rb_extent),
				&new_ext);
	যদি (retval)
		abort();

	new_ext->start = start;
	new_ext->গুনতি = গুনতি;
	*ext = new_ext;
}

অন্তঃস্থ
স্থির শূন্য rb_free_extent(সংরচনা ext2fs_rb_private *bp,
			   সংরচনা bmap_rb_extent *ext)
{
	যদি (bp->wcursor == ext)
		bp->wcursor = রিক্ত;
	যদি (bp->rcursor == ext)
		bp->rcursor = রিক্ত;
	যদি (bp->rcursor_next == ext)
		bp->rcursor_next = রিক্ত;
	ext2fs_free_mem(&ext);
}

স্থির errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	সংরচনা ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(পরিমাণ (সংরচনা ext2fs_rb_private), &bp);
	যদি (retval)
		ঘূৰাই retval;

	bp->মূল = RB_মূল;
	bp->rcursor = রিক্ত;
	bp->rcursor_next = রিক্ত;
	bp->wcursor = রিক্ত;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->গুপ্ত = (শূন্য *) bp;
	ঘূৰাই 0;
}

স্থির errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	যদি (retval)
		ঘূৰাই retval;

	ঘূৰাই 0;
}

স্থির শূন্য rb_free_বৃক্ষ(সংরচনা rb_মূল *মূল)
{
	সংরচনা bmap_rb_extent *ext;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি, *next;

	বাবে (গ্ৰন্থি = ext2fs_rb_first(মূল); গ্ৰন্থি; গ্ৰন্থি = next) {
		next = ext2fs_rb_next(গ্ৰন্থি);
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		ext2fs_rb_erase(গ্ৰন্থি, মূল);
		ext2fs_free_mem(&ext);
	}
}

স্থির শূন্য rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;

	rb_free_বৃক্ষ(&bp->মূল);
	ext2fs_free_mem(&bp);
	bp = 0;
}

স্থির errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	সংরচনা ext2fs_rb_private *src_bp, *dest_bp;
	সংরচনা bmap_rb_extent *src_ext, *dest_ext;
	সংরচনা rb_গ্ৰন্থি *dest_গ্ৰন্থি, *src_গ্ৰন্থি, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	যদি (retval)
		ঘূৰাই retval;

	src_bp = (সংরচনা ext2fs_rb_private *) src->গুপ্ত;
	dest_bp = (সংরচনা ext2fs_rb_private *) dest->গুপ্ত;
	src_bp->rcursor = রিক্ত;
	dest_bp->rcursor = রিক্ত;

	src_গ্ৰন্থি = ext2fs_rb_first(&src_bp->মূল);
	যেতিয়ালৈ (src_গ্ৰন্থি) {
		src_ext = গ্ৰন্থি_to_extent(src_গ্ৰন্থি);
		retval = ext2fs_get_mem(পরিমাণ (সংরচনা bmap_rb_extent),
					&dest_ext);
		যদি (retval)
			ভঙ্গ;

		memcpy(dest_ext, src_ext, পরিমাণ(সংরচনা bmap_rb_extent));

		dest_গ্ৰন্থি = &dest_ext->গ্ৰন্থি;
		n = &dest_bp->মূল.rb_গ্ৰন্থি;

		dest_last = রিক্ত;
		যদি (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->মূল);
			n = &(dest_last)->rb_ডান;
		}

		ext2fs_rb_link_গ্ৰন্থি(dest_গ্ৰন্থি, dest_last, n);
		ext2fs_rb_ঢোকাও_color(dest_গ্ৰন্থি, &dest_bp->মূল);

		src_গ্ৰন্থি = ext2fs_rb_next(src_গ্ৰন্থি);
	}

	ঘূৰাই retval;
}

স্থির শূন্য rb_truncate(__u64 new_max, সংরচনা rb_মূল *মূল)
{
	সংরচনা bmap_rb_extent *ext;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি;

	গ্ৰন্থি = ext2fs_rb_last(মূল);
	যেতিয়ালৈ (গ্ৰন্থি) {
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);

		যদি ((ext->start + ext->গুনতি - 1) <= new_max)
			ভঙ্গ;
		অন্যথা যদি (ext->start > new_max) {
			ext2fs_rb_erase(গ্ৰন্থি, মূল);
			ext2fs_free_mem(&ext);
			গ্ৰন্থি = ext2fs_rb_last(মূল);
			অনুৱর্ত;
		} অন্যথা
			ext->গুনতি = new_max - ext->start + 1;
	}
}

স্থির errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bmap->গুপ্ত;
	bp->rcursor = রিক্ত;
	bp->wcursor = রিক্ত;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->মূল);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	যদি (bmap->end < bmap->real_end)
		rb_ঢোকাও_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	ঘূৰাই 0;

}

অন্তঃস্থ স্থির পূর্ণাঙ্ক
rb_test_bit(সংরচনা ext2fs_rb_private *bp, __u64 bit)
{
	সংরচনা bmap_rb_extent *rcursor, *next_ext = রিক্ত;
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, *next;
	সংরচনা rb_গ্ৰন্থি **n = &bp->মূল.rb_গ্ৰন্থি;
	সংরচনা bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	যদি (!rcursor)
		গচ্ছ অনুসন্ধান_বৃক্ষ;

	যদি (bit >= rcursor->start && bit < rcursor->start + rcursor->গুনতি) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		ঘূৰাই 1;
	}

	next_ext = bp->rcursor_next;
	যদি (!next_ext) {
		next = ext2fs_rb_next(&rcursor->গ্ৰন্থি);
		যদি (next)
			next_ext = গ্ৰন্থি_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	যদি (next_ext) {
		যদি ((bit >= rcursor->start + rcursor->গুনতি) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			ঘূৰাই 0;
		}
	}
	bp->rcursor = রিক্ত;
	bp->rcursor_next = রিক্ত;

	rcursor = bp->wcursor;
	যদি (!rcursor)
		গচ্ছ অনুসন্ধান_বৃক্ষ;

	যদি (bit >= rcursor->start && bit < rcursor->start + rcursor->গুনতি)
		ঘূৰাই 1;

অনুসন্ধান_বৃক্ষ:

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (bit < ext->start)
			n = &(*n)->rb_বাম;
		অন্যথা যদি (bit >= (ext->start + ext->গুনতি))
			n = &(*n)->rb_ডান;
		অন্যথা {
			bp->rcursor = ext;
			bp->rcursor_next = রিক্ত;
			ঘূৰাই 1;
		}
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক rb_ঢোকাও_extent(__u64 start, __u64 গুনতি,
			    সংরচনা ext2fs_rb_private *bp)
{
	সংরচনা rb_মূল *মূল = &bp->মূল;
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, **n = &মূল->rb_গ্ৰন্থি;
	সংরচনা rb_গ্ৰন্থি *new_গ্ৰন্থি, *গ্ৰন্থি, *next;
	সংরচনা bmap_rb_extent *new_ext;
	সংরচনা bmap_rb_extent *ext;
	পূর্ণাঙ্ক retval = 0;

	যদি (গুনতি == 0)
		ঘূৰাই 0;

	bp->rcursor_next = রিক্ত;
	ext = bp->wcursor;
	যদি (ext) {
		যদি (start >= ext->start &&
		    start <= (ext->start + ext->গুনতি)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			গচ্ছ got_extent;
		}
	}

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);

		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start > (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
got_extent:
			যদি ((start + গুনতি) <= (ext->start + ext->গুনতি))
				ঘূৰাই 1;

			যদি ((ext->start + ext->গুনতি) == start)
				retval = 0;
			অন্যথা
				retval = 1;

			গুনতি += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_গ্ৰন্থি = &ext->গ্ৰন্থি;

			গচ্ছ skip_ঢোকাও;
		}
	}

	rb_get_new_extent(&new_ext, start, গুনতি);

	new_গ্ৰন্থি = &new_ext->গ্ৰন্থি;
	ext2fs_rb_link_গ্ৰন্থি(new_গ্ৰন্থি, parent, n);
	ext2fs_rb_ঢোকাও_color(new_গ্ৰন্থি, মূল);
	bp->wcursor = new_ext;

	গ্ৰন্থি = ext2fs_rb_prev(new_গ্ৰন্থি);
	যদি (গ্ৰন্থি) {
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		যদি ((ext->start + ext->গুনতি) == start) {
			start = ext->start;
			গুনতি += ext->গুনতি;
			ext2fs_rb_erase(গ্ৰন্থি, মূল);
			rb_free_extent(bp, ext);
		}
	}

skip_ঢোকাও:
	/* See if we can merge extent to the right */
	বাবে (গ্ৰন্থি = ext2fs_rb_next(new_গ্ৰন্থি); গ্ৰন্থি != রিক্ত; গ্ৰন্থি = next) {
		next = ext2fs_rb_next(গ্ৰন্থি);
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);

		যদি ((ext->start + ext->গুনতি) <= start)
			অনুৱর্ত;

		/* No more merging */
		যদি ((start + গুনতি) < ext->start)
			ভঙ্গ;

		/* ext is embedded in new_ext interval */
		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext2fs_rb_erase(গ্ৰন্থি, মূল);
			rb_free_extent(bp, ext);
			অনুৱর্ত;
		} অন্যথা {
		/* merge ext with new_ext */
			গুনতি += ((ext->start + ext->গুনতি) -
				  (start + গুনতি));
			ext2fs_rb_erase(গ্ৰন্থি, মূল);
			rb_free_extent(bp, ext);
			ভঙ্গ;
		}
	}

	new_ext->start = start;
	new_ext->গুনতি = গুনতি;

	ঘূৰাই retval;
}

স্থির পূর্ণাঙ্ক rb_remove_extent(__u64 start, __u64 গুনতি,
			    সংরচনা ext2fs_rb_private *bp)
{
	সংরচনা rb_মূল *মূল = &bp->মূল;
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, **n = &মূল->rb_গ্ৰন্থি;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি;
	সংরচনা bmap_rb_extent *ext;
	__u64 new_start, new_গুনতি;
	পূর্ণাঙ্ক retval = 0;

	যদি (ext2fs_rb_empty_মূল(মূল))
		ঘূৰাই 0;

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
			অনুৱর্ত;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
			অনুৱর্ত;
		}

		যদি ((start > ext->start) &&
		    (start + গুনতি) < (ext->start + ext->গুনতি)) {
			/* We have to split extent into two */
			new_start = start + গুনতি;
			new_গুনতি = (ext->start + ext->গুনতি) - new_start;

			ext->গুনতি = start - ext->start;

			rb_ঢোকাও_extent(new_start, new_গুনতি, bp);
			ঘূৰাই 1;
		}

		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext->গুনতি = start - ext->start;
			retval = 1;
		}

		যদি (0 == ext->গুনতি) {
			parent = ext2fs_rb_next(&ext->গ্ৰন্থি);
			ext2fs_rb_erase(&ext->গ্ৰন্থি, মূল);
			rb_free_extent(bp, ext);
			ভঙ্গ;
		}

		যদি (start == ext->start) {
			ext->start += গুনতি;
			ext->গুনতি -= গুনতি;
			ঘূৰাই 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	বাবে (; parent != রিক্ত; parent = গ্ৰন্থি) {
		গ্ৰন্থি = ext2fs_rb_next(parent);
		ext = গ্ৰন্থি_to_extent(parent);
		যদি ((ext->start + ext->গুনতি) <= start)
			অনুৱর্ত;

		/* No more extents to be removed/truncated */
		যদি ((start + গুনতি) < ext->start)
			ভঙ্গ;

		/* The entire extent is within the region to be removed */
		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext2fs_rb_erase(parent, মূল);
			rb_free_extent(bp, ext);
			retval = 1;
			অনুৱর্ত;
		} অন্যথা {
			/* modify the last extent in region to be removed */
			ext->গুনতি -= ((start + গুনতি) - ext->start);
			ext->start = start + গুনতি;
			retval = 1;
			ভঙ্গ;
		}
	}

	ঘূৰাই retval;
}

স্থির পূর্ণাঙ্ক rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	সংরচনা ext2fs_rb_private *bp;
	পূর্ণাঙ্ক retval;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	arg -= bitmap->start;

	retval = rb_ঢোকাও_extent(arg, 1, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
	ঘূৰাই retval;
}

স্থির পূর্ণাঙ্ক rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	সংরচনা ext2fs_rb_private *bp;
	পূর্ণাঙ্ক retval;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_বৃক্ষ(&bp->মূল, __func__);

	ঘূৰাই retval;
}

অন্তঃস্থ
স্থির পূর্ণাঙ্ক rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	arg -= bitmap->start;

	ঘূৰাই rb_test_bit(bp, arg);
}

স্থির শূন্য rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned পূর্ণাঙ্ক অঙ্ক)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	arg -= bitmap->start;

	rb_ঢোকাও_extent(arg, অঙ্ক, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির শূন্য rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned পূর্ণাঙ্ক অঙ্ক)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	arg -= bitmap->start;

	rb_remove_extent(arg, অঙ্ক, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির পূর্ণাঙ্ক rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned পূর্ণাঙ্ক len)
{
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, **n;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি, *next;
	সংরচনা ext2fs_rb_private *bp;
	সংরচনা bmap_rb_extent *ext;
	পূর্ণাঙ্ক retval = 1;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	n = &bp->মূল.rb_গ্ৰন্থি;
	start -= bitmap->start;

	যদি (len == 0 || ext2fs_rb_empty_মূল(&bp->মূল))
		ঘূৰাই 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ঘূৰাই 0;
		}
	}

	গ্ৰন্থি = parent;
	যেতিয়ালৈ (গ্ৰন্থি) {
		next = ext2fs_rb_next(গ্ৰন্থি);
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		গ্ৰন্থি = next;

		যদি ((ext->start + ext->গুনতি) <= start)
			অনুৱর্ত;

		/* No more merging */
		যদি ((start + len) <= ext->start)
			ভঙ্গ;

		retval = 0;
		ভঙ্গ;
	}
	ঘূৰাই retval;
}

স্থির errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *in)
{
	সংরচনা ext2fs_rb_private *bp;
	unsigned ৱর্ণ *cp = in;
	size_t i;
	পূর্ণাঙ্ক first_set = -1;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;

	বাবে (i = 0; i < অঙ্ক; i++) {
		যদি ((i & 7) == 0) {
			unsigned ৱর্ণ c = cp[i/8];
			যদি (c == 0xFF) {
				যদি (first_set == -1)
					first_set = i;
				i += 7;
				অনুৱর্ত;
			}
			যদি ((c == 0x00) && (first_set == -1)) {
				i += 7;
				অনুৱর্ত;
			}
		}
		যদি (ext2fs_test_bit(i, in)) {
			যদি (first_set == -1)
				first_set = i;
			অনুৱর্ত;
		}
		যদি (first_set == -1)
			অনুৱর্ত;

		rb_ঢোকাও_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_বৃক্ষ(&bp->মূল, __func__);
		first_set = -1;
	}
	যদি (first_set != -1) {
		rb_ঢোকাও_extent(start + first_set - bitmap->start,
				 অঙ্ক - first_set, bp);
		check_বৃক্ষ(&bp->মূল, __func__);
	}

	ঘূৰাই 0;
}

স্থির errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *out)
{

	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, *next, **n;
	সংরচনা ext2fs_rb_private *bp;
	সংরচনা bmap_rb_extent *ext;
	__u64 গুনতি, pos;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	n = &bp->মূল.rb_গ্ৰন্থি;
	start -= bitmap->start;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ঘূৰাই 0;

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা
			ভঙ্গ;
	}

	memset(out, 0, (অঙ্ক + 7) >> 3);

	বাবে (; parent != রিক্ত; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = গ্ৰন্থি_to_extent(parent);

		pos = ext->start;
		গুনতি = ext->গুনতি;
		যদি (pos >= start + অঙ্ক)
			ভঙ্গ;
		যদি (pos < start) {
			যদি (pos + গুনতি <  start)
				অনুৱর্ত;
			গুনতি -= start - pos;
			pos = start;
		}
		যদি (pos + গুনতি > start + অঙ্ক)
			গুনতি = start + অঙ্ক - pos;

		যেতিয়ালৈ (গুনতি > 0) {
			যদি ((গুনতি >= 8) &&
			    ((pos - start) % 8) == 0) {
				পূর্ণাঙ্ক nbytes = গুনতি >> 3;
				পূর্ণাঙ্ক offset = (pos - start) >> 3;

				memset(((ৱর্ণ *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				গুনতি -= nbytes << 3;
				অনুৱর্ত;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			গুনতি--;
		}
	}
	ঘূৰাই 0;
}

স্থির শূন্য rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	সংরচনা ext2fs_rb_private *bp;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;

	rb_free_বৃক্ষ(&bp->মূল);
	bp->rcursor = রিক্ত;
	bp->rcursor_next = রিক্ত;
	bp->wcursor = রিক্ত;
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, **n;
	সংরচনা ext2fs_rb_private *bp;
	সংরচনা bmap_rb_extent *ext;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	n = &bp->মূল.rb_গ্ৰন্থি;
	start -= bitmap->start;
	end -= bitmap->start;

	যদি (start > end)
		ঘূৰাই EINVAL;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ঘূৰাই ENOENT;

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা যদি (ext->start + ext->গুনতি <= end) {
			*out = ext->start + ext->গুনতি + bitmap->start;
			ঘূৰাই 0;
		} অন্যথা
			ঘূৰাই ENOENT;
	}

	*out = start + bitmap->start;
	ঘূৰাই 0;
}

স্থির errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	সংরচনা rb_গ্ৰন্থি *parent = রিক্ত, **n;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি;
	সংরচনা ext2fs_rb_private *bp;
	সংরচনা bmap_rb_extent *ext;

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;
	n = &bp->মূল.rb_গ্ৰন্থি;
	start -= bitmap->start;
	end -= bitmap->start;

	যদি (start > end)
		ঘূৰাই EINVAL;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ঘূৰাই ENOENT;

	যেতিয়ালৈ (*n) {
		parent = *n;
		ext = গ্ৰন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
			/* The start bit is set */
			*out = start + bitmap->start;
			ঘূৰাই 0;
		}
	}

	গ্ৰন্থি = parent;
	ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
	যদি (ext->start < start) {
		গ্ৰন্থি = ext2fs_rb_next(গ্ৰন্থি);
		যদি (গ্ৰন্থি == রিক্ত)
			ঘূৰাই ENOENT;
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
	}
	যদি (ext->start <= end) {
		*out = ext->start + bitmap->start;
		ঘূৰাই 0;
	}
	ঘূৰাই ENOENT;
}

#ifdef ENABLE_BMAP_STATS
স্থির শূন্য rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	সংরচনা ext2fs_rb_private *bp;
	সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
	সংরচনা bmap_rb_extent *ext;
	__u64 গুনতি = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	দ্ৱিগুণ eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	দ্ৱিগুণ m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (সংরচনা ext2fs_rb_private *) bitmap->গুপ্ত;

	বাবে (গ্ৰন্থি = ext2fs_rb_first(&bp->মূল); গ্ৰন্থি != রিক্ত;
	     গ্ৰন্থি = ext2fs_rb_next(গ্ৰন্থি)) {
		ext = গ্ৰন্থি_to_extent(গ্ৰন্থি);
		গুনতি++;
		যদি (ext->গুনতি > max_size)
			max_size = ext->গুনতি;
		যদি (ext->গুনতি < min_size)
			min_size = ext->গুনতি;
		size += ext->গুনতি;
	}

	যদি (গুনতি)
		avg_size = size / গুনতি;
	যদি (min_size == ULONG_MAX)
		min_size = 0;
	eff = (দ্ৱিগুণ)((গুনতি * পরিমাণ(সংরচনা bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_গুনতি + bitmap->stats.mark_ext_গুনতি;
	test_all = bitmap->stats.test_গুনতি + bitmap->stats.test_ext_গুনতি;
	যদি (mark_all)
		m_hit = ((দ্ৱিগুণ)bp->mark_hit / mark_all) * 100;
	যদি (test_all)
		t_hit = ((দ্ৱিগুণ)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) গুনতি, (unsigned long long)
		((গুনতি * পরিমাণ(সংরচনা bmap_rb_extent)) +
		 পরিমাণ(সংরচনা ext2fs_rb_private)));
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
#অন্যথা
স্থির শূন্য rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

সংরচনা ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
