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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_LINUX_TYPES_H
#அந்தர்பாவ <linux/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "bmap64.h"
#அந்தர்பாவ "rbtree.h"

#அந்தர்பாவ <limits.h>

கட்டமைப்பு bmap_rb_extent {
	கட்டமைப்பு rb_முனை முனை;
	__u64 start;
	__u64 மொத்தம்;
};

கட்டமைப்பு ext2fs_rb_private {
	கட்டமைப்பு rb_வேர் வேர்;
	கட்டமைப்பு bmap_rb_extent *wcursor;
	கட்டமைப்பு bmap_rb_extent *rcursor;
	கட்டமைப்பு bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

உள்வரி நிலையான கட்டமைப்பு bmap_rb_extent *முனை_to_extent(கட்டமைப்பு rb_முனை *முனை)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	திரும்பு (கட்டமைப்பு bmap_rb_extent *) முனை;
}

நிலையான முழு rb_செருகு_extent(__u64 start, __u64 மொத்தம்,
			    கட்டமைப்பு ext2fs_rb_private *);
நிலையான வெறுமை rb_get_new_extent(கட்டமைப்பு bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
நிலையான வெறுமை print_மரம்(கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு rb_முனை *முனை = வெற்று;
	கட்டமைப்பு bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	முனை = ext2fs_rb_first(வேர்);
	க்கு (முனை = ext2fs_rb_first(வேர்); முனை != வெற்று; 
	     முனை = ext2fs_rb_next(முனை)) {
		ext = முனை_to_extent(முனை);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->மொத்தம்);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

நிலையான வெறுமை check_மரம்(கட்டமைப்பு rb_வேர் *வேர், மாறிலி எழுத்து *செய்தி)
{
	கட்டமைப்பு rb_முனை *முனை;
	கட்டமைப்பு bmap_rb_extent *ext, *old = வெற்று;

	க்கு (முனை = ext2fs_rb_first(வேர்); முனை;
	     முனை = ext2fs_rb_next(முனை)) {
		ext = முனை_to_extent(முனை);
		எனில் (ext->மொத்தம் == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->மொத்தம்,
				(unsigned long long) ext->மொத்தம்);
			செல் err_out;
		}
		எனில் (ext->start + ext->மொத்தம் < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->மொத்தம்,
				(unsigned long long) ext->மொத்தம்);
			செல் err_out;
		}

		எனில் (old) {
			எனில் (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->மொத்தம்,
					(unsigned long long) old->மொத்தம்);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->மொத்தம்,
					(unsigned long long) ext->மொத்தம்);
				செல் err_out;
			}
			எனில் ((old->start + old->மொத்தம்) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->மொத்தம்,
					(unsigned long long) old->மொத்தம்);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->மொத்தம்,
					(unsigned long long) ext->மொத்தம்);
				செல் err_out;
			}
		}
		old = ext;
	}
	திரும்பு;

err_out:
	fprintf(stderr, "%s\n", செய்தி);
	print_மரம்(வேர்);
	exit(1);
}
#இல்லையெனில்
#define check_மரம்(வேர், செய்தி) செய் {} போது (0)
#define print_மரம்(வேர்) செய் {} போது (0)
#endif

நிலையான வெறுமை rb_get_new_extent(கட்டமைப்பு bmap_rb_extent **ext, __u64 start,
			     __u64 மொத்தம்)
{
	கட்டமைப்பு bmap_rb_extent *new_ext;
	முழு retval;

	retval = ext2fs_get_mem(அளவு (கட்டமைப்பு bmap_rb_extent),
				&new_ext);
	எனில் (retval)
		abort();

	new_ext->start = start;
	new_ext->மொத்தம் = மொத்தம்;
	*ext = new_ext;
}

உள்வரி
நிலையான வெறுமை rb_free_extent(கட்டமைப்பு ext2fs_rb_private *bp,
			   கட்டமைப்பு bmap_rb_extent *ext)
{
	எனில் (bp->wcursor == ext)
		bp->wcursor = வெற்று;
	எனில் (bp->rcursor == ext)
		bp->rcursor = வெற்று;
	எனில் (bp->rcursor_next == ext)
		bp->rcursor_next = வெற்று;
	ext2fs_free_mem(&ext);
}

நிலையான errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	கட்டமைப்பு ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(அளவு (கட்டமைப்பு ext2fs_rb_private), &bp);
	எனில் (retval)
		திரும்பு retval;

	bp->வேர் = RB_வேர்;
	bp->rcursor = வெற்று;
	bp->rcursor_next = வெற்று;
	bp->wcursor = வெற்று;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->தனிப்பட்ட = (வெறுமை *) bp;
	திரும்பு 0;
}

நிலையான errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	எனில் (retval)
		திரும்பு retval;

	திரும்பு 0;
}

நிலையான வெறுமை rb_free_மரம்(கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு bmap_rb_extent *ext;
	கட்டமைப்பு rb_முனை *முனை, *next;

	க்கு (முனை = ext2fs_rb_first(வேர்); முனை; முனை = next) {
		next = ext2fs_rb_next(முனை);
		ext = முனை_to_extent(முனை);
		ext2fs_rb_erase(முனை, வேர்);
		ext2fs_free_mem(&ext);
	}
}

நிலையான வெறுமை rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;

	rb_free_மரம்(&bp->வேர்);
	ext2fs_free_mem(&bp);
	bp = 0;
}

நிலையான errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	கட்டமைப்பு ext2fs_rb_private *src_bp, *dest_bp;
	கட்டமைப்பு bmap_rb_extent *src_ext, *dest_ext;
	கட்டமைப்பு rb_முனை *dest_முனை, *src_முனை, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	எனில் (retval)
		திரும்பு retval;

	src_bp = (கட்டமைப்பு ext2fs_rb_private *) src->தனிப்பட்ட;
	dest_bp = (கட்டமைப்பு ext2fs_rb_private *) dest->தனிப்பட்ட;
	src_bp->rcursor = வெற்று;
	dest_bp->rcursor = வெற்று;

	src_முனை = ext2fs_rb_first(&src_bp->வேர்);
	போது (src_முனை) {
		src_ext = முனை_to_extent(src_முனை);
		retval = ext2fs_get_mem(அளவு (கட்டமைப்பு bmap_rb_extent),
					&dest_ext);
		எனில் (retval)
			நிறுத்து;

		memcpy(dest_ext, src_ext, அளவு(கட்டமைப்பு bmap_rb_extent));

		dest_முனை = &dest_ext->முனை;
		n = &dest_bp->வேர்.rb_முனை;

		dest_last = வெற்று;
		எனில் (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->வேர்);
			n = &(dest_last)->rb_வலது;
		}

		ext2fs_rb_link_முனை(dest_முனை, dest_last, n);
		ext2fs_rb_செருகு_color(dest_முனை, &dest_bp->வேர்);

		src_முனை = ext2fs_rb_next(src_முனை);
	}

	திரும்பு retval;
}

நிலையான வெறுமை rb_truncate(__u64 new_max, கட்டமைப்பு rb_வேர் *வேர்)
{
	கட்டமைப்பு bmap_rb_extent *ext;
	கட்டமைப்பு rb_முனை *முனை;

	முனை = ext2fs_rb_last(வேர்);
	போது (முனை) {
		ext = முனை_to_extent(முனை);

		எனில் ((ext->start + ext->மொத்தம் - 1) <= new_max)
			நிறுத்து;
		இல்லையெனில் எனில் (ext->start > new_max) {
			ext2fs_rb_erase(முனை, வேர்);
			ext2fs_free_mem(&ext);
			முனை = ext2fs_rb_last(வேர்);
			தொடர்;
		} இல்லையெனில்
			ext->மொத்தம் = new_max - ext->start + 1;
	}
}

நிலையான errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bmap->தனிப்பட்ட;
	bp->rcursor = வெற்று;
	bp->wcursor = வெற்று;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->வேர்);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	எனில் (bmap->end < bmap->real_end)
		rb_செருகு_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	திரும்பு 0;

}

உள்வரி நிலையான முழு
rb_test_bit(கட்டமைப்பு ext2fs_rb_private *bp, __u64 bit)
{
	கட்டமைப்பு bmap_rb_extent *rcursor, *next_ext = வெற்று;
	கட்டமைப்பு rb_முனை *parent = வெற்று, *next;
	கட்டமைப்பு rb_முனை **n = &bp->வேர்.rb_முனை;
	கட்டமைப்பு bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	எனில் (!rcursor)
		செல் தேடல்_மரம்;

	எனில் (bit >= rcursor->start && bit < rcursor->start + rcursor->மொத்தம்) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		திரும்பு 1;
	}

	next_ext = bp->rcursor_next;
	எனில் (!next_ext) {
		next = ext2fs_rb_next(&rcursor->முனை);
		எனில் (next)
			next_ext = முனை_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	எனில் (next_ext) {
		எனில் ((bit >= rcursor->start + rcursor->மொத்தம்) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			திரும்பு 0;
		}
	}
	bp->rcursor = வெற்று;
	bp->rcursor_next = வெற்று;

	rcursor = bp->wcursor;
	எனில் (!rcursor)
		செல் தேடல்_மரம்;

	எனில் (bit >= rcursor->start && bit < rcursor->start + rcursor->மொத்தம்)
		திரும்பு 1;

தேடல்_மரம்:

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (bit < ext->start)
			n = &(*n)->rb_இடது;
		இல்லையெனில் எனில் (bit >= (ext->start + ext->மொத்தம்))
			n = &(*n)->rb_வலது;
		இல்லையெனில் {
			bp->rcursor = ext;
			bp->rcursor_next = வெற்று;
			திரும்பு 1;
		}
	}
	திரும்பு 0;
}

நிலையான முழு rb_செருகு_extent(__u64 start, __u64 மொத்தம்,
			    கட்டமைப்பு ext2fs_rb_private *bp)
{
	கட்டமைப்பு rb_வேர் *வேர் = &bp->வேர்;
	கட்டமைப்பு rb_முனை *parent = வெற்று, **n = &வேர்->rb_முனை;
	கட்டமைப்பு rb_முனை *new_முனை, *முனை, *next;
	கட்டமைப்பு bmap_rb_extent *new_ext;
	கட்டமைப்பு bmap_rb_extent *ext;
	முழு retval = 0;

	எனில் (மொத்தம் == 0)
		திரும்பு 0;

	bp->rcursor_next = வெற்று;
	ext = bp->wcursor;
	எனில் (ext) {
		எனில் (start >= ext->start &&
		    start <= (ext->start + ext->மொத்தம்)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			செல் got_extent;
		}
	}

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);

		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
		} இல்லையெனில் எனில் (start > (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
		} இல்லையெனில் {
got_extent:
			எனில் ((start + மொத்தம்) <= (ext->start + ext->மொத்தம்))
				திரும்பு 1;

			எனில் ((ext->start + ext->மொத்தம்) == start)
				retval = 0;
			இல்லையெனில்
				retval = 1;

			மொத்தம் += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_முனை = &ext->முனை;

			செல் skip_செருகு;
		}
	}

	rb_get_new_extent(&new_ext, start, மொத்தம்);

	new_முனை = &new_ext->முனை;
	ext2fs_rb_link_முனை(new_முனை, parent, n);
	ext2fs_rb_செருகு_color(new_முனை, வேர்);
	bp->wcursor = new_ext;

	முனை = ext2fs_rb_prev(new_முனை);
	எனில் (முனை) {
		ext = முனை_to_extent(முனை);
		எனில் ((ext->start + ext->மொத்தம்) == start) {
			start = ext->start;
			மொத்தம் += ext->மொத்தம்;
			ext2fs_rb_erase(முனை, வேர்);
			rb_free_extent(bp, ext);
		}
	}

skip_செருகு:
	/* See if we can merge extent to the right */
	க்கு (முனை = ext2fs_rb_next(new_முனை); முனை != வெற்று; முனை = next) {
		next = ext2fs_rb_next(முனை);
		ext = முனை_to_extent(முனை);

		எனில் ((ext->start + ext->மொத்தம்) <= start)
			தொடர்;

		/* No more merging */
		எனில் ((start + மொத்தம்) < ext->start)
			நிறுத்து;

		/* ext is embedded in new_ext interval */
		எனில் ((start + மொத்தம்) >= (ext->start + ext->மொத்தம்)) {
			ext2fs_rb_erase(முனை, வேர்);
			rb_free_extent(bp, ext);
			தொடர்;
		} இல்லையெனில் {
		/* merge ext with new_ext */
			மொத்தம் += ((ext->start + ext->மொத்தம்) -
				  (start + மொத்தம்));
			ext2fs_rb_erase(முனை, வேர்);
			rb_free_extent(bp, ext);
			நிறுத்து;
		}
	}

	new_ext->start = start;
	new_ext->மொத்தம் = மொத்தம்;

	திரும்பு retval;
}

நிலையான முழு rb_remove_extent(__u64 start, __u64 மொத்தம்,
			    கட்டமைப்பு ext2fs_rb_private *bp)
{
	கட்டமைப்பு rb_வேர் *வேர் = &bp->வேர்;
	கட்டமைப்பு rb_முனை *parent = வெற்று, **n = &வேர்->rb_முனை;
	கட்டமைப்பு rb_முனை *முனை;
	கட்டமைப்பு bmap_rb_extent *ext;
	__u64 new_start, new_மொத்தம்;
	முழு retval = 0;

	எனில் (ext2fs_rb_empty_வேர்(வேர்))
		திரும்பு 0;

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
			தொடர்;
		} இல்லையெனில் எனில் (start >= (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
			தொடர்;
		}

		எனில் ((start > ext->start) &&
		    (start + மொத்தம்) < (ext->start + ext->மொத்தம்)) {
			/* We have to split extent into two */
			new_start = start + மொத்தம்;
			new_மொத்தம் = (ext->start + ext->மொத்தம்) - new_start;

			ext->மொத்தம் = start - ext->start;

			rb_செருகு_extent(new_start, new_மொத்தம், bp);
			திரும்பு 1;
		}

		எனில் ((start + மொத்தம்) >= (ext->start + ext->மொத்தம்)) {
			ext->மொத்தம் = start - ext->start;
			retval = 1;
		}

		எனில் (0 == ext->மொத்தம்) {
			parent = ext2fs_rb_next(&ext->முனை);
			ext2fs_rb_erase(&ext->முனை, வேர்);
			rb_free_extent(bp, ext);
			நிறுத்து;
		}

		எனில் (start == ext->start) {
			ext->start += மொத்தம்;
			ext->மொத்தம் -= மொத்தம்;
			திரும்பு 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	க்கு (; parent != வெற்று; parent = முனை) {
		முனை = ext2fs_rb_next(parent);
		ext = முனை_to_extent(parent);
		எனில் ((ext->start + ext->மொத்தம்) <= start)
			தொடர்;

		/* No more extents to be removed/truncated */
		எனில் ((start + மொத்தம்) < ext->start)
			நிறுத்து;

		/* The entire extent is within the region to be removed */
		எனில் ((start + மொத்தம்) >= (ext->start + ext->மொத்தம்)) {
			ext2fs_rb_erase(parent, வேர்);
			rb_free_extent(bp, ext);
			retval = 1;
			தொடர்;
		} இல்லையெனில் {
			/* modify the last extent in region to be removed */
			ext->மொத்தம் -= ((start + மொத்தம்) - ext->start);
			ext->start = start + மொத்தம்;
			retval = 1;
			நிறுத்து;
		}
	}

	திரும்பு retval;
}

நிலையான முழு rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	கட்டமைப்பு ext2fs_rb_private *bp;
	முழு retval;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	arg -= bitmap->start;

	retval = rb_செருகு_extent(arg, 1, bp);
	check_மரம்(&bp->வேர், __func__);
	திரும்பு retval;
}

நிலையான முழு rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	கட்டமைப்பு ext2fs_rb_private *bp;
	முழு retval;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_மரம்(&bp->வேர், __func__);

	திரும்பு retval;
}

உள்வரி
நிலையான முழு rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	arg -= bitmap->start;

	திரும்பு rb_test_bit(bp, arg);
}

நிலையான வெறுமை rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned முழு இலக்கம்)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	arg -= bitmap->start;

	rb_செருகு_extent(arg, இலக்கம், bp);
	check_மரம்(&bp->வேர், __func__);
}

நிலையான வெறுமை rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned முழு இலக்கம்)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	arg -= bitmap->start;

	rb_remove_extent(arg, இலக்கம், bp);
	check_மரம்(&bp->வேர், __func__);
}

நிலையான முழு rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned முழு len)
{
	கட்டமைப்பு rb_முனை *parent = வெற்று, **n;
	கட்டமைப்பு rb_முனை *முனை, *next;
	கட்டமைப்பு ext2fs_rb_private *bp;
	கட்டமைப்பு bmap_rb_extent *ext;
	முழு retval = 1;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	n = &bp->வேர்.rb_முனை;
	start -= bitmap->start;

	எனில் (len == 0 || ext2fs_rb_empty_வேர்(&bp->வேர்))
		திரும்பு 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
		} இல்லையெனில் எனில் (start >= (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
		} இல்லையெனில் {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			திரும்பு 0;
		}
	}

	முனை = parent;
	போது (முனை) {
		next = ext2fs_rb_next(முனை);
		ext = முனை_to_extent(முனை);
		முனை = next;

		எனில் ((ext->start + ext->மொத்தம்) <= start)
			தொடர்;

		/* No more merging */
		எனில் ((start + len) <= ext->start)
			நிறுத்து;

		retval = 0;
		நிறுத்து;
	}
	திரும்பு retval;
}

நிலையான errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t இலக்கம், வெறுமை *in)
{
	கட்டமைப்பு ext2fs_rb_private *bp;
	unsigned எழுத்து *cp = in;
	size_t i;
	முழு first_set = -1;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;

	க்கு (i = 0; i < இலக்கம்; i++) {
		எனில் ((i & 7) == 0) {
			unsigned எழுத்து c = cp[i/8];
			எனில் (c == 0xFF) {
				எனில் (first_set == -1)
					first_set = i;
				i += 7;
				தொடர்;
			}
			எனில் ((c == 0x00) && (first_set == -1)) {
				i += 7;
				தொடர்;
			}
		}
		எனில் (ext2fs_test_bit(i, in)) {
			எனில் (first_set == -1)
				first_set = i;
			தொடர்;
		}
		எனில் (first_set == -1)
			தொடர்;

		rb_செருகு_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_மரம்(&bp->வேர், __func__);
		first_set = -1;
	}
	எனில் (first_set != -1) {
		rb_செருகு_extent(start + first_set - bitmap->start,
				 இலக்கம் - first_set, bp);
		check_மரம்(&bp->வேர், __func__);
	}

	திரும்பு 0;
}

நிலையான errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t இலக்கம், வெறுமை *out)
{

	கட்டமைப்பு rb_முனை *parent = வெற்று, *next, **n;
	கட்டமைப்பு ext2fs_rb_private *bp;
	கட்டமைப்பு bmap_rb_extent *ext;
	__u64 மொத்தம், pos;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	n = &bp->வேர்.rb_முனை;
	start -= bitmap->start;

	எனில் (ext2fs_rb_empty_வேர்(&bp->வேர்))
		திரும்பு 0;

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
		} இல்லையெனில் எனில் (start >= (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
		} இல்லையெனில்
			நிறுத்து;
	}

	memset(out, 0, (இலக்கம் + 7) >> 3);

	க்கு (; parent != வெற்று; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = முனை_to_extent(parent);

		pos = ext->start;
		மொத்தம் = ext->மொத்தம்;
		எனில் (pos >= start + இலக்கம்)
			நிறுத்து;
		எனில் (pos < start) {
			எனில் (pos + மொத்தம் <  start)
				தொடர்;
			மொத்தம் -= start - pos;
			pos = start;
		}
		எனில் (pos + மொத்தம் > start + இலக்கம்)
			மொத்தம் = start + இலக்கம் - pos;

		போது (மொத்தம் > 0) {
			எனில் ((மொத்தம் >= 8) &&
			    ((pos - start) % 8) == 0) {
				முழு nbytes = மொத்தம் >> 3;
				முழு offset = (pos - start) >> 3;

				memset(((எழுத்து *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				மொத்தம் -= nbytes << 3;
				தொடர்;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			மொத்தம்--;
		}
	}
	திரும்பு 0;
}

நிலையான வெறுமை rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	கட்டமைப்பு ext2fs_rb_private *bp;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;

	rb_free_மரம்(&bp->வேர்);
	bp->rcursor = வெற்று;
	bp->rcursor_next = வெற்று;
	bp->wcursor = வெற்று;
	check_மரம்(&bp->வேர், __func__);
}

நிலையான errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	கட்டமைப்பு rb_முனை *parent = வெற்று, **n;
	கட்டமைப்பு ext2fs_rb_private *bp;
	கட்டமைப்பு bmap_rb_extent *ext;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	n = &bp->வேர்.rb_முனை;
	start -= bitmap->start;
	end -= bitmap->start;

	எனில் (start > end)
		திரும்பு EINVAL;

	எனில் (ext2fs_rb_empty_வேர்(&bp->வேர்))
		திரும்பு ENOENT;

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
		} இல்லையெனில் எனில் (start >= (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
		} இல்லையெனில் எனில் (ext->start + ext->மொத்தம் <= end) {
			*out = ext->start + ext->மொத்தம் + bitmap->start;
			திரும்பு 0;
		} இல்லையெனில்
			திரும்பு ENOENT;
	}

	*out = start + bitmap->start;
	திரும்பு 0;
}

நிலையான errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	கட்டமைப்பு rb_முனை *parent = வெற்று, **n;
	கட்டமைப்பு rb_முனை *முனை;
	கட்டமைப்பு ext2fs_rb_private *bp;
	கட்டமைப்பு bmap_rb_extent *ext;

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;
	n = &bp->வேர்.rb_முனை;
	start -= bitmap->start;
	end -= bitmap->start;

	எனில் (start > end)
		திரும்பு EINVAL;

	எனில் (ext2fs_rb_empty_வேர்(&bp->வேர்))
		திரும்பு ENOENT;

	போது (*n) {
		parent = *n;
		ext = முனை_to_extent(parent);
		எனில் (start < ext->start) {
			n = &(*n)->rb_இடது;
		} இல்லையெனில் எனில் (start >= (ext->start + ext->மொத்தம்)) {
			n = &(*n)->rb_வலது;
		} இல்லையெனில் {
			/* The start bit is set */
			*out = start + bitmap->start;
			திரும்பு 0;
		}
	}

	முனை = parent;
	ext = முனை_to_extent(முனை);
	எனில் (ext->start < start) {
		முனை = ext2fs_rb_next(முனை);
		எனில் (முனை == வெற்று)
			திரும்பு ENOENT;
		ext = முனை_to_extent(முனை);
	}
	எனில் (ext->start <= end) {
		*out = ext->start + bitmap->start;
		திரும்பு 0;
	}
	திரும்பு ENOENT;
}

#ifdef ENABLE_BMAP_STATS
நிலையான வெறுமை rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	கட்டமைப்பு ext2fs_rb_private *bp;
	கட்டமைப்பு rb_முனை *முனை = வெற்று;
	கட்டமைப்பு bmap_rb_extent *ext;
	__u64 மொத்தம் = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	இரட்டை eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	இரட்டை m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (கட்டமைப்பு ext2fs_rb_private *) bitmap->தனிப்பட்ட;

	க்கு (முனை = ext2fs_rb_first(&bp->வேர்); முனை != வெற்று;
	     முனை = ext2fs_rb_next(முனை)) {
		ext = முனை_to_extent(முனை);
		மொத்தம்++;
		எனில் (ext->மொத்தம் > max_size)
			max_size = ext->மொத்தம்;
		எனில் (ext->மொத்தம் < min_size)
			min_size = ext->மொத்தம்;
		size += ext->மொத்தம்;
	}

	எனில் (மொத்தம்)
		avg_size = size / மொத்தம்;
	எனில் (min_size == ULONG_MAX)
		min_size = 0;
	eff = (இரட்டை)((மொத்தம் * அளவு(கட்டமைப்பு bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_மொத்தம் + bitmap->stats.mark_ext_மொத்தம்;
	test_all = bitmap->stats.test_மொத்தம் + bitmap->stats.test_ext_மொத்தம்;
	எனில் (mark_all)
		m_hit = ((இரட்டை)bp->mark_hit / mark_all) * 100;
	எனில் (test_all)
		t_hit = ((இரட்டை)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) மொத்தம், (unsigned long long)
		((மொத்தம் * அளவு(கட்டமைப்பு bmap_rb_extent)) +
		 அளவு(கட்டமைப்பு ext2fs_rb_private)));
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
#இல்லையெனில்
நிலையான வெறுமை rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

கட்டமைப்பு ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
