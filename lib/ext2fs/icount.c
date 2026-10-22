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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
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

ಸಂರಚನಾ ext2_icount_el {
	ext2_ino_t	ino;
	__u32		ಎಣಿಕೆ;
};

ಸಂರಚನಾ ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		ಎಣಿಕೆ;
	ext2_ino_t		size;
	ext2_ino_t		ಅಂಕೆ_inodes;
	ext2_ino_t		cursor;
	ಸಂರಚನಾ ext2_icount_el	*list;
	ಸಂರಚನಾ ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	ವರ್ಣ			*tdb_fn;
	TDB_CONTEXT		*tdb;
#endif
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#define icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

ಶೂನ್ಯ ext2fs_free_icount(ext2_icount_t icount)
{
	ಯದಿ (!icount)
		ಪ್ರತ್ಯಾಗಮ;

	icount->magic = 0;
	ಯದಿ (icount->list)
		ext2fs_free_mem(&icount->list);
	ಯದಿ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ಯದಿ (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	ಯದಿ (icount->tdb)
		tdb_close(icount->tdb);
	ಯದಿ (icount->tdb_fn) {
		(ಶೂನ್ಯ) unlink(icount->tdb_fn);
		ಮುಕ್ತ(icount->tdb_fn);
	}
#endif

	ಯದಿ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ಸ್ಥಿರ errcode_t alloc_icount(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount), &icount);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memset(icount, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->ಅಂಕೆ_inodes = fs->super->s_inodes_ಎಣಿಕೆ;

	ಯದಿ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = ಪರಿಮಾಣ(*icount->fullmap) * icount->ಅಂಕೆ_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ಯದಿ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

	ಯದಿ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	} ಅನ್ಯಥಾ
		icount->multiple = 0;

	*ret = icount;
	ಪ್ರತ್ಯಾಗಮ 0;

errout:
	ext2fs_free_icount(icount);
	ಪ್ರತ್ಯಾಗಮ(retval);
}

#ifdef CONFIG_TDB
ಸಂರಚನಾ uuid {
	__u32	time_ಕೆಳಗಿನ;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	ಗಂಟು[6];
};

ಸ್ಥಿರ ಶೂನ್ಯ unpack_uuid(ಶೂನ್ಯ *in, ಸಂರಚನಾ uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_ಕೆಳಗಿನ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->ಗಂಟು, ptr, 6);
}

ಸ್ಥಿರ ಶೂನ್ಯ uuid_unparse(ಶೂನ್ಯ *uu, ವರ್ಣ *out)
{
	ಸಂರಚನಾ uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_ಕೆಳಗಿನ, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ಗಂಟು[0], uuid.ಗಂಟು[1], uuid.ಗಂಟು[2],
		uuid.ಗಂಟು[3], uuid.ಗಂಟು[4], uuid.ಗಂಟು[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   ವರ್ಣ *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	ವರ್ಣ 		*fn, uuid[40];
	ext2_ino_t	ಅಂಕೆ_inodes;
	mode_t		save_umask;
	ಪೂರ್ಣಾಙ್ಕ		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ಯದಿ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		ಗಚ್ಛ errout;
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
	ಯದಿ (icount->tdb == ರಿಕ್ತ) {
		retval = errno;
		ಗಚ್ಛ errout;
	}
	*ret = icount;
	ಪ್ರತ್ಯಾಗಮ 0;
errout:
	ext2fs_free_icount(icount);
	ಪ್ರತ್ಯಾಗಮ(retval);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, unsigned ಪೂರ್ಣಾಙ್ಕ size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	ಯದಿ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ಯದಿ (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (icount->fullmap)
		ಗಚ್ಛ successout;

	ಯದಿ (size) {
		icount->size = size;
	} ಅನ್ಯಥಾ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_ಅಂಕೆ_dirs(fs, &icount->size);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		icount->size += fs->super->s_inodes_ಎಣಿಕೆ / 50;
	}

	bytes = (size_t) (icount->size * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount_el));
#ಯದಿ 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount_el),
			 &icount->list);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(icount->list, 0, bytes);

	icount->ಎಣಿಕೆ = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ಯದಿ (hint) {
		ಪ್ರತಿ (i=0; i < hint->ಎಣಿಕೆ; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->ಎಣಿಕೆ = hint->ಎಣಿಕೆ;
	}

successout:
	*ret = icount;
	ಪ್ರತ್ಯಾಗಮ 0;

errout:
	ext2fs_free_icount(icount);
	ಪ್ರತ್ಯಾಗಮ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
			       unsigned ಪೂರ್ಣಾಙ್ಕ size,
			       ext2_icount_t *ret)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ಸ್ಥಿರ ಸಂರಚನಾ ext2_icount_el *ಸೇರಿಸು_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ pos)
{
	ಸಂರಚನಾ ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	ಪೂರ್ಣಾಙ್ಕ			ಅಂಕೆ;

	ಯದಿ (icount->last_lookup && icount->last_lookup->ino == ino)
		ಪ್ರತ್ಯಾಗಮ icount->last_lookup;

	ಯದಿ (icount->ಎಣಿಕೆ >= icount->size) {
		ಯದಿ (icount->ಎಣಿಕೆ) {
			new_size = icount->list[(unsigned)icount->ಎಣಿಕೆ-1].ino;
			new_size = (ext2_ino_t) (icount->ಎಣಿಕೆ *
				((ಭಿನ್ನ) icount->ಅಂಕೆ_inodes / new_size));
		}
		ಯದಿ (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#ಯದಿ 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount_el),
					   (size_t) new_size *
					   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount_el),
					   &icount->list);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ 0;
		icount->size = new_size;
	}
	ಅಂಕೆ = (ಪೂರ್ಣಾಙ್ಕ) icount->ಎಣಿಕೆ - pos;
	ಯದಿ (ಅಂಕೆ < 0)
		ಪ್ರತ್ಯಾಗಮ 0;	/* should never happen */
	ಯದಿ (ಅಂಕೆ) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_icount_el) * ಅಂಕೆ);
	}
	icount->ಎಣಿಕೆ++;
	el = &icount->list[pos];
	el->ಎಣಿಕೆ = 0;
	el->ino = ino;
	icount->last_lookup = el;
	ಪ್ರತ್ಯಾಗಮ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ಸ್ಥಿರ ಸಂರಚನಾ ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ create)
{
	ಪೂರ್ಣಾಙ್ಕ	ಕೆಳಗಿನ, ಮೇಲಿನ, mid;

	ಯದಿ (!icount || !icount->list)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (create && ((icount->ಎಣಿಕೆ == 0) ||
		       (ino > icount->list[(unsigned)icount->ಎಣಿಕೆ-1].ino))) {
		ಪ್ರತ್ಯಾಗಮ ಸೇರಿಸು_icount_el(icount, ino, (unsigned) icount->ಎಣಿಕೆ);
	}
	ಯದಿ (icount->ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (icount->cursor >= icount->ಎಣಿಕೆ)
		icount->cursor = 0;
	ಯದಿ (ino == icount->list[icount->cursor].ino)
		ಪ್ರತ್ಯಾಗಮ &icount->list[icount->cursor++];
#ಯದಿ 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	ಕೆಳಗಿನ = 0;
	ಮೇಲಿನ = (ಪೂರ್ಣಾಙ್ಕ) icount->ಎಣಿಕೆ-1;
	ಯಾವತ್ (ಕೆಳಗಿನ <= ಮೇಲಿನ) {
		mid = ((unsigned)ಕೆಳಗಿನ + (unsigned)ಮೇಲಿನ) >> 1;
		ಯದಿ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ಪ್ರತ್ಯಾಗಮ &icount->list[mid];
		}
		ಯದಿ (ino < icount->list[mid].ino)
			ಮೇಲಿನ = mid-1;
		ಅನ್ಯಥಾ
			ಕೆಳಗಿನ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ಯದಿ (create)
		ಪ್ರತ್ಯಾಗಮ ಸೇರಿಸು_icount_el(icount, ino, ಕೆಳಗಿನ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t set_inode_ಎಣಿಕೆ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 ಎಣಿಕೆ)
{
	ಸಂರಚನಾ ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ಕೀಲಿ, data;

	ಯದಿ (icount->tdb) {
		ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *) &ino;
		ಕೀಲಿ.dsize = ಪರಿಮಾಣ(ext2_ino_t);
		data.dptr = (unsigned ವರ್ಣ *) &ಎಣಿಕೆ;
		data.dsize = ಪರಿಮಾಣ(__u32);
		ಯದಿ (ಎಣಿಕೆ) {
			ಯದಿ (tdb_store(icount->tdb, ಕೀಲಿ, data, TDB_REPLACE))
				ಪ್ರತ್ಯಾಗಮ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ಅನ್ಯಥಾ {
			ಯದಿ (tdb_delete(icount->tdb, ಕೀಲಿ))
				ಪ್ರತ್ಯಾಗಮ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
	ಯದಿ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(ಎಣಿಕೆ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	el = get_icount_el(icount, ino, 1);
	ಯದಿ (!el)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	el->ಎಣಿಕೆ = ಎಣಿಕೆ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t get_inode_ಎಣಿಕೆ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *ಎಣಿಕೆ)
{
	ಸಂರಚನಾ ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ಕೀಲಿ, data;

	ಯದಿ (icount->tdb) {
		ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *) &ino;
		ಕೀಲಿ.dsize = ಪರಿಮಾಣ(ext2_ino_t);

		data = tdb_fetch(icount->tdb, ಕೀಲಿ);
		ಯದಿ (data.dptr == ರಿಕ್ತ) {
			*ಎಣಿಕೆ = 0;
			ಪ್ರತ್ಯಾಗಮ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*ಎಣಿಕೆ = *((__u32 *) data.dptr);
		ಮುಕ್ತ(data.dptr);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
	ಯದಿ (icount->fullmap) {
		*ಎಣಿಕೆ = icount->fullmap[ino];
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	el = get_icount_el(icount, ino, 0);
	ಯದಿ (!el) {
		*ಎಣಿಕೆ = 0;
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	}

	*ಎಣಿಕೆ = el->ಎಣಿಕೆ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ	i;
	ಧ್ರುವ ವರ್ಣ *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಯದಿ (icount->ಎಣಿಕೆ > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	}
	ಪ್ರತಿ (i=1; i < icount->ಎಣಿಕೆ; i++) {
		ಯದಿ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಯದಿ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (!icount->fullmap) {
		ಯದಿ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಯದಿ (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}
	get_inode_ಎಣಿಕೆ(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ಮೌಲ್ಯ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಯದಿ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (icount->fullmap) {
		curr_ಮೌಲ್ಯ = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_ಮೌಲ್ಯ;
	} ಅನ್ಯಥಾ ಯದಿ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ಯದಿ (set_inode_ಎಣಿಕೆ(icount, ino, 2))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
		curr_ಮೌಲ್ಯ = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ಅನ್ಯಥಾ ಯದಿ (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ಯದಿ (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
			curr_ಮೌಲ್ಯ++;
			ಯದಿ (set_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
		} ಅನ್ಯಥಾ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ಯದಿ (ret)
				*ret = 1;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	} ಅನ್ಯಥಾ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
		curr_ಮೌಲ್ಯ++;
		ಯದಿ (set_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	}
	ಯದಿ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ಯದಿ (ret)
		*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ಮೌಲ್ಯ;

	ಯದಿ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಯದಿ (icount->fullmap) {
		ಯದಿ (!icount->fullmap[ino])
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

		curr_ಮೌಲ್ಯ = --icount->fullmap[ino];
		ಯದಿ (ret)
			*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ಯದಿ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ಅನ್ಯಥಾ {
			set_inode_ಎಣಿಕೆ(icount, ino, 0);
		}
		ಯದಿ (ret)
			*ret = 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	get_inode_ಎಣಿಕೆ(icount, ino, &curr_ಮೌಲ್ಯ);
	ಯದಿ (!curr_ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	curr_ಮೌಲ್ಯ--;
	ಯದಿ (set_inode_ಎಣಿಕೆ(icount, ino, curr_ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	ಯದಿ (curr_ಮೌಲ್ಯ == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ಯದಿ ((curr_ಮೌಲ್ಯ == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	ಯದಿ (ret)
		*ret = icount_16_xlate(curr_ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 ಎಣಿಕೆ)
{
	ಯದಿ (!ino || (ino > icount->ಅಂಕೆ_inodes))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ಯದಿ (icount->fullmap)
		ಪ್ರತ್ಯಾಗಮ set_inode_ಎಣಿಕೆ(icount, ino, ಎಣಿಕೆ);

	ಯದಿ (ಎಣಿಕೆ == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ಯದಿ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (ಎಣಿಕೆ == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ಯದಿ (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} ಅನ್ಯಥಾ
			set_inode_ಎಣಿಕೆ(icount, ino, 0);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (set_inode_ಎಣಿಕೆ(icount, ino, ಎಣಿಕೆ))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ಯದಿ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	ಯದಿ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

ಸಂರಚನಾ test_program {
	ಪೂರ್ಣಾಙ್ಕ		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ಸಂರಚನಾ test_program prog[] = {
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

ಸಂರಚನಾ test_program extended[] = {
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
ಸ್ಥಿರ ಶೂನ್ಯ setup(ಶೂನ್ಯ)
{
	errcode_t	retval;
	ಸಂರಚನಾ ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, ಪರಿಮಾಣ(param));
	ext2fs_blocks_ಎಣಿಕೆ_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	ಯದಿ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ಯದಿ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

ಪೂರ್ಣಾಙ್ಕ run_test(ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ size, ವರ್ಣ *dir, ಸಂರಚನಾ test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ಸಂರಚನಾ test_program *pc;
	__u16		result;
	ಪೂರ್ಣಾಙ್ಕ		problem = 0;

	ಯದಿ (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ಯದಿ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#ಅನ್ಯಥಾ
		printf("Skipped\n");
		ಪ್ರತ್ಯಾಗಮ 0;
#endif
	} ಅನ್ಯಥಾ {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		ಯದಿ (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	ಪ್ರತಿ (pc = prog; pc->cmd != EXIT; pc++) {
		ವಿಕಲ್ಪ (pc->cmd) {
		ಪಕ್ಷ FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			ಭಙ್ಗ;
		ಪಕ್ಷ STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ಯದಿ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			ಭಙ್ಗ;
		ಪಕ್ಷ INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ಯದಿ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			ಭಙ್ಗ;
		ಪಕ್ಷ DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ಯದಿ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			ಭಙ್ಗ;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		ಯದಿ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		ಯದಿ (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ಯದಿ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	ಪ್ರತ್ಯಾಗಮ problem;
}


ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ failed = 0;

	setup();
	printf("Standard icount run:\n");
	failed += run_test(0, 0, 0, prog);
	printf("\nMultiple bitmap test:\n");
	failed += run_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	printf("\nResizing icount:\n");
	failed += run_test(0, 3, 0, extended);
	printf("\nStandard icount run with tdb:\n");
	failed += run_test(0, 0, ".", prog);
	printf("\nMultiple bitmap test with tdb:\n");
	failed += run_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	ಯದಿ (failed)
		printf("FAILED!\n");
	ಪ್ರತ್ಯಾಗಮ failed;
}
#endif
