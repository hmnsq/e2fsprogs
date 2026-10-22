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
	بنية rb_گرہ گرہ;
	__u64 start;
	__u64 گنتی;
};

بنية ext2fs_rb_private {
	بنية rb_جڑ جڑ;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *گرہ_to_extent(بنية rb_گرہ *گرہ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) گرہ;
}

ساكن صحيح rb_داخل_کرو_extent(__u64 start, __u64 گنتی,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ print_درخت(بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *گرہ = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گرہ = ext2fs_rb_first(جڑ);
	لكل (گرہ = ext2fs_rb_first(جڑ); گرہ != عدم; 
	     گرہ = ext2fs_rb_next(گرہ)) {
		ext = گرہ_to_extent(گرہ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->گنتی);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_درخت(بنية rb_جڑ *جڑ, ثابت حرف *پیغام)
{
	بنية rb_گرہ *گرہ;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (گرہ = ext2fs_rb_first(جڑ); گرہ;
	     گرہ = ext2fs_rb_next(گرہ)) {
		ext = گرہ_to_extent(گرہ);
		إذا (ext->گنتی == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->گنتی,
				(unsigned long long) ext->گنتی);
			اذهب err_out;
		}
		إذا (ext->start + ext->گنتی < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->گنتی,
				(unsigned long long) ext->گنتی);
			اذهب err_out;
		}

		إذا (old) {
			إذا (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->گنتی,
					(unsigned long long) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->گنتی,
					(unsigned long long) ext->گنتی);
				اذهب err_out;
			}
			إذا ((old->start + old->گنتی) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->گنتی,
					(unsigned long long) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->گنتی,
					(unsigned long long) ext->گنتی);
				اذهب err_out;
			}
		}
		old = ext;
	}
	أرجع;

err_out:
	fprintf(stderr, "%s\n", پیغام);
	print_درخت(جڑ);
	exit(1);
}
#وإلا
#define check_درخت(جڑ, پیغام) افعل {} طالما (0)
#define print_درخت(جڑ) افعل {} طالما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 گنتی)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->start = start;
	new_ext->گنتی = گنتی;
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

	bp->جڑ = RB_جڑ;
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

ساكن فراغ rb_free_درخت(بنية rb_جڑ *جڑ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرہ *گرہ, *next;

	لكل (گرہ = ext2fs_rb_first(جڑ); گرہ; گرہ = next) {
		next = ext2fs_rb_next(گرہ);
		ext = گرہ_to_extent(گرہ);
		ext2fs_rb_erase(گرہ, جڑ);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_درخت(&bp->جڑ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_گرہ *dest_گرہ, *src_گرہ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_گرہ = ext2fs_rb_first(&src_bp->جڑ);
	طالما (src_گرہ) {
		src_ext = گرہ_to_extent(src_گرہ);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_گرہ = &dest_ext->گرہ;
		n = &dest_bp->جڑ.rb_گرہ;

		dest_last = عدم;
		إذا (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->جڑ);
			n = &(dest_last)->rb_دایاں;
		}

		ext2fs_rb_link_گرہ(dest_گرہ, dest_last, n);
		ext2fs_rb_داخل_کرو_color(dest_گرہ, &dest_bp->جڑ);

		src_گرہ = ext2fs_rb_next(src_گرہ);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_جڑ *جڑ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرہ *گرہ;

	گرہ = ext2fs_rb_last(جڑ);
	طالما (گرہ) {
		ext = گرہ_to_extent(گرہ);

		إذا ((ext->start + ext->گنتی - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->start > new_max) {
			ext2fs_rb_erase(گرہ, جڑ);
			ext2fs_free_mem(&ext);
			گرہ = ext2fs_rb_last(جڑ);
			استمر;
		} وإلا
			ext->گنتی = new_max - ext->start + 1;
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
		    &bp->جڑ);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	إذا (bmap->end < bmap->real_end)
		rb_داخل_کرو_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_گرہ *parent = عدم, *next;
	بنية rb_گرہ **n = &bp->جڑ.rb_گرہ;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب تلاش_درخت;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->گنتی) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		أرجع 1;
	}

	next_ext = bp->rcursor_next;
	إذا (!next_ext) {
		next = ext2fs_rb_next(&rcursor->گرہ);
		إذا (next)
			next_ext = گرہ_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	إذا (next_ext) {
		إذا ((bit >= rcursor->start + rcursor->گنتی) &&
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
		اذهب تلاش_درخت;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->گنتی)
		أرجع 1;

تلاش_درخت:

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (bit < ext->start)
			n = &(*n)->rb_بایاں;
		وإلا إذا (bit >= (ext->start + ext->گنتی))
			n = &(*n)->rb_دایاں;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_داخل_کرو_extent(__u64 start, __u64 گنتی,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_جڑ *جڑ = &bp->جڑ;
	بنية rb_گرہ *parent = عدم, **n = &جڑ->rb_گرہ;
	بنية rb_گرہ *new_گرہ, *گرہ, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (گنتی == 0)
		أرجع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (start >= ext->start &&
		    start <= (ext->start + ext->گنتی)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);

		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (start > (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
got_extent:
			إذا ((start + گنتی) <= (ext->start + ext->گنتی))
				أرجع 1;

			إذا ((ext->start + ext->گنتی) == start)
				retval = 0;
			وإلا
				retval = 1;

			گنتی += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_گرہ = &ext->گرہ;

			اذهب skip_داخل_کرو;
		}
	}

	rb_get_new_extent(&new_ext, start, گنتی);

	new_گرہ = &new_ext->گرہ;
	ext2fs_rb_link_گرہ(new_گرہ, parent, n);
	ext2fs_rb_داخل_کرو_color(new_گرہ, جڑ);
	bp->wcursor = new_ext;

	گرہ = ext2fs_rb_prev(new_گرہ);
	إذا (گرہ) {
		ext = گرہ_to_extent(گرہ);
		إذا ((ext->start + ext->گنتی) == start) {
			start = ext->start;
			گنتی += ext->گنتی;
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
		}
	}

skip_داخل_کرو:
	/* See if we can merge extent to the right */
	لكل (گرہ = ext2fs_rb_next(new_گرہ); گرہ != عدم; گرہ = next) {
		next = ext2fs_rb_next(گرہ);
		ext = گرہ_to_extent(گرہ);

		إذا ((ext->start + ext->گنتی) <= start)
			استمر;

		/* No more merging */
		إذا ((start + گنتی) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((start + گنتی) >= (ext->start + ext->گنتی)) {
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			گنتی += ((ext->start + ext->گنتی) -
				  (start + گنتی));
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->گنتی = گنتی;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 گنتی,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_جڑ *جڑ = &bp->جڑ;
	بنية rb_گرہ *parent = عدم, **n = &جڑ->rb_گرہ;
	بنية rb_گرہ *گرہ;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_گنتی;
	صحيح retval = 0;

	إذا (ext2fs_rb_empty_جڑ(جڑ))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
			استمر;
		} وإلا إذا (start >= (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
			استمر;
		}

		إذا ((start > ext->start) &&
		    (start + گنتی) < (ext->start + ext->گنتی)) {
			/* We have to split extent into two */
			new_start = start + گنتی;
			new_گنتی = (ext->start + ext->گنتی) - new_start;

			ext->گنتی = start - ext->start;

			rb_داخل_کرو_extent(new_start, new_گنتی, bp);
			أرجع 1;
		}

		إذا ((start + گنتی) >= (ext->start + ext->گنتی)) {
			ext->گنتی = start - ext->start;
			retval = 1;
		}

		إذا (0 == ext->گنتی) {
			parent = ext2fs_rb_next(&ext->گرہ);
			ext2fs_rb_erase(&ext->گرہ, جڑ);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (start == ext->start) {
			ext->start += گنتی;
			ext->گنتی -= گنتی;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = گرہ) {
		گرہ = ext2fs_rb_next(parent);
		ext = گرہ_to_extent(parent);
		إذا ((ext->start + ext->گنتی) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((start + گنتی) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((start + گنتی) >= (ext->start + ext->گنتی)) {
			ext2fs_rb_erase(parent, جڑ);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->گنتی -= ((start + گنتی) - ext->start);
			ext->start = start + گنتی;
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

	retval = rb_داخل_کرو_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);

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
				unsigned صحيح عدد)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_داخل_کرو_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح عدد)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_گرہ *parent = عدم, **n;
	بنية rb_گرہ *گرہ, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	start -= bitmap->start;

	إذا (len == 0 || ext2fs_rb_empty_جڑ(&bp->جڑ))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (start >= (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	گرہ = parent;
	طالما (گرہ) {
		next = ext2fs_rb_next(گرہ);
		ext = گرہ_to_extent(گرہ);
		گرہ = next;

		إذا ((ext->start + ext->گنتی) <= start)
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
				     __u64 start, size_t عدد, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < عدد; i++) {
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

		rb_داخل_کرو_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_درخت(&bp->جڑ, __func__);
		first_set = -1;
	}
	إذا (first_set != -1) {
		rb_داخل_کرو_extent(start + first_set - bitmap->start,
				 عدد - first_set, bp);
		check_درخت(&bp->جڑ, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t عدد, فراغ *out)
{

	بنية rb_گرہ *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 گنتی, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	start -= bitmap->start;

	إذا (ext2fs_rb_empty_جڑ(&bp->جڑ))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (start >= (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا
			اكسر;
	}

	memset(out, 0, (عدد + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = گرہ_to_extent(parent);

		pos = ext->start;
		گنتی = ext->گنتی;
		إذا (pos >= start + عدد)
			اكسر;
		إذا (pos < start) {
			إذا (pos + گنتی <  start)
				استمر;
			گنتی -= start - pos;
			pos = start;
		}
		إذا (pos + گنتی > start + عدد)
			گنتی = start + عدد - pos;

		طالما (گنتی > 0) {
			إذا ((گنتی >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = گنتی >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				گنتی -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			گنتی--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_درخت(&bp->جڑ);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_درخت(&bp->جڑ, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_گرہ *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_جڑ(&bp->جڑ))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (start >= (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا إذا (ext->start + ext->گنتی <= end) {
			*out = ext->start + ext->گنتی + bitmap->start;
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
	بنية rb_گرہ *parent = عدم, **n;
	بنية rb_گرہ *گرہ;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_جڑ(&bp->جڑ))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = گرہ_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (start >= (ext->start + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			أرجع 0;
		}
	}

	گرہ = parent;
	ext = گرہ_to_extent(گرہ);
	إذا (ext->start < start) {
		گرہ = ext2fs_rb_next(گرہ);
		إذا (گرہ == عدم)
			أرجع ENOENT;
		ext = گرہ_to_extent(گرہ);
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
	بنية rb_گرہ *گرہ = عدم;
	بنية bmap_rb_extent *ext;
	__u64 گنتی = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (گرہ = ext2fs_rb_first(&bp->جڑ); گرہ != عدم;
	     گرہ = ext2fs_rb_next(گرہ)) {
		ext = گرہ_to_extent(گرہ);
		گنتی++;
		إذا (ext->گنتی > max_size)
			max_size = ext->گنتی;
		إذا (ext->گنتی < min_size)
			min_size = ext->گنتی;
		size += ext->گنتی;
	}

	إذا (گنتی)
		avg_size = size / گنتی;
	إذا (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((گنتی * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_گنتی + bitmap->stats.mark_ext_گنتی;
	test_all = bitmap->stats.test_گنتی + bitmap->stats.test_ext_گنتی;
	إذا (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	إذا (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) گنتی, (unsigned long long)
		((گنتی * حجم(بنية bmap_rb_extent)) +
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
