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
	بنية rb_كلي كلي;
	__u64 start;
	__u64 كدي;
};

بنية ext2fs_rb_private {
	بنية rb_سيو سيو;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *كلي_to_extent(بنية rb_كلي *كلي)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) كلي;
}

ساكن صحيح rb_سك_extent(__u64 start, __u64 كدي,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ بغ_بشيا(بنية rb_سيو *سيو)
{
	بنية rb_كلي *كلي = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	كلي = ext2fs_rb_first(سيو);
	لكل (كلي = ext2fs_rb_first(سيو); كلي != عدم; 
	     كلي = ext2fs_rb_next(كلي)) {
		ext = كلي_to_extent(كلي);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->كدي);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_بشيا(بنية rb_سيو *سيو, ثابت حرف *سكو)
{
	بنية rb_كلي *كلي;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (كلي = ext2fs_rb_first(سيو); كلي;
	     كلي = ext2fs_rb_next(كلي)) {
		ext = كلي_to_extent(كلي);
		إذا (ext->كدي == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->كدي,
				(unsigned long long) ext->كدي);
			اذهب err_out;
		}
		إذا (ext->start + ext->كدي < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->كدي,
				(unsigned long long) ext->كدي);
			اذهب err_out;
		}

		إذا (old) {
			إذا (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->كدي,
					(unsigned long long) old->كدي);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->كدي,
					(unsigned long long) ext->كدي);
				اذهب err_out;
			}
			إذا ((old->start + old->كدي) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->كدي,
					(unsigned long long) old->كدي);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->كدي,
					(unsigned long long) ext->كدي);
				اذهب err_out;
			}
		}
		old = ext;
	}
	أرجع;

err_out:
	fprintf(stderr, "%s\n", سكو);
	بغ_بشيا(سيو);
	exit(1);
}
#وإلا
#define check_بشيا(سيو, سكو) افعل {} طالما (0)
#define بغ_بشيا(سيو) افعل {} طالما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 كدي)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->start = start;
	new_ext->كدي = كدي;
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

	bp->سيو = RB_سيو;
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

ساكن فراغ rb_free_بشيا(بنية rb_سيو *سيو)
{
	بنية bmap_rb_extent *ext;
	بنية rb_كلي *كلي, *next;

	لكل (كلي = ext2fs_rb_first(سيو); كلي; كلي = next) {
		next = ext2fs_rb_next(كلي);
		ext = كلي_to_extent(كلي);
		ext2fs_rb_erase(كلي, سيو);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_بشيا(&bp->سيو);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_كلي *dest_كلي, *src_كلي, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_كلي = ext2fs_rb_first(&src_bp->سيو);
	طالما (src_كلي) {
		src_ext = كلي_to_extent(src_كلي);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_كلي = &dest_ext->كلي;
		n = &dest_bp->سيو.rb_كلي;

		dest_last = عدم;
		إذا (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->سيو);
			n = &(dest_last)->rb_دام;
		}

		ext2fs_rb_link_كلي(dest_كلي, dest_last, n);
		ext2fs_rb_سك_color(dest_كلي, &dest_bp->سيو);

		src_كلي = ext2fs_rb_next(src_كلي);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_سيو *سيو)
{
	بنية bmap_rb_extent *ext;
	بنية rb_كلي *كلي;

	كلي = ext2fs_rb_last(سيو);
	طالما (كلي) {
		ext = كلي_to_extent(كلي);

		إذا ((ext->start + ext->كدي - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->start > new_max) {
			ext2fs_rb_erase(كلي, سيو);
			ext2fs_free_mem(&ext);
			كلي = ext2fs_rb_last(سيو);
			استمر;
		} وإلا
			ext->كدي = new_max - ext->start + 1;
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
		    &bp->سيو);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	إذا (bmap->end < bmap->real_end)
		rb_سك_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_كلي *parent = عدم, *next;
	بنية rb_كلي **n = &bp->سيو.rb_كلي;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب نيما_بشيا;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->كدي) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		أرجع 1;
	}

	next_ext = bp->rcursor_next;
	إذا (!next_ext) {
		next = ext2fs_rb_next(&rcursor->كلي);
		إذا (next)
			next_ext = كلي_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	إذا (next_ext) {
		إذا ((bit >= rcursor->start + rcursor->كدي) &&
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
		اذهب نيما_بشيا;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->كدي)
		أرجع 1;

نيما_بشيا:

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (bit < ext->start)
			n = &(*n)->rb_هغو;
		وإلا إذا (bit >= (ext->start + ext->كدي))
			n = &(*n)->rb_دام;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_سك_extent(__u64 start, __u64 كدي,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_سيو *سيو = &bp->سيو;
	بنية rb_كلي *parent = عدم, **n = &سيو->rb_كلي;
	بنية rb_كلي *new_كلي, *كلي, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (كدي == 0)
		أرجع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (start >= ext->start &&
		    start <= (ext->start + ext->كدي)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);

		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
		} وإلا إذا (start > (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
		} وإلا {
got_extent:
			إذا ((start + كدي) <= (ext->start + ext->كدي))
				أرجع 1;

			إذا ((ext->start + ext->كدي) == start)
				retval = 0;
			وإلا
				retval = 1;

			كدي += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_كلي = &ext->كلي;

			اذهب skip_سك;
		}
	}

	rb_get_new_extent(&new_ext, start, كدي);

	new_كلي = &new_ext->كلي;
	ext2fs_rb_link_كلي(new_كلي, parent, n);
	ext2fs_rb_سك_color(new_كلي, سيو);
	bp->wcursor = new_ext;

	كلي = ext2fs_rb_prev(new_كلي);
	إذا (كلي) {
		ext = كلي_to_extent(كلي);
		إذا ((ext->start + ext->كدي) == start) {
			start = ext->start;
			كدي += ext->كدي;
			ext2fs_rb_erase(كلي, سيو);
			rb_free_extent(bp, ext);
		}
	}

skip_سك:
	/* See if we can merge extent to the right */
	لكل (كلي = ext2fs_rb_next(new_كلي); كلي != عدم; كلي = next) {
		next = ext2fs_rb_next(كلي);
		ext = كلي_to_extent(كلي);

		إذا ((ext->start + ext->كدي) <= start)
			استمر;

		/* No more merging */
		إذا ((start + كدي) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((start + كدي) >= (ext->start + ext->كدي)) {
			ext2fs_rb_erase(كلي, سيو);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			كدي += ((ext->start + ext->كدي) -
				  (start + كدي));
			ext2fs_rb_erase(كلي, سيو);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->كدي = كدي;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 كدي,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_سيو *سيو = &bp->سيو;
	بنية rb_كلي *parent = عدم, **n = &سيو->rb_كلي;
	بنية rb_كلي *كلي;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_كدي;
	صحيح retval = 0;

	إذا (ext2fs_rb_empty_سيو(سيو))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
			استمر;
		} وإلا إذا (start >= (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
			استمر;
		}

		إذا ((start > ext->start) &&
		    (start + كدي) < (ext->start + ext->كدي)) {
			/* We have to split extent into two */
			new_start = start + كدي;
			new_كدي = (ext->start + ext->كدي) - new_start;

			ext->كدي = start - ext->start;

			rb_سك_extent(new_start, new_كدي, bp);
			أرجع 1;
		}

		إذا ((start + كدي) >= (ext->start + ext->كدي)) {
			ext->كدي = start - ext->start;
			retval = 1;
		}

		إذا (0 == ext->كدي) {
			parent = ext2fs_rb_next(&ext->كلي);
			ext2fs_rb_erase(&ext->كلي, سيو);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (start == ext->start) {
			ext->start += كدي;
			ext->كدي -= كدي;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = كلي) {
		كلي = ext2fs_rb_next(parent);
		ext = كلي_to_extent(parent);
		إذا ((ext->start + ext->كدي) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((start + كدي) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((start + كدي) >= (ext->start + ext->كدي)) {
			ext2fs_rb_erase(parent, سيو);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->كدي -= ((start + كدي) - ext->start);
			ext->start = start + كدي;
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

	retval = rb_سك_extent(arg, 1, bp);
	check_بشيا(&bp->سيو, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_بشيا(&bp->سيو, __func__);

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
				unsigned صحيح لمب)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_سك_extent(arg, لمب, bp);
	check_بشيا(&bp->سيو, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح لمب)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, لمب, bp);
	check_بشيا(&bp->سيو, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_كلي *parent = عدم, **n;
	بنية rb_كلي *كلي, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->سيو.rb_كلي;
	start -= bitmap->start;

	إذا (len == 0 || ext2fs_rb_empty_سيو(&bp->سيو))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
		} وإلا إذا (start >= (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	كلي = parent;
	طالما (كلي) {
		next = ext2fs_rb_next(كلي);
		ext = كلي_to_extent(كلي);
		كلي = next;

		إذا ((ext->start + ext->كدي) <= start)
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
				     __u64 start, size_t لمب, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < لمب; i++) {
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

		rb_سك_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_بشيا(&bp->سيو, __func__);
		first_set = -1;
	}
	إذا (first_set != -1) {
		rb_سك_extent(start + first_set - bitmap->start,
				 لمب - first_set, bp);
		check_بشيا(&bp->سيو, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t لمب, فراغ *out)
{

	بنية rb_كلي *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 كدي, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->سيو.rb_كلي;
	start -= bitmap->start;

	إذا (ext2fs_rb_empty_سيو(&bp->سيو))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
		} وإلا إذا (start >= (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
		} وإلا
			اكسر;
	}

	memset(out, 0, (لمب + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = كلي_to_extent(parent);

		pos = ext->start;
		كدي = ext->كدي;
		إذا (pos >= start + لمب)
			اكسر;
		إذا (pos < start) {
			إذا (pos + كدي <  start)
				استمر;
			كدي -= start - pos;
			pos = start;
		}
		إذا (pos + كدي > start + لمب)
			كدي = start + لمب - pos;

		طالما (كدي > 0) {
			إذا ((كدي >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = كدي >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				كدي -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			كدي--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_بشيا(&bp->سيو);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_بشيا(&bp->سيو, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_كلي *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->سيو.rb_كلي;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_سيو(&bp->سيو))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
		} وإلا إذا (start >= (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
		} وإلا إذا (ext->start + ext->كدي <= end) {
			*out = ext->start + ext->كدي + bitmap->start;
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
	بنية rb_كلي *parent = عدم, **n;
	بنية rb_كلي *كلي;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->سيو.rb_كلي;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_سيو(&bp->سيو))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = كلي_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_هغو;
		} وإلا إذا (start >= (ext->start + ext->كدي)) {
			n = &(*n)->rb_دام;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			أرجع 0;
		}
	}

	كلي = parent;
	ext = كلي_to_extent(كلي);
	إذا (ext->start < start) {
		كلي = ext2fs_rb_next(كلي);
		إذا (كلي == عدم)
			أرجع ENOENT;
		ext = كلي_to_extent(كلي);
	}
	إذا (ext->start <= end) {
		*out = ext->start + bitmap->start;
		أرجع 0;
	}
	أرجع ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ rb_بغ_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	بنية rb_كلي *كلي = عدم;
	بنية bmap_rb_extent *ext;
	__u64 كدي = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (كلي = ext2fs_rb_first(&bp->سيو); كلي != عدم;
	     كلي = ext2fs_rb_next(كلي)) {
		ext = كلي_to_extent(كلي);
		كدي++;
		إذا (ext->كدي > max_size)
			max_size = ext->كدي;
		إذا (ext->كدي < min_size)
			min_size = ext->كدي;
		size += ext->كدي;
	}

	إذا (كدي)
		avg_size = size / كدي;
	إذا (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((كدي * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_كدي + bitmap->stats.mark_ext_كدي;
	test_all = bitmap->stats.test_كدي + bitmap->stats.test_ext_كدي;
	إذا (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	إذا (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) كدي, (unsigned long long)
		((كدي * حجم(بنية bmap_rb_extent)) +
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
ساكن فراغ rb_بغ_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
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
	.بغ_stats = rb_بغ_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};
