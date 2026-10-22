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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#যদি HAVE_LINUX_TYPES_H
#অন্তর্ভাব <linux/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"
#অন্তর্ভাব "bmap64.h"
#অন্তর্ভাব "rbtree.h"

#অন্তর্ভাব <limits.h>

কাঠামো bmap_rb_extent {
	কাঠামো rb_গ্রন্থি গ্রন্থি;
	__u64 start;
	__u64 গুনতি;
};

কাঠামো ext2fs_rb_private {
	কাঠামো rb_মূল মূল;
	কাঠামো bmap_rb_extent *wcursor;
	কাঠামো bmap_rb_extent *rcursor;
	কাঠামো bmap_rb_extent *rcursor_পরবর্তী;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

অন্তঃরেখ স্থির কাঠামো bmap_rb_extent *গ্রন্থি_to_extent(কাঠামো rb_গ্রন্থি *গ্রন্থি)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ফেরত (কাঠামো bmap_rb_extent *) গ্রন্থি;
}

স্থির পূর্ণসংখ্যা rb_ঢোকাও_extent(__u64 start, __u64 গুনতি,
			    কাঠামো ext2fs_rb_private *);
স্থির শূন্য rb_get_new_extent(কাঠামো bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
স্থির শূন্য print_বৃক্ষ(কাঠামো rb_মূল *মূল)
{
	কাঠামো rb_গ্রন্থি *গ্রন্থি = ফাঁকা;
	কাঠামো bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	গ্রন্থি = ext2fs_rb_first(মূল);
	জন্য (গ্রন্থি = ext2fs_rb_first(মূল); গ্রন্থি != ফাঁকা; 
	     গ্রন্থি = ext2fs_rb_পরবর্তী(গ্রন্থি)) {
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->গুনতি);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

স্থির শূন্য check_বৃক্ষ(কাঠামো rb_মূল *মূল, ধ্রুবক অক্ষর *বার্তা)
{
	কাঠামো rb_গ্রন্থি *গ্রন্থি;
	কাঠামো bmap_rb_extent *ext, *old = ফাঁকা;

	জন্য (গ্রন্থি = ext2fs_rb_first(মূল); গ্রন্থি;
	     গ্রন্থি = ext2fs_rb_পরবর্তী(গ্রন্থি)) {
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		যদি (ext->গুনতি == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->গুনতি,
				(unsigned long long) ext->গুনতি);
			যাও err_out;
		}
		যদি (ext->start + ext->গুনতি < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->গুনতি,
				(unsigned long long) ext->গুনতি);
			যাও err_out;
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
				যাও err_out;
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
				যাও err_out;
			}
		}
		old = ext;
	}
	ফেরত;

err_out:
	fprintf(stderr, "%s\n", বার্তা);
	print_বৃক্ষ(মূল);
	exit(1);
}
#অন্যথা
#define check_বৃক্ষ(মূল, বার্তা) করো {} যখন (0)
#define print_বৃক্ষ(মূল) করো {} যখন (0)
#endif

স্থির শূন্য rb_get_new_extent(কাঠামো bmap_rb_extent **ext, __u64 start,
			     __u64 গুনতি)
{
	কাঠামো bmap_rb_extent *new_ext;
	পূর্ণসংখ্যা retval;

	retval = ext2fs_get_mem(আকার (কাঠামো bmap_rb_extent),
				&new_ext);
	যদি (retval)
		abort();

	new_ext->start = start;
	new_ext->গুনতি = গুনতি;
	*ext = new_ext;
}

অন্তঃরেখ
স্থির শূন্য rb_free_extent(কাঠামো ext2fs_rb_private *bp,
			   কাঠামো bmap_rb_extent *ext)
{
	যদি (bp->wcursor == ext)
		bp->wcursor = ফাঁকা;
	যদি (bp->rcursor == ext)
		bp->rcursor = ফাঁকা;
	যদি (bp->rcursor_পরবর্তী == ext)
		bp->rcursor_পরবর্তী = ফাঁকা;
	ext2fs_free_mem(&ext);
}

স্থির errcode_t rb_alloc_private_তথ্য (ext2fs_generic_bitmap_64 bitmap)
{
	কাঠামো ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(আকার (কাঠামো ext2fs_rb_private), &bp);
	যদি (retval)
		ফেরত retval;

	bp->মূল = RB_মূল;
	bp->rcursor = ফাঁকা;
	bp->rcursor_পরবর্তী = ফাঁকা;
	bp->wcursor = ফাঁকা;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->ব্যক্তিগত = (শূন্য *) bp;
	ফেরত 0;
}

স্থির errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_তথ্য (bitmap);
	যদি (retval)
		ফেরত retval;

	ফেরত 0;
}

স্থির শূন্য rb_free_বৃক্ষ(কাঠামো rb_মূল *মূল)
{
	কাঠামো bmap_rb_extent *ext;
	কাঠামো rb_গ্রন্থি *গ্রন্থি, *পরবর্তী;

	জন্য (গ্রন্থি = ext2fs_rb_first(মূল); গ্রন্থি; গ্রন্থি = পরবর্তী) {
		পরবর্তী = ext2fs_rb_পরবর্তী(গ্রন্থি);
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		ext2fs_rb_erase(গ্রন্থি, মূল);
		ext2fs_free_mem(&ext);
	}
}

স্থির শূন্য rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;

	rb_free_বৃক্ষ(&bp->মূল);
	ext2fs_free_mem(&bp);
	bp = 0;
}

স্থির errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	কাঠামো ext2fs_rb_private *src_bp, *dest_bp;
	কাঠামো bmap_rb_extent *src_ext, *dest_ext;
	কাঠামো rb_গ্রন্থি *dest_গ্রন্থি, *src_গ্রন্থি, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_তথ্য (dest);
	যদি (retval)
		ফেরত retval;

	src_bp = (কাঠামো ext2fs_rb_private *) src->ব্যক্তিগত;
	dest_bp = (কাঠামো ext2fs_rb_private *) dest->ব্যক্তিগত;
	src_bp->rcursor = ফাঁকা;
	dest_bp->rcursor = ফাঁকা;

	src_গ্রন্থি = ext2fs_rb_first(&src_bp->মূল);
	যখন (src_গ্রন্থি) {
		src_ext = গ্রন্থি_to_extent(src_গ্রন্থি);
		retval = ext2fs_get_mem(আকার (কাঠামো bmap_rb_extent),
					&dest_ext);
		যদি (retval)
			বিরতি;

		memcpy(dest_ext, src_ext, আকার(কাঠামো bmap_rb_extent));

		dest_গ্রন্থি = &dest_ext->গ্রন্থি;
		n = &dest_bp->মূল.rb_গ্রন্থি;

		dest_last = ফাঁকা;
		যদি (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->মূল);
			n = &(dest_last)->rb_ডান;
		}

		ext2fs_rb_link_গ্রন্থি(dest_গ্রন্থি, dest_last, n);
		ext2fs_rb_ঢোকাও_color(dest_গ্রন্থি, &dest_bp->মূল);

		src_গ্রন্থি = ext2fs_rb_পরবর্তী(src_গ্রন্থি);
	}

	ফেরত retval;
}

স্থির শূন্য rb_truncate(__u64 new_max, কাঠামো rb_মূল *মূল)
{
	কাঠামো bmap_rb_extent *ext;
	কাঠামো rb_গ্রন্থি *গ্রন্থি;

	গ্রন্থি = ext2fs_rb_last(মূল);
	যখন (গ্রন্থি) {
		ext = গ্রন্থি_to_extent(গ্রন্থি);

		যদি ((ext->start + ext->গুনতি - 1) <= new_max)
			বিরতি;
		অন্যথা যদি (ext->start > new_max) {
			ext2fs_rb_erase(গ্রন্থি, মূল);
			ext2fs_free_mem(&ext);
			গ্রন্থি = ext2fs_rb_last(মূল);
			চালু;
		} অন্যথা
			ext->গুনতি = new_max - ext->start + 1;
	}
}

স্থির errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bmap->ব্যক্তিগত;
	bp->rcursor = ফাঁকা;
	bp->wcursor = ফাঁকা;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->মূল);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	যদি (bmap->end < bmap->real_end)
		rb_ঢোকাও_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	ফেরত 0;

}

অন্তঃরেখ স্থির পূর্ণসংখ্যা
rb_test_bit(কাঠামো ext2fs_rb_private *bp, __u64 bit)
{
	কাঠামো bmap_rb_extent *rcursor, *পরবর্তী_ext = ফাঁকা;
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, *পরবর্তী;
	কাঠামো rb_গ্রন্থি **n = &bp->মূল.rb_গ্রন্থি;
	কাঠামো bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	যদি (!rcursor)
		যাও অনুসন্ধান_বৃক্ষ;

	যদি (bit >= rcursor->start && bit < rcursor->start + rcursor->গুনতি) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		ফেরত 1;
	}

	পরবর্তী_ext = bp->rcursor_পরবর্তী;
	যদি (!পরবর্তী_ext) {
		পরবর্তী = ext2fs_rb_পরবর্তী(&rcursor->গ্রন্থি);
		যদি (পরবর্তী)
			পরবর্তী_ext = গ্রন্থি_to_extent(পরবর্তী);
		bp->rcursor_পরবর্তী = পরবর্তী_ext;
	}
	যদি (পরবর্তী_ext) {
		যদি ((bit >= rcursor->start + rcursor->গুনতি) &&
		    (bit < পরবর্তী_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			ফেরত 0;
		}
	}
	bp->rcursor = ফাঁকা;
	bp->rcursor_পরবর্তী = ফাঁকা;

	rcursor = bp->wcursor;
	যদি (!rcursor)
		যাও অনুসন্ধান_বৃক্ষ;

	যদি (bit >= rcursor->start && bit < rcursor->start + rcursor->গুনতি)
		ফেরত 1;

অনুসন্ধান_বৃক্ষ:

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (bit < ext->start)
			n = &(*n)->rb_বাম;
		অন্যথা যদি (bit >= (ext->start + ext->গুনতি))
			n = &(*n)->rb_ডান;
		অন্যথা {
			bp->rcursor = ext;
			bp->rcursor_পরবর্তী = ফাঁকা;
			ফেরত 1;
		}
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা rb_ঢোকাও_extent(__u64 start, __u64 গুনতি,
			    কাঠামো ext2fs_rb_private *bp)
{
	কাঠামো rb_মূল *মূল = &bp->মূল;
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, **n = &মূল->rb_গ্রন্থি;
	কাঠামো rb_গ্রন্থি *new_গ্রন্থি, *গ্রন্থি, *পরবর্তী;
	কাঠামো bmap_rb_extent *new_ext;
	কাঠামো bmap_rb_extent *ext;
	পূর্ণসংখ্যা retval = 0;

	যদি (গুনতি == 0)
		ফেরত 0;

	bp->rcursor_পরবর্তী = ফাঁকা;
	ext = bp->wcursor;
	যদি (ext) {
		যদি (start >= ext->start &&
		    start <= (ext->start + ext->গুনতি)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			যাও got_extent;
		}
	}

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);

		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start > (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
got_extent:
			যদি ((start + গুনতি) <= (ext->start + ext->গুনতি))
				ফেরত 1;

			যদি ((ext->start + ext->গুনতি) == start)
				retval = 0;
			অন্যথা
				retval = 1;

			গুনতি += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_গ্রন্থি = &ext->গ্রন্থি;

			যাও skip_ঢোকাও;
		}
	}

	rb_get_new_extent(&new_ext, start, গুনতি);

	new_গ্রন্থি = &new_ext->গ্রন্থি;
	ext2fs_rb_link_গ্রন্থি(new_গ্রন্থি, parent, n);
	ext2fs_rb_ঢোকাও_color(new_গ্রন্থি, মূল);
	bp->wcursor = new_ext;

	গ্রন্থি = ext2fs_rb_পূর্ব(new_গ্রন্থি);
	যদি (গ্রন্থি) {
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		যদি ((ext->start + ext->গুনতি) == start) {
			start = ext->start;
			গুনতি += ext->গুনতি;
			ext2fs_rb_erase(গ্রন্থি, মূল);
			rb_free_extent(bp, ext);
		}
	}

skip_ঢোকাও:
	/* See if we can merge extent to the right */
	জন্য (গ্রন্থি = ext2fs_rb_পরবর্তী(new_গ্রন্থি); গ্রন্থি != ফাঁকা; গ্রন্থি = পরবর্তী) {
		পরবর্তী = ext2fs_rb_পরবর্তী(গ্রন্থি);
		ext = গ্রন্থি_to_extent(গ্রন্থি);

		যদি ((ext->start + ext->গুনতি) <= start)
			চালু;

		/* No more merging */
		যদি ((start + গুনতি) < ext->start)
			বিরতি;

		/* ext is embedded in new_ext interval */
		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext2fs_rb_erase(গ্রন্থি, মূল);
			rb_free_extent(bp, ext);
			চালু;
		} অন্যথা {
		/* merge ext with new_ext */
			গুনতি += ((ext->start + ext->গুনতি) -
				  (start + গুনতি));
			ext2fs_rb_erase(গ্রন্থি, মূল);
			rb_free_extent(bp, ext);
			বিরতি;
		}
	}

	new_ext->start = start;
	new_ext->গুনতি = গুনতি;

	ফেরত retval;
}

স্থির পূর্ণসংখ্যা rb_remove_extent(__u64 start, __u64 গুনতি,
			    কাঠামো ext2fs_rb_private *bp)
{
	কাঠামো rb_মূল *মূল = &bp->মূল;
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, **n = &মূল->rb_গ্রন্থি;
	কাঠামো rb_গ্রন্থি *গ্রন্থি;
	কাঠামো bmap_rb_extent *ext;
	__u64 new_start, new_গুনতি;
	পূর্ণসংখ্যা retval = 0;

	যদি (ext2fs_rb_empty_মূল(মূল))
		ফেরত 0;

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
			চালু;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
			চালু;
		}

		যদি ((start > ext->start) &&
		    (start + গুনতি) < (ext->start + ext->গুনতি)) {
			/* We have to split extent into two */
			new_start = start + গুনতি;
			new_গুনতি = (ext->start + ext->গুনতি) - new_start;

			ext->গুনতি = start - ext->start;

			rb_ঢোকাও_extent(new_start, new_গুনতি, bp);
			ফেরত 1;
		}

		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext->গুনতি = start - ext->start;
			retval = 1;
		}

		যদি (0 == ext->গুনতি) {
			parent = ext2fs_rb_পরবর্তী(&ext->গ্রন্থি);
			ext2fs_rb_erase(&ext->গ্রন্থি, মূল);
			rb_free_extent(bp, ext);
			বিরতি;
		}

		যদি (start == ext->start) {
			ext->start += গুনতি;
			ext->গুনতি -= গুনতি;
			ফেরত 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	জন্য (; parent != ফাঁকা; parent = গ্রন্থি) {
		গ্রন্থি = ext2fs_rb_পরবর্তী(parent);
		ext = গ্রন্থি_to_extent(parent);
		যদি ((ext->start + ext->গুনতি) <= start)
			চালু;

		/* No more extents to be removed/truncated */
		যদি ((start + গুনতি) < ext->start)
			বিরতি;

		/* The entire extent is within the region to be removed */
		যদি ((start + গুনতি) >= (ext->start + ext->গুনতি)) {
			ext2fs_rb_erase(parent, মূল);
			rb_free_extent(bp, ext);
			retval = 1;
			চালু;
		} অন্যথা {
			/* modify the last extent in region to be removed */
			ext->গুনতি -= ((start + গুনতি) - ext->start);
			ext->start = start + গুনতি;
			retval = 1;
			বিরতি;
		}
	}

	ফেরত retval;
}

স্থির পূর্ণসংখ্যা rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	কাঠামো ext2fs_rb_private *bp;
	পূর্ণসংখ্যা retval;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	arg -= bitmap->start;

	retval = rb_ঢোকাও_extent(arg, 1, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
	ফেরত retval;
}

স্থির পূর্ণসংখ্যা rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	কাঠামো ext2fs_rb_private *bp;
	পূর্ণসংখ্যা retval;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_বৃক্ষ(&bp->মূল, __func__);

	ফেরত retval;
}

অন্তঃরেখ
স্থির পূর্ণসংখ্যা rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	arg -= bitmap->start;

	ফেরত rb_test_bit(bp, arg);
}

স্থির শূন্য rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned পূর্ণসংখ্যা অঙ্ক)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	arg -= bitmap->start;

	rb_ঢোকাও_extent(arg, অঙ্ক, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির শূন্য rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned পূর্ণসংখ্যা অঙ্ক)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	arg -= bitmap->start;

	rb_remove_extent(arg, অঙ্ক, bp);
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির পূর্ণসংখ্যা rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned পূর্ণসংখ্যা len)
{
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, **n;
	কাঠামো rb_গ্রন্থি *গ্রন্থি, *পরবর্তী;
	কাঠামো ext2fs_rb_private *bp;
	কাঠামো bmap_rb_extent *ext;
	পূর্ণসংখ্যা retval = 1;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	n = &bp->মূল.rb_গ্রন্থি;
	start -= bitmap->start;

	যদি (len == 0 || ext2fs_rb_empty_মূল(&bp->মূল))
		ফেরত 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ফেরত 0;
		}
	}

	গ্রন্থি = parent;
	যখন (গ্রন্থি) {
		পরবর্তী = ext2fs_rb_পরবর্তী(গ্রন্থি);
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		গ্রন্থি = পরবর্তী;

		যদি ((ext->start + ext->গুনতি) <= start)
			চালু;

		/* No more merging */
		যদি ((start + len) <= ext->start)
			বিরতি;

		retval = 0;
		বিরতি;
	}
	ফেরত retval;
}

স্থির errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *in)
{
	কাঠামো ext2fs_rb_private *bp;
	unsigned অক্ষর *cp = in;
	size_t i;
	পূর্ণসংখ্যা first_set = -1;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;

	জন্য (i = 0; i < অঙ্ক; i++) {
		যদি ((i & 7) == 0) {
			unsigned অক্ষর c = cp[i/8];
			যদি (c == 0xFF) {
				যদি (first_set == -1)
					first_set = i;
				i += 7;
				চালু;
			}
			যদি ((c == 0x00) && (first_set == -1)) {
				i += 7;
				চালু;
			}
		}
		যদি (ext2fs_test_bit(i, in)) {
			যদি (first_set == -1)
				first_set = i;
			চালু;
		}
		যদি (first_set == -1)
			চালু;

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

	ফেরত 0;
}

স্থির errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *out)
{

	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, *পরবর্তী, **n;
	কাঠামো ext2fs_rb_private *bp;
	কাঠামো bmap_rb_extent *ext;
	__u64 গুনতি, pos;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	n = &bp->মূল.rb_গ্রন্থি;
	start -= bitmap->start;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ফেরত 0;

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা
			বিরতি;
	}

	memset(out, 0, (অঙ্ক + 7) >> 3);

	জন্য (; parent != ফাঁকা; parent = পরবর্তী) {
		পরবর্তী = ext2fs_rb_পরবর্তী(parent);
		ext = গ্রন্থি_to_extent(parent);

		pos = ext->start;
		গুনতি = ext->গুনতি;
		যদি (pos >= start + অঙ্ক)
			বিরতি;
		যদি (pos < start) {
			যদি (pos + গুনতি <  start)
				চালু;
			গুনতি -= start - pos;
			pos = start;
		}
		যদি (pos + গুনতি > start + অঙ্ক)
			গুনতি = start + অঙ্ক - pos;

		যখন (গুনতি > 0) {
			যদি ((গুনতি >= 8) &&
			    ((pos - start) % 8) == 0) {
				পূর্ণসংখ্যা nbytes = গুনতি >> 3;
				পূর্ণসংখ্যা offset = (pos - start) >> 3;

				memset(((অক্ষর *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				গুনতি -= nbytes << 3;
				চালু;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			গুনতি--;
		}
	}
	ফেরত 0;
}

স্থির শূন্য rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	কাঠামো ext2fs_rb_private *bp;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;

	rb_free_বৃক্ষ(&bp->মূল);
	bp->rcursor = ফাঁকা;
	bp->rcursor_পরবর্তী = ফাঁকা;
	bp->wcursor = ফাঁকা;
	check_বৃক্ষ(&bp->মূল, __func__);
}

স্থির errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, **n;
	কাঠামো ext2fs_rb_private *bp;
	কাঠামো bmap_rb_extent *ext;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	n = &bp->মূল.rb_গ্রন্থি;
	start -= bitmap->start;
	end -= bitmap->start;

	যদি (start > end)
		ফেরত EINVAL;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ফেরত ENOENT;

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা যদি (ext->start + ext->গুনতি <= end) {
			*out = ext->start + ext->গুনতি + bitmap->start;
			ফেরত 0;
		} অন্যথা
			ফেরত ENOENT;
	}

	*out = start + bitmap->start;
	ফেরত 0;
}

স্থির errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	কাঠামো rb_গ্রন্থি *parent = ফাঁকা, **n;
	কাঠামো rb_গ্রন্থি *গ্রন্থি;
	কাঠামো ext2fs_rb_private *bp;
	কাঠামো bmap_rb_extent *ext;

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;
	n = &bp->মূল.rb_গ্রন্থি;
	start -= bitmap->start;
	end -= bitmap->start;

	যদি (start > end)
		ফেরত EINVAL;

	যদি (ext2fs_rb_empty_মূল(&bp->মূল))
		ফেরত ENOENT;

	যখন (*n) {
		parent = *n;
		ext = গ্রন্থি_to_extent(parent);
		যদি (start < ext->start) {
			n = &(*n)->rb_বাম;
		} অন্যথা যদি (start >= (ext->start + ext->গুনতি)) {
			n = &(*n)->rb_ডান;
		} অন্যথা {
			/* The start bit is set */
			*out = start + bitmap->start;
			ফেরত 0;
		}
	}

	গ্রন্থি = parent;
	ext = গ্রন্থি_to_extent(গ্রন্থি);
	যদি (ext->start < start) {
		গ্রন্থি = ext2fs_rb_পরবর্তী(গ্রন্থি);
		যদি (গ্রন্থি == ফাঁকা)
			ফেরত ENOENT;
		ext = গ্রন্থি_to_extent(গ্রন্থি);
	}
	যদি (ext->start <= end) {
		*out = ext->start + bitmap->start;
		ফেরত 0;
	}
	ফেরত ENOENT;
}

#ifdef ENABLE_BMAP_STATS
স্থির শূন্য rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	কাঠামো ext2fs_rb_private *bp;
	কাঠামো rb_গ্রন্থি *গ্রন্থি = ফাঁকা;
	কাঠামো bmap_rb_extent *ext;
	__u64 গুনতি = 0;
	__u64 max_আকার = 0;
	__u64 min_আকার = ULONG_MAX;
	__u64 আকার_2 = 0, avg_আকার = 0;
	দ্বিগুণ eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	দ্বিগুণ m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (কাঠামো ext2fs_rb_private *) bitmap->ব্যক্তিগত;

	জন্য (গ্রন্থি = ext2fs_rb_first(&bp->মূল); গ্রন্থি != ফাঁকা;
	     গ্রন্থি = ext2fs_rb_পরবর্তী(গ্রন্থি)) {
		ext = গ্রন্থি_to_extent(গ্রন্থি);
		গুনতি++;
		যদি (ext->গুনতি > max_আকার)
			max_আকার = ext->গুনতি;
		যদি (ext->গুনতি < min_আকার)
			min_আকার = ext->গুনতি;
		আকার_2 += ext->গুনতি;
	}

	যদি (গুনতি)
		avg_আকার = আকার_2 / গুনতি;
	যদি (min_আকার == ULONG_MAX)
		min_আকার = 0;
	eff = (দ্বিগুণ)((গুনতি * আকার(কাঠামো bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_গুনতি + bitmap->stats.mark_ext_গুনতি;
	test_all = bitmap->stats.test_গুনতি + bitmap->stats.test_ext_গুনতি;
	যদি (mark_all)
		m_hit = ((দ্বিগুণ)bp->mark_hit / mark_all) * 100;
	যদি (test_all)
		t_hit = ((দ্বিগুণ)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) গুনতি, (unsigned long long)
		((গুনতি * আকার(কাঠামো bmap_rb_extent)) +
		 আকার(কাঠামো ext2fs_rb_private)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(unsigned long long) min_আকার);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(unsigned long long) max_আকার, (unsigned long long) avg_আকার);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(unsigned long long) আকার_2,
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

কাঠামো ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
