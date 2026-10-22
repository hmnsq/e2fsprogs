/*
 * icount.c --- an efficient inode count abstraction
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "tdb.h"

/*
 * The data storage strategy used by icount relies on the observation
 * that most inode counts are either zero (for non-allocated inodes),
 * one (for most files), and only a few that are two or more
 * (directories and files that are linked to more than one directory).
 *
 * Also, e2fsck tends to load the icount data sequentially.
 *
 * So, we use an inode bitmap to indicate which inodes have a count of
 * one, and then use a sorted list to store the counts for inodes
 * which are greater than one.
 *
 * We also use an optional bitmap to indicate which inodes are already
 * in the sorted list, to speed up the use of this abstraction by
 * e2fsck's pass 2.  Pass 2 increments inode counts as it finds them,
 * so this extra bitmap avoids searching the sorted list to see if a
 * particular inode is on the sorted list already.
 */

ರಚನೆ ext2_icount_el {
	ext2_ino_t	ino;
	__u32		ಎಣಿಕೆ_2;
};

ರಚನೆ ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	ಅಪವರ್ತ್ಯ;
	ext2_ino_t		ಎಣಿಕೆ_2;
	ext2_ino_t		ಗಾತ್ರ_2;
	ext2_ino_t		ಅಂಕೆ_inodes;
	ext2_ino_t		cursor;
	ರಚನೆ ext2_icount_el	*list;
	ರಚನೆ ext2_icount_el	*ಕೊನೆಯ_lookup;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
	ಅಕ್ಷರ			*tdb_fn;
	TDB_CONTEXT		*tdb;
#ಆದರೆ_ಅಂತ್ಯ
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

ಶೂನ್ಯ ext2fs_free_icount(ext2_icount_t icount)
{
	ಆದರೆ (!icount)
		ಹಿಂತಿರುಗಿ;

	icount->magic = 0;
	ಆದರೆ (icount->list)
		ext2fs_free_mem(&icount->list);
	ಆದರೆ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ಆದರೆ (icount->ಅಪವರ್ತ್ಯ)
		ext2fs_free_inode_bitmap(icount->ಅಪವರ್ತ್ಯ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
	ಆದರೆ (icount->tdb)
		tdb_close(icount->tdb);
	ಆದರೆ (icount->tdb_fn) {
		(ಶೂನ್ಯ) unlink(icount->tdb_fn);
		ಬಿಡುಗಡೆ(icount->tdb_fn);
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ಸ್ಥಿರ errcode_t alloc_icount(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_icount), &icount);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(icount, 0, ಗಾತ್ರ(ರಚನೆ ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->ಅಂಕೆ_inodes = fs->super->s_inodes_ಎಣಿಕೆ;

	ಆದರೆ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		ಚಿಹ್ನೆರಹಿತ sz = ಗಾತ್ರ(*icount->fullmap) * icount->ಅಂಕೆ_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ಆದರೆ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಆದರೆ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->ಅಪವರ್ತ್ಯ);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	} ಇಲ್ಲದಿದ್ದರೆ
		icount->ಅಪವರ್ತ್ಯ = 0;

	*ret = icount;
	ಹಿಂತಿರುಗಿ 0;

errout:
	ext2fs_free_icount(icount);
	ಹಿಂತಿರುಗಿ(retval);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
ರಚನೆ uuid {
	__u32	ಸಮಯ_ಕೆಳಗಿನ;
	__u16	ಸಮಯ_mid;
	__u16	ಸಮಯ_hi_and_ಆವೃತ್ತಿ;
	__u16	clock_seq;
	__u8	ಗಂಟು[6];
};

ಸ್ಥಿರ ಶೂನ್ಯ unpack_uuid(ಶೂನ್ಯ *ಒಳ, ರಚನೆ uuid *uu)
{
	__u8	*ptr = ಒಳ;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ಸಮಯ_ಕೆಳಗಿನ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ಸಮಯ_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ಸಮಯ_hi_and_ಆವೃತ್ತಿ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	ಸ್ಮರಣೆ_ನಕಲು(uu->ಗಂಟು, ptr, 6);
}

ಸ್ಥಿರ ಶೂನ್ಯ uuid_unparse(ಶೂನ್ಯ *uu, ಅಕ್ಷರ *ಹೊರಹೋಗು)
{
	ರಚನೆ uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(ಹೊರಹೋಗು,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.ಸಮಯ_ಕೆಳಗಿನ, uuid.ಸಮಯ_mid, uuid.ಸಮಯ_hi_and_ಆವೃತ್ತಿ,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ಗಂಟು[0], uuid.ಗಂಟು[1], uuid.ಗಂಟು[2],
		uuid.ಗಂಟು[3], uuid.ಗಂಟು[4], uuid.ಗಂಟು[5]);
}
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_ಬಳಸದ,
				   ಅಕ್ಷರ *tdb_dir EXT2_FS_NO_TDB_ಬಳಸದ,
				   ಪೂರ್ಣಾಂಕ flags EXT2_FS_NO_TDB_ಬಳಸದ,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_ಬಳಸದ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	ಅಕ್ಷರ 		*fn, uuid[40];
	ext2_ino_t	ಅಂಕೆ_inodes;
	mode_t		save_umask;
	ಪೂರ್ಣಾಂಕ		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tdb_dir) + 64, &fn);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ಆದರೆ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		ಹೋಗು errout;
	}
	icount->tdb_fn = fn;
	umask(save_umask);
	/*
	 * This is an overestimate of the size that we will need; the
	 * ideal value is the number of used inodes with a count
	 * greater than 1.  OTOH the times when we really need this is
	 * with the backup programs that use lots of hard links, in
	 * which case the number of inodes in use approaches the ideal
	 * value.
	 */
	ಅಂಕೆ_inodes = fs->super->s_inodes_ಎಣಿಕೆ - fs->super->s_free_inodes_ಎಣಿಕೆ;

	icount->tdb = tdb_open(fn, ಅಂಕೆ_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	ಆದರೆ (icount->tdb == ಖಾಲಿ) {
		retval = errno;
		ಹೋಗು errout;
	}
	*ret = icount;
	ಹಿಂತಿರುಗಿ 0;
errout:
	ext2fs_free_icount(icount);
	ಹಿಂತಿರುಗಿ(retval);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
#ಆದರೆ_ಅಂತ್ಯ
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		ಬೈಟುಗಳು;
	ext2_ino_t	i;

	ಆದರೆ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ಆದರೆ (hint->ಗಾತ್ರ_2 > ಗಾತ್ರ_2)
			ಗಾತ್ರ_2 = (size_t) hint->ಗಾತ್ರ_2;
	}

	retval = alloc_icount(fs, flags, &icount);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (icount->fullmap)
		ಹೋಗು successout;

	ಆದರೆ (ಗಾತ್ರ_2) {
		icount->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_ಅಂಕೆ_dirs(fs, &icount->ಗಾತ್ರ_2);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		icount->ಗಾತ್ರ_2 += fs->super->s_inodes_ಎಣಿಕೆ / 50;
	}

	ಬೈಟುಗಳು = (size_t) (icount->ಗಾತ್ರ_2 * ಗಾತ್ರ(ರಚನೆ ext2_icount_el));
#ಆದರೆ 0
	ಬರೆ("Icount allocated %u entries, %d bytes.\n",
	       icount->ಗಾತ್ರ_2, ಬೈಟುಗಳು);
#ಆದರೆ_ಅಂತ್ಯ
	retval = ext2fs_get_array(icount->ಗಾತ್ರ_2, ಗಾತ್ರ(ರಚನೆ ext2_icount_el),
			 &icount->list);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(icount->list, 0, ಬೈಟುಗಳು);

	icount->ಎಣಿಕೆ_2 = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ಆದರೆ (hint) {
		ಗಾಗಿ (i=0; i < hint->ಎಣಿಕೆ_2; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->ಎಣಿಕೆ_2 = hint->ಎಣಿಕೆ_2;
	}

successout:
	*ret = icount;
	ಹಿಂತಿರುಗಿ 0;

errout:
	ext2fs_free_icount(icount);
	ಹಿಂತಿರುಗಿ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2,
			       ext2_icount_t *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_create_icount2(fs, flags, ಗಾತ್ರ_2, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ಸ್ಥಿರ ರಚನೆ ext2_icount_el *ಸೇರಿಸು_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ಪೂರ್ಣಾಂಕ pos)
{
	ರಚನೆ ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_ಗಾತ್ರ = 0;
	ಪೂರ್ಣಾಂಕ			ಅಂಕೆ;

	ಆದರೆ (icount->ಕೊನೆಯ_lookup && icount->ಕೊನೆಯ_lookup->ino == ino)
		ಹಿಂತಿರುಗಿ icount->ಕೊನೆಯ_lookup;

	ಆದರೆ (icount->ಎಣಿಕೆ_2 >= icount->ಗಾತ್ರ_2) {
		ಆದರೆ (icount->ಎಣಿಕೆ_2) {
			new_ಗಾತ್ರ = icount->list[(ಚಿಹ್ನೆರಹಿತ)icount->ಎಣಿಕೆ_2-1].ino;
			new_ಗಾತ್ರ = (ext2_ino_t) (icount->ಎಣಿಕೆ_2 *
				((ದಶಮಾಂಶ) icount->ಅಂಕೆ_inodes / new_ಗಾತ್ರ));
		}
		ಆದರೆ (new_ಗಾತ್ರ < (icount->ಗಾತ್ರ_2 + 100))
			new_ಗಾತ್ರ = icount->ಗಾತ್ರ_2 + 100;
#ಆದರೆ 0
		ಬರೆ("Reallocating icount %u entries...\n", new_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
		retval = ext2fs_resize_mem((size_t) icount->ಗಾತ್ರ_2 *
					   ಗಾತ್ರ(ರಚನೆ ext2_icount_el),
					   (size_t) new_ಗಾತ್ರ *
					   ಗಾತ್ರ(ರಚನೆ ext2_icount_el),
					   &icount->list);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ 0;
		icount->ಗಾತ್ರ_2 = new_ಗಾತ್ರ;
	}
	ಅಂಕೆ = (ಪೂರ್ಣಾಂಕ) icount->ಎಣಿಕೆ_2 - pos;
	ಆದರೆ (ಅಂಕೆ < 0)
		ಹಿಂತಿರುಗಿ 0;	/* should never happen */
	ಆದರೆ (ಅಂಕೆ) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			ಗಾತ್ರ(ರಚನೆ ext2_icount_el) * ಅಂಕೆ);
	}
	icount->ಎಣಿಕೆ_2++;
	el = &icount->list[pos];
	el->ಎಣಿಕೆ_2 = 0;
	el->ino = ino;
	icount->ಕೊನೆಯ_lookup = el;
	ಹಿಂತಿರುಗಿ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ಸ್ಥಿರ ರಚನೆ ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ಪೂರ್ಣಾಂಕ create)
{
	ಪೂರ್ಣಾಂಕ	ಕೆಳಗಿನ, ಮೇಲಿನ, mid;

	ಆದರೆ (!icount || !icount->list)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (create && ((icount->ಎಣಿಕೆ_2 == 0) ||
		       (ino > icount->list[(ಚಿಹ್ನೆರಹಿತ)icount->ಎಣಿಕೆ_2-1].ino))) {
		ಹಿಂತಿರುಗಿ ಸೇರಿಸು_icount_el(icount, ino, (ಚಿಹ್ನೆರಹಿತ) icount->ಎಣಿಕೆ_2);
	}
	ಆದರೆ (icount->ಎಣಿಕೆ_2 == 0)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (icount->cursor >= icount->ಎಣಿಕೆ_2)
		icount->cursor = 0;
	ಆದರೆ (ino == icount->list[icount->cursor].ino)
		ಹಿಂತಿರುಗಿ &icount->list[icount->cursor++];
#ಆದರೆ 0
	ಬರೆ("Non-cursor get_icount_el: %u\n", ino);
#ಆದರೆ_ಅಂತ್ಯ
	ಕೆಳಗಿನ = 0;
	ಮೇಲಿನ = (ಪೂರ್ಣಾಂಕ) icount->ಎಣಿಕೆ_2-1;
	ಆಗ (ಕೆಳಗಿನ <= ಮೇಲಿನ) {
		mid = ((ಚಿಹ್ನೆರಹಿತ)ಕೆಳಗಿನ + (ಚಿಹ್ನೆರಹಿತ)ಮೇಲಿನ) >> 1;
		ಆದರೆ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ಹಿಂತಿರುಗಿ &icount->list[mid];
		}
		ಆದರೆ (ino < icount->list[mid].ino)
			ಮೇಲಿನ = mid-1;
		ಇಲ್ಲದಿದ್ದರೆ
			ಕೆಳಗಿನ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ಆದರೆ (create)
		ಹಿಂತಿರುಗಿ ಸೇರಿಸು_icount_el(icount, ino, ಕೆಳಗಿನ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ಗಣ_inode_ಎಣಿಕೆ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 ಎಣಿಕೆ_2)
{
	ರಚನೆ ext2_icount_el 	*el;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, ದತ್ತಾಂಶ;

	ಆದರೆ (icount->tdb) {
		ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &ino;
		ಕೀಲಿ.dsize = ಗಾತ್ರ(ext2_ino_t);
		ದತ್ತಾಂಶ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &ಎಣಿಕೆ_2;
		ದತ್ತಾಂಶ.dsize = ಗಾತ್ರ(__u32);
		ಆದರೆ (ಎಣಿಕೆ_2) {
			ಆದರೆ (tdb_store(icount->tdb, ಕೀಲಿ, ದತ್ತಾಂಶ, TDB_REPLACE))
				ಹಿಂತಿರುಗಿ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (tdb_delete(icount->tdb, ಕೀಲಿ))
				ಹಿಂತಿರುಗಿ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(ಎಣಿಕೆ_2);
		ಹಿಂತಿರುಗಿ 0;
	}

	el = get_icount_el(icount, ino, 1);
	ಆದರೆ (!el)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	el->ಎಣಿಕೆ_2 = ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t get_inode_ಎಣಿಕೆ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *ಎಣಿಕೆ_2)
{
	ರಚನೆ ext2_icount_el 	*el;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, ದತ್ತಾಂಶ;

	ಆದರೆ (icount->tdb) {
		ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &ino;
		ಕೀಲಿ.dsize = ಗಾತ್ರ(ext2_ino_t);

		ದತ್ತಾಂಶ = tdb_fetch(icount->tdb, ಕೀಲಿ);
		ಆದರೆ (ದತ್ತಾಂಶ.dptr == ಖಾಲಿ) {
			*ಎಣಿಕೆ_2 = 0;
			ಹಿಂತಿರುಗಿ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*ಎಣಿಕೆ_2 = *((__u32 *) ದತ್ತಾಂಶ.dptr);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ.dptr);
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (icount->fullmap) {
		*ಎಣಿಕೆ_2 = icount->fullmap[ino];
		ಹಿಂತಿರುಗಿ 0;
	}

	el = get_icount_el(icount, ino, 0);
	ಆದರೆ (!el) {
		*ಎಣಿಕೆ_2 = 0;
		ಹಿಂತಿರುಗಿ ENOENT;
	}

	*ಎಣಿಕೆ_2 = el->ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *ಹೊರಹೋಗು)
{
	errcode_t	ret = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	i;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಆದರೆ (icount->ಎಣಿಕೆ_2 > icount->ಗಾತ್ರ_2) {
		fprintf(ಹೊರಹೋಗು, "%s: count > size\n", bad);
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	}
	ಗಾಗಿ (i=1; i < icount->ಎಣಿಕೆ_2; i++) {
		ಆದರೆ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(ಹೊರಹೋಗು, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ವಾದ;
		}
	}
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಆದರೆ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (!icount->fullmap) {
		ಆದರೆ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (icount->ಅಪವರ್ತ್ಯ &&
			!ext2fs_test_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino)) {
			*ret = 0;
			ಹಿಂತಿರುಗಿ 0;
		}
	}
	get_inode_ಎಣಿಕೆ(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ಮೌಲ್ಯ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಆದರೆ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (icount->fullmap) {
		curr_ಮೌಲ್ಯ = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_ಮೌಲ್ಯ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ಆದರೆ (ಗಣ_inode_ಎಣಿಕೆ(icount, ino, 2))
			ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
		curr_ಮೌಲ್ಯ = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (icount->ಅಪವರ್ತ್ಯ) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ಆದರೆ (ext2fs_test_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino)) {
			get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
			curr_ಮೌಲ್ಯ++;
			ಆದರೆ (ಗಣ_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
				ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ಆದರೆ (ret)
				*ret = 1;
			ಹಿಂತಿರುಗಿ 0;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
		curr_ಮೌಲ್ಯ++;
		ಆದರೆ (ಗಣ_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
			ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	}
	ಆದರೆ (icount->ಅಪವರ್ತ್ಯ)
		ext2fs_mark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);
	ಆದರೆ (ret)
		*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ಮೌಲ್ಯ;

	ಆದರೆ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಆದರೆ (icount->fullmap) {
		ಆದರೆ (!icount->fullmap[ino])
			ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

		curr_ಮೌಲ್ಯ = --icount->fullmap[ino];
		ಆದರೆ (ret)
			*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ಆದರೆ (icount->ಅಪವರ್ತ್ಯ)
			ext2fs_unmark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);
		ಇಲ್ಲದಿದ್ದರೆ {
			ಗಣ_inode_ಎಣಿಕೆ(icount, ino, 0);
		}
		ಆದರೆ (ret)
			*ret = 0;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (icount->ಅಪವರ್ತ್ಯ &&
	    !ext2fs_test_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
	ಆದರೆ (!curr_ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	curr_ಮೌಲ್ಯ--;
	ಆದರೆ (ಗಣ_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	ಆದರೆ (curr_ಮೌಲ್ಯ == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ಆದರೆ ((curr_ಮೌಲ್ಯ == 0) && icount->ಅಪವರ್ತ್ಯ)
		ext2fs_unmark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);

	ಆದರೆ (ret)
		*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 ಎಣಿಕೆ_2)
{
	ಆದರೆ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಆದರೆ (icount->fullmap)
		ಹಿಂತಿರುಗಿ ಗಣ_inode_ಎಣಿಕೆ(icount, ino, ಎಣಿಕೆ_2);

	ಆದರೆ (ಎಣಿಕೆ_2 == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ಆದರೆ (icount->ಅಪವರ್ತ್ಯ)
			ext2fs_unmark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (ಎಣಿಕೆ_2 == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ಆದರೆ (icount->ಅಪವರ್ತ್ಯ) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);
		} ಇಲ್ಲದಿದ್ದರೆ
			ಗಣ_inode_ಎಣಿಕೆ(icount, ino, 0);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ಗಣ_inode_ಎಣಿಕೆ(icount, ino, ಎಣಿಕೆ_2))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ಆದರೆ (icount->ಅಪವರ್ತ್ಯ)
		ext2fs_mark_inode_bitmap2(icount->ಅಪವರ್ತ್ಯ, ino);
	ಹಿಂತಿರುಗಿ 0;
}

ext2_ino_t ext2fs_get_icount_ಗಾತ್ರ(ext2_icount_t icount)
{
	ಆದರೆ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ icount->ಗಾತ್ರ_2;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#ವ್ಯಾಖ್ಯಾನಿಸು EXIT		0x00
#ವ್ಯಾಖ್ಯಾನಿಸು FETCH		0x01
#ವ್ಯಾಖ್ಯಾನಿಸು STORE		0x02
#ವ್ಯಾಖ್ಯಾನಿಸು INCREMENT	0x03
#ವ್ಯಾಖ್ಯಾನಿಸು DECREMENT	0x04

ರಚನೆ test_ಕಾರ್ಯಕ್ರಮ {
	ಪೂರ್ಣಾಂಕ		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ರಚನೆ test_ಕಾರ್ಯಕ್ರಮ prog[] = {
	{ STORE, 42, 42, 42 },
	{ STORE, 1,  1, 1 },
	{ STORE, 2,  2, 2 },
	{ STORE, 3,  3, 3 },
	{ STORE, 10, 1, 1 },
	{ STORE, 42, 0, 0 },
	{ INCREMENT, 5, 0, 1 },
	{ INCREMENT, 5, 0, 2 },
	{ INCREMENT, 5, 0, 3 },
	{ INCREMENT, 5, 0, 4 },
	{ DECREMENT, 5, 0, 3 },
	{ DECREMENT, 5, 0, 2 },
	{ DECREMENT, 5, 0, 1 },
	{ DECREMENT, 5, 0, 0 },
	{ FETCH, 10, 0, 1 },
	{ FETCH, 1, 0, 1 },
	{ FETCH, 2, 0, 2 },
	{ FETCH, 3, 0, 3 },
	{ INCREMENT, 1, 0, 2 },
	{ DECREMENT, 2, 0, 1 },
	{ DECREMENT, 2, 0, 0 },
	{ FETCH, 12, 0, 0 },
	{ EXIT, 0, 0, 0 }
};

ರಚನೆ test_ಕಾರ್ಯಕ್ರಮ extended[] = {
	{ STORE, 1,  1, 1 },
	{ STORE, 2,  2, 2 },
	{ STORE, 3,  3, 3 },
	{ STORE, 4,  4, 4 },
	{ STORE, 5,  5, 5 },
	{ STORE, 6,  1, 1 },
	{ STORE, 7,  2, 2 },
	{ STORE, 8,  3, 3 },
	{ STORE, 9,  4, 4 },
	{ STORE, 10, 5, 5 },
	{ STORE, 11, 1, 1 },
	{ STORE, 12, 2, 2 },
	{ STORE, 13, 3, 3 },
	{ STORE, 14, 4, 4 },
	{ STORE, 15, 5, 5 },
	{ STORE, 16, 1, 1 },
	{ STORE, 17, 2, 2 },
	{ STORE, 18, 3, 3 },
	{ STORE, 19, 4, 4 },
	{ STORE, 20, 5, 5 },
	{ STORE, 21, 1, 1 },
	{ STORE, 22, 2, 2 },
	{ STORE, 23, 3, 3 },
	{ STORE, 24, 4, 4 },
	{ STORE, 25, 5, 5 },
	{ STORE, 26, 1, 1 },
	{ STORE, 27, 2, 2 },
	{ STORE, 28, 3, 3 },
	{ STORE, 29, 4, 4 },
	{ STORE, 30, 5, 5 },
	{ EXIT, 0, 0, 0 }
};

/*
 * Setup the variables for doing the inode scan test.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ(ಶೂನ್ಯ)
{
	errcode_t	retval;
	ರಚನೆ ext2_super_ಖಂಡ param;

	initialize_ext2_error_ಕೋಷ್ಟಕ();

	memset(&param, 0, ಗಾತ್ರ(param));
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು, &param,
				   test_io_manager, &test_fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}
}

ಪೂರ್ಣಾಂಕ ಸತತ_test(ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಅಕ್ಷರ *dir, ರಚನೆ test_ಕಾರ್ಯಕ್ರಮ *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ರಚನೆ test_ಕಾರ್ಯಕ್ರಮ *pc;
	__u16		ಫಲಿತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		problem = 0;

	ಆದರೆ (dir) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ಆದರೆ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			ನಿರ್ಗಮಿಸು(1);
		}
#ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("Skipped\n");
		ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_create_icount2(test_fs, flags, ಗಾತ್ರ_2, 0,
					       &icount);
		ಆದರೆ (retval) {
			com_err("run_test", retval, "while creating icount");
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಗಾಗಿ (pc = prog; pc->cmd != EXIT; pc++) {
		ಆಯ್ಕೆ (pc->cmd) {
		ಸ್ಥಿತಿ FETCH:
			ಬರೆ("icount_fetch(%u) = ", pc->ino);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ಆದರೆ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				ನಿರ್ಗಮಿಸು(1);
			}
			ಬರೆ("icount_store(%u, %u) = ", pc->ino, pc->arg);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ಆದರೆ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				ನಿರ್ಗಮಿಸು(1);
			}
			ಬರೆ("icount_increment(%u) = ", pc->ino);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ಆದರೆ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				ನಿರ್ಗಮಿಸು(1);
			}
			ಬರೆ("icount_decrement(%u) = ", pc->ino);
			ನಿಲ್ಲಿಸು;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &ಫಲಿತಾಂಶ);
		ಆದರೆ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			ನಿರ್ಗಮಿಸು(1);
		}
		ಬರೆ("%u (%s)\n", ಫಲಿತಾಂಶ, (ಫಲಿತಾಂಶ == pc->expected) ?
		       "OK" : "NOT OK");
		ಆದರೆ (ಫಲಿತಾಂಶ != pc->expected)
			problem++;
	}
	ಬರೆ("icount size is %u\n", ext2fs_get_icount_ಗಾತ್ರ(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ಆದರೆ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		ನಿರ್ಗಮಿಸು(1);
	}
	ext2fs_free_icount(icount);
	ಹಿಂತಿರುಗಿ problem;
}


ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ failed = 0;

	ಸಿದ್ಧತೆ();
	ಬರೆ("Standard icount run:\n");
	failed += ಸತತ_test(0, 0, 0, prog);
	ಬರೆ("\nMultiple bitmap test:\n");
	failed += ಸತತ_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	ಬರೆ("\nResizing icount:\n");
	failed += ಸತತ_test(0, 3, 0, extended);
	ಬರೆ("\nStandard icount run with tdb:\n");
	failed += ಸತತ_test(0, 0, ".", prog);
	ಬರೆ("\nMultiple bitmap test with tdb:\n");
	failed += ಸತತ_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	ಆದರೆ (failed)
		ಬರೆ("FAILED!\n");
	ಹಿಂತಿರುಗಿ failed;
}
#ಆದರೆ_ಅಂತ್ಯ
