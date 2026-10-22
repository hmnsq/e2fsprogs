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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"

ರಚನೆ ext2_inode_large inode;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ offsetof
#ವ್ಯಾಖ್ಯಾನಿಸು offsetof(type, member)  __builtin_offsetof(type, member)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು check_ಕ್ಷೇತ್ರ(x, s) cur_ಸ್ಥಾನಾಂತರ = do_ಕ್ಷೇತ್ರ(#x, s, ಗಾತ್ರ(inode.x),	       \
					offsetof(ರಚನೆ ext2_inode_large, x),  \
					cur_ಸ್ಥಾನಾಂತರ)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ, ಚಿಹ್ನೆರಹಿತ ಗಾತ್ರ_2, ಚಿಹ್ನೆರಹಿತ cur_ಗಾತ್ರ,
		    ಚಿಹ್ನೆರಹಿತ ಸ್ಥಾನಾಂತರ, ಚಿಹ್ನೆರಹಿತ cur_ಸ್ಥಾನಾಂತರ)
{
	ಆದರೆ (ಗಾತ್ರ_2 != cur_ಗಾತ್ರ) {
		ಬರೆ("error: %s size %u should be %u\n",
		       ಕ್ಷೇತ್ರ, cur_ಗಾತ್ರ, ಗಾತ್ರ_2);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಸ್ಥಾನಾಂತರ != cur_ಸ್ಥಾನಾಂತರ) {
		ಬರೆ("error: %s offset %u should be %u\n",
		       ಕ್ಷೇತ್ರ, cur_ಸ್ಥಾನಾಂತರ, ಸ್ಥಾನಾಂತರ);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("%8d %-30s %3u\n", ಸ್ಥಾನಾಂತರ, ಕ್ಷೇತ್ರ, (ಚಿಹ್ನೆರಹಿತ) ಗಾತ್ರ_2);
	ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ_2;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
#ಆದರೆ (__GNUC__ >= 4)
	ಪೂರ್ಣಾಂಕ cur_ಸ್ಥಾನಾಂತರ = 0;

	ಬರೆ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ಕ್ಷೇತ್ರ(i_ಬಹುಲಕ, 2);
	check_ಕ್ಷೇತ್ರ(i_uid, 2);
	check_ಕ್ಷೇತ್ರ(i_ಗಾತ್ರ, 4);
	check_ಕ್ಷೇತ್ರ(i_atime, 4);
	check_ಕ್ಷೇತ್ರ(i_ctime, 4);
	check_ಕ್ಷೇತ್ರ(i_mtime, 4);
	check_ಕ್ಷೇತ್ರ(i_dtime, 4);
	check_ಕ್ಷೇತ್ರ(i_gid, 2);
	check_ಕ್ಷೇತ್ರ(i_links_ಎಣಿಕೆ, 2);
	check_ಕ್ಷೇತ್ರ(i_ಖಂಡಗಳು, 4);
	check_ಕ್ಷೇತ್ರ(i_flags, 4);
	check_ಕ್ಷೇತ್ರ(osd1.linux1.l_i_ಆವೃತ್ತಿ, 4);
	check_ಕ್ಷೇತ್ರ(i_ಖಂಡ, 15 * 4);
	check_ಕ್ಷೇತ್ರ(i_generation, 4);
	check_ಕ್ಷೇತ್ರ(i_file_acl, 4);
	check_ಕ್ಷೇತ್ರ(i_ಗಾತ್ರ_ಮೇಲಿನ, 4);
	check_ಕ್ಷೇತ್ರ(i_faddr, 4);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_ಖಂಡಗಳು_hi, 2);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_file_acl_ಮೇಲಿನ, 2);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_uid_ಮೇಲಿನ, 2);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_gid_ಮೇಲಿನ, 2);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_checksum_lo, 2);
	check_ಕ್ಷೇತ್ರ(osd2.linux2.l_i_reserved, 2);
	do_ಕ್ಷೇತ್ರ("Small inode end", 0, 0, cur_ಸ್ಥಾನಾಂತರ, 128);
	check_ಕ್ಷೇತ್ರ(i_extra_isize, 2);
	check_ಕ್ಷೇತ್ರ(i_checksum_hi, 2);
	check_ಕ್ಷೇತ್ರ(i_ctime_extra, 4);
	check_ಕ್ಷೇತ್ರ(i_mtime_extra, 4);
	check_ಕ್ಷೇತ್ರ(i_atime_extra, 4);
	check_ಕ್ಷೇತ್ರ(i_crtime, 4);
	check_ಕ್ಷೇತ್ರ(i_crtime_extra, 4);
	check_ಕ್ಷೇತ್ರ(i_ಆವೃತ್ತಿ_hi, 4);
	check_ಕ್ಷೇತ್ರ(i_projid, 4);
	/* This size will change as new fields are added */
	do_ಕ್ಷೇತ್ರ("Large inode end", 0, 0, cur_ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ(inode));
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}
