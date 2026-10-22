/*
URL: svn://svnanon.samba.org/samba/branches/SAMBA_4_0/source/lib/tdb/common
Rev: 23590
Last Changed Date: 2007-06-22 13:36:10 -0400 (Fri, 22 Jun 2007)
*/
 /*
   trivial database library - standalone version

   Copyright (C) Andrew Tridgell              1999-2005
   Copyright (C) Jeremy Allison               2000-2006
   Copyright (C) Paul `Rusty' Russell         2000

     ** NOTE! The following LGPL license applies to the tdb
     ** library. This does NOT imply that all of Samba is released
     ** under the LGPL

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifdef CONFIG_STAND_ALONE
#define HAVE_MMAP
#define HAVE_STRDUP
#define HAVE_SYS_MMAN_H
#define HAVE_UTIME_H
#define HAVE_UTIME
#endif
#ifndef __FreeBSD__
#define _XOPEN_SOURCE 600
#endif

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <string.h>
#ifdef HAVE_SYS_SELECT_H
#அந்தர்பாவ <sys/select.h>
#endif
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <time.h>
#ifdef HAVE_UTIME_H
#அந்தர்பாவ <utime.h>
#endif
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/file.h>
#அந்தர்பாவ <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#அந்தர்பாவ <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#இல்லையெனில்
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((வெறுமை *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
நிலையான எழுத்து *rep_strdup(மாறிலி எழுத்து *s)
{
	எழுத்து *ret;
	முழு length;

	எனில் (!s)
		திரும்பு வெற்று;
	length = strlen(s);
	ret = malloc(length + 1);
	எனில் (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	திரும்பு ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#எனில் (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#இல்லையெனில்
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#எனில் defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
வகைவரை முழு bool;
#endif

#அந்தர்பாவ "tdb.h"

நிலையான TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

வகைவரை u32 tdb_len_t;
வகைவரை u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned முழு)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*அளவு(கட்டமைப்பு list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_உச்சி (அளவு(கட்டமைப்பு tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_உச்சி(hash) (FREELIST_உச்சி + (BUCKET(hash)+1)*அளவு(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*அளவு(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_உச்சி(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(கட்டமைப்பு tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(கட்டமைப்பு tdb_header, sequence_எண்)
#define TDB_PAD_BYTE 0x42
#define TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#define TDB_LOG(x) tdb->log.log_fn x

/* lock offsets */
#define GLOBAL_LOCK      0
#define ACTIVE_LOCK      4
#define TRANSACTION_LOCK 8

/* free memory if the pointer is valid and zero the pointer */
#ifndef SAFE_FREE
#define SAFE_FREE(x) செய் { எனில் ((x) != வெற்று) {விடுவி(x); (x)=வெற்று;} } போது(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, அளவு(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
கட்டமைப்பு list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t சாவி_len; /* byte length of key */
	tdb_len_t data_len; /* byte length of data */
	u32 full_hash; /* the full 32 bit hash of the key */
	u32 magic;   /* try to catch errors */
	/* the following union is implied:
		union {
			char record[rec_len];
			struct {
				char key[key_len];
				char data[data_len];
			}
			u32 totalsize; (tailer)
		}
	*/
};


/* this is stored at the front of every database */
கட்டமைப்பு tdb_header {
	எழுத்து magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_எண்; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

கட்டமைப்பு tdb_lock_type {
	முழு list;
	u32 மொத்தம்;
	u32 ltype;
};

கட்டமைப்பு tdb_traverse_lock {
	கட்டமைப்பு tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	முழு lock_rw;
};


கட்டமைப்பு tdb_methods {
	முழு (*tdb_read)(கட்டமைப்பு tdb_context *, tdb_off_t , வெறுமை *, tdb_len_t , முழு );
	முழு (*tdb_write)(கட்டமைப்பு tdb_context *, tdb_off_t, மாறிலி வெறுமை *, tdb_len_t);
	வெறுமை (*next_hash_chain)(கட்டமைப்பு tdb_context *, u32 *);
	முழு (*tdb_oob)(கட்டமைப்பு tdb_context *, tdb_off_t , முழு );
	முழு (*tdb_expand_file)(கட்டமைப்பு tdb_context *, tdb_off_t , tdb_off_t );
	முழு (*tdb_brlock)(கட்டமைப்பு tdb_context *, tdb_off_t , முழு, முழு, முழு, size_t);
};

கட்டமைப்பு tdb_context {
	எழுத்து *name; /* the name of the database */
	வெறுமை *map_ptr; /* where it is currently mapped */
	முழு fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	முழு read_only; /* opened read-only */
	முழு traverse_read; /* read-only traversal */
	கட்டமைப்பு tdb_lock_type global_lock;
	முழு இலக்கம்_lockrecs;
	கட்டமைப்பு tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	எண்ணிக்கை TDB_ERROR ecode; /* error code for last tdb error */
	கட்டமைப்பு tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	கட்டமைப்பு tdb_traverse_lock travlocks; /* current traversal locks */
	கட்டமைப்பு tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	கட்டமைப்பு tdb_logging_context log;
	unsigned முழு (*hash_fn)(TDB_DATA *சாவி);
	முழு open_flags; /* flags used in the open - needed by reopen */
	unsigned முழு இலக்கம்_locks; /* number of chain locks held */
	மாறிலி கட்டமைப்பு tdb_methods *methods;
	கட்டமைப்பு tdb_transaction *transaction;
	முழு page_size;
	முழு max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
நிலையான முழு tdb_munmap(கட்டமைப்பு tdb_context *tdb);
நிலையான வெறுமை tdb_mmap(கட்டமைப்பு tdb_context *tdb);
நிலையான முழு tdb_lock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype);
முழு tdb_lock_nonblock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype);
நிலையான முழு tdb_unlock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype);
நிலையான முழு tdb_brlock(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, முழு rw_type, முழு lck_type, முழு probe, size_t len);
நிலையான முழு tdb_transaction_lock(கட்டமைப்பு tdb_context *tdb, முழு ltype);
நிலையான முழு tdb_transaction_unlock(கட்டமைப்பு tdb_context *tdb);
நிலையான முழு tdb_brlock_upgrade(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, size_t len);
நிலையான முழு tdb_write_lock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off);
நிலையான முழு tdb_write_unlock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off);
நிலையான முழு tdb_ofs_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
நிலையான முழு tdb_ofs_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
நிலையான வெறுமை *tdb_convert(வெறுமை *buf, u32 size);
நிலையான முழு tdb_free(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec);
நிலையான tdb_off_t tdb_allocate(கட்டமைப்பு tdb_context *tdb, tdb_len_t length, கட்டமைப்பு list_struct *rec);
நிலையான முழு tdb_ofs_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
நிலையான முழு tdb_ofs_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
நிலையான முழு tdb_lock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off);
நிலையான முழு tdb_unlock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off);
நிலையான முழு tdb_rec_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec);
நிலையான முழு tdb_rec_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec);
நிலையான முழு tdb_do_delete(கட்டமைப்பு tdb_context *tdb, tdb_off_t rec_ptr, கட்டமைப்பு list_struct *rec);
நிலையான unsigned எழுத்து *tdb_alloc_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
நிலையான முழு tdb_parse_data(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி,
		   tdb_off_t offset, tdb_len_t len,
		   முழு (*parser)(TDB_DATA சாவி, TDB_DATA data,
				 வெறுமை *private_data),
		   வெறுமை *private_data);
நிலையான tdb_off_t tdb_find_lock_hash(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash, முழு locktype,
			   கட்டமைப்பு list_struct *rec);
நிலையான வெறுமை tdb_io_init(கட்டமைப்பு tdb_context *tdb);
நிலையான முழு tdb_expand(கட்டமைப்பு tdb_context *tdb, tdb_off_t size);
நிலையான முழு tdb_rec_free_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t off,
		      கட்டமைப்பு list_struct *rec);


/* file: error.c */

எண்ணிக்கை TDB_ERROR tdb_error(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->ecode;
}

நிலையான கட்டமைப்பு tdb_errname {
	எண்ணிக்கை TDB_ERROR ecode; மாறிலி எழுத்து *estring;
} emap[] = { {TDB_SUCCESS, "Success"},
	     {TDB_ERR_CORRUPT, "Corrupt database"},
	     {TDB_ERR_IO, "IO Error"},
	     {TDB_ERR_LOCK, "Locking error"},
	     {TDB_ERR_OOM, "Out of memory"},
	     {TDB_ERR_EXISTS, "Record exists"},
	     {TDB_ERR_NOLOCK, "Lock exists on other keys"},
	     {TDB_ERR_EINVAL, "Invalid parameter"},
	     {TDB_ERR_NOEXIST, "Record does not exist"},
	     {TDB_ERR_RDONLY, "write not permitted"} };

/* Error string for the last tdb error */
மாறிலி எழுத்து *tdb_errorstr(கட்டமைப்பு tdb_context *tdb)
{
	u32 i;
	க்கு (i = 0; i < அளவு(emap) / அளவு(கட்டமைப்பு tdb_errname); i++)
		எனில் (tdb->ecode == emap[i].ecode)
			திரும்பு emap[i].estring;
	திரும்பு "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
முழு tdb_brlock(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset,
	       முழு rw_type, முழு lck_type, முழு probe, size_t len)
{
	கட்டமைப்பு flock fl;
	முழு ret;

	எனில் (tdb->flags & TDB_NOLOCK) {
		திரும்பு 0;
	}

	எனில் ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		திரும்பு -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	செய் {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} போது (ret == -1 && errno == EINTR);

	எனில் (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		எனில் (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (முழு)len));
		}
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	திரும்பு 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
முழு tdb_brlock_upgrade(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, size_t len)
{
	முழு மொத்தம் = 1000;
	போது (மொத்தம்--) {
		கட்டமைப்பு timeval tv;
		எனில் (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			திரும்பு 0;
		}
		எனில் (errno != EDEADLK) {
			நிறுத்து;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, வெற்று, வெற்று, வெற்று, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	திரும்பு -1;
}


/* lock a list in the database. list -1 is the alloc list */
நிலையான முழு _tdb_lock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype, முழு op)
{
	கட்டமைப்பு tdb_lock_type *new_lck;
	முழு i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	எனில் (tdb->global_lock.மொத்தம் &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		திரும்பு 0;
	}

	எனில் (tdb->global_lock.மொத்தம்) {
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (list < -1 || list >= (முழு)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		திரும்பு -1;
	}
	எனில் (tdb->flags & TDB_NOLOCK)
		திரும்பு 0;

	க்கு (i=0; i<tdb->இலக்கம்_lockrecs; i++) {
		எனில் (tdb->lockrecs[i].list == list) {
			எனில் (tdb->lockrecs[i].மொத்தம் == 0) {
				/*
				 * Can't happen, see tdb_unlock(). It should
				 * be an assert.
				 */
				TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock: "
					 "lck->count == 0 for list %d", list));
			}
			/*
			 * Just increment the in-memory struct, posix locks
			 * don't stack.
			 */
			tdb->lockrecs[i].மொத்தம்++;
			திரும்பு 0;
		}
	}

	new_lck = (கட்டமைப்பு tdb_lock_type *)realloc(
		tdb->lockrecs,
		அளவு(*tdb->lockrecs) * (tdb->இலக்கம்_lockrecs+1));
	எனில் (new_lck == வெற்று) {
		errno = ENOMEM;
		திரும்பு -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	எனில் (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_உச்சி+4*list, ltype, op,
				     0, 1)) {
		திரும்பு -1;
	}

	tdb->இலக்கம்_locks++;

	tdb->lockrecs[tdb->இலக்கம்_lockrecs].list = list;
	tdb->lockrecs[tdb->இலக்கம்_lockrecs].மொத்தம் = 1;
	tdb->lockrecs[tdb->இலக்கம்_lockrecs].ltype = ltype;
	tdb->இலக்கம்_lockrecs += 1;

	திரும்பு 0;
}

/* lock a list in the database. list -1 is the alloc list */
முழு tdb_lock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype)
{
	முழு ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	எனில் (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	திரும்பு ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
முழு tdb_lock_nonblock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype)
{
	திரும்பு _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
முழு tdb_unlock(கட்டமைப்பு tdb_context *tdb, முழு list, முழு ltype)
{
	முழு ret = -1;
	முழு i;
	கட்டமைப்பு tdb_lock_type *lck = வெற்று;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	எனில் (tdb->global_lock.மொத்தம் &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		திரும்பு 0;
	}

	எனில் (tdb->global_lock.மொத்தம்) {
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (tdb->flags & TDB_NOLOCK)
		திரும்பு 0;

	/* Sanity checks */
	எனில் (list < -1 || list >= (முழு)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		திரும்பு ret;
	}

	க்கு (i=0; i<tdb->இலக்கம்_lockrecs; i++) {
		எனில் (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			நிறுத்து;
		}
	}

	எனில் ((lck == வெற்று) || (lck->மொத்தம் == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		திரும்பு -1;
	}

	எனில் (lck->மொத்தம் > 1) {
		lck->மொத்தம்--;
		திரும்பு 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	எனில் (mark_lock) {
		ret = 0;
	} இல்லையெனில் {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_உச்சி+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->இலக்கம்_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	எனில் (tdb->இலக்கம்_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->இலக்கம்_lockrecs-1];
	}
	tdb->இலக்கம்_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	எனில் (tdb->இலக்கம்_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	எனில் (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	திரும்பு ret;
}

/*
  get the transaction lock
 */
முழு tdb_transaction_lock(கட்டமைப்பு tdb_context *tdb, முழு ltype)
{
	எனில் (tdb->have_transaction_lock || tdb->global_lock.மொத்தம்) {
		திரும்பு 0;
	}
	எனில் (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		திரும்பு -1;
	}
	tdb->have_transaction_lock = 1;
	திரும்பு 0;
}

/*
  release the transaction lock
 */
முழு tdb_transaction_unlock(கட்டமைப்பு tdb_context *tdb)
{
	முழு ret;
	எனில் (!tdb->have_transaction_lock) {
		திரும்பு 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	எனில் (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	திரும்பு ret;
}




/* lock/unlock entire database */
நிலையான முழு _tdb_lockall(கட்டமைப்பு tdb_context *tdb, முழு ltype, முழு op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	எனில் (tdb->read_only || tdb->traverse_read)
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);

	எனில் (tdb->global_lock.மொத்தம் && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.மொத்தம்++;
		திரும்பு 0;
	}

	எனில் (tdb->global_lock.மொத்தம்) {
		/* a global lock of a different type exists */
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (tdb->இலக்கம்_locks != 0) {
		/* can't combine global and chain locks */
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_உச்சி, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		எனில் (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		திரும்பு -1;
	}

	tdb->global_lock.மொத்தம் = 1;
	tdb->global_lock.ltype = ltype;

	திரும்பு 0;
}



/* unlock entire db */
நிலையான முழு _tdb_unlockall(கட்டமைப்பு tdb_context *tdb, முழு ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	எனில் (tdb->read_only || tdb->traverse_read) {
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.மொத்தம் == 0) {
		திரும்பு TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	எனில் (tdb->global_lock.மொத்தம் > 1) {
		tdb->global_lock.மொத்தம்--;
		திரும்பு 0;
	}

	எனில் (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_உச்சி, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		திரும்பு -1;
	}

	tdb->global_lock.மொத்தம் = 0;
	tdb->global_lock.ltype = 0;

	திரும்பு 0;
}

/* lock entire database with write lock */
முழு tdb_lockall(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
முழு tdb_lockall_mark(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
முழு tdb_lockall_unmark(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
முழு tdb_lockall_nonblock(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
முழு tdb_unlockall(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
முழு tdb_lockall_read(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
முழு tdb_lockall_read_nonblock(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
முழு tdb_unlockall_read(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
முழு tdb_chainlock(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_lock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
முழு tdb_chainlock_nonblock(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
முழு tdb_chainlock_mark(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_lock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
முழு tdb_chainlock_unmark(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_unlock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_WRLCK | TDB_MARK_LOCK);
}

முழு tdb_chainunlock(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_unlock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_WRLCK);
}

முழு tdb_chainlock_read(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_lock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_RDLCK);
}

முழு tdb_chainunlock_read(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	திரும்பு tdb_unlock(tdb, BUCKET(tdb->hash_fn(&சாவி)), F_RDLCK);
}



/* record lock stops delete underneath */
முழு tdb_lock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off)
{
	திரும்பு off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
முழு tdb_write_lock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off)
{
	கட்டமைப்பு tdb_traverse_lock *i;
	க்கு (i = &tdb->travlocks; i; i = i->next)
		எனில் (i->off == off)
			திரும்பு -1;
	திரும்பு tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
முழு tdb_write_unlock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off)
{
	திரும்பு tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
முழு tdb_unlock_record(கட்டமைப்பு tdb_context *tdb, tdb_off_t off)
{
	கட்டமைப்பு tdb_traverse_lock *i;
	u32 மொத்தம் = 0;

	எனில் (off == 0)
		திரும்பு 0;
	க்கு (i = &tdb->travlocks; i; i = i->next)
		எனில் (i->off == off)
			மொத்தம்++;
	திரும்பு (மொத்தம் == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
நிலையான முழு tdb_oob(கட்டமைப்பு tdb_context *tdb, tdb_off_t len, முழு probe)
{
	கட்டமைப்பு stat st;
	எனில் (len <= tdb->map_size)
		திரும்பு 0;
	எனில் (tdb->flags & TDB_INTERNAL) {
		எனில் (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (முழு)len, (முழு)tdb->map_size));
		}
		திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	எனில் (fstat(tdb->fd, &st) == -1) {
		திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	எனில் (st.st_size < (off_t)len) {
		எனில் (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (முழு)len, (முழு)st.st_size));
		}
		திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	எனில் (tdb_munmap(tdb) == -1)
		திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	திரும்பு 0;
}

/* write a lump of data at a specified offset */
நிலையான முழு tdb_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t off,
		     மாறிலி வெறுமை *buf, tdb_len_t len)
{
	எனில் (len == 0) {
		திரும்பு 0;
	}

	எனில் (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		திரும்பு -1;
	}

	எனில் (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		திரும்பு -1;

	எனில் (tdb->map_ptr) {
		memcpy(off + (எழுத்து *)tdb->map_ptr, buf, len);
	} இல்லையெனில் எனில் (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	திரும்பு 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
வெறுமை *tdb_convert(வெறுமை *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	க்கு (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	திரும்பு buf;
}


/* read a lump of data at a specified offset, maybe convert */
நிலையான முழு tdb_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t off, வெறுமை *buf,
		    tdb_len_t len, முழு cv)
{
	எனில் (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		திரும்பு -1;
	}

	எனில் (tdb->map_ptr) {
		memcpy(buf, off + (எழுத்து *)tdb->map_ptr, len);
	} இல்லையெனில் {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		எனில் (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (முழு)off, (முழு)len, (முழு)ret, strerror(errno),
				 (முழு)tdb->map_size));
			திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	எனில் (cv) {
		tdb_convert(buf, len);
	}
	திரும்பு 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
நிலையான வெறுமை tdb_next_hash_chain(கட்டமைப்பு tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	எனில் (tdb->map_ptr) {
		க்கு (;h < tdb->header.hash_size;h++) {
			எனில் (0 != *(u32 *)(TDB_HASH_உச்சி(h) + (unsigned எழுத்து *)tdb->map_ptr)) {
				நிறுத்து;
			}
		}
	} இல்லையெனில் {
		u32 off=0;
		க்கு (;h < tdb->header.hash_size;h++) {
			எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(h), &off) != 0 || off != 0) {
				நிறுத்து;
			}
		}
	}
	(*chain) = h;
}


முழு tdb_munmap(கட்டமைப்பு tdb_context *tdb)
{
	எனில் (tdb->flags & TDB_INTERNAL)
		திரும்பு 0;

#ifdef HAVE_MMAP
	எனில் (tdb->map_ptr) {
		முழு ret = munmap(tdb->map_ptr, tdb->real_map_size);
		எனில் (ret != 0)
			திரும்பு ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = வெற்று;
	திரும்பு 0;
}

வெறுமை tdb_mmap(கட்டமைப்பு tdb_context *tdb)
{
	எனில் (tdb->flags & TDB_INTERNAL)
		திரும்பு;

#ifdef HAVE_MMAP
	எனில் (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(வெற்று, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		எனில் (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = வெற்று;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} இல்லையெனில் {
		tdb->map_ptr = வெற்று;
	}
#இல்லையெனில்
	tdb->map_ptr = வெற்று;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
நிலையான முழு tdb_expand_file(கட்டமைப்பு tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	எழுத்து buf[1024];

	எனில் (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		திரும்பு -1;
	}

	எனில் (ftruncate(tdb->fd, size+addition) == -1) {
		எழுத்து b = 0;
		எனில் (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			திரும்பு -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, அளவு(buf));
	போது (addition) {
		முழு n = addition>அளவு(buf)?அளவு(buf):addition;
		முழு ret = pwrite(tdb->fd, buf, n, size);
		எனில் (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			திரும்பு -1;
		}
		addition -= n;
		size += n;
	}
	திரும்பு 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
முழு tdb_expand(கட்டமைப்பு tdb_context *tdb, tdb_off_t size)
{
	கட்டமைப்பு list_struct rec;
	tdb_off_t offset;

	எனில் (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		திரும்பு -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	எனில் (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	எனில் (!(tdb->flags & TDB_INTERNAL)) {
		எனில் (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			செல் fail;
	}

	tdb->map_size += size;

	எனில் (tdb->flags & TDB_INTERNAL) {
		எழுத்து *new_map_ptr = (எழுத்து *)realloc(tdb->map_ptr,
						    tdb->map_size);
		எனில் (!new_map_ptr) {
			tdb->map_size -= size;
			செல் fail;
		}
		tdb->map_ptr = new_map_ptr;
	} இல்லையெனில் {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',அளவு(rec));
	rec.rec_len = size - அளவு(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	எனில் (tdb_free(tdb, offset, &rec) == -1)
		செல் fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு -1;
}

/* read/write a tdb_off_t */
முழு tdb_ofs_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	திரும்பு tdb->methods->tdb_read(tdb, offset, (எழுத்து*)d, அளவு(*d), DOCONV());
}

முழு tdb_ofs_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	திரும்பு tdb->methods->tdb_write(tdb, offset, CONVERT(off), அளவு(*d));
}


/* read a lump of data, allocating the space for it */
unsigned எழுத்து *tdb_alloc_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned எழுத்து *buf;

	/* some systems don't like zero length malloc */
	எனில் (len == 0) {
		len = 1;
	}

	எனில் (!(buf = (unsigned எழுத்து *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		திரும்பு TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	எனில் (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		திரும்பு வெற்று;
	}
	திரும்பு buf;
}

/* Give a piece of tdb data to a parser */

முழு tdb_parse_data(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி,
		   tdb_off_t offset, tdb_len_t len,
		   முழு (*parser)(TDB_DATA சாவி, TDB_DATA data,
				 வெறுமை *private_data),
		   வெறுமை *private_data)
{
	TDB_DATA data;
	முழு result;

	data.dsize = len;

	எனில் ((tdb->transaction == வெற்று) && (tdb->map_ptr != வெற்று)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		எனில் (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			திரும்பு -1;
		}
		data.dptr = offset + (unsigned எழுத்து *)tdb->map_ptr;
		திரும்பு parser(சாவி, data, private_data);
	}

	எனில் (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		திரும்பு -1;
	}

	result = parser(சாவி, data, private_data);
	விடுவி(data.dptr);
	திரும்பு result;
}

/* read/write a record */
முழு tdb_rec_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec)
{
	எனில் (tdb->methods->tdb_read(tdb, offset, rec, அளவு(*rec),DOCONV()) == -1)
		திரும்பு -1;
	எனில் (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		திரும்பு TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	திரும்பு tdb->methods->tdb_oob(tdb, rec->next+அளவு(*rec), 0);
}

முழு tdb_rec_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec)
{
	கட்டமைப்பு list_struct r = *rec;
	திரும்பு tdb->methods->tdb_write(tdb, offset, CONVERT(r), அளவு(r));
}

நிலையான மாறிலி கட்டமைப்பு tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_next_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
வெறுமை tdb_io_init(கட்டமைப்பு tdb_context *tdb)
{
	tdb->methods = &io_methods;
}

/* file: transaction.c */

/*
  transaction design:

  - only allow a single transaction at a time per database. This makes
    using the transaction API simpler, as otherwise the caller would
    have to cope with temporary failures in transactions that conflict
    with other current transactions

  - keep the transaction recovery information in the same file as the
    database, using a special 'transaction recovery' record pointed at
    by the header. This removes the need for extra journal files as
    used by some other databases

  - dynamically allocated the transaction recover record, re-using it
    for subsequent transactions. If a larger record is needed then
    tdb_free() the old record to place it on the normal tdb freelist
    before allocating the new record

  - during transactions, keep a linked list of writes all that have
    been performed by intercepting all tdb_write() calls. The hooked
    transaction versions of tdb_read() and tdb_write() check this
    linked list and try to use the elements of the list in preference
    to the real database.

  - don't allow any locks to be held when a transaction starts,
    otherwise we can end up with deadlock (plus lack of lock nesting
    in posix locks would mean the lock is lost)

  - if the caller gains a lock during the transaction but doesn't
    release it then fail the commit

  - allow for nested calls to tdb_transaction_start(), re-using the
    existing transaction record. If the inner transaction is cancelled
    then a subsequent commit will fail

  - keep a mirrored copy of the tdb hash chain heads to allow for the
    fast hash heads scan on traverse, updating the mirrored copy in
    the transaction version of tdb_write

  - allow callers to mix transaction and non-transaction use of tdb,
    although once a transaction is started then an exclusive lock is
    gained until the transaction is committed or cancelled

  - the commit strategy involves first saving away all modified data
    into a linearised buffer in the transaction recovery area, then
    marking the transaction recovery area with a magic value to
    indicate a valid recovery record. In total 4 fsync/msync calls are
    needed per commit to prevent race conditions. It might be possible
    to reduce this to 3 or even 2 with some more work.

  - check for a valid recovery record on open of the tdb, while the
    global lock is held. Automatically recover from the transaction
    recovery area if needed, then continue with the open as
    usual. This allows for smooth crash recovery with no administrator
    intervention.

  - if TDB_NOSYNC is passed to flags in tdb_open then transactions are
    still available, but no transaction recovery area is used and no
    fsync/msync calls are made.

*/

கட்டமைப்பு tdb_transaction_el {
	கட்டமைப்பு tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned எழுத்து *data;
};

/*
  hold the context of any current transaction
*/
கட்டமைப்பு tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	மாறிலி கட்டமைப்பு tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	கட்டமைப்பு tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	முழு transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	முழு nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
நிலையான முழு transaction_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t off, வெறுமை *buf,
			    tdb_len_t len, முழு cv)
{
	கட்டமைப்பு tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	க்கு (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		எனில் (off+len <= el->offset) {
			தொடர்;
		}
		எனில் (off >= el->offset + el->length) {
			தொடர்;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		எனில் (off < el->offset) {
			partial = el->offset - off;
			எனில் (transaction_read(tdb, off, buf, partial, cv) != 0) {
				செல் fail;
			}
			len -= partial;
			off += partial;
			buf = (வெறுமை *)(partial + (எழுத்து *)buf);
		}
		எனில் (off + len <= el->offset + el->length) {
			partial = len;
		} இல்லையெனில் {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		எனில் (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (வெறுமை *)(partial + (எழுத்து *)buf);

		எனில் (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			செல் fail;
		}

		திரும்பு 0;
	}

	/* its not in the transaction elements - do a real read */
	திரும்பு tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	திரும்பு -1;
}


/*
  write while in a transaction
*/
நிலையான முழு transaction_write(கட்டமைப்பு tdb_context *tdb, tdb_off_t off,
			     மாறிலி வெறுமை *buf, tdb_len_t len)
{
	கட்டமைப்பு tdb_transaction_el *el, *best_el=வெற்று;

	எனில் (len == 0) {
		திரும்பு 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	எனில் (len == அளவு(tdb_off_t) && off >= FREELIST_உச்சி &&
	    off < FREELIST_உச்சி+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_உச்சி) / அளவு(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	க்கு (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		எனில் (best_el == வெற்று && off == el->offset+el->length) {
			best_el = el;
		}

		எனில் (off+len <= el->offset) {
			தொடர்;
		}
		எனில் (off >= el->offset + el->length) {
			தொடர்;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		எனில் (off < el->offset) {
			partial = el->offset - off;
			எனில் (transaction_write(tdb, off, buf, partial) != 0) {
				செல் fail;
			}
			len -= partial;
			off += partial;
			buf = (மாறிலி வெறுமை *)(partial + (மாறிலி எழுத்து *)buf);
		}
		எனில் (off + len <= el->offset + el->length) {
			partial = len;
		} இல்லையெனில் {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (மாறிலி வெறுமை *)(partial + (மாறிலி எழுத்து *)buf);

		எனில் (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			செல் fail;
		}

		திரும்பு 0;
	}

	/* see if we can append the new entry to an existing entry */
	எனில் (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned எழுத்து *data = best_el->data;
		el = best_el;
		el->data = (unsigned எழுத்து *)realloc(el->data,
						    el->length + len);
		எனில் (el->data == வெற்று) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			திரும்பு -1;
		}
		எனில் (buf) {
			memcpy(el->data + el->length, buf, len);
		} இல்லையெனில் {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		திரும்பு 0;
	}

	/* add a new entry at the end of the list */
	el = (கட்டமைப்பு tdb_transaction_el *)malloc(அளவு(*el));
	எனில் (el == வெற்று) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		திரும்பு -1;
	}
	el->next = வெற்று;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned எழுத்து *)malloc(len);
	எனில் (el->data == வெற்று) {
		விடுவி(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		திரும்பு -1;
	}
	எனில் (buf) {
		memcpy(el->data, buf, len);
	} இல்லையெனில் {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	எனில் (el->prev) {
		el->prev->next = el;
	} இல்லையெனில் {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	திரும்பு 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	திரும்பு -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
நிலையான வெறுமை transaction_next_hash_chain(கட்டமைப்பு tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	க்கு (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		எனில் (0 != tdb->transaction->hash_heads[h+1]) {
			நிறுத்து;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
நிலையான முழு transaction_oob(கட்டமைப்பு tdb_context *tdb, tdb_off_t len,
			   முழு probe EXT2FS_ATTR((unused)))
{
	எனில் (len <= tdb->map_size) {
		திரும்பு 0;
	}
	திரும்பு TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
நிலையான முழு transaction_expand_file(கட்டமைப்பு tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	எனில் (transaction_write(tdb, size, வெற்று, addition) != 0) {
		திரும்பு -1;
	}

	திரும்பு 0;
}

/*
  brlock during a transaction - ignore them
*/
நிலையான முழு transaction_brlock(கட்டமைப்பு tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      முழு rw_type EXT2FS_ATTR((unused)),
			      முழு lck_type EXT2FS_ATTR((unused)),
			      முழு probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	திரும்பு 0;
}

நிலையான மாறிலி கட்டமைப்பு tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_next_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
முழு tdb_transaction_start(கட்டமைப்பு tdb_context *tdb)
{
	/* some sanity checks */
	எனில் (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		திரும்பு -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	எனில் (tdb->transaction != வெற்று) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		திரும்பு 0;
	}

	எனில் (tdb->இலக்கம்_locks != 0 || tdb->global_lock.மொத்தம்) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		திரும்பு -1;
	}

	எனில் (tdb->travlocks.next != வெற்று) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		திரும்பு -1;
	}

	tdb->transaction = (கட்டமைப்பு tdb_transaction *)
		calloc(அளவு(கட்டமைப்பு tdb_transaction), 1);
	எனில் (tdb->transaction == வெற்று) {
		tdb->ecode = TDB_ERR_OOM;
		திரும்பு -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	எனில் (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		திரும்பு -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	எனில் (tdb_brlock(tdb, FREELIST_உச்சி, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		செல் fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, அளவு(u32));
	எனில் (tdb->transaction->hash_heads == வெற்று) {
		tdb->ecode = TDB_ERR_OOM;
		செல் fail;
	}
	எனில் (tdb->methods->tdb_read(tdb, FREELIST_உச்சி, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		செல் fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	tdb->transaction->old_map_size = tdb->map_size;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	எனில் (transaction_write(tdb, FREELIST_உச்சி, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		செல் fail;
	}

	திரும்பு 0;

fail:
	tdb_brlock(tdb, FREELIST_உச்சி, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	திரும்பு -1;
}


/*
  cancel the current transaction
*/
முழு tdb_transaction_cancel(கட்டமைப்பு tdb_context *tdb)
{
	எனில் (tdb->transaction == வெற்று) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		திரும்பு -1;
	}

	எனில் (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		திரும்பு 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	போது (tdb->transaction->elements) {
		கட்டமைப்பு tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		விடுவி(el->data);
		விடுவி(el);
	}

	/* remove any global lock created during the transaction */
	எனில் (tdb->global_lock.மொத்தம் != 0) {
		tdb_brlock(tdb, FREELIST_உச்சி, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.மொத்தம் = 0;
	}

	/* remove any locks created during the transaction */
	எனில் (tdb->இலக்கம்_locks != 0) {
		முழு i;
		க்கு (i=0;i<tdb->இலக்கம்_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_உச்சி+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->இலக்கம்_locks = 0;
		tdb->இலக்கம்_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_உச்சி, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	திரும்பு 0;
}

/*
  sync to disk
*/
நிலையான முழு transaction_sync(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	எனில் (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		திரும்பு -1;
	}
#எனில் defined(HAVE_MSYNC) && defined(MS_SYNC)
	எனில் (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		எனில் (msync(moffset + (எழுத்து *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			திரும்பு -1;
		}
	}
#endif
	திரும்பு 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
நிலையான tdb_len_t tdb_recovery_size(கட்டமைப்பு tdb_context *tdb)
{
	கட்டமைப்பு tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = அளவு(u32);
	க்கு (el=tdb->transaction->elements;el;el=el->next) {
		எனில் (el->offset >= tdb->transaction->old_map_size) {
			தொடர்;
		}
		recovery_size += 2*அளவு(tdb_off_t) + el->length;
	}

	திரும்பு recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
நிலையான முழு tdb_recovery_allocate(கட்டமைப்பு tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	கட்டமைப்பு list_struct rec;
	மாறிலி கட்டமைப்பு tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	எனில் (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		திரும்பு -1;
	}

	rec.rec_len = 0;

	எனில் (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, அளவு(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		திரும்பு -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	எனில் (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		திரும்பு 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	எனில் (recovery_head != 0) {
		எனில் (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			திரும்பு -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(அளவு(rec) + *recovery_size, tdb->page_size) - அளவு(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	எனில் (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     அளவு(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		திரும்பு -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	எனில் (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, அளவு(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		திரும்பு -1;
	}

	திரும்பு 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
நிலையான முழு transaction_setup_recovery(கட்டமைப்பு tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	கட்டமைப்பு tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned எழுத்து *data, *p;
	மாறிலி கட்டமைப்பு tdb_methods *methods = tdb->transaction->io_methods;
	கட்டமைப்பு list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	எனில் (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		திரும்பு -1;
	}

	data = (unsigned எழுத்து *)malloc(recovery_size + அளவு(*rec));
	எனில் (data == வெற்று) {
		tdb->ecode = TDB_ERR_OOM;
		திரும்பு -1;
	}

	rec = (கட்டமைப்பு list_struct *)data;
	memset(rec, 0, அளவு(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->சாவி_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + அளவு(*rec);
	க்கு (el=tdb->transaction->elements;el;el=el->next) {
		எனில் (el->offset >= old_map_size) {
			தொடர்;
		}
		எனில் (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			விடுவி(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			திரும்பு -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		எனில் (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		எனில் (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			விடுவி(data);
			tdb->ecode = TDB_ERR_IO;
			திரும்பு -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = அளவு(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	எனில் (methods->tdb_write(tdb, recovery_offset, data, அளவு(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		விடுவி(data);
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	எனில் (transaction_sync(tdb, recovery_offset, அளவு(*rec) + recovery_size) == -1) {
		விடுவி(data);
		திரும்பு -1;
	}

	விடுவி(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(கட்டமைப்பு list_struct, magic);

	எனில் (methods->tdb_write(tdb, *magic_offset, &magic, அளவு(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	/* ensure the recovery magic marker is on disk */
	எனில் (transaction_sync(tdb, *magic_offset, அளவு(magic)) == -1) {
		திரும்பு -1;
	}

	திரும்பு 0;
}

/*
  commit the current transaction
*/
முழு tdb_transaction_commit(கட்டமைப்பு tdb_context *tdb)
{
	மாறிலி கட்டமைப்பு tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	எனில் (tdb->transaction == வெற்று) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		திரும்பு -1;
	}

	எனில் (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		திரும்பு -1;
	}

	எனில் (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		திரும்பு 0;
	}

	/* check for a null transaction */
	எனில் (tdb->transaction->elements == வெற்று) {
		tdb_transaction_cancel(tdb);
		திரும்பு 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	எனில் (tdb->இலக்கம்_locks || tdb->global_lock.மொத்தம்) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		திரும்பு -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	எனில் (tdb_brlock_upgrade(tdb, FREELIST_உச்சி, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		திரும்பு -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	எனில் (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		திரும்பு -1;
	}

	எனில் (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		எனில் (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			திரும்பு -1;
		}
	}

	/* expand the file to the new size if needed */
	எனில் (tdb->map_size != tdb->transaction->old_map_size) {
		எனில் (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			திரும்பு -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	போது (tdb->transaction->elements) {
		கட்டமைப்பு tdb_transaction_el *el = tdb->transaction->elements;

		எனில் (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			திரும்பு -1;
		}
		tdb->transaction->elements = el->next;
		விடுவி(el->data);
		விடுவி(el);
	}

	எனில் (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		எனில் (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			திரும்பு -1;
		}

		/* remove the recovery marker */
		எனில் (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			திரும்பு -1;
		}

		/* ensure the recovery marker has been removed on disk */
		எனில் (transaction_sync(tdb, magic_offset, 4) == -1) {
			திரும்பு -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

	/*
	  TODO: maybe write to some dummy hdr field, or write to magic
	  offset without mmap, before the last sync, instead of the
	  utime() call
	*/

	/* on some systems (like Linux 2.6.x) changes via mmap/msync
	   don't change the mtime of the file, this means the file may
	   not be backed up (as tdb rounding to block sizes means that
	   file size changes are quite rare too). The following forces
	   mtime changes when a transaction completes */
#ifdef HAVE_UTIME
	utime(tdb->name, வெற்று);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	திரும்பு 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
முழு tdb_transaction_recover(கட்டமைப்பு tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned எழுத்து *data, *p;
	u32 zero = 0;
	கட்டமைப்பு list_struct rec;

	/* find the recovery area */
	எனில் (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	எனில் (recovery_head == 0) {
		/* we have never allocated a recovery record */
		திரும்பு 0;
	}

	/* read the recovery record */
	எனில் (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   அளவு(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	எனில் (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		திரும்பு 0;
	}

	எனில் (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		திரும்பு -1;
	}

	recovery_eof = rec.சாவி_len;

	data = (unsigned எழுத்து *)malloc(rec.data_len);
	எனில் (data == வெற்று) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		திரும்பு -1;
	}

	/* read the full recovery data */
	எனில் (tdb->methods->tdb_read(tdb, recovery_head + அளவு(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		விடுவி(data);
		திரும்பு -1;
	}

	/* recover the file data */
	p = data;
	போது (p+8 < data + rec.data_len) {
		u32 ofs, len;
		எனில் (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		எனில் (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			விடுவி(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			திரும்பு -1;
		}
		p += 8 + len;
	}

	விடுவி(data);

	எனில் (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	எனில் (recovery_eof <= recovery_head) {
		எனில் (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			திரும்பு -1;
		}
	}

	/* remove the recovery magic */
	எனில் (tdb_ofs_write(tdb, recovery_head + offsetof(கட்டமைப்பு list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	எனில் (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	எனில் (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		திரும்பு -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	திரும்பு 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
நிலையான முழு tdb_rec_free_read(கட்டமைப்பு tdb_context *tdb, tdb_off_t off, கட்டமைப்பு list_struct *rec)
{
	எனில் (tdb->methods->tdb_read(tdb, off, rec, அளவு(*rec),DOCONV()) == -1)
		திரும்பு -1;

	எனில் (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		எனில் (tdb->methods->tdb_write(tdb, off, rec, அளவு(*rec)) == -1)
			திரும்பு -1;
	}

	எனில் (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		திரும்பு TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	எனில் (tdb->methods->tdb_oob(tdb, rec->next+அளவு(*rec), 0) != 0)
		திரும்பு -1;
	திரும்பு 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
நிலையான முழு remove_from_freelist(கட்டமைப்பு tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_உச்சி;
	போது (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		எனில் (i == off) {
			/* We've found it! */
			திரும்பு tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	திரும்பு TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
நிலையான முழு update_tailer(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset,
			 மாறிலி கட்டமைப்பு list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = அளவு(*rec) + rec->rec_len;
	திரும்பு tdb_ofs_write(tdb, offset + totalsize - அளவு(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
முழு tdb_free(கட்டமைப்பு tdb_context *tdb, tdb_off_t offset, கட்டமைப்பு list_struct *rec)
{
	tdb_off_t வலது, இடது;

	/* Allocation and tailer lock */
	எனில் (tdb_lock(tdb, -1, F_WRLCK) != 0)
		திரும்பு -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	எனில் (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		செல் fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	வலது = offset + அளவு(*rec) + rec->rec_len;
	எனில் (வலது + அளவு(*rec) <= tdb->map_size) {
		கட்டமைப்பு list_struct r;

		எனில் (tdb->methods->tdb_read(tdb, வலது, &r, அளவு(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", வலது));
			செல் இடது;
		}

		/* If it's free, expand to include it. */
		எனில் (r.magic == TDB_FREE_MAGIC) {
			எனில் (remove_from_freelist(tdb, வலது, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", வலது));
				செல் இடது;
			}
			rec->rec_len += அளவு(r) + r.rec_len;
		}
	}

இடது:
	/* Look left */
	இடது = offset - அளவு(tdb_off_t);
	எனில் (இடது > TDB_DATA_START(tdb->header.hash_size)) {
		கட்டமைப்பு list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		எனில் (tdb_ofs_read(tdb, இடது, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", இடது));
			செல் update;
		}

		/* it could be uninitialised data */
		எனில் (leftsize == 0 || leftsize == TDB_PAD_U32) {
			செல் update;
		}

		இடது = offset - leftsize;

		/* Now read in record */
		எனில் (tdb->methods->tdb_read(tdb, இடது, &l, அளவு(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", இடது, leftsize));
			செல் update;
		}

		/* If it's free, expand to include it. */
		எனில் (l.magic == TDB_FREE_MAGIC) {
			எனில் (remove_from_freelist(tdb, இடது, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", இடது));
				செல் update;
			} இல்லையெனில் {
				offset = இடது;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	எனில் (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		செல் fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	எனில் (tdb_ofs_read(tdb, FREELIST_உச்சி, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_உச்சி, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		செல் fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
நிலையான tdb_off_t tdb_allocate_ofs(கட்டமைப்பு tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				கட்டமைப்பு list_struct *rec, tdb_off_t last_ptr)
{
	கட்டமைப்பு list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', அளவு(newrec));

	/* found it - now possibly split it up  */
	எனில் (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (அளவு(*rec) + length);
		newrec_ptr = rec_ptr + அளவு(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} இல்லையெனில் {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	எனில் (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		திரும்பு 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	எனில் (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		திரும்பு 0;
	}

	/* Did we create new block? */
	எனில் (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		எனில் (update_tailer(tdb, rec_ptr, rec) == -1) {
			திரும்பு 0;
		}

		/* Free new record */
		எனில் (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			திரும்பு 0;
		}
	}

	/* all done - return the new record offset */
	திரும்பு rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(கட்டமைப்பு tdb_context *tdb, tdb_len_t length, கட்டமைப்பு list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	கட்டமைப்பு {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	எனில் (tdb_lock(tdb, -1, F_WRLCK) == -1)
		திரும்பு 0;

	/* Extra bytes required for tailer */
	length += அளவு(tdb_off_t);

 again:
	last_ptr = FREELIST_உச்சி;

	/* read in the freelist top */
	எனில் (tdb_ofs_read(tdb, FREELIST_உச்சி, &rec_ptr) == -1)
		செல் fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	போது (rec_ptr) {
		எனில் (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			செல் fail;
		}

		எனில் (rec->rec_len >= length) {
			எனில் (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				எனில் (bestfit.rec_len < 2*length) {
					நிறுத்து;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	எனில் (bestfit.rec_ptr != 0) {
		எனில் (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			செல் fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		திரும்பு newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	எனில் (tdb_expand(tdb, length + அளவு(*rec)) == 0)
		செல் again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

நிலையான முழு seen_செருகு(கட்டமைப்பு tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA சாவி, data;

	memset(&data, '\0', அளவு(data));
	சாவி.dptr = (unsigned எழுத்து *)&rec_ptr;
	சாவி.dsize = அளவு(rec_ptr);
	திரும்பு tdb_store(mem_tdb, சாவி, data, TDB_செருகு);
}

முழு tdb_validate_freelist(கட்டமைப்பு tdb_context *tdb, முழு *pnum_entries)
{
	கட்டமைப்பு tdb_context *mem_tdb = வெற்று;
	கட்டமைப்பு list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	முழு ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	எனில் (!mem_tdb) {
		திரும்பு -1;
	}

	எனில் (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		திரும்பு 0;
	}

	last_ptr = FREELIST_உச்சி;

	/* Store the FREELIST_TOP record. */
	எனில் (seen_செருகு(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		செல் fail;
	}

	/* read in the freelist top */
	எனில் (tdb_ofs_read(tdb, FREELIST_உச்சி, &rec_ptr) == -1) {
		செல் fail;
	}

	போது (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		எனில் (seen_செருகு(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			செல் fail;
		}

		எனில் (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			செல் fail;
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec.next;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
நிலையான முழு tdb_next_lock(கட்டமைப்பு tdb_context *tdb, கட்டமைப்பு tdb_traverse_lock *tlock,
			 கட்டமைப்பு list_struct *rec)
{
	முழு want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	க்கு (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		எனில் (!tlock->off && tlock->hash != 0) {
			/* this is an optimisation for the common case where
			   the hash chain is empty, which is particularly
			   common for the use of tdb with ldb, where large
			   hashes are used. In that case we spend most of our
			   time in tdb_brlock(), locking empty hash chains.

			   To avoid this, we do an unlocked pre-check to see
			   if the hash chain is empty before starting to look
			   inside it. If it is empty then we can avoid that
			   hash chain. If it isn't empty then we can't believe
			   the value we get back, as we read it without a
			   lock, so instead we get the lock and re-fetch the
			   value below.

			   Notice that not doing this optimisation on the
			   first hash chain is critical. We must guarantee
			   that we have done at least one fcntl lock at the
			   start of a search to guarantee that memory is
			   coherent on SMP systems. If records are added by
			   others during the search then that's OK, and we
			   could possibly miss those with this trick, but we
			   could miss them anyway without this trick, so the
			   semantics don't change.

			   With a non-indexed ldb search this trick gains us a
			   factor of around 80 in speed on a linux 2.6.x
			   system (testing using ldbtest).
			*/
			tdb->methods->next_hash_chain(tdb, &tlock->hash);
			எனில் (tlock->hash == tdb->header.hash_size) {
				தொடர்;
			}
		}

		எனில் (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			திரும்பு -1;

		/* No previous record?  Start at top of chain. */
		எனில் (!tlock->off) {
			எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(tlock->hash),
				     &tlock->off) == -1)
				செல் fail;
		} இல்லையெனில் {
			/* Otherwise unlock the previous record. */
			எனில் (tdb_unlock_record(tdb, tlock->off) != 0)
				செல் fail;
		}

		எனில் (want_next) {
			/* We have offset of old record: grab next */
			எனில் (tdb_rec_read(tdb, tlock->off, rec) == -1)
				செல் fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		போது( tlock->off) {
			tdb_off_t current;
			எனில் (tdb_rec_read(tdb, tlock->off, rec) == -1)
				செல் fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			எனில் (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				செல் fail;
			}

			எனில் (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				எனில் (tdb_lock_record(tdb, tlock->off) != 0)
					செல் fail;
				திரும்பு tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			எனில் (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				செல் fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	திரும்பு TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	எனில் (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	திரும்பு -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
நிலையான முழு tdb_traverse_internal(கட்டமைப்பு tdb_context *tdb,
				 tdb_traverse_func fn, வெறுமை *private_data,
				 கட்டமைப்பு tdb_traverse_lock *tl)
{
	TDB_DATA சாவி, dbuf;
	கட்டமைப்பு list_struct rec;
	முழு ret, மொத்தம் = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	போது ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		மொத்தம்++;
		/* now read the full record */
		சாவி.dptr = tdb_alloc_read(tdb, tl->off + அளவு(rec),
					  rec.சாவி_len + rec.data_len);
		எனில் (!சாவி.dptr) {
			ret = -1;
			எனில் (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				செல் out;
			எனில் (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			செல் out;
		}
		சாவி.dsize = rec.சாவி_len;
		dbuf.dptr = சாவி.dptr + rec.சாவி_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		எனில் (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(சாவி.dptr);
			செல் out;
		}
		எனில் (fn && fn(tdb, சாவி, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = மொத்தம்;
			எனில் (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(சாவி.dptr);
			செல் out;
		}
		SAFE_FREE(சாவி.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	எனில் (ret < 0)
		திரும்பு -1;
	இல்லையெனில்
		திரும்பு மொத்தம்;
}


/*
  a write style traverse - temporarily marks the db read only
*/
முழு tdb_traverse_read(கட்டமைப்பு tdb_context *tdb,
		      tdb_traverse_func fn, வெறுமை *private_data)
{
	கட்டமைப்பு tdb_traverse_lock tl = { வெற்று, 0, 0, F_RDLCK };
	முழு ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	எனில் (tdb_transaction_lock(tdb, F_RDLCK)) {
		திரும்பு -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	திரும்பு ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
முழு tdb_traverse(கட்டமைப்பு tdb_context *tdb,
		 tdb_traverse_func fn, வெறுமை *private_data)
{
	கட்டமைப்பு tdb_traverse_lock tl = { வெற்று, 0, 0, F_WRLCK };
	முழு ret;

	எனில் (tdb->read_only || tdb->traverse_read) {
		திரும்பு tdb_traverse_read(tdb, fn, private_data);
	}

	எனில் (tdb_transaction_lock(tdb, F_WRLCK)) {
		திரும்பு -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	திரும்பு ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(கட்டமைப்பு tdb_context *tdb)
{
	TDB_DATA சாவி;
	கட்டமைப்பு list_struct rec;

	/* release any old lock */
	எனில் (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		திரும்பு tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	எனில் (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		திரும்பு tdb_null;
	/* now read the key */
	சாவி.dsize = rec.சாவி_len;
	சாவி.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+அளவு(rec),சாவி.dsize);

	/* Unlock the hash chain of the record we just read. */
	எனில் (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	திரும்பு சாவி;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(கட்டமைப்பு tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA சாவி = tdb_null;
	கட்டமைப்பு list_struct rec;
	unsigned எழுத்து *k = வெற்று;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	எனில் (tdb->travlocks.off) {
		எனில் (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			திரும்பு tdb_null;
		எனில் (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+அளவு(rec),
					    rec.சாவி_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			எனில் (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				திரும்பு tdb_null;
			}
			எனில் (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				திரும்பு tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	எனில் (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		எனில் (!tdb->travlocks.off)
			திரும்பு tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		எனில் (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			திரும்பு tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	எனில் (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		சாவி.dsize = rec.சாவி_len;
		சாவி.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+அளவு(rec),
					  சாவி.dsize);
		/* Unlock the chain of this new record */
		எனில் (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	எனில் (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	திரும்பு சாவி;
}

/* file: dump.c */

நிலையான tdb_off_t tdb_dump_record(கட்டமைப்பு tdb_context *tdb, முழு hash,
				 tdb_off_t offset)
{
	கட்டமைப்பு list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	எனில் (tdb->methods->tdb_read(tdb, offset, (எழுத்து *)&rec,
				   அளவு(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		திரும்பு 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.சாவி_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + அளவு(rec) + rec.rec_len - அளவு(tdb_off_t);

	எனில் (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		திரும்பு rec.next;
	}

	எனில் (tailer != rec.rec_len + அளவு(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned முழு)tailer, (unsigned முழு)(rec.rec_len + அளவு(rec)));
	}
	திரும்பு rec.next;
}

நிலையான முழு tdb_dump_chain(கட்டமைப்பு tdb_context *tdb, முழு i)
{
	tdb_off_t rec_ptr, உச்சி;

	உச்சி = TDB_HASH_உச்சி(i);

	எனில் (tdb_lock(tdb, i, F_WRLCK) != 0)
		திரும்பு -1;

	எனில் (tdb_ofs_read(tdb, உச்சி, &rec_ptr) == -1)
		திரும்பு tdb_unlock(tdb, i, F_WRLCK);

	எனில் (rec_ptr)
		printf("hash=%d\n", i);

	போது (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	திரும்பு tdb_unlock(tdb, i, F_WRLCK);
}

வெறுமை tdb_dump_all(கட்டமைப்பு tdb_context *tdb)
{
	முழு i;
	க்கு (i = 0; i < (முழு)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

முழு tdb_printfreelist(கட்டமைப்பு tdb_context *tdb)
{
	முழு ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	கட்டமைப்பு list_struct rec;

	எனில் ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		திரும்பு ret;

	offset = FREELIST_உச்சி;

	/* read in the freelist top */
	எனில் (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		திரும்பு 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	போது (rec_ptr) {
		எனில் (tdb->methods->tdb_read(tdb, rec_ptr, (எழுத்து *)&rec,
					   அளவு(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			திரும்பு -1;
		}

		எனில் (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			திரும்பு -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (முழு)total_free,
               (முழு)total_free);

	திரும்பு tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
வெறுமை tdb_increment_seqnum_nonblock(கட்டமைப்பு tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	எனில் (!(tdb->flags & TDB_SEQNUM)) {
		திரும்பு;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	எனில் (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		திரும்பு;
	seqnum++;
	(வெறுமை) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
நிலையான வெறுமை tdb_increment_seqnum(கட்டமைப்பு tdb_context *tdb)
{
	எனில் (!(tdb->flags & TDB_SEQNUM)) {
		திரும்பு;
	}

	எனில் (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		திரும்பு;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

நிலையான முழு tdb_சாவி_compare(TDB_DATA சாவி, TDB_DATA data,
			   வெறுமை *private_data EXT2FS_ATTR((unused)))
{
	திரும்பு memcmp(data.dptr, சாவி.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
நிலையான tdb_off_t tdb_find(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash,
			கட்டமைப்பு list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(hash), &rec_ptr) == -1)
		திரும்பு 0;

	/* keep looking until we find the right record */
	போது (rec_ptr) {
		எனில் (tdb_rec_read(tdb, rec_ptr, r) == -1)
			திரும்பு 0;

		எனில் (!TDB_DEAD(r) && hash==r->full_hash
		    && சாவி.dsize==r->சாவி_len
		    && tdb_parse_data(tdb, சாவி, rec_ptr + அளவு(*r),
				      r->சாவி_len, tdb_சாவி_compare,
				      வெற்று) == 0) {
			திரும்பு rec_ptr;
		}
		rec_ptr = r->next;
	}
	திரும்பு TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash, முழு locktype,
			   கட்டமைப்பு list_struct *rec)
{
	u32 rec_ptr;

	எனில் (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		திரும்பு 0;
	எனில் (!(rec_ptr = tdb_find(tdb, சாவி, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	திரும்பு rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
நிலையான முழு tdb_update_hash(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash, TDB_DATA dbuf)
{
	கட்டமைப்பு list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	எனில் (!(rec_ptr = tdb_find(tdb, சாவி, hash, &rec)))
		திரும்பு -1;

	/* must be long enough key, data and tailer */
	எனில் (rec.rec_len < சாவி.dsize + dbuf.dsize + அளவு(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		திரும்பு -1;
	}

	எனில் (tdb->methods->tdb_write(tdb, rec_ptr + அளவு(rec) + rec.சாவி_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		திரும்பு -1;

	எனில் (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		திரும்பு tdb_rec_write(tdb, rec_ptr, &rec);
	}

	திரும்பு 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	tdb_off_t rec_ptr;
	கட்டமைப்பு list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&சாவி);
	எனில் (!(rec_ptr = tdb_find_lock_hash(tdb,சாவி,hash,F_RDLCK,&rec)))
		திரும்பு tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + அளவு(rec) + rec.சாவி_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	திரும்பு ret;
}

/*
 * Find an entry in the database and hand the record's data to a parsing
 * function. The parsing function is executed under the chain read lock, so it
 * should be fast and should not block on other syscalls.
 *
 * DONT CALL OTHER TDB CALLS FROM THE PARSER, THIS MIGHT LEAD TO SEGFAULTS.
 *
 * For mmapped tdb's that do not have a transaction open it points the parsing
 * function directly at the mmap area, it avoids the malloc/memcpy in this
 * case. If a transaction is open or no mmap is available, it has to do
 * malloc/read/parse/free.
 *
 * This is interesting for all readers of potentially large data structures in
 * the tdb records, ldb indexes being one example.
 */

முழு tdb_parse_record(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி,
		     முழு (*parser)(TDB_DATA சாவி, TDB_DATA data,
				   வெறுமை *private_data),
		     வெறுமை *private_data)
{
	tdb_off_t rec_ptr;
	கட்டமைப்பு list_struct rec;
	முழு ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&சாவி);

	எனில் (!(rec_ptr = tdb_find_lock_hash(tdb,சாவி,hash,F_RDLCK,&rec))) {
		திரும்பு TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, சாவி, rec_ptr + அளவு(rec) + rec.சாவி_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	திரும்பு ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
நிலையான முழு tdb_exists_hash(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash)
{
	கட்டமைப்பு list_struct rec;

	எனில் (tdb_find_lock_hash(tdb, சாவி, hash, F_RDLCK, &rec) == 0)
		திரும்பு 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	திரும்பு 1;
}

முழு tdb_exists(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	u32 hash = tdb->hash_fn(&சாவி);
	திரும்பு tdb_exists_hash(tdb, சாவி, hash);
}

/* actually delete an entry in the database given the offset */
முழு tdb_do_delete(கட்டமைப்பு tdb_context *tdb, tdb_off_t rec_ptr, கட்டமைப்பு list_struct*rec)
{
	tdb_off_t last_ptr, i;
	கட்டமைப்பு list_struct lastrec;

	எனில் (tdb->read_only || tdb->traverse_read) திரும்பு -1;

	எனில் (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		திரும்பு tdb_rec_write(tdb, rec_ptr, rec);
	}
	எனில் (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		திரும்பு -1;

	/* find previous record in hash chain */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(rec->full_hash), &i) == -1)
		திரும்பு -1;
	க்கு (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		எனில் (tdb_rec_read(tdb, i, &lastrec) == -1)
			திரும்பு -1;

	/* unlink it: next ptr is at start of record. */
	எனில் (last_ptr == 0)
		last_ptr = TDB_HASH_உச்சி(rec->full_hash);
	எனில் (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		திரும்பு -1;

	/* recover the space */
	எனில் (tdb_free(tdb, rec_ptr, rec) == -1)
		திரும்பு -1;
	திரும்பு 0;
}

நிலையான முழு tdb_மொத்தம்_dead(கட்டமைப்பு tdb_context *tdb, u32 hash)
{
	முழு res = 0;
	tdb_off_t rec_ptr;
	கட்டமைப்பு list_struct rec;

	/* read in the hash top */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(hash), &rec_ptr) == -1)
		திரும்பு 0;

	போது (rec_ptr) {
		எனில் (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			திரும்பு 0;

		எனில் (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	திரும்பு res;
}

/*
 * Purge all DEAD records from a hash chain
 */
நிலையான முழு tdb_purge_dead(கட்டமைப்பு tdb_context *tdb, u32 hash)
{
	முழு res = -1;
	கட்டமைப்பு list_struct rec;
	tdb_off_t rec_ptr;

	எனில் (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		திரும்பு -1;
	}

	/* read in the hash top */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(hash), &rec_ptr) == -1)
		செல் fail;

	போது (rec_ptr) {
		tdb_off_t next;

		எனில் (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			செல் fail;
		}

		next = rec.next;

		எனில் (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			செல் fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	திரும்பு res;
}

/* delete an entry in the database given a key */
நிலையான முழு tdb_delete_hash(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, u32 hash)
{
	tdb_off_t rec_ptr;
	கட்டமைப்பு list_struct rec;
	முழு ret;

	எனில் (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		எனில் (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			திரும்பு -1;

		எனில் (tdb_மொத்தம்_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		எனில் (!(rec_ptr = tdb_find(tdb, சாவி, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			திரும்பு -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	இல்லையெனில் {
		எனில் (!(rec_ptr = tdb_find_lock_hash(tdb, சாவி, hash, F_WRLCK,
						   &rec)))
			திரும்பு -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	எனில் (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	எனில் (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	திரும்பு ret;
}

முழு tdb_delete(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி)
{
	u32 hash = tdb->hash_fn(&சாவி);
	திரும்பு tdb_delete_hash(tdb, சாவி, hash);
}

/*
 * See if we have a dead record around with enough space
 */
நிலையான tdb_off_t tdb_find_dead(கட்டமைப்பு tdb_context *tdb, u32 hash,
			       கட்டமைப்பு list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(hash), &rec_ptr) == -1)
		திரும்பு 0;

	/* keep looking until we find the right record */
	போது (rec_ptr) {
		எனில் (tdb_rec_read(tdb, rec_ptr, r) == -1)
			திரும்பு 0;

		எனில் (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			திரும்பு rec_ptr;
		}
		rec_ptr = r->next;
	}
	திரும்பு 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
முழு tdb_store(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, TDB_DATA dbuf, முழு flag)
{
	கட்டமைப்பு list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	எழுத்து *p = வெற்று;
	முழு ret = -1;

	எனில் (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		திரும்பு -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&சாவி);
	எனில் (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		திரும்பு -1;

	/* check for it existing, on insert. */
	எனில் (flag == TDB_செருகு) {
		எனில் (tdb_exists_hash(tdb, சாவி, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			செல் fail;
		}
	} இல்லையெனில் {
		/* first try in-place update, on modify or replace. */
		எனில் (tdb_update_hash(tdb, சாவி, hash, dbuf) == 0) {
			செல் done;
		}
		எனில் (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			செல் fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	எனில் (flag != TDB_செருகு)
		tdb_delete_hash(tdb, சாவி, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	எனில் (!(p = (எழுத்து *)malloc(சாவி.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		செல் fail;
	}

	memcpy(p, சாவி.dptr, சாவி.dsize);
	எனில் (dbuf.dsize)
		memcpy(p+சாவி.dsize, dbuf.dptr, dbuf.dsize);

	எனில் (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			சாவி.dsize + dbuf.dsize + அளவு(tdb_off_t));

		எனில் (rec_ptr != 0) {
			rec.சாவி_len = சாவி.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			எனில் (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + அளவு(rec),
				    p, சாவி.dsize + dbuf.dsize) == -1) {
				செல் fail;
			}
			செல் done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	எனில் (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		செல் fail;
	}

	எனில் ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		செல் fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, சாவி.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	எனில் (rec_ptr == 0) {
		செல் fail;
	}

	/* Read hash top into next ptr */
	எனில் (tdb_ofs_read(tdb, TDB_HASH_உச்சி(hash), &rec.next) == -1)
		செல் fail;

	rec.சாவி_len = சாவி.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	எனில் (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+அளவு(rec), p, சாவி.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_உச்சி(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		செல் fail;
	}

 done:
	ret = 0;
 fail:
	எனில் (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	திரும்பு ret;
}


/* Append to an entry. Create if not exist. */
முழு tdb_append(கட்டமைப்பு tdb_context *tdb, TDB_DATA சாவி, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	முழு ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&சாவி);
	எனில் (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		திரும்பு -1;

	dbuf = tdb_fetch(tdb, சாவி);

	எனில் (dbuf.dptr == வெற்று) {
		dbuf.dptr = (unsigned எழுத்து *)malloc(new_dbuf.dsize);
	} இல்லையெனில் {
		unsigned எழுத்து *new_dptr = (unsigned எழுத்து *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		எனில் (new_dptr == வெற்று) {
			விடுவி(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	எனில் (dbuf.dptr == வெற்று) {
		tdb->ecode = TDB_ERR_OOM;
		செல் failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, சாவி, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	திரும்பு ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
மாறிலி எழுத்து *tdb_name(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
முழு tdb_fd(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->log.log_fn;
}


/*
  get the tdb sequence number. Only makes sense if the writers opened
  with TDB_SEQNUM set. Note that this sequence number will wrap quite
  quickly, so it should only be used for a 'has something changed'
  test, not for code that relies on the count of the number of changes
  made. If you want a counter then use a tdb record.

  The aim of this sequence number is to allow for a very lightweight
  test of a possible tdb change.
*/
முழு tdb_get_seqnum(கட்டமைப்பு tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	எனில் (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		திரும்பு 0;
	திரும்பு seqnum;
}

முழு tdb_hash_size(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->header.hash_size;
}

size_t tdb_map_size(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->map_size;
}

முழு tdb_get_flags(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
வெறுமை tdb_enable_seqnum(கட்டமைப்பு tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
நிலையான கட்டமைப்பு tdb_context *tdbs = வெற்று;


/* This is from a hash algorithm suggested by Rogier Wolff */
நிலையான unsigned முழு default_tdb_hash(TDB_DATA *சாவி)
{
	u32 மதிப்பு;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	க்கு (மதிப்பு = 0, i=0; i < சாவி->dsize; i++)
		மதிப்பு = மதிப்பு * 256 + சாவி->dptr[i] + (மதிப்பு >> 24) * 241;

	திரும்பு மதிப்பு;
}


/* initialise a new database with a specified hash size */
நிலையான முழு tdb_new_database(கட்டமைப்பு tdb_context *tdb, முழு hash_size)
{
	கட்டமைப்பு tdb_header *newdb;
	முழு size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = அளவு(கட்டமைப்பு tdb_header) + (hash_size+1)*அளவு(tdb_off_t);
	எனில் (!(newdb = (கட்டமைப்பு tdb_header *)calloc(size, 1)))
		திரும்பு TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	எனில் (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (எழுத்து *)newdb;
		memcpy(&tdb->header, newdb, அளவு(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		திரும்பு 0;
	}
	எனில் (lseek(tdb->fd, 0, SEEK_SET) == -1)
		செல் fail;

	எனில் (ftruncate(tdb->fd, 0) == -1)
		செல் fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, அளவு(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	எனில் (லிக(tdb->fd, newdb, size) != size) {
		ret = -1;
	} இல்லையெனில் {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	திரும்பு ret;
}



நிலையான முழு tdb_already_open(dev_t device,
			    ino_t ino)
{
	கட்டமைப்பு tdb_context *i;

	க்கு (i = tdbs; i; i = i->next) {
		எனில் (i->device == device && i->inode == ino) {
			திரும்பு 1;
		}
	}

	திரும்பு 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
கட்டமைப்பு tdb_context *tdb_open(மாறிலி எழுத்து *name, முழு hash_size, முழு tdb_flags,
		      முழு open_flags, mode_t mode)
{
	திரும்பு tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, வெற்று, வெற்று);
}

/* a default logging function */
நிலையான வெறுமை null_log_fn(கட்டமைப்பு tdb_context *tdb, எண்ணிக்கை tdb_debug_level level, மாறிலி எழுத்து *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
நிலையான வெறுமை null_log_fn(கட்டமைப்பு tdb_context *tdb EXT2FS_ATTR((unused)),
			எண்ணிக்கை tdb_debug_level level EXT2FS_ATTR((unused)),
			மாறிலி எழுத்து *fmt EXT2FS_ATTR((unused)), ...)
{
}


கட்டமைப்பு tdb_context *tdb_open_ex(மாறிலி எழுத்து *name, முழு hash_size, முழு tdb_flags,
				முழு open_flags, mode_t mode,
				மாறிலி கட்டமைப்பு tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	கட்டமைப்பு tdb_context *tdb;
	கட்டமைப்பு stat st;
	முழு rev = 0, locked = 0;
	unsigned எழுத்து *vp;
	u32 vertest;

	எனில் (!(tdb = (கட்டமைப்பு tdb_context *)calloc(1, அளவு *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		செல் fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = வெற்று;
	tdb->map_ptr = வெற்று;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	எனில் (log_ctx) {
		tdb->log = *log_ctx;
	} இல்லையெனில் {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = வெற்று;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	எனில் (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	எனில் ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		செல் fail;
	}

	எனில் (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	எனில் ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	எனில் (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		எனில் (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			செல் fail;
		}
		செல் internal;
	}

	எனில் ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		செல் fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	எனில் (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		செல் fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	எனில் ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		எனில் (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			செல் fail; /* errno set by ftruncate */
		}
	}

	எனில் (பட(tdb->fd, &tdb->header, அளவு(tdb->header)) != அளவு(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      அளவு(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		எனில் (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			செல் fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned எழுத்து *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	எனில் (!rev)
		tdb->flags &= ~TDB_CONVERT;
	இல்லையெனில் {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, அளவு(tdb->header));
	}
	எனில் (fstat(tdb->fd, &st) == -1)
		செல் fail;

	எனில் (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		செல் fail;
	}

	/* Is it already in the open list?  If so, fail. */
	எனில் (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (முழு)st.st_dev, (முழு)st.st_ino));
		errno = EBUSY;
		செல் fail;
	}

	எனில் (!(tdb->name = (எழுத்து *)strdup(name))) {
		errno = ENOMEM;
		செல் fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	எனில் (locked) {
		எனில் (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			செல் fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	எனில் (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		எனில் (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			செல் fail;
	}

	/* if needed, run recovery */
	எனில் (tdb_transaction_recover(tdb) == -1) {
		செல் fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	எனில் (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		செல் fail;
	tdb->next = tdbs;
	tdbs = tdb;
	திரும்பு tdb;

 fail:
	{ முழு save_errno = errno;

	எனில் (!tdb)
		திரும்பு வெற்று;

	எனில் (tdb->map_ptr) {
		எனில் (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		இல்லையெனில்
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	எனில் (tdb->fd != -1)
		எனில் (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	திரும்பு வெற்று;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

வெறுமை tdb_set_max_dead(கட்டமைப்பு tdb_context *tdb, முழு max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
முழு tdb_close(கட்டமைப்பு tdb_context *tdb)
{
	கட்டமைப்பு tdb_context **i;
	முழு ret = 0;

	எனில் (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	எனில் (tdb->map_ptr) {
		எனில் (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		இல்லையெனில்
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	எனில் (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	க்கு (i = &tdbs; *i; i = &(*i)->next) {
		எனில் (*i == tdb) {
			*i = tdb->next;
			நிறுத்து;
		}
	}

	memset(tdb, 0, அளவு(*tdb));
	SAFE_FREE(tdb);

	திரும்பு ret;
}

/* register a logging function */
வெறுமை tdb_set_logging_function(கட்டமைப்பு tdb_context *tdb,
                              மாறிலி கட்டமைப்பு tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

வெறுமை *tdb_get_logging_private(கட்டமைப்பு tdb_context *tdb)
{
	திரும்பு tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
முழு tdb_reopen(கட்டமைப்பு tdb_context *tdb)
{
	கட்டமைப்பு stat st;

	எனில் (tdb->flags & TDB_INTERNAL) {
		திரும்பு 0; /* Nothing to do. */
	}

	எனில் (tdb->இலக்கம்_locks != 0 || tdb->global_lock.மொத்தம்) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		செல் fail;
	}

	எனில் (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		செல் fail;
	}

	எனில் (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		செல் fail;
	}
	எனில் (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	எனில் (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		செல் fail;
	}
	எனில் ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		செல் fail;
	}
	எனில் (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		செல் fail;
	}
	எனில் (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		செல் fail;
	}
	tdb_mmap(tdb);

	திரும்பு 0;

fail:
	tdb_close(tdb);
	திரும்பு -1;
}

/* reopen all tdb's */
முழு tdb_reopen_all(முழு parent_longlived)
{
	கட்டமைப்பு tdb_context *tdb;

	க்கு (tdb=tdbs; tdb; tdb = tdb->next) {
		/*
		 * If the parent is longlived (ie. a
		 * parent daemon architecture), we know
		 * it will keep it's active lock on a
		 * tdb opened with CLEAR_IF_FIRST. Thus
		 * for child processes we don't have to
		 * add an active lock. This is essential
		 * to improve performance on systems that
		 * keep POSIX locks as a non-scalable data
		 * structure in the kernel.
		 */
		எனில் (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		எனில் (tdb_reopen(tdb) != 0)
			திரும்பு -1;
	}

	திரும்பு 0;
}

/**
 * Flush a database file from the page cache.
 **/
முழு tdb_flush(கட்டமைப்பு tdb_context *tdb)
{
	எனில் (tdb->fd != -1)
		திரும்பு fsync(tdb->fd);
	திரும்பு 0;
}
