/*
 * image.c --- writes out the critical parts of the filesystem as a
 * 	flat file.
 *
 * Copyright (C) 2000 Theodore Ts'o.
 *
 * Note: this uses the POSIX IO interfaces, unlike most of the other
 * functions in this library.  So sue me.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
प्रकारव्याख्या पूर्णांक ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
स्थिर पूर्णांक check_zero_block(अक्षर *buf, पूर्णांक blocksize)
{
	अक्षर	*cp = buf;
	पूर्णांक	डावा = blocksize;

	पर्यंत (डावा > 0) {
		जर (*cp++)
			परत 0;
		डावा--;
	}
	परत 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	dgrp_t		group;
	ssize_t		डावा, c, d;
	अक्षर		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	जर (!buf)
		परत ENOMEM;

	साठी (group = 0; group < fs->group_desc_मोजणी; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		जर (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			जा errout;
		}
		डावा = fs->inode_blocks_per_group;
		जर ((blk < fs->super->s_first_data_block) ||
		    (blk + डावा - 1 >= ext2fs_blocks_मोजणी(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			जा errout;
		}
		पर्यंत (डावा) {
			c = BUF_BLOCKS;
			जर (c > डावा)
				c = डावा;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			जर (retval)
				जा errout;
			cp = buf;
			पर्यंत (c) {
				जर (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					जा skip_sparse;
				}
				/* Skip zero blocks */
				जर (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					डावा--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  प्रस्तुत_से);
					जर (r < 0) {
						retval = त्रुटि_सं;
						जा errout;
					}
					सुरूठेवा;
				}
				/* Find non-zero blocks */
				साठी (d = 1; d < c; d++) {
					जर (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						थांबा;
				}
			skip_sparse:
				actual = लिख(fd, cp, d * fs->blocksize);
				जर (actual == -1) {
					retval = त्रुटि_सं;
					जा errout;
				}
				जर (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					जा errout;
				}
				blk += d;
				डावा -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	मुक्त(buf);
	परत retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णांक fd,
				  पूर्णांक flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, डावा;
	अक्षर		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	जर (!buf)
		परत ENOMEM;

	साठी (group = 0; group < fs->group_desc_मोजणी; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		जर (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			जा errout;
		}
		डावा = fs->inode_blocks_per_group;
		पर्यंत (डावा) {
			c = BUF_BLOCKS;
			जर (c > डावा)
				c = डावा;
			actual = पठ(fd, buf, fs->blocksize * c);
			जर (actual == -1) {
				retval = त्रुटि_सं;
				जा errout;
			}
			जर (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				जा errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			जर (retval)
				जा errout;

			blk += c;
			डावा -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	मुक्त(buf);
	परत retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णांक fd,
				   पूर्णांक flags EXT2FS_ATTR((unused)))
{
	अक्षर		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef शब्द_BIGENDIAN
	अचिन्हित पूर्णांक	groups_per_block;
	संरचना		ext2_group_desc *gdp;
	पूर्णांक		j;
#endif

	जर (fs->group_desc == रिक्त)
		परत EXT2_ET_NO_GDESC;

	buf = दो_स्मृति(fs->blocksize);
	जर (!buf)
		परत ENOMEM;

	/*
	 * Write out the superblock
	 */
	स_रखो(buf, 0, fs->blocksize);
#ifdef शब्द_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	स_नकल(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#अन्यथा
	स_नकल(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = लिख(fd, buf, fs->blocksize);
	जर (actual == -1) {
		retval = त्रुटि_सं;
		जा errout;
	}
	जर (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		जा errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (अक्षर *) fs->group_desc;

#ifdef शब्द_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	साठी (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		जर (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = लिख(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef शब्द_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	साठी (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		जर (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	जर (actual == -1) {
		retval = त्रुटि_सं;
		जा errout;
	}
	जर (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		जा errout;
	}

	retval = 0;

errout:
	मुक्त(buf);
	परत retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णांक fd,
				  पूर्णांक flags EXT2FS_ATTR((unused)))
{
	अक्षर		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = दो_स्मृति(size);
	जर (!buf)
		परत ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = पठ(fd, buf, size);
	जर (actual == -1) {
		retval = त्रुटि_सं;
		जा errout;
	}
	जर (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		जा errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	स_नकल(fs->super, buf, SUPERBLOCK_SIZE);

	स_नकल(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	मुक्त(buf);
	परत retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	माप_प्रकार			c;
	__u64			itr, cnt, size, total_size;
	अक्षर			buf[1024];

	जर (flags & IMAGER_FLAG_INODEMAP) {
		जर (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			जर (retval)
				परत retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_मोजणी;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		जर (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			जर (retval)
				परत retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_मोजणी);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_मोजणी;

	पर्यंत (cnt > 0) {
		size = आकार(buf);
		जर (size > (cnt >> 3))
			size = (cnt >> 3);
		जर (size == 0)
			थांबा;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		जर (retval)
			परत retval;

		actual = लिख(fd, buf, size);
		जर (actual == -1)
			परत त्रुटि_सं;
		जर (actual != (पूर्णांक) size)
			परत EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	स_रखो(buf, 0, आकार(buf));
	जर (size) {
		size = fs->blocksize - size;
		पर्यंत (size) {
			c = size;
			जर (c > (पूर्णांक) आकार(buf))
				c = आकार(buf);
			actual = लिख(fd, buf, c);
			जर (actual < 0)
				परत त्रुटि_सं;
			जर ((माप_प्रकार) actual != c)
				परत EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	परत 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	अक्षर			buf[1024];
	अचिन्हित पूर्णांक		size;
	ssize_t			actual;

	जर (flags & IMAGER_FLAG_INODEMAP) {
		जर (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			जर (retval)
				परत retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_मोजणी;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		जर (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			जर (retval)
				परत retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_मोजणी);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	पर्यंत (cnt > 0) {
		size = आकार(buf);
		जर (size > (cnt >> 3))
			size = (cnt >> 3);
		जर (size == 0)
			थांबा;

		actual = पठ(fd, buf, size);
		जर (actual == -1)
			परत त्रुटि_सं;
		जर (actual != (पूर्णांक) size)
			परत EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		जर (retval)
			परत retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	परत 0;
}
