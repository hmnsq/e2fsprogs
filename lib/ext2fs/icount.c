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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "tdb.h"

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

ସଂରଚନା ext2_icount_el {
	ext2_ino_t	ino;
	__u32		ଗଣନା_2;
};

ସଂରଚନା ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		ଗଣନା_2;
	ext2_ino_t		size;
	ext2_ino_t		ଅଙ୍କ_inodes;
	ext2_ino_t		cursor;
	ସଂରଚନା ext2_icount_el	*list;
	ସଂରଚନା ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	ଅକ୍ଷର			*tdb_fn;
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

ଶୂନ୍ୟ ext2fs_free_icount(ext2_icount_t icount)
{
	ଯଦି (!icount)
		ଫେରନ୍ତୁ;

	icount->magic = 0;
	ଯଦି (icount->list)
		ext2fs_free_mem(&icount->list);
	ଯଦି (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ଯଦି (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	ଯଦି (icount->tdb)
		tdb_close(icount->tdb);
	ଯଦି (icount->tdb_fn) {
		(ଶୂନ୍ୟ) unlink(icount->tdb_fn);
		ମୁକ୍ତ(icount->tdb_fn);
	}
#endif

	ଯଦି (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ସ୍ଥିର errcode_t alloc_icount(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_icount), &icount);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(icount, 0, ଆକାର(ସଂରଚନା ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->ଅଙ୍କ_inodes = fs->super->s_inodes_ଗଣନା;

	ଯଦି ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = ଆକାର(*icount->fullmap) * icount->ଅଙ୍କ_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ଯଦି (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ଫେରନ୍ତୁ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ଯଦି (retval)
		ଯାଅ errout;

	ଯଦି (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		ଯଦି (retval)
			ଯାଅ errout;
	} ଅନ୍ୟଥା
		icount->multiple = 0;

	*ret = icount;
	ଫେରନ୍ତୁ 0;

errout:
	ext2fs_free_icount(icount);
	ଫେରନ୍ତୁ(retval);
}

#ifdef CONFIG_TDB
ସଂରଚନା uuid {
	__u32	time_ତଳ;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	ଗଣ୍ଠି[6];
};

ସ୍ଥିର ଶୂନ୍ୟ unpack_uuid(ଶୂନ୍ୟ *in, ସଂରଚନା uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_ତଳ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->ଗଣ୍ଠି, ptr, 6);
}

ସ୍ଥିର ଶୂନ୍ୟ uuid_unparse(ଶୂନ୍ୟ *uu, ଅକ୍ଷର *out)
{
	ସଂରଚନା uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_ତଳ, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ଗଣ୍ଠି[0], uuid.ଗଣ୍ଠି[1], uuid.ଗଣ୍ଠି[2],
		uuid.ଗଣ୍ଠି[3], uuid.ଗଣ୍ଠି[4], uuid.ଗଣ୍ଠି[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   ଅକ୍ଷର *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	ଅକ୍ଷର 		*fn, uuid[40];
	ext2_ino_t	ଅଙ୍କ_inodes;
	mode_t		save_umask;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	ଯଦି (retval)
		ଯାଅ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ଯଦି (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		ଯାଅ errout;
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
	ଅଙ୍କ_inodes = fs->super->s_inodes_ଗଣନା - fs->super->s_free_inodes_ଗଣନା;

	icount->tdb = tdb_open(fn, ଅଙ୍କ_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	ଯଦି (icount->tdb == ଖାଲି) {
		retval = errno;
		ଯାଅ errout;
	}
	*ret = icount;
	ଫେରନ୍ତୁ 0;
errout:
	ext2fs_free_icount(icount);
	ଫେରନ୍ତୁ(retval);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	ଯଦି (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ଯଦି (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (icount->fullmap)
		ଯାଅ successout;

	ଯଦି (size) {
		icount->size = size;
	} ଅନ୍ୟଥା {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_ଅଙ୍କ_dirs(fs, &icount->size);
		ଯଦି (retval)
			ଯାଅ errout;
		icount->size += fs->super->s_inodes_ଗଣନା / 50;
	}

	bytes = (size_t) (icount->size * ଆକାର(ସଂରଚନା ext2_icount_el));
#ଯଦି 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, ଆକାର(ସଂରଚନା ext2_icount_el),
			 &icount->list);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(icount->list, 0, bytes);

	icount->ଗଣନା_2 = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ଯଦି (hint) {
		ପାଇଁ (i=0; i < hint->ଗଣନା_2; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->ଗଣନା_2 = hint->ଗଣନା_2;
	}

successout:
	*ret = icount;
	ଫେରନ୍ତୁ 0;

errout:
	ext2fs_free_icount(icount);
	ଫେରନ୍ତୁ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
			       ext2_icount_t *ret)
{
	ଫେରନ୍ତୁ ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ସ୍ଥିର ସଂରଚନା ext2_icount_el *ଭର୍ତ୍ତି_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
	ସଂରଚନା ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ଅଙ୍କ;

	ଯଦି (icount->last_lookup && icount->last_lookup->ino == ino)
		ଫେରନ୍ତୁ icount->last_lookup;

	ଯଦି (icount->ଗଣନା_2 >= icount->size) {
		ଯଦି (icount->ଗଣନା_2) {
			new_size = icount->list[(unsigned)icount->ଗଣନା_2-1].ino;
			new_size = (ext2_ino_t) (icount->ଗଣନା_2 *
				((ଦଶମିକ) icount->ଅଙ୍କ_inodes / new_size));
		}
		ଯଦି (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#ଯଦି 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   ଆକାର(ସଂରଚନା ext2_icount_el),
					   (size_t) new_size *
					   ଆକାର(ସଂରଚନା ext2_icount_el),
					   &icount->list);
		ଯଦି (retval)
			ଫେରନ୍ତୁ 0;
		icount->size = new_size;
	}
	ଅଙ୍କ = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) icount->ଗଣନା_2 - pos;
	ଯଦି (ଅଙ୍କ < 0)
		ଫେରନ୍ତୁ 0;	/* should never happen */
	ଯଦି (ଅଙ୍କ) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			ଆକାର(ସଂରଚନା ext2_icount_el) * ଅଙ୍କ);
	}
	icount->ଗଣନା_2++;
	el = &icount->list[pos];
	el->ଗଣନା_2 = 0;
	el->ino = ino;
	icount->last_lookup = el;
	ଫେରନ୍ତୁ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ସ୍ଥିର ସଂରଚନା ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା create)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ତଳ, ଉପର, mid;

	ଯଦି (!icount || !icount->list)
		ଫେରନ୍ତୁ 0;

	ଯଦି (create && ((icount->ଗଣନା_2 == 0) ||
		       (ino > icount->list[(unsigned)icount->ଗଣନା_2-1].ino))) {
		ଫେରନ୍ତୁ ଭର୍ତ୍ତି_icount_el(icount, ino, (unsigned) icount->ଗଣନା_2);
	}
	ଯଦି (icount->ଗଣନା_2 == 0)
		ଫେରନ୍ତୁ 0;

	ଯଦି (icount->cursor >= icount->ଗଣନା_2)
		icount->cursor = 0;
	ଯଦି (ino == icount->list[icount->cursor].ino)
		ଫେରନ୍ତୁ &icount->list[icount->cursor++];
#ଯଦି 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	ତଳ = 0;
	ଉପର = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) icount->ଗଣନା_2-1;
	ଯେତେବେଳେ (ତଳ <= ଉପର) {
		mid = ((unsigned)ତଳ + (unsigned)ଉପର) >> 1;
		ଯଦି (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ଫେରନ୍ତୁ &icount->list[mid];
		}
		ଯଦି (ino < icount->list[mid].ino)
			ଉପର = mid-1;
		ଅନ୍ୟଥା
			ତଳ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ଯଦି (create)
		ଫେରନ୍ତୁ ଭର୍ତ୍ତି_icount_el(icount, ino, ତଳ);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t set_inode_ଗଣନା(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 ଗଣନା_2)
{
	ସଂରଚନା ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ଚାବି, data;

	ଯଦି (icount->tdb) {
		ଚାବି.dptr = (unsigned ଅକ୍ଷର *) &ino;
		ଚାବି.dsize = ଆକାର(ext2_ino_t);
		data.dptr = (unsigned ଅକ୍ଷର *) &ଗଣନା_2;
		data.dsize = ଆକାର(__u32);
		ଯଦି (ଗଣନା_2) {
			ଯଦି (tdb_store(icount->tdb, ଚାବି, data, TDB_REPLACE))
				ଫେରନ୍ତୁ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ଅନ୍ୟଥା {
			ଯଦି (tdb_delete(icount->tdb, ଚାବି))
				ଫେରନ୍ତୁ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ଫେରନ୍ତୁ 0;
	}
#endif
	ଯଦି (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(ଗଣନା_2);
		ଫେରନ୍ତୁ 0;
	}

	el = get_icount_el(icount, ino, 1);
	ଯଦି (!el)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;

	el->ଗଣନା_2 = ଗଣନା_2;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t get_inode_ଗଣନା(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *ଗଣନା_2)
{
	ସଂରଚନା ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ଚାବି, data;

	ଯଦି (icount->tdb) {
		ଚାବି.dptr = (unsigned ଅକ୍ଷର *) &ino;
		ଚାବି.dsize = ଆକାର(ext2_ino_t);

		data = tdb_fetch(icount->tdb, ଚାବି);
		ଯଦି (data.dptr == ଖାଲି) {
			*ଗଣନା_2 = 0;
			ଫେରନ୍ତୁ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*ଗଣନା_2 = *((__u32 *) data.dptr);
		ମୁକ୍ତ(data.dptr);
		ଫେରନ୍ତୁ 0;
	}
#endif
	ଯଦି (icount->fullmap) {
		*ଗଣନା_2 = icount->fullmap[ino];
		ଫେରନ୍ତୁ 0;
	}

	el = get_icount_el(icount, ino, 0);
	ଯଦି (!el) {
		*ଗଣନା_2 = 0;
		ଫେରନ୍ତୁ ENOENT;
	}

	*ଗଣନା_2 = el->ଗଣନା_2;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;
	ଅଚଳ ଅକ୍ଷର *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ଯଦି (icount->ଗଣନା_2 > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	}
	ପାଇଁ (i=1; i < icount->ଗଣନା_2; i++) {
		ଯଦି (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ଯଦି (!ino || (ino > icount->ଅଙ୍କ_inodes))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (!icount->fullmap) {
		ଯଦି (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			ଫେରନ୍ତୁ 0;
		}
	}
	get_inode_ଗଣନା(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ମୂଲ୍ୟ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ଯଦି (!ino || (ino > icount->ଅଙ୍କ_inodes))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (icount->fullmap) {
		curr_ମୂଲ୍ୟ = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_ମୂଲ୍ୟ;
	} ଅନ୍ୟଥା ଯଦି (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ଯଦି (set_inode_ଗଣନା(icount, ino, 2))
			ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
		curr_ମୂଲ୍ୟ = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ଅନ୍ୟଥା ଯଦି (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ଯଦି (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_ଗଣନା(icount, ino, &curr_ମୂଲ୍ୟ);
			curr_ମୂଲ୍ୟ++;
			ଯଦି (set_inode_ଗଣନା(icount, ino, curr_ମୂଲ୍ୟ))
				ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
		} ଅନ୍ୟଥା {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ଯଦି (ret)
				*ret = 1;
			ଫେରନ୍ତୁ 0;
		}
	} ଅନ୍ୟଥା {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_ଗଣନା(icount, ino, &curr_ମୂଲ୍ୟ);
		curr_ମୂଲ୍ୟ++;
		ଯଦି (set_inode_ଗଣନା(icount, ino, curr_ମୂଲ୍ୟ))
			ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	}
	ଯଦି (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ଯଦି (ret)
		*ret = icount_16_xlate(curr_ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ମୂଲ୍ୟ;

	ଯଦି (!ino || (ino > icount->ଅଙ୍କ_inodes))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ଯଦି (icount->fullmap) {
		ଯଦି (!icount->fullmap[ino])
			ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

		curr_ମୂଲ୍ୟ = --icount->fullmap[ino];
		ଯଦି (ret)
			*ret = icount_16_xlate(curr_ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ଯଦି (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ଅନ୍ୟଥା {
			set_inode_ଗଣନା(icount, ino, 0);
		}
		ଯଦି (ret)
			*ret = 0;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	get_inode_ଗଣନା(icount, ino, &curr_ମୂଲ୍ୟ);
	ଯଦି (!curr_ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	curr_ମୂଲ୍ୟ--;
	ଯଦି (set_inode_ଗଣନା(icount, ino, curr_ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;

	ଯଦି (curr_ମୂଲ୍ୟ == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ଯଦି ((curr_ମୂଲ୍ୟ == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	ଯଦି (ret)
		*ret = icount_16_xlate(curr_ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 ଗଣନା_2)
{
	ଯଦି (!ino || (ino > icount->ଅଙ୍କ_inodes))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ଯଦି (icount->fullmap)
		ଫେରନ୍ତୁ set_inode_ଗଣନା(icount, ino, ଗଣନା_2);

	ଯଦି (ଗଣନା_2 == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ଯଦି (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (ଗଣନା_2 == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ଯଦି (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} ଅନ୍ୟଥା
			set_inode_ଗଣନା(icount, ino, 0);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (set_inode_ଗଣନା(icount, ino, ଗଣନା_2))
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ଯଦି (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ଫେରନ୍ତୁ 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	ଯଦି (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

ସଂରଚନା test_program {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ସଂରଚନା test_program prog[] = {
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

ସଂରଚନା test_program extended[] = {
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
ସ୍ଥିର ଶୂନ୍ୟ setup(ଶୂନ୍ୟ)
{
	errcode_t	retval;
	ସଂରଚନା ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, ଆକାର(param));
	ext2fs_blocks_ଗଣନା_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_test(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅକ୍ଷର *dir, ସଂରଚନା test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ସଂରଚନା test_program *pc;
	__u16		result;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		problem = 0;

	ଯଦି (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ଯଦି (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#ଅନ୍ୟଥା
		printf("Skipped\n");
		ଫେରନ୍ତୁ 0;
#endif
	} ଅନ୍ୟଥା {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		ଯଦି (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	ପାଇଁ (pc = prog; pc->cmd != EXIT; pc++) {
		ବାଛ (pc->cmd) {
		ସ୍ଥିତି FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ଯଦି (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ଯଦି (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ଯଦି (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			ଭାଙ୍ଗ;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		ଯଦି (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		ଯଦି (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ଯଦି (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	ଫେରନ୍ତୁ problem;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା failed = 0;

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
	ଯଦି (failed)
		printf("FAILED!\n");
	ଫେରନ୍ତୁ failed;
}
#endif
