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
#endif
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
	__u32		گنتی;
};

بنية ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		گنتی;
	ext2_ino_t		size;
	ext2_ino_t		عدد_inodes;
	ext2_ino_t		cursor;
	بنية ext2_icount_el	*list;
	بنية ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	حرف			*tdb_fn;
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

فراغ ext2fs_free_icount(ext2_icount_t icount)
{
	إذا (!icount)
		أرجع;

	icount->magic = 0;
	إذا (icount->list)
		ext2fs_free_mem(&icount->list);
	إذا (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	إذا (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	إذا (icount->tdb)
		tdb_close(icount->tdb);
	إذا (icount->tdb_fn) {
		(فراغ) unlink(icount->tdb_fn);
		حرر(icount->tdb_fn);
	}
#endif

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
	icount->عدد_inodes = fs->super->s_inodes_گنتی;

	إذا ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = حجم(*icount->fullmap) * icount->عدد_inodes;

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
						      &icount->multiple);
		إذا (retval)
			اذهب errout;
	} وإلا
		icount->multiple = 0;

	*ret = icount;
	أرجع 0;

errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
}

#ifdef CONFIG_TDB
بنية uuid {
	__u32	time_کم;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	گرہ[6];
};

ساكن فراغ unpack_uuid(فراغ *in, بنية uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_کم = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->گرہ, ptr, 6);
}

ساكن فراغ uuid_unparse(فراغ *uu, حرف *out)
{
	بنية uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_کم, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.گرہ[0], uuid.گرہ[1], uuid.گرہ[2],
		uuid.گرہ[3], uuid.گرہ[4], uuid.گرہ[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   حرف *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   صحيح flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	حرف 		*fn, uuid[40];
	ext2_ino_t	عدد_inodes;
	mode_t		save_umask;
	صحيح		fd;

	retval = alloc_icount(fs, flags,  &icount);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
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
	عدد_inodes = fs->super->s_inodes_گنتی - fs->super->s_free_inodes_گنتی;

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
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, صحيح flags, unsigned صحيح size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	إذا (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		إذا (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	إذا (retval)
		أرجع retval;

	إذا (icount->fullmap)
		اذهب successout;

	إذا (size) {
		icount->size = size;
	} وإلا {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_عدد_dirs(fs, &icount->size);
		إذا (retval)
			اذهب errout;
		icount->size += fs->super->s_inodes_گنتی / 50;
	}

	bytes = (size_t) (icount->size * حجم(بنية ext2_icount_el));
#إذا 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, حجم(بنية ext2_icount_el),
			 &icount->list);
	إذا (retval)
		اذهب errout;
	memset(icount->list, 0, bytes);

	icount->گنتی = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	إذا (hint) {
		لكل (i=0; i < hint->گنتی; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->گنتی = hint->گنتی;
	}

successout:
	*ret = icount;
	أرجع 0;

errout:
	ext2fs_free_icount(icount);
	أرجع(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, صحيح flags,
			       unsigned صحيح size,
			       ext2_icount_t *ret)
{
	أرجع ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ساكن بنية ext2_icount_el *داخل_کرو_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, صحيح pos)
{
	بنية ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	صحيح			عدد;

	إذا (icount->last_lookup && icount->last_lookup->ino == ino)
		أرجع icount->last_lookup;

	إذا (icount->گنتی >= icount->size) {
		إذا (icount->گنتی) {
			new_size = icount->list[(unsigned)icount->گنتی-1].ino;
			new_size = (ext2_ino_t) (icount->گنتی *
				((عائم) icount->عدد_inodes / new_size));
		}
		إذا (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#إذا 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   حجم(بنية ext2_icount_el),
					   (size_t) new_size *
					   حجم(بنية ext2_icount_el),
					   &icount->list);
		إذا (retval)
			أرجع 0;
		icount->size = new_size;
	}
	عدد = (صحيح) icount->گنتی - pos;
	إذا (عدد < 0)
		أرجع 0;	/* should never happen */
	إذا (عدد) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			حجم(بنية ext2_icount_el) * عدد);
	}
	icount->گنتی++;
	el = &icount->list[pos];
	el->گنتی = 0;
	el->ino = ino;
	icount->last_lookup = el;
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
	صحيح	کم, بلند, mid;

	إذا (!icount || !icount->list)
		أرجع 0;

	إذا (create && ((icount->گنتی == 0) ||
		       (ino > icount->list[(unsigned)icount->گنتی-1].ino))) {
		أرجع داخل_کرو_icount_el(icount, ino, (unsigned) icount->گنتی);
	}
	إذا (icount->گنتی == 0)
		أرجع 0;

	إذا (icount->cursor >= icount->گنتی)
		icount->cursor = 0;
	إذا (ino == icount->list[icount->cursor].ino)
		أرجع &icount->list[icount->cursor++];
#إذا 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	کم = 0;
	بلند = (صحيح) icount->گنتی-1;
	طالما (کم <= بلند) {
		mid = ((unsigned)کم + (unsigned)بلند) >> 1;
		إذا (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			أرجع &icount->list[mid];
		}
		إذا (ino < icount->list[mid].ino)
			بلند = mid-1;
		وإلا
			کم = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	إذا (create)
		أرجع داخل_کرو_icount_el(icount, ino, کم);
	أرجع 0;
}

ساكن errcode_t set_inode_گنتی(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 گنتی)
{
	بنية ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA کلید, data;

	إذا (icount->tdb) {
		کلید.dptr = (unsigned حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);
		data.dptr = (unsigned حرف *) &گنتی;
		data.dsize = حجم(__u32);
		إذا (گنتی) {
			إذا (tdb_store(icount->tdb, کلید, data, TDB_REPLACE))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} وإلا {
			إذا (tdb_delete(icount->tdb, کلید))
				أرجع tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		أرجع 0;
	}
#endif
	إذا (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(گنتی);
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 1);
	إذا (!el)
		أرجع EXT2_ET_NO_MEMORY;

	el->گنتی = گنتی;
	أرجع 0;
}

ساكن errcode_t get_inode_گنتی(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *گنتی)
{
	بنية ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA کلید, data;

	إذا (icount->tdb) {
		کلید.dptr = (unsigned حرف *) &ino;
		کلید.dsize = حجم(ext2_ino_t);

		data = tdb_fetch(icount->tdb, کلید);
		إذا (data.dptr == عدم) {
			*گنتی = 0;
			أرجع tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*گنتی = *((__u32 *) data.dptr);
		حرر(data.dptr);
		أرجع 0;
	}
#endif
	إذا (icount->fullmap) {
		*گنتی = icount->fullmap[ino];
		أرجع 0;
	}

	el = get_icount_el(icount, ino, 0);
	إذا (!el) {
		*گنتی = 0;
		أرجع ENOENT;
	}

	*گنتی = el->گنتی;
	أرجع 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned صحيح	i;
	ثابت حرف *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->گنتی > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		أرجع EXT2_ET_INVALID_ARGUMENT;
	}
	لكل (i=1; i < icount->گنتی; i++) {
		إذا (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	إذا (!icount->fullmap) {
		إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			أرجع 0;
		}
		إذا (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			أرجع 0;
		}
	}
	get_inode_گنتی(icount, ino, &val);
	*ret = icount_16_xlate(val);
	أرجع 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_قدر;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	إذا (icount->fullmap) {
		curr_قدر = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_قدر;
	} وإلا إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		إذا (set_inode_گنتی(icount, ino, 2))
			أرجع EXT2_ET_NO_MEMORY;
		curr_قدر = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} وإلا إذا (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		إذا (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_گنتی(icount, ino, &curr_قدر);
			curr_قدر++;
			إذا (set_inode_گنتی(icount, ino, curr_قدر))
				أرجع EXT2_ET_NO_MEMORY;
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
		get_inode_گنتی(icount, ino, &curr_قدر);
		curr_قدر++;
		إذا (set_inode_گنتی(icount, ino, curr_قدر))
			أرجع EXT2_ET_NO_MEMORY;
	}
	إذا (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	إذا (ret)
		*ret = icount_16_xlate(curr_قدر);
	أرجع 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_قدر;

	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap) {
		إذا (!icount->fullmap[ino])
			أرجع EXT2_ET_INVALID_ARGUMENT;

		curr_قدر = --icount->fullmap[ino];
		إذا (ret)
			*ret = icount_16_xlate(curr_قدر);
		أرجع 0;
	}

	إذا (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		وإلا {
			set_inode_گنتی(icount, ino, 0);
		}
		إذا (ret)
			*ret = 0;
		أرجع 0;
	}

	إذا (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	get_inode_گنتی(icount, ino, &curr_قدر);
	إذا (!curr_قدر)
		أرجع EXT2_ET_INVALID_ARGUMENT;
	curr_قدر--;
	إذا (set_inode_گنتی(icount, ino, curr_قدر))
		أرجع EXT2_ET_NO_MEMORY;

	إذا (curr_قدر == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	إذا ((curr_قدر == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	إذا (ret)
		*ret = icount_16_xlate(curr_قدر);
	أرجع 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 گنتی)
{
	إذا (!ino || (ino > icount->عدد_inodes))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	إذا (icount->fullmap)
		أرجع set_inode_گنتی(icount, ino, گنتی);

	إذا (گنتی == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		إذا (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		أرجع 0;
	}
	إذا (گنتی == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		إذا (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} وإلا
			set_inode_گنتی(icount, ino, 0);
		أرجع 0;
	}

	إذا (set_inode_گنتی(icount, ino, گنتی))
		أرجع EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	إذا (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	أرجع 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	إذا (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		أرجع 0;

	أرجع icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

بنية test_program {
	صحيح		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

بنية test_program prog[] = {
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

بنية test_program extended[] = {
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
ساكن فراغ setup(فراغ)
{
	errcode_t	retval;
	بنية ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, حجم(param));
	ext2fs_blocks_گنتی_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

صحيح run_test(صحيح flags, صحيح size, حرف *dir, بنية test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	بنية test_program *pc;
	__u16		result;
	صحيح		problem = 0;

	إذا (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		إذا (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#وإلا
		printf("Skipped\n");
		أرجع 0;
#endif
	} وإلا {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		إذا (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	لكل (pc = prog; pc->cmd != EXIT; pc++) {
		بدل (pc->cmd) {
		حالة FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			اكسر;
		حالة STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			اكسر;
		حالة INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			اكسر;
		حالة DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			إذا (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			اكسر;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		إذا (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		إذا (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	إذا (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	أرجع problem;
}


صحيح main(صحيح argc, حرف **argv)
{
	صحيح failed = 0;

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
	إذا (failed)
		printf("FAILED!\n");
	أرجع failed;
}
#endif
