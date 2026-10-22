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

#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#ifdef HAVE_UTIME_H
#include <utime.h>
#endif
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#иначе
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
static char *rep_strdup(const char *s)
{
	char *ret;
	целое length;

	если (!s)
		вернуть NULL;
	length = strlen(s);
	ret = malloc(length + 1);
	если (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	вернуть ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#если (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#иначе
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#если defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
typedef целое bool;
#endif

#include "tdb.h"

static TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

typedef u32 tdb_len_t;
typedef u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned целое)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*sizeof(struct list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_TOP (sizeof(struct tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_TOP(hash) (FREELIST_TOP + (BUCKET(hash)+1)*sizeof(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*sizeof(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_TOP(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(struct tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(struct tdb_header, sequence_number)
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
#define SAFE_FREE(x) do { если ((x) != NULL) {free(x); (x)=NULL;} } пока(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, sizeof(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
struct list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t key_len; /* byte length of key */
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
struct tdb_header {
	char magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_number; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

struct tdb_lock_type {
	целое list;
	u32 count;
	u32 ltype;
};

struct tdb_traverse_lock {
	struct tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	целое lock_rw;
};


struct tdb_methods {
	целое (*tdb_read)(struct tdb_context *, tdb_off_t , void *, tdb_len_t , целое );
	целое (*tdb_write)(struct tdb_context *, tdb_off_t, const void *, tdb_len_t);
	void (*next_hash_chain)(struct tdb_context *, u32 *);
	целое (*tdb_oob)(struct tdb_context *, tdb_off_t , целое );
	целое (*tdb_expand_file)(struct tdb_context *, tdb_off_t , tdb_off_t );
	целое (*tdb_brlock)(struct tdb_context *, tdb_off_t , целое, целое, целое, size_t);
};

struct tdb_context {
	char *name; /* the name of the database */
	void *map_ptr; /* where it is currently mapped */
	целое fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	целое read_only; /* opened read-only */
	целое traverse_read; /* read-only traversal */
	struct tdb_lock_type global_lock;
	целое num_lockrecs;
	struct tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	enum TDB_ERROR ecode; /* error code for last tdb error */
	struct tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	struct tdb_traverse_lock travlocks; /* current traversal locks */
	struct tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	struct tdb_logging_context log;
	unsigned целое (*hash_fn)(TDB_DATA *key);
	целое open_flags; /* flags used in the open - needed by reopen */
	unsigned целое num_locks; /* number of chain locks held */
	const struct tdb_methods *methods;
	struct tdb_transaction *transaction;
	целое page_size;
	целое max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
static целое tdb_munmap(struct tdb_context *tdb);
static void tdb_mmap(struct tdb_context *tdb);
static целое tdb_lock(struct tdb_context *tdb, целое list, целое ltype);
целое tdb_lock_nonblock(struct tdb_context *tdb, целое list, целое ltype);
static целое tdb_unlock(struct tdb_context *tdb, целое list, целое ltype);
static целое tdb_brlock(struct tdb_context *tdb, tdb_off_t offset, целое rw_type, целое lck_type, целое probe, size_t len);
static целое tdb_transaction_lock(struct tdb_context *tdb, целое ltype);
static целое tdb_transaction_unlock(struct tdb_context *tdb);
static целое tdb_brlock_upgrade(struct tdb_context *tdb, tdb_off_t offset, size_t len);
static целое tdb_write_lock_record(struct tdb_context *tdb, tdb_off_t off);
static целое tdb_write_unlock_record(struct tdb_context *tdb, tdb_off_t off);
static целое tdb_ofs_read(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static целое tdb_ofs_write(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static void *tdb_convert(void *buf, u32 size);
static целое tdb_free(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec);
static tdb_off_t tdb_allocate(struct tdb_context *tdb, tdb_len_t length, struct list_struct *rec);
static целое tdb_ofs_read(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static целое tdb_ofs_write(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static целое tdb_lock_record(struct tdb_context *tdb, tdb_off_t off);
static целое tdb_unlock_record(struct tdb_context *tdb, tdb_off_t off);
static целое tdb_rec_read(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec);
static целое tdb_rec_write(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec);
static целое tdb_do_delete(struct tdb_context *tdb, tdb_off_t rec_ptr, struct list_struct *rec);
static unsigned char *tdb_alloc_read(struct tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
static целое tdb_parse_data(struct tdb_context *tdb, TDB_DATA key,
		   tdb_off_t offset, tdb_len_t len,
		   целое (*parser)(TDB_DATA key, TDB_DATA data,
				 void *private_data),
		   void *private_data);
static tdb_off_t tdb_find_lock_hash(struct tdb_context *tdb, TDB_DATA key, u32 hash, целое locktype,
			   struct list_struct *rec);
static void tdb_io_init(struct tdb_context *tdb);
static целое tdb_expand(struct tdb_context *tdb, tdb_off_t size);
static целое tdb_rec_free_read(struct tdb_context *tdb, tdb_off_t off,
		      struct list_struct *rec);


/* file: error.c */

enum TDB_ERROR tdb_error(struct tdb_context *tdb)
{
	вернуть tdb->ecode;
}

static struct tdb_errname {
	enum TDB_ERROR ecode; const char *estring;
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
const char *tdb_errorstr(struct tdb_context *tdb)
{
	u32 i;
	для (i = 0; i < sizeof(emap) / sizeof(struct tdb_errname); i++)
		если (tdb->ecode == emap[i].ecode)
			вернуть emap[i].estring;
	вернуть "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
целое tdb_brlock(struct tdb_context *tdb, tdb_off_t offset,
	       целое rw_type, целое lck_type, целое probe, size_t len)
{
	struct flock fl;
	целое ret;

	если (tdb->flags & TDB_NOLOCK) {
		вернуть 0;
	}

	если ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		вернуть -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	do {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} пока (ret == -1 && errno == EINTR);

	если (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		если (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (целое)len));
		}
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	вернуть 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
целое tdb_brlock_upgrade(struct tdb_context *tdb, tdb_off_t offset, size_t len)
{
	целое count = 1000;
	пока (count--) {
		struct timeval tv;
		если (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			вернуть 0;
		}
		если (errno != EDEADLK) {
			break;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, NULL, NULL, NULL, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	вернуть -1;
}


/* lock a list in the database. list -1 is the alloc list */
static целое _tdb_lock(struct tdb_context *tdb, целое list, целое ltype, целое op)
{
	struct tdb_lock_type *new_lck;
	целое i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	если (tdb->global_lock.count &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		вернуть 0;
	}

	если (tdb->global_lock.count) {
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (list < -1 || list >= (целое)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		вернуть -1;
	}
	если (tdb->flags & TDB_NOLOCK)
		вернуть 0;

	для (i=0; i<tdb->num_lockrecs; i++) {
		если (tdb->lockrecs[i].list == list) {
			если (tdb->lockrecs[i].count == 0) {
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
			tdb->lockrecs[i].count++;
			вернуть 0;
		}
	}

	new_lck = (struct tdb_lock_type *)realloc(
		tdb->lockrecs,
		sizeof(*tdb->lockrecs) * (tdb->num_lockrecs+1));
	если (new_lck == NULL) {
		errno = ENOMEM;
		вернуть -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	если (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_TOP+4*list, ltype, op,
				     0, 1)) {
		вернуть -1;
	}

	tdb->num_locks++;

	tdb->lockrecs[tdb->num_lockrecs].list = list;
	tdb->lockrecs[tdb->num_lockrecs].count = 1;
	tdb->lockrecs[tdb->num_lockrecs].ltype = ltype;
	tdb->num_lockrecs += 1;

	вернуть 0;
}

/* lock a list in the database. list -1 is the alloc list */
целое tdb_lock(struct tdb_context *tdb, целое list, целое ltype)
{
	целое ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	если (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	вернуть ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
целое tdb_lock_nonblock(struct tdb_context *tdb, целое list, целое ltype)
{
	вернуть _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
целое tdb_unlock(struct tdb_context *tdb, целое list, целое ltype)
{
	целое ret = -1;
	целое i;
	struct tdb_lock_type *lck = NULL;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	если (tdb->global_lock.count &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		вернуть 0;
	}

	если (tdb->global_lock.count) {
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (tdb->flags & TDB_NOLOCK)
		вернуть 0;

	/* Sanity checks */
	если (list < -1 || list >= (целое)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		вернуть ret;
	}

	для (i=0; i<tdb->num_lockrecs; i++) {
		если (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			break;
		}
	}

	если ((lck == NULL) || (lck->count == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		вернуть -1;
	}

	если (lck->count > 1) {
		lck->count--;
		вернуть 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	если (mark_lock) {
		ret = 0;
	} иначе {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_TOP+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->num_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	если (tdb->num_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->num_lockrecs-1];
	}
	tdb->num_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	если (tdb->num_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	если (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	вернуть ret;
}

/*
  get the transaction lock
 */
целое tdb_transaction_lock(struct tdb_context *tdb, целое ltype)
{
	если (tdb->have_transaction_lock || tdb->global_lock.count) {
		вернуть 0;
	}
	если (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		вернуть -1;
	}
	tdb->have_transaction_lock = 1;
	вернуть 0;
}

/*
  release the transaction lock
 */
целое tdb_transaction_unlock(struct tdb_context *tdb)
{
	целое ret;
	если (!tdb->have_transaction_lock) {
		вернуть 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	если (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	вернуть ret;
}




/* lock/unlock entire database */
static целое _tdb_lockall(struct tdb_context *tdb, целое ltype, целое op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	если (tdb->read_only || tdb->traverse_read)
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);

	если (tdb->global_lock.count && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.count++;
		вернуть 0;
	}

	если (tdb->global_lock.count) {
		/* a global lock of a different type exists */
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (tdb->num_locks != 0) {
		/* can't combine global and chain locks */
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_TOP, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		если (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		вернуть -1;
	}

	tdb->global_lock.count = 1;
	tdb->global_lock.ltype = ltype;

	вернуть 0;
}



/* unlock entire db */
static целое _tdb_unlockall(struct tdb_context *tdb, целое ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	если (tdb->read_only || tdb->traverse_read) {
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.count == 0) {
		вернуть TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	если (tdb->global_lock.count > 1) {
		tdb->global_lock.count--;
		вернуть 0;
	}

	если (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		вернуть -1;
	}

	tdb->global_lock.count = 0;
	tdb->global_lock.ltype = 0;

	вернуть 0;
}

/* lock entire database with write lock */
целое tdb_lockall(struct tdb_context *tdb)
{
	вернуть _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
целое tdb_lockall_mark(struct tdb_context *tdb)
{
	вернуть _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
целое tdb_lockall_unmark(struct tdb_context *tdb)
{
	вернуть _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
целое tdb_lockall_nonblock(struct tdb_context *tdb)
{
	вернуть _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
целое tdb_unlockall(struct tdb_context *tdb)
{
	вернуть _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
целое tdb_lockall_read(struct tdb_context *tdb)
{
	вернуть _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
целое tdb_lockall_read_nonblock(struct tdb_context *tdb)
{
	вернуть _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
целое tdb_unlockall_read(struct tdb_context *tdb)
{
	вернуть _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
целое tdb_chainlock(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
целое tdb_chainlock_nonblock(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
целое tdb_chainlock_mark(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
целое tdb_chainlock_unmark(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK | TDB_MARK_LOCK);
}

целое tdb_chainunlock(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

целое tdb_chainlock_read(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_RDLCK);
}

целое tdb_chainunlock_read(struct tdb_context *tdb, TDB_DATA key)
{
	вернуть tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_RDLCK);
}



/* record lock stops delete underneath */
целое tdb_lock_record(struct tdb_context *tdb, tdb_off_t off)
{
	вернуть off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
целое tdb_write_lock_record(struct tdb_context *tdb, tdb_off_t off)
{
	struct tdb_traverse_lock *i;
	для (i = &tdb->travlocks; i; i = i->next)
		если (i->off == off)
			вернуть -1;
	вернуть tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
целое tdb_write_unlock_record(struct tdb_context *tdb, tdb_off_t off)
{
	вернуть tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
целое tdb_unlock_record(struct tdb_context *tdb, tdb_off_t off)
{
	struct tdb_traverse_lock *i;
	u32 count = 0;

	если (off == 0)
		вернуть 0;
	для (i = &tdb->travlocks; i; i = i->next)
		если (i->off == off)
			count++;
	вернуть (count == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
static целое tdb_oob(struct tdb_context *tdb, tdb_off_t len, целое probe)
{
	struct stat st;
	если (len <= tdb->map_size)
		вернуть 0;
	если (tdb->flags & TDB_INTERNAL) {
		если (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (целое)len, (целое)tdb->map_size));
		}
		вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	если (fstat(tdb->fd, &st) == -1) {
		вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	если (st.st_size < (off_t)len) {
		если (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (целое)len, (целое)st.st_size));
		}
		вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	если (tdb_munmap(tdb) == -1)
		вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	вернуть 0;
}

/* write a lump of data at a specified offset */
static целое tdb_write(struct tdb_context *tdb, tdb_off_t off,
		     const void *buf, tdb_len_t len)
{
	если (len == 0) {
		вернуть 0;
	}

	если (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		вернуть -1;
	}

	если (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		вернуть -1;

	если (tdb->map_ptr) {
		memcpy(off + (char *)tdb->map_ptr, buf, len);
	} иначе если (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	вернуть 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
void *tdb_convert(void *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	для (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	вернуть buf;
}


/* read a lump of data at a specified offset, maybe convert */
static целое tdb_read(struct tdb_context *tdb, tdb_off_t off, void *buf,
		    tdb_len_t len, целое cv)
{
	если (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		вернуть -1;
	}

	если (tdb->map_ptr) {
		memcpy(buf, off + (char *)tdb->map_ptr, len);
	} иначе {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		если (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (целое)off, (целое)len, (целое)ret, strerror(errno),
				 (целое)tdb->map_size));
			вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	если (cv) {
		tdb_convert(buf, len);
	}
	вернуть 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
static void tdb_next_hash_chain(struct tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	если (tdb->map_ptr) {
		для (;h < tdb->header.hash_size;h++) {
			если (0 != *(u32 *)(TDB_HASH_TOP(h) + (unsigned char *)tdb->map_ptr)) {
				break;
			}
		}
	} иначе {
		u32 off=0;
		для (;h < tdb->header.hash_size;h++) {
			если (tdb_ofs_read(tdb, TDB_HASH_TOP(h), &off) != 0 || off != 0) {
				break;
			}
		}
	}
	(*chain) = h;
}


целое tdb_munmap(struct tdb_context *tdb)
{
	если (tdb->flags & TDB_INTERNAL)
		вернуть 0;

#ifdef HAVE_MMAP
	если (tdb->map_ptr) {
		целое ret = munmap(tdb->map_ptr, tdb->real_map_size);
		если (ret != 0)
			вернуть ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = NULL;
	вернуть 0;
}

void tdb_mmap(struct tdb_context *tdb)
{
	если (tdb->flags & TDB_INTERNAL)
		вернуть;

#ifdef HAVE_MMAP
	если (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(NULL, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		если (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = NULL;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} иначе {
		tdb->map_ptr = NULL;
	}
#иначе
	tdb->map_ptr = NULL;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
static целое tdb_expand_file(struct tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	char buf[1024];

	если (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		вернуть -1;
	}

	если (ftruncate(tdb->fd, size+addition) == -1) {
		char b = 0;
		если (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			вернуть -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, sizeof(buf));
	пока (addition) {
		целое n = addition>sizeof(buf)?sizeof(buf):addition;
		целое ret = pwrite(tdb->fd, buf, n, size);
		если (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			вернуть -1;
		}
		addition -= n;
		size += n;
	}
	вернуть 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
целое tdb_expand(struct tdb_context *tdb, tdb_off_t size)
{
	struct list_struct rec;
	tdb_off_t offset;

	если (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		вернуть -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	если (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	если (!(tdb->flags & TDB_INTERNAL)) {
		если (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			goto fail;
	}

	tdb->map_size += size;

	если (tdb->flags & TDB_INTERNAL) {
		char *new_map_ptr = (char *)realloc(tdb->map_ptr,
						    tdb->map_size);
		если (!new_map_ptr) {
			tdb->map_size -= size;
			goto fail;
		}
		tdb->map_ptr = new_map_ptr;
	} иначе {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',sizeof(rec));
	rec.rec_len = size - sizeof(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	если (tdb_free(tdb, offset, &rec) == -1)
		goto fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть -1;
}

/* read/write a tdb_off_t */
целое tdb_ofs_read(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	вернуть tdb->methods->tdb_read(tdb, offset, (char*)d, sizeof(*d), DOCONV());
}

целое tdb_ofs_write(struct tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	вернуть tdb->methods->tdb_write(tdb, offset, CONVERT(off), sizeof(*d));
}


/* read a lump of data, allocating the space for it */
unsigned char *tdb_alloc_read(struct tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned char *buf;

	/* some systems don't like zero length malloc */
	если (len == 0) {
		len = 1;
	}

	если (!(buf = (unsigned char *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		вернуть TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	если (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		вернуть NULL;
	}
	вернуть buf;
}

/* Give a piece of tdb data to a parser */

целое tdb_parse_data(struct tdb_context *tdb, TDB_DATA key,
		   tdb_off_t offset, tdb_len_t len,
		   целое (*parser)(TDB_DATA key, TDB_DATA data,
				 void *private_data),
		   void *private_data)
{
	TDB_DATA data;
	целое result;

	data.dsize = len;

	если ((tdb->transaction == NULL) && (tdb->map_ptr != NULL)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		если (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			вернуть -1;
		}
		data.dptr = offset + (unsigned char *)tdb->map_ptr;
		вернуть parser(key, data, private_data);
	}

	если (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		вернуть -1;
	}

	result = parser(key, data, private_data);
	free(data.dptr);
	вернуть result;
}

/* read/write a record */
целое tdb_rec_read(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec)
{
	если (tdb->methods->tdb_read(tdb, offset, rec, sizeof(*rec),DOCONV()) == -1)
		вернуть -1;
	если (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		вернуть TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	вернуть tdb->methods->tdb_oob(tdb, rec->next+sizeof(*rec), 0);
}

целое tdb_rec_write(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec)
{
	struct list_struct r = *rec;
	вернуть tdb->methods->tdb_write(tdb, offset, CONVERT(r), sizeof(r));
}

static const struct tdb_methods io_methods = {
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
void tdb_io_init(struct tdb_context *tdb)
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

struct tdb_transaction_el {
	struct tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned char *data;
};

/*
  hold the context of any current transaction
*/
struct tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	const struct tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	struct tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	целое transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	целое nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
static целое transaction_read(struct tdb_context *tdb, tdb_off_t off, void *buf,
			    tdb_len_t len, целое cv)
{
	struct tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	для (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		если (off+len <= el->offset) {
			continue;
		}
		если (off >= el->offset + el->length) {
			continue;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		если (off < el->offset) {
			partial = el->offset - off;
			если (transaction_read(tdb, off, buf, partial, cv) != 0) {
				goto fail;
			}
			len -= partial;
			off += partial;
			buf = (void *)(partial + (char *)buf);
		}
		если (off + len <= el->offset + el->length) {
			partial = len;
		} иначе {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		если (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (void *)(partial + (char *)buf);

		если (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			goto fail;
		}

		вернуть 0;
	}

	/* its not in the transaction elements - do a real read */
	вернуть tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	вернуть -1;
}


/*
  write while in a transaction
*/
static целое transaction_write(struct tdb_context *tdb, tdb_off_t off,
			     const void *buf, tdb_len_t len)
{
	struct tdb_transaction_el *el, *best_el=NULL;

	если (len == 0) {
		вернуть 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	если (len == sizeof(tdb_off_t) && off >= FREELIST_TOP &&
	    off < FREELIST_TOP+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_TOP) / sizeof(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	для (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		если (best_el == NULL && off == el->offset+el->length) {
			best_el = el;
		}

		если (off+len <= el->offset) {
			continue;
		}
		если (off >= el->offset + el->length) {
			continue;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		если (off < el->offset) {
			partial = el->offset - off;
			если (transaction_write(tdb, off, buf, partial) != 0) {
				goto fail;
			}
			len -= partial;
			off += partial;
			buf = (const void *)(partial + (const char *)buf);
		}
		если (off + len <= el->offset + el->length) {
			partial = len;
		} иначе {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (const void *)(partial + (const char *)buf);

		если (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			goto fail;
		}

		вернуть 0;
	}

	/* see if we can append the new entry to an existing entry */
	если (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned char *data = best_el->data;
		el = best_el;
		el->data = (unsigned char *)realloc(el->data,
						    el->length + len);
		если (el->data == NULL) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			вернуть -1;
		}
		если (buf) {
			memcpy(el->data + el->length, buf, len);
		} иначе {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		вернуть 0;
	}

	/* add a new entry at the end of the list */
	el = (struct tdb_transaction_el *)malloc(sizeof(*el));
	если (el == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		вернуть -1;
	}
	el->next = NULL;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned char *)malloc(len);
	если (el->data == NULL) {
		free(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		вернуть -1;
	}
	если (buf) {
		memcpy(el->data, buf, len);
	} иначе {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	если (el->prev) {
		el->prev->next = el;
	} иначе {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	вернуть 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	вернуть -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
static void transaction_next_hash_chain(struct tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	для (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		если (0 != tdb->transaction->hash_heads[h+1]) {
			break;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
static целое transaction_oob(struct tdb_context *tdb, tdb_off_t len,
			   целое probe EXT2FS_ATTR((unused)))
{
	если (len <= tdb->map_size) {
		вернуть 0;
	}
	вернуть TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
static целое transaction_expand_file(struct tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	если (transaction_write(tdb, size, NULL, addition) != 0) {
		вернуть -1;
	}

	вернуть 0;
}

/*
  brlock during a transaction - ignore them
*/
static целое transaction_brlock(struct tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      целое rw_type EXT2FS_ATTR((unused)),
			      целое lck_type EXT2FS_ATTR((unused)),
			      целое probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	вернуть 0;
}

static const struct tdb_methods transaction_methods = {
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
целое tdb_transaction_start(struct tdb_context *tdb)
{
	/* some sanity checks */
	если (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		вернуть -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	если (tdb->transaction != NULL) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		вернуть 0;
	}

	если (tdb->num_locks != 0 || tdb->global_lock.count) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		вернуть -1;
	}

	если (tdb->travlocks.next != NULL) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		вернуть -1;
	}

	tdb->transaction = (struct tdb_transaction *)
		calloc(sizeof(struct tdb_transaction), 1);
	если (tdb->transaction == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		вернуть -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	если (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		вернуть -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	если (tdb_brlock(tdb, FREELIST_TOP, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		goto fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, sizeof(u32));
	если (tdb->transaction->hash_heads == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		goto fail;
	}
	если (tdb->methods->tdb_read(tdb, FREELIST_TOP, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		goto fail;
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
	если (transaction_write(tdb, FREELIST_TOP, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		goto fail;
	}

	вернуть 0;

fail:
	tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	вернуть -1;
}


/*
  cancel the current transaction
*/
целое tdb_transaction_cancel(struct tdb_context *tdb)
{
	если (tdb->transaction == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		вернуть -1;
	}

	если (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		вернуть 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	пока (tdb->transaction->elements) {
		struct tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		free(el->data);
		free(el);
	}

	/* remove any global lock created during the transaction */
	если (tdb->global_lock.count != 0) {
		tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.count = 0;
	}

	/* remove any locks created during the transaction */
	если (tdb->num_locks != 0) {
		целое i;
		для (i=0;i<tdb->num_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_TOP+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->num_locks = 0;
		tdb->num_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	вернуть 0;
}

/*
  sync to disk
*/
static целое transaction_sync(struct tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	если (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		вернуть -1;
	}
#если defined(HAVE_MSYNC) && defined(MS_SYNC)
	если (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		если (msync(moffset + (char *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			вернуть -1;
		}
	}
#endif
	вернуть 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
static tdb_len_t tdb_recovery_size(struct tdb_context *tdb)
{
	struct tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = sizeof(u32);
	для (el=tdb->transaction->elements;el;el=el->next) {
		если (el->offset >= tdb->transaction->old_map_size) {
			continue;
		}
		recovery_size += 2*sizeof(tdb_off_t) + el->length;
	}

	вернуть recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
static целое tdb_recovery_allocate(struct tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	struct list_struct rec;
	const struct tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	если (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		вернуть -1;
	}

	rec.rec_len = 0;

	если (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, sizeof(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		вернуть -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	если (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		вернуть 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	если (recovery_head != 0) {
		если (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			вернуть -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(sizeof(rec) + *recovery_size, tdb->page_size) - sizeof(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	если (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     sizeof(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		вернуть -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	если (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, sizeof(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		вернуть -1;
	}

	вернуть 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
static целое transaction_setup_recovery(struct tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	struct tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned char *data, *p;
	const struct tdb_methods *methods = tdb->transaction->io_methods;
	struct list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	если (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		вернуть -1;
	}

	data = (unsigned char *)malloc(recovery_size + sizeof(*rec));
	если (data == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		вернуть -1;
	}

	rec = (struct list_struct *)data;
	memset(rec, 0, sizeof(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->key_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + sizeof(*rec);
	для (el=tdb->transaction->elements;el;el=el->next) {
		если (el->offset >= old_map_size) {
			continue;
		}
		если (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			free(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			вернуть -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		если (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		если (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			free(data);
			tdb->ecode = TDB_ERR_IO;
			вернуть -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = sizeof(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	если (methods->tdb_write(tdb, recovery_offset, data, sizeof(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		free(data);
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	если (transaction_sync(tdb, recovery_offset, sizeof(*rec) + recovery_size) == -1) {
		free(data);
		вернуть -1;
	}

	free(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(struct list_struct, magic);

	если (methods->tdb_write(tdb, *magic_offset, &magic, sizeof(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	/* ensure the recovery magic marker is on disk */
	если (transaction_sync(tdb, *magic_offset, sizeof(magic)) == -1) {
		вернуть -1;
	}

	вернуть 0;
}

/*
  commit the current transaction
*/
целое tdb_transaction_commit(struct tdb_context *tdb)
{
	const struct tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	если (tdb->transaction == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		вернуть -1;
	}

	если (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		вернуть -1;
	}

	если (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		вернуть 0;
	}

	/* check for a null transaction */
	если (tdb->transaction->elements == NULL) {
		tdb_transaction_cancel(tdb);
		вернуть 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	если (tdb->num_locks || tdb->global_lock.count) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		вернуть -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	если (tdb_brlock_upgrade(tdb, FREELIST_TOP, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		вернуть -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	если (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		вернуть -1;
	}

	если (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		если (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			вернуть -1;
		}
	}

	/* expand the file to the new size if needed */
	если (tdb->map_size != tdb->transaction->old_map_size) {
		если (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			вернуть -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	пока (tdb->transaction->elements) {
		struct tdb_transaction_el *el = tdb->transaction->elements;

		если (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			вернуть -1;
		}
		tdb->transaction->elements = el->next;
		free(el->data);
		free(el);
	}

	если (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		если (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			вернуть -1;
		}

		/* remove the recovery marker */
		если (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			вернуть -1;
		}

		/* ensure the recovery marker has been removed on disk */
		если (transaction_sync(tdb, magic_offset, 4) == -1) {
			вернуть -1;
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
	utime(tdb->name, NULL);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	вернуть 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
целое tdb_transaction_recover(struct tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned char *data, *p;
	u32 zero = 0;
	struct list_struct rec;

	/* find the recovery area */
	если (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	если (recovery_head == 0) {
		/* we have never allocated a recovery record */
		вернуть 0;
	}

	/* read the recovery record */
	если (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   sizeof(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	если (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		вернуть 0;
	}

	если (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		вернуть -1;
	}

	recovery_eof = rec.key_len;

	data = (unsigned char *)malloc(rec.data_len);
	если (data == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		вернуть -1;
	}

	/* read the full recovery data */
	если (tdb->methods->tdb_read(tdb, recovery_head + sizeof(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		free(data);
		вернуть -1;
	}

	/* recover the file data */
	p = data;
	пока (p+8 < data + rec.data_len) {
		u32 ofs, len;
		если (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		если (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			free(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			вернуть -1;
		}
		p += 8 + len;
	}

	free(data);

	если (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	если (recovery_eof <= recovery_head) {
		если (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			вернуть -1;
		}
	}

	/* remove the recovery magic */
	если (tdb_ofs_write(tdb, recovery_head + offsetof(struct list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	если (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	если (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		вернуть -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	вернуть 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
static целое tdb_rec_free_read(struct tdb_context *tdb, tdb_off_t off, struct list_struct *rec)
{
	если (tdb->methods->tdb_read(tdb, off, rec, sizeof(*rec),DOCONV()) == -1)
		вернуть -1;

	если (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		если (tdb->methods->tdb_write(tdb, off, rec, sizeof(*rec)) == -1)
			вернуть -1;
	}

	если (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		вернуть TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	если (tdb->methods->tdb_oob(tdb, rec->next+sizeof(*rec), 0) != 0)
		вернуть -1;
	вернуть 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
static целое remove_from_freelist(struct tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_TOP;
	пока (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		если (i == off) {
			/* We've found it! */
			вернуть tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	вернуть TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
static целое update_tailer(struct tdb_context *tdb, tdb_off_t offset,
			 const struct list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = sizeof(*rec) + rec->rec_len;
	вернуть tdb_ofs_write(tdb, offset + totalsize - sizeof(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
целое tdb_free(struct tdb_context *tdb, tdb_off_t offset, struct list_struct *rec)
{
	tdb_off_t right, left;

	/* Allocation and tailer lock */
	если (tdb_lock(tdb, -1, F_WRLCK) != 0)
		вернуть -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	если (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		goto fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	right = offset + sizeof(*rec) + rec->rec_len;
	если (right + sizeof(*rec) <= tdb->map_size) {
		struct list_struct r;

		если (tdb->methods->tdb_read(tdb, right, &r, sizeof(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", right));
			goto left;
		}

		/* If it's free, expand to include it. */
		если (r.magic == TDB_FREE_MAGIC) {
			если (remove_from_freelist(tdb, right, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", right));
				goto left;
			}
			rec->rec_len += sizeof(r) + r.rec_len;
		}
	}

left:
	/* Look left */
	left = offset - sizeof(tdb_off_t);
	если (left > TDB_DATA_START(tdb->header.hash_size)) {
		struct list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		если (tdb_ofs_read(tdb, left, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", left));
			goto update;
		}

		/* it could be uninitialised data */
		если (leftsize == 0 || leftsize == TDB_PAD_U32) {
			goto update;
		}

		left = offset - leftsize;

		/* Now read in record */
		если (tdb->methods->tdb_read(tdb, left, &l, sizeof(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", left, leftsize));
			goto update;
		}

		/* If it's free, expand to include it. */
		если (l.magic == TDB_FREE_MAGIC) {
			если (remove_from_freelist(tdb, left, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", left));
				goto update;
			} иначе {
				offset = left;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	если (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		goto fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	если (tdb_ofs_read(tdb, FREELIST_TOP, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_TOP, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		goto fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
static tdb_off_t tdb_allocate_ofs(struct tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				struct list_struct *rec, tdb_off_t last_ptr)
{
	struct list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', sizeof(newrec));

	/* found it - now possibly split it up  */
	если (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (sizeof(*rec) + length);
		newrec_ptr = rec_ptr + sizeof(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} иначе {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	если (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		вернуть 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	если (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		вернуть 0;
	}

	/* Did we create new block? */
	если (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		если (update_tailer(tdb, rec_ptr, rec) == -1) {
			вернуть 0;
		}

		/* Free new record */
		если (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			вернуть 0;
		}
	}

	/* all done - return the new record offset */
	вернуть rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(struct tdb_context *tdb, tdb_len_t length, struct list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	struct {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	если (tdb_lock(tdb, -1, F_WRLCK) == -1)
		вернуть 0;

	/* Extra bytes required for tailer */
	length += sizeof(tdb_off_t);

 again:
	last_ptr = FREELIST_TOP;

	/* read in the freelist top */
	если (tdb_ofs_read(tdb, FREELIST_TOP, &rec_ptr) == -1)
		goto fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	пока (rec_ptr) {
		если (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			goto fail;
		}

		если (rec->rec_len >= length) {
			если (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				если (bestfit.rec_len < 2*length) {
					break;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	если (bestfit.rec_ptr != 0) {
		если (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			goto fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		вернуть newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	если (tdb_expand(tdb, length + sizeof(*rec)) == 0)
		goto again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

static целое seen_insert(struct tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA key, data;

	memset(&data, '\0', sizeof(data));
	key.dptr = (unsigned char *)&rec_ptr;
	key.dsize = sizeof(rec_ptr);
	вернуть tdb_store(mem_tdb, key, data, TDB_INSERT);
}

целое tdb_validate_freelist(struct tdb_context *tdb, целое *pnum_entries)
{
	struct tdb_context *mem_tdb = NULL;
	struct list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	целое ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	если (!mem_tdb) {
		вернуть -1;
	}

	если (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		вернуть 0;
	}

	last_ptr = FREELIST_TOP;

	/* Store the FREELIST_TOP record. */
	если (seen_insert(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		goto fail;
	}

	/* read in the freelist top */
	если (tdb_ofs_read(tdb, FREELIST_TOP, &rec_ptr) == -1) {
		goto fail;
	}

	пока (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		если (seen_insert(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			goto fail;
		}

		если (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			goto fail;
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
	вернуть ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
static целое tdb_next_lock(struct tdb_context *tdb, struct tdb_traverse_lock *tlock,
			 struct list_struct *rec)
{
	целое want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	для (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		если (!tlock->off && tlock->hash != 0) {
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
			если (tlock->hash == tdb->header.hash_size) {
				continue;
			}
		}

		если (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			вернуть -1;

		/* No previous record?  Start at top of chain. */
		если (!tlock->off) {
			если (tdb_ofs_read(tdb, TDB_HASH_TOP(tlock->hash),
				     &tlock->off) == -1)
				goto fail;
		} иначе {
			/* Otherwise unlock the previous record. */
			если (tdb_unlock_record(tdb, tlock->off) != 0)
				goto fail;
		}

		если (want_next) {
			/* We have offset of old record: grab next */
			если (tdb_rec_read(tdb, tlock->off, rec) == -1)
				goto fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		пока( tlock->off) {
			tdb_off_t current;
			если (tdb_rec_read(tdb, tlock->off, rec) == -1)
				goto fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			если (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				goto fail;
			}

			если (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				если (tdb_lock_record(tdb, tlock->off) != 0)
					goto fail;
				вернуть tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			если (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				goto fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	вернуть TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	если (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	вернуть -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
static целое tdb_traverse_internal(struct tdb_context *tdb,
				 tdb_traverse_func fn, void *private_data,
				 struct tdb_traverse_lock *tl)
{
	TDB_DATA key, dbuf;
	struct list_struct rec;
	целое ret, count = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	пока ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		count++;
		/* now read the full record */
		key.dptr = tdb_alloc_read(tdb, tl->off + sizeof(rec),
					  rec.key_len + rec.data_len);
		если (!key.dptr) {
			ret = -1;
			если (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				goto out;
			если (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			goto out;
		}
		key.dsize = rec.key_len;
		dbuf.dptr = key.dptr + rec.key_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		если (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(key.dptr);
			goto out;
		}
		если (fn && fn(tdb, key, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = count;
			если (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(key.dptr);
			goto out;
		}
		SAFE_FREE(key.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	если (ret < 0)
		вернуть -1;
	иначе
		вернуть count;
}


/*
  a write style traverse - temporarily marks the db read only
*/
целое tdb_traverse_read(struct tdb_context *tdb,
		      tdb_traverse_func fn, void *private_data)
{
	struct tdb_traverse_lock tl = { NULL, 0, 0, F_RDLCK };
	целое ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	если (tdb_transaction_lock(tdb, F_RDLCK)) {
		вернуть -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	вернуть ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
целое tdb_traverse(struct tdb_context *tdb,
		 tdb_traverse_func fn, void *private_data)
{
	struct tdb_traverse_lock tl = { NULL, 0, 0, F_WRLCK };
	целое ret;

	если (tdb->read_only || tdb->traverse_read) {
		вернуть tdb_traverse_read(tdb, fn, private_data);
	}

	если (tdb_transaction_lock(tdb, F_WRLCK)) {
		вернуть -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	вернуть ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(struct tdb_context *tdb)
{
	TDB_DATA key;
	struct list_struct rec;

	/* release any old lock */
	если (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		вернуть tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	если (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		вернуть tdb_null;
	/* now read the key */
	key.dsize = rec.key_len;
	key.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+sizeof(rec),key.dsize);

	/* Unlock the hash chain of the record we just read. */
	если (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	вернуть key;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(struct tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA key = tdb_null;
	struct list_struct rec;
	unsigned char *k = NULL;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	если (tdb->travlocks.off) {
		если (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			вернуть tdb_null;
		если (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+sizeof(rec),
					    rec.key_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			если (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				вернуть tdb_null;
			}
			если (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				вернуть tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	если (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		если (!tdb->travlocks.off)
			вернуть tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		если (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			вернуть tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	если (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		key.dsize = rec.key_len;
		key.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+sizeof(rec),
					  key.dsize);
		/* Unlock the chain of this new record */
		если (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	если (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	вернуть key;
}

/* file: dump.c */

static tdb_off_t tdb_dump_record(struct tdb_context *tdb, целое hash,
				 tdb_off_t offset)
{
	struct list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	если (tdb->methods->tdb_read(tdb, offset, (char *)&rec,
				   sizeof(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		вернуть 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.key_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + sizeof(rec) + rec.rec_len - sizeof(tdb_off_t);

	если (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		вернуть rec.next;
	}

	если (tailer != rec.rec_len + sizeof(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned целое)tailer, (unsigned целое)(rec.rec_len + sizeof(rec)));
	}
	вернуть rec.next;
}

static целое tdb_dump_chain(struct tdb_context *tdb, целое i)
{
	tdb_off_t rec_ptr, top;

	top = TDB_HASH_TOP(i);

	если (tdb_lock(tdb, i, F_WRLCK) != 0)
		вернуть -1;

	если (tdb_ofs_read(tdb, top, &rec_ptr) == -1)
		вернуть tdb_unlock(tdb, i, F_WRLCK);

	если (rec_ptr)
		printf("hash=%d\n", i);

	пока (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	вернуть tdb_unlock(tdb, i, F_WRLCK);
}

void tdb_dump_all(struct tdb_context *tdb)
{
	целое i;
	для (i = 0; i < (целое)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

целое tdb_printfreelist(struct tdb_context *tdb)
{
	целое ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	struct list_struct rec;

	если ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		вернуть ret;

	offset = FREELIST_TOP;

	/* read in the freelist top */
	если (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		вернуть 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	пока (rec_ptr) {
		если (tdb->methods->tdb_read(tdb, rec_ptr, (char *)&rec,
					   sizeof(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			вернуть -1;
		}

		если (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			вернуть -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (целое)total_free,
               (целое)total_free);

	вернуть tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
void tdb_increment_seqnum_nonblock(struct tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	если (!(tdb->flags & TDB_SEQNUM)) {
		вернуть;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	если (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		вернуть;
	seqnum++;
	(void) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
static void tdb_increment_seqnum(struct tdb_context *tdb)
{
	если (!(tdb->flags & TDB_SEQNUM)) {
		вернуть;
	}

	если (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		вернуть;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

static целое tdb_key_compare(TDB_DATA key, TDB_DATA data,
			   void *private_data EXT2FS_ATTR((unused)))
{
	вернуть memcmp(data.dptr, key.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
static tdb_off_t tdb_find(struct tdb_context *tdb, TDB_DATA key, u32 hash,
			struct list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		вернуть 0;

	/* keep looking until we find the right record */
	пока (rec_ptr) {
		если (tdb_rec_read(tdb, rec_ptr, r) == -1)
			вернуть 0;

		если (!TDB_DEAD(r) && hash==r->full_hash
		    && key.dsize==r->key_len
		    && tdb_parse_data(tdb, key, rec_ptr + sizeof(*r),
				      r->key_len, tdb_key_compare,
				      NULL) == 0) {
			вернуть rec_ptr;
		}
		rec_ptr = r->next;
	}
	вернуть TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(struct tdb_context *tdb, TDB_DATA key, u32 hash, целое locktype,
			   struct list_struct *rec)
{
	u32 rec_ptr;

	если (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		вернуть 0;
	если (!(rec_ptr = tdb_find(tdb, key, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	вернуть rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
static целое tdb_update_hash(struct tdb_context *tdb, TDB_DATA key, u32 hash, TDB_DATA dbuf)
{
	struct list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	если (!(rec_ptr = tdb_find(tdb, key, hash, &rec)))
		вернуть -1;

	/* must be long enough key, data and tailer */
	если (rec.rec_len < key.dsize + dbuf.dsize + sizeof(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		вернуть -1;
	}

	если (tdb->methods->tdb_write(tdb, rec_ptr + sizeof(rec) + rec.key_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		вернуть -1;

	если (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		вернуть tdb_rec_write(tdb, rec_ptr, &rec);
	}

	вернуть 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(struct tdb_context *tdb, TDB_DATA key)
{
	tdb_off_t rec_ptr;
	struct list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	если (!(rec_ptr = tdb_find_lock_hash(tdb,key,hash,F_RDLCK,&rec)))
		вернуть tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + sizeof(rec) + rec.key_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	вернуть ret;
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

целое tdb_parse_record(struct tdb_context *tdb, TDB_DATA key,
		     целое (*parser)(TDB_DATA key, TDB_DATA data,
				   void *private_data),
		     void *private_data)
{
	tdb_off_t rec_ptr;
	struct list_struct rec;
	целое ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);

	если (!(rec_ptr = tdb_find_lock_hash(tdb,key,hash,F_RDLCK,&rec))) {
		вернуть TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, key, rec_ptr + sizeof(rec) + rec.key_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	вернуть ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
static целое tdb_exists_hash(struct tdb_context *tdb, TDB_DATA key, u32 hash)
{
	struct list_struct rec;

	если (tdb_find_lock_hash(tdb, key, hash, F_RDLCK, &rec) == 0)
		вернуть 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	вернуть 1;
}

целое tdb_exists(struct tdb_context *tdb, TDB_DATA key)
{
	u32 hash = tdb->hash_fn(&key);
	вернуть tdb_exists_hash(tdb, key, hash);
}

/* actually delete an entry in the database given the offset */
целое tdb_do_delete(struct tdb_context *tdb, tdb_off_t rec_ptr, struct list_struct*rec)
{
	tdb_off_t last_ptr, i;
	struct list_struct lastrec;

	если (tdb->read_only || tdb->traverse_read) вернуть -1;

	если (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		вернуть tdb_rec_write(tdb, rec_ptr, rec);
	}
	если (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		вернуть -1;

	/* find previous record in hash chain */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(rec->full_hash), &i) == -1)
		вернуть -1;
	для (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		если (tdb_rec_read(tdb, i, &lastrec) == -1)
			вернуть -1;

	/* unlink it: next ptr is at start of record. */
	если (last_ptr == 0)
		last_ptr = TDB_HASH_TOP(rec->full_hash);
	если (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		вернуть -1;

	/* recover the space */
	если (tdb_free(tdb, rec_ptr, rec) == -1)
		вернуть -1;
	вернуть 0;
}

static целое tdb_count_dead(struct tdb_context *tdb, u32 hash)
{
	целое res = 0;
	tdb_off_t rec_ptr;
	struct list_struct rec;

	/* read in the hash top */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		вернуть 0;

	пока (rec_ptr) {
		если (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			вернуть 0;

		если (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	вернуть res;
}

/*
 * Purge all DEAD records from a hash chain
 */
static целое tdb_purge_dead(struct tdb_context *tdb, u32 hash)
{
	целое res = -1;
	struct list_struct rec;
	tdb_off_t rec_ptr;

	если (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		вернуть -1;
	}

	/* read in the hash top */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		goto fail;

	пока (rec_ptr) {
		tdb_off_t next;

		если (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			goto fail;
		}

		next = rec.next;

		если (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			goto fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	вернуть res;
}

/* delete an entry in the database given a key */
static целое tdb_delete_hash(struct tdb_context *tdb, TDB_DATA key, u32 hash)
{
	tdb_off_t rec_ptr;
	struct list_struct rec;
	целое ret;

	если (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		если (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			вернуть -1;

		если (tdb_count_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		если (!(rec_ptr = tdb_find(tdb, key, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			вернуть -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	иначе {
		если (!(rec_ptr = tdb_find_lock_hash(tdb, key, hash, F_WRLCK,
						   &rec)))
			вернуть -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	если (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	если (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	вернуть ret;
}

целое tdb_delete(struct tdb_context *tdb, TDB_DATA key)
{
	u32 hash = tdb->hash_fn(&key);
	вернуть tdb_delete_hash(tdb, key, hash);
}

/*
 * See if we have a dead record around with enough space
 */
static tdb_off_t tdb_find_dead(struct tdb_context *tdb, u32 hash,
			       struct list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		вернуть 0;

	/* keep looking until we find the right record */
	пока (rec_ptr) {
		если (tdb_rec_read(tdb, rec_ptr, r) == -1)
			вернуть 0;

		если (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			вернуть rec_ptr;
		}
		rec_ptr = r->next;
	}
	вернуть 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
целое tdb_store(struct tdb_context *tdb, TDB_DATA key, TDB_DATA dbuf, целое flag)
{
	struct list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	char *p = NULL;
	целое ret = -1;

	если (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		вернуть -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	если (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		вернуть -1;

	/* check for it existing, on insert. */
	если (flag == TDB_INSERT) {
		если (tdb_exists_hash(tdb, key, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			goto fail;
		}
	} иначе {
		/* first try in-place update, on modify or replace. */
		если (tdb_update_hash(tdb, key, hash, dbuf) == 0) {
			goto done;
		}
		если (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			goto fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	если (flag != TDB_INSERT)
		tdb_delete_hash(tdb, key, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	если (!(p = (char *)malloc(key.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		goto fail;
	}

	memcpy(p, key.dptr, key.dsize);
	если (dbuf.dsize)
		memcpy(p+key.dsize, dbuf.dptr, dbuf.dsize);

	если (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			key.dsize + dbuf.dsize + sizeof(tdb_off_t));

		если (rec_ptr != 0) {
			rec.key_len = key.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			если (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + sizeof(rec),
				    p, key.dsize + dbuf.dsize) == -1) {
				goto fail;
			}
			goto done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	если (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		goto fail;
	}

	если ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		goto fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, key.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	если (rec_ptr == 0) {
		goto fail;
	}

	/* Read hash top into next ptr */
	если (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec.next) == -1)
		goto fail;

	rec.key_len = key.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	если (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+sizeof(rec), p, key.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		goto fail;
	}

 done:
	ret = 0;
 fail:
	если (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	вернуть ret;
}


/* Append to an entry. Create if not exist. */
целое tdb_append(struct tdb_context *tdb, TDB_DATA key, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	целое ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	если (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		вернуть -1;

	dbuf = tdb_fetch(tdb, key);

	если (dbuf.dptr == NULL) {
		dbuf.dptr = (unsigned char *)malloc(new_dbuf.dsize);
	} иначе {
		unsigned char *new_dptr = (unsigned char *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		если (new_dptr == NULL) {
			free(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	если (dbuf.dptr == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		goto failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, key, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	вернуть ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
const char *tdb_name(struct tdb_context *tdb)
{
	вернуть tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
целое tdb_fd(struct tdb_context *tdb)
{
	вернуть tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(struct tdb_context *tdb)
{
	вернуть tdb->log.log_fn;
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
целое tdb_get_seqnum(struct tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	если (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		вернуть 0;
	вернуть seqnum;
}

целое tdb_hash_size(struct tdb_context *tdb)
{
	вернуть tdb->header.hash_size;
}

size_t tdb_map_size(struct tdb_context *tdb)
{
	вернуть tdb->map_size;
}

целое tdb_get_flags(struct tdb_context *tdb)
{
	вернуть tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
void tdb_enable_seqnum(struct tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
static struct tdb_context *tdbs = NULL;


/* This is from a hash algorithm suggested by Rogier Wolff */
static unsigned целое default_tdb_hash(TDB_DATA *key)
{
	u32 value;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	для (value = 0, i=0; i < key->dsize; i++)
		value = value * 256 + key->dptr[i] + (value >> 24) * 241;

	вернуть value;
}


/* initialise a new database with a specified hash size */
static целое tdb_new_database(struct tdb_context *tdb, целое hash_size)
{
	struct tdb_header *newdb;
	целое size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = sizeof(struct tdb_header) + (hash_size+1)*sizeof(tdb_off_t);
	если (!(newdb = (struct tdb_header *)calloc(size, 1)))
		вернуть TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	если (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (char *)newdb;
		memcpy(&tdb->header, newdb, sizeof(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		вернуть 0;
	}
	если (lseek(tdb->fd, 0, SEEK_SET) == -1)
		goto fail;

	если (ftruncate(tdb->fd, 0) == -1)
		goto fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, sizeof(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	если (write(tdb->fd, newdb, size) != size) {
		ret = -1;
	} иначе {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	вернуть ret;
}



static целое tdb_already_open(dev_t device,
			    ino_t ino)
{
	struct tdb_context *i;

	для (i = tdbs; i; i = i->next) {
		если (i->device == device && i->inode == ino) {
			вернуть 1;
		}
	}

	вернуть 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
struct tdb_context *tdb_open(const char *name, целое hash_size, целое tdb_flags,
		      целое open_flags, mode_t mode)
{
	вернуть tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, NULL, NULL);
}

/* a default logging function */
static void null_log_fn(struct tdb_context *tdb, enum tdb_debug_level level, const char *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
static void null_log_fn(struct tdb_context *tdb EXT2FS_ATTR((unused)),
			enum tdb_debug_level level EXT2FS_ATTR((unused)),
			const char *fmt EXT2FS_ATTR((unused)), ...)
{
}


struct tdb_context *tdb_open_ex(const char *name, целое hash_size, целое tdb_flags,
				целое open_flags, mode_t mode,
				const struct tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	struct tdb_context *tdb;
	struct stat st;
	целое rev = 0, locked = 0;
	unsigned char *vp;
	u32 vertest;

	если (!(tdb = (struct tdb_context *)calloc(1, sizeof *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		goto fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = NULL;
	tdb->map_ptr = NULL;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	если (log_ctx) {
		tdb->log = *log_ctx;
	} иначе {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = NULL;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	если (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	если ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		goto fail;
	}

	если (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	если ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	если (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		если (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			goto fail;
		}
		goto internal;
	}

	если ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		goto fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	если (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		goto fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	если ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		если (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			goto fail; /* errno set by ftruncate */
		}
	}

	если (read(tdb->fd, &tdb->header, sizeof(tdb->header)) != sizeof(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      sizeof(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		если (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			goto fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned char *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	если (!rev)
		tdb->flags &= ~TDB_CONVERT;
	иначе {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, sizeof(tdb->header));
	}
	если (fstat(tdb->fd, &st) == -1)
		goto fail;

	если (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		goto fail;
	}

	/* Is it already in the open list?  If so, fail. */
	если (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (целое)st.st_dev, (целое)st.st_ino));
		errno = EBUSY;
		goto fail;
	}

	если (!(tdb->name = (char *)strdup(name))) {
		errno = ENOMEM;
		goto fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	если (locked) {
		если (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			goto fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	если (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		если (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			goto fail;
	}

	/* if needed, run recovery */
	если (tdb_transaction_recover(tdb) == -1) {
		goto fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	если (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		goto fail;
	tdb->next = tdbs;
	tdbs = tdb;
	вернуть tdb;

 fail:
	{ целое save_errno = errno;

	если (!tdb)
		вернуть NULL;

	если (tdb->map_ptr) {
		если (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		иначе
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	если (tdb->fd != -1)
		если (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	вернуть NULL;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

void tdb_set_max_dead(struct tdb_context *tdb, целое max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
целое tdb_close(struct tdb_context *tdb)
{
	struct tdb_context **i;
	целое ret = 0;

	если (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	если (tdb->map_ptr) {
		если (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		иначе
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	если (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	для (i = &tdbs; *i; i = &(*i)->next) {
		если (*i == tdb) {
			*i = tdb->next;
			break;
		}
	}

	memset(tdb, 0, sizeof(*tdb));
	SAFE_FREE(tdb);

	вернуть ret;
}

/* register a logging function */
void tdb_set_logging_function(struct tdb_context *tdb,
                              const struct tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

void *tdb_get_logging_private(struct tdb_context *tdb)
{
	вернуть tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
целое tdb_reopen(struct tdb_context *tdb)
{
	struct stat st;

	если (tdb->flags & TDB_INTERNAL) {
		вернуть 0; /* Nothing to do. */
	}

	если (tdb->num_locks != 0 || tdb->global_lock.count) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		goto fail;
	}

	если (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		goto fail;
	}

	если (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		goto fail;
	}
	если (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	если (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		goto fail;
	}
	если ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		goto fail;
	}
	если (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		goto fail;
	}
	если (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		goto fail;
	}
	tdb_mmap(tdb);

	вернуть 0;

fail:
	tdb_close(tdb);
	вернуть -1;
}

/* reopen all tdb's */
целое tdb_reopen_all(целое parent_longlived)
{
	struct tdb_context *tdb;

	для (tdb=tdbs; tdb; tdb = tdb->next) {
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
		если (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		если (tdb_reopen(tdb) != 0)
			вернуть -1;
	}

	вернуть 0;
}

/**
 * Flush a database file from the page cache.
 **/
целое tdb_flush(struct tdb_context *tdb)
{
	если (tdb->fd != -1)
		вернуть fsync(tdb->fd);
	вернуть 0;
}
