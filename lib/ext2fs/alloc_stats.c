/*
 * alloc_stats.c --- Update allocation statistics for ext2fs
 *
 * Copyright (C) 2001 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

शून्य ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       पूर्णाङ्क inuse, पूर्णाङ्क isdir)
{
	पूर्णाङ्क	गणः = ext2fs_गणः_सम्बन्धि_ino(fs, ino);

	यदि (ino > fs->super->s_inodes_गणनम्) {
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (अचिह्नित दीर्घ) ino);
#यदि_अन्तः
		प्रत्यागम;
	}
	यदि (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	अन्यथा
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_गणनम्_समुच्चयः(fs, गणः, ext2fs_bg_free_inodes_गणनम्(fs, गणः) - inuse);
	यदि (isdir)
		ext2fs_bg_प्रयुक्तम्_dirs_गणनम्_समुच्चयः(fs, गणः, ext2fs_bg_प्रयुक्तम्_dirs_गणनम्(fs, गणः) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_INODE_UNINIT);
	यदि (ext2fs_has_गणः_desc_csum(fs)) {
		ext2_ino_t first_अप्रयुक्तम्_inode =	fs->super->s_inodes_प्रत्येकम्_गणः -
			ext2fs_bg_itable_अप्रयुक्तम्(fs, गणः) +
			गणः * fs->super->s_inodes_प्रत्येकम्_गणः + 1;

		यदि (ino >= first_अप्रयुक्तम्_inode)
			ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, गणः, गणः * fs->super->s_inodes_प्रत्येकम्_गणः + fs->super->s_inodes_प्रत्येकम्_गणः - ino);
		ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
	}

	fs->super->s_free_inodes_गणनम् -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

शून्य ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, पूर्णाङ्क inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

शून्य ext2fs_खण्डम्_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse)
{
	पूर्णाङ्क	गणः = ext2fs_गणः_सम्बन्धि_blk2(fs, blk);

	यदि (blk < fs->super->s_first_दत्त_खण्डम् ||
	    blk >= ext2fs_खण्डानि_गणनम्(fs->super)) {
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (अचिह्नित दीर्घ) blk);
#यदि_अन्तः
		प्रत्यागम;
	}
	यदि (inuse > 0)
		ext2fs_mark_खण्डम्_bitmap2(fs->खण्डम्_map, blk);
	अन्यथा
		ext2fs_unmark_खण्डम्_bitmap2(fs->खण्डम्_map, blk);
	ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, गणः, ext2fs_bg_free_खण्डानि_गणनम्(fs, गणः) - inuse);
	ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_खण्डम्_UNINIT);
	ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);

	ext2fs_free_खण्डानि_गणनम्_सङ्कलय(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_अनुपातः(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	यदि (fs->खण्डम्_alloc_stats)
		(fs->खण्डम्_alloc_stats)(fs, (blk64_t) blk, inuse);
}

शून्य ext2fs_खण्डम्_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णाङ्क inuse)
{
	ext2fs_खण्डम्_alloc_stats2(fs, blk, inuse);
}

शून्य ext2fs_समुच्चयः_खण्डम्_alloc_stats_callback(ext2_filsys fs,
					   शून्य (*func)(ext2_filsys fs,
							blk64_t blk,
							पूर्णाङ्क inuse),
					   शून्य (**old)(ext2_filsys fs,
							blk64_t blk,
							पूर्णाङ्क inuse))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;
	यदि (old)
		*old = fs->खण्डम्_alloc_stats;

	fs->खण्डम्_alloc_stats = func;
}

शून्य ext2fs_खण्डम्_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse)
{
	ध्रुव blk64_t orig_blk = blk;
	ध्रुव blk_t orig_अङ्कः = अङ्कः;

#यदि_अपरिभाषितम् OMIT_COM_ERR
	यदि (blk + अङ्कः > ext2fs_खण्डानि_गणनम्(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(अचिह्नित दीर्घ दीर्घ) blk, अङ्कः);
		प्रत्यागम;
	}
#यदि_अन्तः
	यदि (inuse == 0)
		प्रत्यागम;
	यदि (inuse > 0) {
		ext2fs_mark_खण्डम्_bitmap_range2(fs->खण्डम्_map, blk, अङ्कः);
		inuse = 1;
	} अन्यथा {
		ext2fs_unmark_खण्डम्_bitmap_range2(fs->खण्डम्_map, blk, अङ्कः);
		inuse = -1;
	}
	यावत् (अङ्कः) {
		पूर्णाङ्क गणः = ext2fs_गणः_सम्बन्धि_blk2(fs, blk);
		blk64_t चरमः_blk = ext2fs_गणः_चरमः_block2(fs, गणः);
		blk64_t n = अङ्कः;

		यदि (blk + अङ्कः > चरमः_blk)
			n = चरमः_blk - blk + 1;

		ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, गणः,
			ext2fs_bg_free_खण्डानि_गणनम्(fs, गणः) -
			inuse*n/EXT2_FS_CLUSTER_अनुपातः(fs));
		ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_खण्डम्_UNINIT);
		ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
		ext2fs_free_खण्डानि_गणनम्_सङ्कलय(fs->super, -inuse * (blk64_t) n);
		blk += n;
		अङ्कः -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	यदि (fs->खण्डम्_alloc_stats_range)
		(fs->खण्डम्_alloc_stats_range)(fs, orig_blk, orig_अङ्कः, inuse);
}

शून्य ext2fs_समुच्चयः_खण्डम्_alloc_stats_range_callback(ext2_filsys fs,
	शून्य (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse),
	शून्य (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अङ्कः, पूर्णाङ्क inuse))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;
	यदि (old)
		*old = fs->खण्डम्_alloc_stats_range;

	fs->खण्डम्_alloc_stats_range = func;
}
