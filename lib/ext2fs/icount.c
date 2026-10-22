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

#અન્તર્ભાવ "config.h"
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "tdb.h"

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

સંરચના ext2_icount_el {
	ext2_ino_t	ino;
	__u32		ગણતરી;
};

સંરચના ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		ગણતરી;
	ext2_ino_t		size;
	ext2_ino_t		અંક_inodes;
	ext2_ino_t		cursor;
	સંરચના ext2_icount_el	*list;
	સંરચના ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	અક્ષર			*tdb_fn;
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

શૂન્ય ext2fs_free_icount(ext2_icount_t icount)
{
	જો (!icount)
		પાછા;

	icount->magic = 0;
	જો (icount->list)
		ext2fs_free_mem(&icount->list);
	જો (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	જો (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	જો (icount->tdb)
		tdb_close(icount->tdb);
	જો (icount->tdb_fn) {
		(શૂન્ય) unlink(icount->tdb_fn);
		મુક્ત(icount->tdb_fn);
	}
#endif

	જો (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

સ્થિર errcode_t alloc_icount(ext2_filsys fs, પૂર્ણાંક flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_icount), &icount);
	જો (retval)
		પાછા retval;
	memset(icount, 0, કદ(સંરચના ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->અંક_inodes = fs->super->s_inodes_ગણતરી;

	જો ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = કદ(*icount->fullmap) * icount->અંક_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		જો (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			પાછા 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	જો (retval)
		જાઓ errout;

	જો (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		જો (retval)
			જાઓ errout;
	} નહીંતર
		icount->multiple = 0;

	*ret = icount;
	પાછા 0;

errout:
	ext2fs_free_icount(icount);
	પાછા(retval);
}

#ifdef CONFIG_TDB
સંરચના uuid {
	__u32	time_નીચલું;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	ગાંઠ[6];
};

સ્થિર શૂન્ય unpack_uuid(શૂન્ય *in, સંરચના uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_નીચલું = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->ગાંઠ, ptr, 6);
}

સ્થિર શૂન્ય uuid_unparse(શૂન્ય *uu, અક્ષર *out)
{
	સંરચના uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_નીચલું, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ગાંઠ[0], uuid.ગાંઠ[1], uuid.ગાંઠ[2],
		uuid.ગાંઠ[3], uuid.ગાંઠ[4], uuid.ગાંઠ[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   અક્ષર *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   પૂર્ણાંક flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	અક્ષર 		*fn, uuid[40];
	ext2_ino_t	અંક_inodes;
	mode_t		save_umask;
	પૂર્ણાંક		fd;

	retval = alloc_icount(fs, flags,  &icount);
	જો (retval)
		પાછા retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	જો (retval)
		જાઓ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	જો (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		જાઓ errout;
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
	અંક_inodes = fs->super->s_inodes_ગણતરી - fs->super->s_free_inodes_ગણતરી;

	icount->tdb = tdb_open(fn, અંક_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	જો (icount->tdb == ખાલી) {
		retval = errno;
		જાઓ errout;
	}
	*ret = icount;
	પાછા 0;
errout:
	ext2fs_free_icount(icount);
	પાછા(retval);
#નહીંતર
	પાછા EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, પૂર્ણાંક flags, unsigned પૂર્ણાંક size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	જો (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		જો (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	જો (retval)
		પાછા retval;

	જો (icount->fullmap)
		જાઓ successout;

	જો (size) {
		icount->size = size;
	} નહીંતર {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_અંક_dirs(fs, &icount->size);
		જો (retval)
			જાઓ errout;
		icount->size += fs->super->s_inodes_ગણતરી / 50;
	}

	bytes = (size_t) (icount->size * કદ(સંરચના ext2_icount_el));
#જો 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, કદ(સંરચના ext2_icount_el),
			 &icount->list);
	જો (retval)
		જાઓ errout;
	memset(icount->list, 0, bytes);

	icount->ગણતરી = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	જો (hint) {
		માટે (i=0; i < hint->ગણતરી; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->ગણતરી = hint->ગણતરી;
	}

successout:
	*ret = icount;
	પાછા 0;

errout:
	ext2fs_free_icount(icount);
	પાછા(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, પૂર્ણાંક flags,
			       unsigned પૂર્ણાંક size,
			       ext2_icount_t *ret)
{
	પાછા ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
સ્થિર સંરચના ext2_icount_el *ઉમેરો_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, પૂર્ણાંક pos)
{
	સંરચના ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	પૂર્ણાંક			અંક;

	જો (icount->last_lookup && icount->last_lookup->ino == ino)
		પાછા icount->last_lookup;

	જો (icount->ગણતરી >= icount->size) {
		જો (icount->ગણતરી) {
			new_size = icount->list[(unsigned)icount->ગણતરી-1].ino;
			new_size = (ext2_ino_t) (icount->ગણતરી *
				((દશાંશ) icount->અંક_inodes / new_size));
		}
		જો (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#જો 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   કદ(સંરચના ext2_icount_el),
					   (size_t) new_size *
					   કદ(સંરચના ext2_icount_el),
					   &icount->list);
		જો (retval)
			પાછા 0;
		icount->size = new_size;
	}
	અંક = (પૂર્ણાંક) icount->ગણતરી - pos;
	જો (અંક < 0)
		પાછા 0;	/* should never happen */
	જો (અંક) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			કદ(સંરચના ext2_icount_el) * અંક);
	}
	icount->ગણતરી++;
	el = &icount->list[pos];
	el->ગણતરી = 0;
	el->ino = ino;
	icount->last_lookup = el;
	પાછા el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
સ્થિર સંરચના ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, પૂર્ણાંક create)
{
	પૂર્ણાંક	નીચલું, ઉપલું, mid;

	જો (!icount || !icount->list)
		પાછા 0;

	જો (create && ((icount->ગણતરી == 0) ||
		       (ino > icount->list[(unsigned)icount->ગણતરી-1].ino))) {
		પાછા ઉમેરો_icount_el(icount, ino, (unsigned) icount->ગણતરી);
	}
	જો (icount->ગણતરી == 0)
		પાછા 0;

	જો (icount->cursor >= icount->ગણતરી)
		icount->cursor = 0;
	જો (ino == icount->list[icount->cursor].ino)
		પાછા &icount->list[icount->cursor++];
#જો 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	નીચલું = 0;
	ઉપલું = (પૂર્ણાંક) icount->ગણતરી-1;
	જ્યારે (નીચલું <= ઉપલું) {
		mid = ((unsigned)નીચલું + (unsigned)ઉપલું) >> 1;
		જો (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			પાછા &icount->list[mid];
		}
		જો (ino < icount->list[mid].ino)
			ઉપલું = mid-1;
		નહીંતર
			નીચલું = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	જો (create)
		પાછા ઉમેરો_icount_el(icount, ino, નીચલું);
	પાછા 0;
}

સ્થિર errcode_t set_inode_ગણતરી(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 ગણતરી)
{
	સંરચના ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ચાવી, data;

	જો (icount->tdb) {
		ચાવી.dptr = (unsigned અક્ષર *) &ino;
		ચાવી.dsize = કદ(ext2_ino_t);
		data.dptr = (unsigned અક્ષર *) &ગણતરી;
		data.dsize = કદ(__u32);
		જો (ગણતરી) {
			જો (tdb_store(icount->tdb, ચાવી, data, TDB_REPLACE))
				પાછા tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} નહીંતર {
			જો (tdb_delete(icount->tdb, ચાવી))
				પાછા tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		પાછા 0;
	}
#endif
	જો (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(ગણતરી);
		પાછા 0;
	}

	el = get_icount_el(icount, ino, 1);
	જો (!el)
		પાછા EXT2_ET_NO_MEMORY;

	el->ગણતરી = ગણતરી;
	પાછા 0;
}

સ્થિર errcode_t get_inode_ગણતરી(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *ગણતરી)
{
	સંરચના ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA ચાવી, data;

	જો (icount->tdb) {
		ચાવી.dptr = (unsigned અક્ષર *) &ino;
		ચાવી.dsize = કદ(ext2_ino_t);

		data = tdb_fetch(icount->tdb, ચાવી);
		જો (data.dptr == ખાલી) {
			*ગણતરી = 0;
			પાછા tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*ગણતરી = *((__u32 *) data.dptr);
		મુક્ત(data.dptr);
		પાછા 0;
	}
#endif
	જો (icount->fullmap) {
		*ગણતરી = icount->fullmap[ino];
		પાછા 0;
	}

	el = get_icount_el(icount, ino, 0);
	જો (!el) {
		*ગણતરી = 0;
		પાછા ENOENT;
	}

	*ગણતરી = el->ગણતરી;
	પાછા 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned પૂર્ણાંક	i;
	અચલ અક્ષર *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	જો (icount->ગણતરી > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		પાછા EXT2_ET_INVALID_ARGUMENT;
	}
	માટે (i=1; i < icount->ગણતરી; i++) {
		જો (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	પાછા ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	જો (!ino || (ino > icount->અંક_inodes))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (!icount->fullmap) {
		જો (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			પાછા 0;
		}
		જો (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			પાછા 0;
		}
	}
	get_inode_ગણતરી(icount, ino, &val);
	*ret = icount_16_xlate(val);
	પાછા 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_કિંમત;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	જો (!ino || (ino > icount->અંક_inodes))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (icount->fullmap) {
		curr_કિંમત = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_કિંમત;
	} નહીંતર જો (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		જો (set_inode_ગણતરી(icount, ino, 2))
			પાછા EXT2_ET_NO_MEMORY;
		curr_કિંમત = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} નહીંતર જો (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		જો (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_ગણતરી(icount, ino, &curr_કિંમત);
			curr_કિંમત++;
			જો (set_inode_ગણતરી(icount, ino, curr_કિંમત))
				પાછા EXT2_ET_NO_MEMORY;
		} નહીંતર {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			જો (ret)
				*ret = 1;
			પાછા 0;
		}
	} નહીંતર {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_ગણતરી(icount, ino, &curr_કિંમત);
		curr_કિંમત++;
		જો (set_inode_ગણતરી(icount, ino, curr_કિંમત))
			પાછા EXT2_ET_NO_MEMORY;
	}
	જો (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	જો (ret)
		*ret = icount_16_xlate(curr_કિંમત);
	પાછા 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_કિંમત;

	જો (!ino || (ino > icount->અંક_inodes))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	જો (icount->fullmap) {
		જો (!icount->fullmap[ino])
			પાછા EXT2_ET_INVALID_ARGUMENT;

		curr_કિંમત = --icount->fullmap[ino];
		જો (ret)
			*ret = icount_16_xlate(curr_કિંમત);
		પાછા 0;
	}

	જો (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		જો (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		નહીંતર {
			set_inode_ગણતરી(icount, ino, 0);
		}
		જો (ret)
			*ret = 0;
		પાછા 0;
	}

	જો (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	get_inode_ગણતરી(icount, ino, &curr_કિંમત);
	જો (!curr_કિંમત)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	curr_કિંમત--;
	જો (set_inode_ગણતરી(icount, ino, curr_કિંમત))
		પાછા EXT2_ET_NO_MEMORY;

	જો (curr_કિંમત == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	જો ((curr_કિંમત == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	જો (ret)
		*ret = icount_16_xlate(curr_કિંમત);
	પાછા 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 ગણતરી)
{
	જો (!ino || (ino > icount->અંક_inodes))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	જો (icount->fullmap)
		પાછા set_inode_ગણતરી(icount, ino, ગણતરી);

	જો (ગણતરી == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		જો (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		પાછા 0;
	}
	જો (ગણતરી == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		જો (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} નહીંતર
			set_inode_ગણતરી(icount, ino, 0);
		પાછા 0;
	}

	જો (set_inode_ગણતરી(icount, ino, ગણતરી))
		પાછા EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	જો (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	પાછા 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	જો (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		પાછા 0;

	પાછા icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

સંરચના test_program {
	પૂર્ણાંક		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

સંરચના test_program prog[] = {
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

સંરચના test_program extended[] = {
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
સ્થિર શૂન્ય setup(શૂન્ય)
{
	errcode_t	retval;
	સંરચના ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, કદ(param));
	ext2fs_blocks_ગણતરી_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	જો (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	જો (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

પૂર્ણાંક run_test(પૂર્ણાંક flags, પૂર્ણાંક size, અક્ષર *dir, સંરચના test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	સંરચના test_program *pc;
	__u16		result;
	પૂર્ણાંક		problem = 0;

	જો (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		જો (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#નહીંતર
		printf("Skipped\n");
		પાછા 0;
#endif
	} નહીંતર {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		જો (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	માટે (pc = prog; pc->cmd != EXIT; pc++) {
		પસંદગી (pc->cmd) {
		સ્થિતિ FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			તોડો;
		સ્થિતિ STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			જો (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			તોડો;
		સ્થિતિ INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			જો (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			તોડો;
		સ્થિતિ DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			જો (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			તોડો;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		જો (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		જો (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	જો (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	પાછા problem;
}


પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	પૂર્ણાંક failed = 0;

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
	જો (failed)
		printf("FAILED!\n");
	પાછા failed;
}
#endif
