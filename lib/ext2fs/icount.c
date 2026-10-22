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

#അന്തര്ഭാവ "config.h"
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "tdb.h"

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

ഘടന ext2_icount_el {
	ext2_ino_t	ino;
	__u32		എണ്ണം;
};

ഘടന ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		എണ്ണം;
	ext2_ino_t		size;
	ext2_ino_t		അക്കം_inodes;
	ext2_ino_t		cursor;
	ഘടന ext2_icount_el	*list;
	ഘടന ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	അക്ഷരം			*tdb_fn;
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

ശൂന്യം ext2fs_free_icount(ext2_icount_t icount)
{
	എങ്കിൽ (!icount)
		തിരികെ;

	icount->magic = 0;
	എങ്കിൽ (icount->list)
		ext2fs_free_mem(&icount->list);
	എങ്കിൽ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	എങ്കിൽ (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	എങ്കിൽ (icount->tdb)
		tdb_close(icount->tdb);
	എങ്കിൽ (icount->tdb_fn) {
		(ശൂന്യം) unlink(icount->tdb_fn);
		സ്വതന്ത്രം(icount->tdb_fn);
	}
#endif

	എങ്കിൽ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

സ്ഥിതം errcode_t alloc_icount(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_icount), &icount);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(icount, 0, വലുപ്പം(ഘടന ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->അക്കം_inodes = fs->super->s_inodes_എണ്ണം;

	എങ്കിൽ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = വലുപ്പം(*icount->fullmap) * icount->അക്കം_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		എങ്കിൽ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			തിരികെ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	എങ്കിൽ (retval)
		പോകുക errout;

	എങ്കിൽ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		എങ്കിൽ (retval)
			പോകുക errout;
	} അല്ലെങ്കിൽ
		icount->multiple = 0;

	*ret = icount;
	തിരികെ 0;

errout:
	ext2fs_free_icount(icount);
	തിരികെ(retval);
}

#ifdef CONFIG_TDB
ഘടന uuid {
	__u32	time_താഴെയുള്ള;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	കണ്ണി[6];
};

സ്ഥിതം ശൂന്യം unpack_uuid(ശൂന്യം *in, ഘടന uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_താഴെയുള്ള = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->കണ്ണി, ptr, 6);
}

സ്ഥിതം ശൂന്യം uuid_unparse(ശൂന്യം *uu, അക്ഷരം *out)
{
	ഘടന uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_താഴെയുള്ള, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.കണ്ണി[0], uuid.കണ്ണി[1], uuid.കണ്ണി[2],
		uuid.കണ്ണി[3], uuid.കണ്ണി[4], uuid.കണ്ണി[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   അക്ഷരം *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   പൂർണ്ണസംഖ്യ flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	അക്ഷരം 		*fn, uuid[40];
	ext2_ino_t	അക്കം_inodes;
	mode_t		save_umask;
	പൂർണ്ണസംഖ്യ		fd;

	retval = alloc_icount(fs, flags,  &icount);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	എങ്കിൽ (retval)
		പോകുക errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	എങ്കിൽ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		പോകുക errout;
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
	അക്കം_inodes = fs->super->s_inodes_എണ്ണം - fs->super->s_free_inodes_എണ്ണം;

	icount->tdb = tdb_open(fn, അക്കം_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	എങ്കിൽ (icount->tdb == ശൂന്യ) {
		retval = errno;
		പോകുക errout;
	}
	*ret = icount;
	തിരികെ 0;
errout:
	ext2fs_free_icount(icount);
	തിരികെ(retval);
#അല്ലെങ്കിൽ
	തിരികെ EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, unsigned പൂർണ്ണസംഖ്യ size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	എങ്കിൽ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		എങ്കിൽ (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (icount->fullmap)
		പോകുക successout;

	എങ്കിൽ (size) {
		icount->size = size;
	} അല്ലെങ്കിൽ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_അക്കം_dirs(fs, &icount->size);
		എങ്കിൽ (retval)
			പോകുക errout;
		icount->size += fs->super->s_inodes_എണ്ണം / 50;
	}

	bytes = (size_t) (icount->size * വലുപ്പം(ഘടന ext2_icount_el));
#എങ്കിൽ 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, വലുപ്പം(ഘടന ext2_icount_el),
			 &icount->list);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(icount->list, 0, bytes);

	icount->എണ്ണം = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	എങ്കിൽ (hint) {
		വേണ്ടി (i=0; i < hint->എണ്ണം; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->എണ്ണം = hint->എണ്ണം;
	}

successout:
	*ret = icount;
	തിരികെ 0;

errout:
	ext2fs_free_icount(icount);
	തിരികെ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
			       unsigned പൂർണ്ണസംഖ്യ size,
			       ext2_icount_t *ret)
{
	തിരികെ ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
സ്ഥിതം ഘടന ext2_icount_el *തിരുകുക_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, പൂർണ്ണസംഖ്യ pos)
{
	ഘടന ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	പൂർണ്ണസംഖ്യ			അക്കം;

	എങ്കിൽ (icount->last_lookup && icount->last_lookup->ino == ino)
		തിരികെ icount->last_lookup;

	എങ്കിൽ (icount->എണ്ണം >= icount->size) {
		എങ്കിൽ (icount->എണ്ണം) {
			new_size = icount->list[(unsigned)icount->എണ്ണം-1].ino;
			new_size = (ext2_ino_t) (icount->എണ്ണം *
				((ദശാംശം) icount->അക്കം_inodes / new_size));
		}
		എങ്കിൽ (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#എങ്കിൽ 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   വലുപ്പം(ഘടന ext2_icount_el),
					   (size_t) new_size *
					   വലുപ്പം(ഘടന ext2_icount_el),
					   &icount->list);
		എങ്കിൽ (retval)
			തിരികെ 0;
		icount->size = new_size;
	}
	അക്കം = (പൂർണ്ണസംഖ്യ) icount->എണ്ണം - pos;
	എങ്കിൽ (അക്കം < 0)
		തിരികെ 0;	/* should never happen */
	എങ്കിൽ (അക്കം) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			വലുപ്പം(ഘടന ext2_icount_el) * അക്കം);
	}
	icount->എണ്ണം++;
	el = &icount->list[pos];
	el->എണ്ണം = 0;
	el->ino = ino;
	icount->last_lookup = el;
	തിരികെ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
സ്ഥിതം ഘടന ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, പൂർണ്ണസംഖ്യ create)
{
	പൂർണ്ണസംഖ്യ	താഴെയുള്ള, മുകളിലെ, mid;

	എങ്കിൽ (!icount || !icount->list)
		തിരികെ 0;

	എങ്കിൽ (create && ((icount->എണ്ണം == 0) ||
		       (ino > icount->list[(unsigned)icount->എണ്ണം-1].ino))) {
		തിരികെ തിരുകുക_icount_el(icount, ino, (unsigned) icount->എണ്ണം);
	}
	എങ്കിൽ (icount->എണ്ണം == 0)
		തിരികെ 0;

	എങ്കിൽ (icount->cursor >= icount->എണ്ണം)
		icount->cursor = 0;
	എങ്കിൽ (ino == icount->list[icount->cursor].ino)
		തിരികെ &icount->list[icount->cursor++];
#എങ്കിൽ 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	താഴെയുള്ള = 0;
	മുകളിലെ = (പൂർണ്ണസംഖ്യ) icount->എണ്ണം-1;
	ആകുമ്പോൾ (താഴെയുള്ള <= മുകളിലെ) {
		mid = ((unsigned)താഴെയുള്ള + (unsigned)മുകളിലെ) >> 1;
		എങ്കിൽ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			തിരികെ &icount->list[mid];
		}
		എങ്കിൽ (ino < icount->list[mid].ino)
			മുകളിലെ = mid-1;
		അല്ലെങ്കിൽ
			താഴെയുള്ള = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	എങ്കിൽ (create)
		തിരികെ തിരുകുക_icount_el(icount, ino, താഴെയുള്ള);
	തിരികെ 0;
}

സ്ഥിതം errcode_t set_inode_എണ്ണം(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 എണ്ണം)
{
	ഘടന ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA താക്കോൽ, data;

	എങ്കിൽ (icount->tdb) {
		താക്കോൽ.dptr = (unsigned അക്ഷരം *) &ino;
		താക്കോൽ.dsize = വലുപ്പം(ext2_ino_t);
		data.dptr = (unsigned അക്ഷരം *) &എണ്ണം;
		data.dsize = വലുപ്പം(__u32);
		എങ്കിൽ (എണ്ണം) {
			എങ്കിൽ (tdb_store(icount->tdb, താക്കോൽ, data, TDB_REPLACE))
				തിരികെ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (tdb_delete(icount->tdb, താക്കോൽ))
				തിരികെ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		തിരികെ 0;
	}
#endif
	എങ്കിൽ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(എണ്ണം);
		തിരികെ 0;
	}

	el = get_icount_el(icount, ino, 1);
	എങ്കിൽ (!el)
		തിരികെ EXT2_ET_NO_MEMORY;

	el->എണ്ണം = എണ്ണം;
	തിരികെ 0;
}

സ്ഥിതം errcode_t get_inode_എണ്ണം(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *എണ്ണം)
{
	ഘടന ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA താക്കോൽ, data;

	എങ്കിൽ (icount->tdb) {
		താക്കോൽ.dptr = (unsigned അക്ഷരം *) &ino;
		താക്കോൽ.dsize = വലുപ്പം(ext2_ino_t);

		data = tdb_fetch(icount->tdb, താക്കോൽ);
		എങ്കിൽ (data.dptr == ശൂന്യ) {
			*എണ്ണം = 0;
			തിരികെ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*എണ്ണം = *((__u32 *) data.dptr);
		സ്വതന്ത്രം(data.dptr);
		തിരികെ 0;
	}
#endif
	എങ്കിൽ (icount->fullmap) {
		*എണ്ണം = icount->fullmap[ino];
		തിരികെ 0;
	}

	el = get_icount_el(icount, ino, 0);
	എങ്കിൽ (!el) {
		*എണ്ണം = 0;
		തിരികെ ENOENT;
	}

	*എണ്ണം = el->എണ്ണം;
	തിരികെ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned പൂർണ്ണസംഖ്യ	i;
	സ്ഥിരം അക്ഷരം *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	എങ്കിൽ (icount->എണ്ണം > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	}
	വേണ്ടി (i=1; i < icount->എണ്ണം; i++) {
		എങ്കിൽ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	തിരികെ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	എങ്കിൽ (!ino || (ino > icount->അക്കം_inodes))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (!icount->fullmap) {
		എങ്കിൽ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			തിരികെ 0;
		}
		എങ്കിൽ (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			തിരികെ 0;
		}
	}
	get_inode_എണ്ണം(icount, ino, &val);
	*ret = icount_16_xlate(val);
	തിരികെ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_മൂല്യം;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	എങ്കിൽ (!ino || (ino > icount->അക്കം_inodes))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (icount->fullmap) {
		curr_മൂല്യം = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_മൂല്യം;
	} അല്ലെങ്കിൽ എങ്കിൽ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		എങ്കിൽ (set_inode_എണ്ണം(icount, ino, 2))
			തിരികെ EXT2_ET_NO_MEMORY;
		curr_മൂല്യം = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} അല്ലെങ്കിൽ എങ്കിൽ (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		എങ്കിൽ (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_എണ്ണം(icount, ino, &curr_മൂല്യം);
			curr_മൂല്യം++;
			എങ്കിൽ (set_inode_എണ്ണം(icount, ino, curr_മൂല്യം))
				തിരികെ EXT2_ET_NO_MEMORY;
		} അല്ലെങ്കിൽ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			എങ്കിൽ (ret)
				*ret = 1;
			തിരികെ 0;
		}
	} അല്ലെങ്കിൽ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_എണ്ണം(icount, ino, &curr_മൂല്യം);
		curr_മൂല്യം++;
		എങ്കിൽ (set_inode_എണ്ണം(icount, ino, curr_മൂല്യം))
			തിരികെ EXT2_ET_NO_MEMORY;
	}
	എങ്കിൽ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	എങ്കിൽ (ret)
		*ret = icount_16_xlate(curr_മൂല്യം);
	തിരികെ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_മൂല്യം;

	എങ്കിൽ (!ino || (ino > icount->അക്കം_inodes))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	എങ്കിൽ (icount->fullmap) {
		എങ്കിൽ (!icount->fullmap[ino])
			തിരികെ EXT2_ET_INVALID_ARGUMENT;

		curr_മൂല്യം = --icount->fullmap[ino];
		എങ്കിൽ (ret)
			*ret = icount_16_xlate(curr_മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		എങ്കിൽ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		അല്ലെങ്കിൽ {
			set_inode_എണ്ണം(icount, ino, 0);
		}
		എങ്കിൽ (ret)
			*ret = 0;
		തിരികെ 0;
	}

	എങ്കിൽ (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	get_inode_എണ്ണം(icount, ino, &curr_മൂല്യം);
	എങ്കിൽ (!curr_മൂല്യം)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	curr_മൂല്യം--;
	എങ്കിൽ (set_inode_എണ്ണം(icount, ino, curr_മൂല്യം))
		തിരികെ EXT2_ET_NO_MEMORY;

	എങ്കിൽ (curr_മൂല്യം == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	എങ്കിൽ ((curr_മൂല്യം == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	എങ്കിൽ (ret)
		*ret = icount_16_xlate(curr_മൂല്യം);
	തിരികെ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 എണ്ണം)
{
	എങ്കിൽ (!ino || (ino > icount->അക്കം_inodes))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	എങ്കിൽ (icount->fullmap)
		തിരികെ set_inode_എണ്ണം(icount, ino, എണ്ണം);

	എങ്കിൽ (എണ്ണം == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		എങ്കിൽ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		തിരികെ 0;
	}
	എങ്കിൽ (എണ്ണം == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		എങ്കിൽ (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} അല്ലെങ്കിൽ
			set_inode_എണ്ണം(icount, ino, 0);
		തിരികെ 0;
	}

	എങ്കിൽ (set_inode_എണ്ണം(icount, ino, എണ്ണം))
		തിരികെ EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	എങ്കിൽ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	തിരികെ 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	എങ്കിൽ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		തിരികെ 0;

	തിരികെ icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

ഘടന test_program {
	പൂർണ്ണസംഖ്യ		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ഘടന test_program prog[] = {
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

ഘടന test_program extended[] = {
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
സ്ഥിതം ശൂന്യം setup(ശൂന്യം)
{
	errcode_t	retval;
	ഘടന ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, വലുപ്പം(param));
	ext2fs_blocks_എണ്ണം_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

പൂർണ്ണസംഖ്യ run_test(പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ size, അക്ഷരം *dir, ഘടന test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ഘടന test_program *pc;
	__u16		result;
	പൂർണ്ണസംഖ്യ		problem = 0;

	എങ്കിൽ (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		എങ്കിൽ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#അല്ലെങ്കിൽ
		printf("Skipped\n");
		തിരികെ 0;
#endif
	} അല്ലെങ്കിൽ {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		എങ്കിൽ (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	വേണ്ടി (pc = prog; pc->cmd != EXIT; pc++) {
		തിരഞ്ഞെടുപ്പ് (pc->cmd) {
		അവസ്ഥ FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			നിർത്തുക;
		അവസ്ഥ STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			എങ്കിൽ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			നിർത്തുക;
		അവസ്ഥ INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			എങ്കിൽ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			നിർത്തുക;
		അവസ്ഥ DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			എങ്കിൽ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			നിർത്തുക;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		എങ്കിൽ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		എങ്കിൽ (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	എങ്കിൽ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	തിരികെ problem;
}


പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ failed = 0;

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
	എങ്കിൽ (failed)
		printf("FAILED!\n");
	തിരികെ failed;
}
#endif
