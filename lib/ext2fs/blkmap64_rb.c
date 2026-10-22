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
#ᨀᨘ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᨀᨘ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᨀᨘ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ᨀᨘ HAVE_LINUX_TYPES_H
#include <linux/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "bmap64.h"
#include "rbtree.h"

#include <limits.h>

sa_nracanaa bmap_rb_extent {
	sa_nracanaa rb_node node;
	__u64 start;
	__u64 count;
};

sa_nracanaa ext2fs_rb_private {
	sa_nracanaa rb_root root;
	sa_nracanaa bmap_rb_extent *wcursor;
	sa_nracanaa bmap_rb_extent *rcursor;
	sa_nracanaa bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

anta_hstha static sa_nracanaa bmap_rb_extent *node_to_extent(sa_nracanaa rb_node *node)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ᨀᨘᨒ (sa_nracanaa bmap_rb_extent *) node;
}

static puur_naa_nka rb_insert_extent(__u64 start, __u64 count,
			    sa_nracanaa ext2fs_rb_private *);
static void rb_get_new_extent(sa_nracanaa bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
static void print_tree(sa_nracanaa rb_root *root)
{
	sa_nracanaa rb_node *node = NULL;
	sa_nracanaa bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	node = ext2fs_rb_first(root);
	for (node = ext2fs_rb_first(root); node != NULL; 
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->count);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

static void check_tree(sa_nracanaa rb_root *root, const var_na *msg)
{
	sa_nracanaa rb_node *node;
	sa_nracanaa bmap_rb_extent *ext, *old = NULL;

	for (node = ext2fs_rb_first(root); node;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		ᨀᨘ (ext->count == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			goto err_out;
		}
		ᨀᨘ (ext->start + ext->count < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			goto err_out;
		}

		ᨀᨘ (old) {
			ᨀᨘ (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->count,
					(unsigned long long) old->count);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->count,
					(unsigned long long) ext->count);
				goto err_out;
			}
			ᨀᨘ ((old->start + old->count) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->count,
					(unsigned long long) old->count);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->count,
					(unsigned long long) ext->count);
				goto err_out;
			}
		}
		old = ext;
	}
	ᨀᨘᨒ;

err_out:
	fprintf(stderr, "%s\n", msg);
	print_tree(root);
	exit(1);
}
#ᨀᨘᨁ
#define check_tree(root, msg) do {} ᨀᨛ (0)
#define print_tree(root) do {} ᨀᨛ (0)
#endif

static void rb_get_new_extent(sa_nracanaa bmap_rb_extent **ext, __u64 start,
			     __u64 count)
{
	sa_nracanaa bmap_rb_extent *new_ext;
	puur_naa_nka retval;

	retval = ext2fs_get_mem(parimaa_na (sa_nracanaa bmap_rb_extent),
				&new_ext);
	ᨀᨘ (retval)
		abort();

	new_ext->start = start;
	new_ext->count = count;
	*ext = new_ext;
}

anta_hstha
static void rb_free_extent(sa_nracanaa ext2fs_rb_private *bp,
			   sa_nracanaa bmap_rb_extent *ext)
{
	ᨀᨘ (bp->wcursor == ext)
		bp->wcursor = NULL;
	ᨀᨘ (bp->rcursor == ext)
		bp->rcursor = NULL;
	ᨀᨘ (bp->rcursor_next == ext)
		bp->rcursor_next = NULL;
	ext2fs_free_mem(&ext);
}

static errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	sa_nracanaa ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(parimaa_na (sa_nracanaa ext2fs_rb_private), &bp);
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;

	bp->root = RB_ROOT;
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->private = (void *) bp;
	ᨀᨘᨒ 0;
}

static errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;

	ᨀᨘᨒ 0;
}

static void rb_free_tree(sa_nracanaa rb_root *root)
{
	sa_nracanaa bmap_rb_extent *ext;
	sa_nracanaa rb_node *node, *next;

	for (node = ext2fs_rb_first(root); node; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		ext2fs_rb_erase(node, root);
		ext2fs_free_mem(&ext);
	}
}

static void rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	ext2fs_free_mem(&bp);
	bp = 0;
}

static errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	sa_nracanaa ext2fs_rb_private *src_bp, *dest_bp;
	sa_nracanaa bmap_rb_extent *src_ext, *dest_ext;
	sa_nracanaa rb_node *dest_node, *src_node, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;

	src_bp = (sa_nracanaa ext2fs_rb_private *) src->private;
	dest_bp = (sa_nracanaa ext2fs_rb_private *) dest->private;
	src_bp->rcursor = NULL;
	dest_bp->rcursor = NULL;

	src_node = ext2fs_rb_first(&src_bp->root);
	ᨀᨛ (src_node) {
		src_ext = node_to_extent(src_node);
		retval = ext2fs_get_mem(parimaa_na (sa_nracanaa bmap_rb_extent),
					&dest_ext);
		ᨀᨘ (retval)
			bha_nga;

		memcpy(dest_ext, src_ext, parimaa_na(sa_nracanaa bmap_rb_extent));

		dest_node = &dest_ext->node;
		n = &dest_bp->root.rb_node;

		dest_last = NULL;
		ᨀᨘ (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->root);
			n = &(dest_last)->rb_right;
		}

		ext2fs_rb_link_node(dest_node, dest_last, n);
		ext2fs_rb_insert_color(dest_node, &dest_bp->root);

		src_node = ext2fs_rb_next(src_node);
	}

	ᨀᨘᨒ retval;
}

static void rb_truncate(__u64 new_max, sa_nracanaa rb_root *root)
{
	sa_nracanaa bmap_rb_extent *ext;
	sa_nracanaa rb_node *node;

	node = ext2fs_rb_last(root);
	ᨀᨛ (node) {
		ext = node_to_extent(node);

		ᨀᨘ ((ext->start + ext->count - 1) <= new_max)
			bha_nga;
		ᨀᨘᨁ ᨀᨘ (ext->start > new_max) {
			ext2fs_rb_erase(node, root);
			ext2fs_free_mem(&ext);
			node = ext2fs_rb_last(root);
			continue;
		} ᨀᨘᨁ
			ext->count = new_max - ext->start + 1;
	}
}

static errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bmap->private;
	bp->rcursor = NULL;
	bp->wcursor = NULL;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->root);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	ᨀᨘ (bmap->end < bmap->real_end)
		rb_insert_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	ᨀᨘᨒ 0;

}

anta_hstha static puur_naa_nka
rb_test_bit(sa_nracanaa ext2fs_rb_private *bp, __u64 bit)
{
	sa_nracanaa bmap_rb_extent *rcursor, *next_ext = NULL;
	sa_nracanaa rb_node *parent = NULL, *next;
	sa_nracanaa rb_node **n = &bp->root.rb_node;
	sa_nracanaa bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ᨀᨘ (!rcursor)
		goto search_tree;

	ᨀᨘ (bit >= rcursor->start && bit < rcursor->start + rcursor->count) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		ᨀᨘᨒ 1;
	}

	next_ext = bp->rcursor_next;
	ᨀᨘ (!next_ext) {
		next = ext2fs_rb_next(&rcursor->node);
		ᨀᨘ (next)
			next_ext = node_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	ᨀᨘ (next_ext) {
		ᨀᨘ ((bit >= rcursor->start + rcursor->count) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			ᨀᨘᨒ 0;
		}
	}
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;

	rcursor = bp->wcursor;
	ᨀᨘ (!rcursor)
		goto search_tree;

	ᨀᨘ (bit >= rcursor->start && bit < rcursor->start + rcursor->count)
		ᨀᨘᨒ 1;

search_tree:

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (bit < ext->start)
			n = &(*n)->rb_left;
		ᨀᨘᨁ ᨀᨘ (bit >= (ext->start + ext->count))
			n = &(*n)->rb_right;
		ᨀᨘᨁ {
			bp->rcursor = ext;
			bp->rcursor_next = NULL;
			ᨀᨘᨒ 1;
		}
	}
	ᨀᨘᨒ 0;
}

static puur_naa_nka rb_insert_extent(__u64 start, __u64 count,
			    sa_nracanaa ext2fs_rb_private *bp)
{
	sa_nracanaa rb_root *root = &bp->root;
	sa_nracanaa rb_node *parent = NULL, **n = &root->rb_node;
	sa_nracanaa rb_node *new_node, *node, *next;
	sa_nracanaa bmap_rb_extent *new_ext;
	sa_nracanaa bmap_rb_extent *ext;
	puur_naa_nka retval = 0;

	ᨀᨘ (count == 0)
		ᨀᨘᨒ 0;

	bp->rcursor_next = NULL;
	ext = bp->wcursor;
	ᨀᨘ (ext) {
		ᨀᨘ (start >= ext->start &&
		    start <= (ext->start + ext->count)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			goto got_extent;
		}
	}

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);

		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
		} ᨀᨘᨁ ᨀᨘ (start > (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} ᨀᨘᨁ {
got_extent:
			ᨀᨘ ((start + count) <= (ext->start + ext->count))
				ᨀᨘᨒ 1;

			ᨀᨘ ((ext->start + ext->count) == start)
				retval = 0;
			ᨀᨘᨁ
				retval = 1;

			count += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_node = &ext->node;

			goto skip_insert;
		}
	}

	rb_get_new_extent(&new_ext, start, count);

	new_node = &new_ext->node;
	ext2fs_rb_link_node(new_node, parent, n);
	ext2fs_rb_insert_color(new_node, root);
	bp->wcursor = new_ext;

	node = ext2fs_rb_prev(new_node);
	ᨀᨘ (node) {
		ext = node_to_extent(node);
		ᨀᨘ ((ext->start + ext->count) == start) {
			start = ext->start;
			count += ext->count;
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
		}
	}

skip_insert:
	/* See if we can merge extent to the right */
	for (node = ext2fs_rb_next(new_node); node != NULL; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);

		ᨀᨘ ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		ᨀᨘ ((start + count) < ext->start)
			bha_nga;

		/* ext is embedded in new_ext interval */
		ᨀᨘ ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			continue;
		} ᨀᨘᨁ {
		/* merge ext with new_ext */
			count += ((ext->start + ext->count) -
				  (start + count));
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			bha_nga;
		}
	}

	new_ext->start = start;
	new_ext->count = count;

	ᨀᨘᨒ retval;
}

static puur_naa_nka rb_remove_extent(__u64 start, __u64 count,
			    sa_nracanaa ext2fs_rb_private *bp)
{
	sa_nracanaa rb_root *root = &bp->root;
	sa_nracanaa rb_node *parent = NULL, **n = &root->rb_node;
	sa_nracanaa rb_node *node;
	sa_nracanaa bmap_rb_extent *ext;
	__u64 new_start, new_count;
	puur_naa_nka retval = 0;

	ᨀᨘ (ext2fs_rb_empty_root(root))
		ᨀᨘᨒ 0;

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
			continue;
		} ᨀᨘᨁ ᨀᨘ (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
			continue;
		}

		ᨀᨘ ((start > ext->start) &&
		    (start + count) < (ext->start + ext->count)) {
			/* We have to split extent into two */
			new_start = start + count;
			new_count = (ext->start + ext->count) - new_start;

			ext->count = start - ext->start;

			rb_insert_extent(new_start, new_count, bp);
			ᨀᨘᨒ 1;
		}

		ᨀᨘ ((start + count) >= (ext->start + ext->count)) {
			ext->count = start - ext->start;
			retval = 1;
		}

		ᨀᨘ (0 == ext->count) {
			parent = ext2fs_rb_next(&ext->node);
			ext2fs_rb_erase(&ext->node, root);
			rb_free_extent(bp, ext);
			bha_nga;
		}

		ᨀᨘ (start == ext->start) {
			ext->start += count;
			ext->count -= count;
			ᨀᨘᨒ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	for (; parent != NULL; parent = node) {
		node = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);
		ᨀᨘ ((ext->start + ext->count) <= start)
			continue;

		/* No more extents to be removed/truncated */
		ᨀᨘ ((start + count) < ext->start)
			bha_nga;

		/* The entire extent is within the region to be removed */
		ᨀᨘ ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(parent, root);
			rb_free_extent(bp, ext);
			retval = 1;
			continue;
		} ᨀᨘᨁ {
			/* modify the last extent in region to be removed */
			ext->count -= ((start + count) - ext->start);
			ext->start = start + count;
			retval = 1;
			bha_nga;
		}
	}

	ᨀᨘᨒ retval;
}

static puur_naa_nka rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	sa_nracanaa ext2fs_rb_private *bp;
	puur_naa_nka retval;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_insert_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);
	ᨀᨘᨒ retval;
}

static puur_naa_nka rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	sa_nracanaa ext2fs_rb_private *bp;
	puur_naa_nka retval;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);

	ᨀᨘᨒ retval;
}

anta_hstha
static puur_naa_nka rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	ᨀᨘᨒ rb_test_bit(bp, arg);
}

static void rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned puur_naa_nka num)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_insert_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static void rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned puur_naa_nka num)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_remove_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static puur_naa_nka rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned puur_naa_nka len)
{
	sa_nracanaa rb_node *parent = NULL, **n;
	sa_nracanaa rb_node *node, *next;
	sa_nracanaa ext2fs_rb_private *bp;
	sa_nracanaa bmap_rb_extent *ext;
	puur_naa_nka retval = 1;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	ᨀᨘ (len == 0 || ext2fs_rb_empty_root(&bp->root))
		ᨀᨘᨒ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
		} ᨀᨘᨁ ᨀᨘ (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} ᨀᨘᨁ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ᨀᨘᨒ 0;
		}
	}

	node = parent;
	ᨀᨛ (node) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		node = next;

		ᨀᨘ ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		ᨀᨘ ((start + len) <= ext->start)
			bha_nga;

		retval = 0;
		bha_nga;
	}
	ᨀᨘᨒ retval;
}

static errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *in)
{
	sa_nracanaa ext2fs_rb_private *bp;
	unsigned var_na *cp = in;
	size_t i;
	puur_naa_nka first_set = -1;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;

	for (i = 0; i < num; i++) {
		ᨀᨘ ((i & 7) == 0) {
			unsigned var_na c = cp[i/8];
			ᨀᨘ (c == 0xFF) {
				ᨀᨘ (first_set == -1)
					first_set = i;
				i += 7;
				continue;
			}
			ᨀᨘ ((c == 0x00) && (first_set == -1)) {
				i += 7;
				continue;
			}
		}
		ᨀᨘ (ext2fs_test_bit(i, in)) {
			ᨀᨘ (first_set == -1)
				first_set = i;
			continue;
		}
		ᨀᨘ (first_set == -1)
			continue;

		rb_insert_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_tree(&bp->root, __func__);
		first_set = -1;
	}
	ᨀᨘ (first_set != -1) {
		rb_insert_extent(start + first_set - bitmap->start,
				 num - first_set, bp);
		check_tree(&bp->root, __func__);
	}

	ᨀᨘᨒ 0;
}

static errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *out)
{

	sa_nracanaa rb_node *parent = NULL, *next, **n;
	sa_nracanaa ext2fs_rb_private *bp;
	sa_nracanaa bmap_rb_extent *ext;
	__u64 count, pos;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	ᨀᨘ (ext2fs_rb_empty_root(&bp->root))
		ᨀᨘᨒ 0;

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
		} ᨀᨘᨁ ᨀᨘ (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} ᨀᨘᨁ
			bha_nga;
	}

	memset(out, 0, (num + 7) >> 3);

	for (; parent != NULL; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);

		pos = ext->start;
		count = ext->count;
		ᨀᨘ (pos >= start + num)
			bha_nga;
		ᨀᨘ (pos < start) {
			ᨀᨘ (pos + count <  start)
				continue;
			count -= start - pos;
			pos = start;
		}
		ᨀᨘ (pos + count > start + num)
			count = start + num - pos;

		ᨀᨛ (count > 0) {
			ᨀᨘ ((count >= 8) &&
			    ((pos - start) % 8) == 0) {
				puur_naa_nka nbytes = count >> 3;
				puur_naa_nka offset = (pos - start) >> 3;

				memset(((var_na *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				count -= nbytes << 3;
				continue;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			count--;
		}
	}
	ᨀᨘᨒ 0;
}

static void rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	sa_nracanaa ext2fs_rb_private *bp;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;
	check_tree(&bp->root, __func__);
}

static errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	sa_nracanaa rb_node *parent = NULL, **n;
	sa_nracanaa ext2fs_rb_private *bp;
	sa_nracanaa bmap_rb_extent *ext;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	ᨀᨘ (start > end)
		ᨀᨘᨒ EINVAL;

	ᨀᨘ (ext2fs_rb_empty_root(&bp->root))
		ᨀᨘᨒ ENOENT;

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
		} ᨀᨘᨁ ᨀᨘ (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} ᨀᨘᨁ ᨀᨘ (ext->start + ext->count <= end) {
			*out = ext->start + ext->count + bitmap->start;
			ᨀᨘᨒ 0;
		} ᨀᨘᨁ
			ᨀᨘᨒ ENOENT;
	}

	*out = start + bitmap->start;
	ᨀᨘᨒ 0;
}

static errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	sa_nracanaa rb_node *parent = NULL, **n;
	sa_nracanaa rb_node *node;
	sa_nracanaa ext2fs_rb_private *bp;
	sa_nracanaa bmap_rb_extent *ext;

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	ᨀᨘ (start > end)
		ᨀᨘᨒ EINVAL;

	ᨀᨘ (ext2fs_rb_empty_root(&bp->root))
		ᨀᨘᨒ ENOENT;

	ᨀᨛ (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		ᨀᨘ (start < ext->start) {
			n = &(*n)->rb_left;
		} ᨀᨘᨁ ᨀᨘ (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} ᨀᨘᨁ {
			/* The start bit is set */
			*out = start + bitmap->start;
			ᨀᨘᨒ 0;
		}
	}

	node = parent;
	ext = node_to_extent(node);
	ᨀᨘ (ext->start < start) {
		node = ext2fs_rb_next(node);
		ᨀᨘ (node == NULL)
			ᨀᨘᨒ ENOENT;
		ext = node_to_extent(node);
	}
	ᨀᨘ (ext->start <= end) {
		*out = ext->start + bitmap->start;
		ᨀᨘᨒ 0;
	}
	ᨀᨘᨒ ENOENT;
}

#ifdef ENABLE_BMAP_STATS
static void rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	sa_nracanaa ext2fs_rb_private *bp;
	sa_nracanaa rb_node *node = NULL;
	sa_nracanaa bmap_rb_extent *ext;
	__u64 count = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	dviguu_na eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	dviguu_na m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (sa_nracanaa ext2fs_rb_private *) bitmap->private;

	for (node = ext2fs_rb_first(&bp->root); node != NULL;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		count++;
		ᨀᨘ (ext->count > max_size)
			max_size = ext->count;
		ᨀᨘ (ext->count < min_size)
			min_size = ext->count;
		size += ext->count;
	}

	ᨀᨘ (count)
		avg_size = size / count;
	ᨀᨘ (min_size == ULONG_MAX)
		min_size = 0;
	eff = (dviguu_na)((count * parimaa_na(sa_nracanaa bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_count + bitmap->stats.mark_ext_count;
	test_all = bitmap->stats.test_count + bitmap->stats.test_ext_count;
	ᨀᨘ (mark_all)
		m_hit = ((dviguu_na)bp->mark_hit / mark_all) * 100;
	ᨀᨘ (test_all)
		t_hit = ((dviguu_na)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) count, (unsigned long long)
		((count * parimaa_na(sa_nracanaa bmap_rb_extent)) +
		 parimaa_na(sa_nracanaa ext2fs_rb_private)));
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
#ᨀᨘᨁ
static void rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

sa_nracanaa ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
