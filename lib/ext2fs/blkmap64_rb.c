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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_LINUX_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bmap64.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rbtree.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

ਬਣਤਰ bmap_rb_extent {
	ਬਣਤਰ rb_ਗੰਢ ਗੰਢ;
	__u64 ਸ਼ੁਰੂ;
	__u64 ਗਿਣਤੀ_2;
};

ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ {
	ਬਣਤਰ rb_ਜੜ੍ਹ ਜੜ੍ਹ;
	ਬਣਤਰ bmap_rb_extent *wcursor;
	ਬਣਤਰ bmap_rb_extent *rcursor;
	ਬਣਤਰ bmap_rb_extent *rcursor_ਅਗਲਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	__u64 mark_ਪ੍ਰਾਪਤੀ;
	__u64 test_ਪ੍ਰਾਪਤੀ;
#ਜੇ_ਅੰਤ
};

ਸਤਰਅੰਦਰ ਸਥਿਰ ਬਣਤਰ bmap_rb_extent *ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	ਵਾਪਸ (ਬਣਤਰ bmap_rb_extent *) ਗੰਢ;
}

ਸਥਿਰ ਪੂਰਨ rb_ਪਾਓ_extent(__u64 ਸ਼ੁਰੂ, __u64 ਗਿਣਤੀ_2,
			    ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *);
ਸਥਿਰ ਖਾਲੀ rb_get_new_extent(ਬਣਤਰ bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_RB
ਸਥਿਰ ਖਾਲੀ print_ਰੁੱਖ(ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
	ਬਣਤਰ bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(ਜੜ੍ਹ);
	ਲਈ (ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(ਜੜ੍ਹ); ਗੰਢ != ਨਿਰਾ; 
	     ਗੰਢ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ)) {
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ,
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ਸਥਿਰ ਖਾਲੀ check_ਰੁੱਖ(ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ;
	ਬਣਤਰ bmap_rb_extent *ext, *old = ਨਿਰਾ;

	ਲਈ (ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(ਜੜ੍ਹ); ਗੰਢ;
	     ਗੰਢ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ)) {
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		ਜੇ (ext->ਗਿਣਤੀ_2 == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਗਿਣਤੀ_2);
			ਜਾਓ err_ਬਾਹਰਲਾ;
		}
		ਜੇ (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2 < ext->ਸ਼ੁਰੂ) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਗਿਣਤੀ_2);
			ਜਾਓ err_ਬਾਹਰਲਾ;
		}

		ਜੇ (old) {
			ਜੇ (old->ਸ਼ੁਰੂ > ext->ਸ਼ੁਰੂ) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਸ਼ੁਰੂ,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਸ਼ੁਰੂ + old->ਗਿਣਤੀ_2,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਗਿਣਤੀ_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਗਿਣਤੀ_2);
				ਜਾਓ err_ਬਾਹਰਲਾ;
			}
			ਜੇ ((old->ਸ਼ੁਰੂ + old->ਗਿਣਤੀ_2) >= ext->ਸ਼ੁਰੂ) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਸ਼ੁਰੂ,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਸ਼ੁਰੂ + old->ਗਿਣਤੀ_2,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) old->ਗਿਣਤੀ_2);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2,
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ext->ਗਿਣਤੀ_2);
				ਜਾਓ err_ਬਾਹਰਲਾ;
			}
		}
		old = ext;
	}
	ਵਾਪਸ;

err_ਬਾਹਰਲਾ:
	fprintf(stderr, "%s\n", ਸੁਨੇਹਾ);
	print_ਰੁੱਖ(ਜੜ੍ਹ);
	ਬਾਹਰ_ਨਿਕਲੋ(1);
}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_ਰੁੱਖ(ਜੜ੍ਹ, ਸੁਨੇਹਾ) ਕਰੋ {} ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_ਰੁੱਖ(ਜੜ੍ਹ) ਕਰੋ {} ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ rb_get_new_extent(ਬਣਤਰ bmap_rb_extent **ext, __u64 ਸ਼ੁਰੂ,
			     __u64 ਗਿਣਤੀ_2)
{
	ਬਣਤਰ bmap_rb_extent *new_ext;
	ਪੂਰਨ retval;

	retval = ext2fs_get_mem(ਆਕਾਰ (ਬਣਤਰ bmap_rb_extent),
				&new_ext);
	ਜੇ (retval)
		abort();

	new_ext->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
	new_ext->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;
	*ext = new_ext;
}

ਸਤਰਅੰਦਰ
ਸਥਿਰ ਖਾਲੀ rb_free_extent(ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp,
			   ਬਣਤਰ bmap_rb_extent *ext)
{
	ਜੇ (bp->wcursor == ext)
		bp->wcursor = ਨਿਰਾ;
	ਜੇ (bp->rcursor == ext)
		bp->rcursor = ਨਿਰਾ;
	ਜੇ (bp->rcursor_ਅਗਲਾ == ext)
		bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;
	ext2fs_free_mem(&ext);
}

ਸਥਿਰ errcode_t rb_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (ext2fs_generic_bitmap_64 bitmap)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(ਆਕਾਰ (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ), &bp);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	bp->ਜੜ੍ਹ = RB_ਜੜ੍ਹ;
	bp->rcursor = ਨਿਰਾ;
	bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;
	bp->wcursor = ਨਿਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	bp->test_ਪ੍ਰਾਪਤੀ = 0;
	bp->mark_ਪ੍ਰਾਪਤੀ = 0;
#ਜੇ_ਅੰਤ

	bitmap->ਨਿਜੀ = (ਖਾਲੀ *) bp;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (bitmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ rb_free_ਰੁੱਖ(ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ bmap_rb_extent *ext;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, *ਅਗਲਾ;

	ਲਈ (ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(ਜੜ੍ਹ); ਗੰਢ; ਗੰਢ = ਅਗਲਾ) {
		ਅਗਲਾ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ);
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		ext2fs_rb_erase(ਗੰਢ, ਜੜ੍ਹ);
		ext2fs_free_mem(&ext);
	}
}

ਸਥਿਰ ਖਾਲੀ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;

	rb_free_ਰੁੱਖ(&bp->ਜੜ੍ਹ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ਸਥਿਰ errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *src_bp, *dest_bp;
	ਬਣਤਰ bmap_rb_extent *src_ext, *dest_ext;
	ਬਣਤਰ rb_ਗੰਢ *dest_ਗੰਢ, *src_ਗੰਢ, *dest_ਆਖਰੀ, **n;
	errcode_t retval = 0;

	retval = rb_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (dest);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	src_bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) src->ਨਿਜੀ;
	dest_bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) dest->ਨਿਜੀ;
	src_bp->rcursor = ਨਿਰਾ;
	dest_bp->rcursor = ਨਿਰਾ;

	src_ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(&src_bp->ਜੜ੍ਹ);
	ਜਦੋਂ (src_ਗੰਢ) {
		src_ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(src_ਗੰਢ);
		retval = ext2fs_get_mem(ਆਕਾਰ (ਬਣਤਰ bmap_rb_extent),
					&dest_ext);
		ਜੇ (retval)
			ਰੋਕੋ;

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(dest_ext, src_ext, ਆਕਾਰ(ਬਣਤਰ bmap_rb_extent));

		dest_ਗੰਢ = &dest_ext->ਗੰਢ;
		n = &dest_bp->ਜੜ੍ਹ.rb_ਗੰਢ;

		dest_ਆਖਰੀ = ਨਿਰਾ;
		ਜੇ (*n) {
			dest_ਆਖਰੀ = ext2fs_rb_ਆਖਰੀ(&dest_bp->ਜੜ੍ਹ);
			n = &(dest_ਆਖਰੀ)->rb_ਸੱਜਾ;
		}

		ext2fs_rb_link_ਗੰਢ(dest_ਗੰਢ, dest_ਆਖਰੀ, n);
		ext2fs_rb_ਪਾਓ_color(dest_ਗੰਢ, &dest_bp->ਜੜ੍ਹ);

		src_ਗੰਢ = ext2fs_rb_ਅਗਲਾ(src_ਗੰਢ);
	}

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਖਾਲੀ rb_truncate(__u64 new_max, ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ bmap_rb_extent *ext;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ;

	ਗੰਢ = ext2fs_rb_ਆਖਰੀ(ਜੜ੍ਹ);
	ਜਦੋਂ (ਗੰਢ) {
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);

		ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2 - 1) <= new_max)
			ਰੋਕੋ;
		ਨਹੀਂਤਾਂ ਜੇ (ext->ਸ਼ੁਰੂ > new_max) {
			ext2fs_rb_erase(ਗੰਢ, ਜੜ੍ਹ);
			ext2fs_free_mem(&ext);
			ਗੰਢ = ext2fs_rb_ਆਖਰੀ(ਜੜ੍ਹ);
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ
			ext->ਗਿਣਤੀ_2 = new_max - ext->ਸ਼ੁਰੂ + 1;
	}
}

ਸਥਿਰ errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_ਸਿਰਾ, __u64 new_real_ਸਿਰਾ)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bmap->ਨਿਜੀ;
	bp->rcursor = ਨਿਰਾ;
	bp->wcursor = ਨਿਰਾ;

	rb_truncate(((new_ਸਿਰਾ < bmap->ਸਿਰਾ) ? new_ਸਿਰਾ : bmap->ਸਿਰਾ) - bmap->ਸ਼ੁਰੂ,
		    &bp->ਜੜ੍ਹ);

	bmap->ਸਿਰਾ = new_ਸਿਰਾ;
	bmap->real_ਸਿਰਾ = new_real_ਸਿਰਾ;

	ਜੇ (bmap->ਸਿਰਾ < bmap->real_ਸਿਰਾ)
		rb_ਪਾਓ_extent(bmap->ਸਿਰਾ + 1 - bmap->ਸ਼ੁਰੂ,
				 bmap->real_ਸਿਰਾ - bmap->ਸਿਰਾ, bp);
	ਵਾਪਸ 0;

}

ਸਤਰਅੰਦਰ ਸਥਿਰ ਪੂਰਨ
rb_test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp, __u64 ਦੋਆਧਾਰੀ_ਅੰਕ)
{
	ਬਣਤਰ bmap_rb_extent *rcursor, *ਅਗਲਾ_ext = ਨਿਰਾ;
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, *ਅਗਲਾ;
	ਬਣਤਰ rb_ਗੰਢ **n = &bp->ਜੜ੍ਹ.rb_ਗੰਢ;
	ਬਣਤਰ bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ਜੇ (!rcursor)
		ਜਾਓ ਖੋਜ_ਰੁੱਖ;

	ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ >= rcursor->ਸ਼ੁਰੂ && ਦੋਆਧਾਰੀ_ਅੰਕ < rcursor->ਸ਼ੁਰੂ + rcursor->ਗਿਣਤੀ_2) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
		bp->test_ਪ੍ਰਾਪਤੀ++;
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 1;
	}

	ਅਗਲਾ_ext = bp->rcursor_ਅਗਲਾ;
	ਜੇ (!ਅਗਲਾ_ext) {
		ਅਗਲਾ = ext2fs_rb_ਅਗਲਾ(&rcursor->ਗੰਢ);
		ਜੇ (ਅਗਲਾ)
			ਅਗਲਾ_ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਅਗਲਾ);
		bp->rcursor_ਅਗਲਾ = ਅਗਲਾ_ext;
	}
	ਜੇ (ਅਗਲਾ_ext) {
		ਜੇ ((ਦੋਆਧਾਰੀ_ਅੰਕ >= rcursor->ਸ਼ੁਰੂ + rcursor->ਗਿਣਤੀ_2) &&
		    (ਦੋਆਧਾਰੀ_ਅੰਕ < ਅਗਲਾ_ext->ਸ਼ੁਰੂ)) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BMAP_STATS_OPS
			bp->test_ਪ੍ਰਾਪਤੀ++;
#ਜੇ_ਅੰਤ
			ਵਾਪਸ 0;
		}
	}
	bp->rcursor = ਨਿਰਾ;
	bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;

	rcursor = bp->wcursor;
	ਜੇ (!rcursor)
		ਜਾਓ ਖੋਜ_ਰੁੱਖ;

	ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ >= rcursor->ਸ਼ੁਰੂ && ਦੋਆਧਾਰੀ_ਅੰਕ < rcursor->ਸ਼ੁਰੂ + rcursor->ਗਿਣਤੀ_2)
		ਵਾਪਸ 1;

ਖੋਜ_ਰੁੱਖ:

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ < ext->ਸ਼ੁਰੂ)
			n = &(*n)->rb_ਖੱਬਾ;
		ਨਹੀਂਤਾਂ ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2))
			n = &(*n)->rb_ਸੱਜਾ;
		ਨਹੀਂਤਾਂ {
			bp->rcursor = ext;
			bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;
			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ rb_ਪਾਓ_extent(__u64 ਸ਼ੁਰੂ, __u64 ਗਿਣਤੀ_2,
			    ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp)
{
	ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ = &bp->ਜੜ੍ਹ;
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, **n = &ਜੜ੍ਹ->rb_ਗੰਢ;
	ਬਣਤਰ rb_ਗੰਢ *new_ਗੰਢ, *ਗੰਢ, *ਅਗਲਾ;
	ਬਣਤਰ bmap_rb_extent *new_ext;
	ਬਣਤਰ bmap_rb_extent *ext;
	ਪੂਰਨ retval = 0;

	ਜੇ (ਗਿਣਤੀ_2 == 0)
		ਵਾਪਸ 0;

	bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;
	ext = bp->wcursor;
	ਜੇ (ext) {
		ਜੇ (ਸ਼ੁਰੂ >= ext->ਸ਼ੁਰੂ &&
		    ਸ਼ੁਰੂ <= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
			bp->mark_ਪ੍ਰਾਪਤੀ++;
#ਜੇ_ਅੰਤ
			ਜਾਓ got_extent;
		}
	}

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);

		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ > (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
		} ਨਹੀਂਤਾਂ {
got_extent:
			ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) <= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2))
				ਵਾਪਸ 1;

			ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) == ਸ਼ੁਰੂ)
				retval = 0;
			ਨਹੀਂਤਾਂ
				retval = 1;

			ਗਿਣਤੀ_2 += (ਸ਼ੁਰੂ - ext->ਸ਼ੁਰੂ);
			ਸ਼ੁਰੂ = ext->ਸ਼ੁਰੂ;
			new_ext = ext;
			new_ਗੰਢ = &ext->ਗੰਢ;

			ਜਾਓ skip_ਪਾਓ;
		}
	}

	rb_get_new_extent(&new_ext, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2);

	new_ਗੰਢ = &new_ext->ਗੰਢ;
	ext2fs_rb_link_ਗੰਢ(new_ਗੰਢ, ਮਾਪਾ, n);
	ext2fs_rb_ਪਾਓ_color(new_ਗੰਢ, ਜੜ੍ਹ);
	bp->wcursor = new_ext;

	ਗੰਢ = ext2fs_rb_prev(new_ਗੰਢ);
	ਜੇ (ਗੰਢ) {
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) == ਸ਼ੁਰੂ) {
			ਸ਼ੁਰੂ = ext->ਸ਼ੁਰੂ;
			ਗਿਣਤੀ_2 += ext->ਗਿਣਤੀ_2;
			ext2fs_rb_erase(ਗੰਢ, ਜੜ੍ਹ);
			rb_free_extent(bp, ext);
		}
	}

skip_ਪਾਓ:
	/* See if we can merge extent to the right */
	ਲਈ (ਗੰਢ = ext2fs_rb_ਅਗਲਾ(new_ਗੰਢ); ਗੰਢ != ਨਿਰਾ; ਗੰਢ = ਅਗਲਾ) {
		ਅਗਲਾ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ);
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);

		ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) <= ਸ਼ੁਰੂ)
			ਜਾਰੀ;

		/* No more merging */
		ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) < ext->ਸ਼ੁਰੂ)
			ਰੋਕੋ;

		/* ext is embedded in new_ext interval */
		ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			ext2fs_rb_erase(ਗੰਢ, ਜੜ੍ਹ);
			rb_free_extent(bp, ext);
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ {
		/* merge ext with new_ext */
			ਗਿਣਤੀ_2 += ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) -
				  (ਸ਼ੁਰੂ + ਗਿਣਤੀ_2));
			ext2fs_rb_erase(ਗੰਢ, ਜੜ੍ਹ);
			rb_free_extent(bp, ext);
			ਰੋਕੋ;
		}
	}

	new_ext->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
	new_ext->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ rb_remove_extent(__u64 ਸ਼ੁਰੂ, __u64 ਗਿਣਤੀ_2,
			    ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp)
{
	ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ = &bp->ਜੜ੍ਹ;
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, **n = &ਜੜ੍ਹ->rb_ਗੰਢ;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ;
	ਬਣਤਰ bmap_rb_extent *ext;
	__u64 new_ਸ਼ੁਰੂ, new_ਗਿਣਤੀ;
	ਪੂਰਨ retval = 0;

	ਜੇ (ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(ਜੜ੍ਹ))
		ਵਾਪਸ 0;

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
			ਜਾਰੀ;
		}

		ਜੇ ((ਸ਼ੁਰੂ > ext->ਸ਼ੁਰੂ) &&
		    (ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) < (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			/* We have to split extent into two */
			new_ਸ਼ੁਰੂ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
			new_ਗਿਣਤੀ = (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) - new_ਸ਼ੁਰੂ;

			ext->ਗਿਣਤੀ_2 = ਸ਼ੁਰੂ - ext->ਸ਼ੁਰੂ;

			rb_ਪਾਓ_extent(new_ਸ਼ੁਰੂ, new_ਗਿਣਤੀ, bp);
			ਵਾਪਸ 1;
		}

		ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			ext->ਗਿਣਤੀ_2 = ਸ਼ੁਰੂ - ext->ਸ਼ੁਰੂ;
			retval = 1;
		}

		ਜੇ (0 == ext->ਗਿਣਤੀ_2) {
			ਮਾਪਾ = ext2fs_rb_ਅਗਲਾ(&ext->ਗੰਢ);
			ext2fs_rb_erase(&ext->ਗੰਢ, ਜੜ੍ਹ);
			rb_free_extent(bp, ext);
			ਰੋਕੋ;
		}

		ਜੇ (ਸ਼ੁਰੂ == ext->ਸ਼ੁਰੂ) {
			ext->ਸ਼ੁਰੂ += ਗਿਣਤੀ_2;
			ext->ਗਿਣਤੀ_2 -= ਗਿਣਤੀ_2;
			ਵਾਪਸ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	ਲਈ (; ਮਾਪਾ != ਨਿਰਾ; ਮਾਪਾ = ਗੰਢ) {
		ਗੰਢ = ext2fs_rb_ਅਗਲਾ(ਮਾਪਾ);
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) <= ਸ਼ੁਰੂ)
			ਜਾਰੀ;

		/* No more extents to be removed/truncated */
		ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) < ext->ਸ਼ੁਰੂ)
			ਰੋਕੋ;

		/* The entire extent is within the region to be removed */
		ਜੇ ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			ext2fs_rb_erase(ਮਾਪਾ, ਜੜ੍ਹ);
			rb_free_extent(bp, ext);
			retval = 1;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ {
			/* modify the last extent in region to be removed */
			ext->ਗਿਣਤੀ_2 -= ((ਸ਼ੁਰੂ + ਗਿਣਤੀ_2) - ext->ਸ਼ੁਰੂ);
			ext->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
			retval = 1;
			ਰੋਕੋ;
		}
	}

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਪੂਰਨ retval;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	arg -= bitmap->ਸ਼ੁਰੂ;

	retval = rb_ਪਾਓ_extent(arg, 1, bp);
	check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਪੂਰਨ retval;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	arg -= bitmap->ਸ਼ੁਰੂ;

	retval = rb_remove_extent(arg, 1, bp);
	check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);

	ਵਾਪਸ retval;
}

ਸਤਰਅੰਦਰ
ਸਥਿਰ ਪੂਰਨ rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	arg -= bitmap->ਸ਼ੁਰੂ;

	ਵਾਪਸ rb_test_ਦੋਆਧਾਰੀ_ਅੰਕ(bp, arg);
}

ਸਥਿਰ ਖਾਲੀ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	arg -= bitmap->ਸ਼ੁਰੂ;

	rb_ਪਾਓ_extent(arg, ਅੰਕ, bp);
	check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
}

ਸਥਿਰ ਖਾਲੀ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	arg -= bitmap->ਸ਼ੁਰੂ;

	rb_remove_extent(arg, ਅੰਕ, bp);
	check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
}

ਸਥਿਰ ਪੂਰਨ rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, **n;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ, *ਅਗਲਾ;
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਣਤਰ bmap_rb_extent *ext;
	ਪੂਰਨ retval = 1;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	n = &bp->ਜੜ੍ਹ.rb_ਗੰਢ;
	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;

	ਜੇ (len == 0 || ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&bp->ਜੜ੍ਹ))
		ਵਾਪਸ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
		} ਨਹੀਂਤਾਂ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			ਵਾਪਸ 0;
		}
	}

	ਗੰਢ = ਮਾਪਾ;
	ਜਦੋਂ (ਗੰਢ) {
		ਅਗਲਾ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ);
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		ਗੰਢ = ਅਗਲਾ;

		ਜੇ ((ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2) <= ਸ਼ੁਰੂ)
			ਜਾਰੀ;

		/* No more merging */
		ਜੇ ((ਸ਼ੁਰੂ + len) <= ext->ਸ਼ੁਰੂ)
			ਰੋਕੋ;

		retval = 0;
		ਰੋਕੋ;
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t rb_ਸੈੱਟ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਵਿੱਚ)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp = ਵਿੱਚ;
	size_t i;
	ਪੂਰਨ ਪਹਿਲਾ_ਸੈੱਟ = -1;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;

	ਲਈ (i = 0; i < ਅੰਕ; i++) {
		ਜੇ ((i & 7) == 0) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = cp[i/8];
			ਜੇ (c == 0xFF) {
				ਜੇ (ਪਹਿਲਾ_ਸੈੱਟ == -1)
					ਪਹਿਲਾ_ਸੈੱਟ = i;
				i += 7;
				ਜਾਰੀ;
			}
			ਜੇ ((c == 0x00) && (ਪਹਿਲਾ_ਸੈੱਟ == -1)) {
				i += 7;
				ਜਾਰੀ;
			}
		}
		ਜੇ (ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(i, ਵਿੱਚ)) {
			ਜੇ (ਪਹਿਲਾ_ਸੈੱਟ == -1)
				ਪਹਿਲਾ_ਸੈੱਟ = i;
			ਜਾਰੀ;
		}
		ਜੇ (ਪਹਿਲਾ_ਸੈੱਟ == -1)
			ਜਾਰੀ;

		rb_ਪਾਓ_extent(ਸ਼ੁਰੂ + ਪਹਿਲਾ_ਸੈੱਟ - bitmap->ਸ਼ੁਰੂ,
				 i - ਪਹਿਲਾ_ਸੈੱਟ, bp);
		check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
		ਪਹਿਲਾ_ਸੈੱਟ = -1;
	}
	ਜੇ (ਪਹਿਲਾ_ਸੈੱਟ != -1) {
		rb_ਪਾਓ_extent(ਸ਼ੁਰੂ + ਪਹਿਲਾ_ਸੈੱਟ - bitmap->ਸ਼ੁਰੂ,
				 ਅੰਕ - ਪਹਿਲਾ_ਸੈੱਟ, bp);
		check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਬਾਹਰਲਾ)
{

	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, *ਅਗਲਾ, **n;
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਣਤਰ bmap_rb_extent *ext;
	__u64 ਗਿਣਤੀ_2, pos;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	n = &bp->ਜੜ੍ਹ.rb_ਗੰਢ;
	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;

	ਜੇ (ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&bp->ਜੜ੍ਹ))
		ਵਾਪਸ 0;

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
		} ਨਹੀਂਤਾਂ
			ਰੋਕੋ;
	}

	memset(ਬਾਹਰਲਾ, 0, (ਅੰਕ + 7) >> 3);

	ਲਈ (; ਮਾਪਾ != ਨਿਰਾ; ਮਾਪਾ = ਅਗਲਾ) {
		ਅਗਲਾ = ext2fs_rb_ਅਗਲਾ(ਮਾਪਾ);
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);

		pos = ext->ਸ਼ੁਰੂ;
		ਗਿਣਤੀ_2 = ext->ਗਿਣਤੀ_2;
		ਜੇ (pos >= ਸ਼ੁਰੂ + ਅੰਕ)
			ਰੋਕੋ;
		ਜੇ (pos < ਸ਼ੁਰੂ) {
			ਜੇ (pos + ਗਿਣਤੀ_2 <  ਸ਼ੁਰੂ)
				ਜਾਰੀ;
			ਗਿਣਤੀ_2 -= ਸ਼ੁਰੂ - pos;
			pos = ਸ਼ੁਰੂ;
		}
		ਜੇ (pos + ਗਿਣਤੀ_2 > ਸ਼ੁਰੂ + ਅੰਕ)
			ਗਿਣਤੀ_2 = ਸ਼ੁਰੂ + ਅੰਕ - pos;

		ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
			ਜੇ ((ਗਿਣਤੀ_2 >= 8) &&
			    ((pos - ਸ਼ੁਰੂ) % 8) == 0) {
				ਪੂਰਨ nbytes = ਗਿਣਤੀ_2 >> 3;
				ਪੂਰਨ ਵਿੱਥ = (pos - ਸ਼ੁਰੂ) >> 3;

				memset(((ਅੱਖਰ *) ਬਾਹਰਲਾ) + ਵਿੱਥ, 0xFF, nbytes);
				pos += nbytes << 3;
				ਗਿਣਤੀ_2 -= nbytes << 3;
				ਜਾਰੀ;
			}
			ext2fs_fast_ਸੈੱਟ_bit64((pos - ਸ਼ੁਰੂ), ਬਾਹਰਲਾ);
			pos++;
			ਗਿਣਤੀ_2--;
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;

	rb_free_ਰੁੱਖ(&bp->ਜੜ੍ਹ);
	bp->rcursor = ਨਿਰਾ;
	bp->rcursor_ਅਗਲਾ = ਨਿਰਾ;
	bp->wcursor = ਨਿਰਾ;
	check_ਰੁੱਖ(&bp->ਜੜ੍ਹ, __func__);
}

ਸਥਿਰ errcode_t rb_ਲੱਭੋ_ਪਹਿਲਾ_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, **n;
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਣਤਰ bmap_rb_extent *ext;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	n = &bp->ਜੜ੍ਹ.rb_ਗੰਢ;
	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;
	ਸਿਰਾ -= bitmap->ਸ਼ੁਰੂ;

	ਜੇ (ਸ਼ੁਰੂ > ਸਿਰਾ)
		ਵਾਪਸ EINVAL;

	ਜੇ (ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&bp->ਜੜ੍ਹ))
		ਵਾਪਸ ENOENT;

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2 <= ਸਿਰਾ) {
			*ਬਾਹਰਲਾ = ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2 + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ
			ਵਾਪਸ ENOENT;
	}

	*ਬਾਹਰਲਾ = ਸ਼ੁਰੂ + bitmap->ਸ਼ੁਰੂ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t rb_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ rb_ਗੰਢ *ਮਾਪਾ = ਨਿਰਾ, **n;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ;
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਣਤਰ bmap_rb_extent *ext;

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;
	n = &bp->ਜੜ੍ਹ.rb_ਗੰਢ;
	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;
	ਸਿਰਾ -= bitmap->ਸ਼ੁਰੂ;

	ਜੇ (ਸ਼ੁਰੂ > ਸਿਰਾ)
		ਵਾਪਸ EINVAL;

	ਜੇ (ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&bp->ਜੜ੍ਹ))
		ਵਾਪਸ ENOENT;

	ਜਦੋਂ (*n) {
		ਮਾਪਾ = *n;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਮਾਪਾ);
		ਜੇ (ਸ਼ੁਰੂ < ext->ਸ਼ੁਰੂ) {
			n = &(*n)->rb_ਖੱਬਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸ਼ੁਰੂ >= (ext->ਸ਼ੁਰੂ + ext->ਗਿਣਤੀ_2)) {
			n = &(*n)->rb_ਸੱਜਾ;
		} ਨਹੀਂਤਾਂ {
			/* The start bit is set */
			*ਬਾਹਰਲਾ = ਸ਼ੁਰੂ + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
	}

	ਗੰਢ = ਮਾਪਾ;
	ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
	ਜੇ (ext->ਸ਼ੁਰੂ < ਸ਼ੁਰੂ) {
		ਗੰਢ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ);
		ਜੇ (ਗੰਢ == ਨਿਰਾ)
			ਵਾਪਸ ENOENT;
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
	}
	ਜੇ (ext->ਸ਼ੁਰੂ <= ਸਿਰਾ) {
		*ਬਾਹਰਲਾ = ext->ਸ਼ੁਰੂ + bitmap->ਸ਼ੁਰੂ;
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ENOENT;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
ਸਥਿਰ ਖਾਲੀ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *bp;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
	ਬਣਤਰ bmap_rb_extent *ext;
	__u64 ਗਿਣਤੀ_2 = 0;
	__u64 max_ਮਾਪ = 0;
	__u64 min_ਮਾਪ = ULONG_MAX;
	__u64 ਮਾਪ = 0, avg_ਮਾਪ = 0;
	ਦੁੱਗਣਾ eff;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ਦੁੱਗਣਾ m_ਪ੍ਰਾਪਤੀ = 0.0, t_ਪ੍ਰਾਪਤੀ = 0.0;
#ਜੇ_ਅੰਤ

	bp = (ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ *) bitmap->ਨਿਜੀ;

	ਲਈ (ਗੰਢ = ext2fs_rb_ਪਹਿਲਾ(&bp->ਜੜ੍ਹ); ਗੰਢ != ਨਿਰਾ;
	     ਗੰਢ = ext2fs_rb_ਅਗਲਾ(ਗੰਢ)) {
		ext = ਗੰਢ_ਮੰਜ਼ਿਲ_extent(ਗੰਢ);
		ਗਿਣਤੀ_2++;
		ਜੇ (ext->ਗਿਣਤੀ_2 > max_ਮਾਪ)
			max_ਮਾਪ = ext->ਗਿਣਤੀ_2;
		ਜੇ (ext->ਗਿਣਤੀ_2 < min_ਮਾਪ)
			min_ਮਾਪ = ext->ਗਿਣਤੀ_2;
		ਮਾਪ += ext->ਗਿਣਤੀ_2;
	}

	ਜੇ (ਗਿਣਤੀ_2)
		avg_ਮਾਪ = ਮਾਪ / ਗਿਣਤੀ_2;
	ਜੇ (min_ਮਾਪ == ULONG_MAX)
		min_ਮਾਪ = 0;
	eff = (ਦੁੱਗਣਾ)((ਗਿਣਤੀ_2 * ਆਕਾਰ(ਬਣਤਰ bmap_rb_extent)) << 3) /
	      (bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ਗਿਣਤੀ + bitmap->stats.mark_ext_ਗਿਣਤੀ;
	test_all = bitmap->stats.test_ਗਿਣਤੀ + bitmap->stats.test_ext_ਗਿਣਤੀ;
	ਜੇ (mark_all)
		m_ਪ੍ਰਾਪਤੀ = ((ਦੁੱਗਣਾ)bp->mark_ਪ੍ਰਾਪਤੀ / mark_all) * 100;
	ਜੇ (test_all)
		t_ਪ੍ਰਾਪਤੀ = ((ਦੁੱਗਣਾ)bp->test_ਪ੍ਰਾਪਤੀ / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_ਪ੍ਰਾਪਤੀ, t_ਪ੍ਰਾਪਤੀ, bp->mark_ਪ੍ਰਾਪਤੀ, m_ਪ੍ਰਾਪਤੀ);
#ਜੇ_ਅੰਤ
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਗਿਣਤੀ_2, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)
		((ਗਿਣਤੀ_2 * ਆਕਾਰ(ਬਣਤਰ bmap_rb_extent)) +
		 ਆਕਾਰ(ਬਣਤਰ ext2fs_rb_ਨਿੱਜੀ)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) min_ਮਾਪ);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) max_ਮਾਪ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) avg_ਮਾਪ);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਮਾਪ,
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਖਾਲੀ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
}
#ਜੇ_ਅੰਤ

ਬਣਤਰ ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.ਸੈੱਟ_bmap_range = rb_ਸੈੱਟ_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.ਲੱਭੋ_ਪਹਿਲਾ_zero = rb_ਲੱਭੋ_ਪਹਿਲਾ_zero,
	.ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ = rb_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ,
};
