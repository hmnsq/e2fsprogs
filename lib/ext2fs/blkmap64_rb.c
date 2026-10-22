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

#include "config.h"
#include <stdio.h>
#include <string.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_LINUX_TYPES_H
#include <linux/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "bmap64.h"
#include "rbtree.h"

#include <limits.h>

struct bmap_rb_extent {
	struct rb_बुहोल बुहोल;
	__u64 start;
	__u64 बिलांग;
};

struct ext2fs_rb_private {
	struct rb_उगात उगात;
	struct bmap_rb_extent *wcursor;
	struct bmap_rb_extent *rcursor;
	struct bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

inline static struct bmap_rb_extent *बुहोल_to_extent(struct rb_बुहोल *बुहोल)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	return (struct bmap_rb_extent *) बुहोल;
}

static int rb_इसिंगित_extent(__u64 start, __u64 बिलांग,
			    struct ext2fs_rb_private *);
static void rb_get_new_extent(struct bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
static void इलिम्बाग_पुनो(struct rb_उगात *उगात)
{
	struct rb_बुहोल *बुहोल = NULL;
	struct bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	बुहोल = ext2fs_rb_first(उगात);
	for (बुहोल = ext2fs_rb_first(उगात); बुहोल != NULL; 
	     बुहोल = ext2fs_rb_next(बुहोल)) {
		ext = बुहोल_to_extent(बुहोल);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->बिलांग);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

static void check_पुनो(struct rb_उगात *उगात, const char *मेनसाहे)
{
	struct rb_बुहोल *बुहोल;
	struct bmap_rb_extent *ext, *old = NULL;

	for (बुहोल = ext2fs_rb_first(उगात); बुहोल;
	     बुहोल = ext2fs_rb_next(बुहोल)) {
		ext = बुहोल_to_extent(बुहोल);
		if (ext->बिलांग == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->बिलांग,
				(unsigned long long) ext->बिलांग);
			goto err_out;
		}
		if (ext->start + ext->बिलांग < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->बिलांग,
				(unsigned long long) ext->बिलांग);
			goto err_out;
		}

		if (old) {
			if (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->बिलांग,
					(unsigned long long) old->बिलांग);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->बिलांग,
					(unsigned long long) ext->बिलांग);
				goto err_out;
			}
			if ((old->start + old->बिलांग) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->बिलांग,
					(unsigned long long) old->बिलांग);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->बिलांग,
					(unsigned long long) ext->बिलांग);
				goto err_out;
			}
		}
		old = ext;
	}
	return;

err_out:
	fprintf(stderr, "%s\n", मेनसाहे);
	इलिम्बाग_पुनो(उगात);
	exit(1);
}
#else
#define check_पुनो(उगात, मेनसाहे) do {} while (0)
#define इलिम्बाग_पुनो(उगात) do {} while (0)
#endif

static void rb_get_new_extent(struct bmap_rb_extent **ext, __u64 start,
			     __u64 बिलांग)
{
	struct bmap_rb_extent *new_ext;
	int retval;

	retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
				&new_ext);
	if (retval)
		abort();

	new_ext->start = start;
	new_ext->बिलांग = बिलांग;
	*ext = new_ext;
}

inline
static void rb_free_extent(struct ext2fs_rb_private *bp,
			   struct bmap_rb_extent *ext)
{
	if (bp->wcursor == ext)
		bp->wcursor = NULL;
	if (bp->rcursor == ext)
		bp->rcursor = NULL;
	if (bp->rcursor_next == ext)
		bp->rcursor_next = NULL;
	ext2fs_free_mem(&ext);
}

static errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof (struct ext2fs_rb_private), &bp);
	if (retval)
		return retval;

	bp->उगात = RB_उगात;
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->private = (void *) bp;
	return 0;
}

static errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	if (retval)
		return retval;

	return 0;
}

static void rb_free_पुनो(struct rb_उगात *उगात)
{
	struct bmap_rb_extent *ext;
	struct rb_बुहोल *बुहोल, *next;

	for (बुहोल = ext2fs_rb_first(उगात); बुहोल; बुहोल = next) {
		next = ext2fs_rb_next(बुहोल);
		ext = बुहोल_to_extent(बुहोल);
		ext2fs_rb_erase(बुहोल, उगात);
		ext2fs_free_mem(&ext);
	}
}

static void rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_पुनो(&bp->उगात);
	ext2fs_free_mem(&bp);
	bp = 0;
}

static errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	struct ext2fs_rb_private *src_bp, *dest_bp;
	struct bmap_rb_extent *src_ext, *dest_ext;
	struct rb_बुहोल *dest_बुहोल, *src_बुहोल, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	if (retval)
		return retval;

	src_bp = (struct ext2fs_rb_private *) src->private;
	dest_bp = (struct ext2fs_rb_private *) dest->private;
	src_bp->rcursor = NULL;
	dest_bp->rcursor = NULL;

	src_बुहोल = ext2fs_rb_first(&src_bp->उगात);
	while (src_बुहोल) {
		src_ext = बुहोल_to_extent(src_बुहोल);
		retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
					&dest_ext);
		if (retval)
			break;

		memcpy(dest_ext, src_ext, sizeof(struct bmap_rb_extent));

		dest_बुहोल = &dest_ext->बुहोल;
		n = &dest_bp->उगात.rb_बुहोल;

		dest_last = NULL;
		if (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->उगात);
			n = &(dest_last)->rb_कानान;
		}

		ext2fs_rb_link_बुहोल(dest_बुहोल, dest_last, n);
		ext2fs_rb_इसिंगित_color(dest_बुहोल, &dest_bp->उगात);

		src_बुहोल = ext2fs_rb_next(src_बुहोल);
	}

	return retval;
}

static void rb_truncate(__u64 new_max, struct rb_उगात *उगात)
{
	struct bmap_rb_extent *ext;
	struct rb_बुहोल *बुहोल;

	बुहोल = ext2fs_rb_last(उगात);
	while (बुहोल) {
		ext = बुहोल_to_extent(बुहोल);

		if ((ext->start + ext->बिलांग - 1) <= new_max)
			break;
		else if (ext->start > new_max) {
			ext2fs_rb_erase(बुहोल, उगात);
			ext2fs_free_mem(&ext);
			बुहोल = ext2fs_rb_last(उगात);
			continue;
		} else
			ext->बिलांग = new_max - ext->start + 1;
	}
}

static errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bmap->private;
	bp->rcursor = NULL;
	bp->wcursor = NULL;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->उगात);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	if (bmap->end < bmap->real_end)
		rb_इसिंगित_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	return 0;

}

inline static int
rb_test_bit(struct ext2fs_rb_private *bp, __u64 bit)
{
	struct bmap_rb_extent *rcursor, *next_ext = NULL;
	struct rb_बुहोल *parent = NULL, *next;
	struct rb_बुहोल **n = &bp->उगात.rb_बुहोल;
	struct bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	if (!rcursor)
		goto हानाप_पुनो;

	if (bit >= rcursor->start && bit < rcursor->start + rcursor->बिलांग) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		return 1;
	}

	next_ext = bp->rcursor_next;
	if (!next_ext) {
		next = ext2fs_rb_next(&rcursor->बुहोल);
		if (next)
			next_ext = बुहोल_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	if (next_ext) {
		if ((bit >= rcursor->start + rcursor->बिलांग) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			return 0;
		}
	}
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;

	rcursor = bp->wcursor;
	if (!rcursor)
		goto हानाप_पुनो;

	if (bit >= rcursor->start && bit < rcursor->start + rcursor->बिलांग)
		return 1;

हानाप_पुनो:

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (bit < ext->start)
			n = &(*n)->rb_कालिवा;
		else if (bit >= (ext->start + ext->बिलांग))
			n = &(*n)->rb_कानान;
		else {
			bp->rcursor = ext;
			bp->rcursor_next = NULL;
			return 1;
		}
	}
	return 0;
}

static int rb_इसिंगित_extent(__u64 start, __u64 बिलांग,
			    struct ext2fs_rb_private *bp)
{
	struct rb_उगात *उगात = &bp->उगात;
	struct rb_बुहोल *parent = NULL, **n = &उगात->rb_बुहोल;
	struct rb_बुहोल *new_बुहोल, *बुहोल, *next;
	struct bmap_rb_extent *new_ext;
	struct bmap_rb_extent *ext;
	int retval = 0;

	if (बिलांग == 0)
		return 0;

	bp->rcursor_next = NULL;
	ext = bp->wcursor;
	if (ext) {
		if (start >= ext->start &&
		    start <= (ext->start + ext->बिलांग)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			goto got_extent;
		}
	}

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);

		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
		} else if (start > (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
		} else {
got_extent:
			if ((start + बिलांग) <= (ext->start + ext->बिलांग))
				return 1;

			if ((ext->start + ext->बिलांग) == start)
				retval = 0;
			else
				retval = 1;

			बिलांग += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_बुहोल = &ext->बुहोल;

			goto skip_इसिंगित;
		}
	}

	rb_get_new_extent(&new_ext, start, बिलांग);

	new_बुहोल = &new_ext->बुहोल;
	ext2fs_rb_link_बुहोल(new_बुहोल, parent, n);
	ext2fs_rb_इसिंगित_color(new_बुहोल, उगात);
	bp->wcursor = new_ext;

	बुहोल = ext2fs_rb_prev(new_बुहोल);
	if (बुहोल) {
		ext = बुहोल_to_extent(बुहोल);
		if ((ext->start + ext->बिलांग) == start) {
			start = ext->start;
			बिलांग += ext->बिलांग;
			ext2fs_rb_erase(बुहोल, उगात);
			rb_free_extent(bp, ext);
		}
	}

skip_इसिंगित:
	/* See if we can merge extent to the right */
	for (बुहोल = ext2fs_rb_next(new_बुहोल); बुहोल != NULL; बुहोल = next) {
		next = ext2fs_rb_next(बुहोल);
		ext = बुहोल_to_extent(बुहोल);

		if ((ext->start + ext->बिलांग) <= start)
			continue;

		/* No more merging */
		if ((start + बिलांग) < ext->start)
			break;

		/* ext is embedded in new_ext interval */
		if ((start + बिलांग) >= (ext->start + ext->बिलांग)) {
			ext2fs_rb_erase(बुहोल, उगात);
			rb_free_extent(bp, ext);
			continue;
		} else {
		/* merge ext with new_ext */
			बिलांग += ((ext->start + ext->बिलांग) -
				  (start + बिलांग));
			ext2fs_rb_erase(बुहोल, उगात);
			rb_free_extent(bp, ext);
			break;
		}
	}

	new_ext->start = start;
	new_ext->बिलांग = बिलांग;

	return retval;
}

static int rb_remove_extent(__u64 start, __u64 बिलांग,
			    struct ext2fs_rb_private *bp)
{
	struct rb_उगात *उगात = &bp->उगात;
	struct rb_बुहोल *parent = NULL, **n = &उगात->rb_बुहोल;
	struct rb_बुहोल *बुहोल;
	struct bmap_rb_extent *ext;
	__u64 new_start, new_बिलांग;
	int retval = 0;

	if (ext2fs_rb_empty_उगात(उगात))
		return 0;

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
			continue;
		} else if (start >= (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
			continue;
		}

		if ((start > ext->start) &&
		    (start + बिलांग) < (ext->start + ext->बिलांग)) {
			/* We have to split extent into two */
			new_start = start + बिलांग;
			new_बिलांग = (ext->start + ext->बिलांग) - new_start;

			ext->बिलांग = start - ext->start;

			rb_इसिंगित_extent(new_start, new_बिलांग, bp);
			return 1;
		}

		if ((start + बिलांग) >= (ext->start + ext->बिलांग)) {
			ext->बिलांग = start - ext->start;
			retval = 1;
		}

		if (0 == ext->बिलांग) {
			parent = ext2fs_rb_next(&ext->बुहोल);
			ext2fs_rb_erase(&ext->बुहोल, उगात);
			rb_free_extent(bp, ext);
			break;
		}

		if (start == ext->start) {
			ext->start += बिलांग;
			ext->बिलांग -= बिलांग;
			return 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	for (; parent != NULL; parent = बुहोल) {
		बुहोल = ext2fs_rb_next(parent);
		ext = बुहोल_to_extent(parent);
		if ((ext->start + ext->बिलांग) <= start)
			continue;

		/* No more extents to be removed/truncated */
		if ((start + बिलांग) < ext->start)
			break;

		/* The entire extent is within the region to be removed */
		if ((start + बिलांग) >= (ext->start + ext->बिलांग)) {
			ext2fs_rb_erase(parent, उगात);
			rb_free_extent(bp, ext);
			retval = 1;
			continue;
		} else {
			/* modify the last extent in region to be removed */
			ext->बिलांग -= ((start + बिलांग) - ext->start);
			ext->start = start + बिलांग;
			retval = 1;
			break;
		}
	}

	return retval;
}

static int rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	int retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_इसिंगित_extent(arg, 1, bp);
	check_पुनो(&bp->उगात, __func__);
	return retval;
}

static int rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	int retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_पुनो(&bp->उगात, __func__);

	return retval;
}

inline
static int rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	return rb_test_bit(bp, arg);
}

static void rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned int नुमेरो)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_इसिंगित_extent(arg, नुमेरो, bp);
	check_पुनो(&bp->उगात, __func__);
}

static void rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned int नुमेरो)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_remove_extent(arg, नुमेरो, bp);
	check_पुनो(&bp->उगात, __func__);
}

static int rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned int len)
{
	struct rb_बुहोल *parent = NULL, **n;
	struct rb_बुहोल *बुहोल, *next;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	int retval = 1;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->उगात.rb_बुहोल;
	start -= bitmap->start;

	if (len == 0 || ext2fs_rb_empty_उगात(&bp->उगात))
		return 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
		} else if (start >= (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
		} else {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			return 0;
		}
	}

	बुहोल = parent;
	while (बुहोल) {
		next = ext2fs_rb_next(बुहोल);
		ext = बुहोल_to_extent(बुहोल);
		बुहोल = next;

		if ((ext->start + ext->बिलांग) <= start)
			continue;

		/* No more merging */
		if ((start + len) <= ext->start)
			break;

		retval = 0;
		break;
	}
	return retval;
}

static errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t नुमेरो, void *in)
{
	struct ext2fs_rb_private *bp;
	unsigned char *cp = in;
	size_t i;
	int first_set = -1;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	for (i = 0; i < नुमेरो; i++) {
		if ((i & 7) == 0) {
			unsigned char c = cp[i/8];
			if (c == 0xFF) {
				if (first_set == -1)
					first_set = i;
				i += 7;
				continue;
			}
			if ((c == 0x00) && (first_set == -1)) {
				i += 7;
				continue;
			}
		}
		if (ext2fs_test_bit(i, in)) {
			if (first_set == -1)
				first_set = i;
			continue;
		}
		if (first_set == -1)
			continue;

		rb_इसिंगित_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_पुनो(&bp->उगात, __func__);
		first_set = -1;
	}
	if (first_set != -1) {
		rb_इसिंगित_extent(start + first_set - bitmap->start,
				 नुमेरो - first_set, bp);
		check_पुनो(&bp->उगात, __func__);
	}

	return 0;
}

static errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t नुमेरो, void *out)
{

	struct rb_बुहोल *parent = NULL, *next, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	__u64 बिलांग, pos;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->उगात.rb_बुहोल;
	start -= bitmap->start;

	if (ext2fs_rb_empty_उगात(&bp->उगात))
		return 0;

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
		} else if (start >= (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
		} else
			break;
	}

	memset(out, 0, (नुमेरो + 7) >> 3);

	for (; parent != NULL; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = बुहोल_to_extent(parent);

		pos = ext->start;
		बिलांग = ext->बिलांग;
		if (pos >= start + नुमेरो)
			break;
		if (pos < start) {
			if (pos + बिलांग <  start)
				continue;
			बिलांग -= start - pos;
			pos = start;
		}
		if (pos + बिलांग > start + नुमेरो)
			बिलांग = start + नुमेरो - pos;

		while (बिलांग > 0) {
			if ((बिलांग >= 8) &&
			    ((pos - start) % 8) == 0) {
				int nbytes = बिलांग >> 3;
				int offset = (pos - start) >> 3;

				memset(((char *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				बिलांग -= nbytes << 3;
				continue;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			बिलांग--;
		}
	}
	return 0;
}

static void rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_पुनो(&bp->उगात);
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;
	check_पुनो(&bp->उगात, __func__);
}

static errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_बुहोल *parent = NULL, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->उगात.rb_बुहोल;
	start -= bitmap->start;
	end -= bitmap->start;

	if (start > end)
		return EINVAL;

	if (ext2fs_rb_empty_उगात(&bp->उगात))
		return ENOENT;

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
		} else if (start >= (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
		} else if (ext->start + ext->बिलांग <= end) {
			*out = ext->start + ext->बिलांग + bitmap->start;
			return 0;
		} else
			return ENOENT;
	}

	*out = start + bitmap->start;
	return 0;
}

static errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_बुहोल *parent = NULL, **n;
	struct rb_बुहोल *बुहोल;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->उगात.rb_बुहोल;
	start -= bitmap->start;
	end -= bitmap->start;

	if (start > end)
		return EINVAL;

	if (ext2fs_rb_empty_उगात(&bp->उगात))
		return ENOENT;

	while (*n) {
		parent = *n;
		ext = बुहोल_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_कालिवा;
		} else if (start >= (ext->start + ext->बिलांग)) {
			n = &(*n)->rb_कानान;
		} else {
			/* The start bit is set */
			*out = start + bitmap->start;
			return 0;
		}
	}

	बुहोल = parent;
	ext = बुहोल_to_extent(बुहोल);
	if (ext->start < start) {
		बुहोल = ext2fs_rb_next(बुहोल);
		if (बुहोल == NULL)
			return ENOENT;
		ext = बुहोल_to_extent(बुहोल);
	}
	if (ext->start <= end) {
		*out = ext->start + bitmap->start;
		return 0;
	}
	return ENOENT;
}

#ifdef ENABLE_BMAP_STATS
static void rb_इलिम्बाग_stats(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;
	struct rb_बुहोल *बुहोल = NULL;
	struct bmap_rb_extent *ext;
	__u64 बिलांग = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	double eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	double m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (struct ext2fs_rb_private *) bitmap->private;

	for (बुहोल = ext2fs_rb_first(&bp->उगात); बुहोल != NULL;
	     बुहोल = ext2fs_rb_next(बुहोल)) {
		ext = बुहोल_to_extent(बुहोल);
		बिलांग++;
		if (ext->बिलांग > max_size)
			max_size = ext->बिलांग;
		if (ext->बिलांग < min_size)
			min_size = ext->बिलांग;
		size += ext->बिलांग;
	}

	if (बिलांग)
		avg_size = size / बिलांग;
	if (min_size == ULONG_MAX)
		min_size = 0;
	eff = (double)((बिलांग * sizeof(struct bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_बिलांग + bitmap->stats.mark_ext_बिलांग;
	test_all = bitmap->stats.test_बिलांग + bitmap->stats.test_ext_बिलांग;
	if (mark_all)
		m_hit = ((double)bp->mark_hit / mark_all) * 100;
	if (test_all)
		t_hit = ((double)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) बिलांग, (unsigned long long)
		((बिलांग * sizeof(struct bmap_rb_extent)) +
		 sizeof(struct ext2fs_rb_private)));
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
#else
static void rb_इलिम्बाग_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

struct ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.इलिम्बाग_stats = rb_इलिम्बाग_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};
