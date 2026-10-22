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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tdb.h"

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

ਬਣਤਰ ext2_icount_el {
	ext2_ino_t	ino;
	__u32		ਗਿਣਤੀ_2;
};

ਬਣਤਰ ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	ਗੁਣਜ;
	ext2_ino_t		ਗਿਣਤੀ_2;
	ext2_ino_t		ਮਾਪ;
	ext2_ino_t		ਅੰਕ_inodes;
	ext2_ino_t		cursor;
	ਬਣਤਰ ext2_icount_el	*list;
	ਬਣਤਰ ext2_icount_el	*ਆਖਰੀ_lookup;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
	ਅੱਖਰ			*tdb_fn;
	TDB_CONTEXT		*tdb;
#ਜੇ_ਅੰਤ
	__u16			*fullmap;
};

/*
 * We now use a 32-bit counter field because it doesn't cost us
 * anything extra for the in-memory data structure, due to alignment
 * padding.  But there's no point changing the interface if most of
 * the time we only care if the number is bigger than 65,000 or not.
 * So use the following translation function to return a 16-bit count.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ icount_16_xlate(x) (((x) > 65500) ? 65500 : (x))

ਖਾਲੀ ext2fs_free_icount(ext2_icount_t icount)
{
	ਜੇ (!icount)
		ਵਾਪਸ;

	icount->magic = 0;
	ਜੇ (icount->list)
		ext2fs_free_mem(&icount->list);
	ਜੇ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ਜੇ (icount->ਗੁਣਜ)
		ext2fs_free_inode_bitmap(icount->ਗੁਣਜ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
	ਜੇ (icount->tdb)
		tdb_close(icount->tdb);
	ਜੇ (icount->tdb_fn) {
		(ਖਾਲੀ) unlink(icount->tdb_fn);
		ਮੁਕਤ(icount->tdb_fn);
	}
#ਜੇ_ਅੰਤ

	ਜੇ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

ਸਥਿਰ errcode_t alloc_icount(ext2_filsys fs, ਪੂਰਨ flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_icount), &icount);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(icount, 0, ਆਕਾਰ(ਬਣਤਰ ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->ਅੰਕ_inodes = fs->super->s_inodes_ਗਿਣਤੀ;

	ਜੇ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ sz = ਆਕਾਰ(*icount->fullmap) * icount->ਅੰਕ_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ਜੇ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ਵਾਪਸ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਜੇ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->ਗੁਣਜ);
		ਜੇ (retval)
			ਜਾਓ errout;
	} ਨਹੀਂਤਾਂ
		icount->ਗੁਣਜ = 0;

	*ret = icount;
	ਵਾਪਸ 0;

errout:
	ext2fs_free_icount(icount);
	ਵਾਪਸ(retval);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
ਬਣਤਰ uuid {
	__u32	ਸਮਾਂ_ਨੀਵਾਂ;
	__u16	ਸਮਾਂ_mid;
	__u16	ਸਮਾਂ_hi_and_ਸੰਸਕਰਣ;
	__u16	clock_seq;
	__u8	ਗੰਢ[6];
};

ਸਥਿਰ ਖਾਲੀ unpack_uuid(ਖਾਲੀ *ਵਿੱਚ, ਬਣਤਰ uuid *uu)
{
	__u8	*ptr = ਵਿੱਚ;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ਸਮਾਂ_ਨੀਵਾਂ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ਸਮਾਂ_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->ਸਮਾਂ_hi_and_ਸੰਸਕਰਣ = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(uu->ਗੰਢ, ptr, 6);
}

ਸਥਿਰ ਖਾਲੀ uuid_unparse(ਖਾਲੀ *uu, ਅੱਖਰ *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(ਬਾਹਰਲਾ,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.ਸਮਾਂ_ਨੀਵਾਂ, uuid.ਸਮਾਂ_mid, uuid.ਸਮਾਂ_hi_and_ਸੰਸਕਰਣ,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.ਗੰਢ[0], uuid.ਗੰਢ[1], uuid.ਗੰਢ[2],
		uuid.ਗੰਢ[3], uuid.ਗੰਢ[4], uuid.ਗੰਢ[5]);
}
#ਜੇ_ਅੰਤ

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ,
				   ਅੱਖਰ *tdb_dir EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ,
				   ਪੂਰਨ flags EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ,
				   ext2_icount_t *ret EXT2_FS_NO_TDB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	ਅੱਖਰ 		*fn, uuid[40];
	ext2_ino_t	ਅੰਕ_inodes;
	mode_t		save_umask;
	ਪੂਰਨ		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(tdb_dir) + 64, &fn);
	ਜੇ (retval)
		ਜਾਓ errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ਜੇ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		ਜਾਓ errout;
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
	ਅੰਕ_inodes = fs->super->s_inodes_ਗਿਣਤੀ - fs->super->s_free_inodes_ਗਿਣਤੀ;

	icount->tdb = tdb_open(fn, ਅੰਕ_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	ਜੇ (icount->tdb == ਨਿਰਾ) {
		retval = errno;
		ਜਾਓ errout;
	}
	*ret = icount;
	ਵਾਪਸ 0;
errout:
	ext2fs_free_icount(icount);
	ਵਾਪਸ(retval);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
#ਜੇ_ਅੰਤ
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, ਪੂਰਨ flags, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		ਬਾਈਟਾਂ;
	ext2_ino_t	i;

	ਜੇ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ਜੇ (hint->ਮਾਪ > ਮਾਪ)
			ਮਾਪ = (size_t) hint->ਮਾਪ;
	}

	retval = alloc_icount(fs, flags, &icount);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (icount->fullmap)
		ਜਾਓ successout;

	ਜੇ (ਮਾਪ) {
		icount->ਮਾਪ = ਮਾਪ;
	} ਨਹੀਂਤਾਂ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_ਅੰਕ_dirs(fs, &icount->ਮਾਪ);
		ਜੇ (retval)
			ਜਾਓ errout;
		icount->ਮਾਪ += fs->super->s_inodes_ਗਿਣਤੀ / 50;
	}

	ਬਾਈਟਾਂ = (size_t) (icount->ਮਾਪ * ਆਕਾਰ(ਬਣਤਰ ext2_icount_el));
#ਜੇ 0
	ਲਿਖੋ("Icount allocated %u entries, %d bytes.\n",
	       icount->ਮਾਪ, ਬਾਈਟਾਂ);
#ਜੇ_ਅੰਤ
	retval = ext2fs_get_array(icount->ਮਾਪ, ਆਕਾਰ(ਬਣਤਰ ext2_icount_el),
			 &icount->list);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(icount->list, 0, ਬਾਈਟਾਂ);

	icount->ਗਿਣਤੀ_2 = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ਜੇ (hint) {
		ਲਈ (i=0; i < hint->ਗਿਣਤੀ_2; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->ਗਿਣਤੀ_2 = hint->ਗਿਣਤੀ_2;
	}

successout:
	*ret = icount;
	ਵਾਪਸ 0;

errout:
	ext2fs_free_icount(icount);
	ਵਾਪਸ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, ਪੂਰਨ flags,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
			       ext2_icount_t *ret)
{
	ਵਾਪਸ ext2fs_create_icount2(fs, flags, ਮਾਪ, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
ਸਥਿਰ ਬਣਤਰ ext2_icount_el *ਪਾਓ_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ਪੂਰਨ pos)
{
	ਬਣਤਰ ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_ਮਾਪ = 0;
	ਪੂਰਨ			ਅੰਕ;

	ਜੇ (icount->ਆਖਰੀ_lookup && icount->ਆਖਰੀ_lookup->ino == ino)
		ਵਾਪਸ icount->ਆਖਰੀ_lookup;

	ਜੇ (icount->ਗਿਣਤੀ_2 >= icount->ਮਾਪ) {
		ਜੇ (icount->ਗਿਣਤੀ_2) {
			new_ਮਾਪ = icount->list[(ਬਿਨਾਨਿਸ਼ਾਨੀ)icount->ਗਿਣਤੀ_2-1].ino;
			new_ਮਾਪ = (ext2_ino_t) (icount->ਗਿਣਤੀ_2 *
				((ਦਸ਼ਮਲਵ) icount->ਅੰਕ_inodes / new_ਮਾਪ));
		}
		ਜੇ (new_ਮਾਪ < (icount->ਮਾਪ + 100))
			new_ਮਾਪ = icount->ਮਾਪ + 100;
#ਜੇ 0
		ਲਿਖੋ("Reallocating icount %u entries...\n", new_ਮਾਪ);
#ਜੇ_ਅੰਤ
		retval = ext2fs_resize_mem((size_t) icount->ਮਾਪ *
					   ਆਕਾਰ(ਬਣਤਰ ext2_icount_el),
					   (size_t) new_ਮਾਪ *
					   ਆਕਾਰ(ਬਣਤਰ ext2_icount_el),
					   &icount->list);
		ਜੇ (retval)
			ਵਾਪਸ 0;
		icount->ਮਾਪ = new_ਮਾਪ;
	}
	ਅੰਕ = (ਪੂਰਨ) icount->ਗਿਣਤੀ_2 - pos;
	ਜੇ (ਅੰਕ < 0)
		ਵਾਪਸ 0;	/* should never happen */
	ਜੇ (ਅੰਕ) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			ਆਕਾਰ(ਬਣਤਰ ext2_icount_el) * ਅੰਕ);
	}
	icount->ਗਿਣਤੀ_2++;
	el = &icount->list[pos];
	el->ਗਿਣਤੀ_2 = 0;
	el->ino = ino;
	icount->ਆਖਰੀ_lookup = el;
	ਵਾਪਸ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
ਸਥਿਰ ਬਣਤਰ ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, ਪੂਰਨ create)
{
	ਪੂਰਨ	ਨੀਵਾਂ, ਉੱਚਾ, mid;

	ਜੇ (!icount || !icount->list)
		ਵਾਪਸ 0;

	ਜੇ (create && ((icount->ਗਿਣਤੀ_2 == 0) ||
		       (ino > icount->list[(ਬਿਨਾਨਿਸ਼ਾਨੀ)icount->ਗਿਣਤੀ_2-1].ino))) {
		ਵਾਪਸ ਪਾਓ_icount_el(icount, ino, (ਬਿਨਾਨਿਸ਼ਾਨੀ) icount->ਗਿਣਤੀ_2);
	}
	ਜੇ (icount->ਗਿਣਤੀ_2 == 0)
		ਵਾਪਸ 0;

	ਜੇ (icount->cursor >= icount->ਗਿਣਤੀ_2)
		icount->cursor = 0;
	ਜੇ (ino == icount->list[icount->cursor].ino)
		ਵਾਪਸ &icount->list[icount->cursor++];
#ਜੇ 0
	ਲਿਖੋ("Non-cursor get_icount_el: %u\n", ino);
#ਜੇ_ਅੰਤ
	ਨੀਵਾਂ = 0;
	ਉੱਚਾ = (ਪੂਰਨ) icount->ਗਿਣਤੀ_2-1;
	ਜਦੋਂ (ਨੀਵਾਂ <= ਉੱਚਾ) {
		mid = ((ਬਿਨਾਨਿਸ਼ਾਨੀ)ਨੀਵਾਂ + (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਉੱਚਾ) >> 1;
		ਜੇ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ਵਾਪਸ &icount->list[mid];
		}
		ਜੇ (ino < icount->list[mid].ino)
			ਉੱਚਾ = mid-1;
		ਨਹੀਂਤਾਂ
			ਨੀਵਾਂ = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ਜੇ (create)
		ਵਾਪਸ ਪਾਓ_icount_el(icount, ino, ਨੀਵਾਂ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ਸੈੱਟ_inode_ਗਿਣਤੀ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 ਗਿਣਤੀ_2)
{
	ਬਣਤਰ ext2_icount_el 	*el;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, ਅੰਕੜੇ;

	ਜੇ (icount->tdb) {
		ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &ino;
		ਕੁੰਜੀ.dsize = ਆਕਾਰ(ext2_ino_t);
		ਅੰਕੜੇ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &ਗਿਣਤੀ_2;
		ਅੰਕੜੇ.dsize = ਆਕਾਰ(__u32);
		ਜੇ (ਗਿਣਤੀ_2) {
			ਜੇ (tdb_store(icount->tdb, ਕੁੰਜੀ, ਅੰਕੜੇ, TDB_REPLACE))
				ਵਾਪਸ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ਨਹੀਂਤਾਂ {
			ਜੇ (tdb_delete(icount->tdb, ਕੁੰਜੀ))
				ਵਾਪਸ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
	ਜੇ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(ਗਿਣਤੀ_2);
		ਵਾਪਸ 0;
	}

	el = get_icount_el(icount, ino, 1);
	ਜੇ (!el)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	el->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t get_inode_ਗਿਣਤੀ(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *ਗਿਣਤੀ_2)
{
	ਬਣਤਰ ext2_icount_el 	*el;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, ਅੰਕੜੇ;

	ਜੇ (icount->tdb) {
		ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &ino;
		ਕੁੰਜੀ.dsize = ਆਕਾਰ(ext2_ino_t);

		ਅੰਕੜੇ = tdb_fetch(icount->tdb, ਕੁੰਜੀ);
		ਜੇ (ਅੰਕੜੇ.dptr == ਨਿਰਾ) {
			*ਗਿਣਤੀ_2 = 0;
			ਵਾਪਸ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*ਗਿਣਤੀ_2 = *((__u32 *) ਅੰਕੜੇ.dptr);
		ਮੁਕਤ(ਅੰਕੜੇ.dptr);
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
	ਜੇ (icount->fullmap) {
		*ਗਿਣਤੀ_2 = icount->fullmap[ino];
		ਵਾਪਸ 0;
	}

	el = get_icount_el(icount, ino, 0);
	ਜੇ (!el) {
		*ਗਿਣਤੀ_2 = 0;
		ਵਾਪਸ ENOENT;
	}

	*ਗਿਣਤੀ_2 = el->ਗਿਣਤੀ_2;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *ਬਾਹਰਲਾ)
{
	errcode_t	ret = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	i;
	ਅਚਲ ਅੱਖਰ *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ਜੇ (icount->ਗਿਣਤੀ_2 > icount->ਮਾਪ) {
		fprintf(ਬਾਹਰਲਾ, "%s: count > size\n", bad);
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	}
	ਲਈ (i=1; i < icount->ਗਿਣਤੀ_2; i++) {
		ਜੇ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(ਬਾਹਰਲਾ, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ਦਲੀਲ;
		}
	}
	ਵਾਪਸ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ਜੇ (!ino || (ino > icount->ਅੰਕ_inodes))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (!icount->fullmap) {
		ਜੇ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ਵਾਪਸ 0;
		}
		ਜੇ (icount->ਗੁਣਜ &&
			!ext2fs_test_inode_bitmap2(icount->ਗੁਣਜ, ino)) {
			*ret = 0;
			ਵਾਪਸ 0;
		}
	}
	get_inode_ਗਿਣਤੀ(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ਮੁੱਲ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ਜੇ (!ino || (ino > icount->ਅੰਕ_inodes))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (icount->fullmap) {
		curr_ਮੁੱਲ = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_ਮੁੱਲ;
	} ਨਹੀਂਤਾਂ ਜੇ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ਜੇ (ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, 2))
			ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		curr_ਮੁੱਲ = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ਨਹੀਂਤਾਂ ਜੇ (icount->ਗੁਣਜ) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ਜੇ (ext2fs_test_inode_bitmap2(icount->ਗੁਣਜ, ino)) {
			get_inode_ਗਿਣਤੀ(icount, ino, &curr_ਮੁੱਲ);
			curr_ਮੁੱਲ++;
			ਜੇ (ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, curr_ਮੁੱਲ))
				ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		} ਨਹੀਂਤਾਂ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ਜੇ (ret)
				*ret = 1;
			ਵਾਪਸ 0;
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_ਗਿਣਤੀ(icount, ino, &curr_ਮੁੱਲ);
		curr_ਮੁੱਲ++;
		ਜੇ (ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, curr_ਮੁੱਲ))
			ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	}
	ਜੇ (icount->ਗੁਣਜ)
		ext2fs_mark_inode_bitmap2(icount->ਗੁਣਜ, ino);
	ਜੇ (ret)
		*ret = icount_16_xlate(curr_ਮੁੱਲ);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_ਮੁੱਲ;

	ਜੇ (!ino || (ino > icount->ਅੰਕ_inodes))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ਜੇ (icount->fullmap) {
		ਜੇ (!icount->fullmap[ino])
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		curr_ਮੁੱਲ = --icount->fullmap[ino];
		ਜੇ (ret)
			*ret = icount_16_xlate(curr_ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ਜੇ (icount->ਗੁਣਜ)
			ext2fs_unmark_inode_bitmap2(icount->ਗੁਣਜ, ino);
		ਨਹੀਂਤਾਂ {
			ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, 0);
		}
		ਜੇ (ret)
			*ret = 0;
		ਵਾਪਸ 0;
	}

	ਜੇ (icount->ਗੁਣਜ &&
	    !ext2fs_test_inode_bitmap2(icount->ਗੁਣਜ, ino))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	get_inode_ਗਿਣਤੀ(icount, ino, &curr_ਮੁੱਲ);
	ਜੇ (!curr_ਮੁੱਲ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	curr_ਮੁੱਲ--;
	ਜੇ (ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, curr_ਮੁੱਲ))
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	ਜੇ (curr_ਮੁੱਲ == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ਜੇ ((curr_ਮੁੱਲ == 0) && icount->ਗੁਣਜ)
		ext2fs_unmark_inode_bitmap2(icount->ਗੁਣਜ, ino);

	ਜੇ (ret)
		*ret = icount_16_xlate(curr_ਮੁੱਲ);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 ਗਿਣਤੀ_2)
{
	ਜੇ (!ino || (ino > icount->ਅੰਕ_inodes))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ਜੇ (icount->fullmap)
		ਵਾਪਸ ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, ਗਿਣਤੀ_2);

	ਜੇ (ਗਿਣਤੀ_2 == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ਜੇ (icount->ਗੁਣਜ)
			ext2fs_unmark_inode_bitmap2(icount->ਗੁਣਜ, ino);
		ਵਾਪਸ 0;
	}
	ਜੇ (ਗਿਣਤੀ_2 == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ਜੇ (icount->ਗੁਣਜ) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->ਗੁਣਜ, ino);
		} ਨਹੀਂਤਾਂ
			ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, 0);
		ਵਾਪਸ 0;
	}

	ਜੇ (ਸੈੱਟ_inode_ਗਿਣਤੀ(icount, ino, ਗਿਣਤੀ_2))
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ਜੇ (icount->ਗੁਣਜ)
		ext2fs_mark_inode_bitmap2(icount->ਗੁਣਜ, ino);
	ਵਾਪਸ 0;
}

ext2_ino_t ext2fs_get_icount_ਮਾਪ(ext2_icount_t icount)
{
	ਜੇ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ਵਾਪਸ 0;

	ਵਾਪਸ icount->ਮਾਪ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXIT		0x00
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FETCH		0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STORE		0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCREMENT	0x03
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECREMENT	0x04

ਬਣਤਰ test_ਪ੍ਰੋਗਰਾਮ {
	ਪੂਰਨ		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

ਬਣਤਰ test_ਪ੍ਰੋਗਰਾਮ prog[] = {
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

ਬਣਤਰ test_ਪ੍ਰੋਗਰਾਮ extended[] = {
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
ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ(ਖਾਲੀ)
{
	errcode_t	retval;
	ਬਣਤਰ ext2_super_ਖੰਡ param;

	initialize_ext2_error_ਸਾਰਣੀ();

	memset(&param, 0, ਆਕਾਰ(param));
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ਬਿੱਟ, &param,
				   test_io_manager, &test_fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
}

ਪੂਰਨ ਲੜੀਵਾਰ_test(ਪੂਰਨ flags, ਪੂਰਨ ਮਾਪ, ਅੱਖਰ *dir, ਬਣਤਰ test_ਪ੍ਰੋਗਰਾਮ *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	ਬਣਤਰ test_ਪ੍ਰੋਗਰਾਮ *pc;
	__u16		ਨਤੀਜਾ;
	ਪੂਰਨ		problem = 0;

	ਜੇ (dir) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ਜੇ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
#ਨਹੀਂਤਾਂ
		ਲਿਖੋ("Skipped\n");
		ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_create_icount2(test_fs, flags, ਮਾਪ, 0,
					       &icount);
		ਜੇ (retval) {
			com_err("run_test", retval, "while creating icount");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਈ (pc = prog; pc->cmd != EXIT; pc++) {
		ਚੋਣ (pc->cmd) {
		ਹਾਲਤ FETCH:
			ਲਿਖੋ("icount_fetch(%u) = ", pc->ino);
			ਰੋਕੋ;
		ਹਾਲਤ STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ਜੇ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
			ਲਿਖੋ("icount_store(%u, %u) = ", pc->ino, pc->arg);
			ਰੋਕੋ;
		ਹਾਲਤ INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ਜੇ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
			ਲਿਖੋ("icount_increment(%u) = ", pc->ino);
			ਰੋਕੋ;
		ਹਾਲਤ DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ਜੇ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
			ਲਿਖੋ("icount_decrement(%u) = ", pc->ino);
			ਰੋਕੋ;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &ਨਤੀਜਾ);
		ਜੇ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		ਲਿਖੋ("%u (%s)\n", ਨਤੀਜਾ, (ਨਤੀਜਾ == pc->expected) ?
		       "OK" : "NOT OK");
		ਜੇ (ਨਤੀਜਾ != pc->expected)
			problem++;
	}
	ਲਿਖੋ("icount size is %u\n", ext2fs_get_icount_ਮਾਪ(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ਜੇ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ext2fs_free_icount(icount);
	ਵਾਪਸ problem;
}


ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ failed = 0;

	ਤਿਆਰੀ();
	ਲਿਖੋ("Standard icount run:\n");
	failed += ਲੜੀਵਾਰ_test(0, 0, 0, prog);
	ਲਿਖੋ("\nMultiple bitmap test:\n");
	failed += ਲੜੀਵਾਰ_test(EXT2_ICOUNT_OPT_INCREMENT, 0, 0, prog);
	ਲਿਖੋ("\nResizing icount:\n");
	failed += ਲੜੀਵਾਰ_test(0, 3, 0, extended);
	ਲਿਖੋ("\nStandard icount run with tdb:\n");
	failed += ਲੜੀਵਾਰ_test(0, 0, ".", prog);
	ਲਿਖੋ("\nMultiple bitmap test with tdb:\n");
	failed += ਲੜੀਵਾਰ_test(EXT2_ICOUNT_OPT_INCREMENT, 0, ".", prog);
	ਜੇ (failed)
		ਲਿਖੋ("FAILED!\n");
	ਵਾਪਸ failed;
}
#ਜੇ_ਅੰਤ
