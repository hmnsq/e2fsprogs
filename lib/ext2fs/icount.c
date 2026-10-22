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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
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
	ext2fs_inode_bitmap	गुणजः;
	ext2_ino_t		गणनम्;
	ext2_ino_t		आकार;
	ext2_ino_t		अङ्कः_inodes;
	ext2_ino_t		cursor;
	संरचना ext2_icount_el	*list;
	संरचना ext2_icount_el	*चरमः_lookup;
#यदि_परिभाषितम् CONFIG_TDB
	वर्ण			*tdb_fn;
	TDB_CONTEXT		*tdb;
#यदि_अन्तः
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#परिभाषय icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

शून्य ext2fs_free_icount(ext2_icount_t icount)
{
	यदि (!icount)
		प्रत्यागम;

	icount->magic = 0;
	यदि (icount->list)
		ext2fs_free_mem(&icount->list);
	यदि (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	यदि (icount->गुणजः)
		ext2fs_free_inode_bitmap(icount->गुणजः);
#यदि_परिभाषितम् CONFIG_TDB
	यदि (icount->tdb)
		tdb_close(icount->tdb);
	यदि (icount->tdb_fn) {
		(शून्य) unlink(icount->tdb_fn);
		मुक्त(icount->tdb_fn);
	}
#यदि_अन्तः

	यदि (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

स्थिर errcode_t alloc_icount(ext2_filsys fs, पूर्णाङ्क flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_icount), &icount);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(icount, 0, परिमाण(संरचना ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->अङ्कः_inodes = fs->super->s_inodes_गणनम्;

	यदि ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		अचिह्नित sz = परिमाण(*icount->fullmap) * icount->अङ्कः_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		यदि (!retval) {
			स्मृतिं_पूरय(icount->fullmap, 0, sz);
			*ret = icount;
			प्रत्यागम 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	यदि (retval)
		गच्छ errout;

	यदि (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->गुणजः);
		यदि (retval)
			गच्छ errout;
	} अन्यथा
		icount->गुणजः = 0;

	*ret = icount;
	प्रत्यागम 0;

errout:
	ext2fs_free_icount(icount);
	प्रत्यागम(retval);
}

#यदि_परिभाषितम् CONFIG_TDB
संरचना uuid {
	__u32	time_नीचः;
	__u16	time_mid;
	__u16	time_hi_and_संस्करणम्;
	__u16	clock_seq;
	__u8	ग्रन्थिः[6];
};

स्थिर शून्य unpack_uuid(शून्य *अन्तर्, संरचना uuid *uu)
{
	__u8	*ptr = अन्तर्;
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
	uu->time_hi_and_संस्करणम् = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	स्मृतिं_प्रतिलिख(uu->ग्रन्थिः, ptr, 6);
}

स्थिर शून्य uuid_unparse(शून्य *uu, वर्ण *बहिर्गमनम्)
{
	संरचना uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(बहिर्गमनम्,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_नीचः, uuid.time_mid, uuid.time_hi_and_संस्करणम्,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ग्रन्थिः[0], uuid.ग्रन्थिः[1], uuid.ग्रन्थिः[2],
		uuid.ग्रन्थिः[3], uuid.ग्रन्थिः[4], uuid.ग्रन्थिः[5]);
}
#यदि_अन्तः

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_अप्रयुक्तम्,
				   वर्ण *tdb_dir EXT2_FS_NO_TDB_अप्रयुक्तम्,
				   पूर्णाङ्क flags EXT2_FS_NO_TDB_अप्रयुक्तम्,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_अप्रयुक्तम्)
{
#यदि_परिभाषितम् CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	वर्ण 		*fn, uuid[40];
	ext2_ino_t	अङ्कः_inodes;
	mode_t		save_umask;
	पूर्णाङ्क		fd;

	retval = alloc_icount(fs, flags,  &icount);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(tdb_dir) + 64, &fn);
	यदि (retval)
		गच्छ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	यदि (fd < 0) {
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
	यदि (icount->tdb == रिक्त) {
		retval = errno;
		गच्छ errout;
	}
	*ret = icount;
	प्रत्यागम 0;
errout:
	ext2fs_free_icount(icount);
	प्रत्यागम(retval);
#अन्यथा
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
#यदि_अन्तः
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, पूर्णाङ्क flags, अचिह्नित पूर्णाङ्क आकार,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		अष्टकानि;
	ext2_ino_t	i;

	यदि (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		यदि (hint->आकार > आकार)
			आकार = (size_t) hint->आकार;
	}

	retval = alloc_icount(fs, flags, &icount);
	यदि (retval)
		प्रत्यागम retval;

	यदि (icount->fullmap)
		गच्छ successout;

	यदि (आकार) {
		icount->आकार = आकार;
	} अन्यथा {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_अङ्कः_dirs(fs, &icount->आकार);
		यदि (retval)
			गच्छ errout;
		icount->आकार += fs->super->s_inodes_गणनम् / 50;
	}

	अष्टकानि = (size_t) (icount->आकार * परिमाण(संरचना ext2_icount_el));
#यदि 0
	प्रारूप_लिख("Icount allocated %u entries, %d bytes.\n",
	       icount->आकार, अष्टकानि);
#यदि_अन्तः
	retval = ext2fs_get_array(icount->आकार, परिमाण(संरचना ext2_icount_el),
			 &icount->list);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(icount->list, 0, अष्टकानि);

	icount->गणनम् = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	यदि (hint) {
		प्रति (i=0; i < hint->गणनम्; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->गणनम् = hint->गणनम्;
	}

successout:
	*ret = icount;
	प्रत्यागम 0;

errout:
	ext2fs_free_icount(icount);
	प्रत्यागम(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, पूर्णाङ्क flags,
			       अचिह्नित पूर्णाङ्क आकार,
			       ext2_icount_t *ret)
{
	प्रत्यागम ext2fs_create_icount2(fs, flags, आकार, 0, ret);
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
	ext2_ino_t		new_आकार = 0;
	पूर्णाङ्क			अङ्कः;

	यदि (icount->चरमः_lookup && icount->चरमः_lookup->ino == ino)
		प्रत्यागम icount->चरमः_lookup;

	यदि (icount->गणनम् >= icount->आकार) {
		यदि (icount->गणनम्) {
			new_आकार = icount->list[(अचिह्नित)icount->गणनम्-1].ino;
			new_आकार = (ext2_ino_t) (icount->गणनम् *
				((भिन्न) icount->अङ्कः_inodes / new_आकार));
		}
		यदि (new_आकार < (icount->आकार + 100))
			new_आकार = icount->आकार + 100;
#यदि 0
		प्रारूप_लिख("Reallocating icount %u entries...\n", new_आकार);
#यदि_अन्तः
		retval = ext2fs_resize_mem((size_t) icount->आकार *
					   परिमाण(संरचना ext2_icount_el),
					   (size_t) new_आकार *
					   परिमाण(संरचना ext2_icount_el),
					   &icount->list);
		यदि (retval)
			प्रत्यागम 0;
		icount->आकार = new_आकार;
	}
	अङ्कः = (पूर्णाङ्क) icount->गणनम् - pos;
	यदि (अङ्कः < 0)
		प्रत्यागम 0;	/* should never happen */
	यदि (अङ्कः) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			परिमाण(संरचना ext2_icount_el) * अङ्कः);
	}
	icount->गणनम्++;
	el = &icount->list[pos];
	el->गणनम् = 0;
	el->ino = ino;
	icount->चरमः_lookup = el;
	प्रत्यागम el;
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

	यदि (!icount || !icount->list)
		प्रत्यागम 0;

	यदि (create && ((icount->गणनम् == 0) ||
		       (ino > icount->list[(अचिह्नित)icount->गणनम्-1].ino))) {
		प्रत्यागम निवेशय_icount_el(icount, ino, (अचिह्नित) icount->गणनम्);
	}
	यदि (icount->गणनम् == 0)
		प्रत्यागम 0;

	यदि (icount->cursor >= icount->गणनम्)
		icount->cursor = 0;
	यदि (ino == icount->list[icount->cursor].ino)
		प्रत्यागम &icount->list[icount->cursor++];
#यदि 0
	प्रारूप_लिख("Non-cursor get_icount_el: %u\n", ino);
#यदि_अन्तः
	नीचः = 0;
	उच्चः = (पूर्णाङ्क) icount->गणनम्-1;
	यावत् (नीचः <= उच्चः) {
		mid = ((अचिह्नित)नीचः + (अचिह्नित)उच्चः) >> 1;
		यदि (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			प्रत्यागम &icount->list[mid];
		}
		यदि (ino < icount->list[mid].ino)
			उच्चः = mid-1;
		अन्यथा
			नीचः = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	यदि (create)
		प्रत्यागम निवेशय_icount_el(icount, ino, नीचः);
	प्रत्यागम 0;
}

स्थिर errcode_t समुच्चयः_inode_गणनम्(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 गणनम्)
{
	संरचना ext2_icount_el 	*el;
#यदि_परिभाषितम् CONFIG_TDB
	TDB_दत्त कुञ्चिका, दत्त;

	यदि (icount->tdb) {
		कुञ्चिका.dptr = (अचिह्नित वर्ण *) &ino;
		कुञ्चिका.dsize = परिमाण(ext2_ino_t);
		दत्त.dptr = (अचिह्नित वर्ण *) &गणनम्;
		दत्त.dsize = परिमाण(__u32);
		यदि (गणनम्) {
			यदि (tdb_store(icount->tdb, कुञ्चिका, दत्त, TDB_REPLACE))
				प्रत्यागम tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} अन्यथा {
			यदि (tdb_delete(icount->tdb, कुञ्चिका))
				प्रत्यागम tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		प्रत्यागम 0;
	}
#यदि_अन्तः
	यदि (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(गणनम्);
		प्रत्यागम 0;
	}

	el = get_icount_el(icount, ino, 1);
	यदि (!el)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	el->गणनम् = गणनम्;
	प्रत्यागम 0;
}

स्थिर errcode_t get_inode_गणनम्(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *गणनम्)
{
	संरचना ext2_icount_el 	*el;
#यदि_परिभाषितम् CONFIG_TDB
	TDB_दत्त कुञ्चिका, दत्त;

	यदि (icount->tdb) {
		कुञ्चिका.dptr = (अचिह्नित वर्ण *) &ino;
		कुञ्चिका.dsize = परिमाण(ext2_ino_t);

		दत्त = tdb_fetch(icount->tdb, कुञ्चिका);
		यदि (दत्त.dptr == रिक्त) {
			*गणनम् = 0;
			प्रत्यागम tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*गणनम् = *((__u32 *) दत्त.dptr);
		मुक्त(दत्त.dptr);
		प्रत्यागम 0;
	}
#यदि_अन्तः
	यदि (icount->fullmap) {
		*गणनम् = icount->fullmap[ino];
		प्रत्यागम 0;
	}

	el = get_icount_el(icount, ino, 0);
	यदि (!el) {
		*गणनम् = 0;
		प्रत्यागम ENOENT;
	}

	*गणनम् = el->गणनम्;
	प्रत्यागम 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *बहिर्गमनम्)
{
	errcode_t	ret = 0;
	अचिह्नित पूर्णाङ्क	i;
	ध्रुव वर्ण *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	यदि (icount->गणनम् > icount->आकार) {
		fprintf(बहिर्गमनम्, "%s: count > size\n", bad);
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	}
	प्रति (i=1; i < icount->गणनम्; i++) {
		यदि (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(बहिर्गमनम्, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_तर्कः;
		}
	}
	प्रत्यागम ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	यदि (!ino || (ino > icount->अङ्कः_inodes))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (!icount->fullmap) {
		यदि (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			प्रत्यागम 0;
		}
		यदि (icount->गुणजः &&
			!ext2fs_test_inode_bitmap2(icount->गुणजः, ino)) {
			*ret = 0;
			प्रत्यागम 0;
		}
	}
	get_inode_गणनम्(icount, ino, &val);
	*ret = icount_16_xlate(val);
	प्रत्यागम 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मानम्;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	यदि (!ino || (ino > icount->अङ्कः_inodes))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (icount->fullmap) {
		curr_मानम् = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_मानम्;
	} अन्यथा यदि (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		यदि (समुच्चयः_inode_गणनम्(icount, ino, 2))
			प्रत्यागम EXT2_ET_NO_स्मृतिः;
		curr_मानम् = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} अन्यथा यदि (icount->गुणजः) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		यदि (ext2fs_test_inode_bitmap2(icount->गुणजः, ino)) {
			get_inode_गणनम्(icount, ino, &curr_मानम्);
			curr_मानम्++;
			यदि (समुच्चयः_inode_गणनम्(icount, ino, curr_मानम्))
				प्रत्यागम EXT2_ET_NO_स्मृतिः;
		} अन्यथा {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			यदि (ret)
				*ret = 1;
			प्रत्यागम 0;
		}
	} अन्यथा {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_गणनम्(icount, ino, &curr_मानम्);
		curr_मानम्++;
		यदि (समुच्चयः_inode_गणनम्(icount, ino, curr_मानम्))
			प्रत्यागम EXT2_ET_NO_स्मृतिः;
	}
	यदि (icount->गुणजः)
		ext2fs_mark_inode_bitmap2(icount->गुणजः, ino);
	यदि (ret)
		*ret = icount_16_xlate(curr_मानम्);
	प्रत्यागम 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_मानम्;

	यदि (!ino || (ino > icount->अङ्कः_inodes))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	यदि (icount->fullmap) {
		यदि (!icount->fullmap[ino])
			प्रत्यागम EXT2_ET_INVALID_तर्कः;

		curr_मानम् = --icount->fullmap[ino];
		यदि (ret)
			*ret = icount_16_xlate(curr_मानम्);
		प्रत्यागम 0;
	}

	यदि (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		यदि (icount->गुणजः)
			ext2fs_unmark_inode_bitmap2(icount->गुणजः, ino);
		अन्यथा {
			समुच्चयः_inode_गणनम्(icount, ino, 0);
		}
		यदि (ret)
			*ret = 0;
		प्रत्यागम 0;
	}

	यदि (icount->गुणजः &&
	    !ext2fs_test_inode_bitmap2(icount->गुणजः, ino))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	get_inode_गणनम्(icount, ino, &curr_मानम्);
	यदि (!curr_मानम्)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	curr_मानम्--;
	यदि (समुच्चयः_inode_गणनम्(icount, ino, curr_मानम्))
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	यदि (curr_मानम् == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	यदि ((curr_मानम् == 0) && icount->गुणजः)
		ext2fs_unmark_inode_bitmap2(icount->गुणजः, ino);

	यदि (ret)
		*ret = icount_16_xlate(curr_मानम्);
	प्रत्यागम 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 गणनम्)
{
	यदि (!ino || (ino > icount->अङ्कः_inodes))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	यदि (icount->fullmap)
		प्रत्यागम समुच्चयः_inode_गणनम्(icount, ino, गणनम्);

	यदि (गणनम् == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		यदि (icount->गुणजः)
			ext2fs_unmark_inode_bitmap2(icount->गुणजः, ino);
		प्रत्यागम 0;
	}
	यदि (गणनम् == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		यदि (icount->गुणजः) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->गुणजः, ino);
		} अन्यथा
			समुच्चयः_inode_गणनम्(icount, ino, 0);
		प्रत्यागम 0;
	}

	यदि (समुच्चयः_inode_गणनम्(icount, ino, गणनम्))
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	यदि (icount->गुणजः)
		ext2fs_mark_inode_bitmap2(icount->गुणजः, ino);
	प्रत्यागम 0;
}

ext2_ino_t ext2fs_get_icount_आकार(ext2_icount_t icount)
{
	यदि (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		प्रत्यागम 0;

	प्रत्यागम icount->आकार;
}

#यदि_परिभाषितम् DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#परिभाषय EXIT		0x00
#परिभाषय FETCH		0x01
#परिभाषय STORE		0x02
#परिभाषय INCREMENT	0x03
#परिभाषय DECREMENT	0x04

संरचना test_क्रमादेशः {
	पूर्णाङ्क		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

संरचना test_क्रमादेशः prog[] = {
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

संरचना test_क्रमादेशः extended[] = {
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
स्थिर शून्य सज्जता(शून्य)
{
	errcode_t	retval;
	संरचना ext2_super_खण्डम् param;

	initialize_ext2_error_सारणी();

	स्मृतिं_पूरय(&param, 0, परिमाण(param));
	ext2fs_खण्डानि_गणनम्_समुच्चयः(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_द्वयङ्काः, &param,
				   test_io_manager, &test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निर्गच्छ(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निर्गच्छ(1);
	}
}

पूर्णाङ्क सततम्_test(पूर्णाङ्क flags, पूर्णाङ्क आकार, वर्ण *dir, संरचना test_क्रमादेशः *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	संरचना test_क्रमादेशः *pc;
	__u16		परिणाम;
	पूर्णाङ्क		problem = 0;

	यदि (dir) {
#यदि_परिभाषितम् CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		यदि (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			निर्गच्छ(1);
		}
#अन्यथा
		प्रारूप_लिख("Skipped\n");
		प्रत्यागम 0;
#यदि_अन्तः
	} अन्यथा {
		retval = ext2fs_create_icount2(test_fs, flags, आकार, 0,
					       &icount);
		यदि (retval) {
			com_err("run_test", retval, "while creating icount");
			निर्गच्छ(1);
		}
	}
	प्रति (pc = prog; pc->cmd != EXIT; pc++) {
		विकल्प (pc->cmd) {
		पक्ष FETCH:
			प्रारूप_लिख("icount_fetch(%u) = ", pc->ino);
			भङ्ग;
		पक्ष STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			यदि (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				निर्गच्छ(1);
			}
			प्रारूप_लिख("icount_store(%u, %u) = ", pc->ino, pc->arg);
			भङ्ग;
		पक्ष INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			यदि (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				निर्गच्छ(1);
			}
			प्रारूप_लिख("icount_increment(%u) = ", pc->ino);
			भङ्ग;
		पक्ष DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			यदि (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				निर्गच्छ(1);
			}
			प्रारूप_लिख("icount_decrement(%u) = ", pc->ino);
			भङ्ग;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &परिणाम);
		यदि (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			निर्गच्छ(1);
		}
		प्रारूप_लिख("%u (%s)\n", परिणाम, (परिणाम == pc->expected) ?
		       "OK" : "NOT OK");
		यदि (परिणाम != pc->expected)
			problem++;
	}
	प्रारूप_लिख("icount size is %u\n", ext2fs_get_icount_आकार(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	यदि (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		निर्गच्छ(1);
	}
	ext2fs_free_icount(icount);
	प्रत्यागम problem;
}


पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क failed = 0;

	सज्जता();
	प्रारूप_लिख("Standard icount run:\n");
	failed += सततम्_test(0, 0, 0, prog);
	प्रारूप_लिख("\nMultiple bitmap test:\n");
	failed += सततम्_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	प्रारूप_लिख("\nResizing icount:\n");
	failed += सततम्_test(0, 3, 0, extended);
	प्रारूप_लिख("\nStandard icount run with tdb:\n");
	failed += सततम्_test(0, 0, ".", prog);
	प्रारूप_लिख("\nMultiple bitmap test with tdb:\n");
	failed += सततम्_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	यदि (failed)
		प्रारूप_लिख("FAILED!\n");
	प्रत्यागम failed;
}
#यदि_अन्तः
