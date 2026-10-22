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
	بنية rb_عقدة عقدة;
	__u64 start;
	__u64 تعداد_2;
};

بنية ext2fs_rb_private {
	بنية rb_جذر جذر;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *عقدة_to_extent(بنية rb_عقدة *عقدة)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	إرجاع (بنية bmap_rb_extent *) عقدة;
}

ساكن صحيح rb_أدرج_extent(__u64 start, __u64 تعداد_2,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ print_شجرة(بنية rb_جذر *جذر)
{
	بنية rb_عقدة *عقدة = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	عقدة = ext2fs_rb_first(جذر);
	لكل (عقدة = ext2fs_rb_first(جذر); عقدة != عدم; 
	     عقدة = ext2fs_rb_next(عقدة)) {
		ext = عقدة_to_extent(عقدة);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->تعداد_2);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_شجرة(بنية rb_جذر *جذر, ثابت حرف *رسالة)
{
	بنية rb_عقدة *عقدة;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (عقدة = ext2fs_rb_first(جذر); عقدة;
	     عقدة = ext2fs_rb_next(عقدة)) {
		ext = عقدة_to_extent(عقدة);
		إذا (ext->تعداد_2 == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->تعداد_2,
				(unsigned long long) ext->تعداد_2);
			اذهب err_out;
		}
		إذا (ext->start + ext->تعداد_2 < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->تعداد_2,
				(unsigned long long) ext->تعداد_2);
			اذهب err_out;
		}

		إذا (old) {
			إذا (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->تعداد_2,
					(unsigned long long) old->تعداد_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->تعداد_2,
					(unsigned long long) ext->تعداد_2);
				اذهب err_out;
			}
			إذا ((old->start + old->تعداد_2) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->تعداد_2,
					(unsigned long long) old->تعداد_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->تعداد_2,
					(unsigned long long) ext->تعداد_2);
				اذهب err_out;
			}
		}
		old = ext;
	}
	إرجاع;

err_out:
	fprintf(stderr, "%s\n", رسالة);
	print_شجرة(جذر);
	exit(1);
}
#وإلا
#define check_شجرة(جذر, رسالة) افعل {} بينما (0)
#define print_شجرة(جذر) افعل {} بينما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 تعداد_2)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->start = start;
	new_ext->تعداد_2 = تعداد_2;
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
		إرجاع retval;

	bp->جذر = RB_جذر;
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->خاص = (فراغ *) bp;
	إرجاع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	إذا (retval)
		إرجاع retval;

	إرجاع 0;
}

ساكن فراغ rb_free_شجرة(بنية rb_جذر *جذر)
{
	بنية bmap_rb_extent *ext;
	بنية rb_عقدة *عقدة, *next;

	لكل (عقدة = ext2fs_rb_first(جذر); عقدة; عقدة = next) {
		next = ext2fs_rb_next(عقدة);
		ext = عقدة_to_extent(عقدة);
		ext2fs_rb_erase(عقدة, جذر);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_شجرة(&bp->جذر);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_عقدة *dest_عقدة, *src_عقدة, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	إذا (retval)
		إرجاع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_عقدة = ext2fs_rb_first(&src_bp->جذر);
	بينما (src_عقدة) {
		src_ext = عقدة_to_extent(src_عقدة);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_عقدة = &dest_ext->عقدة;
		n = &dest_bp->جذر.rb_عقدة;

		dest_last = عدم;
		إذا (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->جذر);
			n = &(dest_last)->rb_يمين;
		}

		ext2fs_rb_link_عقدة(dest_عقدة, dest_last, n);
		ext2fs_rb_أدرج_color(dest_عقدة, &dest_bp->جذر);

		src_عقدة = ext2fs_rb_next(src_عقدة);
	}

	إرجاع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_جذر *جذر)
{
	بنية bmap_rb_extent *ext;
	بنية rb_عقدة *عقدة;

	عقدة = ext2fs_rb_last(جذر);
	بينما (عقدة) {
		ext = عقدة_to_extent(عقدة);

		إذا ((ext->start + ext->تعداد_2 - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->start > new_max) {
			ext2fs_rb_erase(عقدة, جذر);
			ext2fs_free_mem(&ext);
			عقدة = ext2fs_rb_last(جذر);
			استمر;
		} وإلا
			ext->تعداد_2 = new_max - ext->start + 1;
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
		    &bp->جذر);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	إذا (bmap->end < bmap->real_end)
		rb_أدرج_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	إرجاع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_عقدة *parent = عدم, *next;
	بنية rb_عقدة **n = &bp->جذر.rb_عقدة;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب شجرة_البحث;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->تعداد_2) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		إرجاع 1;
	}

	next_ext = bp->rcursor_next;
	إذا (!next_ext) {
		next = ext2fs_rb_next(&rcursor->عقدة);
		إذا (next)
			next_ext = عقدة_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	إذا (next_ext) {
		إذا ((bit >= rcursor->start + rcursor->تعداد_2) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			إرجاع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب شجرة_البحث;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->تعداد_2)
		إرجاع 1;

شجرة_البحث:

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (bit < ext->start)
			n = &(*n)->rb_يسار;
		وإلا إذا (bit >= (ext->start + ext->تعداد_2))
			n = &(*n)->rb_يمين;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			إرجاع 1;
		}
	}
	إرجاع 0;
}

ساكن صحيح rb_أدرج_extent(__u64 start, __u64 تعداد_2,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_جذر *جذر = &bp->جذر;
	بنية rb_عقدة *parent = عدم, **n = &جذر->rb_عقدة;
	بنية rb_عقدة *new_عقدة, *عقدة, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (تعداد_2 == 0)
		إرجاع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (start >= ext->start &&
		    start <= (ext->start + ext->تعداد_2)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);

		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
		} وإلا إذا (start > (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
		} وإلا {
got_extent:
			إذا ((start + تعداد_2) <= (ext->start + ext->تعداد_2))
				إرجاع 1;

			إذا ((ext->start + ext->تعداد_2) == start)
				retval = 0;
			وإلا
				retval = 1;

			تعداد_2 += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_عقدة = &ext->عقدة;

			اذهب skip_أدرج;
		}
	}

	rb_get_new_extent(&new_ext, start, تعداد_2);

	new_عقدة = &new_ext->عقدة;
	ext2fs_rb_link_عقدة(new_عقدة, parent, n);
	ext2fs_rb_أدرج_color(new_عقدة, جذر);
	bp->wcursor = new_ext;

	عقدة = ext2fs_rb_prev(new_عقدة);
	إذا (عقدة) {
		ext = عقدة_to_extent(عقدة);
		إذا ((ext->start + ext->تعداد_2) == start) {
			start = ext->start;
			تعداد_2 += ext->تعداد_2;
			ext2fs_rb_erase(عقدة, جذر);
			rb_free_extent(bp, ext);
		}
	}

skip_أدرج:
	/* See if we can merge extent to the right */
	لكل (عقدة = ext2fs_rb_next(new_عقدة); عقدة != عدم; عقدة = next) {
		next = ext2fs_rb_next(عقدة);
		ext = عقدة_to_extent(عقدة);

		إذا ((ext->start + ext->تعداد_2) <= start)
			استمر;

		/* No more merging */
		إذا ((start + تعداد_2) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((start + تعداد_2) >= (ext->start + ext->تعداد_2)) {
			ext2fs_rb_erase(عقدة, جذر);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			تعداد_2 += ((ext->start + ext->تعداد_2) -
				  (start + تعداد_2));
			ext2fs_rb_erase(عقدة, جذر);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->تعداد_2 = تعداد_2;

	إرجاع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 تعداد_2,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_جذر *جذر = &bp->جذر;
	بنية rb_عقدة *parent = عدم, **n = &جذر->rb_عقدة;
	بنية rb_عقدة *عقدة;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_تعداد;
	صحيح retval = 0;

	إذا (ext2fs_rb_empty_جذر(جذر))
		إرجاع 0;

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
			استمر;
		} وإلا إذا (start >= (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
			استمر;
		}

		إذا ((start > ext->start) &&
		    (start + تعداد_2) < (ext->start + ext->تعداد_2)) {
			/* We have to split extent into two */
			new_start = start + تعداد_2;
			new_تعداد = (ext->start + ext->تعداد_2) - new_start;

			ext->تعداد_2 = start - ext->start;

			rb_أدرج_extent(new_start, new_تعداد, bp);
			إرجاع 1;
		}

		إذا ((start + تعداد_2) >= (ext->start + ext->تعداد_2)) {
			ext->تعداد_2 = start - ext->start;
			retval = 1;
		}

		إذا (0 == ext->تعداد_2) {
			parent = ext2fs_rb_next(&ext->عقدة);
			ext2fs_rb_erase(&ext->عقدة, جذر);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (start == ext->start) {
			ext->start += تعداد_2;
			ext->تعداد_2 -= تعداد_2;
			إرجاع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = عقدة) {
		عقدة = ext2fs_rb_next(parent);
		ext = عقدة_to_extent(parent);
		إذا ((ext->start + ext->تعداد_2) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((start + تعداد_2) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((start + تعداد_2) >= (ext->start + ext->تعداد_2)) {
			ext2fs_rb_erase(parent, جذر);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->تعداد_2 -= ((start + تعداد_2) - ext->start);
			ext->start = start + تعداد_2;
			retval = 1;
			اكسر;
		}
	}

	إرجاع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_أدرج_extent(arg, 1, bp);
	check_شجرة(&bp->جذر, __func__);
	إرجاع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_شجرة(&bp->جذر, __func__);

	إرجاع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	إرجاع rb_test_bit(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned صحيح رقم)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_أدرج_extent(arg, رقم, bp);
	check_شجرة(&bp->جذر, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح رقم)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, رقم, bp);
	check_شجرة(&bp->جذر, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_عقدة *parent = عدم, **n;
	بنية rb_عقدة *عقدة, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جذر.rb_عقدة;
	start -= bitmap->start;

	إذا (len == 0 || ext2fs_rb_empty_جذر(&bp->جذر))
		إرجاع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
		} وإلا إذا (start >= (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			إرجاع 0;
		}
	}

	عقدة = parent;
	بينما (عقدة) {
		next = ext2fs_rb_next(عقدة);
		ext = عقدة_to_extent(عقدة);
		عقدة = next;

		إذا ((ext->start + ext->تعداد_2) <= start)
			استمر;

		/* No more merging */
		إذا ((start + len) <= ext->start)
			اكسر;

		retval = 0;
		اكسر;
	}
	إرجاع retval;
}

ساكن errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < رقم; i++) {
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

		rb_أدرج_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_شجرة(&bp->جذر, __func__);
		first_set = -1;
	}
	إذا (first_set != -1) {
		rb_أدرج_extent(start + first_set - bitmap->start,
				 رقم - first_set, bp);
		check_شجرة(&bp->جذر, __func__);
	}

	إرجاع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, فراغ *out)
{

	بنية rb_عقدة *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 تعداد_2, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جذر.rb_عقدة;
	start -= bitmap->start;

	إذا (ext2fs_rb_empty_جذر(&bp->جذر))
		إرجاع 0;

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
		} وإلا إذا (start >= (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
		} وإلا
			اكسر;
	}

	memset(out, 0, (رقم + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = عقدة_to_extent(parent);

		pos = ext->start;
		تعداد_2 = ext->تعداد_2;
		إذا (pos >= start + رقم)
			اكسر;
		إذا (pos < start) {
			إذا (pos + تعداد_2 <  start)
				استمر;
			تعداد_2 -= start - pos;
			pos = start;
		}
		إذا (pos + تعداد_2 > start + رقم)
			تعداد_2 = start + رقم - pos;

		بينما (تعداد_2 > 0) {
			إذا ((تعداد_2 >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = تعداد_2 >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				تعداد_2 -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			تعداد_2--;
		}
	}
	إرجاع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_شجرة(&bp->جذر);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_شجرة(&bp->جذر, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_عقدة *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جذر.rb_عقدة;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		إرجاع EINVAL;

	إذا (ext2fs_rb_empty_جذر(&bp->جذر))
		إرجاع ENOENT;

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
		} وإلا إذا (start >= (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
		} وإلا إذا (ext->start + ext->تعداد_2 <= end) {
			*out = ext->start + ext->تعداد_2 + bitmap->start;
			إرجاع 0;
		} وإلا
			إرجاع ENOENT;
	}

	*out = start + bitmap->start;
	إرجاع 0;
}

ساكن errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_عقدة *parent = عدم, **n;
	بنية rb_عقدة *عقدة;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->جذر.rb_عقدة;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		إرجاع EINVAL;

	إذا (ext2fs_rb_empty_جذر(&bp->جذر))
		إرجاع ENOENT;

	بينما (*n) {
		parent = *n;
		ext = عقدة_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_يسار;
		} وإلا إذا (start >= (ext->start + ext->تعداد_2)) {
			n = &(*n)->rb_يمين;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			إرجاع 0;
		}
	}

	عقدة = parent;
	ext = عقدة_to_extent(عقدة);
	إذا (ext->start < start) {
		عقدة = ext2fs_rb_next(عقدة);
		إذا (عقدة == عدم)
			إرجاع ENOENT;
		ext = عقدة_to_extent(عقدة);
	}
	إذا (ext->start <= end) {
		*out = ext->start + bitmap->start;
		إرجاع 0;
	}
	إرجاع ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	بنية rb_عقدة *عقدة = عدم;
	بنية bmap_rb_extent *ext;
	__u64 تعداد_2 = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (عقدة = ext2fs_rb_first(&bp->جذر); عقدة != عدم;
	     عقدة = ext2fs_rb_next(عقدة)) {
		ext = عقدة_to_extent(عقدة);
		تعداد_2++;
		إذا (ext->تعداد_2 > max_size)
			max_size = ext->تعداد_2;
		إذا (ext->تعداد_2 < min_size)
			min_size = ext->تعداد_2;
		size += ext->تعداد_2;
	}

	إذا (تعداد_2)
		avg_size = size / تعداد_2;
	إذا (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((تعداد_2 * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_تعداد + bitmap->stats.mark_ext_تعداد;
	test_all = bitmap->stats.test_تعداد + bitmap->stats.test_ext_تعداد;
	إذا (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	إذا (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) تعداد_2, (unsigned long long)
		((تعداد_2 * حجم(بنية bmap_rb_extent)) +
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
