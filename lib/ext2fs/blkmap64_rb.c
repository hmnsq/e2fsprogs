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
	بنية rb_تۈگۈن تۈگۈن;
	__u64 start;
	__u64 سانى;
};

بنية ext2fs_rb_private {
	بنية rb_يىلتىز يىلتىز;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *تۈگۈن_to_extent(بنية rb_تۈگۈن *تۈگۈن)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) تۈگۈن;
}

ساكن صحيح rb_قىستۇرۇش_extent(__u64 start, __u64 سانى,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ print_دەرەخ(بنية rb_يىلتىز *يىلتىز)
{
	بنية rb_تۈگۈن *تۈگۈن = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	تۈگۈن = ext2fs_rb_first(يىلتىز);
	لكل (تۈگۈن = ext2fs_rb_first(يىلتىز); تۈگۈن != عدم; 
	     تۈگۈن = ext2fs_rb_next(تۈگۈن)) {
		ext = تۈگۈن_to_extent(تۈگۈن);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->سانى);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_دەرەخ(بنية rb_يىلتىز *يىلتىز, ثابت حرف *ئۇچۇر)
{
	بنية rb_تۈگۈن *تۈگۈن;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (تۈگۈن = ext2fs_rb_first(يىلتىز); تۈگۈن;
	     تۈگۈن = ext2fs_rb_next(تۈگۈن)) {
		ext = تۈگۈن_to_extent(تۈگۈن);
		إذا (ext->سانى == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->سانى,
				(unsigned long long) ext->سانى);
			اذهب err_out;
		}
		إذا (ext->start + ext->سانى < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->سانى,
				(unsigned long long) ext->سانى);
			اذهب err_out;
		}

		إذا (old) {
			إذا (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->سانى,
					(unsigned long long) old->سانى);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->سانى,
					(unsigned long long) ext->سانى);
				اذهب err_out;
			}
			إذا ((old->start + old->سانى) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->سانى,
					(unsigned long long) old->سانى);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->سانى,
					(unsigned long long) ext->سانى);
				اذهب err_out;
			}
		}
		old = ext;
	}
	أرجع;

err_out:
	fprintf(stderr, "%s\n", ئۇچۇر);
	print_دەرەخ(يىلتىز);
	exit(1);
}
#وإلا
#define check_دەرەخ(يىلتىز, ئۇچۇر) افعل {} طالما (0)
#define print_دەرەخ(يىلتىز) افعل {} طالما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 سانى)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->start = start;
	new_ext->سانى = سانى;
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

	bp->يىلتىز = RB_يىلتىز;
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

ساكن فراغ rb_free_دەرەخ(بنية rb_يىلتىز *يىلتىز)
{
	بنية bmap_rb_extent *ext;
	بنية rb_تۈگۈن *تۈگۈن, *next;

	لكل (تۈگۈن = ext2fs_rb_first(يىلتىز); تۈگۈن; تۈگۈن = next) {
		next = ext2fs_rb_next(تۈگۈن);
		ext = تۈگۈن_to_extent(تۈگۈن);
		ext2fs_rb_erase(تۈگۈن, يىلتىز);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_دەرەخ(&bp->يىلتىز);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_تۈگۈن *dest_تۈگۈن, *src_تۈگۈن, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_تۈگۈن = ext2fs_rb_first(&src_bp->يىلتىز);
	طالما (src_تۈگۈن) {
		src_ext = تۈگۈن_to_extent(src_تۈگۈن);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_تۈگۈن = &dest_ext->تۈگۈن;
		n = &dest_bp->يىلتىز.rb_تۈگۈن;

		dest_last = عدم;
		إذا (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->يىلتىز);
			n = &(dest_last)->rb_ئوڭ;
		}

		ext2fs_rb_link_تۈگۈن(dest_تۈگۈن, dest_last, n);
		ext2fs_rb_قىستۇرۇش_color(dest_تۈگۈن, &dest_bp->يىلتىز);

		src_تۈگۈن = ext2fs_rb_next(src_تۈگۈن);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_يىلتىز *يىلتىز)
{
	بنية bmap_rb_extent *ext;
	بنية rb_تۈگۈن *تۈگۈن;

	تۈگۈن = ext2fs_rb_last(يىلتىز);
	طالما (تۈگۈن) {
		ext = تۈگۈن_to_extent(تۈگۈن);

		إذا ((ext->start + ext->سانى - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->start > new_max) {
			ext2fs_rb_erase(تۈگۈن, يىلتىز);
			ext2fs_free_mem(&ext);
			تۈگۈن = ext2fs_rb_last(يىلتىز);
			استمر;
		} وإلا
			ext->سانى = new_max - ext->start + 1;
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
		    &bp->يىلتىز);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	إذا (bmap->end < bmap->real_end)
		rb_قىستۇرۇش_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_تۈگۈن *parent = عدم, *next;
	بنية rb_تۈگۈن **n = &bp->يىلتىز.rb_تۈگۈن;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب ئىزدەش_دەرىخى;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->سانى) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		أرجع 1;
	}

	next_ext = bp->rcursor_next;
	إذا (!next_ext) {
		next = ext2fs_rb_next(&rcursor->تۈگۈن);
		إذا (next)
			next_ext = تۈگۈن_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	إذا (next_ext) {
		إذا ((bit >= rcursor->start + rcursor->سانى) &&
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
		اذهب ئىزدەش_دەرىخى;

	إذا (bit >= rcursor->start && bit < rcursor->start + rcursor->سانى)
		أرجع 1;

ئىزدەش_دەرىخى:

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (bit < ext->start)
			n = &(*n)->rb_سول;
		وإلا إذا (bit >= (ext->start + ext->سانى))
			n = &(*n)->rb_ئوڭ;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_قىستۇرۇش_extent(__u64 start, __u64 سانى,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_يىلتىز *يىلتىز = &bp->يىلتىز;
	بنية rb_تۈگۈن *parent = عدم, **n = &يىلتىز->rb_تۈگۈن;
	بنية rb_تۈگۈن *new_تۈگۈن, *تۈگۈن, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (سانى == 0)
		أرجع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (start >= ext->start &&
		    start <= (ext->start + ext->سانى)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);

		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
		} وإلا إذا (start > (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
		} وإلا {
got_extent:
			إذا ((start + سانى) <= (ext->start + ext->سانى))
				أرجع 1;

			إذا ((ext->start + ext->سانى) == start)
				retval = 0;
			وإلا
				retval = 1;

			سانى += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_تۈگۈن = &ext->تۈگۈن;

			اذهب skip_قىستۇرۇش;
		}
	}

	rb_get_new_extent(&new_ext, start, سانى);

	new_تۈگۈن = &new_ext->تۈگۈن;
	ext2fs_rb_link_تۈگۈن(new_تۈگۈن, parent, n);
	ext2fs_rb_قىستۇرۇش_color(new_تۈگۈن, يىلتىز);
	bp->wcursor = new_ext;

	تۈگۈن = ext2fs_rb_prev(new_تۈگۈن);
	إذا (تۈگۈن) {
		ext = تۈگۈن_to_extent(تۈگۈن);
		إذا ((ext->start + ext->سانى) == start) {
			start = ext->start;
			سانى += ext->سانى;
			ext2fs_rb_erase(تۈگۈن, يىلتىز);
			rb_free_extent(bp, ext);
		}
	}

skip_قىستۇرۇش:
	/* See if we can merge extent to the right */
	لكل (تۈگۈن = ext2fs_rb_next(new_تۈگۈن); تۈگۈن != عدم; تۈگۈن = next) {
		next = ext2fs_rb_next(تۈگۈن);
		ext = تۈگۈن_to_extent(تۈگۈن);

		إذا ((ext->start + ext->سانى) <= start)
			استمر;

		/* No more merging */
		إذا ((start + سانى) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((start + سانى) >= (ext->start + ext->سانى)) {
			ext2fs_rb_erase(تۈگۈن, يىلتىز);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			سانى += ((ext->start + ext->سانى) -
				  (start + سانى));
			ext2fs_rb_erase(تۈگۈن, يىلتىز);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->سانى = سانى;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 سانى,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_يىلتىز *يىلتىز = &bp->يىلتىز;
	بنية rb_تۈگۈن *parent = عدم, **n = &يىلتىز->rb_تۈگۈن;
	بنية rb_تۈگۈن *تۈگۈن;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_سانى;
	صحيح retval = 0;

	إذا (ext2fs_rb_empty_يىلتىز(يىلتىز))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
			استمر;
		} وإلا إذا (start >= (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
			استمر;
		}

		إذا ((start > ext->start) &&
		    (start + سانى) < (ext->start + ext->سانى)) {
			/* We have to split extent into two */
			new_start = start + سانى;
			new_سانى = (ext->start + ext->سانى) - new_start;

			ext->سانى = start - ext->start;

			rb_قىستۇرۇش_extent(new_start, new_سانى, bp);
			أرجع 1;
		}

		إذا ((start + سانى) >= (ext->start + ext->سانى)) {
			ext->سانى = start - ext->start;
			retval = 1;
		}

		إذا (0 == ext->سانى) {
			parent = ext2fs_rb_next(&ext->تۈگۈن);
			ext2fs_rb_erase(&ext->تۈگۈن, يىلتىز);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (start == ext->start) {
			ext->start += سانى;
			ext->سانى -= سانى;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = تۈگۈن) {
		تۈگۈن = ext2fs_rb_next(parent);
		ext = تۈگۈن_to_extent(parent);
		إذا ((ext->start + ext->سانى) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((start + سانى) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((start + سانى) >= (ext->start + ext->سانى)) {
			ext2fs_rb_erase(parent, يىلتىز);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->سانى -= ((start + سانى) - ext->start);
			ext->start = start + سانى;
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

	retval = rb_قىستۇرۇش_extent(arg, 1, bp);
	check_دەرەخ(&bp->يىلتىز, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_دەرەخ(&bp->يىلتىز, __func__);

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
				unsigned صحيح رەقەم)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_قىستۇرۇش_extent(arg, رەقەم, bp);
	check_دەرەخ(&bp->يىلتىز, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح رەقەم)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, رەقەم, bp);
	check_دەرەخ(&bp->يىلتىز, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_تۈگۈن *parent = عدم, **n;
	بنية rb_تۈگۈن *تۈگۈن, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->يىلتىز.rb_تۈگۈن;
	start -= bitmap->start;

	إذا (len == 0 || ext2fs_rb_empty_يىلتىز(&bp->يىلتىز))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
		} وإلا إذا (start >= (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	تۈگۈن = parent;
	طالما (تۈگۈن) {
		next = ext2fs_rb_next(تۈگۈن);
		ext = تۈگۈن_to_extent(تۈگۈن);
		تۈگۈن = next;

		إذا ((ext->start + ext->سانى) <= start)
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
				     __u64 start, size_t رەقەم, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < رەقەم; i++) {
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

		rb_قىستۇرۇش_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_دەرەخ(&bp->يىلتىز, __func__);
		first_set = -1;
	}
	إذا (first_set != -1) {
		rb_قىستۇرۇش_extent(start + first_set - bitmap->start,
				 رەقەم - first_set, bp);
		check_دەرەخ(&bp->يىلتىز, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رەقەم, فراغ *out)
{

	بنية rb_تۈگۈن *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 سانى, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->يىلتىز.rb_تۈگۈن;
	start -= bitmap->start;

	إذا (ext2fs_rb_empty_يىلتىز(&bp->يىلتىز))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
		} وإلا إذا (start >= (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
		} وإلا
			اكسر;
	}

	memset(out, 0, (رەقەم + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = تۈگۈن_to_extent(parent);

		pos = ext->start;
		سانى = ext->سانى;
		إذا (pos >= start + رەقەم)
			اكسر;
		إذا (pos < start) {
			إذا (pos + سانى <  start)
				استمر;
			سانى -= start - pos;
			pos = start;
		}
		إذا (pos + سانى > start + رەقەم)
			سانى = start + رەقەم - pos;

		طالما (سانى > 0) {
			إذا ((سانى >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = سانى >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				سانى -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			سانى--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_دەرەخ(&bp->يىلتىز);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_دەرەخ(&bp->يىلتىز, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_تۈگۈن *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->يىلتىز.rb_تۈگۈن;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_يىلتىز(&bp->يىلتىز))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
		} وإلا إذا (start >= (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
		} وإلا إذا (ext->start + ext->سانى <= end) {
			*out = ext->start + ext->سانى + bitmap->start;
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
	بنية rb_تۈگۈن *parent = عدم, **n;
	بنية rb_تۈگۈن *تۈگۈن;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->يىلتىز.rb_تۈگۈن;
	start -= bitmap->start;
	end -= bitmap->start;

	إذا (start > end)
		أرجع EINVAL;

	إذا (ext2fs_rb_empty_يىلتىز(&bp->يىلتىز))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = تۈگۈن_to_extent(parent);
		إذا (start < ext->start) {
			n = &(*n)->rb_سول;
		} وإلا إذا (start >= (ext->start + ext->سانى)) {
			n = &(*n)->rb_ئوڭ;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			أرجع 0;
		}
	}

	تۈگۈن = parent;
	ext = تۈگۈن_to_extent(تۈگۈن);
	إذا (ext->start < start) {
		تۈگۈن = ext2fs_rb_next(تۈگۈن);
		إذا (تۈگۈن == عدم)
			أرجع ENOENT;
		ext = تۈگۈن_to_extent(تۈگۈن);
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
	بنية rb_تۈگۈن *تۈگۈن = عدم;
	بنية bmap_rb_extent *ext;
	__u64 سانى = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (تۈگۈن = ext2fs_rb_first(&bp->يىلتىز); تۈگۈن != عدم;
	     تۈگۈن = ext2fs_rb_next(تۈگۈن)) {
		ext = تۈگۈن_to_extent(تۈگۈن);
		سانى++;
		إذا (ext->سانى > max_size)
			max_size = ext->سانى;
		إذا (ext->سانى < min_size)
			min_size = ext->سانى;
		size += ext->سانى;
	}

	إذا (سانى)
		avg_size = size / سانى;
	إذا (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((سانى * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_سانى + bitmap->stats.mark_ext_سانى;
	test_all = bitmap->stats.test_سانى + bitmap->stats.test_ext_سانى;
	إذا (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	إذا (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) سانى, (unsigned long long)
		((سانى * حجم(بنية bmap_rb_extent)) +
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
