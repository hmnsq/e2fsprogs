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

#ضمن "config.h"
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <stdarg.h>
#ضمن <stddef.h>
#ضمن <errno.h>
#ضمن <string.h>
#ifdef HAVE_SYS_SELECT_H
#ضمن <sys/select.h>
#endif
#ضمن <sys/time.h>
#ضمن <sys/types.h>
#ضمن <time.h>
#ifdef HAVE_UTIME_H
#ضمن <utime.h>
#endif
#ضمن <sys/stat.h>
#ضمن <sys/file.h>
#ضمن <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#ضمن <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#وإلا
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((فراغ *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
ساكن حرف *rep_strdup(ثابت حرف *s)
{
	حرف *ret;
	صحيح length;

	إذا (!s)
		أرجع عدم;
	length = strlen(s);
	ret = malloc(length + 1);
	إذا (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	أرجع ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#إذا defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
تعريفنوع صحيح bool;
#endif

#ضمن "tdb.h"

ساكن TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

تعريفنوع u32 tdb_len_t;
تعريفنوع u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned صحيح)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*حجم(بنية list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_لووتکە (حجم(بنية tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_لووتکە(hash) (FREELIST_لووتکە + (BUCKET(hash)+1)*حجم(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*حجم(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_لووتکە(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(بنية tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(بنية tdb_header, sequence_ژمار)
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
#define SAFE_FREE(x) افعل { إذا ((x) != عدم) {حرر(x); (x)=عدم;} } طالما(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, حجم(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
بنية list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t کلیل_len; /* byte length of key */
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
بنية tdb_header {
	حرف magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_ژمار; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

بنية tdb_lock_type {
	صحيح list;
	u32 ژماردن;
	u32 ltype;
};

بنية tdb_traverse_lock {
	بنية tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	صحيح lock_rw;
};


بنية tdb_methods {
	صحيح (*tdb_read)(بنية tdb_context *, tdb_off_t , فراغ *, tdb_len_t , صحيح );
	صحيح (*tdb_write)(بنية tdb_context *, tdb_off_t, ثابت فراغ *, tdb_len_t);
	فراغ (*next_hash_chain)(بنية tdb_context *, u32 *);
	صحيح (*tdb_oob)(بنية tdb_context *, tdb_off_t , صحيح );
	صحيح (*tdb_expand_file)(بنية tdb_context *, tdb_off_t , tdb_off_t );
	صحيح (*tdb_brlock)(بنية tdb_context *, tdb_off_t , صحيح, صحيح, صحيح, size_t);
};

بنية tdb_context {
	حرف *name; /* the name of the database */
	فراغ *map_ptr; /* where it is currently mapped */
	صحيح fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	صحيح read_only; /* opened read-only */
	صحيح traverse_read; /* read-only traversal */
	بنية tdb_lock_type global_lock;
	صحيح ژمارە_lockrecs;
	بنية tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	تعداد TDB_ERROR ecode; /* error code for last tdb error */
	بنية tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	بنية tdb_traverse_lock travlocks; /* current traversal locks */
	بنية tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	بنية tdb_logging_context log;
	unsigned صحيح (*hash_fn)(TDB_DATA *کلیل);
	صحيح open_flags; /* flags used in the open - needed by reopen */
	unsigned صحيح ژمارە_locks; /* number of chain locks held */
	ثابت بنية tdb_methods *methods;
	بنية tdb_transaction *transaction;
	صحيح page_size;
	صحيح max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ساكن صحيح tdb_munmap(بنية tdb_context *tdb);
ساكن فراغ tdb_mmap(بنية tdb_context *tdb);
ساكن صحيح tdb_lock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
صحيح tdb_lock_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t offset, صحيح rw_type, صحيح lck_type, صحيح probe, size_t len);
ساكن صحيح tdb_transaction_lock(بنية tdb_context *tdb, صحيح ltype);
ساكن صحيح tdb_transaction_unlock(بنية tdb_context *tdb);
ساكن صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t offset, size_t len);
ساكن صحيح tdb_write_lock_record(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_write_unlock_record(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ساكن فراغ *tdb_convert(فراغ *buf, u32 size);
ساكن صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec);
ساكن tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t length, بنية list_struct *rec);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ساكن صحيح tdb_lock_record(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_unlock_record(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec);
ساكن صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec);
ساكن صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct *rec);
ساكن unsigned حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
ساكن صحيح tdb_parse_data(بنية tdb_context *tdb, TDB_DATA کلیل,
		   tdb_off_t offset, tdb_len_t len,
		   صحيح (*parser)(TDB_DATA کلیل, TDB_DATA data,
				 فراغ *private_data),
		   فراغ *private_data);
ساكن tdb_off_t tdb_find_lock_hash(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash, صحيح locktype,
			   بنية list_struct *rec);
ساكن فراغ tdb_io_init(بنية tdb_context *tdb);
ساكن صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t size);
ساكن صحيح tdb_rec_free_read(بنية tdb_context *tdb, tdb_off_t off,
		      بنية list_struct *rec);


/* file: error.c */

تعداد TDB_ERROR tdb_error(بنية tdb_context *tdb)
{
	أرجع tdb->ecode;
}

ساكن بنية tdb_errname {
	تعداد TDB_ERROR ecode; ثابت حرف *estring;
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
ثابت حرف *tdb_errorstr(بنية tdb_context *tdb)
{
	u32 i;
	لكل (i = 0; i < حجم(emap) / حجم(بنية tdb_errname); i++)
		إذا (tdb->ecode == emap[i].ecode)
			أرجع emap[i].estring;
	أرجع "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t offset,
	       صحيح rw_type, صحيح lck_type, صحيح probe, size_t len)
{
	بنية flock fl;
	صحيح ret;

	إذا (tdb->flags & TDB_NOLOCK) {
		أرجع 0;
	}

	إذا ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	افعل {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} طالما (ret == -1 && errno == EINTR);

	إذا (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		إذا (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (صحيح)len));
		}
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	أرجع 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t offset, size_t len)
{
	صحيح ژماردن = 1000;
	طالما (ژماردن--) {
		بنية timeval tv;
		إذا (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			أرجع 0;
		}
		إذا (errno != EDEADLK) {
			اكسر;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, عدم, عدم, عدم, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	أرجع -1;
}


/* lock a list in the database. list -1 is the alloc list */
ساكن صحيح _tdb_lock(بنية tdb_context *tdb, صحيح list, صحيح ltype, صحيح op)
{
	بنية tdb_lock_type *new_lck;
	صحيح i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_lock.ژماردن &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_lock.ژماردن) {
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (list < -1 || list >= (صحيح)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		أرجع -1;
	}
	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	لكل (i=0; i<tdb->ژمارە_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			إذا (tdb->lockrecs[i].ژماردن == 0) {
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
			tdb->lockrecs[i].ژماردن++;
			أرجع 0;
		}
	}

	new_lck = (بنية tdb_lock_type *)realloc(
		tdb->lockrecs,
		حجم(*tdb->lockrecs) * (tdb->ژمارە_lockrecs+1));
	إذا (new_lck == عدم) {
		errno = ENOMEM;
		أرجع -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	إذا (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_لووتکە+4*list, ltype, op,
				     0, 1)) {
		أرجع -1;
	}

	tdb->ژمارە_locks++;

	tdb->lockrecs[tdb->ژمارە_lockrecs].list = list;
	tdb->lockrecs[tdb->ژمارە_lockrecs].ژماردن = 1;
	tdb->lockrecs[tdb->ژمارە_lockrecs].ltype = ltype;
	tdb->ژمارە_lockrecs += 1;

	أرجع 0;
}

/* lock a list in the database. list -1 is the alloc list */
صحيح tdb_lock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	إذا (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	أرجع ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
صحيح tdb_lock_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	أرجع _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret = -1;
	صحيح i;
	بنية tdb_lock_type *lck = عدم;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_lock.ژماردن &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_lock.ژماردن) {
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	/* Sanity checks */
	إذا (list < -1 || list >= (صحيح)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		أرجع ret;
	}

	لكل (i=0; i<tdb->ژمارە_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			اكسر;
		}
	}

	إذا ((lck == عدم) || (lck->ژماردن == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		أرجع -1;
	}

	إذا (lck->ژماردن > 1) {
		lck->ژماردن--;
		أرجع 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	إذا (mark_lock) {
		ret = 0;
	} وإلا {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_لووتکە+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->ژمارە_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	إذا (tdb->ژمارە_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->ژمارە_lockrecs-1];
	}
	tdb->ژمارە_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	إذا (tdb->ژمارە_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	إذا (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	أرجع ret;
}

/*
  get the transaction lock
 */
صحيح tdb_transaction_lock(بنية tdb_context *tdb, صحيح ltype)
{
	إذا (tdb->have_transaction_lock || tdb->global_lock.ژماردن) {
		أرجع 0;
	}
	إذا (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		أرجع -1;
	}
	tdb->have_transaction_lock = 1;
	أرجع 0;
}

/*
  release the transaction lock
 */
صحيح tdb_transaction_unlock(بنية tdb_context *tdb)
{
	صحيح ret;
	إذا (!tdb->have_transaction_lock) {
		أرجع 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	إذا (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	أرجع ret;
}




/* lock/unlock entire database */
ساكن صحيح _tdb_lockall(بنية tdb_context *tdb, صحيح ltype, صحيح op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read)
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);

	إذا (tdb->global_lock.ژماردن && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.ژماردن++;
		أرجع 0;
	}

	إذا (tdb->global_lock.ژماردن) {
		/* a global lock of a different type exists */
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (tdb->ژمارە_locks != 0) {
		/* can't combine global and chain locks */
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_لووتکە, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		إذا (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		أرجع -1;
	}

	tdb->global_lock.ژماردن = 1;
	tdb->global_lock.ltype = ltype;

	أرجع 0;
}



/* unlock entire db */
ساكن صحيح _tdb_unlockall(بنية tdb_context *tdb, صحيح ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.ژماردن == 0) {
		أرجع TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	إذا (tdb->global_lock.ژماردن > 1) {
		tdb->global_lock.ژماردن--;
		أرجع 0;
	}

	إذا (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_لووتکە, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		أرجع -1;
	}

	tdb->global_lock.ژماردن = 0;
	tdb->global_lock.ltype = 0;

	أرجع 0;
}

/* lock entire database with write lock */
صحيح tdb_lockall(بنية tdb_context *tdb)
{
	أرجع _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
صحيح tdb_lockall_mark(بنية tdb_context *tdb)
{
	أرجع _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
صحيح tdb_lockall_unmark(بنية tdb_context *tdb)
{
	أرجع _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
صحيح tdb_lockall_nonblock(بنية tdb_context *tdb)
{
	أرجع _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
صحيح tdb_unlockall(بنية tdb_context *tdb)
{
	أرجع _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
صحيح tdb_lockall_read(بنية tdb_context *tdb)
{
	أرجع _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
صحيح tdb_lockall_read_nonblock(بنية tdb_context *tdb)
{
	أرجع _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
صحيح tdb_unlockall_read(بنية tdb_context *tdb)
{
	أرجع _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_WRLCK | TDB_MARK_LOCK);
}

صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_WRLCK);
}

صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_lock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_RDLCK);
}

صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	أرجع tdb_unlock(tdb, BUCKET(tdb->hash_fn(&کلیل)), F_RDLCK);
}



/* record lock stops delete underneath */
صحيح tdb_lock_record(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_lock_record(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_lock *i;
	لكل (i = &tdb->travlocks; i; i = i->next)
		إذا (i->off == off)
			أرجع -1;
	أرجع tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_unlock_record(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
صحيح tdb_unlock_record(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_lock *i;
	u32 ژماردن = 0;

	إذا (off == 0)
		أرجع 0;
	لكل (i = &tdb->travlocks; i; i = i->next)
		إذا (i->off == off)
			ژماردن++;
	أرجع (ژماردن == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ساكن صحيح tdb_oob(بنية tdb_context *tdb, tdb_off_t len, صحيح probe)
{
	بنية stat st;
	إذا (len <= tdb->map_size)
		أرجع 0;
	إذا (tdb->flags & TDB_INTERNAL) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (صحيح)len, (صحيح)tdb->map_size));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (fstat(tdb->fd, &st) == -1) {
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (st.st_size < (off_t)len) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (صحيح)len, (صحيح)st.st_size));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	إذا (tdb_munmap(tdb) == -1)
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	أرجع 0;
}

/* write a lump of data at a specified offset */
ساكن صحيح tdb_write(بنية tdb_context *tdb, tdb_off_t off,
		     ثابت فراغ *buf, tdb_len_t len)
{
	إذا (len == 0) {
		أرجع 0;
	}

	إذا (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	إذا (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		أرجع -1;

	إذا (tdb->map_ptr) {
		memcpy(off + (حرف *)tdb->map_ptr, buf, len);
	} وإلا إذا (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	أرجع 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
فراغ *tdb_convert(فراغ *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	لكل (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	أرجع buf;
}


/* read a lump of data at a specified offset, maybe convert */
ساكن صحيح tdb_read(بنية tdb_context *tdb, tdb_off_t off, فراغ *buf,
		    tdb_len_t len, صحيح cv)
{
	إذا (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		أرجع -1;
	}

	إذا (tdb->map_ptr) {
		memcpy(buf, off + (حرف *)tdb->map_ptr, len);
	} وإلا {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		إذا (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (صحيح)off, (صحيح)len, (صحيح)ret, strerror(errno),
				 (صحيح)tdb->map_size));
			أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	إذا (cv) {
		tdb_convert(buf, len);
	}
	أرجع 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ساكن فراغ tdb_next_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	إذا (tdb->map_ptr) {
		لكل (;h < tdb->header.hash_size;h++) {
			إذا (0 != *(u32 *)(TDB_HASH_لووتکە(h) + (unsigned حرف *)tdb->map_ptr)) {
				اكسر;
			}
		}
	} وإلا {
		u32 off=0;
		لكل (;h < tdb->header.hash_size;h++) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(h), &off) != 0 || off != 0) {
				اكسر;
			}
		}
	}
	(*chain) = h;
}


صحيح tdb_munmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_INTERNAL)
		أرجع 0;

#ifdef HAVE_MMAP
	إذا (tdb->map_ptr) {
		صحيح ret = munmap(tdb->map_ptr, tdb->real_map_size);
		إذا (ret != 0)
			أرجع ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = عدم;
	أرجع 0;
}

فراغ tdb_mmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_INTERNAL)
		أرجع;

#ifdef HAVE_MMAP
	إذا (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(عدم, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		إذا (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = عدم;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} وإلا {
		tdb->map_ptr = عدم;
	}
#وإلا
	tdb->map_ptr = عدم;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ساكن صحيح tdb_expand_file(بنية tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	حرف buf[1024];

	إذا (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	إذا (ftruncate(tdb->fd, size+addition) == -1) {
		حرف b = 0;
		إذا (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			أرجع -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, حجم(buf));
	طالما (addition) {
		صحيح n = addition>حجم(buf)?حجم(buf):addition;
		صحيح ret = pwrite(tdb->fd, buf, n, size);
		إذا (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			أرجع -1;
		}
		addition -= n;
		size += n;
	}
	أرجع 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t size)
{
	بنية list_struct rec;
	tdb_off_t offset;

	إذا (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		أرجع -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	إذا (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	إذا (!(tdb->flags & TDB_INTERNAL)) {
		إذا (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			اذهب fail;
	}

	tdb->map_size += size;

	إذا (tdb->flags & TDB_INTERNAL) {
		حرف *new_map_ptr = (حرف *)realloc(tdb->map_ptr,
						    tdb->map_size);
		إذا (!new_map_ptr) {
			tdb->map_size -= size;
			اذهب fail;
		}
		tdb->map_ptr = new_map_ptr;
	} وإلا {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',حجم(rec));
	rec.rec_len = size - حجم(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	إذا (tdb_free(tdb, offset, &rec) == -1)
		اذهب fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع -1;
}

/* read/write a tdb_off_t */
صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	أرجع tdb->methods->tdb_read(tdb, offset, (حرف*)d, حجم(*d), DOCONV());
}

صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	أرجع tdb->methods->tdb_write(tdb, offset, CONVERT(off), حجم(*d));
}


/* read a lump of data, allocating the space for it */
unsigned حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned حرف *buf;

	/* some systems don't like zero length malloc */
	إذا (len == 0) {
		len = 1;
	}

	إذا (!(buf = (unsigned حرف *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		أرجع TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	إذا (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		أرجع عدم;
	}
	أرجع buf;
}

/* Give a piece of tdb data to a parser */

صحيح tdb_parse_data(بنية tdb_context *tdb, TDB_DATA کلیل,
		   tdb_off_t offset, tdb_len_t len,
		   صحيح (*parser)(TDB_DATA کلیل, TDB_DATA data,
				 فراغ *private_data),
		   فراغ *private_data)
{
	TDB_DATA data;
	صحيح result;

	data.dsize = len;

	إذا ((tdb->transaction == عدم) && (tdb->map_ptr != عدم)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		إذا (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			أرجع -1;
		}
		data.dptr = offset + (unsigned حرف *)tdb->map_ptr;
		أرجع parser(کلیل, data, private_data);
	}

	إذا (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		أرجع -1;
	}

	result = parser(کلیل, data, private_data);
	حرر(data.dptr);
	أرجع result;
}

/* read/write a record */
صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec)
{
	إذا (tdb->methods->tdb_read(tdb, offset, rec, حجم(*rec),DOCONV()) == -1)
		أرجع -1;
	إذا (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	أرجع tdb->methods->tdb_oob(tdb, rec->next+حجم(*rec), 0);
}

صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec)
{
	بنية list_struct r = *rec;
	أرجع tdb->methods->tdb_write(tdb, offset, CONVERT(r), حجم(r));
}

ساكن ثابت بنية tdb_methods io_methods = {
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
فراغ tdb_io_init(بنية tdb_context *tdb)
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

بنية tdb_transaction_el {
	بنية tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned حرف *data;
};

/*
  hold the context of any current transaction
*/
بنية tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ثابت بنية tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	بنية tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	صحيح transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	صحيح nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ساكن صحيح transaction_read(بنية tdb_context *tdb, tdb_off_t off, فراغ *buf,
			    tdb_len_t len, صحيح cv)
{
	بنية tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	لكل (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		إذا (off+len <= el->offset) {
			استمر;
		}
		إذا (off >= el->offset + el->length) {
			استمر;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		إذا (off < el->offset) {
			partial = el->offset - off;
			إذا (transaction_read(tdb, off, buf, partial, cv) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (فراغ *)(partial + (حرف *)buf);
		}
		إذا (off + len <= el->offset + el->length) {
			partial = len;
		} وإلا {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		إذا (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (فراغ *)(partial + (حرف *)buf);

		إذا (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			اذهب fail;
		}

		أرجع 0;
	}

	/* its not in the transaction elements - do a real read */
	أرجع tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	أرجع -1;
}


/*
  write while in a transaction
*/
ساكن صحيح transaction_write(بنية tdb_context *tdb, tdb_off_t off,
			     ثابت فراغ *buf, tdb_len_t len)
{
	بنية tdb_transaction_el *el, *best_el=عدم;

	إذا (len == 0) {
		أرجع 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	إذا (len == حجم(tdb_off_t) && off >= FREELIST_لووتکە &&
	    off < FREELIST_لووتکە+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_لووتکە) / حجم(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	لكل (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		إذا (best_el == عدم && off == el->offset+el->length) {
			best_el = el;
		}

		إذا (off+len <= el->offset) {
			استمر;
		}
		إذا (off >= el->offset + el->length) {
			استمر;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		إذا (off < el->offset) {
			partial = el->offset - off;
			إذا (transaction_write(tdb, off, buf, partial) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);
		}
		إذا (off + len <= el->offset + el->length) {
			partial = len;
		} وإلا {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);

		إذا (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			اذهب fail;
		}

		أرجع 0;
	}

	/* see if we can append the new entry to an existing entry */
	إذا (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned حرف *data = best_el->data;
		el = best_el;
		el->data = (unsigned حرف *)realloc(el->data,
						    el->length + len);
		إذا (el->data == عدم) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			أرجع -1;
		}
		إذا (buf) {
			memcpy(el->data + el->length, buf, len);
		} وإلا {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		أرجع 0;
	}

	/* add a new entry at the end of the list */
	el = (بنية tdb_transaction_el *)malloc(حجم(*el));
	إذا (el == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	el->next = عدم;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned حرف *)malloc(len);
	إذا (el->data == عدم) {
		حرر(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	إذا (buf) {
		memcpy(el->data, buf, len);
	} وإلا {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	إذا (el->prev) {
		el->prev->next = el;
	} وإلا {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	أرجع 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	أرجع -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ساكن فراغ transaction_next_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	لكل (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		إذا (0 != tdb->transaction->hash_heads[h+1]) {
			اكسر;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ساكن صحيح transaction_oob(بنية tdb_context *tdb, tdb_off_t len,
			   صحيح probe EXT2FS_ATTR((unused)))
{
	إذا (len <= tdb->map_size) {
		أرجع 0;
	}
	أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ساكن صحيح transaction_expand_file(بنية tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	إذا (transaction_write(tdb, size, عدم, addition) != 0) {
		أرجع -1;
	}

	أرجع 0;
}

/*
  brlock during a transaction - ignore them
*/
ساكن صحيح transaction_brlock(بنية tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      صحيح rw_type EXT2FS_ATTR((unused)),
			      صحيح lck_type EXT2FS_ATTR((unused)),
			      صحيح probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	أرجع 0;
}

ساكن ثابت بنية tdb_methods transaction_methods = {
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
صحيح tdb_transaction_start(بنية tdb_context *tdb)
{
	/* some sanity checks */
	إذا (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		أرجع -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	إذا (tdb->transaction != عدم) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		أرجع 0;
	}

	إذا (tdb->ژمارە_locks != 0 || tdb->global_lock.ژماردن) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		أرجع -1;
	}

	إذا (tdb->travlocks.next != عدم) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		أرجع -1;
	}

	tdb->transaction = (بنية tdb_transaction *)
		calloc(حجم(بنية tdb_transaction), 1);
	إذا (tdb->transaction == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	إذا (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		أرجع -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	إذا (tdb_brlock(tdb, FREELIST_لووتکە, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		اذهب fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, حجم(u32));
	إذا (tdb->transaction->hash_heads == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}
	إذا (tdb->methods->tdb_read(tdb, FREELIST_لووتکە, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		اذهب fail;
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
	إذا (transaction_write(tdb, FREELIST_لووتکە, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		اذهب fail;
	}

	أرجع 0;

fail:
	tdb_brlock(tdb, FREELIST_لووتکە, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	أرجع -1;
}


/*
  cancel the current transaction
*/
صحيح tdb_transaction_cancel(بنية tdb_context *tdb)
{
	إذا (tdb->transaction == عدم) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		أرجع -1;
	}

	إذا (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		أرجع 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		حرر(el->data);
		حرر(el);
	}

	/* remove any global lock created during the transaction */
	إذا (tdb->global_lock.ژماردن != 0) {
		tdb_brlock(tdb, FREELIST_لووتکە, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.ژماردن = 0;
	}

	/* remove any locks created during the transaction */
	إذا (tdb->ژمارە_locks != 0) {
		صحيح i;
		لكل (i=0;i<tdb->ژمارە_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_لووتکە+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->ژمارە_locks = 0;
		tdb->ژمارە_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_لووتکە, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	أرجع 0;
}

/*
  sync to disk
*/
ساكن صحيح transaction_sync(بنية tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	إذا (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		أرجع -1;
	}
#إذا defined(HAVE_MSYNC) && defined(MS_SYNC)
	إذا (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		إذا (msync(moffset + (حرف *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			أرجع -1;
		}
	}
#endif
	أرجع 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ساكن tdb_len_t tdb_recovery_size(بنية tdb_context *tdb)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = حجم(u32);
	لكل (el=tdb->transaction->elements;el;el=el->next) {
		إذا (el->offset >= tdb->transaction->old_map_size) {
			استمر;
		}
		recovery_size += 2*حجم(tdb_off_t) + el->length;
	}

	أرجع recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ساكن صحيح tdb_recovery_allocate(بنية tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	بنية list_struct rec;
	ثابت بنية tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	إذا (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		أرجع -1;
	}

	rec.rec_len = 0;

	إذا (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, حجم(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		أرجع -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	إذا (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		أرجع 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	إذا (recovery_head != 0) {
		إذا (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			أرجع -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(حجم(rec) + *recovery_size, tdb->page_size) - حجم(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     حجم(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		أرجع -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	إذا (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, حجم(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		أرجع -1;
	}

	أرجع 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ساكن صحيح transaction_setup_recovery(بنية tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned حرف *data, *p;
	ثابت بنية tdb_methods *methods = tdb->transaction->io_methods;
	بنية list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	إذا (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		أرجع -1;
	}

	data = (unsigned حرف *)malloc(recovery_size + حجم(*rec));
	إذا (data == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	rec = (بنية list_struct *)data;
	memset(rec, 0, حجم(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->کلیل_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + حجم(*rec);
	لكل (el=tdb->transaction->elements;el;el=el->next) {
		إذا (el->offset >= old_map_size) {
			استمر;
		}
		إذا (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			حرر(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			أرجع -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		إذا (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			حرر(data);
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = حجم(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	إذا (methods->tdb_write(tdb, recovery_offset, data, حجم(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		حرر(data);
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	إذا (transaction_sync(tdb, recovery_offset, حجم(*rec) + recovery_size) == -1) {
		حرر(data);
		أرجع -1;
	}

	حرر(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(بنية list_struct, magic);

	إذا (methods->tdb_write(tdb, *magic_offset, &magic, حجم(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* ensure the recovery magic marker is on disk */
	إذا (transaction_sync(tdb, *magic_offset, حجم(magic)) == -1) {
		أرجع -1;
	}

	أرجع 0;
}

/*
  commit the current transaction
*/
صحيح tdb_transaction_commit(بنية tdb_context *tdb)
{
	ثابت بنية tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	إذا (tdb->transaction == عدم) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		أرجع -1;
	}

	إذا (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		أرجع -1;
	}

	إذا (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		أرجع 0;
	}

	/* check for a null transaction */
	إذا (tdb->transaction->elements == عدم) {
		tdb_transaction_cancel(tdb);
		أرجع 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	إذا (tdb->ژمارە_locks || tdb->global_lock.ژماردن) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	إذا (tdb_brlock_upgrade(tdb, FREELIST_لووتکە, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	إذا (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		إذا (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
	}

	/* expand the file to the new size if needed */
	إذا (tdb->map_size != tdb->transaction->old_map_size) {
		إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;

		إذا (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			أرجع -1;
		}
		tdb->transaction->elements = el->next;
		حرر(el->data);
		حرر(el);
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		إذا (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			أرجع -1;
		}

		/* remove the recovery marker */
		إذا (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			أرجع -1;
		}

		/* ensure the recovery marker has been removed on disk */
		إذا (transaction_sync(tdb, magic_offset, 4) == -1) {
			أرجع -1;
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
	utime(tdb->name, عدم);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	أرجع 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
صحيح tdb_transaction_recover(بنية tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned حرف *data, *p;
	u32 zero = 0;
	بنية list_struct rec;

	/* find the recovery area */
	إذا (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	إذا (recovery_head == 0) {
		/* we have never allocated a recovery record */
		أرجع 0;
	}

	/* read the recovery record */
	إذا (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   حجم(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	إذا (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		أرجع 0;
	}

	إذا (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		أرجع -1;
	}

	recovery_eof = rec.کلیل_len;

	data = (unsigned حرف *)malloc(rec.data_len);
	إذا (data == عدم) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* read the full recovery data */
	إذا (tdb->methods->tdb_read(tdb, recovery_head + حجم(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		حرر(data);
		أرجع -1;
	}

	/* recover the file data */
	p = data;
	طالما (p+8 < data + rec.data_len) {
		u32 ofs, len;
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		إذا (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			حرر(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + len;
	}

	حرر(data);

	إذا (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	إذا (recovery_eof <= recovery_head) {
		إذا (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
	}

	/* remove the recovery magic */
	إذا (tdb_ofs_write(tdb, recovery_head + offsetof(بنية list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	إذا (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	إذا (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	أرجع 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ساكن صحيح tdb_rec_free_read(بنية tdb_context *tdb, tdb_off_t off, بنية list_struct *rec)
{
	إذا (tdb->methods->tdb_read(tdb, off, rec, حجم(*rec),DOCONV()) == -1)
		أرجع -1;

	إذا (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		إذا (tdb->methods->tdb_write(tdb, off, rec, حجم(*rec)) == -1)
			أرجع -1;
	}

	إذا (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	إذا (tdb->methods->tdb_oob(tdb, rec->next+حجم(*rec), 0) != 0)
		أرجع -1;
	أرجع 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ساكن صحيح remove_from_freelist(بنية tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_لووتکە;
	طالما (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		إذا (i == off) {
			/* We've found it! */
			أرجع tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ساكن صحيح update_tailer(بنية tdb_context *tdb, tdb_off_t offset,
			 ثابت بنية list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = حجم(*rec) + rec->rec_len;
	أرجع tdb_ofs_write(tdb, offset + totalsize - حجم(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t offset, بنية list_struct *rec)
{
	tdb_off_t ڕاست, چەپ;

	/* Allocation and tailer lock */
	إذا (tdb_lock(tdb, -1, F_WRLCK) != 0)
		أرجع -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	إذا (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		اذهب fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ڕاست = offset + حجم(*rec) + rec->rec_len;
	إذا (ڕاست + حجم(*rec) <= tdb->map_size) {
		بنية list_struct r;

		إذا (tdb->methods->tdb_read(tdb, ڕاست, &r, حجم(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ڕاست));
			اذهب چەپ;
		}

		/* If it's free, expand to include it. */
		إذا (r.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, ڕاست, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ڕاست));
				اذهب چەپ;
			}
			rec->rec_len += حجم(r) + r.rec_len;
		}
	}

چەپ:
	/* Look left */
	چەپ = offset - حجم(tdb_off_t);
	إذا (چەپ > TDB_DATA_START(tdb->header.hash_size)) {
		بنية list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		إذا (tdb_ofs_read(tdb, چەپ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", چەپ));
			اذهب update;
		}

		/* it could be uninitialised data */
		إذا (leftsize == 0 || leftsize == TDB_PAD_U32) {
			اذهب update;
		}

		چەپ = offset - leftsize;

		/* Now read in record */
		إذا (tdb->methods->tdb_read(tdb, چەپ, &l, حجم(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", چەپ, leftsize));
			اذهب update;
		}

		/* If it's free, expand to include it. */
		إذا (l.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, چەپ, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", چەپ));
				اذهب update;
			} وإلا {
				offset = چەپ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	إذا (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		اذهب fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	إذا (tdb_ofs_read(tdb, FREELIST_لووتکە, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_لووتکە, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		اذهب fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ساكن tdb_off_t tdb_allocate_ofs(بنية tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				بنية list_struct *rec, tdb_off_t last_ptr)
{
	بنية list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', حجم(newrec));

	/* found it - now possibly split it up  */
	إذا (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (حجم(*rec) + length);
		newrec_ptr = rec_ptr + حجم(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} وإلا {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	إذا (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		أرجع 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	إذا (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		أرجع 0;
	}

	/* Did we create new block? */
	إذا (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		إذا (update_tailer(tdb, rec_ptr, rec) == -1) {
			أرجع 0;
		}

		/* Free new record */
		إذا (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			أرجع 0;
		}
	}

	/* all done - return the new record offset */
	أرجع rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t length, بنية list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	بنية {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	إذا (tdb_lock(tdb, -1, F_WRLCK) == -1)
		أرجع 0;

	/* Extra bytes required for tailer */
	length += حجم(tdb_off_t);

 again:
	last_ptr = FREELIST_لووتکە;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_لووتکە, &rec_ptr) == -1)
		اذهب fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	طالما (rec_ptr) {
		إذا (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			اذهب fail;
		}

		إذا (rec->rec_len >= length) {
			إذا (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				إذا (bestfit.rec_len < 2*length) {
					اكسر;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	إذا (bestfit.rec_ptr != 0) {
		إذا (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			اذهب fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	إذا (tdb_expand(tdb, length + حجم(*rec)) == 0)
		اذهب again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ساكن صحيح seen_زیادکردن(بنية tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA کلیل, data;

	memset(&data, '\0', حجم(data));
	کلیل.dptr = (unsigned حرف *)&rec_ptr;
	کلیل.dsize = حجم(rec_ptr);
	أرجع tdb_store(mem_tdb, کلیل, data, TDB_زیادکردن);
}

صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries)
{
	بنية tdb_context *mem_tdb = عدم;
	بنية list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	صحيح ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	إذا (!mem_tdb) {
		أرجع -1;
	}

	إذا (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		أرجع 0;
	}

	last_ptr = FREELIST_لووتکە;

	/* Store the FREELIST_TOP record. */
	إذا (seen_زیادکردن(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		اذهب fail;
	}

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_لووتکە, &rec_ptr) == -1) {
		اذهب fail;
	}

	طالما (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		إذا (seen_زیادکردن(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			اذهب fail;
		}

		إذا (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
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
	أرجع ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ساكن صحيح tdb_next_lock(بنية tdb_context *tdb, بنية tdb_traverse_lock *tlock,
			 بنية list_struct *rec)
{
	صحيح want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	لكل (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		إذا (!tlock->off && tlock->hash != 0) {
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
			إذا (tlock->hash == tdb->header.hash_size) {
				استمر;
			}
		}

		إذا (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			أرجع -1;

		/* No previous record?  Start at top of chain. */
		إذا (!tlock->off) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(tlock->hash),
				     &tlock->off) == -1)
				اذهب fail;
		} وإلا {
			/* Otherwise unlock the previous record. */
			إذا (tdb_unlock_record(tdb, tlock->off) != 0)
				اذهب fail;
		}

		إذا (want_next) {
			/* We have offset of old record: grab next */
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		طالما( tlock->off) {
			tdb_off_t current;
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			إذا (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				اذهب fail;
			}

			إذا (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				إذا (tdb_lock_record(tdb, tlock->off) != 0)
					اذهب fail;
				أرجع tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			إذا (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				اذهب fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	أرجع TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	إذا (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	أرجع -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ساكن صحيح tdb_traverse_internal(بنية tdb_context *tdb,
				 tdb_traverse_func fn, فراغ *private_data,
				 بنية tdb_traverse_lock *tl)
{
	TDB_DATA کلیل, dbuf;
	بنية list_struct rec;
	صحيح ret, ژماردن = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	طالما ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		ژماردن++;
		/* now read the full record */
		کلیل.dptr = tdb_alloc_read(tdb, tl->off + حجم(rec),
					  rec.کلیل_len + rec.data_len);
		إذا (!کلیل.dptr) {
			ret = -1;
			إذا (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				اذهب out;
			إذا (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			اذهب out;
		}
		کلیل.dsize = rec.کلیل_len;
		dbuf.dptr = کلیل.dptr + rec.کلیل_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		إذا (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(کلیل.dptr);
			اذهب out;
		}
		إذا (fn && fn(tdb, کلیل, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = ژماردن;
			إذا (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(کلیل.dptr);
			اذهب out;
		}
		SAFE_FREE(کلیل.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	إذا (ret < 0)
		أرجع -1;
	وإلا
		أرجع ژماردن;
}


/*
  a write style traverse - temporarily marks the db read only
*/
صحيح tdb_traverse_read(بنية tdb_context *tdb,
		      tdb_traverse_func fn, فراغ *private_data)
{
	بنية tdb_traverse_lock tl = { عدم, 0, 0, F_RDLCK };
	صحيح ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	إذا (tdb_transaction_lock(tdb, F_RDLCK)) {
		أرجع -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	أرجع ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
صحيح tdb_traverse(بنية tdb_context *tdb,
		 tdb_traverse_func fn, فراغ *private_data)
{
	بنية tdb_traverse_lock tl = { عدم, 0, 0, F_WRLCK };
	صحيح ret;

	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع tdb_traverse_read(tdb, fn, private_data);
	}

	إذا (tdb_transaction_lock(tdb, F_WRLCK)) {
		أرجع -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	أرجع ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(بنية tdb_context *tdb)
{
	TDB_DATA کلیل;
	بنية list_struct rec;

	/* release any old lock */
	إذا (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		أرجع tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	إذا (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		أرجع tdb_null;
	/* now read the key */
	کلیل.dsize = rec.کلیل_len;
	کلیل.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),کلیل.dsize);

	/* Unlock the hash chain of the record we just read. */
	إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	أرجع کلیل;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(بنية tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA کلیل = tdb_null;
	بنية list_struct rec;
	unsigned حرف *k = عدم;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	إذا (tdb->travlocks.off) {
		إذا (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			أرجع tdb_null;
		إذا (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),
					    rec.کلیل_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			إذا (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				أرجع tdb_null;
			}
			إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				أرجع tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	إذا (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		إذا (!tdb->travlocks.off)
			أرجع tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		إذا (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			أرجع tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	إذا (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		کلیل.dsize = rec.کلیل_len;
		کلیل.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+حجم(rec),
					  کلیل.dsize);
		/* Unlock the chain of this new record */
		إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	إذا (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	أرجع کلیل;
}

/* file: dump.c */

ساكن tdb_off_t tdb_dump_record(بنية tdb_context *tdb, صحيح hash,
				 tdb_off_t offset)
{
	بنية list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	إذا (tdb->methods->tdb_read(tdb, offset, (حرف *)&rec,
				   حجم(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		أرجع 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.کلیل_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + حجم(rec) + rec.rec_len - حجم(tdb_off_t);

	إذا (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		أرجع rec.next;
	}

	إذا (tailer != rec.rec_len + حجم(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned صحيح)tailer, (unsigned صحيح)(rec.rec_len + حجم(rec)));
	}
	أرجع rec.next;
}

ساكن صحيح tdb_dump_chain(بنية tdb_context *tdb, صحيح i)
{
	tdb_off_t rec_ptr, لووتکە;

	لووتکە = TDB_HASH_لووتکە(i);

	إذا (tdb_lock(tdb, i, F_WRLCK) != 0)
		أرجع -1;

	إذا (tdb_ofs_read(tdb, لووتکە, &rec_ptr) == -1)
		أرجع tdb_unlock(tdb, i, F_WRLCK);

	إذا (rec_ptr)
		printf("hash=%d\n", i);

	طالما (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	أرجع tdb_unlock(tdb, i, F_WRLCK);
}

فراغ tdb_dump_all(بنية tdb_context *tdb)
{
	صحيح i;
	لكل (i = 0; i < (صحيح)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

صحيح tdb_printfreelist(بنية tdb_context *tdb)
{
	صحيح ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	بنية list_struct rec;

	إذا ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		أرجع ret;

	offset = FREELIST_لووتکە;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	طالما (rec_ptr) {
		إذا (tdb->methods->tdb_read(tdb, rec_ptr, (حرف *)&rec,
					   حجم(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		إذا (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (صحيح)total_free,
               (صحيح)total_free);

	أرجع tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
فراغ tdb_increment_seqnum_nonblock(بنية tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	إذا (!(tdb->flags & TDB_SEQNUM)) {
		أرجع;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	إذا (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		أرجع;
	seqnum++;
	(فراغ) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ساكن فراغ tdb_increment_seqnum(بنية tdb_context *tdb)
{
	إذا (!(tdb->flags & TDB_SEQNUM)) {
		أرجع;
	}

	إذا (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		أرجع;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ساكن صحيح tdb_کلیل_compare(TDB_DATA کلیل, TDB_DATA data,
			   فراغ *private_data EXT2FS_ATTR((unused)))
{
	أرجع memcmp(data.dptr, کلیل.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ساكن tdb_off_t tdb_find(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash,
			بنية list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (!TDB_DEAD(r) && hash==r->full_hash
		    && کلیل.dsize==r->کلیل_len
		    && tdb_parse_data(tdb, کلیل, rec_ptr + حجم(*r),
				      r->کلیل_len, tdb_کلیل_compare,
				      عدم) == 0) {
			أرجع rec_ptr;
		}
		rec_ptr = r->next;
	}
	أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash, صحيح locktype,
			   بنية list_struct *rec)
{
	u32 rec_ptr;

	إذا (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		أرجع 0;
	إذا (!(rec_ptr = tdb_find(tdb, کلیل, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	أرجع rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ساكن صحيح tdb_update_hash(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash, TDB_DATA dbuf)
{
	بنية list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	إذا (!(rec_ptr = tdb_find(tdb, کلیل, hash, &rec)))
		أرجع -1;

	/* must be long enough key, data and tailer */
	إذا (rec.rec_len < کلیل.dsize + dbuf.dsize + حجم(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		أرجع -1;
	}

	إذا (tdb->methods->tdb_write(tdb, rec_ptr + حجم(rec) + rec.کلیل_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		أرجع -1;

	إذا (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		أرجع tdb_rec_write(tdb, rec_ptr, &rec);
	}

	أرجع 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلیل);
	إذا (!(rec_ptr = tdb_find_lock_hash(tdb,کلیل,hash,F_RDLCK,&rec)))
		أرجع tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + حجم(rec) + rec.کلیل_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	أرجع ret;
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

صحيح tdb_parse_record(بنية tdb_context *tdb, TDB_DATA کلیل,
		     صحيح (*parser)(TDB_DATA کلیل, TDB_DATA data,
				   فراغ *private_data),
		     فراغ *private_data)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلیل);

	إذا (!(rec_ptr = tdb_find_lock_hash(tdb,کلیل,hash,F_RDLCK,&rec))) {
		أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, کلیل, rec_ptr + حجم(rec) + rec.کلیل_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	أرجع ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ساكن صحيح tdb_exists_hash(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash)
{
	بنية list_struct rec;

	إذا (tdb_find_lock_hash(tdb, کلیل, hash, F_RDLCK, &rec) == 0)
		أرجع 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	أرجع 1;
}

صحيح tdb_exists(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	u32 hash = tdb->hash_fn(&کلیل);
	أرجع tdb_exists_hash(tdb, کلیل, hash);
}

/* actually delete an entry in the database given the offset */
صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct*rec)
{
	tdb_off_t last_ptr, i;
	بنية list_struct lastrec;

	إذا (tdb->read_only || tdb->traverse_read) أرجع -1;

	إذا (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		أرجع tdb_rec_write(tdb, rec_ptr, rec);
	}
	إذا (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		أرجع -1;

	/* find previous record in hash chain */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(rec->full_hash), &i) == -1)
		أرجع -1;
	لكل (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		إذا (tdb_rec_read(tdb, i, &lastrec) == -1)
			أرجع -1;

	/* unlink it: next ptr is at start of record. */
	إذا (last_ptr == 0)
		last_ptr = TDB_HASH_لووتکە(rec->full_hash);
	إذا (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		أرجع -1;

	/* recover the space */
	إذا (tdb_free(tdb, rec_ptr, rec) == -1)
		أرجع -1;
	أرجع 0;
}

ساكن صحيح tdb_ژماردن_dead(بنية tdb_context *tdb, u32 hash)
{
	صحيح res = 0;
	tdb_off_t rec_ptr;
	بنية list_struct rec;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(hash), &rec_ptr) == -1)
		أرجع 0;

	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			أرجع 0;

		إذا (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	أرجع res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ساكن صحيح tdb_purge_dead(بنية tdb_context *tdb, u32 hash)
{
	صحيح res = -1;
	بنية list_struct rec;
	tdb_off_t rec_ptr;

	إذا (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		أرجع -1;
	}

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(hash), &rec_ptr) == -1)
		اذهب fail;

	طالما (rec_ptr) {
		tdb_off_t next;

		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}

		next = rec.next;

		إذا (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع res;
}

/* delete an entry in the database given a key */
ساكن صحيح tdb_delete_hash(بنية tdb_context *tdb, TDB_DATA کلیل, u32 hash)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;

	إذا (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		إذا (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			أرجع -1;

		إذا (tdb_ژماردن_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		إذا (!(rec_ptr = tdb_find(tdb, کلیل, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			أرجع -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	وإلا {
		إذا (!(rec_ptr = tdb_find_lock_hash(tdb, کلیل, hash, F_WRLCK,
						   &rec)))
			أرجع -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	إذا (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	أرجع ret;
}

صحيح tdb_delete(بنية tdb_context *tdb, TDB_DATA کلیل)
{
	u32 hash = tdb->hash_fn(&کلیل);
	أرجع tdb_delete_hash(tdb, کلیل, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ساكن tdb_off_t tdb_find_dead(بنية tdb_context *tdb, u32 hash,
			       بنية list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			أرجع rec_ptr;
		}
		rec_ptr = r->next;
	}
	أرجع 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
صحيح tdb_store(بنية tdb_context *tdb, TDB_DATA کلیل, TDB_DATA dbuf, صحيح flag)
{
	بنية list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	حرف *p = عدم;
	صحيح ret = -1;

	إذا (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلیل);
	إذا (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		أرجع -1;

	/* check for it existing, on insert. */
	إذا (flag == TDB_زیادکردن) {
		إذا (tdb_exists_hash(tdb, کلیل, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			اذهب fail;
		}
	} وإلا {
		/* first try in-place update, on modify or replace. */
		إذا (tdb_update_hash(tdb, کلیل, hash, dbuf) == 0) {
			اذهب done;
		}
		إذا (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			اذهب fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	إذا (flag != TDB_زیادکردن)
		tdb_delete_hash(tdb, کلیل, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	إذا (!(p = (حرف *)malloc(کلیل.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}

	memcpy(p, کلیل.dptr, کلیل.dsize);
	إذا (dbuf.dsize)
		memcpy(p+کلیل.dsize, dbuf.dptr, dbuf.dsize);

	إذا (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			کلیل.dsize + dbuf.dsize + حجم(tdb_off_t));

		إذا (rec_ptr != 0) {
			rec.کلیل_len = کلیل.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + حجم(rec),
				    p, کلیل.dsize + dbuf.dsize) == -1) {
				اذهب fail;
			}
			اذهب done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	إذا (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		اذهب fail;
	}

	إذا ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		اذهب fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, کلیل.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	إذا (rec_ptr == 0) {
		اذهب fail;
	}

	/* Read hash top into next ptr */
	إذا (tdb_ofs_read(tdb, TDB_HASH_لووتکە(hash), &rec.next) == -1)
		اذهب fail;

	rec.کلیل_len = کلیل.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+حجم(rec), p, کلیل.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_لووتکە(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		اذهب fail;
	}

 done:
	ret = 0;
 fail:
	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	أرجع ret;
}


/* Append to an entry. Create if not exist. */
صحيح tdb_append(بنية tdb_context *tdb, TDB_DATA کلیل, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	صحيح ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلیل);
	إذا (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		أرجع -1;

	dbuf = tdb_fetch(tdb, کلیل);

	إذا (dbuf.dptr == عدم) {
		dbuf.dptr = (unsigned حرف *)malloc(new_dbuf.dsize);
	} وإلا {
		unsigned حرف *new_dptr = (unsigned حرف *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		إذا (new_dptr == عدم) {
			حرر(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	إذا (dbuf.dptr == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, کلیل, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	أرجع ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ثابت حرف *tdb_name(بنية tdb_context *tdb)
{
	أرجع tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
صحيح tdb_fd(بنية tdb_context *tdb)
{
	أرجع tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(بنية tdb_context *tdb)
{
	أرجع tdb->log.log_fn;
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
صحيح tdb_get_seqnum(بنية tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	إذا (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		أرجع 0;
	أرجع seqnum;
}

صحيح tdb_hash_size(بنية tdb_context *tdb)
{
	أرجع tdb->header.hash_size;
}

size_t tdb_map_size(بنية tdb_context *tdb)
{
	أرجع tdb->map_size;
}

صحيح tdb_get_flags(بنية tdb_context *tdb)
{
	أرجع tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
فراغ tdb_enable_seqnum(بنية tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ساكن بنية tdb_context *tdbs = عدم;


/* This is from a hash algorithm suggested by Rogier Wolff */
ساكن unsigned صحيح default_tdb_hash(TDB_DATA *کلیل)
{
	u32 بەها;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	لكل (بەها = 0, i=0; i < کلیل->dsize; i++)
		بەها = بەها * 256 + کلیل->dptr[i] + (بەها >> 24) * 241;

	أرجع بەها;
}


/* initialise a new database with a specified hash size */
ساكن صحيح tdb_new_database(بنية tdb_context *tdb, صحيح hash_size)
{
	بنية tdb_header *newdb;
	صحيح size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = حجم(بنية tdb_header) + (hash_size+1)*حجم(tdb_off_t);
	إذا (!(newdb = (بنية tdb_header *)calloc(size, 1)))
		أرجع TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	إذا (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (حرف *)newdb;
		memcpy(&tdb->header, newdb, حجم(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		أرجع 0;
	}
	إذا (lseek(tdb->fd, 0, SEEK_SET) == -1)
		اذهب fail;

	إذا (ftruncate(tdb->fd, 0) == -1)
		اذهب fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, حجم(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	إذا (اكتب(tdb->fd, newdb, size) != size) {
		ret = -1;
	} وإلا {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	أرجع ret;
}



ساكن صحيح tdb_already_open(dev_t device,
			    ino_t ino)
{
	بنية tdb_context *i;

	لكل (i = tdbs; i; i = i->next) {
		إذا (i->device == device && i->inode == ino) {
			أرجع 1;
		}
	}

	أرجع 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_size, صحيح tdb_flags,
		      صحيح open_flags, mode_t mode)
{
	أرجع tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, عدم, عدم);
}

/* a default logging function */
ساكن فراغ null_log_fn(بنية tdb_context *tdb, تعداد tdb_debug_level level, ثابت حرف *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ساكن فراغ null_log_fn(بنية tdb_context *tdb EXT2FS_ATTR((unused)),
			تعداد tdb_debug_level level EXT2FS_ATTR((unused)),
			ثابت حرف *fmt EXT2FS_ATTR((unused)), ...)
{
}


بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_size, صحيح tdb_flags,
				صحيح open_flags, mode_t mode,
				ثابت بنية tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	بنية tdb_context *tdb;
	بنية stat st;
	صحيح rev = 0, locked = 0;
	unsigned حرف *vp;
	u32 vertest;

	إذا (!(tdb = (بنية tdb_context *)calloc(1, حجم *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		اذهب fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = عدم;
	tdb->map_ptr = عدم;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	إذا (log_ctx) {
		tdb->log = *log_ctx;
	} وإلا {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = عدم;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	إذا (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	إذا ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		اذهب fail;
	}

	إذا (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	إذا ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	إذا (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		إذا (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			اذهب fail;
		}
		اذهب internal;
	}

	إذا ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	إذا ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		إذا (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			اذهب fail; /* errno set by ftruncate */
		}
	}

	إذا (اقرأ(tdb->fd, &tdb->header, حجم(tdb->header)) != حجم(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      حجم(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		إذا (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			اذهب fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned حرف *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	إذا (!rev)
		tdb->flags &= ~TDB_CONVERT;
	وإلا {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, حجم(tdb->header));
	}
	إذا (fstat(tdb->fd, &st) == -1)
		اذهب fail;

	إذا (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		اذهب fail;
	}

	/* Is it already in the open list?  If so, fail. */
	إذا (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (صحيح)st.st_dev, (صحيح)st.st_ino));
		errno = EBUSY;
		اذهب fail;
	}

	إذا (!(tdb->name = (حرف *)strdup(name))) {
		errno = ENOMEM;
		اذهب fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	إذا (locked) {
		إذا (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			اذهب fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	إذا (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		إذا (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			اذهب fail;
	}

	/* if needed, run recovery */
	إذا (tdb_transaction_recover(tdb) == -1) {
		اذهب fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		اذهب fail;
	tdb->next = tdbs;
	tdbs = tdb;
	أرجع tdb;

 fail:
	{ صحيح save_errno = errno;

	إذا (!tdb)
		أرجع عدم;

	إذا (tdb->map_ptr) {
		إذا (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	إذا (tdb->fd != -1)
		إذا (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	أرجع عدم;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

فراغ tdb_set_max_dead(بنية tdb_context *tdb, صحيح max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
صحيح tdb_close(بنية tdb_context *tdb)
{
	بنية tdb_context **i;
	صحيح ret = 0;

	إذا (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	إذا (tdb->map_ptr) {
		إذا (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	إذا (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	لكل (i = &tdbs; *i; i = &(*i)->next) {
		إذا (*i == tdb) {
			*i = tdb->next;
			اكسر;
		}
	}

	memset(tdb, 0, حجم(*tdb));
	SAFE_FREE(tdb);

	أرجع ret;
}

/* register a logging function */
فراغ tdb_set_logging_function(بنية tdb_context *tdb,
                              ثابت بنية tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

فراغ *tdb_get_logging_private(بنية tdb_context *tdb)
{
	أرجع tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
صحيح tdb_reopen(بنية tdb_context *tdb)
{
	بنية stat st;

	إذا (tdb->flags & TDB_INTERNAL) {
		أرجع 0; /* Nothing to do. */
	}

	إذا (tdb->ژمارە_locks != 0 || tdb->global_lock.ژماردن) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		اذهب fail;
	}

	إذا (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		اذهب fail;
	}

	إذا (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		اذهب fail;
	}
	إذا (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	إذا (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		اذهب fail;
	}
	إذا ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		اذهب fail;
	}
	إذا (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		اذهب fail;
	}
	إذا (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		اذهب fail;
	}
	tdb_mmap(tdb);

	أرجع 0;

fail:
	tdb_close(tdb);
	أرجع -1;
}

/* reopen all tdb's */
صحيح tdb_reopen_all(صحيح parent_longlived)
{
	بنية tdb_context *tdb;

	لكل (tdb=tdbs; tdb; tdb = tdb->next) {
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
		إذا (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		إذا (tdb_reopen(tdb) != 0)
			أرجع -1;
	}

	أرجع 0;
}

/**
 * Flush a database file from the page cache.
 **/
صحيح tdb_flush(بنية tdb_context *tdb)
{
	إذا (tdb->fd != -1)
		أرجع fsync(tdb->fd);
	أرجع 0;
}
