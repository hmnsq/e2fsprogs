/*
 * extent.c --- routines to implement extents support
 *
 * Copyright (C) 2007 Theodore Ts'o.
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
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "e2image.h"

#अपरिभाषय DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

संरचना extent_मार्गः {
	वर्ण		*buf;
	पूर्णाङ्क		entries;
	पूर्णाङ्क		max_entries;
	पूर्णाङ्क		वामः;
	पूर्णाङ्क		दर्शय_अङ्कः;
	पूर्णाङ्क		flags;
	blk64_t		अन्तः_blk;
	blk64_t		blk;
	शून्य		*curr;
};


संरचना ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	संरचना ext2_inode	*inode;
	संरचना ext2_inode	inodebuf;
	पूर्णाङ्क			type;
	पूर्णाङ्क			level;
	पूर्णाङ्क			max_गभीरता;
	पूर्णाङ्क			max_paths;
	संरचना extent_मार्गः	*मार्गः;
};

संरचना ext2_extent_मार्गः {
	errcode_t		magic;
	पूर्णाङ्क			पर्णम्_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#यदि_परिभाषितम् DEBUG
स्थिर शून्य dbg_show_शीर्षकम्(संरचना ext3_extent_शीर्षकम् *eh)
{
	प्रारूप_लिख("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_गन्तव्यम्_cpu(eh->eh_magic),
			ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries),
			ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max),
			ext2fs_le16_गन्तव्यम्_cpu(eh->eh_गभीरता),
			ext2fs_le32_गन्तव्यम्_cpu(eh->eh_generation));
}

स्थिर शून्य dbg_show_सूचकांक(संरचना ext3_extent_idx *ix)
{
	प्रारूप_लिख("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_गन्तव्यम्_cpu(ix->ei_खण्डम्),
			ext2fs_le32_गन्तव्यम्_cpu(ix->ei_पर्णम्),
			ext2fs_le16_गन्तव्यम्_cpu(ix->ei_पर्णम्_hi),
			ext2fs_le16_गन्तव्यम्_cpu(ix->ei_अप्रयुक्तम्));
}

स्थिर शून्य dbg_show_extent(संरचना ext3_extent *ex)
{
	प्रारूप_लिख("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_गन्तव्यम्_cpu(ex->ee_खण्डम्),
			ext2fs_le32_गन्तव्यम्_cpu(ex->ee_खण्डम्) +
			ext2fs_le16_गन्तव्यम्_cpu(ex->ee_len) - 1,
			ext2fs_le16_गन्तव्यम्_cpu(ex->ee_len),
			ext2fs_le32_गन्तव्यम्_cpu(ex->ee_start),
			ext2fs_le16_गन्तव्यम्_cpu(ex->ee_start_hi));
}

स्थिर शून्य dbg_print_extent(वर्ण *desc, संरचना ext2fs_extent *extent)
{
	यदि (desc)
		प्रारूप_लिख("%s: ", desc);
	प्रारूप_लिख("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_पर्णम्)
		fputs("LEAF ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_दर्शय)
		fputs("2ND_VISIT ", stdout);
	यदि (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

स्थिर शून्य dump_मार्गः(ध्रुव वर्ण *चिह्नकम्, संरचना ext2_extent_handle *handle,
		      संरचना extent_मार्गः *मार्गः)
{
	संरचना extent_मार्गः *ppp = मार्गः;
	प्रारूप_लिख("%s: level=%d\n", चिह्नकम्, handle->level);

	कुरु {
		प्रारूप_लिख("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       चिह्नकम्, (ppp - handle->मार्गः), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->वामः, ppp->दर्शय_अङ्कः, ppp->flags,
		       ppp->अन्तः_blk, ppp->curr, ppp->curr - (शून्य *)ppp->buf);
		प्रारूप_लिख("  ");
		dbg_show_शीर्षकम्((संरचना ext3_extent_शीर्षकम् *)ppp->buf);
		यदि (ppp->curr) {
			प्रारूप_लिख("  ");
			dbg_show_सूचकांक(ppp->curr);
			प्रारूप_लिख("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} यावत् (ppp >= handle->मार्गः);
	fflush(stdout);

	प्रत्यागम;
}

#अन्यथा
#परिभाषय dbg_show_शीर्षकम्(eh) कुरु { } यावत् (0)
#परिभाषय dbg_show_सूचकांक(ix) कुरु { } यावत् (0)
#परिभाषय dbg_show_extent(ex) कुरु { } यावत् (0)
#परिभाषय dbg_print_extent(desc, ex) कुरु { } यावत् (0)
#परिभाषय dump_मार्गः(चिह्नकम्, handle, मार्गः) कुरु { } यावत् (0)
#यदि_अन्तः

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_शीर्षकम्_verify(शून्य *ptr, पूर्णाङ्क आकार)
{
	पूर्णाङ्क eh_max, प्रविष्टिः_आकार;
	संरचना ext3_extent_शीर्षकम् *eh = ptr;

	dbg_show_शीर्षकम्(eh);
	यदि (ext2fs_le16_गन्तव्यम्_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		प्रत्यागम EXT2_ET_EXTENT_शीर्षकम्_BAD;
	यदि (ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries) > ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max))
		प्रत्यागम EXT2_ET_EXTENT_शीर्षकम्_BAD;
	यदि (eh->eh_गभीरता == 0)
		प्रविष्टिः_आकार = परिमाण(संरचना ext3_extent);
	अन्यथा
		प्रविष्टिः_आकार = परिमाण(संरचना ext3_extent_idx);

	eh_max = (आकार - परिमाण(*eh)) / प्रविष्टिः_आकार;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	यदि ((ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max) < (eh_max - 2)))
		प्रत्यागम EXT2_ET_EXTENT_शीर्षकम्_BAD;

	प्रत्यागम 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
शून्य ext2fs_extent_free(ext2_extent_handle_t handle)
{
	पूर्णाङ्क			i;

	यदि (!handle)
		प्रत्यागम;

	यदि (handle->मार्गः) {
		प्रति (i = 1; i < handle->max_paths; i++) {
			यदि (handle->मार्गः[i].buf)
				ext2fs_free_mem(&handle->मार्गः[i].buf);
		}
		ext2fs_free_mem(&handle->मार्गः);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	प्रत्यागम ext2fs_extent_open2(fs, ino, रिक्त, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    संरचना ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	संरचना ext2_extent_handle	*handle;
	errcode_t			retval;
	पूर्णाङ्क				i;
	संरचना ext3_extent_शीर्षकम्	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!inode)
		यदि ((ino == 0) || (ino > fs->super->s_inodes_गणनम्))
			प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_extent_handle), &handle);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(handle, 0, परिमाण(संरचना ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	यदि (inode) {
		handle->inode = inode;
	} अन्यथा {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		यदि (retval)
			गच्छ errout;
	}

	eh = (संरचना ext3_extent_शीर्षकम् *) &handle->inode->i_खण्डम्[0];

	प्रति (i=0; i < EXT2_N_खण्डानि; i++)
		यदि (handle->inode->i_खण्डम्[i])
			भङ्ग;
	यदि (i >= EXT2_N_खण्डानि) {
		eh->eh_magic = ext2fs_cpu_गन्तव्यम्_le16(EXT3_EXT_MAGIC);
		eh->eh_गभीरता = 0;
		eh->eh_entries = 0;
		i = (परिमाण(handle->inode->i_खण्डम्) - परिमाण(*eh)) /
			परिमाण(संरचना ext3_extent);
		eh->eh_max = ext2fs_cpu_गन्तव्यम्_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	यदि (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		गच्छ errout;
	}

	retval = ext2fs_extent_शीर्षकम्_verify(eh, परिमाण(handle->inode->i_खण्डम्));
	यदि (retval)
		गच्छ errout;

	handle->max_गभीरता = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_गभीरता);
	handle->type = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_magic);

	handle->max_paths = handle->max_गभीरता + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    परिमाण(संरचना extent_मार्गः),
				    &handle->मार्गः);
	handle->मार्गः[0].buf = (वर्ण *) handle->inode->i_खण्डम्;

	handle->मार्गः[0].वामः = handle->मार्गः[0].entries =
		ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries);
	handle->मार्गः[0].max_entries = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max);
	handle->मार्गः[0].curr = 0;
	handle->मार्गः[0].अन्तः_blk =
		(EXT2_I_आकार(handle->inode) + fs->blocksize - 1) >>
		 EXT2_खण्डम्_आकार_द्वयङ्काः(fs->super);
	handle->मार्गः[0].blk = 0;
	handle->मार्गः[0].दर्शय_अङ्कः = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	प्रत्यागम 0;

errout:
	ext2fs_extent_free(handle);
	प्रत्यागम retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    पूर्णाङ्क flags, संरचना ext2fs_extent *extent)
{
	संरचना extent_मार्गः	*मार्गः, *newpath, *tp;
	संरचना ext3_extent_शीर्षकम्	*eh;
	संरचना ext3_extent_idx		*ix = 0;
	संरचना ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				अन्तः_blk;
	पूर्णाङ्क				orig_op, op, l;
	पूर्णाङ्क				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	orig_op = op = flags & EXT2_EXTENT_MOVE_आवरणम्;

retry:
	मार्गः = handle->मार्गः + handle->level;
	यदि ((orig_op == EXT2_EXTENT_अग्र) ||
	    (orig_op == EXT2_EXTENT_अग्र_पर्णम्)) {
		यदि (handle->level < handle->max_गभीरता) {
			/* interior node */
			यदि (मार्गः->दर्शय_अङ्कः == 0) {
				मार्गः->दर्शय_अङ्कः++;
				op = EXT2_EXTENT_अधस्तात्;
			} अन्यथा यदि (मार्गः->वामः > 0)
				op = EXT2_EXTENT_अग्र_SIB;
			अन्यथा यदि (handle->level > 0)
				op = EXT2_EXTENT_ऊर्ध्वम्;
			अन्यथा
				प्रत्यागम EXT2_ET_EXTENT_NO_अग्र;
		} अन्यथा {
			/* leaf node */
			यदि (मार्गः->वामः > 0)
				op = EXT2_EXTENT_अग्र_SIB;
			अन्यथा यदि (handle->level > 0)
				op = EXT2_EXTENT_ऊर्ध्वम्;
			अन्यथा
				प्रत्यागम EXT2_ET_EXTENT_NO_अग्र;
		}
		यदि (op != EXT2_EXTENT_अग्र_SIB) {
#यदि_परिभाषितम् DEBUG_GET_EXTENT
			प्रारूप_लिख("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_अधस्तात्) ? "down" :
			       ((op == EXT2_EXTENT_ऊर्ध्वम्) ? "up" : "unknown"));
#यदि_अन्तः
		}
	}

	यदि ((orig_op == EXT2_EXTENT_पूर्व) ||
	    (orig_op == EXT2_EXTENT_पूर्व_पर्णम्)) {
		यदि (handle->level < handle->max_गभीरता) {
			/* interior node */
			यदि (मार्गः->दर्शय_अङ्कः > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_अधस्तात्_AND_चरमः;
			} अन्यथा यदि (मार्गः->वामः < मार्गः->entries-1)
				op = EXT2_EXTENT_पूर्व_SIB;
			अन्यथा यदि (handle->level > 0)
				op = EXT2_EXTENT_ऊर्ध्वम्;
			अन्यथा
				प्रत्यागम EXT2_ET_EXTENT_NO_पूर्व;
		} अन्यथा {
			/* leaf node */
			यदि (मार्गः->वामः < मार्गः->entries-1)
				op = EXT2_EXTENT_पूर्व_SIB;
			अन्यथा यदि (handle->level > 0)
				op = EXT2_EXTENT_ऊर्ध्वम्;
			अन्यथा
				प्रत्यागम EXT2_ET_EXTENT_NO_पूर्व;
		}
		यदि (op != EXT2_EXTENT_पूर्व_SIB) {
#यदि_परिभाषितम् DEBUG_GET_EXTENT
			प्रारूप_लिख("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_अधस्तात्_AND_चरमः) ? "down/last" :
			       ((op == EXT2_EXTENT_ऊर्ध्वम्) ? "up" : "unknown"));
#यदि_अन्तः
		}
	}

	यदि (orig_op == EXT2_EXTENT_चरमः_पर्णम्) {
		यदि ((handle->level < handle->max_गभीरता) &&
		    (मार्गः->वामः == 0))
			op = EXT2_EXTENT_अधस्तात्;
		अन्यथा
			op = EXT2_EXTENT_चरमः_SIB;
#यदि_परिभाषितम् DEBUG_GET_EXTENT
		प्रारूप_लिख("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_अधस्तात्) ? "down" : "last_sib");
#यदि_अन्तः
	}

	विकल्प (op) {
	पक्ष EXT2_EXTENT_CURRENT:
		ix = मार्गः->curr;
		भङ्ग;
	पक्ष EXT2_EXTENT_मूलम्:
		handle->level = 0;
		मार्गः = handle->मार्गः + handle->level;
		/* fallthrough */
	पक्ष EXT2_EXTENT_FIRST_SIB:
		मार्गः->वामः = मार्गः->entries;
		मार्गः->curr = 0;
		/* fallthrough */
	पक्ष EXT2_EXTENT_अग्र_SIB:
		यदि (मार्गः->वामः <= 0)
			प्रत्यागम EXT2_ET_EXTENT_NO_अग्र;
		यदि (मार्गः->curr) {
			ix = मार्गः->curr;
			ix++;
		} अन्यथा {
			eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
			ix = EXT_FIRST_सूचकांक(eh);
		}
		मार्गः->वामः--;
		मार्गः->curr = ix;
		मार्गः->दर्शय_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_पूर्व_SIB:
		यदि (!मार्गः->curr ||
		    मार्गः->वामः+1 >= मार्गः->entries)
			प्रत्यागम EXT2_ET_EXTENT_NO_पूर्व;
		ix = मार्गः->curr;
		ix--;
		मार्गः->curr = ix;
		मार्गः->वामः++;
		यदि (handle->level < handle->max_गभीरता)
			मार्गः->दर्शय_अङ्कः = 1;
		भङ्ग;
	पक्ष EXT2_EXTENT_चरमः_SIB:
		eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
		मार्गः->curr = EXT_चरमः_EXTENT(eh);
		ix = मार्गः->curr;
		मार्गः->वामः = 0;
		मार्गः->दर्शय_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_ऊर्ध्वम्:
		यदि (handle->level <= 0)
			प्रत्यागम EXT2_ET_EXTENT_NO_ऊर्ध्वम्;
		handle->level--;
		मार्गः--;
		ix = मार्गः->curr;
		यदि ((orig_op == EXT2_EXTENT_पूर्व) ||
		    (orig_op == EXT2_EXTENT_पूर्व_पर्णम्))
			मार्गः->दर्शय_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_अधस्तात्:
	पक्ष EXT2_EXTENT_अधस्तात्_AND_चरमः:
		यदि (!मार्गः->curr ||(handle->level >= handle->max_गभीरता))
			प्रत्यागम EXT2_ET_EXTENT_NO_अधस्तात्;

		ix = मार्गः->curr;
		newpath = मार्गः + 1;
		यदि (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			यदि (retval)
				प्रत्यागम retval;
		}
		blk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_पर्णम्) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ix->ei_पर्णम्_hi) << 32);
		प्रति (l = handle->level, tp = मार्गः; l > 0; l--, tp--) {
			यदि (blk == tp->blk)
				प्रत्यागम EXT2_ET_EXTENT_आवृत्तिः;
		}
		newpath->blk = blk;
		यदि ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			स्मृतिं_पूरय(newpath->buf, 0, handle->fs->blocksize);
		अन्यथा {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			यदि (retval)
				प्रत्यागम retval;
		}
		handle->level++;

		eh = (संरचना ext3_extent_शीर्षकम् *) newpath->buf;

		retval = ext2fs_extent_शीर्षकम्_verify(eh, handle->fs->blocksize);
		यदि (retval) {
			handle->level--;
			प्रत्यागम retval;
		}

		यदि (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_खण्डम्_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->वामः = newpath->entries =
			ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		यदि (newpath->वामः <= 0)
			प्रत्यागम EXT2_ET_EXTENT_NO_अधस्तात्;

		यदि (मार्गः->वामः > 0) {
			ix++;
			newpath->अन्तः_blk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_खण्डम्);
		} अन्यथा
			newpath->अन्तः_blk = मार्गः->अन्तः_blk;

		मार्गः = newpath;
		यदि (op == EXT2_EXTENT_अधस्तात्) {
			ix = EXT_FIRST_सूचकांक((संरचना ext3_extent_शीर्षकम् *) eh);
			मार्गः->curr = ix;
			मार्गः->वामः = मार्गः->entries - 1;
			मार्गः->दर्शय_अङ्कः = 0;
		} अन्यथा {
			ix = EXT_चरमः_सूचकांक((संरचना ext3_extent_शीर्षकम् *) eh);
			मार्गः->curr = ix;
			मार्गः->वामः = 0;
			यदि (handle->level < handle->max_गभीरता)
				मार्गः->दर्शय_अङ्कः = 1;
		}
#यदि_परिभाषितम् DEBUG_GET_EXTENT
		प्रारूप_लिख("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_गभीरता,
			   मार्गः->अन्तः_blk);
#यदि_अन्तः
		भङ्ग;
	स्वभाव:
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
	}

	यदि (!ix)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	extent->e_flags = 0;
#यदि_परिभाषितम् DEBUG_GET_EXTENT
	प्रारूप_लिख("(Left %d)\n", मार्गः->वामः);
#यदि_अन्तः

	यदि (handle->level == handle->max_गभीरता) {
		ex = (संरचना ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_गन्तव्यम्_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_गन्तव्यम्_cpu(ex->ee_खण्डम्);
		extent->e_len = ext2fs_le16_गन्तव्यम्_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_पर्णम्;
		यदि (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} अन्यथा {
		extent->e_pblk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_पर्णम्) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ix->ei_पर्णम्_hi) << 32);
		extent->e_lblk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_खण्डम्);
		यदि (मार्गः->वामः > 0) {
			ix++;
			अन्तः_blk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_खण्डम्);
		} अन्यथा
			अन्तः_blk = मार्गः->अन्तः_blk;

		extent->e_len = अन्तः_blk - extent->e_lblk;
	}
	यदि (मार्गः->दर्शय_अङ्कः)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_दर्शय;

	यदि (((orig_op == EXT2_EXTENT_अग्र_पर्णम्) ||
	     (orig_op == EXT2_EXTENT_पूर्व_पर्णम्)) &&
	    (handle->level != handle->max_गभीरता))
		गच्छ retry;

	यदि ((orig_op == EXT2_EXTENT_चरमः_पर्णम्) &&
	    ((handle->level != handle->max_गभीरता) ||
	     (मार्गः->वामः != 0)))
		गच्छ retry;

	यदि (failed_csum)
		प्रत्यागम EXT2_ET_EXTENT_CSUM_INVALID;

	प्रत्यागम 0;
}

स्थिर errcode_t update_मार्गः(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_शीर्षकम्	*eh;

	यदि (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} अन्यथा {
		ix = handle->मार्गः[handle->level - 1].curr;
		blk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_पर्णम्) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ix->ei_पर्णम्_hi) << 32);

		/* then update the checksum */
		eh = (संरचना ext3_extent_शीर्षकम् *)
				handle->मार्गः[handle->level].buf;
		retval = ext2fs_extent_खण्डम्_csum_समुच्चयः(handle->fs, handle->ino,
						      eh);
		यदि (retval)
			प्रत्यागम retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->मार्गः[handle->level].buf);
	}
	प्रत्यागम retval;
}

#यदि 0
errcode_t ext2fs_extent_save_मार्गः(ext2_extent_handle_t handle,
				  ext2_extent_मार्गः_t *ret_मार्गः)
{
	ext2_extent_मार्गः_t	save_मार्गः;
	संरचना ext2fs_extent	extent;
	संरचना ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_extent_get_info(handle, &info);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_extent_मार्गः), &save_मार्गः);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(save_मार्गः, 0, परिमाण(संरचना ext2_extent_मार्गः));

	save_मार्गः->magic = EXT2_ET_MAGIC_EXTENT_मार्गः;
	save_मार्गः->पर्णम्_height = info.max_गभीरता - info.curr_level - 1;
	save_मार्गः->lblk = extent.e_lblk;

	*ret_मार्गः = save_मार्गः;
	प्रत्यागम 0;
}

errcode_t ext2fs_extent_free_मार्गः(ext2_extent_मार्गः_t मार्गः)
{
	EXT2_CHECK_MAGIC(मार्गः, EXT2_ET_MAGIC_EXTENT_मार्गः);

	ext2fs_free_mem(&मार्गः);
	प्रत्यागम 0;
}
#यदि_अन्तः

/*
 * Go to the node at leaf_level which contains logical block blk.
 *
 * leaf_level is height from the leaf node level, i.e.
 * leaf_level 0 is at leaf node, leaf_level 1 is 1 above etc.
 *
 * If "blk" has no mapping (hole) then handle is left at last
 * extent before blk.
 */
errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
			      पूर्णाङ्क पर्णम्_level, blk64_t blk)
{
	संरचना ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	यदि (retval) {
		यदि (retval == EXT2_ET_EXTENT_NO_अग्र)
			retval = EXT2_ET_EXTENT_NOT_प्राप्तम्;
		प्रत्यागम retval;
	}

	यदि (पर्णम्_level > handle->max_गभीरता) {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("leaf level %d greater than tree depth %d\n",
			पर्णम्_level, handle->max_गभीरता);
#यदि_अन्तः
		प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
	}

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("goto extent ino %u, level %d, %llu\n", handle->ino,
	       पर्णम्_level, blk);
#यदि_अन्तः

#यदि_परिभाषितम् DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#यदि_अन्तः
	यावत् (1) {
		यदि (handle->max_गभीरता - handle->level == पर्णम्_level) {
			/* block is in this &extent */
			यदि ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				प्रत्यागम 0;
			यदि (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_पूर्व_SIB,
							   &extent);
				प्रत्यागम EXT2_ET_EXTENT_NOT_प्राप्तम्;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_SIB,
						   &extent);
			यदि (retval == EXT2_ET_EXTENT_NO_अग्र)
				प्रत्यागम EXT2_ET_EXTENT_NOT_प्राप्तम्;
			यदि (retval)
				प्रत्यागम retval;
			अनुवर्त;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_SIB,
					   &extent);
		यदि (retval == EXT2_ET_EXTENT_NO_अग्र)
			गच्छ go_अधस्तात्;
		यदि (retval)
			प्रत्यागम retval;

#यदि_परिभाषितम् DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#यदि_अन्तः
		यदि (blk == extent.e_lblk)
			गच्छ go_अधस्तात्;
		यदि (blk > extent.e_lblk)
			अनुवर्त;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_पूर्व_SIB,
					   &extent);
		यदि (retval)
			प्रत्यागम retval;

#यदि_परिभाषितम् DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#यदि_अन्तः

	go_अधस्तात्:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_अधस्तात्,
					   &extent);
		यदि (retval)
			प्रत्यागम retval;

#यदि_परिभाषितम् DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#यदि_अन्तः
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	प्रत्यागम ext2fs_extent_goto2(handle, 0, blk);
}

/*
 * Traverse back up to root fixing parents of current node as needed.
 *
 * If we changed start of first entry in a node, fix parent index start
 * and so on.
 *
 * Safe to call for any position in node; if not at the first entry,
 * it will simply return.
 *
 * Note a subtlety of this function -- if there happen to be two extents
 * mapping the same lblk and someone calls fix_parents on the second of the two
 * extents, the position of the extent handle after the call will be the second
 * extent if nothing happened, or the first extent if something did.  A caller
 * in this situation must use ext2fs_extent_goto() after calling this function.
 * Or simply don't map the same lblk with two extents, ever.
 */
errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle)
{
	पूर्णाङ्क				retval = 0;
	पूर्णाङ्क				orig_height;
	blk64_t				start;
	संरचना extent_मार्गः		*मार्गः;
	संरचना ext2fs_extent		extent;
	संरचना ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	मार्गः = handle->मार्गः + handle->level;
	यदि (!मार्गः->curr)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		गच्छ समाप्तम्;

	/* modified node's start block */
	start = extent.e_lblk;

	यदि ((retval = ext2fs_extent_get_info(handle, &info)))
		प्रत्यागम retval;
	orig_height = info.max_गभीरता - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	यावत् (handle->level > 0 &&
	       (मार्गः->वामः == मार्गः->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ऊर्ध्वम्, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
		यदि (extent.e_lblk == start)
			भङ्ग;
		मार्गः = handle->मार्गः + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
		update_मार्गः(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
समाप्तम्:
	प्रत्यागम retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)),
				संरचना ext2fs_extent *extent)
{
	संरचना extent_मार्गः		*मार्गः;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	मार्गः = handle->मार्गः + handle->level;
	यदि (!मार्गः->curr)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#यदि_अन्तः

	यदि (handle->level == handle->max_गभीरता) {
		ex = मार्गः->curr;

		ex->ee_खण्डम् = ext2fs_cpu_गन्तव्यम्_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_गन्तव्यम्_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_गन्तव्यम्_le16(extent->e_pblk >> 32);
		यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			यदि (extent->e_len > EXT_UNINIT_MAX_LEN)
				प्रत्यागम EXT2_ET_EXTENT_INVALID_दीर्घता;
			ex->ee_len = ext2fs_cpu_गन्तव्यम्_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} अन्यथा {
			यदि (extent->e_len > EXT_INIT_MAX_LEN)
				प्रत्यागम EXT2_ET_EXTENT_INVALID_दीर्घता;
			ex->ee_len = ext2fs_cpu_गन्तव्यम्_le16(extent->e_len);
		}
	} अन्यथा {
		ix = मार्गः->curr;

		ix->ei_पर्णम् = ext2fs_cpu_गन्तव्यम्_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_पर्णम्_hi = ext2fs_cpu_गन्तव्यम्_le16(extent->e_pblk >> 32);
		ix->ei_खण्डम् = ext2fs_cpu_गन्तव्यम्_le32(extent->e_lblk);
		ix->ei_अप्रयुक्तम् = 0;
	}
	update_मार्गः(handle);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क splitting_at_eof(संरचना ext2_extent_handle *handle,
			    संरचना extent_मार्गः *मार्गः)
{
	संरचना extent_मार्गः *ppp = मार्गः;
	dump_मार्गः(__func__, handle, मार्गः);

	यदि (handle->level == 0)
		प्रत्यागम 0;

	कुरु {
		यदि (ppp->वामः)
			प्रत्यागम 0;
		ppp--;
	} यावत् (ppp >= handle->मार्गः);

	प्रत्यागम 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
स्थिर errcode_t extent_ग्रन्थिः_विभज(ext2_extent_handle_t handle,
				   पूर्णाङ्क expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ग्रन्थिः_pblk;
	blk64_t				new_ग्रन्थिः_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	पूर्णाङ्क				orig_height;
	वर्ण				*खण्डम्_buf = रिक्त;
	संरचना ext2fs_extent		extent;
	संरचना extent_मार्गः		*मार्गः, *newpath = 0;
	संरचना ext3_extent_शीर्षकम्	*eh, *neweh;
	पूर्णाङ्क				tocopy;
	पूर्णाङ्क				new_मूलम् = 0;
	संरचना ext2_extent_info		info;
	पूर्णाङ्क				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("splitting node at level %d\n", handle->level);
#यदि_अन्तः
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		गच्छ समाप्तम्;

	retval = ext2fs_extent_get_info(handle, &info);
	यदि (retval)
		गच्छ समाप्तम्;

	/* save the position we were originally splitting... */
	orig_height = info.max_गभीरता - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	मार्गः = handle->मार्गः + handle->level;
	eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
	यदि (handle->level == handle->max_गभीरता) {
		संरचना ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_गन्तव्यम्_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ex->ee_start_hi) << 32);
	} अन्यथा {
		संरचना ext3_extent_idx	*ix;

		ix = EXT_FIRST_सूचकांक(eh);
		goal_blk = ext2fs_le32_गन्तव्यम्_cpu(ix->ei_पर्णम्) +
			((__u64) ext2fs_le16_गन्तव्यम्_cpu(ix->ei_पर्णम्_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_अनुपातः(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_आवरणम्(handle->fs);

	/* Is there room in the parent for a new entry? */
	यदि (handle->level &&
			(handle->मार्गः[handle->level - 1].entries >=
			 handle->मार्गः[handle->level - 1].max_entries)) {

#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("parent level %d full; splitting it too\n",
							handle->level - 1);
#यदि_अन्तः
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ऊर्ध्वम्, &extent);
		यदि (retval)
			गच्छ समाप्तम्;

		retval = extent_ग्रन्थिः_विभज(handle, expand_allowed);
		यदि (retval)
			गच्छ समाप्तम्;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		यदि (retval)
			गच्छ समाप्तम्;
	}

	/* At this point, parent should have room for this split */
	मार्गः = handle->मार्गः + handle->level;
	यदि (!मार्गः->curr)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, मार्गः) : 0;

	/* extent header of the current node we'll split */
	eh = (संरचना ext3_extent_शीर्षकम् *)मार्गः->buf;

	/* splitting root level means moving them all out */
	यदि (handle->level == 0) {
		new_मूलम् = 1;
		tocopy = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    परिमाण(संरचना extent_मार्गः),
					    &newpath);
		यदि (retval)
			गच्छ समाप्तम्;
	} अन्यथा {
		यदि (no_balance)
			tocopy = 1;
		अन्यथा
			tocopy = ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries) / 2;
	}

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries),
				handle->level);
#यदि_अन्तः

	यदि (!tocopy && !no_balance) {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("Nothing to copy to new block!\n");
#यदि_अन्तः
		retval = EXT2_ET_CANT_विभज_EXTENT;
		गच्छ समाप्तम्;
	}

	/* first we need a new block, or can do nothing. */
	खण्डम्_buf = स्मृतिं_देहि(handle->fs->blocksize);
	यदि (!खण्डम्_buf) {
		retval = ENOMEM;
		गच्छ समाप्तम्;
	}

	यदि (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, खण्डम्_buf,
				    &new_ग्रन्थिः_pblk);
	यदि (retval)
		गच्छ समाप्तम्;

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("will copy to new node at block %lu\n",
	       (अचिह्नित दीर्घ) new_ग्रन्थिः_pblk);
#यदि_अन्तः

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (संरचना ext3_extent_शीर्षकम् *) खण्डम्_buf;
	स्मृतिं_प्रतिलिख(neweh, eh, परिमाण(संरचना ext3_extent_शीर्षकम्));
	neweh->eh_entries = ext2fs_cpu_गन्तव्यम्_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_गन्तव्यम्_le16((handle->fs->blocksize -
			 परिमाण(संरचना ext3_extent_शीर्षकम्)) /
				परिमाण(संरचना ext3_extent));

	/* then the entries for the new block... */
	स्मृतिं_प्रतिलिख(EXT_FIRST_सूचकांक(neweh),
		EXT_FIRST_सूचकांक(eh) +
			(ext2fs_le16_गन्तव्यम्_cpu(eh->eh_entries) - tocopy),
		परिमाण(संरचना ext3_extent_idx) * tocopy);

	new_ग्रन्थिः_start = ext2fs_le32_गन्तव्यम्_cpu(EXT_FIRST_सूचकांक(neweh)->ei_खण्डम्);

	/* then update the checksum */
	retval = ext2fs_extent_खण्डम्_csum_समुच्चयः(handle->fs, handle->ino, neweh);
	यदि (retval)
		गच्छ समाप्तम्;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ग्रन्थिः_pblk, 1,
					खण्डम्_buf);

	यदि (retval)
		गच्छ समाप्तम्;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	यदि (handle->level == 0) {
		स्मृतिं_प्रतिलिख(newpath, मार्गः,
		       परिमाण(संरचना extent_मार्गः) * handle->max_paths);
		handle->मार्गः = newpath;
		newpath = मार्गः;
		मार्गः = handle->मार्गः;
		मार्गः->entries = 1;
		मार्गः->वामः = मार्गः->max_entries - 1;
		handle->max_गभीरता++;
		handle->max_paths++;
		eh->eh_गभीरता = ext2fs_cpu_गन्तव्यम्_le16(handle->max_गभीरता);
	} अन्यथा {
		मार्गः->entries -= tocopy;
		मार्गः->वामः -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_गन्तव्यम्_le16(मार्गः->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_मार्गः(handle);
	यदि (retval)
		गच्छ समाप्तम्;

	/* now go up and insert/replace index for new node we created */
	यदि (new_मूलम्) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		यदि (retval)
			गच्छ समाप्तम्;

		extent.e_lblk = new_ग्रन्थिः_start;
		extent.e_pblk = new_ग्रन्थिः_pblk;
		extent.e_len = handle->मार्गः[0].अन्तः_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
	} अन्यथा {
		__u32 new_ग्रन्थिः_दीर्घता;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ऊर्ध्वम्, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ग्रन्थिः_दीर्घता = new_ग्रन्थिः_start - extent.e_lblk;
		extent.e_len -= new_ग्रन्थिः_दीर्घता;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ग्रन्थिः_start;
		extent.e_pblk = new_ग्रन्थिः_pblk;
		extent.e_len = new_ग्रन्थिः_दीर्घता;
		retval = ext2fs_extent_निवेशय(handle, EXT2_EXTENT_निवेशय_AFTER, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	यदि (retval)
		गच्छ समाप्तम्;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_सङ्कलय_खण्डानि(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	यदि (retval)
		गच्छ समाप्तम्;

समाप्तम्:
	यदि (newpath)
		ext2fs_free_mem(&newpath);
	मुक्त(खण्डम्_buf);

	प्रत्यागम retval;
}

errcode_t ext2fs_extent_ग्रन्थिः_विभज(ext2_extent_handle_t handle)
{
	प्रत्यागम extent_ग्रन्थिः_विभज(handle, 0);
}

errcode_t ext2fs_extent_निवेशय(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				      संरचना ext2fs_extent *extent)
{
	संरचना extent_मार्गः		*मार्गः;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_शीर्षकम्	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#यदि_अन्तः

	मार्गः = handle->मार्गः + handle->level;

	यदि (मार्गः->entries >= मार्गः->max_entries) {
		यदि (flags & EXT2_EXTENT_निवेशय_NOSPLIT) {
			प्रत्यागम EXT2_ET_CANT_निवेशय_EXTENT;
		} अन्यथा {
#यदि_परिभाषितम् DEBUG
			प्रारूप_लिख("node full (level %d) - splitting\n",
				   handle->level);
#यदि_अन्तः
			retval = extent_ग्रन्थिः_विभज(handle, 1);
			यदि (retval)
				प्रत्यागम retval;
			मार्गः = handle->मार्गः + handle->level;
		}
	}

	eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
	यदि (मार्गः->curr) {
		ix = मार्गः->curr;
		यदि (flags & EXT2_EXTENT_निवेशय_AFTER) {
			ix++;
			मार्गः->वामः--;
		}
	} अन्यथा {
		ix = EXT_FIRST_सूचकांक(eh);
		मार्गः->वामः = -1;
	}

	मार्गः->curr = ix;

	यदि (मार्गः->वामः >= 0)
		memmove(ix + 1, ix,
			(मार्गः->वामः+1) * परिमाण(संरचना ext3_extent_idx));
	मार्गः->वामः++;
	मार्गः->entries++;

	eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
	eh->eh_entries = ext2fs_cpu_गन्तव्यम्_le16(मार्गः->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	यदि (retval)
		गच्छ errout;

	retval = update_मार्गः(handle);
	यदि (retval)
		गच्छ errout;

	प्रत्यागम 0;

errout:
	ext2fs_extent_delete(handle, 0);
	प्रत्यागम retval;
}

/*
 * Sets the physical block for a logical file block in the extent tree.
 *
 * May: map unmapped, unmap mapped, or remap mapped blocks.
 *
 * Mapping an unmapped block adds a single-block extent.
 *
 * Unmapping first or last block modifies extent in-place
 *  - But may need to fix parent's starts too in first-block case
 *
 * Mapping any unmapped block requires adding a (single-block) extent
 * and inserting into proper point in tree.
 *
 * Modifying (unmapping or remapping) a block in the middle
 * of an extent requires splitting the extent.
 *  - Remapping case requires new single-block extent.
 *
 * Remapping first or last block adds an extent.
 *
 * We really need extent adding to be smart about merging.
 */

errcode_t ext2fs_extent_समुच्चयः_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, पूर्णाङ्क flags)
{
	errcode_t		ec, retval = 0;
	पूर्णाङ्क			mapped = 1; /* logical is mapped? */
	पूर्णाङ्क			orig_height;
	पूर्णाङ्क			extent_uninit = 0;
	पूर्णाङ्क			पूर्व_uninit = 0;
	पूर्णाङ्क			अग्र_uninit = 0;
	पूर्णाङ्क			new_uninit = 0;
	पूर्णाङ्क			max_len = EXT_INIT_MAX_LEN;
	पूर्णाङ्क			has_पूर्व, has_अग्र;
	blk64_t			orig_lblk;
	संरचना extent_मार्गः	*मार्गः;
	संरचना ext2fs_extent	extent, अग्र_extent, पूर्व_extent;
	संरचना ext2fs_extent	newextent;
	संरचना ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#यदि_अन्तः

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	मार्गः = handle->मार्गः + handle->level;

	यदि (flags & EXT2_EXTENT_समुच्चयः_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	यदि (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_पर्णम्;
		यदि (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	यदि ((handle->max_गभीरता == 0) && (मार्गः->entries == 0)) {
		retval = ext2fs_extent_निवेशय(handle, 0, &newextent);
		प्रत्यागम retval;
	}

	/* save our original location in the extent tree */
	यदि ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		यदि (retval != EXT2_ET_NO_CURRENT_ग्रन्थिः)
			प्रत्यागम retval;
		स्मृतिं_पूरय(&extent, 0, परिमाण(extent));
	}
	यदि ((retval = ext2fs_extent_get_info(handle, &info)))
		प्रत्यागम retval;
	orig_height = info.max_गभीरता - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	यदि (retval) {
		यदि (retval == EXT2_ET_EXTENT_NOT_प्राप्तम्) {
			retval = 0;
			mapped = 0;
			यदि (!physical) {
#यदि_परिभाषितम् DEBUG
				प्रारूप_लिख("block %llu already unmapped\n",
					logical);
#यदि_अन्तः
				गच्छ समाप्तम्;
			}
		} अन्यथा
			गच्छ समाप्तम्;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		गच्छ समाप्तम्;
	यदि (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_पर्णम्, &अग्र_extent);
	यदि (retval) {
		has_अग्र = 0;
		यदि (retval != EXT2_ET_EXTENT_NO_अग्र)
			गच्छ समाप्तम्;
	} अन्यथा {
		dbg_print_extent("set_bmap: next_extent",
				 &अग्र_extent);
		has_अग्र = 1;
		यदि (अग्र_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			अग्र_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	यदि (retval && retval != EXT2_ET_EXTENT_NOT_प्राप्तम्)
		गच्छ समाप्तम्;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_पूर्व_पर्णम्, &पूर्व_extent);
	यदि (retval) {
		has_पूर्व = 0;
		यदि (retval != EXT2_ET_EXTENT_NO_पूर्व)
			गच्छ समाप्तम्;
	} अन्यथा {
		has_पूर्व = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &पूर्व_extent);
		यदि (पूर्व_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			पूर्व_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	यदि (retval && retval != EXT2_ET_EXTENT_NOT_प्राप्तम्)
		गच्छ समाप्तम्;

	/* check if already pointing to the requested physical */
	यदि (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("physical block (at %llu) unchanged\n", logical);
#यदि_अन्तः
		गच्छ समाप्तम्;
	}

	यदि (!mapped) {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("mapping unmapped logical block %llu\n", logical);
#यदि_अन्तः
		यदि ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((पूर्णाङ्क) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा यदि ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((पूर्णाङ्क) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा यदि (has_अग्र &&
			   (logical == अग्र_extent.e_lblk - 1) &&
			   (physical == अग्र_extent.e_pblk - 1) &&
			   (new_uninit == अग्र_uninit) &&
			   ((पूर्णाङ्क) अग्र_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_पर्णम्,
						   &अग्र_extent);
			यदि (retval)
				गच्छ समाप्तम्;
			अग्र_extent.e_len++;
			अग्र_extent.e_lblk--;
			अग्र_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &अग्र_extent);
		} अन्यथा यदि (logical < extent.e_lblk)
			retval = ext2fs_extent_निवेशय(handle, 0, &newextent);
		अन्यथा
			retval = ext2fs_extent_निवेशय(handle,
				      EXT2_EXTENT_निवेशय_AFTER, &newextent);
		यदि (retval)
			गच्छ समाप्तम्;
		retval = ext2fs_extent_fix_parents(handle);
		यदि (retval)
			गच्छ समाप्तम्;
	} अन्यथा यदि ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("(re/un)mapping only block in extent\n");
#यदि_अन्तः
		यदि (physical) {
			यदि (has_पूर्व &&
			    (logical == (पूर्व_extent.e_lblk +
					 पूर्व_extent.e_len)) &&
			    (physical == (पूर्व_extent.e_pblk +
					  पूर्व_extent.e_len)) &&
			    (new_uninit == पूर्व_uninit) &&
			    ((पूर्णाङ्क) पूर्व_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_पूर्व_पर्णम्, &पूर्व_extent);
				यदि (retval)
					गच्छ समाप्तम्;
				पूर्व_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &पूर्व_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_अग्र_पर्णम्,
							   &extent);
				यदि (retval)
					गच्छ समाप्तम्;
				गच्छ delete_ग्रन्थिः;

			} अन्यथा
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} अन्यथा {
		delete_ग्रन्थिः:
			retval = ext2fs_extent_delete(handle, 0);
			यदि (retval)
				गच्छ समाप्तम्;
			ec = ext2fs_extent_fix_parents(handle);
			यदि (ec != EXT2_ET_NO_CURRENT_ग्रन्थिः)
				retval = ec;
		}

		यदि (retval)
			गच्छ समाप्तम्;
	} अन्यथा यदि (logical == extent.e_lblk + extent.e_len - 1)  {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("(re/un)mapping last block in extent\n");
#यदि_अन्तः
		यदि (physical) {
			यदि (has_अग्र &&
			    (logical == (अग्र_extent.e_lblk - 1)) &&
			    (physical == (अग्र_extent.e_pblk - 1)) &&
			    (new_uninit == अग्र_uninit) &&
			    ((पूर्णाङ्क) अग्र_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_अग्र_पर्णम्, &अग्र_extent);
				यदि (retval)
					गच्छ समाप्तम्;
				अग्र_extent.e_len++;
				अग्र_extent.e_lblk--;
				अग्र_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &अग्र_extent);
				यदि (retval)
					गच्छ समाप्तम्;
			} अन्यथा
				retval = ext2fs_extent_निवेशय(handle,
				      EXT2_EXTENT_निवेशय_AFTER, &newextent);
			यदि (retval)
				गच्छ समाप्तम्;
			retval = ext2fs_extent_fix_parents(handle);
			यदि (retval)
				गच्छ समाप्तम्;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			यदि (retval)
				गच्छ समाप्तम्;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			यदि (retval)
				गच्छ समाप्तम्;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
	} अन्यथा यदि (logical == extent.e_lblk) {
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("(re/un)mapping first block in extent\n");
#यदि_अन्तः
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
		retval = ext2fs_extent_fix_parents(handle);
		यदि (retval)
			गच्छ समाप्तम्;
		यदि (physical) {
			यदि (has_पूर्व &&
			    (logical == (पूर्व_extent.e_lblk +
					 पूर्व_extent.e_len)) &&
			    (physical == (पूर्व_extent.e_pblk +
					  पूर्व_extent.e_len)) &&
			    (new_uninit == पूर्व_uninit) &&
			    ((पूर्णाङ्क) पूर्व_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_पूर्व_पर्णम्, &पूर्व_extent);
				यदि (retval)
					गच्छ समाप्तम्;
				पूर्व_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &पूर्व_extent);
			} अन्यथा
				retval = ext2fs_extent_निवेशय(handle,
							      0, &newextent);
			यदि (retval)
				गच्छ समाप्तम्;
			retval = ext2fs_extent_fix_parents(handle);
			यदि (retval)
				गच्छ समाप्तम्;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_पर्णम्,
						   &extent);
			यदि (retval)
				गच्छ समाप्तम्;
		}
	} अन्यथा {
		__u32	save_दीर्घता;
		blk64_t	save_lblk;
		संरचना ext2fs_extent save_extent;
		errcode_t r2;

#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("(re/un)mapping in middle of extent\n");
#यदि_अन्तः
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_दीर्घता = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		यदि (retval)
			गच्छ समाप्तम्;
		/* insert our new extent, if any */
		यदि (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_निवेशय(handle,
					EXT2_EXTENT_निवेशय_AFTER, &newextent);
			यदि (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				यदि (r2 == 0)
					(शून्य)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				गच्छ समाप्तम्;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_दीर्घता - extent.e_len - 1;
		retval = ext2fs_extent_निवेशय(handle,
				EXT2_EXTENT_निवेशय_AFTER, &extent);
		यदि (retval) {
			यदि (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				यदि (r2 == 0)
					(शून्य)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			यदि (r2 == 0)
				(शून्य)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			गच्छ समाप्तम्;
		}
	}

समाप्तम्:
	/* get handle back to its position */
	यदि (orig_height > handle->max_गभीरता)
		orig_height = handle->max_गभीरता; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	प्रत्यागम retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णाङ्क flags)
{
	संरचना extent_मार्गः		*मार्गः;
	वर्ण 				*cp;
	संरचना ext3_extent_शीर्षकम्	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	यदि (!(handle->fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!handle->मार्गः)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

#यदि_परिभाषितम् DEBUG
	{
		संरचना ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		यदि (retval == 0) {
			प्रारूप_लिख("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#यदि_अन्तः

	मार्गः = handle->मार्गः + handle->level;
	यदि (!मार्गः->curr)
		प्रत्यागम EXT2_ET_NO_CURRENT_ग्रन्थिः;

	cp = मार्गः->curr;

	/* Sanity check before memmove() */
	यदि (मार्गः->वामः < 0)
		प्रत्यागम EXT2_ET_EXTENT_पर्णम्_BAD;

	यदि (मार्गः->वामः) {
		memmove(cp, cp + परिमाण(संरचना ext3_extent_idx),
			मार्गः->वामः * परिमाण(संरचना ext3_extent_idx));
		मार्गः->वामः--;
	} अन्यथा {
		संरचना ext3_extent_idx	*ix = मार्गः->curr;
		ix--;
		मार्गः->curr = ix;
	}
	यदि (--मार्गः->entries == 0)
		मार्गः->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	यदि (मार्गः->entries == 0 && handle->level) {
		यदि (!(flags & EXT2_EXTENT_DELETE_KEEP_रिक्तानि)) {
			संरचना ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_ऊर्ध्वम्,
								&extent);
			यदि (retval)
				प्रत्यागम retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_खण्डानि -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_अनुपातः(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_खण्डम्_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} अन्यथा {
		eh = (संरचना ext3_extent_शीर्षकम् *) मार्गः->buf;
		eh->eh_entries = ext2fs_cpu_गन्तव्यम्_le16(मार्गः->entries);
		यदि ((मार्गः->entries == 0) && (handle->level == 0)) {
			eh->eh_गभीरता = 0;
			handle->max_गभीरता = 0;
		}
		retval = update_मार्गः(handle);
	}
	प्रत्यागम retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 संरचना ext2_extent_info *info)
{
	संरचना extent_मार्गः		*मार्गः;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	स्मृतिं_पूरय(info, 0, परिमाण(संरचना ext2_extent_info));

	मार्गः = handle->मार्गः + handle->level;
	यदि (मार्गः) {
		यदि (मार्गः->curr)
			info->curr_प्रविष्टिः = ((वर्ण *) मार्गः->curr - मार्गः->buf) /
				परिमाण(संरचना ext3_extent_idx);
		अन्यथा
			info->curr_प्रविष्टिः = 0;
		info->अङ्कः_entries = मार्गः->entries;
		info->max_entries = मार्गः->max_entries;
		info->अष्टकानि_avail = (मार्गः->max_entries - मार्गः->entries) *
			परिमाण(संरचना ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_गभीरता = handle->max_गभीरता;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	प्रत्यागम 0;
}

size_t ext2fs_max_extent_गभीरता(ext2_extent_handle_t handle)
{
	size_t iblock_sz = परिमाण(((संरचना ext2_inode *)रिक्त)->i_खण्डम्);
	size_t iblock_extents = (iblock_sz - परिमाण(संरचना ext3_extent_शीर्षकम्)) /
				परिमाण(संरचना ext3_extent);
	size_t extents_प्रत्येकम्_खण्डम् = (handle->fs->blocksize -
				    परिमाण(संरचना ext3_extent_शीर्षकम्)) /
				   परिमाण(संरचना ext3_extent);
	स्थिर अचिह्नित पूर्णाङ्क चरमः_blocksize = 0;
	स्थिर size_t चरमः_परिणाम = 0;

	यदि (चरमः_blocksize && चरमः_blocksize == handle->fs->blocksize)
		प्रत्यागम चरमः_परिणाम;

	चरमः_परिणाम = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_प्रत्येकम्_खण्डम्));
	चरमः_blocksize = handle->fs->blocksize;
	प्रत्यागम चरमः_परिणाम;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णाङ्क    			save_flags = fs->flags;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		प्रत्यागम 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (errcode) {
		यदि (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		प्रत्यागम errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	यदि (errcode)
		गच्छ बहिर्गमनम्;

	कुरु {
		/* Skip to the end of a block of leaf nodes */
		यदि (extent.e_flags & EXT2_EXTENT_FLAGS_पर्णम्) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_चरमः_SIB,
						    &extent);
			यदि (errcode)
				भङ्ग;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र, &extent);
		यदि (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_मार्गः(handle);
	} यावत् (errcode == 0);

बहिर्गमनम्:
	/* Ok if we run off the end */
	यदि (errcode == EXT2_ET_EXTENT_NO_अग्र)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	प्रत्यागम errcode;
}

errcode_t ext2fs_decode_extent(संरचना ext2fs_extent *गन्तव्यम्, शून्य *addr, पूर्णाङ्क len)
{
	संरचना ext3_extent *from = (संरचना ext3_extent *)addr;

	यदि (len != परिमाण(संरचना ext3_extent))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	गन्तव्यम्->e_pblk = ext2fs_le32_गन्तव्यम्_cpu(from->ee_start) +
		((__u64) ext2fs_le16_गन्तव्यम्_cpu(from->ee_start_hi)
			<< 32);
	गन्तव्यम्->e_lblk = ext2fs_le32_गन्तव्यम्_cpu(from->ee_खण्डम्);
	गन्तव्यम्->e_len = ext2fs_le16_गन्तव्यम्_cpu(from->ee_len);
	गन्तव्यम्->e_flags = EXT2_EXTENT_FLAGS_पर्णम्;
	यदि (गन्तव्यम्->e_len > EXT_INIT_MAX_LEN) {
		गन्तव्यम्->e_len -= EXT_INIT_MAX_LEN;
		गन्तव्यम्->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	प्रत्यागम 0;
}

errcode_t ext2fs_गणनम्_खण्डानि(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode, blk64_t *ret_गणनम्)
{
	ext2_extent_handle_t	handle = रिक्त;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णाङ्क			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (errcode)
		गच्छ बहिर्गमनम्;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	यदि (errcode)
		गच्छ बहिर्गमनम्;

	errcode = ext2fs_get_array(handle->max_गभीरता, परिमाण(blk64_t),
				   &intermediate_nodes);
	यदि (errcode)
		गच्छ बहिर्गमनम्;

	blkcount = handle->level;
	यावत् (!errcode) {
		यदि (extent.e_flags & EXT2_EXTENT_FLAGS_पर्णम्) {
			blkcount += extent.e_len;
			प्रति (i = 0; i < handle->level; i++) {
				यदि (intermediate_nodes[i] !=
					handle->मार्गः[i].अन्तः_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->मार्गः[i].अन्तः_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
बहिर्गमनम्:
	*ret_गणनम् = blkcount;
	ext2fs_extent_free(handle);

	प्रत्यागम 0;
}

#यदि_परिभाषितम् DEBUG
/*
 * Override debugfs's prompt
 */
ध्रुव वर्ण *debug_prog_name = "tst_extents";

#यदि_अन्तः

