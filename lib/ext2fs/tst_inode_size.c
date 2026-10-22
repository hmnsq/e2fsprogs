/*
 * This testing program makes sure the ext2_inode structure is 1024 bytes long
 *
 * Copyright (C) 2007 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2_fs.h"

संरचना ext2_inode_large inode;

#यदि_अपरिभाषितम् offsetof
#परिभाषय offsetof(type, member)  __builtin_offsetof(type, member)
#यदि_अन्तः

#परिभाषय check_क्षेत्रम्(x, s) cur_विस्थापनम् = do_क्षेत्रम्(#x, s, परिमाण(inode.x),	       \
					offsetof(संरचना ext2_inode_large, x),  \
					cur_विस्थापनम्)

स्थिर पूर्णाङ्क do_क्षेत्रम्(ध्रुव वर्ण *क्षेत्रम्, अचिह्नित आकार, अचिह्नित cur_आकार,
		    अचिह्नित विस्थापनम्, अचिह्नित cur_विस्थापनम्)
{
	यदि (आकार != cur_आकार) {
		प्रारूप_लिख("error: %s size %u should be %u\n",
		       क्षेत्रम्, cur_आकार, आकार);
		निर्गच्छ(1);
	}
	यदि (विस्थापनम् != cur_विस्थापनम्) {
		प्रारूप_लिख("error: %s offset %u should be %u\n",
		       क्षेत्रम्, cur_विस्थापनम्, विस्थापनम्);
		निर्गच्छ(1);
	}
	प्रारूप_लिख("%8d %-30s %3u\n", विस्थापनम्, क्षेत्रम्, (अचिह्नित) आकार);
	प्रत्यागम विस्थापनम् + आकार;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
#यदि (__GNUC__ >= 4)
	पूर्णाङ्क cur_विस्थापनम् = 0;

	प्रारूप_लिख("%8s %-30s %3s\n", "offset", "field", "size");
	check_क्षेत्रम्(i_बहुलकम्, 2);
	check_क्षेत्रम्(i_uid, 2);
	check_क्षेत्रम्(i_आकार, 4);
	check_क्षेत्रम्(i_atime, 4);
	check_क्षेत्रम्(i_ctime, 4);
	check_क्षेत्रम्(i_mtime, 4);
	check_क्षेत्रम्(i_dtime, 4);
	check_क्षेत्रम्(i_gid, 2);
	check_क्षेत्रम्(i_links_गणनम्, 2);
	check_क्षेत्रम्(i_खण्डानि, 4);
	check_क्षेत्रम्(i_flags, 4);
	check_क्षेत्रम्(osd1.linux1.l_i_संस्करणम्, 4);
	check_क्षेत्रम्(i_खण्डम्, 15 * 4);
	check_क्षेत्रम्(i_generation, 4);
	check_क्षेत्रम्(i_file_acl, 4);
	check_क्षेत्रम्(i_आकार_उच्चः, 4);
	check_क्षेत्रम्(i_faddr, 4);
	check_क्षेत्रम्(osd2.linux2.l_i_खण्डानि_hi, 2);
	check_क्षेत्रम्(osd2.linux2.l_i_file_acl_उच्चः, 2);
	check_क्षेत्रम्(osd2.linux2.l_i_uid_उच्चः, 2);
	check_क्षेत्रम्(osd2.linux2.l_i_gid_उच्चः, 2);
	check_क्षेत्रम्(osd2.linux2.l_i_checksum_lo, 2);
	check_क्षेत्रम्(osd2.linux2.l_i_reserved, 2);
	do_क्षेत्रम्("Small inode end", 0, 0, cur_विस्थापनम्, 128);
	check_क्षेत्रम्(i_extra_isize, 2);
	check_क्षेत्रम्(i_checksum_hi, 2);
	check_क्षेत्रम्(i_ctime_extra, 4);
	check_क्षेत्रम्(i_mtime_extra, 4);
	check_क्षेत्रम्(i_atime_extra, 4);
	check_क्षेत्रम्(i_crtime, 4);
	check_क्षेत्रम्(i_crtime_extra, 4);
	check_क्षेत्रम्(i_संस्करणम्_hi, 4);
	check_क्षेत्रम्(i_projid, 4);
	/* This size will change as new fields are added */
	do_क्षेत्रम्("Large inode end", 0, 0, cur_विस्थापनम्, परिमाण(inode));
#यदि_अन्तः
	प्रत्यागम 0;
}
