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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
प्रकारनिर्देश पूर्णाङ्क ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
स्थिर पूर्णाङ्क check_zero_block(वर्ण *buf, पूर्णाङ्क blocksize)
{
	वर्ण	*cp = buf;
	पूर्णाङ्क	बायाँ = blocksize;

	जबसम्म (बायाँ > 0) {
		यदि (*cp++)
			फर्काऊ 0;
		बायाँ--;
	}
	फर्काऊ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	dgrp_t		group;
	ssize_t		बायाँ, c, d;
	वर्ण		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	यदि (!buf)
		फर्काऊ ENOMEM;

	लागि (group = 0; group < fs->group_desc_गन्ती; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		यदि (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			गच्छ errout;
		}
		बायाँ = fs->inode_blocks_per_group;
		यदि ((blk < fs->super->s_first_data_block) ||
		    (blk + बायाँ - 1 >= ext2fs_blocks_गन्ती(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			गच्छ errout;
		}
		जबसम्म (बायाँ) {
			c = BUF_BLOCKS;
			यदि (c > बायाँ)
				c = बायाँ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			यदि (retval)
				गच्छ errout;
			cp = buf;
			जबसम्म (c) {
				यदि (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					गच्छ skip_sparse;
				}
				/* Skip zero blocks */
				यदि (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					बायाँ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  प्रस्तुत_से);
					यदि (r < 0) {
						retval = त्रुटि_सं;
						गच्छ errout;
					}
					जारी;
				}
				/* Find non-zero blocks */
				लागि (d = 1; d < c; d++) {
					यदि (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						रोक;
				}
			skip_sparse:
				actual = लिख(fd, cp, d * fs->blocksize);
				यदि (actual == -1) {
					retval = त्रुटि_सं;
					गच्छ errout;
				}
				यदि (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					गच्छ errout;
				}
				blk += d;
				बायाँ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	मुक्त(buf);
	फर्काऊ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णाङ्क fd,
				  पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, बायाँ;
	वर्ण		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	यदि (!buf)
		फर्काऊ ENOMEM;

	लागि (group = 0; group < fs->group_desc_गन्ती; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		यदि (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			गच्छ errout;
		}
		बायाँ = fs->inode_blocks_per_group;
		जबसम्म (बायाँ) {
			c = BUF_BLOCKS;
			यदि (c > बायाँ)
				c = बायाँ;
			actual = पठ(fd, buf, fs->blocksize * c);
			यदि (actual == -1) {
				retval = त्रुटि_सं;
				गच्छ errout;
			}
			यदि (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				गच्छ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			यदि (retval)
				गच्छ errout;

			blk += c;
			बायाँ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	मुक्त(buf);
	फर्काऊ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णाङ्क fd,
				   पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	वर्ण		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef शब्दहरू_BIGENDIAN
	अचिन्हित पूर्णाङ्क	groups_per_block;
	संरचना		ext2_group_desc *gdp;
	पूर्णाङ्क		j;
#endif

	यदि (fs->group_desc == रिक्त)
		फर्काऊ EXT2_ET_NO_GDESC;

	buf = दो_स्मृति(fs->blocksize);
	यदि (!buf)
		फर्काऊ ENOMEM;

	/*
	 * Write out the superblock
	 */
	स_रखो(buf, 0, fs->blocksize);
#ifdef शब्दहरू_BIGENDIAN
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
	यदि (actual == -1) {
		retval = त्रुटि_सं;
		गच्छ errout;
	}
	यदि (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (वर्ण *) fs->group_desc;

#ifdef शब्दहरू_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	लागि (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		यदि (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = लिख(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef शब्दहरू_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	लागि (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		यदि (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	यदि (actual == -1) {
		retval = त्रुटि_सं;
		गच्छ errout;
	}
	यदि (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	retval = 0;

errout:
	मुक्त(buf);
	फर्काऊ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णाङ्क fd,
				  पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	वर्ण		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = दो_स्मृति(size);
	यदि (!buf)
		फर्काऊ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = पठ(fd, buf, size);
	यदि (actual == -1) {
		retval = त्रुटि_सं;
		गच्छ errout;
	}
	यदि (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		गच्छ errout;
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
	फर्काऊ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	माप_प्रकार			c;
	__u64			itr, cnt, size, total_size;
	वर्ण			buf[1024];

	यदि (flags & IMAGER_FLAG_INODEMAP) {
		यदि (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			यदि (retval)
				फर्काऊ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गन्ती;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		यदि (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			यदि (retval)
				फर्काऊ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_गन्ती);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_गन्ती;

	जबसम्म (cnt > 0) {
		size = परिमाण(buf);
		यदि (size > (cnt >> 3))
			size = (cnt >> 3);
		यदि (size == 0)
			रोक;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		यदि (retval)
			फर्काऊ retval;

		actual = लिख(fd, buf, size);
		यदि (actual == -1)
			फर्काऊ त्रुटि_सं;
		यदि (actual != (पूर्णाङ्क) size)
			फर्काऊ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	स_रखो(buf, 0, परिमाण(buf));
	यदि (size) {
		size = fs->blocksize - size;
		जबसम्म (size) {
			c = size;
			यदि (c > (पूर्णाङ्क) परिमाण(buf))
				c = परिमाण(buf);
			actual = लिख(fd, buf, c);
			यदि (actual < 0)
				फर्काऊ त्रुटि_सं;
			यदि ((माप_प्रकार) actual != c)
				फर्काऊ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	फर्काऊ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	वर्ण			buf[1024];
	अचिन्हित पूर्णाङ्क		size;
	ssize_t			actual;

	यदि (flags & IMAGER_FLAG_INODEMAP) {
		यदि (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			यदि (retval)
				फर्काऊ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गन्ती;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		यदि (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			यदि (retval)
				फर्काऊ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_गन्ती);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	जबसम्म (cnt > 0) {
		size = परिमाण(buf);
		यदि (size > (cnt >> 3))
			size = (cnt >> 3);
		यदि (size == 0)
			रोक;

		actual = पठ(fd, buf, size);
		यदि (actual == -1)
			फर्काऊ त्रुटि_सं;
		यदि (actual != (पूर्णाङ्क) size)
			फर्काऊ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		यदि (retval)
			फर्काऊ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	फर्काऊ 0;
}
