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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#إذا HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_غوټه غوټه;
	__u64 پیل;
	__u64 شمېر;
};

بنية ext2fs_rb_شخصي {
	بنية rb_ریښه ریښه;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_بل;
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	__u64 mark_موندنه;
	__u64 test_موندنه;
#د_که_پای
};

مضمن ساكن بنية bmap_rb_extent *غوټه_منزل_extent(بنية rb_غوټه *غوټه)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) غوټه;
}

ساكن صحيح rb_ورننباسه_extent(__u64 پیل, __u64 شمېر,
			    بنية ext2fs_rb_شخصي *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#که_تعریف_شوی DEBUG_RB
ساكن فراغ print_ونه(بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *غوټه = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	غوټه = ext2fs_rb_لومړی(ریښه);
	لكل (غوټه = ext2fs_rb_لومړی(ریښه); غوټه != عدم; 
	     غوټه = ext2fs_rb_بل(غوټه)) {
		ext = غوټه_منزل_extent(غوټه);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(بینښه اوږد اوږد) ext->پیل,
			(بینښه اوږد اوږد) ext->پیل + ext->شمېر);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_ونه(بنية rb_ریښه *ریښه, ثابت حرف *پیغام)
{
	بنية rb_غوټه *غوټه;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (غوټه = ext2fs_rb_لومړی(ریښه); غوټه;
	     غوټه = ext2fs_rb_بل(غوټه)) {
		ext = غوټه_منزل_extent(غوټه);
		إذا (ext->شمېر == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بینښه اوږد اوږد) ext->پیل,
				(بینښه اوږد اوږد) ext->پیل + ext->شمېر,
				(بینښه اوږد اوږد) ext->شمېر);
			اذهب err_بهر;
		}
		إذا (ext->پیل + ext->شمېر < ext->پیل) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بینښه اوږد اوږد) ext->پیل,
				(بینښه اوږد اوږد) ext->پیل + ext->شمېر,
				(بینښه اوږد اوږد) ext->شمېر);
			اذهب err_بهر;
		}

		إذا (old) {
			إذا (old->پیل > ext->پیل) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بینښه اوږد اوږد) old->پیل,
					(بینښه اوږد اوږد) old->پیل + old->شمېر,
					(بینښه اوږد اوږد) old->شمېر);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بینښه اوږد اوږد) ext->پیل,
					(بینښه اوږد اوږد) ext->پیل + ext->شمېر,
					(بینښه اوږد اوږد) ext->شمېر);
				اذهب err_بهر;
			}
			إذا ((old->پیل + old->شمېر) >= ext->پیل) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بینښه اوږد اوږد) old->پیل,
					(بینښه اوږد اوږد) old->پیل + old->شمېر,
					(بینښه اوږد اوږد) old->شمېر);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بینښه اوږد اوږد) ext->پیل,
					(بینښه اوږد اوږد) ext->پیل + ext->شمېر,
					(بینښه اوږد اوږد) ext->شمېر);
				اذهب err_بهر;
			}
		}
		old = ext;
	}
	أرجع;

err_بهر:
	fprintf(stderr, "%s\n", پیغام);
	print_ونه(ریښه);
	وتل(1);
}
#وإلا
#تعریف_کړه check_ونه(ریښه, پیغام) افعل {} طالما (0)
#تعریف_کړه print_ونه(ریښه) افعل {} طالما (0)
#د_که_پای

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 پیل,
			     __u64 شمېر)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->پیل = پیل;
	new_ext->شمېر = شمېر;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_شخصي *bp,
			   بنية bmap_rb_extent *ext)
{
	إذا (bp->wcursor == ext)
		bp->wcursor = عدم;
	إذا (bp->rcursor == ext)
		bp->rcursor = عدم;
	إذا (bp->rcursor_بل == ext)
		bp->rcursor_بل = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_شخصي_معلومات (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_شخصي *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_شخصي), &bp);
	إذا (retval)
		أرجع retval;

	bp->ریښه = RB_ریښه;
	bp->rcursor = عدم;
	bp->rcursor_بل = عدم;
	bp->wcursor = عدم;

#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	bp->test_موندنه = 0;
	bp->mark_موندنه = 0;
#د_که_پای

	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((ناکارول_شوی)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_شخصي_معلومات (bitmap);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_ونه(بنية rb_ریښه *ریښه)
{
	بنية bmap_rb_extent *ext;
	بنية rb_غوټه *غوټه, *بل;

	لكل (غوټه = ext2fs_rb_لومړی(ریښه); غوټه; غوټه = بل) {
		بل = ext2fs_rb_بل(غوټه);
		ext = غوټه_منزل_extent(غوټه);
		ext2fs_rb_erase(غوټه, ریښه);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;

	rb_free_ونه(&bp->ریښه);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_شخصي *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_غوټه *dest_غوټه, *src_غوټه, *dest_وروستی, **n;
	errcode_t retval = 0;

	retval = rb_alloc_شخصي_معلومات (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_شخصي *) src->خاص;
	dest_bp = (بنية ext2fs_rb_شخصي *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_غوټه = ext2fs_rb_لومړی(&src_bp->ریښه);
	طالما (src_غوټه) {
		src_ext = غوټه_منزل_extent(src_غوټه);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		حافظه_نقل(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_غوټه = &dest_ext->غوټه;
		n = &dest_bp->ریښه.rb_غوټه;

		dest_وروستی = عدم;
		إذا (*n) {
			dest_وروستی = ext2fs_rb_وروستی(&dest_bp->ریښه);
			n = &(dest_وروستی)->rb_ښی;
		}

		ext2fs_rb_link_غوټه(dest_غوټه, dest_وروستی, n);
		ext2fs_rb_ورننباسه_color(dest_غوټه, &dest_bp->ریښه);

		src_غوټه = ext2fs_rb_بل(src_غوټه);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_ریښه *ریښه)
{
	بنية bmap_rb_extent *ext;
	بنية rb_غوټه *غوټه;

	غوټه = ext2fs_rb_وروستی(ریښه);
	طالما (غوټه) {
		ext = غوټه_منزل_extent(غوټه);

		إذا ((ext->پیل + ext->شمېر - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->پیل > new_max) {
			ext2fs_rb_erase(غوټه, ریښه);
			ext2fs_free_mem(&ext);
			غوټه = ext2fs_rb_وروستی(ریښه);
			استمر;
		} وإلا
			ext->شمېر = new_max - ext->پیل + 1;
	}
}

ساكن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_پای_ټکی, __u64 new_real_پای_ټکی)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bmap->خاص;
	bp->rcursor = عدم;
	bp->wcursor = عدم;

	rb_truncate(((new_پای_ټکی < bmap->پای_ټکی) ? new_پای_ټکی : bmap->پای_ټکی) - bmap->پیل,
		    &bp->ریښه);

	bmap->پای_ټکی = new_پای_ټکی;
	bmap->real_پای_ټکی = new_real_پای_ټکی;

	إذا (bmap->پای_ټکی < bmap->real_پای_ټکی)
		rb_ورننباسه_extent(bmap->پای_ټکی + 1 - bmap->پیل,
				 bmap->real_پای_ټکی - bmap->پای_ټکی, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_بټ(بنية ext2fs_rb_شخصي *bp, __u64 بټ)
{
	بنية bmap_rb_extent *rcursor, *بل_ext = عدم;
	بنية rb_غوټه *پلار = عدم, *بل;
	بنية rb_غوټه **n = &bp->ریښه.rb_غوټه;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب پلټنې_ونه;

	إذا (بټ >= rcursor->پیل && بټ < rcursor->پیل + rcursor->شمېر) {
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
		bp->test_موندنه++;
#د_که_پای
		أرجع 1;
	}

	بل_ext = bp->rcursor_بل;
	إذا (!بل_ext) {
		بل = ext2fs_rb_بل(&rcursor->غوټه);
		إذا (بل)
			بل_ext = غوټه_منزل_extent(بل);
		bp->rcursor_بل = بل_ext;
	}
	إذا (بل_ext) {
		إذا ((بټ >= rcursor->پیل + rcursor->شمېر) &&
		    (بټ < بل_ext->پیل)) {
#که_تعریف_شوی BMAP_STATS_OPS
			bp->test_موندنه++;
#د_که_پای
			أرجع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_بل = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب پلټنې_ونه;

	إذا (بټ >= rcursor->پیل && بټ < rcursor->پیل + rcursor->شمېر)
		أرجع 1;

پلټنې_ونه:

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (بټ < ext->پیل)
			n = &(*n)->rb_کیڼ;
		وإلا إذا (بټ >= (ext->پیل + ext->شمېر))
			n = &(*n)->rb_ښی;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_بل = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_ورننباسه_extent(__u64 پیل, __u64 شمېر,
			    بنية ext2fs_rb_شخصي *bp)
{
	بنية rb_ریښه *ریښه = &bp->ریښه;
	بنية rb_غوټه *پلار = عدم, **n = &ریښه->rb_غوټه;
	بنية rb_غوټه *new_غوټه, *غوټه, *بل;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (شمېر == 0)
		أرجع 0;

	bp->rcursor_بل = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (پیل >= ext->پیل &&
		    پیل <= (ext->پیل + ext->شمېر)) {
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
			bp->mark_موندنه++;
#د_که_پای
			اذهب got_extent;
		}
	}

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);

		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
		} وإلا إذا (پیل > (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
		} وإلا {
got_extent:
			إذا ((پیل + شمېر) <= (ext->پیل + ext->شمېر))
				أرجع 1;

			إذا ((ext->پیل + ext->شمېر) == پیل)
				retval = 0;
			وإلا
				retval = 1;

			شمېر += (پیل - ext->پیل);
			پیل = ext->پیل;
			new_ext = ext;
			new_غوټه = &ext->غوټه;

			اذهب skip_ورننباسه;
		}
	}

	rb_get_new_extent(&new_ext, پیل, شمېر);

	new_غوټه = &new_ext->غوټه;
	ext2fs_rb_link_غوټه(new_غوټه, پلار, n);
	ext2fs_rb_ورننباسه_color(new_غوټه, ریښه);
	bp->wcursor = new_ext;

	غوټه = ext2fs_rb_prev(new_غوټه);
	إذا (غوټه) {
		ext = غوټه_منزل_extent(غوټه);
		إذا ((ext->پیل + ext->شمېر) == پیل) {
			پیل = ext->پیل;
			شمېر += ext->شمېر;
			ext2fs_rb_erase(غوټه, ریښه);
			rb_free_extent(bp, ext);
		}
	}

skip_ورننباسه:
	/* See if we can merge extent to the right */
	لكل (غوټه = ext2fs_rb_بل(new_غوټه); غوټه != عدم; غوټه = بل) {
		بل = ext2fs_rb_بل(غوټه);
		ext = غوټه_منزل_extent(غوټه);

		إذا ((ext->پیل + ext->شمېر) <= پیل)
			استمر;

		/* No more merging */
		إذا ((پیل + شمېر) < ext->پیل)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((پیل + شمېر) >= (ext->پیل + ext->شمېر)) {
			ext2fs_rb_erase(غوټه, ریښه);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			شمېر += ((ext->پیل + ext->شمېر) -
				  (پیل + شمېر));
			ext2fs_rb_erase(غوټه, ریښه);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->پیل = پیل;
	new_ext->شمېر = شمېر;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 پیل, __u64 شمېر,
			    بنية ext2fs_rb_شخصي *bp)
{
	بنية rb_ریښه *ریښه = &bp->ریښه;
	بنية rb_غوټه *پلار = عدم, **n = &ریښه->rb_غوټه;
	بنية rb_غوټه *غوټه;
	بنية bmap_rb_extent *ext;
	__u64 new_پیل, new_شمېر;
	صحيح retval = 0;

	إذا (ext2fs_rb_تشه_ریښه(ریښه))
		أرجع 0;

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
			استمر;
		} وإلا إذا (پیل >= (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
			استمر;
		}

		إذا ((پیل > ext->پیل) &&
		    (پیل + شمېر) < (ext->پیل + ext->شمېر)) {
			/* We have to split extent into two */
			new_پیل = پیل + شمېر;
			new_شمېر = (ext->پیل + ext->شمېر) - new_پیل;

			ext->شمېر = پیل - ext->پیل;

			rb_ورننباسه_extent(new_پیل, new_شمېر, bp);
			أرجع 1;
		}

		إذا ((پیل + شمېر) >= (ext->پیل + ext->شمېر)) {
			ext->شمېر = پیل - ext->پیل;
			retval = 1;
		}

		إذا (0 == ext->شمېر) {
			پلار = ext2fs_rb_بل(&ext->غوټه);
			ext2fs_rb_erase(&ext->غوټه, ریښه);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (پیل == ext->پیل) {
			ext->پیل += شمېر;
			ext->شمېر -= شمېر;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; پلار != عدم; پلار = غوټه) {
		غوټه = ext2fs_rb_بل(پلار);
		ext = غوټه_منزل_extent(پلار);
		إذا ((ext->پیل + ext->شمېر) <= پیل)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((پیل + شمېر) < ext->پیل)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((پیل + شمېر) >= (ext->پیل + ext->شمېر)) {
			ext2fs_rb_erase(پلار, ریښه);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->شمېر -= ((پیل + شمېر) - ext->پیل);
			ext->پیل = پیل + شمېر;
			retval = 1;
			اكسر;
		}
	}

	أرجع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_شخصي *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	arg -= bitmap->پیل;

	retval = rb_ورننباسه_extent(arg, 1, bp);
	check_ونه(&bp->ریښه, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_شخصي *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	arg -= bitmap->پیل;

	retval = rb_remove_extent(arg, 1, bp);
	check_ونه(&bp->ریښه, __func__);

	أرجع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	arg -= bitmap->پیل;

	أرجع rb_test_بټ(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بینښه صحيح عدد)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	arg -= bitmap->پیل;

	rb_ورننباسه_extent(arg, عدد, bp);
	check_ونه(&bp->ریښه, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بینښه صحيح عدد)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	arg -= bitmap->پیل;

	rb_remove_extent(arg, عدد, bp);
	check_ونه(&bp->ریښه, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, بینښه صحيح len)
{
	بنية rb_غوټه *پلار = عدم, **n;
	بنية rb_غوټه *غوټه, *بل;
	بنية ext2fs_rb_شخصي *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	n = &bp->ریښه.rb_غوټه;
	پیل -= bitmap->پیل;

	إذا (len == 0 || ext2fs_rb_تشه_ریښه(&bp->ریښه))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
		} وإلا إذا (پیل >= (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	غوټه = پلار;
	طالما (غوټه) {
		بل = ext2fs_rb_بل(غوټه);
		ext = غوټه_منزل_extent(غوټه);
		غوټه = بل;

		إذا ((ext->پیل + ext->شمېر) <= پیل)
			استمر;

		/* No more merging */
		إذا ((پیل + len) <= ext->پیل)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_سېټ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, size_t عدد, فراغ *دننه)
{
	بنية ext2fs_rb_شخصي *bp;
	بینښه حرف *cp = دننه;
	size_t i;
	صحيح لومړی_سېټ = -1;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;

	لكل (i = 0; i < عدد; i++) {
		إذا ((i & 7) == 0) {
			بینښه حرف c = cp[i/8];
			إذا (c == 0xFF) {
				إذا (لومړی_سېټ == -1)
					لومړی_سېټ = i;
				i += 7;
				استمر;
			}
			إذا ((c == 0x00) && (لومړی_سېټ == -1)) {
				i += 7;
				استمر;
			}
		}
		إذا (ext2fs_test_بټ(i, دننه)) {
			إذا (لومړی_سېټ == -1)
				لومړی_سېټ = i;
			استمر;
		}
		إذا (لومړی_سېټ == -1)
			استمر;

		rb_ورننباسه_extent(پیل + لومړی_سېټ - bitmap->پیل,
				 i - لومړی_سېټ, bp);
		check_ونه(&bp->ریښه, __func__);
		لومړی_سېټ = -1;
	}
	إذا (لومړی_سېټ != -1) {
		rb_ورننباسه_extent(پیل + لومړی_سېټ - bitmap->پیل,
				 عدد - لومړی_سېټ, bp);
		check_ونه(&bp->ریښه, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, size_t عدد, فراغ *بهر)
{

	بنية rb_غوټه *پلار = عدم, *بل, **n;
	بنية ext2fs_rb_شخصي *bp;
	بنية bmap_rb_extent *ext;
	__u64 شمېر, pos;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	n = &bp->ریښه.rb_غوټه;
	پیل -= bitmap->پیل;

	إذا (ext2fs_rb_تشه_ریښه(&bp->ریښه))
		أرجع 0;

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
		} وإلا إذا (پیل >= (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
		} وإلا
			اكسر;
	}

	memset(بهر, 0, (عدد + 7) >> 3);

	لكل (; پلار != عدم; پلار = بل) {
		بل = ext2fs_rb_بل(پلار);
		ext = غوټه_منزل_extent(پلار);

		pos = ext->پیل;
		شمېر = ext->شمېر;
		إذا (pos >= پیل + عدد)
			اكسر;
		إذا (pos < پیل) {
			إذا (pos + شمېر <  پیل)
				استمر;
			شمېر -= پیل - pos;
			pos = پیل;
		}
		إذا (pos + شمېر > پیل + عدد)
			شمېر = پیل + عدد - pos;

		طالما (شمېر > 0) {
			إذا ((شمېر >= 8) &&
			    ((pos - پیل) % 8) == 0) {
				صحيح nbytes = شمېر >> 3;
				صحيح دننه_واټن = (pos - پیل) >> 3;

				memset(((حرف *) بهر) + دننه_واټن, 0xFF, nbytes);
				pos += nbytes << 3;
				شمېر -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_سېټ_bit64((pos - پیل), بهر);
			pos++;
			شمېر--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_شخصي *bp;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;

	rb_free_ونه(&bp->ریښه);
	bp->rcursor = عدم;
	bp->rcursor_بل = عدم;
	bp->wcursor = عدم;
	check_ونه(&bp->ریښه, __func__);
}

ساكن errcode_t rb_پیدا_کړه_لومړی_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	بنية rb_غوټه *پلار = عدم, **n;
	بنية ext2fs_rb_شخصي *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	n = &bp->ریښه.rb_غوټه;
	پیل -= bitmap->پیل;
	پای_ټکی -= bitmap->پیل;

	إذا (پیل > پای_ټکی)
		أرجع EINVAL;

	إذا (ext2fs_rb_تشه_ریښه(&bp->ریښه))
		أرجع ENOENT;

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
		} وإلا إذا (پیل >= (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
		} وإلا إذا (ext->پیل + ext->شمېر <= پای_ټکی) {
			*بهر = ext->پیل + ext->شمېر + bitmap->پیل;
			أرجع 0;
		} وإلا
			أرجع ENOENT;
	}

	*بهر = پیل + bitmap->پیل;
	أرجع 0;
}

ساكن errcode_t rb_پیدا_کړه_لومړی_سېټ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	بنية rb_غوټه *پلار = عدم, **n;
	بنية rb_غوټه *غوټه;
	بنية ext2fs_rb_شخصي *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;
	n = &bp->ریښه.rb_غوټه;
	پیل -= bitmap->پیل;
	پای_ټکی -= bitmap->پیل;

	إذا (پیل > پای_ټکی)
		أرجع EINVAL;

	إذا (ext2fs_rb_تشه_ریښه(&bp->ریښه))
		أرجع ENOENT;

	طالما (*n) {
		پلار = *n;
		ext = غوټه_منزل_extent(پلار);
		إذا (پیل < ext->پیل) {
			n = &(*n)->rb_کیڼ;
		} وإلا إذا (پیل >= (ext->پیل + ext->شمېر)) {
			n = &(*n)->rb_ښی;
		} وإلا {
			/* The start bit is set */
			*بهر = پیل + bitmap->پیل;
			أرجع 0;
		}
	}

	غوټه = پلار;
	ext = غوټه_منزل_extent(غوټه);
	إذا (ext->پیل < پیل) {
		غوټه = ext2fs_rb_بل(غوټه);
		إذا (غوټه == عدم)
			أرجع ENOENT;
		ext = غوټه_منزل_extent(غوټه);
	}
	إذا (ext->پیل <= پای_ټکی) {
		*بهر = ext->پیل + bitmap->پیل;
		أرجع 0;
	}
	أرجع ENOENT;
}

#که_تعریف_شوی ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_شخصي *bp;
	بنية rb_غوټه *غوټه = عدم;
	بنية bmap_rb_extent *ext;
	__u64 شمېر = 0;
	__u64 max_اندازه = 0;
	__u64 min_اندازه = ULONG_MAX;
	__u64 اندازه = 0, avg_اندازه = 0;
	مزدوج eff;
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_موندنه = 0.0, t_موندنه = 0.0;
#د_که_پای

	bp = (بنية ext2fs_rb_شخصي *) bitmap->خاص;

	لكل (غوټه = ext2fs_rb_لومړی(&bp->ریښه); غوټه != عدم;
	     غوټه = ext2fs_rb_بل(غوټه)) {
		ext = غوټه_منزل_extent(غوټه);
		شمېر++;
		إذا (ext->شمېر > max_اندازه)
			max_اندازه = ext->شمېر;
		إذا (ext->شمېر < min_اندازه)
			min_اندازه = ext->شمېر;
		اندازه += ext->شمېر;
	}

	إذا (شمېر)
		avg_اندازه = اندازه / شمېر;
	إذا (min_اندازه == ULONG_MAX)
		min_اندازه = 0;
	eff = (مزدوج)((شمېر * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_پای_ټکی - bitmap->پیل);
#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_شمېر + bitmap->stats.mark_ext_شمېر;
	test_all = bitmap->stats.test_شمېر + bitmap->stats.test_ext_شمېر;
	إذا (mark_all)
		m_موندنه = ((مزدوج)bp->mark_موندنه / mark_all) * 100;
	إذا (test_all)
		t_موندنه = ((مزدوج)bp->test_موندنه / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_موندنه, t_موندنه, bp->mark_موندنه, m_موندنه);
#د_که_پای
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(بینښه اوږد اوږد) شمېر, (بینښه اوږد اوږد)
		((شمېر * حجم(بنية bmap_rb_extent)) +
		 حجم(بنية ext2fs_rb_شخصي)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(بینښه اوږد اوږد) min_اندازه);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(بینښه اوږد اوږد) max_اندازه, (بینښه اوږد اوږد) avg_اندازه);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(بینښه اوږد اوږد) اندازه,
		(بینښه اوږد اوږد) bitmap->real_پای_ټکی - bitmap->پیل);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#وإلا
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ناکارول_شوی)))
{
}
#د_که_پای

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
	.سېټ_bmap_range = rb_سېټ_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.پیدا_کړه_لومړی_zero = rb_پیدا_کړه_لومړی_zero,
	.پیدا_کړه_لومړی_سېټ = rb_پیدا_کړه_لومړی_سېټ,
};
