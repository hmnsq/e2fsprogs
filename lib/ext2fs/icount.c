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

#అన్తర్భావ "config.h"
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "tdb.h"

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

నిర్మాణం ext2_icount_el {
	ext2_ino_t	ino;
	__u32		లెక్క;
};

నిర్మాణం ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	గుణిజం;
	ext2_ino_t		లెక్క;
	ext2_ino_t		కొలత;
	ext2_ino_t		అంకె_inodes;
	ext2_ino_t		cursor;
	నిర్మాణం ext2_icount_el	*list;
	నిర్మాణం ext2_icount_el	*చివరి_lookup;
#నిర్వచితమైతే CONFIG_TDB
	అక్షరం			*tdb_fn;
	TDB_CONTEXT		*tdb;
#అయితే_ముగింపు
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#నిర్వచించు icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

శూన్య ext2fs_free_icount(ext2_icount_t icount)
{
	ఒకవేళ (!icount)
		తిరిగి;

	icount->magic = 0;
	ఒకవేళ (icount->list)
		ext2fs_free_mem(&icount->list);
	ఒకవేళ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ఒకవేళ (icount->గుణిజం)
		ext2fs_free_inode_bitmap(icount->గుణిజం);
#నిర్వచితమైతే CONFIG_TDB
	ఒకవేళ (icount->tdb)
		tdb_close(icount->tdb);
	ఒకవేళ (icount->tdb_fn) {
		(శూన్య) unlink(icount->tdb_fn);
		విడుదల(icount->tdb_fn);
	}
#అయితే_ముగింపు

	ఒకవేళ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

స్థిర errcode_t alloc_icount(ext2_filsys fs, పూర్ణాంకం flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_icount), &icount);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(icount, 0, పరిమాణం(నిర్మాణం ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->అంకె_inodes = fs->super->s_inodes_లెక్క;

	ఒకవేళ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		గుర్తులేని sz = పరిమాణం(*icount->fullmap) * icount->అంకె_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ఒకవేళ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			తిరిగి 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	ఒకవేళ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->గుణిజం);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	} లేకపోతే
		icount->గుణిజం = 0;

	*ret = icount;
	తిరిగి 0;

errout:
	ext2fs_free_icount(icount);
	తిరిగి(retval);
}

#నిర్వచితమైతే CONFIG_TDB
నిర్మాణం uuid {
	__u32	సమయం_దిగువ;
	__u16	సమయం_mid;
	__u16	సమయం_hi_and_సంస్కరణ;
	__u16	clock_seq;
	__u8	కణుపు[6];
};

స్థిర శూన్య unpack_uuid(శూన్య *లోపల, నిర్మాణం uuid *uu)
{
	__u8	*ptr = లోపల;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->సమయం_దిగువ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->సమయం_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->సమయం_hi_and_సంస్కరణ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	జ్ఞాపకం_నకలు(uu->కణుపు, ptr, 6);
}

స్థిర శూన్య uuid_unparse(శూన్య *uu, అక్షరం *బహిర్గతం)
{
	నిర్మాణం uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(బహిర్గతం,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.సమయం_దిగువ, uuid.సమయం_mid, uuid.సమయం_hi_and_సంస్కరణ,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.కణుపు[0], uuid.కణుపు[1], uuid.కణుపు[2],
		uuid.కణుపు[3], uuid.కణుపు[4], uuid.కణుపు[5]);
}
#అయితే_ముగింపు

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_వాడని,
				   అక్షరం *tdb_dir EXT2_FS_NO_TDB_వాడని,
				   పూర్ణాంకం flags EXT2_FS_NO_TDB_వాడని,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_వాడని)
{
#నిర్వచితమైతే CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	అక్షరం 		*fn, uuid[40];
	ext2_ino_t	అంకె_inodes;
	mode_t		save_umask;
	పూర్ణాంకం		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(tdb_dir) + 64, &fn);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ఒకవేళ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		వెళ్ళు errout;
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
	అంకె_inodes = fs->super->s_inodes_లెక్క - fs->super->s_free_inodes_లెక్క;

	icount->tdb = tdb_open(fn, అంకె_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	ఒకవేళ (icount->tdb == ఖాళీ) {
		retval = errno;
		వెళ్ళు errout;
	}
	*ret = icount;
	తిరిగి 0;
errout:
	ext2fs_free_icount(icount);
	తిరిగి(retval);
#లేకపోతే
	తిరిగి EXT2_ET_UNIMPLEMENTED;
#అయితే_ముగింపు
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, పూర్ణాంకం flags, గుర్తులేని పూర్ణాంకం కొలత,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		బైట్లు;
	ext2_ino_t	i;

	ఒకవేళ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ఒకవేళ (hint->కొలత > కొలత)
			కొలత = (size_t) hint->కొలత;
	}

	retval = alloc_icount(fs, flags, &icount);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (icount->fullmap)
		వెళ్ళు successout;

	ఒకవేళ (కొలత) {
		icount->కొలత = కొలత;
	} లేకపోతే {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_అంకె_dirs(fs, &icount->కొలత);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		icount->కొలత += fs->super->s_inodes_లెక్క / 50;
	}

	బైట్లు = (size_t) (icount->కొలత * పరిమాణం(నిర్మాణం ext2_icount_el));
#ఒకవేళ 0
	రాయి("Icount allocated %u entries, %d bytes.\n",
	       icount->కొలత, బైట్లు);
#అయితే_ముగింపు
	retval = ext2fs_get_array(icount->కొలత, పరిమాణం(నిర్మాణం ext2_icount_el),
			 &icount->list);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(icount->list, 0, బైట్లు);

	icount->లెక్క = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ఒకవేళ (hint) {
		కొరకు (i=0; i < hint->లెక్క; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->లెక్క = hint->లెక్క;
	}

successout:
	*ret = icount;
	తిరిగి 0;

errout:
	ext2fs_free_icount(icount);
	తిరిగి(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, పూర్ణాంకం flags,
			       గుర్తులేని పూర్ణాంకం కొలత,
			       ext2_icount_t *ret)
{
	తిరిగి ext2fs_create_icount2(fs, flags, కొలత, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
స్థిర నిర్మాణం ext2_icount_el *చొప్పించు_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, పూర్ణాంకం pos)
{
	నిర్మాణం ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_కొలత = 0;
	పూర్ణాంకం			అంకె;

	ఒకవేళ (icount->చివరి_lookup && icount->చివరి_lookup->ino == ino)
		తిరిగి icount->చివరి_lookup;

	ఒకవేళ (icount->లెక్క >= icount->కొలత) {
		ఒకవేళ (icount->లెక్క) {
			new_కొలత = icount->list[(గుర్తులేని)icount->లెక్క-1].ino;
			new_కొలత = (ext2_ino_t) (icount->లెక్క *
				((దశాంశం) icount->అంకె_inodes / new_కొలత));
		}
		ఒకవేళ (new_కొలత < (icount->కొలత + 100))
			new_కొలత = icount->కొలత + 100;
#ఒకవేళ 0
		రాయి("Reallocating icount %u entries...\n", new_కొలత);
#అయితే_ముగింపు
		retval = ext2fs_resize_mem((size_t) icount->కొలత *
					   పరిమాణం(నిర్మాణం ext2_icount_el),
					   (size_t) new_కొలత *
					   పరిమాణం(నిర్మాణం ext2_icount_el),
					   &icount->list);
		ఒకవేళ (retval)
			తిరిగి 0;
		icount->కొలత = new_కొలత;
	}
	అంకె = (పూర్ణాంకం) icount->లెక్క - pos;
	ఒకవేళ (అంకె < 0)
		తిరిగి 0;	/* should never happen */
	ఒకవేళ (అంకె) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			పరిమాణం(నిర్మాణం ext2_icount_el) * అంకె);
	}
	icount->లెక్క++;
	el = &icount->list[pos];
	el->లెక్క = 0;
	el->ino = ino;
	icount->చివరి_lookup = el;
	తిరిగి el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
స్థిర నిర్మాణం ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, పూర్ణాంకం create)
{
	పూర్ణాంకం	దిగువ, ఎగువ, mid;

	ఒకవేళ (!icount || !icount->list)
		తిరిగి 0;

	ఒకవేళ (create && ((icount->లెక్క == 0) ||
		       (ino > icount->list[(గుర్తులేని)icount->లెక్క-1].ino))) {
		తిరిగి చొప్పించు_icount_el(icount, ino, (గుర్తులేని) icount->లెక్క);
	}
	ఒకవేళ (icount->లెక్క == 0)
		తిరిగి 0;

	ఒకవేళ (icount->cursor >= icount->లెక్క)
		icount->cursor = 0;
	ఒకవేళ (ino == icount->list[icount->cursor].ino)
		తిరిగి &icount->list[icount->cursor++];
#ఒకవేళ 0
	రాయి("Non-cursor get_icount_el: %u\n", ino);
#అయితే_ముగింపు
	దిగువ = 0;
	ఎగువ = (పూర్ణాంకం) icount->లెక్క-1;
	అయితే (దిగువ <= ఎగువ) {
		mid = ((గుర్తులేని)దిగువ + (గుర్తులేని)ఎగువ) >> 1;
		ఒకవేళ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			తిరిగి &icount->list[mid];
		}
		ఒకవేళ (ino < icount->list[mid].ino)
			ఎగువ = mid-1;
		లేకపోతే
			దిగువ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ఒకవేళ (create)
		తిరిగి చొప్పించు_icount_el(icount, ino, దిగువ);
	తిరిగి 0;
}

స్థిర errcode_t సమితి_inode_లెక్క(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 లెక్క)
{
	నిర్మాణం ext2_icount_el 	*el;
#నిర్వచితమైతే CONFIG_TDB
	TDB_దత్తాంశం కీలకం, దత్తాంశం;

	ఒకవేళ (icount->tdb) {
		కీలకం.dptr = (గుర్తులేని అక్షరం *) &ino;
		కీలకం.dsize = పరిమాణం(ext2_ino_t);
		దత్తాంశం.dptr = (గుర్తులేని అక్షరం *) &లెక్క;
		దత్తాంశం.dsize = పరిమాణం(__u32);
		ఒకవేళ (లెక్క) {
			ఒకవేళ (tdb_store(icount->tdb, కీలకం, దత్తాంశం, TDB_REPLACE))
				తిరిగి tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} లేకపోతే {
			ఒకవేళ (tdb_delete(icount->tdb, కీలకం))
				తిరిగి tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		తిరిగి 0;
	}
#అయితే_ముగింపు
	ఒకవేళ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(లెక్క);
		తిరిగి 0;
	}

	el = get_icount_el(icount, ino, 1);
	ఒకవేళ (!el)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	el->లెక్క = లెక్క;
	తిరిగి 0;
}

స్థిర errcode_t get_inode_లెక్క(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *లెక్క)
{
	నిర్మాణం ext2_icount_el 	*el;
#నిర్వచితమైతే CONFIG_TDB
	TDB_దత్తాంశం కీలకం, దత్తాంశం;

	ఒకవేళ (icount->tdb) {
		కీలకం.dptr = (గుర్తులేని అక్షరం *) &ino;
		కీలకం.dsize = పరిమాణం(ext2_ino_t);

		దత్తాంశం = tdb_fetch(icount->tdb, కీలకం);
		ఒకవేళ (దత్తాంశం.dptr == ఖాళీ) {
			*లెక్క = 0;
			తిరిగి tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*లెక్క = *((__u32 *) దత్తాంశం.dptr);
		విడుదల(దత్తాంశం.dptr);
		తిరిగి 0;
	}
#అయితే_ముగింపు
	ఒకవేళ (icount->fullmap) {
		*లెక్క = icount->fullmap[ino];
		తిరిగి 0;
	}

	el = get_icount_el(icount, ino, 0);
	ఒకవేళ (!el) {
		*లెక్క = 0;
		తిరిగి ENOENT;
	}

	*లెక్క = el->లెక్క;
	తిరిగి 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *బహిర్గతం)
{
	errcode_t	ret = 0;
	గుర్తులేని పూర్ణాంకం	i;
	స్థిరరాశి అక్షరం *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ఒకవేళ (icount->లెక్క > icount->కొలత) {
		fprintf(బహిర్గతం, "%s: count > size\n", bad);
		తిరిగి EXT2_ET_INVALID_వాదం;
	}
	కొరకు (i=1; i < icount->లెక్క; i++) {
		ఒకవేళ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(బహిర్గతం, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_వాదం;
		}
	}
	తిరిగి ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ఒకవేళ (!ino || (ino > icount->అంకె_inodes))
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (!icount->fullmap) {
		ఒకవేళ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			తిరిగి 0;
		}
		ఒకవేళ (icount->గుణిజం &&
			!ext2fs_test_inode_bitmap2(icount->గుణిజం, ino)) {
			*ret = 0;
			తిరిగి 0;
		}
	}
	get_inode_లెక్క(icount, ino, &val);
	*ret = icount_16_xlate(val);
	తిరిగి 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_విలువ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ఒకవేళ (!ino || (ino > icount->అంకె_inodes))
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (icount->fullmap) {
		curr_విలువ = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_విలువ;
	} లేకపోతే ఒకవేళ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ఒకవేళ (సమితి_inode_లెక్క(icount, ino, 2))
			తిరిగి EXT2_ET_NO_జ్ఞాపకం;
		curr_విలువ = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} లేకపోతే ఒకవేళ (icount->గుణిజం) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ఒకవేళ (ext2fs_test_inode_bitmap2(icount->గుణిజం, ino)) {
			get_inode_లెక్క(icount, ino, &curr_విలువ);
			curr_విలువ++;
			ఒకవేళ (సమితి_inode_లెక్క(icount, ino, curr_విలువ))
				తిరిగి EXT2_ET_NO_జ్ఞాపకం;
		} లేకపోతే {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ఒకవేళ (ret)
				*ret = 1;
			తిరిగి 0;
		}
	} లేకపోతే {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_లెక్క(icount, ino, &curr_విలువ);
		curr_విలువ++;
		ఒకవేళ (సమితి_inode_లెక్క(icount, ino, curr_విలువ))
			తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	}
	ఒకవేళ (icount->గుణిజం)
		ext2fs_mark_inode_bitmap2(icount->గుణిజం, ino);
	ఒకవేళ (ret)
		*ret = icount_16_xlate(curr_విలువ);
	తిరిగి 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_విలువ;

	ఒకవేళ (!ino || (ino > icount->అంకె_inodes))
		తిరిగి EXT2_ET_INVALID_వాదం;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ఒకవేళ (icount->fullmap) {
		ఒకవేళ (!icount->fullmap[ino])
			తిరిగి EXT2_ET_INVALID_వాదం;

		curr_విలువ = --icount->fullmap[ino];
		ఒకవేళ (ret)
			*ret = icount_16_xlate(curr_విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ఒకవేళ (icount->గుణిజం)
			ext2fs_unmark_inode_bitmap2(icount->గుణిజం, ino);
		లేకపోతే {
			సమితి_inode_లెక్క(icount, ino, 0);
		}
		ఒకవేళ (ret)
			*ret = 0;
		తిరిగి 0;
	}

	ఒకవేళ (icount->గుణిజం &&
	    !ext2fs_test_inode_bitmap2(icount->గుణిజం, ino))
		తిరిగి EXT2_ET_INVALID_వాదం;

	get_inode_లెక్క(icount, ino, &curr_విలువ);
	ఒకవేళ (!curr_విలువ)
		తిరిగి EXT2_ET_INVALID_వాదం;
	curr_విలువ--;
	ఒకవేళ (సమితి_inode_లెక్క(icount, ino, curr_విలువ))
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	ఒకవేళ (curr_విలువ == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ఒకవేళ ((curr_విలువ == 0) && icount->గుణిజం)
		ext2fs_unmark_inode_bitmap2(icount->గుణిజం, ino);

	ఒకవేళ (ret)
		*ret = icount_16_xlate(curr_విలువ);
	తిరిగి 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 లెక్క)
{
	ఒకవేళ (!ino || (ino > icount->అంకె_inodes))
		తిరిగి EXT2_ET_INVALID_వాదం;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ఒకవేళ (icount->fullmap)
		తిరిగి సమితి_inode_లెక్క(icount, ino, లెక్క);

	ఒకవేళ (లెక్క == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ఒకవేళ (icount->గుణిజం)
			ext2fs_unmark_inode_bitmap2(icount->గుణిజం, ino);
		తిరిగి 0;
	}
	ఒకవేళ (లెక్క == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ఒకవేళ (icount->గుణిజం) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->గుణిజం, ino);
		} లేకపోతే
			సమితి_inode_లెక్క(icount, ino, 0);
		తిరిగి 0;
	}

	ఒకవేళ (సమితి_inode_లెక్క(icount, ino, లెక్క))
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ఒకవేళ (icount->గుణిజం)
		ext2fs_mark_inode_bitmap2(icount->గుణిజం, ino);
	తిరిగి 0;
}

ext2_ino_t ext2fs_get_icount_కొలత(ext2_icount_t icount)
{
	ఒకవేళ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		తిరిగి 0;

	తిరిగి icount->కొలత;
}

#నిర్వచితమైతే DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#నిర్వచించు EXIT		0x00
#నిర్వచించు FETCH		0x01
#నిర్వచించు STORE		0x02
#నిర్వచించు INCREMENT	0x03
#నిర్వచించు DECREMENT	0x04

నిర్మాణం test_కార్యక్రమం {
	పూర్ణాంకం		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

నిర్మాణం test_కార్యక్రమం prog[] = {
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

నిర్మాణం test_కార్యక్రమం extended[] = {
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
స్థిర శూన్య అమరిక(శూన్య)
{
	errcode_t	retval;
	నిర్మాణం ext2_super_ఖండం param;

	initialize_ext2_error_పట్టిక();

	memset(&param, 0, పరిమాణం(param));
	ext2fs_ఖండాలు_లెక్క_సమితి(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_బిట్లు, &param,
				   test_io_manager, &test_fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		నిష్క్రమించు(1);
	}
}

పూర్ణాంకం నిరంతరం_test(పూర్ణాంకం flags, పూర్ణాంకం కొలత, అక్షరం *dir, నిర్మాణం test_కార్యక్రమం *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	నిర్మాణం test_కార్యక్రమం *pc;
	__u16		ఫలితం;
	పూర్ణాంకం		problem = 0;

	ఒకవేళ (dir) {
#నిర్వచితమైతే CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ఒకవేళ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			నిష్క్రమించు(1);
		}
#లేకపోతే
		రాయి("Skipped\n");
		తిరిగి 0;
#అయితే_ముగింపు
	} లేకపోతే {
		retval = ext2fs_create_icount2(test_fs, flags, కొలత, 0,
					       &icount);
		ఒకవేళ (retval) {
			com_err("run_test", retval, "while creating icount");
			నిష్క్రమించు(1);
		}
	}
	కొరకు (pc = prog; pc->cmd != EXIT; pc++) {
		ఎంపిక (pc->cmd) {
		స్థితి FETCH:
			రాయి("icount_fetch(%u) = ", pc->ino);
			ఆపు;
		స్థితి STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ఒకవేళ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				నిష్క్రమించు(1);
			}
			రాయి("icount_store(%u, %u) = ", pc->ino, pc->arg);
			ఆపు;
		స్థితి INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ఒకవేళ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				నిష్క్రమించు(1);
			}
			రాయి("icount_increment(%u) = ", pc->ino);
			ఆపు;
		స్థితి DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ఒకవేళ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				నిష్క్రమించు(1);
			}
			రాయి("icount_decrement(%u) = ", pc->ino);
			ఆపు;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &ఫలితం);
		ఒకవేళ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			నిష్క్రమించు(1);
		}
		రాయి("%u (%s)\n", ఫలితం, (ఫలితం == pc->expected) ?
		       "OK" : "NOT OK");
		ఒకవేళ (ఫలితం != pc->expected)
			problem++;
	}
	రాయి("icount size is %u\n", ext2fs_get_icount_కొలత(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ఒకవేళ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		నిష్క్రమించు(1);
	}
	ext2fs_free_icount(icount);
	తిరిగి problem;
}


పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం failed = 0;

	అమరిక();
	రాయి("Standard icount run:\n");
	failed += నిరంతరం_test(0, 0, 0, prog);
	రాయి("\nMultiple bitmap test:\n");
	failed += నిరంతరం_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	రాయి("\nResizing icount:\n");
	failed += నిరంతరం_test(0, 3, 0, extended);
	రాయి("\nStandard icount run with tdb:\n");
	failed += నిరంతరం_test(0, 0, ".", prog);
	రాయి("\nMultiple bitmap test with tdb:\n");
	failed += నిరంతరం_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	ఒకవేళ (failed)
		రాయి("FAILED!\n");
	తిరిగి failed;
}
#అయితే_ముగింపు
