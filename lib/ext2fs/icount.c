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

#அந்தர்பாவ "config.h"
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "tdb.h"

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

கட்டமைப்பு ext2_icount_el {
	ext2_ino_t	ino;
	__u32		மொத்தம்;
};

கட்டமைப்பு ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		மொத்தம்;
	ext2_ino_t		size;
	ext2_ino_t		இலக்கம்_inodes;
	ext2_ino_t		cursor;
	கட்டமைப்பு ext2_icount_el	*list;
	கட்டமைப்பு ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	எழுத்து			*tdb_fn;
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

வெறுமை ext2fs_free_icount(ext2_icount_t icount)
{
	எனில் (!icount)
		திரும்பு;

	icount->magic = 0;
	எனில் (icount->list)
		ext2fs_free_mem(&icount->list);
	எனில் (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	எனில் (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	எனில் (icount->tdb)
		tdb_close(icount->tdb);
	எனில் (icount->tdb_fn) {
		(வெறுமை) unlink(icount->tdb_fn);
		விடுவி(icount->tdb_fn);
	}
#endif

	எனில் (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

நிலையான errcode_t alloc_icount(ext2_filsys fs, முழு flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_icount), &icount);
	எனில் (retval)
		திரும்பு retval;
	memset(icount, 0, அளவு(கட்டமைப்பு ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->இலக்கம்_inodes = fs->super->s_inodes_மொத்தம்;

	எனில் ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = அளவு(*icount->fullmap) * icount->இலக்கம்_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		எனில் (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			திரும்பு 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	எனில் (retval)
		செல் errout;

	எனில் (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		எனில் (retval)
			செல் errout;
	} இல்லையெனில்
		icount->multiple = 0;

	*ret = icount;
	திரும்பு 0;

errout:
	ext2fs_free_icount(icount);
	திரும்பு(retval);
}

#ifdef CONFIG_TDB
கட்டமைப்பு uuid {
	__u32	time_கீழ்;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	முனை[6];
};

நிலையான வெறுமை unpack_uuid(வெறுமை *in, கட்டமைப்பு uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_கீழ் = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->முனை, ptr, 6);
}

நிலையான வெறுமை uuid_unparse(வெறுமை *uu, எழுத்து *out)
{
	கட்டமைப்பு uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_கீழ், uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.முனை[0], uuid.முனை[1], uuid.முனை[2],
		uuid.முனை[3], uuid.முனை[4], uuid.முனை[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   எழுத்து *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   முழு flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	எழுத்து 		*fn, uuid[40];
	ext2_ino_t	இலக்கம்_inodes;
	mode_t		save_umask;
	முழு		fd;

	retval = alloc_icount(fs, flags,  &icount);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	எனில் (retval)
		செல் errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	எனில் (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		செல் errout;
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
	இலக்கம்_inodes = fs->super->s_inodes_மொத்தம் - fs->super->s_free_inodes_மொத்தம்;

	icount->tdb = tdb_open(fn, இலக்கம்_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	எனில் (icount->tdb == வெற்று) {
		retval = errno;
		செல் errout;
	}
	*ret = icount;
	திரும்பு 0;
errout:
	ext2fs_free_icount(icount);
	திரும்பு(retval);
#இல்லையெனில்
	திரும்பு EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, முழு flags, unsigned முழு size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	எனில் (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		எனில் (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	எனில் (retval)
		திரும்பு retval;

	எனில் (icount->fullmap)
		செல் successout;

	எனில் (size) {
		icount->size = size;
	} இல்லையெனில் {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_இலக்கம்_dirs(fs, &icount->size);
		எனில் (retval)
			செல் errout;
		icount->size += fs->super->s_inodes_மொத்தம் / 50;
	}

	bytes = (size_t) (icount->size * அளவு(கட்டமைப்பு ext2_icount_el));
#எனில் 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, அளவு(கட்டமைப்பு ext2_icount_el),
			 &icount->list);
	எனில் (retval)
		செல் errout;
	memset(icount->list, 0, bytes);

	icount->மொத்தம் = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	எனில் (hint) {
		க்கு (i=0; i < hint->மொத்தம்; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->மொத்தம் = hint->மொத்தம்;
	}

successout:
	*ret = icount;
	திரும்பு 0;

errout:
	ext2fs_free_icount(icount);
	திரும்பு(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, முழு flags,
			       unsigned முழு size,
			       ext2_icount_t *ret)
{
	திரும்பு ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
நிலையான கட்டமைப்பு ext2_icount_el *செருகு_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, முழு pos)
{
	கட்டமைப்பு ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	முழு			இலக்கம்;

	எனில் (icount->last_lookup && icount->last_lookup->ino == ino)
		திரும்பு icount->last_lookup;

	எனில் (icount->மொத்தம் >= icount->size) {
		எனில் (icount->மொத்தம்) {
			new_size = icount->list[(unsigned)icount->மொத்தம்-1].ino;
			new_size = (ext2_ino_t) (icount->மொத்தம் *
				((தசம) icount->இலக்கம்_inodes / new_size));
		}
		எனில் (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#எனில் 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   அளவு(கட்டமைப்பு ext2_icount_el),
					   (size_t) new_size *
					   அளவு(கட்டமைப்பு ext2_icount_el),
					   &icount->list);
		எனில் (retval)
			திரும்பு 0;
		icount->size = new_size;
	}
	இலக்கம் = (முழு) icount->மொத்தம் - pos;
	எனில் (இலக்கம் < 0)
		திரும்பு 0;	/* should never happen */
	எனில் (இலக்கம்) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			அளவு(கட்டமைப்பு ext2_icount_el) * இலக்கம்);
	}
	icount->மொத்தம்++;
	el = &icount->list[pos];
	el->மொத்தம் = 0;
	el->ino = ino;
	icount->last_lookup = el;
	திரும்பு el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
நிலையான கட்டமைப்பு ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, முழு create)
{
	முழு	கீழ், உயர்வு, mid;

	எனில் (!icount || !icount->list)
		திரும்பு 0;

	எனில் (create && ((icount->மொத்தம் == 0) ||
		       (ino > icount->list[(unsigned)icount->மொத்தம்-1].ino))) {
		திரும்பு செருகு_icount_el(icount, ino, (unsigned) icount->மொத்தம்);
	}
	எனில் (icount->மொத்தம் == 0)
		திரும்பு 0;

	எனில் (icount->cursor >= icount->மொத்தம்)
		icount->cursor = 0;
	எனில் (ino == icount->list[icount->cursor].ino)
		திரும்பு &icount->list[icount->cursor++];
#எனில் 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	கீழ் = 0;
	உயர்வு = (முழு) icount->மொத்தம்-1;
	போது (கீழ் <= உயர்வு) {
		mid = ((unsigned)கீழ் + (unsigned)உயர்வு) >> 1;
		எனில் (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			திரும்பு &icount->list[mid];
		}
		எனில் (ino < icount->list[mid].ino)
			உயர்வு = mid-1;
		இல்லையெனில்
			கீழ் = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	எனில் (create)
		திரும்பு செருகு_icount_el(icount, ino, கீழ்);
	திரும்பு 0;
}

நிலையான errcode_t set_inode_மொத்தம்(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 மொத்தம்)
{
	கட்டமைப்பு ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA சாவி, data;

	எனில் (icount->tdb) {
		சாவி.dptr = (unsigned எழுத்து *) &ino;
		சாவி.dsize = அளவு(ext2_ino_t);
		data.dptr = (unsigned எழுத்து *) &மொத்தம்;
		data.dsize = அளவு(__u32);
		எனில் (மொத்தம்) {
			எனில் (tdb_store(icount->tdb, சாவி, data, TDB_REPLACE))
				திரும்பு tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} இல்லையெனில் {
			எனில் (tdb_delete(icount->tdb, சாவி))
				திரும்பு tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		திரும்பு 0;
	}
#endif
	எனில் (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(மொத்தம்);
		திரும்பு 0;
	}

	el = get_icount_el(icount, ino, 1);
	எனில் (!el)
		திரும்பு EXT2_ET_NO_MEMORY;

	el->மொத்தம் = மொத்தம்;
	திரும்பு 0;
}

நிலையான errcode_t get_inode_மொத்தம்(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *மொத்தம்)
{
	கட்டமைப்பு ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA சாவி, data;

	எனில் (icount->tdb) {
		சாவி.dptr = (unsigned எழுத்து *) &ino;
		சாவி.dsize = அளவு(ext2_ino_t);

		data = tdb_fetch(icount->tdb, சாவி);
		எனில் (data.dptr == வெற்று) {
			*மொத்தம் = 0;
			திரும்பு tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*மொத்தம் = *((__u32 *) data.dptr);
		விடுவி(data.dptr);
		திரும்பு 0;
	}
#endif
	எனில் (icount->fullmap) {
		*மொத்தம் = icount->fullmap[ino];
		திரும்பு 0;
	}

	el = get_icount_el(icount, ino, 0);
	எனில் (!el) {
		*மொத்தம் = 0;
		திரும்பு ENOENT;
	}

	*மொத்தம் = el->மொத்தம்;
	திரும்பு 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned முழு	i;
	மாறிலி எழுத்து *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	எனில் (icount->மொத்தம் > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	}
	க்கு (i=1; i < icount->மொத்தம்; i++) {
		எனில் (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	திரும்பு ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	எனில் (!ino || (ino > icount->இலக்கம்_inodes))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (!icount->fullmap) {
		எனில் (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			திரும்பு 0;
		}
		எனில் (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			திரும்பு 0;
		}
	}
	get_inode_மொத்தம்(icount, ino, &val);
	*ret = icount_16_xlate(val);
	திரும்பு 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_மதிப்பு;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	எனில் (!ino || (ino > icount->இலக்கம்_inodes))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (icount->fullmap) {
		curr_மதிப்பு = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_மதிப்பு;
	} இல்லையெனில் எனில் (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		எனில் (set_inode_மொத்தம்(icount, ino, 2))
			திரும்பு EXT2_ET_NO_MEMORY;
		curr_மதிப்பு = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} இல்லையெனில் எனில் (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		எனில் (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_மொத்தம்(icount, ino, &curr_மதிப்பு);
			curr_மதிப்பு++;
			எனில் (set_inode_மொத்தம்(icount, ino, curr_மதிப்பு))
				திரும்பு EXT2_ET_NO_MEMORY;
		} இல்லையெனில் {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			எனில் (ret)
				*ret = 1;
			திரும்பு 0;
		}
	} இல்லையெனில் {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_மொத்தம்(icount, ino, &curr_மதிப்பு);
		curr_மதிப்பு++;
		எனில் (set_inode_மொத்தம்(icount, ino, curr_மதிப்பு))
			திரும்பு EXT2_ET_NO_MEMORY;
	}
	எனில் (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	எனில் (ret)
		*ret = icount_16_xlate(curr_மதிப்பு);
	திரும்பு 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_மதிப்பு;

	எனில் (!ino || (ino > icount->இலக்கம்_inodes))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	எனில் (icount->fullmap) {
		எனில் (!icount->fullmap[ino])
			திரும்பு EXT2_ET_INVALID_ARGUMENT;

		curr_மதிப்பு = --icount->fullmap[ino];
		எனில் (ret)
			*ret = icount_16_xlate(curr_மதிப்பு);
		திரும்பு 0;
	}

	எனில் (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		எனில் (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		இல்லையெனில் {
			set_inode_மொத்தம்(icount, ino, 0);
		}
		எனில் (ret)
			*ret = 0;
		திரும்பு 0;
	}

	எனில் (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	get_inode_மொத்தம்(icount, ino, &curr_மதிப்பு);
	எனில் (!curr_மதிப்பு)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	curr_மதிப்பு--;
	எனில் (set_inode_மொத்தம்(icount, ino, curr_மதிப்பு))
		திரும்பு EXT2_ET_NO_MEMORY;

	எனில் (curr_மதிப்பு == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	எனில் ((curr_மதிப்பு == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	எனில் (ret)
		*ret = icount_16_xlate(curr_மதிப்பு);
	திரும்பு 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 மொத்தம்)
{
	எனில் (!ino || (ino > icount->இலக்கம்_inodes))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	எனில் (icount->fullmap)
		திரும்பு set_inode_மொத்தம்(icount, ino, மொத்தம்);

	எனில் (மொத்தம் == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		எனில் (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		திரும்பு 0;
	}
	எனில் (மொத்தம் == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		எனில் (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} இல்லையெனில்
			set_inode_மொத்தம்(icount, ino, 0);
		திரும்பு 0;
	}

	எனில் (set_inode_மொத்தம்(icount, ino, மொத்தம்))
		திரும்பு EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	எனில் (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	திரும்பு 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	எனில் (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		திரும்பு 0;

	திரும்பு icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

கட்டமைப்பு test_program {
	முழு		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

கட்டமைப்பு test_program prog[] = {
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

கட்டமைப்பு test_program extended[] = {
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
நிலையான வெறுமை setup(வெறுமை)
{
	errcode_t	retval;
	கட்டமைப்பு ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, அளவு(param));
	ext2fs_blocks_மொத்தம்_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	எனில் (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	எனில் (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

முழு run_test(முழு flags, முழு size, எழுத்து *dir, கட்டமைப்பு test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	கட்டமைப்பு test_program *pc;
	__u16		result;
	முழு		problem = 0;

	எனில் (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		எனில் (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#இல்லையெனில்
		printf("Skipped\n");
		திரும்பு 0;
#endif
	} இல்லையெனில் {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		எனில் (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	க்கு (pc = prog; pc->cmd != EXIT; pc++) {
		தேர்வு (pc->cmd) {
		நிலை FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			நிறுத்து;
		நிலை STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			எனில் (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			நிறுத்து;
		நிலை INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			எனில் (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			நிறுத்து;
		நிலை DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			எனில் (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			நிறுத்து;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		எனில் (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		எனில் (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	எனில் (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	திரும்பு problem;
}


முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	முழு failed = 0;

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
	எனில் (failed)
		printf("FAILED!\n");
	திரும்பு failed;
}
#endif
