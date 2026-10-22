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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#nếu HAVE_LINUX_TYPES_H
#include <linux/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "bmap64.h"
#include "rbtree.h"

#include <limits.h>

struct bmap_rb_extent {
	struct rb_node node;
	__u64 start;
	__u64 count;
};

struct ext2fs_rb_private {
	struct rb_root root;
	struct bmap_rb_extent *wcursor;
	struct bmap_rb_extent *rcursor;
	struct bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

inline static struct bmap_rb_extent *node_to_extent(struct rb_node *node)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	trả_về (struct bmap_rb_extent *) node;
}

static số_nguyên rb_insert_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *);
static void rb_get_new_extent(struct bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
static void print_tree(struct rb_root *root)
{
	struct rb_node *node = NULL;
	struct bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	node = ext2fs_rb_first(root);
	cho (node = ext2fs_rb_first(root); node != NULL; 
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->count);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

static void check_tree(struct rb_root *root, const char *msg)
{
	struct rb_node *node;
	struct bmap_rb_extent *ext, *old = NULL;

	cho (node = ext2fs_rb_first(root); node;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		nếu (ext->count == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			goto err_out;
		}
		nếu (ext->start + ext->count < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			goto err_out;
		}

		nếu (old) {
			nếu (old->start > ext->start) {
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
			nếu ((old->start + old->count) >= ext->start) {
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
	trả_về;

err_out:
	fprintf(stderr, "%s\n", msg);
	print_tree(root);
	exit(1);
}
#không_thì
#define check_tree(root, msg) do {} trong_khi (0)
#define print_tree(root) do {} trong_khi (0)
#endif

static void rb_get_new_extent(struct bmap_rb_extent **ext, __u64 start,
			     __u64 count)
{
	struct bmap_rb_extent *new_ext;
	số_nguyên retval;

	retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
				&new_ext);
	nếu (retval)
		abort();

	new_ext->start = start;
	new_ext->count = count;
	*ext = new_ext;
}

inline
static void rb_free_extent(struct ext2fs_rb_private *bp,
			   struct bmap_rb_extent *ext)
{
	nếu (bp->wcursor == ext)
		bp->wcursor = NULL;
	nếu (bp->rcursor == ext)
		bp->rcursor = NULL;
	nếu (bp->rcursor_next == ext)
		bp->rcursor_next = NULL;
	ext2fs_free_mem(&ext);
}

static errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof (struct ext2fs_rb_private), &bp);
	nếu (retval)
		trả_về retval;

	bp->root = RB_ROOT;
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->private = (void *) bp;
	trả_về 0;
}

static errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	nếu (retval)
		trả_về retval;

	trả_về 0;
}

static void rb_free_tree(struct rb_root *root)
{
	struct bmap_rb_extent *ext;
	struct rb_node *node, *next;

	cho (node = ext2fs_rb_first(root); node; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		ext2fs_rb_erase(node, root);
		ext2fs_free_mem(&ext);
	}
}

static void rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	ext2fs_free_mem(&bp);
	bp = 0;
}

static errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	struct ext2fs_rb_private *src_bp, *dest_bp;
	struct bmap_rb_extent *src_ext, *dest_ext;
	struct rb_node *dest_node, *src_node, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	nếu (retval)
		trả_về retval;

	src_bp = (struct ext2fs_rb_private *) src->private;
	dest_bp = (struct ext2fs_rb_private *) dest->private;
	src_bp->rcursor = NULL;
	dest_bp->rcursor = NULL;

	src_node = ext2fs_rb_first(&src_bp->root);
	trong_khi (src_node) {
		src_ext = node_to_extent(src_node);
		retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
					&dest_ext);
		nếu (retval)
			break;

		memcpy(dest_ext, src_ext, sizeof(struct bmap_rb_extent));

		dest_node = &dest_ext->node;
		n = &dest_bp->root.rb_node;

		dest_last = NULL;
		nếu (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->root);
			n = &(dest_last)->rb_right;
		}

		ext2fs_rb_link_node(dest_node, dest_last, n);
		ext2fs_rb_insert_color(dest_node, &dest_bp->root);

		src_node = ext2fs_rb_next(src_node);
	}

	trả_về retval;
}

static void rb_truncate(__u64 new_max, struct rb_root *root)
{
	struct bmap_rb_extent *ext;
	struct rb_node *node;

	node = ext2fs_rb_last(root);
	trong_khi (node) {
		ext = node_to_extent(node);

		nếu ((ext->start + ext->count - 1) <= new_max)
			break;
		không_thì nếu (ext->start > new_max) {
			ext2fs_rb_erase(node, root);
			ext2fs_free_mem(&ext);
			node = ext2fs_rb_last(root);
			continue;
		} không_thì
			ext->count = new_max - ext->start + 1;
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
		    &bp->root);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	nếu (bmap->end < bmap->real_end)
		rb_insert_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	trả_về 0;

}

inline static số_nguyên
rb_test_bit(struct ext2fs_rb_private *bp, __u64 bit)
{
	struct bmap_rb_extent *rcursor, *next_ext = NULL;
	struct rb_node *parent = NULL, *next;
	struct rb_node **n = &bp->root.rb_node;
	struct bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	nếu (!rcursor)
		goto search_tree;

	nếu (bit >= rcursor->start && bit < rcursor->start + rcursor->count) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		trả_về 1;
	}

	next_ext = bp->rcursor_next;
	nếu (!next_ext) {
		next = ext2fs_rb_next(&rcursor->node);
		nếu (next)
			next_ext = node_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	nếu (next_ext) {
		nếu ((bit >= rcursor->start + rcursor->count) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			trả_về 0;
		}
	}
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;

	rcursor = bp->wcursor;
	nếu (!rcursor)
		goto search_tree;

	nếu (bit >= rcursor->start && bit < rcursor->start + rcursor->count)
		trả_về 1;

search_tree:

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (bit < ext->start)
			n = &(*n)->rb_left;
		không_thì nếu (bit >= (ext->start + ext->count))
			n = &(*n)->rb_right;
		không_thì {
			bp->rcursor = ext;
			bp->rcursor_next = NULL;
			trả_về 1;
		}
	}
	trả_về 0;
}

static số_nguyên rb_insert_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *bp)
{
	struct rb_root *root = &bp->root;
	struct rb_node *parent = NULL, **n = &root->rb_node;
	struct rb_node *new_node, *node, *next;
	struct bmap_rb_extent *new_ext;
	struct bmap_rb_extent *ext;
	số_nguyên retval = 0;

	nếu (count == 0)
		trả_về 0;

	bp->rcursor_next = NULL;
	ext = bp->wcursor;
	nếu (ext) {
		nếu (start >= ext->start &&
		    start <= (ext->start + ext->count)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			goto got_extent;
		}
	}

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);

		nếu (start < ext->start) {
			n = &(*n)->rb_left;
		} không_thì nếu (start > (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} không_thì {
got_extent:
			nếu ((start + count) <= (ext->start + ext->count))
				trả_về 1;

			nếu ((ext->start + ext->count) == start)
				retval = 0;
			không_thì
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
	nếu (node) {
		ext = node_to_extent(node);
		nếu ((ext->start + ext->count) == start) {
			start = ext->start;
			count += ext->count;
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
		}
	}

skip_insert:
	/* See if we can merge extent to the right */
	cho (node = ext2fs_rb_next(new_node); node != NULL; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);

		nếu ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		nếu ((start + count) < ext->start)
			break;

		/* ext is embedded in new_ext interval */
		nếu ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			continue;
		} không_thì {
		/* merge ext with new_ext */
			count += ((ext->start + ext->count) -
				  (start + count));
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			break;
		}
	}

	new_ext->start = start;
	new_ext->count = count;

	trả_về retval;
}

static số_nguyên rb_remove_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *bp)
{
	struct rb_root *root = &bp->root;
	struct rb_node *parent = NULL, **n = &root->rb_node;
	struct rb_node *node;
	struct bmap_rb_extent *ext;
	__u64 new_start, new_count;
	số_nguyên retval = 0;

	nếu (ext2fs_rb_empty_root(root))
		trả_về 0;

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (start < ext->start) {
			n = &(*n)->rb_left;
			continue;
		} không_thì nếu (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
			continue;
		}

		nếu ((start > ext->start) &&
		    (start + count) < (ext->start + ext->count)) {
			/* We have to split extent into two */
			new_start = start + count;
			new_count = (ext->start + ext->count) - new_start;

			ext->count = start - ext->start;

			rb_insert_extent(new_start, new_count, bp);
			trả_về 1;
		}

		nếu ((start + count) >= (ext->start + ext->count)) {
			ext->count = start - ext->start;
			retval = 1;
		}

		nếu (0 == ext->count) {
			parent = ext2fs_rb_next(&ext->node);
			ext2fs_rb_erase(&ext->node, root);
			rb_free_extent(bp, ext);
			break;
		}

		nếu (start == ext->start) {
			ext->start += count;
			ext->count -= count;
			trả_về 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	cho (; parent != NULL; parent = node) {
		node = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);
		nếu ((ext->start + ext->count) <= start)
			continue;

		/* No more extents to be removed/truncated */
		nếu ((start + count) < ext->start)
			break;

		/* The entire extent is within the region to be removed */
		nếu ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(parent, root);
			rb_free_extent(bp, ext);
			retval = 1;
			continue;
		} không_thì {
			/* modify the last extent in region to be removed */
			ext->count -= ((start + count) - ext->start);
			ext->start = start + count;
			retval = 1;
			break;
		}
	}

	trả_về retval;
}

static số_nguyên rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	số_nguyên retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_insert_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);
	trả_về retval;
}

static số_nguyên rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	số_nguyên retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);

	trả_về retval;
}

inline
static số_nguyên rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	trả_về rb_test_bit(bp, arg);
}

static void rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned số_nguyên num)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_insert_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static void rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned số_nguyên num)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_remove_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static số_nguyên rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned số_nguyên len)
{
	struct rb_node *parent = NULL, **n;
	struct rb_node *node, *next;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	số_nguyên retval = 1;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	nếu (len == 0 || ext2fs_rb_empty_root(&bp->root))
		trả_về 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (start < ext->start) {
			n = &(*n)->rb_left;
		} không_thì nếu (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} không_thì {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			trả_về 0;
		}
	}

	node = parent;
	trong_khi (node) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		node = next;

		nếu ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		nếu ((start + len) <= ext->start)
			break;

		retval = 0;
		break;
	}
	trả_về retval;
}

static errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *in)
{
	struct ext2fs_rb_private *bp;
	unsigned char *cp = in;
	size_t i;
	số_nguyên first_set = -1;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	cho (i = 0; i < num; i++) {
		nếu ((i & 7) == 0) {
			unsigned char c = cp[i/8];
			nếu (c == 0xFF) {
				nếu (first_set == -1)
					first_set = i;
				i += 7;
				continue;
			}
			nếu ((c == 0x00) && (first_set == -1)) {
				i += 7;
				continue;
			}
		}
		nếu (ext2fs_test_bit(i, in)) {
			nếu (first_set == -1)
				first_set = i;
			continue;
		}
		nếu (first_set == -1)
			continue;

		rb_insert_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_tree(&bp->root, __func__);
		first_set = -1;
	}
	nếu (first_set != -1) {
		rb_insert_extent(start + first_set - bitmap->start,
				 num - first_set, bp);
		check_tree(&bp->root, __func__);
	}

	trả_về 0;
}

static errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *out)
{

	struct rb_node *parent = NULL, *next, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	__u64 count, pos;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	nếu (ext2fs_rb_empty_root(&bp->root))
		trả_về 0;

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (start < ext->start) {
			n = &(*n)->rb_left;
		} không_thì nếu (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} không_thì
			break;
	}

	memset(out, 0, (num + 7) >> 3);

	cho (; parent != NULL; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);

		pos = ext->start;
		count = ext->count;
		nếu (pos >= start + num)
			break;
		nếu (pos < start) {
			nếu (pos + count <  start)
				continue;
			count -= start - pos;
			pos = start;
		}
		nếu (pos + count > start + num)
			count = start + num - pos;

		trong_khi (count > 0) {
			nếu ((count >= 8) &&
			    ((pos - start) % 8) == 0) {
				số_nguyên nbytes = count >> 3;
				số_nguyên offset = (pos - start) >> 3;

				memset(((char *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				count -= nbytes << 3;
				continue;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			count--;
		}
	}
	trả_về 0;
}

static void rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;
	check_tree(&bp->root, __func__);
}

static errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_node *parent = NULL, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	nếu (start > end)
		trả_về EINVAL;

	nếu (ext2fs_rb_empty_root(&bp->root))
		trả_về ENOENT;

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (start < ext->start) {
			n = &(*n)->rb_left;
		} không_thì nếu (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} không_thì nếu (ext->start + ext->count <= end) {
			*out = ext->start + ext->count + bitmap->start;
			trả_về 0;
		} không_thì
			trả_về ENOENT;
	}

	*out = start + bitmap->start;
	trả_về 0;
}

static errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_node *parent = NULL, **n;
	struct rb_node *node;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	nếu (start > end)
		trả_về EINVAL;

	nếu (ext2fs_rb_empty_root(&bp->root))
		trả_về ENOENT;

	trong_khi (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		nếu (start < ext->start) {
			n = &(*n)->rb_left;
		} không_thì nếu (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} không_thì {
			/* The start bit is set */
			*out = start + bitmap->start;
			trả_về 0;
		}
	}

	node = parent;
	ext = node_to_extent(node);
	nếu (ext->start < start) {
		node = ext2fs_rb_next(node);
		nếu (node == NULL)
			trả_về ENOENT;
		ext = node_to_extent(node);
	}
	nếu (ext->start <= end) {
		*out = ext->start + bitmap->start;
		trả_về 0;
	}
	trả_về ENOENT;
}

#ifdef ENABLE_BMAP_STATS
static void rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	struct ext2fs_rb_private *bp;
	struct rb_node *node = NULL;
	struct bmap_rb_extent *ext;
	__u64 count = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	double eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	double m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (struct ext2fs_rb_private *) bitmap->private;

	cho (node = ext2fs_rb_first(&bp->root); node != NULL;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		count++;
		nếu (ext->count > max_size)
			max_size = ext->count;
		nếu (ext->count < min_size)
			min_size = ext->count;
		size += ext->count;
	}

	nếu (count)
		avg_size = size / count;
	nếu (min_size == ULONG_MAX)
		min_size = 0;
	eff = (double)((count * sizeof(struct bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_count + bitmap->stats.mark_ext_count;
	test_all = bitmap->stats.test_count + bitmap->stats.test_ext_count;
	nếu (mark_all)
		m_hit = ((double)bp->mark_hit / mark_all) * 100;
	nếu (test_all)
		t_hit = ((double)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) count, (unsigned long long)
		((count * sizeof(struct bmap_rb_extent)) +
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
#không_thì
static void rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
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
	.print_stats = rb_print_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};
