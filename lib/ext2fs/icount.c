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

#अन्तर्भाव "config.h"
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "tdb.h"

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

संरचना ext2_icount_el {
	ext2_ino_t	ino;
	__u32		गणनम्;
};

संरचना ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		गणनम्;
	ext2_ino_t		size;
	ext2_ino_t		अङ्कः_inodes;
	ext2_ino_t		cursor;
	संरचना ext2_icount_el	*list;
	संरचना ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	वर्ण			*tdb_fn;
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

शून्य ext2fs_free_icount(ext2_icount_t icount)
{
	सचे (!icount)
		पटिनिवत्त;

	icount->magic = 0;
	सचे (icount->list)
		ext2fs_free_mem(&icount->list);
	सचे (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	सचे (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	सचे (icount->tdb)
		tdb_close(icount->tdb);
	सचे (icount->tdb_fn) {
		(शून्य) unlink(icount->tdb_fn);
		मुक्त(icount->tdb_fn);
	}
#endif

	सचे (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

स्थिर errcode_t alloc_icount(ext2_filsys fs, पूर्णाङ्क flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_icount), &icount);
	सचे (retval)
		पटिनिवत्त retval;
	memset(icount, 0, परिमाण(संरचना ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->अङ्कः_inodes = fs->super->s_inodes_गणनम्;

	सचे ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = परिमाण(*icount->fullmap) * icount->अङ्कः_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		सचे (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			पटिनिवत्त 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	सचे (retval)
		गच्छ errout;

	सचे (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		सचे (retval)
			गच्छ errout;
	} अञ्ञथा
		icount->multiple = 0;

	*ret = icount;
	पटिनिवत्त 0;

errout:
	ext2fs_free_icount(icount);
	पटिनिवत्त(retval);
}

#ifdef CONFIG_TDB
संरचना uuid {
	__u32	time_नीचः;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	ग्रन्थिः[6];
};

स्थिर शून्य unpack_uuid(शून्य *in, संरचना uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_नीचः = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->ग्रन्थिः, ptr, 6);
}

स्थिर शून्य uuid_unparse(शून्य *uu, वर्ण *out)
{
	संरचना uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_नीचः, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ग्रन्थिः[0], uuid.ग्रन्थिः[1], uuid.ग्रन्थिः[2],
		uuid.ग्रन्थिः[3], uuid.ग्रन्थिः[4], uuid.ग्रन्थिः[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   वर्ण *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   पूर्णाङ्क flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	वर्ण 		*fn, uuid[40];
	ext2_ino_t	अङ्कः_inodes;
	mode_t		save_umask;
	पूर्णाङ्क		fd;

	retval = alloc_icount(fs, flags,  &icount);
	सचे (retval)
		पटिनिवत्त retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	सचे (retval)
		गच्छ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	सचे (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		गच्छ errout;
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
	अङ्कः_inodes = fs->super->s_inodes_गणनम् - fs->super->s_free_inodes_गणनम्;

	icount->tdb = tdb_open(fn, अङ्कः_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	सचे (icount->tdb == रिक्त) {
		retval = errno;
		गच्छ errout;
	}
	*ret = icount;
	पटिनिवत्त 0;
errout:
	ext2fs_free_icount(icount);
	पटिनिवत्त(retval);
#अञ्ञथा
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णाङ्क flags, unsigned पूर्णाङ्क size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	सचे (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		सचे (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	सचे (retval)
		पटिनिवत्त retval;

	सचे (icount->fullmap)
		गच्छ successout;

	सचे (size) {
		icount->size = size;
	} अञ्ञथा {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_अङ्कः_dirs(fs, &icount->size);
		सचे (retval)
			गच्छ errout;
		icount->size += fs->super->s_inodes_गणनम् / 50;
	}

	bytes = (size_t) (icount->size * परिमाण(संरचना ext2_icount_el));
#सचे 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, परिमाण(संरचना ext2_icount_el),
			 &icount->list);
	सचे (retval)
		गच्छ errout;
	memset(icount->list, 0, bytes);

	icount->गणनम् = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	सचे (hint) {
		अत्थाय (i=0; i < hint->गणनम्; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->गणनम् = hint->गणनम्;
	}

successout:
	*ret = icount;
	पटिनिवत्त 0;

errout:
	ext2fs_free_icount(icount);
	पटिनिवत्त(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णाङ्क flags,
			       unsigned पूर्णाङ्क size,
			       ext2_icount_t *ret)
{
	पटिनिवत्त ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
स्थिर संरचना ext2_icount_el *निवेशय_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णाङ्क pos)
{
	संरचना ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	पूर्णाङ्क			अङ्कः;

	सचे (icount->last_lookup && icount->last_lookup->ino == ino)
		पटिनिवत्त icount->last_lookup;

	सचे (icount->गणनम् >= icount->size) {
		सचे (icount->गणनम्) {
			new_size = icount->list[(unsigned)icount->गणनम्-1].ino;
			new_size = (ext2_ino_t) (icount->गणनम् *
				((भिन्न) icount->अङ्कः_inodes / new_size));
		}
		सचे (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#सचे 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   परिमाण(संरचना ext2_icount_el),
					   (size_t) new_size *
					   परिमाण(संरचना ext2_icount_el),
					   &icount->list);
		सचे (retval)
			पटिनिवत्त 0;
		icount->size = new_size;
	}
	अङ्कः = (पूर्णाङ्क) icount->गणनम् - pos;
	सचे (अङ्कः < 0)
		पटिनिवत्त 0;	/* should never happen */
	सचे (अङ्कः) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			परिमाण(संरचना ext2_icount_el) * अङ्कः);
	}
	icount->गणनम्++;
	el = &icount->list[pos];
	el->गणनम् = 0;
	el->ino = ino;
	icount->last_lookup = el;
	पटिनिवत्त el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
स्थिर संरचना ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णाङ्क create)
{
	पूर्णाङ्क	नीचः, उच्चः, mid;

	सचे (!icount || !icount->list)
		पटिनिवत्त 0;

	सचे (create && ((icount->गणनम् == 0) ||
		       (ino > icount->list[(unsigned)icount->गणनम्-1].ino))) {
		पटिनिवत्त निवेशय_icount_el(icount, ino, (unsigned) icount->गणनम्);
	}
	सचे (icount->गणनम् == 0)
		पटिनिवत्त 0;

	सचे (icount->cursor >= icount->गणनम्)
		icount->cursor = 0;
	सचे (ino == icount->list[icount->cursor].ino)
		पटिनिवत्त &icount->list[icount->cursor++];
#सचे 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	नीचः = 0;
	उच्चः = (पूर्णाङ्क) icount->गणनम्-1;
	याव (नीचः <= उच्चः) {
		mid = ((unsigned)नीचः + (unsigned)उच्चः) >> 1;
		सचे (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			पटिनिवत्त &icount->list[mid];
		}
		सचे (ino < icount->list[mid].ino)
			उच्चः = mid-1;
		अञ्ञथा
			नीचः = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	सचे (create)
		पटिनिवत्त निवेशय_icount_el(icount, ino, नीचः);
	पटिनिवत्त 0;
}

स्थिर errcode_t set_inode_गणनम्(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 गणनम्)
{
	संरचना ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA कुञ्चिका, data;

	सचे (icount->tdb) {
		कुञ्चिका.dptr = (unsigned वर्ण *) &ino;
		कुञ्चिका.dsize = परिमाण(ext2_ino_t);
		data.dptr = (unsigned वर्ण *) &गणनम्;
		data.dsize = परिमाण(__u32);
		सचे (गणनम्) {
			सचे (tdb_store(icount->tdb, कुञ्चिका, data, TDB_REPLACE))
				पटिनिवत्त tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} अञ्ञथा {
			सचे (tdb_delete(icount->tdb, कुञ्चिका))
				पटिनिवत्त tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		पटिनिवत्त 0;
	}
#endif
	सचे (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(गणनम्);
		पटिनिवत्त 0;
	}

	el = get_icount_el(icount, ino, 1);
	सचे (!el)
		पटिनिवत्त EXT2_ET_NO_MEMORY;

	el->गणनम् = गणनम्;
	पटिनिवत्त 0;
}

स्थिर errcode_t get_inode_गणनम्(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *गणनम्)
{
	संरचना ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA कुञ्चिका, data;

	सचे (icount->tdb) {
		कुञ्चिका.dptr = (unsigned वर्ण *) &ino;
		कुञ्चिका.dsize = परिमाण(ext2_ino_t);

		data = tdb_fetch(icount->tdb, कुञ्चिका);
		सचे (data.dptr == रिक्त) {
			*गणनम् = 0;
			पटिनिवत्त tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*गणनम् = *((__u32 *) data.dptr);
		मुक्त(data.dptr);
		पटिनिवत्त 0;
	}
#endif
	सचे (icount->fullmap) {
		*गणनम् = icount->fullmap[ino];
		पटिनिवत्त 0;
	}

	el = get_icount_el(icount, ino, 0);
	सचे (!el) {
		*गणनम् = 0;
		पटिनिवत्त ENOENT;
	}

	*गणनम् = el->गणनम्;
	पटिनिवत्त 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned पूर्णाङ्क	i;
	ध्रुव वर्ण *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	सचे (icount->गणनम् > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	}
	अत्थाय (i=1; i < icount->गणनम्; i++) {
		सचे (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	पटिनिवत्त ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	सचे (!ino || (ino > icount->अङ्कः_inodes))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	सचे (!icount->fullmap) {
		सचे (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			पटिनिवत्त 0;
		}
		सचे (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			पटिनिवत्त 0;
		}
	}
	get_inode_गणनम्(icount, ino, &val);
	*ret = icount_16_xlate(val);
	पटिनिवत्त 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मानम्;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	सचे (!ino || (ino > icount->अङ्कः_inodes))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	सचे (icount->fullmap) {
		curr_मानम् = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_मानम्;
	} अञ्ञथा सचे (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		सचे (set_inode_गणनम्(icount, ino, 2))
			पटिनिवत्त EXT2_ET_NO_MEMORY;
		curr_मानम् = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} अञ्ञथा सचे (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		सचे (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_गणनम्(icount, ino, &curr_मानम्);
			curr_मानम्++;
			सचे (set_inode_गणनम्(icount, ino, curr_मानम्))
				पटिनिवत्त EXT2_ET_NO_MEMORY;
		} अञ्ञथा {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			सचे (ret)
				*ret = 1;
			पटिनिवत्त 0;
		}
	} अञ्ञथा {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_गणनम्(icount, ino, &curr_मानम्);
		curr_मानम्++;
		सचे (set_inode_गणनम्(icount, ino, curr_मानम्))
			पटिनिवत्त EXT2_ET_NO_MEMORY;
	}
	सचे (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	सचे (ret)
		*ret = icount_16_xlate(curr_मानम्);
	पटिनिवत्त 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मानम्;

	सचे (!ino || (ino > icount->अङ्कः_inodes))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	सचे (icount->fullmap) {
		सचे (!icount->fullmap[ino])
			पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

		curr_मानम् = --icount->fullmap[ino];
		सचे (ret)
			*ret = icount_16_xlate(curr_मानम्);
		पटिनिवत्त 0;
	}

	सचे (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		सचे (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		अञ्ञथा {
			set_inode_गणनम्(icount, ino, 0);
		}
		सचे (ret)
			*ret = 0;
		पटिनिवत्त 0;
	}

	सचे (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	get_inode_गणनम्(icount, ino, &curr_मानम्);
	सचे (!curr_मानम्)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	curr_मानम्--;
	सचे (set_inode_गणनम्(icount, ino, curr_मानम्))
		पटिनिवत्त EXT2_ET_NO_MEMORY;

	सचे (curr_मानम् == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	सचे ((curr_मानम् == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	सचे (ret)
		*ret = icount_16_xlate(curr_मानम्);
	पटिनिवत्त 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 गणनम्)
{
	सचे (!ino || (ino > icount->अङ्कः_inodes))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	सचे (icount->fullmap)
		पटिनिवत्त set_inode_गणनम्(icount, ino, गणनम्);

	सचे (गणनम् == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		सचे (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		पटिनिवत्त 0;
	}
	सचे (गणनम् == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		सचे (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} अञ्ञथा
			set_inode_गणनम्(icount, ino, 0);
		पटिनिवत्त 0;
	}

	सचे (set_inode_गणनम्(icount, ino, गणनम्))
		पटिनिवत्त EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	सचे (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	पटिनिवत्त 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	सचे (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		पटिनिवत्त 0;

	पटिनिवत्त icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

संरचना test_program {
	पूर्णाङ्क		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

संरचना test_program prog[] = {
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

संरचना test_program extended[] = {
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
स्थिर शून्य setup(शून्य)
{
	errcode_t	retval;
	संरचना ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, परिमाण(param));
	ext2fs_blocks_गणनम्_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	सचे (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	सचे (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

पूर्णाङ्क run_test(पूर्णाङ्क flags, पूर्णाङ्क size, वर्ण *dir, संरचना test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	संरचना test_program *pc;
	__u16		result;
	पूर्णाङ्क		problem = 0;

	सचे (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		सचे (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#अञ्ञथा
		printf("Skipped\n");
		पटिनिवत्त 0;
#endif
	} अञ्ञथा {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		सचे (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	अत्थाय (pc = prog; pc->cmd != EXIT; pc++) {
		विकल्प (pc->cmd) {
		पक्ष FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			भङ्ग;
		पक्ष STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			सचे (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			भङ्ग;
		पक्ष INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			सचे (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			भङ्ग;
		पक्ष DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			सचे (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			भङ्ग;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		सचे (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		सचे (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	सचे (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	पटिनिवत्त problem;
}


पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क failed = 0;

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
	सचे (failed)
		printf("FAILED!\n");
	पटिनिवत्त failed;
}
#endif
