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

#समावेश "config.h"
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <stdio.h>
#समावेश <sys/stat.h>
#समावेश <fcntl.h>
#समावेश <errno.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "tdb.h"

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

काष्ठा ext2_icount_el {
	ext2_ino_t	ino;
	__u32		गिनती;
};

काष्ठा ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		गिनती;
	ext2_ino_t		आकार;
	ext2_ino_t		अंक_inodes;
	ext2_ino_t		cursor;
	काष्ठा ext2_icount_el	*list;
	काष्ठा ext2_icount_el	*last_lookup;
#अगर_घोषित CONFIG_TDB
	अक्षर			*tdb_fn;
	TDB_CONTEXT		*tdb;
#पूर्ण_अगर
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#घोषणा icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

व्योम ext2fs_free_icount(ext2_icount_t icount)
{
	अगर (!icount)
		वापस;

	icount->magic = 0;
	अगर (icount->list)
		ext2fs_free_mem(&icount->list);
	अगर (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	अगर (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#अगर_घोषित CONFIG_TDB
	अगर (icount->tdb)
		tdb_close(icount->tdb);
	अगर (icount->tdb_fn) {
		(व्योम) unlink(icount->tdb_fn);
		मुक्त(icount->tdb_fn);
	}
#पूर्ण_अगर

	अगर (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

अटल errcode_t alloc_icount(ext2_filsys fs, पूर्णांक flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_icount), &icount);
	अगर (retval)
		वापस retval;
	स_रखो(icount, 0, माप(काष्ठा ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->अंक_inodes = fs->super->s_inodes_गिनती;

	अगर ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		अचिन्हित sz = माप(*icount->fullmap) * icount->अंक_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		अगर (!retval) {
			स_रखो(icount->fullmap, 0, sz);
			*ret = icount;
			वापस 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	अगर (retval)
		जाओ errout;

	अगर (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		अगर (retval)
			जाओ errout;
	} अन्यथा
		icount->multiple = 0;

	*ret = icount;
	वापस 0;

errout:
	ext2fs_free_icount(icount);
	वापस(retval);
}

#अगर_घोषित CONFIG_TDB
काष्ठा uuid {
	__u32	time_निचला;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	गाँठ[6];
};

अटल व्योम unpack_uuid(व्योम *in, काष्ठा uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_निचला = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	स_नकल(uu->गाँठ, ptr, 6);
}

अटल व्योम uuid_unparse(व्योम *uu, अक्षर *out)
{
	काष्ठा uuid uuid;

	unpack_uuid(uu, &uuid);
	प्र_लिखो(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_निचला, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.गाँठ[0], uuid.गाँठ[1], uuid.गाँठ[2],
		uuid.गाँठ[3], uuid.गाँठ[4], uuid.गाँठ[5]);
}
#पूर्ण_अगर

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   अक्षर *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   पूर्णांक flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#अगर_घोषित CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	अक्षर 		*fn, uuid[40];
	ext2_ino_t	अंक_inodes;
	mode_t		save_umask;
	पूर्णांक		fd;

	retval = alloc_icount(fs, flags,  &icount);
	अगर (retval)
		वापस retval;

	retval = ext2fs_get_mem(म_माप(tdb_dir) + 64, &fn);
	अगर (retval)
		जाओ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	प्र_लिखो(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	अगर (fd < 0) {
		retval = त्रुटि_सं;
		ext2fs_free_mem(&fn);
		जाओ errout;
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
	अंक_inodes = fs->super->s_inodes_गिनती - fs->super->s_free_inodes_गिनती;

	icount->tdb = tdb_open(fn, अंक_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	बंद(fd);
	अगर (icount->tdb == शून्य) {
		retval = त्रुटि_सं;
		जाओ errout;
	}
	*ret = icount;
	वापस 0;
errout:
	ext2fs_free_icount(icount);
	वापस(retval);
#अन्यथा
	वापस EXT2_ET_UNIMPLEMENTED;
#पूर्ण_अगर
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णांक flags, अचिन्हित पूर्णांक आकार,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	माप_प्रकार		bytes;
	ext2_ino_t	i;

	अगर (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		अगर (hint->आकार > आकार)
			आकार = (माप_प्रकार) hint->आकार;
	}

	retval = alloc_icount(fs, flags, &icount);
	अगर (retval)
		वापस retval;

	अगर (icount->fullmap)
		जाओ successout;

	अगर (आकार) {
		icount->आकार = आकार;
	} अन्यथा {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_अंक_dirs(fs, &icount->आकार);
		अगर (retval)
			जाओ errout;
		icount->आकार += fs->super->s_inodes_गिनती / 50;
	}

	bytes = (माप_प्रकार) (icount->आकार * माप(काष्ठा ext2_icount_el));
#अगर 0
	म_लिखो("Icount allocated %u entries, %d bytes.\n",
	       icount->आकार, bytes);
#पूर्ण_अगर
	retval = ext2fs_get_array(icount->आकार, माप(काष्ठा ext2_icount_el),
			 &icount->list);
	अगर (retval)
		जाओ errout;
	स_रखो(icount->list, 0, bytes);

	icount->गिनती = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	अगर (hint) {
		क्रम (i=0; i < hint->गिनती; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->गिनती = hint->गिनती;
	}

successout:
	*ret = icount;
	वापस 0;

errout:
	ext2fs_free_icount(icount);
	वापस(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णांक flags,
			       अचिन्हित पूर्णांक आकार,
			       ext2_icount_t *ret)
{
	वापस ext2fs_create_icount2(fs, flags, आकार, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
अटल काष्ठा ext2_icount_el *जोड़ो_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णांक pos)
{
	काष्ठा ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_आकार = 0;
	पूर्णांक			अंक;

	अगर (icount->last_lookup && icount->last_lookup->ino == ino)
		वापस icount->last_lookup;

	अगर (icount->गिनती >= icount->आकार) {
		अगर (icount->गिनती) {
			new_आकार = icount->list[(अचिन्हित)icount->गिनती-1].ino;
			new_आकार = (ext2_ino_t) (icount->गिनती *
				((भग्न) icount->अंक_inodes / new_आकार));
		}
		अगर (new_आकार < (icount->आकार + 100))
			new_आकार = icount->आकार + 100;
#अगर 0
		म_लिखो("Reallocating icount %u entries...\n", new_आकार);
#पूर्ण_अगर
		retval = ext2fs_resize_mem((माप_प्रकार) icount->आकार *
					   माप(काष्ठा ext2_icount_el),
					   (माप_प्रकार) new_आकार *
					   माप(काष्ठा ext2_icount_el),
					   &icount->list);
		अगर (retval)
			वापस 0;
		icount->आकार = new_आकार;
	}
	अंक = (पूर्णांक) icount->गिनती - pos;
	अगर (अंक < 0)
		वापस 0;	/* should never happen */
	अगर (अंक) {
		स_हटाओ(&icount->list[pos+1], &icount->list[pos],
			माप(काष्ठा ext2_icount_el) * अंक);
	}
	icount->गिनती++;
	el = &icount->list[pos];
	el->गिनती = 0;
	el->ino = ino;
	icount->last_lookup = el;
	वापस el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
अटल काष्ठा ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, पूर्णांक create)
{
	पूर्णांक	निचला, ऊपरी, mid;

	अगर (!icount || !icount->list)
		वापस 0;

	अगर (create && ((icount->गिनती == 0) ||
		       (ino > icount->list[(अचिन्हित)icount->गिनती-1].ino))) {
		वापस जोड़ो_icount_el(icount, ino, (अचिन्हित) icount->गिनती);
	}
	अगर (icount->गिनती == 0)
		वापस 0;

	अगर (icount->cursor >= icount->गिनती)
		icount->cursor = 0;
	अगर (ino == icount->list[icount->cursor].ino)
		वापस &icount->list[icount->cursor++];
#अगर 0
	म_लिखो("Non-cursor get_icount_el: %u\n", ino);
#पूर्ण_अगर
	निचला = 0;
	ऊपरी = (पूर्णांक) icount->गिनती-1;
	जबतक (निचला <= ऊपरी) {
		mid = ((अचिन्हित)निचला + (अचिन्हित)ऊपरी) >> 1;
		अगर (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			वापस &icount->list[mid];
		}
		अगर (ino < icount->list[mid].ino)
			ऊपरी = mid-1;
		अन्यथा
			निचला = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	अगर (create)
		वापस जोड़ो_icount_el(icount, ino, निचला);
	वापस 0;
}

अटल errcode_t set_inode_गिनती(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 गिनती)
{
	काष्ठा ext2_icount_el 	*el;
#अगर_घोषित CONFIG_TDB
	TDB_तथ्य कुंजी, तथ्य;

	अगर (icount->tdb) {
		कुंजी.dptr = (अचिन्हित अक्षर *) &ino;
		कुंजी.dsize = माप(ext2_ino_t);
		तथ्य.dptr = (अचिन्हित अक्षर *) &गिनती;
		तथ्य.dsize = माप(__u32);
		अगर (गिनती) {
			अगर (tdb_store(icount->tdb, कुंजी, तथ्य, TDB_REPLACE))
				वापस tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} अन्यथा {
			अगर (tdb_delete(icount->tdb, कुंजी))
				वापस tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		वापस 0;
	}
#पूर्ण_अगर
	अगर (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(गिनती);
		वापस 0;
	}

	el = get_icount_el(icount, ino, 1);
	अगर (!el)
		वापस EXT2_ET_NO_MEMORY;

	el->गिनती = गिनती;
	वापस 0;
}

अटल errcode_t get_inode_गिनती(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *गिनती)
{
	काष्ठा ext2_icount_el 	*el;
#अगर_घोषित CONFIG_TDB
	TDB_तथ्य कुंजी, तथ्य;

	अगर (icount->tdb) {
		कुंजी.dptr = (अचिन्हित अक्षर *) &ino;
		कुंजी.dsize = माप(ext2_ino_t);

		तथ्य = tdb_fetch(icount->tdb, कुंजी);
		अगर (तथ्य.dptr == शून्य) {
			*गिनती = 0;
			वापस tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*गिनती = *((__u32 *) तथ्य.dptr);
		मुक्त(तथ्य.dptr);
		वापस 0;
	}
#पूर्ण_अगर
	अगर (icount->fullmap) {
		*गिनती = icount->fullmap[ino];
		वापस 0;
	}

	el = get_icount_el(icount, ino, 0);
	अगर (!el) {
		*गिनती = 0;
		वापस ENOENT;
	}

	*गिनती = el->गिनती;
	वापस 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, खाता *out)
{
	errcode_t	ret = 0;
	अचिन्हित पूर्णांक	i;
	स्थिर अक्षर *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	अगर (icount->गिनती > icount->आकार) {
		ख_लिखो(out, "%s: count > size\n", bad);
		वापस EXT2_ET_INVALID_ARGUMENT;
	}
	क्रम (i=1; i < icount->गिनती; i++) {
		अगर (icount->list[i-1].ino >= icount->list[i].ino) {
			ख_लिखो(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	वापस ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	अगर (!ino || (ino > icount->अंक_inodes))
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (!icount->fullmap) {
		अगर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			वापस 0;
		}
		अगर (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			वापस 0;
		}
	}
	get_inode_गिनती(icount, ino, &val);
	*ret = icount_16_xlate(val);
	वापस 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मूल्य;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	अगर (!ino || (ino > icount->अंक_inodes))
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (icount->fullmap) {
		curr_मूल्य = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_मूल्य;
	} अन्यथा अगर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		अगर (set_inode_गिनती(icount, ino, 2))
			वापस EXT2_ET_NO_MEMORY;
		curr_मूल्य = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} अन्यथा अगर (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		अगर (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_गिनती(icount, ino, &curr_मूल्य);
			curr_मूल्य++;
			अगर (set_inode_गिनती(icount, ino, curr_मूल्य))
				वापस EXT2_ET_NO_MEMORY;
		} अन्यथा {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			अगर (ret)
				*ret = 1;
			वापस 0;
		}
	} अन्यथा {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_गिनती(icount, ino, &curr_मूल्य);
		curr_मूल्य++;
		अगर (set_inode_गिनती(icount, ino, curr_मूल्य))
			वापस EXT2_ET_NO_MEMORY;
	}
	अगर (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	अगर (ret)
		*ret = icount_16_xlate(curr_मूल्य);
	वापस 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मूल्य;

	अगर (!ino || (ino > icount->अंक_inodes))
		वापस EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	अगर (icount->fullmap) {
		अगर (!icount->fullmap[ino])
			वापस EXT2_ET_INVALID_ARGUMENT;

		curr_मूल्य = --icount->fullmap[ino];
		अगर (ret)
			*ret = icount_16_xlate(curr_मूल्य);
		वापस 0;
	}

	अगर (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		अगर (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		अन्यथा {
			set_inode_गिनती(icount, ino, 0);
		}
		अगर (ret)
			*ret = 0;
		वापस 0;
	}

	अगर (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		वापस EXT2_ET_INVALID_ARGUMENT;

	get_inode_गिनती(icount, ino, &curr_मूल्य);
	अगर (!curr_मूल्य)
		वापस EXT2_ET_INVALID_ARGUMENT;
	curr_मूल्य--;
	अगर (set_inode_गिनती(icount, ino, curr_मूल्य))
		वापस EXT2_ET_NO_MEMORY;

	अगर (curr_मूल्य == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	अगर ((curr_मूल्य == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	अगर (ret)
		*ret = icount_16_xlate(curr_मूल्य);
	वापस 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 गिनती)
{
	अगर (!ino || (ino > icount->अंक_inodes))
		वापस EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	अगर (icount->fullmap)
		वापस set_inode_गिनती(icount, ino, गिनती);

	अगर (गिनती == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		अगर (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		वापस 0;
	}
	अगर (गिनती == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		अगर (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} अन्यथा
			set_inode_गिनती(icount, ino, 0);
		वापस 0;
	}

	अगर (set_inode_गिनती(icount, ino, गिनती))
		वापस EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	अगर (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	वापस 0;
}

ext2_ino_t ext2fs_get_icount_आकार(ext2_icount_t icount)
{
	अगर (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		वापस 0;

	वापस icount->आकार;
}

#अगर_घोषित DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#घोषणा EXIT		0x00
#घोषणा FETCH		0x01
#घोषणा STORE		0x02
#घोषणा INCREMENT	0x03
#घोषणा DECREMENT	0x04

काष्ठा test_program {
	पूर्णांक		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

काष्ठा test_program prog[] = {
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

काष्ठा test_program extended[] = {
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
अटल व्योम setup(व्योम)
{
	errcode_t	retval;
	काष्ठा ext2_super_block param;

	initialize_ext2_error_table();

	स_रखो(&param, 0, माप(param));
	ext2fs_blocks_गिनती_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	अगर (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	अगर (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निकास(1);
	}
}

पूर्णांक run_test(पूर्णांक flags, पूर्णांक आकार, अक्षर *dir, काष्ठा test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	काष्ठा test_program *pc;
	__u16		परिणाम;
	पूर्णांक		problem = 0;

	अगर (dir) {
#अगर_घोषित CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		अगर (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			निकास(1);
		}
#अन्यथा
		म_लिखो("Skipped\n");
		वापस 0;
#पूर्ण_अगर
	} अन्यथा {
		retval = ext2fs_create_icount2(test_fs, flags, आकार, 0,
					       &icount);
		अगर (retval) {
			com_err("run_test", retval, "while creating icount");
			निकास(1);
		}
	}
	क्रम (pc = prog; pc->cmd != EXIT; pc++) {
		चयन (pc->cmd) {
		हाल FETCH:
			म_लिखो("icount_fetch(%u) = ", pc->ino);
			अवरोध;
		हाल STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			अगर (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				निकास(1);
			}
			म_लिखो("icount_store(%u, %u) = ", pc->ino, pc->arg);
			अवरोध;
		हाल INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			अगर (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				निकास(1);
			}
			म_लिखो("icount_increment(%u) = ", pc->ino);
			अवरोध;
		हाल DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			अगर (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				निकास(1);
			}
			म_लिखो("icount_decrement(%u) = ", pc->ino);
			अवरोध;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &परिणाम);
		अगर (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			निकास(1);
		}
		म_लिखो("%u (%s)\n", परिणाम, (परिणाम == pc->expected) ?
		       "OK" : "NOT OK");
		अगर (परिणाम != pc->expected)
			problem++;
	}
	म_लिखो("icount size is %u\n", ext2fs_get_icount_आकार(icount));
	retval = ext2fs_icount_validate(icount, मानक_निकास);
	अगर (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		निकास(1);
	}
	ext2fs_free_icount(icount);
	वापस problem;
}


पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
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
	अगर (failed)
		म_लिखो("FAILED!\n");
	वापस failed;
}
#पूर्ण_अगर
