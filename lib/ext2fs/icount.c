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

#ضمن "config.h"
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <string.h>
#ضمن <stdio.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "tdb.h"

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

بنية ext2_icount_el {
	ext2_ino_t	ino;
	__u32		شمېر;
};

بنية ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	مضرب;
	ext2_ino_t		شمېر;
	ext2_ino_t		اندازه;
	ext2_ino_t		عدد_inodes;
	ext2_ino_t		cursor;
	بنية ext2_icount_el	*list;
	بنية ext2_icount_el	*وروستی_lookup;
#که_تعریف_شوی CONFIG_TDB
	حرف			*tdb_fn;
	TDB_CONTEXT		*tdb;
#د_که_پای
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#تعریف_کړه icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

فراغ ext2fs_free_icount(ext2_icount_t icount)
{
	إذا (!icount)
		أرجع;

	icount->magic = 0;
	إذا (icount->list)
		ext2fs_free_mem(&icount->list);
	إذا (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	إذا (icount->مضرب)
		ext2fs_free_inode_bitmap(icount->مضرب);
#که_تعریف_شوی CONFIG_TDB
	إذا (icount->tdb)
		tdb_close(icount->tdb);
	إذا (icount->tdb_fn) {
		(فراغ) unlink(icount->tdb_fn);
		حرر(icount->tdb_fn);
	}
#د_که_پای

	إذا (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ساكن errcode_t alloc_icount(ext2_filsys fs, صحيح flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(حجم(بنية ext2_icount), &icount);
	إذا (retval)
		أرجع retval;
	memset(icount, 0, حجم(بنية ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->عدد_inodes = fs->super->s_inodes_شمېر;

	إذا ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		بینښه sz = حجم(*icount->fullmap) * icount->عدد_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		إذا (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			أرجع 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	إذا (retval)
		اذهب errout;

	إذا (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->مضرب);
		إذا (retval)
			اذهب errout;
	} وإلا
		icount->مضرب = 0;

	*ret = icount;
	أرجع 0;

errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
}

#که_تعریف_شوی CONFIG_TDB
بنية uuid {
	__u32	وخت_ټیټ;
	__u16	وخت_mid;
	__u16	وخت_hi_and_نسخه;
	__u16	clock_seq;
	__u8	غوټه[6];
};

ساكن فراغ unpack_uuid(فراغ *دننه, بنية uuid *uu)
{
	__u8	*ptr = دننه;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وخت_ټیټ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وخت_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وخت_hi_and_نسخه = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	حافظه_نقل(uu->غوټه, ptr, 6);
}

ساكن فراغ uuid_unparse(فراغ *uu, حرف *بهر)
{
	بنية uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(بهر,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.وخت_ټیټ, uuid.وخت_mid, uuid.وخت_hi_and_نسخه,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.غوټه[0], uuid.غوټه[1], uuid.غوټه[2],
		uuid.غوټه[3], uuid.غوټه[4], uuid.غوټه[5]);
}
#د_که_پای

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_ناکارول_شوی,
				   حرف *tdb_dir EXT2_FS_NO_TDB_ناکارول_شوی,
				   صحيح flags EXT2_FS_NO_TDB_ناکارول_شوی,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_ناکارول_شوی)
{
#که_تعریف_شوی CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	حرف 		*fn, uuid[40];
	ext2_ino_t	عدد_inodes;
	mode_t		save_umask;
	صحيح		fd;

	retval = alloc_icount(fs, flags,  &icount);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(مزي_اوږدوالی(tdb_dir) + 64, &fn);
	إذا (retval)
		اذهب errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	إذا (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		اذهب errout;
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
	عدد_inodes = fs->super->s_inodes_شمېر - fs->super->s_free_inodes_شمېر;

	icount->tdb = tdb_open(fn, عدد_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	إذا (icount->tdb == عدم) {
		retval = errno;
		اذهب errout;
	}
	*ret = icount;
	أرجع 0;
errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
#وإلا
	أرجع EXT2_ET_UNIMPLEMENTED;
#د_که_پای
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags, بینښه صحيح اندازه,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		بایټونه;
	ext2_ino_t	i;

	إذا (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		إذا (hint->اندازه > اندازه)
			اندازه = (size_t) hint->اندازه;
	}

	retval = alloc_icount(fs, flags, &icount);
	إذا (retval)
		أرجع retval;

	إذا (icount->fullmap)
		اذهب successout;

	إذا (اندازه) {
		icount->اندازه = اندازه;
	} وإلا {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_عدد_dirs(fs, &icount->اندازه);
		إذا (retval)
			اذهب errout;
		icount->اندازه += fs->super->s_inodes_شمېر / 50;
	}

	بایټونه = (size_t) (icount->اندازه * حجم(بنية ext2_icount_el));
#إذا 0
	چاپ_کړه("Icount allocated %u entries, %d bytes.\n",
	       icount->اندازه, بایټونه);
#د_که_پای
	retval = ext2fs_get_array(icount->اندازه, حجم(بنية ext2_icount_el),
			 &icount->list);
	إذا (retval)
		اذهب errout;
	memset(icount->list, 0, بایټونه);

	icount->شمېر = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	إذا (hint) {
		لكل (i=0; i < hint->شمېر; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->شمېر = hint->شمېر;
	}

successout:
	*ret = icount;
	أرجع 0;

errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
			       بینښه صحيح اندازه,
			       ext2_icount_t *ret)
{
	أرجع ext2fs_create_icount2(fs, flags, اندازه, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ساكن بنية ext2_icount_el *ورننباسه_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحيح pos)
{
	بنية ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_اندازه = 0;
	صحيح			عدد;

	إذا (icount->وروستی_lookup && icount->وروستی_lookup->ino == ino)
		أرجع icount->وروستی_lookup;

	إذا (icount->شمېر >= icount->اندازه) {
		إذا (icount->شمېر) {
			new_اندازه = icount->list[(بینښه)icount->شمېر-1].ino;
			new_اندازه = (ext2_ino_t) (icount->شمېر *
				((عائم) icount->عدد_inodes / new_اندازه));
		}
		إذا (new_اندازه < (icount->اندازه + 100))
			new_اندازه = icount->اندازه + 100;
#إذا 0
		چاپ_کړه("Reallocating icount %u entries...\n", new_اندازه);
#د_که_پای
		retval = ext2fs_resize_mem((size_t) icount->اندازه *
					   حجم(بنية ext2_icount_el),
					   (size_t) new_اندازه *
					   حجم(بنية ext2_icount_el),
					   &icount->list);
		إذا (retval)
			أرجع 0;
		icount->اندازه = new_اندازه;
	}
	عدد = (صحيح) icount->شمېر - pos;
	إذا (عدد < 0)
		أرجع 0;	/* should never happen */
	إذا (عدد) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			حجم(بنية ext2_icount_el) * عدد);
	}
	icount->شمېر++;
	el = &icount->list[pos];
	el->شمېر = 0;
	el->ino = ino;
	icount->وروستی_lookup = el;
	أرجع el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ساكن بنية ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحيح create)
{
	صحيح	ټیټ, لوړ, mid;

	إذا (!icount || !icount->list)
		أرجع 0;

	إذا (create && ((icount->شمېر == 0) ||
		       (ino > icount->list[(بینښه)icount->شمېر-1].ino))) {
		أرجع ورننباسه_icount_el(icount, ino, (بینښه) icount->شمېر);
	}
	إذا (icount->شمېر == 0)
		أرجع 0;

	إذا (icount->cursor >= icount->شمېر)
		icount->cursor = 0;
	إذا (ino == icount->list[icount->cursor].ino)
		أرجع &icount->list[icount->cursor++];
#إذا 0
	چاپ_کړه("Non-cursor get_icount_el: %u\n", ino);
#د_که_پای
	ټیټ = 0;
	لوړ = (صحيح) icount->شمېر-1;
	طالما (ټیټ <= لوړ) {
		mid = ((بینښه)ټیټ + (بینښه)لوړ) >> 1;
		إذا (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			أرجع &icount->list[mid];
		}
		إذا (ino < icount->list[mid].ino)
			لوړ = mid-1;
		وإلا
			ټیټ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	إذا (create)
		أرجع ورننباسه_icount_el(icount, ino, ټیټ);
	أرجع 0;
}

ساكن errcode_t سېټ_inode_شمېر(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 شمېر)
{
	بنية ext2_icount_el 	*el;
#که_تعریف_شوی CONFIG_TDB
	TDB_معلومات کلی, معلومات;

	إذا (icount->tdb) {
		کلی.dptr = (بینښه حرف *) &ino;
		کلی.dsize = حجم(ext2_ino_t);
		معلومات.dptr = (بینښه حرف *) &شمېر;
		معلومات.dsize = حجم(__u32);
		إذا (شمېر) {
			إذا (tdb_store(icount->tdb, کلی, معلومات, TDB_REPLACE))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} وإلا {
			إذا (tdb_delete(icount->tdb, کلی))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		أرجع 0;
	}
#د_که_پای
	إذا (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(شمېر);
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 1);
	إذا (!el)
		أرجع EXT2_ET_NO_حافظه;

	el->شمېر = شمېر;
	أرجع 0;
}

ساكن errcode_t get_inode_شمېر(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *شمېر)
{
	بنية ext2_icount_el 	*el;
#که_تعریف_شوی CONFIG_TDB
	TDB_معلومات کلی, معلومات;

	إذا (icount->tdb) {
		کلی.dptr = (بینښه حرف *) &ino;
		کلی.dsize = حجم(ext2_ino_t);

		معلومات = tdb_fetch(icount->tdb, کلی);
		إذا (معلومات.dptr == عدم) {
			*شمېر = 0;
			أرجع tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*شمېر = *((__u32 *) معلومات.dptr);
		حرر(معلومات.dptr);
		أرجع 0;
	}
#د_که_پای
	إذا (icount->fullmap) {
		*شمېر = icount->fullmap[ino];
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 0);
	إذا (!el) {
		*شمېر = 0;
		أرجع ENOENT;
	}

	*شمېر = el->شمېر;
	أرجع 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *بهر)
{
	errcode_t	ret = 0;
	بینښه صحيح	i;
	ثابت حرف *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->شمېر > icount->اندازه) {
		fprintf(بهر, "%s: count > size\n", bad);
		أرجع EXT2_ET_INVALID_دلیل;
	}
	لكل (i=1; i < icount->شمېر; i++) {
		إذا (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(بهر, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_دلیل;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (!icount->fullmap) {
		إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			أرجع 0;
		}
		إذا (icount->مضرب &&
			!ext2fs_test_inode_bitmap2(icount->مضرب, ino)) {
			*ret = 0;
			أرجع 0;
		}
	}
	get_inode_شمېر(icount, ino, &val);
	*ret = icount_16_xlate(val);
	أرجع 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ارزښت;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (icount->fullmap) {
		curr_ارزښت = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_ارزښت;
	} وإلا إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		إذا (سېټ_inode_شمېر(icount, ino, 2))
			أرجع EXT2_ET_NO_حافظه;
		curr_ارزښت = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} وإلا إذا (icount->مضرب) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		إذا (ext2fs_test_inode_bitmap2(icount->مضرب, ino)) {
			get_inode_شمېر(icount, ino, &curr_ارزښت);
			curr_ارزښت++;
			إذا (سېټ_inode_شمېر(icount, ino, curr_ارزښت))
				أرجع EXT2_ET_NO_حافظه;
		} وإلا {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			إذا (ret)
				*ret = 1;
			أرجع 0;
		}
	} وإلا {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_شمېر(icount, ino, &curr_ارزښت);
		curr_ارزښت++;
		إذا (سېټ_inode_شمېر(icount, ino, curr_ارزښت))
			أرجع EXT2_ET_NO_حافظه;
	}
	إذا (icount->مضرب)
		ext2fs_mark_inode_bitmap2(icount->مضرب, ino);
	إذا (ret)
		*ret = icount_16_xlate(curr_ارزښت);
	أرجع 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ارزښت;

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_دلیل;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap) {
		إذا (!icount->fullmap[ino])
			أرجع EXT2_ET_INVALID_دلیل;

		curr_ارزښت = --icount->fullmap[ino];
		إذا (ret)
			*ret = icount_16_xlate(curr_ارزښت);
		أرجع 0;
	}

	إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		وإلا {
			سېټ_inode_شمېر(icount, ino, 0);
		}
		إذا (ret)
			*ret = 0;
		أرجع 0;
	}

	إذا (icount->مضرب &&
	    !ext2fs_test_inode_bitmap2(icount->مضرب, ino))
		أرجع EXT2_ET_INVALID_دلیل;

	get_inode_شمېر(icount, ino, &curr_ارزښت);
	إذا (!curr_ارزښت)
		أرجع EXT2_ET_INVALID_دلیل;
	curr_ارزښت--;
	إذا (سېټ_inode_شمېر(icount, ino, curr_ارزښت))
		أرجع EXT2_ET_NO_حافظه;

	إذا (curr_ارزښت == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	إذا ((curr_ارزښت == 0) && icount->مضرب)
		ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);

	إذا (ret)
		*ret = icount_16_xlate(curr_ارزښت);
	أرجع 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 شمېر)
{
	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_دلیل;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap)
		أرجع سېټ_inode_شمېر(icount, ino, شمېر);

	إذا (شمېر == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		أرجع 0;
	}
	إذا (شمېر == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		} وإلا
			سېټ_inode_شمېر(icount, ino, 0);
		أرجع 0;
	}

	إذا (سېټ_inode_شمېر(icount, ino, شمېر))
		أرجع EXT2_ET_NO_حافظه;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	إذا (icount->مضرب)
		ext2fs_mark_inode_bitmap2(icount->مضرب, ino);
	أرجع 0;
}

ext2_ino_t ext2fs_get_icount_اندازه(ext2_icount_t icount)
{
	إذا (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		أرجع 0;

	أرجع icount->اندازه;
}

#که_تعریف_شوی DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#تعریف_کړه EXIT		0x00
#تعریف_کړه FETCH		0x01
#تعریف_کړه STORE		0x02
#تعریف_کړه INCREMENT	0x03
#تعریف_کړه DECREMENT	0x04

بنية test_پروګرام {
	صحيح		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

بنية test_پروګرام prog[] = {
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

بنية test_پروګرام extended[] = {
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
ساكن فراغ تیاری(فراغ)
{
	errcode_t	retval;
	بنية ext2_super_بلاک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکونه_شمېر_سېټ(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بټونه, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		وتل(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		وتل(1);
	}
}

صحيح پرلپسې_test(صحيح flags, صحيح اندازه, حرف *dir, بنية test_پروګرام *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	بنية test_پروګرام *pc;
	__u16		پایله;
	صحيح		problem = 0;

	إذا (dir) {
#که_تعریف_شوی CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		إذا (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			وتل(1);
		}
#وإلا
		چاپ_کړه("Skipped\n");
		أرجع 0;
#د_که_پای
	} وإلا {
		retval = ext2fs_create_icount2(test_fs, flags, اندازه, 0,
					       &icount);
		إذا (retval) {
			com_err("run_test", retval, "while creating icount");
			وتل(1);
		}
	}
	لكل (pc = prog; pc->cmd != EXIT; pc++) {
		بدل (pc->cmd) {
		حالة FETCH:
			چاپ_کړه("icount_fetch(%u) = ", pc->ino);
			اكسر;
		حالة STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				وتل(1);
			}
			چاپ_کړه("icount_store(%u, %u) = ", pc->ino, pc->arg);
			اكسر;
		حالة INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				وتل(1);
			}
			چاپ_کړه("icount_increment(%u) = ", pc->ino);
			اكسر;
		حالة DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				وتل(1);
			}
			چاپ_کړه("icount_decrement(%u) = ", pc->ino);
			اكسر;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &پایله);
		إذا (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			وتل(1);
		}
		چاپ_کړه("%u (%s)\n", پایله, (پایله == pc->expected) ?
		       "OK" : "NOT OK");
		إذا (پایله != pc->expected)
			problem++;
	}
	چاپ_کړه("icount size is %u\n", ext2fs_get_icount_اندازه(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	إذا (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		وتل(1);
	}
	ext2fs_free_icount(icount);
	أرجع problem;
}


صحيح اصلي(صحيح argc, حرف **argv)
{
	صحيح failed = 0;

	تیاری();
	چاپ_کړه("Standard icount run:\n");
	failed += پرلپسې_test(0, 0, 0, prog);
	چاپ_کړه("\nMultiple bitmap test:\n");
	failed += پرلپسې_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	چاپ_کړه("\nResizing icount:\n");
	failed += پرلپسې_test(0, 3, 0, extended);
	چاپ_کړه("\nStandard icount run with tdb:\n");
	failed += پرلپسې_test(0, 0, ".", prog);
	چاپ_کړه("\nMultiple bitmap test with tdb:\n");
	failed += پرلپسې_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	إذا (failed)
		چاپ_کړه("FAILED!\n");
	أرجع failed;
}
#د_که_پای
