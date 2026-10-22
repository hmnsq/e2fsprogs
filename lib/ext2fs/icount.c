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

#شامل "config.h"
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <stdio.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "tdb.h"

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

ساختار ext2_icount_el {
	ext2_ino_t	ino;
	__u32		شمار;
};

ساختار ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		شمار;
	ext2_ino_t		size;
	ext2_ino_t		رقم_inodes;
	ext2_ino_t		cursor;
	ساختار ext2_icount_el	*list;
	ساختار ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	نویسه			*tdb_fn;
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

تهی ext2fs_free_icount(ext2_icount_t icount)
{
	اگر (!icount)
		بازگشت;

	icount->magic = 0;
	اگر (icount->list)
		ext2fs_free_mem(&icount->list);
	اگر (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	اگر (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	اگر (icount->tdb)
		tdb_close(icount->tdb);
	اگر (icount->tdb_fn) {
		(تهی) unlink(icount->tdb_fn);
		آزاد(icount->tdb_fn);
	}
#endif

	اگر (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ایستا errcode_t alloc_icount(ext2_filsys fs, صحیح flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_icount), &icount);
	اگر (retval)
		بازگشت retval;
	memset(icount, 0, اندازه(ساختار ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->رقم_inodes = fs->super->s_inodes_شمار;

	اگر ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = اندازه(*icount->fullmap) * icount->رقم_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		اگر (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			بازگشت 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	اگر (retval)
		برو errout;

	اگر (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		اگر (retval)
			برو errout;
	} وگرنه
		icount->multiple = 0;

	*ret = icount;
	بازگشت 0;

errout:
	ext2fs_free_icount(icount);
	بازگشت(retval);
}

#ifdef CONFIG_TDB
ساختار uuid {
	__u32	time_کم;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	گره[6];
};

ایستا تهی unpack_uuid(تهی *in, ساختار uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_کم = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->گره, ptr, 6);
}

ایستا تهی uuid_unparse(تهی *uu, نویسه *out)
{
	ساختار uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_کم, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.گره[0], uuid.گره[1], uuid.گره[2],
		uuid.گره[3], uuid.گره[4], uuid.گره[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   نویسه *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   صحیح flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	نویسه 		*fn, uuid[40];
	ext2_ino_t	رقم_inodes;
	mode_t		save_umask;
	صحیح		fd;

	retval = alloc_icount(fs, flags,  &icount);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	اگر (retval)
		برو errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	اگر (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		برو errout;
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
	رقم_inodes = fs->super->s_inodes_شمار - fs->super->s_free_inodes_شمار;

	icount->tdb = tdb_open(fn, رقم_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	اگر (icount->tdb == پوچ) {
		retval = errno;
		برو errout;
	}
	*ret = icount;
	بازگشت 0;
errout:
	ext2fs_free_icount(icount);
	بازگشت(retval);
#وگرنه
	بازگشت EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, صحیح flags, unsigned صحیح size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	اگر (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		اگر (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	اگر (retval)
		بازگشت retval;

	اگر (icount->fullmap)
		برو successout;

	اگر (size) {
		icount->size = size;
	} وگرنه {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_رقم_dirs(fs, &icount->size);
		اگر (retval)
			برو errout;
		icount->size += fs->super->s_inodes_شمار / 50;
	}

	bytes = (size_t) (icount->size * اندازه(ساختار ext2_icount_el));
#اگر 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, اندازه(ساختار ext2_icount_el),
			 &icount->list);
	اگر (retval)
		برو errout;
	memset(icount->list, 0, bytes);

	icount->شمار = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	اگر (hint) {
		برای (i=0; i < hint->شمار; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->شمار = hint->شمار;
	}

successout:
	*ret = icount;
	بازگشت 0;

errout:
	ext2fs_free_icount(icount);
	بازگشت(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, صحیح flags,
			       unsigned صحیح size,
			       ext2_icount_t *ret)
{
	بازگشت ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ایستا ساختار ext2_icount_el *درج_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحیح pos)
{
	ساختار ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	صحیح			رقم;

	اگر (icount->last_lookup && icount->last_lookup->ino == ino)
		بازگشت icount->last_lookup;

	اگر (icount->شمار >= icount->size) {
		اگر (icount->شمار) {
			new_size = icount->list[(unsigned)icount->شمار-1].ino;
			new_size = (ext2_ino_t) (icount->شمار *
				((اعشاری) icount->رقم_inodes / new_size));
		}
		اگر (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#اگر 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   اندازه(ساختار ext2_icount_el),
					   (size_t) new_size *
					   اندازه(ساختار ext2_icount_el),
					   &icount->list);
		اگر (retval)
			بازگشت 0;
		icount->size = new_size;
	}
	رقم = (صحیح) icount->شمار - pos;
	اگر (رقم < 0)
		بازگشت 0;	/* should never happen */
	اگر (رقم) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			اندازه(ساختار ext2_icount_el) * رقم);
	}
	icount->شمار++;
	el = &icount->list[pos];
	el->شمار = 0;
	el->ino = ino;
	icount->last_lookup = el;
	بازگشت el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ایستا ساختار ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحیح create)
{
	صحیح	کم, بالا, mid;

	اگر (!icount || !icount->list)
		بازگشت 0;

	اگر (create && ((icount->شمار == 0) ||
		       (ino > icount->list[(unsigned)icount->شمار-1].ino))) {
		بازگشت درج_icount_el(icount, ino, (unsigned) icount->شمار);
	}
	اگر (icount->شمار == 0)
		بازگشت 0;

	اگر (icount->cursor >= icount->شمار)
		icount->cursor = 0;
	اگر (ino == icount->list[icount->cursor].ino)
		بازگشت &icount->list[icount->cursor++];
#اگر 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	کم = 0;
	بالا = (صحیح) icount->شمار-1;
	وقتی (کم <= بالا) {
		mid = ((unsigned)کم + (unsigned)بالا) >> 1;
		اگر (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			بازگشت &icount->list[mid];
		}
		اگر (ino < icount->list[mid].ino)
			بالا = mid-1;
		وگرنه
			کم = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	اگر (create)
		بازگشت درج_icount_el(icount, ino, کم);
	بازگشت 0;
}

ایستا errcode_t set_inode_شمار(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 شمار)
{
	ساختار ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA کلید, data;

	اگر (icount->tdb) {
		کلید.dptr = (unsigned نویسه *) &ino;
		کلید.dsize = اندازه(ext2_ino_t);
		data.dptr = (unsigned نویسه *) &شمار;
		data.dsize = اندازه(__u32);
		اگر (شمار) {
			اگر (tdb_store(icount->tdb, کلید, data, TDB_REPLACE))
				بازگشت tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} وگرنه {
			اگر (tdb_delete(icount->tdb, کلید))
				بازگشت tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		بازگشت 0;
	}
#endif
	اگر (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(شمار);
		بازگشت 0;
	}

	el = get_icount_el(icount, ino, 1);
	اگر (!el)
		بازگشت EXT2_ET_NO_MEMORY;

	el->شمار = شمار;
	بازگشت 0;
}

ایستا errcode_t get_inode_شمار(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *شمار)
{
	ساختار ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA کلید, data;

	اگر (icount->tdb) {
		کلید.dptr = (unsigned نویسه *) &ino;
		کلید.dsize = اندازه(ext2_ino_t);

		data = tdb_fetch(icount->tdb, کلید);
		اگر (data.dptr == پوچ) {
			*شمار = 0;
			بازگشت tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*شمار = *((__u32 *) data.dptr);
		آزاد(data.dptr);
		بازگشت 0;
	}
#endif
	اگر (icount->fullmap) {
		*شمار = icount->fullmap[ino];
		بازگشت 0;
	}

	el = get_icount_el(icount, ino, 0);
	اگر (!el) {
		*شمار = 0;
		بازگشت ENOENT;
	}

	*شمار = el->شمار;
	بازگشت 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned صحیح	i;
	ثابت نویسه *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->شمار > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	}
	برای (i=1; i < icount->شمار; i++) {
		اگر (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	بازگشت ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (!ino || (ino > icount->رقم_inodes))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (!icount->fullmap) {
		اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			بازگشت 0;
		}
		اگر (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			بازگشت 0;
		}
	}
	get_inode_شمار(icount, ino, &val);
	*ret = icount_16_xlate(val);
	بازگشت 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_مقدار;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (!ino || (ino > icount->رقم_inodes))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (icount->fullmap) {
		curr_مقدار = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_مقدار;
	} وگرنه اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		اگر (set_inode_شمار(icount, ino, 2))
			بازگشت EXT2_ET_NO_MEMORY;
		curr_مقدار = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} وگرنه اگر (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		اگر (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_شمار(icount, ino, &curr_مقدار);
			curr_مقدار++;
			اگر (set_inode_شمار(icount, ino, curr_مقدار))
				بازگشت EXT2_ET_NO_MEMORY;
		} وگرنه {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			اگر (ret)
				*ret = 1;
			بازگشت 0;
		}
	} وگرنه {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_شمار(icount, ino, &curr_مقدار);
		curr_مقدار++;
		اگر (set_inode_شمار(icount, ino, curr_مقدار))
			بازگشت EXT2_ET_NO_MEMORY;
	}
	اگر (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	اگر (ret)
		*ret = icount_16_xlate(curr_مقدار);
	بازگشت 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_مقدار;

	اگر (!ino || (ino > icount->رقم_inodes))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->fullmap) {
		اگر (!icount->fullmap[ino])
			بازگشت EXT2_ET_INVALID_ARGUMENT;

		curr_مقدار = --icount->fullmap[ino];
		اگر (ret)
			*ret = icount_16_xlate(curr_مقدار);
		بازگشت 0;
	}

	اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		اگر (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		وگرنه {
			set_inode_شمار(icount, ino, 0);
		}
		اگر (ret)
			*ret = 0;
		بازگشت 0;
	}

	اگر (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	get_inode_شمار(icount, ino, &curr_مقدار);
	اگر (!curr_مقدار)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	curr_مقدار--;
	اگر (set_inode_شمار(icount, ino, curr_مقدار))
		بازگشت EXT2_ET_NO_MEMORY;

	اگر (curr_مقدار == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	اگر ((curr_مقدار == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	اگر (ret)
		*ret = icount_16_xlate(curr_مقدار);
	بازگشت 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 شمار)
{
	اگر (!ino || (ino > icount->رقم_inodes))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->fullmap)
		بازگشت set_inode_شمار(icount, ino, شمار);

	اگر (شمار == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		اگر (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		بازگشت 0;
	}
	اگر (شمار == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		اگر (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} وگرنه
			set_inode_شمار(icount, ino, 0);
		بازگشت 0;
	}

	اگر (set_inode_شمار(icount, ino, شمار))
		بازگشت EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	اگر (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	بازگشت 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	اگر (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		بازگشت 0;

	بازگشت icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

ساختار test_program {
	صحیح		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ساختار test_program prog[] = {
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

ساختار test_program extended[] = {
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
ایستا تهی setup(تهی)
{
	errcode_t	retval;
	ساختار ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, اندازه(param));
	ext2fs_blocks_شمار_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

صحیح run_test(صحیح flags, صحیح size, نویسه *dir, ساختار test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ساختار test_program *pc;
	__u16		result;
	صحیح		problem = 0;

	اگر (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		اگر (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#وگرنه
		printf("Skipped\n");
		بازگشت 0;
#endif
	} وگرنه {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		اگر (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	برای (pc = prog; pc->cmd != EXIT; pc++) {
		انتخاب (pc->cmd) {
		حالت FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			بشکن;
		حالت STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			بشکن;
		حالت INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			بشکن;
		حالت DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			بشکن;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		اگر (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		اگر (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	اگر (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	بازگشت problem;
}


صحیح اصلی(صحیح argc, نویسه **argv)
{
	صحیح failed = 0;

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
	اگر (failed)
		printf("FAILED!\n");
	بازگشت failed;
}
#endif
