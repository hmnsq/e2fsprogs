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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#اگر HAVE_LINUX_TYPES_H
#شامل <linux/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"
#شامل "rbtree.h"

#شامل <limits.h>

ساختار bmap_rb_extent {
	ساختار rb_گره گره;
	__u64 start;
	__u64 شمار;
};

ساختار ext2fs_rb_private {
	ساختار rb_ریشه ریشه;
	ساختار bmap_rb_extent *wcursor;
	ساختار bmap_rb_extent *rcursor;
	ساختار bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

درونخطی ایستا ساختار bmap_rb_extent *گره_to_extent(ساختار rb_گره *گره)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	بازگشت (ساختار bmap_rb_extent *) گره;
}

ایستا صحیح rb_درج_extent(__u64 start, __u64 شمار,
			    ساختار ext2fs_rb_private *);
ایستا تهی rb_get_new_extent(ساختار bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
ایستا تهی print_درخت(ساختار rb_ریشه *ریشه)
{
	ساختار rb_گره *گره = پوچ;
	ساختار bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گره = ext2fs_rb_first(ریشه);
	برای (گره = ext2fs_rb_first(ریشه); گره != پوچ; 
	     گره = ext2fs_rb_next(گره)) {
		ext = گره_to_extent(گره);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->شمار);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ایستا تهی check_درخت(ساختار rb_ریشه *ریشه, ثابت نویسه *پیام)
{
	ساختار rb_گره *گره;
	ساختار bmap_rb_extent *ext, *old = پوچ;

	برای (گره = ext2fs_rb_first(ریشه); گره;
	     گره = ext2fs_rb_next(گره)) {
		ext = گره_to_extent(گره);
		اگر (ext->شمار == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->شمار,
				(unsigned long long) ext->شمار);
			برو err_out;
		}
		اگر (ext->start + ext->شمار < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->شمار,
				(unsigned long long) ext->شمار);
			برو err_out;
		}

		اگر (old) {
			اگر (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->شمار,
					(unsigned long long) old->شمار);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->شمار,
					(unsigned long long) ext->شمار);
				برو err_out;
			}
			اگر ((old->start + old->شمار) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->شمار,
					(unsigned long long) old->شمار);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->شمار,
					(unsigned long long) ext->شمار);
				برو err_out;
			}
		}
		old = ext;
	}
	بازگشت;

err_out:
	fprintf(stderr, "%s\n", پیام);
	print_درخت(ریشه);
	exit(1);
}
#وگرنه
#define check_درخت(ریشه, پیام) انجامبده {} وقتی (0)
#define print_درخت(ریشه) انجامبده {} وقتی (0)
#endif

ایستا تهی rb_get_new_extent(ساختار bmap_rb_extent **ext, __u64 start,
			     __u64 شمار)
{
	ساختار bmap_rb_extent *new_ext;
	صحیح retval;

	retval = ext2fs_get_mem(اندازه (ساختار bmap_rb_extent),
				&new_ext);
	اگر (retval)
		abort();

	new_ext->start = start;
	new_ext->شمار = شمار;
	*ext = new_ext;
}

درونخطی
ایستا تهی rb_free_extent(ساختار ext2fs_rb_private *bp,
			   ساختار bmap_rb_extent *ext)
{
	اگر (bp->wcursor == ext)
		bp->wcursor = پوچ;
	اگر (bp->rcursor == ext)
		bp->rcursor = پوچ;
	اگر (bp->rcursor_next == ext)
		bp->rcursor_next = پوچ;
	ext2fs_free_mem(&ext);
}

ایستا errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ساختار ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(اندازه (ساختار ext2fs_rb_private), &bp);
	اگر (retval)
		بازگشت retval;

	bp->ریشه = RB_ریشه;
	bp->rcursor = پوچ;
	bp->rcursor_next = پوچ;
	bp->wcursor = پوچ;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->خصوصی = (تهی *) bp;
	بازگشت 0;
}

ایستا errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	اگر (retval)
		بازگشت retval;

	بازگشت 0;
}

ایستا تهی rb_free_درخت(ساختار rb_ریشه *ریشه)
{
	ساختار bmap_rb_extent *ext;
	ساختار rb_گره *گره, *next;

	برای (گره = ext2fs_rb_first(ریشه); گره; گره = next) {
		next = ext2fs_rb_next(گره);
		ext = گره_to_extent(گره);
		ext2fs_rb_erase(گره, ریشه);
		ext2fs_free_mem(&ext);
	}
}

ایستا تهی rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;

	rb_free_درخت(&bp->ریشه);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ایستا errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ساختار ext2fs_rb_private *src_bp, *dest_bp;
	ساختار bmap_rb_extent *src_ext, *dest_ext;
	ساختار rb_گره *dest_گره, *src_گره, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	اگر (retval)
		بازگشت retval;

	src_bp = (ساختار ext2fs_rb_private *) src->خصوصی;
	dest_bp = (ساختار ext2fs_rb_private *) dest->خصوصی;
	src_bp->rcursor = پوچ;
	dest_bp->rcursor = پوچ;

	src_گره = ext2fs_rb_first(&src_bp->ریشه);
	وقتی (src_گره) {
		src_ext = گره_to_extent(src_گره);
		retval = ext2fs_get_mem(اندازه (ساختار bmap_rb_extent),
					&dest_ext);
		اگر (retval)
			بشکن;

		memcpy(dest_ext, src_ext, اندازه(ساختار bmap_rb_extent));

		dest_گره = &dest_ext->گره;
		n = &dest_bp->ریشه.rb_گره;

		dest_last = پوچ;
		اگر (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->ریشه);
			n = &(dest_last)->rb_راست;
		}

		ext2fs_rb_link_گره(dest_گره, dest_last, n);
		ext2fs_rb_درج_color(dest_گره, &dest_bp->ریشه);

		src_گره = ext2fs_rb_next(src_گره);
	}

	بازگشت retval;
}

ایستا تهی rb_truncate(__u64 new_max, ساختار rb_ریشه *ریشه)
{
	ساختار bmap_rb_extent *ext;
	ساختار rb_گره *گره;

	گره = ext2fs_rb_last(ریشه);
	وقتی (گره) {
		ext = گره_to_extent(گره);

		اگر ((ext->start + ext->شمار - 1) <= new_max)
			بشکن;
		وگرنه اگر (ext->start > new_max) {
			ext2fs_rb_erase(گره, ریشه);
			ext2fs_free_mem(&ext);
			گره = ext2fs_rb_last(ریشه);
			ادامه;
		} وگرنه
			ext->شمار = new_max - ext->start + 1;
	}
}

ایستا errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bmap->خصوصی;
	bp->rcursor = پوچ;
	bp->wcursor = پوچ;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->ریشه);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	اگر (bmap->end < bmap->real_end)
		rb_درج_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	بازگشت 0;

}

درونخطی ایستا صحیح
rb_test_bit(ساختار ext2fs_rb_private *bp, __u64 bit)
{
	ساختار bmap_rb_extent *rcursor, *next_ext = پوچ;
	ساختار rb_گره *parent = پوچ, *next;
	ساختار rb_گره **n = &bp->ریشه.rb_گره;
	ساختار bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	اگر (!rcursor)
		برو درخت_جستجو;

	اگر (bit >= rcursor->start && bit < rcursor->start + rcursor->شمار) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		بازگشت 1;
	}

	next_ext = bp->rcursor_next;
	اگر (!next_ext) {
		next = ext2fs_rb_next(&rcursor->گره);
		اگر (next)
			next_ext = گره_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	اگر (next_ext) {
		اگر ((bit >= rcursor->start + rcursor->شمار) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			بازگشت 0;
		}
	}
	bp->rcursor = پوچ;
	bp->rcursor_next = پوچ;

	rcursor = bp->wcursor;
	اگر (!rcursor)
		برو درخت_جستجو;

	اگر (bit >= rcursor->start && bit < rcursor->start + rcursor->شمار)
		بازگشت 1;

درخت_جستجو:

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (bit < ext->start)
			n = &(*n)->rb_چپ;
		وگرنه اگر (bit >= (ext->start + ext->شمار))
			n = &(*n)->rb_راست;
		وگرنه {
			bp->rcursor = ext;
			bp->rcursor_next = پوچ;
			بازگشت 1;
		}
	}
	بازگشت 0;
}

ایستا صحیح rb_درج_extent(__u64 start, __u64 شمار,
			    ساختار ext2fs_rb_private *bp)
{
	ساختار rb_ریشه *ریشه = &bp->ریشه;
	ساختار rb_گره *parent = پوچ, **n = &ریشه->rb_گره;
	ساختار rb_گره *new_گره, *گره, *next;
	ساختار bmap_rb_extent *new_ext;
	ساختار bmap_rb_extent *ext;
	صحیح retval = 0;

	اگر (شمار == 0)
		بازگشت 0;

	bp->rcursor_next = پوچ;
	ext = bp->wcursor;
	اگر (ext) {
		اگر (start >= ext->start &&
		    start <= (ext->start + ext->شمار)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			برو got_extent;
		}
	}

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);

		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
		} وگرنه اگر (start > (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وگرنه {
got_extent:
			اگر ((start + شمار) <= (ext->start + ext->شمار))
				بازگشت 1;

			اگر ((ext->start + ext->شمار) == start)
				retval = 0;
			وگرنه
				retval = 1;

			شمار += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_گره = &ext->گره;

			برو skip_درج;
		}
	}

	rb_get_new_extent(&new_ext, start, شمار);

	new_گره = &new_ext->گره;
	ext2fs_rb_link_گره(new_گره, parent, n);
	ext2fs_rb_درج_color(new_گره, ریشه);
	bp->wcursor = new_ext;

	گره = ext2fs_rb_prev(new_گره);
	اگر (گره) {
		ext = گره_to_extent(گره);
		اگر ((ext->start + ext->شمار) == start) {
			start = ext->start;
			شمار += ext->شمار;
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
		}
	}

skip_درج:
	/* See if we can merge extent to the right */
	برای (گره = ext2fs_rb_next(new_گره); گره != پوچ; گره = next) {
		next = ext2fs_rb_next(گره);
		ext = گره_to_extent(گره);

		اگر ((ext->start + ext->شمار) <= start)
			ادامه;

		/* No more merging */
		اگر ((start + شمار) < ext->start)
			بشکن;

		/* ext is embedded in new_ext interval */
		اگر ((start + شمار) >= (ext->start + ext->شمار)) {
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
			ادامه;
		} وگرنه {
		/* merge ext with new_ext */
			شمار += ((ext->start + ext->شمار) -
				  (start + شمار));
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
			بشکن;
		}
	}

	new_ext->start = start;
	new_ext->شمار = شمار;

	بازگشت retval;
}

ایستا صحیح rb_remove_extent(__u64 start, __u64 شمار,
			    ساختار ext2fs_rb_private *bp)
{
	ساختار rb_ریشه *ریشه = &bp->ریشه;
	ساختار rb_گره *parent = پوچ, **n = &ریشه->rb_گره;
	ساختار rb_گره *گره;
	ساختار bmap_rb_extent *ext;
	__u64 new_start, new_شمار;
	صحیح retval = 0;

	اگر (ext2fs_rb_empty_ریشه(ریشه))
		بازگشت 0;

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
			ادامه;
		} وگرنه اگر (start >= (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
			ادامه;
		}

		اگر ((start > ext->start) &&
		    (start + شمار) < (ext->start + ext->شمار)) {
			/* We have to split extent into two */
			new_start = start + شمار;
			new_شمار = (ext->start + ext->شمار) - new_start;

			ext->شمار = start - ext->start;

			rb_درج_extent(new_start, new_شمار, bp);
			بازگشت 1;
		}

		اگر ((start + شمار) >= (ext->start + ext->شمار)) {
			ext->شمار = start - ext->start;
			retval = 1;
		}

		اگر (0 == ext->شمار) {
			parent = ext2fs_rb_next(&ext->گره);
			ext2fs_rb_erase(&ext->گره, ریشه);
			rb_free_extent(bp, ext);
			بشکن;
		}

		اگر (start == ext->start) {
			ext->start += شمار;
			ext->شمار -= شمار;
			بازگشت 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	برای (; parent != پوچ; parent = گره) {
		گره = ext2fs_rb_next(parent);
		ext = گره_to_extent(parent);
		اگر ((ext->start + ext->شمار) <= start)
			ادامه;

		/* No more extents to be removed/truncated */
		اگر ((start + شمار) < ext->start)
			بشکن;

		/* The entire extent is within the region to be removed */
		اگر ((start + شمار) >= (ext->start + ext->شمار)) {
			ext2fs_rb_erase(parent, ریشه);
			rb_free_extent(bp, ext);
			retval = 1;
			ادامه;
		} وگرنه {
			/* modify the last extent in region to be removed */
			ext->شمار -= ((start + شمار) - ext->start);
			ext->start = start + شمار;
			retval = 1;
			بشکن;
		}
	}

	بازگشت retval;
}

ایستا صحیح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساختار ext2fs_rb_private *bp;
	صحیح retval;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	arg -= bitmap->start;

	retval = rb_درج_extent(arg, 1, bp);
	check_درخت(&bp->ریشه, __func__);
	بازگشت retval;
}

ایستا صحیح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساختار ext2fs_rb_private *bp;
	صحیح retval;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_درخت(&bp->ریشه, __func__);

	بازگشت retval;
}

درونخطی
ایستا صحیح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	arg -= bitmap->start;

	بازگشت rb_test_bit(bp, arg);
}

ایستا تهی rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned صحیح رقم)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	arg -= bitmap->start;

	rb_درج_extent(arg, رقم, bp);
	check_درخت(&bp->ریشه, __func__);
}

ایستا تهی rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحیح رقم)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	arg -= bitmap->start;

	rb_remove_extent(arg, رقم, bp);
	check_درخت(&bp->ریشه, __func__);
}

ایستا صحیح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحیح len)
{
	ساختار rb_گره *parent = پوچ, **n;
	ساختار rb_گره *گره, *next;
	ساختار ext2fs_rb_private *bp;
	ساختار bmap_rb_extent *ext;
	صحیح retval = 1;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	n = &bp->ریشه.rb_گره;
	start -= bitmap->start;

	اگر (len == 0 || ext2fs_rb_empty_ریشه(&bp->ریشه))
		بازگشت 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
		} وگرنه اگر (start >= (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وگرنه {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			بازگشت 0;
		}
	}

	گره = parent;
	وقتی (گره) {
		next = ext2fs_rb_next(گره);
		ext = گره_to_extent(گره);
		گره = next;

		اگر ((ext->start + ext->شمار) <= start)
			ادامه;

		/* No more merging */
		اگر ((start + len) <= ext->start)
			بشکن;

		retval = 0;
		بشکن;
	}
	بازگشت retval;
}

ایستا errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, تهی *in)
{
	ساختار ext2fs_rb_private *bp;
	unsigned نویسه *cp = in;
	size_t i;
	صحیح first_set = -1;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;

	برای (i = 0; i < رقم; i++) {
		اگر ((i & 7) == 0) {
			unsigned نویسه c = cp[i/8];
			اگر (c == 0xFF) {
				اگر (first_set == -1)
					first_set = i;
				i += 7;
				ادامه;
			}
			اگر ((c == 0x00) && (first_set == -1)) {
				i += 7;
				ادامه;
			}
		}
		اگر (ext2fs_test_bit(i, in)) {
			اگر (first_set == -1)
				first_set = i;
			ادامه;
		}
		اگر (first_set == -1)
			ادامه;

		rb_درج_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_درخت(&bp->ریشه, __func__);
		first_set = -1;
	}
	اگر (first_set != -1) {
		rb_درج_extent(start + first_set - bitmap->start,
				 رقم - first_set, bp);
		check_درخت(&bp->ریشه, __func__);
	}

	بازگشت 0;
}

ایستا errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, تهی *out)
{

	ساختار rb_گره *parent = پوچ, *next, **n;
	ساختار ext2fs_rb_private *bp;
	ساختار bmap_rb_extent *ext;
	__u64 شمار, pos;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	n = &bp->ریشه.rb_گره;
	start -= bitmap->start;

	اگر (ext2fs_rb_empty_ریشه(&bp->ریشه))
		بازگشت 0;

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
		} وگرنه اگر (start >= (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وگرنه
			بشکن;
	}

	memset(out, 0, (رقم + 7) >> 3);

	برای (; parent != پوچ; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = گره_to_extent(parent);

		pos = ext->start;
		شمار = ext->شمار;
		اگر (pos >= start + رقم)
			بشکن;
		اگر (pos < start) {
			اگر (pos + شمار <  start)
				ادامه;
			شمار -= start - pos;
			pos = start;
		}
		اگر (pos + شمار > start + رقم)
			شمار = start + رقم - pos;

		وقتی (شمار > 0) {
			اگر ((شمار >= 8) &&
			    ((pos - start) % 8) == 0) {
				صحیح nbytes = شمار >> 3;
				صحیح offset = (pos - start) >> 3;

				memset(((نویسه *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				شمار -= nbytes << 3;
				ادامه;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			شمار--;
		}
	}
	بازگشت 0;
}

ایستا تهی rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ساختار ext2fs_rb_private *bp;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;

	rb_free_درخت(&bp->ریشه);
	bp->rcursor = پوچ;
	bp->rcursor_next = پوچ;
	bp->wcursor = پوچ;
	check_درخت(&bp->ریشه, __func__);
}

ایستا errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ساختار rb_گره *parent = پوچ, **n;
	ساختار ext2fs_rb_private *bp;
	ساختار bmap_rb_extent *ext;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	n = &bp->ریشه.rb_گره;
	start -= bitmap->start;
	end -= bitmap->start;

	اگر (start > end)
		بازگشت EINVAL;

	اگر (ext2fs_rb_empty_ریشه(&bp->ریشه))
		بازگشت ENOENT;

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
		} وگرنه اگر (start >= (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وگرنه اگر (ext->start + ext->شمار <= end) {
			*out = ext->start + ext->شمار + bitmap->start;
			بازگشت 0;
		} وگرنه
			بازگشت ENOENT;
	}

	*out = start + bitmap->start;
	بازگشت 0;
}

ایستا errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ساختار rb_گره *parent = پوچ, **n;
	ساختار rb_گره *گره;
	ساختار ext2fs_rb_private *bp;
	ساختار bmap_rb_extent *ext;

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;
	n = &bp->ریشه.rb_گره;
	start -= bitmap->start;
	end -= bitmap->start;

	اگر (start > end)
		بازگشت EINVAL;

	اگر (ext2fs_rb_empty_ریشه(&bp->ریشه))
		بازگشت ENOENT;

	وقتی (*n) {
		parent = *n;
		ext = گره_to_extent(parent);
		اگر (start < ext->start) {
			n = &(*n)->rb_چپ;
		} وگرنه اگر (start >= (ext->start + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وگرنه {
			/* The start bit is set */
			*out = start + bitmap->start;
			بازگشت 0;
		}
	}

	گره = parent;
	ext = گره_to_extent(گره);
	اگر (ext->start < start) {
		گره = ext2fs_rb_next(گره);
		اگر (گره == پوچ)
			بازگشت ENOENT;
		ext = گره_to_extent(گره);
	}
	اگر (ext->start <= end) {
		*out = ext->start + bitmap->start;
		بازگشت 0;
	}
	بازگشت ENOENT;
}

#ifdef ENABLE_BMAP_STATS
ایستا تهی rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ساختار ext2fs_rb_private *bp;
	ساختار rb_گره *گره = پوچ;
	ساختار bmap_rb_extent *ext;
	__u64 شمار = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	دوگانه eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	دوگانه m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (ساختار ext2fs_rb_private *) bitmap->خصوصی;

	برای (گره = ext2fs_rb_first(&bp->ریشه); گره != پوچ;
	     گره = ext2fs_rb_next(گره)) {
		ext = گره_to_extent(گره);
		شمار++;
		اگر (ext->شمار > max_size)
			max_size = ext->شمار;
		اگر (ext->شمار < min_size)
			min_size = ext->شمار;
		size += ext->شمار;
	}

	اگر (شمار)
		avg_size = size / شمار;
	اگر (min_size == ULONG_MAX)
		min_size = 0;
	eff = (دوگانه)((شمار * اندازه(ساختار bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_شمار + bitmap->stats.mark_ext_شمار;
	test_all = bitmap->stats.test_شمار + bitmap->stats.test_ext_شمار;
	اگر (mark_all)
		m_hit = ((دوگانه)bp->mark_hit / mark_all) * 100;
	اگر (test_all)
		t_hit = ((دوگانه)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) شمار, (unsigned long long)
		((شمار * اندازه(ساختار bmap_rb_extent)) +
		 اندازه(ساختار ext2fs_rb_private)));
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
#وگرنه
ایستا تهی rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

ساختار ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
