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

#include "config.h"
#ᱡᱩᱫᱤ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fs.h"
#include "tdb.h"

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

sa_nracanaa ext2_icount_el {
	ext2_ino_t	ino;
	__u32		count;
};

sa_nracanaa ext2_icount {
	errcode_t		magic;
	ext2fs_inode_bitmap	single;
	ext2fs_inode_bitmap	multiple;
	ext2_ino_t		count;
	ext2_ino_t		size;
	ext2_ino_t		num_inodes;
	ext2_ino_t		cursor;
	sa_nracanaa ext2_icount_el	*list;
	sa_nracanaa ext2_icount_el	*last_lookup;
#ifdef CONFIG_TDB
	var_na			*tdb_fn;
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

void ext2fs_free_icount(ext2_icount_t icount)
{
	ᱡᱩᱫᱤ (!icount)
		ᱨᱩᱣᱟᱹᱲ;

	icount->magic = 0;
	ᱡᱩᱫᱤ (icount->list)
		ext2fs_free_mem(&icount->list);
	ᱡᱩᱫᱤ (icount->single)
		ext2fs_free_inode_bitmap(icount->single);
	ᱡᱩᱫᱤ (icount->multiple)
		ext2fs_free_inode_bitmap(icount->multiple);
#ifdef CONFIG_TDB
	ᱡᱩᱫᱤ (icount->tdb)
		tdb_close(icount->tdb);
	ᱡᱩᱫᱤ (icount->tdb_fn) {
		(void) unlink(icount->tdb_fn);
		free(icount->tdb_fn);
	}
#endif

	ᱡᱩᱫᱤ (icount->fullmap)
		ext2fs_free_mem(&icount->fullmap);

	ext2fs_free_mem(&icount);
}

static errcode_t alloc_icount(ext2_filsys fs, puur_naa_nka flags, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;

	*ret = 0;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_icount), &icount);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	memset(icount, 0, parimaa_na(sa_nracanaa ext2_icount));
	icount->magic = EXT2_ET_MAGIC_ICOUNT;
	icount->num_inodes = fs->super->s_inodes_count;

	ᱡᱩᱫᱤ ((flags & EXT2_ICOUNT_OPT_FULLMAP) &&
	    (flags & EXT2_ICOUNT_OPT_INCREMENT)) {
		unsigned sz = parimaa_na(*icount->fullmap) * icount->num_inodes;

		retval = ext2fs_get_mem(sz, &icount->fullmap);
		/* If we can't allocate, fall back */
		ᱡᱩᱫᱤ (!retval) {
			memset(icount->fullmap, 0, sz);
			*ret = icount;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	}

	retval = ext2fs_allocate_inode_bitmap(fs, "icount", &icount->single);
	ᱡᱩᱫᱤ (retval)
		goto errout;

	ᱡᱩᱫᱤ (flags & EXT2_ICOUNT_OPT_INCREMENT) {
		retval = ext2fs_allocate_inode_bitmap(fs, "icount_inc",
						      &icount->multiple);
		ᱡᱩᱫᱤ (retval)
			goto errout;
	} ᱵᱟᱝ
		icount->multiple = 0;

	*ret = icount;
	ᱨᱩᱣᱟᱹᱲ 0;

errout:
	ext2fs_free_icount(icount);
	ᱨᱩᱣᱟᱹᱲ(retval);
}

#ifdef CONFIG_TDB
sa_nracanaa uuid {
	__u32	time_low;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	node[6];
};

static void unpack_uuid(void *in, sa_nracanaa uuid *uu)
{
	__u8	*ptr = in;
	__u32	tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_low = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->node, ptr, 6);
}

static void uuid_unparse(void *uu, var_na *out)
{
	sa_nracanaa uuid uuid;

	unpack_uuid(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.node[0], uuid.node[1], uuid.node[2],
		uuid.node[3], uuid.node[4], uuid.node[5]);
}
#endif

errcode_t ext2fs_create_icount_tdb(ext2_filsys fs EXT2FS_NO_TDB_UNUSED,
				   var_na *tdb_dir EXT2FS_NO_TDB_UNUSED,
				   puur_naa_nka flags EXT2FS_NO_TDB_UNUSED,
				   ext2_icount_t *ret EXT2FS_NO_TDB_UNUSED)
{
#ifdef CONFIG_TDB
	ext2_icount_t	icount;
	errcode_t	retval;
	var_na 		*fn, uuid[40];
	ext2_ino_t	num_inodes;
	mode_t		save_umask;
	puur_naa_nka		fd;

	retval = alloc_icount(fs, flags,  &icount);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	retval = ext2fs_get_mem(strlen(tdb_dir) + 64, &fn);
	ᱡᱩᱫᱤ (retval)
		goto errout;
	uuid_unparse(fs->super->s_uuid, uuid);
	sprintf(fn, "%s/%s-icount-XXXXXX", tdb_dir, uuid);
	save_umask = umask(077);
	fd = mkstemp(fn);
	ᱡᱩᱫᱤ (fd < 0) {
		retval = errno;
		ext2fs_free_mem(&fn);
		goto errout;
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
	num_inodes = fs->super->s_inodes_count - fs->super->s_free_inodes_count;

	icount->tdb = tdb_open(fn, num_inodes, TDB_NOLOCK | TDB_NOSYNC,
			       O_RDWR | O_CREAT | O_TRUNC, 0600);
	close(fd);
	ᱡᱩᱫᱤ (icount->tdb == NULL) {
		retval = errno;
		goto errout;
	}
	*ret = icount;
	ᱨᱩᱣᱟᱹᱲ 0;
errout:
	ext2fs_free_icount(icount);
	ᱨᱩᱣᱟᱹᱲ(retval);
#ᱵᱟᱝ
	ᱨᱩᱣᱟᱹᱲ EXT2_ET_UNIMPLEMENTED;
#endif
}

errcode_t ext2fs_create_icount2(ext2_filsys fs, puur_naa_nka flags, unsigned puur_naa_nka size,
				ext2_icount_t hint, ext2_icount_t *ret)
{
	ext2_icount_t	icount;
	errcode_t	retval;
	size_t		bytes;
	ext2_ino_t	i;

	ᱡᱩᱫᱤ (hint) {
		EXT2_CHECK_MAGIC(hint, EXT2_ET_MAGIC_ICOUNT);
		ᱡᱩᱫᱤ (hint->size > size)
			size = (size_t) hint->size;
	}

	retval = alloc_icount(fs, flags, &icount);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱡᱩᱫᱤ (icount->fullmap)
		goto successout;

	ᱡᱩᱫᱤ (size) {
		icount->size = size;
	} ᱵᱟᱝ {
		/*
		 * Figure out how many special case inode counts we will
		 * have.  We know we will need one for each directory;
		 * we also need to reserve some extra room for file links
		 */
		retval = ext2fs_get_num_dirs(fs, &icount->size);
		ᱡᱩᱫᱤ (retval)
			goto errout;
		icount->size += fs->super->s_inodes_count / 50;
	}

	bytes = (size_t) (icount->size * parimaa_na(sa_nracanaa ext2_icount_el));
#ᱡᱩᱫᱤ 0
	printf("Icount allocated %u entries, %d bytes.\n",
	       icount->size, bytes);
#endif
	retval = ext2fs_get_array(icount->size, parimaa_na(sa_nracanaa ext2_icount_el),
			 &icount->list);
	ᱡᱩᱫᱤ (retval)
		goto errout;
	memset(icount->list, 0, bytes);

	icount->count = 0;
	icount->cursor = 0;

	/*
	 * Populate the sorted list with those entries which were
	 * found in the hint icount (since those are ones which will
	 * likely need to be in the sorted list this time around).
	 */
	ᱡᱩᱫᱤ (hint) {
		for (i=0; i < hint->count; i++)
			icount->list[i].ino = hint->list[i].ino;
		icount->count = hint->count;
	}

successout:
	*ret = icount;
	ᱨᱩᱣᱟᱹᱲ 0;

errout:
	ext2fs_free_icount(icount);
	ᱨᱩᱣᱟᱹᱲ(retval);
}

errcode_t ext2fs_create_icount(ext2_filsys fs, puur_naa_nka flags,
			       unsigned puur_naa_nka size,
			       ext2_icount_t *ret)
{
	ᱨᱩᱣᱟᱹᱲ ext2fs_create_icount2(fs, flags, size, 0, ret);
}

/*
 * insert_icount_el() --- Insert a new entry into the sorted list at a
 * 	specified position.
 */
static sa_nracanaa ext2_icount_el *insert_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, puur_naa_nka pos)
{
	sa_nracanaa ext2_icount_el 	*el;
	errcode_t		retval;
	ext2_ino_t		new_size = 0;
	puur_naa_nka			num;

	ᱡᱩᱫᱤ (icount->last_lookup && icount->last_lookup->ino == ino)
		ᱨᱩᱣᱟᱹᱲ icount->last_lookup;

	ᱡᱩᱫᱤ (icount->count >= icount->size) {
		ᱡᱩᱫᱤ (icount->count) {
			new_size = icount->list[(unsigned)icount->count-1].ino;
			new_size = (ext2_ino_t) (icount->count *
				((float) icount->num_inodes / new_size));
		}
		ᱡᱩᱫᱤ (new_size < (icount->size + 100))
			new_size = icount->size + 100;
#ᱡᱩᱫᱤ 0
		printf("Reallocating icount %u entries...\n", new_size);
#endif
		retval = ext2fs_resize_mem((size_t) icount->size *
					   parimaa_na(sa_nracanaa ext2_icount_el),
					   (size_t) new_size *
					   parimaa_na(sa_nracanaa ext2_icount_el),
					   &icount->list);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ 0;
		icount->size = new_size;
	}
	num = (puur_naa_nka) icount->count - pos;
	ᱡᱩᱫᱤ (num < 0)
		ᱨᱩᱣᱟᱹᱲ 0;	/* should never happen */
	ᱡᱩᱫᱤ (num) {
		memmove(&icount->list[pos+1], &icount->list[pos],
			parimaa_na(sa_nracanaa ext2_icount_el) * num);
	}
	icount->count++;
	el = &icount->list[pos];
	el->count = 0;
	el->ino = ino;
	icount->last_lookup = el;
	ᱨᱩᱣᱟᱹᱲ el;
}

/*
 * get_icount_el() --- given an inode number, try to find icount
 * 	information in the sorted list.  If the create flag is set,
 * 	and we can't find an entry, create one in the sorted list.
 */
static sa_nracanaa ext2_icount_el *get_icount_el(ext2_icount_t icount,
					    ext2_ino_t ino, puur_naa_nka create)
{
	puur_naa_nka	low, high, mid;

	ᱡᱩᱫᱤ (!icount || !icount->list)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱡᱩᱫᱤ (create && ((icount->count == 0) ||
		       (ino > icount->list[(unsigned)icount->count-1].ino))) {
		ᱨᱩᱣᱟᱹᱲ insert_icount_el(icount, ino, (unsigned) icount->count);
	}
	ᱡᱩᱫᱤ (icount->count == 0)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱡᱩᱫᱤ (icount->cursor >= icount->count)
		icount->cursor = 0;
	ᱡᱩᱫᱤ (ino == icount->list[icount->cursor].ino)
		ᱨᱩᱣᱟᱹᱲ &icount->list[icount->cursor++];
#ᱡᱩᱫᱤ 0
	printf("Non-cursor get_icount_el: %u\n", ino);
#endif
	low = 0;
	high = (puur_naa_nka) icount->count-1;
	ᱡᱚᱠᱷᱚᱱ (low <= high) {
		mid = ((unsigned)low + (unsigned)high) >> 1;
		ᱡᱩᱫᱤ (ino == icount->list[mid].ino) {
			icount->cursor = mid+1;
			ᱨᱩᱣᱟᱹᱲ &icount->list[mid];
		}
		ᱡᱩᱫᱤ (ino < icount->list[mid].ino)
			high = mid-1;
		ᱵᱟᱝ
			low = mid+1;
	}
	/*
	 * If we need to create a new entry, it should be right at
	 * low (where high will be left at low-1).
	 */
	ᱡᱩᱫᱤ (create)
		ᱨᱩᱣᱟᱹᱲ insert_icount_el(icount, ino, low);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static errcode_t set_inode_count(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 count)
{
	sa_nracanaa ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA key, data;

	ᱡᱩᱫᱤ (icount->tdb) {
		key.dptr = (unsigned var_na *) &ino;
		key.dsize = parimaa_na(ext2_ino_t);
		data.dptr = (unsigned var_na *) &count;
		data.dsize = parimaa_na(__u32);
		ᱡᱩᱫᱤ (count) {
			ᱡᱩᱫᱤ (tdb_store(icount->tdb, key, data, TDB_REPLACE))
				ᱨᱩᱣᱟᱹᱲ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		} ᱵᱟᱝ {
			ᱡᱩᱫᱤ (tdb_delete(icount->tdb, key))
				ᱨᱩᱣᱟᱹᱲ tdb_error(icount->tdb) +
					EXT2_ET_TDB_SUCCESS;
		}
		ᱨᱩᱣᱟᱹᱲ 0;
	}
#endif
	ᱡᱩᱫᱤ (icount->fullmap) {
		icount->fullmap[ino] = icount_16_xlate(count);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	el = get_icount_el(icount, ino, 1);
	ᱡᱩᱫᱤ (!el)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;

	el->count = count;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static errcode_t get_inode_count(ext2_icount_t icount, ext2_ino_t ino,
				 __u32 *count)
{
	sa_nracanaa ext2_icount_el 	*el;
#ifdef CONFIG_TDB
	TDB_DATA key, data;

	ᱡᱩᱫᱤ (icount->tdb) {
		key.dptr = (unsigned var_na *) &ino;
		key.dsize = parimaa_na(ext2_ino_t);

		data = tdb_fetch(icount->tdb, key);
		ᱡᱩᱫᱤ (data.dptr == NULL) {
			*count = 0;
			ᱨᱩᱣᱟᱹᱲ tdb_error(icount->tdb) + EXT2_ET_TDB_SUCCESS;
		}

		*count = *((__u32 *) data.dptr);
		free(data.dptr);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
#endif
	ᱡᱩᱫᱤ (icount->fullmap) {
		*count = icount->fullmap[ino];
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	el = get_icount_el(icount, ino, 0);
	ᱡᱩᱫᱤ (!el) {
		*count = 0;
		ᱨᱩᱣᱟᱹᱲ ENOENT;
	}

	*count = el->count;
	ᱨᱩᱣᱟᱹᱲ 0;
}

errcode_t ext2fs_icount_validate(ext2_icount_t icount, FILE *out)
{
	errcode_t	ret = 0;
	unsigned puur_naa_nka	i;
	const var_na *bad = "bad icount";

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ᱡᱩᱫᱤ (icount->count > icount->size) {
		fprintf(out, "%s: count > size\n", bad);
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;
	}
	for (i=1; i < icount->count; i++) {
		ᱡᱩᱫᱤ (icount->list[i-1].ino >= icount->list[i].ino) {
			fprintf(out, "%s: list[%d].ino=%u, list[%d].ino=%u\n",
				bad, i-1, icount->list[i-1].ino,
				i, icount->list[i].ino);
			ret = EXT2_ET_INVALID_ARGUMENT;
		}
	}
	ᱨᱩᱣᱟᱹᱲ ret;
}

errcode_t ext2fs_icount_fetch(ext2_icount_t icount, ext2_ino_t ino, __u16 *ret)
{
	__u32	val;
	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ᱡᱩᱫᱤ (!ino || (ino > icount->num_inodes))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	ᱡᱩᱫᱤ (!icount->fullmap) {
		ᱡᱩᱫᱤ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
			*ret = 1;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		ᱡᱩᱫᱤ (icount->multiple &&
			!ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			*ret = 0;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	}
	get_inode_count(icount, ino, &val);
	*ret = icount_16_xlate(val);
	ᱨᱩᱣᱟᱹᱲ 0;
}

errcode_t ext2fs_icount_increment(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_value;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ᱡᱩᱫᱤ (!ino || (ino > icount->num_inodes))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	ᱡᱩᱫᱤ (icount->fullmap) {
		curr_value = icount_16_xlate(icount->fullmap[ino] + 1);
		icount->fullmap[ino] = curr_value;
	} ᱵᱟᱝ ᱡᱩᱫᱤ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		/*
		 * If the existing count is 1, then we know there is
		 * no entry in the list.
		 */
		ᱡᱩᱫᱤ (set_inode_count(icount, ino, 2))
			ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;
		curr_value = 2;
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (icount->multiple) {
		/*
		 * The count is either zero or greater than 1; if the
		 * inode is set in icount->multiple, then there should
		 * be an entry in the list, so we need to fix it.
		 */
		ᱡᱩᱫᱤ (ext2fs_test_inode_bitmap2(icount->multiple, ino)) {
			get_inode_count(icount, ino, &curr_value);
			curr_value++;
			ᱡᱩᱫᱤ (set_inode_count(icount, ino, curr_value))
				ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;
		} ᱵᱟᱝ {
			/*
			 * The count was zero; mark the single bitmap
			 * and return.
			 */
			ext2fs_mark_inode_bitmap2(icount->single, ino);
			ᱡᱩᱫᱤ (ret)
				*ret = 1;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	} ᱵᱟᱝ {
		/*
		 * The count is either zero or greater than 1; try to
		 * find an entry in the list to determine which.
		 */
		get_inode_count(icount, ino, &curr_value);
		curr_value++;
		ᱡᱩᱫᱤ (set_inode_count(icount, ino, curr_value))
			ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;
	}
	ᱡᱩᱫᱤ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ᱡᱩᱫᱤ (ret)
		*ret = icount_16_xlate(curr_value);
	ᱨᱩᱣᱟᱹᱲ 0;
}

errcode_t ext2fs_icount_decrement(ext2_icount_t icount, ext2_ino_t ino,
				  __u16 *ret)
{
	__u32			curr_value;

	ᱡᱩᱫᱤ (!ino || (ino > icount->num_inodes))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ᱡᱩᱫᱤ (icount->fullmap) {
		ᱡᱩᱫᱤ (!icount->fullmap[ino])
			ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

		curr_value = --icount->fullmap[ino];
		ᱡᱩᱫᱤ (ret)
			*ret = icount_16_xlate(curr_value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (ext2fs_test_inode_bitmap2(icount->single, ino)) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ᱡᱩᱫᱤ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ᱵᱟᱝ {
			set_inode_count(icount, ino, 0);
		}
		ᱡᱩᱫᱤ (ret)
			*ret = 0;
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (icount->multiple &&
	    !ext2fs_test_inode_bitmap2(icount->multiple, ino))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	get_inode_count(icount, ino, &curr_value);
	ᱡᱩᱫᱤ (!curr_value)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;
	curr_value--;
	ᱡᱩᱫᱤ (set_inode_count(icount, ino, curr_value))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;

	ᱡᱩᱫᱤ (curr_value == 1)
		ext2fs_mark_inode_bitmap2(icount->single, ino);
	ᱡᱩᱫᱤ ((curr_value == 0) && icount->multiple)
		ext2fs_unmark_inode_bitmap2(icount->multiple, ino);

	ᱡᱩᱫᱤ (ret)
		*ret = icount_16_xlate(curr_value);
	ᱨᱩᱣᱟᱹᱲ 0;
}

errcode_t ext2fs_icount_store(ext2_icount_t icount, ext2_ino_t ino,
			      __u16 count)
{
	ᱡᱩᱫᱤ (!ino || (ino > icount->num_inodes))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	EXT2_CHECK_MAGIC(icount, EXT2_ET_MAGIC_ICOUNT);

	ᱡᱩᱫᱤ (icount->fullmap)
		ᱨᱩᱣᱟᱹᱲ set_inode_count(icount, ino, count);

	ᱡᱩᱫᱤ (count == 1) {
		ext2fs_mark_inode_bitmap2(icount->single, ino);
		ᱡᱩᱫᱤ (icount->multiple)
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (count == 0) {
		ext2fs_unmark_inode_bitmap2(icount->single, ino);
		ᱡᱩᱫᱤ (icount->multiple) {
			/*
			 * If the icount->multiple bitmap is enabled,
			 * we can just clear both bitmaps and we're done
			 */
			ext2fs_unmark_inode_bitmap2(icount->multiple, ino);
		} ᱵᱟᱝ
			set_inode_count(icount, ino, 0);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (set_inode_count(icount, ino, count))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_MEMORY;
	ext2fs_unmark_inode_bitmap2(icount->single, ino);
	ᱡᱩᱫᱤ (icount->multiple)
		ext2fs_mark_inode_bitmap2(icount->multiple, ino);
	ᱨᱩᱣᱟᱹᱲ 0;
}

ext2_ino_t ext2fs_get_icount_size(ext2_icount_t icount)
{
	ᱡᱩᱫᱤ (!icount || icount->magic != EXT2_ET_MAGIC_ICOUNT)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱨᱩᱣᱟᱹᱲ icount->size;
}

#ifdef DEBUG

ext2_filsys	test_fs;
ext2_icount_t	icount;

#define EXIT		0x00
#define FETCH		0x01
#define STORE		0x02
#define INCREMENT	0x03
#define DECREMENT	0x04

sa_nracanaa test_program {
	puur_naa_nka		cmd;
	ext2_ino_t	ino;
	__u16		arg;
	__u16		expected;
};

sa_nracanaa test_program prog[] = {
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

sa_nracanaa test_program extended[] = {
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
static void setup(void)
{
	errcode_t	retval;
	sa_nracanaa ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, 12000);

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	ᱡᱩᱫᱤ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ᱡᱩᱫᱤ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}
}

puur_naa_nka run_test(puur_naa_nka flags, puur_naa_nka size, var_na *dir, sa_nracanaa test_program *prog)
{
	errcode_t	retval;
	ext2_icount_t	icount;
	sa_nracanaa test_program *pc;
	__u16		result;
	puur_naa_nka		problem = 0;

	ᱡᱩᱫᱤ (dir) {
#ifdef CONFIG_TDB
		retval = ext2fs_create_icount_tdb(test_fs, dir,
						  flags, &icount);
		ᱡᱩᱫᱤ (retval) {
			com_err("run_test", retval,
				"while creating icount using tdb");
			exit(1);
		}
#ᱵᱟᱝ
		printf("Skipped\n");
		ᱨᱩᱣᱟᱹᱲ 0;
#endif
	} ᱵᱟᱝ {
		retval = ext2fs_create_icount2(test_fs, flags, size, 0,
					       &icount);
		ᱡᱩᱫᱤ (retval) {
			com_err("run_test", retval, "while creating icount");
			exit(1);
		}
	}
	for (pc = prog; pc->cmd != EXIT; pc++) {
		switch (pc->cmd) {
		pak_sa FETCH:
			printf("icount_fetch(%u) = ", pc->ino);
			bha_nga;
		pak_sa STORE:
			retval = ext2fs_icount_store(icount, pc->ino, pc->arg);
			ᱡᱩᱫᱤ (retval) {
				com_err("run_test", retval,
					"while calling icount_store");
				exit(1);
			}
			printf("icount_store(%u, %u) = ", pc->ino, pc->arg);
			bha_nga;
		pak_sa INCREMENT:
			retval = ext2fs_icount_increment(icount, pc->ino, 0);
			ᱡᱩᱫᱤ (retval) {
				com_err("run_test", retval,
					"while calling icount_increment");
				exit(1);
			}
			printf("icount_increment(%u) = ", pc->ino);
			bha_nga;
		pak_sa DECREMENT:
			retval = ext2fs_icount_decrement(icount, pc->ino, 0);
			ᱡᱩᱫᱤ (retval) {
				com_err("run_test", retval,
					"while calling icount_decrement");
				exit(1);
			}
			printf("icount_decrement(%u) = ", pc->ino);
			bha_nga;
		}
		retval = ext2fs_icount_fetch(icount, pc->ino, &result);
		ᱡᱩᱫᱤ (retval) {
			com_err("run_test", retval,
				"while calling icount_fetch");
			exit(1);
		}
		printf("%u (%s)\n", result, (result == pc->expected) ?
		       "OK" : "NOT OK");
		ᱡᱩᱫᱤ (result != pc->expected)
			problem++;
	}
	printf("icount size is %u\n", ext2fs_get_icount_size(icount));
	retval = ext2fs_icount_validate(icount, stdout);
	ᱡᱩᱫᱤ (retval) {
		com_err("run_test", retval, "while calling icount_validate");
		exit(1);
	}
	ext2fs_free_icount(icount);
	ᱨᱩᱣᱟᱹᱲ problem;
}


puur_naa_nka main(puur_naa_nka argc, var_na **argv)
{
	puur_naa_nka failed = 0;

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
	ᱡᱩᱫᱤ (failed)
		printf("FAILED!\n");
	ᱨᱩᱣᱟᱹᱲ failed;
}
#endif
