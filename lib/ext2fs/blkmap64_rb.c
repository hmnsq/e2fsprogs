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
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#𐤀𐤌 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#𐤀𐤌 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#𐤀𐤌 HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_node node;
	__u64 start;
	__u64 count;
};

بنية ext2fs_rb_private {
	بنية rb_root root;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

مضمن ساكن بنية bmap_rb_extent *node_to_extent(بنية rb_node *node)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) node;
}

ساكن صحيح rb_insert_extent(__u64 start, __u64 count,
			    بنية ext2fs_rb_private *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ساكن فراغ print_tree(بنية rb_root *root)
{
	بنية rb_node *node = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	node = ext2fs_rb_first(root);
	لكل (node = ext2fs_rb_first(root); node != عدم; 
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->count);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_tree(بنية rb_root *root, ثابت حرف *msg)
{
	بنية rb_node *node;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (node = ext2fs_rb_first(root); node;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		𐤀𐤌 (ext->count == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			اذهب err_out;
		}
		𐤀𐤌 (ext->start + ext->count < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->count,
				(unsigned long long) ext->count);
			اذهب err_out;
		}

		𐤀𐤌 (old) {
			𐤀𐤌 (old->start > ext->start) {
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
				اذهب err_out;
			}
			𐤀𐤌 ((old->start + old->count) >= ext->start) {
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
				اذهب err_out;
			}
		}
		old = ext;
	}
	أرجع;

err_out:
	fprintf(stderr, "%s\n", msg);
	print_tree(root);
	exit(1);
}
#وإلا
#define check_tree(root, msg) 𐤐𐤏𐤋 {} طالما (0)
#define print_tree(root) 𐤐𐤏𐤋 {} طالما (0)
#endif

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 start,
			     __u64 count)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	𐤀𐤌 (retval)
		abort();

	new_ext->start = start;
	new_ext->count = count;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_private *bp,
			   بنية bmap_rb_extent *ext)
{
	𐤀𐤌 (bp->wcursor == ext)
		bp->wcursor = عدم;
	𐤀𐤌 (bp->rcursor == ext)
		bp->rcursor = عدم;
	𐤀𐤌 (bp->rcursor_next == ext)
		bp->rcursor_next = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_private), &bp);
	𐤀𐤌 (retval)
		أرجع retval;

	bp->root = RB_ROOT;
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
	𐤀𐤌 (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_tree(بنية rb_root *root)
{
	بنية bmap_rb_extent *ext;
	بنية rb_node *node, *next;

	لكل (node = ext2fs_rb_first(root); node; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		ext2fs_rb_erase(node, root);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_tree(&bp->root);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_private *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_node *dest_node, *src_node, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	𐤀𐤌 (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_private *) src->خاص;
	dest_bp = (بنية ext2fs_rb_private *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_node = ext2fs_rb_first(&src_bp->root);
	طالما (src_node) {
		src_ext = node_to_extent(src_node);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		𐤀𐤌 (retval)
			اكسر;

		memcpy(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_node = &dest_ext->node;
		n = &dest_bp->root.rb_node;

		dest_last = عدم;
		𐤀𐤌 (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->root);
			n = &(dest_last)->rb_right;
		}

		ext2fs_rb_link_node(dest_node, dest_last, n);
		ext2fs_rb_insert_color(dest_node, &dest_bp->root);

		src_node = ext2fs_rb_next(src_node);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_root *root)
{
	بنية bmap_rb_extent *ext;
	بنية rb_node *node;

	node = ext2fs_rb_last(root);
	طالما (node) {
		ext = node_to_extent(node);

		𐤀𐤌 ((ext->start + ext->count - 1) <= new_max)
			اكسر;
		وإلا 𐤀𐤌 (ext->start > new_max) {
			ext2fs_rb_erase(node, root);
			ext2fs_free_mem(&ext);
			node = ext2fs_rb_last(root);
			استمر;
		} وإلا
			ext->count = new_max - ext->start + 1;
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
		    &bp->root);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	𐤀𐤌 (bmap->end < bmap->real_end)
		rb_insert_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_bit(بنية ext2fs_rb_private *bp, __u64 bit)
{
	بنية bmap_rb_extent *rcursor, *next_ext = عدم;
	بنية rb_node *parent = عدم, *next;
	بنية rb_node **n = &bp->root.rb_node;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	𐤀𐤌 (!rcursor)
		اذهب search_tree;

	𐤀𐤌 (bit >= rcursor->start && bit < rcursor->start + rcursor->count) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		أرجع 1;
	}

	next_ext = bp->rcursor_next;
	𐤀𐤌 (!next_ext) {
		next = ext2fs_rb_next(&rcursor->node);
		𐤀𐤌 (next)
			next_ext = node_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	𐤀𐤌 (next_ext) {
		𐤀𐤌 ((bit >= rcursor->start + rcursor->count) &&
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
	𐤀𐤌 (!rcursor)
		اذهب search_tree;

	𐤀𐤌 (bit >= rcursor->start && bit < rcursor->start + rcursor->count)
		أرجع 1;

search_tree:

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (bit < ext->start)
			n = &(*n)->rb_left;
		وإلا 𐤀𐤌 (bit >= (ext->start + ext->count))
			n = &(*n)->rb_right;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_next = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_insert_extent(__u64 start, __u64 count,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_root *root = &bp->root;
	بنية rb_node *parent = عدم, **n = &root->rb_node;
	بنية rb_node *new_node, *node, *next;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	𐤀𐤌 (count == 0)
		أرجع 0;

	bp->rcursor_next = عدم;
	ext = bp->wcursor;
	𐤀𐤌 (ext) {
		𐤀𐤌 (start >= ext->start &&
		    start <= (ext->start + ext->count)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			اذهب got_extent;
		}
	}

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);

		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
		} وإلا 𐤀𐤌 (start > (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} وإلا {
got_extent:
			𐤀𐤌 ((start + count) <= (ext->start + ext->count))
				أرجع 1;

			𐤀𐤌 ((ext->start + ext->count) == start)
				retval = 0;
			وإلا
				retval = 1;

			count += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_node = &ext->node;

			اذهب skip_insert;
		}
	}

	rb_get_new_extent(&new_ext, start, count);

	new_node = &new_ext->node;
	ext2fs_rb_link_node(new_node, parent, n);
	ext2fs_rb_insert_color(new_node, root);
	bp->wcursor = new_ext;

	node = ext2fs_rb_prev(new_node);
	𐤀𐤌 (node) {
		ext = node_to_extent(node);
		𐤀𐤌 ((ext->start + ext->count) == start) {
			start = ext->start;
			count += ext->count;
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
		}
	}

skip_insert:
	/* See if we can merge extent to the right */
	لكل (node = ext2fs_rb_next(new_node); node != عدم; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);

		𐤀𐤌 ((ext->start + ext->count) <= start)
			استمر;

		/* No more merging */
		𐤀𐤌 ((start + count) < ext->start)
			اكسر;

		/* ext is embedded in new_ext interval */
		𐤀𐤌 ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			count += ((ext->start + ext->count) -
				  (start + count));
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->start = start;
	new_ext->count = count;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 start, __u64 count,
			    بنية ext2fs_rb_private *bp)
{
	بنية rb_root *root = &bp->root;
	بنية rb_node *parent = عدم, **n = &root->rb_node;
	بنية rb_node *node;
	بنية bmap_rb_extent *ext;
	__u64 new_start, new_count;
	صحيح retval = 0;

	𐤀𐤌 (ext2fs_rb_empty_root(root))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
			استمر;
		} وإلا 𐤀𐤌 (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
			استمر;
		}

		𐤀𐤌 ((start > ext->start) &&
		    (start + count) < (ext->start + ext->count)) {
			/* We have to split extent into two */
			new_start = start + count;
			new_count = (ext->start + ext->count) - new_start;

			ext->count = start - ext->start;

			rb_insert_extent(new_start, new_count, bp);
			أرجع 1;
		}

		𐤀𐤌 ((start + count) >= (ext->start + ext->count)) {
			ext->count = start - ext->start;
			retval = 1;
		}

		𐤀𐤌 (0 == ext->count) {
			parent = ext2fs_rb_next(&ext->node);
			ext2fs_rb_erase(&ext->node, root);
			rb_free_extent(bp, ext);
			اكسر;
		}

		𐤀𐤌 (start == ext->start) {
			ext->start += count;
			ext->count -= count;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; parent != عدم; parent = node) {
		node = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);
		𐤀𐤌 ((ext->start + ext->count) <= start)
			استمر;

		/* No more extents to be removed/truncated */
		𐤀𐤌 ((start + count) < ext->start)
			اكسر;

		/* The entire extent is within the region to be removed */
		𐤀𐤌 ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(parent, root);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->count -= ((start + count) - ext->start);
			ext->start = start + count;
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

	retval = rb_insert_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_private *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);

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
				unsigned صحيح num)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_insert_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح num)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	arg -= bitmap->start;

	rb_remove_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	بنية rb_node *parent = عدم, **n;
	بنية rb_node *node, *next;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	𐤀𐤌 (len == 0 || ext2fs_rb_empty_root(&bp->root))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
		} وإلا 𐤀𐤌 (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	node = parent;
	طالما (node) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		node = next;

		𐤀𐤌 ((ext->start + ext->count) <= start)
			استمر;

		/* No more merging */
		𐤀𐤌 ((start + len) <= ext->start)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, فراغ *in)
{
	بنية ext2fs_rb_private *bp;
	unsigned حرف *cp = in;
	size_t i;
	صحيح first_set = -1;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (i = 0; i < num; i++) {
		𐤀𐤌 ((i & 7) == 0) {
			unsigned حرف c = cp[i/8];
			𐤀𐤌 (c == 0xFF) {
				𐤀𐤌 (first_set == -1)
					first_set = i;
				i += 7;
				استمر;
			}
			𐤀𐤌 ((c == 0x00) && (first_set == -1)) {
				i += 7;
				استمر;
			}
		}
		𐤀𐤌 (ext2fs_test_bit(i, in)) {
			𐤀𐤌 (first_set == -1)
				first_set = i;
			استمر;
		}
		𐤀𐤌 (first_set == -1)
			استمر;

		rb_insert_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_tree(&bp->root, __func__);
		first_set = -1;
	}
	𐤀𐤌 (first_set != -1) {
		rb_insert_extent(start + first_set - bitmap->start,
				 num - first_set, bp);
		check_tree(&bp->root, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, فراغ *out)
{

	بنية rb_node *parent = عدم, *next, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;
	__u64 count, pos;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	𐤀𐤌 (ext2fs_rb_empty_root(&bp->root))
		أرجع 0;

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
		} وإلا 𐤀𐤌 (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} وإلا
			اكسر;
	}

	memset(out, 0, (num + 7) >> 3);

	لكل (; parent != عدم; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);

		pos = ext->start;
		count = ext->count;
		𐤀𐤌 (pos >= start + num)
			اكسر;
		𐤀𐤌 (pos < start) {
			𐤀𐤌 (pos + count <  start)
				استمر;
			count -= start - pos;
			pos = start;
		}
		𐤀𐤌 (pos + count > start + num)
			count = start + num - pos;

		طالما (count > 0) {
			𐤀𐤌 ((count >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحيح nbytes = count >> 3;
				صحيح offset = (pos - start) >> 3;

				memset(((حرف *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				count -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			count--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	rb_free_tree(&bp->root);
	bp->rcursor = عدم;
	bp->rcursor_next = عدم;
	bp->wcursor = عدم;
	check_tree(&bp->root, __func__);
}

ساكن errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	بنية rb_node *parent = عدم, **n;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	𐤀𐤌 (start > end)
		أرجع EINVAL;

	𐤀𐤌 (ext2fs_rb_empty_root(&bp->root))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
		} وإلا 𐤀𐤌 (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} وإلا 𐤀𐤌 (ext->start + ext->count <= end) {
			*out = ext->start + ext->count + bitmap->start;
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
	بنية rb_node *parent = عدم, **n;
	بنية rb_node *node;
	بنية ext2fs_rb_private *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	𐤀𐤌 (start > end)
		أرجع EINVAL;

	𐤀𐤌 (ext2fs_rb_empty_root(&bp->root))
		أرجع ENOENT;

	طالما (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		𐤀𐤌 (start < ext->start) {
			n = &(*n)->rb_left;
		} وإلا 𐤀𐤌 (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} وإلا {
			/* The start bit is set */
			*out = start + bitmap->start;
			أرجع 0;
		}
	}

	node = parent;
	ext = node_to_extent(node);
	𐤀𐤌 (ext->start < start) {
		node = ext2fs_rb_next(node);
		𐤀𐤌 (node == عدم)
			أرجع ENOENT;
		ext = node_to_extent(node);
	}
	𐤀𐤌 (ext->start <= end) {
		*out = ext->start + bitmap->start;
		أرجع 0;
	}
	أرجع ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_private *bp;
	بنية rb_node *node = عدم;
	بنية bmap_rb_extent *ext;
	__u64 count = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	مزدوج eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (بنية ext2fs_rb_private *) bitmap->خاص;

	لكل (node = ext2fs_rb_first(&bp->root); node != عدم;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		count++;
		𐤀𐤌 (ext->count > max_size)
			max_size = ext->count;
		𐤀𐤌 (ext->count < min_size)
			min_size = ext->count;
		size += ext->count;
	}

	𐤀𐤌 (count)
		avg_size = size / count;
	𐤀𐤌 (min_size == ULONG_MAX)
		min_size = 0;
	eff = (مزدوج)((count * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_count + bitmap->stats.mark_ext_count;
	test_all = bitmap->stats.test_count + bitmap->stats.test_ext_count;
	𐤀𐤌 (mark_all)
		m_hit = ((مزدوج)bp->mark_hit / mark_all) * 100;
	𐤀𐤌 (test_all)
		t_hit = ((مزدوج)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) count, (unsigned long long)
		((count * حجم(بنية bmap_rb_extent)) +
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
