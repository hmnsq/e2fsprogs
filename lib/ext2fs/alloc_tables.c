/*
 * alloc_tables.c --- Allocate tables for a newly initialized
 * filesystem.  Used by mke2fs when initializing a filesystem
 *
 * Copyright (C) 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
स्थिर blk64_t flexbg_विस्थापनम्(ext2_filsys fs, dgrp_t गणः, blk64_t start_blk,
			     ext2fs_खण्डम्_bitmap bmap, पूर्णाङ्क rem_grp,
			     पूर्णाङ्क elem_आकार)
{
	पूर्णाङ्क		flexbg, flexbg_आकार, आकार;
	blk64_t		चरमः_blk, first_free = 0;
	dgrp_t	       	चरमः_grp;

	flexbg_आकार = 1U << fs->super->s_log_groups_प्रत्येकम्_flex;
	flexbg = गणः / flexbg_आकार;
	आकार = rem_grp * elem_आकार;

	यदि (आकार > (पूर्णाङ्क) (fs->super->s_खण्डानि_प्रत्येकम्_गणः / 4))
		आकार = (पूर्णाङ्क) fs->super->s_खण्डानि_प्रत्येकम्_गणः / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	यदि (start_blk && start_blk < ext2fs_खण्डानि_गणनम्(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + आकार, elem_आकार,
				    bmap, &first_free) == 0)
		प्रत्यागम first_free;

	start_blk = ext2fs_गणः_first_block2(fs, flexbg_आकार * flexbg);
	चरमः_grp = गणः | (flexbg_आकार - 1);
	यदि (चरमः_grp > fs->गणः_desc_गणनम्-1)
		चरमः_grp = fs->गणः_desc_गणनम्-1;
	चरमः_blk = ext2fs_गणः_चरमः_block2(fs, चरमः_grp);

	/* Find the first available block */
	यदि (ext2fs_get_free_blocks2(fs, start_blk, चरमः_blk, आकार,
				    bmap, &first_free) == 0)
		प्रत्यागम first_free;

	यदि (ext2fs_get_free_blocks2(fs, start_blk, चरमः_blk, elem_आकार,
				   bmap, &first_free) == 0)
		प्रत्यागम first_free;

	यदि (ext2fs_get_free_blocks2(fs, 0, चरमः_blk, elem_आकार, bmap,
				    &first_free) == 0)
		प्रत्यागम first_free;

	प्रत्यागम first_free;
}

errcode_t ext2fs_allocate_गणः_सारणी(ext2_filsys fs, dgrp_t गणः,
				      ext2fs_खण्डम्_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		गणः_blk, start_blk, चरमः_blk, new_blk;
	dgrp_t		चरमः_grp = 0;
	पूर्णाङ्क		rem_grps = 0, flexbg_आकार = 0, सारणी_विस्थापनम् = 0;

	गणः_blk = ext2fs_गणः_first_block2(fs, गणः);
	चरमः_blk = ext2fs_गणः_चरमः_block2(fs, गणः);

	यदि (!bmap)
		bmap = fs->खण्डम्_map;

	यदि (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_प्रत्येकम्_flex) {
		flexbg_आकार = 1U << fs->super->s_log_groups_प्रत्येकम्_flex;
		चरमः_grp = गणः | (flexbg_आकार - 1);
		यदि (चरमः_grp > fs->गणः_desc_गणनम्-1)
			चरमः_grp = fs->गणः_desc_गणनम्-1;
		rem_grps = चरमः_grp - गणः + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	यदि (fs->stride && !flexbg_आकार) {
		retval = ext2fs_get_free_blocks2(fs, गणः_blk, चरमः_blk,
						 1, bmap, &start_blk);
		यदि (retval)
			प्रत्यागम retval;
		start_blk += fs->inode_खण्डानि_प्रत्येकम्_गणः;
		start_blk += ((fs->stride * गणः) %
			      (चरमः_blk - start_blk + 1));
		यदि (start_blk >= चरमः_blk)
			start_blk = गणः_blk;
	} अन्यथा
		start_blk = गणः_blk;

	यदि (flexbg_आकार) {
		blk64_t पूर्व_खण्डम् = 0;

		सारणी_विस्थापनम् = flexbg_आकार;
		यदि (गणः % flexbg_आकार)
			पूर्व_खण्डम् = ext2fs_खण्डम्_bitmap_loc(fs, गणः - 1) + 1;
		अन्यथा यदि (चरमः_grp == fs->गणः_desc_गणनम्-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			सारणी_विस्थापनम् = चरमः_grp & (flexbg_आकार - 1);
			यदि (सारणी_विस्थापनम् == 0)
				सारणी_विस्थापनम् = flexbg_आकार;
			अन्यथा
				सारणी_विस्थापनम्++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_विस्थापनम्(fs, गणः, पूर्व_खण्डम्, bmap,
					  rem_grps, 1);
		चरमः_blk = ext2fs_गणः_चरमः_block2(fs, चरमः_grp);
	}

	यदि (!ext2fs_खण्डम्_bitmap_loc(fs, गणः)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, चरमः_blk,
						 1, bmap, &new_blk);
		यदि (retval == EXT2_ET_खण्डम्_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, गणः_blk,
					चरमः_blk, 1, bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_mark_खण्डम्_bitmap2(bmap, new_blk);
		ext2fs_खण्डम्_bitmap_loc_समुच्चयः(fs, गणः, new_blk);
		यदि (flexbg_आकार) {
			dgrp_t gr = ext2fs_गणः_सम्बन्धि_blk2(fs, new_blk);
			ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, gr, ext2fs_bg_free_खण्डानि_गणनम्(fs, gr) - 1);
			ext2fs_free_खण्डानि_गणनम्_सङ्कलय(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_खण्डम्_UNINIT);
			ext2fs_गणः_desc_csum_समुच्चयः(fs, gr);
		}
	}

	यदि (flexbg_आकार) {
		blk64_t पूर्व_खण्डम् = 0;
		यदि (गणः % flexbg_आकार)
			पूर्व_खण्डम् = ext2fs_inode_bitmap_loc(fs, गणः - 1) + 1;
		अन्यथा
			पूर्व_खण्डम् = ext2fs_खण्डम्_bitmap_loc(fs, गणः) +
				सारणी_विस्थापनम्;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_विस्थापनम्(fs, गणः, पूर्व_खण्डम्, bmap,
					  rem_grps, 1);
		चरमः_blk = ext2fs_गणः_चरमः_block2(fs, चरमः_grp);
	}

	यदि (!ext2fs_inode_bitmap_loc(fs, गणः)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, चरमः_blk,
						 1, bmap, &new_blk);
		यदि (retval == EXT2_ET_खण्डम्_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, गणः_blk,
					 चरमः_blk, 1, bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_mark_खण्डम्_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_समुच्चयः(fs, गणः, new_blk);
		यदि (flexbg_आकार) {
			dgrp_t gr = ext2fs_गणः_सम्बन्धि_blk2(fs, new_blk);
			ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, gr, ext2fs_bg_free_खण्डानि_गणनम्(fs, gr) - 1);
			ext2fs_free_खण्डानि_गणनम्_सङ्कलय(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_खण्डम्_UNINIT);
			ext2fs_गणः_desc_csum_समुच्चयः(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	यदि (flexbg_आकार) {
		blk64_t पूर्व_खण्डम् = 0;

		यदि (गणः % flexbg_आकार)
			पूर्व_खण्डम् = ext2fs_inode_सारणी_loc(fs, गणः - 1) +
				fs->inode_खण्डानि_प्रत्येकम्_गणः;
		अन्यथा
			पूर्व_खण्डम् = ext2fs_inode_bitmap_loc(fs, गणः) +
				सारणी_विस्थापनम्;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		गणः_blk = flexbg_विस्थापनम्(fs, गणः, पूर्व_खण्डम्, bmap,
					  rem_grps, fs->inode_खण्डानि_प्रत्येकम्_गणः);
		चरमः_blk = ext2fs_गणः_चरमः_block2(fs, चरमः_grp);
	}

	यदि (!ext2fs_inode_सारणी_loc(fs, गणः)) {
		retval = ext2fs_get_free_blocks2(fs, गणः_blk, चरमः_blk,
						fs->inode_खण्डानि_प्रत्येकम्_गणः,
						bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;

		ext2fs_mark_खण्डम्_bitmap_range2(bmap,
			new_blk, fs->inode_खण्डानि_प्रत्येकम्_गणः);
		यदि (flexbg_आकार) {
			blk64_t अङ्कः, blk;
			अङ्कः = fs->inode_खण्डानि_प्रत्येकम्_गणः;
			blk = new_blk;
			यावत् (अङ्कः) {
				पूर्णाङ्क gr = ext2fs_गणः_सम्बन्धि_blk2(fs, blk);
				चरमः_blk = ext2fs_गणः_चरमः_block2(fs, gr);
				blk64_t n = अङ्कः;

				यदि (blk + अङ्कः > चरमः_blk)
					n = चरमः_blk - blk + 1;

				ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, gr,
					ext2fs_bg_free_खण्डानि_गणनम्(fs, gr) -
					n/EXT2_FS_CLUSTER_अनुपातः(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_खण्डम्_UNINIT);
				ext2fs_गणः_desc_csum_समुच्चयः(fs, gr);
				ext2fs_free_खण्डानि_गणनम्_सङ्कलय(fs->super, -n);
				blk += n;
				अङ्कः -= n;
			}
		}
		ext2fs_inode_सारणी_loc_समुच्चयः(fs, गणः, new_blk);
	}
	ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
	प्रत्यागम 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	संरचना ext2fs_numeric_प्रगतिः_struct प्रगतिः;

	यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->init)
		(fs->प्रगतिः_ops->init)(fs, &प्रगतिः, रिक्त,
					 fs->गणः_desc_गणनम्);

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->update)
			(fs->प्रगतिः_ops->update)(fs, &प्रगतिः, i);
		retval = ext2fs_allocate_गणः_सारणी(fs, i, fs->खण्डम्_map);
		यदि (retval)
			प्रत्यागम retval;
	}
	यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->close)
		(fs->प्रगतिः_ops->close)(fs, &प्रगतिः, रिक्त);
	प्रत्यागम 0;
}

