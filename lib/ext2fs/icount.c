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
#जर HAVE_UNISTD_H
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
	__u32		मोजणी;
};

संरचना ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		मोजणी;
	ext2_ino_t		size;
	ext2_ino_t		अंक_inodes;
	ext2_ino_t		cursor;
	संरचना ext2_icount_el	*list;
	संरचना ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	अक्षर			*tdb_fn;
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
	जर (!icount)
		परत;

	icount->magic = 0;
	जर (icount->list)
		ext2fs_free_mem(&icount->list);
	जर (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	जर (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	जर (icount->tdb)
		tdb_close(icount->tdb);
	जर (icount->tdb_fn) {
		(शून्य) unlink(icount->tdb_fn);
		मुक्त(icount->tdb_fn);
	}
#endif

	जर (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

स्थिर errcode_t alloc_icount(ext2_filsys fs, पूर्णांक flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(आकार(संरचना ext2_icount), &icount);
	जर (retval)
		परत retval;
	स_रखो(icount, 0, आकार(संरचना ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->अंक_inodes = fs->super->s_inodes_मोजणी;

	जर ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		अचिन्हित sz = आकार(*icount->fullmap) * icount->अंक_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		जर (!retval) {
			स_रखो(icount->fullmap, 0, sz);
			*ret = icount;
			परत 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	जर (retval)
		जा errout;

	जर (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		जर (retval)
			जा errout;
	} अन्यथा
		icount->multiple = 0;

	*ret = icount;
	परत 0;

errout:
	ext2fs_free_icount(icount);
	परत(retval);
}

#ifdef CONFIG_TDB
संरचना uuid {
	__u32	time_खालचा;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	गाठ[6];
};

स्थिर शून्य unpack_uuid(शून्य *in, संरचना uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_खालचा = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	स_नकल(uu->गाठ, ptr, 6);
}

स्थिर शून्य uuid_unparse(शून्य *uu, अक्षर *out)
{
	संरचना uuid uuid;

	unpack_uuid(uu, &uuid);
	प्र_लिखो(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_खालचा, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.गाठ[0], uuid.गाठ[1], uuid.गाठ[2],
		uuid.गाठ[3], uuid.गाठ[4], uuid.गाठ[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   अक्षर *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   पूर्णांक flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	अक्षर 		*fn, uuid[40];
	ext2_ino_t	अंक_inodes;
	mode_t		save_umask;
	पूर्णांक		fd;

	retval = alloc_icount(fs, flags,  &icount);
	जर (retval)
		परत retval;

	retval = ext2fs_get_mem(म_माप(tdb_dir) + 64, &fn);
	जर (retval)
		जा errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	प्र_लिखो(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	जर (fd < 0) {
		retval = त्रुटि_सं;
		ext2fs_free_mem(&fn);
		जा errout;
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
	अंक_inodes = fs->super->s_inodes_मोजणी - fs->super->s_free_inodes_मोजणी;

	icount->tdb = tdb_open(fn, अंक_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	बंद(fd);
	जर (icount->tdb == रिक्त) {
		retval = त्रुटि_सं;
		जा errout;
	}
	*ret = icount;
	परत 0;
errout:
	ext2fs_free_icount(icount);
	परत(retval);
#अन्यथा
	परत EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णांक flags, अचिन्हित पूर्णांक size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	माप_प्रकार		bytes;
	ext2_ino_t	i;

	जर (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		जर (hint->size > size)
			size = (माप_प्रकार) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	जर (retval)
		परत retval;

	जर (icount->fullmap)
		जा successout;

	जर (size) {
		icount->size = size;
	} अन्यथा {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_अंक_dirs(fs, &icount->size);
		जर (retval)
			जा errout;
		icount->size += fs->super->s_inodes_मोजणी / 50;
	}

	bytes = (माप_प्रकार) (icount->size * आकार(संरचना ext2_icount_el));
#जर 0
	म_लिखो("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, आकार(संरचना ext2_icount_el),
			 &icount->list);
	जर (retval)
		जा errout;
	स_रखो(icount->list, 0, bytes);

	icount->मोजणी = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	जर (hint) {
		साठी (i=0; i < hint->मोजणी; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->मोजणी = hint->मोजणी;
	}

successout:
	*ret = icount;
	परत 0;

errout:
	ext2fs_free_icount(icount);
	परत(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णांक flags,
			       अचिन्हित पूर्णांक size,
			       ext2_icount_t *ret)
{
	परत ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
स्थिर संरचना ext2_icount_el *घाला_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णांक pos)
{
	संरचना ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	पूर्णांक			अंक;

	जर (icount->last_lookup && icount->last_lookup->ino == ino)
		परत icount->last_lookup;

	जर (icount->मोजणी >= icount->size) {
		जर (icount->मोजणी) {
			new_size = icount->list[(अचिन्हित)icount->मोजणी-1].ino;
			new_size = (ext2_ino_t) (icount->मोजणी *
				((दशांश) icount->अंक_inodes / new_size));
		}
		जर (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#जर 0
		म_लिखो("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((माप_प्रकार) icount->size *
					   आकार(संरचना ext2_icount_el),
					   (माप_प्रकार) new_size *
					   आकार(संरचना ext2_icount_el),
					   &icount->list);
		जर (retval)
			परत 0;
		icount->size = new_size;
	}
	अंक = (पूर्णांक) icount->मोजणी - pos;
	जर (अंक < 0)
		परत 0;	/* should never happen */
	जर (अंक) {
		स_हटाओ(&icount->list[pos+1], &icount->list[pos],
			आकार(संरचना ext2_icount_el) * अंक);
	}
	icount->मोजणी++;
	el = &icount->list[pos];
	el->मोजणी = 0;
	el->ino = ino;
	icount->last_lookup = el;
	परत el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
स्थिर संरचना ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णांक create)
{
	पूर्णांक	खालचा, वरचा, mid;

	जर (!icount || !icount->list)
		परत 0;

	जर (create && ((icount->मोजणी == 0) ||
		       (ino > icount->list[(अचिन्हित)icount->मोजणी-1].ino))) {
		परत घाला_icount_el(icount, ino, (अचिन्हित) icount->मोजणी);
	}
	जर (icount->मोजणी == 0)
		परत 0;

	जर (icount->cursor >= icount->मोजणी)
		icount->cursor = 0;
	जर (ino == icount->list[icount->cursor].ino)
		परत &icount->list[icount->cursor++];
#जर 0
	म_लिखो("Non-cursor get_icount_el: %u\n", ino);
#endif
	खालचा = 0;
	वरचा = (पूर्णांक) icount->मोजणी-1;
	पर्यंत (खालचा <= वरचा) {
		mid = ((अचिन्हित)खालचा + (अचिन्हित)वरचा) >> 1;
		जर (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			परत &icount->list[mid];
		}
		जर (ino < icount->list[mid].ino)
			वरचा = mid-1;
		अन्यथा
			खालचा = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	जर (create)
		परत घाला_icount_el(icount, ino, खालचा);
	परत 0;
}

स्थिर errcode_t set_inode_मोजणी(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 मोजणी)
{
	संरचना ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA किल्ली, data;

	जर (icount->tdb) {
		किल्ली.dptr = (अचिन्हित अक्षर *) &ino;
		किल्ली.dsize = आकार(ext2_ino_t);
		data.dptr = (अचिन्हित अक्षर *) &मोजणी;
		data.dsize = आकार(__u32);
		जर (मोजणी) {
			जर (tdb_store(icount->tdb, किल्ली, data, TDB_REPLACE))
				परत tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} अन्यथा {
			जर (tdb_delete(icount->tdb, किल्ली))
				परत tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		परत 0;
	}
#endif
	जर (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(मोजणी);
		परत 0;
	}

	el = get_icount_el(icount, ino, 1);
	जर (!el)
		परत EXT2_ET_NO_MEMORY;

	el->मोजणी = मोजणी;
	परत 0;
}

स्थिर errcode_t get_inode_मोजणी(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *मोजणी)
{
	संरचना ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA किल्ली, data;

	जर (icount->tdb) {
		किल्ली.dptr = (अचिन्हित अक्षर *) &ino;
		किल्ली.dsize = आकार(ext2_ino_t);

		data = tdb_fetch(icount->tdb, किल्ली);
		जर (data.dptr == रिक्त) {
			*मोजणी = 0;
			परत tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*मोजणी = *((__u32 *) data.dptr);
		मुक्त(data.dptr);
		परत 0;
	}
#endif
	जर (icount->fullmap) {
		*मोजणी = icount->fullmap[ino];
		परत 0;
	}

	el = get_icount_el(icount, ino, 0);
	जर (!el) {
		*मोजणी = 0;
		परत ENOENT;
	}

	*मोजणी = el->मोजणी;
	परत 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, खाता *out)
{
	errcode_t	ret = 0;
	अचिन्हित पूर्णांक	i;
	स्थिरांक अक्षर *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	जर (icount->मोजणी > icount->size) {
		ख_लिखो(out, "%s: count > size\n", bad);
		परत EXT2_ET_INVALID_ARGUMENT;
	}
	साठी (i=1; i < icount->मोजणी; i++) {
		जर (icount->list[i-1].ino >= icount->list[i].ino) {
			ख_लिखो(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	परत ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	जर (!ino || (ino > icount->अंक_inodes))
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (!icount->fullmap) {
		जर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			परत 0;
		}
		जर (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			परत 0;
		}
	}
	get_inode_मोजणी(icount, ino, &val);
	*ret = icount_16_xlate(val);
	परत 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मूल्य;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	जर (!ino || (ino > icount->अंक_inodes))
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (icount->fullmap) {
		curr_मूल्य = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_मूल्य;
	} अन्यथा जर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		जर (set_inode_मोजणी(icount, ino, 2))
			परत EXT2_ET_NO_MEMORY;
		curr_मूल्य = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} अन्यथा जर (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		जर (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_मोजणी(icount, ino, &curr_मूल्य);
			curr_मूल्य++;
			जर (set_inode_मोजणी(icount, ino, curr_मूल्य))
				परत EXT2_ET_NO_MEMORY;
		} अन्यथा {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			जर (ret)
				*ret = 1;
			परत 0;
		}
	} अन्यथा {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_मोजणी(icount, ino, &curr_मूल्य);
		curr_मूल्य++;
		जर (set_inode_मोजणी(icount, ino, curr_मूल्य))
			परत EXT2_ET_NO_MEMORY;
	}
	जर (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	जर (ret)
		*ret = icount_16_xlate(curr_मूल्य);
	परत 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मूल्य;

	जर (!ino || (ino > icount->अंक_inodes))
		परत EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	जर (icount->fullmap) {
		जर (!icount->fullmap[ino])
			परत EXT2_ET_INVALID_ARGUMENT;

		curr_मूल्य = --icount->fullmap[ino];
		जर (ret)
			*ret = icount_16_xlate(curr_मूल्य);
		परत 0;
	}

	जर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		जर (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		अन्यथा {
			set_inode_मोजणी(icount, ino, 0);
		}
		जर (ret)
			*ret = 0;
		परत 0;
	}

	जर (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		परत EXT2_ET_INVALID_ARGUMENT;

	get_inode_मोजणी(icount, ino, &curr_मूल्य);
	जर (!curr_मूल्य)
		परत EXT2_ET_INVALID_ARGUMENT;
	curr_मूल्य--;
	जर (set_inode_मोजणी(icount, ino, curr_मूल्य))
		परत EXT2_ET_NO_MEMORY;

	जर (curr_मूल्य == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	जर ((curr_मूल्य == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	जर (ret)
		*ret = icount_16_xlate(curr_मूल्य);
	परत 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 मोजणी)
{
	जर (!ino || (ino > icount->अंक_inodes))
		परत EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	जर (icount->fullmap)
		परत set_inode_मोजणी(icount, ino, मोजणी);

	जर (मोजणी == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		जर (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		परत 0;
	}
	जर (मोजणी == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		जर (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} अन्यथा
			set_inode_मोजणी(icount, ino, 0);
		परत 0;
	}

	जर (set_inode_मोजणी(icount, ino, मोजणी))
		परत EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	जर (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	परत 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	जर (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		परत 0;

	परत icount->size;
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
	पूर्णांक		cmd;
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

	स_रखो(&param, 0, आकार(param));
	ext2fs_blocks_मोजणी_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	जर (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	जर (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निकास(1);
	}
}

पूर्णांक run_test(पूर्णांक flags, पूर्णांक size, अक्षर *dir, संरचना test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	संरचना test_program *pc;
	__u16		result;
	पूर्णांक		problem = 0;

	जर (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		जर (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			निकास(1);
		}
#अन्यथा
		म_लिखो("Skipped\n");
		परत 0;
#endif
	} अन्यथा {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		जर (retval) {
			com_err("run_test", retval, "while creating icount");
			निकास(1);
		}
	}
	साठी (pc = prog; pc->cmd != EXIT; pc++) {
		निवड (pc->cmd) {
		स्थिती FETCH:
			म_लिखो("icount_fetch(%u) = ", pc->ino);
			थांबा;
		स्थिती STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			जर (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				निकास(1);
			}
			म_लिखो("icount_store(%u, %u) = ", pc->ino, pc->arg);
			थांबा;
		स्थिती INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			जर (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				निकास(1);
			}
			म_लिखो("icount_increment(%u) = ", pc->ino);
			थांबा;
		स्थिती DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			जर (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				निकास(1);
			}
			म_लिखो("icount_decrement(%u) = ", pc->ino);
			थांबा;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		जर (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			निकास(1);
		}
		म_लिखो("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		जर (result != pc->expected)
			problem++;
	}
	म_लिखो("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, मानक_निकास);
	जर (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		निकास(1);
	}
	ext2fs_free_icount(icount);
	परत problem;
}


पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक failed = 0;

	setup();
	म_लिखो("Standard icount run:\n");
	failed += run_test(0, 0, 0, prog);
	म_लिखो("\nMultiple bitmap test:\n");
	failed += run_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	म_लिखो("\nResizing icount:\n");
	failed += run_test(0, 3, 0, extended);
	म_लिखो("\nStandard icount run with tdb:\n");
	failed += run_test(0, 0, ".", prog);
	म_लिखो("\nMultiple bitmap test with tdb:\n");
	failed += run_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	जर (failed)
		म_लिखो("FAILED!\n");
	परत failed;
}
#endif
