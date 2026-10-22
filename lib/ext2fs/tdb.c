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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <string.h>
#ifdef HAVE_SYS_SELECT_H
#অন্তর্ভাব <sys/select.h>
#endif
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <time.h>
#ifdef HAVE_UTIME_H
#অন্তর্ভাব <utime.h>
#endif
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/file.h>
#অন্তর্ভাব <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#অন্তর্ভাব <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#অন্যথা
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((শূন্য *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
স্থির অক্ষর *rep_strdup(ধ্রুবক অক্ষর *s)
{
	অক্ষর *ret;
	পূর্ণসংখ্যা দৈর্ঘ্য;

	যদি (!s)
		ফেরত ফাঁকা;
	দৈর্ঘ্য = strlen(s);
	ret = malloc(দৈর্ঘ্য + 1);
	যদি (ret) {
		strncpy(ret, s, দৈর্ঘ্য);
		ret[দৈর্ঘ্য] = '\0';
	}
	ফেরত ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#যদি (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#অন্যথা
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#যদি defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
প্রকারলেখ পূর্ণসংখ্যা bool;
#endif

#অন্তর্ভাব "tdb.h"

স্থির TDB_তথ্য tdb_null;

#ifndef u32
#define u32 unsigned
#endif

প্রকারলেখ u32 tdb_len_t;
প্রকারলেখ u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned পূর্ণসংখ্যা)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_আকার (2*আকার(কাঠামো list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_আকার 131
#define FREELIST_শীর্ষ (আকার(কাঠামো tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_শীর্ষ(hash) (FREELIST_শীর্ষ + (BUCKET(hash)+1)*আকার(tdb_off_t))
#define TDB_HASHTABLE_আকার(tdb) ((tdb->header.hash_আকার+1)*আকার(tdb_off_t))
#define TDB_তথ্য_START(hash_আকার) TDB_HASH_শীর্ষ(hash_আকার-1)
#define TDB_RECOVERY_শীর্ষ offsetof(কাঠামো tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(কাঠামো tdb_header, sequence_সংখ্যা)
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
#define SAFE_FREE(x) করো { যদি ((x) != ফাঁকা) {মুক্ত(x); (x)=ফাঁকা;} } যখন(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_আকার)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, আকার(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
কাঠামো list_struct {
	tdb_off_t পরবর্তী; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t চাবি_len; /* byte length of key */
	tdb_len_t তথ্য_len; /* byte length of data */
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
কাঠামো tdb_header {
	অক্ষর magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_আকার; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_সংখ্যা; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

কাঠামো tdb_lock_type {
	পূর্ণসংখ্যা list;
	u32 গুনতি;
	u32 ltype;
};

কাঠামো tdb_traverse_lock {
	কাঠামো tdb_traverse_lock *পরবর্তী;
	u32 off;
	u32 hash;
	পূর্ণসংখ্যা lock_rw;
};


কাঠামো tdb_methods {
	পূর্ণসংখ্যা (*tdb_read)(কাঠামো tdb_context *, tdb_off_t , শূন্য *, tdb_len_t , পূর্ণসংখ্যা );
	পূর্ণসংখ্যা (*tdb_write)(কাঠামো tdb_context *, tdb_off_t, ধ্রুবক শূন্য *, tdb_len_t);
	শূন্য (*পরবর্তী_hash_chain)(কাঠামো tdb_context *, u32 *);
	পূর্ণসংখ্যা (*tdb_oob)(কাঠামো tdb_context *, tdb_off_t , পূর্ণসংখ্যা );
	পূর্ণসংখ্যা (*tdb_expand_file)(কাঠামো tdb_context *, tdb_off_t , tdb_off_t );
	পূর্ণসংখ্যা (*tdb_brlock)(কাঠামো tdb_context *, tdb_off_t , পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা, size_t);
};

কাঠামো tdb_context {
	অক্ষর *name; /* the name of the database */
	শূন্য *map_ptr; /* where it is currently mapped */
	পূর্ণসংখ্যা fd; /* open file descriptor for the database */
	tdb_len_t map_আকার; /* how much space has been mapped */
	পূর্ণসংখ্যা read_only; /* opened read-only */
	পূর্ণসংখ্যা traverse_read; /* read-only traversal */
	কাঠামো tdb_lock_type global_lock;
	পূর্ণসংখ্যা অঙ্ক_lockrecs;
	কাঠামো tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	গণনা TDB_ERROR ecode; /* error code for last tdb error */
	কাঠামো tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	কাঠামো tdb_traverse_lock travlocks; /* current traversal locks */
	কাঠামো tdb_context *পরবর্তী; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	কাঠামো tdb_logging_context log;
	unsigned পূর্ণসংখ্যা (*hash_fn)(TDB_তথ্য *চাবি);
	পূর্ণসংখ্যা open_flags; /* flags used in the open - needed by reopen */
	unsigned পূর্ণসংখ্যা অঙ্ক_locks; /* number of chain locks held */
	ধ্রুবক কাঠামো tdb_methods *methods;
	কাঠামো tdb_transaction *transaction;
	পূর্ণসংখ্যা page_আকার;
	পূর্ণসংখ্যা max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_আকার; /* how much space has been mapped */
};


/*
  internal prototypes
*/
স্থির পূর্ণসংখ্যা tdb_munmap(কাঠামো tdb_context *tdb);
স্থির শূন্য tdb_mmap(কাঠামো tdb_context *tdb);
স্থির পূর্ণসংখ্যা tdb_lock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype);
পূর্ণসংখ্যা tdb_lock_nonblock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype);
স্থির পূর্ণসংখ্যা tdb_unlock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype);
স্থির পূর্ণসংখ্যা tdb_brlock(কাঠামো tdb_context *tdb, tdb_off_t offset, পূর্ণসংখ্যা rw_type, পূর্ণসংখ্যা lck_type, পূর্ণসংখ্যা probe, size_t len);
স্থির পূর্ণসংখ্যা tdb_transaction_lock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা ltype);
স্থির পূর্ণসংখ্যা tdb_transaction_unlock(কাঠামো tdb_context *tdb);
স্থির পূর্ণসংখ্যা tdb_brlock_upgrade(কাঠামো tdb_context *tdb, tdb_off_t offset, size_t len);
স্থির পূর্ণসংখ্যা tdb_write_lock_record(কাঠামো tdb_context *tdb, tdb_off_t off);
স্থির পূর্ণসংখ্যা tdb_write_unlock_record(কাঠামো tdb_context *tdb, tdb_off_t off);
স্থির পূর্ণসংখ্যা tdb_ofs_read(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
স্থির পূর্ণসংখ্যা tdb_ofs_write(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
স্থির শূন্য *tdb_convert(শূন্য *buf, u32 আকার_2);
স্থির পূর্ণসংখ্যা tdb_free(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec);
স্থির tdb_off_t tdb_allocate(কাঠামো tdb_context *tdb, tdb_len_t দৈর্ঘ্য, কাঠামো list_struct *rec);
স্থির পূর্ণসংখ্যা tdb_ofs_read(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
স্থির পূর্ণসংখ্যা tdb_ofs_write(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
স্থির পূর্ণসংখ্যা tdb_lock_record(কাঠামো tdb_context *tdb, tdb_off_t off);
স্থির পূর্ণসংখ্যা tdb_unlock_record(কাঠামো tdb_context *tdb, tdb_off_t off);
স্থির পূর্ণসংখ্যা tdb_rec_read(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec);
স্থির পূর্ণসংখ্যা tdb_rec_write(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec);
স্থির পূর্ণসংখ্যা tdb_do_delete(কাঠামো tdb_context *tdb, tdb_off_t rec_ptr, কাঠামো list_struct *rec);
স্থির unsigned অক্ষর *tdb_alloc_read(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
স্থির পূর্ণসংখ্যা tdb_parse_তথ্য(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি,
		   tdb_off_t offset, tdb_len_t len,
		   পূর্ণসংখ্যা (*parser)(TDB_তথ্য চাবি, TDB_তথ্য তথ্য,
				 শূন্য *private_তথ্য),
		   শূন্য *private_তথ্য);
স্থির tdb_off_t tdb_find_lock_hash(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash, পূর্ণসংখ্যা locktype,
			   কাঠামো list_struct *rec);
স্থির শূন্য tdb_io_init(কাঠামো tdb_context *tdb);
স্থির পূর্ণসংখ্যা tdb_expand(কাঠামো tdb_context *tdb, tdb_off_t আকার_2);
স্থির পূর্ণসংখ্যা tdb_rec_free_read(কাঠামো tdb_context *tdb, tdb_off_t off,
		      কাঠামো list_struct *rec);


/* file: error.c */

গণনা TDB_ERROR tdb_error(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->ecode;
}

স্থির কাঠামো tdb_errname {
	গণনা TDB_ERROR ecode; ধ্রুবক অক্ষর *estring;
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
ধ্রুবক অক্ষর *tdb_errorstr(কাঠামো tdb_context *tdb)
{
	u32 i;
	জন্য (i = 0; i < আকার(emap) / আকার(কাঠামো tdb_errname); i++)
		যদি (tdb->ecode == emap[i].ecode)
			ফেরত emap[i].estring;
	ফেরত "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
পূর্ণসংখ্যা tdb_brlock(কাঠামো tdb_context *tdb, tdb_off_t offset,
	       পূর্ণসংখ্যা rw_type, পূর্ণসংখ্যা lck_type, পূর্ণসংখ্যা probe, size_t len)
{
	কাঠামো flock fl;
	পূর্ণসংখ্যা ret;

	যদি (tdb->flags & TDB_NOLOCK) {
		ফেরত 0;
	}

	যদি ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ফেরত -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	করো {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} যখন (ret == -1 && errno == EINTR);

	যদি (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		যদি (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (পূর্ণসংখ্যা)len));
		}
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	ফেরত 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
পূর্ণসংখ্যা tdb_brlock_upgrade(কাঠামো tdb_context *tdb, tdb_off_t offset, size_t len)
{
	পূর্ণসংখ্যা গুনতি = 1000;
	যখন (গুনতি--) {
		কাঠামো timeval tv;
		যদি (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ফেরত 0;
		}
		যদি (errno != EDEADLK) {
			বিরতি;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ফাঁকা, ফাঁকা, ফাঁকা, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	ফেরত -1;
}


/* lock a list in the database. list -1 is the alloc list */
স্থির পূর্ণসংখ্যা _tdb_lock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype, পূর্ণসংখ্যা op)
{
	কাঠামো tdb_lock_type *new_lck;
	পূর্ণসংখ্যা i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	যদি (tdb->global_lock.গুনতি &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ফেরত 0;
	}

	যদি (tdb->global_lock.গুনতি) {
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (list < -1 || list >= (পূর্ণসংখ্যা)tdb->header.hash_আকার) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ফেরত -1;
	}
	যদি (tdb->flags & TDB_NOLOCK)
		ফেরত 0;

	জন্য (i=0; i<tdb->অঙ্ক_lockrecs; i++) {
		যদি (tdb->lockrecs[i].list == list) {
			যদি (tdb->lockrecs[i].গুনতি == 0) {
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
			tdb->lockrecs[i].গুনতি++;
			ফেরত 0;
		}
	}

	new_lck = (কাঠামো tdb_lock_type *)realloc(
		tdb->lockrecs,
		আকার(*tdb->lockrecs) * (tdb->অঙ্ক_lockrecs+1));
	যদি (new_lck == ফাঁকা) {
		errno = ENOMEM;
		ফেরত -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	যদি (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_শীর্ষ+4*list, ltype, op,
				     0, 1)) {
		ফেরত -1;
	}

	tdb->অঙ্ক_locks++;

	tdb->lockrecs[tdb->অঙ্ক_lockrecs].list = list;
	tdb->lockrecs[tdb->অঙ্ক_lockrecs].গুনতি = 1;
	tdb->lockrecs[tdb->অঙ্ক_lockrecs].ltype = ltype;
	tdb->অঙ্ক_lockrecs += 1;

	ফেরত 0;
}

/* lock a list in the database. list -1 is the alloc list */
পূর্ণসংখ্যা tdb_lock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype)
{
	পূর্ণসংখ্যা ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	যদি (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ফেরত ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
পূর্ণসংখ্যা tdb_lock_nonblock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype)
{
	ফেরত _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
পূর্ণসংখ্যা tdb_unlock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা list, পূর্ণসংখ্যা ltype)
{
	পূর্ণসংখ্যা ret = -1;
	পূর্ণসংখ্যা i;
	কাঠামো tdb_lock_type *lck = ফাঁকা;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	যদি (tdb->global_lock.গুনতি &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ফেরত 0;
	}

	যদি (tdb->global_lock.গুনতি) {
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (tdb->flags & TDB_NOLOCK)
		ফেরত 0;

	/* Sanity checks */
	যদি (list < -1 || list >= (পূর্ণসংখ্যা)tdb->header.hash_আকার) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_আকার));
		ফেরত ret;
	}

	জন্য (i=0; i<tdb->অঙ্ক_lockrecs; i++) {
		যদি (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			বিরতি;
		}
	}

	যদি ((lck == ফাঁকা) || (lck->গুনতি == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ফেরত -1;
	}

	যদি (lck->গুনতি > 1) {
		lck->গুনতি--;
		ফেরত 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	যদি (mark_lock) {
		ret = 0;
	} অন্যথা {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_শীর্ষ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->অঙ্ক_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	যদি (tdb->অঙ্ক_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->অঙ্ক_lockrecs-1];
	}
	tdb->অঙ্ক_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	যদি (tdb->অঙ্ক_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	যদি (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ফেরত ret;
}

/*
  get the transaction lock
 */
পূর্ণসংখ্যা tdb_transaction_lock(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা ltype)
{
	যদি (tdb->have_transaction_lock || tdb->global_lock.গুনতি) {
		ফেরত 0;
	}
	যদি (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ফেরত -1;
	}
	tdb->have_transaction_lock = 1;
	ফেরত 0;
}

/*
  release the transaction lock
 */
পূর্ণসংখ্যা tdb_transaction_unlock(কাঠামো tdb_context *tdb)
{
	পূর্ণসংখ্যা ret;
	যদি (!tdb->have_transaction_lock) {
		ফেরত 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	যদি (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	ফেরত ret;
}




/* lock/unlock entire database */
স্থির পূর্ণসংখ্যা _tdb_lockall(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা ltype, পূর্ণসংখ্যা op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	যদি (tdb->read_only || tdb->traverse_read)
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);

	যদি (tdb->global_lock.গুনতি && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.গুনতি++;
		ফেরত 0;
	}

	যদি (tdb->global_lock.গুনতি) {
		/* a global lock of a different type exists */
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (tdb->অঙ্ক_locks != 0) {
		/* can't combine global and chain locks */
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_শীর্ষ, ltype, op,
				     0, 4*tdb->header.hash_আকার)) {
		যদি (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ফেরত -1;
	}

	tdb->global_lock.গুনতি = 1;
	tdb->global_lock.ltype = ltype;

	ফেরত 0;
}



/* unlock entire db */
স্থির পূর্ণসংখ্যা _tdb_unlockall(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	যদি (tdb->read_only || tdb->traverse_read) {
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.গুনতি == 0) {
		ফেরত TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	যদি (tdb->global_lock.গুনতি > 1) {
		tdb->global_lock.গুনতি--;
		ফেরত 0;
	}

	যদি (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_শীর্ষ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_আকার)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ফেরত -1;
	}

	tdb->global_lock.গুনতি = 0;
	tdb->global_lock.ltype = 0;

	ফেরত 0;
}

/* lock entire database with write lock */
পূর্ণসংখ্যা tdb_lockall(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
পূর্ণসংখ্যা tdb_lockall_mark(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
পূর্ণসংখ্যা tdb_lockall_unmark(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
পূর্ণসংখ্যা tdb_lockall_nonblock(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
পূর্ণসংখ্যা tdb_unlockall(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
পূর্ণসংখ্যা tdb_lockall_read(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
পূর্ণসংখ্যা tdb_lockall_read_nonblock(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
পূর্ণসংখ্যা tdb_unlockall_read(কাঠামো tdb_context *tdb)
{
	ফেরত _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
পূর্ণসংখ্যা tdb_chainlock(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_lock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
পূর্ণসংখ্যা tdb_chainlock_nonblock(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
পূর্ণসংখ্যা tdb_chainlock_mark(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_lock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
পূর্ণসংখ্যা tdb_chainlock_unmark(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_unlock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_WRLCK | TDB_MARK_LOCK);
}

পূর্ণসংখ্যা tdb_chainunlock(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_unlock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_WRLCK);
}

পূর্ণসংখ্যা tdb_chainlock_read(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_lock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_RDLCK);
}

পূর্ণসংখ্যা tdb_chainunlock_read(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	ফেরত tdb_unlock(tdb, BUCKET(tdb->hash_fn(&চাবি)), F_RDLCK);
}



/* record lock stops delete underneath */
পূর্ণসংখ্যা tdb_lock_record(কাঠামো tdb_context *tdb, tdb_off_t off)
{
	ফেরত off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
পূর্ণসংখ্যা tdb_write_lock_record(কাঠামো tdb_context *tdb, tdb_off_t off)
{
	কাঠামো tdb_traverse_lock *i;
	জন্য (i = &tdb->travlocks; i; i = i->পরবর্তী)
		যদি (i->off == off)
			ফেরত -1;
	ফেরত tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
পূর্ণসংখ্যা tdb_write_unlock_record(কাঠামো tdb_context *tdb, tdb_off_t off)
{
	ফেরত tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
পূর্ণসংখ্যা tdb_unlock_record(কাঠামো tdb_context *tdb, tdb_off_t off)
{
	কাঠামো tdb_traverse_lock *i;
	u32 গুনতি = 0;

	যদি (off == 0)
		ফেরত 0;
	জন্য (i = &tdb->travlocks; i; i = i->পরবর্তী)
		যদি (i->off == off)
			গুনতি++;
	ফেরত (গুনতি == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
স্থির পূর্ণসংখ্যা tdb_oob(কাঠামো tdb_context *tdb, tdb_off_t len, পূর্ণসংখ্যা probe)
{
	কাঠামো stat st;
	যদি (len <= tdb->map_আকার)
		ফেরত 0;
	যদি (tdb->flags & TDB_INTERNAL) {
		যদি (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (পূর্ণসংখ্যা)len, (পূর্ণসংখ্যা)tdb->map_আকার));
		}
		ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	যদি (fstat(tdb->fd, &st) == -1) {
		ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	যদি (st.st_আকার < (off_t)len) {
		যদি (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (পূর্ণসংখ্যা)len, (পূর্ণসংখ্যা)st.st_আকার));
		}
		ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	যদি (tdb_munmap(tdb) == -1)
		ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_আকার = st.st_আকার;
	tdb_mmap(tdb);
	ফেরত 0;
}

/* write a lump of data at a specified offset */
স্থির পূর্ণসংখ্যা tdb_write(কাঠামো tdb_context *tdb, tdb_off_t off,
		     ধ্রুবক শূন্য *buf, tdb_len_t len)
{
	যদি (len == 0) {
		ফেরত 0;
	}

	যদি (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ফেরত -1;
	}

	যদি (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ফেরত -1;

	যদি (tdb->map_ptr) {
		memcpy(off + (অক্ষর *)tdb->map_ptr, buf, len);
	} অন্যথা যদি (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ফেরত 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
শূন্য *tdb_convert(শূন্য *buf, u32 আকার_2)
{
	u32 i, *p = (u32 *)buf;
	জন্য (i = 0; i < আকার_2 / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ফেরত buf;
}


/* read a lump of data at a specified offset, maybe convert */
স্থির পূর্ণসংখ্যা tdb_read(কাঠামো tdb_context *tdb, tdb_off_t off, শূন্য *buf,
		    tdb_len_t len, পূর্ণসংখ্যা cv)
{
	যদি (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ফেরত -1;
	}

	যদি (tdb->map_ptr) {
		memcpy(buf, off + (অক্ষর *)tdb->map_ptr, len);
	} অন্যথা {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		যদি (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (পূর্ণসংখ্যা)off, (পূর্ণসংখ্যা)len, (পূর্ণসংখ্যা)ret, strerror(errno),
				 (পূর্ণসংখ্যা)tdb->map_আকার));
			ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	যদি (cv) {
		tdb_convert(buf, len);
	}
	ফেরত 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
স্থির শূন্য tdb_পরবর্তী_hash_chain(কাঠামো tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	যদি (tdb->map_ptr) {
		জন্য (;h < tdb->header.hash_আকার;h++) {
			যদি (0 != *(u32 *)(TDB_HASH_শীর্ষ(h) + (unsigned অক্ষর *)tdb->map_ptr)) {
				বিরতি;
			}
		}
	} অন্যথা {
		u32 off=0;
		জন্য (;h < tdb->header.hash_আকার;h++) {
			যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(h), &off) != 0 || off != 0) {
				বিরতি;
			}
		}
	}
	(*chain) = h;
}


পূর্ণসংখ্যা tdb_munmap(কাঠামো tdb_context *tdb)
{
	যদি (tdb->flags & TDB_INTERNAL)
		ফেরত 0;

#ifdef HAVE_MMAP
	যদি (tdb->map_ptr) {
		পূর্ণসংখ্যা ret = munmap(tdb->map_ptr, tdb->real_map_আকার);
		যদি (ret != 0)
			ফেরত ret;
		tdb->real_map_আকার = 0;
	}
#endif
	tdb->map_ptr = ফাঁকা;
	ফেরত 0;
}

শূন্য tdb_mmap(কাঠামো tdb_context *tdb)
{
	যদি (tdb->flags & TDB_INTERNAL)
		ফেরত;

#ifdef HAVE_MMAP
	যদি (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ফাঁকা, tdb->map_আকার,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		যদি (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_আকার = 0;
			tdb->map_ptr = ফাঁকা;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_আকার, strerror(errno)));
		}
		tdb->real_map_আকার = tdb->map_আকার;
	} অন্যথা {
		tdb->map_ptr = ফাঁকা;
	}
#অন্যথা
	tdb->map_ptr = ফাঁকা;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
স্থির পূর্ণসংখ্যা tdb_expand_file(কাঠামো tdb_context *tdb, tdb_off_t আকার_2, tdb_off_t addition)
{
	অক্ষর buf[1024];

	যদি (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ফেরত -1;
	}

	যদি (ftruncate(tdb->fd, আকার_2+addition) == -1) {
		অক্ষর b = 0;
		যদি (pwrite(tdb->fd,  &b, 1, (আকার_2+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 আকার_2+addition, strerror(errno)));
			ফেরত -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, আকার(buf));
	যখন (addition) {
		পূর্ণসংখ্যা n = addition>আকার(buf)?আকার(buf):addition;
		পূর্ণসংখ্যা ret = pwrite(tdb->fd, buf, n, আকার_2);
		যদি (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ফেরত -1;
		}
		addition -= n;
		আকার_2 += n;
	}
	ফেরত 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
পূর্ণসংখ্যা tdb_expand(কাঠামো tdb_context *tdb, tdb_off_t আকার_2)
{
	কাঠামো list_struct rec;
	tdb_off_t offset;

	যদি (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ফেরত -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_আকার + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	আকার_2 = TDB_ALIGN(tdb->map_আকার + আকার_2*10, tdb->page_আকার) - tdb->map_আকার;

	যদি (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	যদি (!(tdb->flags & TDB_INTERNAL)) {
		যদি (tdb->methods->tdb_expand_file(tdb, tdb->map_আকার, আকার_2) != 0)
			যাও fail;
	}

	tdb->map_আকার += আকার_2;

	যদি (tdb->flags & TDB_INTERNAL) {
		অক্ষর *new_map_ptr = (অক্ষর *)realloc(tdb->map_ptr,
						    tdb->map_আকার);
		যদি (!new_map_ptr) {
			tdb->map_আকার -= আকার_2;
			যাও fail;
		}
		tdb->map_ptr = new_map_ptr;
	} অন্যথা {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',আকার(rec));
	rec.rec_len = আকার_2 - আকার(rec);

	/* link it into the free list */
	offset = tdb->map_আকার - আকার_2;
	যদি (tdb_free(tdb, offset, &rec) == -1)
		যাও fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত -1;
}

/* read/write a tdb_off_t */
পূর্ণসংখ্যা tdb_ofs_read(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	ফেরত tdb->methods->tdb_read(tdb, offset, (অক্ষর*)d, আকার(*d), DOCONV());
}

পূর্ণসংখ্যা tdb_ofs_write(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ফেরত tdb->methods->tdb_write(tdb, offset, CONVERT(off), আকার(*d));
}


/* read a lump of data, allocating the space for it */
unsigned অক্ষর *tdb_alloc_read(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned অক্ষর *buf;

	/* some systems don't like zero length malloc */
	যদি (len == 0) {
		len = 1;
	}

	যদি (!(buf = (unsigned অক্ষর *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ফেরত TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	যদি (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		ফেরত ফাঁকা;
	}
	ফেরত buf;
}

/* Give a piece of tdb data to a parser */

পূর্ণসংখ্যা tdb_parse_তথ্য(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি,
		   tdb_off_t offset, tdb_len_t len,
		   পূর্ণসংখ্যা (*parser)(TDB_তথ্য চাবি, TDB_তথ্য তথ্য,
				 শূন্য *private_তথ্য),
		   শূন্য *private_তথ্য)
{
	TDB_তথ্য তথ্য;
	পূর্ণসংখ্যা ফলাফল;

	তথ্য.dsize = len;

	যদি ((tdb->transaction == ফাঁকা) && (tdb->map_ptr != ফাঁকা)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		যদি (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			ফেরত -1;
		}
		তথ্য.dptr = offset + (unsigned অক্ষর *)tdb->map_ptr;
		ফেরত parser(চাবি, তথ্য, private_তথ্য);
	}

	যদি (!(তথ্য.dptr = tdb_alloc_read(tdb, offset, len))) {
		ফেরত -1;
	}

	ফলাফল = parser(চাবি, তথ্য, private_তথ্য);
	মুক্ত(তথ্য.dptr);
	ফেরত ফলাফল;
}

/* read/write a record */
পূর্ণসংখ্যা tdb_rec_read(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec)
{
	যদি (tdb->methods->tdb_read(tdb, offset, rec, আকার(*rec),DOCONV()) == -1)
		ফেরত -1;
	যদি (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		ফেরত TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ফেরত tdb->methods->tdb_oob(tdb, rec->পরবর্তী+আকার(*rec), 0);
}

পূর্ণসংখ্যা tdb_rec_write(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec)
{
	কাঠামো list_struct r = *rec;
	ফেরত tdb->methods->tdb_write(tdb, offset, CONVERT(r), আকার(r));
}

স্থির ধ্রুবক কাঠামো tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_পরবর্তী_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
শূন্য tdb_io_init(কাঠামো tdb_context *tdb)
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

কাঠামো tdb_transaction_el {
	কাঠামো tdb_transaction_el *পরবর্তী, *পূর্ব;
	tdb_off_t offset;
	tdb_len_t দৈর্ঘ্য;
	unsigned অক্ষর *তথ্য;
};

/*
  hold the context of any current transaction
*/
কাঠামো tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ধ্রুবক কাঠামো tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	কাঠামো tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	পূর্ণসংখ্যা transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	পূর্ণসংখ্যা nesting;

	/* old file size before transaction */
	tdb_len_t old_map_আকার;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
স্থির পূর্ণসংখ্যা transaction_read(কাঠামো tdb_context *tdb, tdb_off_t off, শূন্য *buf,
			    tdb_len_t len, পূর্ণসংখ্যা cv)
{
	কাঠামো tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	জন্য (el=tdb->transaction->elements_last;el;el=el->পূর্ব) {
		tdb_len_t partial;

		যদি (off+len <= el->offset) {
			চালু;
		}
		যদি (off >= el->offset + el->দৈর্ঘ্য) {
			চালু;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		যদি (off < el->offset) {
			partial = el->offset - off;
			যদি (transaction_read(tdb, off, buf, partial, cv) != 0) {
				যাও fail;
			}
			len -= partial;
			off += partial;
			buf = (শূন্য *)(partial + (অক্ষর *)buf);
		}
		যদি (off + len <= el->offset + el->দৈর্ঘ্য) {
			partial = len;
		} অন্যথা {
			partial = el->offset + el->দৈর্ঘ্য - off;
		}
		memcpy(buf, el->তথ্য + (off - el->offset), partial);
		যদি (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (শূন্য *)(partial + (অক্ষর *)buf);

		যদি (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			যাও fail;
		}

		ফেরত 0;
	}

	/* its not in the transaction elements - do a real read */
	ফেরত tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ফেরত -1;
}


/*
  write while in a transaction
*/
স্থির পূর্ণসংখ্যা transaction_write(কাঠামো tdb_context *tdb, tdb_off_t off,
			     ধ্রুবক শূন্য *buf, tdb_len_t len)
{
	কাঠামো tdb_transaction_el *el, *best_el=ফাঁকা;

	যদি (len == 0) {
		ফেরত 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	যদি (len == আকার(tdb_off_t) && off >= FREELIST_শীর্ষ &&
	    off < FREELIST_শীর্ষ+TDB_HASHTABLE_আকার(tdb)) {
		u32 chain = (off-FREELIST_শীর্ষ) / আকার(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	জন্য (el=tdb->transaction->elements_last;el;el=el->পূর্ব) {
		tdb_len_t partial;

		যদি (best_el == ফাঁকা && off == el->offset+el->দৈর্ঘ্য) {
			best_el = el;
		}

		যদি (off+len <= el->offset) {
			চালু;
		}
		যদি (off >= el->offset + el->দৈর্ঘ্য) {
			চালু;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		যদি (off < el->offset) {
			partial = el->offset - off;
			যদি (transaction_write(tdb, off, buf, partial) != 0) {
				যাও fail;
			}
			len -= partial;
			off += partial;
			buf = (ধ্রুবক শূন্য *)(partial + (ধ্রুবক অক্ষর *)buf);
		}
		যদি (off + len <= el->offset + el->দৈর্ঘ্য) {
			partial = len;
		} অন্যথা {
			partial = el->offset + el->দৈর্ঘ্য - off;
		}
		memcpy(el->তথ্য + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ধ্রুবক শূন্য *)(partial + (ধ্রুবক অক্ষর *)buf);

		যদি (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			যাও fail;
		}

		ফেরত 0;
	}

	/* see if we can append the new entry to an existing entry */
	যদি (best_el && best_el->offset + best_el->দৈর্ঘ্য == off &&
	    (off+len < tdb->transaction->old_map_আকার ||
	     off > tdb->transaction->old_map_আকার)) {
		unsigned অক্ষর *তথ্য = best_el->তথ্য;
		el = best_el;
		el->তথ্য = (unsigned অক্ষর *)realloc(el->তথ্য,
						    el->দৈর্ঘ্য + len);
		যদি (el->তথ্য == ফাঁকা) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->তথ্য = তথ্য;
			ফেরত -1;
		}
		যদি (buf) {
			memcpy(el->তথ্য + el->দৈর্ঘ্য, buf, len);
		} অন্যথা {
			memset(el->তথ্য + el->দৈর্ঘ্য, TDB_PAD_BYTE, len);
		}
		el->দৈর্ঘ্য += len;
		ফেরত 0;
	}

	/* add a new entry at the end of the list */
	el = (কাঠামো tdb_transaction_el *)malloc(আকার(*el));
	যদি (el == ফাঁকা) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ফেরত -1;
	}
	el->পরবর্তী = ফাঁকা;
	el->পূর্ব = tdb->transaction->elements_last;
	el->offset = off;
	el->দৈর্ঘ্য = len;
	el->তথ্য = (unsigned অক্ষর *)malloc(len);
	যদি (el->তথ্য == ফাঁকা) {
		মুক্ত(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ফেরত -1;
	}
	যদি (buf) {
		memcpy(el->তথ্য, buf, len);
	} অন্যথা {
		memset(el->তথ্য, TDB_PAD_BYTE, len);
	}
	যদি (el->পূর্ব) {
		el->পূর্ব->পরবর্তী = el;
	} অন্যথা {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	ফেরত 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ফেরত -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
স্থির শূন্য transaction_পরবর্তী_hash_chain(কাঠামো tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	জন্য (;h < tdb->header.hash_আকার;h++) {
		/* the +1 takes account of the freelist */
		যদি (0 != tdb->transaction->hash_heads[h+1]) {
			বিরতি;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
স্থির পূর্ণসংখ্যা transaction_oob(কাঠামো tdb_context *tdb, tdb_off_t len,
			   পূর্ণসংখ্যা probe EXT2FS_ATTR((unused)))
{
	যদি (len <= tdb->map_আকার) {
		ফেরত 0;
	}
	ফেরত TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
স্থির পূর্ণসংখ্যা transaction_expand_file(কাঠামো tdb_context *tdb, tdb_off_t আকার_2,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	যদি (transaction_write(tdb, আকার_2, ফাঁকা, addition) != 0) {
		ফেরত -1;
	}

	ফেরত 0;
}

/*
  brlock during a transaction - ignore them
*/
স্থির পূর্ণসংখ্যা transaction_brlock(কাঠামো tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      পূর্ণসংখ্যা rw_type EXT2FS_ATTR((unused)),
			      পূর্ণসংখ্যা lck_type EXT2FS_ATTR((unused)),
			      পূর্ণসংখ্যা probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	ফেরত 0;
}

স্থির ধ্রুবক কাঠামো tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_পরবর্তী_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
পূর্ণসংখ্যা tdb_transaction_start(কাঠামো tdb_context *tdb)
{
	/* some sanity checks */
	যদি (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ফেরত -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	যদি (tdb->transaction != ফাঁকা) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ফেরত 0;
	}

	যদি (tdb->অঙ্ক_locks != 0 || tdb->global_lock.গুনতি) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ফেরত -1;
	}

	যদি (tdb->travlocks.পরবর্তী != ফাঁকা) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ফেরত -1;
	}

	tdb->transaction = (কাঠামো tdb_transaction *)
		calloc(আকার(কাঠামো tdb_transaction), 1);
	যদি (tdb->transaction == ফাঁকা) {
		tdb->ecode = TDB_ERR_OOM;
		ফেরত -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	যদি (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		ফেরত -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	যদি (tdb_brlock(tdb, FREELIST_শীর্ষ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		যাও fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_আকার+1, আকার(u32));
	যদি (tdb->transaction->hash_heads == ফাঁকা) {
		tdb->ecode = TDB_ERR_OOM;
		যাও fail;
	}
	যদি (tdb->methods->tdb_read(tdb, FREELIST_শীর্ষ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_আকার(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		যাও fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_আকার + 1, 1);
	tdb->transaction->old_map_আকার = tdb->map_আকার;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	যদি (transaction_write(tdb, FREELIST_শীর্ষ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_আকার(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		যাও fail;
	}

	ফেরত 0;

fail:
	tdb_brlock(tdb, FREELIST_শীর্ষ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	ফেরত -1;
}


/*
  cancel the current transaction
*/
পূর্ণসংখ্যা tdb_transaction_cancel(কাঠামো tdb_context *tdb)
{
	যদি (tdb->transaction == ফাঁকা) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ফেরত -1;
	}

	যদি (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ফেরত 0;
	}

	tdb->map_আকার = tdb->transaction->old_map_আকার;

	/* free all the transaction elements */
	যখন (tdb->transaction->elements) {
		কাঠামো tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->পরবর্তী;
		মুক্ত(el->তথ্য);
		মুক্ত(el);
	}

	/* remove any global lock created during the transaction */
	যদি (tdb->global_lock.গুনতি != 0) {
		tdb_brlock(tdb, FREELIST_শীর্ষ, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_আকার);
		tdb->global_lock.গুনতি = 0;
	}

	/* remove any locks created during the transaction */
	যদি (tdb->অঙ্ক_locks != 0) {
		পূর্ণসংখ্যা i;
		জন্য (i=0;i<tdb->অঙ্ক_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_শীর্ষ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->অঙ্ক_locks = 0;
		tdb->অঙ্ক_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_শীর্ষ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	ফেরত 0;
}

/*
  sync to disk
*/
স্থির পূর্ণসংখ্যা transaction_sync(কাঠামো tdb_context *tdb, tdb_off_t offset, tdb_len_t দৈর্ঘ্য)
{
	যদি (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ফেরত -1;
	}
#যদি defined(HAVE_MSYNC) && defined(MS_SYNC)
	যদি (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_আকার-1);
		যদি (msync(moffset + (অক্ষর *)tdb->map_ptr,
			  দৈর্ঘ্য + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ফেরত -1;
		}
	}
#endif
	ফেরত 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
স্থির tdb_len_t tdb_recovery_আকার(কাঠামো tdb_context *tdb)
{
	কাঠামো tdb_transaction_el *el;
	tdb_len_t recovery_আকার = 0;

	recovery_আকার = আকার(u32);
	জন্য (el=tdb->transaction->elements;el;el=el->পরবর্তী) {
		যদি (el->offset >= tdb->transaction->old_map_আকার) {
			চালু;
		}
		recovery_আকার += 2*আকার(tdb_off_t) + el->দৈর্ঘ্য;
	}

	ফেরত recovery_আকার;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
স্থির পূর্ণসংখ্যা tdb_recovery_allocate(কাঠামো tdb_context *tdb,
				 tdb_len_t *recovery_আকার,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_আকার)
{
	কাঠামো list_struct rec;
	ধ্রুবক কাঠামো tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_শীর্ষ;

	যদি (tdb_ofs_read(tdb, TDB_RECOVERY_শীর্ষ, &recovery_শীর্ষ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ফেরত -1;
	}

	rec.rec_len = 0;

	যদি (recovery_শীর্ষ != 0 &&
	    methods->tdb_read(tdb, recovery_শীর্ষ, &rec, আকার(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ফেরত -1;
	}

	*recovery_আকার = tdb_recovery_আকার(tdb);

	যদি (recovery_শীর্ষ != 0 && *recovery_আকার <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_আকার = rec.rec_len;
		*recovery_offset = recovery_শীর্ষ;
		ফেরত 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	যদি (recovery_শীর্ষ != 0) {
		যদি (tdb_free(tdb, recovery_শীর্ষ, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ফেরত -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_আকার = tdb_recovery_আকার(tdb);

	/* round up to a multiple of page size */
	*recovery_max_আকার = TDB_ALIGN(আকার(rec) + *recovery_আকার, tdb->page_আকার) - আকার(rec);
	*recovery_offset = tdb->map_আকার;
	recovery_শীর্ষ = *recovery_offset;

	যদি (methods->tdb_expand_file(tdb, tdb->transaction->old_map_আকার,
				     (tdb->map_আকার - tdb->transaction->old_map_আকার) +
				     আকার(rec) + *recovery_max_আকার) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ফেরত -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_আকার + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_আকার = tdb->map_আকার;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_শীর্ষ);
	যদি (methods->tdb_write(tdb, TDB_RECOVERY_শীর্ষ,
			       &recovery_শীর্ষ, আকার(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ফেরত -1;
	}

	ফেরত 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
স্থির পূর্ণসংখ্যা transaction_setup_recovery(কাঠামো tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	কাঠামো tdb_transaction_el *el;
	tdb_len_t recovery_আকার;
	unsigned অক্ষর *তথ্য, *p;
	ধ্রুবক কাঠামো tdb_methods *methods = tdb->transaction->io_methods;
	কাঠামো list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_আকার;
	tdb_off_t old_map_আকার = tdb->transaction->old_map_আকার;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	যদি (tdb_recovery_allocate(tdb, &recovery_আকার,
				  &recovery_offset, &recovery_max_আকার) == -1) {
		ফেরত -1;
	}

	তথ্য = (unsigned অক্ষর *)malloc(recovery_আকার + আকার(*rec));
	যদি (তথ্য == ফাঁকা) {
		tdb->ecode = TDB_ERR_OOM;
		ফেরত -1;
	}

	rec = (কাঠামো list_struct *)তথ্য;
	memset(rec, 0, আকার(*rec));

	rec->magic    = 0;
	rec->তথ্য_len = recovery_আকার;
	rec->rec_len  = recovery_max_আকার;
	rec->চাবি_len  = old_map_আকার;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = তথ্য + আকার(*rec);
	জন্য (el=tdb->transaction->elements;el;el=el->পরবর্তী) {
		যদি (el->offset >= old_map_আকার) {
			চালু;
		}
		যদি (el->offset + el->দৈর্ঘ্য > tdb->transaction->old_map_আকার) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			মুক্ত(তথ্য);
			tdb->ecode = TDB_ERR_CORRUPT;
			ফেরত -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->দৈর্ঘ্য, 4);
		যদি (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		যদি (methods->tdb_read(tdb, el->offset, p + 8, el->দৈর্ঘ্য, 0) != 0) {
			মুক্ত(তথ্য);
			tdb->ecode = TDB_ERR_IO;
			ফেরত -1;
		}
		p += 8 + el->দৈর্ঘ্য;
	}

	/* and the tailer */
	tailer = আকার(*rec) + recovery_max_আকার;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	যদি (methods->tdb_write(tdb, recovery_offset, তথ্য, আকার(*rec) + recovery_আকার) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		মুক্ত(তথ্য);
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	যদি (transaction_sync(tdb, recovery_offset, আকার(*rec) + recovery_আকার) == -1) {
		মুক্ত(তথ্য);
		ফেরত -1;
	}

	মুক্ত(তথ্য);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(কাঠামো list_struct, magic);

	যদি (methods->tdb_write(tdb, *magic_offset, &magic, আকার(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	/* ensure the recovery magic marker is on disk */
	যদি (transaction_sync(tdb, *magic_offset, আকার(magic)) == -1) {
		ফেরত -1;
	}

	ফেরত 0;
}

/*
  commit the current transaction
*/
পূর্ণসংখ্যা tdb_transaction_commit(কাঠামো tdb_context *tdb)
{
	ধ্রুবক কাঠামো tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	যদি (tdb->transaction == ফাঁকা) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ফেরত -1;
	}

	যদি (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ফেরত -1;
	}

	যদি (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ফেরত 0;
	}

	/* check for a null transaction */
	যদি (tdb->transaction->elements == ফাঁকা) {
		tdb_transaction_cancel(tdb);
		ফেরত 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	যদি (tdb->অঙ্ক_locks || tdb->global_lock.গুনতি) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ফেরত -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	যদি (tdb_brlock_upgrade(tdb, FREELIST_শীর্ষ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ফেরত -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	যদি (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ফেরত -1;
	}

	যদি (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		যদি (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ফেরত -1;
		}
	}

	/* expand the file to the new size if needed */
	যদি (tdb->map_আকার != tdb->transaction->old_map_আকার) {
		যদি (methods->tdb_expand_file(tdb, tdb->transaction->old_map_আকার,
					     tdb->map_আকার -
					     tdb->transaction->old_map_আকার) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ফেরত -1;
		}
		tdb->map_আকার = tdb->transaction->old_map_আকার;
		methods->tdb_oob(tdb, tdb->map_আকার + 1, 1);
	}

	/* perform all the writes */
	যখন (tdb->transaction->elements) {
		কাঠামো tdb_transaction_el *el = tdb->transaction->elements;

		যদি (methods->tdb_write(tdb, el->offset, el->তথ্য, el->দৈর্ঘ্য) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ফেরত -1;
		}
		tdb->transaction->elements = el->পরবর্তী;
		মুক্ত(el->তথ্য);
		মুক্ত(el);
	}

	যদি (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		যদি (transaction_sync(tdb, 0, tdb->map_আকার) == -1) {
			ফেরত -1;
		}

		/* remove the recovery marker */
		যদি (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ফেরত -1;
		}

		/* ensure the recovery marker has been removed on disk */
		যদি (transaction_sync(tdb, magic_offset, 4) == -1) {
			ফেরত -1;
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
	utime(tdb->name, ফাঁকা);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	ফেরত 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
পূর্ণসংখ্যা tdb_transaction_recover(কাঠামো tdb_context *tdb)
{
	tdb_off_t recovery_শীর্ষ, recovery_eof;
	unsigned অক্ষর *তথ্য, *p;
	u32 zero = 0;
	কাঠামো list_struct rec;

	/* find the recovery area */
	যদি (tdb_ofs_read(tdb, TDB_RECOVERY_শীর্ষ, &recovery_শীর্ষ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	যদি (recovery_শীর্ষ == 0) {
		/* we have never allocated a recovery record */
		ফেরত 0;
	}

	/* read the recovery record */
	যদি (tdb->methods->tdb_read(tdb, recovery_শীর্ষ, &rec,
				   আকার(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	যদি (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ফেরত 0;
	}

	যদি (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ফেরত -1;
	}

	recovery_eof = rec.চাবি_len;

	তথ্য = (unsigned অক্ষর *)malloc(rec.তথ্য_len);
	যদি (তথ্য == ফাঁকা) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ফেরত -1;
	}

	/* read the full recovery data */
	যদি (tdb->methods->tdb_read(tdb, recovery_শীর্ষ + আকার(rec), তথ্য,
				   rec.তথ্য_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		মুক্ত(তথ্য);
		ফেরত -1;
	}

	/* recover the file data */
	p = তথ্য;
	যখন (p+8 < তথ্য + rec.তথ্য_len) {
		u32 ofs, len;
		যদি (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		যদি (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			মুক্ত(তথ্য);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ফেরত -1;
		}
		p += 8 + len;
	}

	মুক্ত(তথ্য);

	যদি (transaction_sync(tdb, 0, tdb->map_আকার) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	যদি (recovery_eof <= recovery_শীর্ষ) {
		যদি (tdb_ofs_write(tdb, TDB_RECOVERY_শীর্ষ, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ফেরত -1;
		}
	}

	/* remove the recovery magic */
	যদি (tdb_ofs_write(tdb, recovery_শীর্ষ + offsetof(কাঠামো list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	যদি (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}
	tdb->map_আকার = recovery_eof;
	tdb_mmap(tdb);

	যদি (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ফেরত -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ফেরত 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
স্থির পূর্ণসংখ্যা tdb_rec_free_read(কাঠামো tdb_context *tdb, tdb_off_t off, কাঠামো list_struct *rec)
{
	যদি (tdb->methods->tdb_read(tdb, off, rec, আকার(*rec),DOCONV()) == -1)
		ফেরত -1;

	যদি (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		যদি (tdb->methods->tdb_write(tdb, off, rec, আকার(*rec)) == -1)
			ফেরত -1;
	}

	যদি (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ফেরত TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	যদি (tdb->methods->tdb_oob(tdb, rec->পরবর্তী+আকার(*rec), 0) != 0)
		ফেরত -1;
	ফেরত 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
স্থির পূর্ণসংখ্যা remove_from_freelist(কাঠামো tdb_context *tdb, tdb_off_t off, tdb_off_t পরবর্তী)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_শীর্ষ;
	যখন (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		যদি (i == off) {
			/* We've found it! */
			ফেরত tdb_ofs_write(tdb, last_ptr, &পরবর্তী);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ফেরত TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
স্থির পূর্ণসংখ্যা update_tailer(কাঠামো tdb_context *tdb, tdb_off_t offset,
			 ধ্রুবক কাঠামো list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = আকার(*rec) + rec->rec_len;
	ফেরত tdb_ofs_write(tdb, offset + totalsize - আকার(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
পূর্ণসংখ্যা tdb_free(কাঠামো tdb_context *tdb, tdb_off_t offset, কাঠামো list_struct *rec)
{
	tdb_off_t ডান, বাম;

	/* Allocation and tailer lock */
	যদি (tdb_lock(tdb, -1, F_WRLCK) != 0)
		ফেরত -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	যদি (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		যাও fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ডান = offset + আকার(*rec) + rec->rec_len;
	যদি (ডান + আকার(*rec) <= tdb->map_আকার) {
		কাঠামো list_struct r;

		যদি (tdb->methods->tdb_read(tdb, ডান, &r, আকার(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ডান));
			যাও বাম;
		}

		/* If it's free, expand to include it. */
		যদি (r.magic == TDB_FREE_MAGIC) {
			যদি (remove_from_freelist(tdb, ডান, r.পরবর্তী) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ডান));
				যাও বাম;
			}
			rec->rec_len += আকার(r) + r.rec_len;
		}
	}

বাম:
	/* Look left */
	বাম = offset - আকার(tdb_off_t);
	যদি (বাম > TDB_তথ্য_START(tdb->header.hash_আকার)) {
		কাঠামো list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		যদি (tdb_ofs_read(tdb, বাম, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", বাম));
			যাও update;
		}

		/* it could be uninitialised data */
		যদি (leftsize == 0 || leftsize == TDB_PAD_U32) {
			যাও update;
		}

		বাম = offset - leftsize;

		/* Now read in record */
		যদি (tdb->methods->tdb_read(tdb, বাম, &l, আকার(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", বাম, leftsize));
			যাও update;
		}

		/* If it's free, expand to include it. */
		যদি (l.magic == TDB_FREE_MAGIC) {
			যদি (remove_from_freelist(tdb, বাম, l.পরবর্তী) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", বাম));
				যাও update;
			} অন্যথা {
				offset = বাম;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	যদি (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		যাও fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	যদি (tdb_ofs_read(tdb, FREELIST_শীর্ষ, &rec->পরবর্তী) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_শীর্ষ, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		যাও fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
স্থির tdb_off_t tdb_allocate_ofs(কাঠামো tdb_context *tdb, tdb_len_t দৈর্ঘ্য, tdb_off_t rec_ptr,
				কাঠামো list_struct *rec, tdb_off_t last_ptr)
{
	কাঠামো list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', আকার(newrec));

	/* found it - now possibly split it up  */
	যদি (rec->rec_len > দৈর্ঘ্য + MIN_REC_আকার) {
		/* Length of left piece */
		দৈর্ঘ্য = TDB_ALIGN(দৈর্ঘ্য, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (আকার(*rec) + দৈর্ঘ্য);
		newrec_ptr = rec_ptr + আকার(*rec) + দৈর্ঘ্য;

		/* And left record is shortened */
		rec->rec_len = দৈর্ঘ্য;
	} অন্যথা {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	যদি (tdb_ofs_write(tdb, last_ptr, &rec->পরবর্তী) == -1) {
		ফেরত 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	যদি (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ফেরত 0;
	}

	/* Did we create new block? */
	যদি (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		যদি (update_tailer(tdb, rec_ptr, rec) == -1) {
			ফেরত 0;
		}

		/* Free new record */
		যদি (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ফেরত 0;
		}
	}

	/* all done - return the new record offset */
	ফেরত rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(কাঠামো tdb_context *tdb, tdb_len_t দৈর্ঘ্য, কাঠামো list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	কাঠামো {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	যদি (tdb_lock(tdb, -1, F_WRLCK) == -1)
		ফেরত 0;

	/* Extra bytes required for tailer */
	দৈর্ঘ্য += আকার(tdb_off_t);

 again:
	last_ptr = FREELIST_শীর্ষ;

	/* read in the freelist top */
	যদি (tdb_ofs_read(tdb, FREELIST_শীর্ষ, &rec_ptr) == -1)
		যাও fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	যখন (rec_ptr) {
		যদি (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			যাও fail;
		}

		যদি (rec->rec_len >= দৈর্ঘ্য) {
			যদি (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				যদি (bestfit.rec_len < 2*দৈর্ঘ্য) {
					বিরতি;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->পরবর্তী;
	}

	যদি (bestfit.rec_ptr != 0) {
		যদি (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			যাও fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, দৈর্ঘ্য, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ফেরত newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	যদি (tdb_expand(tdb, দৈর্ঘ্য + আকার(*rec)) == 0)
		যাও again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

স্থির পূর্ণসংখ্যা seen_ঢোকাও(কাঠামো tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_তথ্য চাবি, তথ্য;

	memset(&তথ্য, '\0', আকার(তথ্য));
	চাবি.dptr = (unsigned অক্ষর *)&rec_ptr;
	চাবি.dsize = আকার(rec_ptr);
	ফেরত tdb_store(mem_tdb, চাবি, তথ্য, TDB_ঢোকাও);
}

পূর্ণসংখ্যা tdb_validate_freelist(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা *pnum_entries)
{
	কাঠামো tdb_context *mem_tdb = ফাঁকা;
	কাঠামো list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	পূর্ণসংখ্যা ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_আকার,
				TDB_INTERNAL, O_RDWR, 0600);
	যদি (!mem_tdb) {
		ফেরত -1;
	}

	যদি (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ফেরত 0;
	}

	last_ptr = FREELIST_শীর্ষ;

	/* Store the FREELIST_TOP record. */
	যদি (seen_ঢোকাও(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		যাও fail;
	}

	/* read in the freelist top */
	যদি (tdb_ofs_read(tdb, FREELIST_শীর্ষ, &rec_ptr) == -1) {
		যাও fail;
	}

	যখন (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		যদি (seen_ঢোকাও(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			যাও fail;
		}

		যদি (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			যাও fail;
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec.পরবর্তী;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
স্থির পূর্ণসংখ্যা tdb_পরবর্তী_lock(কাঠামো tdb_context *tdb, কাঠামো tdb_traverse_lock *tlock,
			 কাঠামো list_struct *rec)
{
	পূর্ণসংখ্যা want_পরবর্তী = (tlock->off != 0);

	/* Lock each chain from the start one. */
	জন্য (; tlock->hash < tdb->header.hash_আকার; tlock->hash++) {
		যদি (!tlock->off && tlock->hash != 0) {
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
			tdb->methods->পরবর্তী_hash_chain(tdb, &tlock->hash);
			যদি (tlock->hash == tdb->header.hash_আকার) {
				চালু;
			}
		}

		যদি (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			ফেরত -1;

		/* No previous record?  Start at top of chain. */
		যদি (!tlock->off) {
			যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(tlock->hash),
				     &tlock->off) == -1)
				যাও fail;
		} অন্যথা {
			/* Otherwise unlock the previous record. */
			যদি (tdb_unlock_record(tdb, tlock->off) != 0)
				যাও fail;
		}

		যদি (want_পরবর্তী) {
			/* We have offset of old record: grab next */
			যদি (tdb_rec_read(tdb, tlock->off, rec) == -1)
				যাও fail;
			tlock->off = rec->পরবর্তী;
		}

		/* Iterate through chain */
		যখন( tlock->off) {
			tdb_off_t current;
			যদি (tdb_rec_read(tdb, tlock->off, rec) == -1)
				যাও fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			যদি (tlock->off == rec->পরবর্তী) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				যাও fail;
			}

			যদি (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				যদি (tdb_lock_record(tdb, tlock->off) != 0)
					যাও fail;
				ফেরত tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->পরবর্তী;
			যদি (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				যাও fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_পরবর্তী = 0;
	}
	/* We finished iteration without finding anything */
	ফেরত TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	যদি (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ফেরত -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
স্থির পূর্ণসংখ্যা tdb_traverse_internal(কাঠামো tdb_context *tdb,
				 tdb_traverse_func fn, শূন্য *private_তথ্য,
				 কাঠামো tdb_traverse_lock *tl)
{
	TDB_তথ্য চাবি, dbuf;
	কাঠামো list_struct rec;
	পূর্ণসংখ্যা ret, গুনতি = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->পরবর্তী = tdb->travlocks.পরবর্তী;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.পরবর্তী = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	যখন ((ret = tdb_পরবর্তী_lock(tdb, tl, &rec)) > 0) {
		গুনতি++;
		/* now read the full record */
		চাবি.dptr = tdb_alloc_read(tdb, tl->off + আকার(rec),
					  rec.চাবি_len + rec.তথ্য_len);
		যদি (!চাবি.dptr) {
			ret = -1;
			যদি (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				যাও out;
			যদি (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			যাও out;
		}
		চাবি.dsize = rec.চাবি_len;
		dbuf.dptr = চাবি.dptr + rec.চাবি_len;
		dbuf.dsize = rec.তথ্য_len;

		/* Drop chain lock, call out */
		যদি (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(চাবি.dptr);
			যাও out;
		}
		যদি (fn && fn(tdb, চাবি, dbuf, private_তথ্য)) {
			/* They want us to terminate traversal */
			ret = গুনতি;
			যদি (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(চাবি.dptr);
			যাও out;
		}
		SAFE_FREE(চাবি.dptr);
	}
out:
	tdb->travlocks.পরবর্তী = tl->পরবর্তী;
	যদি (ret < 0)
		ফেরত -1;
	অন্যথা
		ফেরত গুনতি;
}


/*
  a write style traverse - temporarily marks the db read only
*/
পূর্ণসংখ্যা tdb_traverse_read(কাঠামো tdb_context *tdb,
		      tdb_traverse_func fn, শূন্য *private_তথ্য)
{
	কাঠামো tdb_traverse_lock tl = { ফাঁকা, 0, 0, F_RDLCK };
	পূর্ণসংখ্যা ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	যদি (tdb_transaction_lock(tdb, F_RDLCK)) {
		ফেরত -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_তথ্য, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ফেরত ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
পূর্ণসংখ্যা tdb_traverse(কাঠামো tdb_context *tdb,
		 tdb_traverse_func fn, শূন্য *private_তথ্য)
{
	কাঠামো tdb_traverse_lock tl = { ফাঁকা, 0, 0, F_WRLCK };
	পূর্ণসংখ্যা ret;

	যদি (tdb->read_only || tdb->traverse_read) {
		ফেরত tdb_traverse_read(tdb, fn, private_তথ্য);
	}

	যদি (tdb_transaction_lock(tdb, F_WRLCK)) {
		ফেরত -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_তথ্য, &tl);

	tdb_transaction_unlock(tdb);

	ফেরত ret;
}


/* find the first entry in the database and return its key */
TDB_তথ্য tdb_firstkey(কাঠামো tdb_context *tdb)
{
	TDB_তথ্য চাবি;
	কাঠামো list_struct rec;

	/* release any old lock */
	যদি (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		ফেরত tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	যদি (tdb_পরবর্তী_lock(tdb, &tdb->travlocks, &rec) <= 0)
		ফেরত tdb_null;
	/* now read the key */
	চাবি.dsize = rec.চাবি_len;
	চাবি.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+আকার(rec),চাবি.dsize);

	/* Unlock the hash chain of the record we just read. */
	যদি (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ফেরত চাবি;
}

/* find the next entry in the database, returning its key */
TDB_তথ্য tdb_nextkey(কাঠামো tdb_context *tdb, TDB_তথ্য oldkey)
{
	u32 oldhash;
	TDB_তথ্য চাবি = tdb_null;
	কাঠামো list_struct rec;
	unsigned অক্ষর *k = ফাঁকা;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	যদি (tdb->travlocks.off) {
		যদি (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			ফেরত tdb_null;
		যদি (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+আকার(rec),
					    rec.চাবি_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			যদি (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				ফেরত tdb_null;
			}
			যদি (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				ফেরত tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	যদি (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		যদি (!tdb->travlocks.off)
			ফেরত tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		যদি (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ফেরত tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	যদি (tdb_পরবর্তী_lock(tdb, &tdb->travlocks, &rec) > 0) {
		চাবি.dsize = rec.চাবি_len;
		চাবি.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+আকার(rec),
					  চাবি.dsize);
		/* Unlock the chain of this new record */
		যদি (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	যদি (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ফেরত চাবি;
}

/* file: dump.c */

স্থির tdb_off_t tdb_dump_record(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা hash,
				 tdb_off_t offset)
{
	কাঠামো list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	যদি (tdb->methods->tdb_read(tdb, offset, (অক্ষর *)&rec,
				   আকার(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		ফেরত 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.পরবর্তী, rec.rec_len, rec.চাবি_len, rec.তথ্য_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + আকার(rec) + rec.rec_len - আকার(tdb_off_t);

	যদি (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ফেরত rec.পরবর্তী;
	}

	যদি (tailer != rec.rec_len + আকার(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned পূর্ণসংখ্যা)tailer, (unsigned পূর্ণসংখ্যা)(rec.rec_len + আকার(rec)));
	}
	ফেরত rec.পরবর্তী;
}

স্থির পূর্ণসংখ্যা tdb_dump_chain(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা i)
{
	tdb_off_t rec_ptr, শীর্ষ;

	শীর্ষ = TDB_HASH_শীর্ষ(i);

	যদি (tdb_lock(tdb, i, F_WRLCK) != 0)
		ফেরত -1;

	যদি (tdb_ofs_read(tdb, শীর্ষ, &rec_ptr) == -1)
		ফেরত tdb_unlock(tdb, i, F_WRLCK);

	যদি (rec_ptr)
		printf("hash=%d\n", i);

	যখন (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	ফেরত tdb_unlock(tdb, i, F_WRLCK);
}

শূন্য tdb_dump_all(কাঠামো tdb_context *tdb)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < (পূর্ণসংখ্যা)tdb->header.hash_আকার; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

পূর্ণসংখ্যা tdb_printfreelist(কাঠামো tdb_context *tdb)
{
	পূর্ণসংখ্যা ret;
	long যোগ_free = 0;
	tdb_off_t offset, rec_ptr;
	কাঠামো list_struct rec;

	যদি ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		ফেরত ret;

	offset = FREELIST_শীর্ষ;

	/* read in the freelist top */
	যদি (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ফেরত 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	যখন (rec_ptr) {
		যদি (tdb->methods->tdb_read(tdb, rec_ptr, (অক্ষর *)&rec,
					   আকার(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ফেরত -1;
		}

		যদি (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ফেরত -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		যোগ_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.পরবর্তী;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (পূর্ণসংখ্যা)যোগ_free,
               (পূর্ণসংখ্যা)যোগ_free);

	ফেরত tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
শূন্য tdb_increment_seqnum_nonblock(কাঠামো tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	যদি (!(tdb->flags & TDB_SEQNUM)) {
		ফেরত;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	যদি (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ফেরত;
	seqnum++;
	(শূন্য) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
স্থির শূন্য tdb_increment_seqnum(কাঠামো tdb_context *tdb)
{
	যদি (!(tdb->flags & TDB_SEQNUM)) {
		ফেরত;
	}

	যদি (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ফেরত;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

স্থির পূর্ণসংখ্যা tdb_চাবি_compare(TDB_তথ্য চাবি, TDB_তথ্য তথ্য,
			   শূন্য *private_তথ্য EXT2FS_ATTR((unused)))
{
	ফেরত memcmp(তথ্য.dptr, চাবি.dptr, তথ্য.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
স্থির tdb_off_t tdb_find(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash,
			কাঠামো list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(hash), &rec_ptr) == -1)
		ফেরত 0;

	/* keep looking until we find the right record */
	যখন (rec_ptr) {
		যদি (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ফেরত 0;

		যদি (!TDB_DEAD(r) && hash==r->full_hash
		    && চাবি.dsize==r->চাবি_len
		    && tdb_parse_তথ্য(tdb, চাবি, rec_ptr + আকার(*r),
				      r->চাবি_len, tdb_চাবি_compare,
				      ফাঁকা) == 0) {
			ফেরত rec_ptr;
		}
		rec_ptr = r->পরবর্তী;
	}
	ফেরত TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash, পূর্ণসংখ্যা locktype,
			   কাঠামো list_struct *rec)
{
	u32 rec_ptr;

	যদি (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		ফেরত 0;
	যদি (!(rec_ptr = tdb_find(tdb, চাবি, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	ফেরত rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
স্থির পূর্ণসংখ্যা tdb_update_hash(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash, TDB_তথ্য dbuf)
{
	কাঠামো list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	যদি (!(rec_ptr = tdb_find(tdb, চাবি, hash, &rec)))
		ফেরত -1;

	/* must be long enough key, data and tailer */
	যদি (rec.rec_len < চাবি.dsize + dbuf.dsize + আকার(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ফেরত -1;
	}

	যদি (tdb->methods->tdb_write(tdb, rec_ptr + আকার(rec) + rec.চাবি_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ফেরত -1;

	যদি (dbuf.dsize != rec.তথ্য_len) {
		/* update size */
		rec.তথ্য_len = dbuf.dsize;
		ফেরত tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ফেরত 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_তথ্য tdb_fetch(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	tdb_off_t rec_ptr;
	কাঠামো list_struct rec;
	TDB_তথ্য ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&চাবি);
	যদি (!(rec_ptr = tdb_find_lock_hash(tdb,চাবি,hash,F_RDLCK,&rec)))
		ফেরত tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + আকার(rec) + rec.চাবি_len,
				  rec.তথ্য_len);
	ret.dsize = rec.তথ্য_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ফেরত ret;
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

পূর্ণসংখ্যা tdb_parse_record(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি,
		     পূর্ণসংখ্যা (*parser)(TDB_তথ্য চাবি, TDB_তথ্য তথ্য,
				   শূন্য *private_তথ্য),
		     শূন্য *private_তথ্য)
{
	tdb_off_t rec_ptr;
	কাঠামো list_struct rec;
	পূর্ণসংখ্যা ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&চাবি);

	যদি (!(rec_ptr = tdb_find_lock_hash(tdb,চাবি,hash,F_RDLCK,&rec))) {
		ফেরত TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_তথ্য(tdb, চাবি, rec_ptr + আকার(rec) + rec.চাবি_len,
			     rec.তথ্য_len, parser, private_তথ্য);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	ফেরত ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
স্থির পূর্ণসংখ্যা tdb_exists_hash(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash)
{
	কাঠামো list_struct rec;

	যদি (tdb_find_lock_hash(tdb, চাবি, hash, F_RDLCK, &rec) == 0)
		ফেরত 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ফেরত 1;
}

পূর্ণসংখ্যা tdb_exists(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	u32 hash = tdb->hash_fn(&চাবি);
	ফেরত tdb_exists_hash(tdb, চাবি, hash);
}

/* actually delete an entry in the database given the offset */
পূর্ণসংখ্যা tdb_do_delete(কাঠামো tdb_context *tdb, tdb_off_t rec_ptr, কাঠামো list_struct*rec)
{
	tdb_off_t last_ptr, i;
	কাঠামো list_struct lastrec;

	যদি (tdb->read_only || tdb->traverse_read) ফেরত -1;

	যদি (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ফেরত tdb_rec_write(tdb, rec_ptr, rec);
	}
	যদি (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		ফেরত -1;

	/* find previous record in hash chain */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(rec->full_hash), &i) == -1)
		ফেরত -1;
	জন্য (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.পরবর্তী)
		যদি (tdb_rec_read(tdb, i, &lastrec) == -1)
			ফেরত -1;

	/* unlink it: next ptr is at start of record. */
	যদি (last_ptr == 0)
		last_ptr = TDB_HASH_শীর্ষ(rec->full_hash);
	যদি (tdb_ofs_write(tdb, last_ptr, &rec->পরবর্তী) == -1)
		ফেরত -1;

	/* recover the space */
	যদি (tdb_free(tdb, rec_ptr, rec) == -1)
		ফেরত -1;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা tdb_গুনতি_dead(কাঠামো tdb_context *tdb, u32 hash)
{
	পূর্ণসংখ্যা res = 0;
	tdb_off_t rec_ptr;
	কাঠামো list_struct rec;

	/* read in the hash top */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(hash), &rec_ptr) == -1)
		ফেরত 0;

	যখন (rec_ptr) {
		যদি (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ফেরত 0;

		যদি (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.পরবর্তী;
	}
	ফেরত res;
}

/*
 * Purge all DEAD records from a hash chain
 */
স্থির পূর্ণসংখ্যা tdb_purge_dead(কাঠামো tdb_context *tdb, u32 hash)
{
	পূর্ণসংখ্যা res = -1;
	কাঠামো list_struct rec;
	tdb_off_t rec_ptr;

	যদি (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ফেরত -1;
	}

	/* read in the hash top */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(hash), &rec_ptr) == -1)
		যাও fail;

	যখন (rec_ptr) {
		tdb_off_t পরবর্তী;

		যদি (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			যাও fail;
		}

		পরবর্তী = rec.পরবর্তী;

		যদি (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			যাও fail;
		}
		rec_ptr = পরবর্তী;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ফেরত res;
}

/* delete an entry in the database given a key */
স্থির পূর্ণসংখ্যা tdb_delete_hash(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, u32 hash)
{
	tdb_off_t rec_ptr;
	কাঠামো list_struct rec;
	পূর্ণসংখ্যা ret;

	যদি (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		যদি (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			ফেরত -1;

		যদি (tdb_গুনতি_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		যদি (!(rec_ptr = tdb_find(tdb, চাবি, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			ফেরত -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	অন্যথা {
		যদি (!(rec_ptr = tdb_find_lock_hash(tdb, চাবি, hash, F_WRLCK,
						   &rec)))
			ফেরত -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	যদি (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	যদি (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ফেরত ret;
}

পূর্ণসংখ্যা tdb_delete(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি)
{
	u32 hash = tdb->hash_fn(&চাবি);
	ফেরত tdb_delete_hash(tdb, চাবি, hash);
}

/*
 * See if we have a dead record around with enough space
 */
স্থির tdb_off_t tdb_find_dead(কাঠামো tdb_context *tdb, u32 hash,
			       কাঠামো list_struct *r, tdb_len_t দৈর্ঘ্য)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(hash), &rec_ptr) == -1)
		ফেরত 0;

	/* keep looking until we find the right record */
	যখন (rec_ptr) {
		যদি (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ফেরত 0;

		যদি (TDB_DEAD(r) && r->rec_len >= দৈর্ঘ্য) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ফেরত rec_ptr;
		}
		rec_ptr = r->পরবর্তী;
	}
	ফেরত 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
পূর্ণসংখ্যা tdb_store(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, পূর্ণসংখ্যা flag)
{
	কাঠামো list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	অক্ষর *p = ফাঁকা;
	পূর্ণসংখ্যা ret = -1;

	যদি (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ফেরত -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&চাবি);
	যদি (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ফেরত -1;

	/* check for it existing, on insert. */
	যদি (flag == TDB_ঢোকাও) {
		যদি (tdb_exists_hash(tdb, চাবি, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			যাও fail;
		}
	} অন্যথা {
		/* first try in-place update, on modify or replace. */
		যদি (tdb_update_hash(tdb, চাবি, hash, dbuf) == 0) {
			যাও done;
		}
		যদি (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			যাও fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	যদি (flag != TDB_ঢোকাও)
		tdb_delete_hash(tdb, চাবি, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	যদি (!(p = (অক্ষর *)malloc(চাবি.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		যাও fail;
	}

	memcpy(p, চাবি.dptr, চাবি.dsize);
	যদি (dbuf.dsize)
		memcpy(p+চাবি.dsize, dbuf.dptr, dbuf.dsize);

	যদি (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			চাবি.dsize + dbuf.dsize + আকার(tdb_off_t));

		যদি (rec_ptr != 0) {
			rec.চাবি_len = চাবি.dsize;
			rec.তথ্য_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			যদি (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + আকার(rec),
				    p, চাবি.dsize + dbuf.dsize) == -1) {
				যাও fail;
			}
			যাও done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	যদি (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		যাও fail;
	}

	যদি ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		যাও fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, চাবি.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	যদি (rec_ptr == 0) {
		যাও fail;
	}

	/* Read hash top into next ptr */
	যদি (tdb_ofs_read(tdb, TDB_HASH_শীর্ষ(hash), &rec.পরবর্তী) == -1)
		যাও fail;

	rec.চাবি_len = চাবি.dsize;
	rec.তথ্য_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	যদি (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+আকার(rec), p, চাবি.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_শীর্ষ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		যাও fail;
	}

 done:
	ret = 0;
 fail:
	যদি (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	ফেরত ret;
}


/* Append to an entry. Create if not exist. */
পূর্ণসংখ্যা tdb_append(কাঠামো tdb_context *tdb, TDB_তথ্য চাবি, TDB_তথ্য new_dbuf)
{
	u32 hash;
	TDB_তথ্য dbuf;
	পূর্ণসংখ্যা ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&চাবি);
	যদি (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ফেরত -1;

	dbuf = tdb_fetch(tdb, চাবি);

	যদি (dbuf.dptr == ফাঁকা) {
		dbuf.dptr = (unsigned অক্ষর *)malloc(new_dbuf.dsize);
	} অন্যথা {
		unsigned অক্ষর *new_dptr = (unsigned অক্ষর *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		যদি (new_dptr == ফাঁকা) {
			মুক্ত(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	যদি (dbuf.dptr == ফাঁকা) {
		tdb->ecode = TDB_ERR_OOM;
		যাও failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, চাবি, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	ফেরত ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ধ্রুবক অক্ষর *tdb_name(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
পূর্ণসংখ্যা tdb_fd(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->log.log_fn;
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
পূর্ণসংখ্যা tdb_get_seqnum(কাঠামো tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	যদি (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ফেরত 0;
	ফেরত seqnum;
}

পূর্ণসংখ্যা tdb_hash_আকার(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->header.hash_আকার;
}

size_t tdb_map_আকার(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->map_আকার;
}

পূর্ণসংখ্যা tdb_get_flags(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
শূন্য tdb_enable_seqnum(কাঠামো tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
স্থির কাঠামো tdb_context *tdbs = ফাঁকা;


/* This is from a hash algorithm suggested by Rogier Wolff */
স্থির unsigned পূর্ণসংখ্যা default_tdb_hash(TDB_তথ্য *চাবি)
{
	u32 মান;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	জন্য (মান = 0, i=0; i < চাবি->dsize; i++)
		মান = মান * 256 + চাবি->dptr[i] + (মান >> 24) * 241;

	ফেরত মান;
}


/* initialise a new database with a specified hash size */
স্থির পূর্ণসংখ্যা tdb_new_database(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা hash_আকার)
{
	কাঠামো tdb_header *newdb;
	পূর্ণসংখ্যা আকার_2, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	আকার_2 = আকার(কাঠামো tdb_header) + (hash_আকার+1)*আকার(tdb_off_t);
	যদি (!(newdb = (কাঠামো tdb_header *)calloc(আকার_2, 1)))
		ফেরত TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_আকার = hash_আকার;
	যদি (tdb->flags & TDB_INTERNAL) {
		tdb->map_আকার = আকার_2;
		tdb->map_ptr = (অক্ষর *)newdb;
		memcpy(&tdb->header, newdb, আকার(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ফেরত 0;
	}
	যদি (lseek(tdb->fd, 0, SEEK_SET) == -1)
		যাও fail;

	যদি (ftruncate(tdb->fd, 0) == -1)
		যাও fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, আকার(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	যদি (লিখ(tdb->fd, newdb, আকার_2) != আকার_2) {
		ret = -1;
	} অন্যথা {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	ফেরত ret;
}



স্থির পূর্ণসংখ্যা tdb_already_open(dev_t device,
			    ino_t ino)
{
	কাঠামো tdb_context *i;

	জন্য (i = tdbs; i; i = i->পরবর্তী) {
		যদি (i->device == device && i->inode == ino) {
			ফেরত 1;
		}
	}

	ফেরত 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
কাঠামো tdb_context *tdb_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা hash_আকার, পূর্ণসংখ্যা tdb_flags,
		      পূর্ণসংখ্যা open_flags, mode_t mode)
{
	ফেরত tdb_open_ex(name, hash_আকার, tdb_flags, open_flags, mode, ফাঁকা, ফাঁকা);
}

/* a default logging function */
স্থির শূন্য null_log_fn(কাঠামো tdb_context *tdb, গণনা tdb_debug_level level, ধ্রুবক অক্ষর *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
স্থির শূন্য null_log_fn(কাঠামো tdb_context *tdb EXT2FS_ATTR((unused)),
			গণনা tdb_debug_level level EXT2FS_ATTR((unused)),
			ধ্রুবক অক্ষর *fmt EXT2FS_ATTR((unused)), ...)
{
}


কাঠামো tdb_context *tdb_open_ex(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা hash_আকার, পূর্ণসংখ্যা tdb_flags,
				পূর্ণসংখ্যা open_flags, mode_t mode,
				ধ্রুবক কাঠামো tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	কাঠামো tdb_context *tdb;
	কাঠামো stat st;
	পূর্ণসংখ্যা rev = 0, locked = 0;
	unsigned অক্ষর *vp;
	u32 vertest;

	যদি (!(tdb = (কাঠামো tdb_context *)calloc(1, আকার *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		যাও fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ফাঁকা;
	tdb->map_ptr = ফাঁকা;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	যদি (log_ctx) {
		tdb->log = *log_ctx;
	} অন্যথা {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = ফাঁকা;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_আকার = sysconf(_SC_PAGESIZE);
	যদি (tdb->page_আকার <= 0) {
		tdb->page_আকার = 0x2000;
	}

	যদি ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		যাও fail;
	}

	যদি (hash_আকার == 0)
		hash_আকার = DEFAULT_HASH_আকার;
	যদি ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	যদি (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		যদি (tdb_new_database(tdb, hash_আকার) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			যাও fail;
		}
		যাও internal;
	}

	যদি ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		যাও fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	যদি (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		যাও fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	যদি ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		যদি (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			যাও fail; /* errno set by ftruncate */
		}
	}

	যদি (পঠ(tdb->fd, &tdb->header, আকার(tdb->header)) != আকার(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      আকার(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		যদি (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_আকার) == -1) {
			errno = EIO; /* ie bad format or something */
			যাও fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned অক্ষর *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	যদি (!rev)
		tdb->flags &= ~TDB_CONVERT;
	অন্যথা {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, আকার(tdb->header));
	}
	যদি (fstat(tdb->fd, &st) == -1)
		যাও fail;

	যদি (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		যাও fail;
	}

	/* Is it already in the open list?  If so, fail. */
	যদি (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (পূর্ণসংখ্যা)st.st_dev, (পূর্ণসংখ্যা)st.st_ino));
		errno = EBUSY;
		যাও fail;
	}

	যদি (!(tdb->name = (অক্ষর *)strdup(name))) {
		errno = ENOMEM;
		যাও fail;
	}

	tdb->map_আকার = st.st_আকার;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	যদি (locked) {
		যদি (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			যাও fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	যদি (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		যদি (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			যাও fail;
	}

	/* if needed, run recovery */
	যদি (tdb_transaction_recover(tdb) == -1) {
		যাও fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	যদি (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		যাও fail;
	tdb->পরবর্তী = tdbs;
	tdbs = tdb;
	ফেরত tdb;

 fail:
	{ পূর্ণসংখ্যা save_errno = errno;

	যদি (!tdb)
		ফেরত ফাঁকা;

	যদি (tdb->map_ptr) {
		যদি (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		অন্যথা
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	যদি (tdb->fd != -1)
		যদি (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	ফেরত ফাঁকা;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

শূন্য tdb_set_max_dead(কাঠামো tdb_context *tdb, পূর্ণসংখ্যা max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
পূর্ণসংখ্যা tdb_close(কাঠামো tdb_context *tdb)
{
	কাঠামো tdb_context **i;
	পূর্ণসংখ্যা ret = 0;

	যদি (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	যদি (tdb->map_ptr) {
		যদি (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		অন্যথা
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	যদি (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	জন্য (i = &tdbs; *i; i = &(*i)->পরবর্তী) {
		যদি (*i == tdb) {
			*i = tdb->পরবর্তী;
			বিরতি;
		}
	}

	memset(tdb, 0, আকার(*tdb));
	SAFE_FREE(tdb);

	ফেরত ret;
}

/* register a logging function */
শূন্য tdb_set_logging_function(কাঠামো tdb_context *tdb,
                              ধ্রুবক কাঠামো tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

শূন্য *tdb_get_logging_private(কাঠামো tdb_context *tdb)
{
	ফেরত tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
পূর্ণসংখ্যা tdb_reopen(কাঠামো tdb_context *tdb)
{
	কাঠামো stat st;

	যদি (tdb->flags & TDB_INTERNAL) {
		ফেরত 0; /* Nothing to do. */
	}

	যদি (tdb->অঙ্ক_locks != 0 || tdb->global_lock.গুনতি) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		যাও fail;
	}

	যদি (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		যাও fail;
	}

	যদি (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		যাও fail;
	}
	যদি (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	যদি (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		যাও fail;
	}
	যদি ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		যাও fail;
	}
	যদি (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		যাও fail;
	}
	যদি (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		যাও fail;
	}
	tdb_mmap(tdb);

	ফেরত 0;

fail:
	tdb_close(tdb);
	ফেরত -1;
}

/* reopen all tdb's */
পূর্ণসংখ্যা tdb_reopen_all(পূর্ণসংখ্যা parent_longlived)
{
	কাঠামো tdb_context *tdb;

	জন্য (tdb=tdbs; tdb; tdb = tdb->পরবর্তী) {
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
		যদি (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		যদি (tdb_reopen(tdb) != 0)
			ফেরত -1;
	}

	ফেরত 0;
}

/**
 * Flush a database file from the page cache.
 **/
পূর্ণসংখ্যা tdb_flush(কাঠামো tdb_context *tdb)
{
	যদি (tdb->fd != -1)
		ফেরত fsync(tdb->fd);
	ফেরত 0;
}
