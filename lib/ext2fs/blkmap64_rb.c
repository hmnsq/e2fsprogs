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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_LINUX_TYPES_H
#അന്തര്ഭാവ <linux/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "bmap64.h"
#അന്തര്ഭാവ "rbtree.h"

#അന്തര്ഭാവ <limits.h>

ഘടന bmap_rb_extent {
	ഘടന rb_കണ്ണി കണ്ണി;
	__u64 start;
	__u64 എണ്ണം;
};

ഘടന ext2fs_rb_private {
	ഘടന rb_വേര് വേര്;
	ഘടന bmap_rb_extent *wcursor;
	ഘടന bmap_rb_extent *rcursor;
	ഘടന bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

വരിയിൽ സ്ഥിതം ഘടന bmap_rb_extent *കണ്ണി_to_extent(ഘടന rb_കണ്ണി *കണ്ണി)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	തിരികെ (ഘടന bmap_rb_extent *) കണ്ണി;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_തിരുകുക_extent(__u64 start, __u64 എണ്ണം,
			    ഘടന ext2fs_rb_private *);
സ്ഥിതം ശൂന്യം rb_get_new_extent(ഘടന bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
സ്ഥിതം ശൂന്യം print_വൃക്ഷം(ഘടന rb_വേര് *വേര്)
{
	ഘടന rb_കണ്ണി *കണ്ണി = ശൂന്യ;
	ഘടന bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	കണ്ണി = ext2fs_rb_first(വേര്);
	വേണ്ടി (കണ്ണി = ext2fs_rb_first(വേര്); കണ്ണി != ശൂന്യ; 
	     കണ്ണി = ext2fs_rb_next(കണ്ണി)) {
		ext = കണ്ണി_to_extent(കണ്ണി);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->എണ്ണം);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

സ്ഥിതം ശൂന്യം check_വൃക്ഷം(ഘടന rb_വേര് *വേര്, സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	ഘടന rb_കണ്ണി *കണ്ണി;
	ഘടന bmap_rb_extent *ext, *old = ശൂന്യ;

	വേണ്ടി (കണ്ണി = ext2fs_rb_first(വേര്); കണ്ണി;
	     കണ്ണി = ext2fs_rb_next(കണ്ണി)) {
		ext = കണ്ണി_to_extent(കണ്ണി);
		എങ്കിൽ (ext->എണ്ണം == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->എണ്ണം,
				(unsigned long long) ext->എണ്ണം);
			പോകുക err_out;
		}
		എങ്കിൽ (ext->start + ext->എണ്ണം < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->എണ്ണം,
				(unsigned long long) ext->എണ്ണം);
			പോകുക err_out;
		}

		എങ്കിൽ (old) {
			എങ്കിൽ (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->എണ്ണം,
					(unsigned long long) old->എണ്ണം);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->എണ്ണം,
					(unsigned long long) ext->എണ്ണം);
				പോകുക err_out;
			}
			എങ്കിൽ ((old->start + old->എണ്ണം) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->എണ്ണം,
					(unsigned long long) old->എണ്ണം);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->എണ്ണം,
					(unsigned long long) ext->എണ്ണം);
				പോകുക err_out;
			}
		}
		old = ext;
	}
	തിരികെ;

err_out:
	fprintf(stderr, "%s\n", സന്ദേശം);
	print_വൃക്ഷം(വേര്);
	exit(1);
}
#അല്ലെങ്കിൽ
#define check_വൃക്ഷം(വേര്, സന്ദേശം) ചെയ്യുക {} ആകുമ്പോൾ (0)
#define print_വൃക്ഷം(വേര്) ചെയ്യുക {} ആകുമ്പോൾ (0)
#endif

സ്ഥിതം ശൂന്യം rb_get_new_extent(ഘടന bmap_rb_extent **ext, __u64 start,
			     __u64 എണ്ണം)
{
	ഘടന bmap_rb_extent *new_ext;
	പൂർണ്ണസംഖ്യ retval;

	retval = ext2fs_get_mem(വലുപ്പം (ഘടന bmap_rb_extent),
				&new_ext);
	എങ്കിൽ (retval)
		abort();

	new_ext->start = start;
	new_ext->എണ്ണം = എണ്ണം;
	*ext = new_ext;
}

വരിയിൽ
സ്ഥിതം ശൂന്യം rb_free_extent(ഘടന ext2fs_rb_private *bp,
			   ഘടന bmap_rb_extent *ext)
{
	എങ്കിൽ (bp->wcursor == ext)
		bp->wcursor = ശൂന്യ;
	എങ്കിൽ (bp->rcursor == ext)
		bp->rcursor = ശൂന്യ;
	എങ്കിൽ (bp->rcursor_next == ext)
		bp->rcursor_next = ശൂന്യ;
	ext2fs_free_mem(&ext);
}

സ്ഥിതം errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ഘടന ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(വലുപ്പം (ഘടന ext2fs_rb_private), &bp);
	എങ്കിൽ (retval)
		തിരികെ retval;

	bp->വേര് = RB_വേര്;
	bp->rcursor = ശൂന്യ;
	bp->rcursor_next = ശൂന്യ;
	bp->wcursor = ശൂന്യ;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->സ്വകാര്യ = (ശൂന്യം *) bp;
	തിരികെ 0;
}

സ്ഥിതം errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം rb_free_വൃക്ഷം(ഘടന rb_വേര് *വേര്)
{
	ഘടന bmap_rb_extent *ext;
	ഘടന rb_കണ്ണി *കണ്ണി, *next;

	വേണ്ടി (കണ്ണി = ext2fs_rb_first(വേര്); കണ്ണി; കണ്ണി = next) {
		next = ext2fs_rb_next(കണ്ണി);
		ext = കണ്ണി_to_extent(കണ്ണി);
		ext2fs_rb_erase(കണ്ണി, വേര്);
		ext2fs_free_mem(&ext);
	}
}

സ്ഥിതം ശൂന്യം rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;

	rb_free_വൃക്ഷം(&bp->വേര്);
	ext2fs_free_mem(&bp);
	bp = 0;
}

സ്ഥിതം errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ഘടന ext2fs_rb_private *src_bp, *dest_bp;
	ഘടന bmap_rb_extent *src_ext, *dest_ext;
	ഘടന rb_കണ്ണി *dest_കണ്ണി, *src_കണ്ണി, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	എങ്കിൽ (retval)
		തിരികെ retval;

	src_bp = (ഘടന ext2fs_rb_private *) src->സ്വകാര്യ;
	dest_bp = (ഘടന ext2fs_rb_private *) dest->സ്വകാര്യ;
	src_bp->rcursor = ശൂന്യ;
	dest_bp->rcursor = ശൂന്യ;

	src_കണ്ണി = ext2fs_rb_first(&src_bp->വേര്);
	ആകുമ്പോൾ (src_കണ്ണി) {
		src_ext = കണ്ണി_to_extent(src_കണ്ണി);
		retval = ext2fs_get_mem(വലുപ്പം (ഘടന bmap_rb_extent),
					&dest_ext);
		എങ്കിൽ (retval)
			നിർത്തുക;

		memcpy(dest_ext, src_ext, വലുപ്പം(ഘടന bmap_rb_extent));

		dest_കണ്ണി = &dest_ext->കണ്ണി;
		n = &dest_bp->വേര്.rb_കണ്ണി;

		dest_last = ശൂന്യ;
		എങ്കിൽ (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->വേര്);
			n = &(dest_last)->rb_വലത്;
		}

		ext2fs_rb_link_കണ്ണി(dest_കണ്ണി, dest_last, n);
		ext2fs_rb_തിരുകുക_color(dest_കണ്ണി, &dest_bp->വേര്);

		src_കണ്ണി = ext2fs_rb_next(src_കണ്ണി);
	}

	തിരികെ retval;
}

സ്ഥിതം ശൂന്യം rb_truncate(__u64 new_max, ഘടന rb_വേര് *വേര്)
{
	ഘടന bmap_rb_extent *ext;
	ഘടന rb_കണ്ണി *കണ്ണി;

	കണ്ണി = ext2fs_rb_last(വേര്);
	ആകുമ്പോൾ (കണ്ണി) {
		ext = കണ്ണി_to_extent(കണ്ണി);

		എങ്കിൽ ((ext->start + ext->എണ്ണം - 1) <= new_max)
			നിർത്തുക;
		അല്ലെങ്കിൽ എങ്കിൽ (ext->start > new_max) {
			ext2fs_rb_erase(കണ്ണി, വേര്);
			ext2fs_free_mem(&ext);
			കണ്ണി = ext2fs_rb_last(വേര്);
			തുടരുക;
		} അല്ലെങ്കിൽ
			ext->എണ്ണം = new_max - ext->start + 1;
	}
}

സ്ഥിതം errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bmap->സ്വകാര്യ;
	bp->rcursor = ശൂന്യ;
	bp->wcursor = ശൂന്യ;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->വേര്);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	എങ്കിൽ (bmap->end < bmap->real_end)
		rb_തിരുകുക_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	തിരികെ 0;

}

വരിയിൽ സ്ഥിതം പൂർണ്ണസംഖ്യ
rb_test_bit(ഘടന ext2fs_rb_private *bp, __u64 bit)
{
	ഘടന bmap_rb_extent *rcursor, *next_ext = ശൂന്യ;
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, *next;
	ഘടന rb_കണ്ണി **n = &bp->വേര്.rb_കണ്ണി;
	ഘടന bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	എങ്കിൽ (!rcursor)
		പോകുക തിരയൽ_വൃക്ഷം;

	എങ്കിൽ (bit >= rcursor->start && bit < rcursor->start + rcursor->എണ്ണം) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		തിരികെ 1;
	}

	next_ext = bp->rcursor_next;
	എങ്കിൽ (!next_ext) {
		next = ext2fs_rb_next(&rcursor->കണ്ണി);
		എങ്കിൽ (next)
			next_ext = കണ്ണി_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	എങ്കിൽ (next_ext) {
		എങ്കിൽ ((bit >= rcursor->start + rcursor->എണ്ണം) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			തിരികെ 0;
		}
	}
	bp->rcursor = ശൂന്യ;
	bp->rcursor_next = ശൂന്യ;

	rcursor = bp->wcursor;
	എങ്കിൽ (!rcursor)
		പോകുക തിരയൽ_വൃക്ഷം;

	എങ്കിൽ (bit >= rcursor->start && bit < rcursor->start + rcursor->എണ്ണം)
		തിരികെ 1;

തിരയൽ_വൃക്ഷം:

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (bit < ext->start)
			n = &(*n)->rb_ഇടത്;
		അല്ലെങ്കിൽ എങ്കിൽ (bit >= (ext->start + ext->എണ്ണം))
			n = &(*n)->rb_വലത്;
		അല്ലെങ്കിൽ {
			bp->rcursor = ext;
			bp->rcursor_next = ശൂന്യ;
			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_തിരുകുക_extent(__u64 start, __u64 എണ്ണം,
			    ഘടന ext2fs_rb_private *bp)
{
	ഘടന rb_വേര് *വേര് = &bp->വേര്;
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, **n = &വേര്->rb_കണ്ണി;
	ഘടന rb_കണ്ണി *new_കണ്ണി, *കണ്ണി, *next;
	ഘടന bmap_rb_extent *new_ext;
	ഘടന bmap_rb_extent *ext;
	പൂർണ്ണസംഖ്യ retval = 0;

	എങ്കിൽ (എണ്ണം == 0)
		തിരികെ 0;

	bp->rcursor_next = ശൂന്യ;
	ext = bp->wcursor;
	എങ്കിൽ (ext) {
		എങ്കിൽ (start >= ext->start &&
		    start <= (ext->start + ext->എണ്ണം)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			പോകുക got_extent;
		}
	}

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);

		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (start > (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
		} അല്ലെങ്കിൽ {
got_extent:
			എങ്കിൽ ((start + എണ്ണം) <= (ext->start + ext->എണ്ണം))
				തിരികെ 1;

			എങ്കിൽ ((ext->start + ext->എണ്ണം) == start)
				retval = 0;
			അല്ലെങ്കിൽ
				retval = 1;

			എണ്ണം += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_കണ്ണി = &ext->കണ്ണി;

			പോകുക skip_തിരുകുക;
		}
	}

	rb_get_new_extent(&new_ext, start, എണ്ണം);

	new_കണ്ണി = &new_ext->കണ്ണി;
	ext2fs_rb_link_കണ്ണി(new_കണ്ണി, parent, n);
	ext2fs_rb_തിരുകുക_color(new_കണ്ണി, വേര്);
	bp->wcursor = new_ext;

	കണ്ണി = ext2fs_rb_prev(new_കണ്ണി);
	എങ്കിൽ (കണ്ണി) {
		ext = കണ്ണി_to_extent(കണ്ണി);
		എങ്കിൽ ((ext->start + ext->എണ്ണം) == start) {
			start = ext->start;
			എണ്ണം += ext->എണ്ണം;
			ext2fs_rb_erase(കണ്ണി, വേര്);
			rb_free_extent(bp, ext);
		}
	}

skip_തിരുകുക:
	/* See if we can merge extent to the right */
	വേണ്ടി (കണ്ണി = ext2fs_rb_next(new_കണ്ണി); കണ്ണി != ശൂന്യ; കണ്ണി = next) {
		next = ext2fs_rb_next(കണ്ണി);
		ext = കണ്ണി_to_extent(കണ്ണി);

		എങ്കിൽ ((ext->start + ext->എണ്ണം) <= start)
			തുടരുക;

		/* No more merging */
		എങ്കിൽ ((start + എണ്ണം) < ext->start)
			നിർത്തുക;

		/* ext is embedded in new_ext interval */
		എങ്കിൽ ((start + എണ്ണം) >= (ext->start + ext->എണ്ണം)) {
			ext2fs_rb_erase(കണ്ണി, വേര്);
			rb_free_extent(bp, ext);
			തുടരുക;
		} അല്ലെങ്കിൽ {
		/* merge ext with new_ext */
			എണ്ണം += ((ext->start + ext->എണ്ണം) -
				  (start + എണ്ണം));
			ext2fs_rb_erase(കണ്ണി, വേര്);
			rb_free_extent(bp, ext);
			നിർത്തുക;
		}
	}

	new_ext->start = start;
	new_ext->എണ്ണം = എണ്ണം;

	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_remove_extent(__u64 start, __u64 എണ്ണം,
			    ഘടന ext2fs_rb_private *bp)
{
	ഘടന rb_വേര് *വേര് = &bp->വേര്;
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, **n = &വേര്->rb_കണ്ണി;
	ഘടന rb_കണ്ണി *കണ്ണി;
	ഘടന bmap_rb_extent *ext;
	__u64 new_start, new_എണ്ണം;
	പൂർണ്ണസംഖ്യ retval = 0;

	എങ്കിൽ (ext2fs_rb_empty_വേര്(വേര്))
		തിരികെ 0;

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
			തുടരുക;
		} അല്ലെങ്കിൽ എങ്കിൽ (start >= (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
			തുടരുക;
		}

		എങ്കിൽ ((start > ext->start) &&
		    (start + എണ്ണം) < (ext->start + ext->എണ്ണം)) {
			/* We have to split extent into two */
			new_start = start + എണ്ണം;
			new_എണ്ണം = (ext->start + ext->എണ്ണം) - new_start;

			ext->എണ്ണം = start - ext->start;

			rb_തിരുകുക_extent(new_start, new_എണ്ണം, bp);
			തിരികെ 1;
		}

		എങ്കിൽ ((start + എണ്ണം) >= (ext->start + ext->എണ്ണം)) {
			ext->എണ്ണം = start - ext->start;
			retval = 1;
		}

		എങ്കിൽ (0 == ext->എണ്ണം) {
			parent = ext2fs_rb_next(&ext->കണ്ണി);
			ext2fs_rb_erase(&ext->കണ്ണി, വേര്);
			rb_free_extent(bp, ext);
			നിർത്തുക;
		}

		എങ്കിൽ (start == ext->start) {
			ext->start += എണ്ണം;
			ext->എണ്ണം -= എണ്ണം;
			തിരികെ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	വേണ്ടി (; parent != ശൂന്യ; parent = കണ്ണി) {
		കണ്ണി = ext2fs_rb_next(parent);
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ ((ext->start + ext->എണ്ണം) <= start)
			തുടരുക;

		/* No more extents to be removed/truncated */
		എങ്കിൽ ((start + എണ്ണം) < ext->start)
			നിർത്തുക;

		/* The entire extent is within the region to be removed */
		എങ്കിൽ ((start + എണ്ണം) >= (ext->start + ext->എണ്ണം)) {
			ext2fs_rb_erase(parent, വേര്);
			rb_free_extent(bp, ext);
			retval = 1;
			തുടരുക;
		} അല്ലെങ്കിൽ {
			/* modify the last extent in region to be removed */
			ext->എണ്ണം -= ((start + എണ്ണം) - ext->start);
			ext->start = start + എണ്ണം;
			retval = 1;
			നിർത്തുക;
		}
	}

	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ഘടന ext2fs_rb_private *bp;
	പൂർണ്ണസംഖ്യ retval;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	arg -= bitmap->start;

	retval = rb_തിരുകുക_extent(arg, 1, bp);
	check_വൃക്ഷം(&bp->വേര്, __func__);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ഘടന ext2fs_rb_private *bp;
	പൂർണ്ണസംഖ്യ retval;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_വൃക്ഷം(&bp->വേര്, __func__);

	തിരികെ retval;
}

വരിയിൽ
സ്ഥിതം പൂർണ്ണസംഖ്യ rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	arg -= bitmap->start;

	തിരികെ rb_test_bit(bp, arg);
}

സ്ഥിതം ശൂന്യം rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	arg -= bitmap->start;

	rb_തിരുകുക_extent(arg, അക്കം, bp);
	check_വൃക്ഷം(&bp->വേര്, __func__);
}

സ്ഥിതം ശൂന്യം rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	arg -= bitmap->start;

	rb_remove_extent(arg, അക്കം, bp);
	check_വൃക്ഷം(&bp->വേര്, __func__);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned പൂർണ്ണസംഖ്യ len)
{
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, **n;
	ഘടന rb_കണ്ണി *കണ്ണി, *next;
	ഘടന ext2fs_rb_private *bp;
	ഘടന bmap_rb_extent *ext;
	പൂർണ്ണസംഖ്യ retval = 1;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	n = &bp->വേര്.rb_കണ്ണി;
	start -= bitmap->start;

	എങ്കിൽ (len == 0 || ext2fs_rb_empty_വേര്(&bp->വേര്))
		തിരികെ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (start >= (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
		} അല്ലെങ്കിൽ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			തിരികെ 0;
		}
	}

	കണ്ണി = parent;
	ആകുമ്പോൾ (കണ്ണി) {
		next = ext2fs_rb_next(കണ്ണി);
		ext = കണ്ണി_to_extent(കണ്ണി);
		കണ്ണി = next;

		എങ്കിൽ ((ext->start + ext->എണ്ണം) <= start)
			തുടരുക;

		/* No more merging */
		എങ്കിൽ ((start + len) <= ext->start)
			നിർത്തുക;

		retval = 0;
		നിർത്തുക;
	}
	തിരികെ retval;
}

സ്ഥിതം errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t അക്കം, ശൂന്യം *in)
{
	ഘടന ext2fs_rb_private *bp;
	unsigned അക്ഷരം *cp = in;
	size_t i;
	പൂർണ്ണസംഖ്യ first_set = -1;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;

	വേണ്ടി (i = 0; i < അക്കം; i++) {
		എങ്കിൽ ((i & 7) == 0) {
			unsigned അക്ഷരം c = cp[i/8];
			എങ്കിൽ (c == 0xFF) {
				എങ്കിൽ (first_set == -1)
					first_set = i;
				i += 7;
				തുടരുക;
			}
			എങ്കിൽ ((c == 0x00) && (first_set == -1)) {
				i += 7;
				തുടരുക;
			}
		}
		എങ്കിൽ (ext2fs_test_bit(i, in)) {
			എങ്കിൽ (first_set == -1)
				first_set = i;
			തുടരുക;
		}
		എങ്കിൽ (first_set == -1)
			തുടരുക;

		rb_തിരുകുക_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_വൃക്ഷം(&bp->വേര്, __func__);
		first_set = -1;
	}
	എങ്കിൽ (first_set != -1) {
		rb_തിരുകുക_extent(start + first_set - bitmap->start,
				 അക്കം - first_set, bp);
		check_വൃക്ഷം(&bp->വേര്, __func__);
	}

	തിരികെ 0;
}

സ്ഥിതം errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t അക്കം, ശൂന്യം *out)
{

	ഘടന rb_കണ്ണി *parent = ശൂന്യ, *next, **n;
	ഘടന ext2fs_rb_private *bp;
	ഘടന bmap_rb_extent *ext;
	__u64 എണ്ണം, pos;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	n = &bp->വേര്.rb_കണ്ണി;
	start -= bitmap->start;

	എങ്കിൽ (ext2fs_rb_empty_വേര്(&bp->വേര്))
		തിരികെ 0;

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (start >= (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
		} അല്ലെങ്കിൽ
			നിർത്തുക;
	}

	memset(out, 0, (അക്കം + 7) >> 3);

	വേണ്ടി (; parent != ശൂന്യ; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = കണ്ണി_to_extent(parent);

		pos = ext->start;
		എണ്ണം = ext->എണ്ണം;
		എങ്കിൽ (pos >= start + അക്കം)
			നിർത്തുക;
		എങ്കിൽ (pos < start) {
			എങ്കിൽ (pos + എണ്ണം <  start)
				തുടരുക;
			എണ്ണം -= start - pos;
			pos = start;
		}
		എങ്കിൽ (pos + എണ്ണം > start + അക്കം)
			എണ്ണം = start + അക്കം - pos;

		ആകുമ്പോൾ (എണ്ണം > 0) {
			എങ്കിൽ ((എണ്ണം >= 8) &&
			    ((pos - start) % 8) == 0) {
				പൂർണ്ണസംഖ്യ nbytes = എണ്ണം >> 3;
				പൂർണ്ണസംഖ്യ offset = (pos - start) >> 3;

				memset(((അക്ഷരം *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				എണ്ണം -= nbytes << 3;
				തുടരുക;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			എണ്ണം--;
		}
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ഘടന ext2fs_rb_private *bp;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;

	rb_free_വൃക്ഷം(&bp->വേര്);
	bp->rcursor = ശൂന്യ;
	bp->rcursor_next = ശൂന്യ;
	bp->wcursor = ശൂന്യ;
	check_വൃക്ഷം(&bp->വേര്, __func__);
}

സ്ഥിതം errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, **n;
	ഘടന ext2fs_rb_private *bp;
	ഘടന bmap_rb_extent *ext;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	n = &bp->വേര്.rb_കണ്ണി;
	start -= bitmap->start;
	end -= bitmap->start;

	എങ്കിൽ (start > end)
		തിരികെ EINVAL;

	എങ്കിൽ (ext2fs_rb_empty_വേര്(&bp->വേര്))
		തിരികെ ENOENT;

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (start >= (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (ext->start + ext->എണ്ണം <= end) {
			*out = ext->start + ext->എണ്ണം + bitmap->start;
			തിരികെ 0;
		} അല്ലെങ്കിൽ
			തിരികെ ENOENT;
	}

	*out = start + bitmap->start;
	തിരികെ 0;
}

സ്ഥിതം errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	ഘടന rb_കണ്ണി *parent = ശൂന്യ, **n;
	ഘടന rb_കണ്ണി *കണ്ണി;
	ഘടന ext2fs_rb_private *bp;
	ഘടന bmap_rb_extent *ext;

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;
	n = &bp->വേര്.rb_കണ്ണി;
	start -= bitmap->start;
	end -= bitmap->start;

	എങ്കിൽ (start > end)
		തിരികെ EINVAL;

	എങ്കിൽ (ext2fs_rb_empty_വേര്(&bp->വേര്))
		തിരികെ ENOENT;

	ആകുമ്പോൾ (*n) {
		parent = *n;
		ext = കണ്ണി_to_extent(parent);
		എങ്കിൽ (start < ext->start) {
			n = &(*n)->rb_ഇടത്;
		} അല്ലെങ്കിൽ എങ്കിൽ (start >= (ext->start + ext->എണ്ണം)) {
			n = &(*n)->rb_വലത്;
		} അല്ലെങ്കിൽ {
			/* The start bit is set */
			*out = start + bitmap->start;
			തിരികെ 0;
		}
	}

	കണ്ണി = parent;
	ext = കണ്ണി_to_extent(കണ്ണി);
	എങ്കിൽ (ext->start < start) {
		കണ്ണി = ext2fs_rb_next(കണ്ണി);
		എങ്കിൽ (കണ്ണി == ശൂന്യ)
			തിരികെ ENOENT;
		ext = കണ്ണി_to_extent(കണ്ണി);
	}
	എങ്കിൽ (ext->start <= end) {
		*out = ext->start + bitmap->start;
		തിരികെ 0;
	}
	തിരികെ ENOENT;
}

#ifdef ENABLE_BMAP_STATS
സ്ഥിതം ശൂന്യം rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ഘടന ext2fs_rb_private *bp;
	ഘടന rb_കണ്ണി *കണ്ണി = ശൂന്യ;
	ഘടന bmap_rb_extent *ext;
	__u64 എണ്ണം = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	ഇരട്ട eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ഇരട്ട m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (ഘടന ext2fs_rb_private *) bitmap->സ്വകാര്യ;

	വേണ്ടി (കണ്ണി = ext2fs_rb_first(&bp->വേര്); കണ്ണി != ശൂന്യ;
	     കണ്ണി = ext2fs_rb_next(കണ്ണി)) {
		ext = കണ്ണി_to_extent(കണ്ണി);
		എണ്ണം++;
		എങ്കിൽ (ext->എണ്ണം > max_size)
			max_size = ext->എണ്ണം;
		എങ്കിൽ (ext->എണ്ണം < min_size)
			min_size = ext->എണ്ണം;
		size += ext->എണ്ണം;
	}

	എങ്കിൽ (എണ്ണം)
		avg_size = size / എണ്ണം;
	എങ്കിൽ (min_size == ULONG_MAX)
		min_size = 0;
	eff = (ഇരട്ട)((എണ്ണം * വലുപ്പം(ഘടന bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_എണ്ണം + bitmap->stats.mark_ext_എണ്ണം;
	test_all = bitmap->stats.test_എണ്ണം + bitmap->stats.test_ext_എണ്ണം;
	എങ്കിൽ (mark_all)
		m_hit = ((ഇരട്ട)bp->mark_hit / mark_all) * 100;
	എങ്കിൽ (test_all)
		t_hit = ((ഇരട്ട)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) എണ്ണം, (unsigned long long)
		((എണ്ണം * വലുപ്പം(ഘടന bmap_rb_extent)) +
		 വലുപ്പം(ഘടന ext2fs_rb_private)));
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
#അല്ലെങ്കിൽ
സ്ഥിതം ശൂന്യം rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

ഘടന ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
