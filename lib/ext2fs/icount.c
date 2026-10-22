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
#پایان_اگر
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
	__u32		شمار;
};

بنية ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	مضرب;
	ext2_ino_t		شمار;
	ext2_ino_t		اندازه;
	ext2_ino_t		رقم_inodes;
	ext2_ino_t		cursor;
	بنية ext2_icount_el	*list;
	بنية ext2_icount_el	*آخرین_lookup;
#اگر_تعریف_شده CONFIG_TDB
	حرف			*tdb_fn;
	TDB_CONTEXT		*tdb;
#پایان_اگر
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#تعریف_کن icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

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
#اگر_تعریف_شده CONFIG_TDB
	إذا (icount->tdb)
		tdb_close(icount->tdb);
	إذا (icount->tdb_fn) {
		(فراغ) unlink(icount->tdb_fn);
		حرر(icount->tdb_fn);
	}
#پایان_اگر

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
	icount->رقم_inodes = fs->super->s_inodes_شمار;

	إذا ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		بیعلامت sz = حجم(*icount->fullmap) * icount->رقم_inodes;

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

#اگر_تعریف_شده CONFIG_TDB
بنية uuid {
	__u32	زمان_کم;
	__u16	زمان_mid;
	__u16	زمان_hi_and_نسخه;
	__u16	clock_seq;
	__u8	گره[6];
};

ساكن فراغ unpack_uuid(فراغ *درون, بنية uuid *uu)
{
	__u8	*ptr = درون;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->زمان_کم = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->زمان_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->زمان_hi_and_نسخه = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	کپی_حافظه(uu->گره, ptr, 6);
}

ساكن فراغ uuid_unparse(فراغ *uu, حرف *بیرون)
{
	بنية uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(بیرون,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.زمان_کم, uuid.زمان_mid, uuid.زمان_hi_and_نسخه,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.گره[0], uuid.گره[1], uuid.گره[2],
		uuid.گره[3], uuid.گره[4], uuid.گره[5]);
}
#پایان_اگر

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_استفادهنشده,
				   حرف *tdb_dir EXT2_FS_NO_TDB_استفادهنشده,
				   صحيح flags EXT2_FS_NO_TDB_استفادهنشده,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_استفادهنشده)
{
#اگر_تعریف_شده CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	حرف 		*fn, uuid[40];
	ext2_ino_t	رقم_inodes;
	mode_t		save_umask;
	صحيح		fd;

	retval = alloc_icount(fs, flags,  &icount);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(طول_رشته(tdb_dir) + 64, &fn);
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
	رقم_inodes = fs->super->s_inodes_شمار - fs->super->s_free_inodes_شمار;

	icount->tdb = tdb_open(fn, رقم_inodes, TDB_NOLOCK | TDB_NOSYNC,
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
#پایان_اگر
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags, بیعلامت صحيح اندازه,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		بایتها;
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
		retval = ext2fs_get_رقم_dirs(fs, &icount->اندازه);
		إذا (retval)
			اذهب errout;
		icount->اندازه += fs->super->s_inodes_شمار / 50;
	}

	بایتها = (size_t) (icount->اندازه * حجم(بنية ext2_icount_el));
#إذا 0
	چاپ_کن("Icount allocated %u entries, %d bytes.\n",
	       icount->اندازه, بایتها);
#پایان_اگر
	retval = ext2fs_get_array(icount->اندازه, حجم(بنية ext2_icount_el),
			 &icount->list);
	إذا (retval)
		اذهب errout;
	memset(icount->list, 0, بایتها);

	icount->شمار = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	إذا (hint) {
		لكل (i=0; i < hint->شمار; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->شمار = hint->شمار;
	}

successout:
	*ret = icount;
	أرجع 0;

errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
			       بیعلامت صحيح اندازه,
			       ext2_icount_t *ret)
{
	أرجع ext2fs_create_icount2(fs, flags, اندازه, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ساكن بنية ext2_icount_el *درج_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحيح pos)
{
	بنية ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_اندازه = 0;
	صحيح			رقم;

	إذا (icount->آخرین_lookup && icount->آخرین_lookup->ino == ino)
		أرجع icount->آخرین_lookup;

	إذا (icount->شمار >= icount->اندازه) {
		إذا (icount->شمار) {
			new_اندازه = icount->list[(بیعلامت)icount->شمار-1].ino;
			new_اندازه = (ext2_ino_t) (icount->شمار *
				((عائم) icount->رقم_inodes / new_اندازه));
		}
		إذا (new_اندازه < (icount->اندازه + 100))
			new_اندازه = icount->اندازه + 100;
#إذا 0
		چاپ_کن("Reallocating icount %u entries...\n", new_اندازه);
#پایان_اگر
		retval = ext2fs_resize_mem((size_t) icount->اندازه *
					   حجم(بنية ext2_icount_el),
					   (size_t) new_اندازه *
					   حجم(بنية ext2_icount_el),
					   &icount->list);
		إذا (retval)
			أرجع 0;
		icount->اندازه = new_اندازه;
	}
	رقم = (صحيح) icount->شمار - pos;
	إذا (رقم < 0)
		أرجع 0;	/* should never happen */
	إذا (رقم) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			حجم(بنية ext2_icount_el) * رقم);
	}
	icount->شمار++;
	el = &icount->list[pos];
	el->شمار = 0;
	el->ino = ino;
	icount->آخرین_lookup = el;
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
	صحيح	کم, بالا, mid;

	إذا (!icount || !icount->list)
		أرجع 0;

	إذا (create && ((icount->شمار == 0) ||
		       (ino > icount->list[(بیعلامت)icount->شمار-1].ino))) {
		أرجع درج_icount_el(icount, ino, (بیعلامت) icount->شمار);
	}
	إذا (icount->شمار == 0)
		أرجع 0;

	إذا (icount->cursor >= icount->شمار)
		icount->cursor = 0;
	إذا (ino == icount->list[icount->cursor].ino)
		أرجع &icount->list[icount->cursor++];
#إذا 0
	چاپ_کن("Non-cursor get_icount_el: %u\n", ino);
#پایان_اگر
	کم = 0;
	بالا = (صحيح) icount->شمار-1;
	طالما (کم <= بالا) {
		mid = ((بیعلامت)کم + (بیعلامت)بالا) >> 1;
		إذا (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			أرجع &icount->list[mid];
		}
		إذا (ino < icount->list[mid].ino)
			بالا = mid-1;
		وإلا
			کم = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	إذا (create)
		أرجع درج_icount_el(icount, ino, کم);
	أرجع 0;
}

ساكن errcode_t مجموعه_inode_شمار(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 شمار)
{
	بنية ext2_icount_el 	*el;
#اگر_تعریف_شده CONFIG_TDB
	TDB_داده کلید, داده;

	إذا (icount->tdb) {
		کلید.dptr = (بیعلامت حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);
		داده.dptr = (بیعلامت حرف *) &شمار;
		داده.dsize = حجم(__u32);
		إذا (شمار) {
			إذا (tdb_store(icount->tdb, کلید, داده, TDB_REPLACE))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} وإلا {
			إذا (tdb_delete(icount->tdb, کلید))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		أرجع 0;
	}
#پایان_اگر
	إذا (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(شمار);
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 1);
	إذا (!el)
		أرجع EXT2_ET_NO_حافظه;

	el->شمار = شمار;
	أرجع 0;
}

ساكن errcode_t get_inode_شمار(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *شمار)
{
	بنية ext2_icount_el 	*el;
#اگر_تعریف_شده CONFIG_TDB
	TDB_داده کلید, داده;

	إذا (icount->tdb) {
		کلید.dptr = (بیعلامت حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);

		داده = tdb_fetch(icount->tdb, کلید);
		إذا (داده.dptr == عدم) {
			*شمار = 0;
			أرجع tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*شمار = *((__u32 *) داده.dptr);
		حرر(داده.dptr);
		أرجع 0;
	}
#پایان_اگر
	إذا (icount->fullmap) {
		*شمار = icount->fullmap[ino];
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 0);
	إذا (!el) {
		*شمار = 0;
		أرجع ENOENT;
	}

	*شمار = el->شمار;
	أرجع 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *بیرون)
{
	errcode_t	ret = 0;
	بیعلامت صحيح	i;
	ثابت حرف *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->شمار > icount->اندازه) {
		fprintf(بیرون, "%s: count > size\n", bad);
		أرجع EXT2_ET_INVALID_آرگومان;
	}
	لكل (i=1; i < icount->شمار; i++) {
		إذا (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(بیرون, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_آرگومان;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->رقم_inodes))
		أرجع EXT2_ET_INVALID_آرگومان;

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
	get_inode_شمار(icount, ino, &val);
	*ret = icount_16_xlate(val);
	أرجع 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_مقدار;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->رقم_inodes))
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (icount->fullmap) {
		curr_مقدار = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_مقدار;
	} وإلا إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		إذا (مجموعه_inode_شمار(icount, ino, 2))
			أرجع EXT2_ET_NO_حافظه;
		curr_مقدار = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} وإلا إذا (icount->مضرب) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		إذا (ext2fs_test_inode_bitmap2(icount->مضرب, ino)) {
			get_inode_شمار(icount, ino, &curr_مقدار);
			curr_مقدار++;
			إذا (مجموعه_inode_شمار(icount, ino, curr_مقدار))
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
		get_inode_شمار(icount, ino, &curr_مقدار);
		curr_مقدار++;
		إذا (مجموعه_inode_شمار(icount, ino, curr_مقدار))
			أرجع EXT2_ET_NO_حافظه;
	}
	إذا (icount->مضرب)
		ext2fs_mark_inode_bitmap2(icount->مضرب, ino);
	إذا (ret)
		*ret = icount_16_xlate(curr_مقدار);
	أرجع 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_مقدار;

	إذا (!ino || (ino > icount->رقم_inodes))
		أرجع EXT2_ET_INVALID_آرگومان;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap) {
		إذا (!icount->fullmap[ino])
			أرجع EXT2_ET_INVALID_آرگومان;

		curr_مقدار = --icount->fullmap[ino];
		إذا (ret)
			*ret = icount_16_xlate(curr_مقدار);
		أرجع 0;
	}

	إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		وإلا {
			مجموعه_inode_شمار(icount, ino, 0);
		}
		إذا (ret)
			*ret = 0;
		أرجع 0;
	}

	إذا (icount->مضرب &&
	    !ext2fs_test_inode_bitmap2(icount->مضرب, ino))
		أرجع EXT2_ET_INVALID_آرگومان;

	get_inode_شمار(icount, ino, &curr_مقدار);
	إذا (!curr_مقدار)
		أرجع EXT2_ET_INVALID_آرگومان;
	curr_مقدار--;
	إذا (مجموعه_inode_شمار(icount, ino, curr_مقدار))
		أرجع EXT2_ET_NO_حافظه;

	إذا (curr_مقدار == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	إذا ((curr_مقدار == 0) && icount->مضرب)
		ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);

	إذا (ret)
		*ret = icount_16_xlate(curr_مقدار);
	أرجع 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 شمار)
{
	إذا (!ino || (ino > icount->رقم_inodes))
		أرجع EXT2_ET_INVALID_آرگومان;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap)
		أرجع مجموعه_inode_شمار(icount, ino, شمار);

	إذا (شمار == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		أرجع 0;
	}
	إذا (شمار == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->مضرب) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		} وإلا
			مجموعه_inode_شمار(icount, ino, 0);
		أرجع 0;
	}

	إذا (مجموعه_inode_شمار(icount, ino, شمار))
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

#اگر_تعریف_شده DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#تعریف_کن EXIT		0x00
#تعریف_کن FETCH		0x01
#تعریف_کن STORE		0x02
#تعریف_کن INCREMENT	0x03
#تعریف_کن DECREMENT	0x04

بنية test_برنامه {
	صحيح		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

بنية test_برنامه prog[] = {
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

بنية test_برنامه extended[] = {
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
ساكن فراغ راهاندازی(فراغ)
{
	errcode_t	retval;
	بنية ext2_super_بلوک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلوکها_شمار_مجموعه(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بیتها, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		خروج(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		خروج(1);
	}
}

صحيح پیاپی_test(صحيح flags, صحيح اندازه, حرف *dir, بنية test_برنامه *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	بنية test_برنامه *pc;
	__u16		نتیجه;
	صحيح		problem = 0;

	إذا (dir) {
#اگر_تعریف_شده CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		إذا (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			خروج(1);
		}
#وإلا
		چاپ_کن("Skipped\n");
		أرجع 0;
#پایان_اگر
	} وإلا {
		retval = ext2fs_create_icount2(test_fs, flags, اندازه, 0,
					       &icount);
		إذا (retval) {
			com_err("run_test", retval, "while creating icount");
			خروج(1);
		}
	}
	لكل (pc = prog; pc->cmd != EXIT; pc++) {
		بدل (pc->cmd) {
		حالة FETCH:
			چاپ_کن("icount_fetch(%u) = ", pc->ino);
			اكسر;
		حالة STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				خروج(1);
			}
			چاپ_کن("icount_store(%u, %u) = ", pc->ino, pc->arg);
			اكسر;
		حالة INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				خروج(1);
			}
			چاپ_کن("icount_increment(%u) = ", pc->ino);
			اكسر;
		حالة DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				خروج(1);
			}
			چاپ_کن("icount_decrement(%u) = ", pc->ino);
			اكسر;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &نتیجه);
		إذا (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			خروج(1);
		}
		چاپ_کن("%u (%s)\n", نتیجه, (نتیجه == pc->expected) ?
		       "OK" : "NOT OK");
		إذا (نتیجه != pc->expected)
			problem++;
	}
	چاپ_کن("icount size is %u\n", ext2fs_get_icount_اندازه(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	إذا (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		خروج(1);
	}
	ext2fs_free_icount(icount);
	أرجع problem;
}


صحيح اصلی(صحيح argc, حرف **argv)
{
	صحيح failed = 0;

	راهاندازی();
	چاپ_کن("Standard icount run:\n");
	failed += پیاپی_test(0, 0, 0, prog);
	چاپ_کن("\nMultiple bitmap test:\n");
	failed += پیاپی_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	چاپ_کن("\nResizing icount:\n");
	failed += پیاپی_test(0, 3, 0, extended);
	چاپ_کن("\nStandard icount run with tdb:\n");
	failed += پیاپی_test(0, 0, ".", prog);
	چاپ_کن("\nMultiple bitmap test with tdb:\n");
	failed += پیاپی_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	إذا (failed)
		چاپ_کن("FAILED!\n");
	أرجع failed;
}
#پایان_اگر
