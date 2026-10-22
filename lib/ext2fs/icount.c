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

#شامل "config.h"
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <string.h>
#شامل <stdio.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "tdb.h"

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

ساخت ext2_icount_el {
	ext2_ino_t	ino;
	__u32		گنتی;
};

ساخت ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	مضرب;
	ext2_ino_t		گنتی;
	ext2_ino_t		سائز;
	ext2_ino_t		عدد_inodes;
	ext2_ino_t		cursor;
	ساخت ext2_icount_el	*list;
	ساخت ext2_icount_el	*آخری_lookup;
#اگر_تعریف_شدہ CONFIG_TDB
	حرف			*tdb_fn;
	TDB_CONTEXT		*tdb;
#اگر_ختم
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#تعریف_کرو icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

خالی ext2fs_free_icount(ext2_icount_t icount)
{
	اگر (!icount)
		واپس;

	icount->magic = 0;
	اگر (icount->list)
		ext2fs_free_mem(&icount->list);
	اگر (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	اگر (icount->مضرب)
		ext2fs_free_inode_bitmap(icount->مضرب);
#اگر_تعریف_شدہ CONFIG_TDB
	اگر (icount->tdb)
		tdb_close(icount->tdb);
	اگر (icount->tdb_fn) {
		(خالی) unlink(icount->tdb_fn);
		آزاد(icount->tdb_fn);
	}
#اگر_ختم

	اگر (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ساکن errcode_t alloc_icount(ext2_filsys fs, صحیح flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(حجم(ساخت ext2_icount), &icount);
	اگر (retval)
		واپس retval;
	memset(icount, 0, حجم(ساخت ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->عدد_inodes = fs->super->s_inodes_گنتی;

	اگر ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		غیرعلامتی sz = حجم(*icount->fullmap) * icount->عدد_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		اگر (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			واپس 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	اگر (retval)
		جاؤ errout;

	اگر (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->مضرب);
		اگر (retval)
			جاؤ errout;
	} ورنہ
		icount->مضرب = 0;

	*ret = icount;
	واپس 0;

errout:
	ext2fs_free_icount(icount);
	واپس(retval);
}

#اگر_تعریف_شدہ CONFIG_TDB
ساخت uuid {
	__u32	وقت_کم;
	__u16	وقت_mid;
	__u16	وقت_hi_and_نسخہ;
	__u16	clock_seq;
	__u8	گرہ[6];
};

ساکن خالی unpack_uuid(خالی *اندر, ساخت uuid *uu)
{
	__u8	*ptr = اندر;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وقت_کم = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وقت_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->وقت_hi_and_نسخہ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	حافظہ_نقل(uu->گرہ, ptr, 6);
}

ساکن خالی uuid_unparse(خالی *uu, حرف *برآمد)
{
	ساخت uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(برآمد,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.وقت_کم, uuid.وقت_mid, uuid.وقت_hi_and_نسخہ,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.گرہ[0], uuid.گرہ[1], uuid.گرہ[2],
		uuid.گرہ[3], uuid.گرہ[4], uuid.گرہ[5]);
}
#اگر_ختم

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_غیر_مستعمل,
				   حرف *tdb_dir EXT2_FS_NO_TDB_غیر_مستعمل,
				   صحیح flags EXT2_FS_NO_TDB_غیر_مستعمل,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_غیر_مستعمل)
{
#اگر_تعریف_شدہ CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	حرف 		*fn, uuid[40];
	ext2_ino_t	عدد_inodes;
	mode_t		save_umask;
	صحیح		fd;

	retval = alloc_icount(fs, flags,  &icount);
	اگر (retval)
		واپس retval;

	retval = ext2fs_get_mem(لڑی_لمبائی(tdb_dir) + 64, &fn);
	اگر (retval)
		جاؤ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	اگر (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		جاؤ errout;
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
	عدد_inodes = fs->super->s_inodes_گنتی - fs->super->s_free_inodes_گنتی;

	icount->tdb = tdb_open(fn, عدد_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	اگر (icount->tdb == تہی) {
		retval = errno;
		جاؤ errout;
	}
	*ret = icount;
	واپس 0;
errout:
	ext2fs_free_icount(icount);
	واپس(retval);
#ورنہ
	واپس EXT2_ET_UNIMPLEMENTED;
#اگر_ختم
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, صحیح flags, غیرعلامتی صحیح سائز,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		بائٹیں;
	ext2_ino_t	i;

	اگر (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		اگر (hint->سائز > سائز)
			سائز = (size_t) hint->سائز;
	}

	retval = alloc_icount(fs, flags, &icount);
	اگر (retval)
		واپس retval;

	اگر (icount->fullmap)
		جاؤ successout;

	اگر (سائز) {
		icount->سائز = سائز;
	} ورنہ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_عدد_dirs(fs, &icount->سائز);
		اگر (retval)
			جاؤ errout;
		icount->سائز += fs->super->s_inodes_گنتی / 50;
	}

	بائٹیں = (size_t) (icount->سائز * حجم(ساخت ext2_icount_el));
#اگر 0
	لکھو("Icount allocated %u entries, %d bytes.\n",
	       icount->سائز, بائٹیں);
#اگر_ختم
	retval = ext2fs_get_array(icount->سائز, حجم(ساخت ext2_icount_el),
			 &icount->list);
	اگر (retval)
		جاؤ errout;
	memset(icount->list, 0, بائٹیں);

	icount->گنتی = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	اگر (hint) {
		برائے (i=0; i < hint->گنتی; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->گنتی = hint->گنتی;
	}

successout:
	*ret = icount;
	واپس 0;

errout:
	ext2fs_free_icount(icount);
	واپس(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, صحیح flags,
			       غیرعلامتی صحیح سائز,
			       ext2_icount_t *ret)
{
	واپس ext2fs_create_icount2(fs, flags, سائز, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ساکن ساخت ext2_icount_el *داخل_کرو_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحیح pos)
{
	ساخت ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_سائز = 0;
	صحیح			عدد;

	اگر (icount->آخری_lookup && icount->آخری_lookup->ino == ino)
		واپس icount->آخری_lookup;

	اگر (icount->گنتی >= icount->سائز) {
		اگر (icount->گنتی) {
			new_سائز = icount->list[(غیرعلامتی)icount->گنتی-1].ino;
			new_سائز = (ext2_ino_t) (icount->گنتی *
				((اعشاری) icount->عدد_inodes / new_سائز));
		}
		اگر (new_سائز < (icount->سائز + 100))
			new_سائز = icount->سائز + 100;
#اگر 0
		لکھو("Reallocating icount %u entries...\n", new_سائز);
#اگر_ختم
		retval = ext2fs_resize_mem((size_t) icount->سائز *
					   حجم(ساخت ext2_icount_el),
					   (size_t) new_سائز *
					   حجم(ساخت ext2_icount_el),
					   &icount->list);
		اگر (retval)
			واپس 0;
		icount->سائز = new_سائز;
	}
	عدد = (صحیح) icount->گنتی - pos;
	اگر (عدد < 0)
		واپس 0;	/* should never happen */
	اگر (عدد) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			حجم(ساخت ext2_icount_el) * عدد);
	}
	icount->گنتی++;
	el = &icount->list[pos];
	el->گنتی = 0;
	el->ino = ino;
	icount->آخری_lookup = el;
	واپس el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ساکن ساخت ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحیح create)
{
	صحیح	کم, بلند, mid;

	اگر (!icount || !icount->list)
		واپس 0;

	اگر (create && ((icount->گنتی == 0) ||
		       (ino > icount->list[(غیرعلامتی)icount->گنتی-1].ino))) {
		واپس داخل_کرو_icount_el(icount, ino, (غیرعلامتی) icount->گنتی);
	}
	اگر (icount->گنتی == 0)
		واپس 0;

	اگر (icount->cursor >= icount->گنتی)
		icount->cursor = 0;
	اگر (ino == icount->list[icount->cursor].ino)
		واپس &icount->list[icount->cursor++];
#اگر 0
	لکھو("Non-cursor get_icount_el: %u\n", ino);
#اگر_ختم
	کم = 0;
	بلند = (صحیح) icount->گنتی-1;
	جب (کم <= بلند) {
		mid = ((غیرعلامتی)کم + (غیرعلامتی)بلند) >> 1;
		اگر (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			واپس &icount->list[mid];
		}
		اگر (ino < icount->list[mid].ino)
			بلند = mid-1;
		ورنہ
			کم = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	اگر (create)
		واپس داخل_کرو_icount_el(icount, ino, کم);
	واپس 0;
}

ساکن errcode_t سیٹ_inode_گنتی(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 گنتی)
{
	ساخت ext2_icount_el 	*el;
#اگر_تعریف_شدہ CONFIG_TDB
	TDB_ڈیٹا کلید, ڈیٹا;

	اگر (icount->tdb) {
		کلید.dptr = (غیرعلامتی حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);
		ڈیٹا.dptr = (غیرعلامتی حرف *) &گنتی;
		ڈیٹا.dsize = حجم(__u32);
		اگر (گنتی) {
			اگر (tdb_store(icount->tdb, کلید, ڈیٹا, TDB_REPLACE))
				واپس tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ورنہ {
			اگر (tdb_delete(icount->tdb, کلید))
				واپس tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		واپس 0;
	}
#اگر_ختم
	اگر (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(گنتی);
		واپس 0;
	}

	el = get_icount_el(icount, ino, 1);
	اگر (!el)
		واپس EXT2_ET_NO_حافظہ;

	el->گنتی = گنتی;
	واپس 0;
}

ساکن errcode_t get_inode_گنتی(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *گنتی)
{
	ساخت ext2_icount_el 	*el;
#اگر_تعریف_شدہ CONFIG_TDB
	TDB_ڈیٹا کلید, ڈیٹا;

	اگر (icount->tdb) {
		کلید.dptr = (غیرعلامتی حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);

		ڈیٹا = tdb_fetch(icount->tdb, کلید);
		اگر (ڈیٹا.dptr == تہی) {
			*گنتی = 0;
			واپس tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*گنتی = *((__u32 *) ڈیٹا.dptr);
		آزاد(ڈیٹا.dptr);
		واپس 0;
	}
#اگر_ختم
	اگر (icount->fullmap) {
		*گنتی = icount->fullmap[ino];
		واپس 0;
	}

	el = get_icount_el(icount, ino, 0);
	اگر (!el) {
		*گنتی = 0;
		واپس ENOENT;
	}

	*گنتی = el->گنتی;
	واپس 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *برآمد)
{
	errcode_t	ret = 0;
	غیرعلامتی صحیح	i;
	ثابت حرف *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->گنتی > icount->سائز) {
		fprintf(برآمد, "%s: count > size\n", bad);
		واپس EXT2_ET_INVALID_دلیل;
	}
	برائے (i=1; i < icount->گنتی; i++) {
		اگر (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(برآمد, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_دلیل;
		}
	}
	واپس ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (!ino || (ino > icount->عدد_inodes))
		واپس EXT2_ET_INVALID_دلیل;

	اگر (!icount->fullmap) {
		اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			واپس 0;
		}
		اگر (icount->مضرب &&
			!ext2fs_test_inode_bitmap2(icount->مضرب, ino)) {
			*ret = 0;
			واپس 0;
		}
	}
	get_inode_گنتی(icount, ino, &val);
	*ret = icount_16_xlate(val);
	واپس 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_قدر;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (!ino || (ino > icount->عدد_inodes))
		واپس EXT2_ET_INVALID_دلیل;

	اگر (icount->fullmap) {
		curr_قدر = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_قدر;
	} ورنہ اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		اگر (سیٹ_inode_گنتی(icount, ino, 2))
			واپس EXT2_ET_NO_حافظہ;
		curr_قدر = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ورنہ اگر (icount->مضرب) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		اگر (ext2fs_test_inode_bitmap2(icount->مضرب, ino)) {
			get_inode_گنتی(icount, ino, &curr_قدر);
			curr_قدر++;
			اگر (سیٹ_inode_گنتی(icount, ino, curr_قدر))
				واپس EXT2_ET_NO_حافظہ;
		} ورنہ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			اگر (ret)
				*ret = 1;
			واپس 0;
		}
	} ورنہ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_گنتی(icount, ino, &curr_قدر);
		curr_قدر++;
		اگر (سیٹ_inode_گنتی(icount, ino, curr_قدر))
			واپس EXT2_ET_NO_حافظہ;
	}
	اگر (icount->مضرب)
		ext2fs_mark_inode_bitmap2(icount->مضرب, ino);
	اگر (ret)
		*ret = icount_16_xlate(curr_قدر);
	واپس 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_قدر;

	اگر (!ino || (ino > icount->عدد_inodes))
		واپس EXT2_ET_INVALID_دلیل;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->fullmap) {
		اگر (!icount->fullmap[ino])
			واپس EXT2_ET_INVALID_دلیل;

		curr_قدر = --icount->fullmap[ino];
		اگر (ret)
			*ret = icount_16_xlate(curr_قدر);
		واپس 0;
	}

	اگر (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		اگر (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		ورنہ {
			سیٹ_inode_گنتی(icount, ino, 0);
		}
		اگر (ret)
			*ret = 0;
		واپس 0;
	}

	اگر (icount->مضرب &&
	    !ext2fs_test_inode_bitmap2(icount->مضرب, ino))
		واپس EXT2_ET_INVALID_دلیل;

	get_inode_گنتی(icount, ino, &curr_قدر);
	اگر (!curr_قدر)
		واپس EXT2_ET_INVALID_دلیل;
	curr_قدر--;
	اگر (سیٹ_inode_گنتی(icount, ino, curr_قدر))
		واپس EXT2_ET_NO_حافظہ;

	اگر (curr_قدر == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	اگر ((curr_قدر == 0) && icount->مضرب)
		ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);

	اگر (ret)
		*ret = icount_16_xlate(curr_قدر);
	واپس 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 گنتی)
{
	اگر (!ino || (ino > icount->عدد_inodes))
		واپس EXT2_ET_INVALID_دلیل;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	اگر (icount->fullmap)
		واپس سیٹ_inode_گنتی(icount, ino, گنتی);

	اگر (گنتی == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		اگر (icount->مضرب)
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		واپس 0;
	}
	اگر (گنتی == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		اگر (icount->مضرب) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->مضرب, ino);
		} ورنہ
			سیٹ_inode_گنتی(icount, ino, 0);
		واپس 0;
	}

	اگر (سیٹ_inode_گنتی(icount, ino, گنتی))
		واپس EXT2_ET_NO_حافظہ;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	اگر (icount->مضرب)
		ext2fs_mark_inode_bitmap2(icount->مضرب, ino);
	واپس 0;
}

ext2_ino_t ext2fs_get_icount_سائز(ext2_icount_t icount)
{
	اگر (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		واپس 0;

	واپس icount->سائز;
}

#اگر_تعریف_شدہ DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#تعریف_کرو EXIT		0x00
#تعریف_کرو FETCH		0x01
#تعریف_کرو STORE		0x02
#تعریف_کرو INCREMENT	0x03
#تعریف_کرو DECREMENT	0x04

ساخت test_پروگرام {
	صحیح		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ساخت test_پروگرام prog[] = {
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

ساخت test_پروگرام extended[] = {
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
ساکن خالی تیاری(خالی)
{
	errcode_t	retval;
	ساخت ext2_super_بلاک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکیں_گنتی_سیٹ(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		باہر_نکلو(1);
	}
}

صحیح تسلسل_test(صحیح flags, صحیح سائز, حرف *dir, ساخت test_پروگرام *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ساخت test_پروگرام *pc;
	__u16		نتیجہ;
	صحیح		problem = 0;

	اگر (dir) {
#اگر_تعریف_شدہ CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		اگر (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			باہر_نکلو(1);
		}
#ورنہ
		لکھو("Skipped\n");
		واپس 0;
#اگر_ختم
	} ورنہ {
		retval = ext2fs_create_icount2(test_fs, flags, سائز, 0,
					       &icount);
		اگر (retval) {
			com_err("run_test", retval, "while creating icount");
			باہر_نکلو(1);
		}
	}
	برائے (pc = prog; pc->cmd != EXIT; pc++) {
		انتخاب (pc->cmd) {
		حالت FETCH:
			لکھو("icount_fetch(%u) = ", pc->ino);
			توڑو;
		حالت STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				باہر_نکلو(1);
			}
			لکھو("icount_store(%u, %u) = ", pc->ino, pc->arg);
			توڑو;
		حالت INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				باہر_نکلو(1);
			}
			لکھو("icount_increment(%u) = ", pc->ino);
			توڑو;
		حالت DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			اگر (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				باہر_نکلو(1);
			}
			لکھو("icount_decrement(%u) = ", pc->ino);
			توڑو;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &نتیجہ);
		اگر (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			باہر_نکلو(1);
		}
		لکھو("%u (%s)\n", نتیجہ, (نتیجہ == pc->expected) ?
		       "OK" : "NOT OK");
		اگر (نتیجہ != pc->expected)
			problem++;
	}
	لکھو("icount size is %u\n", ext2fs_get_icount_سائز(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	اگر (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		باہر_نکلو(1);
	}
	ext2fs_free_icount(icount);
	واپس problem;
}


صحیح مین(صحیح argc, حرف **argv)
{
	صحیح failed = 0;

	تیاری();
	لکھو("Standard icount run:\n");
	failed += تسلسل_test(0, 0, 0, prog);
	لکھو("\nMultiple bitmap test:\n");
	failed += تسلسل_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	لکھو("\nResizing icount:\n");
	failed += تسلسل_test(0, 3, 0, extended);
	لکھو("\nStandard icount run with tdb:\n");
	failed += تسلسل_test(0, 0, ".", prog);
	لکھو("\nMultiple bitmap test with tdb:\n");
	failed += تسلسل_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	اگر (failed)
		لکھو("FAILED!\n");
	واپس failed;
}
#اگر_ختم
