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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_LINUX_TYPES_H
#ଅନ୍ତର୍ଭାଵ <linux/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"
#ଅନ୍ତର୍ଭାଵ "bmap64.h"
#ଅନ୍ତର୍ଭାଵ "rbtree.h"

#ଅନ୍ତର୍ଭାଵ <limits.h>

ସଂରଚନା bmap_rb_extent {
	ସଂରଚନା rb_ଗଣ୍ଠି ଗଣ୍ଠି;
	__u64 start;
	__u64 ଗଣନା_2;
};

ସଂରଚନା ext2fs_rb_private {
	ସଂରଚନା rb_ମୂଳ ମୂଳ;
	ସଂରଚନା bmap_rb_extent *wcursor;
	ସଂରଚନା bmap_rb_extent *rcursor;
	ସଂରଚନା bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

ଧାଡ଼ିଭିତରେ ସ୍ଥିର ସଂରଚନା bmap_rb_extent *ଗଣ୍ଠି_to_extent(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ଫେରନ୍ତୁ (ସଂରଚନା bmap_rb_extent *) ଗଣ୍ଠି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_ଭର୍ତ୍ତି_extent(__u64 start, __u64 ଗଣନା_2,
			    ସଂରଚନା ext2fs_rb_private *);
ସ୍ଥିର ଶୂନ୍ୟ rb_get_new_extent(ସଂରଚନା bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ସ୍ଥିର ଶୂନ୍ୟ print_ବୃକ୍ଷ(ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
	ସଂରଚନା bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ଗଣ୍ଠି = ext2fs_rb_first(ମୂଳ);
	ପାଇଁ (ଗଣ୍ଠି = ext2fs_rb_first(ମୂଳ); ଗଣ୍ଠି != ଖାଲି; 
	     ଗଣ୍ଠି = ext2fs_rb_next(ଗଣ୍ଠି)) {
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->ଗଣନା_2);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ସ୍ଥିର ଶୂନ୍ୟ check_ବୃକ୍ଷ(ସଂରଚନା rb_ମୂଳ *ମୂଳ, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି;
	ସଂରଚନା bmap_rb_extent *ext, *old = ଖାଲି;

	ପାଇଁ (ଗଣ୍ଠି = ext2fs_rb_first(ମୂଳ); ଗଣ୍ଠି;
	     ଗଣ୍ଠି = ext2fs_rb_next(ଗଣ୍ଠି)) {
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		ଯଦି (ext->ଗଣନା_2 == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ଗଣନା_2,
				(unsigned long long) ext->ଗଣନା_2);
			ଯାଅ err_out;
		}
		ଯଦି (ext->start + ext->ଗଣନା_2 < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->ଗଣନା_2,
				(unsigned long long) ext->ଗଣନା_2);
			ଯାଅ err_out;
		}

		ଯଦି (old) {
			ଯଦି (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ଗଣନା_2,
					(unsigned long long) old->ଗଣନା_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ଗଣନା_2,
					(unsigned long long) ext->ଗଣନା_2);
				ଯାଅ err_out;
			}
			ଯଦି ((old->start + old->ଗଣନା_2) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->ଗଣନା_2,
					(unsigned long long) old->ଗଣନା_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->ଗଣନା_2,
					(unsigned long long) ext->ଗଣନା_2);
				ଯାଅ err_out;
			}
		}
		old = ext;
	}
	ଫେରନ୍ତୁ;

err_out:
	fprintf(stderr, "%s\n", ସନ୍ଦେଶ);
	print_ବୃକ୍ଷ(ମୂଳ);
	exit(1);
}
#ଅନ୍ୟଥା
#define check_ବୃକ୍ଷ(ମୂଳ, ସନ୍ଦେଶ) କର {} ଯେତେବେଳେ (0)
#define print_ବୃକ୍ଷ(ମୂଳ) କର {} ଯେତେବେଳେ (0)
#endif

ସ୍ଥିର ଶୂନ୍ୟ rb_get_new_extent(ସଂରଚନା bmap_rb_extent **ext, __u64 start,
			     __u64 ଗଣନା_2)
{
	ସଂରଚନା bmap_rb_extent *new_ext;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;

	retval = ext2fs_get_mem(ଆକାର (ସଂରଚନା bmap_rb_extent),
				&new_ext);
	ଯଦି (retval)
		abort();

	new_ext->start = start;
	new_ext->ଗଣନା_2 = ଗଣନା_2;
	*ext = new_ext;
}

ଧାଡ଼ିଭିତରେ
ସ୍ଥିର ଶୂନ୍ୟ rb_free_extent(ସଂରଚନା ext2fs_rb_private *bp,
			   ସଂରଚନା bmap_rb_extent *ext)
{
	ଯଦି (bp->wcursor == ext)
		bp->wcursor = ଖାଲି;
	ଯଦି (bp->rcursor == ext)
		bp->rcursor = ଖାଲି;
	ଯଦି (bp->rcursor_next == ext)
		bp->rcursor_next = ଖାଲି;
	ext2fs_free_mem(&ext);
}

ସ୍ଥିର errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ସଂରଚନା ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(ଆକାର (ସଂରଚନା ext2fs_rb_private), &bp);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	bp->ମୂଳ = RB_ମୂଳ;
	bp->rcursor = ଖାଲି;
	bp->rcursor_next = ଖାଲି;
	bp->wcursor = ଖାଲି;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->ବ୍ୟକ୍ତିଗତ = (ଶୂନ୍ୟ *) bp;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ rb_free_ବୃକ୍ଷ(ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା bmap_rb_extent *ext;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, *next;

	ପାଇଁ (ଗଣ୍ଠି = ext2fs_rb_first(ମୂଳ); ଗଣ୍ଠି; ଗଣ୍ଠି = next) {
		next = ext2fs_rb_next(ଗଣ୍ଠି);
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		ext2fs_rb_erase(ଗଣ୍ଠି, ମୂଳ);
		ext2fs_free_mem(&ext);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;

	rb_free_ବୃକ୍ଷ(&bp->ମୂଳ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ସ୍ଥିର errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ସଂରଚନା ext2fs_rb_private *src_bp, *dest_bp;
	ସଂରଚନା bmap_rb_extent *src_ext, *dest_ext;
	ସଂରଚନା rb_ଗଣ୍ଠି *dest_ଗଣ୍ଠି, *src_ଗଣ୍ଠି, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	src_bp = (ସଂରଚନା ext2fs_rb_private *) src->ବ୍ୟକ୍ତିଗତ;
	dest_bp = (ସଂରଚନା ext2fs_rb_private *) dest->ବ୍ୟକ୍ତିଗତ;
	src_bp->rcursor = ଖାଲି;
	dest_bp->rcursor = ଖାଲି;

	src_ଗଣ୍ଠି = ext2fs_rb_first(&src_bp->ମୂଳ);
	ଯେତେବେଳେ (src_ଗଣ୍ଠି) {
		src_ext = ଗଣ୍ଠି_to_extent(src_ଗଣ୍ଠି);
		retval = ext2fs_get_mem(ଆକାର (ସଂରଚନା bmap_rb_extent),
					&dest_ext);
		ଯଦି (retval)
			ଭାଙ୍ଗ;

		memcpy(dest_ext, src_ext, ଆକାର(ସଂରଚନା bmap_rb_extent));

		dest_ଗଣ୍ଠି = &dest_ext->ଗଣ୍ଠି;
		n = &dest_bp->ମୂଳ.rb_ଗଣ୍ଠି;

		dest_last = ଖାଲି;
		ଯଦି (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->ମୂଳ);
			n = &(dest_last)->rb_ଡାହାଣ;
		}

		ext2fs_rb_link_ଗଣ୍ଠି(dest_ଗଣ୍ଠି, dest_last, n);
		ext2fs_rb_ଭର୍ତ୍ତି_color(dest_ଗଣ୍ଠି, &dest_bp->ମୂଳ);

		src_ଗଣ୍ଠି = ext2fs_rb_next(src_ଗଣ୍ଠି);
	}

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଶୂନ୍ୟ rb_truncate(__u64 new_max, ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା bmap_rb_extent *ext;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି;

	ଗଣ୍ଠି = ext2fs_rb_last(ମୂଳ);
	ଯେତେବେଳେ (ଗଣ୍ଠି) {
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);

		ଯଦି ((ext->start + ext->ଗଣନା_2 - 1) <= new_max)
			ଭାଙ୍ଗ;
		ଅନ୍ୟଥା ଯଦି (ext->start > new_max) {
			ext2fs_rb_erase(ଗଣ୍ଠି, ମୂଳ);
			ext2fs_free_mem(&ext);
			ଗଣ୍ଠି = ext2fs_rb_last(ମୂଳ);
			ଜାରି;
		} ଅନ୍ୟଥା
			ext->ଗଣନା_2 = new_max - ext->start + 1;
	}
}

ସ୍ଥିର errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bmap->ବ୍ୟକ୍ତିଗତ;
	bp->rcursor = ଖାଲି;
	bp->wcursor = ଖାଲି;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->ମୂଳ);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	ଯଦି (bmap->end < bmap->real_end)
		rb_ଭର୍ତ୍ତି_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	ଫେରନ୍ତୁ 0;

}

ଧାଡ଼ିଭିତରେ ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
rb_test_bit(ସଂରଚନା ext2fs_rb_private *bp, __u64 bit)
{
	ସଂରଚନା bmap_rb_extent *rcursor, *next_ext = ଖାଲି;
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, *next;
	ସଂରଚନା rb_ଗଣ୍ଠି **n = &bp->ମୂଳ.rb_ଗଣ୍ଠି;
	ସଂରଚନା bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ଯଦି (!rcursor)
		ଯାଅ ସନ୍ଧାନ_ବୃକ୍ଷ;

	ଯଦି (bit >= rcursor->start && bit < rcursor->start + rcursor->ଗଣନା_2) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		ଫେରନ୍ତୁ 1;
	}

	next_ext = bp->rcursor_next;
	ଯଦି (!next_ext) {
		next = ext2fs_rb_next(&rcursor->ଗଣ୍ଠି);
		ଯଦି (next)
			next_ext = ଗଣ୍ଠି_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	ଯଦି (next_ext) {
		ଯଦି ((bit >= rcursor->start + rcursor->ଗଣନା_2) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			ଫେରନ୍ତୁ 0;
		}
	}
	bp->rcursor = ଖାଲି;
	bp->rcursor_next = ଖାଲି;

	rcursor = bp->wcursor;
	ଯଦି (!rcursor)
		ଯାଅ ସନ୍ଧାନ_ବୃକ୍ଷ;

	ଯଦି (bit >= rcursor->start && bit < rcursor->start + rcursor->ଗଣନା_2)
		ଫେରନ୍ତୁ 1;

ସନ୍ଧାନ_ବୃକ୍ଷ:

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (bit < ext->start)
			n = &(*n)->rb_ବାମ;
		ଅନ୍ୟଥା ଯଦି (bit >= (ext->start + ext->ଗଣନା_2))
			n = &(*n)->rb_ଡାହାଣ;
		ଅନ୍ୟଥା {
			bp->rcursor = ext;
			bp->rcursor_next = ଖାଲି;
			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_ଭର୍ତ୍ତି_extent(__u64 start, __u64 ଗଣନା_2,
			    ସଂରଚନା ext2fs_rb_private *bp)
{
	ସଂରଚନା rb_ମୂଳ *ମୂଳ = &bp->ମୂଳ;
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, **n = &ମୂଳ->rb_ଗଣ୍ଠି;
	ସଂରଚନା rb_ଗଣ୍ଠି *new_ଗଣ୍ଠି, *ଗଣ୍ଠି, *next;
	ସଂରଚନା bmap_rb_extent *new_ext;
	ସଂରଚନା bmap_rb_extent *ext;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;

	ଯଦି (ଗଣନା_2 == 0)
		ଫେରନ୍ତୁ 0;

	bp->rcursor_next = ଖାଲି;
	ext = bp->wcursor;
	ଯଦି (ext) {
		ଯଦି (start >= ext->start &&
		    start <= (ext->start + ext->ଗଣନା_2)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			ଯାଅ got_extent;
		}
	}

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);

		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
		} ଅନ୍ୟଥା ଯଦି (start > (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
		} ଅନ୍ୟଥା {
got_extent:
			ଯଦି ((start + ଗଣନା_2) <= (ext->start + ext->ଗଣନା_2))
				ଫେରନ୍ତୁ 1;

			ଯଦି ((ext->start + ext->ଗଣନା_2) == start)
				retval = 0;
			ଅନ୍ୟଥା
				retval = 1;

			ଗଣନା_2 += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_ଗଣ୍ଠି = &ext->ଗଣ୍ଠି;

			ଯାଅ skip_ଭର୍ତ୍ତି;
		}
	}

	rb_get_new_extent(&new_ext, start, ଗଣନା_2);

	new_ଗଣ୍ଠି = &new_ext->ଗଣ୍ଠି;
	ext2fs_rb_link_ଗଣ୍ଠି(new_ଗଣ୍ଠି, parent, n);
	ext2fs_rb_ଭର୍ତ୍ତି_color(new_ଗଣ୍ଠି, ମୂଳ);
	bp->wcursor = new_ext;

	ଗଣ୍ଠି = ext2fs_rb_prev(new_ଗଣ୍ଠି);
	ଯଦି (ଗଣ୍ଠି) {
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		ଯଦି ((ext->start + ext->ଗଣନା_2) == start) {
			start = ext->start;
			ଗଣନା_2 += ext->ଗଣନା_2;
			ext2fs_rb_erase(ଗଣ୍ଠି, ମୂଳ);
			rb_free_extent(bp, ext);
		}
	}

skip_ଭର୍ତ୍ତି:
	/* See if we can merge extent to the right */
	ପାଇଁ (ଗଣ୍ଠି = ext2fs_rb_next(new_ଗଣ୍ଠି); ଗଣ୍ଠି != ଖାଲି; ଗଣ୍ଠି = next) {
		next = ext2fs_rb_next(ଗଣ୍ଠି);
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);

		ଯଦି ((ext->start + ext->ଗଣନା_2) <= start)
			ଜାରି;

		/* No more merging */
		ଯଦି ((start + ଗଣନା_2) < ext->start)
			ଭାଙ୍ଗ;

		/* ext is embedded in new_ext interval */
		ଯଦି ((start + ଗଣନା_2) >= (ext->start + ext->ଗଣନା_2)) {
			ext2fs_rb_erase(ଗଣ୍ଠି, ମୂଳ);
			rb_free_extent(bp, ext);
			ଜାରି;
		} ଅନ୍ୟଥା {
		/* merge ext with new_ext */
			ଗଣନା_2 += ((ext->start + ext->ଗଣନା_2) -
				  (start + ଗଣନା_2));
			ext2fs_rb_erase(ଗଣ୍ଠି, ମୂଳ);
			rb_free_extent(bp, ext);
			ଭାଙ୍ଗ;
		}
	}

	new_ext->start = start;
	new_ext->ଗଣନା_2 = ଗଣନା_2;

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_remove_extent(__u64 start, __u64 ଗଣନା_2,
			    ସଂରଚନା ext2fs_rb_private *bp)
{
	ସଂରଚନା rb_ମୂଳ *ମୂଳ = &bp->ମୂଳ;
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, **n = &ମୂଳ->rb_ଗଣ୍ଠି;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି;
	ସଂରଚନା bmap_rb_extent *ext;
	__u64 new_start, new_ଗଣନା;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;

	ଯଦି (ext2fs_rb_empty_ମୂଳ(ମୂଳ))
		ଫେରନ୍ତୁ 0;

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
			ଜାରି;
		} ଅନ୍ୟଥା ଯଦି (start >= (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
			ଜାରି;
		}

		ଯଦି ((start > ext->start) &&
		    (start + ଗଣନା_2) < (ext->start + ext->ଗଣନା_2)) {
			/* We have to split extent into two */
			new_start = start + ଗଣନା_2;
			new_ଗଣନା = (ext->start + ext->ଗଣନା_2) - new_start;

			ext->ଗଣନା_2 = start - ext->start;

			rb_ଭର୍ତ୍ତି_extent(new_start, new_ଗଣନା, bp);
			ଫେରନ୍ତୁ 1;
		}

		ଯଦି ((start + ଗଣନା_2) >= (ext->start + ext->ଗଣନା_2)) {
			ext->ଗଣନା_2 = start - ext->start;
			retval = 1;
		}

		ଯଦି (0 == ext->ଗଣନା_2) {
			parent = ext2fs_rb_next(&ext->ଗଣ୍ଠି);
			ext2fs_rb_erase(&ext->ଗଣ୍ଠି, ମୂଳ);
			rb_free_extent(bp, ext);
			ଭାଙ୍ଗ;
		}

		ଯଦି (start == ext->start) {
			ext->start += ଗଣନା_2;
			ext->ଗଣନା_2 -= ଗଣନା_2;
			ଫେରନ୍ତୁ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	ପାଇଁ (; parent != ଖାଲି; parent = ଗଣ୍ଠି) {
		ଗଣ୍ଠି = ext2fs_rb_next(parent);
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି ((ext->start + ext->ଗଣନା_2) <= start)
			ଜାରି;

		/* No more extents to be removed/truncated */
		ଯଦି ((start + ଗଣନା_2) < ext->start)
			ଭାଙ୍ଗ;

		/* The entire extent is within the region to be removed */
		ଯଦି ((start + ଗଣନା_2) >= (ext->start + ext->ଗଣନା_2)) {
			ext2fs_rb_erase(parent, ମୂଳ);
			rb_free_extent(bp, ext);
			retval = 1;
			ଜାରି;
		} ଅନ୍ୟଥା {
			/* modify the last extent in region to be removed */
			ext->ଗଣନା_2 -= ((start + ଗଣନା_2) - ext->start);
			ext->start = start + ଗଣନା_2;
			retval = 1;
			ଭାଙ୍ଗ;
		}
	}

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ସଂରଚନା ext2fs_rb_private *bp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	arg -= bitmap->start;

	retval = rb_ଭର୍ତ୍ତି_extent(arg, 1, bp);
	check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ସଂରଚନା ext2fs_rb_private *bp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);

	ଫେରନ୍ତୁ retval;
}

ଧାଡ଼ିଭିତରେ
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	arg -= bitmap->start;

	ଫେରନ୍ତୁ rb_test_bit(bp, arg);
}

ସ୍ଥିର ଶୂନ୍ୟ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	arg -= bitmap->start;

	rb_ଭର୍ତ୍ତି_extent(arg, ଅଙ୍କ, bp);
	check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
}

ସ୍ଥିର ଶୂନ୍ୟ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	arg -= bitmap->start;

	rb_remove_extent(arg, ଅଙ୍କ, bp);
	check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, **n;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି, *next;
	ସଂରଚନା ext2fs_rb_private *bp;
	ସଂରଚନା bmap_rb_extent *ext;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 1;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	n = &bp->ମୂଳ.rb_ଗଣ୍ଠି;
	start -= bitmap->start;

	ଯଦି (len == 0 || ext2fs_rb_empty_ମୂଳ(&bp->ମୂଳ))
		ଫେରନ୍ତୁ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
		} ଅନ୍ୟଥା ଯଦି (start >= (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
		} ଅନ୍ୟଥା {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ଫେରନ୍ତୁ 0;
		}
	}

	ଗଣ୍ଠି = parent;
	ଯେତେବେଳେ (ଗଣ୍ଠି) {
		next = ext2fs_rb_next(ଗଣ୍ଠି);
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		ଗଣ୍ଠି = next;

		ଯଦି ((ext->start + ext->ଗଣନା_2) <= start)
			ଜାରି;

		/* No more merging */
		ଯଦି ((start + len) <= ext->start)
			ଭାଙ୍ଗ;

		retval = 0;
		ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *in)
{
	ସଂରଚନା ext2fs_rb_private *bp;
	unsigned ଅକ୍ଷର *cp = in;
	size_t i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_set = -1;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;

	ପାଇଁ (i = 0; i < ଅଙ୍କ; i++) {
		ଯଦି ((i & 7) == 0) {
			unsigned ଅକ୍ଷର c = cp[i/8];
			ଯଦି (c == 0xFF) {
				ଯଦି (first_set == -1)
					first_set = i;
				i += 7;
				ଜାରି;
			}
			ଯଦି ((c == 0x00) && (first_set == -1)) {
				i += 7;
				ଜାରି;
			}
		}
		ଯଦି (ext2fs_test_bit(i, in)) {
			ଯଦି (first_set == -1)
				first_set = i;
			ଜାରି;
		}
		ଯଦି (first_set == -1)
			ଜାରି;

		rb_ଭର୍ତ୍ତି_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
		first_set = -1;
	}
	ଯଦି (first_set != -1) {
		rb_ଭର୍ତ୍ତି_extent(start + first_set - bitmap->start,
				 ଅଙ୍କ - first_set, bp);
		check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *out)
{

	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, *next, **n;
	ସଂରଚନା ext2fs_rb_private *bp;
	ସଂରଚନା bmap_rb_extent *ext;
	__u64 ଗଣନା_2, pos;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	n = &bp->ମୂଳ.rb_ଗଣ୍ଠି;
	start -= bitmap->start;

	ଯଦି (ext2fs_rb_empty_ମୂଳ(&bp->ମୂଳ))
		ଫେରନ୍ତୁ 0;

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
		} ଅନ୍ୟଥା ଯଦି (start >= (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
		} ଅନ୍ୟଥା
			ଭାଙ୍ଗ;
	}

	memset(out, 0, (ଅଙ୍କ + 7) >> 3);

	ପାଇଁ (; parent != ଖାଲି; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = ଗଣ୍ଠି_to_extent(parent);

		pos = ext->start;
		ଗଣନା_2 = ext->ଗଣନା_2;
		ଯଦି (pos >= start + ଅଙ୍କ)
			ଭାଙ୍ଗ;
		ଯଦି (pos < start) {
			ଯଦି (pos + ଗଣନା_2 <  start)
				ଜାରି;
			ଗଣନା_2 -= start - pos;
			pos = start;
		}
		ଯଦି (pos + ଗଣନା_2 > start + ଅଙ୍କ)
			ଗଣନା_2 = start + ଅଙ୍କ - pos;

		ଯେତେବେଳେ (ଗଣନା_2 > 0) {
			ଯଦି ((ଗଣନା_2 >= 8) &&
			    ((pos - start) % 8) == 0) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbytes = ଗଣନା_2 >> 3;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = (pos - start) >> 3;

				memset(((ଅକ୍ଷର *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				ଗଣନା_2 -= nbytes << 3;
				ଜାରି;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			ଗଣନା_2--;
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ସଂରଚନା ext2fs_rb_private *bp;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;

	rb_free_ବୃକ୍ଷ(&bp->ମୂଳ);
	bp->rcursor = ଖାଲି;
	bp->rcursor_next = ଖାଲି;
	bp->wcursor = ଖାଲି;
	check_ବୃକ୍ଷ(&bp->ମୂଳ, __func__);
}

ସ୍ଥିର errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, **n;
	ସଂରଚନା ext2fs_rb_private *bp;
	ସଂରଚନା bmap_rb_extent *ext;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	n = &bp->ମୂଳ.rb_ଗଣ୍ଠି;
	start -= bitmap->start;
	end -= bitmap->start;

	ଯଦି (start > end)
		ଫେରନ୍ତୁ EINVAL;

	ଯଦି (ext2fs_rb_empty_ମୂଳ(&bp->ମୂଳ))
		ଫେରନ୍ତୁ ENOENT;

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
		} ଅନ୍ୟଥା ଯଦି (start >= (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
		} ଅନ୍ୟଥା ଯଦି (ext->start + ext->ଗଣନା_2 <= end) {
			*out = ext->start + ext->ଗଣନା_2 + bitmap->start;
			ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା
			ଫେରନ୍ତୁ ENOENT;
	}

	*out = start + bitmap->start;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *parent = ଖାଲି, **n;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି;
	ସଂରଚନା ext2fs_rb_private *bp;
	ସଂରଚନା bmap_rb_extent *ext;

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;
	n = &bp->ମୂଳ.rb_ଗଣ୍ଠି;
	start -= bitmap->start;
	end -= bitmap->start;

	ଯଦି (start > end)
		ଫେରନ୍ତୁ EINVAL;

	ଯଦି (ext2fs_rb_empty_ମୂଳ(&bp->ମୂଳ))
		ଫେରନ୍ତୁ ENOENT;

	ଯେତେବେଳେ (*n) {
		parent = *n;
		ext = ଗଣ୍ଠି_to_extent(parent);
		ଯଦି (start < ext->start) {
			n = &(*n)->rb_ବାମ;
		} ଅନ୍ୟଥା ଯଦି (start >= (ext->start + ext->ଗଣନା_2)) {
			n = &(*n)->rb_ଡାହାଣ;
		} ଅନ୍ୟଥା {
			/* The start bit is set */
			*out = start + bitmap->start;
			ଫେରନ୍ତୁ 0;
		}
	}

	ଗଣ୍ଠି = parent;
	ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
	ଯଦି (ext->start < start) {
		ଗଣ୍ଠି = ext2fs_rb_next(ଗଣ୍ଠି);
		ଯଦି (ଗଣ୍ଠି == ଖାଲି)
			ଫେରନ୍ତୁ ENOENT;
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
	}
	ଯଦି (ext->start <= end) {
		*out = ext->start + bitmap->start;
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ସ୍ଥିର ଶୂନ୍ୟ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ସଂରଚନା ext2fs_rb_private *bp;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
	ସଂରଚନା bmap_rb_extent *ext;
	__u64 ଗଣନା_2 = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	ଦ୍ୱିଗୁଣ eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ଦ୍ୱିଗୁଣ m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (ସଂରଚନା ext2fs_rb_private *) bitmap->ବ୍ୟକ୍ତିଗତ;

	ପାଇଁ (ଗଣ୍ଠି = ext2fs_rb_first(&bp->ମୂଳ); ଗଣ୍ଠି != ଖାଲି;
	     ଗଣ୍ଠି = ext2fs_rb_next(ଗଣ୍ଠି)) {
		ext = ଗଣ୍ଠି_to_extent(ଗଣ୍ଠି);
		ଗଣନା_2++;
		ଯଦି (ext->ଗଣନା_2 > max_size)
			max_size = ext->ଗଣନା_2;
		ଯଦି (ext->ଗଣନା_2 < min_size)
			min_size = ext->ଗଣନା_2;
		size += ext->ଗଣନା_2;
	}

	ଯଦି (ଗଣନା_2)
		avg_size = size / ଗଣନା_2;
	ଯଦି (min_size == ULONG_MAX)
		min_size = 0;
	eff = (ଦ୍ୱିଗୁଣ)((ଗଣନା_2 * ଆକାର(ସଂରଚନା bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ଗଣନା + bitmap->stats.mark_ext_ଗଣନା;
	test_all = bitmap->stats.test_ଗଣନା + bitmap->stats.test_ext_ଗଣନା;
	ଯଦି (mark_all)
		m_hit = ((ଦ୍ୱିଗୁଣ)bp->mark_hit / mark_all) * 100;
	ଯଦି (test_all)
		t_hit = ((ଦ୍ୱିଗୁଣ)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) ଗଣନା_2, (unsigned long long)
		((ଗଣନା_2 * ଆକାର(ସଂରଚନା bmap_rb_extent)) +
		 ଆକାର(ସଂରଚନା ext2fs_rb_private)));
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
#ଅନ୍ୟଥା
ସ୍ଥିର ଶୂନ୍ୟ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

ସଂରଚନା ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
