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
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
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

#यदि_अपरिभाषितम् HAVE_TYPE_SSIZE_T
प्रकारनिर्देश पूर्णाङ्क ssize_t;
#यदि_अन्तः

/*
 * This function returns 1 if the specified block is all zeros
 */
स्थिर पूर्णाङ्क check_zero_खण्डम्(वर्ण *buf, पूर्णाङ्क blocksize)
{
	वर्ण	*cp = buf;
	पूर्णाङ्क	वामः = blocksize;

	यावत् (वामः > 0) {
		यदि (*cp++)
			प्रत्यागम 0;
		वामः--;
	}
	प्रत्यागम 1;
}

/*
 * Write the inode table out as a single block.
 */
#परिभाषय BUF_खण्डानि	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णाङ्क fd, पूर्णाङ्क flags)
{
	dgrp_t		गणः;
	ssize_t		वामः, c, d;
	वर्ण		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = स्मृतिं_देहि(fs->blocksize * BUF_खण्डानि);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	प्रति (गणः = 0; गणः < fs->गणः_desc_गणनम्; गणः++) {
		blk = ext2fs_inode_सारणी_loc(fs, गणः);
		यदि (!blk) {
			retval = EXT2_ET_MISSING_INODE_सारणी;
			गच्छ errout;
		}
		वामः = fs->inode_खण्डानि_प्रत्येकम्_गणः;
		यदि ((blk < fs->super->s_first_दत्त_खण्डम्) ||
		    (blk + वामः - 1 >= ext2fs_खण्डानि_गणनम्(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_सारणी;
			गच्छ errout;
		}
		यावत् (वामः) {
			c = BUF_खण्डानि;
			यदि (c > वामः)
				c = वामः;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			यदि (retval)
				गच्छ errout;
			cp = buf;
			यावत् (c) {
				यदि (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					गच्छ skip_sparse;
				}
				/* Skip zero blocks */
				यदि (check_zero_खण्डम्(cp, fs->blocksize)) {
					c--;
					blk++;
					वामः--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					यदि (r < 0) {
						retval = errno;
						गच्छ errout;
					}
					अनुवर्त;
				}
				/* Find non-zero blocks */
				प्रति (d = 1; d < c; d++) {
					यदि (check_zero_खण्डम्(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						भङ्ग;
				}
			skip_sparse:
				actual = लिख(fd, cp, d * fs->blocksize);
				यदि (actual == -1) {
					retval = errno;
					गच्छ errout;
				}
				यदि (actual != d * fs->blocksize) {
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
	प्रत्यागम retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णाङ्क fd,
				  पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	dgrp_t		गणः;
	ssize_t		c, वामः;
	वर्ण		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = स्मृतिं_देहि(fs->blocksize * BUF_खण्डानि);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	प्रति (गणः = 0; गणः < fs->गणः_desc_गणनम्; गणः++) {
		blk = ext2fs_inode_सारणी_loc(fs, गणः);
		यदि (!blk) {
			retval = EXT2_ET_MISSING_INODE_सारणी;
			गच्छ errout;
		}
		वामः = fs->inode_खण्डानि_प्रत्येकम्_गणः;
		यावत् (वामः) {
			c = BUF_खण्डानि;
			यदि (c > वामः)
				c = वामः;
			actual = पठ(fd, buf, fs->blocksize * c);
			यदि (actual == -1) {
				retval = errno;
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
			वामः -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	मुक्त(buf);
	प्रत्यागम retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णाङ्क fd,
				   पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	वर्ण		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	अचिह्नित पूर्णाङ्क	groups_प्रत्येकम्_खण्डम्;
	संरचना		ext2_गणः_desc *gdp;
	पूर्णाङ्क		j;
#यदि_अन्तः

	यदि (fs->गणः_desc == रिक्त)
		प्रत्यागम EXT2_ET_NO_GDESC;

	buf = स्मृतिं_देहि(fs->blocksize);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	/*
	 * Write out the superblock
	 */
	स्मृतिं_पूरय(buf, 0, fs->blocksize);
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	स्मृतिं_प्रतिलिख(buf, fs->super, SUPERBLOCK_आकार);
	ext2fs_swap_super(fs->super);
#अन्यथा
	स्मृतिं_प्रतिलिख(buf, fs->super, SUPERBLOCK_आकार);
#यदि_अन्तः
	actual = लिख(fd, buf, fs->blocksize);
	यदि (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	यदि (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (वर्ण *) fs->गणः_desc;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_प्रत्येकम्_खण्डम् = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);
	प्रति (j=0; j < groups_प्रत्येकम्_खण्डम्*fs->desc_खण्डानि; j++) {
		gdp = ext2fs_गणः_desc(fs, fs->गणः_desc, j);
		यदि (gdp)
			ext2fs_swap_गणः_desc2(fs, gdp);
	}
#यदि_अन्तः

	actual = लिख(fd, cp, (ssize_t)fs->blocksize * fs->desc_खण्डानि);


#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	groups_प्रत्येकम्_खण्डम् = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);
	प्रति (j=0; j < groups_प्रत्येकम्_खण्डम्*fs->desc_खण्डानि; j++) {
		gdp = ext2fs_गणः_desc(fs, fs->गणः_desc, j);
		यदि (gdp)
			ext2fs_swap_गणः_desc2(fs, gdp);
	}
#यदि_अन्तः

	यदि (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	यदि (actual != (ssize_t)(fs->blocksize * fs->desc_खण्डानि)) {
		retval = EXT2_ET_SHORT_WRITE;
		गच्छ errout;
	}

	retval = 0;

errout:
	मुक्त(buf);
	प्रत्यागम retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णाङ्क fd,
				  पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	वर्ण		*buf;
	ssize_t		actual, आकार;
	errcode_t	retval;

	आकार = (ssize_t)fs->blocksize * (fs->desc_खण्डानि + 1);
	buf = स्मृतिं_देहि(आकार);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = पठ(fd, buf, आकार);
	यदि (actual == -1) {
		retval = errno;
		गच्छ errout;
	}
	यदि (actual != आकार) {
		retval = EXT2_ET_SHORT_READ;
		गच्छ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	स्मृतिं_प्रतिलिख(fs->super, buf, SUPERBLOCK_आकार);

	स्मृतिं_प्रतिलिख(fs->गणः_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_खण्डानि);

	retval = 0;

errout:
	मुक्त(buf);
	प्रत्यागम retval;
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
	__u64			itr, cnt, आकार, योग_आकार;
	वर्ण			buf[1024];

	यदि (flags & IMAGER_FLAG_INODEMAP) {
		यदि (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			यदि (retval)
				प्रत्यागम retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_प्रत्येकम्_गणः(fs->super) *
			fs->गणः_desc_गणनम्;
		आकार = (EXT2_INODES_प्रत्येकम्_गणः(fs->super) / 8);
	} अन्यथा {
		यदि (!fs->खण्डम्_map) {
			retval = ext2fs_read_खण्डम्_bitmap(fs);
			यदि (retval)
				प्रत्यागम retval;
		}
		bmap = fs->खण्डम्_map;
		itr = fs->super->s_first_दत्त_खण्डम्;
		cnt = EXT2_GROUPS_गन्तव्यम्_CLUSTERS(fs->super, fs->गणः_desc_गणनम्);
		आकार = EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super) / 8;
	}
	योग_आकार = आकार * fs->गणः_desc_गणनम्;

	यावत् (cnt > 0) {
		आकार = परिमाण(buf);
		यदि (आकार > (cnt >> 3))
			आकार = (cnt >> 3);
		यदि (आकार == 0)
			भङ्ग;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       आकार << 3, buf);
		यदि (retval)
			प्रत्यागम retval;

		actual = लिख(fd, buf, आकार);
		यदि (actual == -1)
			प्रत्यागम errno;
		यदि (actual != (पूर्णाङ्क) आकार)
			प्रत्यागम EXT2_ET_SHORT_READ;

		itr += आकार << 3;
		cnt -= आकार << 3;
	}

	आकार = योग_आकार % fs->blocksize;
	स्मृतिं_पूरय(buf, 0, परिमाण(buf));
	यदि (आकार) {
		आकार = fs->blocksize - आकार;
		यावत् (आकार) {
			c = आकार;
			यदि (c > (पूर्णाङ्क) परिमाण(buf))
				c = परिमाण(buf);
			actual = लिख(fd, buf, c);
			यदि (actual < 0)
				प्रत्यागम errno;
			यदि ((size_t) actual != c)
				प्रत्यागम EXT2_ET_SHORT_WRITE;
			आकार -= c;
		}
	}
	प्रत्यागम 0;
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
	अचिह्नित पूर्णाङ्क		आकार;
	ssize_t			actual;

	यदि (flags & IMAGER_FLAG_INODEMAP) {
		यदि (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			यदि (retval)
				प्रत्यागम retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_प्रत्येकम्_गणः(fs->super) *
			fs->गणः_desc_गणनम्;
		आकार = (EXT2_INODES_प्रत्येकम्_गणः(fs->super) / 8);
	} अन्यथा {
		यदि (!fs->खण्डम्_map) {
			retval = ext2fs_read_खण्डम्_bitmap(fs);
			यदि (retval)
				प्रत्यागम retval;
		}
		bmap = fs->खण्डम्_map;
		itr = fs->super->s_first_दत्त_खण्डम्;
		cnt = EXT2_GROUPS_गन्तव्यम्_खण्डानि(fs->super, fs->गणः_desc_गणनम्);
		आकार = EXT2_खण्डानि_प्रत्येकम्_गणः(fs->super) / 8;
	}

	यावत् (cnt > 0) {
		आकार = परिमाण(buf);
		यदि (आकार > (cnt >> 3))
			आकार = (cnt >> 3);
		यदि (आकार == 0)
			भङ्ग;

		actual = पठ(fd, buf, आकार);
		यदि (actual == -1)
			प्रत्यागम errno;
		यदि (actual != (पूर्णाङ्क) आकार)
			प्रत्यागम EXT2_ET_SHORT_READ;

		retval = ext2fs_समुच्चयः_generic_bmap_range(bmap, itr,
						       आकार << 3, buf);
		यदि (retval)
			प्रत्यागम retval;

		itr += आकार << 3;
		cnt -= आकार << 3;
	}
	प्रत्यागम 0;
}
