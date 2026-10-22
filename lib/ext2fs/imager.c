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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
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
	पूर्णाङ्क	वामः = blocksize;

	याव (वामः > 0) {
		सचे (*cp++)
			पटिनिवत्त 0;
		वामः--;
	}
	पटिनिवत्त 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	dgrp_t		group;
	ssize_t		वामः, c, d;
	वर्ण		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	सचे (!buf)
		पटिनिवत्त ENOMEM;

	अत्थाय (group = 0; group < fs->group_desc_गणनम्; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		सचे (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			गच्छ errout;
		}
		वामः = fs->inode_blocks_per_group;
		सचे ((blk < fs->super->s_first_data_block) ||
		    (blk + वामः - 1 >= ext2fs_blocks_गणनम्(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			गच्छ errout;
		}
		याव (वामः) {
			c = BUF_BLOCKS;
			सचे (c > वामः)
				c = वामः;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			सचे (retval)
				गच्छ errout;
			cp = buf;
			याव (c) {
				सचे (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					गच्छ skip_sparse;
				}
				/* Skip zero blocks */
				सचे (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					वामः--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					सचे (r < 0) {
						retval = errno;
						गच्छ errout;
					}
					अनुवर्त;
				}
				/* Find non-zero blocks */
				अत्थाय (d = 1; d < c; d++) {
					सचे (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						भङ्ग;
				}
			skip_sparse:
				actual = लिख(fd, cp, d * fs->blocksize);
				सचे (actual == -1) {
					retval = errno;
					गच्छ errout;
				}
				सचे (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					गच्छ errout;
				}
				blk += d;
				वामः -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	मुक्त(buf);
	पटिनिवत्त retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णाङ्क fd,
				  पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, वामः;
	वर्ण		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	सचे (!buf)
		पटिनिवत्त ENOMEM;

	अत्थाय (group = 0; group < fs->group_desc_गणनम्; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		सचे (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			गच्छ errout;
		}
		वामः = fs->inode_blocks_per_group;
		याव (वामः) {
			c = BUF_BLOCKS;
			सचे (c > वामः)
				c = वामः;
			actual = पठ(fd, buf, fs->blocksize * c);
			सचे (actual == -1) {
				retval = errno;
				गच्छ errout;
			}
			सचे (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				गच्छ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			सचे (retval)
				गच्छ errout;

			blk += c;
			वामः -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	मुक्त(buf);
	पटिनिवत्त retval;
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
#ifdef शब्दाः_BIGENDIAN
	unsigned पूर्णाङ्क	groups_per_block;
	संरचना		ext2_group_desc *gdp;
	पूर्णाङ्क		j;
#endif

	सचे (fs->group_desc == रिक्त)
		पटिनिवत्त EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	सचे (!buf)
		पटिनिवत्त ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef शब्दाः_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#अञ्ञथा
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = लिख(fd, buf, fs->blocksize);
	सचे (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	सचे (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (वर्ण *) fs->group_desc;

#ifdef शब्दाः_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	अत्थाय (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		सचे (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = लिख(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef शब्दाः_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	अत्थाय (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		सचे (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	सचे (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	सचे (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	retval = 0;

errout:
	मुक्त(buf);
	पटिनिवत्त retval;
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
	buf = malloc(size);
	सचे (!buf)
		पटिनिवत्त ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = पठ(fd, buf, size);
	सचे (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	सचे (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		गच्छ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	मुक्त(buf);
	पटिनिवत्त retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	वर्ण			buf[1024];

	सचे (flags & IMAGER_FLAG_INODEMAP) {
		सचे (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			सचे (retval)
				पटिनिवत्त retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गणनम्;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अञ्ञथा {
		सचे (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			सचे (retval)
				पटिनिवत्त retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_गणनम्);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_गणनम्;

	याव (cnt > 0) {
		size = परिमाण(buf);
		सचे (size > (cnt >> 3))
			size = (cnt >> 3);
		सचे (size == 0)
			भङ्ग;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		सचे (retval)
			पटिनिवत्त retval;

		actual = लिख(fd, buf, size);
		सचे (actual == -1)
			पटिनिवत्त errno;
		सचे (actual != (पूर्णाङ्क) size)
			पटिनिवत्त EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, परिमाण(buf));
	सचे (size) {
		size = fs->blocksize - size;
		याव (size) {
			c = size;
			सचे (c > (पूर्णाङ्क) परिमाण(buf))
				c = परिमाण(buf);
			actual = लिख(fd, buf, c);
			सचे (actual < 0)
				पटिनिवत्त errno;
			सचे ((size_t) actual != c)
				पटिनिवत्त EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	पटिनिवत्त 0;
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
	unsigned पूर्णाङ्क		size;
	ssize_t			actual;

	सचे (flags & IMAGER_FLAG_INODEMAP) {
		सचे (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			सचे (retval)
				पटिनिवत्त retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गणनम्;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अञ्ञथा {
		सचे (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			सचे (retval)
				पटिनिवत्त retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_गणनम्);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	याव (cnt > 0) {
		size = परिमाण(buf);
		सचे (size > (cnt >> 3))
			size = (cnt >> 3);
		सचे (size == 0)
			भङ्ग;

		actual = पठ(fd, buf, size);
		सचे (actual == -1)
			पटिनिवत्त errno;
		सचे (actual != (पूर्णाङ्क) size)
			पटिनिवत्त EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		सचे (retval)
			पटिनिवत्त retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	पटिनिवत्त 0;
}
