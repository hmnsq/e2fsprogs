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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#إذا HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_گرێ گرێ;
	__u64 start;
	__u64 ژماردن;
};

بنية ext2fs_rb_private {
	بنية rb_ڕەگ ڕەگ;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *گرێ_to_extent(بنية rb_گرێ *گرێ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) گرێ;
}

ساكن صحيح rb_زیادکردن_extent(__u64 start, __u64 ژماردن,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ print_درەخت(بنية rb_ڕەگ *ڕەگ)
{
	بنية rb_گرێ *گرێ = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گرێ = ext2fs_rb_first(ڕەگ);
	لكل (گرێ = ext2fs_rb_first(ڕەگ); گرێ != عدم; 
	     گرێ = ext2fs_rb_next(گرێ)) {
		ext = گرێ_to_extent(گرێ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->ژماردن);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_درەخت(بنية rb_ڕەگ *ڕەگ, ثابت حرف *پەیام)
{
	بنية rb_گرێ *گرێ;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (گرێ = ext2fs_rb_first(ڕەگ); گرێ;
	     گرێ = ext2fs_rb_next(گرێ)) {
		ext = گرێ_to_extent(گرێ);
		إذا (ext->ژماردن == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ژماردن,
				(unsigned long long) ext->ژماردن);
			اذهب err_out;
		}
		إذا (ext->start + ext->ژماردن < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ژماردن,
				(unsigned long long) ext->ژماردن);
			اذهب err_out;
		}

		إذا (old) {
			إذا (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ژماردن,
					(unsigned long long) old->ژماردن);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ژماردن,
					(unsigned long long) ext->ژماردن);
				اذهب err_out;
			}
			إذا ((old->start + old->ژماردن) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ژماردن,
					(unsigned long long) old->ژماردن);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ژماردن,
					(unsigned long long) ext->ژماردن);
				اذهب err_out;
			}
		}
		old = ext;
	}
	أرجع;

err_out:
	fprintf(stderr, "%s\n", پەیام);
	print_درەخت(ڕەگ);
	exit(1);
}
#وإلا
#define check_درەخت(ڕەگ, پەیام) افعل {} طالما (0)
#define print_درەخت(ڕەگ) افعل {} طالما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 ژماردن)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->start = start;
	new_ext->ژماردن = ژماردن;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_private *bp,
			   بنية bmap_rb_extent *ext)
{
	إذا (bp->wcursor == ext)
		bp->wcursor = عدم;
	إذا (bp->rcursor == ext)
		bp->rcursor = عدم;
	إذا (bp->rcursor_next == ext)
		bp->rcursor_next = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_private), &bp);
	إذا (retval)
		أرجع retval;

	bp->ڕەگ = RB_ڕەگ;
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_درەخت(بنية rb_ڕەگ *ڕەگ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرێ *گرێ, *next;

	لكل (گرێ = ext2fs_rb_first(ڕەگ); گرێ; گرێ = next) {
		next = ext2fs_rb_next(گرێ);
		ext = گرێ_to_extent(گرێ);
		ext2fs_rb_erase(گرێ, ڕەگ);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_درەخت(&bp->ڕەگ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_گرێ *dest_گرێ, *src_گرێ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_گرێ = ext2fs_rb_first(&src_bp->ڕەگ);
	طالما (src_گرێ) {
		src_ext = گرێ_to_extent(src_گرێ);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_گرێ = &dest_ext->گرێ;
		n = &dest_bp->ڕەگ.rb_گرێ;

		dest_last = عدم;
		إذا (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->ڕەگ);
			n = &(dest_last)->rb_ڕاست;
		}

		ext2fs_rb_link_گرێ(dest_گرێ, dest_last, n);
		ext2fs_rb_زیادکردن_color(dest_گرێ, &dest_bp->ڕەگ);

		src_گرێ = ext2fs_rb_next(src_گرێ);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_ڕەگ *ڕەگ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرێ *گرێ;

	گرێ = ext2fs_rb_last(ڕەگ);
	طالما (گرێ) {
		ext = گرێ_to_extent(گرێ);

		إذا ((ext->start + ext->ژماردن - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->start > new_max) {
			ext2fs_rb_erase(گرێ, ڕەگ);
			ext2fs_free_mem(&ext);
			گرێ = ext2fs_rb_last(ڕەگ);
			استمر;
		} وإلا
			ext->ژماردن = new_max - ext->start + 1;
	}
}

ساكن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bmap->خاص;
	bp->rcursor = عدم;
	bp->wcursor = عدم;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->ڕەگ);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	إذا (bmap->end < bmap->real_end)
		rb_زیادکردن_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_گرێ *parent = عدم, *next;
	بنية rb_گرێ **n = &bp->ڕەگ.rb_گرێ;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب درەختی_گەڕان;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->ژماردن) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		أرجع 1;
	}

	next_ext = bp->rcursor_next;
	إذا (!next_ext) {
		next = ext2fs_rb_next(&rcursor->گرێ);
		إذا (next)
			next_ext = گرێ_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	إذا (next_ext) {
		إذا ((bit >= rcursor->start + rcursor->ژماردن) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			أرجع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب درەختی_گەڕان;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->ژماردن)
		أرجع 1;

درەختی_گەڕان:

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (bit < ext->start)
			n = &(*n)->rb_چەپ;
		وإلا إذا (bit >= (ext->start + ext->ژماردن))
			n = &(*n)->rb_ڕاست;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_زیادکردن_extent(__u64 start, __u64 ژماردن,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_ڕەگ *ڕەگ = &bp->ڕەگ;
	بنية rb_گرێ *parent = عدم, **n = &ڕەگ->rb_گرێ;
	بنية rb_گرێ *new_گرێ, *گرێ, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (ژماردن == 0)
		أرجع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (start >= ext->start &&
		    start <= (ext->start + ext->ژماردن)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);

		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
		} وإلا إذا (start > (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
		} وإلا {
got_extent:
			إذا ((start + ژماردن) <= (ext->start + ext->ژماردن))
				أرجع 1;

			إذا ((ext->start + ext->ژماردن) == start)
				retval = 0;
			وإلا
				retval = 1;

			ژماردن += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_گرێ = &ext->گرێ;

			اذهب skip_زیادکردن;
		}
	}

	rb_get_new_extent(&new_ext, start, ژماردن);

	new_گرێ = &new_ext->گرێ;
	ext2fs_rb_link_گرێ(new_گرێ, parent, n);
	ext2fs_rb_زیادکردن_color(new_گرێ, ڕەگ);
	bp->wcursor = new_ext;

	گرێ = ext2fs_rb_prev(new_گرێ);
	إذا (گرێ) {
		ext = گرێ_to_extent(گرێ);
		إذا ((ext->start + ext->ژماردن) == start) {
			start = ext->start;
			ژماردن += ext->ژماردن;
			ext2fs_rb_erase(گرێ, ڕەگ);
			rb_free_extent(bp, ext);
		}
	}

skip_زیادکردن:
	/* See if we can merge extent to the right */
	لكل (گرێ = ext2fs_rb_next(new_گرێ); گرێ != عدم; گرێ = next) {
		next = ext2fs_rb_next(گرێ);
		ext = گرێ_to_extent(گرێ);

		إذا ((ext->start + ext->ژماردن) <= start)
			استمر;

		/* No more merging */
		إذا ((start + ژماردن) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((start + ژماردن) >= (ext->start + ext->ژماردن)) {
			ext2fs_rb_erase(گرێ, ڕەگ);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			ژماردن += ((ext->start + ext->ژماردن) -
				  (start + ژماردن));
			ext2fs_rb_erase(گرێ, ڕەگ);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->ژماردن = ژماردن;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 ژماردن,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_ڕەگ *ڕەگ = &bp->ڕەگ;
	بنية rb_گرێ *parent = عدم, **n = &ڕەگ->rb_گرێ;
	بنية rb_گرێ *گرێ;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_ژماردن;
	صحيح retval = 0;

	إذا (ext2fs_rb_empty_ڕەگ(ڕەگ))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
			استمر;
		} وإلا إذا (start >= (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
			استمر;
		}

		إذا ((start > ext->start) &&
		    (start + ژماردن) < (ext->start + ext->ژماردن)) {
			/* We have to split extent into two */
			new_start = start + ژماردن;
			new_ژماردن = (ext->start + ext->ژماردن) - new_start;

			ext->ژماردن = start - ext->start;

			rb_زیادکردن_extent(new_start, new_ژماردن, bp);
			أرجع 1;
		}

		إذا ((start + ژماردن) >= (ext->start + ext->ژماردن)) {
			ext->ژماردن = start - ext->start;
			retval = 1;
		}

		إذا (0 == ext->ژماردن) {
			parent = ext2fs_rb_next(&ext->گرێ);
			ext2fs_rb_erase(&ext->گرێ, ڕەگ);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (start == ext->start) {
			ext->start += ژماردن;
			ext->ژماردن -= ژماردن;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = گرێ) {
		گرێ = ext2fs_rb_next(parent);
		ext = گرێ_to_extent(parent);
		إذا ((ext->start + ext->ژماردن) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((start + ژماردن) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((start + ژماردن) >= (ext->start + ext->ژماردن)) {
			ext2fs_rb_erase(parent, ڕەگ);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->ژماردن -= ((start + ژماردن) - ext->start);
			ext->start = start + ژماردن;
			retval = 1;
			اكسر;
		}
	}

	أرجع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_زیادکردن_extent(arg, 1, bp);
	check_درەخت(&bp->ڕەگ, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_درەخت(&bp->ڕەگ, __func__);

	أرجع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	أرجع rb_test_bit(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned صحيح ژمارە)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_زیادکردن_extent(arg, ژمارە, bp);
	check_درەخت(&bp->ڕەگ, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح ژمارە)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, ژمارە, bp);
	check_درەخت(&bp->ڕەگ, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_گرێ *parent = عدم, **n;
	بنية rb_گرێ *گرێ, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->ڕەگ.rb_گرێ;
	start -= bitmap->start;

	إذا (len == 0 || ext2fs_rb_empty_ڕەگ(&bp->ڕەگ))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
		} وإلا إذا (start >= (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	گرێ = parent;
	طالما (گرێ) {
		next = ext2fs_rb_next(گرێ);
		ext = گرێ_to_extent(گرێ);
		گرێ = next;

		إذا ((ext->start + ext->ژماردن) <= start)
			استمر;

		/* No more merging */
		إذا ((start + len) <= ext->start)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ژمارە, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < ژمارە; i++) {
		إذا ((i & 7) == 0) {
			unsigned حرف c = cp[i/8];
			إذا (c == 0xFF) {
				إذا (first_set == -1)
					first_set = i;
				i += 7;
				استمر;
			}
			إذا ((c == 0x00) && (first_set == -1)) {
				i += 7;
				استمر;
			}
		}
		إذا (ext2fs_test_bit(i, in)) {
			إذا (first_set == -1)
				first_set = i;
			استمر;
		}
		إذا (first_set == -1)
			استمر;

		rb_زیادکردن_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_درەخت(&bp->ڕەگ, __func__);
		first_set = -1;
	}
	إذا (first_set != -1) {
		rb_زیادکردن_extent(start + first_set - bitmap->start,
				 ژمارە - first_set, bp);
		check_درەخت(&bp->ڕەگ, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ژمارە, فراغ *out)
{

	بنية rb_گرێ *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 ژماردن, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->ڕەگ.rb_گرێ;
	start -= bitmap->start;

	إذا (ext2fs_rb_empty_ڕەگ(&bp->ڕەگ))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
		} وإلا إذا (start >= (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
		} وإلا
			اكسر;
	}

	memset(out, 0, (ژمارە + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = گرێ_to_extent(parent);

		pos = ext->start;
		ژماردن = ext->ژماردن;
		إذا (pos >= start + ژمارە)
			اكسر;
		إذا (pos < start) {
			إذا (pos + ژماردن <  start)
				استمر;
			ژماردن -= start - pos;
			pos = start;
		}
		إذا (pos + ژماردن > start + ژمارە)
			ژماردن = start + ژمارە - pos;

		طالما (ژماردن > 0) {
			إذا ((ژماردن >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = ژماردن >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				ژماردن -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			ژماردن--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_درەخت(&bp->ڕەگ);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_درەخت(&bp->ڕەگ, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_گرێ *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->ڕەگ.rb_گرێ;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_ڕەگ(&bp->ڕەگ))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
		} وإلا إذا (start >= (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
		} وإلا إذا (ext->start + ext->ژماردن <= end) {
			*out = ext->start + ext->ژماردن + bitmap->start;
			أرجع 0;
		} وإلا
			أرجع ENOENT;
	}

	*out = start + bitmap->start;
	أرجع 0;
}

ساكن errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_گرێ *parent = عدم, **n;
	بنية rb_گرێ *گرێ;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->ڕەگ.rb_گرێ;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_ڕەگ(&bp->ڕەگ))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = گرێ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_چەپ;
		} وإلا إذا (start >= (ext->start + ext->ژماردن)) {
			n = &(*n)->rb_ڕاست;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			أرجع 0;
		}
	}

	گرێ = parent;
	ext = گرێ_to_extent(گرێ);
	إذا (ext->start < start) {
		گرێ = ext2fs_rb_next(گرێ);
		إذا (گرێ == عدم)
			أرجع ENOENT;
		ext = گرێ_to_extent(گرێ);
	}
	إذا (ext->start <= end) {
		*out = ext->start + bitmap->start;
		أرجع 0;
	}
	أرجع ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	بنية rb_گرێ *گرێ = عدم;
	بنية bmap_rb_extent *ext;
	__u64 ژماردن = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (گرێ = ext2fs_rb_first(&bp->ڕەگ); گرێ != عدم;
	     گرێ = ext2fs_rb_next(گرێ)) {
		ext = گرێ_to_extent(گرێ);
		ژماردن++;
		إذا (ext->ژماردن > max_size)
			max_size = ext->ژماردن;
		إذا (ext->ژماردن < min_size)
			min_size = ext->ژماردن;
		size += ext->ژماردن;
	}

	إذا (ژماردن)
		avg_size = size / ژماردن;
	إذا (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((ژماردن * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ژماردن + bitmap->stats.mark_ext_ژماردن;
	test_all = bitmap->stats.test_ژماردن + bitmap->stats.test_ext_ژماردن;
	إذا (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	إذا (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) ژماردن, (unsigned long long)
		((ژماردن * حجم(بنية bmap_rb_extent)) +
		 حجم(بنية ext2fs_rb_private)));
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
#وإلا
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
