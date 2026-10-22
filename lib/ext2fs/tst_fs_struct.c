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

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ struct_ext2_filsys fs;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ offsetof
#ವ್ಯಾಖ್ಯಾನಿಸು offsetof(type, member)  __builtin_offsetof (type, member)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು check_ಕ್ಷೇತ್ರ(x) cur_ಸ್ಥಾನಾಂತರ = do_ಕ್ಷೇತ್ರ(#x, ಗಾತ್ರ(fs.x),		\
					offsetof(ರಚನೆ struct_ext2_filsys, x), \
					cur_ಸ್ಥಾನಾಂತರ)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ, size_t ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ cur_ಸ್ಥಾನಾಂತರ)
{
	ಆದರೆ (ಸ್ಥಾನಾಂತರ != cur_ಸ್ಥಾನಾಂತರ) {
		ಬರೆ("\t(padding %d bytes?)\n", ಸ್ಥಾನಾಂತರ - cur_ಸ್ಥಾನಾಂತರ);
	}
	ಬರೆ("%8d %-30s %3u\n", ಸ್ಥಾನಾಂತರ, ಕ್ಷೇತ್ರ, (ಚಿಹ್ನೆರಹಿತ) ಗಾತ್ರ_2);
	ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ_2;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
#ಆದರೆ (__GNUC__ >= 4)
	ಪೂರ್ಣಾಂಕ cur_ಸ್ಥಾನಾಂತರ = 0;

	ಬರೆ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ಕ್ಷೇತ್ರ(magic);
	check_ಕ್ಷೇತ್ರ(io);
	check_ಕ್ಷೇತ್ರ(flags);
	check_ಕ್ಷೇತ್ರ(device_name);
	check_ಕ್ಷೇತ್ರ(super);
	check_ಕ್ಷೇತ್ರ(blocksize);
	check_ಕ್ಷೇತ್ರ(fragsize);
	check_ಕ್ಷೇತ್ರ(ಗುಂಪು_desc_ಎಣಿಕೆ);
	check_ಕ್ಷೇತ್ರ(desc_ಖಂಡಗಳು);
	check_ಕ್ಷೇತ್ರ(ಗುಂಪು_desc);
	check_ಕ್ಷೇತ್ರ(inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
	check_ಕ್ಷೇತ್ರ(inode_map);
	check_ಕ್ಷೇತ್ರ(ಖಂಡ_map);
	check_ಕ್ಷೇತ್ರ(get_ಖಂಡಗಳು);
	check_ಕ್ಷೇತ್ರ(check_directory);
	check_ಕ್ಷೇತ್ರ(write_bitmaps);
	check_ಕ್ಷೇತ್ರ(read_inode);
	check_ಕ್ಷೇತ್ರ(write_inode);
	check_ಕ್ಷೇತ್ರ(badblocks);
	check_ಕ್ಷೇತ್ರ(dblist);
	check_ಕ್ಷೇತ್ರ(stride);
	check_ಕ್ಷೇತ್ರ(orig_super);
	check_ಕ್ಷೇತ್ರ(image_ಶೀರ್ಷಿಕೆ);
	check_ಕ್ಷೇತ್ರ(umask);
	check_ಕ್ಷೇತ್ರ(now);
	check_ಕ್ಷೇತ್ರ(cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು);
	check_ಕ್ಷೇತ್ರ(reserved);
	check_ಕ್ಷೇತ್ರ(priv_ದತ್ತಾಂಶ);
	check_ಕ್ಷೇತ್ರ(icache);
	check_ಕ್ಷೇತ್ರ(image_io);
	check_ಕ್ಷೇತ್ರ(get_alloc_ಖಂಡ);
	check_ಕ್ಷೇತ್ರ(ಖಂಡ_alloc_stats);
	check_ಕ್ಷೇತ್ರ(mmp_buf);
	check_ಕ್ಷೇತ್ರ(mmp_cmp);
	check_ಕ್ಷೇತ್ರ(mmp_fd);
	check_ಕ್ಷೇತ್ರ(mmp_ಕೊನೆಯ_written);
	ಬರೆ("Ending offset is %d\n\n", cur_ಸ್ಥಾನಾಂತರ);
#ಆದರೆ_ಅಂತ್ಯ
	ನಿರ್ಗಮಿಸು(0);
}
