/*
 * This testing program checks the offset of the ext2_filsys structure
 *
 * Copyright (C) 2007 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2fs.h"

संरचना struct_ext2_filsys fs;

#यदि_अपरिभाषितम् offsetof
#परिभाषय offsetof(type, member)  __builtin_offsetof (type, member)
#यदि_अन्तः
#परिभाषय check_क्षेत्रम्(x) cur_विस्थापनम् = do_क्षेत्रम्(#x, परिमाण(fs.x),		\
					offsetof(संरचना struct_ext2_filsys, x), \
					cur_विस्थापनम्)

स्थिर पूर्णाङ्क do_क्षेत्रम्(ध्रुव वर्ण *क्षेत्रम्, size_t आकार, पूर्णाङ्क विस्थापनम्, पूर्णाङ्क cur_विस्थापनम्)
{
	यदि (विस्थापनम् != cur_विस्थापनम्) {
		प्रारूप_लिख("\t(padding %d bytes?)\n", विस्थापनम् - cur_विस्थापनम्);
	}
	प्रारूप_लिख("%8d %-30s %3u\n", विस्थापनम्, क्षेत्रम्, (अचिह्नित) आकार);
	प्रत्यागम विस्थापनम् + आकार;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
#यदि (__GNUC__ >= 4)
	पूर्णाङ्क cur_विस्थापनम् = 0;

	प्रारूप_लिख("%8s %-30s %3s\n", "offset", "field", "size");
	check_क्षेत्रम्(magic);
	check_क्षेत्रम्(io);
	check_क्षेत्रम्(flags);
	check_क्षेत्रम्(device_name);
	check_क्षेत्रम्(super);
	check_क्षेत्रम्(blocksize);
	check_क्षेत्रम्(fragsize);
	check_क्षेत्रम्(गणः_desc_गणनम्);
	check_क्षेत्रम्(desc_खण्डानि);
	check_क्षेत्रम्(गणः_desc);
	check_क्षेत्रम्(inode_खण्डानि_प्रत्येकम्_गणः);
	check_क्षेत्रम्(inode_map);
	check_क्षेत्रम्(खण्डम्_map);
	check_क्षेत्रम्(get_खण्डानि);
	check_क्षेत्रम्(check_directory);
	check_क्षेत्रम्(write_bitmaps);
	check_क्षेत्रम्(read_inode);
	check_क्षेत्रम्(write_inode);
	check_क्षेत्रम्(badblocks);
	check_क्षेत्रम्(dblist);
	check_क्षेत्रम्(stride);
	check_क्षेत्रम्(orig_super);
	check_क्षेत्रम्(image_शीर्षकम्);
	check_क्षेत्रम्(umask);
	check_क्षेत्रम्(now);
	check_क्षेत्रम्(cluster_अनुपातः_द्वयङ्काः);
	check_क्षेत्रम्(reserved);
	check_क्षेत्रम्(priv_दत्त);
	check_क्षेत्रम्(icache);
	check_क्षेत्रम्(image_io);
	check_क्षेत्रम्(get_alloc_खण्डम्);
	check_क्षेत्रम्(खण्डम्_alloc_stats);
	check_क्षेत्रम्(mmp_buf);
	check_क्षेत्रम्(mmp_cmp);
	check_क्षेत्रम्(mmp_fd);
	check_क्षेत्रम्(mmp_चरमः_written);
	प्रारूप_लिख("Ending offset is %d\n\n", cur_विस्थापनम्);
#यदि_अन्तः
	निर्गच्छ(0);
}
