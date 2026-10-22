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

#شامل "config.h"
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <stdarg.h>
#شامل <stddef.h>
#شامل <errno.h>
#شامل <string.h>
#ifdef HAVE_SYS_SELECT_H
#شامل <sys/select.h>
#endif
#شامل <sys/time.h>
#شامل <sys/types.h>
#شامل <time.h>
#ifdef HAVE_UTIME_H
#شامل <utime.h>
#endif
#شامل <sys/stat.h>
#شامل <sys/file.h>
#شامل <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#شامل <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#وگرنه
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((تهی *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
ایستا نویسه *rep_strdup(ثابت نویسه *s)
{
	نویسه *ret;
	صحیح length;

	اگر (!s)
		بازگشت پوچ;
	length = strlen(s);
	ret = malloc(length + 1);
	اگر (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	بازگشت ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#اگر (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وگرنه
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#اگر defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
تعریفنوع صحیح bool;
#endif

#شامل "tdb.h"

ایستا TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

تعریفنوع u32 tdb_len_t;
تعریفنوع u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned صحیح)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*اندازه(ساختار list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_بالاترین (اندازه(ساختار tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_بالاترین(hash) (FREELIST_بالاترین + (BUCKET(hash)+1)*اندازه(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*اندازه(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_بالاترین(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(ساختار tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(ساختار tdb_header, sequence_عدد)
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
#define SAFE_FREE(x) انجامبده { اگر ((x) != پوچ) {آزاد(x); (x)=پوچ;} } وقتی(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, اندازه(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ساختار list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t کلید_len; /* byte length of key */
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
ساختار tdb_header {
	نویسه magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_عدد; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ساختار tdb_lock_type {
	صحیح list;
	u32 شمار;
	u32 ltype;
};

ساختار tdb_traverse_lock {
	ساختار tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	صحیح lock_rw;
};


ساختار tdb_methods {
	صحیح (*tdb_read)(ساختار tdb_context *, tdb_off_t , تهی *, tdb_len_t , صحیح );
	صحیح (*tdb_write)(ساختار tdb_context *, tdb_off_t, ثابت تهی *, tdb_len_t);
	تهی (*next_hash_chain)(ساختار tdb_context *, u32 *);
	صحیح (*tdb_oob)(ساختار tdb_context *, tdb_off_t , صحیح );
	صحیح (*tdb_expand_file)(ساختار tdb_context *, tdb_off_t , tdb_off_t );
	صحیح (*tdb_brlock)(ساختار tdb_context *, tdb_off_t , صحیح, صحیح, صحیح, size_t);
};

ساختار tdb_context {
	نویسه *name; /* the name of the database */
	تهی *map_ptr; /* where it is currently mapped */
	صحیح fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	صحیح read_only; /* opened read-only */
	صحیح traverse_read; /* read-only traversal */
	ساختار tdb_lock_type global_lock;
	صحیح رقم_lockrecs;
	ساختار tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	شمارش TDB_ERROR ecode; /* error code for last tdb error */
	ساختار tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ساختار tdb_traverse_lock travlocks; /* current traversal locks */
	ساختار tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ساختار tdb_logging_context log;
	unsigned صحیح (*hash_fn)(TDB_DATA *کلید);
	صحیح open_flags; /* flags used in the open - needed by reopen */
	unsigned صحیح رقم_locks; /* number of chain locks held */
	ثابت ساختار tdb_methods *methods;
	ساختار tdb_transaction *transaction;
	صحیح page_size;
	صحیح max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ایستا صحیح tdb_munmap(ساختار tdb_context *tdb);
ایستا تهی tdb_mmap(ساختار tdb_context *tdb);
ایستا صحیح tdb_lock(ساختار tdb_context *tdb, صحیح list, صحیح ltype);
صحیح tdb_lock_nonblock(ساختار tdb_context *tdb, صحیح list, صحیح ltype);
ایستا صحیح tdb_unlock(ساختار tdb_context *tdb, صحیح list, صحیح ltype);
ایستا صحیح tdb_brlock(ساختار tdb_context *tdb, tdb_off_t offset, صحیح rw_type, صحیح lck_type, صحیح probe, size_t len);
ایستا صحیح tdb_transaction_lock(ساختار tdb_context *tdb, صحیح ltype);
ایستا صحیح tdb_transaction_unlock(ساختار tdb_context *tdb);
ایستا صحیح tdb_brlock_upgrade(ساختار tdb_context *tdb, tdb_off_t offset, size_t len);
ایستا صحیح tdb_write_lock_record(ساختار tdb_context *tdb, tdb_off_t off);
ایستا صحیح tdb_write_unlock_record(ساختار tdb_context *tdb, tdb_off_t off);
ایستا صحیح tdb_ofs_read(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ایستا صحیح tdb_ofs_write(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ایستا تهی *tdb_convert(تهی *buf, u32 size);
ایستا صحیح tdb_free(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec);
ایستا tdb_off_t tdb_allocate(ساختار tdb_context *tdb, tdb_len_t length, ساختار list_struct *rec);
ایستا صحیح tdb_ofs_read(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ایستا صحیح tdb_ofs_write(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ایستا صحیح tdb_lock_record(ساختار tdb_context *tdb, tdb_off_t off);
ایستا صحیح tdb_unlock_record(ساختار tdb_context *tdb, tdb_off_t off);
ایستا صحیح tdb_rec_read(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec);
ایستا صحیح tdb_rec_write(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec);
ایستا صحیح tdb_do_delete(ساختار tdb_context *tdb, tdb_off_t rec_ptr, ساختار list_struct *rec);
ایستا unsigned نویسه *tdb_alloc_read(ساختار tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
ایستا صحیح tdb_parse_data(ساختار tdb_context *tdb, TDB_DATA کلید,
		   tdb_off_t offset, tdb_len_t len,
		   صحیح (*parser)(TDB_DATA کلید, TDB_DATA data,
				 تهی *private_data),
		   تهی *private_data);
ایستا tdb_off_t tdb_find_lock_hash(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash, صحیح locktype,
			   ساختار list_struct *rec);
ایستا تهی tdb_io_init(ساختار tdb_context *tdb);
ایستا صحیح tdb_expand(ساختار tdb_context *tdb, tdb_off_t size);
ایستا صحیح tdb_rec_free_read(ساختار tdb_context *tdb, tdb_off_t off,
		      ساختار list_struct *rec);


/* file: error.c */

شمارش TDB_ERROR tdb_error(ساختار tdb_context *tdb)
{
	بازگشت tdb->ecode;
}

ایستا ساختار tdb_errname {
	شمارش TDB_ERROR ecode; ثابت نویسه *estring;
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
ثابت نویسه *tdb_errorstr(ساختار tdb_context *tdb)
{
	u32 i;
	برای (i = 0; i < اندازه(emap) / اندازه(ساختار tdb_errname); i++)
		اگر (tdb->ecode == emap[i].ecode)
			بازگشت emap[i].estring;
	بازگشت "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
صحیح tdb_brlock(ساختار tdb_context *tdb, tdb_off_t offset,
	       صحیح rw_type, صحیح lck_type, صحیح probe, size_t len)
{
	ساختار flock fl;
	صحیح ret;

	اگر (tdb->flags & TDB_NOLOCK) {
		بازگشت 0;
	}

	اگر ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		بازگشت -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	انجامبده {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} وقتی (ret == -1 && errno == EINTR);

	اگر (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		اگر (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (صحیح)len));
		}
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	بازگشت 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
صحیح tdb_brlock_upgrade(ساختار tdb_context *tdb, tdb_off_t offset, size_t len)
{
	صحیح شمار = 1000;
	وقتی (شمار--) {
		ساختار timeval tv;
		اگر (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			بازگشت 0;
		}
		اگر (errno != EDEADLK) {
			بشکن;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, پوچ, پوچ, پوچ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	بازگشت -1;
}


/* lock a list in the database. list -1 is the alloc list */
ایستا صحیح _tdb_lock(ساختار tdb_context *tdb, صحیح list, صحیح ltype, صحیح op)
{
	ساختار tdb_lock_type *new_lck;
	صحیح i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	اگر (tdb->global_lock.شمار &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		بازگشت 0;
	}

	اگر (tdb->global_lock.شمار) {
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (list < -1 || list >= (صحیح)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		بازگشت -1;
	}
	اگر (tdb->flags & TDB_NOLOCK)
		بازگشت 0;

	برای (i=0; i<tdb->رقم_lockrecs; i++) {
		اگر (tdb->lockrecs[i].list == list) {
			اگر (tdb->lockrecs[i].شمار == 0) {
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
			tdb->lockrecs[i].شمار++;
			بازگشت 0;
		}
	}

	new_lck = (ساختار tdb_lock_type *)realloc(
		tdb->lockrecs,
		اندازه(*tdb->lockrecs) * (tdb->رقم_lockrecs+1));
	اگر (new_lck == پوچ) {
		errno = ENOMEM;
		بازگشت -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	اگر (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_بالاترین+4*list, ltype, op,
				     0, 1)) {
		بازگشت -1;
	}

	tdb->رقم_locks++;

	tdb->lockrecs[tdb->رقم_lockrecs].list = list;
	tdb->lockrecs[tdb->رقم_lockrecs].شمار = 1;
	tdb->lockrecs[tdb->رقم_lockrecs].ltype = ltype;
	tdb->رقم_lockrecs += 1;

	بازگشت 0;
}

/* lock a list in the database. list -1 is the alloc list */
صحیح tdb_lock(ساختار tdb_context *tdb, صحیح list, صحیح ltype)
{
	صحیح ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	اگر (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	بازگشت ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
صحیح tdb_lock_nonblock(ساختار tdb_context *tdb, صحیح list, صحیح ltype)
{
	بازگشت _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
صحیح tdb_unlock(ساختار tdb_context *tdb, صحیح list, صحیح ltype)
{
	صحیح ret = -1;
	صحیح i;
	ساختار tdb_lock_type *lck = پوچ;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	اگر (tdb->global_lock.شمار &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		بازگشت 0;
	}

	اگر (tdb->global_lock.شمار) {
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (tdb->flags & TDB_NOLOCK)
		بازگشت 0;

	/* Sanity checks */
	اگر (list < -1 || list >= (صحیح)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		بازگشت ret;
	}

	برای (i=0; i<tdb->رقم_lockrecs; i++) {
		اگر (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			بشکن;
		}
	}

	اگر ((lck == پوچ) || (lck->شمار == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		بازگشت -1;
	}

	اگر (lck->شمار > 1) {
		lck->شمار--;
		بازگشت 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	اگر (mark_lock) {
		ret = 0;
	} وگرنه {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->رقم_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	اگر (tdb->رقم_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->رقم_lockrecs-1];
	}
	tdb->رقم_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	اگر (tdb->رقم_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	اگر (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	بازگشت ret;
}

/*
  get the transaction lock
 */
صحیح tdb_transaction_lock(ساختار tdb_context *tdb, صحیح ltype)
{
	اگر (tdb->have_transaction_lock || tdb->global_lock.شمار) {
		بازگشت 0;
	}
	اگر (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		بازگشت -1;
	}
	tdb->have_transaction_lock = 1;
	بازگشت 0;
}

/*
  release the transaction lock
 */
صحیح tdb_transaction_unlock(ساختار tdb_context *tdb)
{
	صحیح ret;
	اگر (!tdb->have_transaction_lock) {
		بازگشت 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	اگر (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	بازگشت ret;
}




/* lock/unlock entire database */
ایستا صحیح _tdb_lockall(ساختار tdb_context *tdb, صحیح ltype, صحیح op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	اگر (tdb->read_only || tdb->traverse_read)
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);

	اگر (tdb->global_lock.شمار && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.شمار++;
		بازگشت 0;
	}

	اگر (tdb->global_lock.شمار) {
		/* a global lock of a different type exists */
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (tdb->رقم_locks != 0) {
		/* can't combine global and chain locks */
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		اگر (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		بازگشت -1;
	}

	tdb->global_lock.شمار = 1;
	tdb->global_lock.ltype = ltype;

	بازگشت 0;
}



/* unlock entire db */
ایستا صحیح _tdb_unlockall(ساختار tdb_context *tdb, صحیح ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	اگر (tdb->read_only || tdb->traverse_read) {
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.شمار == 0) {
		بازگشت TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	اگر (tdb->global_lock.شمار > 1) {
		tdb->global_lock.شمار--;
		بازگشت 0;
	}

	اگر (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		بازگشت -1;
	}

	tdb->global_lock.شمار = 0;
	tdb->global_lock.ltype = 0;

	بازگشت 0;
}

/* lock entire database with write lock */
صحیح tdb_lockall(ساختار tdb_context *tdb)
{
	بازگشت _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
صحیح tdb_lockall_mark(ساختار tdb_context *tdb)
{
	بازگشت _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
صحیح tdb_lockall_unmark(ساختار tdb_context *tdb)
{
	بازگشت _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
صحیح tdb_lockall_nonblock(ساختار tdb_context *tdb)
{
	بازگشت _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
صحیح tdb_unlockall(ساختار tdb_context *tdb)
{
	بازگشت _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
صحیح tdb_lockall_read(ساختار tdb_context *tdb)
{
	بازگشت _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
صحیح tdb_lockall_read_nonblock(ساختار tdb_context *tdb)
{
	بازگشت _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
صحیح tdb_unlockall_read(ساختار tdb_context *tdb)
{
	بازگشت _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
صحیح tdb_chainlock(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
صحیح tdb_chainlock_nonblock(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
صحیح tdb_chainlock_mark(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
صحیح tdb_chainlock_unmark(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_LOCK);
}

صحیح tdb_chainunlock(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_WRLCK);
}

صحیح tdb_chainlock_read(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_RDLCK);
}

صحیح tdb_chainunlock_read(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	بازگشت tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلید)), F_RDLCK);
}



/* record lock stops delete underneath */
صحیح tdb_lock_record(ساختار tdb_context *tdb, tdb_off_t off)
{
	بازگشت off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحیح tdb_write_lock_record(ساختار tdb_context *tdb, tdb_off_t off)
{
	ساختار tdb_traverse_lock *i;
	برای (i = &tdb->travlocks; i; i = i->next)
		اگر (i->off == off)
			بازگشت -1;
	بازگشت tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحیح tdb_write_unlock_record(ساختار tdb_context *tdb, tdb_off_t off)
{
	بازگشت tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
صحیح tdb_unlock_record(ساختار tdb_context *tdb, tdb_off_t off)
{
	ساختار tdb_traverse_lock *i;
	u32 شمار = 0;

	اگر (off == 0)
		بازگشت 0;
	برای (i = &tdb->travlocks; i; i = i->next)
		اگر (i->off == off)
			شمار++;
	بازگشت (شمار == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ایستا صحیح tdb_oob(ساختار tdb_context *tdb, tdb_off_t len, صحیح probe)
{
	ساختار stat st;
	اگر (len <= tdb->map_size)
		بازگشت 0;
	اگر (tdb->flags & TDB_INTERNAL) {
		اگر (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (صحیح)len, (صحیح)tdb->map_size));
		}
		بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	اگر (fstat(tdb->fd, &st) == -1) {
		بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	اگر (st.st_size < (off_t)len) {
		اگر (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (صحیح)len, (صحیح)st.st_size));
		}
		بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	اگر (tdb_munmap(tdb) == -1)
		بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	بازگشت 0;
}

/* write a lump of data at a specified offset */
ایستا صحیح tdb_write(ساختار tdb_context *tdb, tdb_off_t off,
		     ثابت تهی *buf, tdb_len_t len)
{
	اگر (len == 0) {
		بازگشت 0;
	}

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		بازگشت -1;
	}

	اگر (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		بازگشت -1;

	اگر (tdb->map_ptr) {
		memcpy(off + (نویسه *)tdb->map_ptr, buf, len);
	} وگرنه اگر (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	بازگشت 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
تهی *tdb_convert(تهی *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	برای (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	بازگشت buf;
}


/* read a lump of data at a specified offset, maybe convert */
ایستا صحیح tdb_read(ساختار tdb_context *tdb, tdb_off_t off, تهی *buf,
		    tdb_len_t len, صحیح cv)
{
	اگر (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		بازگشت -1;
	}

	اگر (tdb->map_ptr) {
		memcpy(buf, off + (نویسه *)tdb->map_ptr, len);
	} وگرنه {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		اگر (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (صحیح)off, (صحیح)len, (صحیح)ret, strerror(errno),
				 (صحیح)tdb->map_size));
			بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	اگر (cv) {
		tdb_convert(buf, len);
	}
	بازگشت 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ایستا تهی tdb_next_hash_chain(ساختار tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	اگر (tdb->map_ptr) {
		برای (;h < tdb->header.hash_size;h++) {
			اگر (0 != *(u32 *)(TDB_HASH_بالاترین(h) + (unsigned نویسه *)tdb->map_ptr)) {
				بشکن;
			}
		}
	} وگرنه {
		u32 off=0;
		برای (;h < tdb->header.hash_size;h++) {
			اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(h), &off) != 0 || off != 0) {
				بشکن;
			}
		}
	}
	(*chain) = h;
}


صحیح tdb_munmap(ساختار tdb_context *tdb)
{
	اگر (tdb->flags & TDB_INTERNAL)
		بازگشت 0;

#ifdef HAVE_MMAP
	اگر (tdb->map_ptr) {
		صحیح ret = munmap(tdb->map_ptr, tdb->real_map_size);
		اگر (ret != 0)
			بازگشت ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = پوچ;
	بازگشت 0;
}

تهی tdb_mmap(ساختار tdb_context *tdb)
{
	اگر (tdb->flags & TDB_INTERNAL)
		بازگشت;

#ifdef HAVE_MMAP
	اگر (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(پوچ, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		اگر (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = پوچ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} وگرنه {
		tdb->map_ptr = پوچ;
	}
#وگرنه
	tdb->map_ptr = پوچ;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ایستا صحیح tdb_expand_file(ساختار tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	نویسه buf[1024];

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		بازگشت -1;
	}

	اگر (ftruncate(tdb->fd, size+addition) == -1) {
		نویسه b = 0;
		اگر (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			بازگشت -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, اندازه(buf));
	وقتی (addition) {
		صحیح n = addition>اندازه(buf)?اندازه(buf):addition;
		صحیح ret = pwrite(tdb->fd, buf, n, size);
		اگر (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			بازگشت -1;
		}
		addition -= n;
		size += n;
	}
	بازگشت 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
صحیح tdb_expand(ساختار tdb_context *tdb, tdb_off_t size)
{
	ساختار list_struct rec;
	tdb_off_t offset;

	اگر (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		بازگشت -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	اگر (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	اگر (!(tdb->flags & TDB_INTERNAL)) {
		اگر (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			برو fail;
	}

	tdb->map_size += size;

	اگر (tdb->flags & TDB_INTERNAL) {
		نویسه *new_map_ptr = (نویسه *)realloc(tdb->map_ptr,
						    tdb->map_size);
		اگر (!new_map_ptr) {
			tdb->map_size -= size;
			برو fail;
		}
		tdb->map_ptr = new_map_ptr;
	} وگرنه {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',اندازه(rec));
	rec.rec_len = size - اندازه(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	اگر (tdb_free(tdb, offset, &rec) == -1)
		برو fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت -1;
}

/* read/write a tdb_off_t */
صحیح tdb_ofs_read(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	بازگشت tdb->methods->tdb_read(tdb, offset, (نویسه*)d, اندازه(*d), DOCONV());
}

صحیح tdb_ofs_write(ساختار tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	بازگشت tdb->methods->tdb_write(tdb, offset, CONVERT(off), اندازه(*d));
}


/* read a lump of data, allocating the space for it */
unsigned نویسه *tdb_alloc_read(ساختار tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned نویسه *buf;

	/* some systems don't like zero length malloc */
	اگر (len == 0) {
		len = 1;
	}

	اگر (!(buf = (unsigned نویسه *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		بازگشت TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	اگر (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		بازگشت پوچ;
	}
	بازگشت buf;
}

/* Give a piece of tdb data to a parser */

صحیح tdb_parse_data(ساختار tdb_context *tdb, TDB_DATA کلید,
		   tdb_off_t offset, tdb_len_t len,
		   صحیح (*parser)(TDB_DATA کلید, TDB_DATA data,
				 تهی *private_data),
		   تهی *private_data)
{
	TDB_DATA data;
	صحیح result;

	data.dsize = len;

	اگر ((tdb->transaction == پوچ) && (tdb->map_ptr != پوچ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		اگر (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			بازگشت -1;
		}
		data.dptr = offset + (unsigned نویسه *)tdb->map_ptr;
		بازگشت parser(کلید, data, private_data);
	}

	اگر (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		بازگشت -1;
	}

	result = parser(کلید, data, private_data);
	آزاد(data.dptr);
	بازگشت result;
}

/* read/write a record */
صحیح tdb_rec_read(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec)
{
	اگر (tdb->methods->tdb_read(tdb, offset, rec, اندازه(*rec),DOCONV()) == -1)
		بازگشت -1;
	اگر (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		بازگشت TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	بازگشت tdb->methods->tdb_oob(tdb, rec->next+اندازه(*rec), 0);
}

صحیح tdb_rec_write(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec)
{
	ساختار list_struct r = *rec;
	بازگشت tdb->methods->tdb_write(tdb, offset, CONVERT(r), اندازه(r));
}

ایستا ثابت ساختار tdb_methods io_methods = {
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
تهی tdb_io_init(ساختار tdb_context *tdb)
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

ساختار tdb_transaction_el {
	ساختار tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned نویسه *data;
};

/*
  hold the context of any current transaction
*/
ساختار tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ثابت ساختار tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ساختار tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	صحیح transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	صحیح nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ایستا صحیح transaction_read(ساختار tdb_context *tdb, tdb_off_t off, تهی *buf,
			    tdb_len_t len, صحیح cv)
{
	ساختار tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	برای (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		اگر (off+len <= el->offset) {
			ادامه;
		}
		اگر (off >= el->offset + el->length) {
			ادامه;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		اگر (off < el->offset) {
			partial = el->offset - off;
			اگر (transaction_read(tdb, off, buf, partial, cv) != 0) {
				برو fail;
			}
			len -= partial;
			off += partial;
			buf = (تهی *)(partial + (نویسه *)buf);
		}
		اگر (off + len <= el->offset + el->length) {
			partial = len;
		} وگرنه {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		اگر (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (تهی *)(partial + (نویسه *)buf);

		اگر (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			برو fail;
		}

		بازگشت 0;
	}

	/* its not in the transaction elements - do a real read */
	بازگشت tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	بازگشت -1;
}


/*
  write while in a transaction
*/
ایستا صحیح transaction_write(ساختار tdb_context *tdb, tdb_off_t off,
			     ثابت تهی *buf, tdb_len_t len)
{
	ساختار tdb_transaction_el *el, *best_el=پوچ;

	اگر (len == 0) {
		بازگشت 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	اگر (len == اندازه(tdb_off_t) && off >= FREELIST_بالاترین &&
	    off < FREELIST_بالاترین+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_بالاترین) / اندازه(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	برای (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		اگر (best_el == پوچ && off == el->offset+el->length) {
			best_el = el;
		}

		اگر (off+len <= el->offset) {
			ادامه;
		}
		اگر (off >= el->offset + el->length) {
			ادامه;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		اگر (off < el->offset) {
			partial = el->offset - off;
			اگر (transaction_write(tdb, off, buf, partial) != 0) {
				برو fail;
			}
			len -= partial;
			off += partial;
			buf = (ثابت تهی *)(partial + (ثابت نویسه *)buf);
		}
		اگر (off + len <= el->offset + el->length) {
			partial = len;
		} وگرنه {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ثابت تهی *)(partial + (ثابت نویسه *)buf);

		اگر (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			برو fail;
		}

		بازگشت 0;
	}

	/* see if we can append the new entry to an existing entry */
	اگر (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned نویسه *data = best_el->data;
		el = best_el;
		el->data = (unsigned نویسه *)realloc(el->data,
						    el->length + len);
		اگر (el->data == پوچ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			بازگشت -1;
		}
		اگر (buf) {
			memcpy(el->data + el->length, buf, len);
		} وگرنه {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		بازگشت 0;
	}

	/* add a new entry at the end of the list */
	el = (ساختار tdb_transaction_el *)malloc(اندازه(*el));
	اگر (el == پوچ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		بازگشت -1;
	}
	el->next = پوچ;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned نویسه *)malloc(len);
	اگر (el->data == پوچ) {
		آزاد(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		بازگشت -1;
	}
	اگر (buf) {
		memcpy(el->data, buf, len);
	} وگرنه {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	اگر (el->prev) {
		el->prev->next = el;
	} وگرنه {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	بازگشت 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	بازگشت -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ایستا تهی transaction_next_hash_chain(ساختار tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	برای (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		اگر (0 != tdb->transaction->hash_heads[h+1]) {
			بشکن;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ایستا صحیح transaction_oob(ساختار tdb_context *tdb, tdb_off_t len,
			   صحیح probe EXT2FS_ATTR((unused)))
{
	اگر (len <= tdb->map_size) {
		بازگشت 0;
	}
	بازگشت TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ایستا صحیح transaction_expand_file(ساختار tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	اگر (transaction_write(tdb, size, پوچ, addition) != 0) {
		بازگشت -1;
	}

	بازگشت 0;
}

/*
  brlock during a transaction - ignore them
*/
ایستا صحیح transaction_brlock(ساختار tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      صحیح rw_type EXT2FS_ATTR((unused)),
			      صحیح lck_type EXT2FS_ATTR((unused)),
			      صحیح probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	بازگشت 0;
}

ایستا ثابت ساختار tdb_methods transaction_methods = {
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
صحیح tdb_transaction_start(ساختار tdb_context *tdb)
{
	/* some sanity checks */
	اگر (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		بازگشت -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	اگر (tdb->transaction != پوچ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		بازگشت 0;
	}

	اگر (tdb->رقم_locks != 0 || tdb->global_lock.شمار) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		بازگشت -1;
	}

	اگر (tdb->travlocks.next != پوچ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		بازگشت -1;
	}

	tdb->transaction = (ساختار tdb_transaction *)
		calloc(اندازه(ساختار tdb_transaction), 1);
	اگر (tdb->transaction == پوچ) {
		tdb->ecode = TDB_ERR_OOM;
		بازگشت -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	اگر (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		بازگشت -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	اگر (tdb_brlock(tdb, FREELIST_بالاترین, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		برو fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, اندازه(u32));
	اگر (tdb->transaction->hash_heads == پوچ) {
		tdb->ecode = TDB_ERR_OOM;
		برو fail;
	}
	اگر (tdb->methods->tdb_read(tdb, FREELIST_بالاترین, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		برو fail;
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
	اگر (transaction_write(tdb, FREELIST_بالاترین, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		برو fail;
	}

	بازگشت 0;

fail:
	tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	بازگشت -1;
}


/*
  cancel the current transaction
*/
صحیح tdb_transaction_cancel(ساختار tdb_context *tdb)
{
	اگر (tdb->transaction == پوچ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		بازگشت -1;
	}

	اگر (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		بازگشت 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	وقتی (tdb->transaction->elements) {
		ساختار tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		آزاد(el->data);
		آزاد(el);
	}

	/* remove any global lock created during the transaction */
	اگر (tdb->global_lock.شمار != 0) {
		tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.شمار = 0;
	}

	/* remove any locks created during the transaction */
	اگر (tdb->رقم_locks != 0) {
		صحیح i;
		برای (i=0;i<tdb->رقم_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_بالاترین+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->رقم_locks = 0;
		tdb->رقم_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	بازگشت 0;
}

/*
  sync to disk
*/
ایستا صحیح transaction_sync(ساختار tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	اگر (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		بازگشت -1;
	}
#اگر defined(HAVE_MSYNC) && defined(MS_SYNC)
	اگر (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		اگر (msync(moffset + (نویسه *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			بازگشت -1;
		}
	}
#endif
	بازگشت 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ایستا tdb_len_t tdb_recovery_size(ساختار tdb_context *tdb)
{
	ساختار tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = اندازه(u32);
	برای (el=tdb->transaction->elements;el;el=el->next) {
		اگر (el->offset >= tdb->transaction->old_map_size) {
			ادامه;
		}
		recovery_size += 2*اندازه(tdb_off_t) + el->length;
	}

	بازگشت recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ایستا صحیح tdb_recovery_allocate(ساختار tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	ساختار list_struct rec;
	ثابت ساختار tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	اگر (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		بازگشت -1;
	}

	rec.rec_len = 0;

	اگر (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, اندازه(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		بازگشت -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	اگر (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		بازگشت 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	اگر (recovery_head != 0) {
		اگر (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			بازگشت -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(اندازه(rec) + *recovery_size, tdb->page_size) - اندازه(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	اگر (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     اندازه(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		بازگشت -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	اگر (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, اندازه(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		بازگشت -1;
	}

	بازگشت 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ایستا صحیح transaction_setup_recovery(ساختار tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	ساختار tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned نویسه *data, *p;
	ثابت ساختار tdb_methods *methods = tdb->transaction->io_methods;
	ساختار list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	اگر (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		بازگشت -1;
	}

	data = (unsigned نویسه *)malloc(recovery_size + اندازه(*rec));
	اگر (data == پوچ) {
		tdb->ecode = TDB_ERR_OOM;
		بازگشت -1;
	}

	rec = (ساختار list_struct *)data;
	memset(rec, 0, اندازه(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->کلید_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + اندازه(*rec);
	برای (el=tdb->transaction->elements;el;el=el->next) {
		اگر (el->offset >= old_map_size) {
			ادامه;
		}
		اگر (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			آزاد(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			بازگشت -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		اگر (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		اگر (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			آزاد(data);
			tdb->ecode = TDB_ERR_IO;
			بازگشت -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = اندازه(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	اگر (methods->tdb_write(tdb, recovery_offset, data, اندازه(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		آزاد(data);
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	اگر (transaction_sync(tdb, recovery_offset, اندازه(*rec) + recovery_size) == -1) {
		آزاد(data);
		بازگشت -1;
	}

	آزاد(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(ساختار list_struct, magic);

	اگر (methods->tdb_write(tdb, *magic_offset, &magic, اندازه(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	/* ensure the recovery magic marker is on disk */
	اگر (transaction_sync(tdb, *magic_offset, اندازه(magic)) == -1) {
		بازگشت -1;
	}

	بازگشت 0;
}

/*
  commit the current transaction
*/
صحیح tdb_transaction_commit(ساختار tdb_context *tdb)
{
	ثابت ساختار tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	اگر (tdb->transaction == پوچ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		بازگشت -1;
	}

	اگر (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		بازگشت -1;
	}

	اگر (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		بازگشت 0;
	}

	/* check for a null transaction */
	اگر (tdb->transaction->elements == پوچ) {
		tdb_transaction_cancel(tdb);
		بازگشت 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	اگر (tdb->رقم_locks || tdb->global_lock.شمار) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		بازگشت -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	اگر (tdb_brlock_upgrade(tdb, FREELIST_بالاترین, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		بازگشت -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	اگر (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		بازگشت -1;
	}

	اگر (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		اگر (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			بازگشت -1;
		}
	}

	/* expand the file to the new size if needed */
	اگر (tdb->map_size != tdb->transaction->old_map_size) {
		اگر (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			بازگشت -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	وقتی (tdb->transaction->elements) {
		ساختار tdb_transaction_el *el = tdb->transaction->elements;

		اگر (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			بازگشت -1;
		}
		tdb->transaction->elements = el->next;
		آزاد(el->data);
		آزاد(el);
	}

	اگر (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		اگر (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			بازگشت -1;
		}

		/* remove the recovery marker */
		اگر (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			بازگشت -1;
		}

		/* ensure the recovery marker has been removed on disk */
		اگر (transaction_sync(tdb, magic_offset, 4) == -1) {
			بازگشت -1;
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
	utime(tdb->name, پوچ);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	بازگشت 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
صحیح tdb_transaction_recover(ساختار tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned نویسه *data, *p;
	u32 zero = 0;
	ساختار list_struct rec;

	/* find the recovery area */
	اگر (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	اگر (recovery_head == 0) {
		/* we have never allocated a recovery record */
		بازگشت 0;
	}

	/* read the recovery record */
	اگر (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   اندازه(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	اگر (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		بازگشت 0;
	}

	اگر (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		بازگشت -1;
	}

	recovery_eof = rec.کلید_len;

	data = (unsigned نویسه *)malloc(rec.data_len);
	اگر (data == پوچ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		بازگشت -1;
	}

	/* read the full recovery data */
	اگر (tdb->methods->tdb_read(tdb, recovery_head + اندازه(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		آزاد(data);
		بازگشت -1;
	}

	/* recover the file data */
	p = data;
	وقتی (p+8 < data + rec.data_len) {
		u32 ofs, len;
		اگر (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		اگر (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			آزاد(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			بازگشت -1;
		}
		p += 8 + len;
	}

	آزاد(data);

	اگر (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	اگر (recovery_eof <= recovery_head) {
		اگر (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			بازگشت -1;
		}
	}

	/* remove the recovery magic */
	اگر (tdb_ofs_write(tdb, recovery_head + offsetof(ساختار list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	اگر (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	اگر (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		بازگشت -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	بازگشت 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ایستا صحیح tdb_rec_free_read(ساختار tdb_context *tdb, tdb_off_t off, ساختار list_struct *rec)
{
	اگر (tdb->methods->tdb_read(tdb, off, rec, اندازه(*rec),DOCONV()) == -1)
		بازگشت -1;

	اگر (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		اگر (tdb->methods->tdb_write(tdb, off, rec, اندازه(*rec)) == -1)
			بازگشت -1;
	}

	اگر (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		بازگشت TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	اگر (tdb->methods->tdb_oob(tdb, rec->next+اندازه(*rec), 0) != 0)
		بازگشت -1;
	بازگشت 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ایستا صحیح remove_from_freelist(ساختار tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_بالاترین;
	وقتی (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		اگر (i == off) {
			/* We've found it! */
			بازگشت tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	بازگشت TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ایستا صحیح update_tailer(ساختار tdb_context *tdb, tdb_off_t offset,
			 ثابت ساختار list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = اندازه(*rec) + rec->rec_len;
	بازگشت tdb_ofs_write(tdb, offset + totalsize - اندازه(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
صحیح tdb_free(ساختار tdb_context *tdb, tdb_off_t offset, ساختار list_struct *rec)
{
	tdb_off_t راست, چپ;

	/* Allocation and tailer lock */
	اگر (tdb_lock(tdb, -1, F_WRLCK) != 0)
		بازگشت -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	اگر (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		برو fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	راست = offset + اندازه(*rec) + rec->rec_len;
	اگر (راست + اندازه(*rec) <= tdb->map_size) {
		ساختار list_struct r;

		اگر (tdb->methods->tdb_read(tdb, راست, &r, اندازه(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", راست));
			برو چپ;
		}

		/* If it's free, expand to include it. */
		اگر (r.magic == TDB_FREE_MAGIC) {
			اگر (remove_from_freelist(tdb, راست, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", راست));
				برو چپ;
			}
			rec->rec_len += اندازه(r) + r.rec_len;
		}
	}

چپ:
	/* Look left */
	چپ = offset - اندازه(tdb_off_t);
	اگر (چپ > TDB_DATA_START(tdb->header.hash_size)) {
		ساختار list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		اگر (tdb_ofs_read(tdb, چپ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", چپ));
			برو update;
		}

		/* it could be uninitialised data */
		اگر (leftsize == 0 || leftsize == TDB_PAD_U32) {
			برو update;
		}

		چپ = offset - leftsize;

		/* Now read in record */
		اگر (tdb->methods->tdb_read(tdb, چپ, &l, اندازه(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", چپ, leftsize));
			برو update;
		}

		/* If it's free, expand to include it. */
		اگر (l.magic == TDB_FREE_MAGIC) {
			اگر (remove_from_freelist(tdb, چپ, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", چپ));
				برو update;
			} وگرنه {
				offset = چپ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	اگر (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		برو fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	اگر (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_بالاترین, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		برو fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ایستا tdb_off_t tdb_allocate_ofs(ساختار tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				ساختار list_struct *rec, tdb_off_t last_ptr)
{
	ساختار list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', اندازه(newrec));

	/* found it - now possibly split it up  */
	اگر (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (اندازه(*rec) + length);
		newrec_ptr = rec_ptr + اندازه(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} وگرنه {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	اگر (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		بازگشت 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	اگر (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		بازگشت 0;
	}

	/* Did we create new block? */
	اگر (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		اگر (update_tailer(tdb, rec_ptr, rec) == -1) {
			بازگشت 0;
		}

		/* Free new record */
		اگر (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			بازگشت 0;
		}
	}

	/* all done - return the new record offset */
	بازگشت rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ساختار tdb_context *tdb, tdb_len_t length, ساختار list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	ساختار {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	اگر (tdb_lock(tdb, -1, F_WRLCK) == -1)
		بازگشت 0;

	/* Extra bytes required for tailer */
	length += اندازه(tdb_off_t);

 again:
	last_ptr = FREELIST_بالاترین;

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec_ptr) == -1)
		برو fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	وقتی (rec_ptr) {
		اگر (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			برو fail;
		}

		اگر (rec->rec_len >= length) {
			اگر (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				اگر (bestfit.rec_len < 2*length) {
					بشکن;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	اگر (bestfit.rec_ptr != 0) {
		اگر (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			برو fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		بازگشت newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	اگر (tdb_expand(tdb, length + اندازه(*rec)) == 0)
		برو again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ایستا صحیح seen_درج(ساختار tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA کلید, data;

	memset(&data, '\0', اندازه(data));
	کلید.dptr = (unsigned نویسه *)&rec_ptr;
	کلید.dsize = اندازه(rec_ptr);
	بازگشت tdb_store(mem_tdb, کلید, data, TDB_درج);
}

صحیح tdb_validate_freelist(ساختار tdb_context *tdb, صحیح *pnum_entries)
{
	ساختار tdb_context *mem_tdb = پوچ;
	ساختار list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	صحیح ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	اگر (!mem_tdb) {
		بازگشت -1;
	}

	اگر (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		بازگشت 0;
	}

	last_ptr = FREELIST_بالاترین;

	/* Store the FREELIST_TOP record. */
	اگر (seen_درج(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		برو fail;
	}

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec_ptr) == -1) {
		برو fail;
	}

	وقتی (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		اگر (seen_درج(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			برو fail;
		}

		اگر (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			برو fail;
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
	بازگشت ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ایستا صحیح tdb_next_lock(ساختار tdb_context *tdb, ساختار tdb_traverse_lock *tlock,
			 ساختار list_struct *rec)
{
	صحیح want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	برای (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		اگر (!tlock->off && tlock->hash != 0) {
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
			اگر (tlock->hash == tdb->header.hash_size) {
				ادامه;
			}
		}

		اگر (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			بازگشت -1;

		/* No previous record?  Start at top of chain. */
		اگر (!tlock->off) {
			اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(tlock->hash),
				     &tlock->off) == -1)
				برو fail;
		} وگرنه {
			/* Otherwise unlock the previous record. */
			اگر (tdb_unlock_record(tdb, tlock->off) != 0)
				برو fail;
		}

		اگر (want_next) {
			/* We have offset of old record: grab next */
			اگر (tdb_rec_read(tdb, tlock->off, rec) == -1)
				برو fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		وقتی( tlock->off) {
			tdb_off_t current;
			اگر (tdb_rec_read(tdb, tlock->off, rec) == -1)
				برو fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			اگر (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				برو fail;
			}

			اگر (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				اگر (tdb_lock_record(tdb, tlock->off) != 0)
					برو fail;
				بازگشت tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			اگر (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				برو fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	بازگشت TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	اگر (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	بازگشت -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ایستا صحیح tdb_traverse_internal(ساختار tdb_context *tdb,
				 tdb_traverse_func fn, تهی *private_data,
				 ساختار tdb_traverse_lock *tl)
{
	TDB_DATA کلید, dbuf;
	ساختار list_struct rec;
	صحیح ret, شمار = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	وقتی ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		شمار++;
		/* now read the full record */
		کلید.dptr = tdb_alloc_read(tdb, tl->off + اندازه(rec),
					  rec.کلید_len + rec.data_len);
		اگر (!کلید.dptr) {
			ret = -1;
			اگر (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				برو out;
			اگر (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			برو out;
		}
		کلید.dsize = rec.کلید_len;
		dbuf.dptr = کلید.dptr + rec.کلید_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		اگر (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(کلید.dptr);
			برو out;
		}
		اگر (fn && fn(tdb, کلید, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = شمار;
			اگر (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(کلید.dptr);
			برو out;
		}
		SAFE_FREE(کلید.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	اگر (ret < 0)
		بازگشت -1;
	وگرنه
		بازگشت شمار;
}


/*
  a write style traverse - temporarily marks the db read only
*/
صحیح tdb_traverse_read(ساختار tdb_context *tdb,
		      tdb_traverse_func fn, تهی *private_data)
{
	ساختار tdb_traverse_lock tl = { پوچ, 0, 0, F_RDLCK };
	صحیح ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	اگر (tdb_transaction_lock(tdb, F_RDLCK)) {
		بازگشت -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	بازگشت ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
صحیح tdb_traverse(ساختار tdb_context *tdb,
		 tdb_traverse_func fn, تهی *private_data)
{
	ساختار tdb_traverse_lock tl = { پوچ, 0, 0, F_WRLCK };
	صحیح ret;

	اگر (tdb->read_only || tdb->traverse_read) {
		بازگشت tdb_traverse_read(tdb, fn, private_data);
	}

	اگر (tdb_transaction_lock(tdb, F_WRLCK)) {
		بازگشت -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	بازگشت ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(ساختار tdb_context *tdb)
{
	TDB_DATA کلید;
	ساختار list_struct rec;

	/* release any old lock */
	اگر (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		بازگشت tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	اگر (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		بازگشت tdb_null;
	/* now read the key */
	کلید.dsize = rec.کلید_len;
	کلید.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+اندازه(rec),کلید.dsize);

	/* Unlock the hash chain of the record we just read. */
	اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	بازگشت کلید;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(ساختار tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA کلید = tdb_null;
	ساختار list_struct rec;
	unsigned نویسه *k = پوچ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	اگر (tdb->travlocks.off) {
		اگر (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			بازگشت tdb_null;
		اگر (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+اندازه(rec),
					    rec.کلید_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			اگر (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				بازگشت tdb_null;
			}
			اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				بازگشت tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	اگر (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		اگر (!tdb->travlocks.off)
			بازگشت tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		اگر (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			بازگشت tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	اگر (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		کلید.dsize = rec.کلید_len;
		کلید.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+اندازه(rec),
					  کلید.dsize);
		/* Unlock the chain of this new record */
		اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	اگر (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	بازگشت کلید;
}

/* file: dump.c */

ایستا tdb_off_t tdb_dump_record(ساختار tdb_context *tdb, صحیح hash,
				 tdb_off_t offset)
{
	ساختار list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	اگر (tdb->methods->tdb_read(tdb, offset, (نویسه *)&rec,
				   اندازه(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		بازگشت 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.کلید_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + اندازه(rec) + rec.rec_len - اندازه(tdb_off_t);

	اگر (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		بازگشت rec.next;
	}

	اگر (tailer != rec.rec_len + اندازه(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned صحیح)tailer, (unsigned صحیح)(rec.rec_len + اندازه(rec)));
	}
	بازگشت rec.next;
}

ایستا صحیح tdb_dump_chain(ساختار tdb_context *tdb, صحیح i)
{
	tdb_off_t rec_ptr, بالاترین;

	بالاترین = TDB_HASH_بالاترین(i);

	اگر (tdb_lock(tdb, i, F_WRLCK) != 0)
		بازگشت -1;

	اگر (tdb_ofs_read(tdb, بالاترین, &rec_ptr) == -1)
		بازگشت tdb_unlock(tdb, i, F_WRLCK);

	اگر (rec_ptr)
		printf("hash=%d\n", i);

	وقتی (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	بازگشت tdb_unlock(tdb, i, F_WRLCK);
}

تهی tdb_dump_all(ساختار tdb_context *tdb)
{
	صحیح i;
	برای (i = 0; i < (صحیح)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

صحیح tdb_printfreelist(ساختار tdb_context *tdb)
{
	صحیح ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	ساختار list_struct rec;

	اگر ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		بازگشت ret;

	offset = FREELIST_بالاترین;

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		بازگشت 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	وقتی (rec_ptr) {
		اگر (tdb->methods->tdb_read(tdb, rec_ptr, (نویسه *)&rec,
					   اندازه(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			بازگشت -1;
		}

		اگر (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			بازگشت -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (صحیح)total_free,
               (صحیح)total_free);

	بازگشت tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
تهی tdb_increment_seqnum_nonblock(ساختار tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	اگر (!(tdb->flags & TDB_SEQNUM)) {
		بازگشت;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	اگر (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		بازگشت;
	seqnum++;
	(تهی) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ایستا تهی tdb_increment_seqnum(ساختار tdb_context *tdb)
{
	اگر (!(tdb->flags & TDB_SEQNUM)) {
		بازگشت;
	}

	اگر (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		بازگشت;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ایستا صحیح tdb_کلید_compare(TDB_DATA کلید, TDB_DATA data,
			   تهی *private_data EXT2FS_ATTR((unused)))
{
	بازگشت memcmp(data.dptr, کلید.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ایستا tdb_off_t tdb_find(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash,
			ساختار list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		بازگشت 0;

	/* keep looking until we find the right record */
	وقتی (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, r) == -1)
			بازگشت 0;

		اگر (!TDB_DEAD(r) && hash==r->full_hash
		    && کلید.dsize==r->کلید_len
		    && tdb_parse_data(tdb, کلید, rec_ptr + اندازه(*r),
				      r->کلید_len, tdb_کلید_compare,
				      پوچ) == 0) {
			بازگشت rec_ptr;
		}
		rec_ptr = r->next;
	}
	بازگشت TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash, صحیح locktype,
			   ساختار list_struct *rec)
{
	u32 rec_ptr;

	اگر (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		بازگشت 0;
	اگر (!(rec_ptr = tdb_find(tdb, کلید, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	بازگشت rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ایستا صحیح tdb_update_hash(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash, TDB_DATA dbuf)
{
	ساختار list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	اگر (!(rec_ptr = tdb_find(tdb, کلید, hash, &rec)))
		بازگشت -1;

	/* must be long enough key, data and tailer */
	اگر (rec.rec_len < کلید.dsize + dbuf.dsize + اندازه(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		بازگشت -1;
	}

	اگر (tdb->methods->tdb_write(tdb, rec_ptr + اندازه(rec) + rec.کلید_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		بازگشت -1;

	اگر (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		بازگشت tdb_rec_write(tdb, rec_ptr, &rec);
	}

	بازگشت 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	tdb_off_t rec_ptr;
	ساختار list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (!(rec_ptr = tdb_find_lock_hash(tdb,کلید,hash,F_RDLCK,&rec)))
		بازگشت tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + اندازه(rec) + rec.کلید_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	بازگشت ret;
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

صحیح tdb_parse_record(ساختار tdb_context *tdb, TDB_DATA کلید,
		     صحیح (*parser)(TDB_DATA کلید, TDB_DATA data,
				   تهی *private_data),
		     تهی *private_data)
{
	tdb_off_t rec_ptr;
	ساختار list_struct rec;
	صحیح ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);

	اگر (!(rec_ptr = tdb_find_lock_hash(tdb,کلید,hash,F_RDLCK,&rec))) {
		بازگشت TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, کلید, rec_ptr + اندازه(rec) + rec.کلید_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	بازگشت ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ایستا صحیح tdb_exists_hash(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash)
{
	ساختار list_struct rec;

	اگر (tdb_find_lock_hash(tdb, کلید, hash, F_RDLCK, &rec) == 0)
		بازگشت 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	بازگشت 1;
}

صحیح tdb_exists(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	بازگشت tdb_exists_hash(tdb, کلید, hash);
}

/* actually delete an entry in the database given the offset */
صحیح tdb_do_delete(ساختار tdb_context *tdb, tdb_off_t rec_ptr, ساختار list_struct*rec)
{
	tdb_off_t last_ptr, i;
	ساختار list_struct lastrec;

	اگر (tdb->read_only || tdb->traverse_read) بازگشت -1;

	اگر (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		بازگشت tdb_rec_write(tdb, rec_ptr, rec);
	}
	اگر (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		بازگشت -1;

	/* find previous record in hash chain */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(rec->full_hash), &i) == -1)
		بازگشت -1;
	برای (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		اگر (tdb_rec_read(tdb, i, &lastrec) == -1)
			بازگشت -1;

	/* unlink it: next ptr is at start of record. */
	اگر (last_ptr == 0)
		last_ptr = TDB_HASH_بالاترین(rec->full_hash);
	اگر (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		بازگشت -1;

	/* recover the space */
	اگر (tdb_free(tdb, rec_ptr, rec) == -1)
		بازگشت -1;
	بازگشت 0;
}

ایستا صحیح tdb_شمار_dead(ساختار tdb_context *tdb, u32 hash)
{
	صحیح res = 0;
	tdb_off_t rec_ptr;
	ساختار list_struct rec;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		بازگشت 0;

	وقتی (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			بازگشت 0;

		اگر (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	بازگشت res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ایستا صحیح tdb_purge_dead(ساختار tdb_context *tdb, u32 hash)
{
	صحیح res = -1;
	ساختار list_struct rec;
	tdb_off_t rec_ptr;

	اگر (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		بازگشت -1;
	}

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		برو fail;

	وقتی (rec_ptr) {
		tdb_off_t next;

		اگر (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			برو fail;
		}

		next = rec.next;

		اگر (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			برو fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	بازگشت res;
}

/* delete an entry in the database given a key */
ایستا صحیح tdb_delete_hash(ساختار tdb_context *tdb, TDB_DATA کلید, u32 hash)
{
	tdb_off_t rec_ptr;
	ساختار list_struct rec;
	صحیح ret;

	اگر (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		اگر (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			بازگشت -1;

		اگر (tdb_شمار_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		اگر (!(rec_ptr = tdb_find(tdb, کلید, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			بازگشت -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	وگرنه {
		اگر (!(rec_ptr = tdb_find_lock_hash(tdb, کلید, hash, F_WRLCK,
						   &rec)))
			بازگشت -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	اگر (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	اگر (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	بازگشت ret;
}

صحیح tdb_delete(ساختار tdb_context *tdb, TDB_DATA کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	بازگشت tdb_delete_hash(tdb, کلید, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ایستا tdb_off_t tdb_find_dead(ساختار tdb_context *tdb, u32 hash,
			       ساختار list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		بازگشت 0;

	/* keep looking until we find the right record */
	وقتی (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, r) == -1)
			بازگشت 0;

		اگر (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			بازگشت rec_ptr;
		}
		rec_ptr = r->next;
	}
	بازگشت 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
صحیح tdb_store(ساختار tdb_context *tdb, TDB_DATA کلید, TDB_DATA dbuf, صحیح flag)
{
	ساختار list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	نویسه *p = پوچ;
	صحیح ret = -1;

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		بازگشت -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		بازگشت -1;

	/* check for it existing, on insert. */
	اگر (flag == TDB_درج) {
		اگر (tdb_exists_hash(tdb, کلید, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			برو fail;
		}
	} وگرنه {
		/* first try in-place update, on modify or replace. */
		اگر (tdb_update_hash(tdb, کلید, hash, dbuf) == 0) {
			برو done;
		}
		اگر (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			برو fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	اگر (flag != TDB_درج)
		tdb_delete_hash(tdb, کلید, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	اگر (!(p = (نویسه *)malloc(کلید.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		برو fail;
	}

	memcpy(p, کلید.dptr, کلید.dsize);
	اگر (dbuf.dsize)
		memcpy(p+کلید.dsize, dbuf.dptr, dbuf.dsize);

	اگر (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			کلید.dsize + dbuf.dsize + اندازه(tdb_off_t));

		اگر (rec_ptr != 0) {
			rec.کلید_len = کلید.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			اگر (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + اندازه(rec),
				    p, کلید.dsize + dbuf.dsize) == -1) {
				برو fail;
			}
			برو done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	اگر (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		برو fail;
	}

	اگر ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		برو fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, کلید.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	اگر (rec_ptr == 0) {
		برو fail;
	}

	/* Read hash top into next ptr */
	اگر (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec.next) == -1)
		برو fail;

	rec.کلید_len = کلید.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	اگر (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+اندازه(rec), p, کلید.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		برو fail;
	}

 done:
	ret = 0;
 fail:
	اگر (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	بازگشت ret;
}


/* Append to an entry. Create if not exist. */
صحیح tdb_append(ساختار tdb_context *tdb, TDB_DATA کلید, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	صحیح ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		بازگشت -1;

	dbuf = tdb_fetch(tdb, کلید);

	اگر (dbuf.dptr == پوچ) {
		dbuf.dptr = (unsigned نویسه *)malloc(new_dbuf.dsize);
	} وگرنه {
		unsigned نویسه *new_dptr = (unsigned نویسه *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		اگر (new_dptr == پوچ) {
			آزاد(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	اگر (dbuf.dptr == پوچ) {
		tdb->ecode = TDB_ERR_OOM;
		برو failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, کلید, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	بازگشت ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ثابت نویسه *tdb_name(ساختار tdb_context *tdb)
{
	بازگشت tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
صحیح tdb_fd(ساختار tdb_context *tdb)
{
	بازگشت tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ساختار tdb_context *tdb)
{
	بازگشت tdb->log.log_fn;
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
صحیح tdb_get_seqnum(ساختار tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	اگر (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		بازگشت 0;
	بازگشت seqnum;
}

صحیح tdb_hash_size(ساختار tdb_context *tdb)
{
	بازگشت tdb->header.hash_size;
}

size_t tdb_map_size(ساختار tdb_context *tdb)
{
	بازگشت tdb->map_size;
}

صحیح tdb_get_flags(ساختار tdb_context *tdb)
{
	بازگشت tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
تهی tdb_enable_seqnum(ساختار tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ایستا ساختار tdb_context *tdbs = پوچ;


/* This is from a hash algorithm suggested by Rogier Wolff */
ایستا unsigned صحیح default_tdb_hash(TDB_DATA *کلید)
{
	u32 مقدار;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	برای (مقدار = 0, i=0; i < کلید->dsize; i++)
		مقدار = مقدار * 256 + کلید->dptr[i] + (مقدار >> 24) * 241;

	بازگشت مقدار;
}


/* initialise a new database with a specified hash size */
ایستا صحیح tdb_new_database(ساختار tdb_context *tdb, صحیح hash_size)
{
	ساختار tdb_header *newdb;
	صحیح size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = اندازه(ساختار tdb_header) + (hash_size+1)*اندازه(tdb_off_t);
	اگر (!(newdb = (ساختار tdb_header *)calloc(size, 1)))
		بازگشت TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	اگر (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (نویسه *)newdb;
		memcpy(&tdb->header, newdb, اندازه(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		بازگشت 0;
	}
	اگر (lseek(tdb->fd, 0, SEEK_SET) == -1)
		برو fail;

	اگر (ftruncate(tdb->fd, 0) == -1)
		برو fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, اندازه(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	اگر (اكتب(tdb->fd, newdb, size) != size) {
		ret = -1;
	} وگرنه {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	بازگشت ret;
}



ایستا صحیح tdb_already_open(dev_t device,
			    ino_t ino)
{
	ساختار tdb_context *i;

	برای (i = tdbs; i; i = i->next) {
		اگر (i->device == device && i->inode == ino) {
			بازگشت 1;
		}
	}

	بازگشت 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ساختار tdb_context *tdb_open(ثابت نویسه *name, صحیح hash_size, صحیح tdb_flags,
		      صحیح open_flags, mode_t mode)
{
	بازگشت tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, پوچ, پوچ);
}

/* a default logging function */
ایستا تهی null_log_fn(ساختار tdb_context *tdb, شمارش tdb_debug_level level, ثابت نویسه *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ایستا تهی null_log_fn(ساختار tdb_context *tdb EXT2FS_ATTR((unused)),
			شمارش tdb_debug_level level EXT2FS_ATTR((unused)),
			ثابت نویسه *fmt EXT2FS_ATTR((unused)), ...)
{
}


ساختار tdb_context *tdb_open_ex(ثابت نویسه *name, صحیح hash_size, صحیح tdb_flags,
				صحیح open_flags, mode_t mode,
				ثابت ساختار tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ساختار tdb_context *tdb;
	ساختار stat st;
	صحیح rev = 0, locked = 0;
	unsigned نویسه *vp;
	u32 vertest;

	اگر (!(tdb = (ساختار tdb_context *)calloc(1, اندازه *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		برو fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = پوچ;
	tdb->map_ptr = پوچ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	اگر (log_ctx) {
		tdb->log = *log_ctx;
	} وگرنه {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = پوچ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	اگر (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	اگر ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		برو fail;
	}

	اگر (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	اگر ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	اگر (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		اگر (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			برو fail;
		}
		برو internal;
	}

	اگر ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		برو fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	اگر (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		برو fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	اگر ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		اگر (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			برو fail; /* errno set by ftruncate */
		}
	}

	اگر (اقرأ(tdb->fd, &tdb->header, اندازه(tdb->header)) != اندازه(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      اندازه(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		اگر (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			برو fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned نویسه *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	اگر (!rev)
		tdb->flags &= ~TDB_CONVERT;
	وگرنه {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, اندازه(tdb->header));
	}
	اگر (fstat(tdb->fd, &st) == -1)
		برو fail;

	اگر (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		برو fail;
	}

	/* Is it already in the open list?  If so, fail. */
	اگر (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (صحیح)st.st_dev, (صحیح)st.st_ino));
		errno = EBUSY;
		برو fail;
	}

	اگر (!(tdb->name = (نویسه *)strdup(name))) {
		errno = ENOMEM;
		برو fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	اگر (locked) {
		اگر (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			برو fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	اگر (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		اگر (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			برو fail;
	}

	/* if needed, run recovery */
	اگر (tdb_transaction_recover(tdb) == -1) {
		برو fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	اگر (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		برو fail;
	tdb->next = tdbs;
	tdbs = tdb;
	بازگشت tdb;

 fail:
	{ صحیح save_errno = errno;

	اگر (!tdb)
		بازگشت پوچ;

	اگر (tdb->map_ptr) {
		اگر (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		وگرنه
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	اگر (tdb->fd != -1)
		اگر (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	بازگشت پوچ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

تهی tdb_set_max_dead(ساختار tdb_context *tdb, صحیح max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
صحیح tdb_close(ساختار tdb_context *tdb)
{
	ساختار tdb_context **i;
	صحیح ret = 0;

	اگر (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	اگر (tdb->map_ptr) {
		اگر (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		وگرنه
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	اگر (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	برای (i = &tdbs; *i; i = &(*i)->next) {
		اگر (*i == tdb) {
			*i = tdb->next;
			بشکن;
		}
	}

	memset(tdb, 0, اندازه(*tdb));
	SAFE_FREE(tdb);

	بازگشت ret;
}

/* register a logging function */
تهی tdb_set_logging_function(ساختار tdb_context *tdb,
                              ثابت ساختار tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

تهی *tdb_get_logging_private(ساختار tdb_context *tdb)
{
	بازگشت tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
صحیح tdb_reopen(ساختار tdb_context *tdb)
{
	ساختار stat st;

	اگر (tdb->flags & TDB_INTERNAL) {
		بازگشت 0; /* Nothing to do. */
	}

	اگر (tdb->رقم_locks != 0 || tdb->global_lock.شمار) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		برو fail;
	}

	اگر (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		برو fail;
	}

	اگر (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		برو fail;
	}
	اگر (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	اگر (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		برو fail;
	}
	اگر ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		برو fail;
	}
	اگر (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		برو fail;
	}
	اگر (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		برو fail;
	}
	tdb_mmap(tdb);

	بازگشت 0;

fail:
	tdb_close(tdb);
	بازگشت -1;
}

/* reopen all tdb's */
صحیح tdb_reopen_all(صحیح parent_longlived)
{
	ساختار tdb_context *tdb;

	برای (tdb=tdbs; tdb; tdb = tdb->next) {
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
		اگر (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		اگر (tdb_reopen(tdb) != 0)
			بازگشت -1;
	}

	بازگشت 0;
}

/**
 * Flush a database file from the page cache.
 **/
صحیح tdb_flush(ساختار tdb_context *tdb)
{
	اگر (tdb->fd != -1)
		بازگشت fsync(tdb->fd);
	بازگشت 0;
}
