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
#ပေႃးဢမ်ႇ
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
static var_na *rep_strdup(const var_na *s)
{
	var_na *ret;
	puur_naa_nka length;

	ပေႃး (!s)
		ပွၵ်ႈ NULL;
	length = strlen(s);
	ret = malloc(length + 1);
	ပေႃး (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	ပွၵ်ႈ ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#ပေႃး (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ပေႃးဢမ်ႇ
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#ပေႃး defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
typedef puur_naa_nka bool;
#endif

#include "tdb.h"

static TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

typedef u32 tdb_len_t;
typedef u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned puur_naa_nka)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*parimaa_na(sa_nracanaa list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_TOP (parimaa_na(sa_nracanaa tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_TOP(hash) (FREELIST_TOP + (BUCKET(hash)+1)*parimaa_na(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*parimaa_na(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_TOP(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(sa_nracanaa tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(sa_nracanaa tdb_header, sequence_number)
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
#define SAFE_FREE(x) do { ပေႃး ((x) != NULL) {free(x); (x)=NULL;} } ၶၢဝ်း(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, parimaa_na(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
sa_nracanaa list_struct {
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
sa_nracanaa tdb_header {
	var_na magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_number; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

sa_nracanaa tdb_lock_type {
	puur_naa_nka list;
	u32 count;
	u32 ltype;
};

sa_nracanaa tdb_traverse_lock {
	sa_nracanaa tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	puur_naa_nka lock_rw;
};


sa_nracanaa tdb_methods {
	puur_naa_nka (*tdb_read)(sa_nracanaa tdb_context *, tdb_off_t , void *, tdb_len_t , puur_naa_nka );
	puur_naa_nka (*tdb_write)(sa_nracanaa tdb_context *, tdb_off_t, const void *, tdb_len_t);
	void (*next_hash_chain)(sa_nracanaa tdb_context *, u32 *);
	puur_naa_nka (*tdb_oob)(sa_nracanaa tdb_context *, tdb_off_t , puur_naa_nka );
	puur_naa_nka (*tdb_expand_file)(sa_nracanaa tdb_context *, tdb_off_t , tdb_off_t );
	puur_naa_nka (*tdb_brlock)(sa_nracanaa tdb_context *, tdb_off_t , puur_naa_nka, puur_naa_nka, puur_naa_nka, size_t);
};

sa_nracanaa tdb_context {
	var_na *name; /* the name of the database */
	void *map_ptr; /* where it is currently mapped */
	puur_naa_nka fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	puur_naa_nka read_only; /* opened read-only */
	puur_naa_nka traverse_read; /* read-only traversal */
	sa_nracanaa tdb_lock_type global_lock;
	puur_naa_nka num_lockrecs;
	sa_nracanaa tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	ga_nanaa TDB_ERROR ecode; /* error code for last tdb error */
	sa_nracanaa tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	sa_nracanaa tdb_traverse_lock travlocks; /* current traversal locks */
	sa_nracanaa tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	sa_nracanaa tdb_logging_context log;
	unsigned puur_naa_nka (*hash_fn)(TDB_DATA *key);
	puur_naa_nka open_flags; /* flags used in the open - needed by reopen */
	unsigned puur_naa_nka num_locks; /* number of chain locks held */
	const sa_nracanaa tdb_methods *methods;
	sa_nracanaa tdb_transaction *transaction;
	puur_naa_nka page_size;
	puur_naa_nka max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
static puur_naa_nka tdb_munmap(sa_nracanaa tdb_context *tdb);
static void tdb_mmap(sa_nracanaa tdb_context *tdb);
static puur_naa_nka tdb_lock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype);
puur_naa_nka tdb_lock_nonblock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype);
static puur_naa_nka tdb_unlock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype);
static puur_naa_nka tdb_brlock(sa_nracanaa tdb_context *tdb, tdb_off_t offset, puur_naa_nka rw_type, puur_naa_nka lck_type, puur_naa_nka probe, size_t len);
static puur_naa_nka tdb_transaction_lock(sa_nracanaa tdb_context *tdb, puur_naa_nka ltype);
static puur_naa_nka tdb_transaction_unlock(sa_nracanaa tdb_context *tdb);
static puur_naa_nka tdb_brlock_upgrade(sa_nracanaa tdb_context *tdb, tdb_off_t offset, size_t len);
static puur_naa_nka tdb_write_lock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off);
static puur_naa_nka tdb_write_unlock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off);
static puur_naa_nka tdb_ofs_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static puur_naa_nka tdb_ofs_write(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static void *tdb_convert(void *buf, u32 size);
static puur_naa_nka tdb_free(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec);
static tdb_off_t tdb_allocate(sa_nracanaa tdb_context *tdb, tdb_len_t length, sa_nracanaa list_struct *rec);
static puur_naa_nka tdb_ofs_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static puur_naa_nka tdb_ofs_write(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
static puur_naa_nka tdb_lock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off);
static puur_naa_nka tdb_unlock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off);
static puur_naa_nka tdb_rec_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec);
static puur_naa_nka tdb_rec_write(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec);
static puur_naa_nka tdb_do_delete(sa_nracanaa tdb_context *tdb, tdb_off_t rec_ptr, sa_nracanaa list_struct *rec);
static unsigned var_na *tdb_alloc_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
static puur_naa_nka tdb_parse_data(sa_nracanaa tdb_context *tdb, TDB_DATA key,
		   tdb_off_t offset, tdb_len_t len,
		   puur_naa_nka (*parser)(TDB_DATA key, TDB_DATA data,
				 void *private_data),
		   void *private_data);
static tdb_off_t tdb_find_lock_hash(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash, puur_naa_nka locktype,
			   sa_nracanaa list_struct *rec);
static void tdb_io_init(sa_nracanaa tdb_context *tdb);
static puur_naa_nka tdb_expand(sa_nracanaa tdb_context *tdb, tdb_off_t size);
static puur_naa_nka tdb_rec_free_read(sa_nracanaa tdb_context *tdb, tdb_off_t off,
		      sa_nracanaa list_struct *rec);


/* file: error.c */

ga_nanaa TDB_ERROR tdb_error(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->ecode;
}

static sa_nracanaa tdb_errname {
	ga_nanaa TDB_ERROR ecode; const var_na *estring;
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
const var_na *tdb_errorstr(sa_nracanaa tdb_context *tdb)
{
	u32 i;
	for (i = 0; i < parimaa_na(emap) / parimaa_na(sa_nracanaa tdb_errname); i++)
		ပေႃး (tdb->ecode == emap[i].ecode)
			ပွၵ်ႈ emap[i].estring;
	ပွၵ်ႈ "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
puur_naa_nka tdb_brlock(sa_nracanaa tdb_context *tdb, tdb_off_t offset,
	       puur_naa_nka rw_type, puur_naa_nka lck_type, puur_naa_nka probe, size_t len)
{
	sa_nracanaa flock fl;
	puur_naa_nka ret;

	ပေႃး (tdb->flags & TDB_NOLOCK) {
		ပွၵ်ႈ 0;
	}

	ပေႃး ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ပွၵ်ႈ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	do {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ၶၢဝ်း (ret == -1 && errno == EINTR);

	ပေႃး (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ပေႃး (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (puur_naa_nka)len));
		}
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	ပွၵ်ႈ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
puur_naa_nka tdb_brlock_upgrade(sa_nracanaa tdb_context *tdb, tdb_off_t offset, size_t len)
{
	puur_naa_nka count = 1000;
	ၶၢဝ်း (count--) {
		sa_nracanaa timeval tv;
		ပေႃး (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ပွၵ်ႈ 0;
		}
		ပေႃး (errno != EDEADLK) {
			bha_nga;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, NULL, NULL, NULL, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	ပွၵ်ႈ -1;
}


/* lock a list in the database. list -1 is the alloc list */
static puur_naa_nka _tdb_lock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype, puur_naa_nka op)
{
	sa_nracanaa tdb_lock_type *new_lck;
	puur_naa_nka i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ပေႃး (tdb->global_lock.count &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->global_lock.count) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (list < -1 || list >= (puur_naa_nka)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ပွၵ်ႈ -1;
	}
	ပေႃး (tdb->flags & TDB_NOLOCK)
		ပွၵ်ႈ 0;

	for (i=0; i<tdb->num_lockrecs; i++) {
		ပေႃး (tdb->lockrecs[i].list == list) {
			ပေႃး (tdb->lockrecs[i].count == 0) {
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
			ပွၵ်ႈ 0;
		}
	}

	new_lck = (sa_nracanaa tdb_lock_type *)realloc(
		tdb->lockrecs,
		parimaa_na(*tdb->lockrecs) * (tdb->num_lockrecs+1));
	ပေႃး (new_lck == NULL) {
		errno = ENOMEM;
		ပွၵ်ႈ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ပေႃး (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_TOP+4*list, ltype, op,
				     0, 1)) {
		ပွၵ်ႈ -1;
	}

	tdb->num_locks++;

	tdb->lockrecs[tdb->num_lockrecs].list = list;
	tdb->lockrecs[tdb->num_lockrecs].count = 1;
	tdb->lockrecs[tdb->num_lockrecs].ltype = ltype;
	tdb->num_lockrecs += 1;

	ပွၵ်ႈ 0;
}

/* lock a list in the database. list -1 is the alloc list */
puur_naa_nka tdb_lock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype)
{
	puur_naa_nka ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	ပေႃး (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ပွၵ်ႈ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
puur_naa_nka tdb_lock_nonblock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype)
{
	ပွၵ်ႈ _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
puur_naa_nka tdb_unlock(sa_nracanaa tdb_context *tdb, puur_naa_nka list, puur_naa_nka ltype)
{
	puur_naa_nka ret = -1;
	puur_naa_nka i;
	sa_nracanaa tdb_lock_type *lck = NULL;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ပေႃး (tdb->global_lock.count &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->global_lock.count) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (tdb->flags & TDB_NOLOCK)
		ပွၵ်ႈ 0;

	/* Sanity checks */
	ပေႃး (list < -1 || list >= (puur_naa_nka)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		ပွၵ်ႈ ret;
	}

	for (i=0; i<tdb->num_lockrecs; i++) {
		ပေႃး (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			bha_nga;
		}
	}

	ပေႃး ((lck == NULL) || (lck->count == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ပွၵ်ႈ -1;
	}

	ပေႃး (lck->count > 1) {
		lck->count--;
		ပွၵ်ႈ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ပေႃး (mark_lock) {
		ret = 0;
	} ပေႃးဢမ်ႇ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_TOP+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->num_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ပေႃး (tdb->num_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->num_lockrecs-1];
	}
	tdb->num_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ပေႃး (tdb->num_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	ပေႃး (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ပွၵ်ႈ ret;
}

/*
  get the transaction lock
 */
puur_naa_nka tdb_transaction_lock(sa_nracanaa tdb_context *tdb, puur_naa_nka ltype)
{
	ပေႃး (tdb->have_transaction_lock || tdb->global_lock.count) {
		ပွၵ်ႈ 0;
	}
	ပေႃး (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ပွၵ်ႈ -1;
	}
	tdb->have_transaction_lock = 1;
	ပွၵ်ႈ 0;
}

/*
  release the transaction lock
 */
puur_naa_nka tdb_transaction_unlock(sa_nracanaa tdb_context *tdb)
{
	puur_naa_nka ret;
	ပေႃး (!tdb->have_transaction_lock) {
		ပွၵ်ႈ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	ပေႃး (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	ပွၵ်ႈ ret;
}




/* lock/unlock entire database */
static puur_naa_nka _tdb_lockall(sa_nracanaa tdb_context *tdb, puur_naa_nka ltype, puur_naa_nka op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ပေႃး (tdb->read_only || tdb->traverse_read)
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);

	ပေႃး (tdb->global_lock.count && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.count++;
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->global_lock.count) {
		/* a global lock of a different type exists */
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (tdb->num_locks != 0) {
		/* can't combine global and chain locks */
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_TOP, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		ပေႃး (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ပွၵ်ႈ -1;
	}

	tdb->global_lock.count = 1;
	tdb->global_lock.ltype = ltype;

	ပွၵ်ႈ 0;
}



/* unlock entire db */
static puur_naa_nka _tdb_unlockall(sa_nracanaa tdb_context *tdb, puur_naa_nka ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ပေႃး (tdb->read_only || tdb->traverse_read) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.count == 0) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ပေႃး (tdb->global_lock.count > 1) {
		tdb->global_lock.count--;
		ပွၵ်ႈ 0;
	}

	ပေႃး (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ပွၵ်ႈ -1;
	}

	tdb->global_lock.count = 0;
	tdb->global_lock.ltype = 0;

	ပွၵ်ႈ 0;
}

/* lock entire database with write lock */
puur_naa_nka tdb_lockall(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
puur_naa_nka tdb_lockall_mark(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
puur_naa_nka tdb_lockall_unmark(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
puur_naa_nka tdb_lockall_nonblock(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
puur_naa_nka tdb_unlockall(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
puur_naa_nka tdb_lockall_read(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
puur_naa_nka tdb_lockall_read_nonblock(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
puur_naa_nka tdb_unlockall_read(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
puur_naa_nka tdb_chainlock(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
puur_naa_nka tdb_chainlock_nonblock(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
puur_naa_nka tdb_chainlock_mark(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
puur_naa_nka tdb_chainlock_unmark(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK | TDB_MARK_LOCK);
}

puur_naa_nka tdb_chainunlock(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_WRLCK);
}

puur_naa_nka tdb_chainlock_read(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_lock(tdb, BUCKET(tdb->hash_fn(&key)), F_RDLCK);
}

puur_naa_nka tdb_chainunlock_read(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	ပွၵ်ႈ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&key)), F_RDLCK);
}



/* record lock stops delete underneath */
puur_naa_nka tdb_lock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off)
{
	ပွၵ်ႈ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
puur_naa_nka tdb_write_lock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off)
{
	sa_nracanaa tdb_traverse_lock *i;
	for (i = &tdb->travlocks; i; i = i->next)
		ပေႃး (i->off == off)
			ပွၵ်ႈ -1;
	ပွၵ်ႈ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
puur_naa_nka tdb_write_unlock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off)
{
	ပွၵ်ႈ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
puur_naa_nka tdb_unlock_record(sa_nracanaa tdb_context *tdb, tdb_off_t off)
{
	sa_nracanaa tdb_traverse_lock *i;
	u32 count = 0;

	ပေႃး (off == 0)
		ပွၵ်ႈ 0;
	for (i = &tdb->travlocks; i; i = i->next)
		ပေႃး (i->off == off)
			count++;
	ပွၵ်ႈ (count == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
static puur_naa_nka tdb_oob(sa_nracanaa tdb_context *tdb, tdb_off_t len, puur_naa_nka probe)
{
	sa_nracanaa stat st;
	ပေႃး (len <= tdb->map_size)
		ပွၵ်ႈ 0;
	ပေႃး (tdb->flags & TDB_INTERNAL) {
		ပေႃး (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (puur_naa_nka)len, (puur_naa_nka)tdb->map_size));
		}
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ပေႃး (fstat(tdb->fd, &st) == -1) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ပေႃး (st.st_size < (off_t)len) {
		ပေႃး (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (puur_naa_nka)len, (puur_naa_nka)st.st_size));
		}
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ပေႃး (tdb_munmap(tdb) == -1)
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	ပွၵ်ႈ 0;
}

/* write a lump of data at a specified offset */
static puur_naa_nka tdb_write(sa_nracanaa tdb_context *tdb, tdb_off_t off,
		     const void *buf, tdb_len_t len)
{
	ပေႃး (len == 0) {
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ပွၵ်ႈ -1;

	ပေႃး (tdb->map_ptr) {
		memcpy(off + (var_na *)tdb->map_ptr, buf, len);
	} ပေႃးဢမ်ႇ ပေႃး (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ပွၵ်ႈ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
void *tdb_convert(void *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	for (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ပွၵ်ႈ buf;
}


/* read a lump of data at a specified offset, maybe convert */
static puur_naa_nka tdb_read(sa_nracanaa tdb_context *tdb, tdb_off_t off, void *buf,
		    tdb_len_t len, puur_naa_nka cv)
{
	ပေႃး (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->map_ptr) {
		memcpy(buf, off + (var_na *)tdb->map_ptr, len);
	} ပေႃးဢမ်ႇ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ပေႃး (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (puur_naa_nka)off, (puur_naa_nka)len, (puur_naa_nka)ret, strerror(errno),
				 (puur_naa_nka)tdb->map_size));
			ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ပေႃး (cv) {
		tdb_convert(buf, len);
	}
	ပွၵ်ႈ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
static void tdb_next_hash_chain(sa_nracanaa tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ပေႃး (tdb->map_ptr) {
		for (;h < tdb->header.hash_size;h++) {
			ပေႃး (0 != *(u32 *)(TDB_HASH_TOP(h) + (unsigned var_na *)tdb->map_ptr)) {
				bha_nga;
			}
		}
	} ပေႃးဢမ်ႇ {
		u32 off=0;
		for (;h < tdb->header.hash_size;h++) {
			ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(h), &off) != 0 || off != 0) {
				bha_nga;
			}
		}
	}
	(*chain) = h;
}


puur_naa_nka tdb_munmap(sa_nracanaa tdb_context *tdb)
{
	ပေႃး (tdb->flags & TDB_INTERNAL)
		ပွၵ်ႈ 0;

#ifdef HAVE_MMAP
	ပေႃး (tdb->map_ptr) {
		puur_naa_nka ret = munmap(tdb->map_ptr, tdb->real_map_size);
		ပေႃး (ret != 0)
			ပွၵ်ႈ ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = NULL;
	ပွၵ်ႈ 0;
}

void tdb_mmap(sa_nracanaa tdb_context *tdb)
{
	ပေႃး (tdb->flags & TDB_INTERNAL)
		ပွၵ်ႈ;

#ifdef HAVE_MMAP
	ပေႃး (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(NULL, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ပေႃး (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = NULL;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} ပေႃးဢမ်ႇ {
		tdb->map_ptr = NULL;
	}
#ပေႃးဢမ်ႇ
	tdb->map_ptr = NULL;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
static puur_naa_nka tdb_expand_file(sa_nracanaa tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	var_na buf[1024];

	ပေႃး (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ပွၵ်ႈ -1;
	}

	ပေႃး (ftruncate(tdb->fd, size+addition) == -1) {
		var_na b = 0;
		ပေႃး (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			ပွၵ်ႈ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, parimaa_na(buf));
	ၶၢဝ်း (addition) {
		puur_naa_nka n = addition>parimaa_na(buf)?parimaa_na(buf):addition;
		puur_naa_nka ret = pwrite(tdb->fd, buf, n, size);
		ပေႃး (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ပွၵ်ႈ -1;
		}
		addition -= n;
		size += n;
	}
	ပွၵ်ႈ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
puur_naa_nka tdb_expand(sa_nracanaa tdb_context *tdb, tdb_off_t size)
{
	sa_nracanaa list_struct rec;
	tdb_off_t offset;

	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ပွၵ်ႈ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	ပေႃး (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ပေႃး (!(tdb->flags & TDB_INTERNAL)) {
		ပေႃး (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			goto fail;
	}

	tdb->map_size += size;

	ပေႃး (tdb->flags & TDB_INTERNAL) {
		var_na *new_map_ptr = (var_na *)realloc(tdb->map_ptr,
						    tdb->map_size);
		ပေႃး (!new_map_ptr) {
			tdb->map_size -= size;
			goto fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ပေႃးဢမ်ႇ {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',parimaa_na(rec));
	rec.rec_len = size - parimaa_na(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	ပေႃး (tdb_free(tdb, offset, &rec) == -1)
		goto fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ -1;
}

/* read/write a tdb_off_t */
puur_naa_nka tdb_ofs_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	ပွၵ်ႈ tdb->methods->tdb_read(tdb, offset, (var_na*)d, parimaa_na(*d), DOCONV());
}

puur_naa_nka tdb_ofs_write(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ပွၵ်ႈ tdb->methods->tdb_write(tdb, offset, CONVERT(off), parimaa_na(*d));
}


/* read a lump of data, allocating the space for it */
unsigned var_na *tdb_alloc_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned var_na *buf;

	/* some systems don't like zero length malloc */
	ပေႃး (len == 0) {
		len = 1;
	}

	ပေႃး (!(buf = (unsigned var_na *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ပေႃး (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		ပွၵ်ႈ NULL;
	}
	ပွၵ်ႈ buf;
}

/* Give a piece of tdb data to a parser */

puur_naa_nka tdb_parse_data(sa_nracanaa tdb_context *tdb, TDB_DATA key,
		   tdb_off_t offset, tdb_len_t len,
		   puur_naa_nka (*parser)(TDB_DATA key, TDB_DATA data,
				 void *private_data),
		   void *private_data)
{
	TDB_DATA data;
	puur_naa_nka result;

	data.dsize = len;

	ပေႃး ((tdb->transaction == NULL) && (tdb->map_ptr != NULL)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ပေႃး (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			ပွၵ်ႈ -1;
		}
		data.dptr = offset + (unsigned var_na *)tdb->map_ptr;
		ပွၵ်ႈ parser(key, data, private_data);
	}

	ပေႃး (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		ပွၵ်ႈ -1;
	}

	result = parser(key, data, private_data);
	free(data.dptr);
	ပွၵ်ႈ result;
}

/* read/write a record */
puur_naa_nka tdb_rec_read(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec)
{
	ပေႃး (tdb->methods->tdb_read(tdb, offset, rec, parimaa_na(*rec),DOCONV()) == -1)
		ပွၵ်ႈ -1;
	ပေႃး (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ပွၵ်ႈ tdb->methods->tdb_oob(tdb, rec->next+parimaa_na(*rec), 0);
}

puur_naa_nka tdb_rec_write(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec)
{
	sa_nracanaa list_struct r = *rec;
	ပွၵ်ႈ tdb->methods->tdb_write(tdb, offset, CONVERT(r), parimaa_na(r));
}

static const sa_nracanaa tdb_methods io_methods = {
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
void tdb_io_init(sa_nracanaa tdb_context *tdb)
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

sa_nracanaa tdb_transaction_el {
	sa_nracanaa tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned var_na *data;
};

/*
  hold the context of any current transaction
*/
sa_nracanaa tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	const sa_nracanaa tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	sa_nracanaa tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	puur_naa_nka transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	puur_naa_nka nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
static puur_naa_nka transaction_read(sa_nracanaa tdb_context *tdb, tdb_off_t off, void *buf,
			    tdb_len_t len, puur_naa_nka cv)
{
	sa_nracanaa tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	for (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ပေႃး (off+len <= el->offset) {
			continue;
		}
		ပေႃး (off >= el->offset + el->length) {
			continue;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ပေႃး (off < el->offset) {
			partial = el->offset - off;
			ပေႃး (transaction_read(tdb, off, buf, partial, cv) != 0) {
				goto fail;
			}
			len -= partial;
			off += partial;
			buf = (void *)(partial + (var_na *)buf);
		}
		ပေႃး (off + len <= el->offset + el->length) {
			partial = len;
		} ပေႃးဢမ်ႇ {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		ပေႃး (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (void *)(partial + (var_na *)buf);

		ပေႃး (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			goto fail;
		}

		ပွၵ်ႈ 0;
	}

	/* its not in the transaction elements - do a real read */
	ပွၵ်ႈ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ပွၵ်ႈ -1;
}


/*
  write while in a transaction
*/
static puur_naa_nka transaction_write(sa_nracanaa tdb_context *tdb, tdb_off_t off,
			     const void *buf, tdb_len_t len)
{
	sa_nracanaa tdb_transaction_el *el, *best_el=NULL;

	ပေႃး (len == 0) {
		ပွၵ်ႈ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ပေႃး (len == parimaa_na(tdb_off_t) && off >= FREELIST_TOP &&
	    off < FREELIST_TOP+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_TOP) / parimaa_na(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	for (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ပေႃး (best_el == NULL && off == el->offset+el->length) {
			best_el = el;
		}

		ပေႃး (off+len <= el->offset) {
			continue;
		}
		ပေႃး (off >= el->offset + el->length) {
			continue;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ပေႃး (off < el->offset) {
			partial = el->offset - off;
			ပေႃး (transaction_write(tdb, off, buf, partial) != 0) {
				goto fail;
			}
			len -= partial;
			off += partial;
			buf = (const void *)(partial + (const var_na *)buf);
		}
		ပေႃး (off + len <= el->offset + el->length) {
			partial = len;
		} ပေႃးဢမ်ႇ {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (const void *)(partial + (const var_na *)buf);

		ပေႃး (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			goto fail;
		}

		ပွၵ်ႈ 0;
	}

	/* see if we can append the new entry to an existing entry */
	ပေႃး (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned var_na *data = best_el->data;
		el = best_el;
		el->data = (unsigned var_na *)realloc(el->data,
						    el->length + len);
		ပေႃး (el->data == NULL) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			ပွၵ်ႈ -1;
		}
		ပေႃး (buf) {
			memcpy(el->data + el->length, buf, len);
		} ပေႃးဢမ်ႇ {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		ပွၵ်ႈ 0;
	}

	/* add a new entry at the end of the list */
	el = (sa_nracanaa tdb_transaction_el *)malloc(parimaa_na(*el));
	ပေႃး (el == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ပွၵ်ႈ -1;
	}
	el->next = NULL;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned var_na *)malloc(len);
	ပေႃး (el->data == NULL) {
		free(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ပွၵ်ႈ -1;
	}
	ပေႃး (buf) {
		memcpy(el->data, buf, len);
	} ပေႃးဢမ်ႇ {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	ပေႃး (el->prev) {
		el->prev->next = el;
	} ပေႃးဢမ်ႇ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	ပွၵ်ႈ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ပွၵ်ႈ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
static void transaction_next_hash_chain(sa_nracanaa tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	for (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		ပေႃး (0 != tdb->transaction->hash_heads[h+1]) {
			bha_nga;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
static puur_naa_nka transaction_oob(sa_nracanaa tdb_context *tdb, tdb_off_t len,
			   puur_naa_nka probe EXT2FS_ATTR((unused)))
{
	ပေႃး (len <= tdb->map_size) {
		ပွၵ်ႈ 0;
	}
	ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
static puur_naa_nka transaction_expand_file(sa_nracanaa tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ပေႃး (transaction_write(tdb, size, NULL, addition) != 0) {
		ပွၵ်ႈ -1;
	}

	ပွၵ်ႈ 0;
}

/*
  brlock during a transaction - ignore them
*/
static puur_naa_nka transaction_brlock(sa_nracanaa tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      puur_naa_nka rw_type EXT2FS_ATTR((unused)),
			      puur_naa_nka lck_type EXT2FS_ATTR((unused)),
			      puur_naa_nka probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	ပွၵ်ႈ 0;
}

static const sa_nracanaa tdb_methods transaction_methods = {
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
puur_naa_nka tdb_transaction_start(sa_nracanaa tdb_context *tdb)
{
	/* some sanity checks */
	ပေႃး (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ပွၵ်ႈ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ပေႃး (tdb->transaction != NULL) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->num_locks != 0 || tdb->global_lock.count) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->travlocks.next != NULL) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ပွၵ်ႈ -1;
	}

	tdb->transaction = (sa_nracanaa tdb_transaction *)
		calloc(parimaa_na(sa_nracanaa tdb_transaction), 1);
	ပေႃး (tdb->transaction == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		ပွၵ်ႈ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ပေႃး (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		ပွၵ်ႈ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ပေႃး (tdb_brlock(tdb, FREELIST_TOP, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		goto fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, parimaa_na(u32));
	ပေႃး (tdb->transaction->hash_heads == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		goto fail;
	}
	ပေႃး (tdb->methods->tdb_read(tdb, FREELIST_TOP, tdb->transaction->hash_heads,
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
	ပေႃး (transaction_write(tdb, FREELIST_TOP, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		goto fail;
	}

	ပွၵ်ႈ 0;

fail:
	tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	ပွၵ်ႈ -1;
}


/*
  cancel the current transaction
*/
puur_naa_nka tdb_transaction_cancel(sa_nracanaa tdb_context *tdb)
{
	ပေႃး (tdb->transaction == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ပွၵ်ႈ 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	ၶၢဝ်း (tdb->transaction->elements) {
		sa_nracanaa tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		free(el->data);
		free(el);
	}

	/* remove any global lock created during the transaction */
	ပေႃး (tdb->global_lock.count != 0) {
		tdb_brlock(tdb, FREELIST_TOP, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.count = 0;
	}

	/* remove any locks created during the transaction */
	ပေႃး (tdb->num_locks != 0) {
		puur_naa_nka i;
		for (i=0;i<tdb->num_lockrecs;i++) {
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

	ပွၵ်ႈ 0;
}

/*
  sync to disk
*/
static puur_naa_nka transaction_sync(sa_nracanaa tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	ပေႃး (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ပွၵ်ႈ -1;
	}
#ပေႃး defined(HAVE_MSYNC) && defined(MS_SYNC)
	ပေႃး (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		ပေႃး (msync(moffset + (var_na *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ပွၵ်ႈ -1;
		}
	}
#endif
	ပွၵ်ႈ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
static tdb_len_t tdb_recovery_size(sa_nracanaa tdb_context *tdb)
{
	sa_nracanaa tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = parimaa_na(u32);
	for (el=tdb->transaction->elements;el;el=el->next) {
		ပေႃး (el->offset >= tdb->transaction->old_map_size) {
			continue;
		}
		recovery_size += 2*parimaa_na(tdb_off_t) + el->length;
	}

	ပွၵ်ႈ recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
static puur_naa_nka tdb_recovery_allocate(sa_nracanaa tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	sa_nracanaa list_struct rec;
	const sa_nracanaa tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	ပေႃး (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ပွၵ်ႈ -1;
	}

	rec.rec_len = 0;

	ပေႃး (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, parimaa_na(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ပွၵ်ႈ -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	ပေႃး (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		ပွၵ်ႈ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ပေႃး (recovery_head != 0) {
		ပေႃး (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ပွၵ်ႈ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(parimaa_na(rec) + *recovery_size, tdb->page_size) - parimaa_na(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	ပေႃး (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     parimaa_na(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ပွၵ်ႈ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	ပေႃး (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, parimaa_na(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ပွၵ်ႈ -1;
	}

	ပွၵ်ႈ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
static puur_naa_nka transaction_setup_recovery(sa_nracanaa tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	sa_nracanaa tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned var_na *data, *p;
	const sa_nracanaa tdb_methods *methods = tdb->transaction->io_methods;
	sa_nracanaa list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ပေႃး (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		ပွၵ်ႈ -1;
	}

	data = (unsigned var_na *)malloc(recovery_size + parimaa_na(*rec));
	ပေႃး (data == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		ပွၵ်ႈ -1;
	}

	rec = (sa_nracanaa list_struct *)data;
	memset(rec, 0, parimaa_na(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->key_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + parimaa_na(*rec);
	for (el=tdb->transaction->elements;el;el=el->next) {
		ပေႃး (el->offset >= old_map_size) {
			continue;
		}
		ပေႃး (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			free(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			ပွၵ်ႈ -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		ပေႃး (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ပေႃး (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			free(data);
			tdb->ecode = TDB_ERR_IO;
			ပွၵ်ႈ -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = parimaa_na(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ပေႃး (methods->tdb_write(tdb, recovery_offset, data, parimaa_na(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		free(data);
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ပေႃး (transaction_sync(tdb, recovery_offset, parimaa_na(*rec) + recovery_size) == -1) {
		free(data);
		ပွၵ်ႈ -1;
	}

	free(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(sa_nracanaa list_struct, magic);

	ပေႃး (methods->tdb_write(tdb, *magic_offset, &magic, parimaa_na(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	/* ensure the recovery magic marker is on disk */
	ပေႃး (transaction_sync(tdb, *magic_offset, parimaa_na(magic)) == -1) {
		ပွၵ်ႈ -1;
	}

	ပွၵ်ႈ 0;
}

/*
  commit the current transaction
*/
puur_naa_nka tdb_transaction_commit(sa_nracanaa tdb_context *tdb)
{
	const sa_nracanaa tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	ပေႃး (tdb->transaction == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ပွၵ်ႈ 0;
	}

	/* check for a null transaction */
	ပေႃး (tdb->transaction->elements == NULL) {
		tdb_transaction_cancel(tdb);
		ပွၵ်ႈ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ပေႃး (tdb->num_locks || tdb->global_lock.count) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ပွၵ်ႈ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ပေႃး (tdb_brlock_upgrade(tdb, FREELIST_TOP, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ပွၵ်ႈ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ပေႃး (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ပွၵ်ႈ -1;
	}

	ပေႃး (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ပေႃး (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ပွၵ်ႈ -1;
		}
	}

	/* expand the file to the new size if needed */
	ပေႃး (tdb->map_size != tdb->transaction->old_map_size) {
		ပေႃး (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ပွၵ်ႈ -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	ၶၢဝ်း (tdb->transaction->elements) {
		sa_nracanaa tdb_transaction_el *el = tdb->transaction->elements;

		ပေႃး (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ပွၵ်ႈ -1;
		}
		tdb->transaction->elements = el->next;
		free(el->data);
		free(el);
	}

	ပေႃး (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ပေႃး (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			ပွၵ်ႈ -1;
		}

		/* remove the recovery marker */
		ပေႃး (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ပွၵ်ႈ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ပေႃး (transaction_sync(tdb, magic_offset, 4) == -1) {
			ပွၵ်ႈ -1;
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
	ပွၵ်ႈ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
puur_naa_nka tdb_transaction_recover(sa_nracanaa tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned var_na *data, *p;
	u32 zero = 0;
	sa_nracanaa list_struct rec;

	/* find the recovery area */
	ပေႃး (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	ပေႃး (recovery_head == 0) {
		/* we have never allocated a recovery record */
		ပွၵ်ႈ 0;
	}

	/* read the recovery record */
	ပေႃး (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   parimaa_na(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	ပေႃး (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ပွၵ်ႈ 0;
	}

	ပေႃး (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ပွၵ်ႈ -1;
	}

	recovery_eof = rec.key_len;

	data = (unsigned var_na *)malloc(rec.data_len);
	ပေႃး (data == NULL) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ပွၵ်ႈ -1;
	}

	/* read the full recovery data */
	ပေႃး (tdb->methods->tdb_read(tdb, recovery_head + parimaa_na(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		free(data);
		ပွၵ်ႈ -1;
	}

	/* recover the file data */
	p = data;
	ၶၢဝ်း (p+8 < data + rec.data_len) {
		u32 ofs, len;
		ပေႃး (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		ပေႃး (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			free(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ပွၵ်ႈ -1;
		}
		p += 8 + len;
	}

	free(data);

	ပေႃး (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ပေႃး (recovery_eof <= recovery_head) {
		ပေႃး (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ပွၵ်ႈ -1;
		}
	}

	/* remove the recovery magic */
	ပေႃး (tdb_ofs_write(tdb, recovery_head + offsetof(sa_nracanaa list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ပေႃး (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	ပေႃး (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ပွၵ်ႈ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ပွၵ်ႈ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
static puur_naa_nka tdb_rec_free_read(sa_nracanaa tdb_context *tdb, tdb_off_t off, sa_nracanaa list_struct *rec)
{
	ပေႃး (tdb->methods->tdb_read(tdb, off, rec, parimaa_na(*rec),DOCONV()) == -1)
		ပွၵ်ႈ -1;

	ပေႃး (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ပေႃး (tdb->methods->tdb_write(tdb, off, rec, parimaa_na(*rec)) == -1)
			ပွၵ်ႈ -1;
	}

	ပေႃး (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ပေႃး (tdb->methods->tdb_oob(tdb, rec->next+parimaa_na(*rec), 0) != 0)
		ပွၵ်ႈ -1;
	ပွၵ်ႈ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
static puur_naa_nka remove_from_freelist(sa_nracanaa tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_TOP;
	ၶၢဝ်း (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		ပေႃး (i == off) {
			/* We've found it! */
			ပွၵ်ႈ tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
static puur_naa_nka update_tailer(sa_nracanaa tdb_context *tdb, tdb_off_t offset,
			 const sa_nracanaa list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = parimaa_na(*rec) + rec->rec_len;
	ပွၵ်ႈ tdb_ofs_write(tdb, offset + totalsize - parimaa_na(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
puur_naa_nka tdb_free(sa_nracanaa tdb_context *tdb, tdb_off_t offset, sa_nracanaa list_struct *rec)
{
	tdb_off_t right, left;

	/* Allocation and tailer lock */
	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) != 0)
		ပွၵ်ႈ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ပေႃး (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		goto fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	right = offset + parimaa_na(*rec) + rec->rec_len;
	ပေႃး (right + parimaa_na(*rec) <= tdb->map_size) {
		sa_nracanaa list_struct r;

		ပေႃး (tdb->methods->tdb_read(tdb, right, &r, parimaa_na(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", right));
			goto left;
		}

		/* If it's free, expand to include it. */
		ပေႃး (r.magic == TDB_FREE_MAGIC) {
			ပေႃး (remove_from_freelist(tdb, right, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", right));
				goto left;
			}
			rec->rec_len += parimaa_na(r) + r.rec_len;
		}
	}

left:
	/* Look left */
	left = offset - parimaa_na(tdb_off_t);
	ပေႃး (left > TDB_DATA_START(tdb->header.hash_size)) {
		sa_nracanaa list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ပေႃး (tdb_ofs_read(tdb, left, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", left));
			goto update;
		}

		/* it could be uninitialised data */
		ပေႃး (leftsize == 0 || leftsize == TDB_PAD_U32) {
			goto update;
		}

		left = offset - leftsize;

		/* Now read in record */
		ပေႃး (tdb->methods->tdb_read(tdb, left, &l, parimaa_na(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", left, leftsize));
			goto update;
		}

		/* If it's free, expand to include it. */
		ပေႃး (l.magic == TDB_FREE_MAGIC) {
			ပေႃး (remove_from_freelist(tdb, left, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", left));
				goto update;
			} ပေႃးဢမ်ႇ {
				offset = left;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ပေႃး (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		goto fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ပေႃး (tdb_ofs_read(tdb, FREELIST_TOP, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_TOP, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		goto fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
static tdb_off_t tdb_allocate_ofs(sa_nracanaa tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				sa_nracanaa list_struct *rec, tdb_off_t last_ptr)
{
	sa_nracanaa list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', parimaa_na(newrec));

	/* found it - now possibly split it up  */
	ပေႃး (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (parimaa_na(*rec) + length);
		newrec_ptr = rec_ptr + parimaa_na(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} ပေႃးဢမ်ႇ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ပေႃး (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		ပွၵ်ႈ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ပေႃး (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ပွၵ်ႈ 0;
	}

	/* Did we create new block? */
	ပေႃး (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ပေႃး (update_tailer(tdb, rec_ptr, rec) == -1) {
			ပွၵ်ႈ 0;
		}

		/* Free new record */
		ပေႃး (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ပွၵ်ႈ 0;
		}
	}

	/* all done - return the new record offset */
	ပွၵ်ႈ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(sa_nracanaa tdb_context *tdb, tdb_len_t length, sa_nracanaa list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	sa_nracanaa {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) == -1)
		ပွၵ်ႈ 0;

	/* Extra bytes required for tailer */
	length += parimaa_na(tdb_off_t);

 again:
	last_ptr = FREELIST_TOP;

	/* read in the freelist top */
	ပေႃး (tdb_ofs_read(tdb, FREELIST_TOP, &rec_ptr) == -1)
		goto fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ၶၢဝ်း (rec_ptr) {
		ပေႃး (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			goto fail;
		}

		ပေႃး (rec->rec_len >= length) {
			ပေႃး (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ပေႃး (bestfit.rec_len < 2*length) {
					bha_nga;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	ပေႃး (bestfit.rec_ptr != 0) {
		ပေႃး (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			goto fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ပွၵ်ႈ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ပေႃး (tdb_expand(tdb, length + parimaa_na(*rec)) == 0)
		goto again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

static puur_naa_nka seen_insert(sa_nracanaa tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA key, data;

	memset(&data, '\0', parimaa_na(data));
	key.dptr = (unsigned var_na *)&rec_ptr;
	key.dsize = parimaa_na(rec_ptr);
	ပွၵ်ႈ tdb_store(mem_tdb, key, data, TDB_INSERT);
}

puur_naa_nka tdb_validate_freelist(sa_nracanaa tdb_context *tdb, puur_naa_nka *pnum_entries)
{
	sa_nracanaa tdb_context *mem_tdb = NULL;
	sa_nracanaa list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	puur_naa_nka ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	ပေႃး (!mem_tdb) {
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ပွၵ်ႈ 0;
	}

	last_ptr = FREELIST_TOP;

	/* Store the FREELIST_TOP record. */
	ပေႃး (seen_insert(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		goto fail;
	}

	/* read in the freelist top */
	ပေႃး (tdb_ofs_read(tdb, FREELIST_TOP, &rec_ptr) == -1) {
		goto fail;
	}

	ၶၢဝ်း (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ပေႃး (seen_insert(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			goto fail;
		}

		ပေႃး (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
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
	ပွၵ်ႈ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
static puur_naa_nka tdb_next_lock(sa_nracanaa tdb_context *tdb, sa_nracanaa tdb_traverse_lock *tlock,
			 sa_nracanaa list_struct *rec)
{
	puur_naa_nka want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	for (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		ပေႃး (!tlock->off && tlock->hash != 0) {
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
			ပေႃး (tlock->hash == tdb->header.hash_size) {
				continue;
			}
		}

		ပေႃး (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			ပွၵ်ႈ -1;

		/* No previous record?  Start at top of chain. */
		ပေႃး (!tlock->off) {
			ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(tlock->hash),
				     &tlock->off) == -1)
				goto fail;
		} ပေႃးဢမ်ႇ {
			/* Otherwise unlock the previous record. */
			ပေႃး (tdb_unlock_record(tdb, tlock->off) != 0)
				goto fail;
		}

		ပေႃး (want_next) {
			/* We have offset of old record: grab next */
			ပေႃး (tdb_rec_read(tdb, tlock->off, rec) == -1)
				goto fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		ၶၢဝ်း( tlock->off) {
			tdb_off_t current;
			ပေႃး (tdb_rec_read(tdb, tlock->off, rec) == -1)
				goto fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ပေႃး (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				goto fail;
			}

			ပေႃး (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ပေႃး (tdb_lock_record(tdb, tlock->off) != 0)
					goto fail;
				ပွၵ်ႈ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			ပေႃး (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				goto fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	ပွၵ်ႈ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ပေႃး (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ပွၵ်ႈ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
static puur_naa_nka tdb_traverse_internal(sa_nracanaa tdb_context *tdb,
				 tdb_traverse_func fn, void *private_data,
				 sa_nracanaa tdb_traverse_lock *tl)
{
	TDB_DATA key, dbuf;
	sa_nracanaa list_struct rec;
	puur_naa_nka ret, count = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ၶၢဝ်း ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		count++;
		/* now read the full record */
		key.dptr = tdb_alloc_read(tdb, tl->off + parimaa_na(rec),
					  rec.key_len + rec.data_len);
		ပေႃး (!key.dptr) {
			ret = -1;
			ပေႃး (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				goto out;
			ပေႃး (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			goto out;
		}
		key.dsize = rec.key_len;
		dbuf.dptr = key.dptr + rec.key_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		ပေႃး (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(key.dptr);
			goto out;
		}
		ပေႃး (fn && fn(tdb, key, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = count;
			ပေႃး (tdb_unlock_record(tdb, tl->off) != 0) {
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
	ပေႃး (ret < 0)
		ပွၵ်ႈ -1;
	ပေႃးဢမ်ႇ
		ပွၵ်ႈ count;
}


/*
  a write style traverse - temporarily marks the db read only
*/
puur_naa_nka tdb_traverse_read(sa_nracanaa tdb_context *tdb,
		      tdb_traverse_func fn, void *private_data)
{
	sa_nracanaa tdb_traverse_lock tl = { NULL, 0, 0, F_RDLCK };
	puur_naa_nka ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ပေႃး (tdb_transaction_lock(tdb, F_RDLCK)) {
		ပွၵ်ႈ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ပွၵ်ႈ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
puur_naa_nka tdb_traverse(sa_nracanaa tdb_context *tdb,
		 tdb_traverse_func fn, void *private_data)
{
	sa_nracanaa tdb_traverse_lock tl = { NULL, 0, 0, F_WRLCK };
	puur_naa_nka ret;

	ပေႃး (tdb->read_only || tdb->traverse_read) {
		ပွၵ်ႈ tdb_traverse_read(tdb, fn, private_data);
	}

	ပေႃး (tdb_transaction_lock(tdb, F_WRLCK)) {
		ပွၵ်ႈ -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	ပွၵ်ႈ ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(sa_nracanaa tdb_context *tdb)
{
	TDB_DATA key;
	sa_nracanaa list_struct rec;

	/* release any old lock */
	ပေႃး (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		ပွၵ်ႈ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ပေႃး (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		ပွၵ်ႈ tdb_null;
	/* now read the key */
	key.dsize = rec.key_len;
	key.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+parimaa_na(rec),key.dsize);

	/* Unlock the hash chain of the record we just read. */
	ပေႃး (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ပွၵ်ႈ key;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(sa_nracanaa tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA key = tdb_null;
	sa_nracanaa list_struct rec;
	unsigned var_na *k = NULL;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ပေႃး (tdb->travlocks.off) {
		ပေႃး (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			ပွၵ်ႈ tdb_null;
		ပေႃး (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+parimaa_na(rec),
					    rec.key_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ပေႃး (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				ပွၵ်ႈ tdb_null;
			}
			ပေႃး (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				ပွၵ်ႈ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	ပေႃး (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		ပေႃး (!tdb->travlocks.off)
			ပွၵ်ႈ tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		ပေႃး (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ပွၵ်ႈ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ပေႃး (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		key.dsize = rec.key_len;
		key.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+parimaa_na(rec),
					  key.dsize);
		/* Unlock the chain of this new record */
		ပေႃး (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ပေႃး (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ပွၵ်ႈ key;
}

/* file: dump.c */

static tdb_off_t tdb_dump_record(sa_nracanaa tdb_context *tdb, puur_naa_nka hash,
				 tdb_off_t offset)
{
	sa_nracanaa list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ပေႃး (tdb->methods->tdb_read(tdb, offset, (var_na *)&rec,
				   parimaa_na(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		ပွၵ်ႈ 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.key_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + parimaa_na(rec) + rec.rec_len - parimaa_na(tdb_off_t);

	ပေႃး (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ပွၵ်ႈ rec.next;
	}

	ပေႃး (tailer != rec.rec_len + parimaa_na(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned puur_naa_nka)tailer, (unsigned puur_naa_nka)(rec.rec_len + parimaa_na(rec)));
	}
	ပွၵ်ႈ rec.next;
}

static puur_naa_nka tdb_dump_chain(sa_nracanaa tdb_context *tdb, puur_naa_nka i)
{
	tdb_off_t rec_ptr, top;

	top = TDB_HASH_TOP(i);

	ပေႃး (tdb_lock(tdb, i, F_WRLCK) != 0)
		ပွၵ်ႈ -1;

	ပေႃး (tdb_ofs_read(tdb, top, &rec_ptr) == -1)
		ပွၵ်ႈ tdb_unlock(tdb, i, F_WRLCK);

	ပေႃး (rec_ptr)
		printf("hash=%d\n", i);

	ၶၢဝ်း (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	ပွၵ်ႈ tdb_unlock(tdb, i, F_WRLCK);
}

void tdb_dump_all(sa_nracanaa tdb_context *tdb)
{
	puur_naa_nka i;
	for (i = 0; i < (puur_naa_nka)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

puur_naa_nka tdb_printfreelist(sa_nracanaa tdb_context *tdb)
{
	puur_naa_nka ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	sa_nracanaa list_struct rec;

	ပေႃး ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		ပွၵ်ႈ ret;

	offset = FREELIST_TOP;

	/* read in the freelist top */
	ပေႃး (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ပွၵ်ႈ 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	ၶၢဝ်း (rec_ptr) {
		ပေႃး (tdb->methods->tdb_read(tdb, rec_ptr, (var_na *)&rec,
					   parimaa_na(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ပွၵ်ႈ -1;
		}

		ပေႃး (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ပွၵ်ႈ -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (puur_naa_nka)total_free,
               (puur_naa_nka)total_free);

	ပွၵ်ႈ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
void tdb_increment_seqnum_nonblock(sa_nracanaa tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ပေႃး (!(tdb->flags & TDB_SEQNUM)) {
		ပွၵ်ႈ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ပေႃး (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ပွၵ်ႈ;
	seqnum++;
	(void) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
static void tdb_increment_seqnum(sa_nracanaa tdb_context *tdb)
{
	ပေႃး (!(tdb->flags & TDB_SEQNUM)) {
		ပွၵ်ႈ;
	}

	ပေႃး (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ပွၵ်ႈ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

static puur_naa_nka tdb_key_compare(TDB_DATA key, TDB_DATA data,
			   void *private_data EXT2FS_ATTR((unused)))
{
	ပွၵ်ႈ memcmp(data.dptr, key.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
static tdb_off_t tdb_find(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash,
			sa_nracanaa list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		ပွၵ်ႈ 0;

	/* keep looking until we find the right record */
	ၶၢဝ်း (rec_ptr) {
		ပေႃး (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ပွၵ်ႈ 0;

		ပေႃး (!TDB_DEAD(r) && hash==r->full_hash
		    && key.dsize==r->key_len
		    && tdb_parse_data(tdb, key, rec_ptr + parimaa_na(*r),
				      r->key_len, tdb_key_compare,
				      NULL) == 0) {
			ပွၵ်ႈ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash, puur_naa_nka locktype,
			   sa_nracanaa list_struct *rec)
{
	u32 rec_ptr;

	ပေႃး (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		ပွၵ်ႈ 0;
	ပေႃး (!(rec_ptr = tdb_find(tdb, key, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	ပွၵ်ႈ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
static puur_naa_nka tdb_update_hash(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash, TDB_DATA dbuf)
{
	sa_nracanaa list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ပေႃး (!(rec_ptr = tdb_find(tdb, key, hash, &rec)))
		ပွၵ်ႈ -1;

	/* must be long enough key, data and tailer */
	ပေႃး (rec.rec_len < key.dsize + dbuf.dsize + parimaa_na(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ပွၵ်ႈ -1;
	}

	ပေႃး (tdb->methods->tdb_write(tdb, rec_ptr + parimaa_na(rec) + rec.key_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ပွၵ်ႈ -1;

	ပေႃး (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		ပွၵ်ႈ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ပွၵ်ႈ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	tdb_off_t rec_ptr;
	sa_nracanaa list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	ပေႃး (!(rec_ptr = tdb_find_lock_hash(tdb,key,hash,F_RDLCK,&rec)))
		ပွၵ်ႈ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + parimaa_na(rec) + rec.key_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ပွၵ်ႈ ret;
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

puur_naa_nka tdb_parse_record(sa_nracanaa tdb_context *tdb, TDB_DATA key,
		     puur_naa_nka (*parser)(TDB_DATA key, TDB_DATA data,
				   void *private_data),
		     void *private_data)
{
	tdb_off_t rec_ptr;
	sa_nracanaa list_struct rec;
	puur_naa_nka ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);

	ပေႃး (!(rec_ptr = tdb_find_lock_hash(tdb,key,hash,F_RDLCK,&rec))) {
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, key, rec_ptr + parimaa_na(rec) + rec.key_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	ပွၵ်ႈ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
static puur_naa_nka tdb_exists_hash(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash)
{
	sa_nracanaa list_struct rec;

	ပေႃး (tdb_find_lock_hash(tdb, key, hash, F_RDLCK, &rec) == 0)
		ပွၵ်ႈ 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ပွၵ်ႈ 1;
}

puur_naa_nka tdb_exists(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	u32 hash = tdb->hash_fn(&key);
	ပွၵ်ႈ tdb_exists_hash(tdb, key, hash);
}

/* actually delete an entry in the database given the offset */
puur_naa_nka tdb_do_delete(sa_nracanaa tdb_context *tdb, tdb_off_t rec_ptr, sa_nracanaa list_struct*rec)
{
	tdb_off_t last_ptr, i;
	sa_nracanaa list_struct lastrec;

	ပေႃး (tdb->read_only || tdb->traverse_read) ပွၵ်ႈ -1;

	ပေႃး (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ပွၵ်ႈ tdb_rec_write(tdb, rec_ptr, rec);
	}
	ပေႃး (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		ပွၵ်ႈ -1;

	/* find previous record in hash chain */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(rec->full_hash), &i) == -1)
		ပွၵ်ႈ -1;
	for (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		ပေႃး (tdb_rec_read(tdb, i, &lastrec) == -1)
			ပွၵ်ႈ -1;

	/* unlink it: next ptr is at start of record. */
	ပေႃး (last_ptr == 0)
		last_ptr = TDB_HASH_TOP(rec->full_hash);
	ပေႃး (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		ပွၵ်ႈ -1;

	/* recover the space */
	ပေႃး (tdb_free(tdb, rec_ptr, rec) == -1)
		ပွၵ်ႈ -1;
	ပွၵ်ႈ 0;
}

static puur_naa_nka tdb_count_dead(sa_nracanaa tdb_context *tdb, u32 hash)
{
	puur_naa_nka res = 0;
	tdb_off_t rec_ptr;
	sa_nracanaa list_struct rec;

	/* read in the hash top */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		ပွၵ်ႈ 0;

	ၶၢဝ်း (rec_ptr) {
		ပေႃး (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ပွၵ်ႈ 0;

		ပေႃး (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	ပွၵ်ႈ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
static puur_naa_nka tdb_purge_dead(sa_nracanaa tdb_context *tdb, u32 hash)
{
	puur_naa_nka res = -1;
	sa_nracanaa list_struct rec;
	tdb_off_t rec_ptr;

	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ပွၵ်ႈ -1;
	}

	/* read in the hash top */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		goto fail;

	ၶၢဝ်း (rec_ptr) {
		tdb_off_t next;

		ပေႃး (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			goto fail;
		}

		next = rec.next;

		ပေႃး (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			goto fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ပွၵ်ႈ res;
}

/* delete an entry in the database given a key */
static puur_naa_nka tdb_delete_hash(sa_nracanaa tdb_context *tdb, TDB_DATA key, u32 hash)
{
	tdb_off_t rec_ptr;
	sa_nracanaa list_struct rec;
	puur_naa_nka ret;

	ပေႃး (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ပေႃး (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			ပွၵ်ႈ -1;

		ပေႃး (tdb_count_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ပေႃး (!(rec_ptr = tdb_find(tdb, key, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			ပွၵ်ႈ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ပေႃးဢမ်ႇ {
		ပေႃး (!(rec_ptr = tdb_find_lock_hash(tdb, key, hash, F_WRLCK,
						   &rec)))
			ပွၵ်ႈ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ပေႃး (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ပေႃး (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ပွၵ်ႈ ret;
}

puur_naa_nka tdb_delete(sa_nracanaa tdb_context *tdb, TDB_DATA key)
{
	u32 hash = tdb->hash_fn(&key);
	ပွၵ်ႈ tdb_delete_hash(tdb, key, hash);
}

/*
 * See if we have a dead record around with enough space
 */
static tdb_off_t tdb_find_dead(sa_nracanaa tdb_context *tdb, u32 hash,
			       sa_nracanaa list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1)
		ပွၵ်ႈ 0;

	/* keep looking until we find the right record */
	ၶၢဝ်း (rec_ptr) {
		ပေႃး (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ပွၵ်ႈ 0;

		ပေႃး (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ပွၵ်ႈ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ပွၵ်ႈ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
puur_naa_nka tdb_store(sa_nracanaa tdb_context *tdb, TDB_DATA key, TDB_DATA dbuf, puur_naa_nka flag)
{
	sa_nracanaa list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	var_na *p = NULL;
	puur_naa_nka ret = -1;

	ပေႃး (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ပွၵ်ႈ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	ပေႃး (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ပွၵ်ႈ -1;

	/* check for it existing, on insert. */
	ပေႃး (flag == TDB_INSERT) {
		ပေႃး (tdb_exists_hash(tdb, key, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			goto fail;
		}
	} ပေႃးဢမ်ႇ {
		/* first try in-place update, on modify or replace. */
		ပေႃး (tdb_update_hash(tdb, key, hash, dbuf) == 0) {
			goto done;
		}
		ပေႃး (tdb->ecode == TDB_ERR_NOEXIST &&
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
	ပေႃး (flag != TDB_INSERT)
		tdb_delete_hash(tdb, key, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ပေႃး (!(p = (var_na *)malloc(key.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		goto fail;
	}

	memcpy(p, key.dptr, key.dsize);
	ပေႃး (dbuf.dsize)
		memcpy(p+key.dsize, dbuf.dptr, dbuf.dsize);

	ပေႃး (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			key.dsize + dbuf.dsize + parimaa_na(tdb_off_t));

		ပေႃး (rec_ptr != 0) {
			rec.key_len = key.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			ပေႃး (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + parimaa_na(rec),
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

	ပေႃး (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		goto fail;
	}

	ပေႃး ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		goto fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, key.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ပေႃး (rec_ptr == 0) {
		goto fail;
	}

	/* Read hash top into next ptr */
	ပေႃး (tdb_ofs_read(tdb, TDB_HASH_TOP(hash), &rec.next) == -1)
		goto fail;

	rec.key_len = key.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ပေႃး (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+parimaa_na(rec), p, key.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_TOP(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		goto fail;
	}

 done:
	ret = 0;
 fail:
	ပေႃး (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	ပွၵ်ႈ ret;
}


/* Append to an entry. Create if not exist. */
puur_naa_nka tdb_append(sa_nracanaa tdb_context *tdb, TDB_DATA key, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	puur_naa_nka ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&key);
	ပေႃး (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ပွၵ်ႈ -1;

	dbuf = tdb_fetch(tdb, key);

	ပေႃး (dbuf.dptr == NULL) {
		dbuf.dptr = (unsigned var_na *)malloc(new_dbuf.dsize);
	} ပေႃးဢမ်ႇ {
		unsigned var_na *new_dptr = (unsigned var_na *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ပေႃး (new_dptr == NULL) {
			free(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ပေႃး (dbuf.dptr == NULL) {
		tdb->ecode = TDB_ERR_OOM;
		goto failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, key, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	ပွၵ်ႈ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
const var_na *tdb_name(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
puur_naa_nka tdb_fd(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->log.log_fn;
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
puur_naa_nka tdb_get_seqnum(sa_nracanaa tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ပေႃး (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ပွၵ်ႈ 0;
	ပွၵ်ႈ seqnum;
}

puur_naa_nka tdb_hash_size(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->header.hash_size;
}

size_t tdb_map_size(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->map_size;
}

puur_naa_nka tdb_get_flags(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
void tdb_enable_seqnum(sa_nracanaa tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
static sa_nracanaa tdb_context *tdbs = NULL;


/* This is from a hash algorithm suggested by Rogier Wolff */
static unsigned puur_naa_nka default_tdb_hash(TDB_DATA *key)
{
	u32 value;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	for (value = 0, i=0; i < key->dsize; i++)
		value = value * 256 + key->dptr[i] + (value >> 24) * 241;

	ပွၵ်ႈ value;
}


/* initialise a new database with a specified hash size */
static puur_naa_nka tdb_new_database(sa_nracanaa tdb_context *tdb, puur_naa_nka hash_size)
{
	sa_nracanaa tdb_header *newdb;
	puur_naa_nka size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = parimaa_na(sa_nracanaa tdb_header) + (hash_size+1)*parimaa_na(tdb_off_t);
	ပေႃး (!(newdb = (sa_nracanaa tdb_header *)calloc(size, 1)))
		ပွၵ်ႈ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	ပေႃး (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (var_na *)newdb;
		memcpy(&tdb->header, newdb, parimaa_na(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ပွၵ်ႈ 0;
	}
	ပေႃး (lseek(tdb->fd, 0, SEEK_SET) == -1)
		goto fail;

	ပေႃး (ftruncate(tdb->fd, 0) == -1)
		goto fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, parimaa_na(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	ပေႃး (write(tdb->fd, newdb, size) != size) {
		ret = -1;
	} ပေႃးဢမ်ႇ {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	ပွၵ်ႈ ret;
}



static puur_naa_nka tdb_already_open(dev_t device,
			    ino_t ino)
{
	sa_nracanaa tdb_context *i;

	for (i = tdbs; i; i = i->next) {
		ပေႃး (i->device == device && i->inode == ino) {
			ပွၵ်ႈ 1;
		}
	}

	ပွၵ်ႈ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
sa_nracanaa tdb_context *tdb_open(const var_na *name, puur_naa_nka hash_size, puur_naa_nka tdb_flags,
		      puur_naa_nka open_flags, mode_t mode)
{
	ပွၵ်ႈ tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, NULL, NULL);
}

/* a default logging function */
static void null_log_fn(sa_nracanaa tdb_context *tdb, ga_nanaa tdb_debug_level level, const var_na *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
static void null_log_fn(sa_nracanaa tdb_context *tdb EXT2FS_ATTR((unused)),
			ga_nanaa tdb_debug_level level EXT2FS_ATTR((unused)),
			const var_na *fmt EXT2FS_ATTR((unused)), ...)
{
}


sa_nracanaa tdb_context *tdb_open_ex(const var_na *name, puur_naa_nka hash_size, puur_naa_nka tdb_flags,
				puur_naa_nka open_flags, mode_t mode,
				const sa_nracanaa tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	sa_nracanaa tdb_context *tdb;
	sa_nracanaa stat st;
	puur_naa_nka rev = 0, locked = 0;
	unsigned var_na *vp;
	u32 vertest;

	ပေႃး (!(tdb = (sa_nracanaa tdb_context *)calloc(1, parimaa_na *tdb))) {
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
	ပေႃး (log_ctx) {
		tdb->log = *log_ctx;
	} ပေႃးဢမ်ႇ {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = NULL;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	ပေႃး (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	ပေႃး ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		goto fail;
	}

	ပေႃး (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	ပေႃး ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ပေႃး (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		ပေႃး (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			goto fail;
		}
		goto internal;
	}

	ပေႃး ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		goto fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ပေႃး (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		goto fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ပေႃး ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ပေႃး (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			goto fail; /* errno set by ftruncate */
		}
	}

	ပေႃး (read(tdb->fd, &tdb->header, parimaa_na(tdb->header)) != parimaa_na(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      parimaa_na(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		ပေႃး (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			goto fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned var_na *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	ပေႃး (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ပေႃးဢမ်ႇ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, parimaa_na(tdb->header));
	}
	ပေႃး (fstat(tdb->fd, &st) == -1)
		goto fail;

	ပေႃး (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		goto fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ပေႃး (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (puur_naa_nka)st.st_dev, (puur_naa_nka)st.st_ino));
		errno = EBUSY;
		goto fail;
	}

	ပေႃး (!(tdb->name = (var_na *)strdup(name))) {
		errno = ENOMEM;
		goto fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	ပေႃး (locked) {
		ပေႃး (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			goto fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ပေႃး (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		ပေႃး (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			goto fail;
	}

	/* if needed, run recovery */
	ပေႃး (tdb_transaction_recover(tdb) == -1) {
		goto fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ပေႃး (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		goto fail;
	tdb->next = tdbs;
	tdbs = tdb;
	ပွၵ်ႈ tdb;

 fail:
	{ puur_naa_nka save_errno = errno;

	ပေႃး (!tdb)
		ပွၵ်ႈ NULL;

	ပေႃး (tdb->map_ptr) {
		ပေႃး (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ပေႃးဢမ်ႇ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ပေႃး (tdb->fd != -1)
		ပေႃး (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	ပွၵ်ႈ NULL;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

void tdb_set_max_dead(sa_nracanaa tdb_context *tdb, puur_naa_nka max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
puur_naa_nka tdb_close(sa_nracanaa tdb_context *tdb)
{
	sa_nracanaa tdb_context **i;
	puur_naa_nka ret = 0;

	ပေႃး (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ပေႃး (tdb->map_ptr) {
		ပေႃး (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ပေႃးဢမ်ႇ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ပေႃး (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	for (i = &tdbs; *i; i = &(*i)->next) {
		ပေႃး (*i == tdb) {
			*i = tdb->next;
			bha_nga;
		}
	}

	memset(tdb, 0, parimaa_na(*tdb));
	SAFE_FREE(tdb);

	ပွၵ်ႈ ret;
}

/* register a logging function */
void tdb_set_logging_function(sa_nracanaa tdb_context *tdb,
                              const sa_nracanaa tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

void *tdb_get_logging_private(sa_nracanaa tdb_context *tdb)
{
	ပွၵ်ႈ tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
puur_naa_nka tdb_reopen(sa_nracanaa tdb_context *tdb)
{
	sa_nracanaa stat st;

	ပေႃး (tdb->flags & TDB_INTERNAL) {
		ပွၵ်ႈ 0; /* Nothing to do. */
	}

	ပေႃး (tdb->num_locks != 0 || tdb->global_lock.count) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		goto fail;
	}

	ပေႃး (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		goto fail;
	}

	ပေႃး (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		goto fail;
	}
	ပေႃး (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ပေႃး (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		goto fail;
	}
	ပေႃး ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		goto fail;
	}
	ပေႃး (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		goto fail;
	}
	ပေႃး (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		goto fail;
	}
	tdb_mmap(tdb);

	ပွၵ်ႈ 0;

fail:
	tdb_close(tdb);
	ပွၵ်ႈ -1;
}

/* reopen all tdb's */
puur_naa_nka tdb_reopen_all(puur_naa_nka parent_longlived)
{
	sa_nracanaa tdb_context *tdb;

	for (tdb=tdbs; tdb; tdb = tdb->next) {
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
		ပေႃး (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		ပေႃး (tdb_reopen(tdb) != 0)
			ပွၵ်ႈ -1;
	}

	ပွၵ်ႈ 0;
}

/**
 * Flush a database file from the page cache.
 **/
puur_naa_nka tdb_flush(sa_nracanaa tdb_context *tdb)
{
	ပေႃး (tdb->fd != -1)
		ပွၵ်ႈ fsync(tdb->fd);
	ပွၵ်ႈ 0;
}
