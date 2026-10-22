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

#অন্তর্ভাব "config.h"
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "tdb.h"

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

কাঠামো ext2_icount_el {
	ext2_ino_t	ino;
	__u32		গুনতি;
};

কাঠামো ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		গুনতি;
	ext2_ino_t		আকার_2;
	ext2_ino_t		অঙ্ক_inodes;
	ext2_ino_t		cursor;
	কাঠামো ext2_icount_el	*list;
	কাঠামো ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	অক্ষর			*tdb_fn;
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

শূন্য ext2fs_free_icount(ext2_icount_t icount)
{
	যদি (!icount)
		ফেরত;

	icount->magic = 0;
	যদি (icount->list)
		ext2fs_free_mem(&icount->list);
	যদি (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	যদি (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	যদি (icount->tdb)
		tdb_close(icount->tdb);
	যদি (icount->tdb_fn) {
		(শূন্য) unlink(icount->tdb_fn);
		মুক্ত(icount->tdb_fn);
	}
#endif

	যদি (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

স্থির errcode_t alloc_icount(ext2_filsys fs, পূর্ণসংখ্যা flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(আকার(কাঠামো ext2_icount), &icount);
	যদি (retval)
		ফেরত retval;
	memset(icount, 0, আকার(কাঠামো ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->অঙ্ক_inodes = fs->super->s_inodes_গুনতি;

	যদি ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = আকার(*icount->fullmap) * icount->অঙ্ক_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		যদি (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ফেরত 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	যদি (retval)
		যাও errout;

	যদি (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		যদি (retval)
			যাও errout;
	} অন্যথা
		icount->multiple = 0;

	*ret = icount;
	ফেরত 0;

errout:
	ext2fs_free_icount(icount);
	ফেরত(retval);
}

#ifdef CONFIG_TDB
কাঠামো uuid {
	__u32	time_নিচের;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	গ্রন্থি[6];
};

স্থির শূন্য unpack_uuid(শূন্য *in, কাঠামো uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_নিচের = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->গ্রন্থি, ptr, 6);
}

স্থির শূন্য uuid_unparse(শূন্য *uu, অক্ষর *out)
{
	কাঠামো uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_নিচের, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.গ্রন্থি[0], uuid.গ্রন্থি[1], uuid.গ্রন্থি[2],
		uuid.গ্রন্থি[3], uuid.গ্রন্থি[4], uuid.গ্রন্থি[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   অক্ষর *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   পূর্ণসংখ্যা flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	অক্ষর 		*fn, uuid[40];
	ext2_ino_t	অঙ্ক_inodes;
	mode_t		save_umask;
	পূর্ণসংখ্যা		fd;

	retval = alloc_icount(fs, flags,  &icount);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	যদি (retval)
		যাও errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	যদি (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		যাও errout;
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
	অঙ্ক_inodes = fs->super->s_inodes_গুনতি - fs->super->s_free_inodes_গুনতি;

	icount->tdb = tdb_open(fn, অঙ্ক_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	যদি (icount->tdb == ফাঁকা) {
		retval = errno;
		যাও errout;
	}
	*ret = icount;
	ফেরত 0;
errout:
	ext2fs_free_icount(icount);
	ফেরত(retval);
#অন্যথা
	ফেরত EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, পূর্ণসংখ্যা flags, unsigned পূর্ণসংখ্যা আকার_2,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	যদি (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		যদি (hint->আকার_2 > আকার_2)
			আকার_2 = (size_t) hint->আকার_2;
	}

	retval = alloc_icount(fs, flags, &icount);
	যদি (retval)
		ফেরত retval;

	যদি (icount->fullmap)
		যাও successout;

	যদি (আকার_2) {
		icount->আকার_2 = আকার_2;
	} অন্যথা {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_অঙ্ক_dirs(fs, &icount->আকার_2);
		যদি (retval)
			যাও errout;
		icount->আকার_2 += fs->super->s_inodes_গুনতি / 50;
	}

	bytes = (size_t) (icount->আকার_2 * আকার(কাঠামো ext2_icount_el));
#যদি 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->আকার_2, bytes);
#endif
	retval = ext2fs_get_array(icount->আকার_2, আকার(কাঠামো ext2_icount_el),
			 &icount->list);
	যদি (retval)
		যাও errout;
	memset(icount->list, 0, bytes);

	icount->গুনতি = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	যদি (hint) {
		জন্য (i=0; i < hint->গুনতি; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->গুনতি = hint->গুনতি;
	}

successout:
	*ret = icount;
	ফেরত 0;

errout:
	ext2fs_free_icount(icount);
	ফেরত(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, পূর্ণসংখ্যা flags,
			       unsigned পূর্ণসংখ্যা আকার_2,
			       ext2_icount_t *ret)
{
	ফেরত ext2fs_create_icount2(fs, flags, আকার_2, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
স্থির কাঠামো ext2_icount_el *ঢোকাও_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, পূর্ণসংখ্যা pos)
{
	কাঠামো ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_আকার = 0;
	পূর্ণসংখ্যা			অঙ্ক;

	যদি (icount->last_lookup && icount->last_lookup->ino == ino)
		ফেরত icount->last_lookup;

	যদি (icount->গুনতি >= icount->আকার_2) {
		যদি (icount->গুনতি) {
			new_আকার = icount->list[(unsigned)icount->গুনতি-1].ino;
			new_আকার = (ext2_ino_t) (icount->গুনতি *
				((দশমিক) icount->অঙ্ক_inodes / new_আকার));
		}
		যদি (new_আকার < (icount->আকার_2 + 100))
			new_আকার = icount->আকার_2 + 100;
#যদি 0
		printf("Reallocating icount %u entries...\n", new_আকার);
#endif
		retval = ext2fs_resize_mem((size_t) icount->আকার_2 *
					   আকার(কাঠামো ext2_icount_el),
					   (size_t) new_আকার *
					   আকার(কাঠামো ext2_icount_el),
					   &icount->list);
		যদি (retval)
			ফেরত 0;
		icount->আকার_2 = new_আকার;
	}
	অঙ্ক = (পূর্ণসংখ্যা) icount->গুনতি - pos;
	যদি (অঙ্ক < 0)
		ফেরত 0;	/* should never happen */
	যদি (অঙ্ক) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			আকার(কাঠামো ext2_icount_el) * অঙ্ক);
	}
	icount->গুনতি++;
	el = &icount->list[pos];
	el->গুনতি = 0;
	el->ino = ino;
	icount->last_lookup = el;
	ফেরত el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
স্থির কাঠামো ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, পূর্ণসংখ্যা create)
{
	পূর্ণসংখ্যা	নিচের, উপরের, mid;

	যদি (!icount || !icount->list)
		ফেরত 0;

	যদি (create && ((icount->গুনতি == 0) ||
		       (ino > icount->list[(unsigned)icount->গুনতি-1].ino))) {
		ফেরত ঢোকাও_icount_el(icount, ino, (unsigned) icount->গুনতি);
	}
	যদি (icount->গুনতি == 0)
		ফেরত 0;

	যদি (icount->cursor >= icount->গুনতি)
		icount->cursor = 0;
	যদি (ino == icount->list[icount->cursor].ino)
		ফেরত &icount->list[icount->cursor++];
#যদি 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	নিচের = 0;
	উপরের = (পূর্ণসংখ্যা) icount->গুনতি-1;
	যখন (নিচের <= উপরের) {
		mid = ((unsigned)নিচের + (unsigned)উপরের) >> 1;
		যদি (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ফেরত &icount->list[mid];
		}
		যদি (ino < icount->list[mid].ino)
			উপরের = mid-1;
		অন্যথা
			নিচের = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	যদি (create)
		ফেরত ঢোকাও_icount_el(icount, ino, নিচের);
	ফেরত 0;
}

স্থির errcode_t set_inode_গুনতি(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 গুনতি)
{
	কাঠামো ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_তথ্য চাবি, তথ্য;

	যদি (icount->tdb) {
		চাবি.dptr = (unsigned অক্ষর *) &ino;
		চাবি.dsize = আকার(ext2_ino_t);
		তথ্য.dptr = (unsigned অক্ষর *) &গুনতি;
		তথ্য.dsize = আকার(__u32);
		যদি (গুনতি) {
			যদি (tdb_store(icount->tdb, চাবি, তথ্য, TDB_REPLACE))
				ফেরত tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} অন্যথা {
			যদি (tdb_delete(icount->tdb, চাবি))
				ফেরত tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ফেরত 0;
	}
#endif
	যদি (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(গুনতি);
		ফেরত 0;
	}

	el = get_icount_el(icount, ino, 1);
	যদি (!el)
		ফেরত EXT2_ET_NO_MEMORY;

	el->গুনতি = গুনতি;
	ফেরত 0;
}

স্থির errcode_t get_inode_গুনতি(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *গুনতি)
{
	কাঠামো ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_তথ্য চাবি, তথ্য;

	যদি (icount->tdb) {
		চাবি.dptr = (unsigned অক্ষর *) &ino;
		চাবি.dsize = আকার(ext2_ino_t);

		তথ্য = tdb_fetch(icount->tdb, চাবি);
		যদি (তথ্য.dptr == ফাঁকা) {
			*গুনতি = 0;
			ফেরত tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*গুনতি = *((__u32 *) তথ্য.dptr);
		মুক্ত(তথ্য.dptr);
		ফেরত 0;
	}
#endif
	যদি (icount->fullmap) {
		*গুনতি = icount->fullmap[ino];
		ফেরত 0;
	}

	el = get_icount_el(icount, ino, 0);
	যদি (!el) {
		*গুনতি = 0;
		ফেরত ENOENT;
	}

	*গুনতি = el->গুনতি;
	ফেরত 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned পূর্ণসংখ্যা	i;
	ধ্রুবক অক্ষর *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	যদি (icount->গুনতি > icount->আকার_2) {
		fprintf(out, "%s: count > size\n", bad);
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	}
	জন্য (i=1; i < icount->গুনতি; i++) {
		যদি (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	ফেরত ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	যদি (!ino || (ino > icount->অঙ্ক_inodes))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (!icount->fullmap) {
		যদি (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ফেরত 0;
		}
		যদি (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			ফেরত 0;
		}
	}
	get_inode_গুনতি(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ফেরত 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_মান;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	যদি (!ino || (ino > icount->অঙ্ক_inodes))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (icount->fullmap) {
		curr_মান = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_মান;
	} অন্যথা যদি (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		যদি (set_inode_গুনতি(icount, ino, 2))
			ফেরত EXT2_ET_NO_MEMORY;
		curr_মান = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} অন্যথা যদি (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		যদি (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_গুনতি(icount, ino, &curr_মান);
			curr_মান++;
			যদি (set_inode_গুনতি(icount, ino, curr_মান))
				ফেরত EXT2_ET_NO_MEMORY;
		} অন্যথা {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			যদি (ret)
				*ret = 1;
			ফেরত 0;
		}
	} অন্যথা {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_গুনতি(icount, ino, &curr_মান);
		curr_মান++;
		যদি (set_inode_গুনতি(icount, ino, curr_মান))
			ফেরত EXT2_ET_NO_MEMORY;
	}
	যদি (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	যদি (ret)
		*ret = icount_16_xlate(curr_মান);
	ফেরত 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_মান;

	যদি (!ino || (ino > icount->অঙ্ক_inodes))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	যদি (icount->fullmap) {
		যদি (!icount->fullmap[ino])
			ফেরত EXT2_ET_INVALID_ARGUMENT;

		curr_মান = --icount->fullmap[ino];
		যদি (ret)
			*ret = icount_16_xlate(curr_মান);
		ফেরত 0;
	}

	যদি (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		যদি (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		অন্যথা {
			set_inode_গুনতি(icount, ino, 0);
		}
		যদি (ret)
			*ret = 0;
		ফেরত 0;
	}

	যদি (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	get_inode_গুনতি(icount, ino, &curr_মান);
	যদি (!curr_মান)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	curr_মান--;
	যদি (set_inode_গুনতি(icount, ino, curr_মান))
		ফেরত EXT2_ET_NO_MEMORY;

	যদি (curr_মান == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	যদি ((curr_মান == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	যদি (ret)
		*ret = icount_16_xlate(curr_মান);
	ফেরত 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 গুনতি)
{
	যদি (!ino || (ino > icount->অঙ্ক_inodes))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	যদি (icount->fullmap)
		ফেরত set_inode_গুনতি(icount, ino, গুনতি);

	যদি (গুনতি == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		যদি (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ফেরত 0;
	}
	যদি (গুনতি == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		যদি (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} অন্যথা
			set_inode_গুনতি(icount, ino, 0);
		ফেরত 0;
	}

	যদি (set_inode_গুনতি(icount, ino, গুনতি))
		ফেরত EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	যদি (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ফেরত 0;
}

ext2_ino_t ext2fs_get_icount_আকার(ext2_icount_t icount)
{
	যদি (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ফেরত 0;

	ফেরত icount->আকার_2;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

কাঠামো test_program {
	পূর্ণসংখ্যা		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

কাঠামো test_program prog[] = {
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

কাঠামো test_program extended[] = {
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
স্থির শূন্য setup(শূন্য)
{
	errcode_t	retval;
	কাঠামো ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, আকার(param));
	ext2fs_blocks_গুনতি_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	যদি (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	যদি (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

পূর্ণসংখ্যা run_test(পূর্ণসংখ্যা flags, পূর্ণসংখ্যা আকার_2, অক্ষর *dir, কাঠামো test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	কাঠামো test_program *pc;
	__u16		ফলাফল;
	পূর্ণসংখ্যা		problem = 0;

	যদি (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		যদি (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#অন্যথা
		printf("Skipped\n");
		ফেরত 0;
#endif
	} অন্যথা {
		retval = ext2fs_create_icount2(test_fs, flags, আকার_2, 0,
					       &icount);
		যদি (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	জন্য (pc = prog; pc->cmd != EXIT; pc++) {
		নির্বাচন (pc->cmd) {
		ক্ষেত্র FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			বিরতি;
		ক্ষেত্র STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			যদি (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			বিরতি;
		ক্ষেত্র INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			যদি (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			বিরতি;
		ক্ষেত্র DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			যদি (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			বিরতি;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &ফলাফল);
		যদি (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", ফলাফল, (ফলাফল == pc->expected) ?
		       "OK" : "NOT OK");
		যদি (ফলাফল != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_আকার(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	যদি (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	ফেরত problem;
}


পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	পূর্ণসংখ্যা failed = 0;

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
	যদি (failed)
		printf("FAILED!\n");
	ফেরত failed;
}
#endif
