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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_LINUX_TYPES_H
#અન્તર્ભાવ <linux/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "bmap64.h"
#અન્તર્ભાવ "rbtree.h"

#અન્તર્ભાવ <limits.h>

સંરચના bmap_rb_extent {
	સંરચના rb_ગાંઠ ગાંઠ;
	__u64 start;
	__u64 ગણતરી;
};

સંરચના ext2fs_rb_private {
	સંરચના rb_મૂળ મૂળ;
	સંરચના bmap_rb_extent *wcursor;
	સંરચના bmap_rb_extent *rcursor;
	સંરચના bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

અંતઃરેખ સ્થિર સંરચના bmap_rb_extent *ગાંઠ_to_extent(સંરચના rb_ગાંઠ *ગાંઠ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	પાછા (સંરચના bmap_rb_extent *) ગાંઠ;
}

સ્થિર પૂર્ણાંક rb_ઉમેરો_extent(__u64 start, __u64 ગણતરી,
			    સંરચના ext2fs_rb_private *);
સ્થિર શૂન્ય rb_get_new_extent(સંરચના bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
સ્થિર શૂન્ય print_વૃક્ષ(સંરચના rb_મૂળ *મૂળ)
{
	સંરચના rb_ગાંઠ *ગાંઠ = ખાલી;
	સંરચના bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ગાંઠ = ext2fs_rb_first(મૂળ);
	માટે (ગાંઠ = ext2fs_rb_first(મૂળ); ગાંઠ != ખાલી; 
	     ગાંઠ = ext2fs_rb_next(ગાંઠ)) {
		ext = ગાંઠ_to_extent(ગાંઠ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->ગણતરી);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

સ્થિર શૂન્ય check_વૃક્ષ(સંરચના rb_મૂળ *મૂળ, અચલ અક્ષર *સંદેશ)
{
	સંરચના rb_ગાંઠ *ગાંઠ;
	સંરચના bmap_rb_extent *ext, *old = ખાલી;

	માટે (ગાંઠ = ext2fs_rb_first(મૂળ); ગાંઠ;
	     ગાંઠ = ext2fs_rb_next(ગાંઠ)) {
		ext = ગાંઠ_to_extent(ગાંઠ);
		જો (ext->ગણતરી == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ગણતરી,
				(unsigned long long) ext->ગણતરી);
			જાઓ err_out;
		}
		જો (ext->start + ext->ગણતરી < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ગણતરી,
				(unsigned long long) ext->ગણતરી);
			જાઓ err_out;
		}

		જો (old) {
			જો (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ગણતરી,
					(unsigned long long) old->ગણતરી);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ગણતરી,
					(unsigned long long) ext->ગણતરી);
				જાઓ err_out;
			}
			જો ((old->start + old->ગણતરી) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ગણતરી,
					(unsigned long long) old->ગણતરી);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ગણતરી,
					(unsigned long long) ext->ગણતરી);
				જાઓ err_out;
			}
		}
		old = ext;
	}
	પાછા;

err_out:
	fprintf(stderr, "%s\n", સંદેશ);
	print_વૃક્ષ(મૂળ);
	exit(1);
}
#નહીંતર
#define check_વૃક્ષ(મૂળ, સંદેશ) કરો {} જ્યારે (0)
#define print_વૃક્ષ(મૂળ) કરો {} જ્યારે (0)
#endif

સ્થિર શૂન્ય rb_get_new_extent(સંરચના bmap_rb_extent **ext, __u64 start,
			     __u64 ગણતરી)
{
	સંરચના bmap_rb_extent *new_ext;
	પૂર્ણાંક retval;

	retval = ext2fs_get_mem(કદ (સંરચના bmap_rb_extent),
				&new_ext);
	જો (retval)
		abort();

	new_ext->start = start;
	new_ext->ગણતરી = ગણતરી;
	*ext = new_ext;
}

અંતઃરેખ
સ્થિર શૂન્ય rb_free_extent(સંરચના ext2fs_rb_private *bp,
			   સંરચના bmap_rb_extent *ext)
{
	જો (bp->wcursor == ext)
		bp->wcursor = ખાલી;
	જો (bp->rcursor == ext)
		bp->rcursor = ખાલી;
	જો (bp->rcursor_next == ext)
		bp->rcursor_next = ખાલી;
	ext2fs_free_mem(&ext);
}

સ્થિર errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	સંરચના ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(કદ (સંરચના ext2fs_rb_private), &bp);
	જો (retval)
		પાછા retval;

	bp->મૂળ = RB_મૂળ;
	bp->rcursor = ખાલી;
	bp->rcursor_next = ખાલી;
	bp->wcursor = ખાલી;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->ખાનગી = (શૂન્ય *) bp;
	પાછા 0;
}

સ્થિર errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	જો (retval)
		પાછા retval;

	પાછા 0;
}

સ્થિર શૂન્ય rb_free_વૃક્ષ(સંરચના rb_મૂળ *મૂળ)
{
	સંરચના bmap_rb_extent *ext;
	સંરચના rb_ગાંઠ *ગાંઠ, *next;

	માટે (ગાંઠ = ext2fs_rb_first(મૂળ); ગાંઠ; ગાંઠ = next) {
		next = ext2fs_rb_next(ગાંઠ);
		ext = ગાંઠ_to_extent(ગાંઠ);
		ext2fs_rb_erase(ગાંઠ, મૂળ);
		ext2fs_free_mem(&ext);
	}
}

સ્થિર શૂન્ય rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;

	rb_free_વૃક્ષ(&bp->મૂળ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

સ્થિર errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	સંરચના ext2fs_rb_private *src_bp, *dest_bp;
	સંરચના bmap_rb_extent *src_ext, *dest_ext;
	સંરચના rb_ગાંઠ *dest_ગાંઠ, *src_ગાંઠ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	જો (retval)
		પાછા retval;

	src_bp = (સંરચના ext2fs_rb_private *) src->ખાનગી;
	dest_bp = (સંરચના ext2fs_rb_private *) dest->ખાનગી;
	src_bp->rcursor = ખાલી;
	dest_bp->rcursor = ખાલી;

	src_ગાંઠ = ext2fs_rb_first(&src_bp->મૂળ);
	જ્યારે (src_ગાંઠ) {
		src_ext = ગાંઠ_to_extent(src_ગાંઠ);
		retval = ext2fs_get_mem(કદ (સંરચના bmap_rb_extent),
					&dest_ext);
		જો (retval)
			તોડો;

		memcpy(dest_ext, src_ext, કદ(સંરચના bmap_rb_extent));

		dest_ગાંઠ = &dest_ext->ગાંઠ;
		n = &dest_bp->મૂળ.rb_ગાંઠ;

		dest_last = ખાલી;
		જો (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->મૂળ);
			n = &(dest_last)->rb_જમણું;
		}

		ext2fs_rb_link_ગાંઠ(dest_ગાંઠ, dest_last, n);
		ext2fs_rb_ઉમેરો_color(dest_ગાંઠ, &dest_bp->મૂળ);

		src_ગાંઠ = ext2fs_rb_next(src_ગાંઠ);
	}

	પાછા retval;
}

સ્થિર શૂન્ય rb_truncate(__u64 new_max, સંરચના rb_મૂળ *મૂળ)
{
	સંરચના bmap_rb_extent *ext;
	સંરચના rb_ગાંઠ *ગાંઠ;

	ગાંઠ = ext2fs_rb_last(મૂળ);
	જ્યારે (ગાંઠ) {
		ext = ગાંઠ_to_extent(ગાંઠ);

		જો ((ext->start + ext->ગણતરી - 1) <= new_max)
			તોડો;
		નહીંતર જો (ext->start > new_max) {
			ext2fs_rb_erase(ગાંઠ, મૂળ);
			ext2fs_free_mem(&ext);
			ગાંઠ = ext2fs_rb_last(મૂળ);
			ચાલુ;
		} નહીંતર
			ext->ગણતરી = new_max - ext->start + 1;
	}
}

સ્થિર errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bmap->ખાનગી;
	bp->rcursor = ખાલી;
	bp->wcursor = ખાલી;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->મૂળ);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	જો (bmap->end < bmap->real_end)
		rb_ઉમેરો_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	પાછા 0;

}

અંતઃરેખ સ્થિર પૂર્ણાંક
rb_test_bit(સંરચના ext2fs_rb_private *bp, __u64 bit)
{
	સંરચના bmap_rb_extent *rcursor, *next_ext = ખાલી;
	સંરચના rb_ગાંઠ *parent = ખાલી, *next;
	સંરચના rb_ગાંઠ **n = &bp->મૂળ.rb_ગાંઠ;
	સંરચના bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	જો (!rcursor)
		જાઓ શોધ_વૃક્ષ;

	જો (bit >= rcursor->start && bit < rcursor->start + rcursor->ગણતરી) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		પાછા 1;
	}

	next_ext = bp->rcursor_next;
	જો (!next_ext) {
		next = ext2fs_rb_next(&rcursor->ગાંઠ);
		જો (next)
			next_ext = ગાંઠ_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	જો (next_ext) {
		જો ((bit >= rcursor->start + rcursor->ગણતરી) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			પાછા 0;
		}
	}
	bp->rcursor = ખાલી;
	bp->rcursor_next = ખાલી;

	rcursor = bp->wcursor;
	જો (!rcursor)
		જાઓ શોધ_વૃક્ષ;

	જો (bit >= rcursor->start && bit < rcursor->start + rcursor->ગણતરી)
		પાછા 1;

શોધ_વૃક્ષ:

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (bit < ext->start)
			n = &(*n)->rb_ડાબું;
		નહીંતર જો (bit >= (ext->start + ext->ગણતરી))
			n = &(*n)->rb_જમણું;
		નહીંતર {
			bp->rcursor = ext;
			bp->rcursor_next = ખાલી;
			પાછા 1;
		}
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક rb_ઉમેરો_extent(__u64 start, __u64 ગણતરી,
			    સંરચના ext2fs_rb_private *bp)
{
	સંરચના rb_મૂળ *મૂળ = &bp->મૂળ;
	સંરચના rb_ગાંઠ *parent = ખાલી, **n = &મૂળ->rb_ગાંઠ;
	સંરચના rb_ગાંઠ *new_ગાંઠ, *ગાંઠ, *next;
	સંરચના bmap_rb_extent *new_ext;
	સંરચના bmap_rb_extent *ext;
	પૂર્ણાંક retval = 0;

	જો (ગણતરી == 0)
		પાછા 0;

	bp->rcursor_next = ખાલી;
	ext = bp->wcursor;
	જો (ext) {
		જો (start >= ext->start &&
		    start <= (ext->start + ext->ગણતરી)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			જાઓ got_extent;
		}
	}

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);

		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
		} નહીંતર જો (start > (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
		} નહીંતર {
got_extent:
			જો ((start + ગણતરી) <= (ext->start + ext->ગણતરી))
				પાછા 1;

			જો ((ext->start + ext->ગણતરી) == start)
				retval = 0;
			નહીંતર
				retval = 1;

			ગણતરી += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_ગાંઠ = &ext->ગાંઠ;

			જાઓ skip_ઉમેરો;
		}
	}

	rb_get_new_extent(&new_ext, start, ગણતરી);

	new_ગાંઠ = &new_ext->ગાંઠ;
	ext2fs_rb_link_ગાંઠ(new_ગાંઠ, parent, n);
	ext2fs_rb_ઉમેરો_color(new_ગાંઠ, મૂળ);
	bp->wcursor = new_ext;

	ગાંઠ = ext2fs_rb_prev(new_ગાંઠ);
	જો (ગાંઠ) {
		ext = ગાંઠ_to_extent(ગાંઠ);
		જો ((ext->start + ext->ગણતરી) == start) {
			start = ext->start;
			ગણતરી += ext->ગણતરી;
			ext2fs_rb_erase(ગાંઠ, મૂળ);
			rb_free_extent(bp, ext);
		}
	}

skip_ઉમેરો:
	/* See if we can merge extent to the right */
	માટે (ગાંઠ = ext2fs_rb_next(new_ગાંઠ); ગાંઠ != ખાલી; ગાંઠ = next) {
		next = ext2fs_rb_next(ગાંઠ);
		ext = ગાંઠ_to_extent(ગાંઠ);

		જો ((ext->start + ext->ગણતરી) <= start)
			ચાલુ;

		/* No more merging */
		જો ((start + ગણતરી) < ext->start)
			તોડો;

		/* ext is embedded in new_ext interval */
		જો ((start + ગણતરી) >= (ext->start + ext->ગણતરી)) {
			ext2fs_rb_erase(ગાંઠ, મૂળ);
			rb_free_extent(bp, ext);
			ચાલુ;
		} નહીંતર {
		/* merge ext with new_ext */
			ગણતરી += ((ext->start + ext->ગણતરી) -
				  (start + ગણતરી));
			ext2fs_rb_erase(ગાંઠ, મૂળ);
			rb_free_extent(bp, ext);
			તોડો;
		}
	}

	new_ext->start = start;
	new_ext->ગણતરી = ગણતરી;

	પાછા retval;
}

સ્થિર પૂર્ણાંક rb_remove_extent(__u64 start, __u64 ગણતરી,
			    સંરચના ext2fs_rb_private *bp)
{
	સંરચના rb_મૂળ *મૂળ = &bp->મૂળ;
	સંરચના rb_ગાંઠ *parent = ખાલી, **n = &મૂળ->rb_ગાંઠ;
	સંરચના rb_ગાંઠ *ગાંઠ;
	સંરચના bmap_rb_extent *ext;
	__u64 new_start, new_ગણતરી;
	પૂર્ણાંક retval = 0;

	જો (ext2fs_rb_empty_મૂળ(મૂળ))
		પાછા 0;

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
			ચાલુ;
		} નહીંતર જો (start >= (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
			ચાલુ;
		}

		જો ((start > ext->start) &&
		    (start + ગણતરી) < (ext->start + ext->ગણતરી)) {
			/* We have to split extent into two */
			new_start = start + ગણતરી;
			new_ગણતરી = (ext->start + ext->ગણતરી) - new_start;

			ext->ગણતરી = start - ext->start;

			rb_ઉમેરો_extent(new_start, new_ગણતરી, bp);
			પાછા 1;
		}

		જો ((start + ગણતરી) >= (ext->start + ext->ગણતરી)) {
			ext->ગણતરી = start - ext->start;
			retval = 1;
		}

		જો (0 == ext->ગણતરી) {
			parent = ext2fs_rb_next(&ext->ગાંઠ);
			ext2fs_rb_erase(&ext->ગાંઠ, મૂળ);
			rb_free_extent(bp, ext);
			તોડો;
		}

		જો (start == ext->start) {
			ext->start += ગણતરી;
			ext->ગણતરી -= ગણતરી;
			પાછા 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	માટે (; parent != ખાલી; parent = ગાંઠ) {
		ગાંઠ = ext2fs_rb_next(parent);
		ext = ગાંઠ_to_extent(parent);
		જો ((ext->start + ext->ગણતરી) <= start)
			ચાલુ;

		/* No more extents to be removed/truncated */
		જો ((start + ગણતરી) < ext->start)
			તોડો;

		/* The entire extent is within the region to be removed */
		જો ((start + ગણતરી) >= (ext->start + ext->ગણતરી)) {
			ext2fs_rb_erase(parent, મૂળ);
			rb_free_extent(bp, ext);
			retval = 1;
			ચાલુ;
		} નહીંતર {
			/* modify the last extent in region to be removed */
			ext->ગણતરી -= ((start + ગણતરી) - ext->start);
			ext->start = start + ગણતરી;
			retval = 1;
			તોડો;
		}
	}

	પાછા retval;
}

સ્થિર પૂર્ણાંક rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	સંરચના ext2fs_rb_private *bp;
	પૂર્ણાંક retval;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	arg -= bitmap->start;

	retval = rb_ઉમેરો_extent(arg, 1, bp);
	check_વૃક્ષ(&bp->મૂળ, __func__);
	પાછા retval;
}

સ્થિર પૂર્ણાંક rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	સંરચના ext2fs_rb_private *bp;
	પૂર્ણાંક retval;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_વૃક્ષ(&bp->મૂળ, __func__);

	પાછા retval;
}

અંતઃરેખ
સ્થિર પૂર્ણાંક rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	arg -= bitmap->start;

	પાછા rb_test_bit(bp, arg);
}

સ્થિર શૂન્ય rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned પૂર્ણાંક અંક)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	arg -= bitmap->start;

	rb_ઉમેરો_extent(arg, અંક, bp);
	check_વૃક્ષ(&bp->મૂળ, __func__);
}

સ્થિર શૂન્ય rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned પૂર્ણાંક અંક)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	arg -= bitmap->start;

	rb_remove_extent(arg, અંક, bp);
	check_વૃક્ષ(&bp->મૂળ, __func__);
}

સ્થિર પૂર્ણાંક rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned પૂર્ણાંક len)
{
	સંરચના rb_ગાંઠ *parent = ખાલી, **n;
	સંરચના rb_ગાંઠ *ગાંઠ, *next;
	સંરચના ext2fs_rb_private *bp;
	સંરચના bmap_rb_extent *ext;
	પૂર્ણાંક retval = 1;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	n = &bp->મૂળ.rb_ગાંઠ;
	start -= bitmap->start;

	જો (len == 0 || ext2fs_rb_empty_મૂળ(&bp->મૂળ))
		પાછા 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
		} નહીંતર જો (start >= (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
		} નહીંતર {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			પાછા 0;
		}
	}

	ગાંઠ = parent;
	જ્યારે (ગાંઠ) {
		next = ext2fs_rb_next(ગાંઠ);
		ext = ગાંઠ_to_extent(ગાંઠ);
		ગાંઠ = next;

		જો ((ext->start + ext->ગણતરી) <= start)
			ચાલુ;

		/* No more merging */
		જો ((start + len) <= ext->start)
			તોડો;

		retval = 0;
		તોડો;
	}
	પાછા retval;
}

સ્થિર errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t અંક, શૂન્ય *in)
{
	સંરચના ext2fs_rb_private *bp;
	unsigned અક્ષર *cp = in;
	size_t i;
	પૂર્ણાંક first_set = -1;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;

	માટે (i = 0; i < અંક; i++) {
		જો ((i & 7) == 0) {
			unsigned અક્ષર c = cp[i/8];
			જો (c == 0xFF) {
				જો (first_set == -1)
					first_set = i;
				i += 7;
				ચાલુ;
			}
			જો ((c == 0x00) && (first_set == -1)) {
				i += 7;
				ચાલુ;
			}
		}
		જો (ext2fs_test_bit(i, in)) {
			જો (first_set == -1)
				first_set = i;
			ચાલુ;
		}
		જો (first_set == -1)
			ચાલુ;

		rb_ઉમેરો_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_વૃક્ષ(&bp->મૂળ, __func__);
		first_set = -1;
	}
	જો (first_set != -1) {
		rb_ઉમેરો_extent(start + first_set - bitmap->start,
				 અંક - first_set, bp);
		check_વૃક્ષ(&bp->મૂળ, __func__);
	}

	પાછા 0;
}

સ્થિર errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t અંક, શૂન્ય *out)
{

	સંરચના rb_ગાંઠ *parent = ખાલી, *next, **n;
	સંરચના ext2fs_rb_private *bp;
	સંરચના bmap_rb_extent *ext;
	__u64 ગણતરી, pos;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	n = &bp->મૂળ.rb_ગાંઠ;
	start -= bitmap->start;

	જો (ext2fs_rb_empty_મૂળ(&bp->મૂળ))
		પાછા 0;

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
		} નહીંતર જો (start >= (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
		} નહીંતર
			તોડો;
	}

	memset(out, 0, (અંક + 7) >> 3);

	માટે (; parent != ખાલી; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = ગાંઠ_to_extent(parent);

		pos = ext->start;
		ગણતરી = ext->ગણતરી;
		જો (pos >= start + અંક)
			તોડો;
		જો (pos < start) {
			જો (pos + ગણતરી <  start)
				ચાલુ;
			ગણતરી -= start - pos;
			pos = start;
		}
		જો (pos + ગણતરી > start + અંક)
			ગણતરી = start + અંક - pos;

		જ્યારે (ગણતરી > 0) {
			જો ((ગણતરી >= 8) &&
			    ((pos - start) % 8) == 0) {
				પૂર્ણાંક nbytes = ગણતરી >> 3;
				પૂર્ણાંક offset = (pos - start) >> 3;

				memset(((અક્ષર *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				ગણતરી -= nbytes << 3;
				ચાલુ;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			ગણતરી--;
		}
	}
	પાછા 0;
}

સ્થિર શૂન્ય rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	સંરચના ext2fs_rb_private *bp;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;

	rb_free_વૃક્ષ(&bp->મૂળ);
	bp->rcursor = ખાલી;
	bp->rcursor_next = ખાલી;
	bp->wcursor = ખાલી;
	check_વૃક્ષ(&bp->મૂળ, __func__);
}

સ્થિર errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	સંરચના rb_ગાંઠ *parent = ખાલી, **n;
	સંરચના ext2fs_rb_private *bp;
	સંરચના bmap_rb_extent *ext;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	n = &bp->મૂળ.rb_ગાંઠ;
	start -= bitmap->start;
	end -= bitmap->start;

	જો (start > end)
		પાછા EINVAL;

	જો (ext2fs_rb_empty_મૂળ(&bp->મૂળ))
		પાછા ENOENT;

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
		} નહીંતર જો (start >= (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
		} નહીંતર જો (ext->start + ext->ગણતરી <= end) {
			*out = ext->start + ext->ગણતરી + bitmap->start;
			પાછા 0;
		} નહીંતર
			પાછા ENOENT;
	}

	*out = start + bitmap->start;
	પાછા 0;
}

સ્થિર errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	સંરચના rb_ગાંઠ *parent = ખાલી, **n;
	સંરચના rb_ગાંઠ *ગાંઠ;
	સંરચના ext2fs_rb_private *bp;
	સંરચના bmap_rb_extent *ext;

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;
	n = &bp->મૂળ.rb_ગાંઠ;
	start -= bitmap->start;
	end -= bitmap->start;

	જો (start > end)
		પાછા EINVAL;

	જો (ext2fs_rb_empty_મૂળ(&bp->મૂળ))
		પાછા ENOENT;

	જ્યારે (*n) {
		parent = *n;
		ext = ગાંઠ_to_extent(parent);
		જો (start < ext->start) {
			n = &(*n)->rb_ડાબું;
		} નહીંતર જો (start >= (ext->start + ext->ગણતરી)) {
			n = &(*n)->rb_જમણું;
		} નહીંતર {
			/* The start bit is set */
			*out = start + bitmap->start;
			પાછા 0;
		}
	}

	ગાંઠ = parent;
	ext = ગાંઠ_to_extent(ગાંઠ);
	જો (ext->start < start) {
		ગાંઠ = ext2fs_rb_next(ગાંઠ);
		જો (ગાંઠ == ખાલી)
			પાછા ENOENT;
		ext = ગાંઠ_to_extent(ગાંઠ);
	}
	જો (ext->start <= end) {
		*out = ext->start + bitmap->start;
		પાછા 0;
	}
	પાછા ENOENT;
}

#ifdef ENABLE_BMAP_STATS
સ્થિર શૂન્ય rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	સંરચના ext2fs_rb_private *bp;
	સંરચના rb_ગાંઠ *ગાંઠ = ખાલી;
	સંરચના bmap_rb_extent *ext;
	__u64 ગણતરી = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	દ્વિગુણ eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	દ્વિગુણ m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (સંરચના ext2fs_rb_private *) bitmap->ખાનગી;

	માટે (ગાંઠ = ext2fs_rb_first(&bp->મૂળ); ગાંઠ != ખાલી;
	     ગાંઠ = ext2fs_rb_next(ગાંઠ)) {
		ext = ગાંઠ_to_extent(ગાંઠ);
		ગણતરી++;
		જો (ext->ગણતરી > max_size)
			max_size = ext->ગણતરી;
		જો (ext->ગણતરી < min_size)
			min_size = ext->ગણતરી;
		size += ext->ગણતરી;
	}

	જો (ગણતરી)
		avg_size = size / ગણતરી;
	જો (min_size == ULONG_MAX)
		min_size = 0;
	eff = (દ્વિગુણ)((ગણતરી * કદ(સંરચના bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ગણતરી + bitmap->stats.mark_ext_ગણતરી;
	test_all = bitmap->stats.test_ગણતરી + bitmap->stats.test_ext_ગણતરી;
	જો (mark_all)
		m_hit = ((દ્વિગુણ)bp->mark_hit / mark_all) * 100;
	જો (test_all)
		t_hit = ((દ્વિગુણ)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) ગણતરી, (unsigned long long)
		((ગણતરી * કદ(સંરચના bmap_rb_extent)) +
		 કદ(સંરચના ext2fs_rb_private)));
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
#નહીંતર
સ્થિર શૂન્ય rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

સંરચના ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
